/*
    IPL flow Callback Function

    This file is the API of the flow callback function.

    @file       IPL_FlowCBSample.c
    @ingroup    mISYSAlg
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/
#include "IPL_MN34110_EVB_FF_Int.h"
#include "Delay.h"
#include "Lens.h"
#include "ae_alg.h"
#include "LensCtrlTsk.h"
#include "sie_lib.h"

#include "rsc_task.h"
#include "rsc_common.h"

void IPL_FCB_GetCapRawNormal(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo);
void IPL_FCB_GetCapRawSim(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo);
void IPL_FCB_GetCapRawInVid(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo);
void IPL_FCB_GetCapRawEBurst(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo);
void IPL_FCB_GetCapRawHDR(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo);
void IPL_FCB_GetCapRawMFHDR(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo);
//#NT#2016/03/30#Harry Tsai -begin
//#NT# Add side by side capture mode
void IPL_FCB_GetCapRawDual(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo);
//#NT#2016/03/30#Harry Tsai -end

typedef void (*CapFP)(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo);

//static FLASH_INFOR MainFlashInfo = {0};

CapFP CaptureFP[IPLCTRL_CAP_FLOW_MAX] =
{
    NULL,
    IPL_FCB_GetCapRawNormal,
    IPL_FCB_GetCapRawNormal,
    IPL_FCB_GetCapRawHDR,
    IPL_FCB_GetCapRawNormal,
    IPL_FCB_GetCapRawEBurst,
    IPL_FCB_GetCapRawInVid,
    IPL_FCB_GetCapRawNormal,
    IPL_FCB_GetCapRawNormal,
    IPL_FCB_GetCapRawSim,
    IPL_FCB_GetCapRawMFHDR,
    IPL_FCB_GetCapRawNormal,
//#NT#2016/03/30#Harry Tsai -begin
//#NT# Add side by side capture mode
    IPL_FCB_GetCapRawDual
//#NT#2016/03/30#Harry Tsai -end
};

void IPL_FCB_FlashTrig(FLASH_INFOR *Data)
{
}

void IPL_FCB_PreFlash(IPL_PROC_ID Id)
{
    //IPL_CBMsgProc(Id, IPL_CBMSG_PREFLASH, NULL);
    //MainFlashInfo = flash_PreFlashFlow(IPL_FCB_FlashTrig);
    //IPL_CBMsgProc(Id, IPL_CBMSG_PREFLASHEND, NULL);
}

void IPL_FCB_PrepareCapAE(IPL_PROC_ID Id)
{

}


void IPL_FCB_GetCapRawNormal(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo)
{
    EXPOSURE_SETTING ExpTSetting = {0};
    GAIN_SETTING Gain = {0};
    SENSOR_MODE Mode = 0;
    UINT32 Ratio[SEN_MFRAME_MAX_NUM] = {0};
    UINT32 capiso[SEN_MFRAME_MAX_NUM] = {0},capext[SEN_MFRAME_MAX_NUM] = {0},iris[SEN_MFRAME_MAX_NUM] = {0};
    IPC_CTRL_CMD ProcCmd[IPL_ID_MAX_NUM] = {IPC_IGNORE,IPC_IGNORE,IPC_IGNORE,IPC_IGNORE};
    IPL_SIE_CUR_FRAME_INFO Rsc_Info = {0};
    SENSOR_ID SenId;

    SenId = IPL_UTI_CONV2_SEN_ID(Id);

    //#NT#2016/10/12#Harry Tsai -begin
    //#NT# Modify CCIR flow
    //change sensor mode to capture
    Sensor_ChgMode(SenId, IPL_AlgGetUIInfo(Id, IPL_SEL_CAPSENMODE));
    //IPC_WaitVD(Id,TRUE);
    if(ExtInfo->Param[0] != 0x123)
    {
        AE_GetCapAEArg(Id, &capiso[0], &capext[0], &iris[0]);
        Sensor_GetCurMode(SenId, &Mode);
        Sensor_GetExpoSetting(SenId, &capext[0], Mode, &ExpTSetting, &Ratio[0]);
        capiso[0] = capiso[0] * Ratio[0] / 100;
        Sensor_GetGainSetting(SenId, &capiso[0], &Gain);
        Sensor_SetExpTime(SenId, &ExpTSetting);
        Sensor_SetGain(SenId, &Gain);
    }
    //#NT#2016/10/12#Harry Tsai -end
/***********************************RSC OPEN*************************************/
    if(RSC_OnOffInfor(Id)==SEL_RSC_ON)
    {
        if(RSC_GetStatus(Id) == SEL_RSC_OFF )
        {
            RSC_OpenTsk();
            RSC_SetStatus(Id, SEL_RSC_ON);
        }
    }
