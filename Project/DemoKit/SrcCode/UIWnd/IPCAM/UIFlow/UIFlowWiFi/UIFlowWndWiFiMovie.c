//This source code is generated by UI Designer Studio.
#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIGraphics.h"
#include "NVTToolCommand.h"
#include "UIFlowWndWiFiMovieRes.c"
#include "UIFlowWndWiFiMovie.h"
#include "UIMovieMapping.h"
#include "SysCommon.h"
#include "AppCommon.h"
#include "PrjCfg.h"
#include "WifiAppCmd.h"
#include "UIAppMoviePlay.h"
#include "UIAppMovie.h"
#include "UIFlowMovieFuncs.h"
#include "md_api.h"
#include "ImageApp_NetMovie.h"
#if(WIFI_AP_FUNC==ENABLE)
#include "RtspNvtApi.h"
#endif
//---------------------UIFlowWndWiFiMovieCtrl Debug Definition -----------------------------
#define _UIFLOWWNDWIFIMOVIE_ERROR_MSG        1
#define _UIFLOWWNDWIFIMOVIE_TRACE_MSG        0

#if (_UIFLOWWNDWIFIMOVIE_ERROR_MSG&(PRJ_DBG_LVL>=PRJ_DBG_LVL_ERR))
#define UIFlowWndWiFiMovieErrMsg(...)            debug_msg ("^R UIFlowWndWiFiMovie: "__VA_ARGS__)
#else
#define UIFlowWndWiFiMovieErrMsg(...)
#endif

#if (_UIFLOWWNDWIFIMOVIE_TRACE_MSG&(PRJ_DBG_LVL>=PRJ_DBG_LVL_TRC))
#define UIFlowWndWiFiMovieTraceMsg(...)          debug_msg ("^B UIFlowWndWiFiMovie: "__VA_ARGS__)
#else
#define UIFlowWndWiFiMovieTraceMsg(...)
#endif

//---------------------UIFlowWndWiFiMovieCtrl Global Variables -----------------------------
//static BOOL    g_UIWiFiMovRecMotionDet = FALSE;

//---------------------UIFlowWndWiFiMovieCtrl Prototype Declaration  -----------------------

//---------------------UIFlowWndWiFiMovieCtrl Public API  ----------------------------------

//---------------------UIFlowWndWiFiMovieCtrl Private API  ---------------------------------

void FlowWiFiMovie_StartRec(void)
{
    UI_SetData(FL_WIFI_MOVIE_FMT,WIFI_RTSP_REC);
    Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIE_REC_START, 0);
}

void FlowWiFiMovie_StopRec(void)
{
    UI_SetData(FL_WIFI_MOVIE_FMT,WIFI_RTSP_LIVEVIEW);
    Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIE_REC_STOP, 0);
}

void FlowWiFiMovie_Stop2Idle(void)
{
    UI_SetData(FL_WIFI_MOVIE_FMT,WIFI_RTSP_IDLE);
    ImageApp_NetMovie_Idle();
}

//---------------------UIFlowWndWiFiMovieCtrl Control List---------------------------
CTRL_LIST_BEGIN(UIFlowWndWiFiMovie)
CTRL_LIST_END

