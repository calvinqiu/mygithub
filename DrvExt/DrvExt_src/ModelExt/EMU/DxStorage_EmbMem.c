#include <string.h>
#include "ErrorNo.h"
#include "Type.h"
#include "DxCfg.h"
#include "IOCfg.h"

#include "DxStorage.h"
#include "DxCommon.h"
#include "DxApi.h"

#include "nand.h"
#include "sdio.h"
#include "pad.h"
#include "StrgDef.h"

// Default debug level
#ifndef __DBGLVL__
    #define __DBGLVL__  1       // Output all message by default. __DBGLVL__ will be set to 1 via make parameter when release code.
#endif

// Default debug filter
#ifndef __DBGFLT__
    #define __DBGFLT__  "*"     // Display everything when debug level is 2
#endif

#include "DebugModule.h"
#if (_ALL_IN_ONE_TYPE_ == _EMU_LINUX_)
#include "PartitionInfo.h"
#endif

//public func
UINT32 DrvNANDGetcaps1(UINT32 CapID, UINT32 Param1); // Get Capability Flag (Base on interface version)
UINT32 DrvNANDInit(void* pInitParam); // Set Init Parameters
UINT32 DrvNANDOpen(void); // Common Constructor
UINT32 DrvNANDClose(void); // Common Destructor
UINT32 DrvNANDState(UINT32 StateID, UINT32 Value); // General Properties
UINT32 DrvNANDControl(UINT32 CtrlID, UINT32 Param1, UINT32 Param2);  // General Methods
UINT32 DrvNANDCommand(CHAR *pcCmdStr); //General Command Console

#if (_ALL_IN_ONE_TYPE_ == _EMU_LINUX_)
UINT32 DrvEmbMemGetcaps0(UINT32 CapID, UINT32 Param1); // Get Capability Flag (Base on interface version)
UINT32 DrvEmbMemGetcaps1(UINT32 CapID, UINT32 Param1); // Get Capability Flag (Base on interface version)
UINT32 DrvEmbMemGetcaps2(UINT32 CapID, UINT32 Param1); // Get Capability Flag (Base on interface version)
UINT32 DrvEmbMemGetcaps3(UINT32 CapID, UINT32 Param1); // Get Capability Flag (Base on interface version)
UINT32 DrvEmbMemGetcaps4(UINT32 CapID, UINT32 Param1); // Get Capability Flag (Base on interface version)
UINT32 DrvEmbMemGetcaps5(UINT32 CapID, UINT32 Param1); // Get Capability Flag (Base on interface version)
UINT32 DrvEmbMemGetcaps6(UINT32 CapID, UINT32 Param1); // Get Capability Flag (Base on interface version)
UINT32 DrvEmbMemInit(void* pInitParam); // Set Init Parameters
UINT32 DrvEmbMemOpen(void); // Common Constructor
UINT32 DrvEmbMemClose(void); // Common Destructor
UINT32 DrvEmbMemState(UINT32 StateID, UINT32 Value); // General Properties
UINT32 DrvEmbMemControl(UINT32 CtrlID, UINT32 Param1, UINT32 Param2);  // General Methods
UINT32 DrvEmbMemCommand(CHAR *pcCmdStr); //General Command Console
#endif




#if (_ALL_IN_ONE_TYPE_ == _EMU_LINUX_)
//dx object
DX_OBJECT gDevEmbMem0 =
{
    DXFLAG_SIGN,
    DX_CLASS_STORAGE_EXT|DX_TYPE_EMBMEM0,
    STORAGE_VER,
    "Storage_EmbMem0",
    0,0,0,0,
    DrvEmbMemGetcaps0,
    0,
    DrvEmbMemInit,
    DrvEmbMemOpen,
    DrvEmbMemClose,
    DrvEmbMemState,
    DrvEmbMemControl,
    DrvEmbMemCommand,
    0,
};

DX_OBJECT gDevEmbMem1 =
{
    DXFLAG_SIGN,
    DX_CLASS_STORAGE_EXT|DX_TYPE_EMBMEM1,
    STORAGE_VER,
    "Storage_EmbMem1",
    0,0,0,0,
    DrvEmbMemGetcaps1,
    0,
    DrvEmbMemInit,
    DrvEmbMemOpen,
    DrvEmbMemClose,
    DrvEmbMemState,
    DrvEmbMemControl,
    DrvEmbMemCommand,
    0,
};

