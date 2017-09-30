/**
    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.

    @file       SysCfg.h
    @ingroup

    @brief      Header file of SysCfg.c
                Config the task, event, semaphore, memory pool, memory layout, mailbox

    @note       Nothing.
*/

#ifndef _SYSCFG_H
#define _SYSCFG_H

#include "Type.h"
#include "SysKer.h"
#include "PrjCfg.h"

//===========================================================================

typedef enum
{
    CODE_SECTION_01 = 0,
    CODE_SECTION_02,
    CODE_SECTION_03,
    CODE_SECTION_04,
    CODE_SECTION_05,
    CODE_SECTION_06,
    CODE_SECTION_07,
    CODE_SECTION_08,
    CODE_SECTION_09,
    CODE_SECTION_10,
    CODE_SECTION_11,
    CODE_SECTION_12,
    CODE_SECTION_13,
    CODE_SECTION_14,
    CODE_SECTION_15,
    CODE_SECTION_16,
    ENUM_DUMMY4WORD(_CODE_SECTION_)
}
CODE_SECTION;

//===========================================================================

// Total number of kernel ID sources
#define MAX_TASK_COUNT              128 // 64
#define MAX_FLAG_COUNT              128
#define MAX_SEMAPHORE_COUNT         256
#define MAX_MAILBOX_COUNT           16
#define MAX_MEMPOOL_COUNT           32
#define BUF_NUM                     255


//===========================================================================
extern void Install_Drv(void);

//===========================================================================
#define VDO_W   DISPLAY_VDO_W
#define VDO_H   DISPLAY_VDO_H
#define OSD_W   DISPLAY_OSD_W
#define OSD_H   DISPLAY_OSD_H

#define FB_W    VDO_W
#define FB_H    VDO_H
#define WIN_W   OSD_W
#define WIN_H   OSD_H

#define FHD_W   1920  //Full-HD buffer width: for H264 decoder, it must align to 16
#define FHD_H   1088  //Full-HD buffer height: for H264 decoder, it must align to 16

//===========================================================================
// User defined Task IDs
//===========================================================================

//===========================================================================
// User defined Semaphore IDs
//===========================================================================

//===========================================================================
//  User defined Mailbox IDs
//===========================================================================
extern UINT32 MBXID_SYS_EVENT;

//===========================================================================
//  User defined Mempool IDs
//===========================================================================
extern UINT32 POOL_ID_STORAGE_SDIO;
#if(COPYCARD2CARD_FUNCTION == ENABLE)
extern UINT32 POOL_ID_STORAGE_SDIO2;
#endif
extern UINT32 POOL_ID_STORAGE_NAND;
extern UINT32 POOL_ID_STORAGE_RAMDISK;
extern UINT32 POOL_ID_FWS_BUFFER;
extern UINT32 POOL_ID_FS_BUFFER;
extern UINT32 POOL_ID_FSIPC;
extern UINT32 POOL_ID_DCF_BUFFER;
#if PST_FUNC
extern UINT32 POOL_ID_PS_BUFFER;
#endif
extern UINT32 POOL_ID_WIFIIPC;
extern UINT32 POOL_ID_USOCKIPC;
extern UINT32 POOL_ID_DHCPIPC;
extern UINT32 POOL_ID_FTYPEIPC;
#if(FACE_GRADE_TEST==ENABLE)
extern UINT32 POOL_ID_FACEGIPC;
#endif
extern UINT32 POOL_ID_GFX_IMAGE;
extern UINT32 POOL_ID_GFX_STRING;
extern UINT32 POOL_ID_GFX_TEMP;
extern UINT32 POOL_ID_EXIF;
extern UINT32 POOL_ID_DATEIMPRINT;
extern UINT32 POOL_ID_FILEDB;
//#NT#2016/08/30#Jeah Yen -begin
//#NT#support StreamReceiver
#if (AUDIO_PLAY_FUNCTION == ENABLE)
extern UINT32 POOL_ID_AUD_AOUT;
extern UINT32 POOL_ID_AUD_IPC;
#endif
//#NT#2016/08/30#Jeah Yen -end
extern UINT32 POOL_ID_DISP_OSD2;
extern UINT32 POOL_ID_DISP_OSD2TEMP;
extern UINT32 POOL_ID_DISP_OSD1;
extern UINT32 POOL_ID_DISP_OSD1TEMP;
extern UINT32 POOL_ID_DISP_VDO2;
extern UINT32 POOL_ID_DISP_VDO2TEMP;
extern UINT32 POOL_ID_DISP_VDO1;
extern UINT32 POOL_ID_DISP_VDO1TEMP;
extern UINT32 POOL_ID_DISP_VDO1TEMP2;
extern UINT32 POOL_ID_DISP_VDO1TEMP3;
extern UINT32 POOL_ID_APP;
extern UINT32 POOL_ID_CPU2; //multi-core reserved
extern UINT32 POOL_ID_DSP; //multi-core reserved
extern UINT32 POOL_ID_HFSIPC;
#if(YOUKU_SDK == ENABLE)
extern UINT32 POOL_ID_YOUKUIPC;
#endif
//#NT#2016/05/31#Ben Wang -begin
//#NT#Add UVC multimedia function.
#if(UVC_MULTIMEDIA_FUNC == ENABLE)
extern UINT32 POOL_ID_USBCMD;
#endif
//#NT#2016/05/31#Ben Wang -end
//#NT#2016/05/06#Jeah Yen -begin
//#NT#Support IOTP2P
#if (IOT_P2P_FUNC == ENABLE)
extern UINT32 POOL_ID_P2PIPC;
#endif
//#NT#2016/05/06#Jeah Yen -end
extern UINT32 POOL_ID_NVTIPC;
extern UINT32 POOL_ID_NVTUCTRL;
extern UINT32 POOL_ID_CAL;
//#NT#2016/04/21#Lincy Lin -begin
//#NT#Support curl (http client)
#if (CURL_FUNC == ENABLE)
extern UINT32 POOL_ID_CURLNVT;
#endif
//#NT#2016/04/21#Lincy Lin -end
//#NT#2016/06/28#Lincy Lin -begin
//#NT#Add memory pool for IPCAM snapshot using
#if (IPCAM_FUNC == ENABLE)
extern UINT32 POOL_ID_IPCAM_SNAPSHOT;
#endif
//#NT#2016/06/28#Lincy Lin -end

