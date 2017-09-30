#ifndef __MENUID_H
#define __MENUID_H

#include "UIResource.h"

// Menu Item ID
enum _MENU_ID
{
    IDM_NULL,

    // Menu common page
    IDM_COMMON_MODE,
    IDM_COMMON_MENU,
    IDM_COMMON_LOCAL,
    IDM_COMMON_CLOUD,
    IDM_COMMON_SETUP,
    IDM_COMMON_EXT_SETUP,

    // menu mode page
    IDM_MODE_MOVIE,
    IDM_MODE_PHOTO,
    IDM_MODE_PLAYBACK,
    // Photo Page
    IDM_SELFTIMER,
    IDM_PHOTO_SIZE,
    IDM_CONTINUE_SHOT,
    IDM_QUALITY,
    IDM_SHARPNESS,
    IDM_WB,
    IDM_ANTISHAKE,
    IDM_QUICK_REVIEW,
    IDM_DATE_STAMP,
    IDM_ISO,
    IDM_EV,
    IDM_FD,
    IDM_FLASH,
    IDM_RSC,
    IDM_HDR,
    IDM_WDR,
    IDM_METERING,
    IDM_SCENE,
    IDM_DUAL_CAM,

    // Movie Page
    IDM_MOVIE_SIZE,
    IDM_MOVIE_CYCLIC_REC,
    IDM_MOVIE_WDR,
    IDM_MOVIE_HDR,
    IDM_MOVIE_EV,
    IDM_MOVIE_MOTION_DET,
    IDM_MOVIE_AUDIO,
    IDM_MOVIE_DATEIMPRINT,
    IDM_MOVIE_RSC,
    IDM_MOVIE_GSENSOR,
    IDM_MOVIE_TIMELAPSE_REC,
    IDM_MOVIE_IR_CUT,
    IDM_MOVIE_SENSOR_ROTATE,
    IDM_MOVIE_PTZ,
    IDM_MOVIE_URGENT_PROTECT_AUTO,
    IDM_MOVIE_URGENT_PROTECT_MANUAL,
    IDM_MOVIE_PIM,
    IDM_MOVIE_FCW,
    IDM_MOVIE_LDWS,
    IDM_MOVIE_DDD,
    IDM_MOVIE_ADAS_CAL,
    IDM_MOVIE_QUALITY,
    IDM_MOVIE_WB,
    IDM_MOVIE_COLOR,
    IDM_MOVIE_DUAL_CAM,

    // Playback Page
    IDM_PROTECT,
    IDM_VOLUME,

    // Setup Page
    IDM_DATE_TIME,
    IDM_AUTO_POWER_OFF,
    IDM_BEEP,
    IDM_SILENT,
    IDM_LANGUAGE,
    IDM_TV_MODE,
    IDM_FREQUENCY,
    IDM_FORMAT,
    IDM_DEFAULT,
    IDM_OPENING_LOGO,
    IDM_SCREEN_SAVE,
    IDM_FW_UPDATE,
    IDM_DELETE_ALL,
    IDM_PROTECT_ALL,
    IDM_UNPROTECT_ALL,
    IDM_WIFI,
    IDM_VERSION,
    IDM_DELETE_THIS,

    // Extra Setup Page
    IDM_MCTF,
    IDM_EDGE,
    IDM_NR,
    IDM_IPP_ISO,

    // Mode Page for touch
    IDM_MODETOUCH_MOVIE,
    IDM_MODETOUCH_PHOTO,
    IDM_MODETOUCH_PLAYBACK,
    IDM_MODETOUCH_SETUP,
};

// Menu String ID
enum _MENU_IDS
{
    // Photo Page

    IDS_SELFTIMER           = STRID_SELFTIMER,
    IDS_PHOTO_SIZE          = STRID_RESOLUTION,
    IDS_CONTINUE_SHOT       = STRID_CAP_BURST,
    IDS_QUALITY             = STRID_QUALITY,
    IDS_SHARPNESS           = STRID_SHARPNESS,
    IDS_WB                  = STRID_WB,
    IDS_ANTISHAKE          = STRID_ANTI_SHAKING,
    IDS_QUICK_REVIEW        = STRID_QUICK_VIEW,
    IDS_DATE_STAMP          = STRID_DATE_STAMP,
    IDS_ISO                 = STRID_ISO,
    IDS_EV                  = STRID_EXPOSURE,
    IDS_FD                  = STRID_FACE_DET,
    IDS_FLASH               = STRID_FLASHMODE,
    IDS_RSC                 = STRID_RSC,
    IDS_HDR                 = STRID_HDR,
    IDS_WDR                 = STRID_WDR,
    IDS_METERING            = STRID_METERING,
    IDS_SCENE               = STRID_SCENE,
    IDS_DUAL_CAM            = STRID_DUAL_CAM,
    IDS_PHOTO               = STRID_STILL,

    // Photo Capture Options
    IDS_SELFTIMER_OFF       = STRID_OFF,
    IDS_SELFTIMER_2SEC      = STRID_CAP_TIMER2S,
    IDS_SELFTIMER_5SEC      = STRID_CAP_TIMER5S,
    IDS_SELFTIMER_10SEC     = STRID_CAP_TIMER10S,

    // Photo Size Options
    IDS_PHOTO_SIZE_15M      = STRID_NULL_,
    IDS_PHOTO_SIZE_14M      = STRID_NULL_,
    IDS_PHOTO_SIZE_12M      = STRID_12MWXH,
    IDS_PHOTO_SIZE_10M      = STRID_10MWXH,
    IDS_PHOTO_SIZE_8M       = STRID_8MWXH,
    IDS_PHOTO_SIZE_7M       = STRID_7MWXH,
    IDS_PHOTO_SIZE_7MHD     = STRID_NULL_,
    IDS_PHOTO_SIZE_5M       = STRID_5MWXH,
    IDS_PHOTO_SIZE_4M       = STRID_4MWXH,
    IDS_PHOTO_SIZE_3M       = STRID_3MWXH,
    IDS_PHOTO_SIZE_2M       = STRID_2MWXH,
    IDS_PHOTO_SIZE_2MHD     = STRID_2MHDWXH,
    IDS_PHOTO_SIZE_1M       = STRID_1MWXH,
    IDS_PHOTO_SIZE_VGA      = STRID_VGAWXH,

