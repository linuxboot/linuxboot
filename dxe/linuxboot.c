/** \file
 * LinuxBoot BDS
 *
 * Locates the Linux kernel and initrd on a possible external volume,
 * finds the command line and uses the BootServices->StartImage()
 * to make it go.
 *
 * This allows LinuxBoot to locate the Linux kernel and initrd
 * outside of the normal DXE volume, which is quite small on some
 * systems.
 *
 */
#define VOLUME_ADDRESS 0xFF840000 // Winterfell
#define VOLUME_LENGTH  0x20000

#define VOLUME2_ADDRESS 0xFF320000 // Winterfell resize ME
#define VOLUME2_LENGTH  0x4E0000

// #define VOLUME_ADDRESS	0xFF500000
// #define VOLUME_LENGTH	0x00400000

#include "serial.h"
#include <efi/efi.h>
#include "efidxe.h"
#include "efifv.h"
#include <asm/bootparam.h>
#include <efi/pci22.h>

EFI_HANDLE gImage;
EFI_SYSTEM_TABLE * gST;
EFI_BOOT_SERVICES * gBS;
EFI_RUNTIME_SERVICES * gRT;
EFI_DXE_SERVICES * gDXE;


static void
hexdump(uint64_t p, unsigned len)
{
	for(unsigned i = 0 ; i < len ; i += 8)
		serial_hex(*(const uint64_t*)(p+i), 16);
}


static int
process_fv(
	const uintptr_t ptr,
	const unsigned len
)
{
	serial_string("FvLoader: adding firmware volume 0x");
	serial_hex(ptr, 8);

	EFI_HANDLE handle;
	int rc = gDXE->ProcessFirmwareVolume(
		(void*) ptr,
		len,
		&handle
	);

	if (rc == 0)
	{
		serial_string("FVLoader: mapped 0x");
		serial_hex(len, 8);
	} else {
		serial_string("FvLoader: error rc="); serial_hex(rc, 8);
		hexdump(ptr, 128);
	}

        return rc;
}


/*
 * The LinuxBoot kernel is invoked as a DXE driver that registers
 * the BDS (Boot Device Selector) protocol.  Once all of the DXE
 * executables have run, the DxeCore dispatcher will jump into the
 * BDS to choose what kernel to run.
 *
 * In our case, it is this kernel.  So we need to stash the config
 * for when we are re-invoked.
 */
static void EFIAPI
empty_notify(void* unused1, void* unused2)
{
	(void) unused1;
	(void) unused2;
}

#define EFI_DXE_SERVICES_TABLE_GUID		((EFI_GUID){ 0x5ad34ba, 0x6f02, 0x4214, { 0x95, 0x2e, 0x4d, 0xa0, 0x39, 0x8e, 0x2b, 0xb9 } })

#define ROOT_BRIDGES_CONNECTED_EVENT_GROUP_GUID	((EFI_GUID){ 0x24a2d66f, 0xeedd, 0x4086, { 0x90, 0x42, 0xf2, 0x6e, 0x47, 0x97, 0xee, 0x69 } })

#define EFI_END_OF_DXE_EVENT_GROUP_GUID		((EFI_GUID){ 0x02ce967a, 0xdd7e, 0x4ffc, { 0x9e, 0xe7, 0x81, 0x0c, 0xf0, 0x47, 0x08, 0x80 } })

#define EFI_EVENT_GROUP_READY_TO_BOOT		((EFI_GUID){ 0x7ce88fb3, 0x4bd7, 0x4679, { 0x87, 0xa8, 0xa8, 0xd8, 0xde, 0xe5, 0x0d, 0x2b } })

#define EFI_DXE_SMM_READY_TO_LOCK_PROTOCOL_GUID	((EFI_GUID){ 0x60ff8964, 0xe906, 0x41d0, { 0xaf, 0xed, 0xf2, 0x41, 0xe9, 0x74, 0xe0, 0x8e } })

#define EFI_BDS_ARCH_PROTOCOL_GUID		((EFI_GUID){ 0x665E3FF6, 0x46CC, 0x11d4, { 0x9A, 0x38, 0x00, 0x90, 0x27, 0x3F, 0xC1, 0x4D } })


static void
efi_event_signal(
	EFI_GUID guid
)
{
	EFI_STATUS status;
	EFI_EVENT event;

	status = gBS->CreateEventEx(
		EVT_NOTIFY_SIGNAL,
		EFI_TPL_CALLBACK,
		empty_notify,
		NULL,
		&guid,
		&event
	);
	if (status)
		serial_hex(status, 8);

	status = gBS->SignalEvent(event);
	if (status)
		serial_hex(status, 8);

	status = gBS->CloseEvent(event);
	if (status)
		serial_hex(status, 8);
}