//#NT#2016/07/27#Charlie Chang -begin
//#NT# support iqtool ipc
#if(IQTOOL_FUNC == ENABLE)
extern UINT32 POOL_ID_IQTOOLIPC;
#endif
//#NT#2016/07/27#Charlie Chang -end

//#NT#2016/11/11#Charlie Chang -begin
//#NT# support save face tracking snapshot
#if (FTG_SNAPSHOT_FUN == ENABLE)
extern UINT32 POOL_ID_FTG_SNAPSHOT;
#endif
//#NT#2016/11/11#Charlie Chang -end


//#NT#2016/08/19#Lincy Lin -begin
//#NT#Support linux cvhost function
#if (LINUX_CVHOST_FUNC == ENABLE)
extern UINT32 POOL_ID_CVHOST;
#endif
//#NT#2016/08/19#Lincy Lin -end
//#NT#2016/11/18#KS Hung -begin
//#NT#For Side By Side calibration from APP
#if (SBS_VIEW_FUNC == ENABLE)
extern UINT32 POOL_ID_SBS_CAL;
#endif
//#NT#2016/11/18#KS Hung -end

#define POOL_CNT_STORAGE_NAND   1
#define POOL_CNT_STORAGE_RAMDISK 1
#define POOL_CNT_STORAGE_SDIO   1
#define POOL_CNT_FWS_BUFFER     1
#if FS_MULTI_STRG_FUNC
//#NT#2016/12/09#Niven Cho -begin
//#NT#LINUX-EMMC-MULTI-STRG
#if (_CPU2_TYPE_==_CPU2_LINUX_ && _INTERSTORAGE_ == _INTERSTORAGE_EMMC_)
#define POOL_CNT_FS_BUFFER      3 // 3rd is for linux-pstore mounted by filesys
#else
#define POOL_CNT_FS_BUFFER      2
#endif
//#NT#2016/12/09#Niven Cho -end
#define POOL_CNT_DCF_BUFFER     2
#else
#define POOL_CNT_FS_BUFFER      1
#define POOL_CNT_DCF_BUFFER     1
#endif
#define POOL_CNT_FSIPC          1
#if PST_FUNC
#define POOL_CNT_PS_BUFFER      1
#endif
#define POOL_CNT_WIFIIPC        1
#if (USOCKET_CLIENT==ENABLE)
#define POOL_CNT_USOCKIPC       2
#else
#define POOL_CNT_USOCKIPC       1
#endif
#define POOL_CNT_DHCPIPC        1
#define POOL_CNT_FTYPEIPC       1
#if(FACE_GRADE_TEST==ENABLE)
#define POOL_CNT_FACEGIPC       1
#endif
#define POOL_CNT_GFX_IMAGE      1
#define POOL_CNT_GFX_STRING     1
#define POOL_CNT_GFX_TEMP       1
#if   (_SENSORLIB4_ != _SENSORLIB4_OFF_)
#define POOL_CNT_EXIF           4
#elif (_SENSORLIB3_ != _SENSORLIB3_OFF_)
#define POOL_CNT_EXIF           3
#elif (_SENSORLIB2_ != _SENSORLIB2_OFF_)
#define POOL_CNT_EXIF           2
#else
#define POOL_CNT_EXIF           1
#endif
#define POOL_CNT_DATEIMPRINT    1
#define POOL_CNT_FILEDB         1
#define POOL_CNT_PCC            1
//#NT#2016/08/30#Jeah Yen -begin
//#NT#support StreamReceiver
#define POOL_CNT_AUD_AOUT       1
#define POOL_CNT_AUD_IPC        1
//#NT#2016/08/30#Jeah Yen -end
#define POOL_CNT_DISP_OSD2      1
#define POOL_CNT_DISP_OSD2TEMP  1
#define POOL_CNT_DISP_OSD1      1
#define POOL_CNT_DISP_OSD1TEMP  1
#define POOL_CNT_DISP_VDO2      1
#define POOL_CNT_DISP_VDO2TEMP  1
#define POOL_CNT_DISP_VDO1      1
#define POOL_CNT_DISP_VDO1TEMP  1
#define POOL_CNT_DISP_VDO1TEMP3 2
#define POOL_CNT_APP            1
#define POOL_CNT_CPU2           1
#define POOL_CNT_DSP            1
#define POOL_CNT_HFSIPC         1
#if(YOUKU_SDK == ENABLE)
#define POOL_CNT_YOUKUIPC       1
#endif
//#NT#2016/05/31#Ben Wang -begin
//#NT#Add UVC multimedia function.
#if(UVC_MULTIMEDIA_FUNC == ENABLE)
#define POOL_CNT_USBCMD         1
#endif
//#NT#2016/05/31#Ben Wang -end
//#NT#2016/05/06#Jeah Yen -begin
//#NT#Support IOTP2P
#if (IOT_P2P_FUNC == ENABLE)
#define POOL_CNT_P2PIPC         1
#endif
//#NT#2016/05/06#Jeah Yen -end
#define POOL_CNT_NVTIPC         1
#define POOL_CNT_NVTUCTRL       1
#define POOL_CNT_CAL            1
//#NT#2016/04/21#Lincy Lin -begin
//#NT#Support curl (http client)
#if (CURL_FUNC == ENABLE)
#define POOL_CNT_CURLNVT        1
#endif
//#NT#2016/04/21#Lincy Lin -end
//#NT#Add memory pool for IPCAM snapshot using
#if (IPCAM_FUNC == ENABLE)
#define POOL_CNT_IPCAM_SNAPSHOT 1
#endif
//#NT#2016/06/28#Lincy Lin -end

