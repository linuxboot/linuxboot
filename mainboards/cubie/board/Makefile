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

uCore: netbooturoot
	mkimage -A arm -n 'u-root for Allwinner A10' -d initramfs.linux_arm.cpio -T ramdisk uCore

qemu: uCore
	qemu-system-arm -kernel uImage -machine cubieboard -serial  stdio -initrd uCore -append  'console=ttyS0'
	echo NO
	exit 0
	qemu-system-arm -kernel uImage -machine cubieboard -serial  stdio -initrd initramfs.linux_arm.cpio
	echo NO
	exit 0
	sudo /usr/bin/qemu-system-x86_64 -kernel kernel \
		-cpu  max \
		-s   \
		-m 1024m \
		-machine q35  \
		-object rng-random,filename=/dev/urandom,id=rng0 \
		-device virtio-rng-pci,rng=rng0 \
		-device e1000,netdev=n1 \
		-netdev user,id=n1,hostfwd=tcp:127.0.0.1:23-:2222,net=192.168.1.0/24,host=192.168.1.1 \
		-serial stdio  \
		-append earlyprintk=ttyS0,115200\ console=ttyS0 \
		-monitor /dev/null  \


# add the following line to qemu if you want to snoop packets.
	-object filter-dump,id=f1,netdev=n1,file=/tmp/vm0.pcap \


kernel: uroot bzImage
	cp linux/arch/x86/boot/bzImage kernel

netbootkernel: netbooturoot bzImage
	cp linux/arch/x86/boot/bzImage kernel

readrom:
	sudo flashrom -p internal -r coreboot.bin

writerom: image.bin
	sudo flashrom -p internal -w image.bin

netbooturoot:
	echo do not do GOARCH=arm u-root -build=bb all
	GOARCH=arm u-root -build=bb \
		-files ~/.ssh/cpu_rsa.pub:key.pub \
		-files ~/.ssh/cpu_rsa:id_rsa \
		-files ~/.ssh/authorized_keys:authorized_keys \
		all \
		github.com/u-root/u-root/cmds/boot/uinit
	cp /tmp/initramfs.linux_arm.cpio .
	lzma -k -f initramfs.linux_arm.cpio
	ls -l initramfs*
	cp *lzma linux

uroot:
	GOARCH=arm u-root -initcmd=/bbin/cpu -build=bb -files ~/.ssh/cpu_rsa.pub:key.pub \
	all \
	github.com/u-root/cpu/cmds/cpu 
	cp /tmp/initramfs.linux_arm.cpio .
	lzma -k -f initramfs.linux_arm.cpio
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