    //Continue shot
    IDS_CONTINUE_SHOT_OFF   = STRID_CAP_SINGLE,
    IDS_CONTINUE_SHOT_3     = STRID_CAP_BURST_3,
    IDS_CONTINUE_SHOT_ON    = STRID_CAP_BURST,

    // Metering Options
    IDS_AVERAGE             = STRID_METER_AVG,
    IDS_CENTER              = STRID_METER_CENTER,
    IDS_SPOT                = STRID_METER_SPOT,

    //ISO
    IDS_ISO_AUTO            = STRID_AUTO,
    IDS_ISO_100             = STRID_ISO100,
    IDS_ISO_200             = STRID_ISO200,
    IDS_ISO_400             = STRID_ISO400,
    IDS_ISO_800             = STRID_ISO800,
    IDS_ISO_1600            = STRID_ISO1600,

    // Sharpness Options
    IDS_SHARP_STRONG        = STRID_STRONG,
    IDS_SHARP_NORMAL        = STRID_NORMAL,
    IDS_SHARP_SOFT          = STRID_SOFT,

    // WB Options
    IDS_WB_AUTO             = STRID_AUTO,
    IDS_WB_DAYLIGHT         = STRID_WB_DAY,
    IDS_WB_CLOUDY           = STRID_WB_CLOUDY,
    IDS_WB_TUNGSTEN         = STRID_WB_TUNGSTEN,
    IDS_WB_FLUORESCENT      = STRID_WB_FLUORESCENT,

    // EV Options
    IDS_EV_P20              = STRID_EV_P2P0,
    IDS_EV_P16              = STRID_EV_P1P6,
    IDS_EV_P13              = STRID_EV_P1P3,
    IDS_EV_P10              = STRID_EV_P1P0,
    IDS_EV_P06              = STRID_EV_P0P6,
    IDS_EV_P03              = STRID_EV_P0P3,
    IDS_EV_00               = STRID_EV_P0P0,
    IDS_EV_N03              = STRID_EV_M0P3,
    IDS_EV_N06              = STRID_EV_M0P6,
    IDS_EV_N10              = STRID_EV_M1P0,
    IDS_EV_N13              = STRID_EV_M1P3,
    IDS_EV_N16              = STRID_EV_M1P6,
    IDS_EV_N20              = STRID_EV_M2P0,

    // Face Detection Options
    IDS_FD_OFF              = STRID_OFF,
    IDS_FD_ON               = STRID_ON,

    // Anti-shaking Options
    IDS_ANTISHAKE_OFF       = STRID_OFF,
    IDS_ANTISHAKE_ON        = STRID_ON,

    // Quick View Time Options
    IDS_QV_OFF              = STRID_OFF,
    IDS_QV_2SEC             = STRID_2SEC,
    IDS_QV_5SEC             = STRID_5SEC,

    // Date Stamp Options
    IDS_DATE_STAMP_OFF      = STRID_OFF,
    IDS_DATE_STAMP_DATE     = STRID_DATE,
    IDS_DATE_STAMP_DATE_TIME = STRID_DATE_TIME,

    // Flash Options
    IDS_FLASH_AUTO          = STRID_FLASHAUTO,
    IDS_FLASH_ON            = STRID_ON,
    IDS_FLASH_OFF           = STRID_OFF,
    IDS_FLASH_RED_EYE       = STRID_REDEYE,

    // RSC Options
    IDS_RSC_ON              = STRID_ON,
    IDS_RSC_OFF             = STRID_OFF,

    // HDR Options
    IDS_HDR_ON              = STRID_ON,
    IDS_HDR_OFF             = STRID_OFF,

    // WDR Options
    IDS_WDR_ON              = STRID_ON,
    IDS_WDR_OFF             = STRID_OFF,

    // SCENE Options
    IDS_SCENE_AUTO          = STRID_AUTO,
    IDS_SCENE_PORTRAIT      = STRID_PORTRAIT,
    IDS_SCENE_LANDSCAPE     = STRID_LANDSCAPE,

    // DUAL CAM Options
    IDS_DUAL_CAM_FRONT      = STRID_FRONT,
    IDS_DUAL_CAM_BEHIND     = STRID_BEHIND,
    IDS_DUAL_CAM_BOTH       = STRID_BOTH,

    // Movie Page
    IDS_MOVIE_SIZE          = STRID_RESOLUTION,
    IDS_MOVIE_EV            = STRID_EXPOSURE,
    IDS_MOVIE_CYCLIC_REC    = STRID_CYCLIC_REC,
    IDS_MOVIE_HDR           = STRID_HDR,
    IDS_MOVIE_WDR           = STRID_WDR,
    IDS_MOVIE_MOTION_DET    = STRID_MOTION_DET,
    IDS_MOVIE_AUDIO         = STRID_RECORD_AUDIO,
    IDS_MOVIE_DATEIMPRINT   = STRID_DATE_STAMP,
    IDS_MOVIE_RSC           = STRID_RSC,
    IDS_MOVIE_GSENSOR       = STRID_G_SENSOR,
    IDS_MOVIE_TIMELAPSE_REC = STRID_TIMELAPSE_REC,
    IDS_MOVIE_IR_CUT        = STRID_IRCUT,
    IDS_MOVIE_SENSOR_ROTATE = STRID_SENSOR_ROTATE,
    IDS_MOVIE_PTZ           = STRID_PTZ,
    IDS_MOVIE_URGENT_PROTECT_AUTO   = STRID_URGENT_PROTECT_AUTO,
    IDS_MOVIE_URGENT_PROTECT_MANUAL = STRID_URGENT_PROTECT_MANUAL,
    IDS_MOVIE_PIM           = STRID_PIM,
    IDS_MOVIE_FCW           = STRID_FCW,
    IDS_MOVIE_LDWS          = STRID_LDWS,
    IDS_MOVIE_DDD           = STRID_DDD,
    IDS_MOVIE_ADAS_CAL      = STRID_ADAS,
    IDS_MOVIE_QUALITY       = STRID_QUALITY,
    IDS_MOVIE_WB            = STRID_WB,
    IDS_MOVIE_COLOR         = STRID_COLOR,
    IDS_MOVIE               = STRID_MOVIE,
    IDS_MOVIE_DUAL_CAM      = STRID_DUAL_CAM,

