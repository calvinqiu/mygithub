/*
    IPL flow Callback Function

    This file is the API of the flow callback function.

    @file       IPL_FlowCBSample.c
    @ingroup    mISYSAlg
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/
#define MS_CTRLBY_TIMER 0
#include "IPL_AR0238CSP_IMX322LQJ_EVB_FF_Int.h"
#include "Delay.h"
#include "Lens.h"
#include "ae_alg.h"
#include "LensCtrlTsk.h"
#include "sie_lib.h"

#include "rsc_task.h"
#include "rsc_common.h"

#if MS_CTRLBY_TIMER
#include "timer.h"
#endif
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

static FLASH_INFOR MainFlashInfo = {0};

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

void IPL_FCB_PrepareCapAE(IPL_PROC_ID Id)
{

}


void IPL_FCB_FlashTrig(FLASH_INFOR *Data)
{
#if 0   //ipl_tmp
    IPL_HAL_GROUP Update;
    IPC_IQparam IQ;
    SIE_SUBFUNC SubF;
    SIE_FLASH   FSH;

    Update.sieUpdate = (SIE_FLASH_PAR|SIE_FLASH_START);
    FSH.OutPolarity = Data->TriggerPolarity;
    FSH.InPhase = _RISING;
    FSH.TrigMode = Data->TriggerMode;
    FSH.DelayLine = Data->DelayLine;
    FSH.Delay2Pixel = Data->DelayPixel;
    FSH.Width = Data->FlashWidth;

    SubF.pFlash = &FSH;
    IQ.Idx = IPL_CtrlGetHalSetId(IPL_ID_1);
    IQ.pGrp = &Update;
    IQ.EngSubF.pSie_sub = &SubF;
    IPL_CTRLRunTimeChg_IQ(IPL_ID_1, IPC_SIE_IQparam,(void*)&IQ);
#endif
}

static void IPL_FCB_FlashCfg_Main(FLASH_INFOR *Data)
{
#if 0   //ipl_tmp
    IPL_HAL_GROUP Update;
    IPC_IQparam IQ;
    SIE_SUBFUNC SubF;
    SIE_FLASH   FSH;


    Update.sieUpdate = SIE_FLASH_PAR;
    FSH.OutPolarity = Data->TriggerPolarity;
    FSH.InPhase = _RISING;
    FSH.TrigMode = FLASH_TRI_MODE_IMD;//Data->TriggerMode;
    FSH.DelayLine = Data->DelayLine;
    FSH.Delay2Pixel = Data->DelayPixel;
    FSH.Width = Data->FlashWidth;

    SubF.pFlash = &FSH;
    IQ.Idx = IPL_CtrlGetHalSetId(IPL_ID_1);
    IQ.pGrp = &Update;
    IQ.EngSubF.pSie_sub = &SubF;
    IPL_CTRLRunTimeChg_IQ(IPL_ID_1, IPC_SIE_IQparam,(void*)&IQ);
#endif
}

static void IPL_FCB_MshutterConfig(IPL_PROC_ID Id)
{
#if 0   //ipl_tmp
    IPL_HAL_GROUP Update;
    IPC_IQparam IQ;
    SIE_SUBFUNC SubF;
    SIE_Mshtr   MSH;

    //DBG_ERR("MS test close!!\r\n");
    Update.sieUpdate = SIE_MSHUT_PAR;
    MSH.DelayFrame = 0;
    MSH.MshtrMode = MSH_PULSE;
    MSH.bPinSwap = TRUE;
    MSH.info.Delay2Pixel = 0;
    MSH.info.DelayLine = 1;
    MSH.info.InPhase = _RISING;
    MSH.info.OutPolarity = high_act;
    MSH.info.TrigMode = TRI_IMD;
    MSH.info.Width = (15*120000);

    SubF.pMshtr = &MSH;
    IQ.Idx = IPL_CtrlGetHalSetId(Id);
    IQ.pGrp = &Update;
    IQ.EngSubF.pSie_sub = &SubF;
    IPL_CTRLRunTimeChg_IQ(Id, IPC_SIE_IQparam,(void*)&IQ);
#endif
}
void IPL_FCB_MshutterClose(UINT32 uiEvent)
{
#if 0   //ipl_tmp
#if MS_CTRLBY_TIMER
    if (MainFlashInfo.TriggerEnable == ENABLE)
        sie_setFlashStart();

    sie_setMshtrCloseStart();
#else
    IPL_HAL_GROUP Update = {0};
    IPC_IQparam IQ;
    SIE_SUBFUNC SubF;
    SIE_Mshtr   MSH;
/*
    //DBG_ERR("MS test close!!\r\n");
    Update.sieUpdate = SIE_MSHUT_PAR;
    MSH.DelayFrame = 0;
    MSH.MshtrMode = MSH_PULSE;
    MSH.bPinSwap = TRUE;
    MSH.info.Delay2Pixel = 0;
    MSH.info.DelayLine = 1;
    MSH.info.InPhase = _RISING;
    MSH.info.OutPolarity = high_act;
    MSH.info.TrigMode = TRI_IMD;
    MSH.info.Width = (15*120000);
*/
    Update.sieUpdate |= SIE_MSHUT_CLOSE;
    SubF.pMshtr = &MSH;
    IQ.Idx = IPL_CtrlGetHalSetId(Id);
    IQ.pGrp = &Update;
    IQ.EngSubF.pSie_sub = &SubF;
    IPL_CTRLRunTimeChg_IQ(Id, IPC_SIE_IQparam,(void*)&IQ);
