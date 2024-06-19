default: build

build:
	echo fetch, flashkernel, testflashkernel and housekeeping

flashkernel: linuxboot-linux.tiny.config flashinitramfs.cpio.lzma Makefile
	cp $< linux/.config
	cp flashinitramfs.cpio.lzma linux/
	echo CONFIG_CMDLINE_BOOL=y >> linux/.config
	echo CONFIG_CMDLINE_OVERRIDE=y >> linux/.config
	stat -c 'CONFIG_CMDLINE="noefi ip=dhcp earlyprintk=ttyS0,115200,keep console=ttyS0,115200"' flashinitramfs.cpio.lzma >> linux/.config
	(cd linux && make oldconfig && make -j32)
	cp linux/arch/x86/boot/bzImage $@

flashinitramfs.cpio.lzma: flashinitramfs.cpio
	lzma -f -k $<

flashinitramfs.cpio: Makefile
	go run github.com/u-root/u-root  -o $@ -build=bb \
		-uinitcmd=/bbin/console \
		github.com/u-root/u-root/cmds/boot/pxeboot \
		github.com/u-root/u-root/cmds/core/cat \
		github.com/u-root/u-root/cmds/core/elvish \
		github.com/u-root/u-root/cmds/core/init \
		github.com/u-root/u-root/cmds/core/ip \
		github.com/u-root/u-root/cmds/core/ls \
		github.com/u-root/u-root/cmds/core/kexec \
		github.com/u-root/u-root/cmds/core/pci \
		github.com/u-root/u-root/cmds/core/wget \
		github.com/u-root/u-root/cmds/exp/console \

fetch: getkernel geturoot getfiano

getkernel:
	rm -rf linux
	git clone --depth=1 -b v5.15 --single-branch https://github.com/torvalds/linux

getfiano:
	go install github.com/linuxboot/fiano/cmds/utk@latest
	go install github.com/linuxboot/fiano/cmds/guid2english@latest

geturoot:
	go install github.com/u-root/u-root@latest
	go install github.com/u-root/cpu/...@latest

testflashkernel: flashkernel
	echo ^C will exit qemu! be careful!
	qemu-system-x86_64 -kernel flashkernel -nographic -serial /dev/tty -initrd flashinitramfs.cpio.lzma -monitor /dev/null

housekeeping:
	rm -rf flashinitramfs.cpio \
	       flashinitramfs.cpio.lzma \
	       flashkernel \

