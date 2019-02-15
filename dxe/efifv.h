/**
 * \file
 * EFI Firmware Volume protocol.
 *
 */
#ifndef _efi_fv_h_
#define _efi_fv_h_

#include <efi/efi.h>

#define EFI_FIRMWARE_VOLUME2_PROTOCOL_GUID	((EFI_GUID){ 0x220e73b6, 0x6bdb, 0x4413, { 0x84, 0x05, 0xb9, 0x74, 0xb1, 0x08, 0x61, 0x9a } })

typedef UINT8 EFI_FV_FILETYPE;
#define EFI_FV_FILETYPE_RAW                   0x01
#define EFI_FV_FILETYPE_FREEFORM              0x02
#define EFI_FV_FILETYPE_SECURITY_CORE         0x03
#define EFI_FV_FILETYPE_PEI_CORE              0x04
#define EFI_FV_FILETYPE_DXE_CORE              0x05
#define EFI_FV_FILETYPE_PEIM                  0x06
#define EFI_FV_FILETYPE_DRIVER                0x07
#define EFI_FV_FILETYPE_COMBINED_PEIM_DRIVER  0x08
#define EFI_FV_FILETYPE_APPLICATION           0x09
#define EFI_FV_FILETYPE_FIRMWARE_VOLUME_IMAGE 0x0b
#define EFI_FV_FILETYPE_FFS_PAD               0xf0

typedef UINT32 EFI_FV_FILE_ATTRIBUTES;
#define EFI_FV_FILE_ATTRIB_ALIGNMENT 0x0000001F

typedef UINT8 EFI_SECTION_TYPE;
#define EFI_SECTION_ALL                   0x00
#define EFI_SECTION_COMPRESSION           0x01
#define EFI_SECTION_GUID_DEFINED          0x02
#define EFI_SECTION_DISPOSABLE            0x03
#define EFI_SECTION_PE32                  0x10
#define EFI_SECTION_PIC                   0x11
#define EFI_SECTION_DXE_DEPEX             0x13
#define EFI_SECTION_COMPATIBILITY16       0x16
#define EFI_SECTION_FIRMWARE_VOLUME_IMAGE 0x17
#define EFI_SECTION_FREEFORM_SUBTYPE_GUID 0x18
#define EFI_SECTION_RAW                   0x19
#define EFI_SECTION_PEI_DEPEX             0x1B


typedef struct _EFI_FIRMWARE_VOLUME2_PROTOCOL EFI_FIRMWARE_VOLUME2_PROTOCOL;

typedef EFI_STATUS
(EFIAPI * EFI_FV_READ_FILE) (
  IN     EFI_FIRMWARE_VOLUME2_PROTOCOL *This,
  IN     EFI_GUID                     *NameGuid,
  IN OUT VOID                         **Buffer,
  IN OUT UINTN                        *BufferSize,
  OUT    EFI_FV_FILETYPE              *FoundType,
  OUT    EFI_FV_FILE_ATTRIBUTES       *FileAttributes,
  OUT    UINT32                       *AuthenticationStatus
  );

typedef EFI_STATUS
(EFIAPI * EFI_FV_READ_SECTION) (
  IN     EFI_FIRMWARE_VOLUME2_PROTOCOL *This,
  IN     EFI_GUID                     *NameGuid,
  IN     EFI_SECTION_TYPE             SectionType,
  IN     UINTN                        SectionInstance,
  IN OUT VOID                         **Buffer,
  IN OUT UINTN                        *BufferSize,
  OUT    UINT32                       *AuthenticationStatus
  );

struct _EFI_FIRMWARE_VOLUME2_PROTOCOL {
  uint64_t    GetVolumeAttributes;
  uint64_t    SetVolumeAttributes;
	EFI_FV_READ_FILE	ReadFile;
	EFI_FV_READ_SECTION	ReadSection;
  uint64_t    WriteFile;
  uint64_t    GetNextFile;
  uint32_t    KeySize;
  uint64_t    ParentHandle;
  uint64_t    GetInfo;
  uint64_t    SetInfo;
};


int
read_ffs(
	EFI_BOOT_SERVICES * gBS,
	EFI_GUID * guid,
	void ** buffer,
	UINTN * size,
	EFI_SECTION_TYPE section_type
);

int
append_read_ffs(
	EFI_BOOT_SERVICES * gBS,
	EFI_GUID * guid,
	void ** buffer,
	UINTN * size,
	EFI_SECTION_TYPE section_type
);

#endif
