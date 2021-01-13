// insert an initramfs, checking sizes.

package main

import (
	"io/ioutil"
	"log"
	"os"
	"strconv"
)

func main() {
	// Arguments are simple:
	// image file
	// initramfs file
	// offset in target
	// size limit
	if len(os.Args) != 5 {
		log.Fatalf("Usage: %s image initramfs offset max-allowed-initramfs-size", os.Args[0])
	}

	image, initramfs, offset, size := os.Args[1], os.Args[2], os.Args[3], os.Args[4]

	img, err := ioutil.ReadFile(image)
	if err != nil {
		log.Fatal(err)
	}
	ramfs, err := ioutil.ReadFile(initramfs)
	if err != nil {
		log.Fatal(err)
	}
	off, err := strconv.ParseUint(offset, 0, 24)
	if err != nil {
		log.Fatal(err)
	}
	sz, err := strconv.ParseUint(size, 0, 24)
	if err != nil {
		log.Fatal(err)
	}

	if uint64(len(ramfs)) > sz {
		log.Fatalf("Size of %q (%d) > allowed size (%d)", initramfs, len(ramfs), sz)
	}
	if off > uint64(len(img)) {
		log.Fatal("Off (%d) is > image size (%d)", off, len(img))
	}
	if off+sz > uint64(len(img)) {
		log.Fatal("Off (%d) is > image size (%d)", off, len(img))
	}

	copy(img[off:], ramfs[:])
	log.Printf("Inserted %q, %d bytes actual size, with max allowed size of %d, at offset %d bytes, into %q, %d bytes",
		initramfs, len(ramfs), sz, off, image, len(img))
	if err := ioutil.WriteFile(image, img, 0666); err != nil {
		log.Fatal(err)
	}
}
