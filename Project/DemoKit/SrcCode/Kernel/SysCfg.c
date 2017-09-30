/**
    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.

    @file       ModuleInstall.c
    @ingroup

    @brief      Install Task,Sem,Flag for module.
                Install Task,Sem,Flag for module.

    @note       Nothing.
*/

#include <stdio.h>
#include <stdlib.h>
#include "Type.h"
#include "SysCfg.h"
#include "EXTERNS.h"
#include "SysKer.h"

//===========================================================================
#include "DxSys.h"

//System Service
#include "SxCmd.h"
#include "GxSystem.h"
#include "DbgUtApi.h"
#include "Utility.h"

//Device Service
#include "FwSrvApi.h"
#include "PStore.h"
#include "FileSysTsk.h"
#include "GxTimer.h"
#include "DispSrvApi.h"

//Lib
//#NT#2017/03/29#Jeah Yen -begin
//#NT#allow calculate buffer with 420 format
#include "GxImage.h"
//#NT#2017/03/29#Jeah Yen -end
#include "DCF.h"
#include "DPOF.h"
#include "Exif.h"
#include "FileDB.h"
#include "GxSound.h"
#include "UMSDC.h"
#include "UVAC.h"
#include "USBSIDC2.h"
#include "Audio.h"
#include "AudFilterAPI.h"
#include "Lens.h"
#include "LensCtrlTsk.h"
#include "GxFlash.h"
#include "DhcpNvt.h"
#include "GxDisplay.h"
#if (AUDIO_PLAY_FUNCTION == ENABLE)
#include "WavStudioTsk.h"
#include "NetAudQue.h"
//#NT#2016/08/30#Jeah Yen -begin
//#NT#support StreamReceiver
#include "StreamReceiverAPI.h"
//#NT#2016/08/30#Jeah Yen -end
//#NT#2016/12/22#HM Tseng -begin
//#NT# Support AEC function
#if (AUDIO_AEC_FUNC == ENABLE)
#include "nvtaec_api.h"
#endif
//#NT#2016/12/22#HM Tseng -end
#endif

#include "VideoCodec_H264.h"
#if (HTTP_LIVEVIEW_FUNC || MEDIA_VASTREAM)
#include "LviewNvtAPI.h"
#endif

//App Playback
#include "PlaybackTsk.h"

//App Photo
#include "ae_task.h"
#include "awb_task.h"
#include "af_task.h"
#include "IPL_Cmd.h"
#include "ImgCaptureAPI.h"
#include "Sensor.h"
#include "PhotoTask.h"
#include "rsc_task.h"

//App Usb
#include "MsdcNvtApi.h"

//App Media
#include "MovieInterface.h"
#include "SMediaPlayAPI.h"
#include "NvtIpcAPI.h"
#include "WiFiIpcAPI.h"
#include "FsIpcAPI.h"
#include "HfsNvtAPI.h"
//#include "P2pIpcAPI.h"
#if(WIFI_AP_FUNC==ENABLE || IPCAM_FUNC==ENABLE)
#include "RtspNvtApi.h"
#include "RtspCliNvtApi.h"
#include "UsockIpcAPI.h"
#if (USOCKET_CLIENT==ENABLE)
#include "UsockCliIpcAPI.h"
#endif
#if (YOUKU_SDK == ENABLE)
#include "YoukuIpcAPI.h"
#endif
#endif
//#NT#2016/05/10#Jeah Yen -begin
//#NT#Support IOTP2P
#if (IOT_P2P_FUNC == ENABLE)
#include "P2pIpcAPI.h"
#endif
//#NT#2016/05/10#Jeah Yen -end

//#NT#2016/06/23#Charlie Chang -begin
//#NT# support IQtool
#include "IQtoolAPI.h"
//#NT#2016/06/23#Charlie Chang -end

#if defined(YQCONFIG_DRIVE_EVENT_SUPPORT)
#include "DriveEvent.h"
#endif
//User
#include "UIControl.h"
#include "NvtUser.h"
#include "NvtBack.h"
#include "UserCmd.h"

//#NT#2016/07/22#YongChang Qui -begin
//#NT#MovieStamp uses unified memory as IPC & MovieStamp buffer for IPCam with AE_LOG off
#include "MovieStamp.h"
//#NT#2016/07/22#YongChang Qui -end

#include "MovieStampAPI.h"

#include "UIGraphics.h"
#include "spiflash.h"

#include "IPL_Display_Id.h"
#include "IQS_Task.h"
#include "IPL_Stitch_Id.h"

#if (GPS_FUNCTION == ENABLE)
#include "GPS.h"
#endif
#if(WIFI_AP_FUNC==ENABLE)
#include "WifiAppCmd.h"
#endif