    // Movie Size Options
    IDS_MOVIE_SIZE_2880x2160P24  = STRID_2880X2160P24, // 2160P  24fps 2880x2160
    IDS_MOVIE_SIZE_2560x1440P30  = STRID_2560X1440P30, // 1440P  30fps 2560x1440
    #if 0
    IDS_MOVIE_SIZE_2304x1296P30  = STRID_2304X1296P30, // 1296P  30fps 2304x1296
    IDS_MOVIE_SIZE_1080P96       = STRID_1080P96,      // 1080P  96fps 1920x1080
    IDS_MOVIE_SIZE_1080P_DUAL    = STRID_1080P_DUAL,   // 1080PD 30fps 1920x1080
    IDS_MOVIE_SIZE_720P120       = STRID_720P120WXH,   // 720P  120fps 1280x720
    #endif
    //MOVIE_SIZE_TAG
    IDS_MOVIE_SIZE_FRONT_2880x2160P24   = STRID_FRONT_2880x2160P24, //UHD P24
    IDS_MOVIE_SIZE_FRONT_2560x1440P30   = STRID_FRONT_2560x1440P30, //QHD P30
    IDS_MOVIE_SIZE_FRONT_2304x1296P30   = STRID_FRONT_2304x1296P30, //3MHD P30
    IDS_MOVIE_SIZE_FRONT_1920x1080P96   = STRID_FRONT_1920x1080P96, //FHD
    IDS_MOVIE_SIZE_FRONT_1920x1080P60   = STRID_FRONT_1920x1080P60, //FHD
    IDS_MOVIE_SIZE_FRONT_1920x1080P30   = STRID_FRONT_1920x1080P30, //FHD
    IDS_MOVIE_SIZE_FRONT_1280x720P120   = STRID_FRONT_1280x720P120, //HD
    IDS_MOVIE_SIZE_FRONT_1280x720P60    = STRID_FRONT_1280x720P60, //HD
    IDS_MOVIE_SIZE_FRONT_1280x720P30    = STRID_FRONT_1280x720P30,  //HD
    IDS_MOVIE_SIZE_FRONT_848x480P30     = STRID_FRONT_848x480P30,   //WVGA
    IDS_MOVIE_SIZE_FRONT_640x480P240    = STRID_FRONT_640x480P240,   //VGA
    IDS_MOVIE_SIZE_FRONT_640x480P30     = STRID_FRONT_640x480P30,   //VGA
    IDS_MOVIE_SIZE_FRONT_320x240P30     = STRID_FRONT_320x240P30,   //QVGA
    IDS_MOVIE_SIZE_DUAL_1920x1080P30_1920x1080P30  = STRID_DUAL_1080P30_1080P30, //FHD FHD
    IDS_MOVIE_SIZE_CLONE_1920x1080P30_1920x1080P30 = STRID_CLONE_1920x1080P30_1920x1080P30,  //FHD FHD
    IDS_MOVIE_SIZE_CLONE_1920x1080P30_1280x720P30  = STRID_CLONE_1920x1080P30_1280x720P30,   //FHD HD
    IDS_MOVIE_SIZE_CLONE_2560x1440P30_848x480P30   = STRID_CLONE_2560x1440P30_848x480P30,  //QHD WVGA
    IDS_MOVIE_SIZE_CLONE_2304x1296P30_848x480P30   = STRID_CLONE_2304x1296P30_848x480P30,  //3MHD WVGA
    IDS_MOVIE_SIZE_CLONE_1920x1080P60_848x480P30   = STRID_CLONE_1920x1080P60_848x480P30,  //FHD WVGA
    IDS_MOVIE_SIZE_CLONE_1920x1080P60_640x360P30   = STRID_CLONE_1920x1080P60_640x360P30,  //FHD WVGA
    IDS_MOVIE_SIZE_CLONE_1920x1080P30_848x480P30   = STRID_CLONE_1920x1080P30_848x480P30,  //FHD WVGA
    IDS_MOVIE_SIZE_CLONE_2048x2048P30_480x480P30   = STRID_CLONE_2048x2048P30_480x480P30,


    #if 0
    IDS_MOVIE_SIZE_1080FHD  = STRID_1080FHDWXH,
    IDS_MOVIE_SIZE_1080P_DUAL = STRID_1080P_DUAL,
    IDS_MOVIE_SIZE_720P120 = STRID_720P120WXH,
    IDS_MOVIE_SIZE_1080P    = STRID_1080PWXH,
    IDS_MOVIE_SIZE_1440P    = STRID_1080PWXH,
    IDS_MOVIE_SIZE_720P     = STRID_720PWXH,
    IDS_MOVIE_SIZE_WVGA     = STRID_WVGAWXH,
    IDS_MOVIE_SIZE_VGA      = STRID_VGAWXH,
    IDS_MOVIE_SIZE_QVGA     = STRID_QVGAWXH,
    #endif

