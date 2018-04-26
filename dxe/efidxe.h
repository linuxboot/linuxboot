#ifndef __efi_h__
#define __efi_h__

typedef struct {
	EFI_TABLE_HEADER Hdr;
	void*             AddMemorySpace;
	void*        AllocateMemorySpace;
	void*            FreeMemorySpace;
	void*          RemoveMemorySpace;
	void*  GetMemorySpaceDescriptor;
	void*  SetMemorySpaceAttributes;
	void*         GetMemorySpaceMap;
	void*                 AddIoSpace;
	void*            AllocateIoSpace;
	void*                FreeIoSpace;
	void*              RemoveIoSpace;
	void*      GetIoSpaceDescriptor;
	void*             GetIoSpaceMap;
	void*                     Dispatch;
	void*                     Schedule;
	void*                        Trust;
	EFI_STATUS EFIAPI (*ProcessFirmwareVolume)(
		void * buffer,
		unsigned len,
		EFI_HANDLE * handle_out
	);
} EFI_DXE_SERVICES;


static inline void *
efi_find_table(
	EFI_SYSTEM_TABLE * st,
	uint32_t search_guid
)
{
	const EFI_CONFIGURATION_TABLE * ct = st->ConfigurationTable;

	serial_string("num tables=");
	serial_hex(st->NumberOfTableEntries, 4);

	for(unsigned i = 0 ; i < st->NumberOfTableEntries ; i++)
	{
		const EFI_GUID * guid = &ct[i].VendorGuid;
		serial_hex(*(uint64_t*)guid, 16);
		if (guid->Data1 == search_guid)
			return ct[i].VendorTable;

	}

	return NULL;
}

#endif
