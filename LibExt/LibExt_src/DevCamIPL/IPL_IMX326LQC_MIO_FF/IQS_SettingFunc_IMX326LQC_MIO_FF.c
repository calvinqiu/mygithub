#ifndef _IQS_SETTINGFUNC_IMX326LQC_MIO_FF_C
#define _IQS_SETTINGFUNC_IMX326LQC_MIO_FF_C
#include "IPL_IMX326LQC_MIO_FF_Int.h"
#include "HwMem.h"
#include "VideoEncode.h"
#include "h264enc_api.h"

UINT32 IQS_IsCapFlow(IQS_FLOW_MSG MsgID)
{
    if ( (MsgID >= IQCB_CAP_SUB_START && MsgID <= IQCB_CAP_SUB_END) ||
         (MsgID >= IQCB_CAP_PRI_START && MsgID <= IQCB_CAP_PRI_END) )
        return TRUE;
    else
        return FALSE;
}

///////////////////////////////////////////////
// Flow Export
///////////////////////////////////////////////
UINT32 IQS_OBSubMode(UINT32 Id, IQS_FLOW_MSG MsgID)
{
    SENSOR_INFO SenInfo;
    SenInfo.Mode = NULL;

    if ( IQS_IsCapFlow(MsgID) == TRUE )
        IPL_GetSensorStatus(Id, IPL_AlgGetUIInfo(Id, IPL_SEL_CAPSENMODE), &SenInfo);
    else
        IPL_GetSensorStatus(Id, IPL_AlgGetUIInfo(Id, IPL_SEL_PRVSENMODE), &SenInfo);
    if (SenInfo.Mode == NULL)
    {
        DBG_ERR("NULL SenInfo.Mode\r\n");
        return 0;
    }
//#NT#2016/05/17#Ted -begin
//#NT# for WDR 2.5 USE SIE OB SUB
    if (SenInfo.Mode->ModeType == SENSOR_MODE_STAGGER_HDR)
        return IQS_SIE_OBSUB;
    else if (IPL_CtrlGetInfor(Id, IPLCTRL_CFMODE) == IPL_CF_RHEWDR)
        return IQS_SIE_OBSUB;
    else
        return IQS_IFE_OBSUB;
//#NT#2016/05/17#Ted -end
}

void IQS_GetRefImageInfo(UINT32 Id, INT32 SensorMode, IQS_REFIMG_INFO *Info)
{
    SENSOR_INFO SenInfo={0};
    IPL_GetSensorStatus(Id, SensorMode, &SenInfo);

    Info->width = SenInfo.Width1X;
    Info->height = SenInfo.Height1X;
    if (SenInfo.Mode != NULL)
    {
        Info->hratio = SenInfo.Mode->Ratio.RatioH;
        Info->vratio = SenInfo.Mode->Ratio.RatioV;
    }
    else
    {
        DBG_ERR("Get Ratio without sensor mode\r\n");
    }
}

UINT32 IQS_GetMSNRRepTime(UINT32 Id)
{
    UINT32 ISO_Value;
    ISO_Value = IQS_GetISOIdx(Id, IQCB_CAP_PRI_START);
    return IQS_MSNR[ISO_Value].uiProcRepNum;
}

UINT32 IQS_GetIFE2LcaTime(UINT32 Id, IQS_FLOW_MSG MsgID)  //PRV & CAP
{
    UINT32 ISO_Value;
    ISO_Value = IQS_GetISOIdx(Id, MsgID);
    ////DBG_IND("LCATimes = %d", IFE2_Filter_Times[ISO_Value].LCATimes);
    return IQS_IFE2Filter[ISO_Value].RepNum;
}

