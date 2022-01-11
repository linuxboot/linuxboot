all:
	echo pick one of debian-packages or repos

debian-packages:
	sudo apt-get install bison flex zlib1g-dev golang build-essential git gcc  isc-dhcp-server tftpd-hpa minicom flashrom
	echo The next command may get an error and that is ok if you have qemu-system-x86_64
	sudo apt-get install qemu-system-x86  

getmecleaner:
	go get -u github.com/u-root/u-root
	go get -u github.com/u-root/cpu/...
	git clone https://github.com/corna/me_cleaner
	@echo Now add $(PWD)/me_cleaner to PATH
