/** \file
 */
#include "serial.h"
#include "efi.h"


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

	return 0;
}
