#!/usr/bin/perl
# Parse GUIDs, generate EFI structs, etc

package EFI;
use warnings;
use strict;
use File::Temp 'tempfile';
use Digest::SHA 'sha1';


# Address Size  Designation
# ------- ----  -----------
# 
# EFI_FFS_FILE_HEADER:
# 0x0000  16    Name (EFI_GUID)
# 0x0010  1     IntegrityCheck.Header (Header Checksum)
# 0x0011  1     IntegrityCheck.File -> set to 0xAA (FFS_FIXED_CHECKSUM) and clear bit 0x40 of Attributes
# 0x0012  1     FileType -> 0x07 = EFI_FV_FILETYPE_DRIVER
# 0x0013  1     Attributes -> 0x00
# 0x0014  3     Size, including header and all other sections
# 0x0017  1     State (unused) -> 0X00
# 
# EFI_COMMON_SECTION_HEADER:
# 0x0000  3     Size, including this header
# 0x0003  1     Type -> 0x10 (EFI_SECTION_PE32)
# 0x0004  ####  <PE data>
# 
# EFI_COMMON_SECTION_HEADER:
# 0x0000  3     Size, including this header
# 0x0003  1     Type -> 0x15 (EFI_SECTION_USER_INTERFACE)
# 0x0004  ####  NUL terminated UTF-16 string (eg "FAT\0")
# 
# EFI_COMMON_SECTION_HEADER:
# 0x0000  3     Size, including this header
# 0x0003  1     Type -> 0x14 (EFI_SECTION_VERSION)
# 0x0004  ####  NUL terminated UTF-16 string (eg "1.0\0")

my $sec_hdr_len = 0x04;
my $ffs_hdr_len = 0x18;

my $fv_hdr_len = 0x48;
my $fv_block_size = 0x1000; # force alignment of files to this spacing



our %file_types = qw/
	RAW                   0x01
	FREEFORM              0x02
	SECURITY_CORE         0x03
	PEI_CORE              0x04
	DXE_CORE              0x05
	PEIM                  0x06
	DRIVER                0x07
	COMBINED_PEIM_DRIVER  0x08
	APPLICATION           0x09
	SMM                   0x0A
	FIRMWARE_VOLUME_IMAGE 0x0B
	COMBINED_SMM_DXE      0x0C
	SMM_CORE              0x0D
	DEBUG_MIN             0xe0
	DEBUG_MAX             0xef
	FFS_PAD               0xf0
/;

our %section_types = qw/
	GUID_DEFINED          0x02
	PE32                  0x10
	PIC                   0x11
	TE                    0x12
	DXE_DEPEX             0x13
	VERSION               0x14
	USER_INTERFACE        0x15
	COMPATIBILITY16       0x16
	FIRMWARE_VOLUME_IMAGE 0x17
	FREEFORM_SUBTYPE_GUID 0x18
	RAW                   0x19
	PEI_DEPEX             0x1B
	SMM_DEPEX             0x1C
/;

# Some special cases for non-PE32 sections
our %section_type_map = qw/
	FREEFORM		RAW
	FIRMWARE_VOLUME_IMAGE	FIRMWARE_VOLUME_IMAGE
/;

# Special cases for DEPEX sections
our %depex_type_map = qw/
	PEIM			PEI_DEPX
	DRIVER			DXE_DEPEX
	SMM			SMM_DEPEX
/;


# convert text GUID to hex
sub guid
{
	my $guid = shift;
	my ($g1,$g2,$g3,$g4,$g5) =
		$guid =~ /
			([0-9a-fA-F]{8})
			-([0-9a-fA-F]{4})
			-([0-9a-fA-F]{4})
			-([0-9a-fA-F]{4})
			-([0-9a-fA-F]{12})
		/x
		or die "$guid: Unable to parse guid\n";

	return pack("VvvnCCCCCC",
		hex $g1,
		hex $g2,
		hex $g3,
		hex $g4,
		hex substr($g5, 0, 2),
		hex substr($g5, 2, 2),
		hex substr($g5, 4, 2),
		hex substr($g5, 6, 2),
		hex substr($g5, 8, 2),
		hex substr($g5,10, 2),
	);
}


