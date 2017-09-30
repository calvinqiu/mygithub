#include <string.h>
#include "ErrorNo.h"
#include "Type.h"

#include "DxStorage.h"
#include "DxCommon.h"

#include "StrgDef.h"

#include "RamDisk.h"

// Default debug level
#ifndef __DBGLVL__
    #define __DBGLVL__  1       // Output all message by default. __DBGLVL__ will be set to 1 via make parameter when release code.
#endif

// Default debug filter
#ifndef __DBGFLT__
    #define __DBGFLT__  "*"     // Display everything when debug level is 2
#endif

#include "DebugModule.h"


//internal func
static UINT32 DrvRamdiskGetcaps1(UINT32 CapID, UINT32 Param1);  // Get Capability Flag (Base on interface version);
static UINT32 DrvRamdiskInit(void* pInitParam);                 // Set Init Parameters
static UINT32 DrvRamdiskOpen(void);                             // Common Constructor
static UINT32 DrvRamdiskClose(void);                            // Common Destructor
static UINT32 DrvRamdiskState(UINT32 StateID, UINT32 Value);    // General Properties
static UINT32 DrvRamdiskControl(UINT32 CtrlID, UINT32 Param1, UINT32 Param2);  // General Methods
static UINT32 DrvRamdiskCommand(CHAR *pcCmdStr);                //General Command Console

//dx object
DX_OBJECT gDevRamdisk0 =
{
    DXFLAG_SIGN,
    DX_CLASS_STORAGE_EXT | DX_TYPE_RAMDISK0,
    STORAGE_VER,
    "Storage_Ramdisk1",
    0,0,0,0,
    DrvRamdiskGetcaps1,
    0,
    DrvRamdiskInit,
    DrvRamdiskOpen,
    DrvRamdiskClose,
    DrvRamdiskState,
    DrvRamdiskControl,
    DrvRamdiskCommand,
    0,
};


static UINT32 DrvRamdiskGetcaps1(UINT32 CapID, UINT32 Param1) // Get Capability Flag (Base on interface version)
{
    UINT32 v = 0;
    switch(CapID)
    {
    case STORAGE_CAPS_HANDLE:
        v = (UINT32)ramdsk_getStorageObject();
        break;
    default:
        break;
    }
    return v;
}

static UINT32 DrvRamdiskInit(void* pInitParam) // Set Init Parameters
{
    const UINT32 uiMemReq = 0x200000;       // 2MB ramdisk for USB emulation
    PSTRG_TAB   pStorageObj = NULL;

    DXSTRG_INIT *g_pInit = (DXSTRG_INIT*)pInitParam; //fat and pstore not complete

    pStorageObj = ramdsk_getStorageObject();

    if(pStorageObj == NULL)
    {
        DBG_ERR("Get Ramdisk storage object fail!\r\n");
    }

    // so do not change this coding if you don't have better idea.
    if(g_pInit->buf.Size < uiMemReq)
    {
        DBG_ERR("Ramdisk memory size too small %d < %d\r\n", g_pInit->buf.Size, uiMemReq);
        return DX_PARAM_ERROR;
    }
    pStorageObj->SetParam(STRG_SET_MEMORY_REGION, (g_pInit->buf.Addr), uiMemReq);

    return DX_OK;
}

static UINT32 DrvRamdiskOpen(void) // Common Constructor
{
    return DX_OK;
}

static UINT32 DrvRamdiskClose(void) // Common Destructor
{
    return DX_OK;
}

static UINT32 DrvRamdiskState(UINT32 StateID, UINT32 Value) // General Properties
{
    if (StateID & DXGET)
    {
        UINT32 rvalue =  0;
        StateID &= ~DXGET;
        DBG_IND("get %08x\r\n", StateID);
        switch(StateID)
        {
        default:
            DBG_ERR("state=0x%02X not support!\r\n", StateID);
            break;
        }
        return rvalue;
    }
    else if (StateID & DXSET)
    {
        StateID &= ~DXSET;
        DBG_IND("set %08x\r\n", StateID);
        switch(StateID)
        {
        default:
            DBG_ERR("state=0x%02X not support!\r\n", StateID);
            break;
        }
    }
    return DX_OK;
}

static UINT32 DrvRamdiskControl(UINT32 CtrlID, UINT32 Param1, UINT32 Param2)  // General Methods
{
    DBG_IND("ctrl %08x\r\n", CtrlID);

    switch(CtrlID)
    {
    default:
        DBG_ERR("ctrlid=0x%02X not support!\r\n", CtrlID);
        break;
    }
    return DX_OK;
}

static UINT32 DrvRamdiskCommand(CHAR *pcCmdStr) //General Command Console
{
    return FALSE;
}


