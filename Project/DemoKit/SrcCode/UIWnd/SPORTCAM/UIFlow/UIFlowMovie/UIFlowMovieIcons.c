
#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIFlow.h"
#include "JpgEnc.h"
#include "Exif.h"
#include "Utility.h"
#include "Audio.h"
#include "rtc.h"
//#include "PhotoTsk.h"
#if defined(YQCONFIG_ANDROID_SYSTEM_SUPPORT)
#include "ImageUnit_ImagePipe.h"/*ZMD*/
#include "DxSensor.h"  /* add begin by ZMD, 2016-09-02,*/
#endif

static CHAR    g_RecMaxTimeStr[20] = {0};
static UINT32  g_RecMaxTime=0;
static CHAR    g_RecCurrTimeStr[20] = {0};
static UINT32  g_RecCurrTime=0;
static CHAR    date_str[20] = {0};
static CHAR    time_str[20] = {0};
#if defined(YQCONFIG_UART_TO_MTK_SUPPORT) && defined(YQCONFIG_ANDROID_SYSTEM_SUPPORT)
static CHAR    g_SpeedStr[20] = {0};
static CHAR    g_DistanceStr[20] = {0};
extern UINT32 uiGpsSpeed;
extern UINT32 g_CurSpeed;
#if _ADAS_FUNC_
extern UINT32 g_uiAdasAlertDistance;
#endif
#endif
#if defined(YQCONFIG_ANDROID_SYSTEM_SUPPORT)
extern BOOL gbGsensorTrig;
extern BOOL gbMovieTouchUIDisplayEn;
#endif

//MOVIE_SIZE_TAG
static CHAR*    resolution_Buf[MOVIE_SIZE_ID_MAX]=
{
    [MOVIE_SIZE_FRONT_2880x2160P24] = "UHD P24",
    [MOVIE_SIZE_FRONT_2560x1440P30] = "QHD P30",
    [MOVIE_SIZE_FRONT_2304x1296P30] = "3MHD P30",
    [MOVIE_SIZE_FRONT_1920x1080P96] = "FHD P96",
    [MOVIE_SIZE_FRONT_1920x1080P60] = "FHD P60",
    [MOVIE_SIZE_FRONT_1920x1080P30] = "FHD P30",
    [MOVIE_SIZE_FRONT_1280x720P120] = "HD P120",
    [MOVIE_SIZE_FRONT_1280x720P60] = "HD P60",
    [MOVIE_SIZE_FRONT_1280x720P30] = "HD P30",
    [MOVIE_SIZE_FRONT_848x480P30] = "WVGA P30",
    [MOVIE_SIZE_FRONT_640x480P240] = "VGA P240",
    [MOVIE_SIZE_FRONT_640x480P30] = "VGA P30",
    [MOVIE_SIZE_FRONT_320x240P30] = "QVGA P30",
    [MOVIE_SIZE_DUAL_1920x1080P30_1920x1080P30] = "FHD P30+FHD P30",
    [MOVIE_SIZE_CLONE_1920x1080P30_1920x1080P30] = "FHD P30+FHD P30",
    [MOVIE_SIZE_CLONE_1920x1080P30_1280x720P30] = "FHD P30+HD P30",
    [MOVIE_SIZE_CLONE_2560x1440P30_848x480P30] = "QHD P30+WVGA P30",
    [MOVIE_SIZE_CLONE_2304x1296P30_848x480P30] = "3MHD P30+WVGA P30",
    [MOVIE_SIZE_CLONE_1920x1080P60_848x480P30] = "FHD P60+WVGA P30",
    [MOVIE_SIZE_CLONE_1920x1080P60_640x360P30] = "FHD P60+VGA P30",
    [MOVIE_SIZE_CLONE_1920x1080P30_848x480P30] = "FHD P30+WVGA P30",
    [MOVIE_SIZE_CLONE_2048x2048P30_480x480P30] = "2048x2048 P30 + 480x480 P30",
};

void FlowMovie_IconDrawDscMode(VControl *pCtrl)
{
    UxCtrl_SetShow(pCtrl, TRUE);
}

void FlowMovie_IconHideDscMode(VControl *pCtrl)
{
    UxCtrl_SetShow(pCtrl, FALSE);
}

void FlowMovie_IconDrawMaxRecTime(VControl *pCtrl)
{
    if(GxStrg_GetDevice(0)==NULL)
    {
        //No Storage, direct to return.
        return;
    }

    memset((void *)g_RecMaxTimeStr, 0, sizeof(g_RecMaxTimeStr));
    g_RecMaxTime = MovieExe_GetMaxRecSec();

    if (g_RecMaxTime <= 2)
    {
        FlowMovie_SetRecMaxTime(0);
    }
    else if (g_RecMaxTime > 86399)
    {
        ///23:59:59
        FlowMovie_SetRecMaxTime(86399);
    }
    else
    {
        FlowMovie_SetRecMaxTime(g_RecMaxTime);
    }

    if (UI_GetData(FL_CardStatus) == CARD_REMOVED)
    {
        snprintf(g_RecMaxTimeStr, 20, "%02d:%02d:%02d", 0, 0, 0);
    }
    else
    {
        snprintf(g_RecMaxTimeStr, 20, "%02d:%02d:%02d", g_RecMaxTime/3600,(g_RecMaxTime%3600)/60, (g_RecMaxTime%3600)%60);
    }

    UxStatic_SetData(pCtrl,STATIC_VALUE,Txt_Pointer(g_RecMaxTimeStr));
    UxCtrl_SetShow(pCtrl, TRUE);
}

