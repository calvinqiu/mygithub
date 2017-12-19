/*
    System Sound Callback

    System Callback for Sound Module.

    @file       SysSoundCB.c
    @ingroup    mIPRJSYS

    @note       這個檔案負責一件事

                1.接收Sound callback的介面
                  Sound_CB()
                  可能的event有:
                     SYSTEM_CB_CONFIG
                     SOUND_CB_START
                       會呼叫GxFlash_SuspendCharge();
                     SOUND_CB_STOP
                       會呼叫GxFlash_ResumeCharge();
                     SOUND_CB_OUTDEVCHG
                       會呼叫GxSound_SetOutput()


    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
////////////////////////////////////////////////////////////////////////////////
#if 0//_SOUND_CB_CHARGE_
#include "GxFlash.h"
#endif
#include "GxSound.h"
#include "Debug.h"
#include "GxSound.h"
#include "AppLib.h"
#if (AUDIO_PLAY_FUNCTION == ENABLE)
#include "WavStudioTsk.h"
#endif

#if AUDIO_NS_FUNC
#include "Aec.h"
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


int SX_TIMER_DET_AUDIODEV_ID = -1;
extern BOOL g_bIsInitSystemFinish;
#if AUDIO_NS_FUNC
extern BOOL gDevNsEn;
#endif

UINT32 guiSndRepeat = FALSE;

void Sound_SetRepeat(UINT32 isTrue)
{
    guiSndRepeat = isTrue;
}

UINT32 Sound_GetRepeat(void)
{
    return guiSndRepeat;
}

void Sound_CB(UINT32 event, UINT32 param1, UINT32 param2)
{
    DBG_IND("event=%d,p1=%08x,p2=%08x\r\n",event,param1,param2);
    switch(event)
    {
    case SYSTEM_CB_CONFIG:
        //1.設定init值
        //2.設定CB值,
        //3.註冊SxJob服務 ---------> System Job
        //4.註冊SxTimer服務 ---------> Detect Job
        //5.註冊SxCmd服務 ---------> Cmd Function
        //System_AddSxCmd(Sound_OnCommand); //GxSound
        break;
    case SOUND_CB_START:
        break;
    case SOUND_CB_STOP:
#if SLIDESHOW_MUSIC_OPTION
        //#NT#2011/02/10#Lily Kao -begin
        if (guiSndRepeat)
        {
            //#NT#2011/02/18#Lily Kao -begin
            FlowPlaySlideShow_PlaySpecificBGMusic(UI_SLIDEMUSIC_MENU_SETTING);
            //#NT#2011/02/18#Lily Kao -end
        }
        //#NT#2011/02/10#Lily Kao -end
#endif
        break;
    case SOUND_CB_PLUG:
        //This CB will be trigger when LINE plug
        DBG_IND("SOUND_CB_PLUG\r\n");
        {
            Ux_PostEvent(NVTEVT_AUDIO_INSERT, 1, param1);
        }
        break;
    case SOUND_CB_UNPLUG:
        //This CB will be trigger when LINE unplug
        DBG_IND("SOUND_CB_UNPLUG\r\n");
        {
            Ux_PostEvent(NVTEVT_AUDIO_REMOVE, 1, param1);
        }
        break;
    default:
        DBG_ERR("Not Support SOUND_CB event=%d\r\n",event);
        break;
    }
    DBG_IND("end\r\n");
}


#if (AUDIO_PLAY_FUNCTION == ENABLE)
void WavPlay_CB(WAVSTUD_ACT act, UINT32 p1, UINT32 p2)
{
    DBG_MSG("^GWavPlay_CB p1=%d,p2=%d\r\n",p1,p2);
    switch(p1)
    {
    case WAVSTUD_CB_EVT_STARTED: //on start
        break;
    case WAVSTUD_CB_EVT_TCHIT: //on time code hit
        break;
    case WAVSTUD_CB_EVT_STOPPED: //on stop
        break;
    default:
        break;
    }
}

//#NT#2016/09/07#HM Tseng -begin
//#NT#Support half-duplex mode
BOOL g_Audio_Feeded = FALSE;

void WavRecv_CB(WAVSTUD_ACT act, UINT32 p1, UINT32 p2)
{
    UINT32 pushData = p1;

    if (act != WAVSTUD_ACT_PLAY)
        return;

    if(pushData)
    {
        if(!g_Audio_Feeded)
        {
            g_Audio_Feeded = TRUE;
            Ux_SendEvent(0, NVTEVT_AUDIO_INPUT, 1, g_Audio_Feeded); //notify: audio input = on
            DBG_IND("Audio input ON!\r\n");
        }
    }
    else
    {
        if(g_Audio_Feeded)
        {
            g_Audio_Feeded = FALSE;
            Ux_SendEvent(0, NVTEVT_AUDIO_INPUT, 1, g_Audio_Feeded); //notify: audio input = off
            DBG_IND("Audio input OFF!\r\n");
        }
    }
}
//#NT#2016/09/07#HM Tseng -end
#endif

#if AUDIO_NS_FUNC
void AudNs_CB(UINT32 p1, UINT32 p2, UINT32 p3)
{
	if (!gDevNsEn)
		return;

	AUDNS_BITSTREAM AudNSIO;
	UINT32 addr = p1;
	UINT32 size = p3;

	AudNSIO.uiBsAddrIn        = addr;
    AudNSIO.uiBsAddrOut       = addr;
    AudNSIO.uiBsLength        = size >> 1;
    AudNS_Run(&AudNSIO);
}
#endif

