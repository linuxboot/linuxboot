# linuxboot
The LinuxBoot project allows you to replace your server's firmware with Linux.

Supported server mainboards
===
* qemu emulated Q35 systems
* [Intel S2600WF](https://trmm.net/S2600wf)
* [Dell R630](https://trmm.net/NERF)
* Winterfell Open Compute node (works well)
* Monolake Open Compute node (not tested)

Build instructions
===
You need to provide:
* The vendor UEFI firmware for the mainboard
* A Linux kernel built with the `CONFIG_EFI_BDS` option enabled
* An `initrd.cpio` file with enough tools to `kexec` the rest of the system.

For the `initrd`, the [Heads firmware](http://osresearch.net/) or
[u-root](https://github.com/u-root/u-root) systems work well.
Both will build minimal runtimes that can fit into the few megabytes
of space available.

For everything except qemu, you'll need to copy the vendor ROM dump
to `boards/$(BOARD)/$(BOARD).rom`.  Due to copyright restrictions, we can't
bundle the ROM images in this tree and you must supply your own ROM from
your own machine.  qemu can built its own ROM from the `edk2` tree,
so this is not necessary.

Configure the build system:

    cp path/to/s2600wf.rom boards/s2600wf/
    make \
    	BOARD=s2600wf \
    	KERNEL=../path/to/bzImage \
    	INITRD=../path/to/initrd.cpio.xz \
    	config
    make

This will write the values into the `.config` file so that you don't
need to specify them each time.  If all goes well you will end up with
a file in `build/$(BOARD)/linuxboot.rom` that can be flashed to your machine.
It will take a while since it also clones the LinuxBoot patched version
of [`tianocore/edk2` UDK2018 branch](https://github.com/linuxboot/edk2/tree/UDK2018)
and build it.


Emulating with qemu
===

If you want to experiment with LinuxBoot you can run it under qemu.
No ROM file is necessary, although you still need a Heads or NERF runtime
kernel/initrd pair.  You can launch the emulator by running:

    make run

This will use your current terminal as the serial console, which
will likely mess with the settings.  After killing qemu by closing
the window you will need to run `stty sane` to restore the terminal
settings (echo is likely turned off, so you'll have to type this in
the blind).


Adding a new mainboard
===

Copy `Makefile.board` from one of the other mainboards and edit it to match
your new board's ROM layout.  The qemu one is not the best example since it has
to match the complex layout of OVMF; most real mainboards are not this messy.

You'll need to figure out which FVs have to be preserved, how much space
can be recovered from the ME region, etc.  The per-board makefile needs
to set the following variables:

* `FVS`: an ordered list of IFD, firmware volumes and padding
* `linuxboot-size`: the final size of the ROM image in bytes (we should verify this against the real ROM instead)


More info
===
* https://www.linuxboot.org/
* https://trmm.net/LinuxBoot_34c3

