////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "SysKer.h"
#include "MsdcNvtApi.h"
#include "MsdcNvtCallback.h"
#include "MsdcNvtCb_UpdFw.h"
#include "FwSrvApi.h"
#include "FileSysTsk.h"
#include "PartLoad.h"
#include "HwPower.h"
#include "GxLinuxApi.h"
#include "AppControl.h"
#include "SysMain.h"
#include <string.h>

#if (_MODEL_DSC_ == _MODEL_EVB_)
#define CFG_SAFE_POWER_OFF DISABLE //because EVB no rtc power
#else
#define CFG_SAFE_POWER_OFF ENABLE //if disalbe it, use watch dog to reset system, but it may cause damage on peripheral.
#endif

#define THIS_DBGLVL         1       ///< 0=OFF, 1=ERROR, 2=TRACE
#define __MODULE__          MsdcNvt_UpdFw
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

//for MSDCVendorNVT_AddCallback_Bi
static void MsdcNvtCb_UpdFwGetInfo(void* pData);
static void MsdcNvtCb_UpdFwGetWorkMem(void* pData);
static void MsdcNvtCb_UpdFwSetBlock(void* pData);
static void MsdcNvtCb_UpdFwAllInOne_Step1(void* pData);
static void MsdcNvtCb_UpdFwAllInOne_Step2(void* pData);

MSDCNVT_REG_BI_BEGIN(m_MsdcNvtUpdfw)
MSDCNVT_REG_BI_ITEM(MsdcNvtCb_UpdFwGetInfo)
MSDCNVT_REG_BI_ITEM(MsdcNvtCb_UpdFwGetWorkMem)
MSDCNVT_REG_BI_ITEM(MsdcNvtCb_UpdFwSetBlock)
MSDCNVT_REG_BI_ITEM(MsdcNvtCb_UpdFwAllInOne_Step1)
MSDCNVT_REG_BI_ITEM(MsdcNvtCb_UpdFwAllInOne_Step2)
MSDCNVT_REG_BI_END()

BOOL MsdcNvtRegBi_UpdFw(void)
{
    return MsdcNvt_AddCallback_Bi(m_MsdcNvtUpdfw);
}

static void MsdcNvtCb_UpdFwGetInfo(void* pData)
{
    UINT32 uiBlkSize;
    FWSRV_CMD Cmd = {0};
    tMSDCEXT_UPDFW_GET_BLOCK_INFO* pDesc = MSDCNVT_CAST(tMSDCEXT_UPDFW_GET_BLOCK_INFO,pData);
    if(pDesc==NULL)
        return;

    Cmd.Idx = FWSRV_CMD_IDX_GET_BLK_SIZE;
    Cmd.Out.pData = &uiBlkSize;
    Cmd.Out.uiNumByte = sizeof(uiBlkSize);
    Cmd.Prop.bExitCmdFinish=TRUE;
    if(FwSrv_Cmd(&Cmd)==FWSRV_ER_OK)
    {
        pDesc->uiBytesPerBlock = uiBlkSize;
        pDesc->tParent.bOK = TRUE;
    }
    else
    {
        debug_msg("MsdcNvtCb_UpdFwGetInfo: FwSrv_Cmd Failed\r\n!");
    }
}

static void MsdcNvtCb_UpdFwGetWorkMem(void* pData)
{
    PARTLOAD_INIT Init={0};
    tMSDCEXT_UPDFW_GET_WORK_MEM* pDesc = MSDCNVT_CAST(tMSDCEXT_UPDFW_GET_WORK_MEM,pData);
    if(pDesc==NULL)
        return;

    PartLoad_GetInit(&Init);
    pDesc->uiAddr = Init.uiWorkingAddr;
    pDesc->uiSize = Init.uiWorkingSize;

    pDesc->tParent.bOK = TRUE;
}

