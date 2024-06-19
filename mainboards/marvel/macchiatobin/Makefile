# This is the makefile for a flash image, a netbootable sshd image,
# and a netbootable cpu image.
#
# sshdkernel produces a kernel with a built-in initramfs for netboot
# It provides (and starts) an sshd, and also has flashrom.
# Hence you can ssh to a node, flashrom -r on it, scp the
# image back, process it, scp the new image to the node, and
# write with flashrom.
# e.g.
# ssh -i ../../class_key -p 2022 board flashrom -r image.bin -p internal
# scp -i ../../class_key -P 2022 board:/image.bin image.bin
# make tiny.bin
# scp -i ../../class_key -P 2022 tiny.bin board:/tiny.bin
# ssh -i ../../class_key -p 2022 board flashrom -w tiny.bin -p internal
#
# The cpu image uses a cpud as its init, and you
# can cpu to it to run commands, including flashrom -p internal
# to flash the flash image.
# e.g.
# cpu board flashrom -r image.bin -p internal
# Note this writes image image.bin to the host directoy; no scp needed
# make tiny.bin
# cpu board flashrom -w tiny.bin -p internal
# cpu is way more convenient than ssh/scp if you can learn it.
# you can also just
# cpu board
# and once you are in:
# flashrom -r image.bin -p internal
# make tiny.bin
# flashrom -w tiny.bin -p internal
# NOTE: those commands are running on the board, and they all work
#
# You can test the flash image kernel with qemu
# make flashtest

MARVELL_GIT?=https://github.com/MarvellEmbeddedProcessors
TOOLCHAIN?=aarch64-linux-gnu-

default: flash-image.bin

u-root:
	go install github.com/u-root/u-root
	@echo Make sure $(GOPATH)/bin is in your PATH
	@echo if GOPATH is empty, then put ~/go/bin in your PATH

todo: sshdkernel flash cpukernel

flash-image.bin: flashkernel sd

sd:
	(cd tfa && make \
		CROSS_COMPILE=$(TOOLCHAIN) \
		Arch=arm64 \
		SCP_BL2=../binaries-marvell/mrvl_scp_bl2_8040.img \
		BL33=../linux/arch/arm64/boot/Image.gz	\
		BL33_DTB=../linux/arch/arm64/boot/dts/marvell/armada-8040-mcbin.dtb \
		USE_COHERENT_MEM=0 \
		ARM_LINUX_KERNEL_AS_BL33=1 \
		MV_DDR_PATH=../mv-ddr-marvell \
		PLAT=a80x0_mcbin \
		all fip)
	echo build result is tfa/build/a80x0_mcbin/release/flash-image.bin

flashtest: testflashkernel

usefultargets:
	echo fetch, uroot, kernel, apt (for crossbuild) or image.bin

tiny.bin: flashkernel flashinitramfs.cpio.xz

sshdkernel: sshd.cpio.xz netbootkernel.config
	cp netbootkernel.config linux/.config
	(cd linux && make oldconfig && make -j32)
	cp linux/arch/x86/boot/bzImage $@

# at the moment, the kernel used for the cpu command and the sshdkernel
# are the same.
cpukernel: sshdkernel
	cp sshdkernel $@

writerom: flash-image.bin
	@echo Use this command. It will not be run from this Makefile
	@echo dd if=flash-image.bin of=/dev/null skip=1

flashinitramfs.cpio.xz: flashinitramfs.cpio
	xz --check=crc32  -f -k $<

flashinitramfs.cpio: u-root Makefile
	GOARCH=arm64 u-root -o $@ -build=bb \
		-uinitcmd=/bbin/pxeboot \
		github.com/u-root/u-root/cmds/boot/pxeboot \
		github.com/u-root/u-root/cmds/core/cat \
		github.com/u-root/u-root/cmds/core/elvish \
		github.com/u-root/u-root/cmds/core/init \
		github.com/u-root/u-root/cmds/core/ip \
		github.com/u-root/u-root/cmds/core/ls \
		github.com/u-root/u-root/cmds/core/kexec \
		github.com/u-root/u-root/cmds/core/pci \
		github.com/u-root/u-root/cmds/core/wget \

