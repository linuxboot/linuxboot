KERNEL		= $(shell uname -s)
CC		= $(CROSS)gcc

TARGETS += fvloader.ffs
TARGETS += hello.ffs

all: $(TARGETS)

clean: FORCE
	$(RM) *.efi *.exe *.rom *.o .*.d $(TARGETS)


FORCE:

%.efi: %.exe
	CROSS=$(CROSS) \
	./efi-wrap $< > $@

%.ffs: %.efi
	../bin/create-ffs \
		-o $@ \
		-t DRIVER \
		-n "$(basename $@)" \
		$<

BITS=64
EFI_ARCH=x86_64
#BITS=32
#EFI_ARCH=x86

%.exe: %.o
	$(CROSS)ld \
	    -T pei-x86-$(BITS).lds \
	    -o $@ \
	    $^

CFLAGS += \
	-std=c99 \
	-D__efi__ \
	-nostdinc \
	-fshort-wchar \
	-mno-red-zone \
	-fno-stack-protector \
	-m$(BITS) \
	-fpic \
	-O3 \
	-W \
	-Wall \
	-I . \
	-MMD \
	-MF .$(notdir $@).d \

-include .*.d