/***********************************RSC OPEN*************************************/

    IPC_WaitVD(Id,TRUE);
    IPL_CTRLRunTimeChg(Id, IPC_SIEx_, (void*)Info);

    ProcCmd[Id] = IPC_RESUME_SIE;
    IPL_CTRLMode(ProcCmd[IPL_ID_1],ProcCmd[IPL_ID_2],ProcCmd[IPL_ID_3],ProcCmd[IPL_ID_4]);
    IPC_WaitVD(Id,FALSE);

    ProcCmd[Id] = IPC_PAUSE_SIE;
    IPL_CTRLMode(ProcCmd[IPL_ID_1],ProcCmd[IPL_ID_2],ProcCmd[IPL_ID_3],ProcCmd[IPL_ID_4]);

    if(RSC_OnOffInfor(Id)==SEL_RSC_ON)
    {
        Rsc_Info.Id = Id;
        IPL_GetCmd(IPL_GET_SIE_CUR_FRAME_INFOR, &Rsc_Info);
        RSC_SetCapFrameCn(Id,Rsc_Info.FrameCnt);
    }

    IPC_WaitVD(Id,FALSE);
/***********************************RSC CLOSE*************************************/
    if(RSC_GetStatus(Id) == SEL_RSC_ON)
    {
        if(RSC_IsIdle()==TRUE)
        {
            RSCCTRL_SETFLAG(FALSE);

            while(RSCCTRL_GETFLAG(Id) == FALSE)
            {
                Delay_DelayMs(1);
            }
        }
        RSC_SetStatus(Id, SEL_RSC_OFF);
        RSC_CloseTsk();
    }
/***********************************RSC CLOSE*************************************/

}

void IPL_FCB_GetCapRawInVid(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo)
{
    IPL_HAL_GROUP Update;
    IPC_IPLInAddr rdeinfo;
    IPC_CTRL_CMD ProcCmd[IPL_ID_MAX_NUM] = {IPC_IGNORE,IPC_IGNORE,IPC_IGNORE,IPC_IGNORE};

    Update.sieUpdate = Info->sieUpdate;

    IPC_WaitVD(Id,TRUE);
    SIEx_ENG_ChgParam(Id, Info->Idx, Update, Info->psie);
    SIEx_ENG_Load(Id, Info->Idx, Update);

    IPC_WaitVD(Id,FALSE);

    ProcCmd[Id] = IPC_PAUSE_IPL;
    IPL_CTRLMode(ProcCmd[IPL_ID_1],ProcCmd[IPL_ID_2],ProcCmd[IPL_ID_3],ProcCmd[IPL_ID_4]);
    //check Ch0 need scale or not
    if ((Info->psie->DmaOutSizeH == Info->psie->IMG_Window.Crop.SizeH) &&
        (Info->psie->DmaOutSizeV == Info->psie->IMG_Window.Crop.SizeV))
    {
        Info->psie->CH0.Src = NonScaled_Img;
    }
    else
    {
        Info->psie->CH0.Src = CH0_Scaled_Img;
    }
    Info->psie->CH0.Out.PPB0_Addr = ExtInfo->Param[1];
    Info->psie->CH0.Out.PPB1_Addr = ExtInfo->Param[0];
    Info->psie->CH0.Out.LineOffset = ExtInfo->Param[2];
    Info->psie->IMG_Window.Crop.StartPix.x = ExtInfo->Param[4];
    Info->psie->IMG_Window.Crop.StartPix.y = ExtInfo->Param[5];
    Info->psie->IMG_Window.Crop.SizeH = ExtInfo->Param[6];
    Info->psie->IMG_Window.Crop.SizeV = ExtInfo->Param[7];
    SIEx_ENG_ChgParam(Id, Info->Idx, Update, Info->psie);
    SIEx_ENG_Load(Id, Info->Idx, Update);

    IPC_WaitVD(Id,FALSE);

    rdeinfo.Idx = Info->Idx;
    rdeinfo.PPB0Addr = ExtInfo->Param[9];
    rdeinfo.PPB1Addr = ExtInfo->Param[8];
    IPL_CTRLRunTimeChg(Id, IPC_Chg_IPL_INAddr,(void*)&rdeinfo);

    ProcCmd[Id] = IPC_RESUME_IPL;
    IPL_CTRLMode(ProcCmd[IPL_ID_1],ProcCmd[IPL_ID_2],ProcCmd[IPL_ID_3],ProcCmd[IPL_ID_4]);
}

