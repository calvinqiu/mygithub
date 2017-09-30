#include "PrjCfg.h"
#if (YOUKU_SDK == ENABLE)
#include "YoukuAppCmd.h"
#include "NVTToolCommand.h"
#include "UIInfo.h"
#include "ykcamera_common.h"
#include "ykcamera.h"
#include "ykcamera_parameter.h"
#include "StatusData.h"
#include "UIFlowWndWifiMovie.h"
#include "SysCommon.h"
#include "UIMovieMapping.h"
#include "rtc.h"
#include "ykcamera_error.h"
#include "UIFlowMovieFuncs.h"
#include "UIAppMovie.h"
#include "UIAppPhoto.h"
#include "UIAppNetwork.h"
#include "FileDB.h"
#include "UIMovieMapping.h"
#include "WifiAppCmd.h"
#include "GxVideoFile.h"
#include "Exif.h"
#include "ExifDef.h"
#include "FileDB.h"
#include "PowerDef.h"
#include "YoukuIpcAPI.h"

#define THIS_DBGLVL         2 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          YoukuAppCmd
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////
#define UITRON_FS_ROOT      "A:\\"
#define MOUNT_FS_ROOT        "/sdcard"

#define DEVICE_COMPANY_NAME           "Novatek"

#define STATUS_NOT_SUPPORT          0xFFFFFFFF

#define DEFAULT_SSID        "DV_"  //this SSID will append MAC
#define DEFAULT_PASSPHRASE  "00000000"


#define PHOTO_FOLDER    "PHOTO"
#define MOVIE_FOLDER    "MOVIE"

#define ALIGNED_POINTER(n)    ((n) && (((n)&0x3)==0))

static CHAR    DateTimeStr[20] = {0};
static int    m_LedBlinkSetting = 0;
static BOOL   m_bNewUploadVideo = FALSE;

#define ipc_getNonCacheAddr(addr)   (((addr) & 0x1FFFFFFF)|0xA0000000)


#if 0
static void DumpMem(UINT32 Addr, UINT32 Size, UINT32 Alignment)
{
    UINT32 i;
    UINT8 *pBuf = (UINT8 *)Addr;
    for (i=0;i<Size;i++)
    {
        if(i%Alignment == 0)
        {
            DBG_DUMP("\r\n");
        }
        DBG_DUMP("0x%02X ",*(pBuf+i));
    }
    DBG_DUMP("\r\n");
}
#endif
static FST_FILE          gMovFilehdl;
static FILEDB_INIT_OBJ gFDBInitObj={
                         "A:\\",  //rootPath
                         NULL,  //defaultfolder
                         NULL,  //filterfolder
                         TRUE,  //bIsRecursive
                         TRUE,  //bIsCyclic
                         TRUE,  //bIsMoveToLastFile
                         TRUE, //bIsSupportLongName
                         FALSE, //bIsDCFFileOnly
                         TRUE,  //bIsFilterOutSameDCFNumFolder
                         {'N','V','T','I','M'}, //OurDCFDirName[5]
                         {'I','M','A','G'}, //OurDCFFileName[4]
                         FALSE,  //bIsFilterOutSameDCFNumFile
                         FALSE, //bIsChkHasFile
                         60,    //u32MaxFilePathLen
                         10000,  //u32MaxFileNum
                         (FILEDB_FMT_JPG|FILEDB_FMT_AVI|FILEDB_FMT_MOV|FILEDB_FMT_MP4|FILEDB_FMT_TS),
                         0,     //u32MemAddr
                         0,     //u32MemSize
                         NULL   //fpChkAbort

};
//============ for UI Mapping Begin =================
#define ITEM_NUM(n) (sizeof(n)/sizeof(YOUKU_UI_ENTRY))
YOUKU_UI_BEGIN(VideoResoFPS)
YOUKU_UI_ITEM(YK_VIDEO_WVGA_30FPS,  MOVIE_SIZE_FRONT_848x480P30)
YOUKU_UI_ITEM(YK_VIDEO_720P_30FPS,  MOVIE_SIZE_FRONT_1280x720P30)
YOUKU_UI_ITEM(YK_VIDEO_1080P_30FPS, MOVIE_SIZE_FRONT_1920x1080P30)
YOUKU_UI_ITEM(YK_VIDEO_1080P_60FPS, MOVIE_SIZE_FRONT_1920x1080P60)
YOUKU_UI_ITEM(YK_VIDEO_3MHD_30FPS,  MOVIE_SIZE_FRONT_2304x1296P30)
YOUKU_UI_ITEM(YK_VIDEO_QHD_30FPS,   MOVIE_SIZE_FRONT_2560x1440P30)
YOUKU_UI_END()
#if 0
YOUKU_UI_BEGIN(VideoFOV)
YOUKU_UI_ITEM(YK_FOV_WIDE,   FOV_ULTRA_WIDE)
YOUKU_UI_ITEM(YK_FOV_MEDIUM, FOV_MEDIUM)
YOUKU_UI_ITEM(YK_FOV_NARROW, FOV_NARROW)
YOUKU_UI_ITEM(YK_FOV_CROP,   FOV_CROP)
YOUKU_UI_END()
#endif
YOUKU_UI_BEGIN(VideoTimelapseRate)
YOUKU_UI_ITEM(YK_VIDEO_TIMELAPSE_CLOSE, MOVIE_TIMELAPSEREC_OFF)
YOUKU_UI_ITEM(YK_VIDEO_TIMELAPSE_1S, MOVIE_TIMELAPSEREC_1SEC)
YOUKU_UI_ITEM(YK_VIDEO_TIMELAPSE_5S, MOVIE_TIMELAPSEREC_5SEC)
YOUKU_UI_ITEM(YK_VIDEO_TIMELAPSE_10S, MOVIE_TIMELAPSEREC_10SEC)
YOUKU_UI_ITEM(YK_VIDEO_TIMELAPSE_30S, MOVIE_TIMELAPSEREC_30SEC)
YOUKU_UI_ITEM(YK_VIDEO_TIMELAPSE_1M, MOVIE_TIMELAPSEREC_1MIN)
YOUKU_UI_ITEM(YK_VIDEO_TIMELAPSE_5M, MOVIE_TIMELAPSEREC_5MIN)
YOUKU_UI_ITEM(YK_VIDEO_TIMELAPSE_10M, MOVIE_TIMELAPSEREC_10MIN)
YOUKU_UI_ITEM(YK_VIDEO_TIMELAPSE_30MIN, MOVIE_TIMELAPSEREC_30MIN)
YOUKU_UI_ITEM(YK_VIDEO_TIMELAPSE_1H, MOVIE_TIMELAPSEREC_1HOUR)
YOUKU_UI_ITEM(YK_VIDEO_TIMELAPSE_2H, MOVIE_TIMELAPSEREC_2HOUR)
YOUKU_UI_ITEM(YK_VIDEO_TIMELAPSE_3H, MOVIE_TIMELAPSEREC_3HOUR)
YOUKU_UI_ITEM(YK_VIDEO_TIMELAPSE_1D, MOVIE_TIMELAPSEREC_1DAY)
YOUKU_UI_END()

YOUKU_UI_BEGIN(VideoLooping)
YOUKU_UI_ITEM(YK_VIDEO_LOOPING_OFF,   MOVIE_CYCLICREC_OFF)
YOUKU_UI_ITEM(YK_VIDEO_LOOPING_3MIN,  MOVIE_CYCLICREC_3MIN)
YOUKU_UI_ITEM(YK_VIDEO_LOOPING_5MIN,  MOVIE_CYCLICREC_5MIN)
YOUKU_UI_ITEM(YK_VIDEO_LOOPING_10MIN, MOVIE_CYCLICREC_10MIN)
YOUKU_UI_END()
#if 0
YOUKU_UI_BEGIN(VideoSpotMeter)
YOUKU_UI_ITEM(YK_METERING_CENTER,MOVIE_CYCLICREC_OFF)
YOUKU_UI_ITEM(YK_METERING_MULTI, METERING_MULTI)
YOUKU_UI_ITEM(YK_METERING_SPOT,  METERING_SPOT)
YOUKU_UI_ITEM(YK_METERING_AIAE,  METERING_AIAE)
YOUKU_UI_END()

YOUKU_UI_BEGIN(VideoEffect)
YOUKU_UI_ITEM(YK_EFFECT_NONE,       IMAGE_EFFECT_NONE)
YOUKU_UI_ITEM(YK_EFFECT_SKETCH,     IMAGE_EFFECT_SKETCH)
YOUKU_UI_ITEM(YK_EFFECT_FISHEYE,    IMAGE_EFFECT_FISHEYE)
YOUKU_UI_ITEM(YK_EFFECT_COLORPENCIL,IMAGE_EFFECT_COLORPENCIL)
YOUKU_UI_END()
#endif
YOUKU_UI_BEGIN(VideoWB)
YOUKU_UI_ITEM(YK_WB_AUTO,       WB_AUTO)
YOUKU_UI_ITEM(YK_WB_DAYLIGHT,   WB_DAYLIGHT)
YOUKU_UI_ITEM(YK_WB_CLOUDY,     WB_CLOUDY)
YOUKU_UI_ITEM(YK_WB_TUNGSTEN,   WB_TUNGSTEN)
YOUKU_UI_ITEM(YK_WB_FLUORESCENT,WB_FLUORESCENT)
YOUKU_UI_END()
#if 0
YOUKU_UI_BEGIN(VideoColor)
YOUKU_UI_ITEM(YK_COLOR_STANDARD,  COLOR_EFFECT_STANDARD)
YOUKU_UI_ITEM(YK_COLOR_MONOCHROME,COLOR_EFFECT_MONOCHROME)
YOUKU_UI_ITEM(YK_COLOR_SEPIR,     COLOR_EFFECT_SEPIA)
YOUKU_UI_END()
#endif
YOUKU_UI_BEGIN(VideoISO)
YOUKU_UI_ITEM(YK_ISO_AUTO,ISO_AUTO)
YOUKU_UI_ITEM(YK_ISO_100, ISO_100)
YOUKU_UI_ITEM(YK_ISO_200, ISO_200)
YOUKU_UI_ITEM(YK_ISO_400, ISO_400)
YOUKU_UI_END()
#if 1
YOUKU_UI_BEGIN(VideoSharpness)
YOUKU_UI_ITEM(YK_SHARPNESS_HIGH,   SHARPNESS_SHARP)
YOUKU_UI_ITEM(YK_SHARPNESS_MEDIUM, SHARPNESS_NORMAL)
YOUKU_UI_ITEM(YK_SHARPNESS_LOW,    SHARPNESS_SOFT)
YOUKU_UI_END()
#endif
YOUKU_UI_BEGIN(VideoEV)
YOUKU_UI_ITEM(YK_EV_N20, EV_N20)
YOUKU_UI_ITEM(YK_EV_N16, EV_N16)
YOUKU_UI_ITEM(YK_EV_N13, EV_N13)
YOUKU_UI_ITEM(YK_EV_N10, EV_N10)
YOUKU_UI_ITEM(YK_EV_N06, EV_N06)
YOUKU_UI_ITEM(YK_EV_N03, EV_N03)
YOUKU_UI_ITEM(YK_EV_0  , EV_00)
YOUKU_UI_ITEM(YK_EV_P03, EV_P03)
YOUKU_UI_ITEM(YK_EV_P06, EV_P06)
YOUKU_UI_ITEM(YK_EV_P10, EV_P10)
YOUKU_UI_ITEM(YK_EV_P13, EV_P13)
YOUKU_UI_ITEM(YK_EV_P16, EV_P16)
YOUKU_UI_ITEM(YK_EV_P20, EV_P20)
YOUKU_UI_END()

