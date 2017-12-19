/*
    System Audio Callback

    System Callback for Audio Module.

    @file       SysLens_Audio.c
    @ingroup    mIPRJSYS

    @note

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIFrameworkExt.h"
#include "AppLib.h"
#include "GxSound.h"
#include "SMediaPlayAPI.h"
#include "SoundData.h"
#include "Dx.h"
#include "DxType.h"
#include "DxApi.h"
#include "DxSound.h"
#include "Audio.h"
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

#if AUDIO_NS_FUNC
#include "Aec.h"
#include "AudFilterAPI.h"
#endif

//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysAudioExe
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

void System_SetAudioOutput(void);
void Sound_CB(UINT32 event, UINT32 param1, UINT32 param2);
#if (AUDIO_PLAY_FUNCTION == ENABLE)
void WavPlay_CB(WAVSTUD_ACT act, UINT32 p1, UINT32 p2);
//#NT#2016/09/07#HM Tseng -begin
//#NT#Support half-duplex mode
void WavRecv_CB(WAVSTUD_ACT act, UINT32 p1, UINT32 p2);
//#NT#2016/09/07#HM Tseng -end
#if 0
void UserAudio_GenData(void);
BOOL UserAudio_LoadCB(PAUDIO_BUF_QUEUE pAudBufQue, UINT32 id);
#endif
#endif

DX_HANDLE gDevAudObj = 0;

#if (AUDIO_PLAY_FUNCTION == ENABLE)
WAVSTUD_AUD_INFO gAudInfo = { AUDIO_SR_48000, AUDIO_CH_LEFT, WAVSTUD_BITS_PER_SAM_16, WAVSTUDCODEC_PCM};
static BOOL gDevAudMode = TRUE; //TURE = full-duplex; FALSE = half-duplex
static BOOL gDevAudInput = FALSE; //send from network
static BOOL gDevAudPlay = FALSE; //play to device
static UINT32 gDevAudPlayVol = 100; //play volume
#define AUDIO_MAX_SAMPLERATE AUDIO_SR_48000
#define AUDIO_UNIT_TIME WAVSTUD_UNIT_TIME_100MS // 1/10 second
#define AUDIO_UNIT_SIZE (AUDIO_SR_48000/10*2) //1 //1 unit audio data, size = MAX_SR/10*2
#define AUDIO_QUEUE_SIZE (AUDIO_UNIT_SIZE*2) //for keep 2 unit 100ms audio data, queue size = unit*2
#define WAVSTUDIO_BUF_SIZE(c) (512+16*(c)+AUDIO_UNIT_SIZE*((c)+2))
static BOOL gDevAecEn = FALSE; //AEC enable
#else
#if (LCD1_FUNC == ENABLE)
extern DX_HANDLE gDevLCDObj;
#endif
#if (TV_FUNC == ENABLE)
extern DX_HANDLE gDevTVObj;
#endif
#if (HDMI_FUNC == ENABLE)
extern DX_HANDLE gDevHDMIObj;
#endif
#endif

#if AUDIO_NS_FUNC
AUDFILTER_NS_CB AudNs_CB(UINT32 p1, UINT32 p2, UINT32 p3);
BOOL gDevNsEn = FALSE;
#endif

//#NT#2016/05/23#HM Tseng -begin
//#NT#Support audio push function
BOOL System_SetDataCB(PAUDIO_BUF_QUEUE pAudBufQue, UINT32 id);
//#NT#2016/05/23#HM Tseng -end

#if AUDIO_NS_FUNC
void System_SetAudNsEnable(BOOL enable);
#endif

static AUDIO_SETTING gProjCommDxSndSet = {0};
static AUDIO_DEVICE_OBJ gProjCommDxSndDev = {0};

int SX_TIMER_DET_AIN_ID = -1;
void Audio_DetAudInsert(void);
#if (LINE_INSERT_FUNCTION == ENABLE)
SX_TIMER_ITEM(Audio_DetAudInsert, Audio_DetAudInsert, 25, FALSE)
static UINT32 audInsertMask = AOUT_DEFAULT_ATTACH_DEVICE; //default: only speaker
static BOOL gDevAudOut = FALSE; //FALSE:SPK, TRUE:LINE
#endif

void System_OnAudioInit(void)
{
    gDevAudObj = Dx_GetObject(DX_CLASS_AUDIO_EXT);
    TM_BOOT_BEGIN("audio","init");
    //PHASE-1 : Init & Open Drv or DrvExt
    {
        if(gDevAudObj)
        {
            Dx_Init(gDevAudObj, (void *)&gProjCommDxSndSet, 0, DXSOUND_VER);
            //fix for CID 44938 - begin
            if(Dx_Open(gDevAudObj)!=DX_OK)
            {
                DBG_ERR("DxAudio open fail!\r\n");
                return;
            }
            //fix for CID 44938 - end
            Dx_Control(gDevAudObj, DXSOUND_CAP_SET_DEVINITDATA, (UINT32)&gProjCommDxSndDev,(UINT32)&gProjCommDxSndSet);
            Dx_Control(gDevAudObj, DXSOUND_CAP_SET_CODEC, (UINT32)NULL, AUDIO_CODECSEL_DEFAULT);
            GxSound_Open(gDevAudObj,Sound_CB);
        }
        UISound_RegTable();
        #if (LINE_INSERT_FUNCTION == ENABLE)
        SX_TIMER_DET_AIN_ID = SxTimer_AddItem(&Timer_Audio_DetAudInsert);
        SxTimer_SetFuncActive(SX_TIMER_DET_AIN_ID, TRUE);
        #endif
    }
    //PHASE-2 : Init & Open Lib or LibExt
    {
        //config audio Output
        System_SetAudioOutput();
#if (AUDIO_PLAY_FUNCTION == ENABLE)
        if(gDevAudObj)
        {
            WAVSTUD_APPOBJ wso;
            WAVSTUD_INFO_SET wsis;
#if 0
            UserAudio_GenData();
#endif
            DBG_MSG("^GAudPlay Open - begin\r\n");
            //#NT#2016/05/23#HM Tseng -begin
            //#NT#Support audio push function
            wso.mem.uiAddr = OS_GetMempoolAddr(POOL_ID_AUD_AOUT);
            wso.mem.uiSize = OS_GetMempoolSize(POOL_ID_AUD_AOUT);
            //#NT#2016/05/23#HM Tseng -end
            wso.pWavStudCB = (WAVSTUD_CB)WavPlay_CB;
            wsis.mode = WAVSTUD_MODE_1P0R; //only play 1 stream
            wsis.playInfo = gAudInfo;
            //#NT#2016/05/23#HM Tseng -begin
            //#NT#Support audio push function
            wsis.playBufCount = 6;
            //#NT#2016/05/23#HM Tseng -end
            wsis.playUnitTime = AUDIO_UNIT_TIME;  //(WAVSTUD_UNIT_TIME_100MS ~ WAVSTUD_UNIT_TIME_1000MS)
            wsis.recInfo = gAudInfo;
            wsis.recBufCount = 0;
            wsis.recUnitTime = 0;
            WavStudio_Open(&wso, gDevAudObj, &wsis);
            DBG_MSG("^GAudPlay buffer addr = %08x, size = %08x\r\n", wso.mem.uiAddr, wso.mem.uiSize);
            DBG_MSG("^GAudPlay Open - end\r\n");

            WavStudio_SetConfig(WAVSTUD_CFG_VOL, WAVSTUD_ACT_PLAY, gDevAudPlayVol);

            //#NT#2016/12/22#HM Tseng -begin
            //#NT# Support AEC function
            #if (AUDIO_AEC_FUNC == ENABLE)
            NvtAec_Open(AUDIO_SR_16000, 1, 1);
            WavStudio_SetConfig(WAVSTUD_CFG_GETSTART_CB, WAVSTUD_ACT_PLAY, (UINT32)NvtAec_GetPlayStart);
            WavStudio_SetConfig(WAVSTUD_CFG_ADDBUFFQUE_CB, WAVSTUD_ACT_PLAY, (UINT32)NvtAec_AddBufToQue);
            WavStudio_SetConfig(WAVSTUD_CFG_STOP_CB, WAVSTUD_ACT_PLAY, (UINT32)NvtAec_ResetPlayQue);
            NvtAec_Enable(gDevAecEn);
            #endif
            //#NT#2016/12/22#HM Tseng -end

            //#NT#2016/09/07#HM Tseng -begin
            //#NT#Support half-duplex mode
            WavStudio_SetConfig(WAVSTUD_CFG_RECV_CB, (UINT32)WavRecv_CB, 0);
            //#NT#2016/09/07#HM Tseng -end
        }
        //#NT#2016/08/30#Jeah Yen -begin
        //#NT#support StreamReceiver
        if(gDevAudObj)
        {
            STREAMRECEIVER_OPEN sro;
            sro.sharedMemAddr = OS_GetMempoolAddr(POOL_ID_AUD_IPC);
            sro.sharedMemSize = OS_GetMempoolSize(POOL_ID_AUD_IPC);
            StreamReceiver_Open(&sro);
            StreamReceiver_RegAStreamCB(System_PutAudioData);
            DBG_MSG("^GStreamReceiver buffer addr = %08x, size = %08x\r\n", sro.sharedMemAddr, sro.sharedMemSize);
        }
        //#NT#2016/08/30#Jeah Yen -end
#endif
		#if AUDIO_NS_FUNC
		System_SetAudNsEnable(ENABLE);
		AudFilterTsk_RegNsCB((AUDFILTER_NS_CB)AudNs_CB);
		AudNS_Open();
		#endif
    }
    //    aud_setTotalVolLevel(AUDIO_VOL_LEVEL8);
    TM_BOOT_END("audio","init");
}

void System_OnAudioExit(void)
{
    //PHASE-2 : Close Lib or LibExt
    {
#if (AUDIO_PLAY_FUNCTION == ENABLE)
        //#NT#2016/08/30#Jeah Yen -begin
        //#NT#support StreamReceiver
        if(gDevAudObj)
        {
            StreamReceiver_Close();
        }
        //#NT#2016/08/30#Jeah Yen -end
        if(gDevAudObj)
        {
            DBG_MSG("^GAudPlay Close - begin\r\n");
            WavStudio_Close();
            DBG_MSG("^GAudPlay Close - end\r\n");
        }
		#if AUDIO_NS_FUNC
		AudNS_Close();
		#endif
#endif
    }
    //PHASE-1 : Close Drv or DrvExt
    {
        if(gDevAudObj)
        {
            GxSound_Close();
        }
    }
}

///////////////////////////////////////////////////////////////////////
void Audio_DetAudInsert(void)
{
    #if (LINE_INSERT_FUNCTION == ENABLE)
    //DBG_MSG("DetAudio\r\n");
    DX_HANDLE pAudDev1 = Dx_GetObject(DX_CLASS_AUDIO_EXT);
    GxSound_DetInsert((UINT32)pAudDev1, 0);
    #endif
    //DBG_MSG("\r\n");
}

///////////////////////////////////////////////////////////////////////

void System_SetAudioOutput(void)
{
#if (LINE_INSERT_FUNCTION == ENABLE)
#if (AUDIO_PLAY_FUNCTION == ENABLE)
    BOOL cPlay = FALSE;
#endif
    //=========================================================================
    // Change Audio Output according to LINE cable inserted or not
    //=========================================================================
    DBG_IND("\r\n");
#if (AUDIO_PLAY_FUNCTION == ENABLE)
    cPlay = gDevAudPlay; //get current play status
    if(cPlay)
        System_AudioOutStop();//stop output
#endif
    if(audInsertMask == SPK_MASK) //only speaker
    {
        // set audio output device entry for LCD/SPK case
        DBG_DUMP("Audio Out = SPK\r\n");
        gDevAudOut = FALSE;
        GxSound_SetOutDevConfigIdx(0);
        #if (AUDIO_PLAY_FUNCTION == ENABLE)
        WavStudio_SetConfig(WAVSTUD_CFG_PLAY_OUT_DEV, 0, 0);
        #endif
        #if (PLAY_MODE==ENABLE)
        SMediaPlay_SetOutDevConfigIdx(0);
        #endif
    }
    else
    if(audInsertMask & LOUT_MASK) //with lineout
    {
        // set audio output device entry for TV/LINE case
        DBG_DUMP("Audio Out = LINE\r\n");
        gDevAudOut = TRUE;
        GxSound_SetOutDevConfigIdx(1);
        #if (AUDIO_PLAY_FUNCTION == ENABLE)
        WavStudio_SetConfig(WAVSTUD_CFG_PLAY_OUT_DEV, 1, 0);
        #endif
        #if (PLAY_MODE==ENABLE)
        SMediaPlay_SetOutDevConfigIdx(1);
        #endif
    }
#if (AUDIO_PLAY_FUNCTION == ENABLE)
    if(cPlay)
        System_AudioOutStart();//start output
#endif
#else //others
    //=========================================================================
    // Change Audio Output according to TV/HDMI cable inserted or not
    //=========================================================================
    DX_HANDLE VideoDevObj1, VideoDevObj2;
    VideoDevObj1 = (DX_HANDLE)GxVideo_GetDevice(DOUT1);
    VideoDevObj2 = (DX_HANDLE)GxVideo_GetDevice(DOUT2);
#if (LCD1_FUNC == ENABLE)
    if((VideoDevObj1 == gDevLCDObj) && (VideoDevObj2 == 0)) //case 1
    {
        // set audio output device entry for LCD case
        GxSound_SetOutDevConfigIdx(0);
        #if (AUDIO_PLAY_FUNCTION == ENABLE)
        WavStudio_SetConfig(WAVSTUD_CFG_PLAY_OUT_DEV, 0, 0);
        #endif
        #if (PLAY_MODE==ENABLE)
        SMediaPlay_SetOutDevConfigIdx(0);
        #endif
    }
#endif
#if (TV_FUNC == ENABLE)
    if((VideoDevObj1 == gDevTVObj) && (VideoDevObj2 == 0)) //case 2
    {
        // set audio output device entry for TV case
        GxSound_SetOutDevConfigIdx(1);
        #if (AUDIO_PLAY_FUNCTION == ENABLE)
        WavStudio_SetConfig(WAVSTUD_CFG_PLAY_OUT_DEV, 1, 0);
        #endif
        #if (PLAY_MODE==ENABLE)
        SMediaPlay_SetOutDevConfigIdx(1);
        #endif
    }
#endif
#if (HDMI_FUNC == ENABLE)
    if((VideoDevObj1 == gDevHDMIObj) && (VideoDevObj2 == 0)) //case 3
    {
        // set audio output device entry for HDMI case
        GxSound_SetOutDevConfigIdx(2);
        #if (AUDIO_PLAY_FUNCTION == ENABLE)
        WavStudio_SetConfig(WAVSTUD_CFG_PLAY_OUT_DEV, 2, 0);
        #endif
        #if (PLAY_MODE==ENABLE)
        SMediaPlay_SetOutDevConfigIdx(2);
        #endif
    }
#endif
#if ((LCD1_FUNC == ENABLE) && (TV_FUNC == ENABLE) && (DUALDISP_FUNC == ENABLE))
    if((VideoDevObj1 == gDevLCDObj) && (VideoDevObj2 == gDevTVObj)) //case 4
    {
        // set audio output device entry for LCD & TV case
        GxSound_SetOutDevConfigIdx(3);
        #if (AUDIO_PLAY_FUNCTION == ENABLE)
        WavStudio_SetConfig(WAVSTUD_CFG_PLAY_OUT_DEV, 3, 0);
        #endif
        #if (PLAY_MODE==ENABLE)
        SMediaPlay_SetOutDevConfigIdx(3);
        #endif
    }
#endif
#if ((LCD1_FUNC == ENABLE) && (HDMI_FUNC == ENABLE) && (DUALDISP_FUNC == ENABLE))
    if((VideoDevObj1 == gDevLCDObj) && (VideoDevObj2 == gDevHDMIObj)) //case 5
    {
        // set audio output device entry for LCD & HDMI case
        GxSound_SetOutDevConfigIdx(4);
        #if (AUDIO_PLAY_FUNCTION == ENABLE)
        WavStudio_SetConfig(WAVSTUD_CFG_PLAY_OUT_DEV, 4, 0);
        #endif
        #if (PLAY_MODE==ENABLE)
        SMediaPlay_SetOutDevConfigIdx(4);
        #endif
    }
#endif
#endif
}

void System_AudioOutStart(void)
{
#if (AUDIO_PLAY_FUNCTION == ENABLE)
    if(gDevAudObj)
    {
        if(!gDevAudPlay) //start
        {
            BOOL r;
            PWAVSTUD_AUD_INFO pCodec = &gAudInfo;
            //#NT#2016/05/23#HM Tseng -begin
            //#NT#Support audio push function
            WavStudio_SetConfig(WAVSTUD_CFG_PASSIVE_PLAY, TRUE, 0);
            //#NT#2016/05/23#HM Tseng -end
            DBG_MSG("^GAudPlay Start - begin [%d/%d/%d/%d]\r\n", pCodec->audSR, pCodec->audCh, pCodec->bitPerSamp, pCodec->codec);
            //r = WavStudio_Start(WAVSTUD_ACT_PLAY, pCodec, WAVSTUD_NON_STOP_COUNT, UserAudio_LoadCB); //start & never auto-stop
            //#NT#2016/05/23#HM Tseng -begin
            //#NT#Support audio push function
            r = WavStudio_Start(WAVSTUD_ACT_PLAY, pCodec, WAVSTUD_NON_STOP_COUNT, System_SetDataCB); //start & never auto-stop
            //#NT#2016/05/23#HM Tseng -end
            if(!r)
            {
                DBG_ERR("Start failed\r\n");
            }
            else
            {
                gDevAudPlay = TRUE;
            }
            DBG_MSG("^GAudPlay Start - end\r\n");
        }
    }
#endif
}

void System_AudioOutStop(void)
{
#if (AUDIO_PLAY_FUNCTION == ENABLE)
    if(gDevAudObj)
    {
        if(gDevAudPlay) //stop
        {
            DBG_MSG("^GAudPlay Stop - begin\r\n");
            WavStudio_Stop(WAVSTUD_ACT_PLAY); //stop
            gDevAudPlay = FALSE;
            DBG_MSG("^GAudPlay Stop - end\r\n");
        }
    }
#endif
}

///////////////////////////////////////////////////////////////////////
//Device flow event

INT32 System_OnAudioInsert(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    DBG_DUMP("^Y===================================== audio plug [%s]\r\n", "LINE");
#if (LINE_INSERT_FUNCTION == ENABLE)
    audInsertMask |= 0x02; //line insert
    System_SetAudioOutput();
#endif
    return NVTEVT_CONSUME;
}

INT32 System_OnAudioRemove(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    DBG_DUMP("^Y===================================== audio unplug [%s]\r\n", "LINE");
#if (LINE_INSERT_FUNCTION == ENABLE)
    audInsertMask &= ~0x02; //line remove
    System_SetAudioOutput();
#endif
    return NVTEVT_CONSUME;
}


INT32 System_OnAudioAttach(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //  AUDIO: if(boot)->play boot sound
    //  AUDIO: if(boot)->start audio srv
    return NVTEVT_CONSUME;
}

INT32 System_OnAudioDetach(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}

#if (AUDIO_PLAY_FUNCTION == ENABLE)
#if 0
UINT16 gAudio_Buf[AUDIO_SR_48000] = {0};
#include "math.h"

void UserAudio_GenData(void)
{
    PWAVSTUD_AUD_INFO pCodec = &gAudInfo;
    FLOAT amplitude = 0xFFFF/2;
    FLOAT frequency = 500;
    UINT32 n;

    DBG_MSG("^GAudPlay User Gen - begin\r\n");
    for (n=0; n<pCodec->audSR; n++)
    {
        *(gAudio_Buf+n) = (INT16)(amplitude * sin((2 * M_PI * (float)n * frequency) / pCodec->audSR));
    }
    DBG_MSG("^GAudPlay User Gen - end\r\n");
}

BOOL UserAudio_LoadCB(PAUDIO_BUF_QUEUE pAudBufQue, UINT32 id)
{
    DBG_MSG("^GAudPlay CB - addr=0x%x,size=0x%x,validsize=0x%x\r\n", pAudBufQue->uiAddress, pAudBufQue->uiSize, pAudBufQue->uiValidSize);
    memcpy((void*)(pAudBufQue->uiAddress), (void*)gAudio_Buf, sizeof(UINT16)*gAudInfo.audSR/10);
    return TRUE;
}
#endif
#endif

void System_SetAudioParam(UINT32 DevID, UINT32 audSR, UINT32 audCh, UINT32 bitPerSamp)
{
#if (AUDIO_PLAY_FUNCTION == ENABLE)
    PWAVSTUD_AUD_INFO pCodec = &gAudInfo;
    if(pCodec)
    {
    pCodec->audCh = audCh;
    pCodec->audSR = audSR;
    if(bitPerSamp == 8)
        pCodec->bitPerSamp= WAVSTUD_BITS_PER_SAM_8;
    else
        pCodec->bitPerSamp= WAVSTUD_BITS_PER_SAM_16;
    //pCodec->codec = WAVSTUDCODEC_PCM;
        DBG_MSG("^GAudPlay SetParam = [%d/%d/%d/%d]\r\n", pCodec->audSR, pCodec->audCh, pCodec->bitPerSamp, pCodec->codec);
    }

    //#NT#2016/12/22#HM Tseng -begin
    //#NT# Support AEC function
    #if (AUDIO_AEC_FUNC == ENABLE)
    NvtAec_SetConfig(NVTAEC_CONFIG_ID_SAMPLERATE, (INT32)audSR);
    if (pCodec->audCh == AUDIO_CH_STEREO) {
        NvtAec_SetConfig(NVTAEC_CONFIG_ID_PLAYBACK_CH_NO, 2);
    } else {
        NvtAec_SetConfig(NVTAEC_CONFIG_ID_PLAYBACK_CH_NO, 1);
    }
    #endif
    //#NT#2016/12/22#HM Tseng -end
#endif
}

//#NT#2016/05/23#HM Tseng -begin
//#NT#Support audio push function
BOOL System_SetDataCB(PAUDIO_BUF_QUEUE pAudBufQue, UINT32 id)
{
    return FALSE;
}
//#NT#2016/05/23#HM Tseng -end

INT32 System_OnAudioMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    if(paramNum < 1)
        return NVTEVT_CONSUME;

#if (AUDIO_PLAY_FUNCTION == ENABLE)
    if(gDevAudObj)
    {
        gDevAudMode = paramArray[0];
    }
#endif

    return NVTEVT_CONSUME;
}

INT32 System_OnAudioInput(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    if(paramNum < 1)
        return NVTEVT_CONSUME;

#if (AUDIO_PLAY_FUNCTION == ENABLE)
    if(gDevAudObj)
    {
        if((paramArray[0] == 1) && (!gDevAudInput)) //start
        {
            DBG_MSG("^GAudOut Start - begin)\r\n");
            gDevAudInput = TRUE;
            if(!gDevAudMode) //half-duplex
            {
                DBG_MSG("audio record - MUTE\r\n");
                Ux_SendEvent(0,   NVTEVT_EXE_MOVIE_AUDIO,         1,  RECMOVIE_AUD_OFF);
            }
            DBG_MSG("^GAudOut Start - end\r\n");
        }
        else
        if((paramArray[0] == 0) && (gDevAudInput)) //stop
        {
            DBG_MSG("^GAudOut Stop - begin\r\n");
            gDevAudInput = FALSE;
            if(!gDevAudMode) //half-duplex
            {
                DBG_MSG("audio record - UNMUTE\r\n");
                Ux_SendEvent(0,   NVTEVT_EXE_MOVIE_AUDIO,         1,  RECMOVIE_AUD_ON);
            }
            DBG_MSG("^GAudOut Stop - end\r\n");
        }
    }
#endif

    return NVTEVT_CONSUME;
}

INT32 System_OnAudioVol(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    if(paramNum < 1)
        return NVTEVT_CONSUME;

#if (AUDIO_PLAY_FUNCTION == ENABLE)
    if(gDevAudObj)
    {
        gDevAudPlayVol = paramArray[0];
        DBG_MSG("^GAudPlay Set Vol = %d\r\n", gDevAudPlayVol);
        WavStudio_SetConfig(WAVSTUD_CFG_VOL, WAVSTUD_ACT_PLAY, gDevAudPlayVol);
    }
#endif

    return NVTEVT_CONSUME;
}

BOOL System_GetAudioOut(void)
{
    #if (LINE_INSERT_FUNCTION == ENABLE)
    return gDevAudOut;
#else
    return 0;
#endif
}

BOOL System_GetAudioMode(void)
{
#if (AUDIO_PLAY_FUNCTION == ENABLE)
    return gDevAudMode;
#else
    return 0;
#endif
}

BOOL System_GetAudioInput(void)
{
#if (AUDIO_PLAY_FUNCTION == ENABLE)
    return gDevAudInput;
#else
    return 0;
#endif
}

UINT32 System_GetAudioVol(void)
{
#if (AUDIO_PLAY_FUNCTION == ENABLE)
    return gDevAudPlayVol;
#else
    return 0;
#endif
}

//#NT#2016/08/30#Jeah Yen -begin
//#NT#support StreamReceiver
INT32 System_PutAudioData(UINT32 did, UINT32 addr, UINT32* ptrsize)
{
#if (AUDIO_PLAY_FUNCTION == ENABLE)
    UINT32 size = *ptrsize;
    DBG_MSG("AudioPlay Push %08x %08x!\r\n", addr, size);
    //#NT#2016/05/23#HM Tseng -begin
    //#NT#Support audio push function
    //#NT#2016/06/06#HM Tseng -begin
    //#NT#Change push buffer API to WavStudio_PushBufToQueue
    if(WavStudio_PushBufToQueue((UINT32)addr, (UINT32*)ptrsize) == FALSE)
    {
        if((size > 0) && ((*ptrsize) == 0))
        {
            DBG_DUMP("^RAudioPlay Q Full!\r\n");
            return STREAMRECEIVER_RET_QUE_FULL;
        }
        else
        {
            DBG_DUMP("^RAudioPlay is not work!\r\n");
            return STREAMRECEIVER_RET_NO_FUNC;
        }
    }
    //#NT#2016/06/06#HM Tseng -end
    //#NT#2016/05/23#HM Tseng -end
#endif
    return 0;
}
//#NT#2016/08/30#Jeah Yen -end

//#NT#2016/09/08#HM Tseng -begin
//#NT#Support audio channel
UINT32 System_GetAudioChannel(void)
{
#if (AUDIO_PLAY_FUNCTION == ENABLE)
    return gAudInfo.audCh;
#else
    return 0;
#endif
}
//#NT#2016/09/08#HM Tseng -end

//#NT#2016/12/22#HM Tseng -begin
//#NT# support AEC function
BOOL System_GetAecEnable(void)
{
#if (AUDIO_PLAY_FUNCTION == ENABLE)
    return gDevAecEn;
#else
    return 0;
#endif
}

void System_SetAecEnable(BOOL enable)
{
#if (AUDIO_PLAY_FUNCTION == ENABLE)
    #if (AUDIO_AEC_FUNC == ENABLE)
    NvtAec_Enable(enable);
    #endif
    gDevAecEn = enable;
#endif
}

#if AUDIO_NS_FUNC
void System_SetAudNsEnable(BOOL enable)
{
	gDevNsEn = enable;
}
#endif
//#NT#2016/12/22#HM Tseng -end

