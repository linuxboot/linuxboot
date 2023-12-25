default: build

build:
	echo fetch, uroot, flashkernel, or image.bin

image.bin:  flashkernel coreboot.bin
	cp coreboot.bin image.bin
	chmod +w image.bin
	cbfstool image.bin print
	cbfstool image.bin remove -n img/nerf
	cbfstool image.bin remove -n genroms/pxe.rom
	cbfstool image.bin add-payload -n img/nerf -f flashkernel
	cbfstool image.bin print
qemu: flashkernel
	sudo /usr/bin/qemu-system-x86_64 -kernel flashkernel \
		-cpu  max \
		-s   \
		-m 1024m \
		-machine q35  \
		-object rng-random,filename=/dev/urandom,id=rng0 \
		-device virtio-rng-pci,rng=rng0 \
		-device e1000,netdev=n1 \
		-netdev user,id=n1,hostfwd=tcp:127.0.0.1:23-:2222,net=192.168.1.0/24,host=192.168.1.1 \
		-serial stdio  \
		-append earlyprintk=ttyS0,115200\ console=ttyS0 \
		-monitor /dev/null  \


# add the following line to qemu if you want to snoop packets.
	-object filter-dump,id=f1,netdev=n1,file=/tmp/vm0.pcap \


flashkernel: uroot bzImage
	cp linux/arch/x86/boot/bzImage flashkernel

netbootkernel: netbooturoot bzImage
	cp linux/arch/x86/boot/bzImage kernel

readrom:
	sudo flashrom -p internal -r coreboot.bin

writerom: image.bin
	sudo flashrom -p internal -w image.bin

netbooturoot:
	go run github.com/u-root/u-root -build=bb github.com/u-root/u-root/cmds/core/init github.com/u-root/u-root/cmds/core/ls github.com/u-root/u-root/cmds/core/ip   github.com/u-root/u-root/cmds/core/elvish    github.com/u-root/u-root/cmds/core/sshd    github.com/u-root/u-root/cmds/core/scp github.com/u-root/u-root/cmds/boot/*
	cp /tmp/initramfs.linux_amd64.cpio .
	lzma -k -f initramfs.linux_amd64.cpio
	ls -l initramfs*
	cp *lzma linux

uroot:
	go run github.com/u-root/u-root -initcmd=/bbin/cpu -build=bb -files ~/.ssh/cpu_rsa.pub:key.pub \
	all \
	github.com/u-root/cpu/cmds/cpu 
	cp /tmp/initramfs.linux_amd64.cpio .
	lzma -k -f initramfs.linux_amd64.cpio
	ls -l initramfs*
	cp *lzma linux

bzImage:
	cp config-4.14.111 linux/.config
	(cd linux && make oldconfig && make -j32)

fetch: getkernel geturoot getfiano getrom
getkernel:
	rm -rf linux
	git clone --depth=1 -b v5.15 --single-branch https://github.com/torvalds/linux
getrom:
	echo you create the flashkernel
	echo on the APU2: flashrom -r coreboot.bin -p internal
	echo copy that file here and for safety:
	echo chmod a-w coreboot.bin
geturoot:
	go get -u github.com/u-root/u-root
	go get -u github.com/u-root/cpu/...
getfiano:
	go get -u github.com/linuxboot/fiano/cmds/utk
	go install github.com/linuxboot/fiano/cmds/utk
	go install github.com/linuxboot/fiano/cmds/guid2english