#endif
/*
    gpio_clearPin(P_GPIO_36);
    gpio_clearPin(P_GPIO_37);
    gpio_setDir(P_GPIO_36, GPIO_DIR_OUTPUT);
    gpio_setDir(P_GPIO_37, GPIO_DIR_OUTPUT);
    gpio_setPin(P_GPIO_36);
*/
#endif
}

void IPL_FCB_MshutterOpen(IPL_PROC_ID Id)
{
#ifdef ipl_hal
#if MS_CTRLBY_TIMER
#if 0   //ipl_tmp
    sie_setMshtrOpenStart();
#endif
#else
    IPL_HAL_GROUP Update = {0};
    IPC_IQparam IQ;
    SIE_SUBFUNC SubF;
    SIE_Mshtr   MSH;
/*
    DBG_ERR("MS test open!!\r\n");
    Update.sieUpdate = SIE_MSHUT_PAR;
    MSH.DelayFrame = 0;
    MSH.MshtrMode = MSH_PULSE;
    MSH.bPinSwap = TRUE;
    MSH.info.Delay2Pixel = 0;
    MSH.info.DelayLine = 1;
    MSH.info.InPhase = _RISING;
    MSH.info.OutPolarity = high_act;
    MSH.info.TrigMode = TRI_IMD;//TRI_COUNT;
    MSH.info.Width = (15*120000);// uint is 2 pixel clocks
*/
    Update.sieUpdate |= SIE_MSHUT_OPEN;

    SubF.pMshtr = &MSH;
    IQ.Idx = IPL_CtrlGetHalSetId(Id);
    IQ.pGrp = &Update;
    IQ.EngSubF.pSie_sub = &SubF;
    IPL_CTRLRunTimeChg_IQ(Id, IPC_SIE_IQparam,(void*)&IQ);
#endif
/*
    gpio_clearPin(P_GPIO_36);
    gpio_clearPin(P_GPIO_37);
    gpio_setDir(P_GPIO_36, GPIO_DIR_OUTPUT);
    gpio_setDir(P_GPIO_37, GPIO_DIR_OUTPUT);
    gpio_setPin(P_GPIO_37);
*/
#endif
}

void IPL_FCB_PreFlash(IPL_PROC_ID Id)
{
    IPL_CBMsgProc(Id, IPL_CBMSG_PREFLASH, NULL);
    //MainFlashInfo = flash_PreFlashFlow(IPL_FCB_FlashTrig);
    MainFlashInfo.TriggerEnable = DISABLE;
    IPL_CBMsgProc(Id, IPL_CBMSG_PREFLASHEND, NULL);
}