UINT32 IQS_GetChromaAdaptRatio(UINT32 Id, BOOL IsCap, BOOL GetMaxRatio)
{
    //get IQS min chroma adaption ratio in streaming mode
    //for IPL prepare buffer
    UINT32 aRatio = 0;
    UINT32 ISO_Value = 0;
    UINT32 i, s = 0, e = 0;
    if ( IsCap == TRUE )
    {
        ISO_Value = IQS_GetISOIdx(Id, IQCB_CAP_PRI_START);
        return IQS_ChromaAdapt[ISO_Value].SubRatio;
    }
    else if ( GetMaxRatio == FALSE )
    {
        ISO_Value = IQS_GetISOIdx(Id, IQCB_PRV_START);
        return IQS_ChromaAdapt[ISO_Value].SubRatio;
    }
    else
    {
        if ( Id == IPL_ID_1 )
        {
            s = IQS_PRV1_ISOSTART;
            e = IQS_PRV1_ISOEND;
        }
        else if ( Id == IPL_ID_2 )
        {
            s = IQS_PRV2_ISOSTART;
            e = IQS_PRV2_ISOEND;
        }
        else
        {
            DBG_ERR("wrong Id (%d)\r\n", Id);
            return 100;
        }
        for ( i=s; i<=e; i++ )
        {
            if ( IQS_ChromaAdapt[i].SubRatio <= 0 || IQS_ChromaAdapt[i].SubRatio >= 100)
                DBG_ERR("invalidate IME subratio (%d)\r\n", IQS_ChromaAdapt[i].SubRatio);
            if ( aRatio < IQS_ChromaAdapt[i].SubRatio )
                aRatio = IQS_ChromaAdapt[i].SubRatio;
        }
        return aRatio;
    }
    return 100;
}

UINT32 IQS_IsChromaAdaptEn(UINT32 Id, BOOL IsCap)
{
    UINT32 ISO_Value = 0;
    if ( IsCap == TRUE )
    {
        ISO_Value = IQS_GetISOIdx(Id, IQCB_CAP_PRI_START);
        if ( IQS_IFE2Filter[ISO_Value].RepNum == 0 )
            return DISABLE;
        if ( IQS_ChromaAdapt[ISO_Value].SubRatio == 0 )
        {
            DBG_ERR("invalidate IME subratio (%d)\r\n", IQS_ChromaAdapt[ISO_Value].SubRatio);
            return DISABLE;
        }
        return ENABLE;
    }
    else
    {
        return ENABLE;
    }
    return DISABLE;
}

void IQS_DCE_ParamSetting(UINT32 Id, UINT32 MsgID, UINT32 ZoomIdx, DCE_Distortion* pDistortion, DIST* pDce_Dist, Fact_Norm* pRadious)
{
    UINT32 ImgWidth=0, ImgHeight=0;
    extern UINT32 IQS_DCE_GLUT_Buf[Total_SENIDNum][65];

    if ( IQS_IsCapFlow(MsgID) ) //capture
    {
        ImgWidth  = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_H);
        ImgHeight = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_V);
    }
    else                        //preview
    {
        ImgWidth  = IPL_CtrlGetInfor(Id, IPLCTRL_IPL_IN_SIZE_H);
        ImgHeight = IPL_CtrlGetInfor(Id, IPLCTRL_IPL_IN_SIZE_V);
    }

    pDistortion->Center.x = ImgWidth>>1;
    pDistortion->Center.y = ImgHeight>>1;
    pDistortion->Fov = IQS_DCE_Fov[ZoomIdx];
    pDistortion->Enh = IQS_DCE_Enh[ZoomIdx];

    pDistortion->ColorAber = IQS_DCE_ColorAbbration[ZoomIdx];
    pDistortion->ColorAber.CACGain.Fact_G = 4096;   // 4096 = 1x
