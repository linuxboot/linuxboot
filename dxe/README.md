Overview
===

These are sample EFI Option ROMs that can be installed in Apple's
Thunderbolt Gigabit Ethernet adapter.  Anything that does printouts
to the console device will only be visible if you have set the
`bootargs` NVRAM parameter to verbose mode:

    sudo nvram bootargs=-v

Once you have build the `hello.rom` file, install it with the `b57tool`.
Unlike Broadcom's `B57UDAIG.EXE`, this does not require rebooting to DOS
and works with a hot-plugged Thunderbolt device:

    sudo ../tools/b57tool --pxe hello.rom

This tool does require that you have installed the `DirectHW.kext` from
the CoreBoot project.


Developing ROMs
===

Calling conventions
---
The EFI environment uses the Microsoft ABI, so gcc must be told which
functions are called from or call into the EFI system.  This is done
with the `EFIAPI` macro, which annotates the functions with the gcc
x86 extension [`__attribute__((ms_abi))`](https://gcc.gnu.org/onlinedocs/gcc/x86-Function-Attributes.html#x86-Function-Attributes)

The entry point into the Option ROM must be named `efi_main()` and
should have the prototype:

	EFI_STATUS
	EFIAPI
	efi_main(
		EFI_HANDLE image,
		EFI_SYSTEM_TABLE * st
	);

Any callbacks that are registered, such as for the `ExitBootServices` event,
must also be flagged with `EFIAPI`.


Console I/O
---
It is possible to print to the screen while EFI is running.  The
`EFI_SYSTEM_TABLE` struct has a `SIMPLE_TEXT_OUTPUT_INTERFACE` pointer `ConOut`
to a struct with an `OutputString()` function pointer.  This function takes
UCS-2 wide characters:

	st->ConOut->OutputString(st->ConOut, L"Hello, world!\n");

The screen and the console will likely not be setup when `efi_main()` is
called, so it is typical to register a callback for `ExitBootServices`
to do any output.

As noted above, it is necessary to boot the system in "verbose" mode to
see any output from EFI.  Set the `bootargs` NVRAM variable to configure this:

	sudo nvram bootargs=-v

Reading keystrokes for a shell or similar can be done with the `ConIn`
struct.  I have not figured out how to use it, but I have figured out how
to hook it to read key strokes.  See `roms/keylogger.c` for an example.


Memory allocation
---
There are lots of pools of memory allocation during EFI, some of which are
cleared when the OS starts, some of which stay resident, etc.  In general
you can request memory with:

        void * buf;

        if (gST->BootServices->AllocatePool(
                EfiBootServicesData,
                len,
                &buf
        ) != 0) {
		// handle an error...
	}


