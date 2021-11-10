#include "stdafx.h"
#include "rtech.h"

/*-----------------------------------------------------------------------------
 * _rtech
 *-----------------------------------------------------------------------------*/

//-----------------------------------------------------------------------------
// Purpose: calculate the GUID from input data
//-----------------------------------------------------------------------------
unsigned __int64 __fastcall RTech::ToGuid(const char* pData)
{
    DWORD*           v1; // r8
    unsigned __int64 v2; // r10
    int              v3; // er11
    unsigned int     v4; // er9
    unsigned int      i; // edx
    __int64          v6; // rcx
    int              v7; // er9
    int              v8; // edx
    int              v9; // eax
    unsigned int    v10; // er8
    int             v12; // ecx
    DWORD* a1 = (DWORD*)pData;

    v1 = a1;
    v2 = 0i64;
    v3 = 0;
    v4 = (*a1 - 45 * ((~(*a1 ^ 0x5C5C5C5Cu) >> 7) & (((*a1 ^ 0x5C5C5C5Cu) - 0x1010101) >> 7) & 0x1010101)) & 0xDFDFDFDF;
    for (i = ~*a1 & (*a1 - 0x1010101) & 0x80808080; !i; i = v8 & 0x80808080)
    {
        v6 = v4;
        v7 = v1[1];
        ++v1;
        v3 += 4;
        v2 = ((((unsigned __int64)(0xFB8C4D96501i64 * v6) >> 24) + 0x633D5F1 * v2) >> 61) ^ (((unsigned __int64)(0xFB8C4D96501i64 * v6) >> 24)
            + 0x633D5F1 * v2);
        v8 = ~v7 & (v7 - 0x1010101);
        v4 = (v7 - 45 * ((~(v7 ^ 0x5C5C5C5Cu) >> 7) & (((v7 ^ 0x5C5C5C5Cu) - 0x1010101) >> 7) & 0x1010101)) & 0xDFDFDFDF;
    }
    v9 = -1;
    v10 = (i & -(signed)i) - 1;
    if (_BitScanReverse((unsigned long*)&v12, v10))
    {
        v9 = v12;
    }
    return 0x633D5F1 * v2 + ((0xFB8C4D96501i64 * (unsigned __int64)(v4 & v10)) >> 24) - 0xAE502812AA7333i64 * (unsigned int)(v3 + v9 / 8);
}