#if IQS_RATIOTABLE_ENABLE
    pDistortion->ColorAber.CACGain.Fact_R = (UINT32)IQS_CLAMP((INT32)IQS_DCE_ColorAbbration[Id].CACGain.Fact_R + IQS_CAC_Table[Id][ADJ_IQRatio_CAC_R], 4055, 4137);
    pDistortion->ColorAber.CACGain.Fact_B = (UINT32)IQS_CLAMP((INT32)IQS_DCE_ColorAbbration[Id].CACGain.Fact_B + IQS_CAC_Table[Id][ADJ_IQRatio_CAC_B], 4055, 4137);
    if(((IQS_DCE_ColorAbbration[Id].CACGain.Fact_R + IQS_CAC_Table[Id][ADJ_IQRatio_CAC_R]) > 4137) || ((IQS_DCE_ColorAbbration[Id].CACGain.Fact_R + IQS_CAC_Table[Id][ADJ_IQRatio_CAC_R]) < 4055))
        DBG_ERR("CAC_R value is over the boundary!!!!\r\n");
    if(((IQS_DCE_ColorAbbration[Id].CACGain.Fact_B + IQS_CAC_Table[Id][ADJ_IQRatio_CAC_B]) > 4137) || ((IQS_DCE_ColorAbbration[Id].CACGain.Fact_B + IQS_CAC_Table[Id][ADJ_IQRatio_CAC_B]) < 4055))
        DBG_ERR("CAC_B value is over the boundary!!!!\r\n");
#else
    pDistortion->ColorAber.CACGain.Fact_R = (UINT32)IQS_CLAMP(IQS_DCE_ColorAbbration[Id].CACGain.Fact_R, 4055, 4137);
    pDistortion->ColorAber.CACGain.Fact_B = (UINT32)IQS_CLAMP(IQS_DCE_ColorAbbration[Id].CACGain.Fact_B, 4055, 4137);
#endif

    pDistortion->UV_FilterEn = FALSE;

    pDistortion->GeoLUT.pGGeoLut = IQS_DCE_GLUT_Buf[ZoomIdx];
    pDistortion->GeoLUT.pRGeoLut = IQS_DCE_RLUT[ZoomIdx];
    pDistortion->GeoLUT.pBGeoLut = IQS_DCE_BLUT[ZoomIdx];

    if ( IQS_IsCapFlow(MsgID) ) //capture
        pDistortion->Fov.Gain = DCE_GetFovGain(Id, TRUE, pDistortion->GeoLUT.pGGeoLut);
    else                        //preview
        pDistortion->Fov.Gain = DCE_GetFovGain(Id, FALSE, pDistortion->GeoLUT.pGGeoLut);

    *pDce_Dist = IQS_DCE_Dist[ZoomIdx];
    *pRadious = IQS_DCE_Radious[ZoomIdx];
}

UINT32 IQS_GetGammaValue(UINT32 Id, UINT32 Data12Bit)
{
    INT32 idx, YLum, LumIdx, LumIdx1, data = Data12Bit;
    UINT32* GammaLUT_128Tab;
    SENSOR_INFO SenInfo;
    UINT32 SenId_Value = IQS_GetSensorIdx(Id);

    SenInfo.Mode = NULL;
    IPL_GetSensorStatus(Id, IPL_AlgGetUIInfo(Id, IPL_SEL_PRVSENMODE), &SenInfo);
    if (SenInfo.Mode == NULL)
    {
        DBG_ERR("NULL SenInfo.Mode\r\n");
        return 0;
    }

    GammaLUT_128Tab = IQS_GammaLut[SenId_Value];

    if (data < 1)
    {
        idx = 0;
        LumIdx = 0;
    }
    else
    {
        idx = (data * 128) / 4096;
        LumIdx = (idx * 4096) / 128;
    }
    LumIdx1 = ((idx + 1) * 4096) / 128;

    YLum = ((GammaLUT_128Tab[(idx + 1)] - GammaLUT_128Tab[idx]) * (data - LumIdx)) / (LumIdx1 - LumIdx);
    YLum += GammaLUT_128Tab[idx];

    if ((YLum < 0) || (YLum > 1023))
    {

        DBG_ERR("%d %d %d %d %d\r\n", idx, LumIdx, LumIdx1, GammaLUT_128Tab[idx], GammaLUT_128Tab[(idx + 1)]);

        DBG_ERR("data = %d YLum = %d\r\n", data, YLum);
    }
    return YLum;
}

