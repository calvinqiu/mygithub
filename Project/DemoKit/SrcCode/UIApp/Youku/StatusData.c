#include "PrjCfg.h"
#if (YOUKU_SDK == ENABLE)
#include "StatusData.h"
#include "Type.h"
#include "YoukuAppCmd.h"


#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          StatusData
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

module_value ykcamera_device_status[DEVICE_MAX_ITEM]=
{
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},        //DEVICE_INTERNAL_BATTERY_PRESENT ,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},         //DEVICE_EXTERNAL_BATTERY_PRESENT,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},         //DEVICE_HOT,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},         //DEVICE_BUSY,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},         //DEVICE_QUICK_CAPTURE_ACTIVE,
    {CAM_TYPE_STRING, (int)STATUS_STRING_NONE},         //DEVICE_DATE_TIME,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //VIDEO_PROGRESS_COUNTER,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},         //SETTING_COMM_STREAM_SUPPORT,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},         //STORAGE_SD_STATUS,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},         //STORAGE_REMAINING_PHOTOS,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},         //STORAGE_REMAINING_VIDEO_TIME,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},         //STORAGE_NUM_TOTAL_PHOTOS,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},         //STORAGE_NUM_TOTAL_VIDEOS,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},         //STORAGE_REMAINING_SPACE,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},         //OTA_STATUS,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},      //OTA_DOWNLOAD_CANCEL_REQUEST_PENDING,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE}      //VIDEO_NEW_UPLOAD_VIDEO,
};

module_value ykcamera_setting_status[SETTING_MAX_ITEM]=
{
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_VIDEO_DEFAULT_MODE,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_VIDEO_RESOLUTION,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_VIDEO_FPS,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_VIDEO_RESOLUTION_FPS
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_VIDEO_FOV,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_VIDEO_TIMELAPSE_RATE,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_VIDEO_LOOPING,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_VIDEO_PIV,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_VIDEO_LOW_LIGHT,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_VIDEO_SPOT_METER,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_VIDEO_EFFECT,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_VIDEO_WHITE_BALANCE,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_VIDEO_COLOR,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_VIDEO_ISO,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_VIDEO_SHARPNESS,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_VIDEO_EV,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_VIDEO_DATETIME_PR,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_VIDEO_AUDIO_RECORD,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_VIDEO_HDR,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_VIDEO_WDR,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_PHOTO_DEFAULT_MODE,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_PHOTO_RESOLUTION,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_PHOTO_CONTINUOUS_RATE,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_PHOTO_EXPOSURE_TIME,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_PHOTO_SPOT_METER,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_PHOTO_EFFECT,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_PHOTO_WHITE_BALANCE,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_PHOTO_COLOR,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_PHOTO_ISO,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_PHOTO_SHARPNESS,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_PHOTO_HDR,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_PHOTO_WDR,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_PHOTO_EV,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_PHOTO_FOV,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_PHOTO_COUNTDOWN_ERVEL,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_PHOTO_CORDINATE,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_PHOTO_DATE_PRINT,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_COMM_STREAM_SIZE,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_COMM_STREAM_WINDOW_SIZE,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_COMM_STREAM_BIT_RATE,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_COMM_LCD_BRIGHTNESS,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_COMM_LCD_LOCK,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_COMM_LCD_SLEEP,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_COMM_ORIENTATION,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_COMM_LED_BLINK,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_COMM_QUICK_CAPTURE,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_COMM_BEEP_VOLUME,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_COMM_AUTO_POWER_DOWN,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_COMM_GPS_MODE,
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_COMM_WIRELESS_MODE
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_COMM_RESET
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_COMM_FORMAT
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_COMM_WIFI_RESET
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_COMM_SHUTTER
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_COMM_CAMERA_MODE
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_COMM_POWEROFF
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_COMM_UPDATE_DOWNLOAD_START
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_COMM_UPDATE_DOWNLOAD_DONE
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_COMM_UPDATE_DOWNLOAD_CANCEL
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_COMM_STORAGE_DELETE_ALL
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_COMM_STORAGE_DELETE
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_COMM_STORAGE_DELETE_LAST
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_COMM_STREAM_ENABLE
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_COMM_THUMBNAIL
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_COMM_DEVINFO_GET
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},     //SETTING_COMM_MEDIA_LIST
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE},      //SETTING_COMM_DATETIME_SYNC
    {CAM_TYPE_INTEGER, STATUS_NUMBER_NONE}       //CAM_REQ_COMM_CLEAN_NEW_VIDEO_FLAG
};




#endif//(YOUKU_SDK == ENABLE)