#include "ImageStream.h"
#if(UCTRL_FUNC==ENABLE)
#include "NvtUctrlAPI.h"
#include "UCtrlMain.h"
#include "UCtrlAppPhoto.h"
#include "UCtrlAppOnvif.h"
#include "UCtrlAppSystem.h"
#endif
#if(CALIBRATION_FUNC==ENABLE)
#include "EngineerMode.h"
//#NT#2016/07/20#XL_Wei -begin
//#NT#Support web calibration
#include "IPL_CalWEB_Task.h"
//#NT#2016/07/20#XL_Wei -end
//#NT#2016/07/19#Silvia Wu -begin
//#NT# support calibration usb type
#include "IPL_CalUSB_Task.h"
//#NT#2016/07/19#Silvia Wu -end
#endif
#include "SysMain.h"
#include "PstoreIpcAPI.h"
#if (BT_FUNC==ENABLE)
#include "BT_API.h"
#endif
#include "FTypeIpcAPI.h"
#if(FACE_GRADE_TEST==ENABLE)
#include "FaceGIpcAPI.h"
#endif
//#NT#2016/02/18#Lincy Lin -begin
//#NT#Support logfile function
#if (LOGFILE_FUNC==ENABLE)
#include "LogFile.h"
#endif
//#NT#2016/02/18#Lincy Lin -end
//#NT#2016/04/21#Lincy Lin -begin
//#NT#Support curl (http client)
#if (CURL_FUNC == ENABLE)
#include "CurlNvtAPI.h"
#endif
//#NT#2016/04/21#Lincy Lin -end
//#NT#2016/04/27#Lincy Lin -begin
//#NT#Support userlog function
#if (USERLOG_FUNC == ENABLE)
#include "userlog.h"
#endif
//#NT#2016/04/27#Lincy Lin -end
//#NT#2016/05/31#Ben Wang -begin
//#NT#Add UVC multimedia function.
#if(UVC_MULTIMEDIA_FUNC == ENABLE)
#include "UsbCmdAPI.h"
#endif
#if defined(YQCONFIG_UART_TO_MTK_SUPPORT)
#include "UART_TO_MTK.h"
#endif
#if defined(YQCONFIG_UART_TO_MCU_SUPPORT)
#include "UART_TO_MCU.h"
#endif
//#NT#2016/05/31#Ben Wang -end
//#NT#2016/06/22#KCHong -begin
//#NT#Install ADAS system flag here
#if (_ADAS_FUNC_ == ENABLE)
#include "adas_Apps.h"
#endif
//#NT#2016/06/22#KCHong -end
//#NT#2016/06/28#KCHong -begin
//#NT#DbgLog in GPS section
#if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
#include "mp4log.h"
#endif
//#NT#2016/06/28#KCHong -end

//#NT#2016/07/18#ML Cui -begin
//#NT#Install DDD library flag here
#if (_DDD_FUNC_ == ENABLE)
#include "DDD_lib.h"
#endif
//#NT#2016/07/18#ML Cui -end

#if (_TSR_FUNC_ == ENABLE)
#include "tsr_lib.h"
#endif

//#NT#2016/07/27#Charlie Chang -begin
//#NT#support iqtool ipc
#include "IqtoolIpcAPI.h"
//#NT#2016/07/27#Charlie Chang -end

#if(MOVIE_FTG_FUNC == ENABLE)
#include "ftg_lib.h"
#endif

//===========================================================================
//static OS resource (Config Task,Flag,Semaphore & MemPool)

DEFINE_OS_RESOURCE
(
    MAX_TASK_COUNT,
    MAX_SEMAPHORE_COUNT,
    MAX_FLAG_COUNT,
    BUF_NUM,
    MAX_MAILBOX_COUNT,
    MAX_MEMPOOL_COUNT
);

//===========================================================================
//Install user OS resource (Config Task,Flag,Semaphore & MemPool)
//  Note: stack is config by Task, heap is config by MemPool

//for sdio driver
UINT32 POOL_ID_STORAGE_SDIO = 0;
#if(COPYCARD2CARD_FUNCTION == ENABLE)
UINT32 POOL_ID_STORAGE_SDIO2 = 0;
#endif
//for nand driver
UINT32 POOL_ID_STORAGE_NAND = 0;
//for ramdisk
UINT32 POOL_ID_STORAGE_RAMDISK = 0;
//for FWStore
UINT32 POOL_ID_FWS_BUFFER = 0;
//for FileSys
UINT32 POOL_ID_FS_BUFFER = 0;
//for FsIpc
UINT32 POOL_ID_FSIPC = 0;
//for DCF
UINT32 POOL_ID_DCF_BUFFER = 0;
#if PST_FUNC
//for PStore
UINT32 POOL_ID_PS_BUFFER = 0;
#endif
//for WiFiIpc
UINT32 POOL_ID_WIFIIPC = 0;
//for P2pIpc
//#NT#2016/05/06#Jeah Yen -begin
//#NT#Support IOTP2P
#if (IOT_P2P_FUNC == ENABLE)
UINT32 POOL_ID_P2PIPC = 0;
#endif
//#NT#2016/05/06#Jeah Yen -end
//for UsockIpc
UINT32 POOL_ID_USOCKIPC = 0;
//for DHCPIpc
UINT32 POOL_ID_DHCPIPC = 0;
UINT32 POOL_ID_FTYPEIPC = 0;
#if(FACE_GRADE_TEST==ENABLE)
UINT32 POOL_ID_FACEGIPC = 0;
#endif
//for Gfx
UINT32 POOL_ID_GFX_IMAGE = 0;
UINT32 POOL_ID_GFX_STRING = 0;
UINT32 POOL_ID_GFX_TEMP = 0;
//for Exif
UINT32 POOL_ID_EXIF = 0;
//for Date Imprint
UINT32 POOL_ID_DATEIMPRINT = 0;
//for fileDB
UINT32 POOL_ID_FILEDB = 0;
//for Audio
//#NT#2016/08/30#Jeah Yen -begin
//#NT#support StreamReceiver
#if (AUDIO_PLAY_FUNCTION == ENABLE)
UINT32 POOL_ID_AUD_AOUT = 0;
UINT32 POOL_ID_AUD_IPC = 0;
#endif
//#NT#2016/08/30#Jeah Yen -end
//for Display
UINT32 POOL_ID_DISP_OSD2 = 0;
UINT32 POOL_ID_DISP_OSD2TEMP = 0;
UINT32 POOL_ID_DISP_OSD1 = 0;
UINT32 POOL_ID_DISP_OSD1TEMP = 0;
UINT32 POOL_ID_DISP_VDO2 = 0;
UINT32 POOL_ID_DISP_VDO2TEMP = 0;
UINT32 POOL_ID_DISP_VDO1 = 0;
UINT32 POOL_ID_DISP_VDO1TEMP = 0;
UINT32 POOL_ID_DISP_VDO1TEMP2 = 0;
UINT32 POOL_ID_DISP_VDO1TEMP3 = 0;
//free
UINT32 POOL_ID_APP = 0;
//for Multi-core
UINT32 POOL_ID_CPU2= 0;
UINT32 POOL_ID_DSP= 0;
UINT32 POOL_ID_HFSIPC = 0;
#if (YOUKU_SDK == ENABLE)
UINT32 POOL_ID_YOUKUIPC = 0;
#endif
//#NT#2016/05/31#Ben Wang -begin
//#NT#Add UVC multimedia function.
#if(UVC_MULTIMEDIA_FUNC == ENABLE)
UINT32 POOL_ID_USBCMD = 0;
#endif
//#NT#2016/05/31#Ben Wang -end
UINT32 POOL_ID_NVTIPC = 0;
UINT32 POOL_ID_NVTUCTRL = 0;
UINT32 POOL_ID_CAL = 0;
//#NT#2016/04/21#Lincy Lin -begin
//#NT#Support curl (http client)
#if (CURL_FUNC == ENABLE)
UINT32 POOL_ID_CURLNVT = 0;
#endif
//#NT#2016/04/21#Lincy Lin -end
//#NT#2016/06/28#Lincy Lin -begin
//#NT#Add memory pool for IPCAM snapshot using
#if (IPCAM_FUNC == ENABLE)
UINT32 POOL_ID_IPCAM_SNAPSHOT = 0;
#endif
//#NT#2016/06/28#Lincy Lin -end

