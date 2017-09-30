/**
    Ctrl sample normal.

    @file       IPL_CtrlNormal_IMX322LQJ_TVP5150_EVB_FF.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include "IPL_IMX322LQJ_TVP5150_EVB_FF_Int.h"

UINT32 IPL_IFE2InitCBFlowA(UINT32 Id,ENG_SUBFUNC *Info,IPL_HAL_GROUP* group);
UINT32 IPL_IFE2EngCBFlowA(UINT32 Id,ENG_SUBFUNC *Info,IPL_HAL_GROUP* group);
IPH_CLK IPL_SIEClkCBFlowA(UINT32 Id);
UINT32 IPL_SIEClkSrcCBFlowA(UINT32 Id);
UINT32 IPL_SIEPclkSelCBFlowA(UINT32 Id);
IPH_CLK IPL_IFEClkCBFlowA(UINT32 Id);
IPH_CLK IPL_DCEClkCBFlowA(UINT32 Id);
IPH_CLK IPL_IPEClkCBFlowA(UINT32 Id);
IPH_CLK IPL_IFE2ClkCBFlowA(UINT32 Id);
IPH_CLK IPL_IMEClkCBFlowA(UINT32 Id);
UINT32 IPL_SIEInitCBFlowA(UINT32 Id,ENG_SUBFUNC *Info,IPL_HAL_GROUP* group);
UINT32 IPL_IFEInitCBFlowA(UINT32 Id,ENG_SUBFUNC *Info,IPL_HAL_GROUP* group);
UINT32 IPL_DCEInitCBFlowA(UINT32 Id,ENG_SUBFUNC *Info,IPL_HAL_GROUP* group);
UINT32 IPL_IPEInitCBFlowA(UINT32 Id,ENG_SUBFUNC *Info,IPL_HAL_GROUP* group);
UINT32 IPL_IMEInitCBFlowA(UINT32 Id,ENG_SUBFUNC *Info,IPL_HAL_GROUP* group);


IPLCTRL_FLOWA_INFO IPL_FlowAInfo =
{
    {IPL_HAL_PAR0, IPL_HAL_PAR0, IPL_HAL_PAR0, IPL_HAL_PAR0},

    IPL_IFE2InitCBFlowA,
    IPL_SIEClkCBFlowA,
    IPL_SIEClkSrcCBFlowA,
    IPL_SIEPclkSelCBFlowA,
    IPL_IFEClkCBFlowA,
    IPL_DCEClkCBFlowA,
    IPL_IPEClkCBFlowA,
    IPL_IFE2ClkCBFlowA,
    IPL_IMEClkCBFlowA,

    IPL_SIEInitCBFlowA,
    IPL_IFEInitCBFlowA,
    IPL_DCEInitCBFlowA,
    IPL_IPEInitCBFlowA,
    IPL_IMEInitCBFlowA
};

static IPLCTRL_FLOWA_INFO* IPL_GetCtrlFlowAInfo(void)
{
    return &IPL_FlowAInfo;
}

IPL_HAL_PARASET IPL_CtrlGetHalSetIdFlowA(UINT32 Id)
{
    return IPL_GetCtrlFlowAInfo()->IPL_HalParamId[Id];
}

UINT32 IPL_IFE2InitCBFlowA(UINT32 Id,ENG_SUBFUNC *Info,IPL_HAL_GROUP* group)
{
    return 1;
}
UINT32 IPL_IFE2EngCBFlowA(UINT32 Id,ENG_SUBFUNC *Info,IPL_HAL_GROUP* group)
{
    return 1;
}

#if 0
#endif
IPH_CLK IPL_SIEClkCBFlowA(UINT32 Id)
{
    IPH_CLK clk = {0};
    SENSOR_INFO SenInfo = {0};
    ER rt = E_OK;

    rt = IPL_GetSensorStatus(Id, IPL_AlgGetUIInfo(Id, IPL_SEL_PRVSENMODE), &SenInfo);
    if (rt != E_OK)
        return clk;

    if (SenInfo.DataType == SENSOR_DATA_LVDS)
    {
        clk.SIEclk = 0;
    }
    else if (SenInfo.DataType == SENSOR_DATA_DUMMY)
    {
        clk.SIEclk = 27000000;
    }
    else
    {
        //set sie clk for parallel sensor
        //clk.SIEclk = 0;
    }
    return clk;
}

UINT32 IPL_SIEClkSrcCBFlowA(UINT32 Id)
{
    return IPH_SIECLKSRC_CURR;
}

UINT32 IPL_SIEPclkSelCBFlowA(UINT32 Id)
{
    return IPH_SIEPXCLK_PAD;
}

IPH_CLK IPL_IFEClkCBFlowA(UINT32 Id)
{
    IPH_CLK clk;
    clk.IFEclk = IPH_IFECLK1;
    return clk;
}

IPH_CLK IPL_DCEClkCBFlowA(UINT32 Id)
{
    IPH_CLK clk;
    clk.DCEclk = IPH_DCECLK1;
    return clk;
}

IPH_CLK IPL_IPEClkCBFlowA(UINT32 Id)
{
    IPH_CLK clk;
    clk.IPEclk = IPH_IPECLK1;
    return clk;
}

IPH_CLK IPL_IFE2ClkCBFlowA(UINT32 Id)
{
    IPH_CLK clk;
    clk.IFE2clk = IPH_IFE2CLK1;
    return clk;
}

IPH_CLK IPL_IMEClkCBFlowA(UINT32 Id)
{
    IPH_CLK clk;
    clk.IMEclk = IPH_IMECLK1;
    return clk;
}
#if 0
#endif

UINT32 IPL_SIEInitCBFlowA(UINT32 Id,ENG_SUBFUNC *Info,IPL_HAL_GROUP* group)
{
    SIE_UPDATE *Update;

    //init info structure
    Update = (SIE_UPDATE *)group;

    IPL_SIESetOB_FCB(Id, &Info->pSie_sub->sieFuncEn, &Info->pSie_sub->InteEn, Update, &Info->pSie_sub->OBAvg, &Info->pSie_sub->OBSub);
    IPL_SIESetCA_FCB(Id, &Info->pSie_sub->sieFuncEn, &Info->pSie_sub->InteEn, Update, &Info->pSie_sub->Ca, &Info->pSie_sub->pStatPathInfo);
    IPL_SIESetLA_FCB(Id, &Info->pSie_sub->sieFuncEn, &Info->pSie_sub->InteEn, Update, &Info->pSie_sub->La, &Info->pSie_sub->pStatPathInfo);
    IPL_SIESetBP_FCB(Id, &Info->pSie_sub->sieFuncEn, &Info->pSie_sub->InteEn, Update, Info->pSie_sub);
    IPL_SIESetDMAOutSrc_FCB(Id, &Info->pSie_sub->sieFuncEn, &Info->pSie_sub->InteEn, Update);
    IPL_SIESetEthOut_FCB(Id,&Info->pSie_sub->sieFuncEn,&Info->pSie_sub->InteEn,Update,Info->pSie_sub);
    IPL_SIESetRawEnc_FCB(Id, Update, &Info->pSie_sub->sieFuncEn, &Info->pSie_sub->RawEnc);
    *Update |= SIE_SUBFEN;

    IQCB_SettingFlowFunc(Id, IQCB_PRV_SIE_INIT, Info, group);
    return 1;
}

UINT32 IPL_IFEInitCBFlowA(UINT32 Id,ENG_SUBFUNC *Info,IPL_HAL_GROUP* group)
{
    IQCB_SettingFlowFunc(Id, IQCB_PRV_IFE_INIT, Info, group);
    return 1;
}

UINT32 IPL_DCEInitCBFlowA(UINT32 Id,ENG_SUBFUNC *Info,IPL_HAL_GROUP* group)
{
    IQCB_SettingFlowFunc(Id, IQCB_PRV_DCE_INIT, Info, group);
    return 1;
}

UINT32 IPL_IPEInitCBFlowA(UINT32 Id,ENG_SUBFUNC *Info,IPL_HAL_GROUP* group)
{
    IPE_UPDATE *Update;

    //init info structure
    Update = (IPE_UPDATE *)group;

    IQCB_SettingFlowFunc(Id, IQCB_PRV_IPE_INIT, Info, group);
    return 1;
}

UINT32 IPL_IMEInitCBFlowA(UINT32 Id,ENG_SUBFUNC *Info,IPL_HAL_GROUP* group)
{
    IQCB_SettingFlowFunc(Id, IQCB_PRV_IME_INIT, Info, group);

    return 1;
}

#if 0
#endif
void IPL_CtrlInitIpcInfoFlowA(UINT32 Id, IPC_INFO *IpcInfo)
{
    SENSOR_INFO SenInfo = {0};
    IPLCTRL_FLOWA_INFO *FlowInfo;
//    IPL_IME_PATH_INFO AdasP4Info;
    ER rt = E_OK;

    FlowInfo = IPL_GetCtrlFlowAInfo();
    IpcInfo->ICF_FA.ParaIdx = FlowInfo->IPL_HalParamId[Id];
    IpcInfo->ICF_FA.BaseInfo.IPLCFMode = IPL_CtrlGetInfor(Id, IPLCTRL_CFMODE);
    IpcInfo->ICF_FA.BaseInfo.IPLCFModeExt = IPL_CtrlGetInfor(Id, IPLCTRL_CFMODEEXT);
    //#NT#2016/02/18#Jarkko Chang -begin
    //#NT# add support dsp flip
    IpcInfo->ICF_FA.BaseInfo.FlipInfo = IPL_CtrlGetInfor(Id, IPLCTRL_FLIP_TYPE);
    //#NT#2016/02/18#Jarkko Chang -end

    rt = IPL_GetSensorStatus(Id, IPL_AlgGetUIInfo(Id, IPL_SEL_PRVSENMODE), &SenInfo);
    if (rt != E_OK)
        return;

    IPL_CtrlInitIpc_SIEBurstLength(Id, &SenInfo, &IpcInfo->ICF_FA.BaseInfo.BurstLength);
    IPL_CtrlInitIpc_SIEAct(Id, &SenInfo, &IpcInfo->ICF_FA.BaseInfo.SIE_Act);
    IPL_CtrlInitIpc_SIEIn(Id, &SenInfo, &IpcInfo->ICF_FA.BaseInfo.SIE_IN);
    IPL_CtrlInitIpc_SIESignal(Id, FALSE, FALSE, _RISING, _RISING, _RISING, &IpcInfo->ICF_FA.BaseInfo.SenSorIn);

    //All direct Flow: Set SIE_CH0 Output Parameters but DISABLE Output channel
    IPL_CtrlInitIpc_SIECh0PPB(Id, &IpcInfo->ICF_FA.BaseInfo.SIE_PPB0, &IpcInfo->ICF_FA.BaseInfo.SIE_EXT_PPB[0], &IpcInfo->ICF_FA.BaseInfo.SIE_PPB_OFS);
    if ((IpcInfo->ICF_FA.BaseInfo.SIE_PPB0.Width == IpcInfo->ICF_FA.BaseInfo.SIE_IN.Crop.SizeH) &&
        (IpcInfo->ICF_FA.BaseInfo.SIE_PPB0.Height == IpcInfo->ICF_FA.BaseInfo.SIE_IN.Crop.SizeV))
    {
        IpcInfo->ICF_FA.BaseInfo.SIE_Src = NonScaled_Img;
    }
    else
    {
        IpcInfo->ICF_FA.BaseInfo.SIE_Src = CH0_Scaled_Img;
    }

    //for channel 1 output
    if (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CA_EN) == ENABLE)
    {
        IPL_CtrlInitIpc_SIECh1PPB(Id, &IpcInfo->ICF_FA.BaseInfo.SIE_CH1, &IpcInfo->ICF_FA.BaseInfo.SIE_EXT_PPB[0], &IpcInfo->ICF_FA.BaseInfo.SIE_PPB_OFS);
    }

    //for channel 2 output
    if (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_LA_EN) == ENABLE)
    {
        IPL_CtrlInitIpc_SIECh2PPB(Id, &IpcInfo->ICF_FA.BaseInfo.SIE_CH2, &IpcInfo->ICF_FA.BaseInfo.SIE_CH2_EXT_PPB[0], &IpcInfo->ICF_FA.BaseInfo.SIE_PPB_OFS);
    }

    IPL_CtrlInitIpc_IPLInPPB(Id, &IpcInfo->ICF_FA.BaseInfo.IPL_PPB0, &IpcInfo->ICF_FA.BaseInfo.IPL_StPos);

    IPL_CtrlInitIpc_DCEIn(Id, &SenInfo, &IpcInfo->ICF_FA.BaseInfo.DCE_Ref_1X, &IpcInfo->ICF_FA.BaseInfo.DCE_Ref_1XCenter, &IpcInfo->ICF_FA.BaseInfo.DCE_Ref_1XCenterOfs, &IpcInfo->ICF_FA.BaseInfo.SenHBinRatio, &IpcInfo->ICF_FA.BaseInfo.SenVBinRatio);

    if (IPL_CtrlGetInfor(Id, IPLCTRL_IFE2_LCA_EN) == ENABLE)
    {
        IPL_CtrlInitIpc_IPESubOutPPB(Id, NULL, NULL, &IpcInfo->ICF_FA.BaseInfo.IPE_PPB.IPE_OUT);
        IPL_CtrlInitIpc_IFE2LcaPPB(Id, &IpcInfo->ICF_FA.BaseInfo.IFE2_PPB0_ADDR, &IpcInfo->ICF_FA.BaseInfo.IFE2_PPB1_ADDR);
    }

    IpcInfo->ICF_FA.BaseInfo.IME_P1_ENABLE = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_EN);
    IpcInfo->ICF_FA.BaseInfo.IME_P1_D2H264 = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_D2H264);
    IPL_CtrlInitIpc_IMEPxOutPPB(Id, IPLCTRL_IME_P1_EN, &IpcInfo->ICF_FA.BaseInfo.IME_P1_PPB0);
    IPL_CtrlInitIpc_DispPxPPB(Id, IPLCTRL_IME_P1_EN);
    IpcInfo->ICF_FA.BaseInfo.IME_P1_Crop.Start.x = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_OUT_CROP_STR_X);
    IpcInfo->ICF_FA.BaseInfo.IME_P1_Crop.Start.y = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_OUT_CROP_STR_Y);
    IpcInfo->ICF_FA.BaseInfo.IME_P1_Crop.SizeH = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_OUT_CROP_H);
    IpcInfo->ICF_FA.BaseInfo.IME_P1_Crop.SizeV = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_OUT_CROP_V);

    IpcInfo->ICF_FA.BaseInfo.IME_P2_ENABLE = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_EN);
    IPL_CtrlInitIpc_IMEPxOutPPB(Id, IPLCTRL_IME_P2_EN, &IpcInfo->ICF_FA.BaseInfo.IME_P2_PPB0);
    IPL_CtrlInitIpc_DispPxPPB(Id, IPLCTRL_IME_P2_EN);
    IpcInfo->ICF_FA.BaseInfo.IME_P2_Crop.Start.x = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_OUT_CROP_STR_X);
    IpcInfo->ICF_FA.BaseInfo.IME_P2_Crop.Start.y = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_OUT_CROP_STR_Y);
    IpcInfo->ICF_FA.BaseInfo.IME_P2_Crop.SizeH = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_OUT_CROP_H);
    IpcInfo->ICF_FA.BaseInfo.IME_P2_Crop.SizeV = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_OUT_CROP_V);

    IpcInfo->ICF_FA.BaseInfo.IME_P3_ENABLE = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_EN);
    IPL_CtrlInitIpc_IMEPxOutPPB(Id, IPLCTRL_IME_P3_EN, &IpcInfo->ICF_FA.BaseInfo.IME_P3_PPB0);
    IPL_CtrlInitIpc_DispPxPPB(Id, IPLCTRL_IME_P3_EN);
    IpcInfo->ICF_FA.BaseInfo.IME_P3_Crop.Start.x = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_OUT_CROP_STR_X);
    IpcInfo->ICF_FA.BaseInfo.IME_P3_Crop.Start.y = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_OUT_CROP_STR_Y);
    IpcInfo->ICF_FA.BaseInfo.IME_P3_Crop.SizeH = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_OUT_CROP_H);
    IpcInfo->ICF_FA.BaseInfo.IME_P3_Crop.SizeV = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_OUT_CROP_V);

    IpcInfo->ICF_FA.BaseInfo.IME_P4_ENABLE = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P4_EN);
    IPL_CtrlInitIpc_IMEPxOutPPB(Id, IPLCTRL_IME_P4_EN, &IpcInfo->ICF_FA.BaseInfo.IME_P4_PPB0);
    IPL_CtrlInitIpc_DispPxPPB(Id, IPLCTRL_IME_P4_EN);
    IpcInfo->ICF_FA.BaseInfo.IME_P4_Crop.Start.x = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P4_OUT_CROP_STR_X);
    IpcInfo->ICF_FA.BaseInfo.IME_P4_Crop.Start.y = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P4_OUT_CROP_STR_Y);
    IpcInfo->ICF_FA.BaseInfo.IME_P4_Crop.SizeH = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P4_OUT_CROP_H);
    IpcInfo->ICF_FA.BaseInfo.IME_P4_Crop.SizeV = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P4_OUT_CROP_V);

    if (IPL_CtrlGetInfor(Id, IPLCTRL_IME_P4_EXTOUT_EN) == ENABLE)
    {
        IpcInfo->ICF_FA.BaseInfo.IME_P4_EXT_OUT_ENABLE = ENABLE;
        IpcInfo->ICF_FA.BaseInfo.IME_P4_EdgeMapBufOfs= IPL_CtrlGetInfor(Id, IPLCTRL_IME_P4_EDGEMAP_BUFOFS);
        IpcInfo->ICF_FA.BaseInfo.IME_P4_HistoBufOfs= IPL_CtrlGetInfor(Id, IPLCTRL_IME_P4_HISTO_BUFOFS);

        //#NT#2016/03/24#Jarkko Chang -begin
        //#NT# remove unusd code
        #if 0
        AdasP4Info = ADAS_getPath4Info();
        IpcInfo->ICF_FA.BaseInfo.IME_P4_Crop.Start.x = AdasP4Info.CropStartX;
        IpcInfo->ICF_FA.BaseInfo.IME_P4_Crop.Start.y = AdasP4Info.CropStartY;
        IpcInfo->ICF_FA.BaseInfo.IME_P4_Crop.SizeH = AdasP4Info.CropSizeH;
        IpcInfo->ICF_FA.BaseInfo.IME_P4_Crop.SizeV = AdasP4Info.CropSizeV;
        #endif
        //#NT#2016/03/24#Jarkko Chang -end
    }

    //engine clock
    IpcInfo->ICF_FA.Ext.fp_SIEClk = FlowInfo->fp_SIEClk;
    IpcInfo->ICF_FA.Ext.fp_SIEClkSrc = FlowInfo->fp_SIEClkSrc;
    IpcInfo->ICF_FA.Ext.fp_SIEPclkSel = FlowInfo->fp_SIEPclkSel;
    IpcInfo->ICF_FA.Ext.fp_IFEClk = FlowInfo->fp_IFEClk;
    IpcInfo->ICF_FA.Ext.fp_IFE2Clk = FlowInfo->fp_IFE2Clk;
    IpcInfo->ICF_FA.Ext.fp_DCEClk = FlowInfo->fp_DCEClk;
    IpcInfo->ICF_FA.Ext.fp_IPEClk = FlowInfo->fp_IPEClk;
    IpcInfo->ICF_FA.Ext.fp_IMEClk = FlowInfo->fp_IMEClk;

    //engine setting
    IpcInfo->ICF_FA.Ext.fp_SieInit = FlowInfo->fp_SieInit;
    IpcInfo->ICF_FA.Ext.fp_IfeInit = FlowInfo->fp_IfeInit;
    IpcInfo->ICF_FA.Ext.fp_DceInit = FlowInfo->fp_DceInit;
    IpcInfo->ICF_FA.Ext.fp_IpeInit = FlowInfo->fp_IpeInit;
    IpcInfo->ICF_FA.Ext.fp_ImeInit = FlowInfo->fp_ImeInit;
    //IFE2 callback
    IpcInfo->ICF_FA.Ext.fp_Ife2Init = FlowInfo->fp_IFE2Init;
}
#if 0
#endif
void IPL_CtrlInitBufInfoFlowA(UINT32 Id, IPLBUF_ENG2DRAMINFO *BufInfo)
{
    //reset parameter
    memset((void *)BufInfo, 0x0, sizeof(IPLBUF_ENG2DRAMINFO));

    BufInfo->SIECh0En = DISABLE;
    BufInfo->SIECh1En = DISABLE;
    BufInfo->SIECh2En = DISABLE;
    BufInfo->SIECh3En = DISABLE;

    BufInfo->SIECAEn = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CA_EN);
    if (BufInfo->SIECAEn == ENABLE)
    {
        BufInfo->SIECABufNum = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH1_BUFNUM);
        BufInfo->SIECABufSize = sizeof(UINT32)*IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CA_WINXNUM)*IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CA_WINYNUM)*4;
    }

    BufInfo->SIELAEn = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_LA_EN);
    if (BufInfo->SIELAEn == ENABLE)
    {
        BufInfo->SIELABufNum = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH2_BUFNUM);
        BufInfo->SIELABufSize = sizeof(UINT32)*IPL_CtrlGetInfor(Id, IPLCTRL_SIE_LA_WINXNUM)*IPL_CtrlGetInfor(Id, IPLCTRL_SIE_LA_WINYNUM)*3;
    }

    BufInfo->SIEVAEn = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_VA_EN);
    if (BufInfo->SIEVAEn == ENABLE)
    {
        BufInfo->SIEVABufNum = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_VA_BUFNUM);
        BufInfo->SIEVABufSize = IPL_CtrlGetSIEVABufInfo();
    }

    BufInfo->SIEEthEn = DISABLE;

    BufInfo->IPEOutEn = DISABLE;
    BufInfo->IFE2LCAEn = IPL_CtrlGetInfor(Id, IPLCTRL_IFE2_LCA_EN);

    if (BufInfo->IFE2LCAEn == ENABLE)
    {
        BufInfo->IPEOut.Fmt = IPL_CtrlGetInfor(Id, IPLCTRL_IPE_OUT_FMT);
        BufInfo->IPEOut.Width = IPL_CtrlGetInfor(Id, IPLCTRL_IPE_OUT_SIZE_H);
        BufInfo->IPEOut.LineOfs = IPL_CtrlGetInfor(Id, IPLCTRL_IPE_OUT_SIZE_YLOS);
        BufInfo->IPEOut.Height = IPL_CtrlGetInfor(Id, IPLCTRL_IPE_OUT_SIZE_V);

        BufInfo->IFE2LCABufNum = IPL_CtrlGetInfor(Id, IPLCTRL_IFE2_OUT_BUFNUM);
    }

    BufInfo->IMEPath1En = ENABLE;
    BufInfo->IMEPath1.BufNum = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_BUFNUM);
    BufInfo->IMEPath1.Width = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_BUF_SIZE_H);
    BufInfo->IMEPath1.Height = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_BUF_SIZE_V);
    BufInfo->IMEPath1.LineOfs = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_BUF_SIZE_H);
    BufInfo->IMEPath1.Fmt = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_BUFFMT);

    //path buffer always set enable for allocate buffer size
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

void IPL_CtrlPrvInitFlowA(UINT32 Id, IPLCTRL_INFO *CtrlInfo)
{
    UINT32 Width = 0, Height = 0;
    IPC_QINFO IpcInfo = {0};
    AWB_CAINFO CAInfo = {0};
    AE_LAINFO LAInfo = {0};
    IPL_IME_PATH_INFO AdasP4Info = {0};

    //All Direct Flow Notification Item
    //1. only support IPL_CF_NORMAL mode
    //2. not support Raww Encode / Crop / SIE Scale
    //3. Direct to IFE2 and H.264
    IpcInfo = IPL_CTRLQuery(Id, ICF_FLOWA);

    IPL_CtrlSetInfor(Id, IPLCTRL_PRVAUTO_OB_EN, DISABLE);

    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_PRVSTPIX, IPL_UTI_CONV_SEN_STPIX(CtrlInfo->Info->Mode->StPix));

    IPL_CtrlSetInfor(Id, IPLCTRL_CFMODE, IPL_CF_NORMAL);
    IPL_CtrlSetInfor(Id, IPLCTRL_CFMODEEXT, IPL_CFEXT_NORMAL);
    //#NT#2016/02/18#Jarkko Chang -begin
    //#NT# add support dsp flip
    IPL_CtrlSetInfor(Id, IPLCTRL_FLIP_TYPE, CtrlInfo->ChgMode->FlipInfo);
    //#NT#2016/02/18#Jarkko Chang -end
    AE_SetAppMode(AE_APPMODE_NORMAL);

    IPL_DzoomGetSIECropSize(Id, IPL_DzoomGetIndex(Id), FALSE, &Width, &Height);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_PRVCROP_SIZE_H, Width);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_PRVCROP_SIZE_V, Height);

    //SIE ch0
    //All Direct Flow need Set SIE Output Parameters but Disable Output Path
    IPL_DzoomGetSIEOutSize(Id, IPL_DzoomGetIndex(Id), &Width, &Height);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_RAW_EN, DISABLE);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_RAW_BUFNUM, 0);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_H, Width);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_V, Height);

    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_RAWENC_EN, DISABLE);

    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_PRVBIT, IPL_UTI_CONV_ALGINFO_RAW_BIT(IPL_AlgGetUIInfo(Id, IPL_SEL_RAW_BITDEPTH_PRV)));
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_HLOS, IPL_UTI_RAWH_CONV2_LOFS(Width, IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_PRVBIT)));

    //SIE ch1   //for DVI Y output
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH1_Y_EN, DISABLE);
    //SIE ch1 CA Out
    AWB_GetCAInfo(Id, &CAInfo);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CA_EN, ENABLE);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CA_WINXNUM, CAInfo.WinNumX);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CA_WINYNUM, CAInfo.WinNumY);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH1_BUFNUM, IpcInfo.QUE_FA.nBUF_CAPPB);
    //SIE ch2   //for DVI UV output
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH2_UV_EN, DISABLE);
    //SIE ch2 LA Out
    AE_GetLAInfo(Id, &LAInfo);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_LA_EN, ENABLE);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_LA_WINXNUM, LAInfo.WinNumX);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_LA_WINYNUM, LAInfo.WinNumY);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH2_BUFNUM, IpcInfo.QUE_FA.nBUF_LAPPB);

    //SIE ch3
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH3_RAW_EN, DISABLE);

    //SIE ch3 VA Out
    if (CtrlInfo->ChgMode->FuncEn & IPL_FUNC_VA)
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_SIE_VA_EN, ENABLE);
        IPL_CtrlSetInfor(Id, IPLCTRL_SIE_VA_BUFNUM, IpcInfo.QUE_FA.nBUF_SIE_VAPPB);
    }
    else
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_SIE_VA_EN, DISABLE);
        IPL_CtrlSetInfor(Id, IPLCTRL_SIE_VA_BUFNUM, 0);
    }

    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_ECS_EN, DISABLE);

    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_ETH_EN, DISABLE);
    IPL_CtrlSetInfor(Id, IPLCTRL_DIS_EN, DISABLE);

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

    //Set IPL IN for IFE IN: All Direct not support RDE
    IPL_CtrlSetInfor(Id, IPLCTRL_IPL_IN_BIT, IPL_CtrlGetInfor(Id,IPLCTRL_SIE_CH0_PRVBIT));
    IPL_CtrlSetInfor(Id, IPLCTRL_IPL_IN_SIZE_H, Width);
    IPL_CtrlSetInfor(Id, IPLCTRL_IPL_IN_SIZE_V, Height);
    IPL_CtrlSetInfor(Id, IPLCTRL_IPL_IN_SIZE_HLOS,IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_HLOS));

    //IPE OUT - TO DMA
    IPL_CtrlSetInfor(Id, IPLCTRL_IPE_OUT_EN, DISABLE);

    if (IQS_IsChromaAdaptEn(Id, FALSE))
    {
        Width  = IPL_UTI_IPESUB_ADJ_HSIZE(IPL_CtrlGetInfor(Id, IPLCTRL_IPL_IN_SIZE_H), IQS_GetChromaAdaptRatio(Id, FALSE, TRUE), 4);
        Height = IPL_UTI_IPESUB_ADJ_VSIZE(IPL_CtrlGetInfor(Id, IPLCTRL_IPL_IN_SIZE_V), IQS_GetChromaAdaptRatio(Id, FALSE, TRUE), 4);

        //IPE OUT - TO IFE2
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_IFE2_EN, ENABLE);

        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_OUT_FMT, IPL_IMG_PACK_YUV444);
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_OUT_SIZE_H, Width);
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_OUT_SIZE_YLOS, IPL_YSIZEH_CONV2LOFS(IPL_CtrlGetInfor(Id, IPLCTRL_IPE_OUT_FMT), IPL_CtrlGetInfor(Id, IPLCTRL_IPE_OUT_SIZE_H)));
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_OUT_SIZE_V, Height);

        //IFE2 OUT
        IPL_CtrlSetInfor(Id, IPLCTRL_IFE2_LCA_EN, ENABLE);
        IPL_CtrlSetInfor(Id, IPLCTRL_IFE2_OUT_BUFNUM, IpcInfo.QUE_FA.nBUF_IFE2PPB);
    }
    else
    {
        //IPE OUT - TO IFE2
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_IFE2_EN, DISABLE);
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_OUT_FMT, IPL_IMG_PACK_YUV444);
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_OUT_SIZE_H, 0);
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_OUT_SIZE_YLOS, 0);
        IPL_CtrlSetInfor(Id, IPLCTRL_IPE_OUT_SIZE_V, 0);

        //IFE2 OUT
        IPL_CtrlSetInfor(Id, IPLCTRL_IFE2_LCA_EN, DISABLE);
        IPL_CtrlSetInfor(Id, IPLCTRL_IFE2_OUT_BUFNUM, IpcInfo.QUE_FC.nBUF_IFE2PPB);
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
        //IME P1
        if (IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_EN) == ENABLE)
        {
            if ((IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_BUF_SIZE_H) * IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_BUF_SIZE_V)) < ((CtrlInfo->ChgMode->ImePath1.CropStartX + CtrlInfo->ChgMode->ImePath1.CropSizeH) * (CtrlInfo->ChgMode->ImePath1.CropStartY + CtrlInfo->ChgMode->ImePath1.CropSizeV)))
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
                        CtrlInfo->ChgMode->ImePath4.BufSizeV, CtrlInfo->ChgMode->ImePath4.CropStartX, CtrlInfo->ChgMode->ImePath4.CropStartY, CtrlInfo->ChgMode->ImePath4.CropSizeH, CtrlInfo->ChgMode->ImePath4.CropSizeV);
            }
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
}
