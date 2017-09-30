#ifndef _IQS_SETTINGFUNC_IQSIM_EVB_FF_C
#define _IQS_SETTINGFUNC_IQSIM_EVB_FF_C
#include "IPL_IQSIM_EVB_FF_Int.h"
#include "HwMem.h"
#include "VideoEncode.h"
#include "FileSysTsk.h"
#include "Random.h"
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
void IQS_GetRefImageInfo(UINT32 Id, INT32 SensorMode, IQS_REFIMG_INFO *Info)
{
    IQSIM_APPINFO *AppInfo;
    AppInfo = (IQSIM_APPINFO*)IQSIM_GetAppInfo();

    Info->width = AppInfo->IQSimPriInfo.RawInfo.Size.w;
    Info->height = AppInfo->IQSimPriInfo.RawInfo.Size.h;

    Info->hratio = 1000;
    Info->vratio = 1000;
}

UINT32 IQS_GetMSNRRepTime(UINT32 Id)
{
    IQSIM_APPINFO *AppInfo;
    AppInfo = (IQSIM_APPINFO*)IQSIM_GetAppInfo();
    DBG_ERR("IQS_MSNR.uiProcRepNum %d\r\n", AppInfo->IQSimPriInfo.IQS_MSNR.uiProcRepNum);
    return AppInfo->IQSimPriInfo.IQS_MSNR.uiProcRepNum;
}

UINT32 IQS_GetIFE2LcaTime(UINT32 Id, IQS_FLOW_MSG MsgID)  //PRV & CAP
{
    IQSIM_APPINFO *AppInfo;
    AppInfo = (IQSIM_APPINFO*)IQSIM_GetAppInfo();
    DBG_ERR("IQS_IFE2Filter.RepNum %d\r\n", AppInfo->IQSimPriInfo.IQS_IFE2Filter.RepNum);
    return AppInfo->IQSimPriInfo.IQS_IFE2Filter.RepNum;
}

UINT32 IQS_GetChromaAdaptRatio(UINT32 Id)
{
    IQSIM_APPINFO *AppInfo;
    AppInfo = (IQSIM_APPINFO*)IQSIM_GetAppInfo();
    DBG_ERR("IQS_ChromaAdapt.SubRatio %d\r\n", AppInfo->IQSimPriInfo.IQS_ChromaAdapt.SubRatio);
    return AppInfo->IQSimPriInfo.IQS_ChromaAdapt.SubRatio;
}