//#NT#2016/07/27#Charlie Chang -begin
//#NT# support iqtool ipc
#if(IQTOOL_FUNC == ENABLE)
UINT32 POOL_ID_IQTOOLIPC = 0;
#endif
//#NT#2016/07/27#Charlie Chang -end

//#NT#2016/08/19#Lincy Lin -begin
//#NT#Support linux cvhost function
#if (LINUX_CVHOST_FUNC == ENABLE)
UINT32 POOL_ID_CVHOST = 0;
#endif
//#NT#2016/08/19#Lincy Lin -end

//#NT#2016/11/11#Charlie Chang -begin
//#NT# support save face tracking snapshot
#if (FTG_SNAPSHOT_FUN == ENABLE)
UINT32 POOL_ID_FTG_SNAPSHOT =0;
#endif
//#NT#2016/11/11#Charlie Chang -end
//#NT#2016/11/18#KS Hung -begin
//#NT#For Side By Side calibration from APP
#if (SBS_VIEW_FUNC == ENABLE)
UINT32 POOL_ID_SBS_CAL = 0;
#endif
//#NT#2016/11/18#Ks Hung -end

void Install_User(void)
{
/*
NOTE: Some special limitations and tips about implement Install ID in OS_Main():
1.  Before Install ID, we needs to confirm ZI section is already clear ok, make sure 1st code section has been load finished, and note that other code sections may not be ready yet.
2.  While Install ID, all function codes have to be include in 1st code section, and all prototype declarations have to add into the _SECTION(".kercfg_text")
3.  While Install ID, all global variables have to be include in 1st code section or ZI section. So prototype declarations have to add into the _SECTION(".kercfg_text")
4.  While Install ID, the stack is only 1K bytes for kernel, do not call functions too deep.
5.  While Install ID, kernel is not start yet, make sure only use OS_CONFIG_ APIs and debug message function, do not call any other kernel functions.
6.  While Install ID, driver and I/O is also not start yet, do not call any driver APIs or i/o control APIs.
*/

//System Service
    SwTimer_InstallID();
    SxCmd_InstallID();
    GxSystem_InstallID();
    UserMainProc_InstallID();

//Device Ext
    spiflash_InstallID();//SPI FLASH

//Device Service
    DbgUt_InstallID(); //CPU,DMA
    //Timer
    GxTimer_InstallID(); //TIMER
    //Storage
    FwSrv_InstallID(); //NAND
    FileSys_InstallID(); //CARD,NAND
    PStore_InstallID(); //NAND
    //Display
    DispSrv_InstallID(); //DISPLAY

//Lib
    DCF_InstallID();
    FileDB_InstallID();
    #if (PLAY_MODE==ENABLE)
    DPOF_InstallID();
    #endif
    EXIF_InstallID();
    Msdc_InstallID();
    #if (USB_MODE==ENABLE || IPCAM_UVC_FUNC==ENABLE)
    UVAC_InstallID();
    #endif
    #if (HTTP_LIVEVIEW_FUNC || MEDIA_VASTREAM)
    LviewNvt_InstallID();
    #endif
    GxSound_InstallID();
    AudFilter_InstallID();
        #if (LENS_FUNCTION == ENABLE)
        LensCtrl_InstallID();
        LensCtrl2_InstallID();
        #endif
    GxFlash_InstallID();

    MP_H264Enc_InstallID();

//App Playback
    #if (PLAY_MODE==ENABLE)
    PB_InstallID();
    #endif

//App Photo
    Sensor_InstallID();
#if (_IPPLIB_ != _IPL_FAKE_)
    IPL_InstallID();
    IQS_InstallID();
    IPL_Display_InstallID();
    IPL_Stitch_InstallID();
    #if (PHOTO_MODE==ENABLE)
    Cap_InstallID();
    #endif
    Photo_InstallID();
    AE_InstallID();
    AF_InstallID();
    AWB_InstallID();
    RSC_InstallID();
#endif
//#NT#2016/06/23#Charlie Chang -begin
//#NT# support IQTOOL
    IQtool_InstallID();
//#NT#2016/06/23#Charlie Chang -end
//#NT#2016/07/27#Charlie Chang -begin
//#NT# support IQtool IPC
    IqtoolIpc_InstallID();
//#NT#2016/07/27#Charlie Chang -end
//App Usb
    MsdcNvt_InstallID();

//App Media
    //SMediaRec_InstallID();
    MovRec_InstallID();
    #if (PLAY_MODE==ENABLE)
    SMediaPlay_InstallID();
    #endif
    NvtIpc_InstallID();
    FsIpc_InstallID();
    HfsNvt_InstallID();
#if (YOUKU_SDK == ENABLE)
    YoukuIpc_InstallID();
#endif
//#NT#2016/05/10#Jeah Yen -begin
//#NT#Support IOTP2P
#if (IOT_P2P_FUNC == ENABLE)
    P2pIpc_InstallID();
#endif
//#NT#2016/05/10#Jeah Yen -end
    Dhcp_InstallID();
#if(WIFI_AP_FUNC==ENABLE)
    RtspNvt_InstallID();
    RtspCliNvt_InstallID();
    WiFiIpc_InstallID();
    UsockIpc_InstallID();
#if (USOCKET_CLIENT==ENABLE)
    UsockCliIpc_InstallID();
#endif
#endif
#if(IPCAM_FUNC==ENABLE)
    RtspNvt_InstallID();
#endif
#if (IPCAM_FUNC)
    FtypeIpc_InstallID();
#endif
#if(FACE_GRADE_TEST==ENABLE)
    FaceGIpc_InstallID();
#endif

//User
    #if (GPS_FUNCTION == ENABLE)
    GPS_InstallID();
    #endif
    NvtUser_InstallID();
    NvtBack_InstallID();
    UIControl_InstallID();
    UI_InstallID();
    MovieStamp_InstallID();
    UserCmd_InstallID();
#if(WIFI_AP_FUNC==ENABLE)
    WifiCmd_InstallID();
#endif
#if (AUDIO_PLAY_FUNCTION == ENABLE)
    WavStudio_InstallID();
    NetAudQue_InstallID();
//#NT#2016/08/30#Jeah Yen -begin
//#NT#support StreamReceiver
    StreamReceiver_InstallID();
//#NT#2016/08/30#Jeah Yen -end
//#NT#2016/12/22#HM Tseng -begin
//#NT# Support AEC function
    #if (AUDIO_AEC_FUNC == ENABLE)
    NvtAec_InstallID();
    #endif
//#NT#2016/12/22#HM Tseng -end
#endif

    ImageStream_InstallID();
#if(UCTRL_FUNC==ENABLE)
    NvtUctrl_InstallID();
    UCtrlMain_InstallID();
    #if (PHOTO_MODE==ENABLE)
    UCtrlAppPhoto_InstallID();
    #endif
    UCtrlAppOnvif_InstallID();
    UCtrlAppSys_InstallID();
#endif

#if(CALIBRATION_FUNC==ENABLE)
    Engineer_InstallID();
//#NT#2016/07/20#XL_Wei -begin
//#NT#Support web calibration
    Cal_WEBInstallID();
//#NT#2016/07/20#XL_Wei -end
//#NT#2016/07/19#Silvia Wu -begin
//#NT# support calibration usb type
    Cal_USBInstallID();
//#NT#2016/07/19#Silvia Wu -end
#endif
#if (_CPU2_TYPE_ == _CPU2_LINUX_)
    PstoreIpc_InstallID();
#endif
#if (BT_FUNC==ENABLE)
    BT_InstallID();
#endif
//#NT#2016/02/18#Lincy Lin -begin
//#NT#Support logfile function
#if (LOGFILE_FUNC==ENABLE)
    LogFile_InstallID();
#endif
//#NT#2016/02/18#Lincy Lin -end
//#NT#2016/04/21#Lincy Lin -begin
//#NT#Support curl (http client)
#if (CURL_FUNC == ENABLE)
    CurlNvt_InstallID();
#endif
//#NT#2016/04/21#Lincy Lin -end
//#NT#2016/04/27#Lincy Lin -begin
//#NT#Support userlog function
#if (USERLOG_FUNC == ENABLE)
    userlog_InstallID();
#endif
//#NT#2016/04/27#Lincy Lin -end
//#NT#2016/05/31#Ben Wang -begin
//#NT#Add UVC multimedia function.
#if(UVC_MULTIMEDIA_FUNC == ENABLE)
    UsbCmd_InstallID();
#endif
#if defined(YQCONFIG_UART_TO_MTK_SUPPORT)
    UART_TO_MTK_InstallID();
#endif
#if defined(YQCONFIG_UART_TO_MCU_SUPPORT)
    UART_TO_MCU_InstallID();
#endif
//#NT#2016/05/31#Ben Wang -end

//#NT#2016/06/22#KCHong -begin
//#NT#Install ADAS system flag here
#if (_ADAS_FUNC_ == ENABLE)
    ADAS_InstallID();
#endif
//#NT#2016/06/22#KCHong -end

//#NT#2016/07/18#ML Cui -begin
//#NT#Install DDD library flag here
#if (_DDD_FUNC_ == ENABLE)
    DDD_InstallID();
#endif
//#NT#2016/07/18#ML Cui -end

//#NT#2016/06/22#KCHong -begin
#if (_TSR_FUNC_ == ENABLE)
    TSR_InstallID();
#endif
//#NT#2016/06/22#KCHong -end


//#NT#2016/06/28#KCHong -begin
//#NT#DbgLog in GPS section
#if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
    mp4log_InstallID();
#endif
//#NT#2016/06/28#KCHong -end

//#NT#2017/01/09#Charlie Chang -begin
//#NT#install face tracking grading ID
#if (MOVIE_FTG_FUNC ==ENABLE)
	FTG_InstallID();
#endif
//#NT#2017/01/09#Charlie Chang -end
}

