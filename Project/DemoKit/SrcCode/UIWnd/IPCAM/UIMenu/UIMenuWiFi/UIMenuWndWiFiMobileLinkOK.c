//This source code is generated by UI Designer Studio.

#include "stdio.h"
#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "NVTToolCommand.h"
#include "UIMenuWndWiFiMobileLinkOKRes.c"
#include "WifiAppCmd.h"
#include "UIFlow.h"
#include "UIAppNetwork.h"
#include "rtc.h"
#include "ImageApp_NetMovie.h"
#include "ImageApp_NetPhoto.h"
#include "rsc_common.h"

//---------------------UIMenuWndWiFiMobileLinkOKCtrl Debug Definition -----------------------------
#define _UIMENUWNDWIFIMOBILELINKOK_ERROR_MSG        1
#define _UIMENUWNDWIFIMOBILELINKOK_TRACE_MSG        0

#if (_UIMENUWNDWIFIMOBILELINKOK_ERROR_MSG&(PRJ_DBG_LVL>=PRJ_DBG_LVL_ERR))
#define UIMenuWndWiFiMobileLinkOKErrMsg(...)            debug_msg ("^R UIMenuWndWiFiMobileLinkOK: "__VA_ARGS__)
#else
#define UIMenuWndWiFiMobileLinkOKErrMsg(...)
#endif

#if (_UIMENUWNDWIFIMOBILELINKOK_TRACE_MSG&(PRJ_DBG_LVL>=PRJ_DBG_LVL_TRC))
#define UIMenuWndWiFiMobileLinkOKTraceMsg(...)          debug_msg ("^B UIMenuWndWiFiMobileLinkOK: "__VA_ARGS__)
#else
#define UIMenuWndWiFiMobileLinkOKTraceMsg(...)
#endif

//---------------------UIMenuWndWiFiMobileLinkOKCtrl Global Variables -----------------------------
static UINT32  gUIMenuWndWiFiMotionDetTimerID = NULL_TIMER;
UINT32 bWaitMoiveVedioReady= FALSE;

//---------------------UIMenuWndWiFiMobileLinkOKCtrl Prototype Declaration  -----------------------

//---------------------UIMenuWndWiFiMobileLinkOKCtrl Public API  ----------------------------------

