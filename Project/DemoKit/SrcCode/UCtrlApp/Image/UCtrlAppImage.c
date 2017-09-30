
#include "AppControl.h"
#include "SysCommon.h"
#include "UIFramework.h"
#include "PrjCfg.h"
#include "uToken.h"
#include "UIInfo.h"
#include "UCtrlAppImage.h"
#include "NvtUctrlAPI.h"
#include "NvtIpcAPI.h"
#include "UCtrlMain.h"
#include "UISetup.h"
#include "IPL_Utility.h"
#include "IPL_AlgInfor.h"
#include "awb_task.h"
#include "awb_api.h"
#include "ae_task.h"
#include "ae_api.h"
#include "afdrv_common.h"
#include "UIAppMovie.h"
#include "UIAppPhoto.h"
#include "iqs_api.h"
#include "VideoCodec_H264.h"
#include "MovieStamp.h"
#include "IPL_Ctrl.h"
#include "NvtMem.h"
#if (IPCAM_FUNC==ENABLE)
#include "ImageApp_IPCam.h"
#endif
#define THIS_DBGLVL         2 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UCtrlAppImage
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"


#define SXCMD_TEST          0


// flag define
#define FLG_UCTRL_IMAGE_START           FLGPTN_BIT(0)
#define FLG_UCTRL_IMAGE_EXIT            FLGPTN_BIT(1)

static UCtrlImageInfoType   imageInfo;
static UCtrlImageInfoType*  pImageInfo = &imageInfo;
#if(IPCAM_FUNC == ENABLE)
static BOOL bMovieModeRestart = FALSE;
#endif
static INT32 param_ipl_id_set(INT32 argc, char* argv[])
{
    UINT32 value;
    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }
    value = atoi(argv[0]);
    if (value >= IPL_ID_MAX_NUM)
    {
        DBG_ERR("value =%d\r\n",value);
        return -1;
    }

    pImageInfo->ipl_id.value = value;
    pImageInfo->ipl_id.isSet = 1;

    return 0;
}

static INT32 param_backlight(INT32 argc, char* argv[])
{
    if(argc!=2)
    {
        DBG_ERR("argc!=2 (%d)\r\n",argc);
        return -1;
    }
    pImageInfo->backlight.isSet = 1;
    pImageInfo->backlight.mode = atoi(argv[0]);
    pImageInfo->backlight.lv   = atoi(argv[1]);
    return 0;
}

static INT32 param_brightness(INT32 argc, char* argv[])
{
    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }

    pImageInfo->brightness.isSet = 1;
    pImageInfo->brightness.value = atoi(argv[0]);
    return 0;
}

static INT32 param_saturation(INT32 argc, char* argv[])
{
    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }
    pImageInfo->saturation.isSet = 1;
    pImageInfo->saturation.value = atoi(argv[0]);
    return 0;
}

static INT32 param_contrast  (INT32 argc, char* argv[])
{
    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }
    pImageInfo->contrast.isSet = 1;
    pImageInfo->contrast.value = atoi(argv[0]);
    return 0;
}

static INT32 param_ae        (INT32 argc, char* argv[])
{
    if(argc!=11)
    {
        DBG_ERR("argc!=11 (%d)\r\n",argc);
        return -1;
    }
    pImageInfo->ae.isSet = 1;
    pImageInfo->ae.mode    = atoi(argv[0]);
    pImageInfo->ae.priority= atoi(argv[1]);
    pImageInfo->ae.maxexp  = atoi(argv[2]);
    pImageInfo->ae.minexp  = atoi(argv[3]);
    pImageInfo->ae.maxgain = atoi(argv[4]);
    pImageInfo->ae.mingain = atoi(argv[5]);
    pImageInfo->ae.maxiris = atoi(argv[6]);
    pImageInfo->ae.miniris = atoi(argv[7]);
    pImageInfo->ae.exp     = atoi(argv[8]);
    pImageInfo->ae.gain    = atoi(argv[9]);
    pImageInfo->ae.iris    = atoi(argv[10]);
    return 0;
}

static INT32 param_aewin  (INT32 argc, char* argv[])
{
    if(argc!=4)
    {
        DBG_ERR("argc!=4 (%d)\r\n",argc);
        return -1;
    }
    pImageInfo->aewin.isSet = 1;
    pImageInfo->aewin.left  = atoi(argv[0]);
    pImageInfo->aewin.right = atoi(argv[1]);
    pImageInfo->aewin.top   = atoi(argv[2]);
    pImageInfo->aewin.bottom= atoi(argv[3]);
    return 0;
}

static INT32 param_focus     (INT32 argc, char* argv[])
{
    if(argc!=4)
    {
        DBG_ERR("argc!=4 (%d)\r\n",argc);
        return -1;
    }
    pImageInfo->focus.isSet = 1;
    pImageInfo->focus.mode  = atoi(argv[0]);
    pImageInfo->focus.speed = atoi(argv[1]);
    pImageInfo->focus.near  = atoi(argv[2]);
    pImageInfo->focus.far   = atoi(argv[3]);
    return 0;
}

static INT32 param_ircut     (INT32 argc, char* argv[])
{
    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }
    pImageInfo->ircut.isSet = 1;
    pImageInfo->ircut.value  = atoi(argv[0]);
    return 0;
}

static INT32 param_wdr       (INT32 argc, char* argv[])
{
    if(argc!=2)
    {
        DBG_ERR("argc!=2 (%d)\r\n",argc);
        return -1;
    }
    pImageInfo->wdr.isSet = 1;
    pImageInfo->wdr.mode  = atoi(argv[0]);
    pImageInfo->wdr.lv    = atoi(argv[1]);
    return 0;
}

static INT32 param_awb       (INT32 argc, char* argv[])
{
    if(argc!=3)
    {
        DBG_ERR("argc!=3 (%d)\r\n",argc);
        return -1;
    }
    pImageInfo->awb.isSet = 1;
    pImageInfo->awb.mode  = atoi(argv[0]);
    pImageInfo->awb.rgain = atoi(argv[1]);
    pImageInfo->awb.bgain = atoi(argv[2]);
    return 0;
}

static INT32 param_dis       (INT32 argc, char* argv[])
{
    if(argc!=2)
    {
        DBG_ERR("argc!=2 (%d)\r\n",argc);
        return -1;
    }
    pImageInfo->dis.isSet = 1;
    pImageInfo->dis.mode  = atoi(argv[0]);
    pImageInfo->dis.lv    = atoi(argv[1]);
    return 0;
}


static INT32 param_sharpness(INT32 argc, char* argv[])
{
    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }
    pImageInfo->sharpness.isSet = 1;
    pImageInfo->sharpness.value  = atoi(argv[0]);
    return 0;
}

static INT32 param_focusmove_absolute(INT32 argc, char* argv[])
{
    if(argc!=2)
    {
        DBG_ERR("argc!=2 (%d)\r\n",argc);
        return -1;
    }
    pImageInfo->focusmove_absolute.isSet = 1;
    pImageInfo->focusmove_absolute.position = atoi(argv[0]);
    pImageInfo->focusmove_absolute.speed    = atoi(argv[1]);
    return 0;
}

static INT32 param_focusmove_continuous(INT32 argc, char* argv[])
{
    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }
    pImageInfo->focusmove_continuous.isSet = 1;
    pImageInfo->focusmove_continuous.speed = atoi(argv[0]);
    return 0;
}

static INT32 param_focusmove_relative(INT32 argc, char* argv[])
{
    if(argc!=2)
    {
        DBG_ERR("argc!=2 (%d)\r\n",argc);
        return -1;
    }
    pImageInfo->focusmove_relative.isSet = 1;
    pImageInfo->focusmove_relative.distance = atoi(argv[0]);
    pImageInfo->focusmove_relative.speed    = atoi(argv[1]);
    return 0;
}

static INT32 param_focusmove_stop(INT32 argc, char* argv[])
{
    pImageInfo->focusmove_stop.isSet = 1;
    return 0;
}

static INT32 param_focus_status(INT32 argc, char* argv[])
{
    pImageInfo->focus_status.isSet = 1;
    pImageInfo->focus_status.status = 1;
    pImageInfo->focus_status.position = 2;
    pImageInfo->focus_status.errcode = 3;
    return 0;
}