UINT32 IQS_GetInvGammaValue(UINT32 Id, UINT32 Data10Bit)
{
    UINT32 i;
    UINT32 ChkFlag = FALSE;
    UINT32 Idx1 = 0, Idx = 0;
    UINT64 RawLum;
    UINT32* GammaLUT_128Tab;
    SENSOR_INFO SenInfo;
    UINT32 SenId_Value = IQS_GetSensorIdx(Id);

    SenInfo.Mode = NULL;
    IPL_GetSensorStatus(Id, IPL_AlgGetUIInfo(Id, IPL_SEL_PRVSENMODE), &SenInfo);
    if (SenInfo.Mode == NULL)
    {
        DBG_ERR("NULL SenInfo.Mode\r\n");
        return 0;
    }

    if (Data10Bit < 1)
    {
        RawLum = 0;
        return RawLum;
    }

    GammaLUT_128Tab =  IQS_GammaLut[SenId_Value];

    for (i = 1; i < 130; i ++)
    {
        if (Data10Bit <= GammaLUT_128Tab[i])
        {
            Idx1 = i;
            Idx = (Idx1 - 1);
            ChkFlag = TRUE;
            break;
        }
    }

    if ( (GammaLUT_128Tab[Idx1] - GammaLUT_128Tab[Idx]) == 0)
    {
        RawLum = (Idx << 10);
        RawLum = ((RawLum << 12) >> 7) >> 10;

        if (RawLum > 4096)
        {
            DBG_ERR("%d %d %d %d %d\r\n", Idx, Idx1, Data10Bit, GammaLUT_128Tab[Idx], GammaLUT_128Tab[Idx1]);
        }
        else if (RawLum == 4096)
        {
            RawLum = 4095;
        }

        return RawLum;
    }

    if (ChkFlag == TRUE)
    {
        RawLum = ( (UINT64)((Idx1 - Idx) * (Data10Bit - GammaLUT_128Tab[Idx])) << 10) / (GammaLUT_128Tab[Idx1] - GammaLUT_128Tab[Idx]);
        RawLum += (Idx << 10);
        RawLum = ((RawLum << 12) >> 7) >> 10;

        if (RawLum > 4096)
        {
            DBG_ERR("%d %d %d %d %d\r\n", Idx, Idx1, Data10Bit, GammaLUT_128Tab[Idx], GammaLUT_128Tab[Idx1]);
        }
        else if (RawLum == 4096)
        {
            RawLum = 4095;
        }

        return RawLum;
    }
    else
    {
        DBG_ERR("input overflow %d\r\n", Data10Bit);
        return 0;
    }
}

////////////////////////////////////
// Movie MCTF setting
////////////////////////////////////

//#NT#2016/10/17#Wendy Liao -begin
//#NT#Define 3DNR parameters in LibExt
H264Enc3dnrCfg UserDefine3DNR_CARDV_LV0 =
{   FALSE,// enable 3DNR
    TRUE,// enable pixel blend
    {0, 0, 0},// P2P and MCTF Threshold and Wight ----"Threshold"
    {0, 0, 0},// P2P and MCTF Threshold and Wight ----"Threshold Weighting map"
    // P2P 3DNR Config---------------------------------------------------------------------------
    {   {                 // Luma Config: Temporal Filter
            {0, 0, 0},    //I
            {0, 0, 0},    //B
            {0, 0, 0},    //B
            {0, 0, 0}   },//P
        {0, 0, 0},     // Luma Config: Range Filter
        {                 // Chroma Config: Temporal Filter
            {0, 0, 0},    //I
            {0, 0, 0},    //B
            {0, 0, 0},    //B
            {0, 0, 0}   },//P
        {0, 0, 0},     // Chroma Config: Range Filter
        2, 1, 2, 1 },   // Clamp Config :uiClampY_TH,Y uiClampY_Div, uiClampUV_TH, uiClampUV_Div
    // MCTF 3DNR Config----------------------------------------------------------------------------
    {   {                 // Luma Config: Temporal Filter
            {0, 0, 0},    //I
            {0, 0, 0},    //B
            {0, 0, 0},    //B
            {0, 0, 0}   },//P
        {0, 0, 0},     // Luma Config: Range Filter
        {                 // Chroma Config: Temporal Filter
            {0, 0, 0},    //I
            {0, 0, 0},    //B
            {0, 0, 0},    //B
            {0, 0, 0}   },//P
        {0, 0, 0},     // Chroma Config: Range Filter
        2, 1, 2, 1  }   // Clamp Config :uiClampY_TH,Y uiClampY_Div, uiClampUV_TH, uiClampUV_Div
};

