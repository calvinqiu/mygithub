#include "Sensor.h"
#include "IPL_Cal_Ctrl_AR0330M_EVB_FF_Int.h"
#include "IPL_Cal_Setting_AR0330M_EVB_FF_Int.h"
#include "IPL_CAL_Debug.h"
#include "GxDisplay.h"
#include "IPL_Display.h"
#include "IPL_AlgInfor.h"
#include "IPL_Cal_DataPassing_AR0330M_EVB_FF_Int.h"
#include "GxVideo.h"

#include "awb_api.h"
#include "IPL_HeaderInfor_Raw.h"

#define DMA333 ENABLE

#define CAL_TRIGGER_BUF_NUM 4

#define RAW_BIT         IPL_RAW_BIT_8//SUPPORT ONLY 8bit or 16bit
static IPLC_BASEINFO    m_BaseInfo = {0};
static IPLC_EXT         m_ExtInfo = {0};
static SIE_UPDATE       m_sieUpdate = {0};
static SIE_SUBFUNC      m_sieSubF = {0};
static SIE              m_sie = {0};

SIE_CACC Cal_cacc;
SIE_STATS_PATH_INFO SIE_Path;
SIE_EmbeddedCS m_Ecs = {ECS_D2F8,ENABLE,ENABLE,_BIT_1_2};

IPL_YCC_CH_INFO m_CAL_IPE_OUT_SIZE;

UINT32 IPL_Num_AR0330M;

_INLINE void* _AdaptSettingCBFlowC(UINT32 Id, IPL_RAW_IMG_INFO IplInInfo, IPL_YCC_CH_INFO *ChInfo)
{
    ChInfo->Width = m_CAL_IPE_OUT_SIZE.Width;
    ChInfo->Height = m_CAL_IPE_OUT_SIZE.Height;
    ChInfo->LineOfs = m_CAL_IPE_OUT_SIZE.LineOfs;
    return NULL;
}

static IPH_CLK _SIEClkCB(UINT32 Id)
{
    IPH_CLK clk;
    SENSOR_INFO SenInfo;
    SenInfo.DataType = SENSOR_DATA_PARALLEL;

    SENSOR_ID SenID = SENSOR_ID_1;

    if (Id >= SENSOR_ID_MAX_NUM)
    {
        DBG_ERR("wrong sensor ID %d\r\n", SenID);
        SenID = SENSOR_ID_1;
    }

    switch(Id)
    {
        case SENSOR_ID_4:
           SenID = SENSOR_ID_4;
           break;
        case SENSOR_ID_3:
           SenID = SENSOR_ID_3;
           break;
        case SENSOR_ID_2:
           SenID = SENSOR_ID_2;
           break;
        case SENSOR_ID_1:
        default:
           SenID = SENSOR_ID_1;
           break;
    }

    Sensor_GetStatus(SenID, IPL_AlgGetUIInfo(Id,IPL_SEL_PRVSENMODE), &SenInfo);

    //#NT#2016/03/25#Silvia Wu -begin
    //#NT# fixed SIE CLK error when VX1 Sensor
    switch (SenInfo.DataType)
    {
        case SENSOR_DATA_LVDS:
        case SENSOR_DATA_MIPI:
            clk.SIEclk = 0;
        break;

        case SENSOR_DATA_PARALLEL:
            clk.SIEclk = 240000000;
        break;

        case SENSOR_DATA_DUMMY:
            clk.SIEclk = 27000000;
        break;

        default:
            clk.SIEclk = 54000000;
        break;
    }
    //#NT#2016/03/25#Silvia Wu -end
    return clk;
}

_INLINE UINT32 _SIEClkSrcCB(UINT32 Id)
{
    return IPH_SIECLKSRC_CURR;
}

_INLINE UINT32 _SIEPclkSelCB(UINT32 Id)
{
    //#NT#2016/03/25#Silvia Wu -begin
    //#NT# fixed SIE PCLK error when VX1 Sensor
    //return IPH_SIEPXCLK_PAD;

    SENSOR_INFO SenInfo = {0};
    SenInfo.DataType = SENSOR_DATA_PARALLEL;
    ER rt = E_OK;

    SENSOR_ID SenID = SENSOR_ID_1;

    if (Id >= SENSOR_ID_MAX_NUM)
    {
        DBG_ERR("wrong sensor ID %d\r\n", SenID);
        SenID = SENSOR_ID_1;
    }

    switch(Id)
    {
        case SENSOR_ID_4:
           SenID = SENSOR_ID_4;
           break;
        case SENSOR_ID_3:
           SenID = SENSOR_ID_3;
           break;
        case SENSOR_ID_2:
           SenID = SENSOR_ID_2;
           break;
        case SENSOR_ID_1:
        default:
           SenID = SENSOR_ID_1;
           break;
    }

    rt = Sensor_GetStatus(SenID, IPL_AlgGetUIInfo(Id,IPL_SEL_PRVSENMODE), &SenInfo);

    if (rt != E_OK)
        return IPH_SIEPXCLK_OFF;

    if (SenInfo.CmdType == SENSOR_CMD_Vx1)
        return IPH_SIEPXCLK_VX1_1X;
    else
        return IPH_SIEPXCLK_PAD;
    //#NT#2016/03/25#Silvia Wu -end
}

static IPH_CLK _RDEClkCB(UINT32 Id)
{
    IPH_CLK clk;
    clk.RDEclk = IPH_RDECLK3;
    return clk;
}

static IPH_CLK _IFEClkCB(UINT32 Id)
{
    IPH_CLK clk;
    clk.IFEclk = IPH_IFECLK3;
    return clk;
}

static IPH_CLK _IFE2ClkCB(UINT32 Id)
{
    IPH_CLK clk;
    clk.IFE2clk = IPH_IFE2CLK3;
    return clk;
}

static IPH_CLK _DCEClkCB(UINT32 Id)
{
    IPH_CLK clk;
    clk.DCEclk = IPH_DCECLK3;
    return clk;
}

static IPH_CLK _IPEClkCB(UINT32 Id)
{
    IPH_CLK clk;
    clk.IPEclk = IPH_IPECLK3;
    return clk;
}

static IPH_CLK _IMEClkCB(UINT32 Id)
{
    IPH_CLK clk;
    clk.IMEclk = IPH_IMECLK3;
    return clk;
}

_INLINE void _SIESetCA_FCB(UINT32 Id, SieFuncEn *FuncEn, SIE_INTE_EN *InteEn, SIE_UPDATE *Update, SIE_CACC *cacc, SIE_STATS_PATH_INFO *pPath)
{
    THRESHOL_SET CA_ThrInit = {0, 1023};

    if (Id != IPL_ID_1 && Id != IPL_ID_2)
    {
        return;
    }

    cacc->CACrop.StartPix.x = 0;
    cacc->CACrop.StartPix.y = 0;
    cacc->CACrop.SizeH = m_BaseInfo.SIE_IN.Act.SizeH;
    cacc->CACrop.SizeV = m_BaseInfo.SIE_IN.Act.SizeV;

    cacc->CA_WIN.WinNum_X = 32;
    cacc->CA_WIN.WinNum_Y = 32;

    //ca threshold init
    cacc->CA_TH_INFO.Gth = CA_ThrInit;
    cacc->CA_TH_INFO.Rth = CA_ThrInit;
    cacc->CA_TH_INFO.Bth = CA_ThrInit;
    cacc->CA_TH_INFO.Pth = CA_ThrInit;
    pPath->bVig = ENABLE;
    pPath->bCaThreshold = ENABLE;

    *FuncEn |= CACCEn;
    *Update |= (SIE_CA_SIZE | SIE_CA_TH | SIE_SUBFEN | SIE_STATS_PATH_CA);
}

_INLINE void _SIESetBP_FCB(UINT32 Id, SieFuncEn *FuncEn, SIE_INTE_EN *InteEn, SIE_UPDATE *Update, SIE_SUBFUNC *Sub)
{
    Sub->BP.BP1 = 10;
    Sub->BP.BP2 = 20;
    Sub->BP.BP3 = 30;

    *InteEn |= (SIE__INTE_BP1|SIE__INTE_BP2|SIE__INTE_BP3);
    *Update |= (SIE_BP_ | SIE_SUBFEN);
}

_INLINE void _SIESetDMAOutSrc_FCB(UINT32 Id, SieFuncEn *FuncEn, SIE_INTE_EN *InteEn, SIE_UPDATE *Update)
{
    *FuncEn |= DMAOutCh0En;
    *Update |= SIE_SUBFEN;
    *Update |= SIE_CH0Out_SRC;
}

_INLINE void _SIESetRawEnc_FCB(UINT32 Id,  SIE_UPDATE *Update, SieFuncEn *FuncEn, SIE_RAWENC *RawEnc)
{
    RawEnc->CompRate = 50;
    RawEnc->QInit= 4;

    {
        *FuncEn &= ~RawEncEn;
    }
}

_INLINE void _SIESetEthOut_FCB(UINT32 Id, SieFuncEn *FuncEn, SIE_INTE_EN *InteEn, SIE_UPDATE *Update, SIE_SUBFUNC *Sub)
{
    {
        *FuncEn &= ~(EthEn);
        *InteEn &= ~(SIE__INTE_DRAM_OUT4_END);
        *Update |= (SIE_SUBFEN);
    }
}

_INLINE void _SIESetOB_FCB(UINT32 Id, SieFuncEn *FuncEn, SIE_INTE_EN *InteEn, SIE_UPDATE *Update, SIE_OBAVG *OBAvg, SIE_OBSUB *OBSub)
{
    {
        OBAvg->StartPix.x = 0;
        OBAvg->StartPix.y = 0;
        OBAvg->SizeH = 0;
        OBAvg->SizeV = 0;
        OBSub->Offset = 0;
        OBAvg->SubRatio_X = 0;
        OBAvg->AvgActThres = 0x80;
        OBAvg->ObSubRatio = 0x20;
        *FuncEn |= (OBSubSelEn);
        *Update |= (SIE_OBOFS_ | SIE_OBAVG_ | SIE_SUBFEN);
    }
}

_INLINE void _SIESetLA_FCB(UINT32 Id, SieFuncEn *FuncEn, SIE_INTE_EN *InteEn, SIE_UPDATE *Update, SIE_LACC *lacc, SIE_STATS_PATH_INFO *pPath)
{
    {
        lacc->LACrop.StartPix.x = (m_BaseInfo.SIE_IN.Act.SizeH - m_BaseInfo.IPL_PPB0.Width) >> 1;
        lacc->LACrop.StartPix.y = (m_BaseInfo.SIE_IN.Act.SizeV - m_BaseInfo.IPL_PPB0.Height) >> 1;
        lacc->LACrop.SizeH = m_BaseInfo.IPL_PPB0.Width;
        lacc->LACrop.SizeV = m_BaseInfo.IPL_PPB0.Height;

        lacc->LA_WIN.WinNum_X = 32;
        lacc->LA_WIN.WinNum_Y = 32;

        pPath->bLaCg = ENABLE;
        pPath->bLaGama1 = ENABLE;
        pPath->bLa1SrcSel = STATS_LA1_POST_CG;
        pPath->bHistogramSel = STATS_HISTO_POST_GAMMA;

        *FuncEn |= (LACCEn|HistoYEn);
        *Update |= (SIE_LA_SIZE | SIE_SUBFEN | SIE_STATS_PATH_LA);
    }
}

