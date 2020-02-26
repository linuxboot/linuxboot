# This is the makefile for a netbootable image and a flashable image.
# The default target depends on netboot and flash
#
# netboot produces a kernel and initramfs for netboot
# The netboot image uses a cpuserver as its init, and you
# can cpu to it to run commands, including flashrom -p internal
# to flash the flash image.
#
# When you have made netboot you can, e.g.,
# sudo cp kernel initramfs.linux_amd64.lzma /var/lib/tftpboot
# Once the node boots, you can cpu to it. Assume it has IP
# a.b.c.d:
# cpu a.b.c.d 
# will get you a bash, and
# cpu a.b.c.d flashrom -r file -o internal
# will read the file into 'file' in your current directory
# All your files and programs from your host node are visible
# to you in the remote node.
#
# flash produces a flash image to be burned.
# flash also produces a kernel and initramfs that can be
# tested independent of the flash image.
#
# You can test the netboot kernel with qemu
# make flashtest
#
# Or you can netboot the flash kernel
# e.g.
# sudo cp flashinitramfs.cpio.lzma /var/lib/tftpboot/initramfs.linux_amd64.cpio.lzma
# sudo cp tinykernel /var/lib/tftpboot/kernel
#
# This makefile needs a lot of cleanup, but if you stick with the first 4 targets
# a lot of the kludgery won't be so visible.

default: netboot flash reflash

netboot: kernel

flash: tiny.bin

reflash: biguroot.cpio.lzma

flashtest: testtinykernel

#### here there be dragons

build:
	echo fetch, uroot, kernel, or image.bin

tiny.bin: edk2.bin tinykernel flashinitramfs.cpio.lzma
	utk \
		-xzPath  /usr/bin/xz \
		$< \
		replace_pe32 Shell tinykernel \
		save $@
		dd if=flashinitramfs.cpio.lzma of=$@ bs=4096 seek=360 conv=notrunc

edk2.bin: dxeremove.bin
	echo Note, this inserts the edk2 bds and shell
	echo Only flash this image if you have an sf100.
	echo It will not be able to boot anything.
	utk \
		-xzPath  /usr/bin/xz \
		$< \
		insert_dxe edk2bdsdxe.ffs insert_dxe edk2shell.ffs \
		save $@

dxeremove.bin: hap.bin
	echo note, we remove DXE to the point the image is not viable
	echo until you put a kernel in it in place of the shell.
	utk $< \
	comment FlashDriver_do_not_remove_efi_panics  \
	comment S3Save_do_not_remove_gets_error \
	comment testremove \
	comment FlashDriver_do_not_remove_efi_panics  \
	comment S3Save_do_not_remove_gets_error \
	comment testremove \
	remove Ahci.* \
	remove AMITSE \
	remove AMITSE.* \
	remove Arp.* \
	remove Ata.* \
	remove Bds \
	remove Button.* \
	remove CryptoD.* \
	remove .*Dhcp.* \
	remove Dhcp6Dxe \
	remove Disk.* \
	remove EbcDxe \
	remove Fat.* \
	remove .*ftp.* \
	remove .*harging.* \
	remove I2c.* \
	remove IntelGopV.* \
	remove IntelIsh.* \
	remove Ip4.* \
	remove Ip6Dxe \
	remove Iscsi.* \
	remove Ish.* \
	remove .*Keyboard.* \
	remove Logo.* \
	remove Maxim.* \
	remove Mmc.* \
	remove MnpDxe \
	remove Mouse.* \
	remove Mtftp4Dxe \
	remove Mtftp6Dxe \
	remove NetworkStackSetupScreen \
	remove Partition.* \
	remove .*PxeDxe.* \
	remove RandomNumberGen \
	remove ReFlash \
	remove Sata.* \
	remove Scsi.* \
	remove SnpDxe \
	remove SoftKbd \
	remove Tcp.* \
	remove TcpDxe \
	remove TimestampDxe \
	remove Udp.* \
	remove Udp4.* \
	remove Udp6Dxe \
	remove UefiPxe.* \
	remove Uhcd \
	remove Usb.* \
	remove Whea.* \
	remove Xpower.* \
	comment S3Save_do_not_remove_gets_error \
		comment below_still_unknown \
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
		chmod a-w $@

hap.bin: tighten.bin
	cp $< $@
	me_cleaner.py -s $@
	chmod a-w $@

tighten.bin: image.bin
	utk $< tighten_me save $@

kernel: biguroot.cpio.lzma linuxboot-linux.config
	cp linuxboot-linux.config linux/.config
	(cd linux && make oldconfig && make -j32)
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

readrom:
	echo You need to get a ROM image from *somewhere*
	echo Once you do, put it in ROM.bin

writerom: image.bin
	echo Here is where you would do the flashrom, e.g.
	echo sudo flashrom -p dediprog -w image.bin

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

uroot.lzma: uroot
	lzma -f -k initramfs.linux_amd64.cpio

# this target builds an initramfs with only one program, the cpu server.
# It depends on the kernel setting the IP address.
# You need ip=dhcp OR ip=fixed.ip.address.here in the command line
uroot:
	go run github.com/u-root/u-root -o initramfs.linux_amd64.cpio -build=bb -initcmd=cpuserver -files ~/.ssh/cpu_rsa.pub:key.pub \
		-defaultsh="" \
		github.com/u-root/cpu/cmds/cpuserver

# this target builds an initramfs with all of u-root, AND all of /bin, /usr/bin, /sbin, /usr/sbin, /etc
# the intent is  to demonstrate the capability of http loading.
# It depends on the kernel setting the IP address.
# You need ip=dhcp OR ip=fixed.ip.address.here in the command line
biguroot.cpio.lzma: 
	go run github.com/u-root/u-root -o biguroot.cpio -build=bb \
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
	lzma -f -k biguroot.cpio
	ls -l biguroot.cpio.*

netbootkernel: linuxboot-linux.netboot.config
	cp $< linux/.config
	(cd linux && make oldconfig && make -j32)
	cp linux/arch/x86/boot/bzImage $@

tinykernel: linuxboot-linux.tiny.config flashinitramfs.cpio.lzma Makefile
	cp $< linux/.config
	echo CONFIG_CMDLINE_BOOL=y >> linux/.config
	echo CONFIG_CMDLINE_OVERRIDE=y >> linux/.config
	stat -c 'CONFIG_CMDLINE="noefi ip=dhcp earlyprintk=ttyS0,115200,keep console=ttyS0,115200 netconsole=6666@192.168.0.2/,6666@192.168.0.1/ initrd=0xff968000,%s"' flashinitramfs.cpio.lzma >> linux/.config
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

# These stanzas fetch code.
fetch: getkernel geturoot getfiano getrom

getkernel:
	rm -rf linux
	git clone --depth=1 -b working_hack --single-branch https://github.com/linuxboot/linux

getfiano:
	go get -u github.com/linuxboot/fiano/cmds/utk
	go install github.com/linuxboot/fiano/cmds/utk
	go install github.com/linuxboot/fiano/cmds/guid2english
getrom:
	echo you can put a wget here
	echo and unxip it 
	echo and cp it to image.bin
geturoot:
	go get -u github.com/u-root/u-root
	go get -u github.com/u-root/cpu/...

pxeserver:
	echo Disabling any system services ...
	sudo systemctl  stop tftpd-hpa.service
	sudo systemctl stop isc-dhcp-server
	sudo `go env GOPATH`/bin/pxeserver  -tftp-dir . -bootfilename grubnetx64.efi.signed -http-dir . -interface $(NETWORK)
ssh:
	ssh -i class_key -p 2022 root@up
