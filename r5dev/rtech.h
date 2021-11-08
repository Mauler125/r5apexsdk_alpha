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

class RTech
{
public:
	unsigned __int64 __fastcall ToGuid(const char* pData);
};

extern RTech* g_pRtech;