_INLINE void _SIEIn(IPL_PROC_ID Id, SENSOR_INFO *pSenInfo, Cal_SENSOR_INFO *pCalSenInfo, SIE_WINDOW *Win)
{
    UINT32 CropShift_X = 0;
    UINT32 CropShift_Y = 0;

    Win->Act.StartPix.x = pSenInfo->Mode->TransHD[0].DataStart;
    Win->Act.StartPix.y = pSenInfo->Mode->TransVD[0].DataStart;

    Win->Act.SizeH = pSenInfo->Mode->TransHD[0].DataSize;
    Win->Act.SizeV = pSenInfo->Mode->TransVD[0].DataSize;
    Win->Act.CFA = pSenInfo->Mode->StPix;

    if( (pCalSenInfo->CropShrinkX % 4) )
    {
        pCalSenInfo->CropShrinkX += (4 - (pCalSenInfo->CropShrinkX % 4));//must be mutiple of 4
        DBG_WRN("\r\n");
    }
    if( (pCalSenInfo->CropShrinkY % 4) )
    {
        pCalSenInfo->CropShrinkY += (4 - (pCalSenInfo->CropShrinkY % 4));//must be mutiple of 4
        DBG_WRN("\r\n");
    }

    //crop size must be mutiple of 4
    if( (Win->Act.SizeH % 4) )
    {
        CropShift_X = Win->Act.SizeH % 4;
    }
    if( (Win->Act.SizeV % 4) )
    {
        CropShift_Y = Win->Act.SizeV % 4;
    }

    Win->Crop.SizeH = Win->Act.SizeH - pCalSenInfo->CropShrinkX - CropShift_X;
    Win->Crop.SizeV = Win->Act.SizeV - pCalSenInfo->CropShrinkY - CropShift_Y;
    Win->Crop.CFA = Win->Act.CFA;

    if ((Win->Act.SizeH < Win->Crop.SizeH) || (Win->Act.SizeV < Win->Crop.SizeV))
    {
        DBG_ERR("act size (%d %d) < crop Size(%d %d)\r\n", Win->Act.SizeH, Win->Act.SizeV, Win->Crop.SizeH, Win->Crop.SizeV);
        Win->Crop.StartPix.x = 0;
        Win->Crop.StartPix.y = 0;
    }
    else
    {
        Win->Crop.StartPix.x = (Win->Act.SizeH - Win->Crop.SizeH) >> 1;
        Win->Crop.StartPix.y = (Win->Act.SizeV - Win->Crop.SizeV) >> 1;
    }
}

_INLINE void _SIESignal(IPL_PROC_ID Id, SENSOR_INFO *pSenInfo, Cal_SENSOR_INFO *pCalSenInfo, IPP_SIE_SIGNAL *Signal)
{
    Signal->Sie_HDVD.HD_ini = pCalSenInfo->HDinv;
    Signal->Sie_HDVD.VD_ini = pCalSenInfo->VDinv;

    Signal->SensorPhase.VD = pCalSenInfo->VDPhase;
    Signal->SensorPhase.HD = pCalSenInfo->HDPhase;
    Signal->SensorPhase.Data = pCalSenInfo->DataPhase;
}

_INLINE void _SIEAct(IPL_PROC_ID Id, SENSOR_INFO *pSenInfo, SieACTMode *ActMode)
{
    if ((pSenInfo->DataType == SENSOR_DATA_LVDS) || (pSenInfo->DataType == SENSOR_DATA_MIPI))
    {
        if (Id == IPL_ID_1)
        {
            *ActMode = SIE_IN_CSI_1;
        }
        else if (Id == IPL_ID_2)
        {
            if (pSenInfo->Mode->ModeType == SENSOR_MODE_BUILTIN_HDR || pSenInfo->Mode->ModeType == SENSOR_MODE_STAGGER_HDR)
                *ActMode = SIE_IN_CSI_1;
            else
                *ActMode = SIE_IN_CSI_2;
        }
        else
        {
            DBG_ERR("unsupport this Data type %d\r\n", pSenInfo->DataType);
        }
    }
    else if (pSenInfo->DataType == SENSOR_DATA_PARALLEL)
    {
        if (pSenInfo->CmdType== SENSOR_CMD_Vx1)
        {
            *ActMode = SIE_IN_VX1_SNR;
        }
        else if (pSenInfo->Mode->ModeType == SENSOR_MODE_LINEAR)
        {
            if (pSenInfo->SigType == SENSOR_SIGNAL_MASTER)
            {
                *ActMode = SIE_IN_PARA_MSTR_SNR;
            }
            else if (pSenInfo->SigType == SENSOR_SIGNAL_SLAVE)
            {
                *ActMode = SIE_IN_PARA_SLAV_SNR;
            }
            else
            {
                DBG_ERR("DataType %d unsupport signal type %d\r\n", pSenInfo->DataType, pSenInfo->SigType);
            }
        }
        else if ((pSenInfo->Mode->ModeType == SENSOR_MODE_CCIR)||(pSenInfo->Mode->ModeType == SENSOR_MODE_CCIR_INTERLACE))
        {
            *ActMode = SIE_IN_PARA_MSTR_SNR;
        }
        else
        {
            DBG_ERR("unsupport mode type %d\r\n", pSenInfo->Mode->ModeType);
        }

    }
    else if (pSenInfo->DataType == SENSOR_DATA_DUMMY)
    {
        *ActMode = SIE_IN_SELF_PATGEN;
    }
    else
    {
        *ActMode = SIE_IN_DRAM;
        DBG_ERR("unsupport Data Type %d, Set to Dram In\r\n", pSenInfo->DataType);
    }
}

_INLINE void _SIE_BURST_LENG(IPL_PROC_ID Id, SIE_BURSTLENGTH_SET *burstLeng)
{
    burstLeng->burstLeng_InCh0 = _BURST_IN0_32WORD;
    burstLeng->burstLeng_InCh1 = _BURST_IN1_32WORD;
    burstLeng->burstLeng_InCh2 = _BURST_IN2_32WORD;
    burstLeng->burstLeng_OutCh0 = _BURST_OUT0_48WORD;
    burstLeng->burstLeng_OutCh1 = _BURST_OUT1_32WORD;
    burstLeng->burstLeng_OutCh2 = _BURST_OUT2_32WORD;
    burstLeng->burstLeng_OutCh3 = _BURST_OUT3_32WORD;
    burstLeng->burstLeng_OutCh4 = _BURST_OUT4_32WORD;
}

UINT16 CAL_SIE_GAMMA_BUFFER[2][65];
_INLINE UINT16* _xfer2CAGamma(UINT32 Id, UINT32* IPELut)
{
    int i = 0, j = 0;
    memset(CAL_SIE_GAMMA_BUFFER[Id], 0, sizeof(UINT16)*65);
    for ( i=0; i<129;i+=2 )
    {
        CAL_SIE_GAMMA_BUFFER[Id][j++] = IPELut[i];
    }
    return (UINT16*)CAL_SIE_GAMMA_BUFFER[Id];
}

_INLINE UINT32 _SieInitCB(UINT32 Id,ENG_SUBFUNC *Info,IPL_HAL_GROUP* group)
{
    CAL_TAG* pTag;
    SIE_UPDATE *SIE_Update;
    CAL_SECMAP CalSectionItem;

    SIE_Update = (SIE_UPDATE *)group;

    _SIESetOB_FCB(Id, &Info->pSie_sub->sieFuncEn, &Info->pSie_sub->InteEn, SIE_Update, &Info->pSie_sub->OBAvg, &Info->pSie_sub->OBSub);
    _SIESetCA_FCB(Id, &Info->pSie_sub->sieFuncEn, &Info->pSie_sub->InteEn, SIE_Update, &Info->pSie_sub->Ca, &Info->pSie_sub->pStatPathInfo);
    _SIESetLA_FCB(Id, &Info->pSie_sub->sieFuncEn, &Info->pSie_sub->InteEn, SIE_Update, &Info->pSie_sub->La, &Info->pSie_sub->pStatPathInfo);
    //_SIESetBP_FCB(Id, &Info->pSie_sub->sieFuncEn, &Info->pSie_sub->InteEn, SIE_Update, Info->pSie_sub);
    _SIESetDMAOutSrc_FCB(Id, &Info->pSie_sub->sieFuncEn, &Info->pSie_sub->InteEn, SIE_Update);
    //_SIESetRawEnc_FCB(Id, SIE_Update, &Info->pSie_sub->sieFuncEn, &Info->pSie_sub->RawEnc);
    //_SIESetEthOut_FCB(Id,&Info->pSie_sub->sieFuncEn,&Info->pSie_sub->InteEn,SIE_Update,Info->pSie_sub);

    *SIE_Update |= SIE_SUBFEN;



    //enable function
    Info->pSie_sub->sieFuncEn |=  (
                            //OBAvgEn     |   ///< enable OB average ( ob detection)
                            OBSubSelEn  |   ///< enable OB subtraction selection
                            0
                        );


    //Setting parameter
    group->sieUpdate |= (
                            SIE_SUBFEN          |   ///< update func enable/disable
                            SIE_LA_GAMMA        |   ///< update LA Gamma
                            SIE_OBOFS_          |   ///< update ob offset
                            //SIE_DGAIN_          |   ///< update Digital gain
                            0
                         );

    if ( group->sieUpdate & SIE_LA_GAMMA )
        Info->pSie_sub->La.GmaTbl = _xfer2CAGamma(Id, (UINT32 *)CAL_IQS_GammaLut);

    Info->pSie_sub->OBSub.Offset = 0x04;
    //group->sieUpdate |=  SIE_CA_TH;
    group->sieUpdate |=  SIE_LA_CG;
    AWB_CAINFO awb_cainfo;
    AWB_GetCAInfo(Id, &awb_cainfo);
    Info->pSie_sub->Ca.CA_TH_INFO.Rth.Upper = awb_cainfo.RGu;
    Info->pSie_sub->Ca.CA_TH_INFO.Rth.Lower = awb_cainfo.RGl;
    Info->pSie_sub->Ca.CA_TH_INFO.Gth.Upper = awb_cainfo.GGu;
    Info->pSie_sub->Ca.CA_TH_INFO.Gth.Lower = awb_cainfo.GGl;
    Info->pSie_sub->Ca.CA_TH_INFO.Bth.Upper = awb_cainfo.BGu;
    Info->pSie_sub->Ca.CA_TH_INFO.Bth.Lower = awb_cainfo.BGl;
    Info->pSie_sub->Ca.CA_TH_INFO.Pth.Upper = awb_cainfo.RBGu;
    Info->pSie_sub->Ca.CA_TH_INFO.Pth.Lower = awb_cainfo.RBGl;

    UINT32 RGain=256, GGain=256, BGain=256;

    AWB_GetColorGain(Id, &RGain, &GGain, &BGain);
    if ( CAL_IQS_ColorGain.IfeGainSel == _2_8 ) //sie fix 2.7
    {
        RGain = RGain >> 1;
        GGain = GGain >> 1;
        BGain = BGain >> 1;
    }
    Info->pSie_sub->La.LA_CG_INFO.RGain = RGain;
    Info->pSie_sub->La.LA_CG_INFO.GGain = GGain;
    Info->pSie_sub->La.LA_CG_INFO.BGain = BGain;

    if (Id == IPL_ID_1)
    {
        CalSectionItem = SEC_ECS_1;
    }
    else if (Id == IPL_ID_2)
    {
        CalSectionItem = SEC_ECS_2;
    }
    else if (Id == IPL_ID_3)
    {
        CalSectionItem = SEC_ECS_3;
    }
    else if (Id == IPL_ID_4)
    {
        CalSectionItem = SEC_ECS_4;
    }
    else
    {
        CalSectionItem = SEC_ECS_1;
        DBG_ERR("IPL Id is out of range\r\n");
    }

    CAL_DATA_INFO CaldataInfo = {0};
    CaldataInfo.StationID = SEC_ECS;
    CaldataInfo.SenMode = IPL_AlgGetUIInfo(Id, IPL_SEL_PRVSENMODE);
    CaldataInfo.CalCondition[0] = CAL_CONDITION_DEFAULT;

    pTag = GetCalData(Id, CaldataInfo);


    if(pTag)
    {
        if(pTag->CAL_STATUS == _CAL_OK)
        {
            CAL_ECS_FMT *pFmt;
            pFmt = (CAL_ECS_FMT *)&pTag->TagData;
            Info->pSie_sub->sieFuncEn |= (ECSEn);
            Info->pSie_sub->Ecs.EcsMap = pFmt->header.Ecs_map;
            m_Ecs.BitSel = pFmt->header.Ecs_bit;
            Info->pSie_sub->Ecs.pecs = &m_Ecs;
            Info->pSie_sub->Ecs.Inaddr = (UINT32)&pFmt->ECSBlockWin;
            group->sieUpdate |= (SIE_SUBFEN|SIE_ECS_);
        }
    }

    if (Id == IPL_ID_1)
    {
        CalSectionItem = SEC_DP_1;
    }
    else if (Id == IPL_ID_2)
    {
        CalSectionItem = SEC_DP_2;
    }
    else if (Id == IPL_ID_3)
    {
        CalSectionItem = SEC_DP_3;
    }
    else if (Id == IPL_ID_4)
    {
        CalSectionItem = SEC_DP_4;
    }
    else
    {
        CalSectionItem = SEC_DP_1;
        DBG_ERR("IPL Id is out of range\r\n");
    }

    CaldataInfo.StationID = SEC_DP;
    CaldataInfo.SenMode = IPL_AlgGetUIInfo(Id, IPL_SEL_PRVSENMODE);
    CaldataInfo.CalCondition[0] = CAL_CONDITION_DEFAULT;

    pTag = GetCalData(Id, CaldataInfo);

    if(pTag)
    {
        if(pTag->CAL_STATUS == _CAL_OK)
        {
            CAL_DP_FMT *pFmt;
            pFmt = (CAL_DP_FMT *)&pTag->TagData;
            Info->pSie_sub->sieFuncEn |= (DpcEn);
            Info->pSie_sub->Dpc.DP_TblAddr = (UINT32)&pFmt->DP_Pool;
            Info->pSie_sub->Dpc.TblSize = pTag->TagDataLen - sizeof(CAL_DPC_HEADER);
            Info->pSie_sub->Dpc.Weight = _50_Percent;
            group->sieUpdate |= (SIE_SUBFEN|SIE_DPC_);
        }
    }
    return 1;
}

