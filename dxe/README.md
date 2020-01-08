Overview
===

These are small DXE modules that help bootstrap the LinuxBoot kernel.
They depend on the gnu-efi-devel package for the headers.

Developing DXE
===

Calling conventions
---
The EFI environment uses the Microsoft ABI, so gcc must be told which
functions are called from or call into the EFI system.  This is done
with the `EFIAPI` macro, which annotates the functions with the gcc
x86 extension [`__attribute__((ms_abi))`](https://gcc.gnu.org/onlinedocs/gcc/x86-Function-Attributes.html#x86-Function-Attributes)

The entry point into the DXE module must be named `efi_main()` and
should have the prototype:

	EFI_STATUS
	EFIAPI
	efi_main(
		EFI_HANDLE image,
		EFI_SYSTEM_TABLE * st
	);

Any callbacks that are registered, such as for the `ExitBootServices` event,
must also be flagged with `EFIAPI`.


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