//#NT#2016/07/27#Charlie Chang -begin
//#NT# support iqtoolipc
#define POOL_CNT_IQTOOLIPC    1
//#NT#2016/07/27#Charlie Chang -end

//#NT#2016/11/11#Charlie Chang -begin
//#NT# support save face tracking snapshot
#if (FTG_SNAPSHOT_FUN == ENABLE)
#define POOL_CNT_FTG_SNAPSHOT    1

#endif
//#NT#2016/11/11#Charlie Chang -end


//#NT#2016/08/19#Lincy Lin -begin
//#NT#Support linux cvhost function
#if (LINUX_CVHOST_FUNC == ENABLE)
#define POOL_CNT_CVHOST       1
#endif
//#NT#2016/08/19#Lincy Lin -end
//#NT#2016/11/18#KS Hung -begin
//#NT#For Side By Side calibration from APP
#if (SBS_VIEW_FUNC == ENABLE)
#define POOL_CNT_SBS_CAL       1
#endif
//#NT#2016/11/18#KS Hung -end
#define POOL_SIZE_STORAGE_NAND  ALIGN_CEIL_32((_EMBMEM_BLK_SIZE_+(_EMBMEM_BLK_SIZE_>>2)))
#define POOL_SIZE_STORAGE_RAMDISK  ALIGN_CEIL_32(0x20000)
#define POOL_SIZE_STORAGE_SDIO  ALIGN_CEIL_32(512)