UINT32 IQS_IsChromaAdaptEn(UINT32 Id, BOOL IsCap)
{
    IQSIM_APPINFO *AppInfo;
    AppInfo = (IQSIM_APPINFO*)IQSIM_GetAppInfo();
    if ( IsCap == TRUE )
    {
        if ( AppInfo->IQSimPriInfo.IQS_ChromaAdapt.SubRatio == 0 )
            return DISABLE;
        if ( AppInfo->IQSimPriInfo.IQS_IFE2Filter.RepNum == 0 )
            return DISABLE;
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
}



UINT32 IQS_GetGammaValue(UINT32 Id, UINT32 Data12Bit)
{
    INT32 idx, YLum, LumIdx, LumIdx1, data = Data12Bit;
    UINT32* GammaLUT_128Tab;
    SENSOR_INFO SenInfo;

    SenInfo.Mode = NULL;
    IPL_GetSensorStatus(Id, IPL_AlgGetUIInfo(Id, IPL_SEL_PRVSENMODE), &SenInfo);
    if (SenInfo.Mode == NULL)
    {
        DBG_ERR("NULL SenInfo.Mode\r\n");
        return 0;
    }

    GammaLUT_128Tab = IQS_GammaLut[Id];

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

    GammaLUT_128Tab =  IQS_GammaLut[Id];

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

void Movie_UIDefine_3DNR(UINT32 level, UINT32 config)
{
    return;
    //DBG_DUMP("========Movie_UIDefine_3DNR========\r\n");
}
//#NT#2016/10/17#Wendy Liao -end

INT32    IQS_MCTF_AE_AVG_old[IPL_ID_MAX_NUM]; //12bits
UINT32   IQS_MCTF_KeepCnt[IPL_ID_MAX_NUM];

void IQS_ResetMCTF(UINT32 Id)
{
    UINT32 i;
    for ( i = 0; i<IPL_ID_MAX_NUM; i++ )
    {
        IQS_MCTF_AE_AVG_old[i] = 0;
        IQS_MCTF_KeepCnt[i] = 0;
    }
}

//#NT#2016/10/24#Wendy Liao -begin
//#NT#Add 3DNR control uart command
INT32 UI_3DNR[IPL_ID_MAX_NUM] = {-1};
//#NT#2016/10/24#Wendy Liao -end

UINT32 IQS_GetMCTFLevel(UINT32 Id)
{
    UINT32  uiISO1, uiExpT1, uiIris1;
    INT32   iAE_AVG;
    UINT32  ui3DNRLevel;

    iAE_AVG=AE_GetCurRawLum(Id, 12);

    AE_GetPrvAEArg(Id, &uiISO1, &uiExpT1, &uiIris1);
    if(((abs(IQS_MCTF_AE_AVG_old[Id]-iAE_AVG)>(10*16))|(IQS_MCTF_KeepCnt[Id])) && IQS_MCTF_AE_AVG_old[Id]!=0 )
    {
        ui3DNRLevel = H264_3DNR_LV1;
        IQS_MCTF_KeepCnt[Id]++;
        if(IQS_MCTF_KeepCnt[Id]>3)
            {IQS_MCTF_KeepCnt[Id]=0;}
    }
    else
    {
        if     (uiISO1 <= 75)   ui3DNRLevel = H264_3DNR_LV1;
        else if(uiISO1 <= 300)  ui3DNRLevel = H264_3DNR_LV1;
        else if(uiISO1 <= 600)  ui3DNRLevel = H264_3DNR_LV2;
        else if(uiISO1 <= 1200) ui3DNRLevel = H264_3DNR_LV3;
        else if(uiISO1 <= 2400) ui3DNRLevel = H264_3DNR_LV4;
        else if(uiISO1 <= 4800) ui3DNRLevel = H264_3DNR_LV5;
        else                    ui3DNRLevel = H264_3DNR_LV6;
   }

    IQS_MCTF_AE_AVG_old[Id]=iAE_AVG;

    return ui3DNRLevel;
}

/////////////////////////////////////////
static void SetTestChInfo(IQSIM_CHINFO* ChInfo, UINT32 ISOIdx)
{
    UINT32 i, j;
    IQSIM_APPINFO *AppInfo;
    AppInfo = (IQSIM_APPINFO*)IQSIM_GetAppInfo();

    ChInfo->IQS_OBSub.Offset = 0x0;
    ChInfo->IQS_ECSMap = ecs65x65;
    memcpy(&ChInfo->IQS_ECSSet, &IQS_ECSSet, sizeof(SIE_EmbeddedCS));
    memcpy(ChInfo->IQS_ECSTab, IQS_ECSTab[ZOOM1], sizeof(UINT32)*4225);

    ////////////////////////////////////////////////
    //IFE
    ////////////////////////////////////////////////
    memcpy(ChInfo->IQS_CGain           ,  IQS_CGain[0]                , sizeof(UINT32        )*4 );
    memcpy(ChInfo->IQS_CGain_Offset    ,  IQS_CGain_Offset[0]         , sizeof(UINT32        )*4 );
    memcpy(&ChInfo->IQS_ColorGain       , &IQS_ColorGain[0]            , sizeof(IFE_CG        )*1 );
    memcpy(ChInfo->IQS_Vig_RLut        ,  IQS_Vig_RLut[0]             , sizeof(UINT32        )*17 );
    memcpy(ChInfo->IQS_Vig_GLut        ,  IQS_Vig_GLut[0]             , sizeof(UINT32        )*17 );
    memcpy(ChInfo->IQS_Vig_BLut        ,  IQS_Vig_BLut[0]             , sizeof(UINT32        )*17 );
    memcpy(&ChInfo->IQS_Vig             , &IQS_Vig[0]                  , sizeof(IFE_VIG       )*1 );
    memcpy(ChInfo->IQS_Outl_Bri        ,  IQS_Outl_Bri[0]             , sizeof(UINT32        )*5 );
    memcpy(ChInfo->IQS_Outl_Dark       ,  IQS_Outl_Dark[0]            , sizeof(UINT32        )*5 );
    memcpy(&ChInfo->IQS_Outl            , &IQS_Outl[0]                 , sizeof(IFE_OUTL      )*1 );
    memcpy(ChInfo->IQS_SF_Weight       ,  IQS_SF_Weight[0]            , sizeof(UINT32        )*6 );
    memcpy(&ChInfo->IQS_SpatialFilter   , &IQS_SpatialFilter[0]        , sizeof(Spatial_Filter)*1 );
    memcpy(ChInfo->IQS_RangeA_Thr      ,  IQS_RangeA_Thr[0]           , sizeof(UINT32        )*6 );
    memcpy(ChInfo->IQS_RangeA_Lut      ,  IQS_RangeA_Lut[0]           , sizeof(UINT32        )*17 );
    memcpy(&ChInfo->IQS_RangeA          , &IQS_RangeA[0]               , sizeof(RangeCFGA     )*1 );
    memcpy(ChInfo->IQS_RangeB_Thr      ,  IQS_RangeB_Thr[0]           , sizeof(UINT32        )*6 );
    memcpy(ChInfo->IQS_RangeB_Lut      ,  IQS_RangeB_Lut[0]           , sizeof(UINT32        )*17 );
    memcpy(&ChInfo->IQS_RangeB          , &IQS_RangeB[0]               , sizeof(RangeCFGB     )*1 );
    memcpy(&ChInfo->IQS_RangeFilter     , &IQS_RangeFilter[0]          , sizeof(Range_Filter  )*1 );
    memcpy(&ChInfo->IQS_Filter          , &IQS_Filter[0]               , sizeof(IFE_FILT      )*1 );
    memcpy(ChInfo->IQS_Gbal_Stab       ,  IQS_Gbal_Stab[0]            , sizeof(UINT8         )*16 );
    memcpy(ChInfo->IQS_Gbal_Dtab       ,  IQS_Gbal_Dtab[0]            , sizeof(UINT8         )*16 );
    memcpy(&ChInfo->IQS_Gbal            , &IQS_Gbal[0]                 , sizeof(IFE_GBAL      )*1 );

    ////////////////////////////////////////////////
    // DCE
    ////////////////////////////////////////////////
    IQS_Distortion[0].Center.x = AppInfo->IQSimPriInfo.RawInfo.Size.w;
    IQS_Distortion[0].Center.y = AppInfo->IQSimPriInfo.RawInfo.Size.h;
    IQS_Distortion[0].Fov = IQS_DCE_Fov[0];
    IQS_Distortion[0].Enh = IQS_DCE_Enh[0];
    IQS_Distortion[0].ColorAber = IQS_DCE_ColorAbbration[0];
    IQS_Distortion[0].GeoLUT.pGGeoLut = IQS_DCE_GLUT[0];
    IQS_Distortion[0].GeoLUT.pRGeoLut = IQS_DCE_RLUT[0];
    IQS_Distortion[0].GeoLUT.pBGeoLut = IQS_DCE_BLUT[0];
    IQS_Distortion[0].UV_FilterEn = DISABLE;
    memcpy(&ChInfo->IQS_Distortion,  &IQS_Distortion[0],  sizeof(DCE_Distortion));
    memcpy(&ChInfo->IQS_DCE_Radious, &IQS_DCE_Radious[0], sizeof(Fact_Norm));
    memcpy(&ChInfo->IQS_DCE_Dist,    &IQS_DCE_Dist[0],    sizeof(DIST));
    memcpy(ChInfo->IQS_DCE_GLUT,     IQS_DCE_GLUT[0],    sizeof(UINT32)*65);
    memcpy(&ChInfo->IQS_DCE_RLUT,    &IQS_DCE_RLUT[0],    sizeof(INT32)*65);
    memcpy(&ChInfo->IQS_DCE_BLUT,    &IQS_DCE_BLUT[0],    sizeof(INT32)*65);
    memcpy(&ChInfo->IQS_CFAInter,    &IQS_CFAInter[0],    sizeof(DCE_CFAPARAM));

    ////////////////////////////////////////////////
    // IPE
    ////////////////////////////////////////////////
    memcpy(ChInfo->IQS_EdgeGammaLut    ,  IQS_EdgeGammaLut         , sizeof(UINT32          )*65 );
    memcpy(&ChInfo->IQS_EdgeMap         , &IQS_EdgeMap[0]           , sizeof(IPE_EDGEMAP     )*1  );
    memcpy(&ChInfo->IQS_EdgeExt         , &IQS_EdgeExt[0]           , sizeof(IPE_EDGEEXT     )*1  );
    memcpy(&ChInfo->IQS_Cctrl           , &IQS_Cctrl[0]             , sizeof(IPE_CCTRL       )*1  );
    memcpy(&ChInfo->IQS_CCOfs           , &IQS_CCOfs[0]             , sizeof(IPE_CCOFS       )*1  );
    memcpy(&ChInfo->IQS_YCCcon          , &IQS_YCCcon[0]            , sizeof(IPE_CCON        )*1  );
    memcpy(ChInfo->IQS_EdgeKernel      ,  IQS_EdgeKernel[0]        , sizeof(INT16           )*12 );
    memcpy(ChInfo->IQS_EDirTab         ,  IQS_EDirTab[0]           , sizeof(UINT8           )*8  );
    memcpy(&ChInfo->IQS_DirEdgeExt      , &IQS_DirEdgeExt[0]        , sizeof(IPE_DEE         )*1  );
    memcpy(&ChInfo->IQS_DirScoreTab     , &IQS_DirScoreTab[0]       , sizeof(IPE_DirScorCal  )*1  );
    memcpy(&ChInfo->IQS_DirScoreTh      , &IQS_DirScoreTh[0]        , sizeof(IPE_DirScorCalTh)*1  );
    memcpy(&ChInfo->IQS_DECon           , &IQS_DECon[0]             , sizeof(IPE_DEConParam  )*1  );
    memcpy(&ChInfo->IQS_EWA             , &IQS_EWA[0]               , sizeof(IPE_KAEdgeW     )*1  );
    memcpy(&ChInfo->IQS_EWB             , &IQS_EWB[0]               , sizeof(IPE_KBEdgeW     )*1  );
    memcpy(&ChInfo->IQS_EWParam         , &IQS_EWParam[0]           , sizeof(IPE_EdgeW       )*1  );
    memcpy(&ChInfo->IQS_EdgeEnh         , &IQS_EdgeEnh[0]           , sizeof(IPE_EDGEENH     )*1  );
    memcpy(ChInfo->IQS_EDTab           ,  IQS_EDTab[0]             , sizeof(UINT8           )*16 );
    memcpy(ChInfo->IQS_EStab           ,  IQS_EStab[0]             , sizeof(UINT8           )*16 );
    memcpy(&ChInfo->IQS_EDMap           , &IQS_EDMap[0]             , sizeof(EdgeMap         )*1  );
    memcpy(&ChInfo->IQS_ESMap           , &IQS_ESMap[0]             , sizeof(EdgeMapS        )*1  );
    memcpy(&ChInfo->IQS_RGBLpf          , &IQS_RGBLpf[0]            , sizeof(IPE_RGBLPF      )*1  );
    memcpy(ChInfo->IQS_FStab           ,  IQS_FStab[0]             , sizeof(UINT8           )*16 );
    memcpy(ChInfo->IQS_FDtab           ,  IQS_FDtab[0]             , sizeof(UINT8           )*16 );
    memcpy(ChInfo->IQS_ColorMatrix     ,  IQS_ColorMatrix[0]       , sizeof(UINT16          )*9  );
    memcpy(&ChInfo->IQS_ColorCorrection , &IQS_ColorCorrection[0]   , sizeof(IPE_CC          )*1  );
    memcpy(ChInfo->IQS_hueTab          ,  IQS_hueTab[0]            , sizeof(UINT8           )*24 );
    memcpy(ChInfo->IQS_satTab          ,  IQS_satTab[0]            , sizeof(INT32           )*24 );
    memcpy(ChInfo->IQS_intTab          ,  IQS_intTab[0]            , sizeof(INT32           )*24 );
    memcpy(ChInfo->IQS_edgTab          ,  IQS_edgTab[0]            , sizeof(UINT8           )*24 );
    memcpy(ChInfo->IQS_ddsTab          ,  IQS_ddsTab[0]            , sizeof(UINT8           )*8  );
    memcpy(ChInfo->IQS_CSTCoef         ,  IQS_CSTCoef[0]           , sizeof(INT16           )*9  );
    memcpy(&ChInfo->IQS_CSTParam        , &IQS_CSTParam[0]          , sizeof(IPE_CST         )*1  );
    memcpy(&ChInfo->IQS_NoiseParam      , &IQS_NoiseParam[0]        , sizeof(IPE_RAND_NR     )*1  );
    memcpy(&ChInfo->IQS_GammaRand       , &IQS_GammaRand[0]         , sizeof(IPE_GamRand     )*1  );
    memcpy(ChInfo->IQS_YCurveLut       ,  IQS_YCurveLut[0]         , sizeof(UINT32          )*130);
    memcpy(ChInfo->IQS_GammaLut        ,  IQS_GammaLut[0]          , sizeof(UINT32          )*130);
    memcpy(&ChInfo->IQS_YCCFix          , &IQS_YCCFix[0]            , sizeof(IPE_YCCFIX      )*1  );
    memcpy(&ChInfo->IQS_3DColorCtrl     , &IQS_3DColorCtrl       , sizeof(IPE_3DCC        )*1  );

    ////////////////////////////////////////////////
    // IFE2
    ////////////////////////////////////////////////
    memcpy(&ChInfo->IQS_GrayStatical,   &IQS_GrayStatical,   sizeof(IFE2_GRAY_Statal));
    memcpy(ChInfo->IQS_RefCenter_YTh,   IQS_RefCenter_YTh,   sizeof(UINT32)*3);
    memcpy(ChInfo->IQS_RefCenter_YWt,   IQS_RefCenter_YWt,   sizeof(UINT32)*4);
    memcpy(&ChInfo->IQS_RefCenter_Y,    &IQS_RefCenter_Y,    sizeof(IFE2_REFCENT_Set));
    memcpy(ChInfo->IQS_RefCenter_UVTh,  IQS_RefCenter_UVTh,  sizeof(UINT32)*3);
    memcpy(ChInfo->IQS_RefCenter_UVWt,  IQS_RefCenter_UVWt,  sizeof(UINT32)*4);
    memcpy(&ChInfo->IQS_RefCenter_UV,   &IQS_RefCenter_UV,   sizeof(IFE2_REFCENT_Set));
    memcpy(ChInfo->IQS_IFE2Filter_YTh,  IQS_IFE2Filter_YTh,  sizeof(UINT32)*5);
    memcpy(ChInfo->IQS_IFE2Filter_YWt,  IQS_IFE2Filter_YWt,  sizeof(UINT32)*6);
    memcpy(ChInfo->IQS_IFE2Filter_UTh,  IQS_IFE2Filter_UTh,  sizeof(UINT32)*5);
    memcpy(ChInfo->IQS_IFE2Filter_UWt,  IQS_IFE2Filter_UWt,  sizeof(UINT32)*6);
    memcpy(ChInfo->IQS_IFE2Filter_VTh,  IQS_IFE2Filter_VTh,  sizeof(UINT32)*5);
    memcpy(ChInfo->IQS_IFE2Filter_VWt,  IQS_IFE2Filter_VWt,  sizeof(UINT32)*6);
    memcpy(&ChInfo->IQS_IFE2Filter,     &IQS_IFE2Filter,     sizeof(IFE2_Filter_Param));


    ////////////////////////////////////////////////
    // IME
    ////////////////////////////////////////////////
    memcpy(&ChInfo->IQS_ChromaAdapt,         &IQS_ChromaAdapt[0],           sizeof(IME_CHRA_ADAP));
    memcpy(&ChInfo->IQS_ChromaAdapt_Lum,     &IQS_ChromaAdapt_Lum[0],       sizeof(IME_CHRA_LADAP));
    memcpy(&ChInfo->IQS_ChromaAdapt_Color,   &IQS_ChromaAdapt_Color[0],     sizeof(IME_CHRA_CADAP));
    memcpy(ChInfo->IQS_ChromaSupp_WeightY,   IQS_ChromaSupp_WeightY,    sizeof(UINT32)*16);
    memcpy(ChInfo->IQS_ChromaSupp_WeightC,   IQS_ChromaSupp_WeightC,    sizeof(UINT32)*16);
    memcpy(&ChInfo->IQS_ChromaSupp,          &IQS_ChromaSupp,            sizeof(IME_ChromaSupp));
    memcpy(&ChInfo->IQS_SuperRes,            &IQS_SuperRes,              sizeof(IME_SuperRes));

    ////////////////////////////////////////////////
    // MSNR & 3DCC
    ////////////////////////////////////////////////
    for ( i=0; i<DRE_DG_PROC_MAX_NUM; i++ )
        ChInfo->IQS_DRE_DGMask[i] = IQS_DRE_DGMask[i];

    for ( i=0; i<DRE_NR_PROC_MAX_NUM; i++ )
    {
        j = IQS_MSNR[0].uiProcRepIdx[i];
        if ( j == 0 )
            continue;
        ChInfo->IQS_MSNR_NR[j] = *IQS_MSNR_NR[j];
    }
    ChInfo->IQS_MSNR = IQS_MSNR[0];

    memcpy(ChInfo->IQS_3DCCLut,     IQS_3DCCLut,       sizeof(UINT32)*900);


    ////////////////////////////////////////////////
    // FuncEn
    ////////////////////////////////////////////////
    ChInfo->SIE_FuncEn = ( OBAvgEn | OBSubSelEn  |  ECSEn);
    ChInfo->IFE_FuncEn = ( OutlierEn   |  ///< enable outLier
                           FilterEn    |  ///< enable bilateral filter
                           CgainEn     |  ///< enable color gain
                           VIGEN       |  ///< enable function: vignette
                           GBALEN      |   ///< enable function: Gbalance
                           0);
    ChInfo->DCE_FuncEn = ( FUNC_CFA        |   ///< CFA interpolation function
                           FUNC_DC         |   ///< Distortion correction function
                           FUNC_CAC        |   ///< Chromatic aberration correction function
                           0 );
    ChInfo->IPE_FuncEn = ( IPE_RGBLPEn         |       ///< RGB low-pass filter function enable
                           IPE_RGBGAMMAEn      |       ///< RGB Gamma function enable
                           IPE_YGAMMAEn        |       ///< Y Gamma function enable,(curve share with ife chroma curve table)
                           IPE_CCREn           |       ///< Color correction function enable
                           IPE_3DCCREn         |       ///< 3D color correction function enable
                           IPE_CSTEn           |       ///< Color space transform function enable, RGB->YCC
                           IPE_CCTRLEn         |       ///< Color control function enable
                           IPE_HADJEn          |       ///< Hue adjustment function enable
                           IPE_CADJEn          |       ///< Color component adjust function enable
                           IPE_CADJ_Y_ENHEn    |       ///< Color component edge enhance function enable, sub-function of IPE_CADJEn
                           IPE_CADJ_Y_CONEn    |       ///< Color component Y contrast adjust function enable, sub-function of IPE_CADJEn
                           IPE_CADJ_CC_CONEn   |       ///< Color component CbCr contrast adjust function enable, sub-function of IPE_CADJEn
                           IPE_YCTHEn          |       ///< Color component YCbCr substitution function enable, sub-function of IPE_CADJEn
                           IPE_CSTPEn          |       ///< Color space transform protect function enable, RGB->YCC
                           0);
    ChInfo->IFE2_FuncEn = (YFtrEn);
    ChInfo->IME_FuncEn = ( //ChromaAdaptEn   |   ///< enable function: chroma adaption (LCA)
                           ChromaMedianEn  |   ///< enable function: chroma median filter
                           ChromaAdjLchrom |   ///< enable function: chroma adjustment of local chroma adaptation
                           ChromaAdjLum    |   ///< enable function: chroma adjustment of local luma adaptation
                           ChromaDBSuppres |   ///< enable function: dark and bright region chroma suppression
                           SingleImgSuperR |   ///< enable function: single image super-resolution
                           0);

    if ( IQS_IsChromaAdaptEn(0, TRUE) == ENABLE )
        ChInfo->IME_FuncEn |= ChromaAdaptEn ;
}

void SetTestAppInfo(void)
{
    FST_FILE pFileHdl;
    IQSIM_APPINFO *AppInfo;
    UINT32 Ch0Addr, Ch2Addr, Size;

    AppInfo = (IQSIM_APPINFO*)IQSIM_GetAppInfo();
    IQSIM_GetRawAddr(&Ch0Addr, &Ch2Addr);

    pFileHdl = FileSys_OpenFile("A:\\IQSIM\\IQSIM.BIN",FST_OPEN_READ);
    if ( pFileHdl != NULL )
    {
        Size = sizeof(AppInfo);
        FileSys_ReadFile(pFileHdl, (UINT8*)AppInfo, &Size, 0, NULL);
        FileSys_CloseFile(pFileHdl);

        pFileHdl = FileSys_OpenFile("A:\\IQSIM\\CH0.RAW",FST_OPEN_READ);
        if ( pFileHdl != NULL )
        {
            Size = AppInfo->IQSimPriInfo.RawInfo.Size.w * AppInfo->IQSimPriInfo.RawInfo.Size.h * AppInfo->IQSimPriInfo.RawInfo.BitNum / 8;
            FileSys_ReadFile(pFileHdl, (UINT8*)Ch0Addr, &Size, 0, NULL);
            FileSys_CloseFile(pFileHdl);
        }
    }
    else
    {
        AppInfo->IQSimJpgInfo.Size.w = 1440;
        AppInfo->IQSimJpgInfo.Size.h = 1080;
        AppInfo->IQSimJpgInfo.BitRate = 95;
        AppInfo->IQSimPriInfo.RawInfo.Size.w = 1440;
        AppInfo->IQSimPriInfo.RawInfo.Size.h = 1080;
        AppInfo->IQSimPriInfo.RawInfo.BitNum = IPL_RAW_BIT_12;
        AppInfo->IQSimPriInfo.RawInfo.cfa = 0;
        SetTestChInfo(&AppInfo->IQSimPriInfo, CAP1);
        Size = AppInfo->IQSimPriInfo.RawInfo.Size.w * AppInfo->IQSimPriInfo.RawInfo.Size.h * AppInfo->IQSimPriInfo.RawInfo.BitNum / 8;
        pFileHdl = FileSys_OpenFile("A:\\IQSIM\\CH0.RAW",FST_OPEN_READ);
        if ( pFileHdl != NULL )
        {
            DBG_ERR("read from file\r\n");
            Size = AppInfo->IQSimPriInfo.RawInfo.Size.w * AppInfo->IQSimPriInfo.RawInfo.Size.h * AppInfo->IQSimPriInfo.RawInfo.BitNum / 8;
            FileSys_ReadFile(pFileHdl, (UINT8*)Ch0Addr, &Size, 0, NULL);
            FileSys_CloseFile(pFileHdl);
        }
        else
            randomPatGen(Ch0Addr, Size);

    }
}


void IQSIM_IFE_GetVigSetting(UINT32 Id, BOOL CapFlag, IQS_VIG_INFO *VigInfo)
{
    SIE_VIG_INFO VigParam;
    SIE_VIG_ADJ_2IMG_INFO VigSetting;
    UINT32 SensorFullHSize,SensorFullVSize;
    UINT32 SensorScaleHRatio, SensorScaleVRatio;
    UINT32 SieCropHSize,SieCropVSize;
    UINT32 SieScaleHSize,SieScaleVSize;
    UINT32 SieScaleHRatio, SieScaleVRatio;
    UINT32 IfeInHSize, IfeInVSize;
    IQS_REFIMG_INFO RefImgInfo;
    IQSIM_APPINFO *AppInfo;

    AppInfo = (IQSIM_APPINFO*)IQSIM_GetAppInfo();

    if ( CapFlag == TRUE ) //capture
    {
        IQS_GetRefImageInfo(Id, IPL_AlgGetUIInfo(Id, IPL_SEL_CAPSENMODE), &RefImgInfo);

        SieCropHSize = AppInfo->IQSimPriInfo.RawInfo.Size.w;
        SieCropVSize = AppInfo->IQSimPriInfo.RawInfo.Size.h;
        SieScaleHSize = SieCropHSize;
        SieScaleVSize = SieCropVSize;
        IfeInHSize = SieCropHSize;
        IfeInVSize = SieCropVSize;
    }
    else  //preview
    {
      return ;
    }
    SensorFullHSize = RefImgInfo.width;
    SensorFullVSize = RefImgInfo.height;
    SensorScaleHRatio = RefImgInfo.hratio;
    SensorScaleVRatio = RefImgInfo.vratio;
    SieScaleHRatio = 1000*SieCropHSize/SieScaleHSize;
    SieScaleVRatio = 1000*SieCropVSize/SieScaleVSize;

    VigSetting.uiCaliImgSizeX = SensorFullHSize;
    VigSetting.uiCaliImgSizeY = SensorFullVSize;
    VigSetting.uiCaliImgVigCntrX = SensorFullHSize*VigInfo->uiCntrPosX/1000;
    VigSetting.uiCaliImgVigCntrY = SensorFullVSize*VigInfo->uiCntrPosY/1000;
    VigSetting.uiCaliImgRatioX = 1000;
    VigSetting.uiCaliImgRatioY = 1000 * SensorFullVSize / SensorFullHSize;
    VigSetting.uiCorrImgSizeX = IfeInHSize;
    VigSetting.uiCorrImgSizeY = IfeInVSize;
    VigSetting.uiCorrImgSclRatioX = SensorScaleHRatio*SieScaleHRatio/1000;
    VigSetting.uiCorrImgSclRatioY = SensorScaleVRatio*SieScaleVRatio/1000;
    VigSetting.uiCorrImgPosRatioX = VigInfo->uiCntrPosX;
    VigSetting.uiCorrImgPosRatioY = VigInfo->uiCntrPosY;

    #if 1
    DBG_ERR("IFE VIG\r\n");
    DBG_ERR("Sensor FullSize %d %d\r\n", SensorFullHSize, SensorFullVSize);
    DBG_ERR("Sensor Scale %d %d\r\n", SensorFullVSize, SensorScaleVRatio);
    DBG_ERR("SIE Crop %d %d\r\n", SieCropHSize,SieCropVSize);
    DBG_ERR("SIE Scale %d %d\r\n", SieScaleHRatio, SieScaleVRatio);
    DBG_ERR("IFE In %d %d\r\n", IfeInHSize, IfeInVSize);
    DBG_ERR("uiCaliImgSizeX %d %d\r\n", VigSetting.uiCaliImgSizeX, VigSetting.uiCaliImgSizeY);
    DBG_ERR("uiCaliImgVigCntrX %d %d\r\n", VigSetting.uiCaliImgVigCntrX, VigSetting.uiCaliImgVigCntrY);
    DBG_ERR("uiCaliImgRatioX %d %d\r\n", VigSetting.uiCaliImgRatioX ,VigSetting.uiCaliImgRatioY);
    DBG_ERR("uiCorrImgSizeX %d %d\r\n", VigSetting.uiCorrImgSizeX , VigSetting.uiCorrImgSizeY);
    DBG_ERR("uiCorrImgSclRatioX %d %d\r\n", VigSetting.uiCorrImgSclRatioX , VigSetting.uiCorrImgSclRatioY);
    DBG_ERR("uiCorrImgPosRatioX %d %d\r\n", VigSetting.uiCorrImgPosRatioX, VigSetting.uiCorrImgPosRatioY);
    #endif

    sie1_calcVIG_TwoImg(&VigParam, &VigSetting);

    #if 1
    DBG_ERR("--> T %d, XDiv %d, YDiv %d, Center(%d, %d)\r\n",VigParam.uiVigT, VigParam.uiVigXDiv, VigParam.uiVigYDiv, VigParam.uiVigX, VigParam.uiVigY);
    #endif
    VigInfo->uiVigX =  VigParam.uiVigX; //AppInfo->IQSimPriInfo.RawInfo.Size.w/2;
    VigInfo->uiVigY =  VigParam.uiVigY; //AppInfo->IQSimPriInfo.RawInfo.Size.h/2;
    VigInfo->uiVigT = VigParam.uiVigT;
    VigInfo->uiVigXDiv = VigParam.uiVigXDiv;
    VigInfo->uiVigYDiv = VigParam.uiVigYDiv;

}
#endif