static INT32 param_image_setting_get(INT32 argc, char* argv[])
{
    UINT8 ipl_id;
    ISPImageSetting sImageSetting={0};
    IQS_EXT_SET_IQ_INFO IQSExtSetIQInfo = {0};

    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }

    if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM)
    {
        DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n",ipl_id, IPL_ID_MAX_NUM);
        return -1;
    }

    //sImageSetting.uiBrightness = IPL_AlgGetUIInfo(ipl_id, IPL_SEL_BRIGHTNESS);
    //sImageSetting.uiContrast = IPL_AlgGetUIInfo(ipl_id, IPL_SEL_CONTRAST);
    //sImageSetting.uiSaturation = IPL_AlgGetUIInfo(ipl_id, IPL_SEL_SATURATION);
    //sImageSetting.uiHue = IPL_AlgGetUIInfo(ipl_id, IPL_SEL_HUE);
    //sImageSetting.uiSharpness = IPL_AlgGetUIInfo(ipl_id, IPL_SEL_SHARPNESS);

    IQS_GetIQInfo(ipl_id, &IQSExtSetIQInfo);
    sImageSetting.uiBrightness = IQSExtSetIQInfo.brightness.ratio;
    sImageSetting.uiContrast = IQSExtSetIQInfo.contrast.ratio;
    sImageSetting.uiSaturation = IQSExtSetIQInfo.saturation.ratio;
    sImageSetting.uiHue = IQSExtSetIQInfo.hue.ratio;
    sImageSetting.uiSharpness = IQSExtSetIQInfo.sharpness.ratio;
    //#NT#2016/07/21#Charlie Chang -begin
    //#NT# adding aspect ratio
    #if (IPCAM_FUNC==ENABLE)
    sImageSetting.aspect_ratio = ImageApp_IPCam_GetConfig(IPCAM_CFG_RATIO);
    #endif
    //#NT#2016/07/21#Charlie Chang -end
    switch (IPL_AlgGetUIInfo(ipl_id, IPL_SEL_IMAGEEFFECT))
    {
        case SEL_IMAGEEFFECT_BW:
            sImageSetting.eColor = ISP_COLOR_TYPE_GRAY;
            break;
        case SEL_IMAGEEFFECT_OFF:
        default:
            sImageSetting.eColor = ISP_COLOR_TYPE_NORMAL;
            break;
    }

    switch (IPL_AlgGetUIInfo(ipl_id, IPL_SEL_IMAGEFLIP))
    {
        //#NT#2016/03/02#David Tsai -begin
        //#NT#Replace sensor mirror/flip with DSP mirror/flip
        case SEL_IMAGEFLIP_DSP_H_V:
            sImageSetting.bMirror = NVT_TRUE;
            sImageSetting.bFlip = NVT_TRUE;
            break;
        case SEL_IMAGEFLIP_DSP_H:
            sImageSetting.bMirror = NVT_TRUE;
            sImageSetting.bFlip = NVT_FALSE;
            break;
        case SEL_IMAGEFLIP_DSP_V:
            sImageSetting.bMirror = NVT_FALSE;
            sImageSetting.bFlip = NVT_TRUE;
            break;
        case SEL_IMAGEFLIP_DSP_OFF:
        default:
            sImageSetting.bMirror = NVT_FALSE;
            sImageSetting.bFlip = NVT_FALSE;
            break;
        //#NT#2016/03/02#David Tsai -end
    }

    //#NT#2016/07/28#David Tsai -begin
    //#NT#Add IR cut control in image setting
    switch (IPL_AlgGetUIInfo(ipl_id, IPL_SEL_IRCUT))
    {
        case SEL_IRCUT_NIGHT:
            sImageSetting.eIRCutMode = ISP_IRCUT_TYPE_NIGHT;
            break;
        case SEL_IRCUT_DAY:
            sImageSetting.eIRCutMode = ISP_IRCUT_TYPE_DAY;
            break;
        case SEL_IRCUT_AUTO:
        default:
            sImageSetting.eIRCutMode = ISP_IRCUT_TYPE_AUTO;
            break;
    }
    //#NT#2016/07/28#David Tsai -end

	//#NT#2017/01/25#Charlie Chang -begin
    //#NT# add dewarping parameter
	#if(IPCAM_FUNC == ENABLE)
		sImageSetting.bDewarp = ImageApp_IPCam_GetConfig(IPCAM_CFG_DEWARP_FLAG);
	#endif
	//#NT#2017/01/25#Charlie Chang -end

    NvtUctrl_SetConfigData((void *)&sImageSetting, sizeof(ISPImageSetting));

    return 0;
}

static INT32 param_image_setting_set(INT32 argc, char* argv[])
{
    UINT8 ipl_id;
    ISPImageSetting sImageSetting = {0};

    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }

    if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM)
    {
        DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n",ipl_id, IPL_ID_MAX_NUM);
        return -1;
    }

    NvtUctrl_GetConfigData((void *)&sImageSetting, sizeof(ISPImageSetting));

	// dewarp enable
	#if(IPCAM_FUNC == ENABLE)
	BOOL dewarp_enable = ImageApp_IPCam_GetConfig(IPCAM_CFG_DEWARP_FLAG);
	if(dewarp_enable != sImageSetting.bDewarp){
		pImageInfo->dewarp_enable.isSet =1;
		pImageInfo->dewarp_enable.value = sImageSetting.bDewarp;
	}
	#endif
    // IPL channel
    pImageInfo->ipl_id.isSet = 1;
    pImageInfo->ipl_id.value = ipl_id;

    // Brightness (0~100)
    pImageInfo->brightness.isSet = 1;
    pImageInfo->brightness.value = sImageSetting.uiBrightness;

    // Contrast (0~100)
    pImageInfo->contrast.isSet = 1;
    pImageInfo->contrast.value = sImageSetting.uiContrast;

    // Saturation (0~100)
    pImageInfo->saturation.isSet = 1;
    pImageInfo->saturation.value = sImageSetting.uiSaturation;

    // Hue (0~360)
    pImageInfo->hue.isSet = 1;
    pImageInfo->hue.value = sImageSetting.uiHue;

    // Sharpness (0~100)
    pImageInfo->sharpness.isSet = 1;
    pImageInfo->sharpness.value = sImageSetting.uiSharpness;

    if (ISP_COLOR_TYPE_NORMAL == sImageSetting.eColor)
        IPL_AlgSetUIInfo(ipl_id, IPL_SEL_IMAGEEFFECT, SEL_IMAGEEFFECT_OFF);
    else
        IPL_AlgSetUIInfo(ipl_id, IPL_SEL_IMAGEEFFECT, SEL_IMAGEEFFECT_BW);

    //#NT#2016/03/02#David Tsai -begin
    //#NT#Replace sensor mirror/flip with DSP mirror/flip
    if(sImageSetting.bMirror == NVT_TRUE && sImageSetting.bFlip == NVT_TRUE)
        IPL_AlgSetUIInfo(ipl_id, IPL_SEL_IMAGEFLIP, SEL_IMAGEFLIP_DSP_H_V);
    else if (sImageSetting.bMirror)
        IPL_AlgSetUIInfo(ipl_id, IPL_SEL_IMAGEFLIP, SEL_IMAGEFLIP_DSP_H);
    else if (sImageSetting.bFlip)
        IPL_AlgSetUIInfo(ipl_id, IPL_SEL_IMAGEFLIP, SEL_IMAGEFLIP_DSP_V);
    else
        IPL_AlgSetUIInfo(ipl_id, IPL_SEL_IMAGEFLIP, SEL_IMAGEFLIP_DSP_OFF);
    //#NT#2016/03/02#David Tsai -end

    //#NT#2016/07/28#David Tsai -begin
    //#NT#Add IR cut control in image setting
    pImageInfo->ircut.isSet = 1;
    switch (sImageSetting.eIRCutMode)
    {
        case ISP_IRCUT_TYPE_NIGHT:
            pImageInfo->ircut.value = MOVIE_IR_CUT_OFF;
            break;
        case ISP_IRCUT_TYPE_DAY:
            pImageInfo->ircut.value = MOVIE_IR_CUT_ON;
            break;
        case ISP_IRCUT_TYPE_AUTO:
        default:
            pImageInfo->ircut.value = MOVIE_IR_CUT_AUTO;
            break;
    }
    //#NT#2016/07/28#David Tsai -end

    return 0;
}

static INT32 param_ae_get(INT32 argc, char* argv[])
{
    UINT8 ipl_id;
    ISPAESetting sAESetting={0};
    AE_EXT_SET_INFO AeSetInfo = {0};

    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }

    if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM)
    {
        DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n",ipl_id, IPL_ID_MAX_NUM);
        return -1;
    }

    AeSetInfo.ExpAreaWeight = sAESetting.uiExpAreaWeight;
    AE_GetExtInfo(ipl_id, &AeSetInfo);
    switch (AeSetInfo.PLineMode)
    {
        case AE_PLINE_MANUAL:
            sAESetting.eAEMode = ISP_CTRL_MODE_MANUAL;
            break;
        case AE_PLINE_AUTOGEN:
        default:
            sAESetting.eAEMode = ISP_CTRL_MODE_AUTO;
            break;
    }

    sAESetting.uiShutterSpeedMax = AeSetInfo.ExpTBound.h;
    sAESetting.uiShutterSpeedMin = AeSetInfo.ExpTBound.l;
    sAESetting.uiGainMax = AeSetInfo.IsoBound.h;
    sAESetting.uiGainMin = AeSetInfo.IsoBound.l;
    sAESetting.uiConvergenceSpeed = AeSetInfo.ConvergenceSpeed;
    sAESetting.uiManualShutterSpeed = AeSetInfo.ExpT;
    sAESetting.uiManualGain = AeSetInfo.ISO;
    // sAESetting.uiConvergenceStep
    sAESetting.eAntiFlicker = IPL_AlgGetUIInfo(ipl_id, IPL_SEL_FREQUENCY);
    // sAESetting.sIrisSetting
    sAESetting.uiEV = IPL_AlgGetUIInfo(ipl_id, IPL_SEL_AEEV);
    sAESetting.uiAETolerance = AeSetInfo.AETolerance;
    sAESetting.uiAEDelay = AeSetInfo.AEDelayCnt;
    sAESetting.eAEStrategy = AeSetInfo.AEStrategy;

    NvtUctrl_SetConfigData((void *)&sAESetting, sizeof(ISPAESetting));

    return 0;
}

