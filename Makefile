#
# Builds the LinuxBoot firmware image.
#
# This requires the vendor firmware image, a Linux kernel and an initrd.cpio.xz file.
#
# 
all: vendor linuxboot

-include .config
include Makefile.rules

# The config file should set the BOARD variable
# as well as point to the bzImage and initrd.cpio files
BOARD		?= qemu
KERNEL		?= bzImage
INITRD		?= initrd.cpio.xz

# Bring in the board specific things
include boards/$(BOARD)/Makefile.board

# Create a .config file based on the current parameters
config:
	echo '# Generated $(DATE)' > .config
	echo 'BOARD ?= $(BOARD)' >> .config
	echo 'KERNEL ?= $(KERNEL)' >> .config
	echo 'INITRD ?= $(INITRD)' >> .config


# edk2 outputs will be in this deeply nested directory
EDK2_OUTPUT_DIR := edk2/Build/MdeModule/DEBUG_GCC5/X64

vendor:

$(EDK2_OUTPUT_DIR)/DxeCore.efi: edk2/.git
	$(MAKE) -C edk2


edk2.force: edk2/.git
	$(MAKE) -C edk2 build

# If the edk2 directory doesn't exist, checkout a shallow branch of it
# and build the various Dxe/Smm files
edk2/.git:
	git clone --depth 1 --branch UDK2018 https://github.com/linuxboot/edk2


vendor: vendor-$(BOARD).vol
linuxboot: linuxboot-$(BOARD).vol

vendor-$(BOARD).vol: \
	DxeCore.ffs \
	PiSmmCore.ffs \
	$(vendor-files) \


linuxboot-$(BOARD).vol: \
	Linux.ffs \
	Initrd.ffs \


Linux.ffs: $(KERNEL)
Initrd.ffs: $(INITRD)

RuntimeArchProtocolGuid	:= b7dfb4e1-052f-449f-87be-9818fc91b733
AcpiTableProtocolGuid	:= FFE06BDD-6107-46A6-7BB2-5A9C7EC5275C
SmbiosProtocolGuid	:= 03583ff6-cb36-4940-947e-b9b39f4afaf7
VariableArchProtocolGuid	:= 1E5668E2-8481-11D4-BCF1-0080C73C8881

Linux-guid := DECAFBAD-6548-6461-732d-2f2d4e455246
Linux-depex := \
	$(RuntimeArchProtocolGuid) \
	$(AcpiTableProtocolGuid) \
	$(SmbiosProtocolGuid) \
	$(VariableArchProtocolGuid) \

Initrd-guid := 74696e69-6472-632e-7069-6f2f62696f73
Initrd-type := FREEFORM

DxeCore-type := DXE_CORE
PiSmmCore-type := SMM_CORE


Linux.ffs: bzImage

%.ffs: $(EDK2_OUTPUT_DIR)/%.efi
	$(create-ffs)
%.ffs:
	$(create-ffs)


%.vol:
	./bin/create-fv \
		-o $@ \
		--size $(or $($(basename $@)-size),0x400000) \
		--compress $(or $($(basename $@)-compress),0) \
		$^

create-ffs = \
	./bin/create-ffs \
		-o $@ \
		--name $(basename $@) \
		--version 1.0 \
		--type $(or $($(basename $@)-type),DRIVER) \
		--depex "$($(basename $@)-depex)" \
		--guid "$($(basename $@)-guid)" \
		$^