# Convert a string to UCS-16 and add a nul terminator
sub ucs16
{
	my $val = shift;

	my $rc = '';
	for(my $i = 0 ; $i < length $val ; $i++)
	{
		$rc .= substr($val, $i, 1) . chr(0x0);
	}

	# nul terminate the string
	$rc .= chr(0x0) . chr(0x0);

	return $rc;
}


# output an EFI Common Section Header
# Since we might be dealing with ones larger than 16 MB, we should use extended
# section type that gives us a 4-byte length.
sub section
{
	my $type = shift;
	my $data = shift;	

	die "$type: Unknown section type\n"
		unless exists $section_types{$type};

	my $len = length($data) + $sec_hdr_len;

	die "Section length $len > 16 MB, can't include it in a section!\n"
		if $len >= 0x1000000;

	my $sec = ''
		. chr(($len >>  0) & 0xFF)
		. chr(($len >>  8) & 0xFF)
		. chr(($len >> 16) & 0xFF)
		. chr(hex $section_types{$type})
		. $data;

	my $unaligned = length($sec) % 4;
	$sec .= chr(0x00) x (4 - $unaligned)
		if $unaligned != 0;

	return $sec;
}


sub ffs
{
	my $file_type = shift;
	my $data = shift;
	my $guid = shift || substr(sha1($data), 0, 16);

	my $len = length($data) + $ffs_hdr_len;

	my $type_byte = $file_types{$file_type}
		or die "$file_type: Unknown file type\n";

	my $attr = 0x28;
	my $state = 0x07;
	if ($file_type eq 'FFS_PAD')
	{
		$attr = 0x00;
		$state = 0xF8;
	}

	my $ffs = ''
		. $guid			# 0x00
		. chr(0x00)		# 0x10 header checksum
		. chr(0x00)		# 0x11 FFS_FIXED_CHECKSUM
		. chr(hex $type_byte)	# 0x12
		. chr($attr)		# 0x13 attributes
		. chr(($len >>  0) & 0xFF) # 0x14 length
		. chr(($len >>  8) & 0xFF)
		. chr(($len >> 16) & 0xFF)
		. chr($state)		# 0x17 state (not included in checksum)
		;

	# fixup the header checksum
	my $sum = 0;
	for my $i (0..length($ffs)-2) {
		$sum -= ord(substr($ffs, $i, 1));
	}

	substr($ffs, 0x10, 2) = chr($sum & 0xFF) . chr(0xAA);

	# Add the rest of the data
	return $ffs . $data;
}


# Generate a padding firmware firle
sub ffs_pad
{
	my $len = shift;
	return '' if $len <= $ffs_hdr_len;

	my $ffs = ffs(FFS_PAD =>
		chr(0xFF) x ($len - $ffs_hdr_len), # data
		chr(0xFF) x 16, # GUID
	);

	return $ffs;
}

# Generate a DEPEX section
sub depex
{
	my $type = shift;
	return unless @_;

	my $section_type = $depex_type_map{$type}
		or die "$type: DEPEX is not supported\n";

	if ($_[0] eq 'TRUE')
	{
		# Special case for short-circuit
		return section($section_type, chr(0x06) . chr(0x08));
	}

	my $data = '';
	my $count = 0;

	for my $guid (@_)
	{
		# push the guid
		$data .= chr(0x02) . guid($guid);
		$count++;
	}

	# AND them all together (1 minus the number of GUIDs)
	$data .= chr(0x03) for 1..$count-1;
	$data .= chr(0x08);
	
	return section($section_type, $data);
}