void FlowMovie_IconHideMaxRecTime(VControl *pCtrl)
{
    UxCtrl_SetShow(pCtrl, FALSE);
}

void FlowMovie_IconDrawRecTime(VControl *pCtrl)
{
//#NT#2016/03/07#KCHong -begin
//#NT#Low power timelapse function
    #if (TIMELAPSE_LPR_FUNCTION == ENABLE)
    UINT32 TimelapseMode = 0;
    #endif
//#NT#2016/03/07#KCHong -end

    switch (gMovData.State)
    {
    case MOV_ST_REC:
    case MOV_ST_REC | MOV_ST_ZOOM:
        //#NT#2016/03/07#KCHong -begin
        //#NT#Low power timelapse function
        #if (TIMELAPSE_LPR_FUNCTION == ENABLE)
        if (MovieTLLPR_GetStatus() == (TL_FLOW_LPR |TL_STATE_RECORD))
        {
            TimelapseMode = UI_GetData(FL_MOVIE_TIMELAPSE_REC);
            if (TimelapseMode >= TL_LPR_TIME_MIN_PERIOD)
            {
                if (TimelapseMode == MOVIE_TIMELAPSEREC_OFF)
                    snprintf(g_RecCurrTimeStr, 20, "OFF");
                else if (TimelapseMode == MOVIE_TIMELAPSEREC_1SEC)
                    snprintf(g_RecCurrTimeStr, 20, "1 SEC");
                else if (TimelapseMode == MOVIE_TIMELAPSEREC_5SEC)
                    snprintf(g_RecCurrTimeStr, 20, "5 SEC");
                else if (TimelapseMode == MOVIE_TIMELAPSEREC_10SEC)
                    snprintf(g_RecCurrTimeStr, 20, "10 SEC");
                else if (TimelapseMode == MOVIE_TIMELAPSEREC_30SEC)
                    snprintf(g_RecCurrTimeStr, 20, "30 SEC");
                else if (TimelapseMode == MOVIE_TIMELAPSEREC_1MIN)
                    snprintf(g_RecCurrTimeStr, 20, "1 MIN");
                else if (TimelapseMode == MOVIE_TIMELAPSEREC_5MIN)
                    snprintf(g_RecCurrTimeStr, 20, "5 MIN");
                else if (TimelapseMode == MOVIE_TIMELAPSEREC_10MIN)
                    snprintf(g_RecCurrTimeStr, 20, "10 MIN");
                else if (TimelapseMode == MOVIE_TIMELAPSEREC_30MIN)
                    snprintf(g_RecCurrTimeStr, 20, "30 MIN");
                else if (TimelapseMode == MOVIE_TIMELAPSEREC_1HOUR)
                    snprintf(g_RecCurrTimeStr, 20, "1 HOUR");
                else if (TimelapseMode == MOVIE_TIMELAPSEREC_2HOUR)
                    snprintf(g_RecCurrTimeStr, 20, "2 HOUR");
                else if (TimelapseMode == MOVIE_TIMELAPSEREC_3HOUR)
                    snprintf(g_RecCurrTimeStr, 20, "3 HOUR");
                else if (TimelapseMode == MOVIE_TIMELAPSEREC_1DAY)
                    snprintf(g_RecCurrTimeStr, 20, "1 DAY");
                UxStatic_SetData(pCtrl,STATIC_VALUE,Txt_Pointer(g_RecCurrTimeStr));
                UxCtrl_SetShow(pCtrl, TRUE);
                break;
            }
        }
        #endif
        //#NT#2016/03/07#KCHong -end
        g_RecCurrTime = FlowMovie_GetRecCurrTime();
        memset((void *)g_RecCurrTimeStr, 0, sizeof(g_RecCurrTimeStr));
        snprintf(g_RecCurrTimeStr, 20, "%02d:%02d:%02d", g_RecCurrTime/3600,(g_RecCurrTime%3600)/60, (g_RecCurrTime%3600)%60);
        UxStatic_SetData(pCtrl,STATIC_VALUE,Txt_Pointer(g_RecCurrTimeStr));
        UxCtrl_SetShow(pCtrl, TRUE);
        break;
    }
}

void FlowMovie_IconDrawDateTime(void)
{
    RTC_DATE    Date;
    RTC_TIME    Time;

    // display Date/Time string in movie mode
    Date = rtc_getDate();
    Time = rtc_getTime();
    snprintf(date_str, 20, "%04d/%02d/%02d",Date.s.year,Date.s.month,Date.s.day);
    snprintf(time_str, 20, "%02d:%02d:%02d",Time.s.hour,Time.s.minute,Time.s.second);
    UxStatic_SetData(&UIFlowWndMovie_YMD_StaticCtrl,STATIC_VALUE,Txt_Pointer(date_str));
    UxStatic_SetData(&UIFlowWndMovie_HMS_StaticCtrl,STATIC_VALUE,Txt_Pointer(time_str));
    UxCtrl_SetShow(&UIFlowWndMovie_YMD_StaticCtrl,TRUE);
    UxCtrl_SetShow(&UIFlowWndMovie_HMS_StaticCtrl,TRUE);
}

void FlowMovie_IconHideDateTime(void)
{
    UxCtrl_SetShow(&UIFlowWndMovie_YMD_StaticCtrl,FALSE);
    UxCtrl_SetShow(&UIFlowWndMovie_HMS_StaticCtrl,FALSE);
}