/**
     POOL_SIZE_FWS_BUFFER

     For Partial Loading, In two case:
     1. un-compressed bin: set POOL_SIZE_FWS_BUFFER to 1 sector size (use STRG_GET_SECTOR_SIZE to get size)
     2. compressed bin: better to set it to (binsize + 2*sector_size)

*/
#define POOL_SIZE_FWS_BUFFER    ALIGN_CEIL_32(_EMBMEM_BLK_SIZE_)// 1 sector(block) size (for 2K Page)

// POOL_SIZE_FS_BUFFER:
// R/W buf = 0xAC000 (FileSysInfo=32k, OpenFile=2K*N, BitMap=256k, Sectbuf1=128K, SectBuf2=128k, ScanBuf=128k, ResvBuf=8k, Total 688k = 0xAC000)
// FAT buf = 0x80020 (FatBuff=512k + 32bytes reserved = 0x80020)
//#NT#2016/06/27#Lincy Lin -begin
//#NT#Reduce IPCAM used memory pool size
//#NT#2016/12/09#Niven Cho -begin
//#NT#LINUX-EMMC-MULTI-STRG
#if (_CPU2_TYPE_==_CPU2_LINUX_)
#define POOL_SIZE_FS_BUFFER     16*1024
#else
#define POOL_SIZE_FS_BUFFER     (ALIGN_CEIL_32(0xAC000)+ALIGN_CEIL_32(0x80020))
#endif
//#NT#2016/12/09#Niven Cho -end
//#NT#2016/06/27#Lincy Lin -end
#if(_CPU2_TYPE_ == _CPU2_ECOS_)
#define POOL_SIZE_FSIPC         ALIGN_CEIL_32(2*1024)
#else
#define POOL_SIZE_FSIPC         ALIGN_CEIL_32(0)
#endif
#define POOL_SIZE_DCF_BUFFER    ALIGN_CEIL_32(70*1024)
#if PST_FUNC
#define POOL_SIZE_PS_BUFFER     (ALIGN_CEIL_32(0x20000)+ALIGN_CEIL_32(0x20000))
#endif                                                                // 128K bytes = 1 block size + work
#define POOL_SIZE_WIFIIPC       ALIGN_CEIL_32(12*1024)
#define POOL_SIZE_USOCKIPC      ALIGN_CEIL_32(2048)
#define POOL_SIZE_DHCPIPC       ALIGN_CEIL_32(0x1580)
#define POOL_SIZE_TEMP          ALIGN_CEIL_32(0x40100)
#define POOL_SIZE_TEMP_TS       ALIGN_CEIL_32(0x250000)

//#NT#2016/08/08#Lincy Lin -begin
//#NT#Reduce gxgfx buffer
#if (GXGFX_ADVANCED_STRING_FUNC)
#define POOL_SIZE_GFX_TEMP      ALIGN_CEIL_32((VDO_W*VDO_H*2)+(0x1000)) //at least 4k for Gxlib
#else
#define POOL_SIZE_GFX_TEMP      ALIGN_CEIL_32(4096) //at least 4k for Gxlib
#endif
//#NT#2016/08/08#Lincy Lin -end

//#NT#2016/07/22#YongChang Qui -begin
//#NT#MovieStamp uses unified memory as IPC & MovieStamp buffer for IPCam with AE_LOG off
#if (_MODEL_DSC_ == _MODEL_TL_IPC350_)
#define POOL_SIZE_FTYPEIPC      ALIGN_CEIL_32( 1400000 * 10) //10 * sum of image resolution(exclusive mask)
#elif (_MODEL_DSC_ == _MODEL_TL_IPC340_)
#define POOL_SIZE_FTYPEIPC      ALIGN_CEIL_32( 1120000 * 10) //10 * sum of image resolution(exclusive mask)
#elif (_MODEL_DSC_ == _MODEL_TL_IPC330_)
#define POOL_SIZE_FTYPEIPC      ALIGN_CEIL_32( 840000 * 10) //10 * sum of image resolution(exclusive mask)
#else
#define POOL_SIZE_FTYPEIPC      ALIGN_CEIL_32( 560000 * 10) //10 * sum of image resolution(exclusive mask)
#endif
//#NT#2016/07/22#YongChang Qui -end
#if(FACE_GRADE_TEST==ENABLE)
#define POOL_SIZE_FACEGIPC      ALIGN_CEIL_32( 52000000 + 2048 * 1024)
#endif

