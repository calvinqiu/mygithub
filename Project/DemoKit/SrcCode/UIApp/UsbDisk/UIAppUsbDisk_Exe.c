
//#NT#2010/07/29#Lily Kao -begin

/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       AppInitUSB.c
    @ingroup    mIPRJAPCfg

    @brief      Appication initialization for USB mode
                Get memory and set parameters for USB mode

    @note       Nothing.

    @date       2010/07/29
*/

/** \addtogroup mIPRJAPCfg */
//@{

////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "GxDisplay.h"
#include "UIAppUsbDisk.h"
#include "AppLib.h"
#include "UIInfo.h"

#include "PlaybackTsk.h"
#include "DxType.h"
#include "DxStorage.h"
#include "MsdcNvtCb.h"
//#NT#2017/03/07#Niven Cho[117156] -begin
//#NT#FS_SWITCH_STRG_FUNC
#include "DxApi.h"
//#NT#2017/03/07#Niven Cho[117156] -end

//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UIAppUsbDiskExe
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////
#if (SDHOTPLUG_FUNCTION == ENABLE)
#include "UIStorageCheck.h"
static BOOL m_bIsMsdcOpened = FALSE;
#endif
//#NT#2009/07/01#Brad Chen -begin
//#NT#Add to support MSDC CDROM function
static DX_HANDLE pLastStrgDev = 0;
//#NT#2009/07/01#Brad Chen -end

//#NT#2010/10/21#Lily Kao -begin
extern void USBMakerInit_SerialNumber(void);

extern BOOL DrvCARD_DetStrgCard(void);
extern BOOL DrvCARD_DetStrgCardWP(void);

_ALIGNED(4) const static UINT8 gMSDCManuStrDesc[] =
{
    USB_VENDER_DESC_STRING_LEN*2+2, // size of String Descriptor = 6 bytes
    0x03,                       // 03: String Descriptor type
    USB_VENDER_DESC_STRING
};

_ALIGNED(4) const static UINT8 gMSDCProdStrDesc[] =
{
    USB_PRODUCT_DESC_STRING_LEN*2+2, // size of String Descriptor = 6 bytes
    0x03,                       // 03: String Descriptor type
    USB_PRODUCT_DESC_STRING
};
_ALIGNED(16) UINT8 gMSDCInquiryData[36]=
{
    0x00, 0x80, 0x05, 0x02, 0x20, 0x00, 0x00, 0x00,
    //Vendor identification,
    USB_VENDER_STRING,/*'\0',*/
    USB_PRODUCT_STRING,'\0',
    USB_PRODUCT_REVISION
};
//#NT#2010/10/21#Lily Kao -end

void USBMakerInit_UMSD(USB_MSDC_INFO *pUSBMSDCInfo)
{
    pUSBMSDCInfo->pManuStringDesc = (USB_STRING_DESC *)gMSDCManuStrDesc;
    pUSBMSDCInfo->pProdStringDesc = (USB_STRING_DESC *)gMSDCProdStrDesc;

    //USBMakerInit_SerialNumber();
    pUSBMSDCInfo->pSerialStringDesc = NULL;

    pUSBMSDCInfo->VID = USB_VID;
    pUSBMSDCInfo->PID = USB_PID_MSDC;

    pUSBMSDCInfo->pInquiryData = (UINT8*)&gMSDCInquiryData[0];
}