//----------------------UIFlowWndWiFiMovieCtrl Event---------------------------
INT32 UIFlowWndWiFiMovie_OnOpen(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndWiFiMovie_OnClose(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndWiFiMovie_OnTimer(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndWiFiMovie_OnExeMovieRec(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndWiFiMovie_OnExeSetMovieSize(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndWiFiMovie_OnExeMovieTriggerRawEnc(VControl *, UINT32, UINT32 *);
EVENT_BEGIN(UIFlowWndWiFiMovie)
EVENT_ITEM(NVTEVT_OPEN_WINDOW,UIFlowWndWiFiMovie_OnOpen)
EVENT_ITEM(NVTEVT_CLOSE_WINDOW,UIFlowWndWiFiMovie_OnClose)
EVENT_ITEM(NVTEVT_TIMER,UIFlowWndWiFiMovie_OnTimer)
EVENT_ITEM(NVTEVT_WIFI_EXE_MOVIE_REC,UIFlowWndWiFiMovie_OnExeMovieRec)
EVENT_ITEM(NVTEVT_WIFI_EXE_MOVIE_REC_SIZE,UIFlowWndWiFiMovie_OnExeSetMovieRecSize)
EVENT_ITEM(NVTEVT_WIFI_EXE_MOVIE_TRIGGER_RAWENC, UIFlowWndWiFiMovie_OnExeMovieTriggerRawEnc)
EVENT_END

INT32 UIFlowWndWiFiMovie_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_DefaultEvent(pCtrl,NVTEVT_OPEN_WINDOW,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndWiFiMovie_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_DefaultEvent(pCtrl,NVTEVT_CLOSE_WINDOW,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndWiFiMovie_OnTimer(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32  uiEvent;
    uiEvent = paramNum ? paramArray[0] : 0;

    switch(uiEvent)
    {
    case NVTEVT_05SEC_TIMER:
        // Motion Detect function
        if (SysGetFlag(FL_MOVIE_MOTION_DET) == MOVIE_MOTIONDET_ON && System_GetState(SYS_STATE_CURRMODE) == PRIMARY_MODE_MOVIE)
        {
            static UINT32  uiMotionDetGo = 0;
            static UINT32  uiMotionDetStop = 0;

#if (_IPPLIB_ != _IPL_FAKE_)
            //#NT#2016/10/18#Jeah Yen -begin
            //#NT#move code to support sensor map
            static UINT32  uiMotionDetRet = 0;
            Ux_SendEvent(0, NVTEVT_EXE_MOTION_DET_RUN, 1, (UINT32)&uiMotionDetRet);
            if (uiMotionDetRet==TRUE)
            //#NT#2016/10/18#Jeah Yen -end
#else
            if(0)
#endif
            {
                 uiMotionDetGo++;
                 if (uiMotionDetGo>=2)
                 {
                    uiMotionDetStop=0;
                    if (UIFlowWndWiFiMovie_GetStatus() == WIFI_MOV_ST_LVIEW)
                    {
                        debug_msg("^GStart Record\r\n");
                        // reset uiMotionDetGo
                        uiMotionDetGo = 0;
                        // Post event to record video and send socket to notify APP recording is started.
                        Ux_PostEvent(NVTEVT_WIFI_EXE_MOVIE_REC, 2, 1, 1);
                    }
                 }
            }
            else
            {
                uiMotionDetStop++;
                if (uiMotionDetStop>=2) // 1 sec
                {
                    uiMotionDetGo=0;
                }
                #if 1
                if (uiMotionDetStop>=20) // 10 Sec
                #else
                if (uiMotionDetStop>=65536) // 10 Sec
                #endif
                {
                    uiMotionDetStop = 0;
                    if (UIFlowWndWiFiMovie_GetStatus() == WIFI_MOV_ST_RECORD)
                    {
                        if (FlowMovie_GetRecCurrTime() >= 1 || (SysGetFlag(FL_MOVIE_TIMELAPSE_REC) != MOVIE_TIMELAPSEREC_OFF))
                        {
                            debug_msg("^RWIFIAPP_RET_RECORD_STOPPED\r\n");
                            WifiApp_SendCmd(WIFIAPP_CMD_NOTIFY_STATUS, WIFIAPP_RET_RECORD_STOPPED);
                        }
                        break;
                    }
                }
            }
        }
        break;
    }

    Ux_DefaultEvent(pCtrl,NVTEVT_TIMER,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndWiFiMovie_OnExeMovieRec(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    BOOL startRec =0;
    UINT32 uiNotifyAPPStatus = 0;
    UINT32 result = WIFIAPP_RET_OK;
    UINT32 curStatus = 0;
    if(paramNum>0)
    {
        startRec=paramArray[0];

        if(paramNum > 1)
        {
            uiNotifyAPPStatus = paramArray[1];
        }
    }

    if (System_GetState(SYS_STATE_CURRMODE) != PRIMARY_MODE_MOVIE)
    {
        WifiCmd_Done(WIFIFLAG_RECORD_DONE,WIFIAPP_RET_STATE_ERR);
        UIFlowWndWiFiMovieErrMsg("not movie mode\r\n");
        return NVTEVT_CONSUME;
    }

    curStatus = UIFlowWndWiFiMovie_GetStatus();

    if(startRec)
    {
        if (curStatus == WIFI_MOV_ST_LVIEW)
        {
            UINT32 MaxTime = MovieExe_GetFreeRecSec();
            if((MaxTime<=3) && (SysGetFlag(FL_MOVIE_CYCLIC_REC) == MOVIE_CYCLICREC_OFF))
            {
                result = WIFIAPP_RET_STORAGE_FULL;
                UIFlowWndWiFiMovieErrMsg("recTime<3 sec\r\n");
            }
            else
            {
                // Reset Current record time
                FlowMovie_SetRecCurrTime(0);

                // Send socket to notify APP movie recording is started.
                if(uiNotifyAPPStatus)
                {
                    WifiApp_SendCmd(WIFIAPP_CMD_NOTIFY_STATUS, WIFIAPP_RET_RECORD_STARTED);
                }
                else
                {
                    FlowWiFiMovie_StartRec();
                }
            }
        }
        else if(curStatus == WIFI_MOV_ST_IDLE)
        {
            result = WIFIAPP_RET_FAIL;
            UIFlowWndWiFiMovieErrMsg("idle state,cannot record\r\n");
        }
    }
    else  //stop rec
    {
        UINT32 recordTime = FlowMovie_GetRecCurrTime();
        if ((curStatus == WIFI_MOV_ST_RECORD) && ((recordTime >= 1) || (SysGetFlag(FL_MOVIE_TIMELAPSE_REC) != MOVIE_TIMELAPSEREC_OFF)))
        {
            FlowWiFiMovie_StopRec();
            // Notify Maximum Record Time
            UI_SetData(FL_WIFI_MOVIE_MAXRECTIME, MovieExe_GetMaxRecSec());
        }
        else if ((curStatus == WIFI_MOV_ST_RECORD) &&(recordTime < 1))
        {
            result = WIFIAPP_RET_FAIL;
            UIFlowWndWiFiMovieErrMsg("recTime<1 sec %d sts %d\r\n",recordTime,UIFlowWndWiFiMovie_GetStatus());
        }
        else if(curStatus == WIFI_MOV_ST_IDLE)
        {
            result = WIFIAPP_RET_FAIL;
            UIFlowWndWiFiMovieErrMsg("idle state,cannot stop\r\n");
        }
    }
    // Notify Done
    //RTSP start OK should wait video read,so not return done
    //HTTP return done directly
    if(!((ImageApp_NetMovie_GetConfig(NETMOVIE_CFG_MODE)==NETMOVIE_TYPE_RTSP)
        &&(result==WIFIAPP_RET_OK)))
    {
        WifiCmd_Done(WIFIFLAG_RECORD_DONE,result);
    }
    return NVTEVT_CONSUME;
}

INT32 UIFlowWndWiFiMovie_OnExeSetMovieRecSize(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 Data;

    Data = paramNum ? paramArray[0] : 0;

    if (System_GetState(SYS_STATE_CURRMODE) != PRIMARY_MODE_MOVIE)
    {
        UIFlowWndWiFiMovieErrMsg("not movie mode\r\n");
        return NVTEVT_CONSUME;
    }

    if (UIFlowWndWiFiMovie_GetStatus() != WIFI_MOV_ST_RECORD)
    {
        #if (WIFI_FINALCAM_APP_STYLE == ENABLE)
        // Transfer movie size index for FinalCam APP.
        switch(Data)
        {
            default:
            case FINALCAM_MOVIE_REC_SIZE_1080P:   // 1920 x 1080
            Data = MOVIE_SIZE_FRONT_1920x1080P30;
            break;

            case FINALCAM_MOVIE_REC_SIZE_720P:    // 1280 x 720
            Data = MOVIE_SIZE_FRONT_1280x720P30;
            break;

            case FINALCAM_MOVIE_REC_SIZE_WVGA:    // 854 x 480
            Data = MOVIE_SIZE_FRONT_848x480P30;
            break;

            case FINALCAM_MOVIE_REC_SIZE_VGA:     // 640 x 480
            Data = MOVIE_SIZE_FRONT_640x480P30;
            break;
        }
        #endif

        //until start preview to set size and change mode
        UI_SetData(FL_MOVIE_SIZE_MENU,Data);
        //Ux_SendEvent(&CustomMovieObjCtrl,NVTEVT_EXE_MOVIESIZE, 1, Data);
        //Ux_SendEvent(&CustomMovieObjCtrl,NVTEVT_EXE_IMAGE_RATIO, 1, GetMovieSizeRatio(UI_GetData(FL_MOVIE_SIZE)));

        //check some menu item change ,we need to re open
        #if (WIFI_FINALCAM_APP_STYLE == ENABLE)
        {
            BOOL bReOpen = FALSE;

            FlowWiFiMovie_Stop2Idle();  // Stop IPL setting first.

            Delay_DelayMs(100);  // Could be fine tuned.

            bReOpen = FlowMovie_CheckReOpenItem();

            if(bReOpen==TRUE)
            {
                Ux_SendEvent(0,NVTEVT_SYSTEM_MODE, 1, System_GetState(SYS_STATE_CURRMODE));
            }
            else
            {
                FlowWiFiMovie_StopRec();
            }
        }
        #endif
    }

    return NVTEVT_CONSUME;
}

INT32 UIFlowWndWiFiMovie_OnExeSetCyclicRec(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 Data;

    Data = paramNum ? paramArray[0] : 0;

    if (System_GetState(SYS_STATE_CURRMODE) != PRIMARY_MODE_MOVIE)
    {
        UIFlowWndWiFiMovieErrMsg("not movie mode\r\n");
        return NVTEVT_CONSUME;
    }

    if (UIFlowWndWiFiMovie_GetStatus() != WIFI_MOV_ST_RECORD)
    {
        Ux_SendEvent(&CustomMovieObjCtrl,NVTEVT_EXE_CYCLIC_REC, 1, Data);
    }

    return NVTEVT_CONSUME;
}

INT32 UIFlowWndWiFiMovie_OnExeSetHDR(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 Data;

    Data = paramNum ? paramArray[0] : 0;

    if (System_GetState(SYS_STATE_CURRMODE) != PRIMARY_MODE_MOVIE)
    {
        UIFlowWndWiFiMovieErrMsg("not movie mode\r\n");
        return NVTEVT_CONSUME;
    }

    if (UIFlowWndWiFiMovie_GetStatus() != WIFI_MOV_ST_RECORD)
    {
        //until start preview to set size and change WDR,
        //650 HDR is 660 WDR.660 HDR is sensor HDR,it is depend on sensor
        UI_SetData(FL_MOVIE_WDR_MENU,Data);
        //Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIE_HDR, 1, Data);
    }

    return NVTEVT_CONSUME;
}

INT32 UIFlowWndWiFiMovie_OnExeSetMovieEV(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 Data;

    Data = paramNum ? paramArray[0] : 0;
//#NT#2015/09/09#Janice Huang -begin
//#NT# photo and movie use the same EV value
#if 0
    if (System_GetState(SYS_STATE_CURRMODE) != PRIMARY_MODE_MOVIE)
    {
        UIFlowWndWiFiMovieErrMsg("not movie mode\r\n");
        return NVTEVT_CONSUME;
    }
#endif
//#NT#2015/09/09#Janice Huang -end

    if (UIFlowWndWiFiMovie_GetStatus() != WIFI_MOV_ST_RECORD)
    {
        Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIE_EV, 1, Data);
    }

    return NVTEVT_CONSUME;
}
//#NT#2016/06/03#Charlie Chang -begin
//#NT# support contrast, two-way audio, flip_mirror, quality set
INT32 UIFlowWndWiFiMovie_OnExeSetMovieContrast(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 Data;

    Data = paramNum ? paramArray[0] : 0;

    if (UIFlowWndWiFiMovie_GetStatus() != WIFI_MOV_ST_RECORD)
    {
        Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIE_CONTRAST, 1, Data);
    }

    return NVTEVT_CONSUME;
}
INT32 UIFlowWndWiFiMovie_OnExeSetMovieAudioIn(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 Data;

    Data = paramNum ? paramArray[0] : 0;

    if (UIFlowWndWiFiMovie_GetStatus() != WIFI_MOV_ST_RECORD)
    {
        Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIE_AUDIOIN, 1, Data);
    }

    return NVTEVT_CONSUME;
}
INT32 UIFlowWndWiFiMovie_OnExeSetMovieAudioInSR(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 Data;

    Data = paramNum ? paramArray[0] : 0;

    if (UIFlowWndWiFiMovie_GetStatus() != WIFI_MOV_ST_RECORD)
    {
        Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIE_AUDIOIN_SR, 1, Data);
    }

    return NVTEVT_CONSUME;
}
INT32 UIFlowWndWiFiMovie_OnExeSetMovieFlipMirror(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 Data;

    Data = paramNum ? paramArray[0] : 0;

    if (UIFlowWndWiFiMovie_GetStatus() != WIFI_MOV_ST_RECORD)
    {
        Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIE_FLIP_MIRROR, 1, Data);
    }

    return NVTEVT_CONSUME;
}
INT32 UIFlowWndWiFiMovie_OnExeSetMovieQualitySet(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 Data;
    Data = paramNum ? paramArray[0] : 0;

    if (UIFlowWndWiFiMovie_GetStatus() != WIFI_MOV_ST_RECORD)
    {
        Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIE_QUALITY_SET, 1, Data);
    }

    return NVTEVT_CONSUME;
}
//#NT#2016/06/03#Charlie Chang -end

INT32 UIFlowWndWiFiMovie_OnExeSetMotionDet(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 Data;

    Data = paramNum ? paramArray[0] : 0;

    if (System_GetState(SYS_STATE_CURRMODE) != PRIMARY_MODE_MOVIE)
    {
        UIFlowWndWiFiMovieErrMsg("not movie mode\r\n");
        return NVTEVT_CONSUME;
    }

    if (UIFlowWndWiFiMovie_GetStatus() != WIFI_MOV_ST_RECORD)
    {
        Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOTION_DET, 1, Data);
    }

    return NVTEVT_CONSUME;
}

INT32 UIFlowWndWiFiMovie_OnExeSetMovieAudio(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 Data;

    Data = paramNum ? paramArray[0] : 0;

    if (System_GetState(SYS_STATE_CURRMODE) != PRIMARY_MODE_MOVIE)
    {
        UIFlowWndWiFiMovieErrMsg("not movie mode\r\n");
        return NVTEVT_CONSUME;
    }

    if (UIFlowWndWiFiMovie_GetStatus() != WIFI_MOV_ST_RECORD)
    {
        Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIE_AUDIO, 1, Data);
    }

    return NVTEVT_CONSUME;
}

INT32 UIFlowWndWiFiMovie_OnExeSetMovieDateImprint(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 Data;

    Data = paramNum ? paramArray[0] : 0;

    if (System_GetState(SYS_STATE_CURRMODE) != PRIMARY_MODE_MOVIE)
    {
        UIFlowWndWiFiMovieErrMsg("not movie mode\r\n");
        return NVTEVT_CONSUME;
    }

    if (UIFlowWndWiFiMovie_GetStatus() != WIFI_MOV_ST_RECORD)
    {
        Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIE_DATE_IMPRINT, 1, Data);
    }

    return NVTEVT_CONSUME;
}

