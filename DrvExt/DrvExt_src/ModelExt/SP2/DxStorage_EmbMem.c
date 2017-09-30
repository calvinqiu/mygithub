#include "Type.h"
#include "DxCfg.h"
#include "IOCfg.h"

#include "DxStorage.h"
#include "DxCommon.h"
#include "DxApi.h"

#include "Debug.h"
#include "sdio.h"
#include "pad.h"
#include "StrgDef.h"
#include <string.h>
#include "PartitionInfo.h"
#include "spiflash.h"

#if (_INTERSTORAGE_ == _INTERSTORAGE_NONE_)
#define EMB_GETSTRGOBJ(x) 0 //Always NULL
#elif (_INTERSTORAGE_ == _INTERSTORAGE_NAND_ || _INTERSTORAGE_ == _INTERSTORAGE_SPINAND_)
#include "nand.h"
#define EMB_GETSTRGOBJ(x) nand_getStorageObject(x)
#elif (_INTERSTORAGE_ == _INTERSTORAGE_SPI_)
#include "spiflash.h"
#include "RamDisk.h"
#define EMB_GETSTRGOBJ(x) spiflash_getStorageObject(x)
#elif (_INTERSTORAGE_ == _INTERSTORAGE_EMMC_)
#include "sdio.h"
#define EMB_GETSTRGOBJ(x) sdio3_getStorageObject(x)
static BOOL xSdioDet(void){return TRUE;}
static BOOL xSdioWp(void){return FALSE;};
#endif

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxDrv
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

//public func
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

static BOOL pmc_identifySpi(UINT32 uiMfgID, UINT32 uiTypeID, UINT32 uiCapacity, PSPIFLASH_IDENTIFY pIdentify)
{
    // sample code to extend SPI flash supporting
    if ((uiMfgID == 0xC2) && (uiTypeID == 0x20) && (uiCapacity == 0x17)) //MXIC
    {
        pIdentify->uiTotalSize = 8*1024*1024;
        pIdentify->uiSectorSize = 0x1000;
        pIdentify->bSupportEWSR = FALSE;
        pIdentify->bSupportAAI = FALSE;
        pIdentify->bSupportSecErase = TRUE;
        pIdentify->uiSectorEraseTime = 200;
        pIdentify->uiPageProgramTime = 5;
        pIdentify->flashWidth = SPI_FLASH_BUSWIDTH_NORMAL | SPI_FLASH_BUSWIDTH_DUAL;

        return TRUE;
    }
    else if ((uiMfgID == 0xBF) && (uiTypeID == 0x25) && (uiCapacity == 0x4A))
    {
        pIdentify->uiTotalSize = 4*1024*1024;
        pIdentify->uiSectorSize = 0x1000;
        pIdentify->bSupportEWSR = TRUE;
        pIdentify->bSupportAAI = TRUE;
        pIdentify->bSupportSecErase = TRUE;
        pIdentify->uiSectorEraseTime = 2500;
        pIdentify->uiPageProgramTime = 1;

        return TRUE;
    }

    return FALSE;
}

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
    PSTORAGE_OBJ pStrgObj = NULL;
    switch(CapID)
    {
    case STORAGE_CAPS_HANDLE:
        v = (UINT32)EMB_GETSTRGOBJ(STRG_OBJ_FW_RSV1);
        pStrgObj = (PSTORAGE_OBJ)v;
        pStrgObj->ExtIOCtrl(STRG_EXT_CMD_SPI_IDENTIFY_CB, (UINT32)pmc_identifySpi, 0);
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
        v = (UINT32)EMB_GETSTRGOBJ(STRG_OBJ_FW_RSV2);
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
        v = (UINT32)EMB_GETSTRGOBJ(STRG_OBJ_FW_RSV3);
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
        v = (UINT32)EMB_GETSTRGOBJ(STRG_OBJ_FW_RSV4);
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
        v = (UINT32)EMB_GETSTRGOBJ(STRG_OBJ_FW_RSV5);
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
#if (_INTERSTORAGE_ == _INTERSTORAGE_SPI_)
        v = (UINT32)ramdsk_getStorageObject();
#else
        v = (UINT32)EMB_GETSTRGOBJ(STRG_OBJ_FAT1);
#endif
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
        v = (UINT32)EMB_GETSTRGOBJ(STRG_OBJ_PSTORE1);
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

#if (_INTERSTORAGE_ == _INTERSTORAGE_EMMC_)
    static BOOL bEmmcInitOnce = FALSE;
    if(!bEmmcInitOnce)
    {
        sdio3_setCallBack(SDIO_CALLBACK_CARD_DETECT, (SDIO_CALLBACK_HDL)xSdioDet);
        sdio3_setCallBack(SDIO_CALLBACK_WRITE_PROTECT, (SDIO_CALLBACK_HDL)xSdioWp);
#if (_FPGA_EMULATION_ == ENABLE)
        // Support default speed only
        sdio3_setConfig(SDIO_CONFIG_ID_BUS_SPEED_MODE,          SDIO_BUS_SPEED_MODE_DS);
        // Source clock 96 MHz can run faster than default value
        sdio3_setConfig(SDIO_CONFIG_ID_SRCLK,                   SDIO_SRC_CLK_96MHZ);
#else
        // Support default speed + high speed
        sdio3_setConfig(SDIO_CONFIG_ID_BUS_SPEED_MODE,          SDIO_BUS_SPEED_MODE_DS | SDIO_BUS_SPEED_MODE_HS);
        // Default speed driving -> 5.0 mA
        sdio3_setConfig(SDIO_CONFIG_ID_DS_DRIVING,              50);
        // High speed driving    -> 10.0 mA
        sdio3_setConfig(SDIO_CONFIG_ID_HS_DRIVING,              100);
        // Adjust driving/sink according to operating mode
        sdio3_setConfig(SDIO_CONFIG_ID_DRIVING_SINK_EN,         TRUE);
#endif
        sdio3_setConfig(SDIO_CONFIG_ID_BUS_WIDTH,               SD_HOST_BUS_4_BITS);
        sdio3_setConfig(SDIO_CONFIG_ID_FORMAD_OEMID,            (UINT32)"NOVATEK");
        sdio3_setConfig(SDIO_CONFIG_ID_FORMAD_VOL_LABLE,        (UINT32)"SD_VOL");
        sdio3_setConfig(SDIO_CONFIG_ID_FORMAD_RAND_VOL_ID_EN,   TRUE);
        bEmmcInitOnce = TRUE;
    }
#elif (_INTERSTORAGE_ == _INTERSTORAGE_SPINAND_)
    static BOOL bSpiNandInitOnce = FALSE;
    if(!bSpiNandInitOnce)
    {
        nand_setConfig(NAND_CONFIG_ID_FREQ,96000000);
        bSpiNandInitOnce = TRUE;
    }
#endif
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