static INT32 param_ae_set(INT32 argc, char* argv[])
{
    UINT8 ipl_id;
    ISPAESetting sAESetting={0};
    AE_EXT_SET_INFO AESetInfo={0};

    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }

    if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM)
    {
        DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n",ipl_id, IPL_ID_MAX_NUM);
        return -1;
    }

    NvtUctrl_GetConfigData((void *)&sAESetting, sizeof(ISPAESetting));

    pImageInfo->ipl_id.value = ipl_id;
    pImageInfo->ipl_id.isSet = 1;

    IPL_AlgSetUIInfo(ipl_id, IPL_SEL_FREQUENCY, sAESetting.eAntiFlicker);
    IPL_AlgSetUIInfo(ipl_id, IPL_SEL_AEEV, sAESetting.uiEV);

    if (sAESetting.uiManualShutterSpeed == 0)
        sAESetting.uiManualShutterSpeed = 33333;
    if (sAESetting.uiManualGain == 0)
        sAESetting.uiManualGain = 100;

    pImageInfo->ae.isSet = 1;
    if (sAESetting.eAEMode == ISP_CTRL_MODE_AUTO)
    {
        AESetInfo.PLineMode = AE_PLINE_AUTOGEN;
        pImageInfo->ae.mode    = 0;
        pImageInfo->ae.maxexp  = sAESetting.uiShutterSpeedMax;
        pImageInfo->ae.minexp  = sAESetting.uiShutterSpeedMin;
        pImageInfo->ae.maxgain = sAESetting.uiGainMax;
        pImageInfo->ae.mingain = sAESetting.uiGainMin;
        //pImageInfo->ae.priority= atoi(argv[1]);
        //pImageInfo->ae.maxiris = atoi(argv[6]);
        //pImageInfo->ae.miniris = atoi(argv[7]);
    }
    else
    {
        AESetInfo.PLineMode = AE_PLINE_MANUAL;
        AESetInfo.ExpT = sAESetting.uiManualShutterSpeed;
        AESetInfo.ISO = sAESetting.uiManualGain;
        pImageInfo->ae.mode    = 1;
        pImageInfo->ae.exp     = sAESetting.uiManualShutterSpeed;
        pImageInfo->ae.gain    = sAESetting.uiManualGain;
        //pImageInfo->ae.iris    = atoi(argv[10]);
    }

    AESetInfo.AEDelayCnt= sAESetting.uiAEDelay; // Unit: frame
    AESetInfo.AEStrategy = sAESetting.eAEStrategy;
    AESetInfo.AETolerance = sAESetting.uiAETolerance; // 3~100, Default: 3
    AESetInfo.ConvergenceSpeed = sAESetting.uiConvergenceSpeed;  // 0~6, Default: 4
    AESetInfo.ExpAreaWeight = sAESetting.uiExpAreaWeight; // 0~255
    AESetInfo.ExpTBound.h = sAESetting.uiShutterSpeedMax;
    AESetInfo.ExpTBound.l = sAESetting.uiShutterSpeedMin;
    AESetInfo.IsoBound.h = sAESetting.uiGainMax;
    AESetInfo.IsoBound.l = sAESetting.uiGainMin;
    //AESetInfo.FnRatioBound

    AE_SetExtInfo(ipl_id, AESetInfo);
    IPL_AlgSetUIInfo(ipl_id, IPL_SEL_AEMODE, SEL_AEMODE_USERDEFINE);

    return 0;
}

static INT32 param_ae_status_get(INT32 argc, char* argv[])
{
    UINT8 ipl_id;
    ISPAEStatus sAEStatus={0};
    AEStatus nativeAEStatus={0};

    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }

    if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM)
    {
        DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n",ipl_id, IPL_ID_MAX_NUM);
        return -1;
    }

    AE_GetStatus(ipl_id, &nativeAEStatus);
    sAEStatus.uiShutter = nativeAEStatus.ExpT;
    sAEStatus.uiGain = nativeAEStatus.Gain;
    memcpy(sAEStatus.uiLumaValue, nativeAEStatus.LumaValue,
           sizeof(UINT16) * AE_LUMA_ROW_NUM * AE_LUMA_COLUMN_NUM);
    sAEStatus.uiLumaAverage = nativeAEStatus.LumaAverage;
    memcpy(sAEStatus.uiLumaHistogram, nativeAEStatus.LumaHistogram,
           sizeof(UINT16) * AE_LUMA_HIST_NUM);
    sAEStatus.uiPreviewLumaValue = AE_GetPrvEVValue(ipl_id);

    NvtUctrl_SetConfigData((void *)&sAEStatus, sizeof(ISPAEStatus));

    return 0;
}

static INT32 param_wb_get(INT32 argc, char* argv[])
{
    UINT8 ipl_id;
    ISPWBSetting sWBSetting={0};
    AwbSetParam awbsetting={0};

    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }

    if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM)
    {
        DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n",ipl_id, IPL_ID_MAX_NUM);
        return -1;
    }

    AWB_GetInfo(ipl_id, &awbsetting);
    if (IPL_AlgGetUIInfo(ipl_id, IPL_SEL_WBMODE) == SEL_WBMODE_AUTO)
        sWBSetting.eWBMode = ISP_CTRL_MODE_AUTO;
    else
        sWBSetting.eWBMode = ISP_CTRL_MODE_MANUAL;

    sWBSetting.uiConvergenceSpeed = awbsetting.uiConvSpeed;
    sWBSetting.uiConvergenceStep = awbsetting.uiConvStep;
    sWBSetting.uiManualRGain = awbsetting.uiManualRGain;
    sWBSetting.uiManualGGain = awbsetting.uiManualGGain;
    sWBSetting.uiManualBGain = awbsetting.uiManualBGain;

    NvtUctrl_SetConfigData((void *)&sWBSetting, sizeof(ISPWBSetting));

    return 0;
}

static INT32 param_wb_set(INT32 argc, char* argv[])
{
    UINT8 ipl_id;
    ISPWBSetting sWBSetting={0};
    AwbSetParam awbsetting={0};

    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }

    if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM)
    {
        DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n",ipl_id, IPL_ID_MAX_NUM);
        return -1;
    }

    NvtUctrl_GetConfigData((void *)&sWBSetting, sizeof(ISPWBSetting));

    pImageInfo->ipl_id.value = ipl_id;
    pImageInfo->ipl_id.isSet = 1;

    pImageInfo->awb.isSet = 1;

    if (ISP_CTRL_MODE_AUTO == sWBSetting.eWBMode)
    {
        pImageInfo->awb.mode  = 0;
    }
    else
    {
        pImageInfo->awb.mode  = 1;
        if (sWBSetting.uiManualRGain < 1023)
            pImageInfo->awb.rgain = sWBSetting.uiManualRGain;
        else
            pImageInfo->awb.rgain = 1023;

        // Manual green gain is reserved
        /*
        if (psWBSetting->uiManualGGain < 256)
            pImageInfo->awb.ggain = psWBSetting->uiManualGGain;
        else
            pImageInfo->awb.ggain = 256;
        */
        if (sWBSetting.uiManualBGain < 1023)
            pImageInfo->awb.bgain = sWBSetting.uiManualBGain;
        else
            pImageInfo->awb.bgain = 1023;
    }

    if (sWBSetting.uiConvergenceSpeed < SEL_AWB_CONV_SPEED_LEVEL_MAX_CNT)
        awbsetting.uiConvSpeed = sWBSetting.uiConvergenceSpeed;
    else
        awbsetting.uiConvSpeed = SEL_AWB_CONV_SPEED_LEVEL_MAX_CNT - 1;

//#NT#2016/10/18#Jeah Yen -begin
//#NT#fix id for support sensor map
    AWB_SetInfo(ipl_id, AWB_SET_SPEED, &awbsetting);

    if (sWBSetting.uiConvergenceStep < SEL_AWB_CONV_STEP_LEVEL_MAX_CNT)
        awbsetting.uiConvStep = sWBSetting.uiConvergenceStep;
    else
        awbsetting.uiConvStep = SEL_AWB_CONV_STEP_LEVEL_MAX_CNT - 1;
    AWB_SetInfo(ipl_id, AWB_SET_STEP, &awbsetting);
//#NT#2016/10/18#Jeah Yen -begin

    return 0;
}

static INT32 param_wb_status_get(INT32 argc, char* argv[])
{
    UINT8 ipl_id;
    ISPWBStatus sWBStatus={0};
    AWBStatus nativeAWBStatus={0};

    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }

    if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM)
    {
        DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n",ipl_id, IPL_ID_MAX_NUM);
        return -1;
    }

    AWB_GetStatus(ipl_id, &nativeAWBStatus);
    sWBStatus.uiRGain = nativeAWBStatus.RGain;
    sWBStatus.uiGGain = nativeAWBStatus.GGain;
    sWBStatus.uiBGain = nativeAWBStatus.BGain;
    sWBStatus.uiColorTemperature = nativeAWBStatus.ColorTemperature;
    AWB_Get_ColorAvg(ipl_id, &(sWBStatus.uiRColorAverage), &(sWBStatus.uiGColorAverage), &(sWBStatus.uiBColorAverage));

    NvtUctrl_SetConfigData((void *)&sWBStatus, sizeof(ISPWBStatus));

    return 0;
}

static INT32 param_wdr_get(INT32 argc, char* argv[])
{
    UINT8 ipl_id;
    ISPWDRSetting sWDRSetting={0};

    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }

    if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM)
    {
        DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n",ipl_id, IPL_ID_MAX_NUM);
        return -1;
    }

    switch (IPL_AlgGetUIInfo(ipl_id, IPL_SEL_WDR))
    {
        case SEL_WDR_ON:
            sWDRSetting.bWDREnable = NVT_TRUE;
            sWDRSetting.eWDRMode = ISP_CTRL_MODE_MANUAL;
            break;
        case SEL_WDR_AUTO:
            sWDRSetting.bWDREnable = NVT_TRUE;
            sWDRSetting.eWDRMode = ISP_CTRL_MODE_AUTO;
            break;
        case SEL_WDR_OFF:
        default:
            sWDRSetting.bWDREnable = NVT_FALSE;
            sWDRSetting.eWDRMode = ISP_CTRL_MODE_MANUAL;
            break;
    }

    switch (IPL_AlgGetUIInfo(ipl_id, IPL_SEL_WDR_LEVEL))
    {
        case SEL_WDR_LV0:
            sWDRSetting.uiWDRStrength = 0;
            break;
        case SEL_WDR_LV1:
            sWDRSetting.uiWDRStrength = 1;
            break;
        case SEL_WDR_LV2:
            sWDRSetting.uiWDRStrength = 2;
            break;
        case SEL_WDR_LV3:
            sWDRSetting.uiWDRStrength = 3;
            break;
        case SEL_WDR_LV4:
            sWDRSetting.uiWDRStrength = 4;
            break;
        case SEL_WDR_LV5:
            sWDRSetting.uiWDRStrength = 5;
            break;
        case SEL_WDR_LV6:
            sWDRSetting.uiWDRStrength = 6;
            break;
        case SEL_WDR_LV7:
            sWDRSetting.uiWDRStrength = 7;
            break;
        case SEL_WDR_LV8:
            sWDRSetting.uiWDRStrength = 8;
            break;
        case SEL_WDR_LV9:
            sWDRSetting.uiWDRStrength = 9;
            break;
        case SEL_WDR_LV10:
        default:
            sWDRSetting.uiWDRStrength = 10;
            break;
    }

    NvtUctrl_SetConfigData((void *)&sWDRSetting, sizeof(ISPWDRSetting));

    return 0;
}