INT32 UIFlowWndWiFiMovie_OnExeSetMovieLiveviewSize(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 Data;

    Data = paramNum ? paramArray[0] : 0;

    if (System_GetState(SYS_STATE_CURRMODE) != PRIMARY_MODE_MOVIE)
    {
        UIFlowWndWiFiMovieErrMsg("not movie mode\r\n");
        return NVTEVT_CONSUME;
    }

    if (UIFlowWndWiFiMovie_GetStatus() != WIFI_MOV_ST_RECORD)
    {
        UI_SetData(FL_WIFI_MOVIE_APP_PREVIEW_SIZE, Data);
    }

    return NVTEVT_CONSUME;
}

INT32 UIFlowWndWiFiMovie_OnExeSetMovieGSesnorSensitivity(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 Data;

    Data = paramNum ? paramArray[0] : 0;

    if (System_GetState(SYS_STATE_CURRMODE) != PRIMARY_MODE_MOVIE)
    {
        UIFlowWndWiFiMovieErrMsg("not movie mode\r\n");
        return NVTEVT_CONSUME;
    }

    if (UIFlowWndWiFiMovie_GetStatus() != WIFI_MOV_ST_RECORD)
    {
        Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_GSENSOR, 1, Data);
    }

    return NVTEVT_CONSUME;
}