//---------------------UIMenuWndWiFiMobileLinkOKCtrl Private API  ---------------------------------
//---------------------UIMenuWndWiFiMobileLinkOKCtrl Control List---------------------------
CTRL_LIST_BEGIN(UIMenuWndWiFiMobileLinkOK)
CTRL_LIST_ITEM(UIMenuWndWiFiMobileLinkOK_StaticTXT_LinkSts)
CTRL_LIST_ITEM(UIMenuWndWiFiMobileLinkOK_Button_Disconnect)
CTRL_LIST_ITEM(UIMenuWndWiFiMobileLinkOK_StaticTXT_MAC)
CTRL_LIST_END
char StringTmpBuf[32] = {0};
//----------------------UIMenuWndWiFiMobileLinkOKCtrl Event---------------------------
INT32 UIMenuWndWiFiMobileLinkOK_OnOpen(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnClose(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_Authorized_OK(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnExePhotoCapture(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnUpdateInfo(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnExeSetPhotoSize(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnExeMovieRec(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnExeSetMovieRecSize(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnExeCyclicRec(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnExeSetMovieHDR(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnExeSetMovieEV(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnExeSetMovieContrast(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnExeSetMovieAudioIn(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnExeSetMovieAudioInSR(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnExeSetMovieFlipMirror(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnExeSetMovieQualitySet(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnExeSetMotionDet(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnExeSetMovieAudio(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnExeSetMovieDateImprint(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnExeSetMovieLiveviewSize(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnExeSetMovieGSesnorSensitivity(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnExeSetAutoRecording(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnExeSetMovieRecBitRate(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnExeSetMovieLiveviewBitRate(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnExeMovieLiveviewStart(VControl *, UINT32 , UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnExeTriggerMovieRawEnc(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);

INT32 UIMenuWndWiFiMobileLinkOK_OnExeModeChange(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnExeSetSSID(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnExeSetPassphrase(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnExeDate(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnExeTime(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnExePoweroff(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnExeLanguage(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnExeTV(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnExeFormat(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnExeSysReset(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnExeFWUpdate(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnExeReconnect(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnExeSaveMenu(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnExeRemoveUser(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnExePipStyle(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnExeSendSSIDPassphrase(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnExeSetWiFiConnectMode(VControl *, UINT32, UINT32 *);

INT32 UIMenuWndWiFiMobileLinkOK_OnBackgroundDone(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnMovieFull(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnMovieWrError(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnStorageSlow(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnMovieOneSec(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnExeMovieRawEncJpgOKCB(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnExeMovieRawEncErr(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnBatteryLow(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnDeAuthenticated(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnExeMovieVedioReady(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnExeChangeModeDone(VControl *, UINT32, UINT32 *);
INT32 UIMenuWndWiFiMobileLinkOK_OnTimer(VControl *, UINT32, UINT32 *);

EVENT_BEGIN(UIMenuWndWiFiMobileLinkOK)
EVENT_ITEM(NVTEVT_OPEN_WINDOW,UIMenuWndWiFiMobileLinkOK_OnOpen)
EVENT_ITEM(NVTEVT_CLOSE_WINDOW,UIMenuWndWiFiMobileLinkOK_OnClose)
EVENT_ITEM(NVTEVT_WIFI_AUTHORIZED_OK,UIMenuWndWiFiMobileLinkOK_Authorized_OK)
EVENT_ITEM(NVTEVT_WIFI_EXE_PHOTO_CAPTURE,UIMenuWndWiFiMobileLinkOK_OnExePhotoCapture)
EVENT_ITEM(NVTEVT_WIFI_EXE_PHOTO_SIZE,UIMenuWndWiFiMobileLinkOK_OnExeSetPhotoSize)
EVENT_ITEM(NVTEVT_UPDATE_INFO,UIMenuWndWiFiMobileLinkOK_OnUpdateInfo)

EVENT_ITEM(NVTEVT_WIFI_EXE_MOVIE_REC,UIMenuWndWiFiMobileLinkOK_OnExeMovieRec)
EVENT_ITEM(NVTEVT_WIFI_EXE_MOVIE_REC_SIZE,UIMenuWndWiFiMobileLinkOK_OnExeSetMovieRecSize)
EVENT_ITEM(NVTEVT_WIFI_EXE_CYCLIC_REC,UIMenuWndWiFiMobileLinkOK_OnExeCyclicRec)
EVENT_ITEM(NVTEVT_WIFI_EXE_MOVIE_WDR,UIMenuWndWiFiMobileLinkOK_OnExeSetMovieHDR)
EVENT_ITEM(NVTEVT_WIFI_EXE_MOVIE_EV,UIMenuWndWiFiMobileLinkOK_OnExeSetMovieEV)
//#NT#2016/06/03#Charlie Chang -begin
//#NT# support contrast two-way audio, flip_mirror, quality set
EVENT_ITEM(NVTEVT_WIFI_EXE_MOVIE_CONTRAST,UIMenuWndWiFiMobileLinkOK_OnExeSetMovieContrast)
EVENT_ITEM(NVTEVT_WIFI_EXE_MOVIE_AUDIOIN,UIMenuWndWiFiMobileLinkOK_OnExeSetMovieAudioIn)
EVENT_ITEM(NVTEVT_WIFI_EXE_MOVIE_AUDIOIN_SR,UIMenuWndWiFiMobileLinkOK_OnExeSetMovieAudioInSR)
EVENT_ITEM(NVTEVT_WIFI_EXE_MOVIE_FLIP_MIRROR,UIMenuWndWiFiMobileLinkOK_OnExeSetMovieFlipMirror)
EVENT_ITEM(NVTEVT_WIFI_EXE_MOVIE_QUALITY_SET,UIMenuWndWiFiMobileLinkOK_OnExeSetMovieQualitySet)
//#NT#2016/06/03#Charlie Chang -end
EVENT_ITEM(NVTEVT_WIFI_EXE_MOTION_DET,UIMenuWndWiFiMobileLinkOK_OnExeSetMotionDet)
EVENT_ITEM(NVTEVT_WIFI_EXE_MOVIE_AUDIO,UIMenuWndWiFiMobileLinkOK_OnExeSetMovieAudio)
EVENT_ITEM(NVTEVT_WIFI_EXE_DATEIMPRINT,UIMenuWndWiFiMobileLinkOK_OnExeSetMovieDateImprint)
EVENT_ITEM(NVTEVT_WIFI_EXE_MOVIE_LIVEVIEW_SIZE, UIMenuWndWiFiMobileLinkOK_OnExeSetMovieLiveviewSize)
EVENT_ITEM(NVTEVT_WIFI_EXE_MOVIE_GSENSOR_SENS, UIMenuWndWiFiMobileLinkOK_OnExeSetMovieGSesnorSensitivity)
EVENT_ITEM(NVTEVT_WIFI_EXE_SET_AUTO_RECORDING, UIMenuWndWiFiMobileLinkOK_OnExeSetAutoRecording)
EVENT_ITEM(NVTEVT_WIFI_EXE_MOVIE_REC_BITRATE, UIMenuWndWiFiMobileLinkOK_OnExeSetMovieRecBitRate)
EVENT_ITEM(NVTEVT_WIFI_EXE_MOVIE_LIVEVIEW_BITRATE, UIMenuWndWiFiMobileLinkOK_OnExeSetMovieLiveviewBitRate)
EVENT_ITEM(NVTEVT_WIFI_EXE_MOVIE_LIVEVIEW_START, UIMenuWndWiFiMobileLinkOK_OnExeMovieLiveviewStart)
EVENT_ITEM(NVTEVT_WIFI_EXE_MOVIE_TRIGGER_RAWENC, UIMenuWndWiFiMobileLinkOK_OnExeTriggerMovieRawEnc)

EVENT_ITEM(NVTEVT_WIFI_EXE_MODE,UIMenuWndWiFiMobileLinkOK_OnExeModeChange)
EVENT_ITEM(NVTEVT_WIFI_EXE_SET_SSID,UIMenuWndWiFiMobileLinkOK_OnExeSetSSID)
EVENT_ITEM(NVTEVT_WIFI_EXE_SET_PASSPHRASE,UIMenuWndWiFiMobileLinkOK_OnExeSetPassphrase)
EVENT_ITEM(NVTEVT_WIFI_EXE_SET_DATE,UIMenuWndWiFiMobileLinkOK_OnExeDate)
EVENT_ITEM(NVTEVT_WIFI_EXE_SET_TIME,UIMenuWndWiFiMobileLinkOK_OnExeTime)
EVENT_ITEM(NVTEVT_WIFI_EXE_POWEROFF,UIMenuWndWiFiMobileLinkOK_OnExePoweroff)
EVENT_ITEM(NVTEVT_WIFI_EXE_LANGUAGE,UIMenuWndWiFiMobileLinkOK_OnExeLanguage)
EVENT_ITEM(NVTEVT_WIFI_EXE_TVFORMAT,UIMenuWndWiFiMobileLinkOK_OnExeTV)
EVENT_ITEM(NVTEVT_WIFI_EXE_FORMAT,UIMenuWndWiFiMobileLinkOK_OnExeFormat)
EVENT_ITEM(NVTEVT_WIFI_EXE_SYSRESET,UIMenuWndWiFiMobileLinkOK_OnExeSysReset)
EVENT_ITEM(NVTEVT_WIFI_EXE_FWUPDATE,UIMenuWndWiFiMobileLinkOK_OnExeFWUpdate)
EVENT_ITEM(NVTEVT_WIFI_EXE_RECONNECT,UIMenuWndWiFiMobileLinkOK_OnExeReconnect)
EVENT_ITEM(NVTEVT_WIFI_EXE_SAVEMENU,UIMenuWndWiFiMobileLinkOK_OnExeSaveMenu)
EVENT_ITEM(NVTEVT_WIFI_EXE_REMOVE_USER,UIMenuWndWiFiMobileLinkOK_OnExeRemoveUser)
EVENT_ITEM(NVTEVT_WIFI_EXE_PIP_STYLE,UIMenuWndWiFiMobileLinkOK_OnExePipStyle)
EVENT_ITEM(NVTEVT_WIFI_EXE_SEND_SSID_PASSPHRASE,UIMenuWndWiFiMobileLinkOK_OnExeSendSSIDPassphrase)
EVENT_ITEM(NVTEVT_WIFI_EXE_SET_WIFI_CONNECT_MODE,UIMenuWndWiFiMobileLinkOK_OnExeSetWiFiConnectMode)

EVENT_ITEM(NVTEVT_BACKGROUND_DONE,UIMenuWndWiFiMobileLinkOK_OnBackgroundDone)
EVENT_ITEM(NVTEVT_CB_MOVIE_OVERTIME,UIMenuWndWiFiMobileLinkOK_OnMovieFull) // the same handling as storage full (may need to show special message)
EVENT_ITEM(NVTEVT_CB_MOVIE_FULL,UIMenuWndWiFiMobileLinkOK_OnMovieFull)
EVENT_ITEM(NVTEVT_CB_MOVIE_LOOPREC_FULL,UIMenuWndWiFiMobileLinkOK_OnMovieFull)
EVENT_ITEM(NVTEVT_CB_MOVIE_WR_ERROR,UIMenuWndWiFiMobileLinkOK_OnMovieWrError)
EVENT_ITEM(NVTEVT_CB_MOVIE_SLOW,UIMenuWndWiFiMobileLinkOK_OnStorageSlow)
EVENT_ITEM(NVTEVT_CB_MOVIE_REC_ONE_SEC,UIMenuWndWiFiMobileLinkOK_OnMovieOneSec)
EVENT_ITEM(NVTEVT_CB_RAWENC_OK, UIMenuWndWiFiMobileLinkOK_OnExeMovieRawEncJpgOKCB)
EVENT_ITEM(NVTEVT_CB_RAWENC_ERR, UIMenuWndWiFiMobileLinkOK_OnExeMovieRawEncErr)
EVENT_ITEM(NVTEVT_CB_RAWENC_WRITE_ERR, UIMenuWndWiFiMobileLinkOK_OnExeMovieRawEncErr)
EVENT_ITEM(NVTEVT_CB_RAWENC_DCF_FULL, UIMenuWndWiFiMobileLinkOK_OnExeMovieRawEncErr)
EVENT_ITEM(NVTEVT_BATTERY_LOW,UIMenuWndWiFiMobileLinkOK_OnBatteryLow)
EVENT_ITEM(NVTEVT_WIFI_DEAUTHENTICATED,UIMenuWndWiFiMobileLinkOK_OnDeAuthenticated)
EVENT_ITEM(NVTEVT_CB_MOVIE_VEDIO_READY, UIMenuWndWiFiMobileLinkOK_OnExeMovieVedioReady)
EVENT_ITEM(NVTEVT_WIFI_EXE_MODE_DONE, UIMenuWndWiFiMobileLinkOK_OnExeChangeModeDone)
EVENT_ITEM(NVTEVT_TIMER,UIMenuWndWiFiMobileLinkOK_OnTimer)

EVENT_END
extern char gLastMacAddr[];
INT32 UIMenuWndWiFiMobileLinkOK_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    static char Buf1[32] = {0},buf2[32] = {0} ;

    Ux_FlushEventByRange(NVTEVT_KEY_EVT_START,NVTEVT_KEY_EVT_END);
    Input_SetKeyMask(KEY_PRESS, FLGKEY_KEY_MASK_DEFAULT);
    Input_SetKeyMask(KEY_RELEASE, 0);
    Input_SetKeyMask(KEY_CONTINUE, 0);

    #if(_NETWORK_ == _NETWORK_SDIO_EVB_WIFI_)
    snprintf(Buf1,sizeof(Buf1),"WiFi Connected");
    snprintf(StringTmpBuf,sizeof(StringTmpBuf)," MAC : %s",UINet_GetConnectedMAC());
    #else
    snprintf(Buf1,sizeof(Buf1),"Ethernet Connected");
    snprintf(StringTmpBuf,sizeof(StringTmpBuf)," IP : %s",UINet_GetIP());
    #endif
    UxStatic_SetData(&UIMenuWndWiFiMobileLinkOK_StaticTXT_LinkStsCtrl,STATIC_VALUE,Txt_Pointer(Buf1));
    UxStatic_SetData(&UIMenuWndWiFiMobileLinkOK_StaticTXT_MACCtrl,STATIC_VALUE,Txt_Pointer(StringTmpBuf));

    snprintf(buf2,sizeof(buf2),"Press Select to Disconnect");
    UxButton_SetItemData(&UIMenuWndWiFiMobileLinkOK_Button_DisconnectCtrl, 0, BTNITM_STRID, Txt_Pointer(buf2));

    if (gUIMenuWndWiFiMotionDetTimerID==NULL_TIMER)
    {
        gUIMenuWndWiFiMotionDetTimerID = GxTimer_StartTimer(TIMER_HALF_SEC, NVTEVT_05SEC_TIMER, CONTINUE);
    }
    //make sure, connec to RTSP mode
    if (System_GetState(SYS_STATE_CURRMODE) != PRIMARY_MODE_MOVIE)
    {
        Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_MOVIE);
    }

    WifiCmd_ReceiveCmd(TRUE);
    Ux_DefaultEvent(pCtrl,NVTEVT_OPEN_WINDOW,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndWiFiMobileLinkOK_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    if(UI_GetData(FL_MOVIE_SIZE_MENU) != UI_GetData(FL_MOVIE_SIZE))
    {
        Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIESIZE, 1, UI_GetData(FL_MOVIE_SIZE_MENU));
    }
    WifiCmd_ReceiveCmd(FALSE);
    if (gUIMenuWndWiFiMotionDetTimerID!=NULL_TIMER)
    {
        GxTimer_StopTimer(&gUIMenuWndWiFiMotionDetTimerID);
    }

    Ux_DefaultEvent(pCtrl,NVTEVT_CLOSE_WINDOW,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndWiFiMobileLinkOK_Authorized_OK(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 bNewUser = 0 ;
    if(paramNum)
        bNewUser = paramArray[0];

    snprintf(StringTmpBuf,sizeof(StringTmpBuf), " MAC : %s",UINet_GetConnectedMAC());
    UxStatic_SetData(&UIMenuWndWiFiMobileLinkOK_StaticTXT_MACCtrl,STATIC_VALUE,Txt_Pointer(StringTmpBuf));

    if(bNewUser)
    {
        //delay 500 ms,disconnect last user and
        //in window,use GxTimer replace Timer Delay,it would occupat UI task
        GxTimer_StartTimer(TIMER_HALF_SEC, NVTEVT_DISCONNECT_LAST_TIMER, ONE_SHOT);
    }

    #if 0 // Moved to UINet_DhcpSrvCBCliConn.
    //make sure, connec to RTSP mode
    if (System_GetState(SYS_STATE_CURRMODE) != PRIMARY_MODE_MOVIE)
    {
        Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_MOVIE);
    }
    #endif

    return NVTEVT_CONSUME;
}


INT32 UIMenuWndWiFiMobileLinkOK_OnExePhotoCapture(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return UIFlowWndWiFiPhoto_OnExePhotoCapture(pCtrl, paramNum, paramArray);
}
INT32 UIMenuWndWiFiMobileLinkOK_OnUpdateInfo(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return UIFlowWndWiFiPhoto_OnUpdateInfo(pCtrl, paramNum, paramArray);
}
INT32 UIMenuWndWiFiMobileLinkOK_OnExeSetPhotoSize(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return UIFlowWndWiFiPhoto_OnExeSetPhotoSize(pCtrl, paramNum, paramArray);
}

/************************************************************************/
INT32 UIMenuWndWiFiMobileLinkOK_OnExeMovieRec(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIMenuWndWiFiMobileLinkOKTraceMsg("%s %d\r\n",__FUNCTION__,paramArray[0]);
    return UIFlowWndWiFiMovie_OnExeMovieRec(pCtrl, paramNum, paramArray);
}
INT32 UIMenuWndWiFiMobileLinkOK_OnExeSetMovieRecSize(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIMenuWndWiFiMobileLinkOKTraceMsg("%s %d\r\n",__FUNCTION__,paramArray[0]);
    return UIFlowWndWiFiMovie_OnExeSetMovieRecSize(pCtrl, paramNum, paramArray);
}
INT32 UIMenuWndWiFiMobileLinkOK_OnExeCyclicRec(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIMenuWndWiFiMobileLinkOKTraceMsg("%s %d\r\n",__FUNCTION__,paramArray[0]);
    return UIFlowWndWiFiMovie_OnExeSetCyclicRec(pCtrl, paramNum, paramArray);
}
INT32 UIMenuWndWiFiMobileLinkOK_OnExeSetMovieHDR(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIMenuWndWiFiMobileLinkOKTraceMsg("%s %d\r\n",__FUNCTION__,paramArray[0]);
    return UIFlowWndWiFiMovie_OnExeSetHDR(pCtrl, paramNum, paramArray);
}
INT32 UIMenuWndWiFiMobileLinkOK_OnExeSetMovieEV(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIMenuWndWiFiMobileLinkOKTraceMsg("%s %d\r\n",__FUNCTION__,paramArray[0]);
    return UIFlowWndWiFiMovie_OnExeSetMovieEV(pCtrl, paramNum, paramArray);
}
//#NT#2016/06/03#Charlie Chang -begin
//#NT# support contrast, twoway audio, flip_mirror, quality set
INT32 UIMenuWndWiFiMobileLinkOK_OnExeSetMovieContrast(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIMenuWndWiFiMobileLinkOKTraceMsg("%s %d\r\n",__FUNCTION__,paramArray[0]);
    return UIFlowWndWiFiMovie_OnExeSetMovieContrast(pCtrl, paramNum, paramArray);
}
INT32 UIMenuWndWiFiMobileLinkOK_OnExeSetMovieAudioIn(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIMenuWndWiFiMobileLinkOKTraceMsg("%s %d\r\n",__FUNCTION__,paramArray[0]);
    return UIFlowWndWiFiMovie_OnExeSetMovieAudioIn(pCtrl, paramNum, paramArray);
}
INT32 UIMenuWndWiFiMobileLinkOK_OnExeSetMovieAudioInSR(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIMenuWndWiFiMobileLinkOKTraceMsg("%s %d\r\n",__FUNCTION__,paramArray[0]);
    return UIFlowWndWiFiMovie_OnExeSetMovieAudioInSR(pCtrl, paramNum, paramArray);
}
INT32 UIMenuWndWiFiMobileLinkOK_OnExeSetMovieFlipMirror(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIMenuWndWiFiMobileLinkOKTraceMsg("%s %d\r\n",__FUNCTION__,paramArray[0]);
    return UIFlowWndWiFiMovie_OnExeSetMovieFlipMirror(pCtrl, paramNum, paramArray);
}
INT32 UIMenuWndWiFiMobileLinkOK_OnExeSetMovieQualitySet(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{

    UIMenuWndWiFiMobileLinkOKTraceMsg("%s %d\r\n",__FUNCTION__,paramArray[0]);
    return UIFlowWndWiFiMovie_OnExeSetMovieQualitySet(pCtrl, paramNum, paramArray);
}
//#NT#2016/06/03#Charlie Chang -end
INT32 UIMenuWndWiFiMobileLinkOK_OnExeSetMotionDet(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIMenuWndWiFiMobileLinkOKTraceMsg("%s %d\r\n",__FUNCTION__,paramArray[0]);
    return UIFlowWndWiFiMovie_OnExeSetMotionDet(pCtrl, paramNum, paramArray);
}
INT32 UIMenuWndWiFiMobileLinkOK_OnExeSetMovieAudio(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIMenuWndWiFiMobileLinkOKTraceMsg("%s %d\r\n",__FUNCTION__,paramArray[0]);
    return UIFlowWndWiFiMovie_OnExeSetMovieAudio(pCtrl, paramNum, paramArray);
}
INT32 UIMenuWndWiFiMobileLinkOK_OnExeSetMovieDateImprint(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIMenuWndWiFiMobileLinkOKTraceMsg("%s %d\r\n",__FUNCTION__,paramArray[0]);
    return UIFlowWndWiFiMovie_OnExeSetMovieDateImprint(pCtrl, paramNum, paramArray);
}
INT32 UIMenuWndWiFiMobileLinkOK_OnExeSetMovieLiveviewSize(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIMenuWndWiFiMobileLinkOKTraceMsg("%s %d\r\n",__FUNCTION__,paramArray[0]);
    return UIFlowWndWiFiMovie_OnExeSetMovieLiveviewSize(pCtrl, paramNum, paramArray);
}
INT32 UIMenuWndWiFiMobileLinkOK_OnExeSetMovieGSesnorSensitivity(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIMenuWndWiFiMobileLinkOKTraceMsg("%s %d\r\n",__FUNCTION__,paramArray[0]);
    return UIFlowWndWiFiMovie_OnExeSetMovieGSesnorSensitivity(pCtrl, paramNum, paramArray);
}
INT32 UIMenuWndWiFiMobileLinkOK_OnExeSetAutoRecording(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIMenuWndWiFiMobileLinkOKTraceMsg("%s %d\r\n",__FUNCTION__,paramArray[0]);
    return UIFlowWndWiFiMovie_OnExeSetAutoRecording(pCtrl, paramNum, paramArray);
}
INT32 UIMenuWndWiFiMobileLinkOK_OnExeSetMovieRecBitRate(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIMenuWndWiFiMobileLinkOKTraceMsg("%s %d\r\n",__FUNCTION__,paramArray[0]);
    return UIFlowWndWiFiMovie_OnExeSetMovieRecBitRate(pCtrl, paramNum, paramArray);
}
INT32 UIMenuWndWiFiMobileLinkOK_OnExeSetMovieLiveviewBitRate(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIMenuWndWiFiMobileLinkOKTraceMsg("%s %d\r\n",__FUNCTION__,paramArray[0]);
    return UIFlowWndWiFiMovie_OnExeSetMovieLiveviewBitRate(pCtrl, paramNum, paramArray);
}
INT32 UIMenuWndWiFiMobileLinkOK_OnExeMovieLiveviewStart(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIMenuWndWiFiMobileLinkOKTraceMsg("%s %d\r\n",__FUNCTION__,paramArray[0]);
    return UIFlowWndWiFiMovie_OnExeMovieLiveviewStart(pCtrl, paramNum, paramArray);
}

INT32 UIMenuWndWiFiMobileLinkOK_OnExeTriggerMovieRawEnc(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIMenuWndWiFiMobileLinkOKTraceMsg("%s %d\r\n",__FUNCTION__,paramArray[0]);
    return UIFlowWndWiFiMovie_OnExeMovieTriggerRawEnc(pCtrl, paramNum, paramArray);
}

/************************************************************************/


INT32 UIMenuWndWiFiMobileLinkOK_OnExeModeChange(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 par = 0;
    INT32 curMode = System_GetState(SYS_STATE_CURRMODE);
    if(paramNum>0)
    {
        par=paramArray[0];
    }

    switch(par)
    {
        case WIFI_APP_MODE_PHOTO:
            //#NT#2015/07/24#KS Hung -begin
            if(curMode != PRIMARY_MODE_PHOTO)
            {
                if (curMode == PRIMARY_MODE_MOVIE)
                {
                    MovieExe_RSC_SetSwitch(SEL_RSC_RUNTIME, SEL_RSC_OFF);
                }
                Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_PHOTO);
            }
            else
                WifiCmd_Done(WIFIFLAG_MODE_DONE,WIFIAPP_RET_OK);
            //#NT#2015/07/24#KS Hung -end
        break;
        case WIFI_APP_MODE_MOVIE:
            //#NT#2015/07/24#KS Hung -begin
            if(curMode != PRIMARY_MODE_MOVIE)
            {
                if (curMode == PRIMARY_MODE_PHOTO)
                {
                    PhotoExe_RSC_SetSwitch(SEL_RSC_RUNTIME, SEL_RSC_OFF);
                }
                Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_MOVIE);
            }
            //#NT#2015/07/24#KS Hung -end
            else
                WifiCmd_Done(WIFIFLAG_MODE_DONE,WIFIAPP_RET_OK);
        break;
        case WIFI_APP_MODE_PLAYBACK:
            if(curMode != PRIMARY_MODE_PLAYBACK)
                Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_PLAYBACK);
            else
                WifiCmd_Done(WIFIFLAG_MODE_DONE,WIFIAPP_RET_OK);
        break;
        default:
            WifiCmd_Done(WIFIFLAG_MODE_DONE,WIFIAPP_RET_PAR_ERR);
        break;
    }

    return NVTEVT_CONSUME;
}

INT32 UIMenuWndWiFiMobileLinkOK_OnExeSetSSID(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIMenuWndWiFiMobileLinkOKTraceMsg("%s %s\r\n",__FUNCTION__,paramArray[0]);
    char *Data = 0;
    char SSID[WIFI_PAR_STR_LEN] ={0};
    if(paramNum)
    {
        Data = (char *)paramArray[0];
        UIMenuWndWiFiMobileLinkOKTraceMsg("%d\r\n",strlen(Data));

        if (1 == sscanf_s(Data, "%s", SSID, sizeof(SSID)))
        {
            Ux_SendEvent(0,NVTEVT_EXE_WIFI_SET_SSID, 1, SSID);
        }
        WifiCmd_UnlockString(Data);
    }
    return NVTEVT_CONSUME;
}

INT32 UIMenuWndWiFiMobileLinkOK_OnExeSetPassphrase(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIMenuWndWiFiMobileLinkOKTraceMsg("%s %s\r\n",__FUNCTION__,paramArray[0]);
    char *Data = 0;
    char Passphrase[WIFI_PAR_STR_LEN] ={0};

    if(paramNum)
    {
        Data = (char *)paramArray[0];
        UIMenuWndWiFiMobileLinkOKTraceMsg("%d\r\n",strlen(Data));

        if (1 == sscanf_s(Data, "%s", Passphrase, sizeof(Passphrase)))
        {
            if(strlen(Passphrase) >= 8)
            {
                Ux_SendEvent(0,NVTEVT_EXE_WIFI_SET_PASSPHRASE, 1, Passphrase);
            }
            else
            {
                UIMenuWndWiFiMobileLinkOKErrMsg("Passphrase length illegal!\r\n");
            }
        }
        WifiCmd_UnlockString(Data);
    }

    return NVTEVT_CONSUME;
}

INT32 UIMenuWndWiFiMobileLinkOK_OnExeDate(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    char *Data = 0;
    UINT32 uiYear, uiMonth, uiDay;
    UIMenuWndWiFiMobileLinkOKTraceMsg("%s %s\r\n",__FUNCTION__,paramArray[0]);
    if(paramNum)
    {
        Data = (char *)paramArray[0];
        UIMenuWndWiFiMobileLinkOKTraceMsg("%d\r\n",strlen(Data));

        if (3 == sscanf_s(Data, "%d-%d-%d;", &uiYear, &uiMonth, &uiDay))
        {
            WifiCmd_UnlockString(Data);//setDate cost time,unlock string before rtc_setDate
            UIMenuWndWiFiMobileLinkOKTraceMsg("Y:%d, M:%d, D:%d\r\n", uiYear, uiMonth, uiDay);
            rtc_setDate(uiYear, uiMonth, uiDay);
            rtc_triggerCSET();
        }
        else
        {
            WifiCmd_UnlockString(Data);
        }
    }

    return NVTEVT_CONSUME;
}

INT32 UIMenuWndWiFiMobileLinkOK_OnExeTime(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    char *Data = 0;
    UINT32 uiHour, uiMin, uiSec;

    UIMenuWndWiFiMobileLinkOKTraceMsg("%s %s\r\n",__FUNCTION__,paramArray[0]);
    if(paramNum)
    {
        Data = (char *)paramArray[0];
        UIMenuWndWiFiMobileLinkOKTraceMsg("%d\r\n",strlen(Data));

        if (3 == sscanf_s(Data, "%d:%d:%d;", &uiHour, &uiMin, &uiSec))
        {
            WifiCmd_UnlockString(Data);//setTime cost time,unlock string before rtc_setTime
            UIMenuWndWiFiMobileLinkOKTraceMsg("hh:%d, mm:%d, dd:%d\r\n", uiHour, uiMin, uiSec);
            rtc_setTime(uiHour, uiMin, uiSec);
            rtc_triggerCSET();
        }
        else
        {
            WifiCmd_UnlockString(Data);
        }
    }

    return NVTEVT_CONSUME;
}

INT32 UIMenuWndWiFiMobileLinkOK_OnExePoweroff(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIMenuWndWiFiMobileLinkOKTraceMsg("%s %d\r\n",__FUNCTION__,paramArray[0]);
    UINT32 Data = 0;
    if(paramNum)
        Data = paramArray[0];
    Ux_SendEvent(0,NVTEVT_EXE_POWEROFF, 1, Data);
    return NVTEVT_CONSUME;
}

INT32 UIMenuWndWiFiMobileLinkOK_OnExeLanguage(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIMenuWndWiFiMobileLinkOKTraceMsg("%s %d\r\n",__FUNCTION__,paramArray[0]);
    UINT32 Data = 0;
    if(paramNum)
        Data = paramArray[0];
    Ux_SendEvent(0,NVTEVT_EXE_LANGUAGE, 1, Data);
    return NVTEVT_CONSUME;
}

INT32 UIMenuWndWiFiMobileLinkOK_OnExeTV(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIMenuWndWiFiMobileLinkOKTraceMsg("%s %d\r\n",__FUNCTION__,paramArray[0]);
    UINT32 Data = 0;
    if(paramNum)
        Data = paramArray[0];

    #if (TV_FUNC == ENABLE)
    Ux_SendEvent(0,NVTEVT_EXE_TVFORMAT, 1, Data);
    #else
    debug_err(("TV function not support!\r\n"));
    #endif

    return NVTEVT_CONSUME;
}
INT32 UIMenuWndWiFiMobileLinkOK_OnExeFormat(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIMenuWndWiFiMobileLinkOKTraceMsg("%s %d\r\n",__FUNCTION__,paramArray[0]);
    UINT32 Data = 0;
    if(paramNum)
        Data = paramArray[0];
    if(Data == 0)
    {
        BKG_PostEvent(NVTEVT_BKW_FORMAT_NAND);
    }
    else if(Data==1)
    {
        if(UI_GetData(FL_CardStatus) == CARD_REMOVED)
        {
            UIMenuWndWiFiMobileLinkOKErrMsg("card removed\r\n");
            WifiCmd_Done(WIFIFLAG_FORMAT_DONE,E_NOEXS);

        }
        else if(UIStorageCheck(STORAGE_CHECK_LOCKED, NULL) == TRUE)
        {
            UIMenuWndWiFiMobileLinkOKErrMsg("card lock\r\n");
            WifiCmd_Done(WIFIFLAG_FORMAT_DONE,E_OBJ);
        }
        else
        {
            BKG_PostEvent(NVTEVT_BKW_FORMAT_CARD);
        }
    }
    else
    {
        UIMenuWndWiFiMobileLinkOKErrMsg("err param\r\n");
        WifiCmd_Done(WIFIFLAG_FORMAT_DONE,E_PAR);
    }
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndWiFiMobileLinkOK_OnExeSysReset(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIMenuWndWiFiMobileLinkOKTraceMsg("%s \r\n",__FUNCTION__);
    Ux_SendEvent(0,NVTEVT_EXE_SYSRESET,0);
    return NVTEVT_CONSUME;
}

INT32 UIMenuWndWiFiMobileLinkOK_OnExeFWUpdate(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIMenuWndWiFiMobileLinkOKTraceMsg("%s \r\n",__FUNCTION__);
    BKG_PostEvent(NVTEVT_BKW_FW_UPDATE);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndWiFiMobileLinkOK_OnExeReconnect(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIMenuWndWiFiMobileLinkOKTraceMsg("%s \r\n",__FUNCTION__);

    //before stop wifi should close RTSP/HTTP,and change to movie wifi mode after wifi start
    if (System_GetState(SYS_STATE_CURRMODE)== PRIMARY_MODE_MOVIE)
    {
        ImageApp_NetMovie_Close();
    }
    else if(System_GetState(SYS_STATE_CURRMODE)== PRIMARY_MODE_PHOTO)
    {
        ImageApp_NetPhoto_Close();
    }

    //#NT#2016/12/22#YongChang Qui -begin
    //#NT#Use new rule to transmit to wifi station mode from ap mode
    UINT32 Data = 0;
    if(paramNum)
        Data = paramArray[0];
    if(Data & SEND_PEER_IP)
        UINet_SetSendPeerIP(1);
    else
        UINet_SetSendPeerIP(0);
    //#NT#2016/12/22#YongChang Qui -end

    Ux_SendEvent(0,NVTEVT_EXE_WIFI_STOP, 0);
    Ux_SendEvent(0,NVTEVT_EXE_WIFI_START, 0);

    Ux_PostEvent(NVTEVT_SYSTEM_MODE, 2, PRIMARY_MODE_MOVIE,SYS_SUBMODE_WIFI);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndWiFiMobileLinkOK_OnExeSaveMenu(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIMenuWndWiFiMobileLinkOKTraceMsg("%s \r\n",__FUNCTION__);
    Save_MenuInfo();
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndWiFiMobileLinkOK_OnExeRemoveUser(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIMenuWndWiFiMobileLinkOKTraceMsg("%s \r\n",__FUNCTION__);
    UINet_RemoveUser();
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndWiFiMobileLinkOK_OnExePipStyle(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIMenuWndWiFiMobileLinkOKTraceMsg("%s \r\n",__FUNCTION__);
    UINT32 Data = 0;
    if(paramNum)
        Data = paramArray[0];

    Ux_SendEvent(0,NVTEVT_EXE_DUALCAM, 1, Data);

    return NVTEVT_CONSUME;
}

INT32 UIMenuWndWiFiMobileLinkOK_OnExeSendSSIDPassphrase(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIMenuWndWiFiMobileLinkOKTraceMsg("%s \r\n",__FUNCTION__);
    UINT32 Data = 0;
    if(paramNum)
        Data = paramArray[0];

    Ux_SendEvent(0,NVTEVT_EXE_WIFI_SEND_SSID_PASSPHRASE, 1, Data);
    return NVTEVT_CONSUME;
}

INT32 UIMenuWndWiFiMobileLinkOK_OnExeSetWiFiConnectMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIMenuWndWiFiMobileLinkOKTraceMsg("%s \r\n",__FUNCTION__);
    UINT32 Data = 0;
    if(paramNum)
        Data = paramArray[0];

    Ux_SendEvent(0,NVTEVT_EXE_WIFI_MODE, 1, Data);
    return NVTEVT_CONSUME;
}

INT32 UIMenuWndWiFiMobileLinkOK_OnBackgroundDone(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    NVTEVT event;
    UINT32 status;

    event = paramArray[ONDONE_PARAM_INDEX_CMD];
    status = paramArray[ONDONE_PARAM_INDEX_RET];
    debug_err(("%s event = 0x%x, status=%d\r\n",__FUNCTION__, event, status));

    switch(event)
    {
        case NVTEVT_BKW_FORMAT_CARD:
        {
            WifiCmd_Done(WIFIFLAG_FORMAT_DONE,WIFIAPP_RET_OK);
            //Rsvd
            break;
        }

        case NVTEVT_BKW_FORMAT_NAND:
        {
            WifiCmd_Done(WIFIFLAG_FORMAT_DONE,WIFIAPP_RET_OK);
            //Rsvd
            break;
        }

        case NVTEVT_BKW_FW_UPDATE:
        {
            if(status)
            {
                debug_err(("%s:update FW fail %d\r\n",__FUNCTION__,status));
                WifiCmd_Done(WIFIFLAG_UPDATE_DONE,status+WIFIAPP_RET_FW_OFFSET);
            }
            else
            {
                WifiCmd_Done(WIFIFLAG_UPDATE_DONE,WIFIAPP_RET_OK);
                Delay_DelayMs(2000);
                // Should power off immediatelly
                System_PowerOff(SYS_POWEROFF_NORMAL);
            }
            break;
        }

        default:
            debug_err(("%s:Unknown event 0x%x\r\n",__FUNCTION__, event));
            break;
    }
    return NVTEVT_CONSUME;
}

INT32 UIMenuWndWiFiMobileLinkOK_OnMovieFull(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    snprintf(StringTmpBuf,sizeof(StringTmpBuf),"Card Full");
    UxStatic_SetData(&UIMenuWndWiFiMobileLinkOK_StaticTXT_MACCtrl,STATIC_VALUE,Txt_Pointer(StringTmpBuf));
    return UIFlowWndWiFiMovie_OnMovieFull(pCtrl, paramNum, paramArray);
}
INT32 UIMenuWndWiFiMobileLinkOK_OnMovieWrError(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    snprintf(StringTmpBuf,sizeof(StringTmpBuf),"Card Error");
    UxStatic_SetData(&UIMenuWndWiFiMobileLinkOK_StaticTXT_MACCtrl,STATIC_VALUE,Txt_Pointer(StringTmpBuf));

    return UIFlowWndWiFiMovie_OnMovieWrError(pCtrl, paramNum, paramArray);
}
INT32 UIMenuWndWiFiMobileLinkOK_OnStorageSlow(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    snprintf(StringTmpBuf,sizeof(StringTmpBuf),"Slow Card");
    UxStatic_SetData(&UIMenuWndWiFiMobileLinkOK_StaticTXT_MACCtrl,STATIC_VALUE,Txt_Pointer(StringTmpBuf));

    return UIFlowWndWiFiMovie_OnStorageSlow(pCtrl, paramNum, paramArray);
}
INT32 UIMenuWndWiFiMobileLinkOK_OnMovieOneSec(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return UIFlowWndWiFiMovie_OnMovieOneSec(pCtrl, paramNum, paramArray);
}

INT32 UIMenuWndWiFiMobileLinkOK_OnExeMovieRawEncJpgOKCB(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return UIFlowWndWiFiMovie_OnMovieRawEncJpgOKCB(pCtrl, paramNum, paramArray);
}

INT32 UIMenuWndWiFiMobileLinkOK_OnExeMovieRawEncErr(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return UIFlowWndWiFiMovie_OnMovieRawEncErr(pCtrl, paramNum, paramArray);
}

INT32 UIMenuWndWiFiMobileLinkOK_OnBatteryLow(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIMenuWndWiFiMobileLinkOKTraceMsg("%s \r\n",__FUNCTION__);

    snprintf(StringTmpBuf,sizeof(StringTmpBuf),"Battery Low");
    UxStatic_SetData(&UIMenuWndWiFiMobileLinkOK_StaticTXT_MACCtrl,STATIC_VALUE,Txt_Pointer(StringTmpBuf));

    WifiApp_SendCmd(WIFIAPP_CMD_NOTIFY_STATUS,WIFIAPP_RET_BATTERY_LOW);
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndWiFiMobileLinkOK_OnDeAuthenticated(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{

    snprintf(StringTmpBuf,sizeof(StringTmpBuf),"No connect");
    UxStatic_SetData(&UIMenuWndWiFiMobileLinkOK_StaticTXT_MACCtrl,STATIC_VALUE,Txt_Pointer(StringTmpBuf));
#if (_NETWORK_ == _NETWORK_SDIO_EVB_WIFI_)
    UINet_CliReConnect(1);
#endif
    return NVTEVT_CONSUME;

    //Ux_CloseWindow(pCtrl,0);

    return NVTEVT_CONSUME;
}
INT32 UIMenuWndWiFiMobileLinkOK_OnExeMovieVedioReady(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    if(ImageApp_NetMovie_GetConfig(NETMOVIE_CFG_MODE)==NETMOVIE_TYPE_RTSP)
    {
        if(paramNum && paramArray[0]==1)
        {
            if(bWaitMoiveVedioReady)
            {
               WifiCmd_Done(WIFIFLAG_MODE_DONE,WIFIAPP_RET_OK);
               bWaitMoiveVedioReady = FALSE;
            }
            else
            {
                WifiCmd_Done(WIFIFLAG_RECORD_DONE,WIFIAPP_RET_OK);
            }
        }
    }
    return NVTEVT_CONSUME;
}
INT32 UIMenuWndWiFiMobileLinkOK_OnExeChangeModeDone(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    if(ImageApp_NetMovie_GetConfig(NETMOVIE_CFG_MODE)==NETMOVIE_TYPE_RTSP)
    {
        if(System_GetState(SYS_STATE_CURRMODE)!=PRIMARY_MODE_MOVIE)
        {
            WifiCmd_Done(WIFIFLAG_MODE_DONE,WIFIAPP_RET_OK);
        }
        else
        {
            bWaitMoiveVedioReady = TRUE;
        }
    }
    else
    {
        WifiCmd_Done(WIFIFLAG_MODE_DONE,WIFIAPP_RET_OK);
    }

    return NVTEVT_CONSUME;
}
INT32 UIMenuWndWiFiMobileLinkOK_OnTimer(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32  uiEvent;
    uiEvent = paramNum ? paramArray[0] : 0;

    switch(uiEvent)
    {
        case NVTEVT_DISCONNECT_LAST_TIMER:
            UINet_AddACLTable();
            return NVTEVT_CONSUME;
        case NVTEVT_DELAY_CLOSE_TIMER:
        {
            //#NT#2016/03/08#Niven Cho -begin
            //#NT#because it is long time closing the linux-wifi, we don't change mode to movie video ahead.
            #if(_CPU2_TYPE_ == _CPU2_LINUX_)
            UINT32 iCurrMode = System_GetState(SYS_STATE_CURRMODE);
            Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_MAIN); //change into main mode to trigger closing HFS, Rtsp, .. app.
            Ux_PostEvent(NVTEVT_EXE_WIFI_STOP, 0);
            Ux_PostEvent(NVTEVT_SYSTEM_MODE, 2, iCurrMode,SYS_SUBMODE_NORMAL);
            #else
            //change to normal mode for close app
            Ux_PostEvent(NVTEVT_SYSTEM_MODE, 2, System_GetState(SYS_STATE_CURRMODE),SYS_SUBMODE_NORMAL);
            //should close network application,then stop wifi
            Ux_PostEvent(NVTEVT_EXE_WIFI_STOP, 0);
            #endif
            //#NT#2016/03/08#Niven Cho -end
            return NVTEVT_CONSUME;
        }
        default:
            return UIFlowWndWiFiMovie_OnTimer(pCtrl, paramNum, paramArray);
    }
}
//----------------------UIMenuWndWiFiMobileLinkOK_StaticTXT_LinkStsCtrl Event---------------------------
EVENT_BEGIN(UIMenuWndWiFiMobileLinkOK_StaticTXT_LinkSts)
EVENT_END

//----------------------UIMenuWndWiFiMobileLinkOK_Button_DisconnectCtrl Event---------------------------
INT32 UIMenuWndWiFiMobileLinkOK_Button_Disconnect_OnKeyEnter(VControl *, UINT32, UINT32 *);
EVENT_BEGIN(UIMenuWndWiFiMobileLinkOK_Button_Disconnect)
EVENT_ITEM(NVTEVT_KEY_SELECT,UIMenuWndWiFiMobileLinkOK_Button_Disconnect_OnKeyEnter)
EVENT_END

INT32 UIMenuWndWiFiMobileLinkOK_Button_Disconnect_OnKeyEnter(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    #if(_NETWORK_ == _NETWORK_SDIO_EVB_WIFI_)
    //notify current connected socket,new connect not get IP and socket not create
    //cannot disconnet immediate,suggest after 500 ms
    WifiApp_SendCmd(WIFIAPP_CMD_NOTIFY_STATUS, WIFIAPP_RET_DISCONNECT);
    //delay 500 ms,and then close window
    //in window,use GxTimer replace Timer Delay,it would occupat UI task
    GxTimer_StartTimer(TIMER_HALF_SEC, NVTEVT_DELAY_CLOSE_TIMER, ONE_SHOT);
    #else

    Ux_PostEvent(NVTEVT_SYSTEM_MODE, 2, System_GetState(SYS_STATE_CURRMODE),SYS_SUBMODE_NORMAL);
    //should close network application,then stop wifi
    Ux_PostEvent(NVTEVT_EXE_WIFI_STOP, 0);

    #endif
    return NVTEVT_CONSUME;
}
//----------------------UIMenuWndWiFiMobileLinkOK_StaticTXT_MACCtrl Event---------------------------
EVENT_BEGIN(UIMenuWndWiFiMobileLinkOK_StaticTXT_MAC)
EVENT_END

