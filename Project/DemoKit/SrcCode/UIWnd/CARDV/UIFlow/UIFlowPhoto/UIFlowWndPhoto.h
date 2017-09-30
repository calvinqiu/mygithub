//This source code is generated by UI Designer Studio.

#ifndef UIFLOWWNDPHOTO_H
#define UIFLOWWNDPHOTO_H

#include "UIFramework.h"

extern VControl UIFlowWndPhotoCtrl;//Panel type

extern VControl UIFlowWndPhoto_StaticICN_DSCModeCtrl;//Static type

extern VControl UIFlowWndPhoto_StatusICN_SelftimerCtrl;//Status type

typedef enum
{
UIFlowWndPhoto_StatusICN_Selftimer_ICONID_NULL,
UIFlowWndPhoto_StatusICN_Selftimer_ICON_SELFTIMER_2SEC,
UIFlowWndPhoto_StatusICN_Selftimer_ICON_SELFTIMER_5SEC,
UIFlowWndPhoto_StatusICN_Selftimer_ICON_SELFTIMER_10SEC,
UIFlowWndPhoto_StatusICN_Selftimer_MAX
}UIFlowWndPhoto_StatusICN_Selftimer_Setting;

extern VControl UIFlowWndPhoto_StaticTXT_FreePicCtrl;//Static type

extern VControl UIFlowWndPhoto_StatusICN_EVCtrl;//Status type

typedef enum
{
UIFlowWndPhoto_StatusICN_EV_ICON_EV_P2P0,
UIFlowWndPhoto_StatusICN_EV_ICON_EV_P1P6,
UIFlowWndPhoto_StatusICN_EV_ICON_EV_P1P3,
UIFlowWndPhoto_StatusICN_EV_ICON_EV_P1P0,
UIFlowWndPhoto_StatusICN_EV_ICON_EV_P0P6,
UIFlowWndPhoto_StatusICN_EV_ICON_EV_P0P3,
UIFlowWndPhoto_StatusICN_EV_ICON_EV_P0P0,
UIFlowWndPhoto_StatusICN_EV_ICON_EV_M0P3,
UIFlowWndPhoto_StatusICN_EV_ICON_EV_M0P6,
UIFlowWndPhoto_StatusICN_EV_ICON_EV_M1P0,
UIFlowWndPhoto_StatusICN_EV_ICON_EV_M1P3,
UIFlowWndPhoto_StatusICN_EV_ICON_EV_M1P6,
UIFlowWndPhoto_StatusICN_EV_ICON_EV_M2P0,
UIFlowWndPhoto_StatusICN_EV_MAX
}UIFlowWndPhoto_StatusICN_EV_Setting;

extern VControl UIFlowWndPhoto_StatusICN_ISOCtrl;//Status type

typedef enum
{
UIFlowWndPhoto_StatusICN_ISO_ICON_ISO_AUTO,
UIFlowWndPhoto_StatusICN_ISO_ICON_ISO_100,
UIFlowWndPhoto_StatusICN_ISO_ICON_ISO_200,
UIFlowWndPhoto_StatusICN_ISO_ICON_ISO_400,
UIFlowWndPhoto_StatusICN_ISO_ICON_ISO_800,
UIFlowWndPhoto_StatusICN_ISO_ICON_ISO_1600,
UIFlowWndPhoto_StatusICN_ISO_MAX
}UIFlowWndPhoto_StatusICN_ISO_Setting;

extern VControl UIFlowWndPhoto_StaticTXT_SizeCtrl;//Static type

extern VControl UIFlowWndPhoto_StatusICN_AntiShakingCtrl;//Status type

typedef enum
{
UIFlowWndPhoto_StatusICN_AntiShaking_ICON_SHAKE_OFF,
UIFlowWndPhoto_StatusICN_AntiShaking_ICON_SHAKE_ON,
UIFlowWndPhoto_StatusICN_AntiShaking_MAX
}UIFlowWndPhoto_StatusICN_AntiShaking_Setting;

extern VControl UIFlowWndPhoto_StatusICN_WBCtrl;//Status type