YOUKU_UI_BEGIN(PhotoDefaultMode)
YOUKU_UI_ITEM(YK_PHOTO_SUBMODE_SINGLE,CONTINUE_SHOT_OFF)
YOUKU_UI_ITEM(YK_PHOTO_SUBMODE_MULTI, CONTINUE_SHOT_BURST)
YOUKU_UI_END()

YOUKU_UI_BEGIN(PhotoResolution)
YOUKU_UI_ITEM(YK_PHOTO_RESOLUTION_1M,PHOTO_SIZE_1M)
YOUKU_UI_ITEM(YK_PHOTO_RESOLUTION_VGA, PHOTO_SIZE_VGA)
YOUKU_UI_ITEM(YK_PHOTO_RESOLUTION_2M,PHOTO_SIZE_2MHD)
YOUKU_UI_ITEM(YK_PHOTO_RESOLUTION_3M, PHOTO_SIZE_3M)
YOUKU_UI_ITEM(YK_PHOTO_RESOLUTION_5M,PHOTO_SIZE_5M)
YOUKU_UI_ITEM(YK_PHOTO_RESOLUTION_8M, PHOTO_SIZE_8M)
YOUKU_UI_ITEM(YK_PHOTO_RESOLUTION_10M,PHOTO_SIZE_10M)
YOUKU_UI_ITEM(YK_PHOTO_RESOLUTION_12M, PHOTO_SIZE_12M)
YOUKU_UI_END()

YOUKU_UI_BEGIN(PhotoDateTimePrint)
YOUKU_UI_ITEM(YK_PHOTO_DATETIME_OFF,       DATE_STAMP_OFF)
YOUKU_UI_ITEM(YK_PHOTO_DATETIME_DATE_ONLY, DATE_STAMP_DATE)
YOUKU_UI_ITEM(YK_PHOTO_DATETIME_DATE_TIME, DATE_STAMP_DATE_TIME)
YOUKU_UI_END()

YOUKU_UI_BEGIN(DevStreamSize)
YOUKU_UI_ITEM(YK_STREAM_WVGA,    WIFI_MOVIE_APP_PREVIEW_SIZE_WVGA)
YOUKU_UI_ITEM(YK_STREAM_VGA_4_3, WIFI_MOVIE_APP_PREVIEW_SIZE_VGA_4_3)
YOUKU_UI_ITEM(YK_STREAM_VGA_16_9,WIFI_MOVIE_APP_PREVIEW_SIZE_VGA_16_9)
YOUKU_UI_END()

#if (AUTOSLEEP_FUNCTION == ENABLE)
YOUKU_UI_BEGIN(LcdSleep)
YOUKU_UI_ITEM(YK_LCD_SLEEP_CLOSE, LCDOFF_ON)
YOUKU_UI_ITEM(YK_LCD_SLEEP_1M,    LCDOFF_1MIN)
YOUKU_UI_END()
#endif
YOUKU_UI_BEGIN(BeepVolume)
YOUKU_UI_ITEM(YK_BEEP_VOLUME_OFF,    BEEP_VOL_MUTE)
YOUKU_UI_ITEM(YK_BEEP_VOLUME_MEDIUM, BEEP_VOL_2)
YOUKU_UI_ITEM(YK_BEEP_VOLUME_HIGH,   BEEP_VOL_3)
YOUKU_UI_END()

YOUKU_UI_BEGIN(AutoPowerDown)
YOUKU_UI_ITEM(YK_AUTO_POWERDOWN_OFF, POWER_ON)
YOUKU_UI_ITEM(YK_AUTO_POWERDOWN_3M,  POWER_3MIN)
YOUKU_UI_ITEM(YK_AUTO_POWERDOWN_5M,  POWER_5MIN)
YOUKU_UI_ITEM(YK_AUTO_POWERDOWN_10M, POWER_10MIN)
YOUKU_UI_END()
static INT32 GetUIMappingFromYK(YOUKU_UI_ENTRY *pTable, UINT32 ItemNum, INT32 value, INT32 UnknownResult)
{
    UINT32 index;

    for(index=0;index<ItemNum;index++)
    {
        if (value == pTable[index].YoukuItem)
        {
            return pTable[index].NvtItem;
        }
    }
    DBG_WRN("Not support this YOUKU value(%d)\r\n",value);
    return UnknownResult;
}
static INT32 GetUIMappingFromNVT(YOUKU_UI_ENTRY *pTable, UINT32 ItemNum, INT32 value, INT32 UnknownResult)
{
    UINT32 index;

    for(index=0;index<ItemNum;index++)
    {
        if (value == pTable[index].NvtItem)
        {
            return pTable[index].YoukuItem;
        }
    }
    DBG_WRN("Not support this NVT value(%d)\r\n",value);
    return UnknownResult;
}
static UINT32 GetStreamBitRateFromYK(YK_STREAM_BIT_RATE_ITEM value)
{
    //To do ?
    //check stream size limitation ?
    UINT32 TargetSize = 0;
    switch(value)
    {
        case YK_STREAM_BIT_RATE_800K:
            TargetSize = 800*1024/8;
        break;
        case YK_STREAM_BIT_RATE_1000K:
            TargetSize = 1000*1024/8;
        break;
        case YK_STREAM_BIT_RATE_1600K:
            TargetSize = 1600*1024/8;
        break;
        case YK_STREAM_BIT_RATE_2000K:
            TargetSize = 2000*1024/8;
        break;
        case YK_STREAM_BIT_RATE_2400K:
            TargetSize = 2400*1024/8;
        break;
        case YK_STREAM_BIT_RATE_3200K:
            TargetSize = 3200*1024/8;
        break;
        case YK_STREAM_BIT_RATE_6400K:
            TargetSize = 6400*1024/8;
        break;
        case YK_STREAM_BIT_RATE_9600K:
            TargetSize = 9600*1024/8;
        break;
        default:
            DBG_WRN("Unknown setting (%d).\r\n",value);
        break;
    }
    return TargetSize;
}
static YK_STREAM_BIT_RATE_ITEM GetStreamBitRateFromNVT(UINT32 TargetSize)
{
    YK_STREAM_BIT_RATE_ITEM YkBitrate;

    if(TargetSize <= 800*1024/8)
        YkBitrate = YK_STREAM_BIT_RATE_800K;
    else if(TargetSize <= 1000*1024/8)
        YkBitrate = YK_STREAM_BIT_RATE_1000K;
    else if(TargetSize <= 1000*1024/8)
        YkBitrate = YK_STREAM_BIT_RATE_1000K;
    else if(TargetSize <= 1600*1024/8)
        YkBitrate = YK_STREAM_BIT_RATE_1600K;
    else if(TargetSize <= 2000*1024/8)
        YkBitrate = YK_STREAM_BIT_RATE_2000K;
    else if(TargetSize <= 2400*1024/8)
        YkBitrate = YK_STREAM_BIT_RATE_2400K;
    else if(TargetSize <= 3200*1024/8)
        YkBitrate = YK_STREAM_BIT_RATE_3200K;
    else if(TargetSize <= 6400*1024/8)
        YkBitrate = YK_STREAM_BIT_RATE_6400K;
    else //if(TargetSize <= 9600*1024/8)
        YkBitrate = YK_STREAM_BIT_RATE_9600K;

    return YkBitrate;
}
//============ for UI Mapping End =================