void FlowMovie_IconDrawRec(VControl *pCtrl)
{
    UxState_SetData(pCtrl,STATE_CURITEM,UIFlowWndMovie_Status_REC_ICON_REC_TRANSPAENT);
}

void FlowMovie_IconHideRec(VControl *pCtrl)
{
    UxState_SetData(pCtrl,STATE_CURITEM,UIFlowWndMovie_Status_REC_ICON_REC_TRANSPAENT);
}

void FlowMovie_IconDrawSize(VControl *pCtrl)
{
    UxStatic_SetData(pCtrl,STATIC_VALUE,Txt_Pointer(resolution_Buf[SysGetFlag(FL_MOVIE_SIZE)]));
    UxCtrl_SetShow(pCtrl, TRUE);
}

void FlowMovie_IconHideSize(VControl *pCtrl)
{
    UxCtrl_SetShow(pCtrl, FALSE);
}

void FlowMovie_IconDrawEV(VControl *pCtrl)
{
    UxState_SetData(pCtrl,STATE_CURITEM,SysGetFlag(FL_EV));
    UxCtrl_SetShow(pCtrl,TRUE);
}

void FlowMovie_IconHideEV(VControl *pCtrl)
{
    UxCtrl_SetShow(pCtrl,FALSE);
}

void FlowMovie_IconDrawStorage(VControl *pCtrl)
{
    /* Update status item data */
    if (UI_GetData(FL_CardStatus) == CARD_REMOVED)
    {
        UxState_SetData(pCtrl,STATE_CURITEM,UIFlowWndMovie_Status_Storage_ICON_INTERNAL_FLASH);
    } else if (UI_GetData(FL_CardStatus) == CARD_LOCKED) {
        UxState_SetData(pCtrl,STATE_CURITEM,UIFlowWndMovie_Status_Storage_ICON_SD_LOCK);
    } else {
        UxState_SetData(pCtrl,STATE_CURITEM,UIFlowWndMovie_Status_Storage_ICON_SD_CARD);
    }

    UxCtrl_SetShow(pCtrl, TRUE);
}

void FlowMovie_IconHideStorage(VControl *pCtrl)
{
    UxCtrl_SetShow(pCtrl, FALSE);
}

void FlowMovie_IconDrawCyclicRec(VControl *pCtrl)
{
    UxState_SetData(pCtrl,STATE_CURITEM,SysGetFlag(FL_MOVIE_CYCLIC_REC));
    UxCtrl_SetShow(pCtrl, TRUE);
}

void FlowMovie_IconHideCyclicRec(VControl *pCtrl)
{
    UxCtrl_SetShow(pCtrl, FALSE);
}

void FlowMovie_IconDrawHDR(VControl *pCtrl)
{
    UxState_SetData(pCtrl,STATE_CURITEM,SysGetFlag(FL_MOVIE_HDR));
    UxCtrl_SetShow(pCtrl, TRUE);
}

void FlowMovie_IconHideHDR(VControl *pCtrl)
{
    UxCtrl_SetShow(pCtrl, FALSE);
}


#if defined(YQCONFIG_ANDROID_SYSTEM_SUPPORT)
void FlowMovie_IconDrawWDR(VControl *pCtrl)
{
    UxState_SetData(pCtrl,STATE_CURITEM,SysGetFlag(FL_MOVIE_WDR));
    UxCtrl_SetShow(pCtrl, TRUE);
}
void FlowMovie_IconHideWDR(VControl *pCtrl)
{
    UxCtrl_SetShow(pCtrl, FALSE);
}
#endif
void FlowMovie_IconDrawMotionDet(VControl *pCtrl)
{
    UxState_SetData(pCtrl,STATE_CURITEM,SysGetFlag(FL_MOVIE_MOTION_DET));
    UxCtrl_SetShow(pCtrl, TRUE);
}

void FlowMovie_IconHideMotionDet(VControl *pCtrl)
{
    #if defined(YQCONFIG_ANDROID_SYSTEM_SUPPORT)
    if(SysGetFlag(FL_MOVIE_MOTION_DET)==MOVIE_MOTIONDET_OFF)
    UxCtrl_SetShow(pCtrl, FALSE);
    #endif
}

void FlowMovie_IconDrawBattery(VControl *pCtrl)
{
    /* modify begin by ZMD, 2017-02-14 */
    #if defined(YQCONFIG_ANDROID_SYSTEM_SUPPORT)
    UxCtrl_SetShow(pCtrl,FALSE);
    #else

    UxState_SetData(pCtrl,STATE_CURITEM,GetBatteryLevel());
    UxCtrl_SetShow(pCtrl,TRUE);
    #endif
    /* modify end by ZMD, 2017-02-14 */
}

void FlowMovie_IconHideBattery(VControl *pCtrl)
{
    UxCtrl_SetShow(pCtrl,FALSE);
}

void FlowMovie_IconDrawDZoom(VControl *pCtrl)
{
    BOOL   bShow;

    UxStatic_SetData(pCtrl,STATIC_VALUE,Txt_Pointer(Get_DZoomRatioString()));
    bShow = (DZOOM_IDX_GET() > DZOOM_IDX_MIN())? TRUE : FALSE;
    UxCtrl_SetShow(pCtrl,bShow);
}

void FlowMovie_IconHideDZoom(VControl *pCtrl)
{
    UxCtrl_SetShow(pCtrl,FALSE);
}

