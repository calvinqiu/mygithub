/*
    System Main Callback

    System Callback for System Module.

    @file       SysMain_Exe.c
    @ingroup    mIPRJSYS

    @note

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIFrameworkExt.h"
#include "UIBackgroundObj.h"
#include "UICommon.h"
#include "AppLib.h"


//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
#include "SysCfg.h"
#include "PStore.h"
#if (BT_FUNC == ENABLE)
#include "BT_API.h"
#endif

#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysBT
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////


#if (BT_FUNC == ENABLE)
#define  PS_BT    "BT_PSINFO"

// RC Command/Data Code definitions
#define RC_MAX_CMD_STR_LENGTH              16
#define RC_MAX_CMD_ENTRY_NUM               4

#define C10_CMD_KEY_S2                  0x40
#define C10_CMD_KEY_REC                 0x80
#define C10_CMD_KEY_REL                 0x00


typedef struct
{
    UINT32          uiCmdCode;
    UINT32          uiMappedKey;
    UINT32          uiMappedLongKey;
    CHAR            CmdString[RC_MAX_CMD_STR_LENGTH];
} RC_CODE_CMD, *PRC_CODE_CMD;

typedef struct
{
    RC_CODE_CMD     CmdCodeSet[RC_MAX_CMD_ENTRY_NUM];
} RC_CODE_TABLE, *PRC_CODE_TABLE;



static RC_CODE_TABLE RcCodeTb_c10 =
{
    {
        { C10_CMD_KEY_S2,           NVTEVT_KEY_RC_SHUTTER2,   NVTEVT_KEY_RC_SHUTTER2_LONG,    "S2"         },
        { C10_CMD_KEY_REC,          NVTEVT_KEY_RC_MOVIEREC,   NVTEVT_KEY_RC_MOVIEREC_LONG,    "MovieREC"   },
        { C10_CMD_KEY_REL,          NVTEVT_KEY_RELEASE,       NVTEVT_KEY_RELEASE,             "Release"    },
        { 0, 0, 0, "" }
    }
};

static RC_CODE_TABLE *g_pRcCodeTb = (PRC_CODE_TABLE)&RcCodeTb_c10;

static BOOL   isRckeyPress = FALSE;
static BOOL   isRckeyLongPress = FALSE;
static UINT32 shortkeyEvent = 0;
static UINT32 longkeyEvent = 0;
static SWTIMER_ID timerid;

static void BT_LoadInfo(UINT32 Addr, UINT32 Size)
{
    PPSTORE_SECTION_HANDLE  pSection;

    DBG_IND("\r\n");
    #if PST_FUNC
    if ((pSection = PStore_OpenSection(PS_BT, PS_RDWR)) != E_PS_SECHDLER)
    {
        PStore_ReadSection((UINT8 *)Addr, 0, Size, pSection);
        PStore_CloseSection(pSection);
        DBG_IND("Load BTInfo Success\r\n");
    }
    else
    #endif
    {
        memset((UINT8 *)Addr, 0, Size);
    }
}

static void BT_SaveInfo(UINT32 Addr, UINT32 Size)
{
    PPSTORE_SECTION_HANDLE  pSection;

    DBG_IND("\r\n");
    #if PST_FUNC
    //save data to PStore
    if ((pSection = PStore_OpenSection(PS_BT, PS_RDWR | PS_CREATE)) != E_PS_SECHDLER)
    {
        PStore_WriteSection((UINT8 *)Addr, 0, Size, pSection);
        PStore_CloseSection(pSection);
        DBG_IND("Save BTInfo Success\r\n");
    }
    #endif
}

static void xBT_RcKeyTimerCb(UINT32 uiEvent)
{
    if (isRckeyPress)
    {
        // this is long key press
        isRckeyLongPress = TRUE;
        Ux_PostEvent(longkeyEvent, 1, NVTEVT_KEY_PRESS);
        DBG_IND("long key event 0x%x\r\n", longkeyEvent);
    }
}
static void  BT_RcKeyCB(UINT8* KeyData, UINT32 DataLen)
{
    UINT32 i;
    DBG_IND("DataLen = %d, KeyCode",DataLen);
    for (i=0;i<DataLen;i++)
    {
        DBG_MSG(" 0x%02x",KeyData[i]);
    }
    DBG_MSG("\r\n");
    for (i=0;i<RC_MAX_CMD_ENTRY_NUM;i++)
    {
        if(g_pRcCodeTb->CmdCodeSet[i].uiMappedKey == 0)
            break;
        if(KeyData[0]  == g_pRcCodeTb->CmdCodeSet[i].uiCmdCode)
        {
            if (KeyData[0] == C10_CMD_KEY_REL)
            {
                isRckeyPress = FALSE;
                if (!isRckeyLongPress)
                {
                    // this is short key press
                    Ux_PostEvent(shortkeyEvent, 1, NVTEVT_KEY_PRESS);
                    DBG_IND("short key event 0x%x\r\n", shortkeyEvent);
                }
                isRckeyLongPress = FALSE;
                SwTimer_Stop(timerid);
                SwTimer_Close(timerid);
            }
            else
            {
                isRckeyPress = TRUE;
                shortkeyEvent = g_pRcCodeTb->CmdCodeSet[i].uiMappedKey;
                longkeyEvent = g_pRcCodeTb->CmdCodeSet[i].uiMappedLongKey;
                SwTimer_Open(&timerid,xBT_RcKeyTimerCb);
                SwTimer_Cfg(timerid,500,SWTIMER_MODE_ONE_SHOT);
                SwTimer_Start(timerid);
            }
            DBG_IND("Rc: Parse key: %s\r\n", g_pRcCodeTb->CmdCodeSet[i].CmdString);
            break;
        }
    }

}

static void BT_Print_Rx_Data(UINT8 *p_data, UINT16 len)
{
    UINT16 i, j, offset = 0;

    /* Display incoming data */
    while (len > 0)
    {
        DBG_MSG("   %04X: ", offset);

        j = 16;
        if (len < 16)
            j = len;

        for (i=0; i<j; i++)
        {
            DBG_MSG("%02X ", ((UINT8 *)p_data)[offset+i]);
        }

        DBG_MSG("\r\n");
        offset += j;
        len -= j;
    }
}
void BT_RfcommRxCB(UINT16 port_handle, void *p_data, UINT16 len)
{
    UINT16 byteWriten = 0;

    BT_Print_Rx_Data(p_data, len);

    BT_Rfcomm_WriteData(port_handle, p_data, len , &byteWriten);
}

#endif
void System_OnBTInit(void)
{
    DBG_IND("\r\n");
#if (BT_FUNC == ENABLE)
    BT_OPEN      btOpen={0};
    btOpen.loadInfo = BT_LoadInfo;
    btOpen.saveInfo = BT_SaveInfo;
    btOpen.rckeyCB = BT_RcKeyCB;
    btOpen.rfcommRxCB = BT_RfcommRxCB;
    BT_Open(&btOpen);
#endif
}


void System_OnBTExit(void)
{
#if (BT_FUNC == ENABLE)
    BT_Close();
#endif
}