_INLINE UINT32 _RdeInitCB(UINT32 Id,ENG_SUBFUNC *Info,IPL_HAL_GROUP* group)
{
    return 1;
}

_INLINE UINT32 _IfeInitCB(UINT32 Id,ENG_SUBFUNC *Info,IPL_HAL_GROUP* group)
{
    //enable function
    Info->pIfe_sub->ifeFuncEn |=  (
                            //OutlierEn   |  ///< enable outLier
                            //FilterEn    |  ///< enable bilateral filter
                            CgainEn     |  ///< enable color gain
                            //GBALEN      |   ///< enable function: Gbalance
                        0);

    //Setting parameter
    group->ifeUpdate |= (
                            IFE_SUBFEN      |       ///< update func enable/disable
                            //IFE_OUTLIER     |
                            IFE_CGAIN       |       ///< update color gain
                            IFE_CGOFS       |       ///< update offset(bayer stage)
                            //IFE_CGEFFECT    |       ///< update color gain output inverse/mask
                            //IFE_FILTER      |       ///< update noise reduction filter
                            //IFE_GBALPARAM   |       ///< update Gbalance parameters
                        0);

    Info->pIfe_sub->pOutl    = &CAL_IQS_Outl;
    Info->pIfe_sub->pFilter  = &CAL_IQS_Filter;
    Info->pIfe_sub->pGbal = &CAL_IQS_Gbal;

    UINT32 RGain=256, GGain=256, BGain=256;

    AWB_GetColorGain(Id, &RGain, &GGain, &BGain);
    CAL_IQS_CGain[0] = RGain;
    CAL_IQS_CGain[1] = GGain;
    CAL_IQS_CGain[2] = GGain;
    CAL_IQS_CGain[3] = BGain;
    CAL_IQS_ColorGain.pCOfs = (UINT32*)CAL_IQS_CGain_Offset;
    CAL_IQS_ColorGain.pCGain = (UINT32*)CAL_IQS_CGain;
    Info->pIfe_sub->CGain = CAL_IQS_ColorGain;
    return 1;
}

_INLINE UINT32 _DceInitCB(UINT32 Id,ENG_SUBFUNC *Info,IPL_HAL_GROUP* group)
{
    //enable function

    //Setting parameter
    group->dceUpdate |= (
                        //DCE_CENTER         |    ///< update distortion center
                        //DCE_DIST           |    ///< update H/V distance factor/ratio selection
                        //DCE_RADIUS         |    ///< update radius norm term
                        //DCE_CAB            |    ///< update color aberration factor
                        //DCE_LUT            |    ///< update LUT table address
                        //DCE_FOV            |    ///< update FOV
                        //DCE_ENH            |    ///< update enhancement
                        //DCE_H_FILT         |    ///< update h direction filter enable/disable(UV only)
                        //DCE_DISTOR         |    ///< update distortion parameter set
                        DCE_CFAINFO        |    ///< update cfa interpolation
                        0
                      );
    Cal_DCE_Distortion.Center.x = m_BaseInfo.SIE_IN.Act.SizeH>>1;
    Cal_DCE_Distortion.Center.y = m_BaseInfo.SIE_IN.Act.SizeV>>1;
    Info->pDce_sub->pDistortion = &Cal_DCE_Distortion;
    Info->pDce_sub->pCFAInterpolation = &CAL_DCE_IQS_CFAInter;
    Info->pDce_sub->Dce_Dist = CAL_DCE_Dist;

    return 1;
}

UINT32 CAL_IQ_IPE_GAMMA_BUFFER[2][195];
_INLINE UINT32* CAL_IPE_GammaReorgon(UINT32 Id, UINT32* RLut, UINT32* GLut, UINT32* BLut)
{
    int i, page;
    memset(CAL_IQ_IPE_GAMMA_BUFFER[Id], 0, sizeof(UINT32)*195);
    page = 0;
    for ( i=0; i<129;i++ )
    {
        if ( i%2 == 0 )
            CAL_IQ_IPE_GAMMA_BUFFER[Id][page+i/2] |= RLut[i];
        else
            CAL_IQ_IPE_GAMMA_BUFFER[Id][page+i/2] |= (RLut[i] <<10);
    }
    page = 65;
    for ( i=0; i<129;i++ )
    {
        if ( i%2 == 0 )
            CAL_IQ_IPE_GAMMA_BUFFER[Id][page+i/2] |= GLut[i];
        else
            CAL_IQ_IPE_GAMMA_BUFFER[Id][page+i/2] |= (GLut[i] <<10);
    }
    page = 65*2;
    for ( i=0; i<129;i++ )
    {
        if ( i%2 == 0 )
            CAL_IQ_IPE_GAMMA_BUFFER[Id][page+i/2] |= BLut[i];
        else
            CAL_IQ_IPE_GAMMA_BUFFER[Id][page+i/2] |= (BLut[i] <<10);
    }

    return (UINT32*)CAL_IQ_IPE_GAMMA_BUFFER[Id];
}

_INLINE UINT32 _IpeInitCB(UINT32 Id,ENG_SUBFUNC *Info,IPL_HAL_GROUP* group)
{
    //enable function
    Info->pIpe_sub->IpeFuncEn |= (
                        //IPE_RGBLPEn         |       ///< RGB low-pass filter function enable
                        IPE_RGBGAMMAEn      |       ///< RGB Gamma function enable
                        IPE_CCREn           |       ///< Color correction function enable
                        IPE_CSTEn           |       ///< Color space transform function enable, RGB->YCC
                         0
                       );

    //Setting parameter
    group->ipeUpdate |= (
                        IPE_SUBFEN     |
                        //IPE_RGBLPF_INFO  |   ///< update rgb low pass
                        IPE_CC_INFO      |   ///< update color correction
                        IPE_CST_INFO     |   ///< update color space transfor
                        0
                      );


    group->ipeUpdate |= ( IPE_RGBGAMMA );

    Info->pIpe_sub->prgblpf           = &CAL_IQS_RGBLpf;

    CAL_IQS_ColorCorrection.Fstab_TblAddr = (UINT32)CAL_IQS_FStab;
    CAL_IQS_ColorCorrection.Fdtab_TblAddr = (UINT32)CAL_IQS_FDtab;
    Info->pIpe_sub->pCC = &CAL_IQS_ColorCorrection;

    Info->pIpe_sub->pCST = &CAL_IQS_CSTParam;

    if ( (group->ipeUpdate & IPE_RGBGAMMA) )
        Info->pIpe_sub->GammaAddr = (UINT32)CAL_IPE_GammaReorgon(Id, (UINT32 *)CAL_IQS_GammaLut, (UINT32 *)CAL_IQS_GammaLut, (UINT32 *)CAL_IQS_GammaLut);

    return 1;
}

_INLINE UINT32 _ImeInitCB(UINT32 Id,ENG_SUBFUNC *Info,IPL_HAL_GROUP* group)
{
    //enable function
    Info->pIme_sub->imeFuncEn |= (
                            //ChromaAdaptEn   |   ///< enable function: chroma adaption (LCA)
                            //ChromaMedianEn  |   ///< enable function: chroma median filter
                            //ChromaAdjLchrom |   ///< enable function: chroma adjustment of local chroma adaptation
                            //ChromaAdjLum    |   ///< enable function: chroma adjustment of local luma adaptation
                            0);
    //Setting parameter
    group->imeUpdate |= (   //IME_SUBFEN      |   ///< update function enable/disable
                            //IME_CHROMAA     |   ///< update chroma adaption
                            //IME_CHROMAA_LUM |   ///< update chroma lumaination adaption
                            //IME_CHROMAA_CLR |   ///< update chroma color adaption
                            0);
    Info->pIme_sub->ChromaAdap.pChromaAdaption      = &CAL_IQS_ChromaAdapt;
    Info->pIme_sub->ChromaAdap.pChromaLumAdaption   = &CAL_IQS_ChromaAdapt_Lum;
    Info->pIme_sub->ChromaAdap.pChromaColorAdaption = &CAL_IQS_ChromaAdapt_Color;
    return 1;
}

_INLINE UINT32 _Ife2InitCB(UINT32 Id,ENG_SUBFUNC *Info,IPL_HAL_GROUP* group)
{
    Info->pIfe2_sub->ife2FuncEn |= YFtrEn;

    Info->pIfe2_sub->GrayStatal = CAL_IQS_GrayStatical;
    CAL_IQS_RefCenter.RefCentY = CAL_IQS_RefCenter_Y;
    CAL_IQS_RefCenter.RefCentUV = CAL_IQS_RefCenter_UV;
    Info->pIfe2_sub->pRefcent = &CAL_IQS_RefCenter;

    Info->pIfe2_sub->pFilter = &CAL_IQS_IFE2Filter;
    return 1;
}