DX_OBJECT gDevEmbMem2 =
{
    DXFLAG_SIGN,
    DX_CLASS_STORAGE_EXT|DX_TYPE_EMBMEM2,
    STORAGE_VER,
    "Storage_EmbMem2",
    0,0,0,0,
    DrvEmbMemGetcaps2,
    0,
    DrvEmbMemInit,
    DrvEmbMemOpen,
    DrvEmbMemClose,
    DrvEmbMemState,
    DrvEmbMemControl,
    DrvEmbMemCommand,
    0,
};

DX_OBJECT gDevEmbMem3 =
{
    DXFLAG_SIGN,
    DX_CLASS_STORAGE_EXT|DX_TYPE_EMBMEM3,
    STORAGE_VER,
    "Storage_EmbMem3",
    0,0,0,0,
    DrvEmbMemGetcaps3,
    0,
    DrvEmbMemInit,
    DrvEmbMemOpen,
    DrvEmbMemClose,
    DrvEmbMemState,
    DrvEmbMemControl,
    DrvEmbMemCommand,
    0,
};

DX_OBJECT gDevEmbMem4 =
{
    DXFLAG_SIGN,
    DX_CLASS_STORAGE_EXT|DX_TYPE_EMBMEM4,
    STORAGE_VER,
    "Storage_EmbMem4",
    0,0,0,0,
    DrvEmbMemGetcaps4,
    0,
    DrvEmbMemInit,
    DrvEmbMemOpen,
    DrvEmbMemClose,
    DrvEmbMemState,
    DrvEmbMemControl,
    DrvEmbMemCommand,
    0,
};

DX_OBJECT gDevEmbMem5 =
{
    DXFLAG_SIGN,
    DX_CLASS_STORAGE_EXT|DX_TYPE_EMBMEM5,
    STORAGE_VER,
    "Storage_EmbMem5",
    0,0,0,0,
    DrvEmbMemGetcaps5,
    0,
    DrvEmbMemInit,
    DrvEmbMemOpen,
    DrvEmbMemClose,
    DrvEmbMemState,
    DrvEmbMemControl,
    DrvEmbMemCommand,
    0,
};

DX_OBJECT gDevEmbMem6 =
{
    DXFLAG_SIGN,
    DX_CLASS_STORAGE_EXT|DX_TYPE_EMBMEM6,
    STORAGE_VER,
    "Storage_EmbMem6",
    0,0,0,0,
    DrvEmbMemGetcaps6,
    0,
    DrvEmbMemInit,
    DrvEmbMemOpen,
    DrvEmbMemClose,
    DrvEmbMemState,
    DrvEmbMemControl,
    DrvEmbMemCommand,
    0,
};

static UINT32 xDrvEmbMemGetUsedSize(EMB_PARTITION* pPat,UINT32 nPat)
{
    UINT32 i;
    UINT32 uiOffset = 0;
    for(i=0;i<nPat;i++)
    {
        if(pPat->EmbType != EMBTYPE_UNKNOWN)
        {
            if(pPat->PartitionOffset<uiOffset)
            {
                DBG_FATAL("Partition[%d] overlaps previous one.\r\n",i);
                return 0;
            }
            uiOffset = pPat->PartitionOffset + pPat->PartitionSize;

            //check partition size is block alignment
            if(pPat->PartitionSize & (_EMBMEM_BLK_SIZE_-1))
            {
                DBG_FATAL("Partition[%d] size must be block alignment.\r\n",i);
                return 0;
            }

            //check reversed size is block alignment
            if(pPat->ReversedSize& (_EMBMEM_BLK_SIZE_-1))
            {
                DBG_FATAL("Partition[%d] reversed size must be block alignment.\r\n",i);
                return 0;
            }
        }

        pPat++;
    }
    return uiOffset;
}

UINT32 DrvEmbMemGetcaps0(UINT32 CapID, UINT32 Param1) // Get Capability Flag (Base on interface version)
{
    UINT32 v = 0;
    switch(CapID)
    {
    case STORAGE_CAPS_HANDLE:
        v = (UINT32)nand_getStorageObject(STRG_OBJ_FW_RSV1);
        break;
    default:
        break;
    }
    return v;
}

