//This source code is generated by UI Designer Studio.

#ifndef UIFLOWWNDMOVIE_H
#define UIFLOWWNDMOVIE_H

#include "UIFramework.h"

extern VControl UIFlowWndMovieCtrl;//Panel type 

extern VControl UIFlowWndMovie_Panel_Normal_DisplayCtrl;//Panel type 

extern VControl UIFlowWndMovie_Static_cameraCtrl;//Static type 

extern VControl UIFlowWndMovie_Status_CyclicRecCtrl;//Status type 

typedef enum
{
UIFlowWndMovie_Status_CyclicRec_ICONID_NULL,
UIFlowWndMovie_Status_CyclicRec_ICON_CYCLIC_REC_3MIN,
UIFlowWndMovie_Status_CyclicRec_ICON_CYCLIC_REC_5MIN,
UIFlowWndMovie_Status_CyclicRec_ICON_CYCLIC_REC_10MIN,
UIFlowWndMovie_Status_CyclicRec_MAX
}UIFlowWndMovie_Status_CyclicRec_Setting;

extern VControl UIFlowWndMovie_Status_MotionDetCtrl;//Status type 

typedef enum
{
UIFlowWndMovie_Status_MotionDet_ICONID_NULL,
UIFlowWndMovie_Status_MotionDet_ICON_MOTION_DET_ON,
UIFlowWndMovie_Status_MotionDet_MAX
}UIFlowWndMovie_Status_MotionDet_Setting;

extern VControl UIFlowWndMovie_Status_batteryCtrl;//Status type 

typedef enum
{
UIFlowWndMovie_Status_battery_ICON_BATTERY_FULL,
UIFlowWndMovie_Status_battery_ICON_BATTERY_MED,
UIFlowWndMovie_Status_battery_ICON_BATTERY_LOW,
UIFlowWndMovie_Status_battery_ICON_BATTERY_EMPTY,
UIFlowWndMovie_Status_battery_ICON_BATTERY_ZERO,
UIFlowWndMovie_Status_battery_ICON_BATTERY_CHARGE,
UIFlowWndMovie_Status_battery_MAX
}UIFlowWndMovie_Status_battery_Setting;

extern VControl UIFlowWndMovie_YMD_StaticCtrl;//Static type 

extern VControl UIFlowWndMovie_HMS_StaticCtrl;//Static type 

extern VControl UIFlowWndMovie_Status_RECCtrl;//Status type 

typedef enum
{
UIFlowWndMovie_Status_REC_ICON_REC_TRANSPAENT,
UIFlowWndMovie_Status_REC_ICON_REC_ELLIPSE,
UIFlowWndMovie_Status_REC_MAX
}UIFlowWndMovie_Status_REC_Setting;

extern VControl UIFlowWndMovie_Status_StorageCtrl;//Status type 

typedef enum
{
UIFlowWndMovie_Status_Storage_ICON_INTERNAL_FLASH,
UIFlowWndMovie_Status_Storage_ICON_SD_CARD,
UIFlowWndMovie_Status_Storage_ICON_SD_LOCK,
UIFlowWndMovie_Status_Storage_MAX
}UIFlowWndMovie_Status_Storage_Setting;

extern VControl UIFlowWndMovie_Static_timeCtrl;//Static type 

extern VControl UIFlowWndMovie_Static_maxtimeCtrl;//Static type 

extern VControl UIFlowWndMovie_Static_resolutionCtrl;//Static type 

extern VControl UIFlowWndMovie_Zoom_StaticCtrl;//Static type 

extern VControl UIFlowWndMovie_StatusICN_EVCtrl;//Status type 

typedef enum
{
UIFlowWndMovie_StatusICN_EV_ICON_EV_P2P0,
UIFlowWndMovie_StatusICN_EV_ICON_EV_P1P6,
UIFlowWndMovie_StatusICN_EV_ICON_EV_P1P3,
UIFlowWndMovie_StatusICN_EV_ICON_EV_P1P0,
UIFlowWndMovie_StatusICN_EV_ICON_EV_P0P6,
UIFlowWndMovie_StatusICN_EV_ICON_EV_P0P3,
UIFlowWndMovie_StatusICN_EV_ICON_EV_P0P0,
UIFlowWndMovie_StatusICN_EV_ICON_EV_M0P3,
UIFlowWndMovie_StatusICN_EV_ICON_EV_M0P6,
UIFlowWndMovie_StatusICN_EV_ICON_EV_M1P0,
UIFlowWndMovie_StatusICN_EV_ICON_EV_M1P3,
UIFlowWndMovie_StatusICN_EV_ICON_EV_M1P6,
UIFlowWndMovie_StatusICN_EV_ICON_EV_M2P0,
UIFlowWndMovie_StatusICN_EV_MAX
}UIFlowWndMovie_StatusICN_EV_Setting;

