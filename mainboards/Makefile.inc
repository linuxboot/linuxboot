# Common makefile for all boards.
# All rules must be parameterized by kernel version,
# architecture, cross compiler, and so on.

# These stanzas fetch code.
fetch: getkernel geturoot 

# This rule gets the kernel and sets up a default minimal config.
getkernel:
	rm -rf linux
	git clone --depth=1 -b $(LINUXVERSION) --single-branch https://github.com/torvalds/linux
	(cd linux && make mrproper)
	(cd linux && ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) make tinyconfig)

geturoot:
	rm -rf u-root cpu
	git clone --depth 1 https://github.com/u-root/u-root
	git clone --depth 1 https://github.com/u-root/cpu
	rm -f go.*
	go work init ./u-root
	go work use ./cpu

# These stanzas are for setting up services for netboot
	
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