    // EV Options
    IDS_MOVIE_EV_P20        = STRID_EV_P2P0,
    IDS_MOVIE_EV_P16        = STRID_EV_P1P6,
    IDS_MOVIE_EV_P13        = STRID_EV_P1P3,
    IDS_MOVIE_EV_P10        = STRID_EV_P1P0,
    IDS_MOVIE_EV_P06        = STRID_EV_P0P6,
    IDS_MOVIE_EV_P03        = STRID_EV_P0P3,
    IDS_MOVIE_EV_00         = STRID_EV_P0P0,
    IDS_MOVIE_EV_N03        = STRID_EV_M0P3,
    IDS_MOVIE_EV_N06        = STRID_EV_M0P6,
    IDS_MOVIE_EV_N10        = STRID_EV_M1P0,
    IDS_MOVIE_EV_N13        = STRID_EV_M1P3,
    IDS_MOVIE_EV_N16        = STRID_EV_M1P6,
    IDS_MOVIE_EV_N20        = STRID_EV_M2P0,


    // Movie loop rec Options
    IDS_MOVIE_CYCLIC_REC_OFF    = STRID_OFF,
    IDS_MOVIE_CYCLIC_REC_3MIN   = STRID_3MIN,
    IDS_MOVIE_CYCLIC_REC_5MIN   = STRID_5MIN,
    IDS_MOVIE_CYCLIC_REC_10MIN  = STRID_10MIN,

    // Movie motion detect Options
    IDS_MOVIE_MOTION_DET_OFF  = STRID_OFF,
    IDS_MOVIE_MOTION_DET_ON   = STRID_ON,

    // Movie HDR options
    IDS_MOVIE_HDR_OFF  = STRID_OFF,
    IDS_MOVIE_HDR_ON   = STRID_ON,

    // Movie WDR options
    IDS_MOVIE_WDR_OFF  = STRID_OFF,
    IDS_MOVIE_WDR_ON   = STRID_ON,

    // Movie audio Options
    IDS_MOVIE_AUDIO_OFF     = STRID_OFF,
    IDS_MOVIE_AUDIO_ON      = STRID_ON,

    // Movie dateImprint Options
    IDS_MOVIE_DATEIMPRINT_OFF = STRID_OFF,
    IDS_MOVIE_DATEIMPRINT_ON  = STRID_ON,

    // Movie RSC Options
    IDS_MOVIE_RSC_OFF         = STRID_OFF,
    IDS_MOVIE_RSC_ON          = STRID_ON,

    // Movie G Sensor Options
    IDS_MOVIE_GSENSOR_OFF     = STRID_OFF,
    IDS_MOVIE_GSENSOR_LOW     = STRID_LOW,
    IDS_MOVIE_GSENSOR_MED     = STRID_MED,
    IDS_MOVIE_GSENSOR_HIGH    = STRID_HIGH,

    // Movie Timelapse record Options
    IDS_MOVIE_TIMELAPSE_REC_OFF     = STRID_OFF,
    IDS_MOVIE_TIMELAPSE_REC_1SEC    = STRID_1SEC,
    IDS_MOVIE_TIMELAPSE_REC_5SEC    = STRID_5SEC,
    IDS_MOVIE_TIMELAPSE_REC_10SEC   = STRID_10SEC,
    IDS_MOVIE_TIMELAPSE_REC_30SEC   = STRID_30SEC,
    IDS_MOVIE_TIMELAPSE_REC_1MIN    = STRID_1MIN,
    IDS_MOVIE_TIMELAPSE_REC_5MIN    = STRID_5MIN,
    IDS_MOVIE_TIMELAPSE_REC_10MIN   = STRID_10MIN,
    IDS_MOVIE_TIMELAPSE_REC_30MIN   = STRID_30MIN,
    IDS_MOVIE_TIMELAPSE_REC_1HOUR   = STRID_1HOUR,
    IDS_MOVIE_TIMELAPSE_REC_2HOUR   = STRID_2HOUR,
    IDS_MOVIE_TIMELAPSE_REC_3HOUR   = STRID_3HOUR,
    IDS_MOVIE_TIMELAPSE_REC_1DAY    = STRID_1DAY,

    // Movie IR Cut Options
    IDS_MOVIE_IR_CUT_OFF         = STRID_OFF,
    IDS_MOVIE_IR_CUT_ON          = STRID_ON,

    // Movie Sensor Rotate Options
    IDS_MOVIE_SENSOR_ROTATE_OFF  = STRID_OFF,
    IDS_MOVIE_SENSOR_ROTATE_ON   = STRID_ON,

    // Movie PTZ Options
    IDS_MOVIE_PTZ_OFF            = STRID_OFF,
    IDS_MOVIE_PTZ_ON             = STRID_ON,

    // Movie auto urgent protect Options
    IDS_MOVIE_URGENT_PROTECT_AUTO_OFF   = STRID_OFF,
    IDS_MOVIE_URGENT_PROTECT_AUTO_ON    = STRID_ON,

    // Movie manual urgent protect Options
    IDS_MOVIE_URGENT_PROTECT_MANUAL_OFF   = STRID_OFF,
    IDS_MOVIE_URGENT_PROTECT_MANUAL_ON    = STRID_ON,

    // Movie PIM Options
    IDS_MOVIE_PIM_OFF                   = STRID_OFF,
    IDS_MOVIE_PIM_ON                    = STRID_ON,

    // Movie FCW Options
    IDS_MOVIE_FCW_OFF                   = STRID_OFF,
    IDS_MOVIE_FCW_ON                    = STRID_ON,

    // Movie LDWS Options
    IDS_MOVIE_LDWS_OFF                   = STRID_OFF,
    IDS_MOVIE_LDWS_ON                    = STRID_ON,

    // Movie DDD Options
    IDS_MOVIE_DDD_OFF                   = STRID_OFF,
    IDS_MOVIE_DDD_ON                    = STRID_ON,

