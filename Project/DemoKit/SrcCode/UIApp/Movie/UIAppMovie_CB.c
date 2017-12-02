////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIAppCommon.h"
#include "UIAppPhoto.h"
#include "UIAppMovie.h"
#include "NVTUserCommand.h"
#include "UIMode.h"
#include "ae_task.h"
#include "awb_task.h"
#include "af_task.h"
#include "af_api.h"
#include "caf_sample.h"
#include "SysCfg.h"
// Audio NR
#include "AudFilterAPI.h"
// Raw Encode
#include "UIAppMovie_AudNR.h"
#include "Lens.h"
#include "iqs_api.h"
#if MOVIE_FD_FUNC_
#include "fd_lib.h"
#endif
#include "UIFlowMovieFuncs.h"
#include "ImageUnit_CamDisp.h"
#include "ImageApp_CamMovie.h"
#include "ImageApp_CamPhoto.h"
#include "HwClock.h"
#include "gpio.h"
//#NT#2016/06/28#KCHong -begin
//#NT#DbgLog in GPS section
#if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
#include "mp4log.h"
#endif
//#NT#2016/06/28#KCHong -end
//#NT#2016/09/20#Bob Huang -begin
//#NT#Support HDMI Display with 3DNR Out
#if (_3DNROUT_FUNC == ENABLE)
#include "AppDisp_3DNRView.h"
#endif
//#NT#2016/09/20#Bob Huang -end
#include "UART_TO_MTK.h"

#define __MODULE__          UIMovieCB
//#define __DBGLVL__ 0        //OFF mode, show nothing
#define __DBGLVL__ 1        //ERROR mode, show err, wrn only
//#define __DBGLVL__ 2        //TRACE mode, show err, wrn, ind, msg and func and ind, msg and func can be filtering by __DBGFLT__ settings
#define __DBGFLT__ "*"      //*=All
#include "DebugModule.h"


#define AUDNR_DELAYFRAME  2

extern BOOL _g_bFirstMovie;
extern BOOL _g_bFirstMovieRec;

// Lens State
static LENS_CB_MESSAGE gUIMovieLensState = 0;

UINT32 gAudNrIrisDelayFrame = AUDNR_DELAYFRAME;
UINT32 gAudNrAFDelayFrame = AUDNR_DELAYFRAME;
BOOL   gbAudNrZoomOperating = FALSE;
//UINT32 g_uiSeconds = 0;
extern UINT32 System_GetEnableSensor(void);
//#NT#2016/09/20#Bob Huang -begin
//#NT#Support HDMI Display with 3DNR Out
#if (_3DNROUT_FUNC == ENABLE)
static IMG_BUF  g3DNR_DispSrv_Src = {0};
extern BOOL     gb3DNROut;
static BOOL     gbFirstPreview = TRUE;
static UINT32   guiDispCount = 0;
#endif
//#NT#2016/09/20#Bob Huang -end