static INT32 param_wdr_set(INT32 argc, char* argv[])
{
    UINT8 ipl_id;
    ISPWDRSetting sWDRSetting = {0};

    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }

    if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM)
    {
        DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n",ipl_id, IPL_ID_MAX_NUM);
        return -1;
    }

    NvtUctrl_GetConfigData((void *)&sWDRSetting, sizeof(ISPWDRSetting));

    pImageInfo->ipl_id.value = ipl_id;
    pImageInfo->ipl_id.isSet = 1;

    pImageInfo->wdr.isSet = 1;
    if(sWDRSetting.bWDREnable == NVT_TRUE){
        if(sWDRSetting.eWDRMode == ISP_CTRL_MODE_MANUAL){
            pImageInfo->wdr.mode = SEL_WDR_ON;
        }
        else{
            pImageInfo->wdr.mode = SEL_WDR_AUTO;
        }
    }
    else{
        pImageInfo->wdr.mode = SEL_WDR_OFF;
    }


    switch (sWDRSetting.uiWDRStrength)
    {
        case 0:
            pImageInfo->wdr.lv = SEL_WDR_LV0;
            break;
        case 1:
            pImageInfo->wdr.lv = SEL_WDR_LV1;
            break;
        case 2:
            pImageInfo->wdr.lv = SEL_WDR_LV2;
            break;
        case 3:
            pImageInfo->wdr.lv = SEL_WDR_LV3;
            break;
        case 4:
            pImageInfo->wdr.lv = SEL_WDR_LV4;
            break;
        case 5:
            pImageInfo->wdr.lv = SEL_WDR_LV5;
            break;
        case 6:
            pImageInfo->wdr.lv = SEL_WDR_LV6;
            break;
        case 7:
            pImageInfo->wdr.lv = SEL_WDR_LV7;
            break;
        case 8:
            pImageInfo->wdr.lv = SEL_WDR_LV8;
            break;
        case 9:
            pImageInfo->wdr.lv = SEL_WDR_LV9;
            break;
        case 10:
        default:
            pImageInfo->wdr.lv = SEL_WDR_LV10;
            break;
    }

    return 0;
}

static INT32 param_nr_get(INT32 argc, char* argv[])
{
    UINT8 ipl_id;
    ISPNRSetting sNRSetting={0};
    IQS_EXT_SET_IQ_INFO IQSExtSetIQInfo = {0};
    UINT32 path_id;

    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }

    if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM)
    {
        DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n",ipl_id, IPL_ID_MAX_NUM);
        return -1;
    }

    IQS_GetIQInfo(ipl_id, &IQSExtSetIQInfo);
    sNRSetting.uiNR2DStrength = IQSExtSetIQInfo.nr2D.ratio;
    sNRSetting.uiNR3DStrength = IQSExtSetIQInfo.nr3D.ratio;

    NvtUctrl_SetConfigData((void *)&sNRSetting, sizeof(ISPNRSetting));
//#NT#2016/04/01#David Tsai -begin
//#NT#Print 3DNR running level when get noise reduction parameters to check 3DNR status
    DBG_DUMP("3DNR Level:\r\n");
    for (path_id = 0; path_id < VIDENC_ID_MAX; path_id++)
        DBG_DUMP("  Path %u = %u\r\n", path_id, MP_H264Enc_Get3DNRLevel(path_id));
//#NT#2016/04/01#David Tsai -end
    return 0;
}

static INT32 param_nr_set(INT32 argc, char* argv[])
{
    UINT8 ipl_id;
    ISPNRSetting sNRSetting={0};

    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }

    if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM)
    {
        DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n",ipl_id, IPL_ID_MAX_NUM);
        return -1;
    }

    NvtUctrl_GetConfigData((void *)&sNRSetting, sizeof(ISPNRSetting));

    pImageInfo->ipl_id.value = ipl_id;
    pImageInfo->ipl_id.isSet = 1;

    pImageInfo->nr2D.isSet = 1;
    pImageInfo->nr2D.value = sNRSetting.uiNR2DStrength; // 0~100

    pImageInfo->nr3D.isSet = 1;
    pImageInfo->nr3D.mode = 1; // Manual
    pImageInfo->nr3D.ratio = sNRSetting.uiNR3DStrength; // 0~10

    return 0;
}

static INT32 param_dis_get(INT32 argc, char* argv[])
{
    UINT8 ipl_id;
    ISPDISSetting sDISSetting={0};

    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }

    if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM)
    {
        DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n",ipl_id, IPL_ID_MAX_NUM);
        return -1;
    }

    switch (IPL_AlgGetUIInfo(ipl_id, IPL_SEL_DISCOMP))
    {
        case SEL_DISCOMP_ON:
            sDISSetting.bDISEnable = NVT_TRUE;
            break;
        case SEL_DISCOMP_OFF:
        default:
            sDISSetting.bDISEnable = NVT_FALSE;
            break;
    }

    NvtUctrl_SetConfigData((void *)&sDISSetting, sizeof(ISPDISSetting));

    return 0;
}

static INT32 param_dis_set(INT32 argc, char* argv[])
{
    UINT8 ipl_id;
    ISPDISSetting sDISSetting = {0};

    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }

    if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM)
    {
        DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n",ipl_id, IPL_ID_MAX_NUM);
        return -1;
    }

    NvtUctrl_GetConfigData((void *)&sDISSetting, sizeof(ISPDISSetting));

    pImageInfo->ipl_id.value = ipl_id;
    pImageInfo->ipl_id.isSet = 1;

    pImageInfo->dis.isSet = 1;
    if (sDISSetting.bDISEnable == NVT_TRUE)
        pImageInfo->dis.mode = 1;
    else
        pImageInfo->dis.mode = 0;

    // pImageInfo->dis.lv

    return 0;
}

static INT32 param_ldc_get(INT32 argc, char* argv[])
{
    UINT8 ipl_id;
    ISPLDCSetting sLDCSetting={0};
    IQS_EXT_SET_IQ_INFO IQSExtSetIQInfo = {0};

    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }

    if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM)
    {
        DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n",ipl_id, IPL_ID_MAX_NUM);
        return -1;
    }


    IQS_GetIQInfo(ipl_id, &IQSExtSetIQInfo);

    sLDCSetting.bLDCEnable = IQSExtSetIQInfo.GDC.isEnable;
    sLDCSetting.uiRatio = IQSExtSetIQInfo.GDC.ratio;

    NvtUctrl_SetConfigData((void *)&sLDCSetting, sizeof(ISPLDCSetting));

    return 0;
}

static INT32 param_ldc_set(INT32 argc, char* argv[])
{
    UINT8 ipl_id;
    ISPLDCSetting sLDCSetting = {0};

    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }

    if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM)
    {
        DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n",ipl_id, IPL_ID_MAX_NUM);
        return -1;
    }

    NvtUctrl_GetConfigData((void *)&sLDCSetting, sizeof(ISPLDCSetting));

    pImageInfo->ipl_id.value = ipl_id;
    pImageInfo->ipl_id.isSet = 1;

    pImageInfo->LDC.isSet = 1;
    pImageInfo->LDC.isEnable = sLDCSetting.bLDCEnable;
    pImageInfo->LDC.ratio = sLDCSetting.uiRatio; // 0~100

    return 0;
}

static INT32 param_lsc_get(INT32 argc, char* argv[])
{
    UINT8 ipl_id;
    ISPLSCSetting sLSCSetting={0};
    IQS_EXT_SET_IQ_INFO IQSExtSetIQInfo = {0};

    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }

    if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM)
    {
        DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n",ipl_id, IPL_ID_MAX_NUM);
        return -1;
    }


    IQS_GetIQInfo(ipl_id, &IQSExtSetIQInfo);

    sLSCSetting.bLSCEnable = IQSExtSetIQInfo.LSC.isEnable;
    sLSCSetting.uiRatio = IQSExtSetIQInfo.LSC.ratio;
    sLSCSetting.uiCenterXOffset = IQSExtSetIQInfo.LSC.CenterXOffset;
    sLSCSetting.uiCenterYOffset = IQSExtSetIQInfo.LSC.CenterYOffset;

    NvtUctrl_SetConfigData((void *)&sLSCSetting, sizeof(ISPLSCSetting));

    return 0;
}

static INT32 param_lsc_set(INT32 argc, char* argv[])
{
    UINT8 ipl_id;
    ISPLSCSetting sLSCSetting={0};

    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }

    if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM)
    {
        DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n",ipl_id, IPL_ID_MAX_NUM);
        return -1;
    }

    NvtUctrl_GetConfigData((void *)&sLSCSetting, sizeof(ISPLSCSetting));

    pImageInfo->ipl_id.value = ipl_id;
    pImageInfo->ipl_id.isSet = 1;

    pImageInfo->LSC.isSet = 1;
    pImageInfo->LSC.isEnable = sLSCSetting.bLSCEnable;
    pImageInfo->LSC.ratio = sLSCSetting.uiRatio;  // 0~100, default 0
    pImageInfo->LSC.CenterXOffset = sLSCSetting.uiCenterXOffset; // 0~100, default 50
    pImageInfo->LSC.CenterYOffset = sLSCSetting.uiCenterYOffset; // 0~100, default 50

    return 0;
}