H264Enc3dnrCfg UserDefine3DNR_CARDV_LV1 =
{   TRUE,// enable 3DNR
    TRUE,// enable pixel blend
    {3, 7, 12},// P2P and MCTF Threshold and Wight ----"Threshold"
    {8, 6, 2},// P2P and MCTF Threshold and Wight ----"Threshold Weighting map"
    // P2P 3DNR Config---------------------------------------------------------------------------
    {   {                 // Luma Config: Temporal Filter
            {2, 2, 2},    //I
            {2, 2, 2},    //B
            {2, 2, 2},    //B
            {3, 2, 3}   },//P
        {6, 10, 16},     // Luma Config: Range Filter
        {                 // Chroma Config: Temporal Filter
            {2, 2, 2},    //I
            {2, 2, 2},    //B
            {2, 2, 2},    //B
            {3, 2, 3}   },//P
        {0, 2, 3},     // Chroma Config: Range Filter
        2, 1, 2, 1 },   // Clamp Config :uiClampY_TH,Y uiClampY_Div, uiClampUV_TH, uiClampUV_Div
    // MCTF 3DNR Config----------------------------------------------------------------------------
    {   {                 // Luma Config: Temporal Filter
            {2, 2, 2},    //I
            {2, 2, 2},    //B
            {2, 2, 2},    //B
            {3, 2, 3}   },//P
        {5, 6, 7},     // Luma Config: Range Filter
        {                 // Chroma Config: Temporal Filter
            {2, 2, 2},    //I
            {2, 2, 2},    //B
            {2, 2, 2},    //B
            {3, 2, 3}   },//P
        {0, 2, 3},     // Chroma Config: Range Filter
        2, 1, 2, 1  }   // Clamp Config :uiClampY_TH,Y uiClampY_Div, uiClampUV_TH, uiClampUV_Div
};

H264Enc3dnrCfg UserDefine3DNR_CARDV_LV2 =
{   TRUE,// enable 3DNR
    TRUE,// enable pixel blend
    {4, 10, 14},// P2P and MCTF Threshold and Wight ----"Threshold"
    {8, 6, 4},// P2P and MCTF Threshold and Wight ----"Threshold Weighting map"
    // P2P 3DNR Config---------------------------------------------------------------------------
    {   {                 // Luma Config: Temporal Filter
            {2, 2, 2},    //I
            {2, 2, 2},    //B
            {2, 2, 2},    //B
            {3, 2, 3}   },//P
        {6, 12, 20},     // Luma Config: Range Filter
        {                 // Chroma Config: Temporal Filter
            {2, 2, 2},    //I
            {2, 2, 2},    //B
            {2, 2, 2},    //B
            {3, 2, 3}   },//P
        {6, 8, 10},     // Chroma Config: Range Filter
        6, 2, 6, 2 },   // Clamp Config :uiClampY_TH,Y uiClampY_Div, uiClampUV_TH, uiClampUV_Div
    // MCTF 3DNR Config----------------------------------------------------------------------------
    {   {                 // Luma Config: Temporal Filter
            {2, 2, 2},    //I
            {2, 2, 2},    //B
            {2, 2, 2},    //B
            {3, 2, 3}   },//P
        {6, 8, 10},     // Luma Config: Range Filter
        {                 // Chroma Config: Temporal Filter
            {2, 2, 2},    //I
            {2, 2, 2},    //B
            {2, 2, 2},    //B
            {3, 2, 3}   },//P
        {6, 8, 10},     // Chroma Config: Range Filter
        2, 1, 2, 1  }   // Clamp Config :uiClampY_TH,Y uiClampY_Div, uiClampUV_TH, uiClampUV_Div
};