//-----------------------------------------------------------------------------
// Purpose: decompress input data
//-----------------------------------------------------------------------------
int RTech::Decompress(long long* param_1, unsigned long long param_2, unsigned long long param_3)
{
    long long* plVar1;
    unsigned char* puVar2;
    unsigned short uVar3;
    unsigned int uVar4;
    long long lVar5;
    long long lVar6;
    unsigned long long uVar7;
    unsigned int uVar8;
    unsigned long long in_RAX;
    unsigned long long uVar9;
    unsigned long long uVar10;
    unsigned int uVar11;
    unsigned int uVar12;
    long long* plVar13;
    unsigned char* puVar14;
    unsigned int uVar15;
    int iVar16;
    long long lVar17;
    unsigned long long* puVar18;
    unsigned long long* puVar19;
    unsigned long long uVar20;
    unsigned long long uVar21;
    unsigned long long uVar22;
    unsigned long long* puVar23;
    unsigned long long uVar24;
    long long* plVar25;
    unsigned char bVar26;
    int iVar27;
    int iVar28;
    long long* plVar29;
    unsigned int uVar30;
    unsigned long long uVar31;
    unsigned long long uVar32;
    unsigned long long uVar33;

    if ((unsigned long long)param_1[0xb] <= param_2) {
        uVar33 = param_1[10];
        in_RAX = (~param_1[7] & uVar33) + 1 + param_1[7];
        if ((in_RAX <= param_3) || ((unsigned long long)param_1[5] <= param_3)) {
            lVar5 = *param_1;
            lVar6 = param_1[1];
            uVar15 = *(unsigned int*)((long long)param_1 + 0x6c);
            uVar12 = *(unsigned int*)(param_1 + 0xd);
            uVar21 = param_1[0xc];
            uVar22 = param_1[9];
            uVar32 = param_1[0xf];
            // not checked yet
            if ((unsigned long long)param_1[0xe] <= uVar32) {
                uVar32 = param_1[0xe];
            }
            unsigned int i = 0;
            do {
                i++;
                /*if (!(i % 500)) {
                    FILE* decomp_progress_file = fopen("C:\\Users\\user\\Desktop\\progress", "wb");
                    if (fwrite(param_1[1], 1, 0x400000, decomp_progress_file) != 0x400000) {
                        printf("");
                    }
                    fclose(decomp_progress_file);
                }*/
                lVar17 = (uVar21 & 0xff) + (unsigned long long)uVar15 * 0x100;
                uVar8 = (unsigned int)((int)((char)(LUT_1)[lVar17]));
                uVar21 >>= ((LUT_2)[lVar17] & 0x3f);
                uVar12 = (unsigned char)(LUT_2)[lVar17] + uVar12;
                uVar10 = (unsigned long long)uVar12;
                if ((char)(LUT_1)[lVar17] < '\0') {
                    uVar11 = uVar15 + 0x4e0;
                    puVar23 = (unsigned long long*)((param_1[2] & uVar22) + lVar5);
                    uVar8 = -uVar8;
                    puVar18 = (unsigned long long*)((uVar33 & param_1[3]) + lVar6);
                    uVar15 = 1;
                    if (uVar8 == (unsigned char)(LUT_1)[uVar11]) {
                        // not checked yet
                        if ((((param_1[6] & ~uVar22) < 0xf) || ((~uVar33 & param_1[7]) < 0xf)) ||
                            (param_1[5] - uVar33 < 0x10)) {
                            uVar8 = 1;
                        }
                        uVar9 = uVar21 >> 3;
                        uVar11 = (int)(char)uVar21 & 7;
                        if (uVar11 == 0) {
                            uVar10 = (unsigned long long)((unsigned int)uVar9 & 0xf);
                            uVar9 = uVar21 >> 7;
                            uVar12 = uVar12 + 4;
                            uVar30 = ((unsigned int*)&LUT_3)[uVar10];
                            bVar26 = (LUT_4)[uVar10];
                        }
                        else {
                            uVar30 = (unsigned int)(unsigned char)((LUT_5)[uVar11]);
                            bVar26 = (LUT_6)[uVar11];
                        }
                        uVar10 = (unsigned long long)(uVar12 + 3 + (unsigned int)bVar26);
                        uVar21 = uVar9 >> (bVar26 & 0x3f);
                        uVar8 = (((1 << (bVar26 & 0x1f)) - 1U) & (unsigned int)uVar9) + uVar30 + uVar8;
                        uVar12 = uVar8 >> 3;
                        while (uVar12 != 0) {
                            uVar7 = *puVar23;
                            puVar23 = puVar23 + 1;
                            *puVar18 = uVar7;
                            uVar12 = uVar12 - 1;
                            puVar18 = puVar18 + 1;
                        }
                        puVar19 = puVar18;
                        if ((uVar8 & 4) != 0) {
                            uVar4 = *(unsigned int*)puVar23;
                            puVar19 = (unsigned long long*)((long long)puVar18 + 4);
                            puVar23 = (unsigned long long*)((long long)puVar23 + 4);
                            *(unsigned int*)puVar18 = uVar4;
                        }
                        puVar18 = puVar19;
                        if ((uVar8 & 2) != 0) {
                            uVar3 = *(unsigned short*)puVar23;
                            puVar18 = (unsigned long long*)((long long)puVar19 + 2);
                            puVar23 = (unsigned long long*)((long long)puVar23 + 2);
                            *(unsigned short*)puVar19 = uVar3;
                        }
                        if ((uVar8 & 1) != 0) {
                            *(unsigned char*)puVar18 = *(unsigned char*)puVar23;
                        }
                        uVar22 = uVar22 + uVar8;
                        uVar33 = uVar33 + uVar8;
                    }
                    else {
                        *puVar18 = *puVar23;
                        puVar18[1] = puVar23[1];
                        uVar22 = uVar22 + uVar8;
                        uVar33 = uVar33 + uVar8;
                        uVar15 = 1;
                    }
                }
                else {
                    uVar30 = (unsigned int)uVar21 & 0xf;
                    uVar11 = ((uVar30 - 0x1f) >> 3) & 6;
                    uVar9 = (unsigned long long)((unsigned int)((uVar21 & 0xffffffff) >> (char)uVar11) & 0x3f);
                    uVar15 = (((uVar30 + 1) >> 3) & 2) * 0x18 >> 4;
                    iVar28 = (uVar15 & (unsigned int)(uVar21 >> 4)) + uVar30;
                    iVar27 = (unsigned char)(LUT_7)[uVar9] + uVar11;
                    iVar16 = 1 << (((unsigned char)uVar15 & (unsigned char)(uVar21 >> 4)) + (char)uVar30);
                    bVar26 = (unsigned char)iVar27;
                    uVar24 = uVar21 >> (bVar26 & 0x3f);
                    uVar21 = uVar21 >> (bVar26 + (char)iVar28 & 0x3f);
                    uVar12 = uVar12 + iVar28 + iVar27;
                    uVar10 = (unsigned long long)uVar12;
                    uVar15 = (unsigned int)(unsigned char)(LUT_8)[uVar9] +
                        (((unsigned int)uVar24 & (iVar16 - 1U)) + iVar16) * 0x10 + -0x10;
                    plVar29 = (long long*)((param_1[3] & uVar33) + lVar6);
                    plVar25 = (long long*)(((uVar33 - uVar15) & param_1[3]) + lVar6);
                    if (uVar8 == 0x11) {
                        uVar9 = uVar21 >> 3;
                        uVar8 = (int)(char)uVar21 & 7;
                        if (uVar8 == 0) {
                            uVar10 = (unsigned long long)((unsigned int)uVar9 & 0xf);
                            uVar9 = uVar21 >> 7;
                            uVar8 = uVar12 + 4;
                            uVar11 = ((unsigned int*)(&LUT_3))[uVar10];
                            uVar30 = (unsigned int)(unsigned char)(LUT_4)[uVar10];
                            // not checked yet
                            if ((lVar5 != 0) && (0x3c < uVar8 + (unsigned char)(LUT_4)[uVar10])) {
                                uVar21 = param_1[2] & uVar22;
                                uVar22 = uVar22 + 1;
                                // check this
                                uVar9 = uVar9 | ((unsigned long long) * (unsigned char*)(uVar21 + lVar5) << ((0x3dU - (char)uVar8) & 0x3f))
                                    ;
                                uVar8 = uVar12 - 4;
                            }
                        }
                        else {
                            uVar11 = (unsigned int)(unsigned char)(LUT_5)[uVar8];
                            uVar30 = (unsigned int)(unsigned char)(LUT_6)[uVar8];
                            uVar8 = uVar12;
                        }
                        uVar10 = (unsigned long long)(uVar8 + 3 + uVar30);
                        uVar21 = uVar9 >> ((unsigned char)uVar30 & 0x3f);
                        uVar11 = ((((1 << ((unsigned char)uVar30 & 0x1f)) - 1U) & (unsigned int)uVar9)) + 0x11 + uVar11;
                        uVar33 = uVar33 + uVar11;
                        if (uVar15 < 8) {
                            uVar12 = uVar11 - 0xd;
                            uVar33 = uVar33 - 0xd;
                            if (uVar15 == 1) {
                                uVar15 = 0;
                                if (uVar12 != 0) {
                                    bVar26 = *(unsigned char*)plVar25;
                                    uVar12 = ((uVar11 - 0xe) >> 3) + 1;
                                    uVar9 = (unsigned long long)uVar12;
                                    uVar15 = 0;
                                    if (uVar12 != 0) {
                                        while (uVar15 = 0, uVar9 != 0) {
                                            uVar9 = uVar9 - 1;
                                            *plVar29 = (unsigned long long)bVar26 * 0x101010101010101;
                                            plVar29 = plVar29 + 1;
                                        }
                                    }
                                }
                                goto LAB_7ff75294416d;
                            }
                            // not checked yet
                            if (uVar12 != 0) {
                                uVar9 = (unsigned long long)uVar12;
                                puVar14 = (unsigned char*)((long long)plVar29 + -1);
                                do {
                                    puVar2 = puVar14 + (long long)plVar25 + (1 - (long long)plVar29);
                                    puVar14 = puVar14 + 1;
                                    *puVar14 = *puVar2;
                                    uVar9 = uVar9 - 1;
                                } while (uVar9 != 0);
                            }
                        }
                        else {
                            if (uVar11 != 0) {
                                plVar13 = plVar29 + -1;
                                uVar9 = (unsigned long long)(((uVar11 - 1) >> 3) + 1);
                                do {
                                    plVar1 = (long long*)
                                        ((long long)plVar25 + (8 - (long long)plVar29) + (long long)plVar13);
                                    plVar13 = plVar13 + 1;
                                    *plVar13 = *plVar1;
                                    uVar9 = uVar9 - 1;
                                } while (uVar9 != 0);
                            }
                        }
                    }
                    else {
                        uVar33 = uVar33 + uVar8;
                        *plVar29 = *plVar25;
                        plVar29[1] = plVar25[1];
                    }
                    uVar15 = 0;
                }
            LAB_7ff75294416d:
                // not checked yet
                if (uVar32 <= uVar22) {
                    if (uVar33 == param_1[0x10]) {
                        uVar32 = param_1[5];
                        if (uVar33 == uVar32) {
                            bVar26 = 1;
                            goto LAB_7ff752944353;
                        }
                        uVar9 = param_1[6];
                        uVar24 = (unsigned long long) * (unsigned int*)(param_1 + 8);
                        uVar21 = uVar21 >> 1;
                        uVar12 = (int)uVar10 + 1;
                        uVar10 = (unsigned long long)uVar12;
                        if ((-uVar22 & uVar9) < uVar24) {
                            uVar22 = uVar22 + (-uVar22 & uVar9);
                            if ((unsigned long long)param_1[0xe] < uVar22) {
                                param_1[0xe] = param_1[0xe] + 1U + uVar9;
                            }
                        }
                        uVar20 = param_1[2] & uVar22;
                        uVar22 = uVar22 + uVar24;
                        uVar20 = ((1 << ((unsigned char)((*(unsigned int*)(param_1 + 8) & 0xff) << 3) & 0x3f)) - 1U) &
                            *(unsigned long long*)(uVar20 + lVar5);
                        uVar31 = param_1[0xb] + uVar20;
                        lVar17 = param_1[0xf] + uVar20;
                        param_1[0xb] = uVar31;
                        uVar20 = param_1[7] + 1 + uVar33;
                        param_1[0xf] = lVar17;
                        if (uVar32 <= uVar20) {
                            param_1[0xf] = lVar17 + uVar24;
                            uVar20 = uVar32;
                        }
                        param_1[0x10] = uVar20;
                        if ((param_2 < uVar31) || (param_3 < uVar20)) {
                            if ((unsigned long long)param_1[0xe] <= uVar22) {
                                uVar22 = (uVar22 + 7) & ~uVar9;
                                param_1[0xe] = param_1[0xe] + 1U + uVar9;
                            }
                            bVar26 = 0;
                            lVar5 = *(long long*)(lVar5 + (param_1[2] & uVar22));
                            uVar22 = uVar22 + (uVar12 >> 3);
                            *(unsigned int*)((long long)param_1 + 0x6c) = uVar15;
                            *(unsigned int*)(param_1 + 0xd) = uVar12 & 7;
                            // check this
                            param_1[0xc] = (lVar5 << (0x40U - (char)uVar12 & 0x3f) | uVar21) &
                                (0xffffffffffffffffU >> (char)(uVar12 & 7));
                        LAB_7ff752944353:
                            param_1[9] = uVar22;
                            param_1[10] = uVar33;
                            return (unsigned long long)bVar26;
                        }
                    }
                    uVar32 = param_1[0xe];
                    if (uVar32 <= uVar22) {
                        uVar22 = (uVar22 + 7) & ~param_1[6];
                        uVar32 = uVar32 + 1 + param_1[6];
                        param_1[0xe] = uVar32;
                    }
                    if ((unsigned long long)param_1[0xf] < uVar32) {
                        uVar32 = param_1[0xf];
                    }
                }
                uVar9 = param_1[2] & uVar22;
                uVar12 = (unsigned int)uVar10 & 7;
                uVar22 = uVar22 + (uVar10 >> 3);
                uVar21 = (0xffffffffffffffffU >> (char)uVar12) &
                    (*(long long*)(uVar9 + lVar5) << ((0x40U - (char)uVar10) & 0x3f) | uVar21);
            } while (true);
        }
    }
    //return in_RAX & 0xffffffffffffff00;
}