void Install_DrvMEM(void)
{
//for nand driver
    OS_CONFIG_MEMPOOL(POOL_ID_STORAGE_NAND,  POOL_SIZE_STORAGE_NAND,    POOL_CNT_STORAGE_NAND);      // nand driver working buffer
#if(_INTERSTORAGE_ == _INTERSTORAGE_SPI_)
//for ramdisk driver
    OS_CONFIG_MEMPOOL(POOL_ID_STORAGE_RAMDISK,  POOL_SIZE_STORAGE_RAMDISK, POOL_CNT_STORAGE_RAMDISK); // ramdisk driver working buffer
#endif
//for sdio driver
    OS_CONFIG_MEMPOOL(POOL_ID_STORAGE_SDIO,  POOL_SIZE_STORAGE_SDIO,    POOL_CNT_STORAGE_SDIO);      // sdio driver working buffer
#if(COPYCARD2CARD_FUNCTION == ENABLE)
    OS_CONFIG_MEMPOOL(POOL_ID_STORAGE_SDIO2, POOL_SIZE_STORAGE_SDIO,    POOL_CNT_STORAGE_SDIO);      // sdio2 driver working buffer
#endif
//for NvtIPC driver
    OS_CONFIG_MEMPOOL(POOL_ID_NVTIPC,        POOL_SIZE_NVTIPC,          POOL_CNT_NVTIPC);
}

