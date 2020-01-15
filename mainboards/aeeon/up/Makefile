default: build

build:
	echo fetch, uroot, kernel, or image.bin

image.bin:  
	utk \
		  -xzPath  /usr/bin/xz \
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
	echo \
		remove AmiTc.* \
	echo need 0x16b4a0

kernel: uroot bzImage
	cp linux/arch/x86/boot/bzImage kernel

netboot: netbooturoot bzImage
	cp linux/arch/x86/boot/bzImage kernel

readrom:
	echo You need to get a ROM image from *somewhere*
	echo Once you do, put it in ROM.bin

writerom: image.bin
	echo Here is where you would do the flashrom, e.g.
	echo sudo flashrom -p dediprog -w image.bin

netbooturoot:
	go run github.com/u-root/u-root  -o linux/initramfs.linux_amd64.cpio -build=bb github.com/u-root/u-root/cmds/core/init github.com/u-root/u-root/cmds/core/ls github.com/u-root/u-root/cmds/core/ip   github.com/u-root/u-root/cmds/core/elvish    github.com/u-root/u-root/cmds/core/sshd    github.com/u-root/u-root/cmds/core/scp github.com/u-root/u-root/cmds/boot/*

uroot.lzma: uroot
	lzma -f -k initramfs.linux_amd64.cpio

uroot:
	go run github.com/u-root/u-root -o initramfs.linux_amd64.cpio -build=bb -initcmd=cpuserver -files ~/.ssh/cpu_rsa.pub:key.pub \
	-defaultsh="" \
	github.com/u-root/cpu/cmds/cpuserver

alluroot:
	go run github.com/u-root/u-root -o initramfs.linux_amd64.cpio -build=bb -initcmd=cpu -files ~/.ssh/cpu_rsa.pub:key.pub \
	-files /bin/fusermount \
	github.com/u-root/u-root/cmds/exp/cpu \
		all 
	lzma -k -f initramfs.linux_amd64.cpio
	ls -l initramfs*
	cp *lzma linux

bzImage:
	cp linuxboot-linux.config linux/.config
	(cd linux && make oldconfig && make -j32)

stablebzImage:
	echo the config is from https://github.com/emutex/ubilinux-kernel/issues/2#issue-294853930
	cp ts.config.txt linux/.config
	(cd linux && make oldconfig && make -j32)

fetch: getkernel geturoot getfiano getrom

getkernel:
	rm -rf linux
	git clone --depth=1 git@github.com:linuxboot/linux

getstablekernel:
	wget -O kernel.xz https://cdn.kernel.org/pub/linux/kernel/v5.x/linux-5.3.10.tar.xz
	xzcat kernel.xz | tar x
	mv linux-5.3.10 linux

upgetkernel:
	rm -rf linux
	git clone --depth=1 https://github.com/emutex/ubilinux-kernel.git -b upboard-4.9 linux

getfiano:
	go get -u github.com/linuxboot/fiano/cmds/utk
	go install github.com/linuxboot/fiano/cmds/utk
getrom:
	echo you can put a wget here
	echo and unxip it 
	echo and cp it to sr630.bin
geturoot:
	go get -u github.com/u-root/u-root

