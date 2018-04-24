#
# Builds the LinuxBoot firmware image.
#
# This requires the vendor firmware image, a Linux kernel and an initrd.cpio.xz file.
#
#
all: linuxboot

-include .config
include Makefile.rules
include Makefile.uefi

# The config file should set the BOARD variable
# as well as point to the bzImage and initrd.cpio files
BOARD		?= qemu
KERNEL		?= bzImage
INITRD		?= initrd.cpio.xz
BUILD		:= build/$(BOARD)

# Make sure that we have a board file for the user
$(shell \
	if [ ! -r boards/$(BOARD)/Makefile.board ]; then \
		echo >&2 "BOARD=$(BOARD) is not valid."; \
		echo >&2 ; \
		echo >&2 "Supported mainboards:"; \
		ls boards/*/Makefile.board | cut -d/ -f2 >&2 ; \
		echo >&2 ; \
		exit 1 ; \
	fi; \
	mkdir -p $(BUILD) ; \
)

# Bring in the board specific things
include boards/$(BOARD)/Makefile.board

# If they don't define a vendor ROM file
ROM ?= boards/$(BOARD)/$(BOARD).rom

ifdef USE_UTK
include Makefile.utk
endif

linuxboot: $(BUILD)/linuxboot.rom

# Create a .config file based on the current parameters
config:
	echo '# Generated $(DATE)' > .config
	echo 'BOARD ?= $(BOARD)' >> .config
	echo 'KERNEL ?= $(KERNEL)' >> .config
	echo 'INITRD ?= $(INITRD)' >> .config
	echo 'ROM ?= $(ROM)' >> .config
	echo 'CUSTOM ?= $(CUSTOM)' >> .config


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

$(BUILD)/Linux.ffs: $(KERNEL)
$(BUILD)/Initrd.ffs: $(INITRD)

ifdef CUSTOM
$(BUILD)/User.ffs: $(CUSTOM) FORCE
$(CUSTOM):
$(BUILD)/user.vol: $(BUILD)/User.ffs
else
$(BUILD)/user.vol: FORCE
endif

$(BUILD)/%.ffs: $(BUILD)/%.vol
	./bin/create-ffs \
		-o $@ \
		--type FIRMWARE_VOLUME_IMAGE \
		$^

$(BUILD)/%.ffs: $(EDK2_OUTPUT_DIR)/%.efi
	$(create-ffs)
$(BUILD)/%.ffs:
	$(create-ffs)

ROM_SIZE ?= `stat -Lc"%s" $(ROM)`

$(BUILD)/%.rom:
	cat > $@.tmp $^
	@if [ `stat -c"%s" $@.tmp` -ne $(ROM_SIZE) ]; then \
		printf >&2 "%s: Wrong output size 0x%x != expected 0x%x\n" \
			$@ `stat -c'%s' $@.tmp` `stat -Lc'%s' $(ROM)` ; \
		exit 1; \
	fi
	mv $@.tmp $@

$(BUILD)/%.vol:
	./bin/create-fv \
		-v \
		-o $@ \
		--size $(or $($(basename $(notdir $@))-size),0x400000) \
		--compress $(or $($(basename $(notdir $@))-compress),0) \
		$(filter-out $(BUILD)/$(BOARD).txt,$^)

create-ffs = \
	./bin/create-ffs \
		-o $@ \
		--name $(basename $(notdir $@)) \
		--version 1.0 \
		--type $(or $($(basename $(notdir $@))-type),DRIVER) \
		--depex "$($(basename $(notdir $@))-depex)" \
		--guid "$($(basename $(notdir $@))-guid)" \
		$^

#
# Extract all of the firmware files from the vendor provided ROM
#		--repack \
#
$(BUILD)/$(BOARD).txt: $(ROM)
	( \
	cd $(BUILD) ; \
	$(pwd)/bin/extract-firmware \
		-o rom \
	) < $^ \
	> $@.tmp
	mv $@.tmp $@

# All of the output volumes depend on extracting the firmware
$(patsubst %.vol,,$(FVS)): $(BUILD)/$(BOARD).txt
$(patsubst %.fv,,$(FVS)): $(BUILD)/$(BOARD).txt

# Most of the boards define a $(dxe-files) that are extracted
# from the vendor ROM. Make sure they depend on the board target
$(dxe-files): $(BUILD)/$(BOARD).txt
	@true

# Any of the DXE modules are produced by running make in the dxe subdir
dxe/%.ffs:
	$(MAKE) -C dxe $(notdir $@)

ifndef USE_UTK
$(BUILD)/linuxboot.rom: $(FVS)
else
$(BUILD)/linuxboot.rom: bin/utk $(DXE_FFS)
	$< \
		$(ROM) \
		remove_dxes_except boards/$(BOARD)/image-files.txt \
		$(foreach ffs,$(DXE_FFS), insert_dxe $(ffs)) \
		$(UTK_EXTRA_OPS) \
		save $@
endif

clean:
	$(RM) $(BUILD)/{*.ffs,*.rom,*.vol,*.tmp}
	$(RM) ./bin/utk
