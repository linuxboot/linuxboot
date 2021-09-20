flashinitramfs.cpio:
	u-root -o $@ \
	github.com/u-root/u-root/cmds/boot/pxeboot \
	github.com/u-root/u-root/cmds/core/cat \
	github.com/u-root/u-root/cmds/core/elvish \
	github.com/u-root/u-root/cmds/core/init \
	github.com/u-root/u-root/cmds/core/ip \
	github.com/u-root/u-root/cmds/core/ls \
	github.com/u-root/u-root/cmds/core/kexec \
	github.com/u-root/u-root/cmds/core/pci \
	github.com/u-root/u-root/cmds/core/wget

flashkernel: flash.config
	cp $< linux/.config
	echo CONFIG_CMDLINE_BOOL=y >> linux/.config
	echo CONFIG_CMDLINE_OVERRIDE=y >> linux/.config
	echo 'CONFIG_CMDLINE="noefi ip=dhcp earlyprintk=ttyS0,115200,keep console=ttyS0,115200"' >> linux/.config
	(cd linux && make olddefconfig && make -j32)
	cp linux/arch/x86/boot/bzImage $@

testflashkernel: flashkernel flashinitramfs.cpio
	qemu-system-x86_64 -kernel flashkernel -nographic -initrd flashinitramfs.cpio

fetch: getkernel geturoot

getkernel:
	rm -rf linux
	git clone --depth=1 -b v5.10 https://github.com/torvalds/linux

geturoot:
	go get -u github.com/u-root/u-root