/**
  Initialize application for USB MSDC mode

  Initialize application for USB MSDC mode.

  @param void
  @return void
*/
//#NT#2010/12/13#Lily Kao -begin
ER AppInit_ModeUSBMSDC(void)
//#NT#2010/12/13#Lily Kao -end
{
    char* pDxName=NULL;
    USB_MSDC_INFO       MSDCInfo={0};
    UINT32              uiPoolAddr;
    DX_HANDLE pStrgDev = 0;
    ER retV = E_OK;
    #if (SDHOTPLUG_FUNCTION == ENABLE)
    if( TRUE == m_bIsMsdcOpened || UIStorageCheck(STORAGE_CHECK_ERROR, NULL) == TRUE)
    {
    	return NVTEVT_CONSUME;
    }
    m_bIsMsdcOpened = TRUE;
    #endif

    // Get buffer memory for MSDC task, MSDC share the same buffer memory with SIDC,
    // make sure the buffer memory size is enough for both
    uiPoolAddr = OS_GetMempoolAddr(POOL_ID_APP);

    MSDCInfo.uiMsdcBufAddr = uiPoolAddr;
    MSDCInfo.uiMsdcBufSize = MSDC_MIN_BUFFER_SIZE;
    USBMakerInit_UMSD(&MSDCInfo);
    //The callback functions for the MSDC Vendor command.
    //If project doesn't need the MSDC Vendor command, set this callback function as NULL.
    MSDCInfo.msdc_check_cb = NULL;
    MSDCInfo.msdc_vendor_cb = NULL;
	//#NT#2016/12/20#Niven Cho -begin
    //#NT#MULTI_DRIVE
    #if (FS_MULTI_STRG_FUNC == ENABLE)
    char* pDxName2=NULL;
    DX_HANDLE pStrgDev2 = 0;
    #endif

	//#NT#2017/03/07#Niven Cho -begin
    //#NT#FIX without sd-card, enter to msdc and system crash
    //#NT#2016/06/07#Niven Cho[102874] -begin
    //#NT# linux must be closed after filesys closing
    //System_CPU2_Stop();
    //#NT#2016/06/07#Niven Cho[102874] -end
    pStrgDev = Dx_GetObject(DX_CLASS_STORAGE_EXT|FS_DX_TYPE_DRIVE_A);
    //#NT#2017/03/07#Niven Cho[117156] -begin
    //#NT#FS_SWITCH_STRG_FUNC
	#if (FS_SWITCH_STRG_FUNC)
	UINT32 uiDxState=0;
    if (Dx_GetState((DX_HANDLE)pStrgDev, STORAGE_STATE_INSERT,&uiDxState)!=DX_OK || uiDxState==FALSE)
	{
        pStrgDev = Dx_GetObject(DX_CLASS_STORAGE_EXT|FS_DX_TYPE_DRIVE_B);
    }
	#endif
	//#NT#2017/03/07#Niven Cho[117156] -end
	#if (FS_MULTI_STRG_FUNC == ENABLE)
	pStrgDev2 = Dx_GetObject(DX_CLASS_STORAGE_EXT|FS_DX_TYPE_DRIVE_B);
	#endif
	//#NT#2017/03/07#Niven Cho -end

    MSDCInfo.pStrgHandle[0] = pStrgDev;

    if(Dx_GetInfo(pStrgDev, DX_INFO_NAME,&pDxName)!=DX_OK)
    {
        pDxName = NULL;
    }

    #if (FS_MULTI_STRG_FUNC == ENABLE)
    MSDCInfo.pStrgHandle[1] = pStrgDev2;

    if(Dx_GetInfo(pStrgDev2, DX_INFO_NAME,&pDxName2)!=DX_OK)
    {
        pDxName2 = NULL;
    }
    #endif

    #if (FS_MULTI_STRG_FUNC == ENABLE)
    MSDCInfo.pStrgHandle[1] = pStrgDev2;

    if(Dx_GetInfo(pStrgDev2, DX_INFO_NAME,&pDxName2)!=DX_OK)
    {
        pDxName2 = NULL;
    }
    #endif

    #if (FS_MULTI_STRG_FUNC == ENABLE)
    MSDCInfo.msdc_storage_detCB[0] = (MSDC_StorageDet_CB)DrvCARD_DetStrgCard;
    MSDCInfo.msdc_strgLock_detCB[0] = (MSDC_StrgLockDet_CB)DrvCARD_DetStrgCardWP;
    MSDCInfo.msdc_storage_detCB[1] = NULL;
    MSDCInfo.msdc_strgLock_detCB[1] = NULL;
    MSDCInfo.msdc_type[0] = MSDC_STRG;
    MSDCInfo.msdc_type[1] = MSDC_STRG;
    MSDCInfo.LUNs = 2;
    #else
    if(pDxName==NULL || strcmp(pDxName, "Storage_EmbMem5")==0)
    {
        MSDCInfo.msdc_storage_detCB[0] = NULL;
        MSDCInfo.msdc_strgLock_detCB[0] = NULL;
    }
    else if(strcmp(pDxName,"Storage_Card1")==0)
    {
        MSDCInfo.msdc_storage_detCB[0] = (MSDC_StorageDet_CB)DrvCARD_DetStrgCard;
        MSDCInfo.msdc_strgLock_detCB[0] = (MSDC_StrgLockDet_CB)DrvCARD_DetStrgCardWP;
    }
    MSDCInfo.msdc_type[0] = MSDC_STRG;
    MSDCInfo.LUNs = 1;
    #endif

    //#NT#2009/08/29#Ben Wang -Begin
    // Close File System task before running USB MSDC
    //GxFile_Exit();
    FileSys_CloseEx('A', FST_TIME_INFINITE);
    //#NT#2009/08/29#Ben Wang -end
    #if (FS_MULTI_STRG_FUNC == ENABLE)
    FileSys_CloseEx('B', FST_TIME_INFINITE);
    #endif
	//#NT#2016/12/20#Niven Cho -end
#if (_CPU2_TYPE_ == _CPU2_LINUX_)
    //#NT#2016/06/07#Niven Cho[102874] -begin
    //#NT# linux must be closed after filesys closing
    System_CPU2_Stop();
    //#NT#2016/06/07#Niven Cho[102874] -end
#endif
    Msdc_SetConfig(USBMSDC_CONFIG_ID_SELECT_POWER, USBMSDC_POW_BUSPOWER);
    Msdc_SetConfig(USBMSDC_CONFIG_ID_RC_DISABLESTALL, FALSE);

#if(MSDCVENDOR_NVT==ENABLE)
    MsdcNvtCb_Attach(&MSDCInfo);
#endif

    retV = Msdc_Open(&MSDCInfo);
    if (retV != E_OK)
    {
        debug_err(("Error open USB MSDC task:%d\r\n",retV));
    }
    return retV;
}