void Install_DevEmbMEM(void)
{
//for FWStore
    OS_CONFIG_MEMPOOL(POOL_ID_FWS_BUFFER,    POOL_SIZE_FWS_BUFFER,      POOL_CNT_FWS_BUFFER);        // FWStore working buffer
#if PST_FUNC
//for PStore
    OS_CONFIG_MEMPOOL(POOL_ID_PS_BUFFER,     POOL_SIZE_PS_BUFFER,       POOL_CNT_PS_BUFFER);         // PStore working buffer
#endif
}

void Install_DevStrgMEM(void)
{
//for FsIpc
#if(POOL_SIZE_FSIPC)
    OS_CONFIG_MEMPOOL(POOL_ID_FSIPC,         POOL_SIZE_FSIPC,           POOL_CNT_FSIPC);             // FsIpc working buffer
#endif
//for FileSys
    OS_CONFIG_MEMPOOL(POOL_ID_FS_BUFFER,     POOL_SIZE_FS_BUFFER,       POOL_CNT_FS_BUFFER);         // FileSys working buffer
//for DCF
    OS_CONFIG_MEMPOOL(POOL_ID_DCF_BUFFER,    POOL_SIZE_DCF_BUFFER,      POOL_CNT_DCF_BUFFER);        // DCF  working buffer
#if(USE_FILEDB==ENABLE)
//for fileDB
    OS_CONFIG_MEMPOOL(POOL_ID_FILEDB,        POOL_SIZE_FILEDB,          POOL_CNT_FILEDB);            // FileDB working buffer
#endif
}

void Install_DevNetMEM(void)
{
#if(WIFI_AP_FUNC==ENABLE)
//for WiFiIpc
    OS_CONFIG_MEMPOOL(POOL_ID_WIFIIPC,       POOL_SIZE_WIFIIPC,         POOL_CNT_WIFIIPC);           // WiFiIPc working buffer
//for UsockIpc
    OS_CONFIG_MEMPOOL(POOL_ID_USOCKIPC,     POOL_SIZE_USOCKIPC,        POOL_CNT_USOCKIPC);          // UsockIPc working buffer
//for DHCPIpc
    OS_CONFIG_MEMPOOL(POOL_ID_DHCPIPC,       POOL_SIZE_DHCPIPC,         POOL_CNT_DHCPIPC);           // WiFiIPc working buffer
#endif
#if(POOL_SIZE_NVTUCTRL)
    OS_CONFIG_MEMPOOL(POOL_ID_NVTUCTRL,      POOL_SIZE_NVTUCTRL,        POOL_CNT_NVTUCTRL);
#endif
#if(POOL_SIZE_HFSIPC)
    OS_CONFIG_MEMPOOL(POOL_ID_HFSIPC,        POOL_SIZE_HFSIPC,          POOL_CNT_HFSIPC);
#endif
#if(YOUKU_SDK == ENABLE)
    OS_CONFIG_MEMPOOL(POOL_ID_YOUKUIPC,      POOL_SIZE_YOUKUIPC,          POOL_CNT_YOUKUIPC);
#endif
//#NT#2016/04/21#Lincy Lin -begin
//#NT#Support curl (http client)
#if (CURL_FUNC == ENABLE)
    OS_CONFIG_MEMPOOL(POOL_ID_CURLNVT,       POOL_SIZE_CURLNVT,           POOL_CNT_CURLNVT);
#endif
//#NT#2016/04/21#Lincy Lin -end
//#NT#2016/06/28#Lincy Lin -begin
//#NT#Add memory pool for IPCAM snapshot using
#if (IPCAM_FUNC == ENABLE)
    OS_CONFIG_MEMPOOL(POOL_ID_IPCAM_SNAPSHOT,  POOL_SIZE_IPCAM_SNAPSHOT,           POOL_CNT_IPCAM_SNAPSHOT);
#endif
//#NT#2016/06/28#Lincy Lin -end
}

void UI_GetOSDBuf(UINT32* pAddr, UINT32* pSize)
{
    UINT32    uiPoolAddr,uiPoolSize;
    uiPoolAddr = OS_GetMempoolAddr(POOL_ID_DISP_OSD2);
    uiPoolSize = OS_GetMempoolSize(POOL_ID_DISP_OSD2)
                +OS_GetMempoolSize(POOL_ID_DISP_OSD2TEMP)
                +OS_GetMempoolSize(POOL_ID_DISP_OSD1)
                +OS_GetMempoolSize(POOL_ID_DISP_OSD1TEMP);
    *pAddr = uiPoolAddr;
    *pSize = uiPoolSize;
}

