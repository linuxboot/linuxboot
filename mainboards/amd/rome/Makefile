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
# make tiny.bin
# scp -i class_key -P 2022 tiny.bin board:/tiny.bin
# ssh -i class_key -p 2022 board flashrom -w tiny.bin -p internal
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

# sshdkernel and cpukernel are very similar,
# so doing builds in this order minimizes kernel
# change between them.
default: flash sshdkernel cpukernel

flash: tiny.bin

flashtest: testflashkernel

usefultargets:
	echo fetch, uroot, kernel, or image.bin

tiny.bin: dxeremove.bin flashkernel flashinitramfs.cpio.lzma
	utk \
		-xzPath  /usr/bin/xz \
		$< \
		replace_pe32 FullShell flashkernel \
		save $@
	echo DO NOT  \
		dd if=flashinitramfs.cpio.lzma of=$@ bs=4096 seek=2560 conv=notrunc

dxeremove.bin: image.bin Makefile
	echo note, we remove DXE to the point the image is not viable
	echo until you put a kernel in it in place of the shell.
	utk $< \
	remove Ahci.* \
	remove AMITSE \
	remove AMITSE.* \
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
	remove I2c.* \
	remove IntelGopV.* \
	remove IntelIsh.* \
	remove Ip4.* \
	remove Ip.* \
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
	remove Shell_Full \
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
	remove DxeIpm.* \
	remove .*Tcg.* \
	remove NvmeInt13 \
	remove Aint13 \
	remove LegacyRegion \
	remove xGbEI2cMasterDxe \
	remove NvmeSmm \
	remove KbcEmul \
	remove IdeBusBoard \
	remove Setup \
	remove SerialMuxControl \
	remove BmcLanConfig \
	remove SecureBootDXE \
	remove SmmHddSecurity \
	remove HttpBootDxe \
	remove HttpDxe \
	remove DnsDxe \
	remove HttpUtilitiesDxe \
	remove TlsDxe \
	remove CsmBlockIo \
	remove CsmVideo \
	comment testing \
	comment huashan_shock_and_awe_undo_later \
	remove FchHuashanMultiFchSmm \
	remove FchHuashanSmmInit \
	remove FchHuashanMultiFchDxe \
	comment FchHuashanDxe \
	comment end_huashan_shock_and_awe \
	comment misc_shock_and_awe \
	comment end_misc_shock_and_awe \
	comment debug_shock_and_awe \
	remove DebugPortTableDxe \
	remove AmdIdsDebugPrintDxe \
	comment end_debug_shock_and_awe \
	comment config_shock_and_awe \
	remove AmdFwConfigSmmSsp \
	remove TlsAuthConfigDxe \
	remove AmdFwConfigDxe \
	comment end_config_shock_and_awe \
	comment smm_shock_and_awe \
	comment remove_all_smm_it_warm_resets_continuously \
	comment StatusCodeSmm \
	comment RuntimeSmm \
	comment SmmPciRbIo \
	comment LegacySmmSredir \
	comment SmmS3SaveState \
	comment SmmBmcElog \
	comment SmmIpmiBmcInitialize \
	comment CpuSmm \
	comment AmdCpmInitSmm \
	comment AmdPlatformJedecNvdimmSmm \
	comment AmdHotPlugSspSmm \
	remove AmdPlatformRasSspSmm \
	remove AmdPlatformRasZpSmm \
	comment AmdSpiLockSmm \
	comment AmdCpmGpioInitSmm \
	comment BctBaseSmmSSP \
	remove AmdRasSspSmm \
	comment AmdSmmCommunication \
	comment FchSmmDispatcher \
	comment AmdFabricSspSmm \
	comment AmdCcxVhSmm \
	comment AmdPspSmmV2Ssp \
	comment AmdPspP2CmboxV2SspSmmBuffer \
	comment AmdApcbSmmV3 \
	comment AmdMemPprSmmDriverSsp \
	comment PiSmmCommunicationSmm \
	comment SmmChildDispatcher2 \
	comment SbRunSmm \
	comment SmmLockBox \
	comment CpuIo2Smm \
	comment NvramSmm \
	comment FlashDriverSmm \
	comment PiSmmCore \
	comment AmdSmmControl \
	comment PiSmmIpl \
	comment SmBusSmm \
		comment TcgSmm \
	comment end_smm_shock_and_awe \
	comment tpm_shock_and_awe \
	remove TpmClearOnRollbackSmm \
	remove Tpm20Acpi \
	remove Tpm20PlatformDxe \
	remove CrbTpm20Acpi \
	remove Tpm20PlatformDxe \
	remove TpmSmbiosDxe \
	remove end_tpm_shock_and_awe \
	comment smbios_shock_and_awe \
	remove SmmSmbiosElog \
	remove Smbios \
	remove SmbiosDmiEdit \
	remove SmbiosElog \
	remove SmbiosBoard \
	remove SmbiosElogSetupScreen \
	remove AmdSmbiosDxe \
	remove AmdMemSmbiosV2RvDxe \
	remove CpuSmbiosDriver \
	comment end_smbios_shock_and_awe \
	comment S3Save_do_not_comment_gets_error \
		comment below_still_unknown \
		comment TcgMor \
		comment AmiTcgNvflagSample \
		comment AmiTcgPlatformDxe \
		comment TcgDxeplatform \
		comment TcgDxe \
		comment Tcg2Dxe \
		comment TcgPlatformSetupPolicy \
	comment FlashDriver_do_not_remove_efi_panics  \
	comment S3Save_do_not_remove_gets_error \
	comment testremove \
	comment FlashDriver_do_not_remove_efi_panics  \
	comment S3Save_do_not_remove_gets_error \
	comment testremove \
	comment RomLayoutDxe \
	comment DxeCore \
	comment DataHubDxe \
	comment DevicePathDxe \
	comment EnglishDxe \
	comment HiiDatabase \
	comment SecurityStubDxe \
	comment CpuIo2Dxe \
	comment CpuDxe \
	comment NbDxe \
	comment SbDxe \
	comment AmdMemSspSp3Dxe \
	comment AmdMemRestoreDxe \
	comment AmdApcbDxeV3 \
	comment AmdPspDxeV2Ssp \
	comment AmdHstiV2 \
	comment ApobSspDxe \
	comment AmdCcxVhDxe \
	comment AmdFabricSspDxe \
	comment AmdSocSp3RmDxe \
	comment AmdErrorLogDxe \
	comment AmdNbioAlibDxe \
	comment AmdNbioBaseSspDxe \
	comment SmuV11Dxe \
	comment AmdNbioPcieDxe \
	comment AmdNbioIOMMUDxe \
	comment FchSmbusDxe \
	comment AmdLegacyInterrupt \
	comment AmdAcpiDxe \
	comment AmdVersionDxe \
	remove AmdRasSspApeiDxe \
	remove AmdRasSspDxe \
	comment JedecNvdimmDxe \
	comment CbsSetupDxeSSP \
	comment CbsBaseDxeSSP \
	comment AmdPbsSetupDxe \
	remove RasOemDimmMap \
	comment ServerHotplugDxe \
	comment Pca9535aDxe \
	comment Pca9545aDxe \
	comment Sff8472Dxe \
	comment SlinkManager \
	comment SlinkEndpointDriver \
	comment AmdBoardIdDxe \
	comment AmdCpmGpioInitDxe \
	comment AmdSpiLockDxe \
	comment AmdI2cMasterDxe \
	comment AmdCpmPcieInitDxe \
	comment ApicInfoDataDxe \
	remove AmdPlatformRasZpDxe \
	remove AmdPlatformRasSspDxe \
	comment AmdCpmInitDxe \
	comment ServerMgmtSetup \
	comment ACPI \
	comment S3SaveStateDxe \
	comment BootScriptExecutorDxe \
	comment AcpiS3SaveDxe \
	comment LegacySredir \
	comment OA3 \
	comment PciRootBridge \
	comment PciDynamicSetup \
	comment PciDxeInit \
	comment ExportHiiDb \
	comment DpcDxe \
	comment CsmDxe \
	comment EventLogsSetupPage \
	comment HardwareSignatureEntry \
	comment HddSecurity \
	comment KbcEmulDxe \
	comment EsrtDxe \
	comment NvmeDynamicSetup \
	comment AmiBoardInfo2 \
	comment AmiSerialUart0 \
	comment AmiSerialUart1 \
	comment OpalSecurity \
	comment CsmRt32 \
	comment RsdpPlus \
	comment AmiRedFishApi \
	comment AmiDeviceGuardApi \
	comment PspPlatform \
	comment PspDxe \
	comment AcpiUpdateCpuInfo \
	comment CheckCrbVsRom \
	comment CapsuleRuntimeDxe \
	comment RuntimeDxe \
	comment CpuIoDxe \
	comment SbRun \
	comment Cf9Reset \
	comment BmcElog \
	comment SecFlashUpdDXE \
	comment SmiFlashDxe \
	comment PCIePTCPkg \
	comment ConSplitter \
	comment GraphicsConsole \
	comment SlinkRootBridgeDriver \
	remove IdeBusSrc \
	remove PciBus \
	remove PlatformToDriver \
	remove TerminalSrc \
	remove SerialIo \
	remove Nvme \
	comment ResetSmi \
	comment PeriodicSmiControl \
	remove PowerButton \
	comment AcpiModeEnable \
	comment AmdPspP2CmboxV2Ssp \
	remove JedecNvdimm \
	remove BiosCfgTool \
	remove CryptoSMM \
	remove BmcAcpi \
	comment OA3_SMM \
	comment SmiVariable \
	comment Ofbd \
	remove SecSMIFlash \
	remove SmiFlash \
	comment StatusCodeDxe \
	comment FlashDriver \
	comment NvramDxe \
	comment PcdDxe \
	remove SbCmos \
	remove SmBusDxe \
	remove DxeSelStatusCode \
	comment DxeFrb \
	remove CmosDxe \
	comment CrbSpiOpCode \
	comment tested_fails_if_removed \
	comment Bds \
	comment needed_for_linuxboot_dont_remove \
	comment FullShell \
		save $@