void IPL_FCB_GetCapRawEBurst(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo)
{
    EXPOSURE_SETTING ExpTSetting;
    GAIN_SETTING Gain;
    SENSOR_MODE Mode;
    UINT32 capiso[SEN_MFRAME_MAX_NUM] = {0}, capext[SEN_MFRAME_MAX_NUM] = {0}, iris[SEN_MFRAME_MAX_NUM] = {0}, Ratio[SEN_MFRAME_MAX_NUM] = {0};
    SENSOR_ID SenId;
    IPC_CTRL_CMD ProcCmd[IPL_ID_MAX_NUM] = {IPC_IGNORE,IPC_IGNORE,IPC_IGNORE,IPC_IGNORE};

    if (ExtInfo->Param[0] == 0)
    {
        AE_GetCapAEArg(Id, &capiso[0], &capext[0], &iris[0]);

        //change sensor mode to capture
        IPC_WaitVD(Id,TRUE);
        SenId = IPL_UTI_CONV2_SEN_ID(Id);
        Sensor_ChgMode(SenId, IPL_AlgGetUIInfo(Id, IPL_SEL_CAPSENMODE));
        Sensor_GetCurMode(SenId, &Mode);
        Sensor_GetExpoSetting(SenId, &capext[0], Mode, &ExpTSetting, &Ratio[0]);
        capiso[0] = capiso[0]*Ratio[0]/100;
        Sensor_GetGainSetting(SenId, &capiso[0], &Gain);
        Sensor_SetExpTime(SenId, &ExpTSetting);
        Sensor_SetGain(SenId, &Gain);
        IPL_CTRLRunTimeChg(Id, IPC_SIEx_, (void*)Info);
        IPC_WaitVD(Id,FALSE);
        ProcCmd[Id] = IPC_RESUME_SIE;
        IPL_CTRLMode(ProcCmd[IPL_ID_1],ProcCmd[IPL_ID_2],ProcCmd[IPL_ID_3],ProcCmd[IPL_ID_4]);
        IPC_WaitVD(Id,FALSE);
    }
    else if (ExtInfo->Param[0] == 1)
    {
        ProcCmd[Id] = IPC_PAUSE_SIE;
        IPL_CTRLMode(ProcCmd[IPL_ID_1],ProcCmd[IPL_ID_2],ProcCmd[IPL_ID_3],ProcCmd[IPL_ID_4]);
        IPL_CTRLRunTimeChg(Id, IPC_SIEx_, (void*)Info);

        ProcCmd[Id] = IPC_RESUME_SIE;
        IPL_CTRLMode(ProcCmd[IPL_ID_1],ProcCmd[IPL_ID_2],ProcCmd[IPL_ID_3],ProcCmd[IPL_ID_4]);
        IPC_WaitVD(Id,FALSE);
    }
    else if (ExtInfo->Param[0] == 2)
    {
        ProcCmd[Id] = IPC_PAUSE_SIE;
        IPL_CTRLMode(ProcCmd[IPL_ID_1],ProcCmd[IPL_ID_2],ProcCmd[IPL_ID_3],ProcCmd[IPL_ID_4]);
        IPC_WaitVD(Id,FALSE);
    }
}