//-----------------------------------------------------------------------------
// Purpose: returns the decompressed size
//-----------------------------------------------------------------------------
unsigned long long RTech::DecompressedSize(long long* param_1, long long param_2, unsigned long long param_3, unsigned long long param_4, unsigned long long param_5, unsigned long long param_6)
{
    long long lVar1;
    char sVar2;
    unsigned int uVar3;
    unsigned long long uVar4;
    unsigned int uVar5;
    unsigned int uVar6;
    unsigned long long uVar7;
    unsigned long long uVar8;
    unsigned long long uVar9;
    unsigned long long uVar10;

    *param_1 = param_2;
    param_1[5] = param_6;
    param_1[1] = 0;
    param_1[3] = 0;
    *(unsigned int*)((long long)param_1 + 0x44) = 0;
    param_1[10] = param_6;
    param_1[4] = param_4;
    param_1[2] = param_3;
    uVar4 = param_6 + 8;
    uVar7 = *(unsigned long long*)((param_3 & param_6) + param_2);
    param_1[9] = uVar4;
    uVar6 = (int)(char)uVar7 & 0x3f;
    uVar7 = uVar7 >> 6;
    sVar2 = (char)uVar6;
    uVar3 = (uVar6 - 2 & 7) + 0xd;
    uVar10 = (1 << sVar2) - 1U & uVar7 | 1 << sVar2;
    param_1[5] = uVar10;
    lVar1 = *(long long*)((uVar4 & param_3) + param_2);
    uVar4 = (uVar6 + 6 >> 3) + uVar4;
    param_1[9] = uVar4;
    uVar7 = 0xffffffffffffffffU >> ((unsigned char)(uVar6 + 6) & 7) &
        (uVar7 >> sVar2 | lVar1 << (0x3aU - sVar2 & 0x3f));
    uVar6 = (((unsigned int)uVar7 & 0xff) - 1 & 0x3f) + 1;
    uVar9 = 0xffffffffffffffff >> (-(char)uVar6 & 0x3fU);
    param_1[6] = uVar9;
    uVar8 = 0xffffffffffffffff >> (0x3f - ((char)(uVar7 >> 6) - 1U & 0x3f) & 0x3f);
    param_1[7] = uVar8;
    lVar1 = *(long long*)((uVar4 & param_3) + param_2);
    uVar4 = (uVar3 >> 3) + uVar4;
    param_1[9] = uVar4;
    if (uVar9 == 0xffffffffffffffff) {
        *(unsigned int*)(param_1 + 8) = 0;
        uVar5 = 0;
    }
    else {
        uVar6 = uVar6 >> 3;
        uVar5 = uVar6 + 1;
        *(unsigned int*)(param_1 + 8) = uVar5;
        param_4 = (1 << ((char)uVar6 * '\b' + 8U & 0x3f)) - 1U &
            *(unsigned long long*)((uVar4 & param_3) + param_2);
        param_1[9] = uVar5 + uVar4;
    }
    param_1[0xb] = param_4;
    param_1[0xe] = uVar9 - 6;
    param_1[0xc] = 0xffffffffffffffffU >> (char)(uVar3 & 7) &
        (lVar1 << (0x40U - (char)uVar3 & 0x3f) | uVar7 >> 0xd);
    *(unsigned int*)(param_1 + 0xd) = uVar3 & 7;
    *(unsigned int*)((long long)param_1 + 0x6c) = 0;
    param_1[0xf] = param_4;
    param_1[0x10] = uVar10;
    param_1[3] = 0x3fffff;
    if (uVar8 < uVar10 - 1) {
        param_1[0x10] = uVar8 + 1;
        param_1[0xf] = param_4 - uVar5;
    }
    return uVar10;
}

///////////////////////////////////////////////////////////////////////////////
RTech* g_pRtech;