void Install_DevOsdMEM(void)
{
//for Display
    UINT32 osd2_size = 0;
    UINT32 osd1_size = 0;
    //UI - manage by UIDisplay, buffer is fixed memory after OS start
    #if (DISPLAY_OSD2_FMT == DISP_PXLFMT_INDEX8)
    osd2_size =
        ALIGN_CEIL_32(GxGfx_CalcDCMemSize(OSD_W,OSD_H,TYPE_FB,PXLFMT_INDEX8));
    #else
    debug_msg("ERR: invalid OSD2 fmt!\r\n");
    #endif

    //UI - manage by UIDisplay, buffer is fixed memory after OS start
    #if (DISPLAY_OSD_FMT == DISP_PXLFMT_RGBA8888_PK)
    osd1_size =
        ALIGN_CEIL_32(GxGfx_CalcDCMemSize(OSD_W,OSD_H,TYPE_FB,PXLFMT_RGBA8888_PK));
    #elif (DISPLAY_OSD_FMT == DISP_PXLFMT_RGBA5658_PK)
    osd1_size =
        ALIGN_CEIL_32(GxGfx_CalcDCMemSize(OSD_W,OSD_H,TYPE_FB,PXLFMT_RGBA5658_PK));
    #elif (DISPLAY_OSD_FMT == DISP_PXLFMT_INDEX8)
    osd1_size =
        ALIGN_CEIL_32(GxGfx_CalcDCMemSize(OSD_W,OSD_H,TYPE_FB,PXLFMT_INDEX8));
    #else
    debug_msg("ERR: invalid OSD1 fmt!\r\n");
    #endif

//#NT#2016/09/07#Jeah Yen -begin
//#NT#support rotate with dual display
#if ((DUALDISP_FUNC == ENABLE) && (OSD_USE_ROTATE_BUFFER == ENABLE))
    //prepare 2 show DC and 1 paint DC
    OS_CONFIG_MEMPOOL(POOL_ID_DISP_OSD2,     osd2_size,       POOL_CNT_DISP_OSD2);     // Display OSD2 buffer
    OS_CONFIG_MEMPOOL(POOL_ID_DISP_OSD2TEMP, osd2_size*2,     POOL_CNT_DISP_OSD2TEMP);     // Display OSD2 buffer 2
    OS_CONFIG_MEMPOOL(POOL_ID_DISP_OSD1,     osd1_size,       POOL_CNT_DISP_OSD1);     // Display OSD1 buffer
    OS_CONFIG_MEMPOOL(POOL_ID_DISP_OSD1TEMP, osd1_size*2,     POOL_CNT_DISP_OSD1TEMP);     // Display OSD1 buffer 2
#else
    //prepare 1 show DC and 1 paint DC
    OS_CONFIG_MEMPOOL(POOL_ID_DISP_OSD2,     osd2_size,       POOL_CNT_DISP_OSD2);     // Display OSD2 buffer
    OS_CONFIG_MEMPOOL(POOL_ID_DISP_OSD2TEMP, osd2_size,       POOL_CNT_DISP_OSD2TEMP);     // Display OSD2 buffer 2
    OS_CONFIG_MEMPOOL(POOL_ID_DISP_OSD1,     osd1_size,       POOL_CNT_DISP_OSD1);     // Display OSD1 buffer
    OS_CONFIG_MEMPOOL(POOL_ID_DISP_OSD1TEMP, osd1_size,       POOL_CNT_DISP_OSD1TEMP);     // Display OSD1 buffer 2
#endif
//#NT#2016/09/07#Jeah Yen -end
//for Gfx
    OS_CONFIG_MEMPOOL(POOL_ID_GFX_IMAGE,     POOL_SIZE_GFX_IMAGE,       POOL_CNT_GFX_IMAGE);         // Gfx jpeg image resource buffer
    OS_CONFIG_MEMPOOL(POOL_ID_GFX_STRING,    POOL_SIZE_GFX_STRING,      POOL_CNT_GFX_STRING);        // Gfx language resource buffer
    OS_CONFIG_MEMPOOL(POOL_ID_GFX_TEMP,      POOL_SIZE_GFX_TEMP,        POOL_CNT_GFX_TEMP);          // Gfx working + JPG decode buffer
}