typedef enum
{
UIFlowWndPhoto_StatusICN_WB_ICON_WB_AUTO,
UIFlowWndPhoto_StatusICN_WB_ICON_WB_DAYLIGHT,
UIFlowWndPhoto_StatusICN_WB_ICON_WB_CLOUDY,
UIFlowWndPhoto_StatusICN_WB_ICON_WB_TUNGSTEN,
UIFlowWndPhoto_StatusICN_WB_ICON_WB_FLUORESCENT,
UIFlowWndPhoto_StatusICN_WB_MAX
}UIFlowWndPhoto_StatusICN_WB_Setting;

extern VControl UIFlowWndPhoto_StatusICN_FDCtrl;//Status type

typedef enum
{
UIFlowWndPhoto_StatusICN_FD_ICON_FACE_OFF,
UIFlowWndPhoto_StatusICN_FD_ICON_FACE_ON,
UIFlowWndPhoto_StatusICN_FD_MAX
}UIFlowWndPhoto_StatusICN_FD_Setting;

extern VControl UIFlowWndPhoto_StatusICN_QualityCtrl;//Status type

typedef enum
{
UIFlowWndPhoto_StatusICN_Quality_ICON_QUALITY_FINE,
UIFlowWndPhoto_StatusICN_Quality_ICON_QUALITY_NORMAL,
UIFlowWndPhoto_StatusICN_Quality_ICON_QUALITY_BASIC,
UIFlowWndPhoto_StatusICN_Quality_MAX
}UIFlowWndPhoto_StatusICN_Quality_Setting;

extern VControl UIFlowWndPhoto_StatusICN_StorageCtrl;//Status type

typedef enum
{
UIFlowWndPhoto_StatusICN_Storage_ICON_INTERNAL_FLASH,
UIFlowWndPhoto_StatusICN_Storage_ICON_SD_CARD,
UIFlowWndPhoto_StatusICN_Storage_ICON_SD_LOCK,
UIFlowWndPhoto_StatusICN_Storage_MAX
}UIFlowWndPhoto_StatusICN_Storage_Setting;

extern VControl UIFlowWndPhoto_StaticTXT_DZoomCtrl;//Static type

extern VControl UIFlowWndPhoto_StatusICN_BatteryCtrl;//Status type

typedef enum
{
UIFlowWndPhoto_StatusICN_Battery_ICON_BATTERY_FULL,
UIFlowWndPhoto_StatusICN_Battery_ICON_BATTERY_MED,
UIFlowWndPhoto_StatusICN_Battery_ICON_BATTERY_LOW,
UIFlowWndPhoto_StatusICN_Battery_ICON_BATTERY_EMPTY,
UIFlowWndPhoto_StatusICN_Battery_ICON_BATTERY_ZERO,
UIFlowWndPhoto_StatusICN_Battery_ICON_BATTERY_CHARGE,
UIFlowWndPhoto_StatusICN_Battery_MAX
}UIFlowWndPhoto_StatusICN_Battery_Setting;

extern VControl UIFlowWndPhoto_StaticTXT_SelftimerCntCtrl;//Static type

extern VControl UIFlowWndPhoto_PNL_HistogramCtrl;//Panel type

extern VControl UIFlowWndPhoto_PNL_FDFrameCtrl;//Panel type

extern VControl UIFlowWndPhoto_StatusICN_ContinueShotCtrl;//Status type 

typedef enum
{
UIFlowWndPhoto_StatusICN_ContinueShot_ICON_BUST_OFF,

UIFlowWndPhoto_StatusICN_ContinueShot_ICON_BUST_CONTINUE,
UIFlowWndPhoto_StatusICN_ContinueShot_MAX
}UIFlowWndPhoto_StatusICN_ContinueShot_Setting;

extern VControl UIFlowWndPhoto_StatusICN_HDRCtrl;//Status type 

typedef enum
{
UIFlowWndPhoto_StatusICN_HDR_ICON_HDR_OFF,
UIFlowWndPhoto_StatusICN_HDR_ICON_HDR,
UIFlowWndPhoto_StatusICN_HDR_MAX
}UIFlowWndPhoto_StatusICN_HDR_Setting;

//---------------------UIFlowWndPhotoCtrl User Export---------------------------
#endif
