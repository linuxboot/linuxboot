# This is the makefile for a cpu kernel and initramfs
# to be used on a rvboards d1
#
# The cpu image uses a cpud as its init, and you
# can cpu to it to run commands.

default: cpukernel

cpukernel: cpu.cpio.xz cpu.config
	cp cpu.config linux/.config
	(cd linux && CROSS_COMPILE=riscv64-linux-gnu- ARCH=riscv make oldconfig && CROSS_COMPILE=riscv64-linux-gnu- ARCH=riscv make -j32)
	cp linux/arch/riscv/boot/Image cpukernel
	cp linux/arch/riscv/boot/dts/allwinner/sun20i-d1-nezha.dtb .

# this target builds an initramfs with cpud as init, and the core
# set of u-root commands.
# It depends on the kernel setting the IP address.
# You need ip=dhcp OR ip=fixed.ip.address.here in the command line
cpu.cpio: Makefile
	GOARCH=riscv64 CGO_ENABLED=0 u-root -o cpu.cpio -build=bb -files ~/.ssh/cpu_rsa.pub:key.pub \
		    -uroot-source $(HOME)/go/src/github.com/u-root/u-root \
		core \
		$(HOME)/go/src/github.com/u-root/cpu/cmds/cpud

cpu.cpio.xz: cpu.cpio
	xz --check=crc32 -9 --lzma2=dict=1MiB --stdout cpu.cpio | dd conv=sync bs=512 of=cpu.cpio.xz

# These stanzas fetch code.
fetch: getkernel geturoot

getkernel:
	rm -rf linux
	git clone --depth=1 -b v5.15.5-openwrt --single-branch https://github.com/orangecms/linux

geturoot:
	GO111MODULE=off go get -u github.com/u-root/u-root
	GO111MODULE=off go get -u github.com/u-root/cpu/...

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