void Install_DevVideoMEM(void)
{
//for Display
    UINT32 vdo2_size = 0;
    UINT32 vdo1_size_1 = 0x10;
    UINT32 vdo1_size_2 = 0x10;
    #if(VDO_USE_ROTATE_BUFFER==ENABLE)
    UINT32 vdo1_size_r = 0x10;
    #endif
    #if (DUALCAM_PIP_BEHIND_FLIP == ENABLE)
    UINT32 vdo1_size_f = 0x10;
    #endif
    USIZE vdo1_lcd = Calc_VideoSize_LCD();
    USIZE vdo1_max = Calc_VideoSize_MAX();
    //#NT#2017/03/29#Jeah Yen -begin
    //#NT#allow calculate buffer with 420 format
    UINT32 lcd_fmt = LCD_PREVIEW_VDO_FMT;
    UINT32 lcd2_fmt = LCD2_PREVIEW_VDO_FMT;
    UINT32 tv_fmt = TV_PREVIEW_VDO_FMT;
    UINT32 hdmi_fmt = HDMI_PREVIEW_VDO_FMT;
    UINT32 vdo_fmt = PXLFMT_YUV422_PK;
    
    if ((lcd_fmt == GX_IMAGE_PIXEL_FMT_YUV420_PACKED)
     && (lcd2_fmt == GX_IMAGE_PIXEL_FMT_YUV420_PACKED)
     && (tv_fmt == GX_IMAGE_PIXEL_FMT_YUV420_PACKED)
     && (hdmi_fmt == GX_IMAGE_PIXEL_FMT_YUV420_PACKED))
    {
        vdo_fmt = PXLFMT_YUV420_PK;
    }
    
    vdo1_lcd.w = ALIGN_CEIL_16(vdo1_lcd.w);
    vdo1_max.w = ALIGN_CEIL_16(vdo1_max.w);

    //UI - manage by UIDisplay, buffer is fixed memory after OS start
    //#NT#2016/03/29#Lincy Lin -begin
    //#NT#Fix the photo quick view display error when image size is 16:9 with rotate panel
    #if(VDO_USE_ROTATE_BUFFER==ENABLE)
    if (vdo1_lcd.w*vdo1_lcd.h > VDO_W * VDO_H)
    {
        vdo2_size = ALIGN_CEIL_32(GxGfx_CalcDCMemSize(vdo1_lcd.w,vdo1_lcd.h,TYPE_FB,vdo_fmt));
    }
    else
    #endif
    {
        vdo2_size = ALIGN_CEIL_32(GxGfx_CalcDCMemSize(VDO_W,VDO_H,TYPE_FB,vdo_fmt));
    }
    //#NT#2016/03/29#Lincy Lin -end
    //#NT#2016/06/27#Lincy Lin -begin
    //#NT#Reduce IPCAM used memory pool size
    #if (POWERONLOGO_FUNCTION==DISABLE && VDO_USE_ROTATE_BUFFER==DISABLE)
    {
        vdo2_size = 32;
    }
    #endif
    //#NT#2016/06/27#Lincy Lin -end
    //App - manage by DispSrv, buffer is fixed memory after OS start
    #if (DUALDISP_FUNC == DISABLE)
    vdo1_size_1 =
        ALIGN_CEIL_32(GxGfx_CalcDCMemSize(vdo1_max.w,vdo1_max.h,TYPE_FB,vdo_fmt))*2;
    #else
    vdo1_size_1 =
        ALIGN_CEIL_32(GxGfx_CalcDCMemSize(vdo1_lcd.w,vdo1_lcd.h,TYPE_FB,vdo_fmt))*2;
    vdo1_size_2 =
        ALIGN_CEIL_32(GxGfx_CalcDCMemSize(vdo1_max.w,vdo1_max.h,TYPE_FB,vdo_fmt))*2;
    #endif
    #if (VDO_USE_ROTATE_BUFFER==ENABLE)
    vdo1_size_r =
        ALIGN_CEIL_32(GxGfx_CalcDCMemSize(vdo1_lcd.w,vdo1_lcd.h,TYPE_FB,vdo_fmt));
    #endif
    #if (DUALCAM_PIP_BEHIND_FLIP == ENABLE)
    vdo1_size_f =
        ALIGN_CEIL_32(GxGfx_CalcDCMemSize(vdo1_max.w,vdo1_max.h,TYPE_FB,vdo_fmt))*2;
    #endif

    OS_CONFIG_MEMPOOL(POOL_ID_DISP_VDO2,     vdo2_size,       POOL_CNT_DISP_VDO2);     // Display VDO2 jpg buffer
    OS_CONFIG_MEMPOOL(POOL_ID_DISP_VDO2TEMP, vdo2_size,       POOL_CNT_DISP_VDO2TEMP);     // Display VDO2 jpg buffer 2
    OS_CONFIG_MEMPOOL(POOL_ID_DISP_VDO1,     vdo1_size_1,     POOL_CNT_DISP_VDO1);     // App: VDO1 frame buffer
    OS_CONFIG_MEMPOOL(POOL_ID_DISP_VDO1TEMP, vdo1_size_2,     POOL_CNT_DISP_VDO1TEMP);     // App: VDO1 frame buffer 2
    #if (VDO_USE_ROTATE_BUFFER == ENABLE)
    OS_CONFIG_MEMPOOL(POOL_ID_DISP_VDO1TEMP2, vdo1_size_r,    POOL_CNT_DISP_VDO1TEMP);     // App: VDO1 frame buffer 3
    #endif
    #if (DUALCAM_PIP_BEHIND_FLIP == ENABLE)
    OS_CONFIG_MEMPOOL(POOL_ID_DISP_VDO1TEMP3, ALIGN_CEIL_32((vdo1_size_f >> 2)), POOL_CNT_DISP_VDO1TEMP3);    // App: frame buffer for PIP flip, need 2 * 1/4 of vdo1_size_f
    #endif
    //#NT#2017/03/29#Jeah Yen -end
}

void Install_MultiCoreMEM(void)
{
#if(POOL_SIZE_CPU2)
    OS_CONFIG_MEMPOOL(POOL_ID_CPU2,          POOL_SIZE_CPU2,            POOL_CNT_CPU2);
#endif

#if(POOL_SIZE_DSP)
    OS_CONFIG_MEMPOOL(POOL_ID_DSP,           POOL_SIZE_DSP,             POOL_CNT_DSP);
#endif
}

void Install_DevVCodecMEM(void)
{
//for Audio Play
#if (AUDIO_PLAY_FUNCTION == ENABLE)
    //#NT#2016/06/29#HM Tseng -begin
    //#NT#reduce AUDIO_PLAY_FUNCTION memory usage
    UINT32 aout_size = 100*1024; //WavStudio need 2MB
    //#NT#2016/06/29#HM Tseng -end
    OS_CONFIG_MEMPOOL(POOL_ID_AUD_AOUT,      aout_size,                 POOL_CNT_AUD_AOUT);     // Audio play buffer
    //#NT#2016/08/30#Jeah Yen -begin
    //#NT#support StreamReceiver
    OS_CONFIG_MEMPOOL(POOL_ID_AUD_IPC,       POOL_SIZE_AUD_IPC,         POOL_CNT_AUD_IPC);      // Audio IPC buffer
    //#NT#2016/08/30#Jeah Yen -end
#endif
//for Exif
    OS_CONFIG_MEMPOOL(POOL_ID_EXIF,          POOL_SIZE_EXIF,            POOL_CNT_EXIF);              // Exif lib working buffer

    //#NT#2016/07/22#YongChang Qui -begin
    //#NT#MovieStamp uses unified memory as IPC & MovieStamp buffer for IPCam with AE_LOG off
    #if !(IPCAM_FUNC && !MOVIE_AE_LOG)
    //for DateImprint
    OS_CONFIG_MEMPOOL(POOL_ID_DATEIMPRINT,   POOL_SIZE_DATEIMPRINT,     POOL_CNT_DATEIMPRINT);       // DATE IMPRINT working buffer
    #endif
    //#NT#2016/07/22#YongChang Qui -end
}