//-------------------------------------------------------------------------------------------------
//
// Callback function for audio filtering & Noise Reduction
//
//-------------------------------------------------------------------------------------------------
void UIMovie_AudFilterProc(void)
{
    // Audio NR
    UINT32              uiNrStopFreq, uiNrReplaceFreq, uiNrDecreaseLvl, uiNrMixBackgrd;
    //INT32               iNrBandNum;
    AUDNR_NOISE_SPEC    *pNoiseSpec = NULL;

    //
    //  Default Config
    //
    AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_SAMPLE_RATE,            32000);
    AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_CHANNEL_NO,             2);

    //
    // AudNR Background Suppress Enabled
    //
    AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_BACKGROUND_ENABLE,       FALSE);
    //AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_BACKGROUND_DECREASE_LVL, AUDNR_COEF(4.0));
    //AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_BACKGROUND_LOWER_BOUND,  AUDNR_COEF(0.1));

    //
    // Aud NR Enabled
    //
    switch (gUIMovieLensState)
    {
        case LENS_CB_ZOOM_START: // Zoom
        {
            //DBG_IND("Lens state is Zoom Moving!\r\n");
            uiNrStopFreq    = gAudNrZoomStopFreq;
            uiNrReplaceFreq = gAudNrZoomReplaceFreq;
            uiNrDecreaseLvl = gAudNrZoomDecreaseLvl;
            uiNrMixBackgrd  = gAudNrZoomMixBackgrd;
            pNoiseSpec      = &gpAudNrNoiseSpec_Zoom;
            //iNrBandNum      = gAudNrZoomBandNum;
            //NoiseSpec.piFreqBandNo = &gAudNrZoomBandNum;
            //NoiseSpec.piFreqBand   = gpAudNrZoomNoiseFreqBand;
            //NoiseSpec.piNoisePower = gpAudNrZoomNoisePower;
            //NoiseSpec.piNoiseSpec  = gpAudNrZoomNoiseSpec;

            gAudNrAFDelayFrame = gAudNrIrisDelayFrame = AUDNR_DELAYFRAME;
            gbAudNrZoomOperating = TRUE;
            AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_NOISE_ENABLE,                         TRUE);
        }
        break;

        case LENS_CB_FOCUS_START: // Focus
        {
            //DBG_IND("Lens state is Focus Moving!\r\n");
            if (gbAudNrZoomOperating) return; // Zoom operation is enabling, skip AF event
            uiNrStopFreq    = gAudNrFocusStopFreq;
            uiNrReplaceFreq = gAudNrFocusReplaceFreq;
            uiNrDecreaseLvl = gAudNrFocusDecreaseLvl;
            uiNrMixBackgrd  = gAudNrFocusMixBackgrd;
            pNoiseSpec      = &gpAudNrNoiseSpec_Focus;
            //iNrBandNum      = gAudNrFocusBandNum;
            //NoiseSpec.piFreqBandNo = &gAudNrFocusBandNum;
            //NoiseSpec.piFreqBand   = gpAudNrFocusNoiseFreqBand;
            //NoiseSpec.piNoisePower = gpAudNrFocusNoisePower;
            //NoiseSpec.piNoiseSpec  = gpAudNrFocusNoiseSpec;

            gAudNrAFDelayFrame = 0; // Reset delay frame count
            AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_NOISE_ENABLE,                         TRUE);
        }
        break;

        case LENS_CB_APERTURE_START: // Iris
        {
            // Support CAF case
            if((gAudNrAFDelayFrame < AUDNR_DELAYFRAME) ||(gbAudNrZoomOperating)) // Zoom operation and Auto Focus is enabling, skip AE event
                return;

            //DBG_IND("Lens state is Iris Moving!\r\n");
            uiNrStopFreq    = gAudNrIrisStopFreq;
            uiNrReplaceFreq = gAudNrIrisReplaceFreq;
            uiNrDecreaseLvl = gAudNrIrisDecreaseLvl;
            uiNrMixBackgrd  = gAudNrIrisMixBackgrd;
            pNoiseSpec      = &gpAudNrNoiseSpec_Iris;
            //iNrBandNum      = gAudNrIrisBandNum;
            //NoiseSpec.piFreqBandNo = &gAudNrIrisBandNum;
            //NoiseSpec.piFreqBand   = gpAudNrIrisNoiseFreqBand;
            //NoiseSpec.piNoisePower = gpAudNrIrisNoisePower;
            //NoiseSpec.piNoiseSpec  = gpAudNrIrisNoiseSpec;

            gAudNrIrisDelayFrame = 0; // Reset delay frame count
            AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_NOISE_ENABLE,                         TRUE);
        }
        break;


        default:
        case LENS_CB_ZOOM_END:
        {
            //DBG_IND("Lens state is ZOOM end!\r\n");
            uiNrStopFreq    = gAudNrZoomStopFreq;
            uiNrReplaceFreq = gAudNrZoomReplaceFreq;
            uiNrDecreaseLvl = gAudNrZoomDecreaseLvl;
            uiNrMixBackgrd  = gAudNrZoomMixBackgrd;
            pNoiseSpec      = &gpAudNrNoiseSpec_Zoom;
            //iNrBandNum      = gAudNrZoomBandNum;
            //NoiseSpec.piFreqBandNo = &gAudNrZoomBandNum;
            //NoiseSpec.piFreqBand   = gpAudNrZoomNoiseFreqBand;
            //NoiseSpec.piNoisePower = gpAudNrZoomNoisePower;
            //NoiseSpec.piNoiseSpec  = gpAudNrZoomNoiseSpec;

            gbAudNrZoomOperating = FALSE;
            AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_NOISE_ENABLE,                         FALSE);
        }
        break;

        case LENS_CB_FOCUS_END:
        {
            //DBG_IND("Lens state is Idle!\r\n");
            if (gbAudNrZoomOperating) return; // Zoom operation is enabling, skip AF event

            uiNrStopFreq    = gAudNrFocusStopFreq;
            uiNrReplaceFreq = gAudNrFocusReplaceFreq;
            uiNrDecreaseLvl = gAudNrFocusDecreaseLvl;
            uiNrMixBackgrd  = gAudNrFocusMixBackgrd;
            pNoiseSpec      = &gpAudNrNoiseSpec_Focus;
            //iNrBandNum      = gAudNrFocusBandNum;
            //NoiseSpec.piFreqBandNo = &gAudNrFocusBandNum;
            //NoiseSpec.piFreqBand   = gpAudNrFocusNoiseFreqBand;
            //NoiseSpec.piNoisePower = gpAudNrFocusNoisePower;
            //NoiseSpec.piNoiseSpec  = gpAudNrFocusNoiseSpec;

            if(gAudNrAFDelayFrame >= AUDNR_DELAYFRAME)
            {
                //DBG_IND("IRIS Lens state is Idle!\r\n");
                AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_NOISE_ENABLE,                     FALSE);
            }
            else
            {
                //DBG_IND("IRIS Lens state is not Idle!\r\n");
                gAudNrAFDelayFrame++;
                AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_NOISE_ENABLE,                     TRUE);
            }
        }
        break;

        case LENS_CB_APERTURE_END:
        {
            // Support CAF case
            if((gAudNrAFDelayFrame < AUDNR_DELAYFRAME) ||(gbAudNrZoomOperating)) // Zoom operation and Auto Focus is enabling, skip AE event
                return;

            uiNrStopFreq    = gAudNrIrisStopFreq;
            uiNrReplaceFreq = gAudNrIrisReplaceFreq;
            uiNrDecreaseLvl = gAudNrIrisDecreaseLvl;
            uiNrMixBackgrd  = gAudNrIrisMixBackgrd;
            pNoiseSpec      = &gpAudNrNoiseSpec_Iris;
            //iNrBandNum      = gAudNrIrisBandNum;
            //NoiseSpec.piFreqBandNo = &gAudNrIrisBandNum;
            //NoiseSpec.piFreqBand   = gpAudNrIrisNoiseFreqBand;
            //NoiseSpec.piNoisePower = gpAudNrIrisNoisePower;
            //NoiseSpec.piNoiseSpec  = gpAudNrIrisNoiseSpec;

            if(gAudNrIrisDelayFrame >= AUDNR_DELAYFRAME)
            {
                //DBG_IND("IRIS Lens state is Idle!\r\n");
                AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_NOISE_ENABLE,                     FALSE);
            }
            else
            {
                //DBG_IND("IRIS Lens state is not Idle!\r\n");
                gAudNrIrisDelayFrame++;
                AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_NOISE_ENABLE,                     TRUE);
            }
        }
        break;
    }

    AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_NOISE_STOP_FREQUENCY,         uiNrStopFreq);
    AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_NOISE_REPLACE_FREQUENCY,   uiNrReplaceFreq);
    AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_NOISE_MIX_BACKGROUND,       uiNrMixBackgrd);
    AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_NOISE_DECREASE_LVL,        uiNrDecreaseLvl);

    // Noise Pattern Spec.
    AudNoiseReduce_SetupParams(AUDNR_CONFIG_ID_NOISE_SPEC,             (UINT32)pNoiseSpec);

}

//-------------------------------------------------------------------------------------------------
//
// API function for Lens state updating
//
//-------------------------------------------------------------------------------------------------
void UIMovie_UpdateLensState(UINT32 LensState)
{
    gUIMovieLensState = LensState;
}

//-------------------------------------------------------------------------------------------------
//
// Callback function for the events which should be processed immediately (cannot use post event)
//
//-------------------------------------------------------------------------------------------------
void UIMovie_3DNRSetting(void)
{
    UINT32  ui3DNRLevel[2];

    if (UI_GetData(FL_MovieMCTFIndex) == MOVIE_MCTF_OFF)
    {
        ui3DNRLevel[0] = H264_3DNR_DISABLE;
        ui3DNRLevel[1] = H264_3DNR_DISABLE;
    }
    else
    {
#if (_IPPLIB_ != _IPL_FAKE_)
        //ui3DNRLevel = UIMovie_3DNRAdjust();
        ui3DNRLevel[0] = IQS_GetMCTFLevel(0);
        ui3DNRLevel[1] = IQS_GetMCTFLevel(1);

#else
        ui3DNRLevel[0] = H264_3DNR_DISABLE;
        ui3DNRLevel[1] = H264_3DNR_DISABLE;
#endif
    }

    MovRec_ChangeParameter(MOVREC_RECPARAM_H2643DNRLEVEL, ui3DNRLevel[0], 0, 0);
    #if(IPCAM_FUNC==ENABLE)
    // ipcam just have one sensor
    MovRec_ChangeParameter(MOVREC_RECPARAM_H2643DNRLEVEL, ui3DNRLevel[0], 1, 0);
    #else
    MovRec_ChangeParameter(MOVREC_RECPARAM_H2643DNRLEVEL, ui3DNRLevel[1], 1, 0);
    #endif
}

void UIMovie_ImmProc(UINT32 type, UINT32 p1, UINT32 p2, UINT32 p3)
{
    switch (type)
    {
    case MOVREC_IMMPROC_CUTFILE_END:
        //#NT#2016/06/14#Niven Cho#[0103235] -begin
        //#NT# remove FlowDCF_UpdateName(); because of no use and it will cause hang.
        //#NT#2016/06/14#Niven Cho#[0103235] -end

        // [TO DO]: log GPS data
        DBG_IND("EVENT %d: Cut file end!\r\n", type);
        break;

    case MOVREC_IMMPROC_DELETE_FILE:
        // [TO DO]: delete related files, such as GPS data file
        DBG_IND("EVENT %d: Delete file!\r\n", type);
        break;

    // special dynamic encoding parameters setting, such as H.264 3DNR
    case MOVREC_IMMPROC_ENC_SETTING:
        UIMovie_3DNRSetting();
        break;

    default:
        break;
    }
}