static INT32 param_antifog_get(INT32 argc, char* argv[])
{
    UINT8 ipl_id;
    ISPAntiFogSetting sAntiFogSetting={0};

    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }

    if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM)
    {
        DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n",ipl_id, IPL_ID_MAX_NUM);
        return -1;
    }

    if (IPL_AlgGetUIInfo(ipl_id, IPL_SEL_IPC_MODE) != SEL_IPC_MODE_ON)
        IPL_AlgSetUIInfo(ipl_id, IPL_SEL_IPC_MODE, SEL_IPC_MODE_ON);

    switch (IPL_AlgGetUIInfo(ipl_id, IPL_SEL_ANTI_FOG))
    {
        case SEL_ANTI_FOG_AUTO:
            sAntiFogSetting.eAntiFogMode = ISP_CTRL_MODE_AUTO;
            sAntiFogSetting.bAntiFogEnable = NVT_TRUE;
            break;
        case SEL_ANTI_FOG_MANUAL:
            sAntiFogSetting.eAntiFogMode = ISP_CTRL_MODE_MANUAL;
            sAntiFogSetting.bAntiFogEnable = NVT_TRUE;
            break;
        case SEL_ANTI_FOG_OFF:
            sAntiFogSetting.eAntiFogMode = ISP_CTRL_MODE_AUTO;
            sAntiFogSetting.bAntiFogEnable = NVT_FALSE;
            break;
    }

    sAntiFogSetting.uiAntiFogStrength = IPL_AlgGetUIInfo(ipl_id, IPL_SEL_ANTI_FOG_LEVEL);

    NvtUctrl_SetConfigData((void *)&sAntiFogSetting, sizeof(ISPAntiFogSetting));

    return 0;
}

static INT32 param_antifog_set(INT32 argc, char* argv[])
{
    UINT8 ipl_id;
    ISPAntiFogSetting sAntiFogSetting={0};

    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }

    if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM)
    {
        DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n",ipl_id, IPL_ID_MAX_NUM);
        return -1;
    }

    if (IPL_AlgGetUIInfo(ipl_id, IPL_SEL_IPC_MODE) != SEL_IPC_MODE_ON)
        IPL_AlgSetUIInfo(ipl_id, IPL_SEL_IPC_MODE, SEL_IPC_MODE_ON);

    NvtUctrl_GetConfigData((void *)&sAntiFogSetting, sizeof(ISPAntiFogSetting));

    pImageInfo->ipl_id.value = ipl_id;
    pImageInfo->ipl_id.isSet = 1;

    pImageInfo->antifog.isSet = 1;
    if (sAntiFogSetting.bAntiFogEnable == NVT_TRUE)
    {
        if (sAntiFogSetting.eAntiFogMode == ISP_CTRL_MODE_AUTO)
            pImageInfo->antifog.mode = 1;
        else
            pImageInfo->antifog.mode = 2;
    }
    else
        pImageInfo->antifog.mode = 0;

    pImageInfo->antifog.lv = sAntiFogSetting.uiAntiFogStrength; // 0~10, default 0

    if (pImageInfo->antifog.lv > 10)
        pImageInfo->antifog.lv = 10;

    return 0;
}

static INT32 param_gamma_get(INT32 argc, char* argv[])
{
    UINT8 ipl_id;
    ISPGammaSetting sGammaSetting={0};

    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }

    if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM)
    {
        DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n",ipl_id, IPL_ID_MAX_NUM);
        return -1;
    }

    if (IPL_AlgGetUIInfo(ipl_id, IPL_SEL_IPC_MODE) != SEL_IPC_MODE_ON)
        IPL_AlgSetUIInfo(ipl_id, IPL_SEL_IPC_MODE, SEL_IPC_MODE_ON);

    switch (IPL_AlgGetUIInfo(ipl_id, IPL_SEL_USER_GAMMA_MODE))
    {
        case SEL_USER_GAMMA_LINEAR:
            sGammaSetting.eGammaCurveMode = ISP_GAMMA_CURVE_MODE_LINEAR;
            break;
        case SEL_USER_GAMMA_CURVE_MODE_16:
            sGammaSetting.eGammaCurveMode = ISP_GAMMA_CURVE_MODE_16;
            break;
        case SEL_USER_GAMMA_CURVE_MODE_18:
            sGammaSetting.eGammaCurveMode = ISP_GAMMA_CURVE_MODE_18;
            break;
        case SEL_USER_GAMMA_CURVE_MODE_20:
            sGammaSetting.eGammaCurveMode = ISP_GAMMA_CURVE_MODE_20;
            break;
        case SEL_USER_GAMMA_CURVE_MODE_22:
            sGammaSetting.eGammaCurveMode = ISP_GAMMA_CURVE_MODE_22;
            break;
        case SEL_USER_GAMMA_CURVE_MODE_24:
            sGammaSetting.eGammaCurveMode = ISP_GAMMA_CURVE_MODE_24;
            break;
        case SEL_USER_GAMMA_CURVE_MODE_USER:
            sGammaSetting.eGammaCurveMode = ISP_GAMMA_CURVE_MODE_USER;
            break;
        case SEL_USER_GAMMA_CURVE_DEFAULT:
        default:
            sGammaSetting.eGammaCurveMode = ISP_GAMMA_CURVE_MODE_DEFAULT;
            break;
    }
#if ((_SENSORLIB_ == _SENSORLIB_CMOS_IMX291M_) || \
     (_SENSORLIB_ == _SENSORLIB_CMOS_IMX123LQT_) || \
     (_SENSORLIB_ == _SENSORLIB_CMOS_AR0230CS_) || \
     (_SENSORLIB_ == _SENSORLIB_CMOS_MN34110_))
    IQS_getUserDefineGamma(sGammaSetting.uiUserGammaTable);
#endif
    NvtUctrl_SetConfigData((void *)&sGammaSetting, sizeof(ISPGammaSetting));

    return 0;
}

static INT32 param_gamma_set(INT32 argc, char* argv[])
{
    UINT8 ipl_id;
    ISPGammaSetting sGammaSetting={0};

    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }

    if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM)
    {
        DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n",ipl_id, IPL_ID_MAX_NUM);
        return -1;
    }

    NvtUctrl_GetConfigData((void *)&sGammaSetting, sizeof(ISPGammaSetting));

    if (IPL_AlgGetUIInfo(ipl_id, IPL_SEL_IPC_MODE) != SEL_IPC_MODE_ON)
        IPL_AlgSetUIInfo(ipl_id, IPL_SEL_IPC_MODE, SEL_IPC_MODE_ON);

#if ((_SENSORLIB_ == _SENSORLIB_CMOS_IMX291M_) || \
     (_SENSORLIB_ == _SENSORLIB_CMOS_IMX123LQT_) || \
     (_SENSORLIB_ == _SENSORLIB_CMOS_AR0230CS_) || \
     (_SENSORLIB_ == _SENSORLIB_CMOS_MN34110_))
    IQS_setUserDefineGamma(sGammaSetting.uiUserGammaTable);
#endif
    switch (sGammaSetting.eGammaCurveMode)
    {
        case ISP_GAMMA_CURVE_MODE_LINEAR:
            IPL_AlgSetUIInfo(ipl_id, IPL_SEL_USER_GAMMA_MODE, SEL_USER_GAMMA_LINEAR);
            break;
        case ISP_GAMMA_CURVE_MODE_16:
            IPL_AlgSetUIInfo(ipl_id, IPL_SEL_USER_GAMMA_MODE, SEL_USER_GAMMA_CURVE_MODE_16);
            break;
        case ISP_GAMMA_CURVE_MODE_18:
            IPL_AlgSetUIInfo(ipl_id, IPL_SEL_USER_GAMMA_MODE, SEL_USER_GAMMA_CURVE_MODE_18);
            break;
        case ISP_GAMMA_CURVE_MODE_20:
            IPL_AlgSetUIInfo(ipl_id, IPL_SEL_USER_GAMMA_MODE, SEL_USER_GAMMA_CURVE_MODE_20);
            break;
        case ISP_GAMMA_CURVE_MODE_22:
            IPL_AlgSetUIInfo(ipl_id, IPL_SEL_USER_GAMMA_MODE, SEL_USER_GAMMA_CURVE_MODE_22);
            break;
        case ISP_GAMMA_CURVE_MODE_24:
            IPL_AlgSetUIInfo(ipl_id, IPL_SEL_USER_GAMMA_MODE, SEL_USER_GAMMA_CURVE_MODE_24);
            break;
        case ISP_GAMMA_CURVE_MODE_USER:
            IPL_AlgSetUIInfo(ipl_id, IPL_SEL_USER_GAMMA_MODE, SEL_USER_GAMMA_CURVE_MODE_USER);
            break;
        case ISP_GAMMA_CURVE_MODE_DEFAULT:
        default:
            IPL_AlgSetUIInfo(ipl_id, IPL_SEL_USER_GAMMA_MODE, SEL_USER_GAMMA_CURVE_DEFAULT);
            break;
    }

    return 0;
}

static INT32 param_ccm_get(INT32 argc, char* argv[])
{
    UINT8 ipl_id;
    ISPCCMSetting sCCMSetting={0};

    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }

    if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM)
    {
        DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n",ipl_id, IPL_ID_MAX_NUM);
        return -1;
    }

    if (IPL_AlgGetUIInfo(ipl_id, IPL_SEL_IPC_MODE) != SEL_IPC_MODE_ON)
        IPL_AlgSetUIInfo(ipl_id, IPL_SEL_IPC_MODE, SEL_IPC_MODE_ON);

    switch (IPL_AlgGetUIInfo(0,IPL_SEL_USER_CC))
    {
        case SEL_USER_CC_MANUAL:
            sCCMSetting.eCCMMode = ISP_CTRL_MODE_MANUAL;
            break;
        case SEL_USER_CC_AUTO:
        default:
            sCCMSetting.eCCMMode = ISP_CTRL_MODE_AUTO;
            break;
    }
