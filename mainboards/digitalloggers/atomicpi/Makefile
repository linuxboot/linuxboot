# TODO: clean up
# There are lots of targets in here and it's confusing.
# They are artifacts of figuring out what works. 
# Eventually, there will be two targets: netboot and flash
#
# netboot produces a kernel and initramfs for netboot
# The netboot image uses a cpuserver as its init, and you
# can cpu to it to run commands, including flashrom -p internal
# to flash the flash image.
#
# flash produces a flash image to be burned.
# flash also produces a kernel and initramfs that can be
# netbooted for testing purposes.
#
# Confusing, but with luck, we can clean up this
# Makefile to make it less so.
# For now
# make kernel # kernel is netbootable, initramfs.linux_amd64.cpio.lzma is cpu ramfs
# make tiny.bin # ram image but tinykernel and flashinitramfs.cpio.lzma can be netbooted
default: build

build:
	echo fetch, uroot, kernel, or image.bin

lbur.bin: upimagewithup2dxeremovemore.bin flashbzImage Makefile
	utk \
		  -xzPath  /usr/bin/xz \
		$< \
		replace_pe32 Shell_Full linux/arch/x86/boot/bzImage \
		save $@

upimagewithup2dxeremovemore.bin: upimagewithup2dxe.bin uroot.lzma Makefile
	utk \
		  -xzPath  /usr/bin/xz \
		$< \
		remove IntelGopV.* \
		remove Ip4.* \
		remove .*harging.* \
		remove TcpDxe \
		remove SnpDxe \
		remove MnpDxe \
		remove .*Dhcp.* \
		remove Arp.* \
		remove Tcp.* \
		remove Maxim.* \
		remove Xpower.* \
		comment FlashDriver_do_not_remove_efi_panics  \
		remove SoftKbd \
		remove ReFlash \
		remove AMITSE.* \
		remove UefiPxe.* \
		remove Usb.* \
		remove .*ftp.* \
		remove Udp.* \
		remove Uhcd \
		remove CryptoD.* \
		remove Button.* \
		remove Mmc.* \
		comment S3Save_do_not_remove_gets_error \
		remove I2c.* \
		remove NetworkStackSetupScreen \
		remove RandomNumberGen \
		remove TimestampDxe \
		tighten_me \
		comment testremove \
		comment TcgMor \
		comment AmiTcgNvflagSample \
		comment AmiTcgPlatformDxe \
		comment TcgDxeplatform \
		comment Tpm20PlatformDxe \
		comment CrbTpm20Acpi \
		comment TcgSmm \
		comment TcgDxe \
		comment Tcg2Dxe \
		comment TcgPlatformSetupPolicy \
		save $@
		dd if=initramfs.linux_amd64.cpio.lzma of=$@ bs=4096 seek=360 conv=notrunc

tiny.bin: upimagewithup2dxeremovemore.bin tinykernel
	utk \
		-xzPath  /usr/bin/xz \
		$< \
		remove .*harging.* \
		remove TcpDxe \
		remove SnpDxe \
		remove MnpDxe \
		remove .*Dhcp.* \
		remove Arp.* \
		remove Tcp.* \
		remove Ip4.* \
		remove .*PxeDxe.* \
		remove Mtftp4Dxe \
		remove Udp4.* \
		replace_pe32 Shell_Full tinykernel \
		save $@
		dd if=flashinitramfs.cpio.lzma of=$@ bs=4096 seek=360 conv=notrunc

netboot.bin: upimagewithup2dxe.bin netbootkernel
	utk \
		-xzPath  /usr/bin/xz \
		$< \
		remove Ip4.* \
		remove .*PxeDxe.* \
		remove Mtftp4Dxe \
		remove Udp4.* \
		remove Ish.* \
		remove IntelIsh.* \
		remove IntelGop.* \
		replace_pe32 Shell_Full netbootkernel \
		save $@

upimagewithup2dxe.bin: image.bin Makefile
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
		remove Logo.* \
		remove Ip6Dxe \
		remove Udp6Dxe \
		remove Dhcp6Dxe \
		remove Mtftp6Dxe \
		remove Bds \
		remove AMITSE \
		remove AMITSE.* \
	save /tmp/$@
	utk /tmp/$@ table | guid2english | grep Free
	utk \
		  -xzPath  /usr/bin/xz \
		/tmp/$@ \
		insert_dxe up2bds.ffs insert_dxe up2shellfull.ffs \
		save $@

flashkernel.bin: pxeshell.bin flashkernel Makefile
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
		remove Logo.* \
		remove Ip6Dxe \
		remove Udp6Dxe \
		remove Dhcp6Dxe \
		remove Mtftp6Dxe \
	save /tmp/$@
	utk /tmp/$@ table | guid2english | grep Free
	echo -n need 
	stat -c %s Shell.efi
	utk \
		  -xzPath  /usr/bin/xz \
		/tmp/$@ \
		replace_pe32 UefiPxeBcDxe flashkernel \
		save $@

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
		remove IntelGop.* \
		remove Logo.* \
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
	rm -f $@
	cp imagex.bin $@
	chmod +w $@
	dd if=initramfs.linux_amd64.cpio.lzma of=$@ bs=4096 seek=360 conv=notrunc
	chmod a-w $@