INT32 UIFlowWndWiFiMovie_OnExeSetAutoRecording(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 Data;

    Data = paramNum ? paramArray[0] : 0;

    if (System_GetState(SYS_STATE_CURRMODE) != PRIMARY_MODE_MOVIE)
    {
        UIFlowWndWiFiMovieErrMsg("not movie mode\r\n");
        return NVTEVT_CONSUME;
    }

    if (UIFlowWndWiFiMovie_GetStatus() != WIFI_MOV_ST_RECORD)
    {
        UI_SetData(FL_WIFI_AUTO_RECORDING, Data);
    }

    return NVTEVT_CONSUME;
}

INT32 UIFlowWndWiFiMovie_OnExeSetMovieRecBitRate(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    char *Data = 0;
    UINT32 uiTargetBitRate;


    if(paramNum)
    {
        Data = (char *)paramArray[0];

        if (System_GetState(SYS_STATE_CURRMODE) != PRIMARY_MODE_MOVIE)
        {
            UIFlowWndWiFiMovieErrMsg("not movie mode\r\n");
            WifiCmd_UnlockString(Data);
            return NVTEVT_CONSUME;
        }

        if (1 == sscanf_s(Data, "%d", &uiTargetBitRate))
        {
            if (UIFlowWndWiFiMovie_GetStatus() != WIFI_MOV_ST_RECORD)
            {
                debug_msg("Rec target bit rate:%d KBytes\r\n", uiTargetBitRate);
                SetMovieTargetBitrate(1, UI_GetData(FL_MOVIE_SIZE), uiTargetBitRate * 1024);
            }
        }
        WifiCmd_UnlockString(Data);
    }

    return NVTEVT_CONSUME;
}