#if ((_SENSORLIB_ == _SENSORLIB_CMOS_IMX291M_) || \
     (_SENSORLIB_ == _SENSORLIB_CMOS_IMX123LQT_) || \
     (_SENSORLIB_ == _SENSORLIB_CMOS_AR0230CS_) || \
     (_SENSORLIB_ == _SENSORLIB_CMOS_MN34110_))
    IQS_getUserDefineCC(0, sCCMSetting.uiLowCCM);
    IQS_getUserDefineCC(1, sCCMSetting.uiMiddleCCM);
    IQS_getUserDefineCC(2, sCCMSetting.uiHighCCM);
#endif
    NvtUctrl_SetConfigData((void *)&sCCMSetting, sizeof(ISPCCMSetting));

    return 0;
}

static INT32 param_ccm_set(INT32 argc, char* argv[])
{
    UINT8 ipl_id;
    ISPCCMSetting sCCMSetting={0};

    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }

    if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM)
    {
        DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n",ipl_id, IPL_ID_MAX_NUM);
        return -1;
    }

    NvtUctrl_GetConfigData((void *)&sCCMSetting, sizeof(ISPCCMSetting));

    if (IPL_AlgGetUIInfo(ipl_id, IPL_SEL_IPC_MODE) != SEL_IPC_MODE_ON)
        IPL_AlgSetUIInfo(ipl_id, IPL_SEL_IPC_MODE, SEL_IPC_MODE_ON);

    switch (sCCMSetting.eCCMMode)
    {
        case ISP_CTRL_MODE_MANUAL:
            IPL_AlgSetUIInfo(ipl_id, IPL_SEL_USER_CC, SEL_USER_CC_MANUAL);
            break;
        case ISP_CTRL_MODE_AUTO:
        default:
            IPL_AlgSetUIInfo(ipl_id, IPL_SEL_USER_CC, SEL_USER_CC_AUTO);
            break;
    }
#if ((_SENSORLIB_ == _SENSORLIB_CMOS_IMX291M_) || \
     (_SENSORLIB_ == _SENSORLIB_CMOS_IMX123LQT_) || \
     (_SENSORLIB_ == _SENSORLIB_CMOS_AR0230CS_) || \
     (_SENSORLIB_ == _SENSORLIB_CMOS_MN34110_))
    IQS_setUserDefineCC(0, sCCMSetting.uiLowCCM);
    IQS_setUserDefineCC(1, sCCMSetting.uiMiddleCCM);
    IQS_setUserDefineCC(2, sCCMSetting.uiHighCCM);
#endif

    return 0;
}


static INT32 param_iplmap_get(INT32 argc, char* argv[])
{
#if(IPCAM_FUNC == ENABLE)

    IPCAM_IPL *pIPL_map=NULL;
    char   retStr[32];
    pIPL_map = ImageApp_IPCam_Get_IPL_Mapping();
	DBG_DUMP("ipl path map: %d %d %d\r\n",pIPL_map[0],pIPL_map[1],pIPL_map[2]);
    snprintf(retStr,sizeof(retStr), "%d %d %d", pIPL_map[0],pIPL_map[1],pIPL_map[2]);
    NvtUctrl_SetRetString(retStr);
	return 0;
#else
    DBG_ERR("not support\r\n");
    return -1;
#endif

}

static INT32 param_iplmap_set(INT32 argc, char* argv[])
{
#if(IPCAM_FUNC == ENABLE)

    if(argc!=3)
    {
        DBG_ERR("argc!=3 (%d)\r\n",argc);
        return -1;
    }

    ImageApp_IPCam_Set_IPL_Mapping( atoi(argv[0]), atoi(argv[1]), atoi(argv[2]));

    ////change mode for restart ImageApp_IPCam
	int iCurrMode = System_GetState(SYS_STATE_CURRMODE);
    int iNextMode = PRIMARY_MODE_MOVIE;
    UI_SetData(FL_PreMode,iCurrMode);
    UI_SetData(FL_NextMode,iNextMode);
    Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, iNextMode);
	return 0;
#else
    DBG_ERR("not support\r\n");
    return -1;
#endif

}

static INT32 param_pip_set(INT32 argc, char* argv[])
{
	ISPPIPSetting sPIPSetting={0};
	NvtUctrl_GetConfigData((void *)&sPIPSetting, sizeof(ISPPIPSetting));
	DBG_DUMP("PIP set:%d\r\n",sPIPSetting.bEnable);
	MovieStamp_SetConfig(MOVIESTAMP_PIP_ENABLE,sPIPSetting.bEnable);
    return 0;
}

static INT32 param_pip_get(INT32 argc, char* argv[])
{

	ISPPIPSetting sPIPSetting;

	sPIPSetting.bEnable = MovieStamp_GetConfig(MOVIESTAMP_PIP_ENABLE);
	DBG_DUMP("PIP: %d\r\n",sPIPSetting.bEnable);
	NvtUctrl_SetConfigData((void *)&sPIPSetting, sizeof(ISPPIPSetting));
    return 0;
}


static INT32 param_sbs_set(INT32 argc, char* argv[])
{
	#if(IPCAM_FUNC == ENABLE)
	if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }
	ISPSBSSetting sSBSSetting;
	NvtUctrl_GetConfigData((void *)&sSBSSetting, sizeof(ISPPIPSetting));
	DBG_DUMP("SBS set: %d\r\n",sSBSSetting.eSBSMode);
	if(sSBSSetting.eSBSMode >= SEL_SBS_MAX_CNT){
		DBG_ERR("SBS mode = %d error\r\n",sSBSSetting.eSBSMode);
		return -1;
	}
    IPL_AlgSetUIInfo(0, IPL_SEL_SBS, sSBSSetting.eSBSMode);//sensor 0
	IPL_AlgSetUIInfo(1, IPL_SEL_SBS, sSBSSetting.eSBSMode);//sensor 1

	ImageApp_IPCam_Config(IPCAM_CFG_SBS_FLAG, sSBSSetting.eSBSMode);
    Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIE_SBS_MODE, sSBSSetting.eSBSMode);

	////change mode for restart ImageApp_IPCam
	int iCurrMode = System_GetState(SYS_STATE_CURRMODE);
    int iNextMode = PRIMARY_MODE_MOVIE;
    UI_SetData(FL_PreMode,iCurrMode);
    UI_SetData(FL_NextMode,iNextMode);
    Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, iNextMode);
	return 0;
	#else
    DBG_ERR("not support\r\n");
    return -1;
	#endif

}

static INT32 param_sbs_get(INT32 argc, char* argv[])
{
	#if(IPCAM_FUNC == ENABLE)
	if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }
	ISPSBSSetting sSBSSetting;

	sSBSSetting.eSBSMode = IPL_AlgGetUIInfo(atoi(argv[0]), IPL_SEL_SBS);
	DBG_DUMP("SBS: %d\r\n",sSBSSetting.eSBSMode);
	NvtUctrl_SetConfigData((void *)&sSBSSetting, sizeof(ISPSBSSetting));
	return 0;
	#else
    DBG_ERR("not support\r\n");
    return -1;
	#endif

}

static INT32 param_dewarp_set(INT32 argc, char* argv[])
{
#if(IPCAM_FUNC == ENABLE)
	#if( FISHEYE_TYPE == ENABLE)
    	if(argc!=1)
    	{
        	DBG_ERR("argc!=1 (%d)\r\n",argc);
        	return -1;
    	}

    	ImageApp_IPCam_Config(IPCAM_CFG_DEWARP_FLAG,atoi(argv[0]));

    	////change mode for restart ImageApp_IPCam
		int iCurrMode = System_GetState(SYS_STATE_CURRMODE);
    	int iNextMode = PRIMARY_MODE_MOVIE;
    	UI_SetData(FL_PreMode,iCurrMode);
    	UI_SetData(FL_NextMode,iNextMode);
    	Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, iNextMode);
	#endif
#endif
    return 0;
}

#define RAW_SIE_PAUSE 	0
#define RAW_DATA_GET  	1
#define RAW_SIE_RESUMNE 2