void IPL_FCB_GetCapRawHDR(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo)
{
    EXPOSURE_SETTING ExpTSetting;
    GAIN_SETTING Gain;
    SENSOR_MODE Mode;
    UINT32 Ratio[SEN_MFRAME_MAX_NUM] = {0};
    UINT32 capiso[SEN_MFRAME_MAX_NUM] = {0},capext[SEN_MFRAME_MAX_NUM] = {0},iris[SEN_MFRAME_MAX_NUM] = {0};
    IPC_CTRL_CMD ProcCmd[IPL_ID_MAX_NUM] = {IPC_IGNORE,IPC_IGNORE,IPC_IGNORE,IPC_IGNORE};

    SENSOR_ID SenId;
    SenId = IPL_UTI_CONV2_SEN_ID(Id);

    //change sensor mode to capture
    AE_GetCapAEArg(Id, &capiso[0], &capext[0], &iris[0]);

    Sensor_ChgMode(SenId, IPL_AlgGetUIInfo(Id, IPL_SEL_CAPSENMODE));
    Sensor_GetCurMode(SenId, &Mode);
    Sensor_GetExpoSetting(SenId, &capext[0], Mode, &ExpTSetting, &Ratio[0]);
    capiso[0] = capiso[0] * Ratio[0] / 100;
    Sensor_GetGainSetting(SenId, &capiso[0], &Gain);
    Sensor_SetExpTime(SenId, &ExpTSetting);
    Sensor_SetGain(SenId, &Gain);
    IPL_CTRLRunTimeChg(Id, IPC_SIEx_, (void*)Info);

    IPC_WaitVD(Id,FALSE);

    ProcCmd[Id] = IPC_RESUME_SIE;
    IPL_CTRLMode(ProcCmd[IPL_ID_1],ProcCmd[IPL_ID_2],ProcCmd[IPL_ID_3],ProcCmd[IPL_ID_4]);
    IPC_WaitVD(Id,FALSE);

    ProcCmd[Id] = IPC_PAUSE_SIE;
    IPL_CTRLMode(ProcCmd[IPL_ID_1],ProcCmd[IPL_ID_2],ProcCmd[IPL_ID_3],ProcCmd[IPL_ID_4]);
    IPC_WaitVD(Id,FALSE);
}


void IPL_FCB_GetCapRawMFHDR(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo)
{
    EXPOSURE_SETTING ExpTSetting;
    GAIN_SETTING Gain;
    SENSOR_MODE Mode;
    UINT32 Ratio[SEN_MFRAME_MAX_NUM] = {0};
    UINT32 capiso[SEN_MFRAME_MAX_NUM] = {0},capext[SEN_MFRAME_MAX_NUM] = {0},iris[SEN_MFRAME_MAX_NUM] = {0};
    IPC_CTRL_CMD ProcCmd[IPL_ID_MAX_NUM] = {IPC_IGNORE,IPC_IGNORE,IPC_IGNORE,IPC_IGNORE};

    SENSOR_ID SenId;
    SenId = IPL_UTI_CONV2_SEN_ID(Id);

    //change sensor mode to capture
    AE_GetCapAEArg(Id, &capiso[0], &capext[0], &iris[0]);

    Sensor_ChgMode(SenId, IPL_AlgGetUIInfo(Id, IPL_SEL_CAPSENMODE));
    Sensor_GetCurMode(SenId, &Mode);
    if(ExtInfo->Param[0] == 1)
    {
        capext[0] >>= 2;
    }
    Sensor_GetExpoSetting(SenId, &capext[0], Mode, &ExpTSetting, &Ratio[0]);
    capiso[0] = capiso[0] * Ratio[0] / 100;
    Sensor_GetGainSetting(SenId, &capiso[0], &Gain);
    Sensor_SetExpTime(SenId, &ExpTSetting);
    Sensor_SetGain(SenId, &Gain);
    IPL_CTRLRunTimeChg(Id, IPC_SIEx_, (void*)Info);

    IPC_WaitVD(Id,FALSE);

    ProcCmd[Id] = IPC_RESUME_SIE;
    IPL_CTRLMode(ProcCmd[IPL_ID_1],ProcCmd[IPL_ID_2],ProcCmd[IPL_ID_3],ProcCmd[IPL_ID_4]);
    IPC_WaitVD(Id,FALSE);

    ProcCmd[Id] = IPC_PAUSE_SIE;
    IPL_CTRLMode(ProcCmd[IPL_ID_1],ProcCmd[IPL_ID_2],ProcCmd[IPL_ID_3],ProcCmd[IPL_ID_4]);
    IPC_WaitVD(Id,FALSE);
}