H264Enc3dnrCfg UserDefine3DNR_CARDV_LV3 =
{   TRUE,// enable 3DNR
    TRUE,// enable pixel blend
    {4, 10, 14},// P2P and MCTF Threshold and Wight ----"Threshold"
    {8, 6, 4},// P2P and MCTF Threshold and Wight ----"Threshold Weighting map"
    // P2P 3DNR Config---------------------------------------------------------------------------
    {   {                 // Luma Config: Temporal Filter
            {2, 2, 2},    //I
            {2, 2, 2},    //B
            {2, 2, 2},    //B
            {4, 2, 4}   },//P
        {10, 20, 30},     // Luma Config: Range Filter
        {                 // Chroma Config: Temporal Filter
            {2, 2, 2},    //I
            {2, 2, 2},    //B
            {2, 2, 2},    //B
            {4, 2, 4}   },//P
        {8, 12, 16},     // Chroma Config: Range Filter
        6, 2, 6, 2 },   // Clamp Config :uiClampY_TH,Y uiClampY_Div, uiClampUV_TH, uiClampUV_Div
    // MCTF 3DNR Config----------------------------------------------------------------------------
    {   {                 // Luma Config: Temporal Filter
            {2, 2, 2},    //I
            {2, 2, 2},    //B
            {2, 2, 2},    //B
            {4, 2, 4}   },//P
        {8, 12, 16},     // Luma Config: Range Filter
        {                 // Chroma Config: Temporal Filter
            {2, 2, 2},    //I
            {2, 2, 2},    //B
            {2, 2, 2},    //B
            {4, 2, 4}   },//P
        {8, 10, 14},     // Chroma Config: Range Filter
        6, 2, 6, 2  }   // Clamp Config :uiClampY_TH,Y uiClampY_Div, uiClampUV_TH, uiClampUV_Div
};

H264Enc3dnrCfg UserDefine3DNR_CARDV_LV4 =
{   TRUE,// enable 3DNR
    TRUE,// enable pixel blend
    {5, 10, 16},// P2P and MCTF Threshold and Wight ----"Threshold"
    {8, 6, 4},// P2P and MCTF Threshold and Wight ----"Threshold Weighting map"
    // P2P 3DNR Config---------------------------------------------------------------------------
    {   {                 // Luma Config: Temporal Filter
            {2, 2, 2},    //I
            {2, 2, 2},    //B
            {2, 2, 2},    //B
            {4, 2, 4}   },//P
        {12, 24, 40},     // Luma Config: Range Filter
        {                 // Chroma Config: Temporal Filter
            {2, 2, 2},    //I
            {2, 2, 2},    //B
            {2, 2, 2},    //B
            {4, 2, 4}   },//P
        {10, 16, 22},     // Chroma Config: Range Filter
        6, 2, 6, 2 },   // Clamp Config :uiClampY_TH,Y uiClampY_Div, uiClampUV_TH, uiClampUV_Div
    // MCTF 3DNR Config----------------------------------------------------------------------------
    {   {                 // Luma Config: Temporal Filter
            {2, 2, 2},    //I
            {2, 2, 2},    //B
            {2, 2, 2},    //B
            {4, 2, 4}   },//P
//        {12, 24, 30},     // Luma Config: Range Filter
        {10, 16, 26},     // Luma Config: Range Filter
        {                 // Chroma Config: Temporal Filter
            {2, 2, 2},   //I
            {2, 2, 2},    //B
            {2, 2, 2},    //B
            {4, 2, 4}   },//P
        {10, 16, 22},     // Chroma Config: Range Filter
        6, 2, 6, 2  }   // Clamp Config :uiClampY_TH,Y uiClampY_Div, uiClampUV_TH, uiClampUV_Div
};