static void MsdcNvtCb_UpdFwSetBlock(void* pData)
{
    UINT32 uiBlkSize;
    FWSRV_BLK_DESC BlkInfo={0};
    FWSRV_CMD Cmd = {0};
    PARTLOAD_INIT Init={0};
    tMSDCEXT_UPDFW_SET_BLOCK_INFO* pDesc = MSDCNVT_CAST(tMSDCEXT_UPDFW_SET_BLOCK_INFO,pData);

    if(pDesc==NULL)
        return;

    if(pDesc->uiKey!=MAKEFOURCC('N','O','V','A'))
    {
        debug_msg("MsdcNvtCb_UpdFwSetBlock: Invalid Key\r\n!");
        return;
    }

    PartLoad_GetInit(&Init);

    Cmd.Idx = FWSRV_CMD_IDX_GET_BLK_SIZE;
    Cmd.Out.pData = &uiBlkSize;
    Cmd.Out.uiNumByte = sizeof(uiBlkSize);
    Cmd.Prop.bExitCmdFinish=TRUE;
    if(FwSrv_Cmd(&Cmd)!=FWSRV_ER_OK)
    {
        return;
    }

    memset(&Cmd,0,sizeof(Cmd));
    BlkInfo.Area = FWSRV_AREA_FIRMWARE;
    BlkInfo.uiIdx = pDesc->iBlock;
    BlkInfo.Buf.uiAddr = Init.uiWorkingAddr;
    BlkInfo.Buf.uiSize = uiBlkSize;
    Cmd.Idx = FWSRV_CMD_IDX_BLK_WRITE;
    Cmd.In.pData = &BlkInfo;
    Cmd.In.uiNumByte = sizeof(BlkInfo);
    Cmd.Prop.bExitCmdFinish=TRUE;
    if(FwSrv_Cmd(&Cmd)!=FWSRV_ER_OK)
    {
        return;
    }

    if(pDesc->IsLastBlock)
    {
#if (USB_MODE == ENABLE)
        if(System_GetState(SYS_STATE_CURRMODE)==PRIMARY_MODE_USBMSDC)
        {
            //because we need to kill old fw in SD, so don't want pc accessing sd card.
            Msdc_SetLunConfig(0,USBMSDC_LUN_CONFIG_ID_WRITEPROTECT,TRUE);
            //Mount SD by uITRON
            // coverity[var_deref_model]
            GxStrg_SetConfig(FILE_CFG_FS_TYPE,GXSTRG_FS_TYPE_UITRON);
            if(GxStrg_OpenDevice(0, Dx_GetObject(DX_CLASS_STORAGE_EXT|DX_TYPE_CARD1))==TRUE)
            {
                FileSys_WaitFinish();
            }
            //No need to stop cpu2, because it has closed whiling changing to msdc mode.
        }
#endif
        //Delete Firmware in Cards
        DBG_DUMP("\r\nDelete firmware in A:\\ \r\n");
        if(FileSys_DeleteFile("A:\\"_BIN_NAME_T_".BIN")!=FST_STA_OK)
        {
            DBG_IND("Ignore deleting file.\r\n");
        }

        if(FileSys_DeleteFile("A:\\"_BIN_NAME_".BIN")!=FST_STA_OK)
        {
            DBG_IND("Ignore deleting file.\r\n");
        }

        #if(_CPU2_TYPE_ == _CPU2_LINUX_)
        debug_msg("\r\nClosing Linux\r\n");
        GxLinux_Close();
        #endif
        HwPower_SetPowerKey(POWER_ID_SWRT,0xFF);
    }

    pDesc->tParent.bOK = TRUE;

}

static void MsdcNvtCb_UpdFwAllInOne_Step1(void* pData)
{
    tMSDCEXT_UPDFWALLINONE_STEP1* pDesc = MSDCNVT_CAST(tMSDCEXT_UPDFWALLINONE_STEP1,pData);
    if(pDesc==NULL)
        return;

#if (USB_MODE == ENABLE)
    //Change Mode to Main for Free Memory
    if(System_GetState(SYS_STATE_CURRMODE)!=PRIMARY_MODE_USBMSDC)
    {
        if(System_GetState(SYS_STATE_CURRMODE)!=PRIMARY_MODE_MAIN)
        {
            INT32 nRetry = 20;
            Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_MAIN);
            //polling to wait finish
            while(System_GetState(SYS_STATE_CURRMODE)!=PRIMARY_MODE_MAIN && nRetry-->0)
            {
                Delay_DelayMs(500);
            }
        }

        if(System_GetState(SYS_STATE_CURRMODE)!=PRIMARY_MODE_MAIN)
        {
            pDesc->tParent.bOK = FALSE;
            DBG_ERR("cannot change mode to MAIN.\r\n");
            return;
        }

        //Delete Firmware in Cards
        DBG_DUMP("\r\nDelete firmware in A:\\ \r\n");
        if(FileSys_DeleteFile("A:\\"_BIN_NAME_T_".BIN")!=FST_STA_OK)
        {
            DBG_IND("Ignore deleting file.\r\n");
        }

        if(FileSys_DeleteFile("A:\\"_BIN_NAME_".BIN")!=FST_STA_OK)
        {
            DBG_IND("Ignore deleting file.\r\n");
        }

        //#NT#2016/08/29#Niven Cho -begin
        //#NT#ETH-Update FW
        //System_CPU2_Stop();
        //#NT#2016/08/29#Niven Cho -end
    }
    else
    {
        //because we need to kill old fw in SD, so don't want pc accessing sd card.
        Msdc_SetLunConfig(0,USBMSDC_LUN_CONFIG_ID_WRITEPROTECT,TRUE);
        //Mount SD by uITRON
        // coverity[var_deref_model]
        GxStrg_SetConfig(FILE_CFG_FS_TYPE,GXSTRG_FS_TYPE_UITRON);
        if(GxStrg_OpenDevice(0, Dx_GetObject(DX_CLASS_STORAGE_EXT|DX_TYPE_CARD1))==TRUE)
        {
            FileSys_WaitFinish();
        }
        //No need to stop cpu2, because it has closed whiling changing to msdc mode.

        //Delete Firmware in Cards
        DBG_DUMP("\r\nDelete firmware in A:\\ \r\n");
        if(FileSys_DeleteFile("A:\\"_BIN_NAME_T_".BIN")!=FST_STA_OK)
        {
            DBG_IND("Ignore deleting file.\r\n");
        }

        if(FileSys_DeleteFile("A:\\"_BIN_NAME_".BIN")!=FST_STA_OK)
        {
            DBG_IND("Ignore deleting file.\r\n");
        }

    }