static void
efi_visit_handles(
	EFI_GUID * protocol,
	void (EFIAPI *callback)(EFI_HANDLE, void*),
	void* priv
)
{
	serial_string("efi_visit_handles ");
	serial_hex(protocol ? *(uint32_t*) protocol : 0, 8);
	EFI_HANDLE * handle_buffer;
	UINTN handle_count;

	EFI_STATUS status = gBS->LocateHandleBuffer(
		protocol ? ByProtocol : AllHandles,
		protocol,
		NULL,
		&handle_count,
		&handle_buffer
	);
	if (status != 0)
	{
		serial_string("status=");
		serial_hex(status, 8);
		return;
	}

	serial_string("handle_count=");
	serial_hex(handle_count, 8);

	for(unsigned i = 0 ; i < handle_count ; i++)
	{
		//serial_hex((uint64_t) handle_buffer[i], 16);
		callback(handle_buffer[i], priv);
	}
	gBS->FreePool (handle_buffer);
}


static void EFIAPI
efi_connect_controllers(
	EFI_HANDLE handle,
	void * recursive_arg
)
{
	gBS->ConnectController(
		handle,
		NULL, // DriverImageHandle
		NULL, // RemainingDevicePath
		recursive_arg ? 1 : 0
	);
}

#define EFI_PCI_DEVICE_ENABLE                     (EFI_PCI_IO_ATTRIBUTE_IO | EFI_PCI_IO_ATTRIBUTE_MEMORY | EFI_PCI_IO_ATTRIBUTE_BUS_MASTER)

#define IS_CLASS1(_p, c)              ((_p)->Hdr.ClassCode[2] == (c))
#define IS_CLASS2(_p, c, s)           (IS_CLASS1 (_p, c) && ((_p)->Hdr.ClassCode[1] == (s)))
#define IS_CLASS3(_p, c, s, p)        (IS_CLASS2 (_p, c, s) && ((_p)->Hdr.ClassCode[0] == (p)))


#define IS_PCI_LPC(_p)                IS_CLASS3 (_p, PCI_CLASS_BRIDGE, PCI_CLASS_ISA, 0)
#define IS_PCI_ISA_PDECODE(_p)        IS_CLASS3 (_p, PCI_CLASS_BRIDGE, PCI_CLASS_ISA_POSITIVE_DECODE, 0)


EFI_GUID pci_io_protocol = EFI_PCI_IO_PROTOCOL_GUID;
EFI_PCI_IO_PROTOCOL * LpcPciIo;

static void EFIAPI
efi_search_lpc(
	EFI_HANDLE handle,
	void * priv
)
{
	EFI_PCI_IO_PROTOCOL * PciIo;
	PCI_TYPE00 Pci;

	priv = priv; // unused

	EFI_STATUS status = gBS->HandleProtocol (handle, &pci_io_protocol, (VOID *)&PciIo);
	if (EFI_ERROR (status)) {
		return;
	}

	status = PciIo->Pci.Read (
		PciIo,
		EfiPciIoWidthUint32,
		0,
		sizeof (Pci) / sizeof (UINT32),
		&Pci
	);
	if (EFI_ERROR (status)) {
		return;
	}
	status = PciIo->Attributes (
		PciIo,
		EfiPciIoAttributeOperationEnable,
		EFI_PCI_DEVICE_ENABLE,
		NULL
	);
	if (EFI_ERROR (status)) {
		return;
	}
	if ((IS_PCI_LPC (&Pci)) || ((IS_PCI_ISA_PDECODE (&Pci)) && (Pci.Hdr.VendorId == 0x8086))) {
		serial_string("Found LPC Bridge device\r\n");
		LpcPciIo = PciIo;
		return;
	}
}

