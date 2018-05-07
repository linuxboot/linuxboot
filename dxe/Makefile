KERNEL		= $(shell uname -s)
CC		= $(CROSS)gcc
BITS		= 64
EFI_ARCH	= x86_64
#BITS		= 32
#EFI_ARCH	= x86


TARGETS += fvloader.ffs
TARGETS += linuxboot.ffs
TARGETS += hello.ffs

all: $(TARGETS)

clean: FORCE
	$(RM) *.efi *.exe *.rom *.o .*.d $(TARGETS)


FORCE:

%.exe: %.o
	$(CROSS)ld \
		$(LDFLAGS) \
		-T pei-x86-64.lds \
		-o $@ \
		$^

%.efi-wrap: %.exe
	./efi-wrap $^ > $@

%.efi: %.exe
	objcopy \
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
	-I /usr/include/efi/x86_64 \
	-MMD \
	-MF .$(notdir $@).d \

LDFLAGS += \
	-nostdlib \
	-znocombreloc \

NO_LFLAGS += \
	-T $(EFI_LDS) \
	-L $(EFILIB) \
	-L $(LIB) \
	$(EFI_CRT_OBJS)  \


-include .*.d