static ER VdoReadCB(UINT32 pos, UINT32 size, UINT32 addr)
{
    ER result =E_SYS;

    //DBG_IND("VdoReadCB  pos=0x%x, size=%d, addr=0x%x\r\n",pos,size,addr);
    if(gMovFilehdl)
    {
        FileSys_SeekFile(gMovFilehdl, pos, FST_SEEK_SET);
        //not close file,close file in XML_GetThumbnail()
        result=FileSys_ReadFile(gMovFilehdl, (UINT8 *)addr, &size, 0,0);
    }
    return result;
}
static BOOL YoukuSdk_ChkMode(int SupportedMode)
{
    if (System_GetState(SYS_STATE_CURRMODE) != SupportedMode)
    {
        DBG_ERR("Not support this mode!(Curr=%d)\r\n",System_GetState(SYS_STATE_CURRMODE));
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}
static BOOL YoukuSdk_IsRecording(void)
{
    if (UIFlowWndWiFiMovie_GetStatus() == WIFI_MOV_ST_RECORD)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
static INT32 YoukuSdk_ChkCardStatus(void)
{
    UINT32 CartStatus = UI_GetData(FL_CardStatus);

    if(CartStatus == CARD_LOCKED)
    {
        return YOUKU_CMD_CARD_LOCKED;
    }
    else if (CartStatus == CARD_REMOVED)
    {
        return YOUKU_CMD_CARD_REMOVED;
    }
    else
    {
        return YOUKU_CMD_SUCCESS;
    }
}
static void YoukuPath2NvtPath(const char* inPath, char* outPath)
{
    char *temp;
    temp = strstr(inPath, MOUNT_FS_ROOT);
    if( 0 == temp)
    {
        DBG_ERR("Path ERROR(%s)\r\n",inPath);
        return;
    }
    temp += strlen(MOUNT_FS_ROOT);
    outPath+=sprintf(outPath,"A:");
    //inPath+=strlen(MOUNT_FS_ROOT);
    while (*temp !=0)
    {
        if (*temp == '/')
            *outPath = '\\';
        else
            *outPath = *temp;
        temp++;
        outPath++;
    }
    //*outPath++ = '\\'; //If adding this, it will be regarded as folder.
    *outPath = 0;
}
static void NvtPath2YoukuPath(const char* inPath, char* outPath)
{
    char *temp;
    temp = strstr(inPath, UITRON_FS_ROOT);
    if( 0 == temp)
    {
        DBG_ERR("Path ERROR(%s)\r\n",inPath);
        return;
    }
    temp += strlen(UITRON_FS_ROOT);
    outPath+=sprintf(outPath,MOUNT_FS_ROOT);
    *outPath++ = '/';
    while (*temp !=0)
    {
        if (*temp == '\\')
            *outPath = '/';
        else
            *outPath = *temp;
        temp++;
        outPath++;
    }
    *outPath = 0;
}
/**
  delete related "MOVIE_WIFI" file by "MOVIE" file name
  EX: DeleteMovieWiFi("A:\\CarDV\\MOVIE\\aaa.mov") will delete A:\CarDV\MOVIE_WIFI\aaa.mov
*/
static void DeleteMovieWiFi(char * pMovieFilePath)
{
    char MovieFolder[128] = {0};
    char MovieWiFiPath[128] = {0};
    char *temp;
    snprintf(MovieFolder,sizeof(MovieFolder),"A:\\%s\\%s\\",FILEDB_CARDV_ROOT,MOVIE_FOLDER);
    temp = strstr(pMovieFilePath, MovieFolder);
    if( 0 == temp)
    {
        return;
    }
    snprintf(MovieWiFiPath,sizeof(MovieWiFiPath),"A:\\%s\\%s\\%s",FILEDB_CARDV_ROOT,WIFI_MOVIE_FOLDER,pMovieFilePath+strlen(MovieFolder));
    DBG_IND("Movie Path=%s\r\n",pMovieFilePath);
    DBG_IND("MovieWiFiPath Path=%s\r\n",MovieWiFiPath);
    FileSys_DeleteFile(MovieWiFiPath);
}

static UINT32 GetDeviceStatus(cam_device_status_item item)
{
    UINT32 Ret = STATUS_NOT_SUPPORT;
    RTC_DATE    Date;
    RTC_TIME    Time;

    switch(item)
    {
        case DEVICE_INTERNAL_BATTERY_PRESENT:
            if (KeyScan_IsACIn())
            {
                Ret = 100;
            }
            else
            {
                switch (GxPower_GetControl(GXPWR_CTRL_BATTERY_LEVEL))
                {
                  case VOLDET_BATTERY_LVL_4:
                       Ret = 100;
                    break;
                  case VOLDET_BATTERY_LVL_3:
                       Ret = 75;
                    break;
                  case VOLDET_BATTERY_LVL_2:
                       Ret = 50;
                    break;
                  case VOLDET_BATTERY_LVL_1:
                       Ret = 25;
                    break;
                  case VOLDET_BATTERY_LVL_0:
                       Ret = 5;
                    break;
                }
            }
        break;
        case DEVICE_EXTERNAL_BATTERY_PRESENT://charger
            Ret = KeyScan_IsACIn();
        break;

        case DEVICE_DATE_TIME:

            Date = rtc_getDate();
            Time = rtc_getTime();
            sprintf(DateTimeStr,"%04d/%02d/%02d %02d:%02d:%02d",Date.s.year,Date.s.month,Date.s.day,Time.s.hour,Time.s.minute,Time.s.second);
            //DBG_IND("DateTimeStr=%s\r\n",DateTimeStr);
            Ret = (UINT32)DateTimeStr;
        break;
        case VIDEO_PROGRESS_COUNTER:
            if(YoukuSdk_IsRecording())
                Ret = FlowMovie_GetRecCurrTime();
        break;
        case SETTING_COMM_STREAM_SUPPORT:
            Ret = 1;
        break;
        case STORAGE_SD_STATUS:
            Ret = UI_GetData(FL_CardStatus)==DEVICE_STATUS_STORAGE_SD_INSERTED?1:0;
        break;
        case STORAGE_REMAINING_PHOTOS:
            Ret = PhotoExe_GetFreePicNum();
        break;
#if 0
        case STORAGE_REMAINING_VIDEO_TIME:
            Ret = UIFlowWiFiMovie_GetMaxRecTime();
        break;
#endif
        case STORAGE_NUM_TOTAL_PHOTOS:
            if(System_GetState(SYS_STATE_CURRMODE) == PRIMARY_MODE_PHOTO|| System_GetState(SYS_STATE_CURRMODE) == PRIMARY_MODE_PLAYBACK)
            {
                Ret = FileDB_GetFilesNumByFileType(0, FILEDB_FMT_JPG);
            }
        break;
        case STORAGE_NUM_TOTAL_VIDEOS:
            if(System_GetState(SYS_STATE_CURRMODE) == PRIMARY_MODE_MOVIE|| System_GetState(SYS_STATE_CURRMODE) == PRIMARY_MODE_PLAYBACK)
            {
                Ret = FileDB_GetFilesNumByFileType(0, FILEDB_FMT_MOV) + FileDB_GetFilesNumByFileType(0, FILEDB_FMT_MP4);
            }
        break;
        case STORAGE_REMAINING_SPACE:
            //unit is KB
            Ret = (UINT32)(FileSys_GetDiskInfo(FST_INFO_FREE_SPACE)>>10);
        break;
        case OTA_STATUS:
        case OTA_DOWNLOAD_CANCEL_REQUEST_PENDING:
        default:
            //DBG_ERR("Not support ID:%d\r\n",item);
        break;
    }
    return Ret;
}


static UINT32 GetSettingStatus(cam_setting_status_item item)
{
    UINT32 Ret = STATUS_NOT_SUPPORT;
    INT32 curMode;
    switch(item)
    {
        case SETTING_VIDEO_RESOLUTION_FPS:
            Ret = GetUIMappingFromNVT(VideoResoFPS, ITEM_NUM(VideoResoFPS), UI_GetData(FL_MOVIE_SIZE), STATUS_NOT_SUPPORT);
        break;
    #if 0
        case SETTING_VIDEO_FOV:
            Ret = GetUIMappingFromNVT(VideoFOV, ITEM_NUM(VideoFOV), UI_GetData(FL_MOVIE_FOV), STATUS_NOT_SUPPORT);
        break;
    #endif
        case SETTING_VIDEO_TIMELAPSE_RATE:
            Ret = GetUIMappingFromNVT(VideoTimelapseRate, ITEM_NUM(VideoTimelapseRate), UI_GetData(FL_MOVIE_TIMELAPSE_REC), STATUS_NOT_SUPPORT);
        break;
        case SETTING_VIDEO_LOOPING:
            Ret = GetUIMappingFromNVT(VideoLooping, ITEM_NUM(VideoLooping), UI_GetData(FL_MOVIE_CYCLIC_REC), STATUS_NOT_SUPPORT);
        break;
    #if 0
        case SETTING_VIDEO_SPOT_METER:
            Ret = GetUIMappingFromNVT(VideoSpotMeter, ITEM_NUM(VideoSpotMeter), UI_GetData(FL_MovieMeteringIndex), STATUS_NOT_SUPPORT);
        break;
        case SETTING_VIDEO_EFFECT:
        case SETTING_PHOTO_EFFECT:
            Ret = GetUIMappingFromNVT(VideoEffect, ITEM_NUM(VideoEffect), UI_GetData(FL_IMAGE_EFFECT), STATUS_NOT_SUPPORT);
        break;
    #endif
        case SETTING_VIDEO_WHITE_BALANCE:
        case SETTING_PHOTO_WHITE_BALANCE:
            Ret = GetUIMappingFromNVT(VideoWB, ITEM_NUM(VideoWB), UI_GetData(FL_WB), STATUS_NOT_SUPPORT);
        break;
        #if 0
        case SETTING_VIDEO_COLOR:
        case SETTING_PHOTO_COLOR:
            Ret = GetUIMappingFromNVT(VideoColor, ITEM_NUM(VideoColor), UI_GetData(FL_COLOR_EFFECT), STATUS_NOT_SUPPORT);
        break;
        #endif
        case SETTING_VIDEO_ISO:
        case SETTING_PHOTO_ISO:
            Ret = GetUIMappingFromNVT(VideoISO, ITEM_NUM(VideoISO), UI_GetData(FL_ISO), STATUS_NOT_SUPPORT);
        break;
        #if 1
        case SETTING_VIDEO_SHARPNESS:
        case SETTING_PHOTO_SHARPNESS:
            Ret = GetUIMappingFromNVT(VideoSharpness, ITEM_NUM(VideoSharpness), UI_GetData(FL_SHARPNESS), STATUS_NOT_SUPPORT);
        break;
        #endif
        case SETTING_VIDEO_EV:
        case SETTING_PHOTO_EV:
            Ret = GetUIMappingFromNVT(VideoEV, ITEM_NUM(VideoEV), UI_GetData(FL_EV), STATUS_NOT_SUPPORT);
        break;
        case SETTING_VIDEO_DATETIME_PR:
            Ret = UI_GetData(FL_MOVIE_DATEIMPRINT);
        break;
        case SETTING_VIDEO_AUDIO_RECORD:
            Ret = UI_GetData(FL_MOVIE_AUDIO);
        break;
        case SETTING_VIDEO_WDR:
            Ret = UI_GetData(FL_MOVIE_WDR);
        break;
        case SETTING_PHOTO_DEFAULT_MODE:
            Ret = GetUIMappingFromNVT(PhotoDefaultMode, ITEM_NUM(PhotoDefaultMode), UI_GetData(FL_CONTINUE_SHOT), STATUS_NOT_SUPPORT);
        break;
        case SETTING_PHOTO_RESOLUTION:
            Ret = GetUIMappingFromNVT(PhotoResolution, ITEM_NUM(PhotoResolution), UI_GetData(FL_PHOTO_SIZE), STATUS_NOT_SUPPORT);
        break;
        #if 0
        case SETTING_PHOTO_SPOT_METER:
            Ret = GetUIMappingFromNVT(VideoSpotMeter, ITEM_NUM(VideoSpotMeter), UI_GetData(FL_METERING), STATUS_NOT_SUPPORT);
        break;
        case SETTING_PHOTO_FOV:
            Ret = GetUIMappingFromNVT(VideoFOV, ITEM_NUM(VideoFOV), UI_GetData(FL_FOV), STATUS_NOT_SUPPORT);
        break;
        #endif
        case SETTING_PHOTO_DATE_PRINT:
            Ret = GetUIMappingFromNVT(PhotoDateTimePrint, ITEM_NUM(PhotoDateTimePrint), UI_GetData(FL_DATE_STAMP), STATUS_NOT_SUPPORT);
        break;
        case SETTING_COMM_STREAM_SIZE:
            Ret = GetUIMappingFromNVT(DevStreamSize, ITEM_NUM(DevStreamSize), UI_GetData(FL_WIFI_MOVIE_APP_PREVIEW_SIZE), STATUS_NOT_SUPPORT);
        break;
        case SETTING_COMM_STREAM_ENABLE:
            if(System_GetState(SYS_STATE_CURRMODE) == PRIMARY_MODE_MOVIE)
            {
                Ret = UIFlowWndWiFiMovie_GetStatus() == WIFI_MOV_ST_IDLE? 1:0;
            }
            else if(System_GetState(SYS_STATE_CURRMODE) == PRIMARY_MODE_PHOTO)
            {
                Ret = 1;
            }
            else
            {
                Ret = 0;
            }
        break;
        case SETTING_COMM_STREAM_BIT_RATE:
            Ret = GetStreamBitRateFromNVT(GetMovieTargetBitrate_2p(1, 0));
        break;
        case SETTING_COMM_LCD_LOCK:
            if(0 == GxVideo_GetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_BACKLIGHT))
            {
                Ret = 1;
            }
            else
            {
                Ret = 0;
            }
        break;
        #if (AUTOSLEEP_FUNCTION == ENABLE)
        case SETTING_COMM_LCD_SLEEP:
            Ret = GetUIMappingFromNVT(LcdSleep, ITEM_NUM(LcdSleep), UI_GetData(FL_LCD_OFF), STATUS_NOT_SUPPORT);
        break;
        #endif
        case SETTING_COMM_LED_BLINK:
            Ret = m_LedBlinkSetting;
        break;
        case SETTING_COMM_BEEP_VOLUME:
            Ret = GetUIMappingFromNVT(BeepVolume, ITEM_NUM(BeepVolume), UI_GetData(FL_BeepVolumnIndex), STATUS_NOT_SUPPORT);
        break;
        #if (AUTOPWROFF_FUNCTION == ENABLE)
        case SETTING_COMM_AUTO_POWER_DOWN:
            Ret = GetUIMappingFromNVT(AutoPowerDown, ITEM_NUM(AutoPowerDown), UI_GetData(FL_AUTO_POWER_OFF), STATUS_NOT_SUPPORT);
        break;
        #endif
        case SETTING_COMM_CAMERA_MODE:
            curMode = System_GetState(SYS_STATE_CURRMODE);
            if(curMode == PRIMARY_MODE_PHOTO)
                Ret = YK_CAMERA_MODE_PHOTO;
            else if(curMode == PRIMARY_MODE_MOVIE)
                Ret = YK_CAMERA_MODE_VIDEO;
            else if(curMode == PRIMARY_MODE_PLAYBACK)
                Ret = YK_CAMERA_MODE_PLAYBACK;
        break;
        default:
            //DBG_ERR("Not support ID:%d\r\n",item);
        break;
    }
    return Ret;
}




static void UpdateAllStatus(void)
{
    UINT32 i, value;
    //update device status
    for(i=DEVICE_INTERNAL_BATTERY_PRESENT; i<DEVICE_MAX_ITEM; i++)
    {
        value = GetDeviceStatus(i);
        //if(value != STATUS_NOT_SUPPORT)
        {
            ykcamera_device_status[i].ivalue = (int)GetDeviceStatus(i);
        }
    }
    //update setting status
    for(i=SETTING_VIDEO_DEFAULT_MODE; i<SETTING_MAX_ITEM; i++)
    {
        value = GetSettingStatus(i);
        //if(value != STATUS_NOT_SUPPORT)
        {
            ykcamera_setting_status[i].ivalue = (int)GetSettingStatus(i);
        }
    }

    #if 0//(THIS_DBGLVL > 1)
    //check setting status
    for(i=DEVICE_INTERNAL_BATTERY_PRESENT; i<DEVICE_MAX_ITEM; i++)
    {
        if(CAM_TYPE_INTEGER == ykcamera_device_status[i].status_type)
        {
            DBG_DUMP("ykcamera_device_status[%2d]=%d\r\n",i,ykcamera_device_status[i].ivalue);
        }
        else if(CAM_TYPE_FLOAT == ykcamera_device_status[i].status_type)
        {
            DBG_DUMP("ykcamera_device_status[%2d]=%f\r\n",i,ykcamera_device_status[i].fvalue);
        }
        else
        {
            DBG_DUMP("ykcamera_device_status[%2d]=%s\r\n",i,ykcamera_device_status[i].svalue);
        }
    }
    //check setting status
    for(i=SETTING_VIDEO_DEFAULT_MODE; i<SETTING_MAX_ITEM; i++)
    {
        if(CAM_TYPE_INTEGER == ykcamera_setting_status[i].status_type)
        {
            DBG_DUMP("ykcamera_setting_status[%2d]=%d\r\n",i,ykcamera_setting_status[i].ivalue);
        }
        else if(CAM_TYPE_FLOAT == ykcamera_setting_status[i].status_type)
        {
            DBG_DUMP("ykcamera_setting_status[%2d]=%f\r\n",i,ykcamera_setting_status[i].fvalue);
        }
        else
        {
            DBG_DUMP("ykcamera_setting_status[%2d]=%s\r\n",i,ykcamera_setting_status[i].svalue);
        }
    }
    #endif

}
static INT32 CAMERA_STATUS(handler_parameter *param, void *userdata)
{
    UpdateAllStatus();
    return YOUKU_CMD_SUCCESS;
}
static INT32 VIDEO_RESOLUTION_FPS(handler_parameter *param, void *userdata)
{
    INT32 req_value;
    req_value = GetUIMappingFromYK(VideoResoFPS, ITEM_NUM(VideoResoFPS), *(INT32 *)param->scall_back_param, MOVIE_SIZE_ID_MAX);

    //refer to g_MovieRecSizeTable
    if(req_value < MOVIE_SIZE_ID_MAX)
    {
        Ux_PostEvent(NVTEVT_WIFI_EXE_MOVIE_REC_SIZE,1,req_value);
        return YOUKU_CMD_SUCCESS;
    }
    else
    {
        return YOUKU_CMD_PARAM_OVERFLOW;
    }

}
#if 0
static INT32 VIDEO_FOV(handler_parameter *param, void *userdata)
{
    INT32 req_value;
    req_value = GetUIMappingFromYK(VideoFOV, ITEM_NUM(VideoFOV), *(INT32 *)param->scall_back_param, FOV_ID_MAX);
    if(req_value < FOV_ID_MAX)
    {
        Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIE_FOV, 1, req_value);
        return YOUKU_CMD_SUCCESS;
    }
    else
    {
        return YOUKU_CMD_PARAM_OVERFLOW;
    }
}
#endif
static INT32 VIDEO_TIMELAPSE_RATE(handler_parameter *param, void *userdata)
{
    INT32 req_value;
    req_value = GetUIMappingFromYK(VideoTimelapseRate, ITEM_NUM(VideoTimelapseRate), *(INT32 *)param->scall_back_param, MOVIE_TIMELAPSEREC_ID_MAX);
    if(req_value < MOVIE_TIMELAPSEREC_ID_MAX)
    {
        //#NT#2016/09/20#KCHong -begin
        //#NT#Should set FL_XXX_MENU instead of FL_XXX and the flag will sync when CheckReOpen
        UI_SetData(FL_MOVIE_TIMELAPSE_REC_MENU,  req_value);
        //#NT#2016/09/20#KCHong -end
        return YOUKU_CMD_SUCCESS;
    }
    else
    {
        return YOUKU_CMD_PARAM_OVERFLOW;
    }

}
static INT32 VIDEO_LOOPING(handler_parameter *param, void *userdata)
{
    INT32 req_value;
    req_value = GetUIMappingFromYK(VideoLooping, ITEM_NUM(VideoLooping), *(INT32 *)param->scall_back_param, MOVIE_CYCLICREC_ID_MAX);

    //refer to _MOVIE_CYCLICREC
    if(req_value < MOVIE_CYCLICREC_ID_MAX)
    {
        Ux_SendEvent(&CustomMovieObjCtrl,NVTEVT_EXE_CYCLIC_REC, 1, req_value);
        return YOUKU_CMD_SUCCESS;
    }
    else
    {
        return YOUKU_CMD_PARAM_OVERFLOW;
    }
}
#if 0
static INT32 VIDEO_SPOT_METER(handler_parameter *param, void *userdata)
{
    INT32 req_value;
    req_value = GetUIMappingFromYK(VideoSpotMeter, ITEM_NUM(VideoSpotMeter), *(INT32 *)param->scall_back_param, METERING_ID_MAX);

    if(req_value < METERING_ID_MAX)
    {
        Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIEMETERING, 1, req_value);
        return YOUKU_CMD_SUCCESS;
    }
    else
    {
        return YOUKU_CMD_PARAM_OVERFLOW;
    }
}
static INT32 VIDEO_EFFECT(handler_parameter *param, void *userdata)
{
    INT32 req_value;
    req_value = GetUIMappingFromYK(VideoEffect, ITEM_NUM(VideoEffect), *(INT32 *)param->scall_back_param, IMAGE_EFFECT_ID_MAX);

    if(req_value < IMAGE_EFFECT_ID_MAX)
    {
        SEL_IMAGEEFFECT ImageEffect;
        UI_SetData(FL_IMAGE_EFFECT, req_value);
        switch(req_value)
        {
            case IMAGE_EFFECT_SKETCH:
                ImageEffect = SEL_IMAGEEFFECT_SKETCH;
                break;
            case IMAGE_EFFECT_FISHEYE:
                ImageEffect = SEL_IMAGEEFFECT_FISHEYE;
                break;
            case IMAGE_EFFECT_COLORPENCIL:
                ImageEffect = SEL_IMAGEEFFECT_COLORPENCIL;
                break;
            default:
                ImageEffect = SEL_IMAGEEFFECT_OFF;
                break;
        }
        IPL_AlgSetUIInfo(IPL_SEL_IMAGEEFFECT, ImageEffect);
        if(req_value)//color, effect and wdr are exclusive
        {
            UI_SetData(FL_MOVIE_HDR, MOVIE_HDR_OFF);
            UI_SetData(FL_COLOR_EFFECT, COLOR_EFFECT_STANDARD);
        }
        return YOUKU_CMD_SUCCESS;
    }
    else
    {
        return YOUKU_CMD_PARAM_OVERFLOW;
    }
}
#endif
static INT32 VIDEO_WHITE_BALANCE(handler_parameter *param, void *userdata)
{
    INT32 req_value;
    req_value = GetUIMappingFromYK(VideoWB, ITEM_NUM(VideoWB), *(INT32 *)param->scall_back_param, WB_ID_MAX);

    if(req_value < WB_ID_MAX)
    {
        Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_WB, 1, req_value);
        return YOUKU_CMD_SUCCESS;
    }
    else
    {
        return YOUKU_CMD_PARAM_OVERFLOW;
    }
}
#if 0
static INT32 VIDEO_COLOR(handler_parameter *param, void *userdata)
{
    INT32 req_value;
    req_value = GetUIMappingFromYK(VideoColor, ITEM_NUM(VideoColor), *(INT32 *)param->scall_back_param, COLOR_EFFECT_ID_MAX);
    if(req_value < COLOR_EFFECT_ID_MAX)
    {
        Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIECOLOR, 1, req_value);
        if(req_value)//color, effect and wdr are exclusive
        {
            UI_SetData(FL_MOVIE_HDR, MOVIE_HDR_OFF);
            UI_SetData(FL_IMAGE_EFFECT, IMAGE_EFFECT_NONE);
        }
        return YOUKU_CMD_SUCCESS;
    }
    else
    {
        return YOUKU_CMD_PARAM_OVERFLOW;
    }
}
#endif
static INT32 VIDEO_ISO(handler_parameter *param, void *userdata)
{
    INT32 req_value;
    req_value = GetUIMappingFromYK(VideoISO, ITEM_NUM(VideoISO), *(INT32 *)param->scall_back_param, ISO_ID_MAX);
    if(req_value < ISO_ID_MAX)
    {
        Ux_SendEvent(&CustomPhotoObjCtrl,   NVTEVT_EXE_ISO,    1, req_value);
        return YOUKU_CMD_SUCCESS;
    }
    else
    {
        return YOUKU_CMD_PARAM_OVERFLOW;
    }
}
static INT32 VIDEO_SHARPNESS(handler_parameter *param, void *userdata)
{

    INT32 req_value;
    req_value = GetUIMappingFromYK(VideoSharpness, ITEM_NUM(VideoSharpness), *(INT32 *)param->scall_back_param, SHARPNESS_ID_MAX);
    if(req_value < SHARPNESS_ID_MAX)
    {
        UI_SetData(FL_SHARPNESS,  req_value);
        Ux_SendEvent(&CustomPhotoObjCtrl,   NVTEVT_EXE_SHARPNESS,    1, req_value);
        return YOUKU_CMD_SUCCESS;
    }
    else
    {
        return YOUKU_CMD_PARAM_OVERFLOW;
    }
}
static INT32 VIDEO_EV(handler_parameter *param, void *userdata)
{
    INT32 req_value;
    req_value = GetUIMappingFromYK(VideoEV, ITEM_NUM(VideoEV), *(INT32 *)param->scall_back_param, EV_SETTING_MAX);
    if(req_value < EV_SETTING_MAX)
    {
        Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIE_EV, 1, req_value);
        return YOUKU_CMD_SUCCESS;
    }
    else
    {
        return YOUKU_CMD_PARAM_OVERFLOW;
    }
}
static INT32 VIDEO_DATETIME_PRINT(handler_parameter *param, void *userdata)
{
    int  req_value = *(int *)param->scall_back_param;
    if(req_value < MOVIE_DATEIMPRINT_ID_MAX)
    {
        Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIE_DATE_IMPRINT, 1, req_value);
        return YOUKU_CMD_SUCCESS;
    }
    else
    {
        return YOUKU_CMD_PARAM_OVERFLOW;
    }
}
static INT32 VIDEO_AUDIO_RECORD(handler_parameter *param, void *userdata)
{
    int  req_value = *(int *)param->scall_back_param;
    if(req_value < MOVIE_AUDIO_ID_MAX)
    {
        Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIE_AUDIO, 1, req_value);
        return YOUKU_CMD_SUCCESS;
    }
    else
    {
        return YOUKU_CMD_PARAM_OVERFLOW;
    }
}
static INT32 VIDEO_WDR(handler_parameter *param, void *userdata)
{
    int  req_value = *(int *)param->scall_back_param;
    if(req_value < MOVIE_WDR_ID_MAX)
    {
        Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIE_WDR, 1, req_value);
        return YOUKU_CMD_SUCCESS;
    }
    else
    {
        return YOUKU_CMD_PARAM_OVERFLOW;
    }
}
static INT32 PHOTO_DEFAULT_MODE(handler_parameter *param, void *userdata)
{
    INT32 req_value;
    req_value = GetUIMappingFromYK(PhotoDefaultMode, ITEM_NUM(PhotoDefaultMode), *(INT32 *)param->scall_back_param, CONTINUE_SHOT_SETTING_MAX);
    //refer to CONTINUE_SHOT_OFF
    if(req_value < CONTINUE_SHOT_SETTING_MAX)
    {
        UI_SetData(FL_CONTINUE_SHOT,req_value);
        return YOUKU_CMD_SUCCESS;
    }
    else
    {
        return YOUKU_CMD_PARAM_OVERFLOW;
    }
}
static INT32 PHOTO_RESOLUTION(handler_parameter *param, void *userdata)
{
    INT32 req_value;
    req_value = GetUIMappingFromYK(PhotoResolution, ITEM_NUM(PhotoResolution), *(INT32 *)param->scall_back_param, PHOTO_SIZE_ID_MAX);
    //g_pcSizeCfgTable  UI_SetData(FL_PHOTO_SIZE,uhSelect);
    if(req_value < PHOTO_SIZE_ID_MAX)
    {
        Ux_PostEvent(NVTEVT_WIFI_EXE_PHOTO_SIZE,1,req_value);
        return YOUKU_CMD_SUCCESS;
    }
    else
    {
        return YOUKU_CMD_PARAM_OVERFLOW;
    }
}
#if 0
static INT32 PHOTO_SPOT_METER(handler_parameter *param, void *userdata)
{
    INT32 req_value;
    req_value = GetUIMappingFromYK(VideoSpotMeter, ITEM_NUM(VideoSpotMeter), *(INT32 *)param->scall_back_param, METERING_ID_MAX);
    if(req_value < METERING_ID_MAX)
    {
        Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_MOVIEMETERING, 1, req_value);
        return YOUKU_CMD_SUCCESS;
    }
    else
    {
        return YOUKU_CMD_PARAM_OVERFLOW;
    }
}
static INT32 PHOTO_HDR(handler_parameter *param, void *userdata)
{
    int  req_value = *(int *)param->scall_back_param;
    if(req_value < 2)
    {
        if(req_value)
        {
            UI_SetData(FL_ModeIndex, DSC_MODE_PHOTO_SCENE);
            UI_SetData(FL_SCENE, IMAGEEFFECT_HDR);
            //this setting will disable image effect
            Ux_SendEvent(&CustomPhotoObjCtrl,NVTEVT_EXE_IMAGE_EFFECT   ,1, UI_GetData(FL_SCENE));
            //reset image effect related setting
            UI_SetData(FL_MOVIE_HDR, MOVIE_HDR_OFF);
        }
        else
        {
            UI_SetData(FL_ModeIndex, DSC_MODE_PHOTO_AUTO);
        }
        return YOUKU_CMD_SUCCESS;
    }
    else
    {
        return YOUKU_CMD_PARAM_OVERFLOW;
    }
}
static INT32 PHOTO_COUNTDOWN_INTERVEL(handler_parameter *param, void *userdata)
{
    int  req_value = *(int *)param->scall_back_param;
    if(req_value < SELFTIMER_ID_MAX)
    {
        Ux_SendEvent(&CustomPhotoObjCtrl,NVTEVT_EXE_SELFTIMER   ,1, req_value);
        return YOUKU_CMD_SUCCESS;
    }
    else
    {
        return YOUKU_CMD_PARAM_OVERFLOW;
    }
}
static INT32 PHOTO_FOV(handler_parameter *param, void *userdata)
{
    INT32 req_value;
    req_value = GetUIMappingFromYK(VideoFOV, ITEM_NUM(VideoFOV), *(INT32 *)param->scall_back_param, FOV_ID_MAX);
    if(req_value < FOV_ID_MAX)
    {
        Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_FOV, 1, req_value);
        return YOUKU_CMD_SUCCESS;
    }
    else
    {
        return YOUKU_CMD_PARAM_OVERFLOW;
    }
}
#endif
static INT32 PHOTO_DATETIME_PRINT(handler_parameter *param, void *userdata)
{
    INT32 req_value;
    req_value = GetUIMappingFromYK(PhotoDateTimePrint, ITEM_NUM(PhotoDateTimePrint), *(INT32 *)param->scall_back_param, DATE_STAMP_ID_MAX);
    if(req_value < DATE_STAMP_ID_MAX)
    {
        Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_DATE_PRINT, 1, req_value);
        return YOUKU_CMD_SUCCESS;
    }
    else
    {
        return YOUKU_CMD_PARAM_OVERFLOW;
    }
}
static INT32 DEV_STREAM_SIZE(handler_parameter *param, void *userdata)
{

    INT32 req_value;
    if(YoukuSdk_ChkMode(PRIMARY_MODE_MOVIE))
    {
        if(YoukuSdk_IsRecording())
            return YOUKU_CMD_INVALID_DURING_RECORD;

        req_value = GetUIMappingFromYK(DevStreamSize, ITEM_NUM(DevStreamSize), *(INT32 *)param->scall_back_param, WIFI_MOVIE_APP_PREVIEW_SIZE_ID_MAX);
        if(req_value < WIFI_MOVIE_APP_PREVIEW_SIZE_ID_MAX)
        {
            UI_SetData(FL_WIFI_MOVIE_APP_PREVIEW_SIZE, req_value);
            //turn off stream
            WifiCmd_ClrFlg(WIFIFLAG_PREVIEW_DONE);
            Ux_PostEvent(NVTEVT_WIFI_EXE_MOVIE_LIVEVIEW_START,1,0);
            WifiCmd_WaitFinish(WIFIFLAG_PREVIEW_DONE);
            //turn on stream
            WifiCmd_ClrFlg(WIFIFLAG_PREVIEW_DONE);
            Ux_PostEvent(NVTEVT_WIFI_EXE_MOVIE_LIVEVIEW_START,1,1);
            WifiCmd_WaitFinish(WIFIFLAG_PREVIEW_DONE);
            return YOUKU_CMD_SUCCESS;
        }
        else
        {
            return YOUKU_CMD_PARAM_OVERFLOW;
        }
    }
    else
    {
        return YOUKU_CMD_MODE_ERROR;
    }
}
static INT32 STREAM_BIT_RATE(handler_parameter *param, void *userdata)
{
    UINT32  req_value;
    if(!YoukuSdk_ChkMode(PRIMARY_MODE_MOVIE))
    {
        return YOUKU_CMD_MODE_ERROR;
    }

    //NVTEVT_WIFI_EXE_MOVIE_LIVEVIEW_BITRATE
    if (YoukuSdk_IsRecording() == FALSE)
    {
        //req_value's unit is bps
        req_value = GetStreamBitRateFromYK(*(INT32 *)param->scall_back_param);
        DBG_IND("Set Bit Rate %d KB.\r\n",req_value/1024);
        if(req_value)
        {
            SetMovieTargetBitrate(0, UI_GetData(FL_WIFI_MOVIE_APP_PREVIEW_SIZE), req_value/8);
            return YOUKU_CMD_SUCCESS;
        }
        else
        {
            return YOUKU_CMD_PARAM_OVERFLOW;
        }
    }
    else
    {
        DBG_ERR("Can't set bitrate during recording!\r\n");
        return YOUKU_CMD_INVALID_DURING_RECORD;
    }
}
static INT32 COMM_LCD_LOCK(handler_parameter *param, void *userdata)
{
    int  req_value = *(int *)param->scall_back_param;
    if(req_value < 2)
    {
        GxVideo_SetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_BACKLIGHT, !req_value);
        return YOUKU_CMD_SUCCESS;
    }
    else
    {
        return YOUKU_CMD_PARAM_OVERFLOW;
    }
}
#if (AUTOSLEEP_FUNCTION == ENABLE)
static INT32 COMM_LCD_SLEEP(handler_parameter *param, void *userdata)
{
    INT32 req_value;
    req_value = GetUIMappingFromYK(LcdSleep, ITEM_NUM(LcdSleep), *(INT32 *)param->scall_back_param, LCDOFF_SETTING_MAX);
    if(req_value < LCDOFF_SETTING_MAX)
    {
        Ux_SendEvent(&UISetupObjCtrl,NVTEVT_EXE_LCDOFF,1,req_value);
        return YOUKU_CMD_SUCCESS;
    }
    else
    {
        return YOUKU_CMD_PARAM_OVERFLOW;
    }
}
#endif
#if 0
static INT32 COMM_LED_BLINK(handler_parameter *param, void *userdata)
{
    int  req_value = *(int *)param->scall_back_param;
    if(req_value < 2)
    {
        m_LedBlinkSetting = req_value;
        if(req_value)
        {
            GxLED_SetCtrl(KEYSCAN_LED_RED,SETLED_SPEED,GXLED_1SEC_LED_TOGGLE_CNT/(req_value*2));
            GxLED_SetCtrl(KEYSCAN_LED_RED,TURNON_LED,FALSE);
            GxLED_SetCtrl(KEYSCAN_LED_RED,SET_TOGGLE_LED,TRUE);
        }
        else
        {
            GxLED_SetCtrl(KEYSCAN_LED_RED,TURNON_LED,FALSE);
            GxLED_SetCtrl(KEYSCAN_LED_RED,SET_TOGGLE_LED,FALSE);
        }
        return YOUKU_CMD_SUCCESS;
    }
    else
    {
        return YOUKU_CMD_PARAM_OVERFLOW;
    }
    return YOUKU_CMD_SUCCESS;
}
#endif
static INT32 COMM_BEEP_VOLUME(handler_parameter *param, void *userdata)
{
    INT32 req_value;
    req_value = GetUIMappingFromYK(BeepVolume, ITEM_NUM(BeepVolume), *(INT32 *)param->scall_back_param, BEEP_VOL_SETTING_MAX);
    if(req_value < BEEP_VOL_SETTING_MAX)
    {
        Ux_SendEvent(&UISetupObjCtrl,NVTEVT_EXE_BEEPVOLUME, 1, req_value);
        return YOUKU_CMD_SUCCESS;
    }
    else
    {
        return YOUKU_CMD_PARAM_OVERFLOW;
    }
    return YOUKU_CMD_SUCCESS;
}
#if (AUTOPWROFF_FUNCTION == ENABLE)
static INT32 COMM_AUTO_POWER_DOWN(handler_parameter *param, void *userdata)
{
    INT32 req_value;
    req_value = GetUIMappingFromYK(AutoPowerDown, ITEM_NUM(AutoPowerDown), *(INT32 *)param->scall_back_param, POWEROFF_SETTING_MAX);
    if(req_value < POWEROFF_SETTING_MAX)
    {
        SxTimer_SetFuncActive(SX_TIMER_DET_AUTOPOWEROFF_ID,TRUE);
        Ux_SendEvent(&UISetupObjCtrl,NVTEVT_EXE_POWEROFF, 1, req_value);
        return YOUKU_CMD_SUCCESS;
    }
    else
    {
        return YOUKU_CMD_PARAM_OVERFLOW;
    }
    return YOUKU_CMD_SUCCESS;
}
#endif
static INT32 COMM_RESET_FACTORY(handler_parameter *param, void *userdata)
{
    Ux_SendEvent(0,NVTEVT_EXE_SYSRESET,0);
    return YOUKU_CMD_SUCCESS;
}
static INT32 DEV_FORMAT_SD(handler_parameter *param, void *userdata)
{
    INT32 ret = YOUKU_CMD_SUCCESS;
    INT32 WaitResult;
    //1 for SD, 0 of NAND
    //to do NVTEVT_WIFI_EXE_FORMAT
    WifiCmd_ClrFlg(WIFIFLAG_FORMAT_DONE);
    Ux_PostEvent(NVTEVT_WIFI_EXE_FORMAT,1,1);
    WaitResult = (INT32)WifiCmd_WaitFinish(WIFIFLAG_FORMAT_DONE);
    switch(WaitResult)
    {
        case E_NOEXS://card removed
            ret = YOUKU_CMD_CARD_REMOVED;
        break;
        case E_OBJ://card locked
            ret = YOUKU_CMD_CARD_LOCKED;
        break;
        case E_PAR:
            ret = YOUKU_CMD_PARAM_OVERFLOW;
        break;
        default:
        break;
    }

    return ret;
}
static INT32 COMM_RESET_SSID(handler_parameter *param, void *userdata)
{
    UINet_SetSSID(DEFAULT_SSID,strlen(DEFAULT_SSID));
    UINet_SetPASSPHRASE(DEFAULT_PASSPHRASE,strlen(DEFAULT_PASSPHRASE));
    return YOUKU_CMD_SUCCESS;
}
static INT32 COMM_SHUTTER(handler_parameter *param, void *userdata)
{
    INT32 ret = YOUKU_CMD_SUCCESS;
    INT32 WaitResult;
    BOOL bRecordStart;
    DBG_IND("Curr Mode =%d\r\n",System_GetState(SYS_STATE_CURRMODE));
    if(System_GetState(SYS_STATE_CURRMODE) == PRIMARY_MODE_MOVIE)
    {
        DBG_IND("Curr Movie Status =%d\r\n",UIFlowWndWiFiMovie_GetStatus());
        WifiCmd_ClrFlg(WIFIFLAG_RECORD_DONE);
        if (YoukuSdk_IsRecording() == FALSE)
        {
            if(FlowMovie_CheckReOpenItem()==TRUE)
            {
                Ux_SendEvent(0,NVTEVT_SYSTEM_MODE, 1, System_GetState(SYS_STATE_CURRMODE));
            }


            bRecordStart = TRUE;
            //start to record
            Ux_PostEvent(NVTEVT_WIFI_EXE_MOVIE_REC,1,1);
        }
        else
        {
            bRecordStart = FALSE;
            //stop recording
            Ux_PostEvent(NVTEVT_WIFI_EXE_MOVIE_REC,1,0);
        }
        WaitResult = (INT32)WifiCmd_WaitFinish(WIFIFLAG_RECORD_DONE);
        switch(WaitResult)
        {
            case WIFIAPP_RET_STORAGE_FULL:
                ret = YOUKU_CMD_STORAGE_FULL;
            break;
            case WIFIAPP_RET_FAIL:
                if(bRecordStart)//idle state,cannot record = stream is disabled
                    ret = YOUKU_CMD_STREAM_DISABLED;
                else//recTime<1 sec
                    ret = YOUKU_CMD_VIDEO_TOO_SHORT;
            break;
            default:
            break;
        }
    }
    else if(System_GetState(SYS_STATE_CURRMODE) == PRIMARY_MODE_PHOTO)
    {
        WifiCmd_ClrFlg(WIFIFLAG_CAPTURE_DONE);
        Ux_PostEvent(NVTEVT_WIFI_EXE_PHOTO_CAPTURE,0);
        WifiCmd_WaitFinish(WIFIFLAG_CAPTURE_DONE);
    }
    else
    {
        return YOUKU_CMD_MODE_ERROR;
    }
    return ret;
}
static INT32 COMM_CHANGE_CAMERA_MODE(handler_parameter *param, void *userdata)
{
    int  req_value = *(int *)param->scall_back_param;
    INT32 ret = YOUKU_CMD_SUCCESS;
    if(req_value <= WIFI_APP_MODE_PLAYBACK)
    {
        WifiCmd_ClrFlg(WIFIFLAG_MODE_DONE);
        Ux_PostEvent(NVTEVT_WIFI_EXE_MODE,1,req_value);
        if(WifiCmd_WaitFinish(WIFIFLAG_MODE_DONE))
        {
            ret = YOUKU_CMD_PARAM_OVERFLOW;
        }
    }
    else
    {
        ret = YOUKU_CMD_PARAM_OVERFLOW;
    }
    return ret;
}
static INT32 COMM_POWEROFF(handler_parameter *param, void *userdata)
{
    Ux_PostEvent(NVTEVT_SYSTEM_SHUTDOWN, 1, 0); //shutdown start
    return YOUKU_CMD_SUCCESS;
}
static INT32 COMM_UPDATE_DOWNLOAD_DONE(handler_parameter *param, void *userdata)
{
    UINT32 Ret;
    INT32 nRetry = 20;
    Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_PLAYBACK);
    //polling to wait finish
    while(System_GetState(SYS_STATE_CURRMODE)!=PRIMARY_MODE_PLAYBACK && nRetry-->0)
    {
        Delay_DelayMs(500);
    }
    Ret = System_OnStrg_UploadFW(0);
    if(Ret)
    {
        INT error;
        DBG_ERR("Ret=%d\r\n",Ret);
        switch(Ret)
        {
            case UPDNAND_STS_FW_INVALID_STG:
            case UPDNAND_STS_FW_READ_ERR:
                error = YOUKU_CMD_FILE_NOT_EXIST;
            break;
            default:
                error = YOUKU_CMD_ERROR_UNKNOWN;
            break;
        }
        return error;
    }
    //delete bin file
    if(FileSys_DeleteFile(FW_UPDATE_NAME)!=FST_STA_OK)
    {
        DBG_IND("Ignore deleting file.\r\n");
    }
    //set power alarm after 2 sec
    GxSystem_EnableHWResetByAlarmTime(0, 0, 0, 2);
    Ux_PostEvent(NVTEVT_SYSTEM_SHUTDOWN, 1, 0);

    return YOUKU_CMD_SUCCESS;
}
static INT32 STORAGE_DELETE_ALL(handler_parameter *param, void *userdata)
{
    if(YoukuSdk_ChkMode(PRIMARY_MODE_PLAYBACK))
    {
        INT32 ChkCard = YoukuSdk_ChkCardStatus();
        if(ChkCard)
            return ChkCard;

        PFILEDB_FILE_ATTR FileAttr = NULL;
        INT32             FileNum,i;
        PFILEDB_INIT_OBJ   pFDBInitObj = &gFDBInitObj;
        FILEDB_HANDLE      FileDBHandle =0;

        Perf_Mark();

        pFDBInitObj->u32MemAddr = SxCmd_GetTempMem(POOL_SIZE_FILEDB);
        if(!pFDBInitObj->u32MemAddr)
        {
            return YOUKU_CMD_MEM_INSUFFICIENT;
        }
        pFDBInitObj->u32MemSize = POOL_SIZE_FILEDB;
        FileDBHandle = FileDB_Create(pFDBInitObj);
        DBG_IND("createTime = %04d ms \r\n",Perf_GetDuration()/1000);
        Perf_Mark();
        FileDB_SortBy(FileDBHandle,FILEDB_SORT_BY_MODDATE,FALSE);
        DBG_IND("sortTime = %04d ms \r\n",Perf_GetDuration()/1000);
        DBG_MSG("################# Delete All Begin ################\r\n");
        FileNum = FileDB_GetTotalFileNum(FileDBHandle);
        for (i=FileNum-1;i>=0;i--)
        {
            FileAttr = FileDB_SearhFile(FileDBHandle,i);
            if (FileAttr)
            {
                if (M_IsReadOnly(FileAttr->attrib))
                {
                    //DBG_IND("File Locked\r\n");
                    DBG_IND("File %s is locked\r\n", FileAttr->filePath);
                    continue;
                }
                FileSys_DeleteFile(FileAttr->filePath);
                DBG_MSG("%s\r\n",FileAttr->filePath);
            }
            else
            {
                DBG_IND("%s not existed\r\n", FileAttr->filePath);
            }
        }
        DBG_MSG("################# Delete All End ################\r\n");
        FileDB_Release(FileDBHandle);
        return YOUKU_CMD_SUCCESS;
    }
    else
    {
        return YOUKU_CMD_MODE_ERROR;
    }
}
static INT32 STORAGE_DELETE(handler_parameter *param, void *userdata)
{
    char *path = (char *)param->scall_back_param;
    if(YoukuSdk_ChkMode(PRIMARY_MODE_PLAYBACK))
    {
        char tempPath[128] = {0};
        INT32 Index;
        INT32 ret;
        PFILEDB_FILE_ATTR FileAttr = NULL;
        INT32 ChkCard = YoukuSdk_ChkCardStatus();
        if(ChkCard)
            return ChkCard;
        YoukuPath2NvtPath(path, tempPath);
        DBG_IND("Youku file path=%s\r\n",path);
        DBG_IND("NVT file path=%s\r\n",tempPath);
        Index = FileDB_GetIndexByPath(0,tempPath);
        FileAttr = FileDB_SearhFile(0,Index);
        if(FileAttr)
        {
            if(M_IsReadOnly(FileAttr->attrib))
                return YOUKU_CMD_FILE_LOCKED;
            ret = FileSys_DeleteFile(FileAttr->filePath);
            if (ret != FST_STA_OK)
            {
                DBG_ERR("Failed to delete %s.\r\n",tempPath);
                return YOUKU_CMD_FILE_ERROR;
            }
            else
            {
                DeleteMovieWiFi(FileAttr->filePath);
                FileDB_DeleteFile(0,Index);
                return YOUKU_CMD_SUCCESS;
            }
        }
        else
        {
            return YOUKU_CMD_FILE_NOT_EXIST;
        }

    }
    else
    {
        return YOUKU_CMD_MODE_ERROR;
    }
}
static INT32 STORAGE_DELETE_LAST(handler_parameter *param, void *userdata)
{
    if(YoukuSdk_ChkMode(PRIMARY_MODE_PLAYBACK))
    {
        INT32 ret;
        INT32 FileNum;
        PFILEDB_FILE_ATTR FileAttr = NULL;
        INT32 ChkCard = YoukuSdk_ChkCardStatus();
        if(ChkCard)
            return ChkCard;
        FileNum = FileDB_GetTotalFileNum(0);
        FileAttr = FileDB_SearhFile(0,FileNum-1);
        if(FileAttr)
        {
            if(M_IsReadOnly(FileAttr->attrib))
                return YOUKU_CMD_FILE_LOCKED;
            ret = FileSys_DeleteFile(FileAttr->filePath);
            if (ret != FST_STA_OK)
            {
                DBG_ERR("Failed to delete %s.\r\n",FileAttr->filePath);
                return YOUKU_CMD_FILE_ERROR;
            }
            else
            {
                DeleteMovieWiFi(FileAttr->filePath);
                FileDB_DeleteFile(0,FileNum-1);
                return YOUKU_CMD_SUCCESS;
            }
        }
        else
        {
            return YOUKU_CMD_FILE_NOT_EXIST;
        }

    }
    else
    {
        return YOUKU_CMD_MODE_ERROR;
    }
}
static INT32 SERVER_STREAM_ENABLE(handler_parameter *param, void *userdata)
{
    int  req_value = *(int *)param->scall_back_param;
    if(YoukuSdk_ChkMode(PRIMARY_MODE_MOVIE))
    {
        WifiCmd_ClrFlg(WIFIFLAG_PREVIEW_DONE);
        Ux_PostEvent(NVTEVT_WIFI_EXE_MOVIE_LIVEVIEW_START,1,req_value);
        if(WifiCmd_WaitFinish(WIFIFLAG_PREVIEW_DONE))
        {
            return YOUKU_CMD_INVALID_DURING_RECORD;
        }
        else
        {
            return YOUKU_CMD_SUCCESS;
        }
    }
    else
    {
        return YOUKU_CMD_MODE_ERROR;
    }
}