INT32 MSDCExe_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_DefaultEvent(pCtrl,NVTEVT_EXE_OPEN,paramNum,paramArray);
//#NT#2009/07/01#Brad Chen -begin
//#NT#Add to support MSDC CDROM function
    pLastStrgDev =GxStrg_GetDevice(0);
//#NT#2009/07/01#Brad Chen -end
    if(pLastStrgDev == 0)
    {
        //PowerOn_CB() POWERON_CB_SXOPEN 沒有提早啟動File System
        debug_err(("MSDC: Wait for FS start...\r\n"));
        FileSys_WaitFinish();

        //update
        pLastStrgDev = GxStrg_GetDevice(0);
    }

    UI_SetData(FL_FSStatus,FS_NOT_INIT);

    return (AppInit_ModeUSBMSDC());
}

INT32 MSDCExe_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	#if (SDHOTPLUG_FUNCTION == ENABLE)
	if( FALSE == m_bIsMsdcOpened)
    {
    	return NVTEVT_CONSUME;
    }
    m_bIsMsdcOpened = FALSE;
    #endif

    Msdc_Close();

	//#NT#2017/02/22#Niven Cho -begin
    //#NT#re-mount flow after usb unplug
	DX_HANDLE pStrgDev = 0;
	#if (FS_MULTI_STRG_FUNC == ENABLE)
	DX_HANDLE pStrgDev2 = 0;
	#endif

	#if (_CPU2_TYPE_ == _CPU2_LINUX_)
    pStrgDev = Dx_GetObject(DX_CLASS_STORAGE_EXT|FS_DX_TYPE_DRIVE_A);
	#if (FS_MULTI_STRG_FUNC == ENABLE)
	pStrgDev = Dx_GetObject(DX_CLASS_STORAGE_EXT|FS_DX_TYPE_DRIVE_B);
	#endif
	#else
    pStrgDev = (DX_HANDLE)GxStrg_GetDevice(0);
	#if (FS_MULTI_STRG_FUNC == ENABLE)
    pStrgDev2 = (DX_HANDLE)GxStrg_GetDevice(1);
	#endif
	#endif

    if (GxStrg_OpenDevice(0, pStrgDev) != TRUE)
    {
        char* pDxName="unknown";
        Dx_GetInfo(pStrgDev, DX_INFO_NAME,&pDxName);
        DBG_ERR("Storage mount %s fail\r\n",pDxName);
    }
	#if (FS_MULTI_STRG_FUNC == ENABLE)
    if (GxStrg_OpenDevice(1, pStrgDev2) != TRUE)
    {
        char* pDxName="unknown";
        Dx_GetInfo(pStrgDev2, DX_INFO_NAME,&pDxName);
        DBG_ERR("Storage mount %s fail\r\n",pDxName);
    }
	#endif
	//#NT#2017/02/22#Niven Cho -end

    Ux_DefaultEvent(pCtrl,NVTEVT_EXE_CLOSE,paramNum,paramArray);
    return NVTEVT_CONSUME;
}

EVENT_ENTRY CustomMSDCObjCmdMap[] =
{
    {NVTEVT_EXE_OPEN,               MSDCExe_OnOpen},
    {NVTEVT_EXE_CLOSE,              MSDCExe_OnClose},
    {NVTEVT_NULL,                   0},  //End of Command Map
};

CREATE_APP(CustomMSDCObj,APP_SETUP)

//#NT#2010/07/29#Lily Kao -end

