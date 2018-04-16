#ifndef __efi_h__
#define __efi_h__

/* Just enough of the EFI API to write some code */

#define EFIAPI __attribute__((ms_abi))
#define NULL 0
typedef int EFI_STATUS;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long uint64_t;
typedef unsigned short wchar_t;

typedef void * EFI_HANDLE;

typedef struct {
    uint64_t                      Signature;
    uint32_t                      Revision;
    uint32_t                      HeaderSize;
    uint32_t                      CRC32;
    uint32_t                      Reserved;
} EFI_TABLE_HEADER;

typedef struct {          
    uint32_t  Data1;
    uint16_t  Data2;
    uint16_t  Data3;
    uint8_t   Data4[8]; 
} EFI_GUID;


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

typedef struct {
    EFI_GUID                VendorGuid;
    void                    *VendorTable;
} EFI_CONFIGURATION_TABLE;

typedef struct {
    EFI_TABLE_HEADER                Hdr;

    wchar_t                          *FirmwareVendor;
    uint32_t                          FirmwareRevision;

    void*                      ConsoleInHandle;
    void*          *ConIn;

    void*                      ConsoleOutHandle;
    void*    *ConOut;

    void*                      StandardErrorHandle;
    void*    *StdErr;

    void*            *RuntimeServices;
    void*               *BootServices;

    unsigned                           NumberOfTableEntries;
    EFI_CONFIGURATION_TABLE         *ConfigurationTable;

} EFI_SYSTEM_TABLE;


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