BOOL GRS_Mode = 0;
void IPL_FCB_GetCapRawNormal(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo)
{
    EXPOSURE_SETTING ExpTSetting = {0};
    GAIN_SETTING Gain = {0};
    SENSOR_MODE Mode = 0;
    UINT32 Ratio[SEN_MFRAME_MAX_NUM] = {0};
    UINT32 capiso[SEN_MFRAME_MAX_NUM] = {0},capext[SEN_MFRAME_MAX_NUM] = {0},iris[SEN_MFRAME_MAX_NUM] = {0}, Iris_Step;
    IPC_CTRL_CMD ProcCmd[IPL_ID_MAX_NUM] = {IPC_IGNORE,IPC_IGNORE,IPC_IGNORE,IPC_IGNORE};
#if MS_CTRLBY_TIMER
    TIMER_ID    TimerID_MS;
    ER  TimerState;

    TimerState = timer_open(&TimerID_MS, IPL_FCB_MshutterClose);
    if(TimerState != E_OK)
    {
        GRS_Mode = 0;
        DBG_ERR("Timer oper fail, use rolling shutter!");
    }
    else
    {
        GRS_Mode = 1;
    }
#endif

    SENSOR_ID SenId;
    SenId = IPL_UTI_CONV2_SEN_ID(Id);

    //#NT#2016/10/12#Harry Tsai -begin
    //#NT# Modify CCIR flow
    //change sensor mode to capture
    if(ExtInfo->Param[0] != 0x123)
    {
        AE_GetCapAEArg(Id, &capiso[0], &capext[0], &iris[0]);

        if (MainFlashInfo.TriggerEnable == ENABLE)
        {
            capiso[0] = MainFlashInfo.Flash_Cap_ISO;
            capext[0] = MainFlashInfo.Flash_Cap_ExpoTime;
        }

        switch(iris[0])
        {
            case AEALG_IRIS_0:
            default:
                Iris_Step = IRIS_POS_F1_8;
                break;
            case AEALG_IRIS_1:
                Iris_Step = IRIS_POS_F2_8;
                break;
            case AEALG_IRIS_2:
                Iris_Step = IRIS_POS_F4_0;
                break;
            case AEALG_IRIS_3:
                Iris_Step = IRIS_POS_F5_6;
                break;
            case AEALG_IRIS_4:
                Iris_Step = IRIS_POS_F8_0;
                break;
        }
        Lens_Aperture_Move(Iris_Step);
        Lens_Shutter_Move(MOTOR_SHUTTER_CAP, ENABLE);
    }
    Sensor_ChgMode(SenId, IPL_AlgGetUIInfo(Id, IPL_SEL_CAPSENMODE));
    IPC_WaitVD(Id,TRUE);
    if(ExtInfo->Param[0] != 0x123)
    {
        Sensor_GetCurMode(SenId, &Mode);
        Sensor_GetExpoSetting(SenId, &capext[0], Mode, &ExpTSetting, &Ratio[0]);
        capiso[0] = capiso[0] * Ratio[0] / 100;
        Sensor_GetGainSetting(SenId, &capiso[0], &Gain);
        Sensor_SetExpTime(SenId, &ExpTSetting);
        Sensor_SetGain(SenId, &Gain);
    }
    //#NT#2016/10/12#Harry Tsai -end

    if(RSC_GetStatus(Id) == SEL_RSC_ON)
    {
        RSC_OpenTsk();
        RSC_SetStatus(Id, SEL_RSC_ON);
    }

    if(GRS_Mode)
    {
        IPL_FCB_MshutterConfig(Id);
        if (MainFlashInfo.TriggerEnable == ENABLE)
            IPL_FCB_FlashCfg_Main(&MainFlashInfo);


//        Sensor_SetGRS(SenId); //ipl tmp
        IPC_WaitVD(Id,TRUE);
#if MS_CTRLBY_TIMER
        timer_cfg(TimerID_MS, 51000, TIMER_MODE_ONE_SHOT | TIMER_MODE_ENABLE_INT, TIMER_STATE_PLAY);
#endif
        IPL_CTRLRunTimeChg(Id, IPC_SIEx_, (void*)Info);

        ProcCmd[Id] = IPC_RESUME_SIE;
        IPL_CTRLMode(ProcCmd[IPL_ID_1],ProcCmd[IPL_ID_2],ProcCmd[IPL_ID_3],ProcCmd[IPL_ID_4]);
#if (MS_CTRLBY_TIMER == 0)
    /*** need to calibrate the delay time and modify the control methold ***/
        if(SENSOR_MODE_2 == Mode)
            Delay_DelayUs(51000);
        else
            Delay_DelayUs(32000);

        IPL_FCB_MshutterClose(0);
    /***********************************************************/
#endif
    }
    else// ERS mode
    {
        IPL_CTRLRunTimeChg(Id, IPC_SIEx_, (void*)Info);

        if (MainFlashInfo.TriggerEnable == ENABLE)
        {
            IPL_FCB_FlashTrig(&MainFlashInfo);
        }

        IPC_WaitVD(Id, FALSE);

        ProcCmd[Id] = IPC_RESUME_SIE;
        IPL_CTRLMode(ProcCmd[IPL_ID_1],ProcCmd[IPL_ID_2],ProcCmd[IPL_ID_3],ProcCmd[IPL_ID_4]);
    }
    IPC_WaitVD(Id, FALSE);

    ProcCmd[Id] = IPC_PAUSE_SIE;
    IPL_CTRLMode(ProcCmd[IPL_ID_1],ProcCmd[IPL_ID_2],ProcCmd[IPL_ID_3],ProcCmd[IPL_ID_4]);
    IPC_WaitVD(Id, FALSE);

    if(GRS_Mode)
    {
        IPL_FCB_MshutterOpen(Id);
        Delay_DelayMs(15);
        Lens_Shutter_Move(MOTOR_SHUTTER_CAP, DISABLE);
#if MS_CTRLBY_TIMER
        timer_close(TimerID_MS);
#endif
    }

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
}