//#NT#2016/09/08#Harry Tsai -begin
//#NT# Modify side by side capture flow
void IPL_FCB_GetCapRawDual(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo)
{
    static EXPOSURE_SETTING ExpTSetting[2] = {0};
    static GAIN_SETTING Gain[2] = {0};
    SENSOR_MODE Mode = 0;
    UINT32 Ratio[SEN_MFRAME_MAX_NUM] = {0};
    UINT32 capiso[SEN_MFRAME_MAX_NUM] = {0},capext[SEN_MFRAME_MAX_NUM] = {0},iris[SEN_MFRAME_MAX_NUM] = {0};
    IPC_CTRL_CMD ProcCmd[IPL_ID_MAX_NUM] = {IPC_IGNORE,IPC_IGNORE,IPC_IGNORE,IPC_IGNORE};
    SENSOR_ID SenId;
    SenId = IPL_UTI_CONV2_SEN_ID(Id);

    //change sensor mode to capture
    AE_GetCapAEArg(Id, &capiso[0], &capext[0], &iris[0]);

    Sensor_ChgMode(SenId, IPL_AlgGetUIInfo(Id, IPL_SEL_CAPSENMODE));
    IPC_WaitVD(Id,TRUE);
    Sensor_GetCurMode(SenId, &Mode);
    Sensor_GetExpoSetting(SenId, &capext[0], Mode, &ExpTSetting[Id], &Ratio[0]);
    capiso[0] = capiso[0] * Ratio[0] / 100;
    Sensor_GetGainSetting(SenId, &capiso[0], &Gain[Id]);
    //Sensor_SetExpTime(SenId, &ExpTSetting);
    //Sensor_SetGain(SenId, &Gain);
    IPL_CTRLRunTimeChg(Id, IPC_SIEx_, (void*)Info);

    IPC_WaitVD(Id,TRUE);

    if(ExtInfo->Param[6] == 0)
        return;

    Sensor_SetExpTime(IPL_ID_1, &ExpTSetting[IPL_ID_1]);
    Sensor_SetGain(IPL_ID_1, &Gain[IPL_ID_1]);
    Sensor_SetExpTime(IPL_ID_2, &ExpTSetting[IPL_ID_2]);
    Sensor_SetGain(IPL_ID_2, &Gain[IPL_ID_2]);

    //Hot code
    IPC_WaitVD(IPL_ID_2,TRUE);
    IPC_WaitVD(IPL_ID_1,TRUE);
    IPC_WaitVD(IPL_ID_2,TRUE);
    IPC_WaitVD(IPL_ID_1,TRUE);

#if 1
    ProcCmd[IPL_ID_1] = IPC_RESUME_SIE;
    ProcCmd[IPL_ID_2] = IPC_RESUME_SIE;
    IPL_CTRLMode(ProcCmd[IPL_ID_1],ProcCmd[IPL_ID_2],ProcCmd[IPL_ID_3],ProcCmd[IPL_ID_4]);

    IPC_WaitVD(IPL_ID_1,FALSE);

    ProcCmd[IPL_ID_1] = IPC_PAUSE_SIE;
    ProcCmd[IPL_ID_2] = IPC_PAUSE_SIE;
    IPL_CTRLMode(ProcCmd[IPL_ID_1],ProcCmd[IPL_ID_2],ProcCmd[IPL_ID_3],ProcCmd[IPL_ID_4]);

    IPC_WaitVD(IPL_ID_1,FALSE);
#else
    ProcCmd[Id] = IPC_RESUME_SIE;
    IPL_CTRLMode(ProcCmd[IPL_ID_1],ProcCmd[IPL_ID_2],ProcCmd[IPL_ID_3],ProcCmd[IPL_ID_4]);
    IPC_WaitVD(Id,FALSE);

    ProcCmd[Id] = IPC_PAUSE_SIE;
    IPL_CTRLMode(ProcCmd[IPL_ID_1],ProcCmd[IPL_ID_2],ProcCmd[IPL_ID_3],ProcCmd[IPL_ID_4]);

    IPC_WaitVD(Id,FALSE);
#endif
}
//#NT#2016/03/30#Harry Tsai -end

