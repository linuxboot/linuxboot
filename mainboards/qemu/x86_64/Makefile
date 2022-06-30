OVMF_BIN ?= /usr/share/qemu/OVMF.fd

flashinitramfs.cpio: flashkernel core.cpio
	GO111MODULE=off u-root -o $@ \
		    -files flashkernel \
		    -files core.cpio \
		    -files /usr/bin/gzip \
	github.com/u-root/u-root/cmds/boot/pxeboot \
	github.com/u-root/u-root/cmds/core/cat \
	github.com/u-root/u-root/cmds/core/elvish \
	github.com/u-root/u-root/cmds/core/init \
	github.com/u-root/u-root/cmds/core/ip \
	github.com/u-root/u-root/cmds/core/ls \
	github.com/u-root/u-root/cmds/core/kexec \
	github.com/u-root/u-root/cmds/core/pci \
	github.com/u-root/u-root/cmds/core/wget

core.cpio: flash.config Makefile
	GO111MODULE=off u-root -o $@

justkernel: flash.config
	cp $< linux/.config
	(cd linux && make olddefconfig && make -j32)
	cp linux/arch/x86/boot/bzImage $@

flashkernel: flash.config core.cpio
	cp $< linux/.config
	echo CONFIG_CMDLINE_BOOL=y >> linux/.config
	echo CONFIG_CMDLINE_OVERRIDE=y >> linux/.config
	echo 'CONFIG_CMDLINE="noefi ip=dhcp earlyprintk=ttyS0,115200,keep console=ttyS0,115200"' >> linux/.config
	(cd linux && make olddefconfig && make -j32)
	cp linux/arch/x86/boot/bzImage $@

efikernel: flash.config
	cp $< linux/.config
	echo CONFIG_CMDLINE_BOOL=y >> linux/.config
	echo CONFIG_CMDLINE_OVERRIDE=y >> linux/.config
	echo CONFIG_EFI_STUB=y >> linux/.config
	echo 'CONFIG_CMDLINE="earlyprintk=ttyS0,115200,keep console=ttyS0,115200"' >> linux/.config
	(cd linux && make olddefconfig && make -j32)
	cp linux/arch/x86/boot/bzImage $@

testflashkernel: flashkernel flashinitramfs.cpio
	qemu-system-x86_64 -m 8192 -kernel flashkernel -nographic -serial stdio -initrd flashinitramfs.cpio -monitor $(MONITOR) $(EXTRA)

testefikernel: efikernel flashinitramfs.cpio
	qemu-system-x86_64 -bios $(OVMF_BIN) -nographic \
		-kernel efikernel -initrd flashinitramfs.cpio

fetch: getkernel geturoot

getkernel:
	rm -rf linux
	git clone --depth=1 -b v5.15 https://github.com/torvalds/linux

geturoot:
	GO111MODULE=off go get -u github.com/u-root/u-root