INT32 UIFlowWndWiFiMovie_OnExeSetMovieLiveviewBitRate(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    char *Data = 0;
    UINT32 uiTargetBitRate;

    if(paramNum)
    {
        Data = (char *)paramArray[0];

        if (System_GetState(SYS_STATE_CURRMODE) != PRIMARY_MODE_MOVIE)
        {
            UIFlowWndWiFiMovieErrMsg("not movie mode\r\n");
            WifiCmd_UnlockString(Data);
            return NVTEVT_CONSUME;
        }

        if (1 == sscanf_s(Data, "%d", &uiTargetBitRate))
        {
            if (UIFlowWndWiFiMovie_GetStatus() != WIFI_MOV_ST_RECORD)
            {
                debug_msg("Preview target bit rate:%d KBytes\r\n", uiTargetBitRate);
                SetMovieTargetBitrate(0, UI_GetData(FL_WIFI_MOVIE_APP_PREVIEW_SIZE), uiTargetBitRate * 1024);
            }
        }
        WifiCmd_UnlockString(Data);
    }

    return NVTEVT_CONSUME;
}

INT32 UIFlowWndWiFiMovie_OnExeMovieLiveviewStart(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    BOOL startLview =0;
    UINT32 result = WIFIAPP_RET_OK;
    UINT32 curStatus = 0;
    if(paramNum>0)
    {
        startLview = paramArray[0];
    }

    if (System_GetState(SYS_STATE_CURRMODE) != PRIMARY_MODE_MOVIE)
    {
        WifiCmd_Done(WIFIFLAG_PREVIEW_DONE, WIFIAPP_RET_STATE_ERR);
        UIFlowWndWiFiMovieErrMsg("not movie mode\r\n");
        return NVTEVT_CONSUME;
    }

    curStatus = UIFlowWndWiFiMovie_GetStatus();

    if(startLview)
    {   //RTSP should be in idle state,MJPG should be in live view state
        //#NT#2016/05/06#Jeah Yen -begin
        //#NT#Support IOTP2P
        if(((curStatus == WIFI_MOV_ST_IDLE)&&(ImageApp_NetMovie_GetConfig(NETMOVIE_CFG_MODE)==NETMOVIE_TYPE_RTSP))
        ||((curStatus == WIFI_MOV_ST_IDLE)&&(ImageApp_NetMovie_GetConfig(NETMOVIE_CFG_MODE)==NETMOVIE_TYPE_IOTP2P))
        ||((curStatus == WIFI_MOV_ST_LVIEW)&&(ImageApp_NetMovie_GetConfig(NETMOVIE_CFG_MODE)==NETMOVIE_TYPE_HTTP))
        ||((curStatus == WIFI_MOV_ST_LVIEW)&&(ImageApp_NetMovie_GetConfig(NETMOVIE_CFG_MODE)==NETMOVIE_TYPE_RTSP_MJPG))
        ||((curStatus == WIFI_MOV_ST_LVIEW)&&(ImageApp_NetMovie_GetConfig(NETMOVIE_CFG_MODE)==NETMOVIE_TYPE_IOTP2P_MJPG)))
        //#NT#2016/05/06#Jeah Yen -end
        {
            //check some menu item change ,we need to re open
            BOOL bReOpen = FlowMovie_CheckReOpenItem();

            if(bReOpen==TRUE)
            {
                Ux_SendEvent(0,NVTEVT_SYSTEM_MODE, 1, System_GetState(SYS_STATE_CURRMODE));
            }
            else
            {
                FlowWiFiMovie_StopRec();
            }
        }
        else if(curStatus==WIFI_MOV_ST_RECORD)
        {
            result = WIFIAPP_RET_FAIL;
            UIFlowWndWiFiMovieErrMsg("recording\r\n");
        }

    }
    else
    {
        if(curStatus==WIFI_MOV_ST_LVIEW)
        {
            FlowWiFiMovie_Stop2Idle();
        }
        else if(curStatus==WIFI_MOV_ST_RECORD)
        {
            result = WIFIAPP_RET_FAIL;
            UIFlowWndWiFiMovieErrMsg("recording\r\n");
        }
    }

    WifiCmd_Done(WIFIFLAG_PREVIEW_DONE,result); // signal wi-fi app cmd is done.
    return NVTEVT_CONSUME;
}