# this target builds an initramfs with only one program, the cpu server.
# It depends on the kernel setting the IP address.
# You need ip=dhcp OR ip=fixed.ip.address.here in the command line
cpu.cpio.xz: u-root Makefile
	GOARCH=arm64 u-root -o cpu.cpio -build=bb -initcmd=cpud -files ~/.ssh/cpu_rsa.pub:key.pub \
		-defaultsh="" \
		github.com/u-root/cpu/cmds/cpud
	xz --check=crc32  -f -k cpu.cpio

# this target builds an initramfs with all of u-root, AND all of /bin, /usr/bin, /sbin, /usr/sbin, /etc
# the intent is  to demonstrate the capability of http loading.
# It depends on the kernel setting the IP address.
# You need ip=dhcp OR ip=fixed.ip.address.here in the command line
sshd.cpio.xz:  u-root Makefile
	GOARCH=arm64 u-root -o sshd.cpio -build=bb \
		-uinitcmd=/bbin/sshd \
		-files ../../class_key.pub:authorized_keys \
		-files ../../classhostkey:id_rsa \
		-files /usr/bin/vi \
		-files /usr/share/vim \
		-files /usr/share/terminfo \
		-files /bin/bash \
		-files /usr/sbin/flashrom \
		-files /usr/bin/xz \
		all
	xz --check=crc32  -f -k sshd.cpio
	ls -l sshd.cpio.*

flashkernel: config-5.6.3 flashinitramfs.cpio.xz
	cp $< linux/.config
	(cd linux && CROSS_COMPILE=aarch64-linux-gnu- ARCH=arm64 make oldconfig && CROSS_COMPILE=aarch64-linux-gnu- ARCH=arm64 make -j32)

testflashkernel: flashkernel
	qemu-system-arm64 -kernel flashkernel -nographic -serial /dev/tty -initrd flashinitramfs.cpio.xz

# These stanzas fetch code.
fetch: getkernel geturoot gettfa fetchblobs

gettfa:
	rm -rf tfa
	echo NOT THIS ONE git clone https://github.com/fozog/arm-trusted-firmware.git tfa
	git clone --branch linuxboot4mcbin https://github.com/fozog/arm-trusted-firmware.git tfa

getkernel:
	rm -rf linux
	git clone --depth=1 -b v5.6 --single-branch https://github.com/torvalds/linux

geturoot:
	go install github.com/u-root/u-root@latest
	go install github.com/u-root/cpu/...@latest

# Serve the combined sshd-kernel and sshd-initramfs image. This includes flashrom
sshd-pxeserver:
	echo Consider Disabling any system services ...
	echo e.g. sudo systemctl  stop tftpd-hpa.service
	echo e.g. sudo systemctl stop isc-dhcp-server
	sudo `go env GOPATH`/bin/pxeserver  -tftp-dir . -bootfilename sshdkernel  -http-dir . -interface $(NETWORK)

fetchblobs:
	rm -rf mv-ddr-marvell binaries-marvell
	git clone --branch mv_ddr-armada-atf-mainline $(MARVELL_GIT)/mv-ddr-marvell.git
	git clone --branch binaries-marvell-armada-17.10 $(MARVELL_GIT)/binaries-marvell

apt:
	sudo apt install gcc-aarch64-linux-gnu

# Serve the combined cpu-kernel and cpu-initramfs image.
cpu-pxeserver:
	echo Consider Disabling any system services ...
	echo e.g. sudo systemctl  stop tftpd-hpa.service
	echo e.g. sudo systemctl stop isc-dhcp-server
	sudo `go env GOPATH`/bin/pxeserver  -tftp-dir . -bootfilename sshdkernel  -http-dir . -interface $(NETWORK)
ssh:
	ssh -i class_key -p 2022 root@up