//-------------------------------------------------------------------------------------------------
//
// Callback function for the events of Raw Encode function
//
//-------------------------------------------------------------------------------------------------
void UIMovie_RawEncCB(UINT32 uiEventID, UINT32 uiPathID)
{
    switch (uiEventID)
    {
    case MOVREC_RAWENC_EVENT_RESULT_OK:
        // [TO DO]: add event handling
        DBG_IND("EVENT %d: Write file OK\r\n", uiEventID);
        Ux_PostEvent(NVTEVT_CB_RAWENC_OK, 0);
        break;

    case MOVREC_RAWENC_EVENT_RESULT_ERR:
        // [TO DO]: add event handling
        DBG_IND("EVENT %d: Encode error\r\n", uiEventID);
        Ux_PostEvent(NVTEVT_CB_RAWENC_ERR, 0);
        break;

    case MOVREC_RAWENC_EVENT_RESULT_WRITE_ERR:
        // [TO DO]: add event handling
        DBG_IND("EVENT %d: Write file error\r\n", uiEventID);
        Ux_PostEvent(NVTEVT_CB_RAWENC_WRITE_ERR, 0);
        break;

    case MOVREC_RAWENC_EVENT_RESULT_DCF_FULL:
        // [TO DO]: add event handling
        DBG_IND("EVENT %d: DCF ID full\r\n", uiEventID);
        Ux_PostEvent(NVTEVT_CB_RAWENC_DCF_FULL, 0);
        break;
    }

    // [TO DO]: inform UI that picture in movie is done
}

