default: build

build:
	echo fetch, uroot, kernel, or image.bin

xxx.bin: imageinitramfs.bin
	utk \
		  -xzPath  /usr/bin/xz \
		$< \
                remove Iscsi.* \
                remove Scsi.* \
                remove Fat.* \
                remove Ahci.* \
                remove Partition.* \
                remove Sata.* \
                remove Disk.* \
                remove Whea.* \
                remove Ata.* \
		remove Mouse.* \
		remove .*Keyboard.* \
		remove EbcDxe \
		remove Ish.* \
		remove IntelIsh.* \
	save $@
	utk $@ table | guid2english | grep Free
	echo -n need 
	stat -c %s kernel

dxeremove.bin: imageinitramfs.bin
	utk \
		  -xzPath  /usr/bin/xz \
		$< \
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
	save $@
	utk $@ table | guid2english | grep Free
	echo \
		remove AmiTc.* \
	echo need
	stat -c %s kernel
	echo bytes

imagex.bin: image.bin
	utk image.bin tighten_me save imagex.bin
	me_cleaner.py -s imagex.bin
	@echo flash, for example cpu u8 flashrom -w imagex.bin -p internal

imageinitramfs.bin: imagex.bin
	cp imagex.bin $@
	dd if=initramfs.linux_amd64.cpio.lzma of=$@ bs=4096 seek=360 conv=notrunc

kernel: uroot bzImage
	cp linux/arch/x86/boot/bzImage kernel

install:
	sudo mkdir -p /var/lib/tftpboot/grub
	sudo cp grub.cfg /var/lib/tftpboot/grub
	sudo cp grubnetx64.efi.signed /var/lib/tftpboot
	lzma -k -f initramfs.linux_amd64.cpio
	sudo cp kernel initramfs.linux_amd64.cpio.lzma /var/lib/tftpboot
	@echo you can put dhcpd.conf in /etc/dhcp
	@echo but check what is there first.
	@echo you may want to copy 50-cloud-init.yaml to /etc/netplan

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
	echo CONFIG_CMDLINE_BOOL=y >> linux/.config
	echo CONFIG_CMDLINE_OVERRIDE=y >> linux/.config
	stat -c 'CONFIG_CMDLINE="ip=dhcp earlyprintk=ttyS0,115200 netconsole=6666@192.168.0.2/,6666@192.168.0.1/ initrd=0xff968000,%s"' kernel >> linux/.config
	(cd linux && make oldconfig && make -j32)

stablebzImage:
	echo the config is from https://github.com/emutex/ubilinux-kernel/issues/2#issue-294853930
	cp ts.config.txt linux/.config
	(cd linux && make oldconfig && make -j32)

fetch: getkernel geturoot getfiano getrom

getkernel:
	rm -rf linux
	git clone --depth=1 -b linuxboot --single-branch git@github.com:linuxboot/linux

getstablekernel:
	wget -O kernel.xz https://cdn.kernel.org/pub/linux/kernel/v5.x/linux-5.3.10.tar.xz
	xzcat kernel.xz | tar x
	mv linux-5.3.10 linux

upgetkernel:
	rm -rf linux
	git clone --depth=1 https://github.com/emutex/ubilinux-kernel.git -b upboard-4.9 linux

getfiano:
	go get github.com/linuxboot/fiano/cmds/utk
	go install github.com/linuxboot/fiano/cmds/utk
getrom:
	echo you can put a wget here
	echo and unxip it 
	echo and cp it to sr630.bin
geturoot:
	go get github.com/u-root/u-root

