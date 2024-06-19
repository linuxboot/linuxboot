# This is the makefile for a flash image, a netbootable sshd image,
# and a netbootable cpu image.
#
# sshdkernel produces a kernel with a built-in initramfs for netboot
# It provides (and starts) an sshd, and also has flashrom.
# Hence you can ssh to a node, flashrom -r on it, scp the
# image back, process it, scp the new image to the node, and
# write with flashrom.
# e.g.
# ssh -i class_key -p 2022 board flashrom -r image.bin -p internal
# scp -i class_key -P 2022 board:/image.bin image.bin
# make flashpxeboot.bin
# scp -i class_key -P 2022 flashpxeboot.bin board:/tiny.bin
# ssh -i class_key -p 2022 board flashrom -w flashpxeboot.bin -p internal
#
# The cpu image uses a cpud as its init, and you
# can cpu to it to run commands, including flashrom -p internal
# to flash the flash image.
# e.g.
# cpu board flashrom -r image.bin -p internal
# Note this writes image image.bin to the host directoy; no scp needed
# make flashpxeboot.bin
# cpu board flashrom -w flashpxeboot.bin -p internal
# cpu is way more convenient than ssh/scp if you can learn it.
# you can also just
# cpu board
# and once you are in:
# flashrom -r image.bin -p internal
# make flashpxeboot.bin
# flashrom -w flashpxeboot.bin -p internal
# NOTE: those commands are running on the board, and they all work
#
# flashpxeboot.bin produces a flash image which will pxeboot.
# cpu.bin produces a flash image that starts a cpu daemon.
#
# You can test the flash image kernel with qemu
# make flashtest

default: flash

flash: flashpxeboot.bin

redgreen:
	utk dxeremove.bin table | guid2english |grep File | grep -v PAD | grep -v PEIM | grep -v RAW | grep -v IMAGE | grep -v Sec | awk '{print $$2","$$3",red"}' | sort > red.csv
	utk image.bin table | guid2english |grep File | grep -v PAD | grep -v PEIM | grep -v RAW | grep -v IMAGE | grep -v Sec | awk '{print $$2","$$3",green"}' | sort > green.csv
	join -a 2 -t , -j1 red.csv green.csv | sed 's/red.*green/red/' > jj.csv

flashtest: testflashkernel

usefultargets:
	echo fetch, uroot, kernel, or image.bin

flashpxeboot.bin: dxeremove.bin flashkernel flashinitramfs.cpio.lzma
	utk \
		-xzPath  /usr/bin/xz \
		$< \
		replace_pe32 7C04A583-9E3E-4F1C-AD65-E05268D0B4D1 flashkernel \
		save $@

cpu.bin: dxeremove.bin cpukernel
	utk \
		-xzPath  /usr/bin/xz \
		$< \
		replace_pe32 7C04A583-9E3E-4F1C-AD65-E05268D0B4D1 cpukernel \
		save $@

edk2.bin: dxeremove.bin
	echo Note, this inserts the edk2 bds and shell
	echo Only flash this image if you have an sf100.
	echo It will not be able to boot anything.
	utk \
		-xzPath  /usr/bin/xz \
		$< \
		insert_dxe edk2bdsdxe.ffs insert_dxe edk2shell.ffs \
		save $@