UINT32 DrvEmbMemGetcaps1(UINT32 CapID, UINT32 Param1) // Get Capability Flag (Base on interface version)
{
    UINT32 v = 0;
    switch(CapID)
    {
    case STORAGE_CAPS_HANDLE:
        v = (UINT32)nand_getStorageObject(STRG_OBJ_FW_RSV2);
        break;
    default:
        break;
    }
    return v;
}

UINT32 DrvEmbMemGetcaps2(UINT32 CapID, UINT32 Param1) // Get Capability Flag (Base on interface version)
{
    UINT32 v = 0;
    switch(CapID)
    {
    case STORAGE_CAPS_HANDLE:
        v = (UINT32)nand_getStorageObject(STRG_OBJ_FW_RSV3);
        break;
    default:
        break;
    }
    return v;
}

UINT32 DrvEmbMemGetcaps3(UINT32 CapID, UINT32 Param1) // Get Capability Flag (Base on interface version)
{
    UINT32 v = 0;
    switch(CapID)
    {
    case STORAGE_CAPS_HANDLE:
        v = (UINT32)nand_getStorageObject(STRG_OBJ_FW_RSV4);
        break;
    default:
        break;
    }
    return v;
}

UINT32 DrvEmbMemGetcaps4(UINT32 CapID, UINT32 Param1) // Get Capability Flag (Base on interface version)
{
    UINT32 v = 0;
    switch(CapID)
    {
    case STORAGE_CAPS_HANDLE:
        v = (UINT32)nand_getStorageObject(STRG_OBJ_FW_RSV5);
        break;
    default:
        break;
    }
    return v;
}

UINT32 DrvEmbMemGetcaps5(UINT32 CapID, UINT32 Param1) // Get Capability Flag (Base on interface version)
{
    UINT32 v = 0;
    switch(CapID)
    {
    case STORAGE_CAPS_HANDLE:
        v = (UINT32)nand_getStorageObject(STRG_OBJ_FAT1);

        break;
    default:
        break;
    }
    return v;
}

UINT32 DrvEmbMemGetcaps6(UINT32 CapID, UINT32 Param1) // Get Capability Flag (Base on interface version)
{
    UINT32 v = 0;
    switch(CapID)
    {
    case STORAGE_CAPS_HANDLE:
        v = (UINT32)nand_getStorageObject(STRG_OBJ_PSTORE1);
        break;
    default:
        break;
    }
    return v;
}

UINT32 DrvEmbMemInit(void* pInitParam) // Set Init Parameters
{
    DXSTRG_INIT *g_pInit = (DXSTRG_INIT*)pInitParam; //fat and pstore not complete

    if(!g_pInit)
        return DX_PARAM_ERROR;

    DBG_FUNC_BEGIN("\r\n");

    {
        DXSTRG_PARTITION* p = &(g_pInit->prt);
        DX_HANDLE DxNandDev = Dx_GetObject(p->uiDxClassType);
        STORAGE_OBJ* pStrg = (STORAGE_OBJ *)Dx_Getcaps(DxNandDev, STORAGE_CAPS_HANDLE, 0);
        UINT32 blksize = 0;

        pStrg->Lock();

        if(p->uiDxClassType ==(DX_CLASS_STORAGE_EXT|DX_TYPE_EMBMEM0))
        {
            pStrg->SetParam(STRG_SET_MEMORY_REGION, g_pInit->buf.Addr, g_pInit->buf.Size);
            pStrg->SetParam(STRG_SET_PARTITION_SIZE,p->uiPhyAddr,p->uiPhySize);
            pStrg->GetParam(STRG_GET_SECTOR_SIZE, (UINT32)&blksize, 0); //get block size
            if(blksize==0)
            {
                DBG_ERR("get zero block size.\r\n");
                return E_PAR;
            }
            pStrg->SetParam(STRG_SET_LOADER_SECTOR_COUNT, p->uiResvSize/blksize, 0);
            if(p->uiPhyAddr!=0)
            {
                DBG_ERR("Fw area have to be at phyical address 0.\r\n");
                return E_PAR;
            }
            if(blksize!=_EMBMEM_BLK_SIZE_)
            {
                DBG_FATAL("EMBMEM_BLK_SIZE in ModelConfig set wrong, must be 0x%0X. Set it right and rebuild ModelExt and Project\r\n",blksize);
                return E_PAR;
            }

            UINT32 PhySize;
            UINT32 UsedSize = xDrvEmbMemGetUsedSize((EMB_PARTITION*)(OS_GetMemAddr(MEM_TOTAL)+EMB_PARTITION_INFO_OFFSET),EMB_PARTITION_INFO_COUNT);
            pStrg->GetParam(STRG_GET_DEVICE_PHY_SIZE, (UINT32)&PhySize, 0);

            if(UsedSize==0)
            {
                return E_PAR;
            }

            if(UsedSize > PhySize)
            {
                DBG_ERR("UsedSize (%08X) > PhySize (%08X)\r\n",UsedSize,PhySize);
                return E_PAR;
            }

        }
        else
        {
            pStrg->SetParam(STRG_SET_MEMORY_REGION, g_pInit->buf.Addr, g_pInit->buf.Size);
            pStrg->SetParam(STRG_SET_PARTITION_RSV_SIZE, p->uiResvSize,0);
            pStrg->SetParam(STRG_SET_PARTITION_SIZE,p->uiPhyAddr,p->uiPhySize);
        }

        pStrg->Unlock();
    }

    static BOOL bSpiNandInitOnce = FALSE;
    if(!bSpiNandInitOnce)
    {
        nand_setConfig(NAND_CONFIG_ID_FREQ,96000000);
        bSpiNandInitOnce = TRUE;
    }

    return DX_OK;
}

