#include "stdafx.h"

struct rpak_h
{
	UINT32 magic;     // 1801539666
	UINT16 version;   // 8
	UINT8 flagA;      // only set in ui.rpak
	bool isCompressed;
	UINT64 type;
	UINT8 unknown1[8];
	UINT32 fileSize;
	UINT8 unknown2[20];
	UINT32 uncompressedFileSize;
	UINT8 unknown3[26];
	UINT16 unknown4;
	UINT32 pad;
	UINT32 us1;
	UINT32 entryCount;
	UINT32 us2;
	UINT32 us3;
	UINT8 unknown5[28];
};

enum class RpakAssetType : uint32_t
{
	Model        = 0x5F6C646D,
	Texture      = 0x72747874,
	UIIA         = 0x61696975,
	DataTable    = 0x6C627464,
	Settings     = 0x73677473,
	Material     = 0x6C74616D,
	AnimationRig = 0x67697261,
	Animation    = 0x71657361
};

class RTech
{
public:
	unsigned __int64 __fastcall ToGuid(const char* pData);
};

extern RTech* g_pRtech;