    // Movie ADAS CAL Options
    IDS_MOVIE_ADAS_CAL_OFF              = STRID_OFF,
    IDS_MOVIE_ADAS_CAL_ON               = STRID_ON,

    // Movie DUAL CAM Options
    IDS_MOVIE_DUAL_CAM_FRONT            = STRID_FRONT,
    IDS_MOVIE_DUAL_CAM_BEHIND           = STRID_BEHIND,
    IDS_MOVIE_DUAL_CAM_BOTH             = STRID_BOTH,

    // Movie Quality Options
    IDS_QUALITY_FINE        = STRID_FINE,
    IDS_QUALITY_NORMAL      = STRID_NORMAL,
    IDS_QUALITY_ECONOMY     = STRID_ECONOMY,

    // Playback Page
    IDS_PROTECT             = STRID_PROTECT,
    IDS_VOLUME              = STRID_VOLUME,
    IDS_PLAYBACK            = STRID_PLAYBACK,

    // Volume Options
    IDS_VOL0                 = STRID_0 ,
    IDS_VOL1                 = STRID_1 ,
    IDS_VOL2                 = STRID_2 ,
    IDS_VOL3                 = STRID_3 ,
    IDS_VOL4                 = STRID_4 ,
    IDS_VOL5                 = STRID_5 ,
    IDS_VOL6                 = STRID_6 ,
    IDS_VOL7                 = STRID_7 ,
    IDS_VOL8                 = STRID_8 ,
    IDS_VOL9                 = STRID_9 ,
    IDS_VOL10                = STRID_10,

    // Common Page
    IDS_COMMON              = STRID_MODE,
    IDS_COMMON_MODE         = STRID_MODE,
    IDS_COMMON_MENU         = STRID_SETUP,
    #if ((_NETWORK_ == _NETWORK_SPI_EVB_ETHERNET_)||(_NETWORK_ == _NETWORK_GMAC_ETHERNET_))
    IDS_COMMON_LOCAL        = STRID_WIFI_ETH,
    #else
    IDS_COMMON_LOCAL        = STRID_WIFI,
    #endif
    IDS_COMMON_CLOUD        = STRID_CLOUD,
    IDS_COMMON_SETUP        = STRID_SETUP,
    IDS_COMMON_EXT_SETUP    = STRID_SETUP,

    IDS_COMMON_LOCAL_OFF    = STRID_OFF,
    IDS_COMMON_LOCAL_ON     = STRID_ON,

    IDS_COMMON_CLOUD_OFF    = STRID_OFF,
    IDS_COMMON_CLOUD_ON     = STRID_ON,

    IDS_MODE                = STRID_MODE,
    IDS_MODE_MOVIE          = STRID_MOVIE,
    IDS_MODE_PHOTO          = STRID_PHOTO_PAPER,
    IDS_MODE_PLAYBACK       = STRID_PLAYBACK,

    // Setup Page
    IDS_DATE_TIME           = STRID_DATE_TIME,
    IDS_AUTO_POWER_OFF      = STRID_AUTO_OFF,
    IDS_BEEP                = STRID_BEEPER,
    IDS_SILENT              = STRID_BEEPER,
    IDS_LANGUAGE            = STRID_LANGUAGE,
    IDS_TV_MODE             = STRID_TV_MODE,
    IDS_FREQUENCY           = STRID_FREQUENCY,
    IDS_FORMAT              = STRID_FORMAT,
    IDS_DEFAULT             = STRID_DEFAULT_SETTING,
    IDS_OPENING_LOGO        = STRID_LOGO_DISPLAY,
    IDS_VERSION             = STRID_VERSION,
    IDS_WIFI                = STRID_WIFI,
    IDS_SCREEN_SAVE         = STRID_SCREEN_SAVE,
    IDS_FW_UPDATE           = STRID_FW_UPDATE,
    IDS_DELETE_ALL          = STRID_DELETEALL,
    IDS_PROTECT_ALL         = STRID_LOCKALL,
    IDS_UNPROTECT_ALL       = STRID_UNLOCKALL,
    IDS_SETUP               = STRID_SETUP,

    // Power Saving Options
    IDS_OFF                 = STRID_OFF,
    IDS_1MIN                = STRID_AUTO_OFF_1MIN,
    IDS_2MIN                = STRID_AUTO_OFF_2MIN,
    IDS_3MIN                = STRID_AUTO_OFF_3MIN,
    IDS_5MIN                = STRID_AUTO_OFF_5MIN,
    IDS_10MIN               = STRID_AUTO_OFF_10MIN,

    // Beep Options
    IDS_SILENT_ON            = STRID_OFF,
    IDS_SILENT_OFF           = STRID_ON,

    IDS_BEEP_OFF            = STRID_OFF,
    IDS_BEEP_ON             = STRID_ON,

    // Language Options
    IDS_LANG_EN             = STRID_LANG_EN,
    IDS_LANG_DE             = STRID_LANG_DE,
    IDS_LANG_FR             = STRID_LANG_FR,
    IDS_LANG_ES             = STRID_LANG_ES,
    IDS_LANG_IT             = STRID_LANG_IT,
    IDS_LANG_PO             = STRID_LANG_PO,
    IDS_LANG_SC             = STRID_LANG_SC,
    IDS_LANG_TC             = STRID_LANG_TC,
    IDS_LANG_JP             = STRID_LANG_JP,
    IDS_LANG_RU             = STRID_LANG_RU,
    IDS_LANG_DU             = STRID_NULL_,
    IDS_LANG_TU             = STRID_NULL_,
    IDS_LANG_PB             = STRID_NULL_,
    IDS_LANG_YI             = STRID_NULL_,
    IDS_LANG_PL             = STRID_NULL_,


    // TV Mode Options
    IDS_TV_NTSC             = STRID_TV_NTSC,
    IDS_TV_PAL              = STRID_TV_PAL,