UINT32 DrvEmbMemOpen(void) // Common Constructor
{
    DBG_FUNC_BEGIN("\r\n");
    return DX_OK;
}

UINT32 DrvEmbMemClose(void) // Common Destructor
{
    DBG_FUNC_BEGIN("\r\n");
    return DX_OK;
}

UINT32 DrvEmbMemState(UINT32 StateID, UINT32 Value) // General Properties
{
    DBG_FUNC_BEGIN("\r\n");

    if(StateID & DXGET)
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
    else if(StateID & DXSET)
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

UINT32 DrvEmbMemControl(UINT32 CtrlID, UINT32 Param1, UINT32 Param2)  // General Methods
{
    DBG_FUNC_BEGIN("\r\n");
    DBG_IND("ctrl %08x\r\n", CtrlID);

    switch(CtrlID)
    {
    default:
        DBG_ERR("ctrlid=0x%02X not support!\r\n", CtrlID);
        break;
    }
    return DX_OK;
}

UINT32 DrvEmbMemCommand(CHAR *pcCmdStr) //General Command Console
{
    switch (*pcCmdStr)
    {
    case 'd':
        /*if (!strncmp(pcCmdStr, "Nand dump", 9))
        {
            return TRUE;
        }*/
        break;
    }
    return FALSE;
}
#else
//dx object
DX_OBJECT gDevEmbMem1 =
{
    DXFLAG_SIGN,
    DX_CLASS_STORAGE_EXT | DX_TYPE_EMBMEM1,
    STORAGE_VER,
    "Storage_Nand1",
    0,0,0,0,
    DrvNANDGetcaps1,
    0,
    DrvNANDInit,
    DrvNANDOpen,
    DrvNANDClose,
    DrvNANDState,
    DrvNANDControl,
    DrvNANDCommand,
    0,
};
#endif

UINT32 DrvNANDGetcaps1(UINT32 CapID, UINT32 Param1) // Get Capability Flag (Base on interface version)
{
    UINT32 v = 0;
    switch(CapID)
    {
    case STORAGE_CAPS_HANDLE:
        v = (UINT32)nand_getStorageObject(STRG_OBJ_FAT1);
        break;
    default:
        break;
    }
    return v;
}

UINT32 DrvNANDInit(void* pInitParam) // Set Init Parameters
{
#if 0
    //char *pBuf;
    UINT32 uiMemReq;
    PSTRG_TAB   pStorageObj = NULL;

    DXSTRG_INIT *g_pInit = (DXSTRG_INIT*)pInitParam;

    pStorageObj = nand_getStorageObject(STRG_OBJ_FAT1);

    if(pStorageObj == NULL)
    {
        DBG_ERR("Get NAND storage object fail!\r\n");
    }

    //(4)Set system parameter size, ver 2.00 don't need this size
    //nand_setSysParamBlockNum(0);

    //(5)After configured the NAND area, It has to open the NAND

    // for very first time nand_open let it use polling mode
    // then nand block mapping can be speed up
    // so do not change this coding if you don't have better idea.
    nand_setConfig(NAND_CONFIG_ID_SET_INTEN, FALSE);

    pStorageObj = nand_getStorageObject(STRG_OBJ_FAT1);

    pStorageObj->SetParam(STRG_SET_PARTITION_RSV_SIZE, 0x40000, 0);
    pStorageObj->SetParam(STRG_SET_PARTITION_SIZE, 0x500000, 0x1500000);

    pStorageObj->GetParam(STRG_GET_MEMORY_SIZE_REQ, (UINT32)&uiMemReq, (UINT32)0);
    if(g_pInit->buf.Size < uiMemReq)
    {
        DBG_ERR("NAND memory size too small %d < %d\r\n", g_pInit->buf.Size, uiMemReq);
        return DX_PARAM_ERROR;
    }
    pStorageObj->SetParam(STRG_SET_MEMORY_REGION, (UINT32)(g_pInit->buf.Addr), (UINT32)uiMemReq);
    pStorageObj->SetParam(STRG_SET_PARTITION_RSV_SIZE, 0x40000, 0);

    return DX_OK;
#else
    UINT32 uiNANDSize;
    UINT32 uiMemReq;
    PSTRG_TAB   pStorageObj = NULL;
    PSTRG_TAB   pStorageObjFirmware = NULL;

    DXSTRG_INIT *g_pInit = (DXSTRG_INIT*)pInitParam;

    pStorageObj = nand_getStorageObject(STRG_OBJ_FAT1);

    if(pStorageObj == NULL)
    {
        DBG_ERR("Get NAND storage object FAT fail!\r\n");
    }
    else
    {
        if(pStorageObj->GetParam(STRG_GET_DEVICE_PHY_SIZE, (UINT32)&uiNANDSize, 0) != E_OK)
        {
            DBG_ERR("Get NAND device size error\r\n");
        }
        else
        {
            DBG_DUMP("Get NAND device size 0x%08x\r\n", uiNANDSize);
        }
    }

    pStorageObjFirmware = nand_getStorageObject(STRG_OBJ_FW_RSV1);

    if(pStorageObjFirmware == NULL)
    {
        DBG_ERR("Get NAND storage object reserved area fail!\r\n");
    }

    nand_setConfig(NAND_CONFIG_ID_SET_INTEN, FALSE);

    //pStorageObj = nand_getStorageObject(STRG_OBJ_FAT1);

    if(pStorageObj)
    {
        pStorageObj->SetParam(STRG_SET_PARTITION_RSV_SIZE, 0x100000, 0);
        pStorageObj->SetParam(STRG_SET_PARTITION_SIZE, 0x700000, (uiNANDSize - 0x700000));
    }

    //Firmware area 7MB
    if(pStorageObjFirmware)
        pStorageObjFirmware->SetParam(STRG_SET_PARTITION_SIZE, 0x0, 0x700000);



    if(pStorageObj != NULL)
    {
        pStorageObj->GetParam(STRG_GET_MEMORY_SIZE_REQ, (UINT32)&uiMemReq, (UINT32)0);
        if(g_pInit->buf.Size < uiMemReq)
        {
            DBG_ERR("NAND memory size too small %d < %d\r\n", g_pInit->buf.Size, uiMemReq);
            return DX_PARAM_ERROR;
        }
        else
        {
            DBG_DUMP("NAND memory %d <=> req size %d\r\n", g_pInit->buf.Size, uiMemReq);
        }

        pStorageObj->SetParam(STRG_SET_MEMORY_REGION, (UINT32)(g_pInit->buf.Addr), (UINT32)uiMemReq);
        pStorageObj->SetParam(STRG_SET_PARTITION_RSV_SIZE, 0x40000, 0);
    }
    else
    {
        //#NT#2015/10/20#Steven Wang -begin
        //#NT#Once if pStorageObj == NULL && pStorageObjFirmware == NULL) return error
        if(pStorageObjFirmware == NULL)
            return DX_NULL_POINTER;
        //#NT#2015/10/20#Steven Wang -end
    }
    return DX_OK;


#endif
}

UINT32 DrvNANDOpen(void) // Common Constructor
{
    return DX_OK;
}

UINT32 DrvNANDClose(void) // Common Destructor
{
    return DX_OK;
}

UINT32 DrvNANDState(UINT32 StateID, UINT32 Value) // General Properties
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

UINT32 DrvNANDControl(UINT32 CtrlID, UINT32 Param1, UINT32 Param2)  // General Methods
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

UINT32 DrvNANDCommand(CHAR *pcCmdStr) //General Command Console
{
    return FALSE;
}
