KERNEL		= $(shell uname -s)
CC		= $(CROSS)gcc
BITS		= 64
EFI_ARCH	= x86_64
#BITS		= 32
#EFI_ARCH	= x86


#TARGETS += fvloader.ffs
TARGETS += linuxboot.ffs
#TARGETS += hello.ffs

all: $(TARGETS)

clean: FORCE
	$(RM) *.efi *.exe *.rom *.o .*.d $(TARGETS)

linuxboot.exe: linuxboot.o efifv.o
hello.exe: hello.o

FORCE:

%.exe:
	$(CROSS)ld \
		$(LDFLAGS) \
		-T elf_x86_64_efi.lds \
		-o $@ \
		$^

%.efi: %.exe
	objcopy \
		-j .text -j .sdata -j .data -j .dynamic \
		-j .dynsym  -j .rel -j .rela -j .reloc \
		--subsystem efi-bsd \
		--target efi-app-$(EFI_ARCH) \
		$^ \
		$@
	# fixup the PE32 characteristic field to make DxeCore happy
	printf '\x2E\x00' | dd of=$@ conv=notrunc bs=1 seek=150 status=none

%.ffs: %.efi
	../bin/create-ffs \
		-o $@ \
		--type DRIVER \
		--version 1.0 \
		--name "$(basename $@)" \
		--depex TRUE \
		$<

CFLAGS += \
	-std=c99 \
	-D__efi__ \
	-DGNU_EFI_USE_MS_ABI \
	-fshort-wchar \
	-mno-red-zone \
	-fno-stack-protector \
	-m$(BITS) \
	-fpic \
	-O3 \
	-W \
	-Wall \
	-I . \
	-I efi/x86_64 \
	-MMD \
	-MF .$(notdir $@).d \

LDFLAGS += \
	-nostdlib \
	-znocombreloc \
	-shared \
	-Bsymbolic \

NO_LFLAGS += \
	-T $(EFI_LDS) \
	-L $(EFILIB) \
	-L $(LIB) \
	$(EFI_CRT_OBJS)  \


-include .*.d