void FlowMovie_DrawPIM(BOOL bDraw)
{
    UxCtrl_SetShow(&UIFlowWndMovie_StaticIcon_PIMCCtrl, bDraw);
}


/* modify begin by ZMD, 2017-02-14 */
#if defined(YQCONFIG_ANDROID_SYSTEM_SUPPORT)
void FlowMovie_IconDrawGsensor(void)
{
    UxState_SetData(&UIFlowWndMovie_Status_G_SensorCtrl,STATE_CURITEM,UI_GetData(FL_GSENSOR));
    UxCtrl_SetShow(&UIFlowWndMovie_Status_G_SensorCtrl, TRUE);
}

void FlowMovie_IconHideGsensor(void)
{
    if(gbGsensorTrig==FALSE)//Jacky lan added 2016.07.26
        UxCtrl_SetShow(&UIFlowWndMovie_Status_G_SensorCtrl, FALSE);
}

void FlowMovie_IconDrawLockFile(void)
{
    UxCtrl_SetShow(&UIFlowWndMovie_Status_File_LockCtrl, TRUE);
}

void FlowMovie_IconHideLockFile(void)
{
    if(gbGsensorTrig==FALSE)
        UxCtrl_SetShow(&UIFlowWndMovie_Status_File_LockCtrl, FALSE);
}

void FlowMovie_IconDrawUrgent_protect_Auto(VControl *pCtrl)
{
    UxState_SetData(pCtrl,STATE_CURITEM,SysGetFlag(FL_MOVIE_URGENT_PROTECT_AUTO));
    UxCtrl_SetShow(pCtrl, TRUE);
}

void FlowMovie_IconHideUrgent_protect_Auto(VControl *pCtrl)
{
    UxCtrl_SetShow(pCtrl, FALSE);
}

void FlowMovie_IconDrawStatus_Urgent_protect_Manual(VControl *pCtrl)
{
    UxState_SetData(pCtrl,STATE_CURITEM,SysGetFlag(FL_MOVIE_URGENT_PROTECT_MANUAL));
    UxCtrl_SetShow(pCtrl, TRUE);
}

void FlowMovie_IconHideUrgent_protect_Manual(VControl *pCtrl)
{
    UxCtrl_SetShow(pCtrl, FALSE);
}

void FlowMovie_IconDrawRecordAudio(void)
{
    UxState_SetData(&UIFlowWndMovie_StatusICN_AudioCtrl,STATE_CURITEM,UI_GetData(FL_MOVIE_AUDIO));
    UxCtrl_SetShow(&UIFlowWndMovie_StatusICN_AudioCtrl, TRUE);
}

void FlowMovie_IconHideRecordAudio(void)
{
    UxCtrl_SetShow(&UIFlowWndMovie_StatusICN_AudioCtrl, FALSE);
}

void FlowMovie_IconDrawADAS(void)
{
    //debug_msg("----------->FlowMovie_IconDrawADAS: %d..\r\n",SysGetFlag(FL_MOVIE_ADAS_CAL));
    UxState_SetData(&UIFlowWndMovie_Status_ADAS_StatusCtrl,STATE_CURITEM,UI_GetData(FL_MOVIE_ADAS_CAL));
    UxCtrl_SetShow(&UIFlowWndMovie_Status_ADAS_StatusCtrl, TRUE);
}

void FlowMovie_IconHideADAS(void)
{
    UxCtrl_SetShow(&UIFlowWndMovie_Status_ADAS_StatusCtrl, FALSE);
}

void FlowMovie_IconDrawFCW(void)
{
    UxState_SetData(&UIFlowWndMovie_Status_FCW_StatusCtrl,STATE_CURITEM,UI_GetData(FL_MOVIE_FCW));
    UxCtrl_SetShow(&UIFlowWndMovie_Status_FCW_StatusCtrl, TRUE);
}

void FlowMovie_IconHideFCW(void)
{
    UxCtrl_SetShow(&UIFlowWndMovie_Status_FCW_StatusCtrl, FALSE);
}

void FlowMovie_IconDrawLDWS(void)
{
    UxState_SetData(&UIFlowWndMovie_Status_LDWS_StatusCtrl,STATE_CURITEM,UI_GetData(FL_MOVIE_LDWS));
    UxCtrl_SetShow(&UIFlowWndMovie_Status_LDWS_StatusCtrl, TRUE);
}

void FlowMovie_IconHideLDWS(void)
{
    UxCtrl_SetShow(&UIFlowWndMovie_Status_LDWS_StatusCtrl, FALSE);
}


void FlowMovie_IconDrawParkingMode(void)
{
    if(UI_GetData(FL_MOVIE_PARKING)!=PARKINGMODE_OFF)
    {
        UxState_SetData(&UIFlowWndMovie_Status_ParkingModeCtrl,STATE_CURITEM,UIFlowWndMovie_Status_ParkingMode_ICON_PARKING_MODE);
    }
    else
    {
        UxState_SetData(&UIFlowWndMovie_Status_ParkingModeCtrl,STATE_CURITEM,UIFlowWndMovie_Status_ParkingMode_ICONID_NULL);
    }
    UxCtrl_SetShow(&UIFlowWndMovie_Status_ParkingModeCtrl, TRUE);
}

void FlowMovie_IconHideParkingMode(void)
{
    UxCtrl_SetShow(&UIFlowWndMovie_Status_ParkingModeCtrl, FALSE);
}


