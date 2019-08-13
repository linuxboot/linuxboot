default: build

build:
	echo getstuff, kernel, or qemu

qemu: kernel
	qemu-system-x86_64 -kernel kernel -serial stdio -monitor /dev/null

kernel: uroot bzImage
	cp linux/arch/x86/boot/bzImage kernel

uroot:
	go run github.com/u-root/u-root -build=bb all   
	cp /tmp/initramfs.linux_amd64.cpio .
	lzma -k -f initramfs.linux_amd64.cpio
	ls -l initramfs*
	cp *lzma linux

bzImage:
	cp config-4.14.111 linux/.config
	(cd linux && make oldconfig && make -j32)

getstuff: getkernel geturoot 
getkernel:
	rm -rf linux
	wget -O kernel.xz https://cdn.kernel.org/pub/linux/kernel/v4.x/linux-4.14.111.tar.xz
	xzcat kernel.xz | tar x
	mv linux-4.14.111 linux
geturoot:
	go get github.com/u-root/u-root