INT32 UIFlowWndWiFiMovie_OnExeMovieTriggerRawEnc(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 result = WIFIAPP_RET_OK;
    //UINT32 curStatus = 0;
    UINT32 uiJpegWidth, uiJpegHeight, uiDAR, uiMovieSize;

    if (System_GetState(SYS_STATE_CURRMODE) != PRIMARY_MODE_MOVIE)
    {
        WifiCmd_Done(WIFIFLAG_PREVIEW_DONE, WIFIAPP_RET_STATE_ERR);
        UIFlowWndWiFiMovieErrMsg("not movie mode\r\n");
        return NVTEVT_CONSUME;
    }

    //curStatus = UIFlowWndWiFiMovie_GetStatus();
    if(MovRec_IsRecording())//curStatus == WIFI_MOV_ST_RECORD)
    {
        uiMovieSize = UI_GetData(FL_MOVIE_SIZE);
        uiJpegWidth = GetMovieSizeWidth(uiMovieSize);
        uiJpegHeight = GetMovieSizeHeight(uiMovieSize);
        uiDAR = GetMovieDispAspectRatio(uiMovieSize);
        if (uiDAR == VIDENC_DAR_16_9)
        {
            uiJpegWidth = (uiJpegHeight * 16) / 9;
        }

        Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIE_REC_RAWENC, 2, uiJpegWidth, uiJpegHeight);
    }
    else
    {
        result = WIFIAPP_RET_FAIL;
        UIFlowWndWiFiMovieErrMsg("Not in recording state\r\n");

        WifiCmd_Done(WIFIFLAG_MOVIE_REC_RAWENC_DONE, result); // signal wi-fi app cmd is done.
    }

    return NVTEVT_CONSUME;
}