static void Prv_CtrlInit(IPL_PROC_ID Id,pIPLC_INFO pIplObj,pCTRL_PTR pCtrlptr)
{
    SENSOR_INFO *pSenInfo = (SENSOR_INFO *)pCtrlptr->pDrv_Sensor;
    Cal_SENSOR_INFO *pCalInfo = (Cal_SENSOR_INFO*)pCtrlptr->pCal_Sensor;
    ISIZE disp_size;
    UINT32 i,j,RawBufNum;
    //IPL_YCC_IMG_INFO imeInfo;

    pIplObj->pBaseInfo = &m_BaseInfo;
    pIplObj->pExtInfo = &m_ExtInfo;

    _SIESignal(Id,pSenInfo,pCalInfo,&(pIplObj->pBaseInfo->SenSorIn));
    _SIEIn(Id,pSenInfo,pCalInfo,&(pIplObj->pBaseInfo->SIE_IN));
    _SIEAct(Id,pSenInfo,&(pIplObj->pBaseInfo->SIE_Act));
    _SIE_BURST_LENG(Id,&(pIplObj->pBaseInfo->BurstLength));

    pIplObj->Id = Id;
    pIplObj->pBaseInfo->SIE_PPB0.Bit = RAW_BIT;
    pIplObj->pBaseInfo->SIE_PPB0.Width = pIplObj->pBaseInfo->SIE_IN.Crop.SizeH;
    if (pIplObj->pBaseInfo->SIE_PPB0.Width > 2112)
        pIplObj->pBaseInfo->SIE_PPB0.Width = 2112;
    pIplObj->pBaseInfo->SIE_PPB0.Height = pIplObj->pBaseInfo->SIE_IN.Crop.SizeV;
    pIplObj->pBaseInfo->SIE_PPB0.LineOffset = pIplObj->pBaseInfo->SIE_PPB0.Width * pIplObj->pBaseInfo->SIE_PPB0.Bit / 8;
    pIplObj->pBaseInfo->SIE_PPB0.STPix = pSenInfo->Mode->StPix;

    pIplObj->pBaseInfo->SIE_CH1.packBus = RAW_BIT;
    pIplObj->pBaseInfo->SIE_CH1.HFlip = DISABLE;
    pIplObj->pBaseInfo->SIE_CH1.VFlip = DISABLE;
    pIplObj->pBaseInfo->SIE_CH1.LineOffset = IPL_UTI_RAWH_CONV2_LOFS(pIplObj->pBaseInfo->SIE_PPB0.Width, pIplObj->pBaseInfo->SIE_PPB0.Bit);

    pIplObj->pBaseInfo->SIE_CH2.packBus = RAW_BIT;
    pIplObj->pBaseInfo->SIE_CH2.HFlip = DISABLE;
    pIplObj->pBaseInfo->SIE_CH2.VFlip = DISABLE;
    pIplObj->pBaseInfo->SIE_CH2.LineOffset = IPL_UTI_RAWH_CONV2_LOFS(pIplObj->pBaseInfo->SIE_PPB0.Width, pIplObj->pBaseInfo->SIE_PPB0.Bit);

    pIplObj->pBaseInfo->SIE_Src = NonScaled_Img;

    pIplObj->pBaseInfo->SenHBinRatio = pSenInfo->Mode->Ratio.RatioH;
    pIplObj->pBaseInfo->SenVBinRatio = pSenInfo->Mode->Ratio.RatioV;

    pIplObj->pBaseInfo->IPL_PPB0.Width = pIplObj->pBaseInfo->SIE_PPB0.Width;
    pIplObj->pBaseInfo->IPL_PPB0.Height = pIplObj->pBaseInfo->SIE_PPB0.Height;
    pIplObj->pBaseInfo->IPL_StPos.x = (pIplObj->pBaseInfo->SIE_PPB0.Width - pIplObj->pBaseInfo->IPL_PPB0.Width)>>1;
    pIplObj->pBaseInfo->IPL_StPos.y = (pIplObj->pBaseInfo->SIE_PPB0.Height - pIplObj->pBaseInfo->IPL_PPB0.Height)>>1;
    pIplObj->pBaseInfo->IPL_PPB0.LineOffset = pIplObj->pBaseInfo->SIE_PPB0.LineOffset;
    pIplObj->pBaseInfo->IPL_PPB0.Bit = pIplObj->pBaseInfo->SIE_PPB0.Bit;
    pIplObj->pBaseInfo->IPL_PPB0.STPix = pIplObj->pBaseInfo->SIE_PPB0.STPix;

    pIplObj->pBaseInfo->DCE_Ref_1X.w = pSenInfo->Mode->TransHD[0].DataSize;
    pIplObj->pBaseInfo->DCE_Ref_1X.h = pSenInfo->Mode->TransVD[0].DataSize;
    pIplObj->pBaseInfo->DCE_Ref_1XCenter.x = pIplObj->pBaseInfo->DCE_Ref_1X.w >>1;
    pIplObj->pBaseInfo->DCE_Ref_1XCenter.y = pIplObj->pBaseInfo->DCE_Ref_1X.h >>1;

    pIplObj->pBaseInfo->IPE_PPB.IPE_OUT.Width = IPL_UTI_IPESUB_ADJ_HSIZE(pIplObj->pBaseInfo->IPL_PPB0.Width, 20, 4); //direct to IFE2 * 20%
    pIplObj->pBaseInfo->IPE_PPB.IPE_OUT.Height = IPL_UTI_IPESUB_ADJ_VSIZE(pIplObj->pBaseInfo->IPL_PPB0.Height, 20, 4); //direct to IFE2 * 20%
    pIplObj->pBaseInfo->IPE_PPB.IPE_OUT.LineOfs = IPL_YSIZEH_CONV2LOFS(IPL_IMG_PACK_YUV444, pIplObj->pBaseInfo->IPE_PPB.IPE_OUT.Width);

    disp_size = GxVideo_GetDeviceSize(DOUT1);

    pIplObj->pBaseInfo->IME_P2_PPB0.type = IPL_IMG_Y_PACK_UV420;
    pIplObj->pBaseInfo->IME_P2_PPB0.Ch[0].Width = disp_size.w;
    pIplObj->pBaseInfo->IME_P2_PPB0.Ch[0].Height = disp_size.h;
    pIplObj->pBaseInfo->IME_P2_PPB0.Ch[0].LineOfs = pIplObj->pBaseInfo->IME_P2_PPB0.Ch[0].Width;
    pIplObj->pBaseInfo->IME_P2_PPB0.Ch[1] = IPL_UTI_Y_INFO_CONV2(pIplObj->pBaseInfo->IME_P2_PPB0.type,pIplObj->pBaseInfo->IME_P2_PPB0.Ch[0]);
    pIplObj->pBaseInfo->IME_P2_PPB0.Ch[2] = IPL_UTI_Y_INFO_CONV2(pIplObj->pBaseInfo->IME_P2_PPB0.type,pIplObj->pBaseInfo->IME_P2_PPB0.Ch[0]);

    pIplObj->pBaseInfo->IME_P2_ENABLE = ENABLE;

    pIplObj->pBaseInfo->IME_P2_Crop.Start.x = 0;
    pIplObj->pBaseInfo->IME_P2_Crop.Start.y = 0;
    pIplObj->pBaseInfo->IME_P2_Crop.SizeH = disp_size.w;
    pIplObj->pBaseInfo->IME_P2_Crop.SizeV = disp_size.h;

    m_CAL_IPE_OUT_SIZE.Width = pIplObj->pBaseInfo->IPE_PPB.IPE_OUT.Width;
    m_CAL_IPE_OUT_SIZE.Height = pIplObj->pBaseInfo->IPE_PPB.IPE_OUT.Height;
    m_CAL_IPE_OUT_SIZE.LineOfs = IPL_YSIZEH_CONV2LOFS(IPL_IMG_PACK_YUV444, pIplObj->pBaseInfo->IPE_PPB.IPE_OUT.Width);
    pIplObj->pAdaptSetting = _AdaptSettingCBFlowC;

    pIplObj->pExtInfo->fp_SIEClk = _SIEClkCB;
    pIplObj->pExtInfo->fp_SIEClkSrc = _SIEClkSrcCB;
    pIplObj->pExtInfo->fp_SIEPclkSel = _SIEPclkSelCB;
    pIplObj->pExtInfo->fp_RDEClk = _RDEClkCB;
    pIplObj->pExtInfo->fp_IFEClk = _IFEClkCB;
    pIplObj->pExtInfo->fp_IFE2Clk = _IFE2ClkCB;
    pIplObj->pExtInfo->fp_DCEClk = _DCEClkCB;
    pIplObj->pExtInfo->fp_IPEClk = _IPEClkCB;
    pIplObj->pExtInfo->fp_IMEClk = _IMEClkCB;

    pIplObj->pExtInfo->fp_SieInit = _SieInitCB;
    pIplObj->pExtInfo->fp_RdeInit = _RdeInitCB;
    pIplObj->pExtInfo->fp_IfeInit = _IfeInitCB;
    pIplObj->pExtInfo->fp_DceInit = _DceInitCB;
    pIplObj->pExtInfo->fp_IpeInit = _IpeInitCB;
    pIplObj->pExtInfo->fp_ImeInit = _ImeInitCB;
    pIplObj->pExtInfo->fp_Ife2Init = _Ife2InitCB;


    UINT32 RawHeaderSize = IPL_HeaderRawGetBufSize();
    Cal_BufID bufID;
    bufID = CalApi()->fpBufEvent()->fpGetCalBufID(Id);

    CalApi()->fpBufEvent()->fpSetAddr(bufID,sie_raw_header_0,
    RawHeaderSize);
    CalApi()->fpBufEvent()->fpSetAddr(bufID,sie_raw_0,
    pIplObj->pBaseInfo->SIE_PPB0.LineOffset * pIplObj->pBaseInfo->SIE_PPB0.Height);

    CalApi()->fpBufEvent()->fpSetAddr(bufID,sie_raw_header_1,
    RawHeaderSize);
    CalApi()->fpBufEvent()->fpSetAddr(bufID,sie_raw_1,
    pIplObj->pBaseInfo->SIE_PPB0.LineOffset * pIplObj->pBaseInfo->SIE_PPB0.Height);

    RawBufNum = CAL_TRIGGER_BUF_NUM - 1;
    for (j = 0; j < RawBufNum; j ++)
    {
        CalApi()->fpBufEvent()->fpSetAddr(bufID,sie_CH0_header_0+j,
        RawHeaderSize);
        CalApi()->fpBufEvent()->fpSetAddr(bufID,sie_CH0_0+j,
        pIplObj->pBaseInfo->SIE_PPB0.LineOffset * pIplObj->pBaseInfo->SIE_PPB0.Height);
    }

    for(i = 0;i<IPL_GetDisplayCtrlBufferNum(Id);i++)
    {
        CalApi()->fpBufEvent()->fpSetAddr(bufID,ime_p2_y_0+i,
            (pIplObj->pBaseInfo->IME_P2_PPB0.Ch[0].LineOfs)*(pIplObj->pBaseInfo->IME_P2_PPB0.Ch[0].Height));
        CalApi()->fpBufEvent()->fpSetAddr(bufID,ime_p2_uv_0+i,
            (pIplObj->pBaseInfo->IME_P2_PPB0.Ch[1].LineOfs)*(pIplObj->pBaseInfo->IME_P2_PPB0.Ch[1].Height));
    }

    CalApi()->fpBufEvent()->fpSetAddr(bufID,ipl_in,
    pIplObj->pBaseInfo->IPL_PPB0.LineOffset * pIplObj->pBaseInfo->IPL_PPB0.Height);

    CalApi()->fpBufEvent()->fpSetAddr(bufID,sie_ca_0, 32*32*8); // 1024 words
    CalApi()->fpBufEvent()->fpSetAddr(bufID,sie_ca_1, 32*32*8);
    CalApi()->fpBufEvent()->fpSetAddr(bufID,sie_la_0, 32*32*8);
    CalApi()->fpBufEvent()->fpSetAddr(bufID,sie_la_1, 32*32*8);

    CalApi()->fpBufEvent()->fpSetAddr(bufID,ipe_ppb_0,
    pIplObj->pBaseInfo->IPE_PPB.IPE_OUT.LineOfs * pIplObj->pBaseInfo->IPE_PPB.IPE_OUT.Height);
    CalApi()->fpBufEvent()->fpSetAddr(bufID,ipe_ppb_1,
    pIplObj->pBaseInfo->IPE_PPB.IPE_OUT.LineOfs * pIplObj->pBaseInfo->IPE_PPB.IPE_OUT.Height);
    CalApi()->fpBufEvent()->fpSetAddr(bufID,ife2_ppb_0,
    pIplObj->pBaseInfo->IPE_PPB.IPE_OUT.LineOfs * pIplObj->pBaseInfo->IPE_PPB.IPE_OUT.Height);
    CalApi()->fpBufEvent()->fpSetAddr(bufID,ife2_ppb_1,
    pIplObj->pBaseInfo->IPE_PPB.IPE_OUT.LineOfs * pIplObj->pBaseInfo->IPE_PPB.IPE_OUT.Height);

    CalApi()->fpBufEvent()->fpGetAddr(bufID,sie_raw_0,
    &pIplObj->pBaseInfo->SIE_PPB0.PixelAddr,NULL);
    RawBufNum = CAL_TRIGGER_BUF_NUM - 1;
    for (j = 0; j < RawBufNum; j ++)
    {
        CalApi()->fpBufEvent()->fpGetAddr(bufID,sie_CH0_0+j,
        &pIplObj->pBaseInfo->SIE_EXT_PPB[j],NULL);
    }

    CalApi()->fpBufEvent()->fpGetAddr(bufID,sie_ca_0,
    &pIplObj->CA_PPB.PPB0_Addr,NULL);
    CalApi()->fpBufEvent()->fpGetAddr(bufID,sie_ca_1,
    &pIplObj->CA_PPB.PPB1_Addr,NULL);

    CalApi()->fpBufEvent()->fpGetAddr(bufID,sie_ca_0,
    &pIplObj->pBaseInfo->SIE_CH1.PPB0_Addr,NULL);
    CalApi()->fpBufEvent()->fpGetAddr(bufID,sie_ca_1,
    &pIplObj->pBaseInfo->SIE_CH1.PPB1_Addr,NULL);

    CalApi()->fpBufEvent()->fpGetAddr(bufID,sie_la_0,
    &pIplObj->pBaseInfo->SIE_CH2.PPB0_Addr,NULL);
    CalApi()->fpBufEvent()->fpGetAddr(bufID,sie_la_1,
    &pIplObj->pBaseInfo->SIE_CH2.PPB1_Addr,NULL);

    CalApi()->fpBufEvent()->fpGetAddr(bufID,ipl_in,
    &pIplObj->pBaseInfo->IPL_PPB0.PixelAddr,NULL);

    CalApi()->fpBufEvent()->fpGetAddr(bufID,ime_p2_y_0,
    &pIplObj->pBaseInfo->IME_P2_PPB0.PixelAddr[0],NULL);
    CalApi()->fpBufEvent()->fpGetAddr(bufID,ime_p2_uv_0,
    &pIplObj->pBaseInfo->IME_P2_PPB0.PixelAddr[1],NULL);
    CalApi()->fpBufEvent()->fpGetAddr(bufID,ime_p2_uv_0,
    &pIplObj->pBaseInfo->IME_P2_PPB0.PixelAddr[2],NULL);

    /*if (Id == IPL_ID_2)
    {
        pIplObj->pBaseInfo->IME_P2_PPB0.PixelAddr[0] = pIplObj->pBaseInfo->IME_P2_PPB0.PixelAddr[0] + pIplObj->pBaseInfo->IME_P2_PPB0.Ch[0].LineOfs;
        pIplObj->pBaseInfo->IME_P2_PPB0.PixelAddr[1] = pIplObj->pBaseInfo->IME_P2_PPB0.PixelAddr[1] + pIplObj->pBaseInfo->IME_P2_PPB0.Ch[0].LineOfs;
        pIplObj->pBaseInfo->IME_P2_PPB0.PixelAddr[2] = pIplObj->pBaseInfo->IME_P2_PPB0.PixelAddr[2] + pIplObj->pBaseInfo->IME_P2_PPB0.Ch[0].LineOfs;
    }*/

    CalApi()->fpBufEvent()->fpGetAddr(bufID,ipe_ppb_0,
    &pIplObj->pBaseInfo->IPE_PPB.IPE_PPB0_ADDR,NULL);
    CalApi()->fpBufEvent()->fpGetAddr(bufID,ipe_ppb_1,
    &pIplObj->pBaseInfo->IPE_PPB.IPE_PPB1_ADDR,NULL);

    CalApi()->fpBufEvent()->fpGetAddr(bufID,ife2_ppb_0,
    &pIplObj->pBaseInfo->IFE2_PPB0_ADDR,NULL);
    CalApi()->fpBufEvent()->fpGetAddr(bufID,ife2_ppb_1,
    &pIplObj->pBaseInfo->IFE2_PPB1_ADDR,NULL);

    pIplObj->pBaseInfo->IME_P1_PPB0 = pIplObj->pBaseInfo->IME_P2_PPB0;
    pIplObj->pBaseInfo->IME_P1_PPB0.type = IPL_IMG_Y_PACK_UV422;
    pIplObj->pBaseInfo->IME_P3_PPB0 = pIplObj->pBaseInfo->IME_P2_PPB0;
    pIplObj->pBaseInfo->IME_P3_PPB0.type = IPL_IMG_Y_PACK_UV422;

    pIplObj->pBaseInfo->IME_P1_Crop.Start.x = pIplObj->pBaseInfo->IME_P2_Crop.Start.x;
    pIplObj->pBaseInfo->IME_P1_Crop.Start.y = pIplObj->pBaseInfo->IME_P2_Crop.Start.y;
    pIplObj->pBaseInfo->IME_P1_Crop.SizeH = pIplObj->pBaseInfo->IME_P2_Crop.SizeH;
    pIplObj->pBaseInfo->IME_P1_Crop.SizeV = pIplObj->pBaseInfo->IME_P2_Crop.SizeV;

    pIplObj->pBaseInfo->IME_P3_Crop.Start.x = pIplObj->pBaseInfo->IME_P2_Crop.Start.x;
    pIplObj->pBaseInfo->IME_P3_Crop.Start.y = pIplObj->pBaseInfo->IME_P2_Crop.Start.y;
    pIplObj->pBaseInfo->IME_P3_Crop.SizeH = pIplObj->pBaseInfo->IME_P2_Crop.SizeH;
    pIplObj->pBaseInfo->IME_P3_Crop.SizeV = pIplObj->pBaseInfo->IME_P2_Crop.SizeV;

    //IPL_SetDeviceInputInfo(1, imeInfo.Ch[0], Id);
}