extern VControl UIFlowWndMovie_Status_HDRCtrl;//Status type 

typedef enum
{
UIFlowWndMovie_Status_HDR_ICONID_NULL,
UIFlowWndMovie_Status_HDR_ICON_HDR,
UIFlowWndMovie_Status_HDR_MAX
}UIFlowWndMovie_Status_HDR_Setting;

extern VControl UIFlowWndMovie_Status_WIFICtrl;//Status type 

typedef enum
{
UIFlowWndMovie_Status_WIFI_ICON_WIFI_OFF,
UIFlowWndMovie_Status_WIFI_ICON_WIFI_ON,
UIFlowWndMovie_Status_WIFI_ICON_CLOUD_ON,
UIFlowWndMovie_Status_WIFI_MAX
}UIFlowWndMovie_Status_WIFI_Setting;

extern VControl UIFlowWndMovie_StaticIcon_PIMCCtrl;//Static type 

extern VControl UIFlowWndMovie_Status_WDRCtrl;//Status type 

typedef enum
{
UIFlowWndMovie_Status_WDR_ICON_HDR_OFF,
UIFlowWndMovie_Status_WDR_ICON_HDR,
UIFlowWndMovie_Status_WDR_MAX
}UIFlowWndMovie_Status_WDR_Setting;

extern VControl UIFlowWndMovie_Status_G_SensorCtrl;//Status type 

typedef enum
{
UIFlowWndMovie_Status_G_Sensor_ICON_G_SENSOR_OFF,
UIFlowWndMovie_Status_G_Sensor_ICON_G_SENSOR_LOW,
UIFlowWndMovie_Status_G_Sensor_ICON_G_SENSOR_MID,
UIFlowWndMovie_Status_G_Sensor_ICON_G_SENSOR_HIGH,
UIFlowWndMovie_Status_G_Sensor_MAX
}UIFlowWndMovie_Status_G_Sensor_Setting;

extern VControl UIFlowWndMovie_Status_File_LockCtrl;//Static type 

extern VControl UIFlowWndMovie_Status_Urgent_protect_AutoCtrl;//Status type 

typedef enum
{
UIFlowWndMovie_Status_Urgent_protect_Auto_ICONID_NULL,
UIFlowWndMovie_Status_Urgent_protect_Auto_ICON_URGENT_PROTECT_AUTO,
UIFlowWndMovie_Status_Urgent_protect_Auto_MAX
}UIFlowWndMovie_Status_Urgent_protect_Auto_Setting;

extern VControl UIFlowWndMovie_Status_Urgent_protect_ManualCtrl;//Status type 

typedef enum
{
UIFlowWndMovie_Status_Urgent_protect_Manual_ICONID_NULL,
UIFlowWndMovie_Status_Urgent_protect_Manual_ICON_URGENT_PROTECT_MANUAL,
UIFlowWndMovie_Status_Urgent_protect_Manual_MAX
}UIFlowWndMovie_Status_Urgent_protect_Manual_Setting;

extern VControl UIFlowWndMovie_StatusICN_AudioCtrl;//Status type 

typedef enum
{
UIFlowWndMovie_StatusICN_Audio_ICON_SOUND_REC_OFF,
UIFlowWndMovie_StatusICN_Audio_ICON_SOUND_REC_ON,
UIFlowWndMovie_StatusICN_Audio_MAX
}UIFlowWndMovie_StatusICN_Audio_Setting;

extern VControl UIFlowWndMovie_Status_GPSCtrl;//Status type 

