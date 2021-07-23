#pragma once

/* Structures */
typedef unsigned __int64 QWORD;

struct __declspec(align(8)) netpacket_t
{
	DWORD family_maybe;
	sockaddr_in sin;
	WORD sin_port;
	BYTE gap16;
	BYTE byte17;
	DWORD source;
	double received;
	unsigned __int8* data;
	QWORD label;
	BYTE byte38;
	QWORD qword40;
	QWORD qword48;
	BYTE gap50[8];
	QWORD qword58;
	QWORD qword60;
	QWORD qword68;
	int less_than_12;
	DWORD wiresize;
	BYTE gap78[8];
	QWORD qword80;
};

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