static INT32 MEDIA_THUMBNAIL(handler_parameter *param, void *userdata)
{
    INT32 ret = YOUKU_CMD_SUCCESS;
    char *path = (char *)param->scall_back_param;
    thumbnail_info *pThumbnail = (thumbnail_info *)param->scall_back_return;;
    FST_FILE          filehdl;
    char              tempPath[128] = {0};
    UINT32            TempLen,TempBuf;
    UINT32            ThumbOffset =0;
    UINT32            ThumbSize =0;
    char*             pch;
    UINT32            result =0;

    pThumbnail->size = 0;
    if(System_GetState(SYS_STATE_CURRMODE) != PRIMARY_MODE_PLAYBACK || 0 == SxCmd_GetTempMem(POOL_SIZE_FILEDB))
    {
        DBG_IND("Only valid in playback mode!\r\n");
        WifiCmd_ClrFlg(WIFIFLAG_MODE_DONE);
        Ux_PostEvent(NVTEVT_WIFI_EXE_MODE,1,2);
        if(WifiCmd_WaitFinish(WIFIFLAG_MODE_DONE))
        {
            DBG_ERR("change to play mode error!\r\n");
            return YOUKU_CMD_PARAM_OVERFLOW;
        }
    }
    YoukuPath2NvtPath(path, tempPath);
    DBG_IND("Youku file path=%s\r\n",path);
    DBG_IND("NVT file path=%s\r\n",tempPath);
    pch=strchr(tempPath,'.');
    if((!path)||(!pch))
    {
        DBG_ERR("Unknown path\r\n");
        return YOUKU_CMD_FILE_NOT_EXIST;
    }
    if((strncmp(pch+1,"mov",3)==0)||(strncmp(pch+1,"MOV",3)==0)||(strncmp(pch+1,"mp4",3)==0)||(strncmp(pch+1,"MP4",3)==0))
    {
        FST_FILE_STATUS FileStat={0};
        GXVIDEO_INFO VideoInfo={0};
        MEM_RANGE WorkBuf={0};
        UINT32 uiBufferNeeded=POOL_SIZE_TEMP;


        gMovFilehdl = FileSys_OpenFile(tempPath,FST_OPEN_READ);
        if(gMovFilehdl)
        {
            FileSys_StatFile(gMovFilehdl,&FileStat);
            //GxVidFile_Query1stFrameWkBufSize(&uiBufferNeeded, FileStat.uiFileSize);
            TempBuf = SxCmd_GetTempMem(uiBufferNeeded);
            //parse video info for single mode only
            WorkBuf.Addr = TempBuf;
            WorkBuf.Size = uiBufferNeeded;
            result = GxVidFile_ParseVideoInfo(VdoReadCB, &WorkBuf, (UINT32)FileStat.uiFileSize, &VideoInfo);
            if((result==GXVIDEO_PRSERR_OK)&&(VideoInfo.uiThumbSize))
            {
                TempBuf = SxCmd_GetTempMem(VideoInfo.uiThumbSize);
                FileSys_SeekFile(gMovFilehdl, VideoInfo.uiThumbOffset, FST_SEEK_SET);
                FileSys_ReadFile(gMovFilehdl, (UINT8 *)TempBuf, &VideoInfo.uiThumbSize, 0,0);
                FileSys_CloseFile(gMovFilehdl);
                pThumbnail->addr = TempBuf;
                pThumbnail->size = VideoInfo.uiThumbSize;
            }
            else
            {
                FileSys_CloseFile(gMovFilehdl);
                ret = YOUKU_CMD_NO_THUMBNAIL;
                DBG_ERR("parsing error\r\n");
            }
        }
        else
        {
            ret = YOUKU_CMD_PARAM_OVERFLOW;
            DBG_ERR("no %s\r\n",tempPath);
        }

    }
    else
    {

        filehdl = FileSys_OpenFile(tempPath,FST_OPEN_READ);

        if(filehdl)
        {
            MEM_RANGE ExifData;
            EXIF_GETTAG exifTag;
            BOOL bIsLittleEndian=0;
            UINT32 uiTiffOffsetBase=0;

            TempLen= MAX_APP1_SIZE;
            TempBuf = SxCmd_GetTempMem(TempLen);
            FileSys_ReadFile(filehdl, (UINT8 *)TempBuf, &TempLen, 0,0);
            FileSys_CloseFile(filehdl);

            ExifData.Size = MAX_APP1_SIZE;
            ExifData.Addr = TempBuf;
            if(EXIF_ER_OK == EXIF_ParseExif(EXIF_HDL_ID_1, &ExifData))
            {
                EXIF_GetInfo(EXIF_HDL_ID_1, EXIFINFO_BYTEORDER, &bIsLittleEndian, sizeof(bIsLittleEndian));
                EXIF_GetInfo(EXIF_HDL_ID_1, EXIFINFO_TIFF_OFFSET_BASE, &uiTiffOffsetBase, sizeof(uiTiffOffsetBase));
                //find thumbnail
                exifTag.uiTagIfd = EXIF_IFD_1ST;
                exifTag.uiTagId = TAG_ID_INTERCHANGE_FORMAT;
                if(E_OK == EXIF_GetTag(EXIF_HDL_ID_1, &exifTag))
                {
                    ThumbOffset = Get32BitsData(exifTag.uiTagDataAddr, bIsLittleEndian) + uiTiffOffsetBase;
                    exifTag.uiTagId = TAG_ID_INTERCHANGE_FORMAT_LENGTH;
                    if(E_OK == EXIF_GetTag(EXIF_HDL_ID_1, &exifTag))
                        ThumbSize = Get32BitsData(exifTag.uiTagDataAddr, bIsLittleEndian);
                }
            }

            if(ThumbSize)
            {
                hwmem_open();
                hwmem_memcpy((UINT32)TempBuf + MAX_APP1_SIZE, TempBuf+ThumbOffset, ThumbSize);
                hwmem_close();
                pThumbnail->addr = TempBuf + MAX_APP1_SIZE;
                pThumbnail->size = ThumbSize;
            }
            else
            {
                ret = YOUKU_CMD_NO_THUMBNAIL;
                DBG_ERR("no thumbnail\r\n");
            }
        }
        else
        {
            ret = YOUKU_CMD_FILE_NOT_EXIST;
            DBG_ERR("no %s\r\n",tempPath);
        }
    }
    #if 0
    if(pThumbnail->size)
    {
        DumpMem(pThumbnail->addr, 512, 512);
    }
    #endif
    return ret;
}