#if 0
#endif
static void CalStation_CtrlInit(IPL_PROC_ID Id,pIPLC_INFO pIplObj,pCTRL_PTR pCtrlptr)
{
    SENSOR_INFO *pSenInfo = (SENSOR_INFO *)pCtrlptr->pDrv_Sensor;
    Cal_SENSOR_INFO *pCalInfo = (Cal_SENSOR_INFO*)pCtrlptr->pCal_Sensor;
    ISIZE disp_size;
    UINT32 i,j,RawBufNum;
    //IPL_YCC_IMG_INFO imeInfo;

    pIplObj->pBaseInfo = &m_BaseInfo;
    pIplObj->pExtInfo = &m_ExtInfo;

    _SIESignal(Id,pSenInfo,pCalInfo,&(pIplObj->pBaseInfo->SenSorIn));
    _SIEIn(Id,pSenInfo,pCalInfo,&(pIplObj->pBaseInfo->SIE_IN));
    _SIEAct(Id,pSenInfo,&(pIplObj->pBaseInfo->SIE_Act));
    _SIE_BURST_LENG(Id,&(pIplObj->pBaseInfo->BurstLength));

    pIplObj->Id = Id;
    pIplObj->pBaseInfo->SIE_PPB0.Bit = RAW_BIT;
    pIplObj->pBaseInfo->SIE_PPB0.Width = pIplObj->pBaseInfo->SIE_IN.Crop.SizeH;
    if (pIplObj->pBaseInfo->SIE_PPB0.Width > 2112)
        pIplObj->pBaseInfo->SIE_PPB0.Width = 2112;
    pIplObj->pBaseInfo->SIE_PPB0.Height = pIplObj->pBaseInfo->SIE_IN.Crop.SizeV;
    pIplObj->pBaseInfo->SIE_PPB0.LineOffset = pIplObj->pBaseInfo->SIE_PPB0.Width * pIplObj->pBaseInfo->SIE_PPB0.Bit / 8;
    pIplObj->pBaseInfo->SIE_PPB0.STPix = pSenInfo->Mode->StPix;

    pIplObj->pBaseInfo->SIE_CH1.packBus = RAW_BIT;
    pIplObj->pBaseInfo->SIE_CH1.HFlip = DISABLE;
    pIplObj->pBaseInfo->SIE_CH1.VFlip = DISABLE;
    pIplObj->pBaseInfo->SIE_CH1.LineOffset = IPL_UTI_RAWH_CONV2_LOFS(pIplObj->pBaseInfo->SIE_PPB0.Width, pIplObj->pBaseInfo->SIE_PPB0.Bit);

    pIplObj->pBaseInfo->SIE_CH2.packBus = RAW_BIT;
    pIplObj->pBaseInfo->SIE_CH2.HFlip = DISABLE;
    pIplObj->pBaseInfo->SIE_CH2.VFlip = DISABLE;
    pIplObj->pBaseInfo->SIE_CH2.LineOffset = IPL_UTI_RAWH_CONV2_LOFS(pIplObj->pBaseInfo->SIE_PPB0.Width, pIplObj->pBaseInfo->SIE_PPB0.Bit);

    pIplObj->pBaseInfo->SIE_Src = NonScaled_Img;

    pIplObj->pBaseInfo->SenHBinRatio = pSenInfo->Mode->Ratio.RatioH;
    pIplObj->pBaseInfo->SenVBinRatio = pSenInfo->Mode->Ratio.RatioV;

    pIplObj->pBaseInfo->IPL_PPB0.Width = pIplObj->pBaseInfo->SIE_PPB0.Width;
    pIplObj->pBaseInfo->IPL_PPB0.Height = pIplObj->pBaseInfo->SIE_PPB0.Height;
    pIplObj->pBaseInfo->IPL_StPos.x = (pIplObj->pBaseInfo->SIE_PPB0.Width - pIplObj->pBaseInfo->IPL_PPB0.Width)>>1;
    pIplObj->pBaseInfo->IPL_StPos.y = (pIplObj->pBaseInfo->SIE_PPB0.Height - pIplObj->pBaseInfo->IPL_PPB0.Height)>>1;
    pIplObj->pBaseInfo->IPL_PPB0.LineOffset = pIplObj->pBaseInfo->SIE_PPB0.LineOffset;
    pIplObj->pBaseInfo->IPL_PPB0.Bit = pIplObj->pBaseInfo->SIE_PPB0.Bit;
    pIplObj->pBaseInfo->IPL_PPB0.STPix = pIplObj->pBaseInfo->SIE_PPB0.STPix;

    pIplObj->pBaseInfo->DCE_Ref_1X.w = pSenInfo->Mode->TransHD[0].DataSize;
    pIplObj->pBaseInfo->DCE_Ref_1X.h = pSenInfo->Mode->TransVD[0].DataSize;
    pIplObj->pBaseInfo->DCE_Ref_1XCenter.x = pIplObj->pBaseInfo->DCE_Ref_1X.w >>1;
    pIplObj->pBaseInfo->DCE_Ref_1XCenter.y = pIplObj->pBaseInfo->DCE_Ref_1X.h >>1;

    pIplObj->pBaseInfo->IPE_PPB.IPE_OUT.Width = IPL_UTI_IPESUB_ADJ_HSIZE(pIplObj->pBaseInfo->IPL_PPB0.Width, 20, 4); //direct to IFE2 * 20%
    pIplObj->pBaseInfo->IPE_PPB.IPE_OUT.Height = IPL_UTI_IPESUB_ADJ_VSIZE(pIplObj->pBaseInfo->IPL_PPB0.Height, 20, 4); //direct to IFE2 * 20%
    pIplObj->pBaseInfo->IPE_PPB.IPE_OUT.LineOfs = IPL_YSIZEH_CONV2LOFS(IPL_IMG_PACK_YUV444, pIplObj->pBaseInfo->IPE_PPB.IPE_OUT.Width);

    disp_size = GxVideo_GetDeviceSize(DOUT1);

    pIplObj->pBaseInfo->IME_P2_PPB0.type = IPL_IMG_Y_PACK_UV420;
    pIplObj->pBaseInfo->IME_P2_PPB0.Ch[0].Width = disp_size.w;
    pIplObj->pBaseInfo->IME_P2_PPB0.Ch[0].Height = disp_size.h;
    pIplObj->pBaseInfo->IME_P2_PPB0.Ch[0].LineOfs = pIplObj->pBaseInfo->IME_P2_PPB0.Ch[0].Width;
    pIplObj->pBaseInfo->IME_P2_PPB0.Ch[1] = IPL_UTI_Y_INFO_CONV2(pIplObj->pBaseInfo->IME_P2_PPB0.type,pIplObj->pBaseInfo->IME_P2_PPB0.Ch[0]);
    pIplObj->pBaseInfo->IME_P2_PPB0.Ch[2] = IPL_UTI_Y_INFO_CONV2(pIplObj->pBaseInfo->IME_P2_PPB0.type,pIplObj->pBaseInfo->IME_P2_PPB0.Ch[0]);

    pIplObj->pBaseInfo->IME_P2_ENABLE = ENABLE;

    pIplObj->pBaseInfo->IME_P2_Crop.Start.x = 0;
    pIplObj->pBaseInfo->IME_P2_Crop.Start.y = 0;
    pIplObj->pBaseInfo->IME_P2_Crop.SizeH = disp_size.w;
    pIplObj->pBaseInfo->IME_P2_Crop.SizeV = disp_size.h;


    m_CAL_IPE_OUT_SIZE.Width = pIplObj->pBaseInfo->IPE_PPB.IPE_OUT.Width;
    m_CAL_IPE_OUT_SIZE.Height = pIplObj->pBaseInfo->IPE_PPB.IPE_OUT.Height;
    m_CAL_IPE_OUT_SIZE.LineOfs = IPL_YSIZEH_CONV2LOFS(IPL_IMG_PACK_YUV444, pIplObj->pBaseInfo->IPE_PPB.IPE_OUT.Width);
    pIplObj->pAdaptSetting = _AdaptSettingCBFlowC;

    pIplObj->pExtInfo->fp_SIEClk = _SIEClkCB;
    pIplObj->pExtInfo->fp_SIEClkSrc = _SIEClkSrcCB;
    pIplObj->pExtInfo->fp_SIEPclkSel = _SIEPclkSelCB;
    pIplObj->pExtInfo->fp_RDEClk = _RDEClkCB;
    pIplObj->pExtInfo->fp_IFEClk = _IFEClkCB;
    pIplObj->pExtInfo->fp_IFE2Clk = _IFE2ClkCB;
    pIplObj->pExtInfo->fp_DCEClk = _DCEClkCB;
    pIplObj->pExtInfo->fp_IPEClk = _IPEClkCB;
    pIplObj->pExtInfo->fp_IMEClk = _IMEClkCB;

    pIplObj->pExtInfo->fp_SieInit = _SieInitCB;
    pIplObj->pExtInfo->fp_RdeInit = _RdeInitCB;
    pIplObj->pExtInfo->fp_IfeInit = _IfeInitCB;
    pIplObj->pExtInfo->fp_DceInit = _DceInitCB;
    pIplObj->pExtInfo->fp_IpeInit = _IpeInitCB;
    pIplObj->pExtInfo->fp_ImeInit = _ImeInitCB;
    pIplObj->pExtInfo->fp_Ife2Init = _Ife2InitCB;

    UINT32 RawHeaderSize = IPL_HeaderRawGetBufSize();
    Cal_BufID bufID;
    bufID = CalApi()->fpBufEvent()->fpGetCalBufID(Id);

    CalApi()->fpBufEvent()->fpSetAddr(bufID,sie_raw_header_0,
    RawHeaderSize);
    CalApi()->fpBufEvent()->fpSetAddr(bufID,sie_raw_0,
    pIplObj->pBaseInfo->SIE_PPB0.LineOffset * pIplObj->pBaseInfo->SIE_PPB0.Height);

    CalApi()->fpBufEvent()->fpSetAddr(bufID,sie_raw_header_1,
    RawHeaderSize);
    CalApi()->fpBufEvent()->fpSetAddr(bufID,sie_raw_1,
    pIplObj->pBaseInfo->SIE_PPB0.LineOffset * pIplObj->pBaseInfo->SIE_PPB0.Height);

    RawBufNum = CAL_TRIGGER_BUF_NUM - 1;
    for (j = 0; j < RawBufNum; j ++)
    {
        CalApi()->fpBufEvent()->fpSetAddr(bufID,sie_CH0_header_0+j,
        RawHeaderSize);
        CalApi()->fpBufEvent()->fpSetAddr(bufID,sie_CH0_0+j,
        pIplObj->pBaseInfo->SIE_PPB0.LineOffset * pIplObj->pBaseInfo->SIE_PPB0.Height);
    }

    for(i = 0;i<IPL_GetDisplayCtrlBufferNum(Id);i++)
    {
        CalApi()->fpBufEvent()->fpSetAddr(bufID,ime_p2_y_0+i,
            (pIplObj->pBaseInfo->IME_P2_PPB0.Ch[0].LineOfs)*(pIplObj->pBaseInfo->IME_P2_PPB0.Ch[0].Height));
        CalApi()->fpBufEvent()->fpSetAddr(bufID,ime_p2_uv_0+i,
            (pIplObj->pBaseInfo->IME_P2_PPB0.Ch[1].LineOfs)*(pIplObj->pBaseInfo->IME_P2_PPB0.Ch[1].Height));
    }

    CalApi()->fpBufEvent()->fpSetAddr(bufID,ipl_in,
    pIplObj->pBaseInfo->IPL_PPB0.LineOffset * pIplObj->pBaseInfo->IPL_PPB0.Height);

    CalApi()->fpBufEvent()->fpSetAddr(bufID,sie_ca_0, 32*32*8); // 1024 words
    CalApi()->fpBufEvent()->fpSetAddr(bufID,sie_ca_1, 32*32*8);
    CalApi()->fpBufEvent()->fpSetAddr(bufID,sie_la_0, 32*32*8);
    CalApi()->fpBufEvent()->fpSetAddr(bufID,sie_la_1, 32*32*8);

    CalApi()->fpBufEvent()->fpSetAddr(bufID,ipe_ppb_0,
    pIplObj->pBaseInfo->IPE_PPB.IPE_OUT.LineOfs * pIplObj->pBaseInfo->IPE_PPB.IPE_OUT.Height);
    CalApi()->fpBufEvent()->fpSetAddr(bufID,ipe_ppb_1,
    pIplObj->pBaseInfo->IPE_PPB.IPE_OUT.LineOfs * pIplObj->pBaseInfo->IPE_PPB.IPE_OUT.Height);
    CalApi()->fpBufEvent()->fpSetAddr(bufID,ife2_ppb_0,
    pIplObj->pBaseInfo->IPE_PPB.IPE_OUT.LineOfs * pIplObj->pBaseInfo->IPE_PPB.IPE_OUT.Height);
    CalApi()->fpBufEvent()->fpSetAddr(bufID,ife2_ppb_1,
    pIplObj->pBaseInfo->IPE_PPB.IPE_OUT.LineOfs * pIplObj->pBaseInfo->IPE_PPB.IPE_OUT.Height);
    CalApi()->fpBufEvent()->fpGetAddr(bufID,sie_raw_0,
    &pIplObj->pBaseInfo->SIE_PPB0.PixelAddr,NULL);

    // same address for ring buffer
    RawBufNum = CAL_TRIGGER_BUF_NUM - 1;
    for (j = 0; j < RawBufNum; j ++)
    {
        CalApi()->fpBufEvent()->fpGetAddr(bufID,sie_raw_0,
        &pIplObj->pBaseInfo->SIE_EXT_PPB[j],NULL);
        /*CalApi()->fpBufEvent()->fpGetAddr(Id,sie_CH0_0+j,
        &pIplObj->pBaseInfo->SIE_EXT_PPB[j],NULL);*/
    }
    CalApi()->fpBufEvent()->fpGetAddr(bufID,sie_ca_0,
    &pIplObj->CA_PPB.PPB0_Addr,NULL);
    CalApi()->fpBufEvent()->fpGetAddr(bufID,sie_ca_1,
    &pIplObj->CA_PPB.PPB1_Addr,NULL);

    CalApi()->fpBufEvent()->fpGetAddr(bufID,sie_ca_0,
    &pIplObj->pBaseInfo->SIE_CH1.PPB0_Addr,NULL);
    CalApi()->fpBufEvent()->fpGetAddr(bufID,sie_ca_1,
    &pIplObj->pBaseInfo->SIE_CH1.PPB1_Addr,NULL);

    CalApi()->fpBufEvent()->fpGetAddr(bufID,sie_la_0,
    &pIplObj->pBaseInfo->SIE_CH2.PPB0_Addr,NULL);
    CalApi()->fpBufEvent()->fpGetAddr(bufID,sie_la_1,
    &pIplObj->pBaseInfo->SIE_CH2.PPB1_Addr,NULL);

    CalApi()->fpBufEvent()->fpGetAddr(bufID,ipl_in,
    &pIplObj->pBaseInfo->IPL_PPB0.PixelAddr,NULL);

    CalApi()->fpBufEvent()->fpGetAddr(bufID,ime_p2_y_0,
    &pIplObj->pBaseInfo->IME_P2_PPB0.PixelAddr[0],NULL);
    CalApi()->fpBufEvent()->fpGetAddr(bufID,ime_p2_uv_0,
    &pIplObj->pBaseInfo->IME_P2_PPB0.PixelAddr[1],NULL);
    CalApi()->fpBufEvent()->fpGetAddr(bufID,ime_p2_uv_0,
    &pIplObj->pBaseInfo->IME_P2_PPB0.PixelAddr[2],NULL);

    CalApi()->fpBufEvent()->fpGetAddr(bufID,ipe_ppb_0,
    &pIplObj->pBaseInfo->IPE_PPB.IPE_PPB0_ADDR,NULL);
    CalApi()->fpBufEvent()->fpGetAddr(bufID,ipe_ppb_1,
    &pIplObj->pBaseInfo->IPE_PPB.IPE_PPB1_ADDR,NULL);

    CalApi()->fpBufEvent()->fpGetAddr(bufID,ife2_ppb_0,
    &pIplObj->pBaseInfo->IFE2_PPB0_ADDR,NULL);
    CalApi()->fpBufEvent()->fpGetAddr(bufID,ife2_ppb_1,
    &pIplObj->pBaseInfo->IFE2_PPB1_ADDR,NULL);

    pIplObj->pBaseInfo->IME_P1_PPB0 = pIplObj->pBaseInfo->IME_P2_PPB0;
    pIplObj->pBaseInfo->IME_P1_PPB0.type = IPL_IMG_Y_PACK_UV422;
    pIplObj->pBaseInfo->IME_P3_PPB0 = pIplObj->pBaseInfo->IME_P2_PPB0;
    pIplObj->pBaseInfo->IME_P3_PPB0.type = IPL_IMG_Y_PACK_UV422;

    pIplObj->pBaseInfo->IME_P1_Crop.Start.x = pIplObj->pBaseInfo->IME_P2_Crop.Start.x;
    pIplObj->pBaseInfo->IME_P1_Crop.Start.y = pIplObj->pBaseInfo->IME_P2_Crop.Start.y;
    pIplObj->pBaseInfo->IME_P1_Crop.SizeH = pIplObj->pBaseInfo->IME_P2_Crop.SizeH;
    pIplObj->pBaseInfo->IME_P1_Crop.SizeV = pIplObj->pBaseInfo->IME_P2_Crop.SizeV;

    pIplObj->pBaseInfo->IME_P3_Crop.Start.x = pIplObj->pBaseInfo->IME_P2_Crop.Start.x;
    pIplObj->pBaseInfo->IME_P3_Crop.Start.y = pIplObj->pBaseInfo->IME_P2_Crop.Start.y;
    pIplObj->pBaseInfo->IME_P3_Crop.SizeH = pIplObj->pBaseInfo->IME_P2_Crop.SizeH;
    pIplObj->pBaseInfo->IME_P3_Crop.SizeV = pIplObj->pBaseInfo->IME_P2_Crop.SizeV;

    //IPL_SetDeviceInputInfo(1, imeInfo.Ch[0], Id);
}

