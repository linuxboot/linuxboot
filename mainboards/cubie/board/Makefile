VER=5.4.0
default: build

build: uCore flashkernel

# this target builds an initramfs with only one program, the cpu server.
# It depends on the kernel setting the IP address.
# You need ip=dhcp OR ip=fixed.ip.address.here in the command line
cpu.cpio:
	GOARCH=arm u-root -o cpu.cpio -build=bb -initcmd=cpud -files ~/.ssh/cpu_rsa.pub:key.pub \
		-defaultsh="" \
		github.com/u-root/cpu/cmds/cpud

uCore: netbooturoot
	mkimage -A arm -n 'u-root for Allwinner A10' -d uroot.cpio -T ramdisk uCore

qemu: uCore flashkernel
	qemu-system-arm -kernel flashkernel -machine cubieboard -serial  stdio -initrd uCore -append  'console=ttyAM0 earlyprintk=ttyAM0,115200,keep' -display none
	echo NO
	exit 0

netbooturoot:
	GOARCH=arm u-root \
	       -o uroot.cpio \
		-files ~/.ssh/cpu_rsa.pub:key.pub \
		-files ~/.ssh/cpu_rsa:id_rsa \
		-files ~/.ssh/authorized_keys:authorized_keys \
		all

flashkernel:
	cp config-$(VER) linux/.config
	(cd linux && ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- make oldconfig && ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- make -j32)
	mkimage -C none -A arm -n 'kernel for Allwinner A10' -d linux/arch/arm/boot/zImage -T kernel -a 0x40008000 -e 0x40008000 kernel
	cp linux/arch/arm/boot/zImage flashkernel

fetch: getkernel geturoot

getkernel:
	rm -rf linux
	git clone https://github.com/linux-sunxi/linux-sunxi.git -b sunxi-next --depth=1 linux
	(cd linux && make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- sunxi_defconfig)

geturoot:
	go install github.com/u-root/u-root@latest

packages:
	sudo apt install gcc-arm-linux-gnueabihf
	sudo apt install u-boot-tools