static INT32 MEDIA_DEVINFO(handler_parameter *param, void *userdata)
{
    INT32 ret = YOUKU_CMD_SUCCESS;
    char *pMacAddr;
    devinfo *pDevInfo = (devinfo *)param->scall_back_return;
    static char RtspPath[32]={0};
    static char HttpPath[32]={0};
    static char FirmwareVer[32]={0};
    static char SerialNum[32]={0};
    static char ApMac[20]={0};
    static char ApSsid[32]={0};

    snprintf((char *)ipc_getNonCacheAddr((UINT32)RtspPath),sizeof(RtspPath) ,"rtsp://%s/xxx.mov",UINet_GetIP());
    snprintf((char *)ipc_getNonCacheAddr((UINT32)HttpPath),sizeof(HttpPath),"http://%s:8192",UINet_GetIP());
    snprintf((char *)ipc_getNonCacheAddr((UINT32)FirmwareVer),sizeof(FirmwareVer) ,FW_VERSION_NUM);
    snprintf((char *)ipc_getNonCacheAddr((UINT32)SerialNum),sizeof(SerialNum) ,"456789");
    #if (MAC_APPEN_SSID==ENABLE)
    pMacAddr = (char*)UINet_GetMAC();
    snprintf((char *)ipc_getNonCacheAddr((UINT32)ApSsid),sizeof(ApSsid),"SSID:%s%02x%02x%02x%02x%02x%02x", UINet_GetSSID() ,pMacAddr[0], pMacAddr[1], pMacAddr[2]
                                                            ,pMacAddr[3], pMacAddr[4], pMacAddr[5]);
    #else
    snprintf((char *)ipc_getNonCacheAddr((UINT32)ApSsid),sizeof(ApSsid),"SSID:%s",UINet_GetSSID());
    #endif
    snprintf((char *)ipc_getNonCacheAddr((UINT32)ApMac),sizeof(ApMac),"%02X:%02X:%02X:%02X:%02X:%02X",pMacAddr[0], pMacAddr[1], pMacAddr[2]
                                                            ,pMacAddr[3], pMacAddr[4], pMacAddr[5]);

    pDevInfo->video_ms_url = (char *)ipc_getNonCacheAddr((UINT32)RtspPath);
    pDevInfo->photo_ms_url = (char *)ipc_getNonCacheAddr((UINT32)HttpPath);

    pDevInfo->firmware_version = (char *)ipc_getNonCacheAddr((UINT32)FirmwareVer);
    pDevInfo->serial_number = (char *)ipc_getNonCacheAddr((UINT32)SerialNum);
    pDevInfo->ap_mac = (char *)ipc_getNonCacheAddr((UINT32)ApMac);
    pDevInfo->ap_ssid = (char *)ipc_getNonCacheAddr((UINT32)ApSsid);

    return ret;

}