#if 0
#endif
static void ISO_CtrlInit(IPL_PROC_ID Id,pIPLC_INFO pIplObj,pCTRL_PTR pCtrlptr)
{
    SENSOR_INFO *pSenInfo = (SENSOR_INFO *)pCtrlptr->pDrv_Sensor;
    Cal_SENSOR_INFO *pCalInfo = (Cal_SENSOR_INFO*)pCtrlptr->pCal_Sensor;

    m_sieUpdate = 0;
    memset((void *)&m_sie,0,sizeof(SIE));
    memset((void *)&m_sieSubF,0,sizeof(SIE_SUBFUNC));

    m_sie.pSubFunc = &m_sieSubF;
    pIplObj->psie = &m_sie;
    pIplObj->psieUpdate = &m_sieUpdate;

    _SIESignal(Id,pSenInfo,pCalInfo,&(pIplObj->psie->Sie_Signal));
    _SIEIn(Id,pSenInfo,pCalInfo,&(pIplObj->psie->IMG_Window));
    _SIEAct(Id,pSenInfo,&(pIplObj->psie->ActMode));
    _SIE_BURST_LENG(Id,&pIplObj->psie->BurstLength);

    pIplObj->psie->pSubFunc->InteEn |= (SIE__INTE_FLDEND);
    pIplObj->psie->bDir2IFE = DISABLE; //plz ref "FC_SIE_TOIFE"

    pIplObj->psie->CH0.Out.packBus = RAW_BIT;
    pIplObj->psie->CH0.SiePPBSW = SIE_BUF_OFF; //plz ref "FC_SIE_DMAOUT_CH0_PPB_BUF"
    pIplObj->psie->CH0.Src = NonScaled_Img;
    pIplObj->psie->DmaOutSizeH = pIplObj->psie->IMG_Window.Crop.SizeH;
    pIplObj->psie->DmaOutSizeV = pIplObj->psie->IMG_Window.Crop.SizeV;
    pIplObj->psie->CH0.Out.LineOffset = pIplObj->psie->DmaOutSizeH * pIplObj->psie->CH0.Out.packBus / 8;

    pIplObj->psie->CH0.Out.HFlip = DISABLE;
    pIplObj->psie->CH0.Out.VFlip = DISABLE;

    pIplObj->psie->SrcimgWin.SizX = pIplObj->psie->IMG_Window.Act.SizeH;
    pIplObj->psie->SrcimgWin.SizY = pIplObj->psie->IMG_Window.Act.SizeV;

    pIplObj->psie->pSubFunc->sieFuncEn |= (DMAOutCh0En);

    UINT32 RawHeaderSize = IPL_HeaderRawGetBufSize();
    Cal_BufID bufID;
    bufID = CalApi()->fpBufEvent()->fpGetCalBufID(Id);

    CalApi()->fpBufEvent()->fpSetAddr(bufID,iso_init_header,
    RawHeaderSize);

    CalApi()->fpBufEvent()->fpSetAddr(bufID,iso_init,
    pIplObj->psie->CH0.Out.LineOffset * pIplObj->psie->DmaOutSizeV);

    CalApi()->fpBufEvent()->fpGetAddr(bufID,iso_init,
    &pIplObj->psie->CH0.Out.PPB0_Addr,NULL);

    pIplObj->psie->CH0.Out.PPB1_Addr = pIplObj->psie->CH0.Out.PPB0_Addr;

    //active information update in SIE_INIT
    m_sieUpdate |= (SIE_OUTADDR_CH0|SIE_IOSIZE|SIE_SUBFEN|SIE_INIT|SIE_BURSTLENG|SIE_CH0Out_SRC);
    //m_sieUpdate |= (SIE_OUTADDR_CH0|SIE_IOSIZE|SIE_SUBFEN|SIE_BURSTLENG|SIE_CH0Out_SRC);
}