//#NT#2016/03/07#KCHong -begin
//#NT#Low power timelapse function
//#if ((YOUKU_SDK == ENABLE && SAVE_DUAL_STREAM_FUNC == ENABLE) || (TIMELAPSE_LPR_FUNCTION == ENABLE))
//-------------------------------------------------------------------------------------------------
//
// Callback function for custom file naming rule
//
//-------------------------------------------------------------------------------------------------
static void UIMovie_SetCustomFileName(UINT32 uiPathId, char *pFileName)
{
//#NT#2016/08/12#Hideo Lin -begin
//#NT#For small size clone movie
#if (SMALL_CLONE_MOVIE == DISABLE)
    //#NT#2016/08/10#Brain Yen -begin
    //#NT#add for clone multi-record
    BOOL bCustomNameByDate=FALSE;
    #if((MOVIE_MULTI_RECORD_FUNC == ENABLE) && ( _UI_STYLE_ == _UI_STYLE_SPORTCAM_))
        if(UI_GetData(FL_IsCloneRec)==CLONE_REC_ON)
            bCustomNameByDate=TRUE;
    #endif
    //#NT#2016/08/10#Brain Yen -end
    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI || bCustomNameByDate==TRUE)
    {//(YOUKU_SDK == ENABLE && SAVE_DUAL_STREAM_FUNC == ENABLE)
        static struct tm   CurDateTime = {0};
        //let both files' name be the same
        if (uiPathId == 0)
        {
            CurDateTime = HwClock_GetTime(TIME_ID_CURRENT);
        }
        snprintf(pFileName,NMC_TOTALFILEPATH_MAX_LEN, "%04d%02d%02d_%02d%02d%02d", CurDateTime.tm_year, CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
    }
    else
    {
        if (uiPathId == 0)
            sprintf(pFileName, "Timelapse0_Tmp");
        else
            sprintf(pFileName, "Timelapse1_Tmp");
    }
#else
    static struct tm   CurDateTime = {0};
    static UINT32      uSetPathCnt = 0;
    static UINT32      uiPrevPathId = 0;

    if (((uSetPathCnt % 2) == 0) || (uiPrevPathId == uiPathId))//2016/12/30
	{
        CurDateTime = HwClock_GetTime(TIME_ID_CURRENT);
        if (uiPrevPathId == uiPathId)
        {
            uSetPathCnt = 0;
        }
    }

    uSetPathCnt++;
    uiPrevPathId = uiPathId;

    if (uiPathId == 0) // main movie
    {
        snprintf(pFileName, NMC_TOTALFILEPATH_MAX_LEN, "%04d%02d%02d_%02d%02d%02d",
            CurDateTime.tm_year, CurDateTime.tm_mon, CurDateTime.tm_mday,
            CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
    }
    else // clone movie
    {
        snprintf(pFileName, NMC_TOTALFILEPATH_MAX_LEN, "%04d%02d%02d_%02d%02d%02d_S",
            CurDateTime.tm_year, CurDateTime.tm_mon, CurDateTime.tm_mday,
            CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
    }
#endif
//#NT#2016/08/12#Hideo Lin -end
}
//#NT#2016/03/07#KCHong -end

void UIMovie_MovieFileNamingCB(UINT32 uiPathId, char *pFileName)
{
    UIMovie_SetCustomFileName(uiPathId, pFileName);
}

void UIMovie_EMRMovFileNamingCB(UINT32 uiPathId, char *pFileName)
{
    UIMovie_SetCustomFileName(uiPathId, pFileName);
   // MovRec_GetLastEMRFilePath(m_cmd_at_last_video_path);//add by qiuhan on 20171027
    debug_msg("QIUHAN=======================UIMovie_EMRMovFileNamingCB ==%s\r\n",pFileName);//add by qiuhan on 20171027
}

void UIMovie_PhotoFileNamingCB(UINT32 uiPathId, char *pFileName)
{
    UIMovie_SetCustomFileName(uiPathId, pFileName);
}
//#endif

//-------------------------------------------------------------------------------------------------
//
// Callback function for AF events
//
//-------------------------------------------------------------------------------------------------
void UIMovie_AFCB(AF_CBMSG uiMsgID, void *Data)
{
    UINT32 uiData;

    switch (uiMsgID)
    {
        case AF_CBMSG_AF_START:
            uiData = *(UINT32 *)Data;
            DBG_IND("===AF Start! - %d===\r\n", uiData);
            break;
        case AF_CBMSG_AF_DONE:
            uiData = *(UINT32 *)Data;
            DBG_IND("===AF Done! - %d===\r\n", uiData);
            //AE_LOCK(FALSE);
            //AWB_LOCK(FALSE);
            //Ux_PostEvent(NVTEVT_ALGMSG_FOCUSEND,1, NVTEVT_ALGMSG_FOCUSEND);
            break;
        default:
            break;
    }
}

//-------------------------------------------------------------------------------------------------
//
// Callback function for IPL events
//
//-------------------------------------------------------------------------------------------------
void Movie_IPLCB(IPL_CBMSG uiMsgID, void *Data)
{
    switch (uiMsgID)
    {
    case IPL_CBMSG_PREVIEW:
        DBG_IND("===IPL_CBMSG_PREVIEW===\r\n");
        if(_g_bFirstMovie){TM_BOOT_END("sensor","ipl_open");}
        if(_g_bFirstMovie){TM_BOOT_BEGIN("sensor","2a_stable");}
        Ux_PostEvent(NVTEVT_CALLBACK,1,NVTEVT_ALGMSG_PREVIEW);
        break;
    case IPL_CBMSG_PREVIEWSTABLE:
        DBG_IND("===IPL_CBMSG_PREVIEWSTABLE===\r\n");
        if(_g_bFirstMovie){TM_BOOT_END("sensor","2a_stable");}
        if(_g_bFirstMovie)
        {
            //#NT#2016/09/20#Bob Huang -begin
            //#NT#Support HDMI Display with 3DNR Out
            #if (_3DNROUT_FUNC == ENABLE)
            if(!gb3DNROut)
            #endif
            //#NT#2016/09/20#Bob Huang -end
            {
                Display_ShowPreview(); //show preview immediately
            }

            if(!_g_bFirstMovieRec)
                _g_bFirstMovie = FALSE;
        }
        Ux_PostEvent(NVTEVT_CALLBACK,1,NVTEVT_ALGMSG_PREVIEW_STABLE);
        break;
    case IPL_CBMSG_CHGDISPSIZEINFOR:
        DBG_IND("===IPL_CBMSG_CHGDISPSIZEINFOR===\r\n");
        break;
    case IPL_CBMSG_DZOOM:
        DBG_IND("===IPL_CBMSG_DZOOM===\r\n");
        // for Dzoom keep on moving
        //Ux_PostEvent(NVTEVT_CALLBACK,1,NVTEVT_ALGMSG_DZOOMSTEP);
        // post event for update UI
        Ux_PostEvent(NVTEVT_CB_ZOOM, 0);
        break;
    case IPL_CBMSG_SETDATASTAMPINFOR:
        break;
    case IPL_CBMSG_SETPHOTOFRAMEINFOR:
        break;
    case IPL_CBMSG_SHUTTER_SOUND:
        //Photo_ShutterSoundCB();
        break;
    default:
        break;
    }
}


#define STREAM_GET_QP     1

#if STREAM_GET_QP
typedef struct
{
    UINT32  P0_FrameType;// 0 : P slice, 1 : B slice , 2,3 : I or  IDR slice
    UINT32  P0_FrameQP;
    UINT32  P1_FrameType;// 0 : P slice, 1 : B slice , 2,3 : I or  IDR slice
    UINT32  P1_FrameQP;
    UINT32  P2_FrameType;// 0 : P slice, 1 : B slice , 2,3 : I or  IDR slice
    UINT32  P2_FrameQP;
    UINT32  P3_FrameType;// 0 : P slice, 1 : B slice , 2,3 : I or  IDR slice
    UINT32  P3_FrameQP;
    UINT32  P4_FrameType;// 0 : P slice, 1 : B slice , 2,3 : I or  IDR slice
    UINT32  P4_FrameQP;
} STREAM_QP_INFO;

STREAM_QP_INFO  g_Stream_QP_info;

void GetStreamQPInfo(UINT32 ID, UINT32 puiParam[2] )
{
    if(ID==0)
    {
        puiParam[0]=g_Stream_QP_info.P0_FrameType;
        puiParam[1]=g_Stream_QP_info.P0_FrameQP;

    }
    else if(ID==1)
    {
        puiParam[0]=g_Stream_QP_info.P1_FrameType;
        puiParam[1]=g_Stream_QP_info.P1_FrameQP;

    }else if(ID==2)
    {
        puiParam[0]=g_Stream_QP_info.P2_FrameType;
        puiParam[1]=g_Stream_QP_info.P2_FrameQP;

    }
    else if(ID==3)
    {
        puiParam[0]=g_Stream_QP_info.P3_FrameType;
        puiParam[1]=g_Stream_QP_info.P3_FrameQP;

    }

}
#endif


//-------------------------------------------------------------------------------------------------
//
// Callback function for Record events
//
//-------------------------------------------------------------------------------------------------

extern void H264Test_GetEncInfo(INT32 puiParam[8]);
extern char EventEMRName[256];
extern char EventLDWSLName[256];
extern char EventLDWSRName[256];
extern char EventFDWSName[256];
extern char EventGsensorName[256];
extern char EventSharpTurnName[256];
extern char EventRushName[256];
extern char EventNastyBrakeName[256];//qiuhan add 20171202
char EventEMRNameString[100] = {0};//qiuhan add 20171027
char EventLDWSLNameString[100] = {0};//qiuhan add 20171027
char EventLDWSRNameString[100] = {0};//qiuhan add 20171027
char EventFDWSNameString[100] = {0};//qiuhan add 20171027
char EventGsensorNameString[100] = {0};//qiuhan add 20171118
char EventSharpTurnNameString[100] = {0};//qiuhan add 20171202
char EventRushNameString[100] = {0};//qiuhan add 20171202
char EventNastyBrakeNameString[100] = {0};//qiuhan add 20171202
void Movie_RecordCB(UINT32 uiEventID, UINT32 param)
{
    UINT32 uiSeconds;
    INT32  uiParam[8];
    UINT8   uiResqData[116]= {0};
    char m_cmd_at_last_video_path[256] = {0};//add by qiuhan on 20171027
    //#NT#2016/06/28#KCHong -begin
    //#NT#DbgLog in GPS section
    #if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
    UINT32 GPSReadyBufAddr = 0;
    #endif
    //#NT#2016/06/28#KCHong -end
    //#NT#2016/09/20#Bob Huang -begin
    //#NT#Support HDMI Display with 3DNR Out
    #if (_3DNROUT_FUNC == ENABLE)
    UINT32              uiLineOff[3] = {0};
    UINT32              PxlAddr[3] = {0};
    UINT32              uiWidth = 0, uiHeight = 0;
    USIZE               ImgSize;
    UINT32              py=0, pu=0, pv=0;
    #endif
    //#NT#2016/09/20#Bob Huang -end

    switch (uiEventID)
    {
    case MOVREC_EVENT_VIDEO_INFO_READY:
	 MovRec_GetLastEMRFilePath(m_cmd_at_last_video_path);//add by qiuhan on 20171027
        debug_msg("QIUHAN=======================lase_video_path READY ==%s\r\n",m_cmd_at_last_video_path);//add by qiuhan on 20171027
        Ux_PostEvent(NVTEVT_CB_MOVIE_VEDIO_READY,1,param);
        break;
    case MOVREC_EVENT_AUDREC_START:
        if(_g_bFirstMovieRec){TM_BOOT_END("flow","rec");}
        _g_bFirstMovie = FALSE;
        _g_bFirstMovieRec = FALSE;
        DBG_IND("start record!\r\n");
        break;
    case MOVREC_EVENT_RESULT_OVERTIME:
        Ux_PostEvent(NVTEVT_CB_MOVIE_OVERTIME, 0);
        DBG_ERR("EVENT %d: Overtime!\r\n", uiEventID);
        break;
    case MOVREC_EVENT_RESULT_FULL:
        Ux_PostEvent(NVTEVT_CB_MOVIE_FULL, 0);
        DBG_ERR("EVENT %d: Full!\r\n", uiEventID);
        break;
    case MOVREC_EVENT_RESULT_HW_ERR:
    case MOVREC_EVENT_RESULT_NOTREADY:
    case MOVREC_EVENT_RESULT_WRITE_ERR:
        Ux_PostEvent(NVTEVT_CB_MOVIE_WR_ERROR, 0);
        DBG_ERR("EVENT %d: HW error!\r\n", uiEventID);
        break;
    case MOVREC_EVENT_THRESHOLD:
        uiSeconds = param;
        #if(IPCAM_FUNC!=ENABLE)
        //if ((g_uiSeconds != uiSeconds) && (uiSeconds > 0))
        {
            //g_uiSeconds = uiSeconds;
            Ux_PostEvent(NVTEVT_CB_MOVIE_REC_ONE_SEC, 1,  uiSeconds);
            DBG_IND("EVENT: uiSeconds %d\r\n", uiSeconds);

            //#NT#2016/06/28#KCHong -begin
            //#NT#DbgLog in GPS section
            #if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
            if (uiSeconds == 0)
            {
                mp4log_ResetFrmCnt();
            }
			//#NT#2016/09/29#KCHong -begin
			//#NT#Log GPS data when toggle buffer instead of OnTimer event
			#if (GPS_FUNCTION == ENABLE)
			RMCINFO RMCInfo;
			GPSRec_GetRMCDate(&RMCInfo);
			mp4log_AddLog (0, (char*)&RMCInfo, sizeof(RMCINFO));
			#endif
			//#NT#2016/09/29#KCHong -end
            if(mp4log_ToggleBuf(&GPSReadyBufAddr, NULL) == E_OK)
            {
                MovRec_SetGPSData(GPSReadyBufAddr, (mp4log_GetTotalLogSize() * 1024 - 16));
            }
            #endif
            //#NT#2016/06/28#KCHong -end
        }
        #endif
        break;
    case MOVREC_EVENT_RESULT_NORMAL:
        // [TO DO]: log GPS data
        Ux_PostEvent(NVTEVT_CB_MOVIE_REC_FINISH, 0);
        //g_uiSeconds = 0;
        break;
    case MOVREC_EVENT_RESULT_SLOW:
    case MOVREC_EVENT_RESULT_CUT_FAIL:
        DBG_ERR("EVENT %d: Slow card!\r\n", uiEventID);
        Ux_PostEvent(NVTEVT_CB_MOVIE_SLOWMEDIA, 0);
        Ux_PostEvent(NVTEVT_CB_MOVIE_SLOW, 0);
        break;
    case MOVREC_EVENT_RESULT_YUVQ_FULL:
        DBG_ERR("EVENT %d: YUV Queue Full!\r\n", uiEventID);
        Ux_PostEvent(NVTEVT_CB_MOVIE_SLOWMEDIA, 0);
        Ux_PostEvent(NVTEVT_CB_MOVIE_SLOW, 0);
        break;
    case MOVREC_EVENT_IMEDIRECT_STOPH264:
        SMediaRec_Vtrig_NoKickH264(1);
        DBG_DUMP("^Rime stop h264!!!!\r\n");
        break;
    case MOVREC_EVENT_LOOPREC_FULL:
        Ux_PostEvent(NVTEVT_CB_MOVIE_LOOPREC_FULL, 0);
        break;
    case MOVREC_EVENT_EMR_FILECOMPLETE:
//        DBG_IND("EVENT %d: Emergency file completed!\r\n", uiEventID);
        MovRec_GetLastEMRFilePath(m_cmd_at_last_video_path);//add by qiuhan on 20171027
        debug_msg("QIUHAN=======================lase_video_path00 ==%s\r\n",m_cmd_at_last_video_path);//add by qiuhan on 20171027
        if(strcmp(EventEMRName,"Emergency-")==0 /*|| strcmp(EventEMRName,"LDWSL-")==0 ||strcmp(EventEMRName,"FCWS-")==0|| strcmp(EventEMRName,"LDWSR-")==0 */){
			// debug_msg("QIUHAN=======================lase_video_path00 ==%d\r\n",strlen(EventEMRName));
			strncpy(EventEMRNameString, m_cmd_at_last_video_path+19,25);
			debug_msg("QIUHAN=======================lase_video_path00 ==%s\r\n",EventEMRNameString);
                     strcat(EventEMRName,EventEMRNameString);
			debug_msg("QIUHAN=======================EventEMRName ==%s\r\n",EventEMRName);
			uiResqData[0]=0x02; 
	              strcat(&uiResqData[1],EventEMRName);
			memset(EventEMRName, 0, sizeof(EventEMRName));
		       MTKComposeCMDRspFrame(0, CMD_EMR_VEDIO,&uiResqData, 116);  
			debug_msg("QIUHAN=======================uiResqData ==%s\r\n",uiResqData);
        }else if(strcmp(EventLDWSLName,"LDWSL-")==0){
                     
                     strncpy(EventLDWSLNameString, m_cmd_at_last_video_path+19,25);
			debug_msg("QIUHAN=======================lase_video_path00 LDWSL==%s\r\n",EventLDWSLNameString);
                     strcat(EventLDWSLName,EventLDWSLNameString);
			debug_msg("QIUHAN=======================EventEMRName LDWSL==%s\r\n",EventLDWSLName);
			uiResqData[0]=0x02; 
	              strcat(&uiResqData[1],EventLDWSLName);
			memset(EventLDWSLName, 0, sizeof(EventLDWSLName));
		       MTKComposeCMDRspFrame(0, CMD_ADAS_LDWSL,&uiResqData, 116);  
			debug_msg("QIUHAN=======================uiResqData ==%s\r\n",uiResqData);

        }else if(strcmp(EventLDWSRName,"LDWSR-")==0){

		       strncpy(EventLDWSRNameString, m_cmd_at_last_video_path+19,25);
			debug_msg("QIUHAN=======================lase_video_path00 LDWSR==%s\r\n",EventLDWSRNameString);
                     strcat(EventLDWSRName,EventLDWSRNameString);
			debug_msg("QIUHAN=======================EventEMRName LDWSR==%s\r\n",EventLDWSRName);
			uiResqData[0]=0x02; 
	              strcat(&uiResqData[1],EventLDWSRName);
			memset(EventLDWSRName, 0, sizeof(EventLDWSRName));
		       MTKComposeCMDRspFrame(0, CMD_ADAS_FCWS,&uiResqData, 116);  
			debug_msg("QIUHAN=======================uiResqData ==%s\r\n",uiResqData);

        }else if(strcmp(EventFDWSName,"FCWS-")==0){

                     strncpy(EventFDWSNameString, m_cmd_at_last_video_path+19,25);
			debug_msg("QIUHAN=======================lase_video_path00 FCWS==%s\r\n",EventFDWSNameString);
                     strcat(EventFDWSName,EventFDWSNameString);
			debug_msg("QIUHAN=======================EventEMRName FCWS==%s\r\n",EventFDWSName);
			uiResqData[0]=0x02; 
	              strcat(&uiResqData[1],EventFDWSName);
			memset(EventFDWSName, 0, sizeof(EventFDWSName));
		       MTKComposeCMDRspFrame(0, CMD_ADAS_LDWSR,&uiResqData, 116);  
			debug_msg("QIUHAN=======================uiResqData ==%s\r\n",uiResqData);
        }else if(strcmp(EventGsensorName, "Collision-")==0){
                     strncpy(EventGsensorNameString, m_cmd_at_last_video_path+19,25);
			debug_msg("QIUHAN=======================lase_video_path00 Collision==%s\r\n",EventGsensorNameString);
                     strcat(EventGsensorName,EventGsensorNameString);
			debug_msg("QIUHAN=======================EventEMRName Collision==%s\r\n",EventGsensorName);
			uiResqData[0]=0x02; 
	              strcat(&uiResqData[1],EventGsensorName);
			memset(EventGsensorName, 0, sizeof(EventGsensorName));
		       MTKComposeCMDRspFrame(0, CMD_GSENSOR_VEDIO,&uiResqData, 116);  
			debug_msg("QIUHAN=======================uiResqData ==%s\r\n",uiResqData);
	}else  if(strcmp(EventSharpTurnName, "Sharpturn-")==0){
                     strncpy(EventSharpTurnNameString, m_cmd_at_last_video_path+19,25);
			debug_msg("QIUHAN=======================lase_video_path00 Sharpturn==%s\r\n",EventSharpTurnNameString);
                     strcat(EventSharpTurnName,EventSharpTurnNameString);
			debug_msg("QIUHAN=======================EventEMRName Sharpturn==%s\r\n",EventSharpTurnName);
			uiResqData[0]=0x02; 
	              strcat(&uiResqData[1],EventSharpTurnName);
			memset(EventSharpTurnName, 0, sizeof(EventSharpTurnName));
		       MTKComposeCMDRspFrame(0, CMD_SHARP_TURN_VEDIO,&uiResqData, 116);  
			debug_msg("QIUHAN=======================uiResqData ==%s\r\n",uiResqData);
	}else  if(strcmp(EventRushName, "Rush-")==0){
                     strncpy(EventRushNameString, m_cmd_at_last_video_path+19,25);
			debug_msg("QIUHAN=======================lase_video_path00 Rush==%s\r\n",EventRushNameString);
                     strcat(EventRushName,EventRushNameString);
			debug_msg("QIUHAN=======================EventEMRName Rush==%s\r\n",EventRushName);
			uiResqData[0]=0x02; 
	              strcat(&uiResqData[1],EventRushName);
			memset(EventRushName, 0, sizeof(EventRushName));
		       MTKComposeCMDRspFrame(0, CMD_RUSH_VEDIO,&uiResqData, 116);  
			debug_msg("QIUHAN=======================uiResqData ==%s\r\n",uiResqData);
	}else  if(strcmp(EventNastyBrakeName, "Rush-")==0){
                     strncpy(EventNastyBrakeNameString, m_cmd_at_last_video_path+19,25);
			debug_msg("QIUHAN=======================lase_video_path00 Nasty Brake==%s\r\n",EventNastyBrakeNameString);
                     strcat(EventNastyBrakeName,EventNastyBrakeNameString);
			debug_msg("QIUHAN=======================EventEMRName Nasty Brake==%s\r\n",EventNastyBrakeName);
			uiResqData[0]=0x02; 
	              strcat(&uiResqData[1],EventNastyBrakeName);
			memset(EventNastyBrakeName, 0, sizeof(EventNastyBrakeName));
		       MTKComposeCMDRspFrame(0, CMD_NASTY_BRAKE_VEDIO,&uiResqData, 116);  
			debug_msg("QIUHAN=======================uiResqData ==%s\r\n",uiResqData);
	}
		
        memset(EventEMRName, 0, sizeof(EventEMRName));
	 debug_msg("QIUHAN=======================EventEMRName_Last ==%s\r\n",EventEMRName);//add by qiuhan on 20171027
        Ux_PostEvent(NVTEVT_CB_EMR_COMPLETED, 1, param);
        break;
    case MOVREC_EVENT_GETNAME_PATH0:
        ImageApp_CamFile_MakeMovieFileNameCB(param);	 
        MovRec_GetLastEMRFilePath(m_cmd_at_last_video_path);//add by qiuhan on 20171027
        debug_msg("QIUHAN=======================lase_video_path ==%s\r\n",m_cmd_at_last_video_path);//add by qiuhan on 20171027
        break;
    //#NT#2016/08/10#Brain Yen -begin
    //#NT# for EMR movie file name setting
    case MOVREC_EVENT_GETNAME_EMRPATH:
        ImageApp_CamFile_MakeEMRMovFileNameCB(param);
	 MovRec_GetLastEMRFilePath(m_cmd_at_last_video_path);//add by qiuhan on 20171027
        debug_msg("QIUHAN=======================lase_video_path00 ==%s\r\n",m_cmd_at_last_video_path);//add by qiuhan on 20171027
        break;
    //#NT#2016/08/10#Brain Yen -end
    #if STREAM_GET_QP
    case MOVREC_EVENT_H264BSCB_P0:
        H264Test_GetEncInfo( uiParam);//uiParm[5] = encId -->待 Paulpaul新增
        if(1)//uiParam[5]==0)//Check ID
        {
            g_Stream_QP_info.P0_FrameType=(UINT32) uiParam[0];// 0 : P slice, 1 : B slice , 2,3 : I or  IDR slice
            g_Stream_QP_info.P0_FrameQP=(UINT32) uiParam[2];//rc_bug[2] = iQP;   // 當次算出來的QP值
        }
        else
        {
           //NeedToCheck;
           DBG_ERR("Stream ID is not matched! Please check \r\n");
        }

        break;
    case MOVREC_EVENT_H264BSCB_P1:
        H264Test_GetEncInfo( uiParam);//uiParm[5] = encId -->待 Paulpaul新增
        if(1)//uiParam[5]==1)//Check ID
        {
            g_Stream_QP_info.P1_FrameType=(UINT32) uiParam[0];// 0 : P slice, 1 : B slice , 2,3 : I or  IDR slice
            g_Stream_QP_info.P1_FrameQP=(UINT32) uiParam[2];//rc_bug[2] = iQP;   // 當次算出來的QP值
        }
        else
        {
            //NeedToCheck;
           DBG_ERR("Stream ID is not matched! Please check \r\n");
        }
        break;
    #endif

    //#NT#2016/09/20#Bob Huang -begin
    //#NT#Support HDMI Display with 3DNR Out
    #if (_3DNROUT_FUNC == ENABLE)
    case MOVREC_EVENT_3DNR_ONE_DISPLAYFRAME:
        // New display frame is presented, get now display frame
        MovRec_GetNowDisplayFrame(param, &py, &pu, &pv);

        if(py != 0 && pu != 0)
        {
            g3DNR_DispSrv_Src.PxlAddr[0] = py;
            g3DNR_DispSrv_Src.PxlAddr[1] = pu;
            g3DNR_DispSrv_Src.PxlAddr[2] = pv;
            // Display trigger
            AppDisp_3DNRView_Trigger();

            if(gbFirstPreview)
            {
                if(guiDispCount > 0)
                {
                    gbFirstPreview = FALSE;
                    Display_ShowPreview(); //show preview
                }
                guiDispCount++;
            }
        }
        break;
    case MOVREC_EVENT_3DNR_MEDIAINFO_READY:
        MovRec_GetInfo(MOVREC_GETINFO_WIDTH_HEIGHT, &uiWidth, &uiHeight, 0);

        // Get Image Size
        ImgSize.w = ALIGN_CEIL_32(uiWidth);
        ImgSize.h = ALIGN_CEIL_16(uiHeight);

        // Init Source Buffer for DispSrv
        uiLineOff[0] = uiLineOff[1] = uiLineOff[2] = ALIGN_CEIL_32(ImgSize.w); // Algien 32-lines base for H.264 HW limitation
        PxlAddr[0]   = PxlAddr[1]   = PxlAddr[2]   = 0; // Set Null first
        GxImg_InitBufEx(&g3DNR_DispSrv_Src,
                        ImgSize.w,
                        ImgSize.h,
                        GX_IMAGE_PIXEL_FMT_YUV420_PACKED,
                        uiLineOff,
                        PxlAddr);

        // Configure the Source Buffer
        AppDisp_3DNRView_Cfg(&g3DNR_DispSrv_Src);
        gbFirstPreview = TRUE;
        guiDispCount = 0;
        break;
    case MOVREC_EVENT_3DNR_DISPSTART:
        AppDisp_3DNRView_Start();
        break;
    case MOVREC_EVENT_3DNR_DISPSTOP:
        AppDisp_3DNRView_Stop();
        break;
    #endif
    //#NT#2016/09/20#Bob Huang -end

    default:
        break;
    }
}


//-------------------------------------------------------------------------------------------------
//
// Callback function for Capture events
//
//-------------------------------------------------------------------------------------------------
void Movie_CaptureCB(IMG_CAP_CBMSG Msg, void *Data)
{
    switch (Msg)
    {
    case IMG_CAP_CBMSG_GET_ONERAWEND:
        Ux_PostEvent(NVTEVT_CB_MOVIE_PIM_READY, 0);
        break;

    default:
        break;
    }
}

#if MOVIE_FD_FUNC_
void Movie_FdCB(FD_CBMSG uiMsgID, void *Data)
{
    switch (uiMsgID)
    {
        case FD_CBMSG_DETEND:
           DBG_IND("===FDEND===\r\n");
           //#NT#2016/06/08#Lincy Lin -begin
           //#NT#Implement generic OSD and video drawing mechanism for ALG function
           Ux_PostEvent(NVTEVT_EXE_MOVIE_ALGEND,1, NVTEVT_EXE_MOVIE_ALGEND);
           //#NT#2016/06/08#Lincy Lin -end
           break;


        default:
           break;
    }
}
#endif

void UIMovie_IRCutCtrl(BOOL isON)
{
    // day mode
    if (isON == TRUE)
    {
        #if( _MODEL_DSC_ == _MODEL_IPCAM1_EVB_ \
            || _MODEL_DSC_ == _MODEL_IPCAM2_EVB_)
            gpio_setPin(P_GPIO_1); ////GPIO_IRCUT_CTRL1
            Delay_DelayMs(300);
            gpio_clearPin(P_GPIO_1);
        #elif(_MODEL_DSC_ == _MODEL_IPCAM1_\
            || _MODEL_DSC_ == _MODEL_IPCAM2_ \
            || _MODEL_DSC_ == _MODEL_IPCAM3_)
            gpio_clearPin(P_GPIO_2);
            gpio_setPin(P_GPIO_3);
            Delay_DelayMs(100);
            gpio_setPin(P_GPIO_2);
            gpio_clearPin(P_GPIO_3);
        #elif(_MODEL_DSC_ == _MODEL_TL_IPC320_)
        //#NT#2016/07/20#David Tsai -begin
        //#NT#Directly control IR cut and IR LED in Linux
            //gpio_clearPin(P_GPIO_18);
            //gpio_clearPin(P_GPIO_1);
        //#NT#2016/07/20#David Tsai -end
        #endif

    }
    // night mode
    else
    {
        #if( _MODEL_DSC_ == _MODEL_IPCAM1_EVB_ \
        || _MODEL_DSC_ == _MODEL_IPCAM2_EVB_)
            gpio_setPin(P_GPIO_0);//GPIO_IRCUT_CTRL0
            Delay_DelayMs(300);
            gpio_clearPin(P_GPIO_0);
        #elif(_MODEL_DSC_ == _MODEL_IPCAM1_\
            || _MODEL_DSC_ == _MODEL_IPCAM2_ \
            || _MODEL_DSC_ == _MODEL_IPCAM3_)
            gpio_clearPin(P_GPIO_2);
            Delay_DelayMs(100);
            gpio_setPin(P_GPIO_2);
        #elif(_MODEL_DSC_ == _MODEL_TL_IPC320_)
        //#NT#2016/07/20#David Tsai -begin
        //#NT#Directly control IR cut and IR LED in Linux
            //gpio_setPin(P_GPIO_18);
            //gpio_setPin(P_GPIO_1);
        //#NT#2016/07/20#David Tsai -end
        #endif
    }
}