#if (_LANG_STORE_ == _PSTORE_)
#define POOL_SIZE_GFX_STRING    ALIGN_CEIL_32(0x8000)
#else
#define POOL_SIZE_GFX_STRING    ALIGN_CEIL_32(0x20)                // Min Memory Pool size is 32 Bytes
#endif

#if (_JPG_STORE_ == _PSTORE_)
#define POOL_SIZE_GFX_IMAGE     ALIGN_CEIL_32(0x20000)
#else
#define POOL_SIZE_GFX_IMAGE     ALIGN_CEIL_32(0x00000020)
#endif

#define POOL_SIZE_EXIF          ALIGN_CEIL_32(0x10000)

//#NT#2016/07/22#YongChang Qui -begin
//#NT#MovieStamp uses unified memory as IPC & MovieStamp buffer for IPCam with AE_LOG off
#define POOL_SIZE_DATEIMPRINT   ALIGN_CEIL_32(0x200000)
//#NT#2016/07/22#YongChang Qui -end

#if USE_FILEDB
#define POOL_SIZE_FILEDB        ALIGN_CEIL_32(0x150000)
#else
#define POOL_SIZE_FILEDB        ALIGN_CEIL_32(0x20)
#endif

#if(_CPU2_TYPE_ ==_CPU2_NONE_)
#define POOL_SIZE_CPU2          ALIGN_CEIL_32(0) //set it 0 to disable
#elif(_CPU2_TYPE_ == _CPU2_ECOS_)
#define POOL_SIZE_CPU2          ALIGN_CEIL_32(COREMEM_ECOS_SIZE)
#elif(_CPU2_TYPE_ == _CPU2_LINUX_)
#define POOL_SIZE_CPU2          ALIGN_CEIL_32(COREMEM_LINUX_SIZE)
#endif

#if(_DSP_TYPE_ == _DSP_NONE_)
#define POOL_SIZE_DSP           ALIGN_CEIL_32(0) //set it 0 to disable
#elif(_DSP_TYPE_ == _DSP_FREERTOS_)
#define POOL_SIZE_DSP           ALIGN_CEIL_32(COREMEM_DSP_SIZE)
#endif
//#NT#2016/12/13#Lincy Lin -begin
//#NT#Enlarge http url command maximum length from 128 to 1024 bytes.
#if(_CPU2_TYPE_ == _CPU2_ECOS_)
#define POOL_SIZE_HFSIPC        ALIGN_CEIL_32(70*1024)
#elif(_CPU2_TYPE_ == _CPU2_LINUX_)
#define POOL_SIZE_HFSIPC        ALIGN_CEIL_32(70*1024)
#else
#define POOL_SIZE_HFSIPC        ALIGN_CEIL_32(0)
#endif
//#NT#2016/12/13#Lincy Lin -end

#if (YOUKU_SDK == ENABLE)
#define POOL_SIZE_YOUKUIPC      ALIGN_CEIL_32(512)
#endif
//#NT#2016/05/31#Ben Wang -begin
//#NT#Add UVC multimedia function.
#if(UVC_MULTIMEDIA_FUNC == ENABLE)
#define POOL_SIZE_USBCMD        ALIGN_CEIL_32(64*1024)
#endif
//#NT#2016/05/31#Ben Wang -end
//#NT#2016/05/06#Jeah Yen -begin
//#NT#Support IOTP2P
#if (IOT_P2P_FUNC == ENABLE)
//#NT#2016/06/03#Charlie Chang -begin
//#NT#redefine p2pipc size from 1024 to 2048
#define POOL_SIZE_P2PIPC        ALIGN_CEIL_32(2048)
//#NT#2016/06/03#Charlie Chang -end
#endif
//#NT#2016/05/06#Jeah Yen -end

//#NT#2016/08/30#Jeah Yen -begin
//#NT#support StreamReceiver
#if (AUDIO_PLAY_FUNCTION == ENABLE)
#define POOL_SIZE_AUD_IPC       ALIGN_CEIL_32(48000/10*2) ///< 1 unit audio data, size = 48000/10*2, 48000=sample-rate, 2=sample-bytes, 1=channel, 1/10=100ms
#endif
//#NT#2016/08/30#Jeah Yen -end

#define POOL_SIZE_NVTIPC        ALIGN_CEIL_32(8192)


