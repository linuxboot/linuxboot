OUTPUT_FORMAT("elf64-x86-64", "elf64-x86-64", "elf64-x86-64")
/*OUTPUT_FORMAT("pei-x86-64", "pei-x86-64", "pei-x86-64") */
OUTPUT_ARCH(i386:x86-64)
ENTRY(efi_main)
SECTIONS
{
    .text 0x1000 :
    {
	*(.text)
    }

    .data :
    {
	*(.rodata*)
	*(.data*)
	/* the EFI loader doesn't seem to like a .bss section, so we stick
	it all into .data: */
	*(.bss)
    }

    /DISCARD/ :
    {
	*(.xdata*)
	*(.idata*)
	*(.pdata*)
	*(.comment)
	*(.eh_fram*)
    }
}