kernel: uroot.lzma bzImage
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

flashinitramfs.cpio.lzma: flashinitramfs.cpio
	lzma -f -k $<

flashinitramfs.cpio: Makefile
	go run github.com/u-root/u-root  -o $@ -build=bb -files /usr/bin/strace \
		github.com/u-root/u-root/cmds/core/cat \
		github.com/u-root/u-root/cmds/core/elvish \
		github.com/u-root/u-root/cmds/core/init \
		github.com/u-root/u-root/cmds/core/ip \
		github.com/u-root/u-root/cmds/core/ls \
		github.com/u-root/u-root/cmds/core/kexec \
		github.com/u-root/u-root/cmds/core/pci \
		github.com/u-root/u-root/cmds/core/wget \
		github.com/u-root/u-root/cmds/boot/pxeboot

uroot.lzma: uroot
	lzma -f -k initramfs.linux_amd64.cpio

# don't bring in the commands for now, it messes up cpu and i don't know why.
uroot:
	go run github.com/u-root/u-root -o initramfs.linux_amd64.cpio -build=bb -initcmd=cpuserver -files ~/.ssh/cpu_rsa.pub:key.pub \
		-defaultsh="" \
		github.com/u-root/cpu/cmds/cpuserver
		echo you can bring these in but it breaks cpu \
		github.com/u-root/u-root/cmds/core/cat \
		github.com/u-root/u-root/cmds/core/dhclient \
		github.com/u-root/u-root/cmds/core/mount \
		github.com/u-root/u-root/cmds/core/ls \
		github.com/u-root/u-root/cmds/core/ip \
		github.com/u-root/u-root/cmds/core/elvish \

alluroot:
	go run github.com/u-root/u-root -o initramfs.linux_amd64.cpio -build=bb -initcmd=cpu -files ~/.ssh/cpu_rsa.pub:key.pub \
	-files /bin/fusermount \
	github.com/u-root/u-root/cmds/exp/cpu \
		all 
	lzma -k -f initramfs.linux_amd64.cpio
	ls -l initramfs*
	cp *lzma linux

netbootkernel: linuxboot-linux.netboot.config
	cp $< linux/.config
	(cd linux && make oldconfig && make -j32)
	cp linux/arch/x86/boot/bzImage $@

tinykernel: linuxboot-linux.tiny.config flashinitramfs.cpio.lzma Makefile
	cp $< linux/.config
	echo CONFIG_CMDLINE_BOOL=y >> linux/.config
	echo CONFIG_CMDLINE_OVERRIDE=y >> linux/.config
	stat -c 'CONFIG_CMDLINE="noefi ip=dhcp earlyprintk=ttyS0,115200,keep console=ttyS0,115200  initrd=0xff968000,%s"' flashinitramfs.cpio.lzma >> linux/.config
	(cd linux && make oldconfig && make -j32)
	cp linux/arch/x86/boot/bzImage $@

testtinykernel: tinykernel
	qemu-system-x86_64 -kernel tinykernel -nographic -serial /dev/tty -initrd flashinitramfs.cpio.lzma

# This image is intended to be net-booted
bzImage: uroot.lzma
	cp linuxboot-linux.config linux/.config
	(cd linux && make oldconfig && make -j32)

# This image is intended to be net-booted but using an initramfs burned into SPI
# It should also be possible to burn it into SPI
flashbzImage: uroot.lzma
	cp linuxboot-linux.config linux/.config
	echo CONFIG_CMDLINE_BOOL=y >> linux/.config
	echo CONFIG_CMDLINE_OVERRIDE=y >> linux/.config
	stat -c 'CONFIG_CMDLINE="noefi ip=dhcp earlyprintk=ttyS0,115200 netconsole=6666@192.168.0.2/,6666@192.168.0.1/ initrd=0xff968000,%s"' initramfs.linux_amd64.cpio.lzma >> linux/.config
	(cd linux && make oldconfig && make -j32)

stablebzImage:
	echo the config is from https://github.com/emutex/ubilinux-kernel/issues/2#issue-294853930
	cp ts.config.txt linux/.config
	(cd linux && make oldconfig && make -j32)

fetch: getkernel geturoot getfiano getrom

getkernel:
	rm -rf linux
	git clone --depth=1 -b working_hack --single-branch https://github.com/linuxboot/linux

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
	go install github.com/linuxboot/fiano/cmds/guid2english
getrom:
	echo you can put a wget here
	echo and unxip it 
	echo and cp it to sr630.bin
geturoot:
	go get -u github.com/u-root/u-root
	go get -u github.com/u-root/cpu/...

