FROM ubuntu:latest


LABEL description="Build environment for Linuxboot mainboards"

# Working directory for mounting git repo in
RUN mkdir /linuxboot-mainboards
WORKDIR /linuxboot-mainboards

# Install dependencies
RUN apt-get update
RUN apt-get install -y \
    gcc-aarch64-linux-gnu \
    gcc-arm-linux-gnueabi \
    gcc-arm-linux-gnueabihf \
    gcc-riscv64-linux-gnu \
    libelf-dev \
    u-boot-tools \
	acpica-tools \
	bc \
	bison \
	build-essential \
	cpio \
	flex \
	kmod \
	libncurses5-dev \
	libssl-dev \
	nasm \
	qemu-system \
	uuid-dev \
	wget \
	git \
	ssh \
	zip

# install golang
RUN wget https://go.dev/dl/go1.21.5.linux-amd64.tar.gz
RUN rm -rf /usr/local/go && \
    tar -C /usr/local -xzf go1.21.5.linux-amd64.tar.gz
ENV GOPATH="/root/go"
ENV GOROOT="/usr/local/go"
ENV PATH="$PATH:/root/go/bin:/usr/local/go/bin"
RUN go env -w GO111MODULE=off


# Fake sshkey
RUN mkdir ~/.ssh && touch ~/.ssh/cpu_rsa.pub
# RUN touch tyan7106/key.pub
# RUN touch tyan7106/ssh_host_rsa_key



ENTRYPOINT ["/bin/bash", "-l", "-c"]