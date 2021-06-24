CONTAINER_NAME := linuxboot-ovmf

docker-build: Dockerfile
	docker build -t $(CONTAINER_NAME) -f $< .

docker-run:
	docker run --tty --interactive --rm --volume "$(shell pwd)":/$(CONTAINER_NAME) $(CONTAINER_NAME)