static INT32 DATETIME_SYNC(handler_parameter *param, void *userdata)
{
    //the string has been converted in YoukuIpcECOS
    UINT32 uiYear, uiMonth, uiDay, uiHour, uiMin, uiSec;
    char *Data = (char *)param->scall_back_param;
    if (6 == sscanf_s(Data, "%d/%d/%d %d:%d:%d", &uiYear, &uiMonth, &uiDay, &uiHour, &uiMin, &uiSec))
    {
        rtc_setDate(uiYear, uiMonth, uiDay);
        rtc_setTime(uiHour, uiMin, uiSec);
        rtc_triggerCSET();
        return YOUKU_CMD_SUCCESS;
    }
    else
    {
        return YOUKU_CMD_PARAM_OVERFLOW;
    }
}
static INT32 CLEAN_NEW_VIDEO_FLAG(handler_parameter *param, void *userdata)
{
    m_bNewUploadVideo = FALSE;
    return YOUKU_CMD_SUCCESS;
}
YOUKU_CMD_BEGIN(youku)
YOUKU_CMD_ITEM(CAM_REQ_CAMERA_STATUS, CAMERA_STATUS)
YOUKU_CMD_ITEM(CAM_REQ_VIDEO_RESOLUTION_FPS, VIDEO_RESOLUTION_FPS)
//YOUKU_CMD_ITEM(CAM_REQ_VIDEO_FOV, VIDEO_FOV)
YOUKU_CMD_ITEM(CAM_REQ_VIDEO_TIMELAPSE_RATE, VIDEO_TIMELAPSE_RATE)
YOUKU_CMD_ITEM(CAM_REQ_VIDEO_LOOPING, VIDEO_LOOPING)
//YOUKU_CMD_ITEM(CAM_REQ_VIDEO_SPOT_METER, VIDEO_SPOT_METER)
//YOUKU_CMD_ITEM(CAM_REQ_VIDEO_EFFECT, VIDEO_EFFECT)
YOUKU_CMD_ITEM(CAM_REQ_VIDEO_WHITE_BALANCE, VIDEO_WHITE_BALANCE)
//YOUKU_CMD_ITEM(CAM_REQ_VIDEO_COLOR, VIDEO_COLOR)
YOUKU_CMD_ITEM(CAM_REQ_VIDEO_ISO, VIDEO_ISO)
YOUKU_CMD_ITEM(CAM_REQ_VIDEO_SHARPNESS, VIDEO_SHARPNESS)
YOUKU_CMD_ITEM(CAM_REQ_VIDEO_EV, VIDEO_EV)
YOUKU_CMD_ITEM(CAM_REQ_VIDEO_DATETIME_PRINT, VIDEO_DATETIME_PRINT)
YOUKU_CMD_ITEM(CAM_REQ_VIDEO_AUDIO_RECORD, VIDEO_AUDIO_RECORD)
YOUKU_CMD_ITEM(CAM_REQ_VIDEO_WDR, VIDEO_WDR)
YOUKU_CMD_ITEM(CAM_REQ_PHOTO_DEFAULT_MODE, PHOTO_DEFAULT_MODE)
YOUKU_CMD_ITEM(CAM_REQ_PHOTO_RESOLUTION, PHOTO_RESOLUTION)
//YOUKU_CMD_ITEM(CAM_REQ_PHOTO_CONTINUOUS_RATE, PHOTO_CONTINUOUS_RATE)
//YOUKU_CMD_ITEM(CAM_REQ_PHOTO_SPOT_METER, PHOTO_SPOT_METER)
//YOUKU_CMD_ITEM(CAM_REQ_PHOTO_FOV, PHOTO_FOV)
//YOUKU_CMD_ITEM(CAM_REQ_PHOTO_EFFECT, VIDEO_EFFECT)                //same with video
YOUKU_CMD_ITEM(CAM_REQ_PHOTO_WHITE_BALANCE, VIDEO_WHITE_BALANCE)  //same with video
//YOUKU_CMD_ITEM(CAM_REQ_PHOTO_COLOR, VIDEO_COLOR)                  //same with video
YOUKU_CMD_ITEM(CAM_REQ_PHOTO_ISO, VIDEO_ISO)                      //same with video
YOUKU_CMD_ITEM(CAM_REQ_PHOTO_SHARPNESS, VIDEO_SHARPNESS)          //same with video
YOUKU_CMD_ITEM(CAM_REQ_PHOTO_EV, VIDEO_EV)                        //same with video
//YOUKU_CMD_ITEM(CAM_REQ_PHOTO_HDR, PHOTO_HDR)
//YOUKU_CMD_ITEM(CAM_REQ_PHOTO_COUNTDOWN_INTERVEL, PHOTO_COUNTDOWN_INTERVEL)
YOUKU_CMD_ITEM(CAM_REQ_PHOTO_DATETIME_PRINT, PHOTO_DATETIME_PRINT)
YOUKU_CMD_ITEM(CAM_REQ_COMM_STREAM_SIZE, DEV_STREAM_SIZE)
YOUKU_CMD_ITEM(CAM_REQ_COMM_STREAM_BIT_RATE, STREAM_BIT_RATE)
YOUKU_CMD_ITEM(CAM_REQ_COMM_LCD_LOCK, COMM_LCD_LOCK)
#if (AUTOSLEEP_FUNCTION == ENABLE)
YOUKU_CMD_ITEM(CAM_REQ_COMM_LCD_SLEEP, COMM_LCD_SLEEP)
#endif
//YOUKU_CMD_ITEM(CAM_REQ_COMM_LED_BLINK, COMM_LED_BLINK)
YOUKU_CMD_ITEM(CAM_REQ_COMM_BEEP_VOLUME, COMM_BEEP_VOLUME)
#if (AUTOPWROFF_FUNCTION == ENABLE)
YOUKU_CMD_ITEM(CAM_REQ_COMM_AUTO_POWER_DOWN, COMM_AUTO_POWER_DOWN)
#endif
YOUKU_CMD_ITEM(CAM_REQ_COMM_RESET_FACTORY, COMM_RESET_FACTORY)
YOUKU_CMD_ITEM(CAM_REQ_COMM_FORMAT_SD, DEV_FORMAT_SD)
YOUKU_CMD_ITEM(CAM_REQ_COMM_RESET_SSID, COMM_RESET_SSID)
YOUKU_CMD_ITEM(CAM_REQ_COMM_SHUTTER, COMM_SHUTTER)
YOUKU_CMD_ITEM(CAM_REQ_COMM_CHANGE_CAMERA_MODE, COMM_CHANGE_CAMERA_MODE)
YOUKU_CMD_ITEM(CAM_REQ_COMM_POWEROFF, COMM_POWEROFF)
YOUKU_CMD_ITEM(CAM_REQ_COMM_UPDATE_DOWNLOAD_DONE, COMM_UPDATE_DOWNLOAD_DONE)
YOUKU_CMD_ITEM(CAM_REQ_COMM_STORAGE_DELETE_ALL, STORAGE_DELETE_ALL)
YOUKU_CMD_ITEM(CAM_REQ_COMM_STORAGE_DELETE, STORAGE_DELETE)
YOUKU_CMD_ITEM(CAM_REQ_COMM_STORAGE_DELETE_LAST, STORAGE_DELETE_LAST)
YOUKU_CMD_ITEM(CAM_REQ_COMM_STREAM_ENABLE, SERVER_STREAM_ENABLE)
YOUKU_CMD_ITEM(CAM_REQ_COMM_THUMBNAIL, MEDIA_THUMBNAIL)
YOUKU_CMD_ITEM(CAM_REQ_COMM_DEVINFO_GET,MEDIA_DEVINFO)
YOUKU_CMD_ITEM(CAM_REQ_COMM_DATETIME_SYNC,DATETIME_SYNC)
YOUKU_CMD_ITEM(CAM_REQ_COMM_CLEAN_NEW_VIDEO_FLAG,CLEAN_NEW_VIDEO_FLAG)
YOUKU_CMD_END()