void FlowMovie_IconDrawGPS(void)
{

    UxCtrl_SetShow(&UIFlowWndMovie_Status_GPSCtrl, TRUE);

}

void FlowMovie_IconHideGPS(void)
{
    UxCtrl_SetShow(&UIFlowWndMovie_Status_GPSCtrl, FALSE);
}

void FlowMovie_IconDrawSpeed(void)
{
    sprintf(g_SpeedStr,"%d  km/h", (UINT32)g_CurSpeed);
    UxStatic_SetData(&UIFlowWndMovie_Static_SpeedCtrl,STATIC_VALUE,Txt_Pointer(g_SpeedStr));
    UxCtrl_SetShow(&UIFlowWndMovie_Static_SpeedCtrl, TRUE);
}

void FlowMovie_IconHideSpeed(void)
{
    UxCtrl_SetShow(&UIFlowWndMovie_Static_SpeedCtrl, FALSE);
}


void FlowMovie_IconDrawDistance(void)
{
    #if _ADAS_FUNC_
    sprintf(g_DistanceStr,"%d m", ADAS_GetFcwsCurrentDist());//(UINT32)g_uiAdasAlertDistance);
    UxStatic_SetData(&UIFlowWndMovie_Static_DistanceCtrl,STATIC_VALUE,Txt_Pointer(g_DistanceStr));
    UxCtrl_SetShow(&UIFlowWndMovie_Static_DistanceCtrl, TRUE);
    #endif
}


void FlowMovie_IconHideDistance(void)
{
    #if _ADAS_FUNC_
    UxCtrl_SetShow(&UIFlowWndMovie_Static_DistanceCtrl, FALSE);
    #endif
}
void FlowMovie_IconDrawTimeLapse(void)
{

    if (SysGetFlag(FL_MOVIE_TIMELAPSE_REC) != MOVIE_TIMELAPSEREC_OFF)
    {
        UxState_SetData(&UIFlowWndMovie_Status_TimeLapseCtrl,STATE_CURITEM,UIFlowWndMovie_Status_TimeLapse_ICON_TIME_LAPSE_ON);
    }
    else
    {
        UxState_SetData(&UIFlowWndMovie_Status_TimeLapseCtrl,STATE_CURITEM,UIFlowWndMovie_Status_TimeLapse_ICON_TIME_LAPSE_OFF);
    }
    UxCtrl_SetShow(&UIFlowWndMovie_Status_TimeLapseCtrl, TRUE);

}

void FlowMovie_IconHideTimeLapse(void)
{
    if(SysGetFlag(FL_MOVIE_TIMELAPSE_REC)==MOVIE_TIMELAPSEREC_OFF)
        UxCtrl_SetShow(&UIFlowWndMovie_Status_TimeLapseCtrl, FALSE);
}

void FlowMovie_IconDrawReverse(void)
{

    /* modify begin by ZMD, 2017-02-15 new version management*/
    #if defined(YQCONFIG_COMB_DET_BACKCAR_BY_GPIO) && defined(YQCONFIG_ANDROID_SYSTEM_SUPPORT)
    if(XmodemGetMTKReverseGearDet())
    {
        UxCtrl_SetShow(&UIFlowWndMovie_Static_ReverseCtrl, TRUE);
        FlowMovie_IconHideLDWS();
        FlowMovie_IconHideRecordAudio();
        FlowMovie_IconHideGPS();
    }
    else
    {
        UxCtrl_SetShow(&UIFlowWndMovie_Static_ReverseCtrl, FALSE);
    }
    #else
    UxCtrl_SetShow(&UIFlowWndMovie_Static_ReverseCtrl, FALSE);
    #endif
    /* modify end by ZMD, 2017-02-15 */
}

void FlowMovie_IconHideReverse(void)
{
    UxCtrl_SetShow(&UIFlowWndMovie_Static_ReverseCtrl, FALSE);
}