H264Enc3dnrCfg UserDefine3DNR_CARDV_LV5 =
{   TRUE,// enable 3DNR
    TRUE,// enable pixel blend
    {6, 12, 20},// P2P and MCTF Threshold and Wight ----"Threshold"
    {8, 6, 4},// P2P and MCTF Threshold and Wight ----"Threshold Weighting map"
    // P2P 3DNR Config---------------------------------------------------------------------------
    {   {                 // Luma Config: Temporal Filter
            {3, 2, 3},    //I
            {2, 2, 2},    //B
            {2, 2, 2},    //B
            {6, 2, 6}   },//P
        {15, 25, 40},     // Luma Config: Range Filter
        {                 // Chroma Config: Temporal Filter
            {3, 2, 3},    //I
            {2, 2, 2},    //B
            {2, 2, 2},    //B
            {6, 2, 6}   },//P
        {10, 16, 30},     // Chroma Config: Range Filter
        0, 2, 0, 2 },   // Clamp Config :uiClampY_TH,Y uiClampY_Div, uiClampUV_TH, uiClampUV_Div
    // MCTF 3DNR Config----------------------------------------------------------------------------
    {   {                 // Luma Config: Temporal Filter
            {3, 2, 3},     //I
            {2, 2, 2},    //B
            {2, 2, 2},    //B
            {6, 2, 6}   },//P
//        {12, 24, 30},     // Luma Config: Range Filter
        {10, 16, 30},     // Luma Config: Range Filter
        {                 // Chroma Config: Temporal Filter
            {3, 2, 3},    //I
            {2, 2, 2},    //B
            {2, 2, 2},    //B
            {6, 2, 6}   },//P
        {10, 16, 30},     // Chroma Config: Range Filter
        0, 2, 0, 2  }   // Clamp Config :uiClampY_TH,Y uiClampY_Div, uiClampUV_TH, uiClampUV_Div
};

void Movie_UIDefine_3DNR(UINT32 level, UINT32 config)
{
    //DBG_DUMP("========Movie_UIDefine_3DNR========\r\n");
    switch (level)
    {
    case H264_3DNR_CarDV_LV1:
        memcpy((void *)config, (void *)&UserDefine3DNR_CARDV_LV1, sizeof(H264Enc3dnrCfg));
        break;
    case H264_3DNR_CarDV_LV2:
        memcpy((void *)config, (void *)&UserDefine3DNR_CARDV_LV2, sizeof(H264Enc3dnrCfg));
        break;
    case H264_3DNR_CarDV_LV3:
        memcpy((void *)config, (void *)&UserDefine3DNR_CARDV_LV3, sizeof(H264Enc3dnrCfg));
        break;
    case H264_3DNR_CarDV_LV4:
        memcpy((void *)config, (void *)&UserDefine3DNR_CARDV_LV4, sizeof(H264Enc3dnrCfg));
        break;
    case H264_3DNR_CarDV_LV5:
    case H264_3DNR_CarDV_LV6:
    case H264_3DNR_CarDV_LV7:
    case H264_3DNR_CarDV_LV8:
    case H264_3DNR_CarDV_LV9:
    case H264_3DNR_CarDV_LV10:
        memcpy((void *)config, (void *)&UserDefine3DNR_CARDV_LV5, sizeof(H264Enc3dnrCfg));
        break;
    default:
        memcpy((void *)config, (void *)&UserDefine3DNR_CARDV_LV0, sizeof(H264Enc3dnrCfg));
        if (level != H264_3DNR_DISABLE)
        {
            DBG_ERR("Wrong parameter %d, disable 3DNR!\r\n", level);
        }
        break;
    }
}
//#NT#2016/10/17#Wendy Liao -end

#define T_CLAMP(in,lb,ub) (((INT32)(in) < (INT32)(lb)) ? (INT32)(lb) : (((INT32)(in) > (INT32)(ub)) ? (INT32)(ub) : (INT32)(in)))

UINT32 g3DNR[IPL_ID_MAX_NUM]={0}, g3DNR_Counter[IPL_ID_MAX_NUM]={0};
//#NT#2016/10/24#Wendy Liao -begin
//#NT#Add 3DNR control uart command
INT32 UI_3DNR[IPL_ID_MAX_NUM] = {-1};
//#NT#2016/10/24#Wendy Liao -end