sshdkernel: sshd.cpio.lzma sshd-linux.config
	cp sshd-linux.config linux/.config
	(cd linux && make oldconfig && make -j32)
	cp linux/arch/x86/boot/bzImage $@

cpukernel: cpu.cpio.lzma cpu-linux.config
	cp cpu-linux.config linux/.config
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
		-uinitcmd=/bbin/console \
		github.com/u-root/u-root/cmds/boot/pxeboot \
		github.com/u-root/u-root/cmds/boot/boot \
		github.com/u-root/u-root/cmds/core/cat \
		github.com/u-root/u-root/cmds/core/elvish \
		github.com/u-root/u-root/cmds/core/init \
		github.com/u-root/u-root/cmds/core/ip \
		github.com/u-root/u-root/cmds/core/ls \
		github.com/u-root/u-root/cmds/core/kexec \
		github.com/u-root/u-root/cmds/core/pci \
		github.com/u-root/u-root/cmds/core/wget \
		github.com/u-root/u-root/cmds/exp/console \

oflashinitramfs.cpio: Makefile
	u-root  -o $@ -build=bb \
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

flashkernel: flashkernel.config flashinitramfs.cpio.lzma Makefile
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
	git clone --depth=1 -b v5.5-rc3 --single-branch https://github.com/torvalds/linux

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