static INT32 param_raw_get(INT32 argc, char* argv[])
{
	if(argc!=2){
        DBG_ERR("argc!=2 (%d)\r\n",argc);
        return -1;
	}


    UINT32 ipl_id = atoi(argv[0]);
	IPL_TUNING_RAWINFO raw_info = {0};
	//static int iCurrMode = PRIMARY_MODE_MAIN;
	//int iNextMode = PRIMARY_MODE_MAIN;
	//UINT32 i=0;
    switch(atoi(argv[1])){
		case RAW_SIE_PAUSE:
			DBG_DUMP("pause SIE\r\n");
			ipl_tuning_setsiemode(ipl_id, 1);
			/*
        	iCurrMode = System_GetState(SYS_STATE_CURRMODE);
        	iNextMode = PRIMARY_MODE_MAIN;
        	UI_SetData(FL_PreMode,iCurrMode);
        	UI_SetData(FL_NextMode,iNextMode);
        	UctrlMain_SendEvent(NVTEVT_SYSTEM_MODE, 1, iNextMode);*/


			break;
		case RAW_DATA_GET:
			ipl_tuning_getrawinfo(ipl_id,(UINT32)&raw_info,sizeof(raw_info));
			DBG_DUMP("raw info===========\r\n");
			DBG_DUMP("width:\t\t%d\r\n",raw_info.width);
			DBG_DUMP("height:\t\t%d\r\n",raw_info.height);
			DBG_DUMP("bits:\t\t%d\r\n",raw_info.bits);
			DBG_DUMP("lineoffset[0]:\t%d\r\n",raw_info.lineoffset[0]);
			DBG_DUMP("lineoffset[1]:\t%d\r\n",raw_info.lineoffset[1]);
			DBG_DUMP("overlap:\t\t%d\r\n",raw_info.overlap);
			DBG_DUMP("rawsize:\t\t%d\r\n",raw_info.rawsize);
			DBG_DUMP("addr :\t\t%08x\r\n",raw_info.addr);

			raw_info.addr = NvtMem_GetPhyAddr(raw_info.addr);
			DBG_DUMP("addr transfer:\t\t%08x\r\n",raw_info.addr);
			NvtUctrl_SetConfigData((void *)&raw_info, sizeof(IPL_TUNING_RAWINFO));
			break;
		case RAW_SIE_RESUMNE:
			DBG_DUMP("resume SIE\r\n");
			ipl_tuning_setsiemode(ipl_id, 0);

			/*
			iNextMode = iCurrMode;
        	int iCurrMode = System_GetState(SYS_STATE_CURRMODE);
        	UI_SetData(FL_PreMode,iCurrMode);
        	UI_SetData(FL_NextMode,iNextMode);
        	UctrlMain_SendEvent(NVTEVT_SYSTEM_MODE, 1, iNextMode);
			*/
			break;
		default:
			DBG_ERR("parameter(%d) error \r\n",atoi(argv[1]));
			break;

    }
	return 0;

}

static INT32 param_begin(char* str, INT32 len)
{
    DBG_IND("get: %s\r\n",str);
    memset(pImageInfo, 0 , sizeof(UCtrlImageInfoType));
    return 0;
}

static INT32 param_end(UTOKEN_END_INFO* p_info)
{
    char retStr[64];
    AFDRV_ONVIFPARM afsetting= {0};
    AE_EXT_SET_INFO AeSetInfo = {0};
//#NT#2016/10/18#Jeah Yen -begin
//#NT#fix for support sensor map
    IPL_PROC_ID ipl_id=0;
//#NT#2016/10/18#Jeah Yen -begin
    IQS_EXT_SET_IQ_INFO IQSExtSetIQInfo = {0};

    if (p_info->err_code < 0)
    {
        snprintf(retStr, sizeof(retStr),"ERR: parm %s", p_info->failed_name);
        NvtUctrl_SetRetString(retStr);
        DBG_ERR("%s\r\n",retStr);
        return p_info->err_code;
    }

    // set channel ID
    if (pImageInfo->ipl_id.isSet)
    {
        ipl_id = pImageInfo->ipl_id.value;
    }
    DBG_IND("ipl_id=%d\r\n",ipl_id);
    if (pImageInfo->backlight.isSet)
    {
        DBG_IND("backlight %d %d\r\n", pImageInfo->backlight.mode, pImageInfo->backlight.lv);
        IPL_AlgSetUIInfo(ipl_id, IPL_SEL_BLC, pImageInfo->backlight.mode);
        IPL_AlgSetUIInfo(ipl_id, IPL_SEL_BLC_LEVEL, pImageInfo->backlight.lv);
    }

    if (pImageInfo->brightness.isSet)
    {
        DBG_IND("brightness %d\r\n", pImageInfo->brightness.value);
        IQSExtSetIQInfo.brightness.isSet = 1;
        IQSExtSetIQInfo.brightness.ratio = pImageInfo->brightness.value;
        IQS_SetIQInfo(ipl_id, IQSExtSetIQInfo);
    }

    if (pImageInfo->saturation.isSet)
    {
        DBG_IND("saturation %d\r\n", pImageInfo->saturation.value);
        IQSExtSetIQInfo.saturation.isSet = 1;
        IQSExtSetIQInfo.saturation.ratio= pImageInfo->saturation.value;
        IQS_SetIQInfo(ipl_id, IQSExtSetIQInfo);
    }

    if (pImageInfo->contrast.isSet)
    {
        DBG_IND("contrast %d\r\n", pImageInfo->contrast.value);
        IQSExtSetIQInfo.contrast.isSet = 1;
        IQSExtSetIQInfo.contrast.ratio= pImageInfo->contrast.value;
        IQS_SetIQInfo(ipl_id, IQSExtSetIQInfo);
    }

    if (pImageInfo->sharpness.isSet)
    {
        DBG_IND("sharpness %d\r\n", pImageInfo->sharpness.value);
        IQSExtSetIQInfo.sharpness.isSet = 1;
        IQSExtSetIQInfo.sharpness.ratio= pImageInfo->sharpness.value;
        IQS_SetIQInfo(ipl_id, IQSExtSetIQInfo);
    }

    if (pImageInfo->hue.isSet)
    {
        DBG_IND("hue %d\r\n", pImageInfo->hue.value);
        IQSExtSetIQInfo.hue.isSet = 1;
        IQSExtSetIQInfo.hue.ratio= pImageInfo->hue.value;
        IQS_SetIQInfo(ipl_id, IQSExtSetIQInfo);
    }

    if (pImageInfo->nr2D.isSet)
    {
        DBG_IND("nr2D %d\r\n", pImageInfo->nr2D.value);
        IQSExtSetIQInfo.nr2D.isSet = 1;
        IQSExtSetIQInfo.nr2D.ratio= pImageInfo->nr2D.value;
        IQS_SetIQInfo(ipl_id, IQSExtSetIQInfo);
    }

    if (pImageInfo->nr3D.isSet)
    {
        DBG_IND("nr3D %d %d\r\n", pImageInfo->nr3D.mode, pImageInfo->nr3D.ratio);
        IQSExtSetIQInfo.nr3D.isSet = 1;
        IQSExtSetIQInfo.nr3D.mode= pImageInfo->nr3D.mode;
        IQSExtSetIQInfo.nr3D.ratio= pImageInfo->nr3D.ratio;
        IQS_SetIQInfo(ipl_id, IQSExtSetIQInfo);
    }

    if (pImageInfo->LDC.isSet)
    {
        DBG_IND("LDC %d %d\r\n", pImageInfo->LDC.isEnable, pImageInfo->LDC.ratio);
        IQSExtSetIQInfo.GDC.isSet = 1;
        IQSExtSetIQInfo.GDC.isEnable = pImageInfo->LDC.isEnable;
        IQSExtSetIQInfo.GDC.ratio = pImageInfo->LDC.ratio;
        IQS_SetIQInfo(ipl_id, IQSExtSetIQInfo);
    }

    if (pImageInfo->LSC.isSet)
    {
        DBG_IND("LSC %d %d %d %d\r\n", pImageInfo->LSC.isEnable, pImageInfo->LSC.ratio, pImageInfo->LSC.CenterXOffset, pImageInfo->LSC.CenterYOffset);
        IQSExtSetIQInfo.LSC.isSet = 1;
        IQSExtSetIQInfo.LSC.isEnable = pImageInfo->LSC.isEnable;
        IQSExtSetIQInfo.LSC.ratio = pImageInfo->LSC.ratio;                  // 0~100; default:0
        IQSExtSetIQInfo.LSC.CenterXOffset = pImageInfo->LSC.CenterXOffset;  // 0~100; default:50
        IQSExtSetIQInfo.LSC.CenterYOffset = pImageInfo->LSC.CenterYOffset;  // 0~100; default:50
        IQS_SetIQInfo(ipl_id, IQSExtSetIQInfo);
    }

    if (pImageInfo->ae.isSet)
    {
        DBG_IND("ae %d %d (%d %d) (%d %d) (%d %d) %d %d %d\r\n",  pImageInfo->ae.mode,    pImageInfo->ae.priority,
                                                        pImageInfo->ae.maxexp,  pImageInfo->ae.minexp,
                                                        pImageInfo->ae.maxgain, pImageInfo->ae.mingain,
                                                        pImageInfo->ae.maxiris, pImageInfo->ae.miniris,
                                                        pImageInfo->ae.exp, pImageInfo->ae.gain, pImageInfo->ae.iris);
        if (pImageInfo->ae.mode == 0)
        {
            AeSetInfo.PLineMode = AE_PLINE_AUTOGEN;
            AeSetInfo.ExpTBound.h = pImageInfo->ae.maxexp;
            AeSetInfo.ExpTBound.l = pImageInfo->ae.minexp;
            AeSetInfo.IsoBound.h = pImageInfo->ae.maxgain;
            AeSetInfo.IsoBound.l = pImageInfo->ae.mingain;
        }
        else
        {
            AeSetInfo.PLineMode = AE_PLINE_MANUAL;
            AeSetInfo.ExpT = pImageInfo->ae.exp;
            AeSetInfo.ISO = pImageInfo->ae.gain;
            AeSetInfo.IrisSetting.PIrisManualSize = pImageInfo->ae.iris;
        }
        AE_SetPLINEInfo(ipl_id, AeSetInfo);
    }

    if (pImageInfo->aewin.isSet)
    {
        DBG_IND("aewin %d %d %d %d\r\n", pImageInfo->aewin.left, pImageInfo->aewin.right, pImageInfo->aewin.top, pImageInfo->aewin.bottom);
    }

    if (pImageInfo->focus.isSet)
    {
        afsetting.parmtype.mode = pImageInfo->focus.mode;
        afsetting.parmtype.speed = pImageInfo->focus.speed;
        afsetting.parmtype.near_limit = pImageInfo->focus.near;
        afsetting.parmtype.far_limit = pImageInfo->focus.far;
        AF_OnvifSet(ipl_id,PARMTYPE,&afsetting);
        DBG_IND("focus %d %d %d %d\r\n", pImageInfo->focus.mode, pImageInfo->focus.speed, pImageInfo->focus.near, pImageInfo->focus.far);
    }

    if (pImageInfo->ircut.isSet)
    {
        DBG_IND("ircut %d\r\n", pImageInfo->ircut.value);
        UctrlMain_SendEvent(NVTEVT_EXE_MOVIE_IR_CUT, 1,  pImageInfo->ircut.value);
    }

    if (pImageInfo->wdr.isSet)
    {
        DBG_IND("wdr %d %d\r\n", pImageInfo->wdr.mode, pImageInfo->wdr.lv);
        if (pImageInfo->wdr.mode == SEL_WDR_AUTO)
        {
            if(IPL_AlgGetUIInfo(ipl_id, IPL_SEL_WDR)==SEL_WDR_OFF)
                UctrlMain_SendEvent(NVTEVT_EXE_MOVIE_WDR, 1, WDR_ON);

            IPL_AlgSetUIInfo(ipl_id, IPL_SEL_WDR, SEL_WDR_AUTO);
        }
        else if (pImageInfo->wdr.mode == SEL_WDR_ON)
        {
            if(IPL_AlgGetUIInfo(ipl_id, IPL_SEL_WDR)==SEL_WDR_OFF)
                UctrlMain_SendEvent(NVTEVT_EXE_MOVIE_WDR, 1, WDR_ON);

            IPL_AlgSetUIInfo(ipl_id, IPL_SEL_WDR, SEL_WDR_ON);
            IPL_AlgSetUIInfo(ipl_id, IPL_SEL_WDR_LEVEL, pImageInfo->wdr.lv);
        }
        else
        {
            UctrlMain_SendEvent(NVTEVT_EXE_MOVIE_WDR, 1, WDR_OFF);
            IPL_AlgSetUIInfo(ipl_id, IPL_SEL_WDR_LEVEL, pImageInfo->wdr.lv);
        }
    }

    if (pImageInfo->awb.isSet)
    {
        DBG_IND("awb %d %d %d\r\n", pImageInfo->awb.mode, pImageInfo->awb.rgain, pImageInfo->awb.bgain);
        AwbSetParam awbsetting;
        if ( pImageInfo->awb.mode == 0 )
        {
            UctrlMain_SendEvent(NVTEVT_EXE_WB, 1, WB_AUTO);
        }
        else if ( pImageInfo->awb.mode == 1 )
        {
            awbsetting.uiManualRGain = pImageInfo->awb.rgain;
            awbsetting.uiManualGGain = 256;
            awbsetting.uiManualBGain = pImageInfo->awb.bgain;
            UctrlMain_SendEvent(NVTEVT_EXE_WB, 1, WB_MANUAL);
            AWB_SetInfo(ipl_id, AWB_SET_GAIN, &awbsetting);
        }
    }

    if (pImageInfo->dis.isSet)
    {
        DBG_IND("dis %d %d\r\n", pImageInfo->dis.mode, pImageInfo->dis.lv);
        if (pImageInfo->dis.mode  == 1)
            UctrlMain_SendEvent(NVTEVT_EXE_MOVIEDIS, 1,  MOVIE_DIS_ON);
        else
            UctrlMain_SendEvent(NVTEVT_EXE_MOVIEDIS, 1,  MOVIE_DIS_OFF);
    }

    if (pImageInfo->antifog.isSet)
    {
        DBG_IND("antifog %d %d\r\n", pImageInfo->antifog.mode, pImageInfo->antifog.lv);
        switch (pImageInfo->antifog.mode)
        {
            case 0:
                IPL_AlgSetUIInfo(ipl_id, IPL_SEL_ANTI_FOG, SEL_ANTI_FOG_OFF);
                break;
            case 1:
                IPL_AlgSetUIInfo(ipl_id, IPL_SEL_ANTI_FOG, SEL_ANTI_FOG_AUTO);
                break;
            case 2:
                IPL_AlgSetUIInfo(ipl_id, IPL_SEL_ANTI_FOG, SEL_ANTI_FOG_MANUAL);
                break;
            default:
                break;
        }

        IPL_AlgSetUIInfo(ipl_id, IPL_SEL_ANTI_FOG_LEVEL, pImageInfo->antifog.lv);
    }

    if (pImageInfo->focusmove_absolute.isSet)
    {
        afsetting.absoluteparm.position = pImageInfo->focusmove_absolute.position;
        afsetting.absoluteparm.speed = pImageInfo->focusmove_absolute.speed;
        AF_OnvifSet(ipl_id,ABSOLUTEPARM,&afsetting);
        DBG_IND("focusmove absolute %d %d\r\n", pImageInfo->focusmove_absolute.position, pImageInfo->focusmove_absolute.speed);
    }

    if (pImageInfo->focusmove_continuous.isSet)
    {
        afsetting.continuousparm.speed = pImageInfo->focusmove_continuous.speed;
        AF_OnvifSet(ipl_id,CONTINUOUSPARM,&afsetting);
        DBG_IND("focusmove continuous %d\r\n", pImageInfo->focusmove_continuous.speed);
    }

    if (pImageInfo->focusmove_relative.isSet)
    {
        afsetting.relativeparm.distance = pImageInfo->focusmove_relative.distance;
        afsetting.relativeparm.speed = pImageInfo->focusmove_relative.speed;
        AF_OnvifSet(ipl_id,RELATIVESPARM,&afsetting);
        DBG_IND("focusmove relative %d %d\r\n", pImageInfo->focusmove_relative.distance, pImageInfo->focusmove_relative.speed);
    }

    if (pImageInfo->focusmove_stop.isSet)
    {
        AF_OnvifSet(ipl_id,STOPPARM,&afsetting);
        DBG_IND("focusmove stop\r\n");
    }

    if (pImageInfo->focus_status.isSet)
    {
        AF_OnvifGet(ipl_id);
        snprintf(retStr, sizeof(retStr),"%d %d %d", pImageInfo->focus_status.status, pImageInfo->focus_status.position, pImageInfo->focus_status.errcode);
        NvtUctrl_SetRetString(retStr);
        DBG_ERR("%s\r\n",retStr);
        return 0;
    }

	if(pImageInfo->dewarp_enable.isSet){
		#if(IPCAM_FUNC == ENABLE)
		ImageApp_IPCam_Config(IPCAM_CFG_DEWARP_FLAG,pImageInfo->dewarp_enable.value);
		bMovieModeRestart = TRUE;
		#endif
	}
	#if(IPCAM_FUNC == ENABLE)
    if(bMovieModeRestart == TRUE){
		bMovieModeRestart = FALSE;
		UctrlMain_SendEvent(NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_MOVIE);
	}
	#endif
    return 0;
}