INT32 UIFlowWndWiFiMovie_OnMovieFull(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIFlowWndWiFiMovieErrMsg("%s \r\n",__FUNCTION__);
    WifiApp_SendCmd(WIFIAPP_CMD_NOTIFY_STATUS,WIFIAPP_RET_MOVIE_FULL);

    //moive is stopped by media record library;project need to stop RTSP
#if(WIFI_AP_FUNC==ENABLE)
    //Ux_SendEvent(&UIMovieRecObjCtrl, NVTEVT_STOP_RTSP, 0);
    //ImageApp_CamMovie_RegRTSPSetBSCB(NULL);
    //ImageUnit_SetParam(&ISF_Media, MEDIA_PARAM_BITSTREAM_CB, 0);  //obsoleted
    //RtspNvt_Close();
#endif
    FlowMovie_SetRecCurrTime(0);
    FlowWiFiMovie_StopRec();

    return NVTEVT_CONSUME;
}
INT32 UIFlowWndWiFiMovie_OnMovieWrError(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIFlowWndWiFiMovieErrMsg("%s \r\n",__FUNCTION__);

    // Something wrong with card. Mark file system error status.
    UI_SetData(FL_FSStatus, FS_DISK_ERROR);
    WifiApp_SendCmd(WIFIAPP_CMD_NOTIFY_STATUS,WIFIAPP_RET_MOVIE_WR_ERROR);

    //moive is stopped by media record library;project need to stop RTSP
#if(WIFI_AP_FUNC==ENABLE)
    //Ux_SendEvent(&UIMovieRecObjCtrl, NVTEVT_STOP_RTSP, 0);
    //ImageApp_CamMovie_RegRTSPSetBSCB(NULL);
    //ImageUnit_SetParam(&ISF_Media, MEDIA_PARAM_BITSTREAM_CB, 0);  //obsoleted
    //RtspNvt_Close();
#endif
    FlowMovie_SetRecCurrTime(0);
    FlowWiFiMovie_StopRec();

    return NVTEVT_CONSUME;
}
INT32 UIFlowWndWiFiMovie_OnStorageSlow(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIFlowWndWiFiMovieErrMsg("%s \r\n",__FUNCTION__);
    WifiApp_SendCmd(WIFIAPP_CMD_NOTIFY_STATUS,WIFIAPP_RET_MOVIE_SLOW);

    //moive is stopped by media record library;project need to stop RTSP
    #if 0
    Ux_SendEvent(&UIMovieRecObjCtrl, NVTEVT_STOP_RTSP, 0);
    FlowMovie_SetRecCurrTime(0);
    FlowWiFiMovie_StartRec(WIFI_RTSP_LIVEVIEW);
    #else // Restart recording automatically when slow card event occurred.
    FlowMovie_SetRecCurrTime(0);
    FlowWiFiMovie_StartRec();
    #endif

    return NVTEVT_CONSUME;
}
INT32 UIFlowWndWiFiMovie_OnMovieOneSec(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    if (UIFlowWndWiFiMovie_GetStatus()==WIFI_MOV_ST_RECORD && paramNum)
    {
        FlowMovie_SetRecCurrTime(paramArray[0]);
        //WifiApp_SendCmd(WIFIAPP_CMD_NOTIFY_MOVIETIME,paramArray[0]);
    }

    return NVTEVT_CONSUME;
}

