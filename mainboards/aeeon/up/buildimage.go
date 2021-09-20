package main

import (
	"fmt"
	"io/ioutil"
	"log"
	"os"
	"os/exec"
)

func shell(s string, a ...string) {

	c := exec.Command(s, a...)
	c.Stdout, c.Stderr = os.Stdout, os.Stderr
	if err := c.Run(); err != nil {
		log.Fatal(err)
	}
}

func main() {
	shell("make", "uroot.lzma")
	// Stat the image, add it to .config
	fi, err := os.Stat("initramfs.linux_amd64.cpio.lzma")
	if err != nil {
		log.Fatal(err)
	}
	b, err := ioutil.ReadFile("linuxboot-linux.config")
	if err != nil {
		log.Fatal(err)
	}
	c := []byte(string(b) + fmt.Sprintf("\nCONFIG_CMDLINE_BOOL=y\nCONFIG_CMDLINE=\"earlyprintk=ttyS0,115200,keep console=ttyS0,115200, initrd=0x0xffa7a000,%d\"", fi.Size()))
	if err := ioutil.WriteFile("linux/.config", c, 0644); err != nil {
		log.Fatal(err)
	}
	shell("make", "kernel")
}