typedef enum
{
UIFlowWndMovie_Status_GPS_ICON_GPS_OFF,
UIFlowWndMovie_Status_GPS_ICON_GPS_ON_0,
UIFlowWndMovie_Status_GPS_ICON_GPS_ON_1,
UIFlowWndMovie_Status_GPS_ICON_GPS_ON_2,
UIFlowWndMovie_Status_GPS_ICON_GPS_OK,
UIFlowWndMovie_Status_GPS_MAX
}UIFlowWndMovie_Status_GPS_Setting;

extern VControl UIFlowWndMovie_Status_TimeLapseCtrl;//Status type 

typedef enum
{
UIFlowWndMovie_Status_TimeLapse_ICON_TIME_LAPSE_OFF,
UIFlowWndMovie_Status_TimeLapse_ICON_TIME_LAPSE_ON,
UIFlowWndMovie_Status_TimeLapse_MAX
}UIFlowWndMovie_Status_TimeLapse_Setting;

extern VControl UIFlowWndMovie_Status_ADAS_StatusCtrl;//Status type 

typedef enum
{
UIFlowWndMovie_Status_ADAS_Status_ICON_ADAS_OFF,
UIFlowWndMovie_Status_ADAS_Status_ICON_ADAS_ON,
UIFlowWndMovie_Status_ADAS_Status_MAX
}UIFlowWndMovie_Status_ADAS_Status_Setting;

extern VControl UIFlowWndMovie_Status_FCW_StatusCtrl;//Status type 

typedef enum
{
UIFlowWndMovie_Status_FCW_Status_ICON_FCW_OFF,
UIFlowWndMovie_Status_FCW_Status_ICON_FCW_ON,
UIFlowWndMovie_Status_FCW_Status_MAX
}UIFlowWndMovie_Status_FCW_Status_Setting;

extern VControl UIFlowWndMovie_Status_LDWS_StatusCtrl;//Status type 

typedef enum
{
UIFlowWndMovie_Status_LDWS_Status_ICON_LDWS_OFF,
UIFlowWndMovie_Status_LDWS_Status_ICON_LDWS_ON,
UIFlowWndMovie_Status_LDWS_Status_MAX
}UIFlowWndMovie_Status_LDWS_Status_Setting;

extern VControl UIFlowWndMovie_Status_ParkingModeCtrl;//Status type 

typedef enum
{
UIFlowWndMovie_Status_ParkingMode_ICONID_NULL,
UIFlowWndMovie_Status_ParkingMode_ICON_PARKING_MODE,
UIFlowWndMovie_Status_ParkingMode_MAX
}UIFlowWndMovie_Status_ParkingMode_Setting;

extern VControl UIFlowWndMovie_Status_Touch_AudioSwitchCtrl;//Status type 

typedef enum
{
UIFlowWndMovie_Status_Touch_AudioSwitch_ICON_SOUND_RELEASE,
UIFlowWndMovie_Status_Touch_AudioSwitch_ICON_SOUND_PRESS,
UIFlowWndMovie_Status_Touch_AudioSwitch_MAX
}UIFlowWndMovie_Status_Touch_AudioSwitch_Setting;

extern VControl UIFlowWndMovie_Status_Touch_LockFileCtrl;//Status type 

typedef enum
{
UIFlowWndMovie_Status_Touch_LockFile_ICON_LOCK_RELEASE,
UIFlowWndMovie_Status_Touch_LockFile_ICON_LOCK_PRESS,
UIFlowWndMovie_Status_Touch_LockFile_MAX
}UIFlowWndMovie_Status_Touch_LockFile_Setting;

extern VControl UIFlowWndMovie_Status_Main_PBCtrl;//Status type 

typedef enum
{
UIFlowWndMovie_Status_Main_PB_ICON_PLAY_MODE,
UIFlowWndMovie_Status_Main_PB_ICON_PLAY_MODE_PRESSED,
UIFlowWndMovie_Status_Main_PB_MAX
}UIFlowWndMovie_Status_Main_PB_Setting;

extern VControl UIFlowWndMovie_Status_Touch_RECCtrl;//Status type 

typedef enum
{
UIFlowWndMovie_Status_Touch_REC_ICON_TP_REC,
UIFlowWndMovie_Status_Touch_REC_ICON_TP_REC_PRESSED,
UIFlowWndMovie_Status_Touch_REC_MAX
}UIFlowWndMovie_Status_Touch_REC_Setting;

