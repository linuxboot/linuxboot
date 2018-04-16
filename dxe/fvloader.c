/** \file
 * Tell DxeCore about an alternate firmware volume in the ROM.
 *
 * This allows LinuxBoot to locate the Linux kernel and initrd
 * outside of the normal DXE volume, which is quite small on some
 * systems.
 */
// #define VOLUME_ADDRESS 0xFF840000 // Winterfell
// #define VOLUME_LENGTH  0x20000

// Heron
#define VOLUME_ADDRESS	0xFF410000
#define VOLUME_LENGTH	0x00800000

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

	//gST = st;
	//gBS = gST->BootServices;
	//gRT = gST->RuntimeServices;

	const EFI_DXE_SERVICES * dxe_services = efi_find_table(st, 0x5ad34ba);

	if (!dxe_services)
	{
		serial_string("FvLoader: No DXE system table found...\r\n");
		return 0x80000001;
	}

	serial_string("FvLoader: adding firmware volume 0x");
	serial_hex(VOLUME_ADDRESS, 8);

	EFI_HANDLE handle;
	int rc = dxe_services->ProcessFirmwareVolume(
		(void*) VOLUME_ADDRESS,
		VOLUME_LENGTH,
		&handle
	);

	serial_string("FvLoader: rc="); serial_hex(rc, 8);

        return rc;
}