dxeremove.bin: image.bin Makefile
	rm -f dxeremove.bin
	echo tighten.bin did not go well.
	echo note, we remove DXE to the point the image is not viable
	echo until you put a kernel in it in place of the shell.
	utk $< \
	comment FlashDriver_do_not_remove_efi_panics  \
	comment S3Save_do_not_remove_gets_error \
	comment testremove \
	comment FlashDriver_do_not_remove_efi_panics  \
	comment S3Save_do_not_remove_gets_error \
	comment leavefornow \
	comment AMITSE \
	comment AMITSE.* \
	comment Bds \
	comment testremove \
	remove Ahci.* \
	remove Arp.* \
	remove Ata.* \
	remove Button.* \
	remove CryptoD.* \
	remove .*Dhcp.* \
	remove Dhcp6Dxe \
	remove Disk.* \
	remove EbcDxe \
	remove Fat.* \
	remove .*ftp.* \
	remove .*harging.* \
	remove .*Http.* \
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
	remove .*HotPl.* \
	comment try \
	remove .*Tcg.* \
	remove .*Tpm.* \
	remove Tls.* \
	comment BiosVideo.* \
	comment .*BootMan.* \
	remove .*Capsule.* \
	remove Capsule.* \
	comment Terminal.* \
	remove Dns.* \
	remove FirmwarePerformanceDxe \
	remove Hsti.* \
	comment fails \
	comment Legacy82.* \
	comment GenericMemoryTest.* \
	comment BiosG.* \
	comment end_fails \
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

hap.bin: tighten.bin
	cp $< $@
	me_cleaner.py -s $@
	chmod a-w $@

tighten.bin: image.bin
	utk $< tighten_me save $@

sshdkernel: sshd.cpio.lzma sshd.config
	cp sshd.config linux/.config
	(cd linux && make oldconfig && make -j32)
	cp linux/arch/x86/boot/bzImage $@

cpukernel: cpu.cpio.lzma cpu.config
	cp cpu.config linux/.config
	(cd linux && make oldconfig && make -j32)
	cp linux/arch/x86/boot/bzImage $@

cpuflashkernel: cpu.cpio.lzma cpuflash.config
	cp cpuflash.config linux/.config
	(cd linux && make oldconfig && make -j32)
	cp linux/arch/x86/boot/bzImage $@

readrom:
	echo You need to get a ROM image from *somewhere*
	echo Once you do, put it in ROM.bin

writerom: image.bin
	echo Here is where you would do the flashrom, e.g.
	echo sudo flashrom -p dediprog -w image.bin

flashinitramfs.cpio.lzma: flashinitramfs.cpio
	lzma -f -k $<

flashinitramfs.cpio: Makefile
		u-root  -o $@ \
		-defaultsh="/bbin/rush" \
		github.com/u-root/u-root/cmds/boot/boot \
		github.com/u-root/u-root/cmds/core/cat \
		github.com/u-root/u-root/cmds/exp/rush \
		github.com/u-root/u-root/cmds/core/init \
		github.com/u-root/u-root/cmds/core/ip \
		github.com/u-root/u-root/cmds/core/ls \
		github.com/u-root/u-root/cmds/core/mount \
		github.com/u-root/u-root/cmds/core/pci \

# this target builds an initramfs with only one program, the cpu server.
# It depends on the kernel setting the IP address.
# You need ip=dhcp OR ip=fixed.ip.address.here in the command line
cpu.cpio.lzma: Makefile
	u-root -o cpu.cpio -build=bb -initcmd=cpud -files ~/.ssh/cpu_rsa.pub:key.pub \
		-defaultsh="" \
		github.com/u-root/cpu/cmds/cpud
	lzma -f -k cpu.cpio

# this target builds an initramfs with all of u-root, AND all of /bin, /usr/bin, /sbin, /usr/sbin, /etc
# the intent is  to demonstrate the capability of http loading.
# It depends on the kernel setting the IP address.
# You need ip=dhcp OR ip=fixed.ip.address.here in the command line
sshd.cpio.lzma:  Makefile
	u-root -o sshd.cpio -build=bb \
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
fetch: getkernel geturoot getfiano getrom

getkernel:
	rm -rf linux
	git clone --depth=1 -b v5.8 --single-branch https://github.com/torvalds/linux

getfiano:
	go install github.com/linuxboot/fiano/cmds/utk@latest
	go install github.com/linuxboot/fiano/cmds/guid2english@latest
getrom:
	echo you can put a wget here
	echo and unzip it
	echo and cp it to image.bin
geturoot:
	go install github.com/u-root/u-root@latest
	go install github.com/u-root/cpu/...@latest

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