#else
    {
        if(System_GetState(SYS_STATE_CURRMODE)!=PRIMARY_MODE_MAIN)
        {
            INT32 nRetry = 20;
            Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_MAIN);
            //polling to wait finish
            while(System_GetState(SYS_STATE_CURRMODE)!=PRIMARY_MODE_MAIN && nRetry-->0)
            {
                Delay_DelayMs(500);
            }
        }

        if(System_GetState(SYS_STATE_CURRMODE)!=PRIMARY_MODE_MAIN)
        {
            pDesc->tParent.bOK = FALSE;
            DBG_ERR("cannot change mode to MAIN.\r\n");
            return;
        }

        //Delete Firmware in Cards
        DBG_DUMP("\r\nDelete firmware in A:\\ \r\n");
        if(FileSys_DeleteFile("A:\\"_BIN_NAME_T_".BIN")!=FST_STA_OK)
        {
            DBG_IND("Ignore deleting file.\r\n");
        }

        if(FileSys_DeleteFile("A:\\"_BIN_NAME_".BIN")!=FST_STA_OK)
        {
            DBG_IND("Ignore deleting file.\r\n");
        }
        //#NT#2016/08/29#Niven Cho -begin
        //#NT#ETH-Update FW
        //System_CPU2_Stop();
        //#NT#2016/08/29#Niven Cho -end
    }
#endif

    pDesc->uiAddr = OS_GetMempoolAddr(POOL_ID_APP)+MSDC_MIN_BUFFER_SIZE;
    pDesc->uiSize = OS_GetMempoolSize(POOL_ID_APP)-MSDC_MIN_BUFFER_SIZE;

    pDesc->tParent.bOK = TRUE;
}

static void MsdcNvtCb_UpdFwAllInOne_Step2(void* pData)
{
    FWSRV_CMD Cmd = {0};
    tMSDCEXT_UPDFWALLINONE_STEP2* pDesc = MSDCNVT_CAST(tMSDCEXT_UPDFWALLINONE_STEP2,pData);

    if(pDesc==NULL)
        return;

    if(pDesc->uiKey!=MAKEFOURCC('N','O','V','A'))
    {
        debug_msg("MsdcNvtCb_UpdFwSetBlock: Invalid Key\r\n!");
        return;
    }

    //#NT#2016/08/29#Niven Cho -begin
    //#NT#ETH-Update FW
    //must close CPU2 because root-fs updated for condition CPU2 OFF.
    System_CPU2_Stop();
    //#NT#2016/08/29#Niven Cho -end

    FWSRV_BIN_UPDATE_ALL_IN_ONE FwInfo = {0};
    FwInfo.uiSrcBufAddr = OS_GetMempoolAddr(POOL_ID_APP)+MSDC_MIN_BUFFER_SIZE;
    FwInfo.uiSrcBufSize = pDesc->EffectDataSize;
    FwInfo.uiWorkBufAddr = FwInfo.uiSrcBufAddr+FwInfo.uiSrcBufSize;
    FwInfo.uiWorkBufSize = OS_GetMempoolAddr(POOL_ID_APP)-MSDC_MIN_BUFFER_SIZE-FwInfo.uiSrcBufSize;
    FwInfo.fpItemCb = NULL;
    memset(&Cmd,0,sizeof(Cmd));
    Cmd.Idx = FWSRV_CMD_IDX_BIN_UPDATE_ALL_IN_ONE;
    Cmd.In.pData = &FwInfo;
    Cmd.In.uiNumByte = sizeof(FwInfo);
    Cmd.Prop.bExitCmdFinish = TRUE;
    FWSRV_ER fws_er = FwSrv_Cmd(&Cmd);
    if(fws_er != FWSRV_ER_OK)
    {
        pDesc->tParent.bOK = FALSE;
        DBG_ERR("FW bin write failed %d\r\n",fws_er);
        return;
    }

#if (CFG_SAFE_POWER_OFF == ENABLE)
    GxSystem_EnableHWReset(0);
#else
    GxSystem_EnableSWReset(0);
    DBG_WRN("Using SW Reset, it might be not safe for peripheral.\r\n");
#endif

    Ux_PostEvent(NVTEVT_SYSTEM_SHUTDOWN, 1, 0);

    pDesc->tParent.bOK = TRUE;
}