#if(UCTRL_FUNC == ENABLE)
#define POOL_SIZE_NVTUCTRL      ALIGN_CEIL_32(1024)
#else
#define POOL_SIZE_NVTUCTRL      ALIGN_CEIL_32(0)
#endif

#if (CALIBRATION_FUNC == ENABLE)

//#NT#2016/07/20#XL_Wei -begin
//#NT#IPCAM(TP_LINK) Calbiration Memory control
#if (IPCAM_FUNC == ENABLE)
#define POOL_SIZE_CAL           ALIGN_CEIL_32(0) ///< here size needs to ask IQ how many should be used
#else
#define POOL_SIZE_CAL           ALIGN_CEIL_32(0x530000) ///< here size needs to ask IQ how many should be used
#endif
//#NT#2016/07/20#XL_Wei -end

#else
#define POOL_SIZE_CAL           ALIGN_CEIL_32(0) ///< here size needs to ask IQ how many should be used
#endif
//#NT#2016/04/21#Lincy Lin -begin
//#NT#Support curl (http client)
#if (CURL_FUNC == ENABLE)
#define POOL_SIZE_CURLNVT       ALIGN_CEIL_32((1024+16384)*4) ///<
#endif
//#NT#2016/04/21#Lincy Lin -end

//#NT#2016/08/31#Lincy Lin -end
//#NT#Adjust IPCAM snapshot mempool size by max image w & h
#if (IPCAM_FUNC == ENABLE)
#define POOL_SIZE_IPCAM_SNAPSHOT    ALIGN_CEIL_32(CROP_SOURCE_W*CROP_SOURCE_H/100*12)
#endif
//#NT#2016/08/31#Lincy Lin -end

//#NT#2016/07/27#Charlie Chang -begin
//#NT# support IQTOOL IPC
#define POOL_SIZE_IQTOOL_IPC  ALIGN_CEIL_32(55*1024)
//#NT#2016/07/27#Charlie Chang -end

//#NT#2016/11/11#Charlie Chang -begin
//#NT# support save face tracking snapshot
#if (FTG_SNAPSHOT_FUN == ENABLE)
#define POOL_SIZE_FTG_SNAPSHOT ALIGN_CEIL_32(FTG_MAX_SNAPSHOT_SIZE)
#endif
//#NT#2016/11/11#Charlie Chang -end

//#NT#2016/08/19#Lincy Lin -begin
//#NT#Support linux cvhost function
#if (LINUX_CVHOST_FUNC == ENABLE)
#define POOL_SIZE_CVHOST      ALIGN_CEIL_32(10*1024+0x200000)   // 10KB for command buffer + 2MB for image buffer FHD Y only
#endif
//#NT#2016/08/19#Lincy Lin -end
//#NT#2016/11/18#KS Hung -begin
//#NT#For Side By Side calibration from APP
#if (SBS_VIEW_FUNC == ENABLE)
#define POOL_SIZE_SBS_CAL     ALIGN_CEIL_32(0x80000) //512KB for Side By Side calibration param from APP
#define PS_SBS_CAL_PARAM            "SBS_CAL"
#endif
//#NT#2016/11/18#KS Hung -end
#define POOL_SIZE_APP           ((UINT32)OS_GetMempoolSize(POOL_ID_APP)) //for query pool size of POOL_ID_APP after config
#define POOL_SIZE_FREE          ((UINT32)OS_GetMemFreeSize(MEM_HEAP)) //query current free heap memory size to config pool size of POOL_ID_APP

extern USIZE Calc_VideoSize_LCD(void) _SECTION(".kercfg_text");
extern USIZE Calc_VideoSize_MAX(void) _SECTION(".kercfg_text");


extern void Install_User(void);
extern void Install_UserMEM(void);

extern void Install_SystemMEM(void);

extern void Install_DrvMEM(void);
//Embedded Storage
extern void Install_DevEmbMEM(void);
//Storage
extern void Install_DevStrgMEM(void);
//OSD
extern void Install_DevOsdMEM(void);
//Video
extern void Install_DevVideoMEM(void);
//Imagepipe
extern void Install_DevImageMEM(void);
//Video Codec
extern void Install_DevVCodecMEM(void);
//Network
extern void Install_DevNetMEM(void);

extern void ImageStream_ConfigUserDump(void);
extern void UI_GetOSDBuf(UINT32* pAddr, UINT32* pSize);


#endif //_SYSCFG_H