# compress a section and Wrap a GUIDed section around it
sub compress
{
	my $data = shift;

	my ($fh,$filename) = tempfile();
	print $fh $data;
	close $fh;

	# -7 produces the same bit-stream as the UEFI tools
	my $lz_data = `lzma --compress --stdout -7 $filename`;
	printf STDERR "%d compressed to %d\n", length($data), length($lz_data);

	# fixup the size field in the lzma compressed data
	substr($lz_data, 5, 8) = pack("VV", length($data), 0);

	# wrap the lzdata in a GUIDed section
	my $lz_header = ''
		. guid('EE4E5898-3914-4259-9D6E-DC7BD79403CF')
		. chr($ffs_hdr_len)  # data offset
		. chr(0x00)
		. chr(0x01)  # Processing required
		. chr(0x00)
		;

	# and replace our data with the GUID defined LZ compressed data
	return section(GUID_DEFINED => $lz_header . $lz_data);
}


# Create a FV header for a given file size
sub fv
{
	my $size = shift;
	my $guid = guid("8C8CE578-8A3D-4F1C-9935-896185C32DD3");

	my $fv_hdr = ''
		. (chr(0x00) x 0x10)		# 0x00 Zero vector
		. $guid				# 0x10
		. pack("Q", $size)		# 0x20
		. '_FVH'			# 0x28
		. pack("V", 0x000CFEFF)		# 0x2C attributes
		. pack("v", $fv_hdr_len)	# 0x30 header length
		. pack("v", 0x0000)		# 0x32 checksum
		. chr(0x00) x 3			# 0x34 reserved
		. chr(0x02)			# 0x37 version
		. pack("V", $size / $fv_block_size) # 0x38 number blocks
		. pack("V", $fv_block_size)	# 0x3C block size
		. (chr(0x00) x 0x08)		# 0x40 map (unused)
		;

	die "FV Header length ", length $fv_hdr, " != $fv_hdr_len\n"
		unless $fv_hdr_len == length $fv_hdr;

	# update the header checksum
	my $sum = 0;
	for(my $i = 0 ; $i < $fv_hdr_len ; $i += 2)
	{
		$sum -= unpack("v", substr($fv_hdr, $i, 2));
	}

	substr($fv_hdr, 0x32, 2) = pack("v", $sum & 0xFFFF);

	return $fv_hdr;
}

# Add files to an existing FV
sub fv_append
{
	my $fv_ref = shift;
	my $ffs = shift;

	# quick sanity check on the file
	my $length = length $ffs;

	my $ffs_length = unpack("V", substr($ffs, 0x14, 4)) & 0xFFFFFF;
	if ($ffs_length == 0xFFFFFF)
	{
		# ffs2 with extended length field
		$ffs_length = unpack("Q", substr($ffs, 0x18, 8));
	}

	# if the size of the file doesn't match the header size
	# we do not want to add it to our output.  signal an error
	return if $ffs_length != $length;

	# force at least 8 byte alignment for the section
	my $unaligned = $length % 8;
	$ffs .= chr(0xFF) x (8 - $unaligned)
		if $unaligned != 0;

	# if the current offset does not align with the block size,
	# we should add a pad section until the next block
	my $block_unaligned = $fv_block_size - (length($$fv_ref) % $fv_block_size);
	$block_unaligned += $fv_block_size if $block_unaligned < $ffs_hdr_len;

	$$fv_ref .= EFI::ffs_pad($block_unaligned - $ffs_hdr_len);
	my $ffs_offset = length($$fv_ref);

	# finally add the section
	$$fv_ref .= $ffs;

	return $ffs_offset;
}

# Finish the FV by padding it to its proper size
sub fv_pad
{
	my $fv_ref = shift;
	my $fv_size = unpack("Q", substr($$fv_ref, 0x20, 8));

	my $size = length($$fv_ref);

	# check for the overflow: if the header size is smaller than the actual size
	return if $fv_size < $size;

	# pad out so that actual size is the same as header size
	$$fv_ref .= chr(0xFF) x ($fv_size - $size);

	return 1;
}


"0, but true";
__END__
