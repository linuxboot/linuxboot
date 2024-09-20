INITRAMFS:=initramfs.linux_amd64.cpio

default: build

build:
	echo Run 'make fetch' first, which prepares the mainline build environment.
	echo Obtain the ROM image; download from the Internet or read from flash.
	echo Then make other targets, starting with a u-root flavor, finally Linux.
	echo When the new image is built, write it to flash.

# Prepare

fetch: getrom prepgows getfiano getkernel

getkernel:
	rm -rf linux
	git clone --depth=1 https://github.com/linuxboot/linux

getstablekernel:
	wget -O kernel.xz https://cdn.kernel.org/pub/linux/kernel/v5.x/linux-5.3.10.tar.xz
	xzcat kernel.xz | tar x
	mv linux-5.3.10 linux

upgetkernel:
	rm -rf linux
	git clone --depth=1 https://github.com/emutex/ubilinux-kernel.git -b upboard-4.9 linux

# Set up the Go workspace, including cpu.
# see https://github.com/u-root/u-root?tab=readme-ov-file#multi-module-workspace-builds
# NOTE: goanywhere is nice, but how would we combine cpud and u-root templates?
prepgows:
	git clone --depth 1 https://github.com/u-root/u-root
	git clone --depth 1 https://github.com/u-root/cpu
	go work init ./u-root
	go work use ./cpu

getfiano:
	go install github.com/linuxboot/fiano/cmds/utk@latest

# initramfs

alluroot:
	cd u-root && go run . -o $(INITRAMFS) \
		-initcmd=cpud -files ~/.ssh/cpu_rsa.pub:key.pub ../cpu/cmds/cpud \
		all

cpuuroot:
	cd u-root && go run . -o $(INITRAMFS) \
		-defaultsh="" \
		-initcmd=cpud -files ~/.ssh/cpu_rsa.pub:key.pub ../cpu/cmds/cpud

# TODO: Should we use a template instead? Or define one in this repo?
# https://github.com/u-root/u-root/blob/main/.mkuimage.yaml
neturoot:
	cd u-root && go run . -o $(INITRAMFS) \
		./cmds/core/init \
		./cmds/core/ls \
		./cmds/core/ip \
		./cmds/core/gosh \
		./cmds/core/sshd \
		./cmds/core/scp \
		./cmds/boot/*

uroot.lzma:
	lzma -f -k ./u-root/$(INITRAMFS)
	ls -l u-root/initramfs*
	cp u-root/*lzma linux

# Linux

# FIXME: The current config does not include the initramfs. Should it?
bzImage:
	cp linuxboot-linux.config linux/.config
	(cd linux && make oldconfig && make -j32)

stablebzImage:
	echo "the config is from https://github.com/emutex/ubilinux-kernel/issues/2#issue-294853930"
	cp ts.config.txt linux/.config
	(cd linux && make oldconfig && make -j32)

flashkernel: cpuuroot uroot.lzma bzImage
	cp linux/arch/x86/boot/bzImage flashkernel

netboot: neturoot uroot.lzma bzImage
	cp linux/arch/x86/boot/bzImage kernel

# ROM operations

image.bin:  
	utk -xzPath /usr/bin/xz \
		ROM.bin \
		remove Nb.* \
		remove IntelIsh.* \
	  remove Ip.*  \
    remove Tcp.*  \
    remove Usb.*  \
    remove Udp.*  \
    remove Dhcp.*  \
    remove .np.*  \
    remove .tftp.*  \
    remove Http.* \
    remove .*Dns.* \
    remove Arp.* \
    remove .*NetworkStackSetupScreen.*  \
    remove Iscsi.* \
    remove Scsi.* \
    remove Fat.* \
    remove Ahci.* \
    remove Partition.* \
    remove Sata.* \
    remove Disk.* \
    remove Whea.* \
    remove .*Pxe.* \
    remove Ata.* \
		remove AmiSeri.* \
		remove IntelGop.* \
		remove Logo.* \
		remove Mouse.* \
		remove .*Keyboard.* \
		remove FlashDriver \
		remove HiiDataBase \
		remove EbcDxe \
		remove AMITSE \
		remove AMITSE.* \
		remove Mmc.* \
		remove Tcg.* \
		remove Ish.* \
		remove Setup \
		save image.bin
	utk image.bin table | guid2english | grep Free
	# FIXME: What is this?
	echo remove AmiTc.*
	echo need 0x16b4a0

getrom:
	echo you can put a wget here
	echo and unxip it 
	echo and cp it to sr630.bin

readrom:
	echo You need to get a ROM image from *somewhere*
	echo Once you do, put it in ROM.bin

writerom: image.bin
	echo Here is where you would do the flashrom, e.g.
	echo sudo flashrom -p dediprog -w image.bin