extern VControl UIFlowWndMovie_Status_Touch_RERTURNCtrl;//Status type 

typedef enum
{
UIFlowWndMovie_Status_Touch_RERTURN_ICON_TP_RETURN,
UIFlowWndMovie_Status_Touch_RERTURN_ICON_TP_RETURN_PRESSED,
UIFlowWndMovie_Status_Touch_RERTURN_MAX
}UIFlowWndMovie_Status_Touch_RERTURN_Setting;

extern VControl UIFlowWndMovie_Status_Touch_SnapeShotCtrl;//Status type 

typedef enum
{
UIFlowWndMovie_Status_Touch_SnapeShot_ICON_SHOT_RELEASE,
UIFlowWndMovie_Status_Touch_SnapeShot_ICON_SHOT_PRESS,
UIFlowWndMovie_Status_Touch_SnapeShot_MAX
}UIFlowWndMovie_Status_Touch_SnapeShot_Setting;

extern VControl UIFlowWndMovie_Status_Touch_Change_DisplayModeCtrl;//Status type 

typedef enum
{
UIFlowWndMovie_Status_Touch_Change_DisplayMode_ICON_DISPLAY_CHANGE_RELEASE,
UIFlowWndMovie_Status_Touch_Change_DisplayMode_ICON_DISPLAY_CHANGE_PRESS,
UIFlowWndMovie_Status_Touch_Change_DisplayMode_MAX
}UIFlowWndMovie_Status_Touch_Change_DisplayMode_Setting;

extern VControl UIFlowWndMovie_Status_Touch_SETUPCtrl;//Status type 
typedef enum
{
UIFlowWndMovie_Status_Touch_SETUP_ICON_TP_SETUP,
UIFlowWndMovie_Status_Touch_SETUP_ICON_TP_SETUP_PRESSED,
UIFlowWndMovie_Status_Touch_SETUP_MAX
}UIFlowWndMovie_Status_Touch_SETUP_Setting;
extern VControl UIFlowWndMovie_Static_ReverseCtrl;//Static type 

extern VControl UIFlowWndMovie_ADAS_DrawingLineCtrl;//Panel type 

extern VControl UIFlowWndMovie_ALG_DrawCtrl;//Panel type 

extern VControl UIFlowWndMovie_ADAS_LineCtrl;//Panel type 

extern VControl UIFlowWndMovie_ADAS_Alert_DisplayCtrl;//Panel type 

extern VControl UIFlowWndMovie_StatusICN_LDWS_AlertCtrl;//Status type 

typedef enum
{
UIFlowWndMovie_StatusICN_LDWS_Alert_ICON_LDWS_LEFT_ALERT,
UIFlowWndMovie_StatusICN_LDWS_Alert_ICON_LDWS_RIGHT_ALERT,
UIFlowWndMovie_StatusICN_LDWS_Alert_MAX
}UIFlowWndMovie_StatusICN_LDWS_Alert_Setting;

extern VControl UIFlowWndMovie_StatusICN_FCWS_AlertCtrl;//Status type 

typedef enum
{
UIFlowWndMovie_StatusICN_FCWS_Alert_ICON_FCW_FAR_ALERT,
UIFlowWndMovie_StatusICN_FCWS_Alert_ICON_FCW_NEAR_ALERT,
UIFlowWndMovie_StatusICN_FCWS_Alert_MAX
}UIFlowWndMovie_StatusICN_FCWS_Alert_Setting;

extern VControl UIFlowWndMovie_StatusICN_SNG_AlertCtrl;//Status type 

typedef enum
{
UIFlowWndMovie_StatusICN_SNG_Alert_ICON_SNG_ALERT,
UIFlowWndMovie_StatusICN_SNG_Alert_MAX
}UIFlowWndMovie_StatusICN_SNG_Alert_Setting;

extern VControl UIFlowWndMovie_OT_DrawRectCtrl;//Panel type 

extern VControl UIFlowWndMovie_TD_DrawRectCtrl;//Panel type 

extern VControl UIFlowWndMovie_Static_SpeedCtrl;//Static type 

extern VControl UIFlowWndMovie_Static_DistanceCtrl;//Static type 

//---------------------UIFlowWndMovieCtrl User Export---------------------------
#endif