void IPL_FCB_GetCapRaw(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo)
{
    if (CaptureFP[IPL_CtrlGetInfor(Id, IPLCTRL_CAPFLOW)] != NULL)
    {
        if (IPLCTRL_CAP_FLOW_EBURST == IPL_CtrlGetInfor(Id, IPLCTRL_CAPFLOW))
        {
            if (ExtInfo->Param[0] != 2)
            {
                IPL_CBMsgProc(Id, IPL_CBMSG_SHUTTER_SOUND, NULL);
            }
        }
        else
        {
            IPL_CBMsgProc(Id, IPL_CBMSG_SHUTTER_SOUND, NULL);
        }
        CaptureFP[IPL_CtrlGetInfor(Id, IPLCTRL_CAPFLOW)](Id, Info, ExtInfo);
    }
    else
    {
        DBG_ERR("capture flow(%d) FP null\r\n", IPL_CtrlGetInfor(Id, IPLCTRL_CAPFLOW));
    }
}

void IPL_FCB_GetCapRawSim(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo)
{
}

void IPL_FCB_SetCapAWB(IPL_PROC_ID Id, IPC_SIE_Info *Info)
{
    /*DBG_ERR("Raw Size = %d %d %d\r\n", IPL_CtrlGetInfor((UINT32)Id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_H),
                            IPL_CtrlGetInfor((UINT32)Id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_V),
                            IPL_CtrlGetInfor((UINT32)Id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_HLOS));*/

    UINT32 pDebugInfo = 0;
    UINT32 size = AWB_SetEmbDebugInfo(Id, &pDebugInfo);

    if ( size != 0 )
        memcpy((void*)Info->psie->CH0.Out.PPB0_Addr, (void*)pDebugInfo, size);
}

void IPL_FCB_AlgGDC(IPL_PROC_ID Id, UINT32 SelIdx, UINT32 Value)
{
    IPL_MODE Mode;

    Mode = IPL_CtrlGetInfor(Id, IPLCTRL_CUR_MODE);

    if ((Mode != IPL_MODE_PREVIEW) &&
        (Mode != IPL_MODE_VIDEO) &&
        (Mode != IPL_MODE_VIDEOREC))
    {
        return;
    }

    IQS_SettingFlowFunc(Id, IQS_MANUAL_TRIG);
}

void IPL_FCB_AlgDIS(IPL_PROC_ID Id,UINT32 SelIdx, UINT32 Value)
{
    UINT32 Buf, Size;
    IPL_MODE Mode;
    IPL_DZOOM Info_DZ;

    Mode = IPL_CtrlGetInfor(Id, IPLCTRL_CUR_MODE);

    if ((Mode != IPL_MODE_PREVIEW) &&
        (Mode != IPL_MODE_VIDEO) &&
        (Mode != IPL_MODE_VIDEOREC))
    {
        return;
    }

    if (Value == SEL_DISCOMP_OFF)
    {
        //IQS_SettingFlowFunc(Id, IQS_DISCOMP_OFF);
        IPL_CtrlSetInfor(Id, IPLCTRL_DIS_EN, DISABLE);
    }
    else if (Value == SEL_DISCOMP_ON)
    {
        IPL_BufGet(Id, IPLBUF_SIEETH_1, &Buf, &Size);
        if (Size != 0)
        {
            //IQS_SettingFlowFunc(Id, IQS_DISCOMP_ON);
            IPL_CtrlSetInfor(Id, IPLCTRL_DIS_EN, ENABLE);
        }
        else
        {
            return;
        }
    }

    Info_DZ.Id = Id;
    Info_DZ.Idx = IPL_DzoomGetIndex(Info_DZ.Id);
    IPL_SetCmd(IPL_SET_DZOOM, (void *)&Info_DZ);
    IPL_WaitCmdFinish();
}

void IPL_FCB_AlgIE(IPL_PROC_ID Id,UINT32 SelIdx, UINT32 Value)
{
    IQS_SettingFlowFunc(Id, IQS_IMAGE_EFFECT);
}