#define BIOS_SEL1	0xD0
#define BIOS_SEL2	0xD4
#define BIOS_DEC_EN1	0xD8
static void
lpc_update_flash_access(void)
{
	UINT32 reg32;
	UINT16 reg16;

	EFI_STATUS status = LpcPciIo->Pci.Read(LpcPciIo, EfiPciIoWidthUint32, BIOS_SEL1, 1, &reg32);
	serial_string("BIOS_SEL1=");
	if (EFI_ERROR (status)) {
		serial_string("status=");
		serial_hex(status, 8);
		return;
	}
	serial_hex(reg32, 8);

	status = LpcPciIo->Pci.Read(LpcPciIo, EfiPciIoWidthUint16, BIOS_SEL2, 1, &reg16);
	serial_string("BIOS_SEL2=");
	serial_hex(reg16, 4);


	reg16 = 0xFFCF;
	status = LpcPciIo->Pci.Write(LpcPciIo, EfiPciIoWidthUint16, BIOS_DEC_EN1, 1, &reg16);
	if (EFI_ERROR (status)) {
		serial_string("status=");
		serial_hex(status, 8);
		return;
	}
	status = LpcPciIo->Pci.Read(LpcPciIo, EfiPciIoWidthUint16, BIOS_DEC_EN1, 1, &reg16);
	serial_string("BIOS_DEC_EN1=");
	serial_hex(reg16, 4);
}

static void
efi_final_init(void)
{
	// equivilant to PlatformBootManagerBeforeConsole

	// connect all the pci root bridges
	serial_string("LinuxBoot: connect pci root bridges\r\n");
	EFI_GUID pci_protocol = EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_GUID;
	efi_visit_handles(&pci_protocol, efi_connect_controllers, (void*) 0);

	// search LPC and update flash access
	serial_string("LinuxBoot: search LPC device\r\n");
	efi_visit_handles(&pci_io_protocol, efi_search_lpc, (void*) 0);

	lpc_update_flash_access();

	if (VOLUME2_ADDRESS)
		process_fv(VOLUME2_ADDRESS, VOLUME2_LENGTH);

	// signal the acpi platform driver that it can download the ACPI tables
	serial_string("LinuxBoot: signal root bridges connected\r\n");
	efi_event_signal(ROOT_BRIDGES_CONNECTED_EVENT_GROUP_GUID);

	// signal that dxe is about to end
	serial_string("LinuxBoot: signal dxe end\r\n");
	efi_event_signal(EFI_END_OF_DXE_EVENT_GROUP_GUID);

	// Prevent further changes to LockBoxes or SMRAM.
	// This should be a configurable option
	EFI_HANDLE handle = NULL;
	EFI_GUID smm_ready_to_lock = EFI_DXE_SMM_READY_TO_LOCK_PROTOCOL_GUID;
	serial_string("LinuxBoot: signal smm ready to lock\r\n");

	gBS->InstallProtocolInterface(
		&handle,
		&smm_ready_to_lock,
		EFI_NATIVE_INTERFACE,
		NULL
	);

	// connect all drivers their contorllers
	// this is copied from BmConnectAllDriversToAllControllers()
	// the DXE services table is buried in the configuration
	// table in the system table
/**
  Connect all the drivers to all the controllers.

  This function makes sure all the current system drivers manage the correspoinding
  controllers if have. And at the same time, makes sure all the system controllers
  have driver to manage it if have.
**/
	do {
		efi_visit_handles(NULL, efi_connect_controllers, (void*) 1);
		serial_string("LinuxBoot: bds_main dispatch\r\n");
	} while(gDXE->Dispatch() == 0);

	// signal that we're ready to boot, which will
	// cause additional drivers to be loaded
	serial_string("LinuxBoot: signal ready to boot\r\n");
	efi_event_signal(EFI_EVENT_GROUP_READY_TO_BOOT);
}



