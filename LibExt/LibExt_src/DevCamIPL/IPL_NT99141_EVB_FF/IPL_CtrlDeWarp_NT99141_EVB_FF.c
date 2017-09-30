/**
    Ctrl sample fw.

    @file       IPL_CtrlSampleFW.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include "IPL_NT99141_EVB_FF_Int.h"

UINT32 IPL_IFE2InitCBFlowD(UINT32 Id,ENG_SUBFUNC *Info,IPL_HAL_GROUP* group);
void* IPL_AdaptSettingCBFlowD(UINT32 Id, IPL_RAW_IMG_INFO IplInInfo, IPL_YCC_CH_INFO *ChInfo);
IPH_CLK IPL_SIEClkCBFlowD(UINT32 Id);
UINT32 IPL_SIEClkSrcCBFlowD(UINT32 Id);
UINT32 IPL_SIEPclkSelCBFlowD(UINT32 Id);
IPH_CLK IPL_RHEClkCBFlowD(UINT32 Id);
IPH_CLK IPL_RDEClkCBFlowD(UINT32 Id);
IPH_CLK IPL_IFEClkCBFlowD(UINT32 Id);
IPH_CLK IPL_DCEClkCBFlowD(UINT32 Id);
IPH_CLK IPL_IPEClkCBFlowD(UINT32 Id);
IPH_CLK IPL_IFE2ClkCBFlowD(UINT32 Id);
IPH_CLK IPL_IMEClkCBFlowD(UINT32 Id);
UINT32 IPL_SIEInitCBFlowD(UINT32 Id,ENG_SUBFUNC *Info,IPL_HAL_GROUP* group);
UINT32 IPL_RHEInitCBFlowD(UINT32 Id,ENG_SUBFUNC *Info,IPL_HAL_GROUP* group);
UINT32 IPL_RDEInitCBFlowD(UINT32 Id,ENG_SUBFUNC *Info,IPL_HAL_GROUP* group);
UINT32 IPL_IFEInitCBFlowD(UINT32 Id,ENG_SUBFUNC *Info,IPL_HAL_GROUP* group);
UINT32 IPL_DCEInitCBFlowD(UINT32 Id,ENG_SUBFUNC *Info,IPL_HAL_GROUP* group);
UINT32 IPL_IPEInitCBFlowD(UINT32 Id,ENG_SUBFUNC *Info,IPL_HAL_GROUP* group);
UINT32 IPL_IMEInitCBFlowD(UINT32 Id,ENG_SUBFUNC *Info,IPL_HAL_GROUP* group);

IPLCTRL_FLOWD_INFO IPL_FlowDInfo =
{
    {IPL_HAL_PAR0, IPL_HAL_PAR0, IPL_HAL_PAR0, IPL_HAL_PAR0},

    IPL_IFE2InitCBFlowD,
    IPL_AdaptSettingCBFlowD,
    IPL_SIEClkCBFlowD,
    IPL_SIEClkSrcCBFlowD,
    IPL_SIEPclkSelCBFlowD,
    IPL_RHEClkCBFlowD,
    IPL_RDEClkCBFlowD,
    IPL_IFEClkCBFlowD,
    IPL_DCEClkCBFlowD,
    IPL_IPEClkCBFlowD,
    IPL_IFE2ClkCBFlowD,
    IPL_IMEClkCBFlowD,

    IPL_SIEInitCBFlowD,
    IPL_RHEInitCBFlowD,
    IPL_RDEInitCBFlowD,
    IPL_IFEInitCBFlowD,
    IPL_DCEInitCBFlowD,
    IPL_IPEInitCBFlowD,
    IPL_IMEInitCBFlowD,
};

static IPLCTRL_FLOWD_INFO* IPL_GetCtrlFlowDInfo(void)
{
    return &IPL_FlowDInfo;
}

IPL_HAL_PARASET IPL_CtrlGetHalSetIdFlowD(UINT32 Id)
{
    return IPL_GetCtrlFlowDInfo()->IPL_HalParamId[Id];
}

UINT32 IPL_IFE2InitCBFlowD(UINT32 Id,ENG_SUBFUNC *Info,IPL_HAL_GROUP* group)
{
    UINT32 Cnt;
    IQCB_SettingFlowFunc(Id, IQCB_PRV_IFE2_INIT, Info, group);
    Cnt = IQS_GetIFE2LcaTime(Id, IQCB_PRV_IFE2_INIT);
    return Cnt;
}

void* IPL_AdaptSettingCBFlowD(UINT32 Id, IPL_RAW_IMG_INFO IplInInfo, IPL_YCC_CH_INFO *ChInfo)
{
    ChInfo->Width  = IPL_UTI_IPESUB_ADJ_HSIZE(IplInInfo.Width, IQS_GetChromaAdaptRatio(Id, FALSE, FALSE), 4);
    if (ChInfo->Width < 80)
    {
        ChInfo->Width = 80;
    }

    ChInfo->Height = IPL_UTI_IPESUB_ADJ_VSIZE(IplInInfo.Height, IQS_GetChromaAdaptRatio(Id, FALSE, FALSE), 4);
    if (ChInfo->Height < 60)
    {
        ChInfo->Height = 60;
    }
    ChInfo->LineOfs = IPL_YSIZEH_CONV2LOFS(IPL_CtrlGetInfor(Id, IPLCTRL_IPE_OUT_FMT), ChInfo->Width);

    IPL_CtrlSetInfor(Id, IPLCTRL_IPE_OUT_SIZE_H, ChInfo->Width);
    IPL_CtrlSetInfor(Id, IPLCTRL_IPE_OUT_SIZE_YLOS, ChInfo->LineOfs);
    IPL_CtrlSetInfor(Id, IPLCTRL_IPE_OUT_SIZE_V, ChInfo->Height);

    return NULL;
}

#if 0
#endif
IPH_CLK IPL_SIEClkCBFlowD(UINT32 Id)
{
    IPH_CLK clk = {0};
    UINT32 sieclk = 0;
    SENSOR_INFO SenInfo = {0};
    ER rt = E_OK;

    rt = IPL_GetSensorStatus(Id, IPL_AlgGetUIInfo(Id, IPL_SEL_PRVSENMODE), &SenInfo);
    if (rt != E_OK)
        return clk;

    switch (SenInfo.DataType)
    {
        case SENSOR_DATA_LVDS:
        case SENSOR_DATA_MIPI:
            sieclk = 0;
        break;

        //#NT#2016/03/08#Jarkko Chang -begin
        //#NT# fixed SIE CLK error when VX1 Sensor
        case SENSOR_DATA_PARALLEL:
            sieclk = SenInfo.Mode->Pclk * 2;
        break;
        //#NT#2016/03/08#Jarkko Chang -end

        case SENSOR_DATA_DUMMY:
            sieclk = 27000000;
        break;

        default:
            sieclk = 54000000;
        break;
    }

    switch (Id)
    {
        case IPL_ID_1:
            clk.SIEclk = sieclk;
            break;

        case IPL_ID_2:
            clk.SIE2clk = sieclk;
            break;

        case IPL_ID_3:
            clk.SIE3clk = sieclk;
            break;

        case IPL_ID_4:
            clk.SIE4clk = sieclk;
            break;

        default:
            DBG_ERR("IPL ID %d Error \r\n", Id);
        break;
    }

    return clk;
}

UINT32 IPL_SIEClkSrcCBFlowD(UINT32 Id)
{
    return IPH_SIECLKSRC_CURR;
}

UINT32 IPL_SIEPclkSelCBFlowD(UINT32 Id)
{
    SENSOR_INFO SenInfo = {0};
    ER rt = E_OK;

    rt = IPL_GetSensorStatus(Id, IPL_AlgGetUIInfo(Id, IPL_SEL_PRVSENMODE), &SenInfo);
    if (rt != E_OK)
        return IPH_SIEPXCLK_OFF;

    if (SenInfo.CmdType == SENSOR_CMD_Vx1)
        return IPH_SIEPXCLK_VX1_2X;
    else
        return IPH_SIEPXCLK_PAD;
}

IPH_CLK IPL_RHEClkCBFlowD(UINT32 Id)
{
    IPH_CLK clk;
    clk.RHEclk = IPH_RHECLK1;
    return clk;
}

IPH_CLK IPL_RDEClkCBFlowD(UINT32 Id)
{
    IPH_CLK clk;
    clk.RDEclk = IPH_RDECLK2;
    return clk;
}

IPH_CLK IPL_IFEClkCBFlowD(UINT32 Id)
{
    IPH_CLK clk;
    clk.IFEclk = IPH_IFECLK3;
    return clk;
}

IPH_CLK IPL_DCEClkCBFlowD(UINT32 Id)
{
    IPH_CLK clk;
    clk.DCEclk = IPH_DCECLK3;
    return clk;
}

IPH_CLK IPL_IPEClkCBFlowD(UINT32 Id)
{
    IPH_CLK clk;
    clk.IPEclk = IPH_IPECLK3;
    return clk;
}

IPH_CLK IPL_IFE2ClkCBFlowD(UINT32 Id)
{
    IPH_CLK clk;
    clk.IFE2clk = IPH_IFE2CLK3;
    return clk;
}

IPH_CLK IPL_IMEClkCBFlowD(UINT32 Id)
{
    IPH_CLK clk;
    clk.IMEclk = IPH_IMECLK3;
    return clk;
}

#if 0
#endif

UINT32 IPL_SIEInitCBFlowD(UINT32 Id,ENG_SUBFUNC *Info,IPL_HAL_GROUP* group)
{
    SIE_UPDATE *SIE_Update;

    SIE_Update = (SIE_UPDATE *)group;
    //init info structure

    IPL_SIESetOB_FCB(Id, &Info->pSie_sub->sieFuncEn, &Info->pSie_sub->InteEn, SIE_Update, &Info->pSie_sub->OBAvg, &Info->pSie_sub->OBSub);
    IPL_SIESetCA_FCB(Id, &Info->pSie_sub->sieFuncEn, &Info->pSie_sub->InteEn, SIE_Update, &Info->pSie_sub->Ca, &Info->pSie_sub->pStatPathInfo);
    IPL_SIESetLA_FCB(Id, &Info->pSie_sub->sieFuncEn, &Info->pSie_sub->InteEn, SIE_Update, &Info->pSie_sub->La, &Info->pSie_sub->pStatPathInfo);
    IPL_SIESetBP_FCB(Id, &Info->pSie_sub->sieFuncEn, &Info->pSie_sub->InteEn, SIE_Update, Info->pSie_sub);
    IPL_SIESetDMAOutSrc_FCB(Id, &Info->pSie_sub->sieFuncEn, &Info->pSie_sub->InteEn, SIE_Update);
    IPL_SIESetDP_FCB(Id,&Info->pSie_sub->sieFuncEn,&Info->pSie_sub->InteEn,SIE_Update,Info->pSie_sub);
    IPL_SIESetRawEnc_FCB(Id, SIE_Update, &Info->pSie_sub->sieFuncEn, &Info->pSie_sub->RawEnc);
    IPL_SIESetEthOut_FCB(Id,&Info->pSie_sub->sieFuncEn,&Info->pSie_sub->InteEn,SIE_Update,Info->pSie_sub);
    IPL_SIECCIR_FCB(Id, Info->pSie_sub->pDvi);

    *SIE_Update |= SIE_SUBFEN;
    IQCB_SettingFlowFunc(Id, IQCB_PRV_SIE_INIT, Info, group);
    return 1;
}

UINT32 IPL_RDEInitCBFlowD(UINT32 Id,ENG_SUBFUNC *Info,IPL_HAL_GROUP* group)
{
    IQCB_SettingFlowFunc(Id, IQCB_PRV_RDE_INIT, Info, group);
    return 1;
}

UINT32 IPL_IFEInitCBFlowD(UINT32 Id,ENG_SUBFUNC *Info,IPL_HAL_GROUP* group)
{
    IQCB_SettingFlowFunc(Id, IQCB_PRV_IFE_INIT, Info, group);
    return 1;
}

UINT32 IPL_DCEInitCBFlowD(UINT32 Id,ENG_SUBFUNC *Info,IPL_HAL_GROUP* group)
{
    IQCB_SettingFlowFunc(Id, IQCB_PRV_DCE_INIT, Info, group);

    UINT32 sieOutH,sieOutV,ipl_sizeH,ipl_sizeV;

    sieOutH  = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_H);
    sieOutV  = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_V);
    ipl_sizeH = IPL_CtrlGetInfor(Id, IPLCTRL_IPL_IN_SIZE_H);
    ipl_sizeV = IPL_CtrlGetInfor(Id, IPLCTRL_IPL_IN_SIZE_V);
    RSC_GetNull2D(Id,sieOutH,sieOutV,ipl_sizeH,ipl_sizeV,Info->pDce_sub, group);
    return 1;
}

UINT32 IPL_IPEInitCBFlowD(UINT32 Id,ENG_SUBFUNC *Info,IPL_HAL_GROUP* group)
{
    IQCB_SettingFlowFunc(Id, IQCB_PRV_IPE_INIT, Info, group);
    return 1;
}

UINT32 IPL_IMEInitCBFlowD(UINT32 Id,ENG_SUBFUNC *Info,IPL_HAL_GROUP* group)
{
    IQCB_SettingFlowFunc(Id, IQCB_PRV_IME_INIT, Info, group);
    return 1;
}

UINT32 IPL_RHEInitCBFlowD(UINT32 Id,ENG_SUBFUNC *Info,IPL_HAL_GROUP* group)
{
    ExtFunc_Init(Id, Info, group);
    return 1;
}


#if 0
#endif
void IPL_CtrlInitIpcInfoFlowD(UINT32 Id, IPC_INFO *IpcInfo)
{
    SENSOR_INFO SenInfo = {0};
    IPLCTRL_FLOWD_INFO *FlowInfo;
//    IPL_IME_PATH_INFO AdasP4Info = {0};
    ER rt = E_OK;

    FlowInfo = IPL_GetCtrlFlowDInfo();

    IpcInfo->ICF_FD.ParaIdx = FlowInfo->IPL_HalParamId[Id];
    IpcInfo->ICF_FD.BaseInfo.IPLCFMode = IPL_CtrlGetInfor(Id, IPLCTRL_CFMODE);
    IpcInfo->ICF_FD.BaseInfo.IPLCFModeExt = IPL_CtrlGetInfor(Id, IPLCTRL_CFMODEEXT);

    rt = IPL_GetSensorStatus(Id, IPL_AlgGetUIInfo(Id, IPL_SEL_PRVSENMODE), &SenInfo);
    if (rt != E_OK)
        return;

    IPL_CtrlInitIpc_SIEBurstLength(Id, &SenInfo, &IpcInfo->ICF_FD.BaseInfo.BurstLength);
    IPL_CtrlInitIpc_SIEAct(Id, &SenInfo, &IpcInfo->ICF_FD.BaseInfo.SIE_Act);
    IPL_CtrlInitIpc_SIEIn(Id, &SenInfo, &IpcInfo->ICF_FD.BaseInfo.SIE_IN);
    IPL_CtrlInitIpc_SIESignal(Id, FALSE, FALSE, _RISING, _RISING, _RISING, &IpcInfo->ICF_FD.BaseInfo.SenSorIn);

    if (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_EN) == ENABLE)
    {
        IPL_CtrlInitIpc_SIECh0PPB(Id, &IpcInfo->ICF_FD.BaseInfo.SIE_PPB0, &IpcInfo->ICF_FD.BaseInfo.SIE_EXT_PPB[0], &IpcInfo->ICF_FD.BaseInfo.SIE_PPB_OFS);

        if ((IpcInfo->ICF_FD.BaseInfo.SIE_PPB0.Width == IpcInfo->ICF_FD.BaseInfo.SIE_IN.Crop.SizeH) &&
            (IpcInfo->ICF_FD.BaseInfo.SIE_PPB0.Height == IpcInfo->ICF_FD.BaseInfo.SIE_IN.Crop.SizeV))
        {
            IpcInfo->ICF_FD.BaseInfo.SIE_Src = NonScaled_Img;
        }
        else
        {
            IpcInfo->ICF_FD.BaseInfo.SIE_Src = CH0_Scaled_Img;
        }
    }
    else
    {
        IpcInfo->ICF_FD.BaseInfo.SIE_Src = NonScaled_Img;
    }

    if (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CA_EN) == ENABLE)
    {
        IPL_CtrlInitIpc_SIECh1PPB(Id, &IpcInfo->ICF_FD.BaseInfo.SIE_CH1, &IpcInfo->ICF_FD.BaseInfo.SIE_EXT_PPB[0], &IpcInfo->ICF_FD.BaseInfo.SIE_PPB_OFS);
    }

    if (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_LA_EN) == ENABLE)
    {
        IPL_CtrlInitIpc_SIECh2PPB(Id, &IpcInfo->ICF_FD.BaseInfo.SIE_CH2, &IpcInfo->ICF_FD.BaseInfo.SIE_CH2_EXT_PPB[0], &IpcInfo->ICF_FD.BaseInfo.SIE_PPB_OFS);
    }

    if( IPL_CtrlGetInfor(Id, IPLCTRL_SIE_ETH_EN) == ENABLE)
    {
        IPL_CtrlInitIpc_SIEEthPPB(Id, &IpcInfo->ICF_FD.BaseInfo.Eth_Out);
    }

    IPL_CtrlInitIpc_IPLInPPB(Id, &IpcInfo->ICF_FD.BaseInfo.IPL_PPB0, &IpcInfo->ICF_FD.BaseInfo.IPL_StPos);

    IPL_CtrlInitIpc_DCEIn(Id, &SenInfo, &IpcInfo->ICF_FD.BaseInfo.DCE_Ref_1X, &IpcInfo->ICF_FD.BaseInfo.DCE_Ref_1XCenter,
                                        &IpcInfo->ICF_FD.BaseInfo.DCE_Ref_1XCenterOfs, &IpcInfo->ICF_FD.BaseInfo.SenHBinRatio, &IpcInfo->ICF_FD.BaseInfo.SenVBinRatio);

	//#NT#2016/09/13#Jarkko Chang -begin
	//#NT# fixed miss dewarp set dce dewarp infor issue
    //#NT#2016/07/20#Jarkko Chang -begin
    //#NT# add suppor vr360 flow
    if (IPL_CtrlGetInfor(Id, IPLCTRL_PRVFLOW) == ICF_FLOW_DEWARP){
	    IPL_CtrlInitIpc_DCEDeWarpInfo(Id, &IpcInfo->ICF_FD.StripeInfo[0], &IpcInfo->ICF_FD._2DInfo[0]);

	    if (IPL_CtrlGetInfor(Id, IPLCTRL_VR360_EN) == ENABLE){
    	    IPL_CtrlInitIpc_VR360Info(Id, &IpcInfo->ICF_FD.vrData);
	    }
    }
    //#NT#2016/07/20#Jarkko Chang -end
    //#NT#2016/09/13#Jarkko Chang -end

    if (IPL_CtrlGetInfor(Id, IPLCTRL_IPE_OUT_EN) == ENABLE)
    {
        IPL_CtrlInitIpc_IPESubOutPPB(Id, &IpcInfo->ICF_FD.BaseInfo.IPE_PPB.IPE_PPB0_ADDR, &IpcInfo->ICF_FD.BaseInfo.IPE_PPB.IPE_PPB1_ADDR, &IpcInfo->ICF_FD.BaseInfo.IPE_PPB.IPE_OUT);
    }

    if (IPL_CtrlGetInfor(Id, IPLCTRL_IFE2_LCA_EN) == ENABLE)
    {
        //Still need set ipt out parameters for ife2 input reference
        IPL_CtrlInitIpc_IPESubOutPPB(Id, NULL, NULL, &IpcInfo->ICF_FD.BaseInfo.IPE_PPB.IPE_OUT);
        IPL_CtrlInitIpc_IFE2LcaPPB(Id, &IpcInfo->ICF_FD.BaseInfo.IFE2_PPB0_ADDR, &IpcInfo->ICF_FD.BaseInfo.IFE2_PPB1_ADDR);
    }

    IpcInfo->ICF_FD.BaseInfo.IME_P1_ENABLE = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_EN);
    IpcInfo->ICF_FD.BaseInfo.IME_P1_D2H264 = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_D2H264);
    IPL_CtrlInitIpc_IMEPxOutPPB(Id, IPLCTRL_IME_P1_EN, &IpcInfo->ICF_FD.BaseInfo.IME_P1_PPB0);
    IPL_CtrlInitIpc_DispPxPPB(Id, IPLCTRL_IME_P1_EN);
    IpcInfo->ICF_FD.BaseInfo.IME_P1_Crop.Start.x = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_OUT_CROP_STR_X);
    IpcInfo->ICF_FD.BaseInfo.IME_P1_Crop.Start.y = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_OUT_CROP_STR_Y);
    IpcInfo->ICF_FD.BaseInfo.IME_P1_Crop.SizeH = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_OUT_CROP_H);
    IpcInfo->ICF_FD.BaseInfo.IME_P1_Crop.SizeV = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_OUT_CROP_V);

    IpcInfo->ICF_FD.BaseInfo.IME_P2_ENABLE = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_EN);
    IPL_CtrlInitIpc_IMEPxOutPPB(Id, IPLCTRL_IME_P2_EN, &IpcInfo->ICF_FD.BaseInfo.IME_P2_PPB0);
    IPL_CtrlInitIpc_DispPxPPB(Id, IPLCTRL_IME_P2_EN);
    IpcInfo->ICF_FD.BaseInfo.IME_P2_Crop.Start.x = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_OUT_CROP_STR_X);
    IpcInfo->ICF_FD.BaseInfo.IME_P2_Crop.Start.y = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_OUT_CROP_STR_Y);
    IpcInfo->ICF_FD.BaseInfo.IME_P2_Crop.SizeH = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_OUT_CROP_H);
    IpcInfo->ICF_FD.BaseInfo.IME_P2_Crop.SizeV = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_OUT_CROP_V);

    IpcInfo->ICF_FD.BaseInfo.IME_P3_ENABLE = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_EN);
    IPL_CtrlInitIpc_IMEPxOutPPB(Id, IPLCTRL_IME_P3_EN, &IpcInfo->ICF_FD.BaseInfo.IME_P3_PPB0);
    IPL_CtrlInitIpc_DispPxPPB(Id, IPLCTRL_IME_P3_EN);
    IpcInfo->ICF_FD.BaseInfo.IME_P3_Crop.Start.x = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_OUT_CROP_STR_X);
    IpcInfo->ICF_FD.BaseInfo.IME_P3_Crop.Start.y = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_OUT_CROP_STR_Y);
    IpcInfo->ICF_FD.BaseInfo.IME_P3_Crop.SizeH = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_OUT_CROP_H);
    IpcInfo->ICF_FD.BaseInfo.IME_P3_Crop.SizeV = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_OUT_CROP_V);

    IpcInfo->ICF_FD.BaseInfo.IME_P4_ENABLE = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P4_EN);
    IPL_CtrlInitIpc_IMEPxOutPPB(Id, IPLCTRL_IME_P4_EN, &IpcInfo->ICF_FD.BaseInfo.IME_P4_PPB0);
    IPL_CtrlInitIpc_DispPxPPB(Id, IPLCTRL_IME_P4_EN);
    IpcInfo->ICF_FD.BaseInfo.IME_P4_Crop.Start.x = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P4_OUT_CROP_STR_X);
    IpcInfo->ICF_FD.BaseInfo.IME_P4_Crop.Start.y = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P4_OUT_CROP_STR_Y);
    IpcInfo->ICF_FD.BaseInfo.IME_P4_Crop.SizeH = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P4_OUT_CROP_H);
    IpcInfo->ICF_FD.BaseInfo.IME_P4_Crop.SizeV = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P4_OUT_CROP_V);

    if (IPL_CtrlGetInfor(Id, IPLCTRL_IME_P4_EXTOUT_EN) == ENABLE)
    {
        IpcInfo->ICF_FD.BaseInfo.IME_P4_EXT_OUT_ENABLE = ENABLE;
        IpcInfo->ICF_FD.BaseInfo.IME_P4_EdgeMapBufOfs= IPL_CtrlGetInfor(Id, IPLCTRL_IME_P4_EDGEMAP_BUFOFS);
        IpcInfo->ICF_FD.BaseInfo.IME_P4_HistoBufOfs= IPL_CtrlGetInfor(Id, IPLCTRL_IME_P4_HISTO_BUFOFS);

        //#NT#2016/03/24#Jarkko Chang -begin
        //#NT# remove unusd code
        #if 0
        AdasP4Info = ADAS_getPath4Info();
        IpcInfo->ICF_FD.BaseInfo.IME_P4_Crop.Start.x = AdasP4Info.CropStartX;
        IpcInfo->ICF_FD.BaseInfo.IME_P4_Crop.Start.y = AdasP4Info.CropStartY;
        IpcInfo->ICF_FD.BaseInfo.IME_P4_Crop.SizeH = AdasP4Info.CropSizeH;
        IpcInfo->ICF_FD.BaseInfo.IME_P4_Crop.SizeV = AdasP4Info.CropSizeV;
        #endif
        //#NT#2016/03/24#Jarkko Chang -end
    }

    IpcInfo->ICF_FD.fp_AdaptSetting = FlowInfo->fp_AdaptSetting;

    //engine clock
    IpcInfo->ICF_FD.Ext.fp_SIEClk = FlowInfo->fp_SIEClk;
    IpcInfo->ICF_FD.Ext.fp_SIEClkSrc = FlowInfo->fp_SIEClkSrc;
    IpcInfo->ICF_FD.Ext.fp_SIEPclkSel = FlowInfo->fp_SIEPclkSel;
    IpcInfo->ICF_FD.Ext.fp_RDEClk = FlowInfo->fp_RDEClk;
    IpcInfo->ICF_FD.Ext.fp_IFEClk = FlowInfo->fp_IFEClk;
    IpcInfo->ICF_FD.Ext.fp_IFE2Clk = FlowInfo->fp_IFE2Clk;
    IpcInfo->ICF_FD.Ext.fp_DCEClk = FlowInfo->fp_DCEClk;
    IpcInfo->ICF_FD.Ext.fp_IPEClk = FlowInfo->fp_IPEClk;
    IpcInfo->ICF_FD.Ext.fp_IMEClk = FlowInfo->fp_IMEClk;

    //engine setting
    IpcInfo->ICF_FD.Ext.fp_SieInit = FlowInfo->fp_SieInit;
    IpcInfo->ICF_FD.Ext.fp_RdeInit = FlowInfo->fp_RdeInit;
    IpcInfo->ICF_FD.Ext.fp_IfeInit = FlowInfo->fp_IfeInit;
    IpcInfo->ICF_FD.Ext.fp_DceInit = FlowInfo->fp_DceInit;
    IpcInfo->ICF_FD.Ext.fp_IpeInit = FlowInfo->fp_IpeInit;
    IpcInfo->ICF_FD.Ext.fp_ImeInit = FlowInfo->fp_ImeInit;
    IpcInfo->ICF_FD.Ext.fp_Ife2Init = FlowInfo->fp_IFE2Init;
}
#if 0
#endif
void IPL_CtrlInitBufInfoFlowD(UINT32 Id, IPLBUF_ENG2DRAMINFO *BufInfo)
{
    //reset parameter
    memset((void *)BufInfo, 0x0, sizeof(IPLBUF_ENG2DRAMINFO));

    BufInfo->SIECh0En = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_EN);
    if (BufInfo->SIECh0En == ENABLE)
    {
        BufInfo->SIEOutCh0.BufNum = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_BUFNUM);
        BufInfo->SIEOutCh0.Width = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_H);
        BufInfo->SIEOutCh0.Height = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_V);
        BufInfo->SIEOutCh0.Bit = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_PRVBIT);
        BufInfo->SIEOutCh0.LineOfs = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_HLOS);
    }

    BufInfo->SIECh1En = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH1_Y_EN);
    BufInfo->SIECAEn = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CA_EN);
    if (BufInfo->SIECh1En == ENABLE)
    {
        BufInfo->SIEOutCh1.BufNum = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH1_BUFNUM);
        BufInfo->SIEOutCh1.Width = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_H);
        BufInfo->SIEOutCh1.Height = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_V);
        BufInfo->SIEOutCh1.LineOfs = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_H);
    }
    else if (BufInfo->SIECAEn == ENABLE)
    {
        BufInfo->SIECABufNum = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH1_BUFNUM);
        //CA Output R/G/B @16bit + Accnt + Lenth
        BufInfo->SIECABufSize = sizeof(UINT32)*IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CA_WINXNUM)*IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CA_WINYNUM)*4;
    }

    BufInfo->SIECh2En = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH2_UV_EN);
    BufInfo->SIELAEn = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_LA_EN);
    if (BufInfo->SIECh2En == ENABLE)
    {   //need Reference to CCIR Sensor Fmt
        BufInfo->SIEOutCh2.BufNum = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH2_BUFNUM);
        BufInfo->SIEOutCh2.Width = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_H);
        BufInfo->SIEOutCh2.Height = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_V);
        BufInfo->SIEOutCh2.LineOfs = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_H);
    }
    else if (BufInfo->SIELAEn == ENABLE)
    {
        BufInfo->SIELABufNum = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH2_BUFNUM);
        //LA Output PreGamma Lum/PostGamma Lum @16bit, Histogram 64bin@12bit
        if (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_LA_WINXNUM)*IPL_CtrlGetInfor(Id, IPLCTRL_SIE_LA_WINYNUM) >= 64)
            BufInfo->SIELABufSize = sizeof(UINT32)*IPL_CtrlGetInfor(Id, IPLCTRL_SIE_LA_WINXNUM)*IPL_CtrlGetInfor(Id, IPLCTRL_SIE_LA_WINYNUM)*3;
        else
            BufInfo->SIELABufSize = sizeof(UINT32)*(IPL_CtrlGetInfor(Id, IPLCTRL_SIE_LA_WINXNUM)*IPL_CtrlGetInfor(Id, IPLCTRL_SIE_LA_WINYNUM)*2 + 64);
    }

    BufInfo->SIECh3En = DISABLE;

    BufInfo->SIEVAEn = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_VA_EN);
    if (BufInfo->SIEVAEn == ENABLE)
    {
        BufInfo->SIEVABufNum = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_VA_BUFNUM);
        BufInfo->SIEVABufSize = IPL_CtrlGetSIEVABufInfo();
    }

    BufInfo->SIEEthEn = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_ETH_EN);
    if (BufInfo->SIEEthEn == ENABLE)
    {
        BufInfo->SIEEthSize = (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_ETH_SIZE_HLOS) * IPL_CtrlGetInfor(Id, IPLCTRL_SIE_ETH_SIZE_V));
    }

    //IPE Output
    BufInfo->IPEOutEn = IPL_CtrlGetInfor(Id, IPLCTRL_IPE_OUT_EN);
    //IFE2 Output
    BufInfo->IFE2LCAEn = IPL_CtrlGetInfor(Id, IPLCTRL_IFE2_LCA_EN);
    if ((BufInfo->IFE2LCAEn == ENABLE) || (BufInfo->IPEOutEn == ENABLE))
    {
        BufInfo->IPEOut.Fmt = IPL_CtrlGetInfor(Id, IPLCTRL_IPE_OUT_FMT);
        BufInfo->IPEOut.Width = IPL_CtrlGetInfor(Id, IPLCTRL_IPE_OUT_SIZE_H);
        BufInfo->IPEOut.LineOfs = IPL_CtrlGetInfor(Id, IPLCTRL_IPE_OUT_SIZE_YLOS);
        BufInfo->IPEOut.Height = IPL_CtrlGetInfor(Id, IPLCTRL_IPE_OUT_SIZE_V);

        BufInfo->IPEOut.BufNum = IPL_CtrlGetInfor(Id, IPLCTRL_IPE_OUT_BUFNUM);
        BufInfo->IFE2LCABufNum = IPL_CtrlGetInfor(Id, IPLCTRL_IFE2_OUT_BUFNUM);
    }

    BufInfo->IMEPath1En = ENABLE;
    BufInfo->IMEPath1.BufNum = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_BUFNUM);
    BufInfo->IMEPath1.Width = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_BUF_SIZE_H);
    BufInfo->IMEPath1.Height = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_BUF_SIZE_V);
    BufInfo->IMEPath1.LineOfs = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_BUF_SIZE_H);
    BufInfo->IMEPath1.Fmt = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_BUFFMT);

    BufInfo->IMEPath2En = ENABLE;
    BufInfo->IMEPath2.BufNum = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_BUFNUM);
    BufInfo->IMEPath2.Width = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_BUF_SIZE_H);
    BufInfo->IMEPath2.Height = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_BUF_SIZE_V);
    BufInfo->IMEPath2.LineOfs = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_BUF_SIZE_H);
    BufInfo->IMEPath2.Fmt = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_BUFFMT);

    BufInfo->IMEPath3En = ENABLE;
    BufInfo->IMEPath3.BufNum = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_BUFNUM);
    BufInfo->IMEPath3.Width = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_BUF_SIZE_H);
    BufInfo->IMEPath3.Height = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_BUF_SIZE_V);
    BufInfo->IMEPath3.LineOfs = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_BUF_SIZE_H);
    BufInfo->IMEPath3.Fmt = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_BUFFMT);

    BufInfo->IMEPath4En = ENABLE;
    BufInfo->IMEPath4.BufNum = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P4_BUFNUM);
    BufInfo->IMEPath4.Width = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P4_BUF_SIZE_H);
    BufInfo->IMEPath4.Height = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P4_BUF_SIZE_V);
    BufInfo->IMEPath4.LineOfs = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P4_BUF_SIZE_H);
}

#if 0
#endif

void IPL_CtrlPrvInitFlowD(UINT32 Id, IPLCTRL_INFO *CtrlInfo)
{
    UINT32 Width = 0, Height = 0;
    IPC_QINFO IpcInfo = {0};
    AWB_CAINFO CAInfo = {0};
    AE_LAINFO LAInfo = {0};
    IPL_IME_PATH_INFO AdasP4Info = {0};

    IpcInfo = IPL_CTRLQuery(Id, ICF_FLOWDEWARP);

    IPL_CtrlSetInfor(Id, IPLCTRL_CFMODEEXT, IPL_CFEXT_NORMAL);

    IPL_CtrlSetInfor(Id, IPLCTRL_PRVAUTO_OB_EN, DISABLE);

    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_PRVSTPIX, IPL_UTI_CONV_SEN_STPIX(CtrlInfo->Info->Mode->StPix));

    IPL_DzoomGetSIECropSize(Id, IPL_DzoomGetIndex(Id), FALSE, &Width, &Height);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_PRVCROP_SIZE_H, Width);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_PRVCROP_SIZE_V, Height);

    //ICF EXT FUNC. ON/OFF
    if((CtrlInfo->ChgMode->FuncEn&IPL_FUNC_WDR)&&(CtrlInfo->ChgMode->FuncEn&IPL_FUNC_SHDR))
    {
        DBG_ERR("Id: %d, Not Support Enable both IPL_FUNC_WDR and IPL_FUNC_SHDR!\r\n", Id);
    }

    if ((CtrlInfo->Info->Mode->ModeType == SENSOR_MODE_STAGGER_HDR) && (CtrlInfo->ChgMode->FuncEn & IPL_FUNC_SHDR))
    {
        DBG_ERR("Id: %d, Flow_D Not Support HDR Flow!\r\n", Id);
    }
    else if (CtrlInfo->ChgMode->FuncEn & IPL_FUNC_WDR)
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_EXT_FUNC_EN, ICF_FUNC_WDR);
        AE_SetAppMode(AE_APPMODE_NORMAL);

        if (CtrlInfo->ChgMode->Mode == IPL_MODE_INIT)
            ExtFunc_Init(Id, NULL, NULL);
    }
    else if (CtrlInfo->ChgMode->FuncEn & IPL_FUNC_STITCH)
    {
        DBG_ERR("Id: %d, Flow_D Not Support Image Stitch Flow!\r\n", Id);
    }
    else
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_EXT_FUNC_EN, ICF_FUNC_NONE);
        AE_SetAppMode(AE_APPMODE_NORMAL);
    }

    //SIE ch0
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_RAW_EN, ENABLE);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_RAW_BUFNUM, IpcInfo.QUE_FD.nBUF_SIEPPB);
    IPL_DzoomGetSIEOutSize(Id, IPL_DzoomGetIndex(Id), &Width, &Height);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_H, Width);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_V, Height);

    if (Width > IpcInfo.QUE_FD.StripMinSize)
    {
        DBG_ERR("DeWarp Flow N.S. 2 Stripe mdoe, Force to use IPL_CF_NORMAL\r\n");
    }
    IPL_CtrlSetInfor(Id, IPLCTRL_CFMODE, IPL_CF_NORMAL);

    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_RAWENC_EN, DISABLE);

    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_PRVBIT, IPL_UTI_CONV_ALGINFO_RAW_BIT(IPL_AlgGetUIInfo(Id, IPL_SEL_RAW_BITDEPTH_PRV)));

    if (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAWENC_EN) == ENABLE)
        IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_HLOS, IPL_ALIGN_ROUNDUP(IPL_UTI_RAWH_CONV2_LOFS(Width, IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_PRVBIT))*RAW_ENC_BUF_RATIO/100, 4));
    else
        IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_HLOS, IPL_UTI_RAWH_CONV2_LOFS(Width, IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_PRVBIT)));

    //SIE ch1   //for DVI Y Out
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH1_Y_EN, DISABLE);
    //SIE ch1 CA Out
    AWB_GetCAInfo(Id, &CAInfo);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CA_EN, ENABLE);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CA_WINXNUM, CAInfo.WinNumX);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CA_WINYNUM, CAInfo.WinNumY);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH1_BUFNUM, IpcInfo.QUE_FD.nBUF_CAPPB);
    //SIE ch2   //for DVI UV Out
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH2_UV_EN, DISABLE);
    //SIE ch2 LA Out

    AE_GetLAInfo(Id, &LAInfo);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_LA_EN, ENABLE);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_LA_WINXNUM, LAInfo.WinNumX);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_LA_WINYNUM, LAInfo.WinNumY);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH2_BUFNUM, IpcInfo.QUE_FD.nBUF_LAPPB);
    //SIE ch3
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH3_RAW_EN, DISABLE);
    //SIE ch3 VA Out
    if (CtrlInfo->ChgMode->FuncEn & IPL_FUNC_VA)
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_SIE_VA_EN, ENABLE);
        IPL_CtrlSetInfor(Id, IPLCTRL_SIE_VA_BUFNUM, IpcInfo.QUE_FD.nBUF_SIE_VAPPB);
    }
    else
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_SIE_VA_EN, DISABLE);
        IPL_CtrlSetInfor(Id, IPLCTRL_SIE_VA_BUFNUM, 0);
    }

    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_ECS_EN, DISABLE);

    //SIE ch4 ETH Out
    if (CtrlInfo->ChgMode->FuncEn & IPL_FUNC_DIS)
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_SIE_ETH_EN, ENABLE);
        IPL_CtrlSetInfor(Id, IPLCTRL_SIE_ETH_SIZE_H, (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_H)*2/8));
        IPL_CtrlSetInfor(Id, IPLCTRL_SIE_ETH_SIZE_HLOS, IPL_ALIGN_ROUNDUP(IPL_CtrlGetInfor(Id, IPLCTRL_SIE_ETH_SIZE_H), 4));
        IPL_CtrlSetInfor(Id, IPLCTRL_SIE_ETH_SIZE_V, IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_V));

        if (IPL_AlgGetUIInfo(Id, IPL_SEL_DISCOMP) == SEL_DISCOMP_ON)
        {
            IPL_CtrlSetInfor(Id, IPLCTRL_DIS_EN, ENABLE);
        }
        else
        {
            IPL_CtrlSetInfor(Id, IPLCTRL_DIS_EN, DISABLE);
        }
        IPL_CtrlSetInfor(Id, IPLCTRL_DIS_RATIO, dis_getDisViewRatio());
    }
    else
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_SIE_ETH_EN, DISABLE);
        IPL_CtrlSetInfor(Id, IPLCTRL_DIS_RATIO, IMG_EISDIS_RATIO_UINT);
    }

    //#NT#2016/05/24#Jarkko Chang -begin
    //#NT# all direct flow only support wdr2.0
    if (IPL_CtrlGetInfor(Id, IPLCTRL_CFMODE) == IPL_CF_RHEWDR)
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_WDR_VER, WDR_VERSION_2_5);
    }
    else
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_WDR_VER, WDR_VERSION_2_0);
    }
    //#NT#2016/05/24#Jarkko Chang -end

    //Set IPL IN for RDE/IFE IN
    IPL_DzoomGetRaw2YUVInSize(Id, IPL_DzoomGetIndex(Id), &Width, &Height);
    IPL_CtrlSetInfor(Id, IPLCTRL_IPL_IN_BIT, IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_PRVBIT));

    IPL_CtrlSetInfor(Id, IPLCTRL_IPL_IN_SIZE_H, Width);
    IPL_CtrlSetInfor(Id, IPLCTRL_IPL_IN_SIZE_V, Height);

    IPL_CtrlSetInfor(Id, IPLCTRL_IPL_IN_SIZE_HLOS,IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_HLOS));

    //////////////////////////////////////////

    if (IQS_IsChromaAdaptEn(Id, FALSE))
    {
        Width  = IPL_UTI_IPESUB_ADJ_HSIZE(IPL_CtrlGetInfor(Id, IPLCTRL_IPL_IN_SIZE_H), IQS_GetChromaAdaptRatio(Id, FALSE, TRUE), 4);
        Height = IPL_UTI_IPESUB_ADJ_VSIZE(IPL_CtrlGetInfor(Id, IPLCTRL_IPL_IN_SIZE_V), IQS_GetChromaAdaptRatio(Id, FALSE, TRUE), 4);

        //IPE OUT - TO DMA
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_OUT_EN, ENABLE);
        //IPE OUT - TO IFE2
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_IFE2_EN, DISABLE);
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_OUT_BUFNUM, IpcInfo.QUE_FD.nBUF_IPE_LCAPPB);

        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_OUT_FMT, IPL_IMG_PACK_YUV444);
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_OUT_SIZE_H, Width);
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_OUT_SIZE_YLOS, IPL_YSIZEH_CONV2LOFS(IPL_CtrlGetInfor(Id, IPLCTRL_IPE_OUT_FMT), IPL_CtrlGetInfor(Id, IPLCTRL_IPE_OUT_SIZE_H)));
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_OUT_SIZE_V, Height);

        //IFE2 OUT
        IPL_CtrlSetInfor(Id, IPLCTRL_IFE2_LCA_EN, ENABLE);
        IPL_CtrlSetInfor(Id, IPLCTRL_IFE2_OUT_BUFNUM, IpcInfo.QUE_FD.nBUF_IFE2PPB);
    }
    else
    {
        //IPE OUT - TO DMA
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_OUT_EN, DISABLE);
        //IPE OUT - TO IFE2
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_IFE2_EN, DISABLE);
        //IFE2 OUT
        IPL_CtrlSetInfor(Id, IPLCTRL_IFE2_LCA_EN, DISABLE);
    }


    //IME P1
    IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_EN, CtrlInfo->ChgMode->ImePath1.ImgEn);

    if (IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_EN) == DISABLE)
        IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_D2H264, FALSE);
    else
        IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_D2H264, CtrlInfo->ChgMode->ImePath1.Direct2H264);

    //IME P2
    IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_EN, CtrlInfo->ChgMode->ImePath2.ImgEn);
    //IME P3
    IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_EN, CtrlInfo->ChgMode->ImePath3.ImgEn);

    IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_BUF_SIZE_H, CtrlInfo->ChgMode->ImePath1.BufSizeH);
    IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_BUF_SIZE_V, CtrlInfo->ChgMode->ImePath1.BufSizeV);

    IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_BUF_SIZE_H, CtrlInfo->ChgMode->ImePath2.BufSizeH);
    IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_BUF_SIZE_V, CtrlInfo->ChgMode->ImePath2.BufSizeV);

    IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_BUF_SIZE_H, CtrlInfo->ChgMode->ImePath3.BufSizeH);
    IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_BUF_SIZE_V, CtrlInfo->ChgMode->ImePath3.BufSizeV);

    IPL_CtrlSetInfor(Id, IPLCTRL_IME_P4_BUF_SIZE_H, CtrlInfo->ChgMode->ImePath4.BufSizeH);
    IPL_CtrlSetInfor(Id, IPLCTRL_IME_P4_BUF_SIZE_V, CtrlInfo->ChgMode->ImePath4.BufSizeV);

    if (CtrlInfo->ChgMode->FuncEn & IPL_FUNC_ADAS)
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_IME_P4_EXTOUT_EN, ENABLE);
    }
    else
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_IME_P4_EXTOUT_EN, DISABLE);
    }

    if (CtrlInfo->ChgMode->Mode != IPL_MODE_INIT)
    {
        if (IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_EN) == ENABLE)
        {
            if (((IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_BUF_SIZE_H) * IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_BUF_SIZE_V)) < ((CtrlInfo->ChgMode->ImePath1.CropStartX + CtrlInfo->ChgMode->ImePath1.CropSizeH) * (CtrlInfo->ChgMode->ImePath1.CropStartY + CtrlInfo->ChgMode->ImePath1.CropSizeV))) &&
                IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_D2H264) == FALSE)
            {
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_OUT_SIZE_H, CtrlInfo->ChgMode->ImePath1.BufSizeH);
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_OUT_SIZE_HLOS, CtrlInfo->ChgMode->ImePath1.BufSizeH);
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_OUT_SIZE_V, CtrlInfo->ChgMode->ImePath1.BufSizeV);

                DBG_ERR("Id: %d IME Path_1: buffer size(%d %d) < image output size, crop start(%d %d),crop size(%d, %d)\r\n", Id, CtrlInfo->ChgMode->ImePath1.BufSizeH,
                        CtrlInfo->ChgMode->ImePath1.BufSizeV, CtrlInfo->ChgMode->ImePath1.CropStartX, CtrlInfo->ChgMode->ImePath1.CropStartY, CtrlInfo->ChgMode->ImePath1.CropSizeH, CtrlInfo->ChgMode->ImePath1.CropSizeV);
            }
            else
            {
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_OUT_SIZE_H, CtrlInfo->ChgMode->ImePath1.ImgSizeH);
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_OUT_SIZE_HLOS, CtrlInfo->ChgMode->ImePath1.ImgSizeLOfs);
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_OUT_SIZE_V, CtrlInfo->ChgMode->ImePath1.ImgSizeV);
            }

            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_OUT_CROP_STR_X, CtrlInfo->ChgMode->ImePath1.CropStartX);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_OUT_CROP_STR_Y, CtrlInfo->ChgMode->ImePath1.CropStartY);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_OUT_CROP_H, CtrlInfo->ChgMode->ImePath1.CropSizeH);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_OUT_CROP_V, CtrlInfo->ChgMode->ImePath1.CropSizeV);
        }
        else
        {
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_D2H264, FALSE);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_OUT_SIZE_H, CtrlInfo->ChgMode->ImePath1.BufSizeH);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_OUT_SIZE_HLOS, CtrlInfo->ChgMode->ImePath1.BufSizeH);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_OUT_SIZE_V, CtrlInfo->ChgMode->ImePath1.BufSizeV);
        }

        //IME P2
        if (IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_EN) == ENABLE)
        {
            if ((IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_BUF_SIZE_H) * IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_BUF_SIZE_V)) < ((CtrlInfo->ChgMode->ImePath2.CropStartX + CtrlInfo->ChgMode->ImePath2.CropSizeH) * (CtrlInfo->ChgMode->ImePath2.CropStartY + CtrlInfo->ChgMode->ImePath2.CropSizeV)))
            {
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_OUT_SIZE_H, CtrlInfo->ChgMode->ImePath2.BufSizeH);
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_OUT_SIZE_HLOS, CtrlInfo->ChgMode->ImePath2.BufSizeH);
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_OUT_SIZE_V, CtrlInfo->ChgMode->ImePath2.BufSizeV);

                DBG_ERR("Id: %d IME Path_2: buffer size(%d %d) < image output size, crop start(%d %d),crop size(%d, %d)\r\n", Id, CtrlInfo->ChgMode->ImePath2.BufSizeH,
                        CtrlInfo->ChgMode->ImePath2.BufSizeV, CtrlInfo->ChgMode->ImePath2.CropStartX, CtrlInfo->ChgMode->ImePath2.CropStartY, CtrlInfo->ChgMode->ImePath2.CropSizeH, CtrlInfo->ChgMode->ImePath2.CropSizeV);
            }
            else
            {
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_OUT_SIZE_H, CtrlInfo->ChgMode->ImePath2.ImgSizeH);
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_OUT_SIZE_HLOS, CtrlInfo->ChgMode->ImePath2.ImgSizeLOfs);
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_OUT_SIZE_V, CtrlInfo->ChgMode->ImePath2.ImgSizeV);
            }

            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_OUT_CROP_STR_X, CtrlInfo->ChgMode->ImePath2.CropStartX);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_OUT_CROP_STR_Y, CtrlInfo->ChgMode->ImePath2.CropStartY);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_OUT_CROP_H, CtrlInfo->ChgMode->ImePath2.CropSizeH);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_OUT_CROP_V, CtrlInfo->ChgMode->ImePath2.CropSizeV);
        }
        else
        {
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_OUT_SIZE_H, CtrlInfo->ChgMode->ImePath2.BufSizeH);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_OUT_SIZE_HLOS, CtrlInfo->ChgMode->ImePath2.BufSizeH);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_OUT_SIZE_V, CtrlInfo->ChgMode->ImePath2.BufSizeV);
        }

        //IME P3
        if (IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_EN) == ENABLE)
        {
            if ((IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_BUF_SIZE_H) * IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_BUF_SIZE_V)) < ((CtrlInfo->ChgMode->ImePath3.CropStartX + CtrlInfo->ChgMode->ImePath3.CropSizeH) * (CtrlInfo->ChgMode->ImePath3.CropStartY + CtrlInfo->ChgMode->ImePath3.CropSizeV)))
            {
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_OUT_SIZE_H, CtrlInfo->ChgMode->ImePath3.BufSizeH);
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_OUT_SIZE_HLOS, CtrlInfo->ChgMode->ImePath3.BufSizeH);
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_OUT_SIZE_V, CtrlInfo->ChgMode->ImePath3.BufSizeV);

                DBG_ERR("Id: %d IME Path_3: buffer size(%d %d) < image output size, crop start(%d %d),crop size(%d, %d)\r\n", Id, CtrlInfo->ChgMode->ImePath3.BufSizeH,
                        CtrlInfo->ChgMode->ImePath3.BufSizeV, CtrlInfo->ChgMode->ImePath3.CropStartX, CtrlInfo->ChgMode->ImePath3.CropStartY, CtrlInfo->ChgMode->ImePath3.CropSizeH, CtrlInfo->ChgMode->ImePath3.CropSizeV);
            }
            else
            {
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_OUT_SIZE_H, CtrlInfo->ChgMode->ImePath3.ImgSizeH);
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_OUT_SIZE_HLOS, CtrlInfo->ChgMode->ImePath3.ImgSizeLOfs);
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_OUT_SIZE_V, CtrlInfo->ChgMode->ImePath3.ImgSizeV);
            }

            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_OUT_CROP_STR_X, CtrlInfo->ChgMode->ImePath3.CropStartX);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_OUT_CROP_STR_Y, CtrlInfo->ChgMode->ImePath3.CropStartY);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_OUT_CROP_H, CtrlInfo->ChgMode->ImePath3.CropSizeH);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_OUT_CROP_V, CtrlInfo->ChgMode->ImePath3.CropSizeV);
        }
        else
        {
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_OUT_SIZE_H, CtrlInfo->ChgMode->ImePath3.BufSizeH);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_OUT_SIZE_HLOS, CtrlInfo->ChgMode->ImePath3.BufSizeH);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_OUT_SIZE_V, CtrlInfo->ChgMode->ImePath3.BufSizeV);
        }

        //#NT#2016/03/24#Jarkko Chang -begin
        //#NT# add support new adas lib
        if (Id == IPL_ID_1 && CtrlInfo->ChgMode->ImePath4.ImgEn == DISABLE)
        {
            AdasP4Info = ADAS_getPath4Info();
            CtrlInfo->ChgMode->ImePath4.ImgEn = AdasP4Info.OutputEn;
            CtrlInfo->ChgMode->ImePath4.ImgSizeH = AdasP4Info.ImgSizeH;
            CtrlInfo->ChgMode->ImePath4.ImgSizeV = AdasP4Info.ImgSizeV;
            CtrlInfo->ChgMode->ImePath4.ImgSizeLOfs = AdasP4Info.ImgSizeLOfs;
            CtrlInfo->ChgMode->ImePath4.CropStartX= AdasP4Info.CropStartX;
            CtrlInfo->ChgMode->ImePath4.CropStartY = AdasP4Info.CropStartY;
            CtrlInfo->ChgMode->ImePath4.CropSizeH = AdasP4Info.CropSizeH;
            CtrlInfo->ChgMode->ImePath4.CropSizeV= AdasP4Info.CropSizeV;
            CtrlInfo->ChgMode->ImePath4.ImgFmt = AdasP4Info.ImgFmt;
        }
        //#NT#2016/03/24#Jarkko Chang -end

        IPL_CtrlSetInfor(Id, IPLCTRL_IME_P4_EN, CtrlInfo->ChgMode->ImePath4.ImgEn);
        //IME P4
        if (IPL_CtrlGetInfor(Id, IPLCTRL_IME_P4_EN) == ENABLE)
        {
            if ((IPL_CtrlGetInfor(Id, IPLCTRL_IME_P4_BUF_SIZE_H) * IPL_CtrlGetInfor(Id, IPLCTRL_IME_P4_BUF_SIZE_V)) < ((CtrlInfo->ChgMode->ImePath4.CropStartX + CtrlInfo->ChgMode->ImePath4.CropSizeH) * (CtrlInfo->ChgMode->ImePath4.CropStartY + CtrlInfo->ChgMode->ImePath4.CropSizeV)))
            {
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P4_OUT_SIZE_H, CtrlInfo->ChgMode->ImePath4.BufSizeH);
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P4_OUT_SIZE_HLOS, CtrlInfo->ChgMode->ImePath4.BufSizeH);
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P4_OUT_SIZE_V, CtrlInfo->ChgMode->ImePath4.BufSizeV);

                DBG_ERR("Id: %d IME Path_4: buffer size(%d %d) < image output size, crop start(%d %d),crop size(%d, %d)\r\n", Id, CtrlInfo->ChgMode->ImePath4.BufSizeH,
                        CtrlInfo->ChgMode->ImePath4.BufSizeV, CtrlInfo->ChgMode->ImePath4.CropStartX, CtrlInfo->ChgMode->ImePath4.CropStartY, CtrlInfo->ChgMode->ImePath4.CropSizeH, CtrlInfo->ChgMode->ImePath4.CropSizeV);                    }
            else
            {
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P4_OUT_SIZE_H, CtrlInfo->ChgMode->ImePath4.ImgSizeH);
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P4_OUT_SIZE_HLOS, CtrlInfo->ChgMode->ImePath4.ImgSizeLOfs);
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P4_OUT_SIZE_V, CtrlInfo->ChgMode->ImePath4.ImgSizeV);
            }

            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P4_OUT_CROP_STR_X, CtrlInfo->ChgMode->ImePath4.CropStartX);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P4_OUT_CROP_STR_Y, CtrlInfo->ChgMode->ImePath4.CropStartY);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P4_OUT_CROP_H, CtrlInfo->ChgMode->ImePath4.CropSizeH);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P4_OUT_CROP_V, CtrlInfo->ChgMode->ImePath4.CropSizeV);

        }
        else
        {
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P4_OUT_SIZE_H, CtrlInfo->ChgMode->ImePath4.BufSizeH);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P4_OUT_SIZE_HLOS, CtrlInfo->ChgMode->ImePath4.BufSizeH);
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P4_OUT_SIZE_V, CtrlInfo->ChgMode->ImePath4.BufSizeV);
        }
    }

    //IME P1
    IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_FMT, CtrlInfo->ChgMode->ImePath1.ImgFmt);
    IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_BUFFMT, CtrlInfo->ChgMode->ImePath1.BufFmt);
    IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_BUFNUM, IPL_GetDisplayCtrlBufferNum(Id));

    //IME P2
    IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_FMT, CtrlInfo->ChgMode->ImePath2.ImgFmt);
    IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_BUFFMT, CtrlInfo->ChgMode->ImePath2.BufFmt);
    IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_BUFNUM, IPL_GetDisplayCtrlBufferNum(Id));

    //IME P3
    IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_FMT, CtrlInfo->ChgMode->ImePath3.ImgFmt);
    IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_BUFFMT, CtrlInfo->ChgMode->ImePath3.BufFmt);
    IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_BUFNUM, IPL_GetDisplayCtrlBufferNum(Id));

    //IME P4
    IPL_CtrlSetInfor(Id, IPLCTRL_IME_P4_FMT, CtrlInfo->ChgMode->ImePath4.ImgFmt);    //IME Path4 only support Y output
    IPL_CtrlSetInfor(Id, IPLCTRL_IME_P4_BUFFMT, CtrlInfo->ChgMode->ImePath4.BufFmt);
    IPL_CtrlSetInfor(Id, IPLCTRL_IME_P4_BUFNUM, IPL_GetDisplayCtrlBufferNum(Id));

    //#NT#2016/07/20#Jarkko Chang -begin
    //#NT# add support vr360 flow
    if (CtrlInfo->ChgMode->FuncEn & IPL_FUNC_VR360)
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_VR360_EN, ENABLE);
        IPL_CtrlSetInfor(Id, IPLCTRL_VR360_ORI_L, CtrlInfo->ChgMode->VR360Info.ori[0]);
        IPL_CtrlSetInfor(Id, IPLCTRL_VR360_ORI_R, CtrlInfo->ChgMode->VR360Info.ori[1]);
        IPL_CtrlSetInfor(Id, IPLCTRL_VR360_IMG_HEI_ADJ, CtrlInfo->ChgMode->VR360Info.ImgHeightAdj);
        IPL_CtrlSetInfor(Id, IPLCTRL_VR360_IME_P1_START_OFS, CtrlInfo->ChgMode->VR360Info.IME_P1_Start_Offset);
        IPL_CtrlSetInfor(Id, IPLCTRL_VR360_IME_P2_START_OFS, CtrlInfo->ChgMode->VR360Info.IME_P2_Start_Offset);
        IPL_CtrlSetInfor(Id, IPLCTRL_VR360_IME_P3_START_OFS, CtrlInfo->ChgMode->VR360Info.IME_P3_Start_Offset);
    }
    else
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_VR360_EN, DISABLE);
        IPL_CtrlSetInfor(Id, IPLCTRL_VR360_ORI_L, 0);
        IPL_CtrlSetInfor(Id, IPLCTRL_VR360_ORI_R, 0);
        IPL_CtrlSetInfor(Id, IPLCTRL_VR360_IMG_HEI_ADJ, 0);
        IPL_CtrlSetInfor(Id, IPLCTRL_VR360_IME_P1_START_OFS, 0);
        IPL_CtrlSetInfor(Id, IPLCTRL_VR360_IME_P2_START_OFS, 0);
        IPL_CtrlSetInfor(Id, IPLCTRL_VR360_IME_P3_START_OFS, 0);
    }
    //#NT#2016/07/20#Jarkko Chang -end

    //Double check SIE CH1/CH2 Output Select
    if (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CA_EN) == ENABLE && IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH1_Y_EN) == ENABLE)
    {
        DBG_ERR("Id: %d Error for Both SIE_CA and SIE_CH1 are ENABLE, Force DISABLE SIE_CA\r\n", Id);
        IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CA_EN, DISABLE);
    }

    if (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_LA_EN) == ENABLE && IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH2_UV_EN) == ENABLE)
    {
        DBG_ERR("Id: %d Error for Both SIE_LA and SIE_CH2 are ENABLE, Force DISABLE SIE_LA\r\n", Id);
        IPL_CtrlSetInfor(Id, IPLCTRL_SIE_LA_EN, DISABLE);
    }
}