void UIMovie_AeIRCutCB(UINT32 Id, SEL_IRCUT Value)
{
    if (Value == SEL_IRCUT_DAY)
    {
        UIMovie_IRCutCtrl(TRUE);
    }
    else if(Value == SEL_IRCUT_NIGHT)
    {
        UIMovie_IRCutCtrl(FALSE);
    }
    else
    {
        DBG_ERR("Value = %d\r\n",Value);
    }
}

void Movie_RegCB(void)
{
#if (_IPPLIB_ != _IPL_FAKE_)
    AF_RegisterCB(UIMovie_AFCB);                            // register AF callback function
    AFTsk_RegisterCAF((AF_PROC_CAF)caf_AutoFocusProcess);   // register CAF function
    AE_SetIRCutFP(UIMovie_AeIRCutCB);
#endif
    #if MOVIE_FD_FUNC_
    FD_RegisterCB(Movie_FdCB);
    #endif
    if (System_GetEnableSensor() == (SENSOR_2) )
        ImageUnit_SetParam(&ISF_ImagePipe2, IMAGEPIPE_PARAM_IPL_CB, (UINT32)Movie_IPLCB);
    else
        ImageUnit_SetParam(&ISF_ImagePipe1, IMAGEPIPE_PARAM_IPL_CB, (UINT32)Movie_IPLCB);
    ImageUnit_SetParam(&ISF_Media, MEDIA_PARAM_REC_CB, (UINT32)Movie_RecordCB);
    #if (MOVIE_PIM_CAPTURE == ENABLE)
    ImageUnit_SetParam(&ISF_Cap, CAP_PARAM_CAP_CB, (UINT32)Movie_CaptureCB);
    ImageUnit_SetParam(&ISF_Cap, CAP_PARAM_DETS2_CB, (UINT32)NULL);
    #endif

    //#NT#2016/10/17#Wendy Liao -begin
    //#NT#Define 3DNR parameters in LibExt
    MovRec_Register3DNRCb(0, Movie_UIDefine_3DNR);
    //#NT#2016/10/17#Wendy Liao -end
}