void Install_DevImageMEM(void)
{
#if(POOL_SIZE_CAL)
    OS_CONFIG_MEMPOOL(POOL_ID_CAL,           POOL_SIZE_CAL,             POOL_CNT_CAL);               // Calibration data buffer
#endif
}

void Install_SystemMEM(void)
{
    //Driver
    Install_DrvMEM();
    //Embedded Storage
    Install_DevEmbMEM();
}

void Install_UserMEM(void)
{
    Install_SystemMEM();
    //Storage
    Install_DevStrgMEM();
    //Video
    Install_DevVideoMEM();
    //OSD
    Install_DevOsdMEM();
    //Imagepipe
    Install_DevImageMEM();
    //Video Codec
    Install_DevVCodecMEM();
    //Network
    Install_DevNetMEM();
//#NT#2016/05/31#Ben Wang -begin
//#NT#Add UVC multimedia function.
#if(UVC_MULTIMEDIA_FUNC == ENABLE)
    OS_CONFIG_MEMPOOL(POOL_ID_USBCMD,      POOL_SIZE_USBCMD,          POOL_CNT_USBCMD);
#endif
//#NT#2016/05/31#Ben Wang -end
//free
#if (IPCAM_FUNC && !MOVIE_AE_LOG)
    OS_CONFIG_MEMPOOL(POOL_ID_FTYPEIPC,       POOL_SIZE_FTYPEIPC,         POOL_CNT_FTYPEIPC);
#endif
#if(FACE_GRADE_TEST==ENABLE)
    OS_CONFIG_MEMPOOL(POOL_ID_FACEGIPC,       POOL_SIZE_FACEGIPC,         POOL_CNT_FACEGIPC);
#endif
//#NT#2016/05/06#Jeah Yen -begin
//#NT#Support IOTP2P
#if (IOT_P2P_FUNC == ENABLE)
    OS_CONFIG_MEMPOOL(POOL_ID_P2PIPC,         POOL_SIZE_P2PIPC,           POOL_CNT_P2PIPC);
#endif
    //#NT#2016/07/27#Charlie Chang -begin
    //#NT# support iqtool ipc
    #if(IQTOOL_FUNC == ENABLE)
    OS_CONFIG_MEMPOOL(POOL_ID_IQTOOLIPC,         POOL_SIZE_IQTOOL_IPC,           POOL_CNT_IQTOOLIPC);
    #endif
    //#NT#2016/07/27#Charlie Chang -end
//#NT#2016/05/06#Jeah Yen -end

//#NT#2016/11/11#Charlie Chang -begin
//#NT# support save face tracking snapshot
#if (FTG_SNAPSHOT_FUN == ENABLE)
    OS_CONFIG_MEMPOOL(POOL_ID_FTG_SNAPSHOT, POOL_SIZE_FTG_SNAPSHOT, POOL_CNT_FTG_SNAPSHOT);

#endif
//#NT#2016/11/11#Charlie Chang -end



    //#NT#2016/08/19#Lincy Lin -begin
    //#NT#Support linux cvhost function
#if (LINUX_CVHOST_FUNC == ENABLE)
    OS_CONFIG_MEMPOOL(POOL_ID_CVHOST,         POOL_SIZE_CVHOST,           POOL_CNT_CVHOST);
#endif
    //#NT#2016/08/19#Lincy Lin -end
    //#NT#2016/11/18#KS Hung -begin
    //#NT#For Side By Side calibration from APP upload
    #if (SBS_VIEW_FUNC == ENABLE)
    OS_CONFIG_MEMPOOL(POOL_ID_SBS_CAL,         POOL_SIZE_SBS_CAL,           POOL_CNT_SBS_CAL);
    #endif
    //#NT#2016/11/18#Ks Hung -end
    OS_CONFIG_MEMPOOL(POOL_ID_APP,           POOL_SIZE_FREE-POOL_SIZE_CPU2-POOL_SIZE_DSP, POOL_CNT_APP);  // App: config all free memory for it

    /// the end of Install_UserMEM
    Install_MultiCoreMEM(); /// IMPORTANT!! This call must be last line in Install_UserMEM();
}

#include "ImageStream.h"
#include "ImageUnit_ImagePipe.h"
#include "ImageUnit_Media.h"
#include "ImageUnit_CamFile.h"
#include "ImageUnit_CamDisp.h"
#include "ImageUnit_NetRTSP.h"
#include "ImageUnit_NetHTTP.h"

void ImageStream_ConfigUserDump(void)
{
    ImageUnit_CfgUserDump(&ISF_ImagePipe, Install_DevImageMEM);
    ImageUnit_CfgUserDump(&ISF_Media, Install_DevVCodecMEM);
    ImageUnit_CfgUserDump(&ISF_CamFile, Install_DevStrgMEM);
    ImageUnit_CfgUserDump(&ISF_CamDisp, Install_DevVideoMEM);
    ImageUnit_CfgUserDump(&ISF_NetHTTP, Install_DevNetMEM);
    ImageUnit_CfgUserDump(&ISF_NetRTSP, Install_DevNetMEM);
}
