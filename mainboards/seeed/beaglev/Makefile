# This is the makefile for a kernel, uboot image, and initramfs from u-root
#
# sshdkernel produces a kernel with a built-in initramfs for netboot
# It provides (and starts) an sshd.
# ssh -i class_key -p 2022 board date
#
# The cpu image uses a cpud as its init, and you
# can cpu to it to run commands.
# to flash the flash image.
# e.g.
# cpu board date
# cpu is way more convenient than ssh/scp if you can learn it.
# you can also just
# cpu board
# NOTE: those commands are running on the board, and they all work
#
# You can test the flash image kernel with qemu
# make flashtest

default: cpu
	echo you can add any of these targets to the beaglev image.
	echo as img/nerf. You can also set priority.

flashtest: testflashkernel

usefultargets:
	echo fetch, sshdkernel, flashkernel, cpukernel, etc.

sshdkernel: sshd.cpio.lzma sshd.config
	cp sshd.config linux/.config
	(cd linux && make oldconfig && make -j32)
	cp linux/arch/x86/boot/bzImage $@

cpu: cpukernel u-boot.bin u-boot.dtb cpu.cpio.gz
	echo Now copy these things to the stick.

cpukernel: cpu.config Makefile
	cp cpu.config linux/.config
	(cd linux && make CROSS_COMPILE=riscv64-linux-gnu- ARCH=riscv -j4 oldconfig && make CROSS_COMPILE=riscv64-linux-gnu- ARCH=riscv -j4 && mkdir -p boot && make CROSS_COMPILE=riscv64-linux-gnu- ARCH=riscv INSTALL_PATH=/tmp zinstall)
	cp /tmp/vmlinuz-5.10.6+ vmlinuz
	cp linux/arch/riscv/boot/Image.gz .

readrom:
	echo You need to get a ROM image from *somewhere*
	echo Once you do, put it in ROM.bin

u-boot.bin: ubootconfig
	(cd uboot && make CROSS_COMPILE=riscv64-linux-gnu- ARCH=riscv u-boot.bin)
	cp uboot/u-boot.bin $@

u-boot.dtb: ubootconfig
	(cd uboot && make CROSS_COMPILE=riscv64-linux-gnu- ARCH=riscv u-boot.dtb)
	cp uboot/u-boot.dtb $@

ubootconfig: Makefile
	echo This is idempotent
	sed -i 's/CONFIG_SYS_TEXT_BASE=.*/CONFIG_SYS_TEXT_BASE=0x80020000/p' uboot/configs/starfive_vic7100_beagle_v_smode_defconfig
	(cd uboot && make CROSS_COMPILE=riscv64-linux-gnu- ARCH=riscv starfive_vic7100_beagle_v_smode_defconfig)

writerom: image.bin
	echo Here is where you would do the flashrom, e.g.
	echo and maybe a cbfstool
	echo cbfstool beaglev.rom remove -n img/nerf
	echo cbfstool beaglev.rom add-payload -n img/nerf -f cpukernel
	echo sudo flashrom -p internal -w image.bin

flashinitramfs.cpio.lzma: flashinitramfs.cpio
	lzma -f -k $<

flashinitramfs.cpio: Makefile
	go run github.com/u-root/u-root  -o $@ -build=bb \
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

# This target builds a full-featured initramfs INCLUDING cpud.
# You need ip=dhcp OR ip=fixed.ip.address.here in the command line
cpu.cpio.gz: Makefile
	GOARCH=riscv64 u-root -o cpu.cpio -files ~/.ssh/cpu_rsa.pub:key.pub \
		all \
		github.com/u-root/cpu/cmds/cpud
	gzip -f -k cpu.cpio

# this target builds an initramfs with all of u-root, AND all of /bin, /usr/bin, /sbin, /usr/sbin, /etc
# the intent is  to demonstrate the capability of http loading.
# It depends on the kernel setting the IP address.
# You need ip=dhcp OR ip=fixed.ip.address.here in the command line
sshd.cpio.lzma:  Makefile
	go run github.com/u-root/u-root -o sshd.cpio -build=bb \
		-uinitcmd=/bbin/sshd \
		-files class_key.pub:authorized_keys \
		-files classhostkey:id_rsa \
		-files /usr/bin/vi \
		-files /usr/share/vim \
		-files /usr/share/terminfo \
		-files /bin/bash \
		-files /usr/sbin/flashrom \
		-files /usr/bin/xz \
		all
	lzma -f -k sshd.cpio
	ls -l sshd.cpio.*

flashkernel: flash.config flashinitramfs.cpio.lzma Makefile
	cp $< linux/.config
	(cd linux && make oldconfig && make -j32)
	cp linux/arch/x86/boot/bzImage $@

testflashkernel: flashkernel
	qemu-system-x86_64 -kernel flashkernel -nographic -serial /dev/tty -initrd flashinitramfs.cpio.lzma

testcpukernel: cpukernel
	qemu-system-x86_64 -kernel cpukernel -nographic -serial /dev/tty

# These stanzas fetch code.
fetch: getkernel geturoot getrom getuboot

getkernel:
	rm -rf linux
	git clone https://github.com/starfive-tech/beagle_kernel_5.10 linux
	(cd linux && git checkout -b starfive origin/starfive)
# fyi: make CROSS_COMPILE=riscv64-linux-gnu- ARCH=riscv starfive_vic7100_evb_sd_net_fedora_defconfig
# Change to taste

getrom:
	echo you can use flashrom to read the beaglev image.

geturoot:
	go get -u github.com/u-root/u-root
	go get -u github.com/u-root/cpu/...

getuboot:
	git clone https://github.com/starfive-tech/beagle_uboot-opensbi uboot

packages:
	echo this sucks.
	sudo apt-get \
		install \
		autoconf \
		automake \
		autotools-dev \
		bc \
		bison \
		build-essential \
		curl \
		flex \
		gawk \
		gdisk \
		git \
		gperf \
		libgmp-dev \
		libmpc-dev \
		libmpfr-dev \
		libncurses-dev \
		libssl-dev \
		libtool \
		patchutils \
		python \
		screen \
		texinfo \
		unzip \
		zlib1g-dev \
		device-tree-compiler
	sudo apt install gcc-riscv64-linux-gnu

# Serve the combined sshd-kernel and sshd-initramfs image. This includes flashrom
sshd-pxeserver:
	echo Consider Disabling any system services ...
	echo e.g. sudo systemctl  stop tftpd-hpa.service
	echo e.g. sudo systemctl stop isc-dhcp-server
	sudo `go env GOPATH`/bin/pxeserver  -tftp-dir . -bootfilename sshdkernel  -http-dir . -interface $(NETWORK)

# Serve the combined cpu-kernel and cpu-initramfs image.
cpu-pxeserver:
	echo Consider Disabling any system services ...
	echo e.g. sudo systemctl  stop tftpd-hpa.service
	echo e.g. sudo systemctl stop isc-dhcp-server
	sudo `go env GOPATH`/bin/pxeserver  -tftp-dir . -bootfilename cpukernel  -http-dir . -interface $(NETWORK)

ssh:
	ssh -i class_key -p 2022 root@up
