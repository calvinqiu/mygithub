#include "patch_reader.h"
#define THIS_DBGLVL         2       //0=OFF, 1=ERROR, 2=TRACE
#define __MODULE__          SampleDrv
#define __DBGLVL__          THIS_DBGLVL
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
enum { ASCII, BIN, NANO, UNKNOWN } m_format;


//charlie test
#include "Delay.h"

//#include "GPS_API.h"
#include "Type.h"
#include "NvtDrvID.h"
#include "stdio.h"


typedef struct 
{
    GlIntS32 iType;
    GlIntU8* pId;
    GlIntU32* pNumber;
    GlIntU8* pString;
    GlIntU32 uiSize;
    GlIntU8 ucDelim;
}Parameter;

GlIntU32 m_ulCrc;
GlIntU32 m_ulBlobLength;
GlIntU32 m_ulPatchAsicVersion;
GlIntU32 m_ulChangeList;
GlIntU32 m_ulPatchLevel;
GlIntU32 m_ulPostPatchLevel;
GlIntU8 m_acCustomerVersion[32];
GlIntU8 m_acPatchName[32];
GlIntU32 m_ulPatchAddress;
GlIntU32 m_ulStartOfBlob = 0;
GlIntU32 m_ulPosition = 0;
GlIntU8 m_ucByte;
GlIntU32 m_ulBlobPosition = 0;
bool m_bEmpty;
bool m_bHasCustomerVersion = false;
GlIntU32 m_ulType;

bool datago = false;
//extern CB g_CB_func;
extern unsigned char patch_data[];
extern unsigned int patch_size;
bool Patch_Initialize(void)
{
    GlIntU8 *aBuf = patch_data;//GlIntU8 aBuf[512] = {};
    GlIntU8 aFormat[] = "<FormatVersion=0x";
    GlIntU32 uIdx = 0;
    GlIntU32 uBufIdx = 0;
    GlIntU8 value[9];
    GlIntU32 i = 0;
    GlIntU32 uFormatVersion = 0;
    
    m_format = UNKNOWN;
    
    while ((aBuf[uIdx] == aFormat[uIdx]) && (aFormat[uIdx]))
    {
        ++uIdx;
    }
    
    for(i = 0; i < 8; i++)
    {
        value[i] = aBuf[uIdx++];
    }    
    value[8] = 0;
    
    if (!ReadHex(value, &uFormatVersion))
    {////DBG_ERR("51111!\n");
        ClosePatchFile();
        return false;
    }
    
    switch (uFormatVersion)
    {
        // ASCII
        case 0x00010001:
        case 0x00010003:
            m_format = ASCII;
            m_bHasCustomerVersion = (uFormatVersion == 0x00010003);
            uBufIdx = uIdx + 1; // +1 to skip trailing >
            break;

        // binary
        case 0x80010001:
        case 0x80010003:
            m_format = BIN;
            m_bHasCustomerVersion = (uFormatVersion == 0x80010003);
            uBufIdx = uIdx + 1; // +1 to skip trailing >
            break;

        default:
            m_format = UNKNOWN;
    }
    
    if (UNKNOWN == m_format)
    {
        ClosePatchFile();
        return false;
    }
    
    switch (m_format)
    {
        // For ascii (hex) files:
        case ASCII:
        {
            enum { HEX, DEC, STR };
           
            GlIntU8 cType;
            const GlIntS32 uLength = 11;
            GlIntS32 iLoop;

            // We expect to read the following fields in order.
            Parameter aArray[11] =
            {
                { DEC, (GlIntU8*)"<Crc=", &m_ulCrc},
                { DEC, (GlIntU8*)"<BlobLength=", &m_ulBlobLength},
                { HEX, (GlIntU8*)"<AsicVersion=0x", &m_ulPatchAsicVersion},
                { DEC, (GlIntU8*)"<ChangeList=", &m_ulChangeList},
                { DEC, (GlIntU8*)"<PatchLevel=", &m_ulPatchLevel},
                { DEC, (GlIntU8*)"<PostPatchLevel=", &m_ulPostPatchLevel},
                { STR, (GlIntU8*)"<CustomerVersion=", 0, m_acCustomerVersion, sizeof(m_acCustomerVersion), '>' },
                { STR, (GlIntU8*)"<Name=", 0, m_acPatchName, sizeof(m_acPatchName), '>' },
                { STR, (GlIntU8*)"<Type=", 0, &cType, 1},
                { HEX, (GlIntU8*)"<PatchAddress=0x", &m_ulPatchAddress},
                { STR, (GlIntU8*)"<Blob="}
            };

            // For each field in the header
            for (iLoop = 0; iLoop < uLength; ++iLoop)
            {
                GlIntU32 uStrIdx = 0;
                
                if (!m_bHasCustomerVersion && (iLoop == 6))
                {
                    continue;   // skip reading CustomerVersion if file format doesn't have it
                }

                // Find the point in the buffer where the field name (pId) ends, and the value begins.
                while (uBufIdx < 512)//while (uBufIdx < sizeof(aBuf))
                {
                    if (aBuf[uBufIdx++] == aArray[iLoop].pId[uStrIdx])
                    {
                        if (!aArray[iLoop].pId[++uStrIdx])
                        {
                            break;
                        }
                    }
                    else
                    {
                        uStrIdx = 0;
                    }
                }

                // We fail if we cannot find the expected field.
                if (aArray[iLoop].pId[uStrIdx])
                {
                    ClosePatchFile();
                    return false;
                }

                // Parse the value depending on the type (iType) of the field.
                if (HEX == aArray[iLoop].iType)
                {
                    if (!ReadHex(&aBuf[uBufIdx], (GlIntU32*)aArray[iLoop].pNumber))
                    {
                        ClosePatchFile();
                        return false;
                    }
                }
                else if (DEC == aArray[iLoop].iType)
                {
                    if (!ReadDec(&aBuf[uBufIdx], aArray[iLoop].pNumber))
                    {
                        ClosePatchFile();
                        return false;
                    }
                }
                else if (STR == aArray[iLoop].iType)
                {
                    uStrIdx = 0;
                    while (uStrIdx < aArray[iLoop].uiSize)
                    {
                        if (aArray[iLoop].ucDelim == aBuf[uBufIdx])
                        {
                            aArray[iLoop].pString[uStrIdx++] = 0;
                            break;
                        }
                        aArray[iLoop].pString[uStrIdx++] = aBuf[uBufIdx];
                        ++uBufIdx;
                    }
                }
            }
            m_ulType = cType;
        }
            break;
        
        default:

            ClosePatchFile();
            return false;
            break;
    }
    
    m_ulStartOfBlob = uBufIdx;

    return true;
}

