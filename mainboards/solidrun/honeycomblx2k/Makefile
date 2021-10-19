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


default: netbootkernel

u-root:
	GO111MODULE=off go install github.com/u-root/u-root
	@echo Make sure $(GOPATH)/bin is in your PATH
	@echo if GOPATH is empty, then put ~/go/bin in your PATH

usefultargets:
	echo fetch, uroot, netbootkernel

flashinitramfs.cpio: u-root Makefile
	GO111MODULE=off GOARCH=arm64 u-root -o $@ -build=bb \
		-files ~/.ssh/cpu_rsa.pub:key.pub \
		github.com/u-root/cpu/cmds/cpud \
		github.com/u-root/u-root/cmds/boot/pxeboot \
		core \

netbootkernel: config-5.4.47 flashinitramfs.cpio
	cp $< linux/.config
	(cd linux && CROSS_COMPILE=aarch64-linux-gnu- ARCH=arm64 make oldconfig && CROSS_COMPILE=aarch64-linux-gnu- ARCH=arm64 make -j32)
	cp linux/arch/arm64/boot/Image $@

# These stanzas fetch code.
fetch: getkernel geturoot 

getkernel:
	rm -rf linux
	git clone --depth=1 -b solidrun-honeycomb --single-branch https://github.com/linuxboot/linux

geturoot:
	GO111MODULE=off go get -u github.com/u-root/u-root
	GO111MODULE=off go get -u github.com/u-root/cpu/...

# Serve the combined sshd-kernel and sshd-initramfs image. This includes flashrom
sshd-pxeserver:
	echo Consider Disabling any system services ...
	echo e.g. sudo systemctl  stop tftpd-hpa.service
	echo e.g. sudo systemctl stop isc-dhcp-server
	sudo `go env GOPATH`/bin/pxeserver  -tftp-dir . -bootfilename sshdkernel  -http-dir . -interface $(NETWORK)

apt:
	sudo apt install gcc-aarch64-linux-gnu

# Serve the combined cpu-kernel and cpu-initramfs image.
cpu-pxeserver:
	echo Consider Disabling any system services ...
	echo e.g. sudo systemctl  stop tftpd-hpa.service
	echo e.g. sudo systemctl stop isc-dhcp-server
	sudo `go env GOPATH`/bin/pxeserver  -tftp-dir . -bootfilename sshdkernel  -http-dir . -interface $(NETWORK)