    // Frequency Options
    IDS_50HZ                = STRID_50HZ,
    IDS_60HZ                = STRID_60HZ,

    // Wi-Fi Options
    IDS_WIFI_OFF            = STRID_OFF,
    IDS_WIFI_ON             = STRID_ON,

    // Opening Logo Display Options
    IDS_LOGO_OFF            = STRID_OFF,
    IDS_LOGO_ON             = STRID_ON,

    // Screen Save Option
    IDS_AUTO_OFF_OFF        = STRID_OFF,
    IDS_AUTO_OFF_1MIN       = STRID_AUTO_OFF_1MIN,
    IDS_AUTO_OFF_3MIN       = STRID_AUTO_OFF_3MIN,
    IDS_AUTO_OFF_5MIN       = STRID_AUTO_OFF_5MIN,
    IDS_AUTO_OFF_10MIN      = STRID_AUTO_OFF_10MIN,

    // Extra Setup Page
    IDS_MCTF                = STRID_MCTF,
    IDS_EDGE                = STRID_EDGE,
    IDS_NR                  = STRID_NR,
    IDS_IPP_ISO             = STRID_ISO,
    IDS_EXT_SETUP           = STRID_SETUP,

    // MCTF Options
    IDS_MCTF_OFF            = STRID_OFF,
    IDS_MCTF_ON             = STRID_ON,

    // EDGE Options
    IDS_EDGE_OFF            = STRID_OFF,
    IDS_EDGE_ON             = STRID_ON,

    // NR Options
    IDS_NR_OFF              = STRID_OFF,
    IDS_NR_ON               = STRID_ON,

    // IPP ISO Options
    IDS_IPP_ISO_AUTO        = STRID_AUTO,
    IDS_IPP_ISO_50          = STRID_ISO50,
    IDS_IPP_ISO_200         = STRID_ISO200,
    IDS_IPP_ISO_800         = STRID_ISO800,
    IDS_IPP_ISO_3200        = STRID_ISO3200,
    IDS_IPP_ISO_6400        = STRID_ISO6400,
    IDS_IPP_ISO_12800       = STRID_ISO12800,

    // Mode Page for touch
    IDS_MODETOUCH           = STRID_MODE,
    IDS_MODETOUCH_MOVIE     = STRID_VIDEO,
    IDS_MODETOUCH_PHOTO     = STRID_STILL,
    IDS_MODETOUCH_PLAYBACK  = STRID_PLAYBACK,
    IDS_MODETOUCH_SETUP     = STRID_SETUP,
};

// Menu Icon ID
enum _MENU_IDI
{
    // Photo Page
    IDI_SELFTIMER          = ICON_SELFTIMER_M,
    IDI_PHOTO_SIZE         = ICON_RESOLUTION_M,
    IDI_CONTINUE_SHOT      = ICON_BUST_M,
    IDI_QUALITY            = ICON_QUALITY_M,
    IDI_SHARPNESS          = ICON_SHARPNESS_M,
    IDI_WB                 = ICON_WB_M,
    IDI_ANTISHAKE          = ICON_SHAKE_ON_M,
    IDI_QUICK_REVIEW       = ICON_QUICK_REVIEW_M,
    IDI_DATE_STAMP         = ICON_DATE_PRINT_M,
    IDI_ISO                = ICON_ISO_M,
    IDI_EV                 = ICON_EV_M,
    IDI_FD                 = ICON_FACE_ON_M,
    IDI_FLASH              = ICON_FLASH_ON_M,
    IDI_RSC                = ICON_RSC_ON_M,
    IDI_HDR                = ICON_HDR_M,
    IDI_WDR                = ICON_MENU_WDR_M,
    IDI_METERING           = ICON_METERING_M,
    IDI_SCENE              = ICON_SCENE_MODE_M,
    IDI_DUAL_CAM           = ICON_DUALCAM_BOTH_M,
    IDI_PHOTO              = NULL,       // Photo menu enable icon
    IDIX_PHOTO             = NULL,      // Photo menu disable icon

    // Movie size options
    IDI_MOVIE_SIZE_1080FHD  = ICON_OK,
    IDI_MOVIE_SIZE_1080P    = ICON_OK,
    IDI_MOVIE_SIZE_1440P    = ICON_OK,
    IDI_MOVIE_SIZE_720P     = ICON_OK,
    IDI_MOVIE_SIZE_WVGA     = ICON_OK,
    IDI_MOVIE_SIZE_VGA      = ICON_OK,
    IDI_MOVIE_SIZE_QVGA     = ICON_OK,

    // Photo Size Options
    IDI_PHOTO_SIZE_15M      = ICON_OK,
    IDI_PHOTO_SIZE_14M      = ICON_OK,
    IDI_PHOTO_SIZE_12M      = ICON_OK,
    IDI_PHOTO_SIZE_10M      = ICON_OK,
    IDI_PHOTO_SIZE_8M       = ICON_OK,
    IDI_PHOTO_SIZE_7M       = ICON_OK,
    IDI_PHOTO_SIZE_7MHD     = ICON_OK,
    IDI_PHOTO_SIZE_5M       = ICON_OK,
    IDI_PHOTO_SIZE_4M       = ICON_OK,
    IDI_PHOTO_SIZE_3M       = ICON_OK,
    IDI_PHOTO_SIZE_2M       = ICON_OK,
    IDI_PHOTO_SIZE_1M       = ICON_OK,
    IDI_PHOTO_SIZE_2MHD     = ICON_OK,
    IDI_PHOTO_SIZE_VGA      = ICON_OK,

