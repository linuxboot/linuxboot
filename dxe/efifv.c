#include "efifv.h"
#include "serial.h"

/*
 * Locate a firmware file based on the GUID
 */
static EFI_FIRMWARE_VOLUME2_PROTOCOL *
find_ffs(
	EFI_BOOT_SERVICES * gBS,
	EFI_GUID * guid
)
{
	EFI_STATUS status;
	EFI_HANDLE * handles = NULL;
	UINTN handle_count;
	EFI_GUID fv_proto = EFI_FIRMWARE_VOLUME2_PROTOCOL_GUID;

	status = gBS->LocateHandleBuffer(
		ByProtocol,
		&fv_proto,
		NULL,
		&handle_count,
		&handles
	);

	if (status != 0)
	{
		serial_string("LinuxBoot: locate_handle rc=");
		serial_hex(status, 8);
		return NULL;
	}

	for(unsigned i = 0 ; i < handle_count ; i++)
	{
		EFI_FIRMWARE_VOLUME2_PROTOCOL * fv = NULL;

		//serial_string("handle=");
		//serial_hex((unsigned long) handles[i], 16);

		status = gBS->HandleProtocol(
			handles[i],
			&fv_proto,
			(void**) &fv
		);

		if (status != 0)
		{
			serial_string("handle proto rc=");
			serial_hex(status, 8);
			continue;
		}

		UINTN size;
		UINT32 auth_status;
		EFI_FV_FILETYPE type;
		EFI_FV_FILE_ATTRIBUTES attributes;
	
		status = fv->ReadFile(
			fv,
			guid,
			NULL,
			&size,
			&type,
			&attributes,
			&auth_status
		);
		if (status != EFI_SUCCESS)
			continue;

		//serial_string("LinuxBoot: fv=");
		//serial_hex((unsigned long) fv, 16);

		return fv;
	}

	// this leaks the handle buffer.
	return NULL;
}


int
read_ffs(
	EFI_BOOT_SERVICES * gBS,
	EFI_GUID * guid,
	void ** buffer,
	UINTN * size,
	EFI_SECTION_TYPE section_type
)
{
	EFI_FIRMWARE_VOLUME2_PROTOCOL * fv = find_ffs(gBS, guid);
	if (!fv)
		return -1;

	UINT32 auth_status;
	EFI_STATUS status = fv->ReadSection(
		fv,
		guid,
		section_type,
		0,
		buffer,
		size,
		&auth_status
	);
	if (status != 0)
	{
		serial_string("LinuxBoot: read section rc=");
		serial_hex(status, 8);
		return -1;
	}

	serial_string("LinuxBoot: FFS buffer=");
	serial_hex((unsigned long) *buffer, 16);
	serial_string("LinuxBoot: FFS length=");
	serial_hex(*size, 8);

	return 0;
}
