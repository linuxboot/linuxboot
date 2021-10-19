# This is the makefile for a netbootable image. It has cpud build in.
# see README.md in this directory for more information.

default: netbootkernel

u-root:
	GO111MODULE=off go install github.com/u-root/u-root
	@echo Make sure $(GOPATH)/bin is in your PATH
	@echo if GOPATH is empty, then put ~/go/bin in your PATH

usefultargets:
	echo fetch, uroot, netbootkernel

# the kexectestimage has standard u-root commands, kexec-tools version of kexec, and two
# test kernels, a 5.10 and a 5.4
kexectestimage-5.10: kexecconfig-5.10 kexecinitramfs.cpio
	cp $< linux-5.10/.config
	(cd linux-5.10 && CROSS_COMPILE=aarch64-linux-gnu- ARCH=arm64 make oldconfig && CROSS_COMPILE=aarch64-linux-gnu- ARCH=arm64 make -j32)
	cp linux-5.10/arch/arm64/boot/Image $@

kexectestimage-5.4: kexecconfig-5.4 kexecinitramfs.cpio
	cp $< linux/.config
	(cd linux && CROSS_COMPILE=aarch64-linux-gnu- ARCH=arm64 make oldconfig && CROSS_COMPILE=aarch64-linux-gnu- ARCH=arm64 make -j32)
	cp linux/arch/arm64/boot/Image $@

kernel-5.4: config-5.4.47 flashinitramfs.cpio
	cp $< linux/.config
	(cd linux && CROSS_COMPILE=aarch64-linux-gnu- ARCH=arm64 make oldconfig && CROSS_COMPILE=aarch64-linux-gnu- ARCH=arm64 make -j32)
	cp linux/arch/arm64/boot/Image $@

kexecinitramfs.cpio: u-root Makefile kernel-5.4 kernel-5.10
	GO111MODULE=off GOARCH=arm64 u-root -o $@ -build=bb \
		-files ~/.ssh/cpu_rsa.pub:key.pub \
		-files kernel-5.4 -files kernel-5.10 \
		github.com/u-root/cpu/cmds/cpud \
		github.com/u-root/u-root/cmds/boot/pxeboot \
		core \

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

kernel-5.10: config-5.10 flashinitramfs.cpio
	cp $< linux/.config
	(cd linux-5.10 && CROSS_COMPILE=aarch64-linux-gnu- ARCH=arm64 make oldconfig && CROSS_COMPILE=aarch64-linux-gnu- ARCH=arm64 make -j32)
	cp linux-5.10/arch/arm64/boot/Image $@

# These stanzas fetch code.
fetch: getkernel geturoot apt

getkernel:
	rm -rf linux
	git clone --depth=1 -b solidrun-honeycomb --single-branch https://github.com/linuxboot/linux

getkernel-5.10:
	rm -rf linux-5.10
	git clone --depth=1 -b v5.10 --single-branch https://github.com/torvalds/linux linux-5.10

geturoot:
	GO111MODULE=off go get -u github.com/u-root/u-root
	GO111MODULE=off go get -u github.com/u-root/cpu/...

apt:
	sudo apt install gcc-aarch64-linux-gnu