void FlowMovie_IconDrawTouchButton(BOOL Show)
{
    if(Show==FALSE)
    {
        //UxState_SetData(&UIFlowWndMovie_Status_Main_PBCtrl,STATE_CURITEM,UIFlowWndMovie_Status_Main_PB_ICON_PLAY_MODE);
        //UxCtrl_SetShow(&UIFlowWndMovie_Status_Main_PBCtrl, FALSE);
        if(!MovRec_IsRecording())
        {
            UxState_SetData(&UIFlowWndMovie_Status_Touch_RECCtrl,STATE_CURITEM,UIFlowWndMovie_Status_Touch_REC_ICON_TP_REC);
        }
        else
        {
            UxState_SetData(&UIFlowWndMovie_Status_Touch_RECCtrl,STATE_CURITEM,UIFlowWndMovie_Status_Touch_REC_ICON_TP_REC_PRESSED);
        }
        UxCtrl_SetShow(&UIFlowWndMovie_Status_Touch_RECCtrl, FALSE);
        UxState_SetData(&UIFlowWndMovie_Status_Touch_RERTURNCtrl,STATE_CURITEM,UIFlowWndMovie_Status_Touch_RERTURN_ICON_TP_RETURN);
        UxCtrl_SetShow(&UIFlowWndMovie_Status_Touch_RERTURNCtrl, FALSE);
        UxState_SetData(&UIFlowWndMovie_Status_Touch_SnapeShotCtrl,STATE_CURITEM,UIFlowWndMovie_Status_Touch_SnapeShot_ICON_SHOT_RELEASE);
        UxCtrl_SetShow(&UIFlowWndMovie_Status_Touch_SnapeShotCtrl, FALSE);
        UxState_SetData(&UIFlowWndMovie_Status_Touch_Change_DisplayModeCtrl,STATE_CURITEM,UIFlowWndMovie_Status_Touch_SnapeShot_ICON_SHOT_RELEASE);
        UxCtrl_SetShow(&UIFlowWndMovie_Status_Touch_Change_DisplayModeCtrl, FALSE);
        /* modify begin by ZMD, 2017-02-15 new version management*/
        #if defined(YQCONFIG_TOUCH_LOCKFILE_OPTION)
        UxState_SetData(&UIFlowWndMovie_Status_Touch_LockFileCtrl,STATE_CURITEM,UIFlowWndMovie_Status_Touch_LockFile_ICON_LOCK_RELEASE);
        UxCtrl_SetShow(&UIFlowWndMovie_Status_Touch_LockFileCtrl, FALSE);
        #endif
        #if defined(YQCONFIG_TOUCH_AUDIOSWITCH_OPTION)
        UxState_SetData(&UIFlowWndMovie_Status_Touch_AudioSwitchCtrl,STATE_CURITEM,UIFlowWndMovie_Status_Touch_AudioSwitch_ICON_SOUND_RELEASE);
        UxCtrl_SetShow(&UIFlowWndMovie_Status_Touch_AudioSwitchCtrl, FALSE);
        #endif
        /* modify end by ZMD, 2017-02-15 */

    }
    else
    {
//        UxState_SetData(&UIFlowWndMovie_Status_Main_PBCtrl,STATE_CURITEM,UIFlowWndMovie_Status_Main_PB_ICON_PLAY_MODE);
//        UxCtrl_SetShow(&UIFlowWndMovie_Status_Main_PBCtrl, TRUE);

        if(!MovRec_IsRecording())
        {
            UxState_SetData(&UIFlowWndMovie_Status_Touch_RECCtrl,STATE_CURITEM,UIFlowWndMovie_Status_Touch_REC_ICON_TP_REC);
        }
        else
        {
            UxState_SetData(&UIFlowWndMovie_Status_Touch_RECCtrl,STATE_CURITEM,UIFlowWndMovie_Status_Touch_REC_ICON_TP_REC_PRESSED);
        }
        UxCtrl_SetShow(&UIFlowWndMovie_Status_Touch_RECCtrl, TRUE);

        UxState_SetData(&UIFlowWndMovie_Status_Touch_RERTURNCtrl,STATE_CURITEM,UIFlowWndMovie_Status_Touch_RERTURN_ICON_TP_RETURN);
        UxCtrl_SetShow(&UIFlowWndMovie_Status_Touch_RERTURNCtrl, TRUE);

        #if(1)//(PHOTO_FUNCTION==ENABLE)
	// if(MovRec_IsRecording()){
          UxState_SetData(&UIFlowWndMovie_Status_Touch_SnapeShotCtrl,STATE_CURITEM,UIFlowWndMovie_Status_Touch_SnapeShot_ICON_SHOT_RELEASE);
          UxCtrl_SetShow(&UIFlowWndMovie_Status_Touch_SnapeShotCtrl, TRUE);
	// }else{
	// UxState_SetData(&UIFlowWndMovie_Status_Touch_SnapeShotCtrl,STATE_CURITEM,UIFlowWndMovie_Status_Touch_SnapeShot_ICON_SHOT_RELEASE);
      //    UxCtrl_SetShow(&UIFlowWndMovie_Status_Touch_SnapeShotCtrl, FALSE);

	// }
        #endif
        if(1==DrvSensor_DetPlugIn(SENSOR_ID_2))//if(ImageUnit_GetParam(&ISF_ImagePipe, IMAGEPIPE_PARAM_SENSORCOUNT)>=2)
        {
            UxState_SetData(&UIFlowWndMovie_Status_Touch_Change_DisplayModeCtrl,STATE_CURITEM,UIFlowWndMovie_Status_Touch_SnapeShot_ICON_SHOT_RELEASE);
            UxCtrl_SetShow(&UIFlowWndMovie_Status_Touch_Change_DisplayModeCtrl, TRUE);
        }
        else
        {
            UxState_SetData(&UIFlowWndMovie_Status_Touch_Change_DisplayModeCtrl,STATE_CURITEM,UIFlowWndMovie_Status_Touch_SnapeShot_ICON_SHOT_RELEASE);
            UxCtrl_SetShow(&UIFlowWndMovie_Status_Touch_Change_DisplayModeCtrl, FALSE);
        }

        /* modify begin by ZMD, 2017-02-15 new version management*/
        #if defined(YQCONFIG_TOUCH_LOCKFILE_OPTION)
        if(MovRec_IsRecording()&&(gbGsensorTrig==FALSE))
        {
            UxState_SetData(&UIFlowWndMovie_Status_Touch_LockFileCtrl,STATE_CURITEM,UIFlowWndMovie_Status_Touch_LockFile_ICON_LOCK_RELEASE);
            UxCtrl_SetShow(&UIFlowWndMovie_Status_Touch_LockFileCtrl, TRUE);
        }
        else
        {
            UxState_SetData(&UIFlowWndMovie_Status_Touch_LockFileCtrl,STATE_CURITEM,UIFlowWndMovie_Status_Touch_LockFile_ICON_LOCK_RELEASE);
            UxCtrl_SetShow(&UIFlowWndMovie_Status_Touch_LockFileCtrl, FALSE);
        }
        #endif
        #if defined(YQCONFIG_TOUCH_AUDIOSWITCH_OPTION)
        if(UI_GetData(FL_MOVIE_AUDIO)==MOVIE_AUDIO_ON)
        {
            UxState_SetData(&UIFlowWndMovie_Status_Touch_AudioSwitchCtrl,STATE_CURITEM,UIFlowWndMovie_Status_Touch_AudioSwitch_ICON_SOUND_RELEASE);
            UxCtrl_SetShow(&UIFlowWndMovie_Status_Touch_AudioSwitchCtrl, TRUE);
        }
        else
        {
            UxState_SetData(&UIFlowWndMovie_Status_Touch_AudioSwitchCtrl,STATE_CURITEM,UIFlowWndMovie_Status_Touch_AudioSwitch_ICON_SOUND_PRESS);
            UxCtrl_SetShow(&UIFlowWndMovie_Status_Touch_AudioSwitchCtrl, TRUE);
        }
        #endif

    }
}
#endif
void FlowMovie_UpdateIcons(BOOL bShow)
{
    /* modify begin by ZMD, 2017-02-14 */
    #if defined(YQCONFIG_ANDROID_SYSTEM_SUPPORT)
    if ((bShow == FALSE) || (SysGetFlag(FL_LCD_DISPLAY) == DISPOUT_OFF)||(XmodemGetMTKReverseGearDet()==TRUE))
    #elif defined(YQCONFIG_BACKCAR_DET_SUPPORT)
    if ((bShow == FALSE) || (SysGetFlag(FL_LCD_DISPLAY) == DISPOUT_OFF)||(Get_BackCar_Status()==TRUE))
    #else
    if ((bShow == FALSE) || (SysGetFlag(FL_LCD_DISPLAY) == DISPOUT_OFF))
    #endif
        /* modify end by ZMD, 2017-02-14 */
    {
        FlowMovie_IconHideDscMode(&UIFlowWndMovie_Static_cameraCtrl);
        FlowMovie_IconHideSize(&UIFlowWndMovie_Static_resolutionCtrl);
        FlowMovie_IconHideMaxRecTime(&UIFlowWndMovie_Static_maxtimeCtrl);
        FlowMovie_IconHideRec(&UIFlowWndMovie_Status_RECCtrl);
        FlowMovie_IconHideStorage(&UIFlowWndMovie_Status_StorageCtrl);
        FlowMovie_IconHideCyclicRec(&UIFlowWndMovie_Status_CyclicRecCtrl);
        FlowMovie_IconHideMotionDet(&UIFlowWndMovie_Status_MotionDetCtrl);
        FlowMovie_IconHideDZoom(&UIFlowWndMovie_Zoom_StaticCtrl);
        FlowMovie_IconHideBattery(&UIFlowWndMovie_Status_batteryCtrl);
        FlowMovie_IconHideEV(&UIFlowWndMovie_StatusICN_EVCtrl);
        FlowMovie_IconHideDateTime();
        FlowMovie_DrawPIM(FALSE);
        /* modify begin by ZMD, 2017-02-14 */
        #if defined(YQCONFIG_ANDROID_SYSTEM_SUPPORT)
        FlowMovie_IconHideGsensor();
        FlowMovie_IconHideWDR(&UIFlowWndMovie_Status_WDRCtrl);
        FlowMovie_IconHideHDR(&UIFlowWndMovie_Status_HDRCtrl);
        UxCtrl_SetShow(&UIFlowWndMovie_Status_WIFICtrl, FALSE);
        FlowMovie_IconHideParkingMode();
        FlowMovie_IconHideRecordAudio();
        FlowMovie_IconHideADAS();
//        UxCtrl_SetShow(&UIFlowWndMovie_Status_Main_PBCtrl, FALSE);
        UxCtrl_SetShow(&UIFlowWndMovie_Status_Touch_RECCtrl, FALSE);
        UxCtrl_SetShow(&UIFlowWndMovie_Status_Touch_RERTURNCtrl, FALSE);
        UxCtrl_SetShow(&UIFlowWndMovie_Status_Touch_SnapeShotCtrl, FALSE);
        if(gMovData.State== MOV_ST_VIEW)
        {
            UxCtrl_SetShow(&UIFlowWndMovie_Static_timeCtrl, FALSE);
        }
        //FlowMovie_IconDrawDistance();
        //UxCtrl_SetShow(&UIFlowWndMovie_Panel_SecondPipviewCtrl, FALSE);
        //UxCtrl_SetShow(&UIFlowWndMovie_Panel_SecondPipview2Ctrl, FALSE);
        UxCtrl_SetShow(&UIFlowWndMovie_Status_Touch_Change_DisplayModeCtrl, FALSE);
        #if defined(YQCONFIG_TOUCH_LOCKFILE_OPTION)
        UxCtrl_SetShow(&UIFlowWndMovie_Status_Touch_LockFileCtrl, FALSE);
        #endif
        #if defined(YQCONFIG_TOUCH_AUDIOSWITCH_OPTION)
        UxCtrl_SetShow(&UIFlowWndMovie_Status_Touch_AudioSwitchCtrl, FALSE);
        #endif

        FlowMovie_IconDrawReverse();
        #endif
    }
    /* modify begin by ZMD, 2017-02-14 */
    #if defined(YQCONFIG_ANDROID_SYSTEM_SUPPORT)
    else
    {
        if(FALSE==XmodemCheckBackSensorBackMirrorDet()) /* add begin by ZMD, 2016-09-01, ?-?¡ì???¡ì??: ??¡ì2??¡ì?¨¦?¡§¡è?¡§o??¡§o?¡ìo??¡ìo?¡§¡è?¡§o??¡ì?D?¡ìa??¡§¡è?¡ìo???¡ìo?*/
        {
            FlowMovie_IconHideDscMode(&UIFlowWndMovie_Static_cameraCtrl);
            FlowMovie_IconDrawSize(&UIFlowWndMovie_Static_resolutionCtrl);
            if(gMovData.State== MOV_ST_VIEW)
            {
                FlowMovie_IconDrawMaxRecTime(&UIFlowWndMovie_Static_maxtimeCtrl);
                UxCtrl_SetShow(&UIFlowWndMovie_Static_timeCtrl, FALSE);
            }
            //FlowMovie_IconHideRec(&UIFlowWndMovie_PanelCtrl);
            FlowMovie_IconHideRec(&UIFlowWndMovie_Status_RECCtrl);
            FlowMovie_IconHideStorage(&UIFlowWndMovie_Status_StorageCtrl);
            FlowMovie_IconDrawCyclicRec(&UIFlowWndMovie_Status_CyclicRecCtrl);

            FlowMovie_IconDrawWDR(&UIFlowWndMovie_Status_WDRCtrl);
            FlowMovie_IconHideHDR(&UIFlowWndMovie_Status_HDRCtrl);

            UxCtrl_SetShow(&UIFlowWndMovie_Status_WIFICtrl, FALSE);
            FlowMovie_IconDrawMotionDet(&UIFlowWndMovie_Status_MotionDetCtrl);
            FlowMovie_IconDrawDZoom(&UIFlowWndMovie_Zoom_StaticCtrl);
            FlowMovie_IconHideBattery(&UIFlowWndMovie_Status_batteryCtrl);
            FlowMovie_IconHideUrgent_protect_Auto(&UIFlowWndMovie_Status_Urgent_protect_AutoCtrl);
            //FlowMovie_IconDrawBattery(&UIFlowWndMovie_Status_batteryCtrl);
            FlowMovie_IconHideEV(&UIFlowWndMovie_StatusICN_EVCtrl);

            FlowMovie_IconDrawDateTime();
            FlowMovie_DrawPIM(FALSE);
            FlowMovie_IconHideLockFile();

            #if (GPS_FUNCTION == ENABLE)
            FlowMovie_IconDrawGPS();
            if(GPIOMap_GPS_Det()==TRUE)
            {
                FlowMovie_IconDrawSpeed();
            }
            else
            {
                //FlowMovie_IconHideSpeed();
            }
            #else
            FlowMovie_IconHideGPS();
            //FlowMovie_IconHideSpeed();
            #endif

            FlowMovie_IconDrawGsensor();
            FlowMovie_IconHideTimeLapse();
            #if defined(AUDIO_MIC_SWITCH_SUPPORT) && (AUDIO_MIC_SWITCH_SUPPORT==ENABLE)
            FlowMovie_IconDrawRecordAudio();
            #endif
            FlowMovie_IconDrawParkingMode();
            FlowMovie_IconHideFCW();
            FlowMovie_IconHideLDWS();
            FlowMovie_IconDrawADAS();
            FlowMovie_IconDrawTouchButton(TRUE);
            FlowMovie_IconDrawSpeed();
            #if _ADAS_FUNC_
            FlowMovie_IconDrawDistance();
            #else
            UxCtrl_SetShow(&UIFlowWndMovie_Static_DistanceCtrl, FALSE);
            #endif
            //UxCtrl_SetShow(&UIFlowWndMovie_Panel_SecondPipviewCtrl, FALSE);
            //UxCtrl_SetShow(&UIFlowWndMovie_Panel_SecondPipview2Ctrl, FALSE);
            FlowMovie_IconDrawReverse();
        }
    }
    gbMovieTouchUIDisplayEn= bShow;
    #else
    else
    {
        FlowMovie_IconDrawDscMode(&UIFlowWndMovie_Static_cameraCtrl);
        FlowMovie_IconDrawSize(&UIFlowWndMovie_Static_resolutionCtrl);
        FlowMovie_IconDrawMaxRecTime(&UIFlowWndMovie_Static_maxtimeCtrl);
        FlowMovie_IconHideRec(&UIFlowWndMovie_Status_RECCtrl);
        FlowMovie_IconDrawStorage(&UIFlowWndMovie_Status_StorageCtrl);
        FlowMovie_IconDrawCyclicRec(&UIFlowWndMovie_Status_CyclicRecCtrl);
        FlowMovie_IconDrawHDR(&UIFlowWndMovie_Status_HDRCtrl);
        FlowMovie_IconDrawMotionDet(&UIFlowWndMovie_Status_MotionDetCtrl);
        FlowMovie_IconDrawDZoom(&UIFlowWndMovie_Zoom_StaticCtrl);
        FlowMovie_IconDrawBattery(&UIFlowWndMovie_Status_batteryCtrl);
        FlowMovie_IconDrawEV(&UIFlowWndMovie_StatusICN_EVCtrl);
        FlowMovie_IconDrawDateTime();
        FlowMovie_DrawPIM(FALSE);
    }
    #endif
}

