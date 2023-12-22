CONTAINER_NAME := linuxboot-mainboards

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


docker-build: Dockerfile
	docker build -t $(CONTAINER_NAME) -f $< .

docker-run-aeeon-up: docker-build
	docker run --rm -v "$(shell pwd)/aeeon/up":/$(CONTAINER_NAME) $(CONTAINER_NAME) 'make fetch flashkernel'


docker-run-aeeon-upxtreme: docker-build
	docker run --rm -v "$(shell pwd)/aeeon/upxtreme":/$(CONTAINER_NAME) $(CONTAINER_NAME) 'make fetch flashkernel'

docker-run-bytedance-g220a: docker-build
	docker run --rm -v "$(shell pwd)/bytedance/g220a":/$(CONTAINER_NAME) $(CONTAINER_NAME) 'make fetch flashkernel'

docker-run-digitallogger-atomicpi: docker-build
	docker run --rm -v "$(shell pwd)/digitalloggers/atomicpi":/$(CONTAINER_NAME) $(CONTAINER_NAME) 'make fetch flashkernel'

docker-run-amd-rome: docker-build
	docker run --rm -v "$(shell pwd)/amd/rome":/$(CONTAINER_NAME) $(CONTAINER_NAME) 'make fetch flashkernel'

docker-run-ampere-jade: docker-build
	docker run --rm -v "$(shell pwd)/ampere/jade":/$(CONTAINER_NAME) $(CONTAINER_NAME) 'make fetch flashkernel'

docker-run-cubie-board: docker-build
	docker run --rm -v "$(shell pwd)/cubie/board":/$(CONTAINER_NAME) $(CONTAINER_NAME) 'make fetch flashkernel'

docker-run-intel-hw: docker-build
	docker run --rm -v "$(shell pwd)/intel/hw":/$(CONTAINER_NAME) $(CONTAINER_NAME) 'make fetch flashkernel'

docker-run-intel-s2600: docker-build
	docker run --rm -v "$(shell pwd)/intel/s2600":/$(CONTAINER_NAME) $(CONTAINER_NAME) 'make fetch flashkernel'

docker-run-lenovo-sr630: docker-build
	docker run --rm -v "$(shell pwd)/lenovo/sr630":/$(CONTAINER_NAME) $(CONTAINER_NAME) 'make fetch flashkernel'

docker-run-marvel-macchiatobin: docker-build
	docker run --rm -v "$(shell pwd)/marvel/macchiatobin":/$(CONTAINER_NAME) $(CONTAINER_NAME) 'make fetch flashkernel'

docker-run-opentitanpilot-dresden: docker-build
	docker run --rm -v "$(shell pwd)/opentitanpilot/dresden":/$(CONTAINER_NAME) $(CONTAINER_NAME) 'make fetch flashkernel'

docker-run-pcengines-apu2: docker-build
	docker run --rm -v "$(shell pwd)/pcengines/apu2":/$(CONTAINER_NAME) $(CONTAINER_NAME) 'make fetch flashkernel'

docker-run-qemu-x86-64: docker-build
	docker run --rm -v "$(shell pwd)/qemu/x86_64":/$(CONTAINER_NAME) $(CONTAINER_NAME) 'make fetch flashkernel'