default: build

build:
	echo fetch, uroot, flashkernel, or image.bin

image.bin: flashkernel apu.bin
	cp apu.bin image.bin
	chmod +w image.bin
	cbfstool image.bin print
	cbfstool image.bin remove -n img/nerf
	cbfstool image.bin remove -n genroms/pxe.rom
	cbfstool image.bin add-payload -n img/nerf -f flashkernel
	cbfstool image.bin print

flashkernel: uroot bzImage
	cp linux/arch/x86/boot/bzImage flashkernel

readrom:
	sudo flashrom -p internal -r apu.bin

writerom: image.bin
	sudo flashrom -p internal -w image.bin

uroot:
	go run github.com/u-root/u-root -build=bb -initcmd=/bbin/cpud -files ~/.ssh/cpu_rsa.pub:key.pub \
		github.com/u-root/cpu/cmds/cpud \
		github.com/u-root/u-root/cmds/core/cat \
		github.com/u-root/u-root/cmds/core/dhclient \
		github.com/u-root/u-root/cmds/core/elvish \
		github.com/u-root/u-root/cmds/core/init \
		github.com/u-root/u-root/cmds/core/ip \
		github.com/u-root/u-root/cmds/core/ls \
		github.com/u-root/u-root/cmds/core/ps \
		github.com/u-root/u-root/cmds/core/pwd \
		github.com/u-root/u-root/cmds/core/rm \
		github.com/u-root/u-root/cmds/core/shutdown 
	cp /tmp/initramfs.linux_amd64.cpio .
	lzma -k -f initramfs.linux_amd64.cpio
	ls -l initramfs*
	cp *lzma linux

bzImage:
	cp config-5.15.0 linux/.config
	(cd linux && make oldconfig && make -j32)

fetch: getkernel geturoot getfiano getrom
getkernel:
	rm -rf linux
	wget -O kernel.xz https://cdn.kernel.org/pub/linux/kernel/v5.x/linux-5.15.tar.xz
	xzcat kernel.xz | tar x
	mv linux-5.15 linux
getrom:
	echo you create the kernel
	echo on the APU2: flashrom -r apu.bin -p internal
	echo copy that file here and for safety:
	echo chmod a-w apu.bin
geturoot:
	go get -u github.com/u-root/u-root
	go get -u github.com/u-root/cpu/...
getfiano:
	go get -u github.com/linuxboot/fiano/cmds/utk
	go install github.com/linuxboot/fiano/cmds/utk
	go install github.com/linuxboot/fiano/cmds/guid2english