void IPL_FCB_GetCapRawInVid(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo)
{
    IPL_HAL_GROUP Update;
    IPC_IPLInAddr rdeinfo;
    IPC_CTRL_CMD ProcCmd[IPL_ID_MAX_NUM] = {IPC_IGNORE,IPC_IGNORE,IPC_IGNORE,IPC_IGNORE};

    Update.sieUpdate = Info->sieUpdate;

    IPC_WaitVD(Id, TRUE);
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
    UINT32 capiso[SEN_MFRAME_MAX_NUM] = {0}, capext[SEN_MFRAME_MAX_NUM] = {0}, iris[SEN_MFRAME_MAX_NUM] = {0}, Ratio[SEN_MFRAME_MAX_NUM] = {0}, Iris_Step;
    SENSOR_ID SenId;
    IPC_CTRL_CMD ProcCmd[IPL_ID_MAX_NUM] = {IPC_IGNORE,IPC_IGNORE,IPC_IGNORE,IPC_IGNORE};

    if (ExtInfo->Param[0] == 0)
    {
        AE_GetCapAEArg(Id, &capiso[0], &capext[0], &iris[0]);
        switch(iris[0])
        {
            case AEALG_IRIS_0:
            default:
                Iris_Step = IRIS_POS_F1_8;
                break;
            case AEALG_IRIS_1:
                Iris_Step = IRIS_POS_F2_8;
                break;
            case AEALG_IRIS_2:
                Iris_Step = IRIS_POS_F4_0;
                break;
            case AEALG_IRIS_3:
                Iris_Step = IRIS_POS_F5_6;
                break;
            case AEALG_IRIS_4:
                Iris_Step = IRIS_POS_F8_0;
                break;
        }
        Lens_Aperture_Move(Iris_Step);

        //change sensor mode to capture
        IPC_WaitVD(Id, TRUE);
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

    if (MainFlashInfo.TriggerEnable == ENABLE)
    {
        IPL_FCB_FlashTrig(&MainFlashInfo);
    }

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

    if (MainFlashInfo.TriggerEnable == ENABLE)
    {
        IPL_FCB_FlashTrig(&MainFlashInfo);
    }

    IPC_WaitVD(Id,FALSE);

    ProcCmd[Id] = IPC_RESUME_SIE;
    IPL_CTRLMode(ProcCmd[IPL_ID_1],ProcCmd[IPL_ID_2],ProcCmd[IPL_ID_3],ProcCmd[IPL_ID_4]);
    IPC_WaitVD(Id,FALSE);

    ProcCmd[Id] = IPC_PAUSE_SIE;
    IPL_CTRLMode(ProcCmd[IPL_ID_1],ProcCmd[IPL_ID_2],ProcCmd[IPL_ID_3],ProcCmd[IPL_ID_4]);
    IPC_WaitVD(Id,FALSE);
}

//#NT#2016/03/30#Harry Tsai -begin
//#NT# Add side by side capture mode
void IPL_FCB_GetCapRawDual(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo)
{
    EXPOSURE_SETTING ExpTSetting = {0};
    GAIN_SETTING Gain = {0};
    SENSOR_MODE Mode = 0;
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

    if(ExtInfo->Param[6] == 0)
        return;

    //Hot code
#if 1
    ProcCmd[IPL_ID_1] = IPC_RESUME_SIE;
    ProcCmd[IPL_ID_2] = IPC_RESUME_SIE;
    IPL_CTRLMode(ProcCmd[IPL_ID_1],ProcCmd[IPL_ID_2],ProcCmd[IPL_ID_3],ProcCmd[IPL_ID_4]);

    IPC_WaitVD(IPL_ID_1,FALSE);
    IPC_WaitVD(IPL_ID_2,FALSE);

    ProcCmd[IPL_ID_1] = IPC_PAUSE_SIE;
    ProcCmd[IPL_ID_2] = IPC_PAUSE_SIE;
    IPL_CTRLMode(ProcCmd[IPL_ID_1],ProcCmd[IPL_ID_2],ProcCmd[IPL_ID_3],ProcCmd[IPL_ID_4]);

    IPC_WaitVD(IPL_ID_1,FALSE);
    IPC_WaitVD(IPL_ID_2,FALSE);

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