void IPL_FCB_ImageFlip(IPL_PROC_ID Id,UINT32 SelIdx, UINT32 Value)
{
//#NT#2016/03/02#Jarkko Chang -begin
//#NT# support both dsp and sensor flip
    if (Value == SEL_IMAGEFLIP_DSP_H_V || Value == SEL_IMAGEFLIP_DSP_H || Value == SEL_IMAGEFLIP_DSP_V || Value == SEL_IMAGEFLIP_DSP_OFF)
    {
        IPL_SET_FLIP_DATA FlipInfo;

        FlipInfo.Id = Id;
        if ( Value == SEL_IMAGEFLIP_DSP_H_V )
        FlipInfo.FlipInfo = FLIP_H_V;
        if ( Value == SEL_IMAGEFLIP_DSP_H )
        FlipInfo.FlipInfo = FLIP_H;
        if ( Value == SEL_IMAGEFLIP_DSP_V )
        FlipInfo.FlipInfo = FLIP_V;
        if ( Value == SEL_IMAGEFLIP_DSP_OFF )
        FlipInfo.FlipInfo = FLIP_NONE;

        IPL_SetCmd(IPL_SET_FLIP, (void *)&FlipInfo);
        IPL_WaitCmdFinish();
    }
    else
    {
        UINT32 option;
        SENSOR_ID SenId;

        SenId = IPL_UTI_CONV2_SEN_ID(Id);
        option = Sensor_GetUserOption(SenId);
        option &= ~(SENSOR_OPTION_MIRROR | SENSOR_OPTION_FLIP);
        if ( Value == SEL_IMAGEFLIP_SEN_H_V )
            option = (SENSOR_OPTION_MIRROR | SENSOR_OPTION_FLIP);
        if ( Value == SEL_IMAGEFLIP_SEN_H )
            option = (SENSOR_OPTION_MIRROR);
        if ( Value == SEL_IMAGEFLIP_SEN_V )
            option = (SENSOR_OPTION_FLIP);
        Sensor_SetUserOption(SenId, option);

    }
//#NT#2016/03/02#Jarkko Chang -end
}


void IPL_FCB_AlgWDR(IPL_PROC_ID Id, UINT32 SelIdx, UINT32 Value)
{
    IPL_MODE Mode;

    Mode = IPL_CtrlGetInfor(Id, IPLCTRL_CUR_MODE);
	//#NT#2017/01/06#Jarkko Chang -begin
    //#NT# modify for wdr_defog
    if (Value != SEL_WDR_OFF)
    {
        ExtFunc_Init(Id, NULL, NULL);
    }
    else
    {
        IQS_SettingFlowFunc(Id, IQS_WDR_END);
    }
	//#NT#2017/01/06#Jarkko Chang -end
}

void IPL_FCB_AlgWDRLevel(IPL_PROC_ID Id, UINT32 SelIdx, UINT32 Value)
{
    ExtFunc_SetWDRLumaLevel(Id, Value);
}

void IPL_RAWHeaderCB(IPL_PROC_ID Id, UINT32 FrmCnt)
{
    if (Id == IPL_ID_1)
    {
        if (IPL_CtrlGetInfor(Id, IPLCTRL_EXT_FUNC_EN) == ICF_FUNC_SHDR)
        {
            UINT32 addr = SHDRCtrl_GetParam(FrmCnt);
            IPL_HeaderRawSetInfo(Id, IPL_HEADER_RAW_RHE, &addr);
        }
    }

    AEALG_ARG AeData = {0};
    IPL_HEADER_RAW_EXP_INFO AeHeaderInfo = {0};

    IPL_HeaderRawLock(Id);
    //write header information at here
    if (FrmCnt > 0)
    {
        AE_GetSyncInfo(Id, FrmCnt, &AeData);

        AeHeaderInfo.expt = AeData.ExpoTime;
        AeHeaderInfo.iris = AeData.Iris;
        AeHeaderInfo.iso = AeData.ISOGain;

        IPL_HeaderRawSetInfo_Dly1VD(Id, IPL_HEADER_RAW_EXP, &AeHeaderInfo);
    }

    IPL_HeaderRawUnlock(Id);
}