int YoukuCmdHandler(int command, handler_parameter *param, void *userdata)
{
    UINT32 tabSize;
    UINT32 index;
    BOOL Hit;
    int ret = YOUKU_CMD_SUCCESS;
#if 1
    if(command)
    {
        DBG_IND("CMD=%d, ",command);
        if(ALIGNED_POINTER((UINT32)param) && ALIGNED_POINTER((UINT32)param->scall_back_param))
        {
            DBG_MSG("Req=%d\r\n",*(int *)param->scall_back_param);
        }
        DBG_MSG("\r\n");
    }
#endif

    if(CAM_REQ_VIDEO_DEFAULT_MODE <= command && command <= CAM_REQ_VIDEO_WDR )
    {
        if(!YoukuSdk_ChkMode(PRIMARY_MODE_MOVIE))
            return YOUKU_CMD_MODE_ERROR;
        else if(YoukuSdk_IsRecording())
            return YOUKU_CMD_INVALID_DURING_RECORD;
    }
    if(CAM_REQ_PHOTO_DEFAULT_MODE <= command && command <= CAM_REQ_PHOTO_DATETIME_PRINT && !YoukuSdk_ChkMode(PRIMARY_MODE_PHOTO))
    {
        return YOUKU_CMD_MODE_ERROR;
    }

    Hit = FALSE;
    tabSize = sizeof(Cmd_youku)/sizeof(YOUKU_CMD_ENTRY);
    for(index=0;index<tabSize;index++)
    {
        if (command==(int)Cmd_youku[index].cmd)
        {
            Hit = TRUE;
            if (Cmd_youku[index].fpFunc)
                ret = Cmd_youku[index].fpFunc(param, userdata);

            break;
        }
    }

    if (Hit==FALSE)
    {
        DBG_DUMP("Cmd[%d] not supported!\r\n",command);
        ret = YOUKU_CMD_NOT_SUPPORTED;
    }
    return ret;

}

