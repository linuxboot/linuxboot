MAJOR ?= 5
STABLE = $(MAJOR).10
CONFIG ?= $(MAJOR).10.0
default: netbooturoot.sfs

cp:netbooturoot.sfs 
	sudo mount /dev/sdb1 /mnt
	sudo cp netbooturoot.sfs  /mnt/
	sudo umount /mnt

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

uCore: netbooturoo.cpio
	mkimage -A arm -n 'u-root for Allwinner A10' -d netbooturoot.cpio -T ramdisk uCore

qemu: uCore kernel
	qemu-system-arm -kernel kernel -machine cubieboard -serial  stdio -initrd uCore -append  'console=ttyS0 earlyprintk=ttyS0,115200,keep'
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


netbootkernel: netbooturoot.cpio bzImage
	cp linux/arch/arm/boot/bzImage kernel

readrom:
	sudo flashrom -p internal -r coreboot.bin

writerom: image.bin
	sudo flashrom -p internal -w image.bin

netbooturoot.sfs: netbooturoot.cpio
	sudo rm -rf SFS
	mkdir -p SFS
	(cd SFS && sudo cpio -iv < ../$<)
	rm -f $@
	sudo mksquashfs SFS $@ -comp xz

netbooturoot.cpio: Makefile
	GOARCH=arm go build -o wifi github.com/u-root/webboot/cmds/wifi
	GOARCH=arm go build -o webboot github.com/u-root/webboot/cmds/webboot
	GOARCH=arm u-root -build=bb \
	       -o $@ \
		-files ~/.ssh/cpu_rsa.pub:key.pub \
		-files ~/.ssh/cpu_rsa:id_rsa \
		-files ~/.ssh/authorized_keys:authorized_keys \
		-files  ~/arm/etc:etc \
		-files  ~/arm/bin:bin \
		-files  ~/arm/sbin:sbin \
		-files  ~/arm/usr:usr \
		-files bak/drv/8188fu.ko.lzma:8188fu.ko.lzma \
		-files boot:boot \
		-files bootSCREEN \
		-files wifi \
		-files webboot \
		all \
		github.com/u-root/cpu/cmds/cpud

# for later:
	#mkdir /tmp/r
# mkdir /tmp/m
# mount -t vfat /dev/mmcblk0p1 /tmp/m
# mount -t squashfs /tmp/m/u-root-victure.sfs /tmp/r

uroot:
	GOARCH=arm u-root -build=bb \
	-files ~/.ssh/cpu_rsa.pub:key.pub \
	all \
	github.com/u-root/cpu/cmds/cpu 
	cp /tmp/initramfs.linux_arm.cpio cpu.cpio

kernel: Makefile
	cp config-$(CONFIG) linux/.config
	(cd linux && ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- make oldconfig && ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- make -j32)
	mkimage -C none -A arm -n 'walmart robot' -d linux/arch/arm/boot/zImage -T kernel -a 0xc1000000 -e 0xc1000000 kernel
	cp linux/arch/arm/boot/zImage kernel

fetch: getkernel geturoot getfiano getrom getpackages

getpackages:
	sudo apt install gcc-9-arm-linux-gnueabihf gcc-10-multilib-arm-linux-gnueabihf

getkernel:
	rm -rf linux
	wget -O kernel.xz https://cdn.kernel.org/pub/linux/kernel/v$(MAJOR).x/linux-$(STABLE).tar.xz
	xzcat kernel.xz | tar x
	mv linux-$(STABLE) linux

getrom:
	echo you create the kernel
	echo on the APU2: flashrom -r coreboot.bin -p internal
	echo copy that file here and for safety:
	echo chmod a-w coreboot.bin
geturoot:
	go get -u github.com/u-root/u-root