bool ReadHex(GlIntU8* pAscii, GlIntU32* Value)
{
    GlIntU32 rValue = 0;
    GlIntS32 iLoop;

    for (iLoop = 0; iLoop < 8; ++iLoop)
    {
        rValue <<= 4;
        if (('0' <= pAscii[iLoop]) && (pAscii[iLoop] <= '9'))
        {
            rValue |= pAscii[iLoop] - '0';
        }
        else if (('A' <= pAscii[iLoop]) && (pAscii[iLoop] <= 'F'))
        {
            rValue |= pAscii[iLoop] - 'A' + 10;

        }
        else if (('a' <= pAscii[iLoop]) && (pAscii[iLoop] <= 'f'))
        {
            rValue |= pAscii[iLoop] - 'a' + 10;
        }
        else
        {
            return false;
        }
    }
    
    *Value = rValue;

    return true;
}

bool ReadDec(GlIntU8* pAscii, GlIntU32* Value)
{
    GlIntU32 rValue = 0;
    GlIntS32 iLoop;
    
    for (iLoop = 0; iLoop < 10; ++iLoop)
    {
        if (('0' <= pAscii[iLoop]) && (pAscii[iLoop] <= '9'))
        {
            rValue *= 10;
            rValue += pAscii[iLoop] - '0';
        }
        else
        {
            break;
        }
    }
    
    *Value = rValue;
    
    return true;
}
GlIntS32 counter=0;
GlIntS32 GetBlob(GlIntU8* aBuf, GlIntU32 iLen)
{
    if(!IsPatchFileOpened())
    {
        OpenPatchFile();

        #if 0
        m_ulPosition = 0;
        GlIntU8 aBuf[512]={0};
        while (m_ulPosition < m_ulStartOfBlob)
        {
            GlIntU32 iLen = m_ulStartOfBlob - m_ulPosition;
            if (sizeof(aBuf) < iLen)
            {
                iLen = sizeof(aBuf);
            }

            char tmp_buf[512]={0};
            UINT32 size = 512;
            g_CB_func.read(tmp_buf, (UINT32)&iLen);
            memcpy(aBuf,tmp_buf,iLen);
            counter+=iLen;

            GlIntS32 iRc = iLen;
            if (iRc <= 0)
            {
                //ClosePatchFile();
                return 0;
            }
            m_ulPosition += iRc;
        }
        #else
        if(m_ulStartOfBlob > patch_size)
            return 0;

        counter = m_ulStartOfBlob;
        m_ulPosition = m_ulStartOfBlob;
        #endif
        m_ulBlobPosition = 0;
        m_ucByte = 0;
        m_bEmpty = true;
    }
    
    // If the blob is ascii, read hex from the file and write the binary blob into the buffer provided.
    if (ASCII == m_format)
    {
        GlIntU8* ptr = aBuf;
        GlIntU32 uBufLen = iLen;
        GlIntU8 aTmp[512]={0};
        while (uBufLen)
        {
            // Make sure we don't try reading too much.
            GlIntU32 uReadLen = uBufLen << 1;
            if (sizeof(aTmp) < (GlIntU32)uReadLen)
            {
                uReadLen = sizeof(aTmp);
            }
            if ((uBufLen << 1) < uReadLen)
            {
                uReadLen = uBufLen << 1;
            }
            if (!uReadLen)
            {
                break;
            }

            #if 0
            GlIntU32 i;
            char tmp_buf[512]={0};
            UINT32 size = 512;
            g_CB_func.read(tmp_buf, (UINT32)&uReadLen);
            memcpy(aTmp,tmp_buf,uReadLen);
            #else
            if((patch_size - counter) < uReadLen)
                uReadLen = (patch_size - counter);
            memcpy(aTmp,&(patch_data[counter]),uReadLen);
            #endif

            counter+=uReadLen;

            GlIntS32 iRc = uReadLen;
            if (iRc < 0)
            {
                //ClosePatchFile();
                return 0;
            }
            uReadLen = iRc;
            if (!uReadLen)
            {
                break;
            }
            m_ulPosition += uReadLen;
           
            GlIntU8 *pSrc = &aTmp[0];

            // Decode hex and store binary into buffer provided.
            for (; pSrc < &aTmp[uReadLen]; ++pSrc)
            {
                m_ucByte <<= 4;
                if (('0' <= *pSrc) && (*pSrc <= '9'))
                {
                    m_ucByte |= *pSrc - '0';
                }
                else if (('A' <= *pSrc) && (*pSrc <= 'F'))
                {
                    m_ucByte |= *pSrc - 'A' + 10;

                }
                else if (('a' <= *pSrc) && (*pSrc <= 'f'))
                {
                    m_ucByte |= *pSrc - 'a' + 10;
                }
                else if ('\r' == *pSrc)
                {
                    continue;
                }
                else if ('\n' == *pSrc)
                {
                    continue;
                }
                else if ('>' == *pSrc)
                {
                    continue;
                }
                else
                {
                    ClosePatchFile();
                    return 0;
                }
                m_bEmpty = !m_bEmpty;
                if (m_bEmpty)
                {
                    *ptr = m_ucByte;
                    ++ptr;
                    --uBufLen;
                }
            }
        }

        iLen = ptr - aBuf;
        if (!iLen)
        {
            ClosePatchFile();
        }

        m_ulBlobPosition += iLen;
        if (m_ulBlobPosition == m_ulBlobLength)
        {
            ClosePatchFile();
        }
                
        return iLen;
    }
    else // Todo: support other formats if necessary.
    {
        ClosePatchFile();
    }
    
    return -1;
}

GlIntU32 GetAsicVersion(void)
{
    return m_ulPatchAsicVersion;
}

GlIntU32 GetChangeList(void)
{
    return m_ulChangeList;
}

GlIntU32 GetPatchLevel(void)
{
    return m_ulPatchLevel;
}

GlIntU32 GetBlobLength(void)
{
    return m_ulBlobLength;
}

GlIntU32 GetPatchAddress(void)
{
    return m_ulPatchAddress;
}

GlIntU32 GetType(void)
{
    return m_ulType;
}

void reset_patch(void)
{
    m_ulStartOfBlob = 0;
    m_ulPosition = 0;
    m_ulBlobPosition = 0;
    m_bHasCustomerVersion = false;
}