INT32 YoukuSdkOpen(void)
{
    int err = E_OK;
    static ykcamera_interface camif={0};
    static camera_status cam_status;
    static hfs_info hfs = {0};
    static web_sever_param wsp;
    static char HfsRoot[32]={0};
    #if (SAVE_DUAL_STREAM_FUNC ==  ENABLE)
    static char WiFiMovie[40]={0};
    #endif
    static char otafile[40]={0};
    #if (CUT_MOVIE_FUNC == ENABLE)
    static char CutMovie[40]={0};
    #endif
    //start IPC for 660 only
    MEM_RANGE WorkBuf;
    WorkBuf.Addr = OS_GetMempoolAddr(POOL_ID_YOUKUIPC);
    WorkBuf.Size = POOL_SIZE_YOUKUIPC;
    YoukuIpc_Open(&WorkBuf);


    snprintf((char *)ipc_getNonCacheAddr((UINT32)HfsRoot), sizeof(HfsRoot),"%s/%s",MOUNT_FS_ROOT,FILEDB_CARDV_ROOT);
    #if (SAVE_DUAL_STREAM_FUNC ==  ENABLE)
    snprintf((char *)ipc_getNonCacheAddr((UINT32)WiFiMovie), sizeof(WiFiMovie),"%s/%s",HfsRoot,WIFI_MOVIE_FOLDER);
    hfs.hfs_preview_path = WiFiMovie;
    #endif
    #if (CUT_MOVIE_FUNC == ENABLE)
    snprintf((char *)ipc_getNonCacheAddr((UINT32)CutMovie), sizeof(CutMovie),"%s/%s",HfsRoot,CUT_MOVIE_FOLDER);
    hfs.hfs_cut_path = CutMovie;
    #endif
    NvtPath2YoukuPath(FW_UPDATE_NAME, (char *)ipc_getNonCacheAddr((UINT32)otafile));
    //DBG_DUMP("^Botafile=%s\r\n",otafile);
    hfs.hfs_path = HfsRoot;
    hfs.ota_path = otafile;
    hfs.hfs_port = "8080";
    wsp.hfs_info = &hfs;
    wsp.user_data = "userdata for callback";
    wsp.cam_handler = YoukuCmdHandler;
    wsp.control_port = "8010";
    DBG_IND("HfsRoot=%s\r\n",hfs.hfs_path);
    #if 0
    cyg_FileSys_RegisterCB();
    err = mount( "", MOUNT_FS_ROOT, "nvtfs" );
    if( err < 0 )
    {
        DBG_ERR("mount fs fail %d\r\n",err);
    }
    #endif
    cam_status.device_status=ykcamera_device_status;
    cam_status.setting_status =ykcamera_setting_status;
#if 0
DBGH(hfs.hfs_path);
DBGH(hfs.hfs_preview_path);
DBGH(hfs.ota_path);
DBGH(hfs.hfs_cut_path);
DBGH(hfs.hfs_port);
DBGH(wsp.hfs_info);
DBGH(wsp.user_data);
DBGH(wsp.cam_handler);
DBGH(wsp.control_port);
DBGH(cam_status.device_status);
DBGH(cam_status.setting_status);
#endif
    camif.wsp = &wsp;
    camif.cam_status = &cam_status;

    UpdateAllStatus();
    youku_camera_init(&camif);
    youku_camera_start();

    return err;
}
void YoukuSdkClose(void)
{
    DBG_IND("++\r\n");
    youku_camera_deinitialize();
    //umount(MOUNT_FS_ROOT);
    YoukuIpc_Close();
    DBG_IND("--\r\n");
}
void YoukuSdkTriggerUpload(void)
{
    m_bNewUploadVideo = TRUE;
}
#endif