    // Movie Page
    IDI_MOVIE_SIZE          = ICON_RESOLUTION_M,
    IDI_MOVIE_EV            = ICON_EV_M,
    IDI_MOVIE_CYCLIC_REC    = ICON_CYCLIC_REC_M,
    IDI_MOVIE_HDR           = ICON_HDR_M,
    IDI_MOVIE_WDR           = ICON_MENU_WDR_M,
    IDI_MOVIE_MOTION_DET    = ICON_MOTION_DET_ON_M,
    IDI_MOVIE_AUDIO         = ICON_SOUND_REC_ON_M,
    IDI_MOVIE_DATEIMPRINT   = ICON_DATE_PRINT_M,
    IDI_MOVIE_RSC           = ICON_RSC_ON_M,
    IDI_MOVIE_TIMELAPSE_REC = ICON_TIME_LAPSE_ON_M,
    IDI_MOVIE_GSENSOR       = ICON_G_SENSOR_M,
    IDI_MOVIE_IR_CUT        = ICON_IRCUT_ON_M,
    IDI_MOVIE_PTZ           = ICON_CAM_PTZ_ON_M,
    IDI_MOVIE_SENSOR_ROTATE = ICON_VIDEO_FLIP_ON_M,
    IDI_MOVIE_DUAL_REC      = ICON_DUALCAM_BOTH_M,
    IDI_MOVIE_URGENT_PROTECT_AUTO    = ICON_URGENT_PROTECT_AUTO_M,
    IDI_MOVIE_URGENT_PROTECT_MANUAL  = ICON_URGENT_PROTECT_MANUAL_M,
    IDI_MOVIE_PIM           = ICON_PIM_ON_M,
    IDI_MOVIE_FCW           = ICON_FCW_ON_M,
    IDI_MOVIE_LDWS          = ICON_LDWS_ON_M,
    IDI_MOVIE_DDD           = ICON_DDD_ON_M,
    IDI_MOVIE_ADAS_CAL      = ICON_ADAS_CAL_M,
    IDI_MOVIE_QUALITY       = ICON_QUALITY,
    IDI_MOVIE_WB            = ICON_WB_AUTO,
    IDI_MOVIE               = NULL,       // Movie menu enable icon
    IDIX_MOVIE              = NULL,      // Movie menu disable icon
    IDI_MOVIE_DUAL_CAM      = ICON_DUALCAM_BOTH_M,

    // Playback Page
    IDI_PROTECT             = ICON_PROTECT_M,
    IDI_VOLUME              = ICON_VOICE_M,
    IDI_PLAYBACK            = NULL,    // Playback menu enable icon
    IDIX_PLAYBACK           = NULL,   // Playback menu disable icon

    // Common Page
    IDI_COMMON              = ICON_MENU_MODECHANG_M,
    IDI_COMMON_MODE         = ICON_MENU_MODECHANG_M,                // no menu icon
    IDI_COMMON_MENU         = ICON_MODE_VIDEO_M,
    IDI_COMMON_LOCAL        = ICON_WIFI_OFF_M,
    IDI_COMMON_CLOUD        = ICON_CLOUD_OFF_M,
    IDI_COMMON_SETUP        = ICON_MENU_SETUP_ON_M,
    IDI_COMMON_EXT_SETUP    = ICON_MENU_SETUP_ON_M,
    IDIX_COMMON             = ICON_MODE_VIDEO,

    // Mode page
    IDI_MODE                = ICON_MODE_VIDEO_M,
    IDI_MODE_MOVIE          = ICON_MODE_VIDEO_M,
    IDI_MODE_PHOTO          = ICON_MODE_CAPTURE_M,
    IDI_MODE_PLAYBACK       = ICON_MODE_PLAYBACK_M,
    IDIX_MODE               = ICON_MODE_VIDEO,
    // Setup Page
    IDI_DATE_TIME           = ICON_DATETIME_SETUP_M,
    IDI_AUTO_POWER_OFF      = ICON_POWER_OFF_M,
    IDI_BEEP                = ICON_VOICE_M,
    IDI_LANGUAGE            = ICON_LANGUAGE_M,
    IDI_FREQUENCY           = ICON_HZ_M,
    IDI_TV_MODE             = ICON_TV_MODE_M,
    IDI_FORMAT              = ICON_FORMAT_M,
    IDI_DEFAULT             = ICON_DEFAULT_M,
    IDI_WIFI                = ICON_WIFI_ON_M,
    IDI_VERSION             = ICON_VERSION_M,
    IDI_SCREEN_SAVE         = ICON_SCREENSAVER_M,
    IDI_FW_UPDATE           = ICON_FIRMWARE_UPDATE_M,
    IDI_DELETE_ALL          = ICON_DELETE_M,
    IDI_PROTECT_ALL         = ICON_LOCK_M,
    IDI_UNPROTECT_ALL       = ICON_LOCK_UNLOCK_M,
    IDI_SETUP               = NULL,
    IDIX_SETUP              = NULL,

    // Extra Setup Page
    IDI_MCTF                = ICON_MENU_SETUP_ON_M,
    IDI_EDGE                = ICON_MENU_SETUP_ON_M,
    IDI_NR                  = ICON_MENU_SETUP_ON_M,
    IDI_IPP_ISO             = ICON_MENU_SETUP_ON_M,
    IDI_EXT_SETUP           = NULL,
    IDIX_EXT_SETUP          = NULL,

#if 0
    // Language Options
    IDI_LANG_EN             = ICON_OK,
    IDI_LANG_FR             = ICON_OK,
    IDI_LANG_ES             = ICON_OK,
    IDI_LANG_PO             = ICON_OK,
    IDI_LANG_DE             = ICON_OK,
    IDI_LANG_DU             = ICON_OK,
    IDI_LANG_IT             = ICON_OK,
    IDI_LANG_RU             = ICON_OK,
    IDI_LANG_TU             = ICON_OK,
    IDI_LANG_JP             = ICON_OK,
    IDI_LANG_TC             = ICON_OK,
    IDI_LANG_SC             = ICON_OK,
    IDI_LANG_PB             = ICON_OK,
    IDI_LANG_YI             = ICON_OK,
    IDI_LANG_PL             = ICON_OK
#endif