#if 0
#endif
static void AWB_CtrlInit(IPL_PROC_ID Id,pIPLC_INFO pIplObj,pCTRL_PTR pCtrlptr)
{
    SENSOR_INFO *pSenInfo = (SENSOR_INFO *)pCtrlptr->pDrv_Sensor;
    Cal_SENSOR_INFO *pCalInfo = (Cal_SENSOR_INFO*)pCtrlptr->pCal_Sensor;
    CAL_TAG* pTag;
    CAL_SECMAP CalSectionItem;

    m_sieUpdate = 0;
    memset((void *)&m_sie,0,sizeof(SIE));
    memset((void *)&m_sieSubF,0,sizeof(SIE_SUBFUNC));

    if (Id == IPL_ID_1)
    {
        CalSectionItem = SEC_ECS_1;
    }
    else if (Id == IPL_ID_2)
    {
        CalSectionItem = SEC_ECS_2;
    }
    else if (Id == IPL_ID_3)
    {
        CalSectionItem = SEC_ECS_3;
    }
    else if (Id == IPL_ID_4)
    {
        CalSectionItem = SEC_ECS_4;
    }
    else
    {
        CalSectionItem = SEC_ECS_1;
        DBG_ERR("IPL Id is out of range\r\n");
    }

    CAL_DATA_INFO CaldataInfo = {0};
    CaldataInfo.StationID = SEC_ECS;
    CaldataInfo.SenMode = pCalInfo->SenMode;
    CaldataInfo.CalCondition[0] = CAL_CONDITION_DEFAULT;

    pTag = GetCalData(Id, CaldataInfo);

    if(pTag)
    {
        if(pTag->CAL_STATUS == _CAL_OK)
        {
            CAL_ECS_FMT *pFmt;
            pFmt = (CAL_ECS_FMT *)&pTag->TagData;
            m_sieSubF.sieFuncEn |= (ECSEn);
            m_sieSubF.Ecs.EcsMap = pFmt->header.Ecs_map;
            m_Ecs.BitSel = pFmt->header.Ecs_bit;
            m_sieSubF.Ecs.pecs = &m_Ecs;
            m_sieSubF.Ecs.Inaddr = (UINT32)&pFmt->ECSBlockWin;
            m_sieUpdate |= (SIE_SUBFEN|SIE_ECS_);
        }
    }

    m_sie.pSubFunc = &m_sieSubF;
    pIplObj->psie = &m_sie;
    pIplObj->psieUpdate = &m_sieUpdate;

    _SIESignal(Id,pSenInfo,pCalInfo,&(pIplObj->psie->Sie_Signal));
    _SIEIn(Id,pSenInfo,pCalInfo,&(pIplObj->psie->IMG_Window));
    _SIEAct(Id,pSenInfo,&(pIplObj->psie->ActMode));
    _SIE_BURST_LENG(Id,&(pIplObj->psie->BurstLength));

    pIplObj->psie->pSubFunc->InteEn |= (SIE__INTE_FLDEND);
    pIplObj->psie->bDir2IFE = DISABLE; //plz ref "FC_SIE_TOIFE"

    pIplObj->psie->CH0.Out.packBus = RAW_BIT;
    pIplObj->psie->CH0.SiePPBSW = SIE_BUF_OFF; //plz ref "FC_SIE_DMAOUT_CH0_PPB_BUF"
    pIplObj->psie->CH0.Src = NonScaled_Img;
    pIplObj->psie->DmaOutSizeH = pIplObj->psie->IMG_Window.Crop.SizeH;
    pIplObj->psie->DmaOutSizeV = pIplObj->psie->IMG_Window.Crop.SizeV;
    pIplObj->psie->CH0.Out.LineOffset = pIplObj->psie->DmaOutSizeH * pIplObj->psie->CH0.Out.packBus / 8;

    pIplObj->psie->CH0.Out.HFlip = DISABLE;
    pIplObj->psie->CH0.Out.VFlip = DISABLE;

    pIplObj->psie->SrcimgWin.SizX = pIplObj->psie->IMG_Window.Act.SizeH;
    pIplObj->psie->SrcimgWin.SizY = pIplObj->psie->IMG_Window.Act.SizeV;

    pIplObj->psie->pSubFunc->sieFuncEn |= (DMAOutCh0En);

    UINT32 RawHeaderSize = IPL_HeaderRawGetBufSize();
    Cal_BufID bufID;
    bufID = CalApi()->fpBufEvent()->fpGetCalBufID(Id);

    CalApi()->fpBufEvent()->fpSetAddr(bufID,awb_init_header,
    RawHeaderSize);
    CalApi()->fpBufEvent()->fpSetAddr(bufID,awb_init,
    pIplObj->psie->CH0.Out.LineOffset * pIplObj->psie->DmaOutSizeV);

    CalApi()->fpBufEvent()->fpGetAddr(bufID,awb_init,
    &pIplObj->psie->CH0.Out.PPB0_Addr,NULL);

    pIplObj->psie->CH0.Out.PPB1_Addr = pIplObj->psie->CH0.Out.PPB0_Addr;

    //active information update in SIE_INIT
    m_sieUpdate |= (SIE_OUTADDR_CH0|SIE_IOSIZE|SIE_SUBFEN|SIE_INIT|SIE_BURSTLENG|SIE_CH0Out_SRC);
    //m_sieUpdate |= (SIE_OUTADDR_CH0|SIE_IOSIZE|SIE_SUBFEN|SIE_BURSTLENG|SIE_CH0Out_SRC);
}

#if 0
#endif
static void ECS_CtrlInit(IPL_PROC_ID Id,pIPLC_INFO pIplObj,pCTRL_PTR pCtrlptr)
{
    SENSOR_INFO *pSenInfo = (SENSOR_INFO *)pCtrlptr->pDrv_Sensor;
    Cal_SENSOR_INFO *pCalInfo = (Cal_SENSOR_INFO*)pCtrlptr->pCal_Sensor;

    m_sieUpdate = 0;
    memset((void *)&m_sie,0,sizeof(SIE));
    memset((void *)&m_sieSubF,0,sizeof(SIE_SUBFUNC));

    m_sie.pSubFunc = &m_sieSubF;
    pIplObj->psie = &m_sie;
    pIplObj->psieUpdate = &m_sieUpdate;

    _SIESignal(Id,pSenInfo,pCalInfo,&(pIplObj->psie->Sie_Signal));
    _SIEIn(Id,pSenInfo,pCalInfo,&(pIplObj->psie->IMG_Window));
    _SIEAct(Id,pSenInfo,&(pIplObj->psie->ActMode));
    _SIE_BURST_LENG(Id,&(pIplObj->psie->BurstLength));

    pIplObj->psie->pSubFunc->InteEn |= (SIE__INTE_FLDEND);
    pIplObj->psie->bDir2IFE = DISABLE; //plz ref "FC_SIE_TOIFE"

    pIplObj->psie->CH0.Out.packBus = RAW_BIT;
    pIplObj->psie->CH0.SiePPBSW = SIE_BUF_OFF; //plz ref "FC_SIE_DMAOUT_CH0_PPB_BUF"
    pIplObj->psie->CH0.Src = NonScaled_Img;
    pIplObj->psie->DmaOutSizeH = pIplObj->psie->IMG_Window.Crop.SizeH;
    pIplObj->psie->DmaOutSizeV = pIplObj->psie->IMG_Window.Crop.SizeV;
    pIplObj->psie->CH0.Out.LineOffset = pIplObj->psie->DmaOutSizeH * pIplObj->psie->CH0.Out.packBus / 8;

    pIplObj->psie->CH0.Out.HFlip = DISABLE;
    pIplObj->psie->CH0.Out.VFlip = DISABLE;

    pIplObj->psie->SrcimgWin.SizX = pIplObj->psie->IMG_Window.Act.SizeH;
    pIplObj->psie->SrcimgWin.SizY = pIplObj->psie->IMG_Window.Act.SizeV;

    pIplObj->psie->pSubFunc->sieFuncEn |= (DMAOutCh0En);

    UINT32 RawHeaderSize = IPL_HeaderRawGetBufSize();
    Cal_BufID bufID;
    bufID = CalApi()->fpBufEvent()->fpGetCalBufID(Id);

    CalApi()->fpBufEvent()->fpSetAddr(bufID,ecs_init_header,
    RawHeaderSize);

    CalApi()->fpBufEvent()->fpSetAddr(bufID,ecs_init,
    pIplObj->psie->CH0.Out.LineOffset * pIplObj->psie->DmaOutSizeV);

    CalApi()->fpBufEvent()->fpGetAddr(bufID,ecs_init,
    &pIplObj->psie->CH0.Out.PPB0_Addr,NULL);

    pIplObj->psie->CH0.Out.PPB1_Addr = pIplObj->psie->CH0.Out.PPB0_Addr;

    //active information update in SIE_INIT
    m_sieUpdate |= (SIE_OUTADDR_CH0|SIE_IOSIZE|SIE_SUBFEN|SIE_INIT|SIE_BURSTLENG|SIE_CH0Out_SRC);
    //m_sieUpdate |= (SIE_OUTADDR_CH0|SIE_IOSIZE|SIE_SUBFEN|SIE_BURSTLENG|SIE_CH0Out_SRC);
}