#if (IPCAM_FUNC==ENABLE)
//-------------------------------------------------------------------------------------------------
//
// Callback function for VA stream update parameters
//
//-------------------------------------------------------------------------------------------------



void UIVAStrm_UpdParamsCB(UINT32 path)
{
    #if 0
    UINT32  uiVidCodec=0, uiFileType=0, uiWidth=0, uiHeight=0, uiTargetBitrate=0;
    UINT32  uiDispAspectRatio=0, uiVidFrameRate=0, uiGOP=0;
    UINT32  uiPath;

    UINT32  uiAudCodec, uiAudSampleRate, uiAudChannelNum, uiAudChannelType;
    BOOL    bAudFilterEn;


    Perf_Mark();

    uiAudCodec          = Movie_GetAudCodec();
    uiAudSampleRate     = Movie_GetAudSampleRate();             // audio sampling rate
    uiAudChannelType    = MOVREC_AUDTYPE_RIGHT;                 // audio channel type
    uiAudChannelNum     = 1;                                    // audio channel number
    bAudFilterEn        = FALSE;

    //---------------------------------------------------------------------------------------------
    // set audio parameters
    //---------------------------------------------------------------------------------------------
    // set audio sampling rate (AUDIO_SR_8000, AUDIO_SR_11025, AUDIO_SR_12000, ...)
    MovRec_ChangeParameter(MOVREC_RECPARAM_AUD_SAMPLERATE, uiAudSampleRate, 0, 0);
    // set audio source (AUDIO_CH_RIGHT, AUDIO_CH_LEFT, AUDIO_CH_STEREO)
    MovRec_ChangeParameter(MOVREC_RECPARAM_AUD_SRC, uiAudChannelType, 0, 0);
    // set audio channel number (1 or 2; if audio channel is mono but channel number is 2, the audio data will be duplicated)
    MovRec_ChangeParameter(MOVREC_RECPARAM_AUD_CHS, uiAudChannelNum, 0, 0);
    // set audio codec (MEDIAREC_ENC_PCM, MEDIAREC_ENC_AAC)
    MovRec_ChangeParameter(MOVREC_RECPARAM_AUD_CODEC, uiAudCodec, 0, 0);
    // set audio filter on/off
    MovRec_ChangeParameter(MOVREC_RECPARAM_EN_AUDFILTER, bAudFilterEn, 0, 0);


    for (uiPath = 0;uiPath < 1;uiPath++)
    //for (uiPath = 0;uiPath < 2;uiPath++)
    {
        uiFileType         = MEDIA_FILEFORMAT_MP4;
        uiVidCodec         = GetMovieVidEncFmt_2p(uiPath);
        uiWidth            = GetMovieSizeWidth_2p(uiPath, 0);         // image width
        uiHeight           = GetMovieSizeHeight_2p(uiPath, 0);        // image height
        uiTargetBitrate    = GetMovieTargetBitrate_2p(uiPath, 0);     // enable rate control and set target rate
        uiDispAspectRatio  = GetMovieDispAspectRatio_2p(uiPath, 0);   // display aspect ratio
        uiVidFrameRate     = GetMovieFrameRate_2p(uiPath, 0);
        uiGOP              = GetMovieGOP_2p(uiPath);

        //select parameter for input path 0
        MovRec_ChangeParameter(MOVREC_RECPARAM_FILETYPE, uiFileType, uiPath, 0);
        // set video codec
        MovRec_ChangeParameter(MOVREC_RECPARAM_VIDEOCODEC, uiVidCodec, uiPath, 0);
        // set image width
        MovRec_ChangeParameter(MOVREC_RECPARAM_WIDTH, uiWidth, uiPath, 0);
        // set image height
        MovRec_ChangeParameter(MOVREC_RECPARAM_HEIGHT, uiHeight, uiPath, 0);
        // set target data rate (bytes per second)
        MovRec_ChangeParameter(MOVREC_RECPARAM_TARGETRATE, uiTargetBitrate, uiPath, 0);
        // set target aspect ratio
        MovRec_ChangeParameter(MOVREC_RECPARAM_DAR, uiDispAspectRatio, uiPath, 0);
        // set video frame rate
        MovRec_ChangeParameter(MOVREC_RECPARAM_FRAMERATE, uiVidFrameRate, uiPath, 0);
        // set video GOP
        MovRec_ChangeParameter(MOVREC_RECPARAM_SVC, uiGOP, uiPath, 0);
    }
    DBG_DUMP("Time = %d ms",Perf_GetDuration()/1000);


    #endif
    UINT32  uiVidCodec=0, uiWidth=0, uiHeight=0, uiTargetBitrate,uiVidFrameRate=0, uiGOP=0, uiSVC=0;
    UINT32  uiBrcInfoAddr;
    MOVIE_BRC_TYPE BrcType;

    DBG_DUMP("UIVAStrm_UpdParamsCB\r\n");
    if (path >= MOVIE_MAX_CHANNEL)
    {
        DBG_ERR("path=%d\r\n",path);
        return;
    }
    uiVidCodec         = GetMovieVidEncFmt_2p(path);
    uiWidth            = GetMovieSizeWidth_2p(path, 0);         // image width
    uiHeight           = GetMovieSizeHeight_2p(path, 0);        // image height
    uiTargetBitrate    = GetMovieTargetBitrate_2p(path, 0);     // enable rate control and set target rate
    uiVidFrameRate     = GetMovieFrameRate_2p(path, 0);
    uiGOP              = GetMovieGOP_2p(path);
    uiSVC              = GetMovieSVCT_2p(path);
    BrcType            = GetMovieBRC_2p(path, &uiBrcInfoAddr);

    if (uiVidCodec == MEDIAVIDENC_H264)
    {
        DBG_DUMP("path %d, H264\r\n", path);
        MovRec_UpdateParam(MOVREC_RECPARAM_VID_ACTCODEC_ID, 0, path, 0);
    }
    else
    {
        DBG_DUMP("path %d, MJPG\r\n");
        MovRec_UpdateParam(MOVREC_RECPARAM_VID_ACTCODEC_ID, 1, path, 0);
    }
    MovRec_UpdateParam(MOVREC_RECPARAM_WIDTH, uiWidth, path, 0);
    MovRec_UpdateParam(MOVREC_RECPARAM_HEIGHT, uiHeight, path, 0);
    MovRec_UpdateParam(MOVREC_RECPARAM_TARGETRATE, uiTargetBitrate, path, 0);
    MovRec_UpdateParam(MOVREC_RECPARAM_FRAMERATE,  uiVidFrameRate, path, 0);
    MovRec_UpdateParam(MOVREC_RECPARAM_H264GOPNUM, uiGOP, path, 0);
    MovRec_UpdateParam(MOVREC_RECPARAM_SVC, uiSVC, path, 0);

    if(MOVIE_BRC_TYPE_CBR == BrcType)
        MovRec_UpdateParam(MOVREC_RECPARAM_CBRINFO, uiBrcInfoAddr, path, 0);
    else if(MOVIE_BRC_TYPE_VBR == BrcType)
        MovRec_UpdateParam(MOVREC_RECPARAM_VBRINFO, uiBrcInfoAddr, path, 0);

    DBG_DUMP("uiGOP = %d\r\n",uiGOP);
}
#endif