    // Mode Page for touch
    IDI_MODETOUCH           = ICON_MODE_VIDEO_M,
    IDI_MODETOUCH_MOVIE     = ICON_MODE_VIDEO_M,
    IDI_MODETOUCH_PHOTO     = ICON_MODE_CAPTURE_M,
    IDI_MODETOUCH_PLAYBACK  = ICON_MODE_PLAYBACK_M,
    IDI_MODETOUCH_SETUP     = ICON_DEFAULT_M,
    IDIX_MODETOUCH          = ICON_MODE_VIDEO,
};
// Menu Flag ID
enum _MENU_IDF
{
    // Menu common page
    IDF_COMMON_MODE                      = FL_COMMON_MODE,
    IDF_COMMON_MENU                      = FL_COMMON_MENU,
    IDF_COMMON_LOCAL                     = FL_COMMON_LOCAL,
    IDF_COMMON_CLOUD                     = FL_COMMON_CLOUD,
    IDF_COMMON_SETUP                     = FL_COMMON_SETUP,
    IDF_COMMON_EXT_SETUP                 = FL_COMMON_EXT_SETUP,
    // Movie Page
    IDF_MOVIE_SIZE                       = FL_MOVIE_SIZE_MENU, //only keep in menu
    IDF_MOVIE_CYCLIC_REC                 = FL_MOVIE_CYCLIC_REC,
    IDF_MOVIE_HDR                        = FL_MOVIE_HDR_MENU,
    IDF_MOVIE_WDR                        = FL_MOVIE_WDR_MENU,
    IDF_MOVIE_MOTION_DET                 = FL_MOVIE_MOTION_DET,
    IDF_MOVIE_AUDIO                      = FL_MOVIE_AUDIO,
    IDF_MOVIE_DATEIMPRINT                = FL_MOVIE_DATEIMPRINT,
    IDF_MOVIE_RSC                        = FL_MovieRSCIndex_MENU,
    IDF_MOVIE_GSENSOR                    = FL_GSENSOR,
    //#NT#2016/08/15#KCHong#[0106941] -begin
    //#NT#Fixed FCW/LDWS/Timelapse cannot reset to default
    IDF_MOVIE_TIMELAPSE_REC              = FL_MOVIE_TIMELAPSE_REC_MENU,
    //#NT#2016/08/15#KCHong -end
    IDF_MOVIE_IR_CUT                     = FL_MOVIE_IR_CUT,
    IDF_MOVIE_SENSOR_ROTATE              = FL_MOVIE_SENSOR_ROTATE,
    IDF_MOVIE_PTZ                        = FL_MOVIE_PTZ,
    IDF_MOVIE_URGENT_PROTECT_AUTO        = FL_MOVIE_URGENT_PROTECT_AUTO,
    IDF_MOVIE_URGENT_PROTECT_MANUAL      = FL_MOVIE_URGENT_PROTECT_MANUAL,
    IDF_MOVIE_PIM                        = FL_MOVIE_PIM,
    //#NT#2016/08/15#KCHong#[0106941] -begin
    //#NT#Fixed FCW/LDWS/Timelapse cannot reset to default
    IDF_MOVIE_LDWS                       = FL_MOVIE_LDWS_MENU,
    IDF_MOVIE_FCW                        = FL_MOVIE_FCW_MENU,
    //#NT#2016/08/15#KCHong -end
    IDF_MOVIE_DDD                        = FL_MOVIE_DDD,
    IDF_MOVIE_ADAS_CAL                   = FL_MOVIE_ADAS_CAL,
    IDF_MOVIE_DUAL_CAM                   = FL_DUAL_CAM_MENU,
    // Photo Page
    IDF_SELFTIMER                        = FL_SELFTIMER,
    IDF_PHOTO_SIZE                       = FL_PHOTO_SIZE,
    IDF_CONTINUE_SHOT                    = FL_CONTINUE_SHOT,
    IDF_QUALITY                          = FL_QUALITY,
    IDF_SHARPNESS                        = FL_SHARPNESS,
    IDF_WB                               = FL_WB,
    IDF_ANTISHAKE                        = FL_ANTISHAKE,
    IDF_QUICK_REVIEW                     = FL_QUICK_REVIEW,
    IDF_DATE_STAMP                       = FL_DATE_STAMP,
    IDF_ISO                              = FL_ISO,
    IDF_EV                               = FL_EV,
    IDF_FD                               = FL_FD,
    IDF_FLASH                            = FL_FLASH_MODE,
    //#NT#2016/08/19#Lincy Lin#[0106935] -begin
    //#NT# Support change WDR, SHDR, RSC setting will change mode after exit menu
    IDF_RSC                              = FL_RSC_MENU,
    IDF_HDR                              = FL_SHDR_MENU,
    IDF_WDR                              = FL_WDR_MENU,
    //#NT#2016/08/19#Lincy Lin -end
    IDF_METERING                         = FL_METERING,
    IDF_SCENE                            = FL_SCENE,
    IDF_DUAL_CAM                         = FL_DUAL_CAM_MENU,
    // Playback page
    IDF_VOLUME                           = FL_MovieAudioPlayIndex,
    // Setup Page
    IDF_AUTO_POWER_OFF                   = FL_AUTO_POWER_OFF,
    IDF_BEEP                             = FL_BEEP,
    IDF_LANGUAGE                         = FL_LANGUAGE,
    IDF_FREQUENCY                        = FL_FREQUENCY,
    IDF_TV_MODE                          = FL_TV_MODE,
    IDF_SCREEN_SAVE                      = FL_LCD_OFF,
    // Extra Setup Page
    IDF_MCTF                             = FL_MovieMCTFIndex,
    IDF_EDGE                             = FL_EDGE,
    IDF_NR                               = FL_NR,
    IDF_IPP_ISO                          = FL_IPP_ISO,

};

#endif // __MENUID_H

