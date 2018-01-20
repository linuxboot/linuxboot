#!/usr/bin/perl
# Parse GUIDs
use warnings;
use strict;

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

"0, but true";
__END__