static INT32 param_unknown(char* name)
{
    DBG_ERR("unknown param: %s\r\n",name);
    return 0;
}

UTOKEN_PARAM tbl_image_param[] = {
    {"channelset"   , param_ipl_id_set},    // [IPL ID set]
    {"bkc"  ,param_backlight },
    {"bri"  ,param_brightness},
    {"sat"  ,param_saturation},
    {"con"  ,param_contrast  },
    {"ae"   ,param_ae        },
    {"aewin",param_aewin     },
    {"focus",param_focus     },
    {"ircut",param_ircut     },
    {"wdr"  ,param_wdr       },
    {"awb"  ,param_awb       },
    {"dis"  ,param_dis       },
    {"edge" ,param_sharpness },
    {"fma"  ,param_focusmove_absolute},
    {"fmc"  ,param_focusmove_continuous},
    {"fmr"  ,param_focusmove_relative},
    {"fms"  ,param_focusmove_stop},
    {"fsts" ,param_focus_status},
    {"ImgSGet" ,param_image_setting_get},
    {"ImgSSet" ,param_image_setting_set},
    {"AEGet" ,param_ae_get},
    {"AESet" ,param_ae_set},
    {"AEStaGet", param_ae_status_get},
    {"WBGet" ,param_wb_get},
    {"WBSet" ,param_wb_set},
    {"WBStaGet", param_wb_status_get},
    {"WDRGet" ,param_wdr_get},
    {"WDRSet" ,param_wdr_set},
    {"NRGet" ,param_nr_get},
    {"NRSet" ,param_nr_set},
    {"DISGet" ,param_dis_get},
    {"DISSet" ,param_dis_set},
    {"LDCGet" ,param_ldc_get},
    {"LDCSet" ,param_ldc_set},
    {"LSCGet" ,param_lsc_get},
    {"LSCSet" ,param_lsc_set},
    {"AntiFogGet" ,param_antifog_get},
    {"AntiFogSet" ,param_antifog_set},
    {"GammaGet" ,param_gamma_get},
    {"GammaSet" ,param_gamma_set},
    {"CCMGet" ,param_ccm_get},
    {"CCMSet" ,param_ccm_set},
    {"IPLMapSet",param_iplmap_set},
    {"IPLMapGet",param_iplmap_get},
    {"DewarpSet",param_dewarp_set},
	{"PIPSet",param_pip_set},
    {"PIPGet",param_pip_get},
	{"SBSSet",param_sbs_set},//side by side
    {"SBSGet",param_sbs_get},
    {"RAWGet",param_raw_get},
    {NULL,NULL}, //last tag, it must be
};

UTOKEN_MODULE module_image = {
    "uimage",
    tbl_image_param,
    param_begin,
    param_end,
    param_unknown,
    NULL
};


void UCtrlAppImage_Open(void)
{
//    clr_flg(UCTRL_IMAGE_FLG_ID, 0xFFFFFFFF);
}

void UCtrlAppImage_Close(void)
{
}






















#if SXCMD_TEST

static BOOL Cmd_uimage_set(CHAR* strCmd)
{
    uToken_Parse(strCmd,&module_image);
    return TRUE;
}

SXCMD_BEGIN(uimage, "uimage")
SXCMD_ITEM("set %", Cmd_uimage_set, "")
SXCMD_END()
#endif


