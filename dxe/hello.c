/** \file
 */
#include "serial.h"
#include <efi/efi.h>


EFI_STATUS
EFIAPI
efi_main(
    EFI_HANDLE image,
    EFI_SYSTEM_TABLE * const st
)
{
	(void) image;
	(void) st;

	serial_string("+---------------+\r\n");
	serial_string("| Hello, world! |\r\n");
	serial_string("+---------------+\r\n");
	serial_hex((uint64_t) image, 16);

	if (st->ConOut)
		st->ConOut->OutputString(st->ConOut, L"hello: Console output\n");

	return 0;
}