UINT32 IQS_GetMCTFLevel(UINT32 Id)
{
    UINT32 ui3DNRLevel = 0, ui3DNRLevel_ini = H264_3DNR_LV0;
    UINT32 uiISO1, uiExpT1, uiIris1;
    AE_GetPrvAEArg(Id, &uiISO1, &uiExpT1, &uiIris1);
    //#NT#2016/06/03#Ted -begin
    //#NT# single sensor use single config
    // if(Id == 0)
    {
        if     (uiISO1 <= 75)   ui3DNRLevel = H264_3DNR_LV1;
        else if(uiISO1 <= 300)  ui3DNRLevel = H264_3DNR_LV1;
        else if(uiISO1 <= 600)  ui3DNRLevel = H264_3DNR_LV2;
        else if(uiISO1 <= 1200) ui3DNRLevel = H264_3DNR_LV3;
        else if(uiISO1 <= 2400) ui3DNRLevel = H264_3DNR_LV4;
        else if(uiISO1 <= 4800) ui3DNRLevel = H264_3DNR_LV5;
        else                    ui3DNRLevel = H264_3DNR_LV6;
    }
    /* else
    {
        if     (uiISO1 <= 75)   ui3DNRLevel = H264_3DNR_LV1;
        else if(uiISO1 <= 300)  ui3DNRLevel = H264_3DNR_LV1;
        else if(uiISO1 <= 600)  ui3DNRLevel = H264_3DNR_LV2;
        else if(uiISO1 <= 1200) ui3DNRLevel = H264_3DNR_LV3;
        else if(uiISO1 <= 2400) ui3DNRLevel = H264_3DNR_LV4;
        else if(uiISO1 <= 4800) ui3DNRLevel = H264_3DNR_LV5;
        else                    ui3DNRLevel = H264_3DNR_LV6;
    }*/
    //#NT#2016/06/03#Ted -end
if (IQS_IPCAM_ENABLE)
{
    UINT32 SenId_Value = IQS_GetSensorIdx(Id);
    if( IQS_IPCAM_RatioTable[SenId_Value].nr3D.ratio == 0 )
        ui3DNRLevel = H264_3DNR_IPCAM_LV0;
    else
    {
        ui3DNRLevel = ui3DNRLevel + IQS_IPCAM_RatioTable[SenId_Value].nr3D.ratio - 5;
    }
}

#if IQUC_ENABLE
    extern UINT32 IQUC_CtrlIndex[IQUC_ID_MAX_NUM];
    extern IQUC_CTRL_ITEM IQUC_CtrlItem[IQUC_ID_MAX_NUM][IQUC_CTRLITEM_MAX_NUM];
    ui3DNRLevel = T_CLAMP((INT32)((INT32)ui3DNRLevel+IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_TNR]), H264_3DNR_LV0, H264_3DNR_LV10);

//    IQUC_CTRL_ITEM* pIQUserAdj = IQUCIF_GetCtrlItem(Id, IQUCIF_GetCtrlIndex(Id));
//    if(pIQUserAdj != NULL)
//        ui3DNRLevel = T_CLAMP((INT32)((INT32)ui3DNRLevel+pIQUserAdj->ADJ_ITEM[ADJ_TNR]), H264_3DNR_LV0, H264_3DNR_LV10);
#endif

    g3DNR[Id]=ui3DNRLevel_ini+(INT32)((ui3DNRLevel-ui3DNRLevel_ini)*T_CLAMP(g3DNR_Counter[Id],0,10)/10);
    g3DNR_Counter[Id]++;

    if(IQS_IPCAM_ENABLE)
        g3DNR[Id] = IQS_CLAMP(ui3DNRLevel, H264_3DNR_IPCAM_LV0, H264_3DNR_IPCAM_LV13);
    else
        g3DNR[Id] = IQS_CLAMP(ui3DNRLevel, H264_3DNR_CarDV_LV0, H264_3DNR_CarDV_LV10);

//#NT#2016/10/24#Wendy Liao -begin
//#NT#Add 3DNR control uart command
    if(UI_3DNR[Id] >=0)
    {
        g3DNR[Id]=UI_3DNR[Id];
    }
//#NT#2016/10/24#Wendy Liao -end

    return g3DNR[Id];
}

UINT32 IQS_Rst3DNRCounter(void)
{
    UINT32 i;
    for ( i = 0; i<IPL_ID_MAX_NUM; i++ )
    {
        g3DNR_Counter[i] = 0;
    }
    return 0;
}
#endif