#if 0
#endif
static void DP_CtrlInit(IPL_PROC_ID Id,pIPLC_INFO pIplObj,pCTRL_PTR pCtrlptr)
{
    SENSOR_INFO *pSenInfo = (SENSOR_INFO *)pCtrlptr->pDrv_Sensor;
    Cal_SENSOR_INFO *pCalInfo = (Cal_SENSOR_INFO*)pCtrlptr->pCal_Sensor;

    m_sieUpdate = 0;
    memset((void *)&m_sie,0,sizeof(SIE));
    memset((void *)&m_sieSubF,0,sizeof(SIE_SUBFUNC));

    m_sie.pSubFunc = &m_sieSubF;
    pIplObj->psie = &m_sie;
    pIplObj->psieUpdate = &m_sieUpdate;

    _SIESignal(Id,pSenInfo,pCalInfo,&(pIplObj->psie->Sie_Signal));
    _SIEIn(Id,pSenInfo,pCalInfo,&(pIplObj->psie->IMG_Window));
    _SIEAct(Id,pSenInfo,&(pIplObj->psie->ActMode));
    _SIE_BURST_LENG(Id,&(pIplObj->psie->BurstLength));

    pIplObj->psie->pSubFunc->InteEn |= (SIE__INTE_FLDEND);
    pIplObj->psie->bDir2IFE = DISABLE; //plz ref "FC_SIE_TOIFE"

    pIplObj->psie->CH0.Out.packBus = RAW_BIT;
    pIplObj->psie->CH0.SiePPBSW = SIE_BUF_OFF; //plz ref "FC_SIE_DMAOUT_CH0_PPB_BUF"
    pIplObj->psie->CH0.Src = NonScaled_Img;
    pIplObj->psie->DmaOutSizeH = pIplObj->psie->IMG_Window.Crop.SizeH;
    pIplObj->psie->DmaOutSizeV = pIplObj->psie->IMG_Window.Crop.SizeV;
    pIplObj->psie->CH0.Out.LineOffset = pIplObj->psie->DmaOutSizeH * pIplObj->psie->CH0.Out.packBus / 8;

    pIplObj->psie->CH0.Out.HFlip = DISABLE;
    pIplObj->psie->CH0.Out.VFlip = DISABLE;

    pIplObj->psie->SrcimgWin.SizX = pIplObj->psie->IMG_Window.Act.SizeH;
    pIplObj->psie->SrcimgWin.SizY = pIplObj->psie->IMG_Window.Act.SizeV;

    pIplObj->psie->pSubFunc->sieFuncEn |= (DMAOutCh0En);

    UINT32 RawHeaderSize = IPL_HeaderRawGetBufSize();
    Cal_BufID bufID;
    bufID = CalApi()->fpBufEvent()->fpGetCalBufID(Id);

    CalApi()->fpBufEvent()->fpSetAddr(bufID,dp_init_header,
    RawHeaderSize);

    CalApi()->fpBufEvent()->fpSetAddr(bufID,dp_init,
    pIplObj->psie->CH0.Out.LineOffset * pIplObj->psie->DmaOutSizeV);

    CalApi()->fpBufEvent()->fpGetAddr(bufID,dp_init,
    &pIplObj->psie->CH0.Out.PPB0_Addr,NULL);

    pIplObj->psie->CH0.Out.PPB1_Addr = pIplObj->psie->CH0.Out.PPB0_Addr;

    //active information update in SIE_INIT
    m_sieUpdate |= (SIE_OUTADDR_CH0|SIE_IOSIZE|SIE_SUBFEN|SIE_INIT|SIE_BURSTLENG|SIE_CH0Out_SRC);
    //m_sieUpdate |= (SIE_OUTADDR_CH0|SIE_IOSIZE|SIE_SUBFEN|SIE_BURSTLENG|SIE_CH0Out_SRC);
}

#if 0
#endif
static void MSHUT_CtrlInit(IPL_PROC_ID Id,pIPLC_INFO pIplObj,pCTRL_PTR pCtrlptr)
{
#if 0
    SENSOR_INFO *pSenInfo = (SENSOR_INFO *)pCtrlptr->pDrv_Sensor;
    Cal_SENSOR_INFO *pCalInfo = (Cal_SENSOR_INFO*)pCtrlptr->pCal_Sensor;

    m_sieUpdate = 0;
    memset((void *)&m_sie,0,sizeof(SIE));
    memset((void *)&m_sieSubF,0,sizeof(SIE_SUBFUNC));

    m_sie.pSubFunc = &m_sieSubF;
    pIplObj->psie = &m_sie;
    pIplObj->psieUpdate = &m_sieUpdate;

    _SIESignal(Id,pSenInfo,pCalInfo,&(pIplObj->psie->Sie_Signal));
    _SIEIn(Id,pSenInfo,pCalInfo,&(pIplObj->psie->IMG_Window));
    _SIEAct(Id,pSenInfo,&(pIplObj->psie->ActMode));
    _SIE_BURST_LENG(Id,&(pIplObj->psie->BurstLength));

    pIplObj->psie->pSubFunc->InteEn |= (SIE__INTE_FLDEND);
    pIplObj->psie->bDir2IFE = DISABLE; //plz ref "FC_SIE_TOIFE"

    pIplObj->psie->CH0.Out.packBus = RAW_BIT;
    pIplObj->psie->CH0.SiePPBSW = SIE_BUF_OFF; //plz ref "FC_SIE_DMAOUT_CH0_PPB_BUF"
    pIplObj->psie->CH0.Src = NonScaled_Img;
    pIplObj->psie->DmaOutSizeH = pIplObj->psie->IMG_Window.Crop.SizeH;
    pIplObj->psie->DmaOutSizeV = pIplObj->psie->IMG_Window.Crop.SizeV;
    pIplObj->psie->CH0.Out.LineOffset = pIplObj->psie->DmaOutSizeH * pIplObj->psie->CH0.Out.packBus / 8;

    pIplObj->psie->CH0.Out.HFlip = DISABLE;
    pIplObj->psie->CH0.Out.VFlip = DISABLE;

    pIplObj->psie->SrcimgWin.SizX = pIplObj->psie->IMG_Window.Act.SizeH;
    pIplObj->psie->SrcimgWin.SizY = pIplObj->psie->IMG_Window.Act.SizeV;

    pIplObj->psie->pSubFunc->sieFuncEn |= (DMAOutCh0En);

    CalApi()->fpBufEvent()->fpSetAddr(Id,Id,
    pIplObj->psie->CH0.Out.LineOffset * pIplObj->psie->DmaOutSizeV);

    CalApi()->fpBufEvent()->fpGetAddr(Id,Id,
    &pIplObj->psie->CH0.Out.PPB0_Addr,NULL);

    pIplObj->psie->CH0.Out.PPB1_Addr = pIplObj->psie->CH0.Out.PPB0_Addr;

    m_sieUpdate |= (SIE_OUTADDR_CH0|SIE_IOSIZE|SIE_SUBFEN|SIE_INIT|SIE_BURSTLENG);
#endif
}

static void LENSCEN_CtrlInit(IPL_PROC_ID Id,pIPLC_INFO pIplObj,pCTRL_PTR pCtrlptr)
{
    SENSOR_INFO *pSenInfo = (SENSOR_INFO *)pCtrlptr->pDrv_Sensor;
    Cal_SENSOR_INFO *pCalInfo = (Cal_SENSOR_INFO*)pCtrlptr->pCal_Sensor;

    m_sieUpdate = 0;
    memset((void *)&m_sie,0,sizeof(SIE));
    memset((void *)&m_sieSubF,0,sizeof(SIE_SUBFUNC));

    m_sie.pSubFunc = &m_sieSubF;
    pIplObj->psie = &m_sie;
    pIplObj->psieUpdate = &m_sieUpdate;

    _SIESignal(Id,pSenInfo,pCalInfo,&(pIplObj->psie->Sie_Signal));
    _SIEIn(Id,pSenInfo,pCalInfo,&(pIplObj->psie->IMG_Window));
    _SIEAct(Id,pSenInfo,&(pIplObj->psie->ActMode));
    _SIE_BURST_LENG(Id,&(pIplObj->psie->BurstLength));

    pIplObj->psie->pSubFunc->InteEn |= (SIE__INTE_FLDEND);
    pIplObj->psie->bDir2IFE = DISABLE; //plz ref "FC_SIE_TOIFE"

    pIplObj->psie->CH0.Out.packBus = RAW_BIT;
    pIplObj->psie->CH0.SiePPBSW = SIE_BUF_OFF; //plz ref "FC_SIE_DMAOUT_CH0_PPB_BUF"
    pIplObj->psie->CH0.Src = NonScaled_Img;
    pIplObj->psie->DmaOutSizeH = pIplObj->psie->IMG_Window.Crop.SizeH;
    pIplObj->psie->DmaOutSizeV = pIplObj->psie->IMG_Window.Crop.SizeV;
    pIplObj->psie->CH0.Out.LineOffset = pIplObj->psie->DmaOutSizeH * pIplObj->psie->CH0.Out.packBus / 8;

    pIplObj->psie->CH0.Out.HFlip = DISABLE;
    pIplObj->psie->CH0.Out.VFlip = DISABLE;

    pIplObj->psie->SrcimgWin.SizX = pIplObj->psie->IMG_Window.Act.SizeH;
    pIplObj->psie->SrcimgWin.SizY = pIplObj->psie->IMG_Window.Act.SizeV;

    pIplObj->psie->pSubFunc->sieFuncEn |= (DMAOutCh0En);

    UINT32 RawHeaderSize = IPL_HeaderRawGetBufSize();
    Cal_BufID bufID;
    bufID = CalApi()->fpBufEvent()->fpGetCalBufID(Id);

    CalApi()->fpBufEvent()->fpSetAddr(bufID,lenscen_init_header,
    RawHeaderSize);

    CalApi()->fpBufEvent()->fpSetAddr(bufID,lenscen_init,
    pIplObj->psie->CH0.Out.LineOffset * pIplObj->psie->DmaOutSizeV);

    CalApi()->fpBufEvent()->fpGetAddr(bufID,lenscen_init,
    &pIplObj->psie->CH0.Out.PPB0_Addr,NULL);

    pIplObj->psie->CH0.Out.PPB1_Addr = pIplObj->psie->CH0.Out.PPB0_Addr;

    //active information update in SIE_INIT
    m_sieUpdate |= (SIE_OUTADDR_CH0|SIE_IOSIZE|SIE_SUBFEN|SIE_INIT|SIE_BURSTLENG|SIE_CH0Out_SRC);
    //m_sieUpdate |= (SIE_OUTADDR_CH0|SIE_IOSIZE|SIE_SUBFEN|SIE_BURSTLENG|SIE_CH0Out_SRC);
}

#if 0
#endif
static CAL_CTRL_OBJ m_CTRL_OBJ = {
    Prv_CtrlInit,
    CalStation_CtrlInit,
    ISO_CtrlInit,
    AWB_CtrlInit,
    ECS_CtrlInit,
    MSHUT_CtrlInit,
    DP_CtrlInit,
    LENSCEN_CtrlInit
};

pCAL_CTRL_OBJ Cal_CtrlInit(void)
{
    return &m_CTRL_OBJ;
}
