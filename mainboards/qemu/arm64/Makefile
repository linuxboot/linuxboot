flashinitramfs.cpio: flashkernel core.cpio
	GOARCH=arm64 u-root -o $@ \
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
	GOARCH=arm64 u-root -o $@

flashkernel: flash.config core.cpio
	cp $< linux/.config
	echo CONFIG_CMDLINE_BOOL=y >> linux/.config
	echo CONFIG_CMDLINE_OVERRIDE=y >> linux/.config
	(cd linux && make ARCH=arm64 olddefconfig && make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- -j32)
	cp linux/arch/arm64/boot/Image $@

testflashkernel: flashkernel flashinitramfs.cpio
	qemu-system-aarch64 -device VGA,edid=on -device qemu-xhci -device usb-tablet -device usb-kbd \
		-object rng-random,filename=/dev/urandom,id=rng0 -device virtio-rng-pci,rng=rng0 \
		-nographic -machine virt -cpu cortex-a57 -m 512 -serial mon:stdio -serial null \
		-initrd flashinitramfs.cpio \
		-kernel flashkernel \
		-append console=ttyAMA0,115200

fetch: getkernel geturoot

installdep:
	sudo apt install gcc-aarch64-linux-gnu

getkernel:
	rm -rf linux
	git clone --depth=1 -b v5.11 https://github.com/torvalds/linux

geturoot:
	go install github.com/u-root/u-root@latest