INT32 UIFlowWndWiFiMovie_OnMovieRawEncJpgOKCB(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 result = WIFIAPP_RET_OK;

    WifiCmd_Done(WIFIFLAG_MOVIE_REC_RAWENC_DONE, result); // signal wi-fi app cmd is done.

    return NVTEVT_CONSUME;
}

INT32 UIFlowWndWiFiMovie_OnMovieRawEncErr(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 result = WIFIAPP_RET_FAIL;

    WifiCmd_Done(WIFIFLAG_MOVIE_REC_RAWENC_DONE, result); // signal wi-fi app cmd is done.

    return NVTEVT_CONSUME;
}

INT32 UIFlowWndWiFiMovie_GetStatus(void)
{
    UINT32 status;
    //#NT#2016/05/06#Jeah Yen -begin
    //#NT#Support IOTP2P
    if((ImageApp_NetMovie_GetConfig(NETMOVIE_CFG_MODE)==NETMOVIE_TYPE_RTSP)
    || (ImageApp_NetMovie_GetConfig(NETMOVIE_CFG_MODE)==NETMOVIE_TYPE_IOTP2P))
    //#NT#2016/05/06#Jeah Yen -end
    {
        if(UI_GetData(FL_WIFI_MOVIE_FMT)==WIFI_RTSP_LIVEVIEW)
            status = WIFI_MOV_ST_LVIEW;
        else if(UI_GetData(FL_WIFI_MOVIE_FMT)==WIFI_RTSP_REC)
            status = WIFI_MOV_ST_RECORD;
        else
            status = WIFI_MOV_ST_IDLE;
    }
    else
    {
        if(MovRec_IsRecording()==TRUE)
            status = WIFI_MOV_ST_RECORD;
        else
            status = WIFI_MOV_ST_LVIEW;
    }

    //DBGD(status);
    return status;
}


