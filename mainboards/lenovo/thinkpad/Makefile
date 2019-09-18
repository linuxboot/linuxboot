default: build

build:
	echo fetch, uroot, kernel, or image.bin

image.bin:  kernel coreboot.bin
	cp coreboot.bin image.bin
	chmod +w image.bin
	cbfstool image.bin print
	cbfstool image.bin remove -n img/nerf
	cbfstool image.bin remove -n genroms/pxe.rom
	cbfstool image.bin add-payload -n img/nerf -f kernel
	cbfstool image.bin print

kernel: uroot bzImage
	cp linux/arch/x86/boot/bzImage kernel

readrom:
	sudo flashrom -p internal -r coreboot.bin

writerom: image.bin
	sudo flashrom -p internal -w image.bin

uroot:
	go run github.com/u-root/u-root -build=bb -files ~/.ssh/cpu_rsa.pub:key.pub \
	-files /bin/fusermount \
	all \
	github.com/u-root/u-root/cmds/exp/cpu 
	cp /tmp/initramfs.linux_amd64.cpio .
	lzma -k -f initramfs.linux_amd64.cpio
	ls -l initramfs*
	cp *lzma linux

bzImage:
	cp config-4.14.111 linux/.config
	(cd linux && make oldconfig && make -j32)

fetch: getkernel geturoot getfiano getrom
getkernel:
	rm -rf linux
	wget -O kernel.xz https://cdn.kernel.org/pub/linux/kernel/v4.x/linux-4.14.111.tar.xz
	xzcat kernel.xz | tar x
	mv linux-4.14.111 linux
getrom:
	echo you create the kernel
	echo on the APU2: flashrom -r coreboot.bin -p internal
	echo copy that file here and for safety:
	echo chmod a-w coreboot.bin
geturoot:
	go get github.com/u-root/u-root