// code in MdeModulePkg/Library/UefiBootManagerLib/BmBoot.c
static int
linuxboot_start()
{
	EFI_STATUS status;
	EFI_GUID bzimage_guid = { 0xDECAFBAD, 0x6548, 0x6461, { 0x73, 0x2d, 0x2f, 0x2d, 0x4e, 0x45, 0x52, 0x46 }};
	EFI_GUID initrd_guid = { 0x74696e69, 0x6472, 0x632e, { 0x70, 0x69, 0x6f, 0x2f, 0x62, 0x69, 0x6f, 0x73 }};
	EFI_GUID initrdpart_guid = { 0x74696e69, 0x6472, 0x632e, { 0x70, 0x69, 0x6f, 0x2f, 0x70, 0x61, 0x72, 0x74 }};

	void * bzimage_buffer = NULL;
	UINTN bzimage_length = 0;
	serial_string("LinuxBoot: Looking for bzimage\r\n");
	if (read_ffs(gBS, &bzimage_guid, &bzimage_buffer, &bzimage_length, EFI_SECTION_PE32) < 0)
		return -1;

	// convert the RAM image of the kernel into a loaded image
	EFI_HANDLE bzimage_handle = NULL;
	status = gBS->LoadImage(
		TRUE, // Boot
		gImage,
		NULL, // no device path
		bzimage_buffer,
		bzimage_length,
		&bzimage_handle
	);
	if (status != 0)
	{
		serial_string("LinuxBoot: unable to load bzImage image\r\n");
		return -1;
	}

	EFI_GUID loaded_image_guid = LOADED_IMAGE_PROTOCOL;
	EFI_LOADED_IMAGE_PROTOCOL * loaded_image = NULL;
	status = gBS->HandleProtocol(
		bzimage_handle,
		&loaded_image_guid,
		(void**) &loaded_image
	);
	if (status != 0)
	{
		serial_string("LinuxBoot: unable to get LoadedImageProtocol\r\n");
		return -1;
	}

	void * initrd_buffer = NULL;
	UINTN initrd_length = 0;
	serial_string("LinuxBoot: Looking for initrd\r\n");
	if (read_ffs(gBS, &initrd_guid, &initrd_buffer, &initrd_length, EFI_SECTION_RAW) < 0)
	{
		serial_string("LinuxBoot: no initrd found\r\n");
	} else {
		static uint16_t cmdline[] = L"found_initd";
		loaded_image->LoadOptions = cmdline;
		loaded_image->LoadOptionsSize = sizeof(cmdline);

		serial_string("LinuxBoot: Looking for a second initrd part\r\n");
		append_read_ffs(gBS, &initrdpart_guid, &initrd_buffer, &initrd_length, EFI_SECTION_RAW);
		serial_string("LinuxBoot: initrd buffer=");
		serial_hex((unsigned long) initrd_buffer, 16);
		serial_string("LinuxBoot: initrd length=");
		serial_hex(initrd_length, 8);
		uintptr_t hdr = (uintptr_t) loaded_image->ImageBase;
		*(uint32_t*)(hdr + 0x218) = (uint32_t)(uintptr_t) initrd_buffer;
		*(uint32_t*)(hdr + 0x21c) = (uint32_t)(uintptr_t) initrd_length;
	}

	// attempt to load the kernel
	UINTN exit_data_len = 0;
	CHAR16 * exit_data = NULL;

	serial_string("LinuxBoot: Starting bzImage\r\n");
	status = gBS->StartImage(
		bzimage_handle,
		&exit_data_len,
		&exit_data
	);
	if (status != 0)
	{
		serial_string("LinuxBoot: Unable to start bzImage\r\n");
		return -1;
	}

	return 0;
}


static EFI_STATUS EFIAPI
efi_bds_main(void)
{
	serial_string("LinuxBoot: BDS time has arrived\r\n");
	efi_final_init();

	if (linuxboot_start() < 0)
		return 0;

	serial_string("LinuxBoot: SOMETHING IS WRONG\r\n");
	return EFI_NOT_FOUND;
}


static struct
{
	EFI_STATUS (EFIAPI *bds_main)(void);
} efi_bds_arch_protocol;



EFI_STATUS
EFIAPI
efi_main(
    EFI_HANDLE image,
    EFI_SYSTEM_TABLE * const st
)
{
	serial_string("+--------------------+\r\n");
	serial_string("|                    |\r\n");
	serial_string("| Starting LinuxBoot |\r\n");
	serial_string("|                    |\r\n");
	serial_string("+--------------------+\r\n");

	gImage = image;
	gST = st;
	gBS = gST->BootServices;
	gRT = gST->RuntimeServices;
	gDXE = efi_find_table(gST, 0x5ad34ba); // should use DXE table guid

	if (!gDXE)
	{
		serial_string("LinuxBoot: No DXE system table found...\r\n");
		return EFI_LOAD_ERROR;
	}

	// update the PCH to map the entire flashchip
	// BIOS_SEL1 and BIOS_SEL2
	// moved to efi_final_init as PCI is needed

	// create any new volumes
	if (VOLUME_ADDRESS)
		process_fv(VOLUME_ADDRESS, VOLUME_LENGTH);

	// register the BDS callback
	efi_bds_arch_protocol.bds_main = efi_bds_main;
	EFI_GUID bds_guid = EFI_BDS_ARCH_PROTOCOL_GUID;

	gBS->InstallProtocolInterface(
		&image,
		&bds_guid,
		EFI_NATIVE_INTERFACE,
		&efi_bds_arch_protocol
	);

	serial_string("LinuxBoot: waiting for BDS callback\r\n");
	return 0;
}
