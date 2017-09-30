#ifndef IQS_SETTINGTABLE_IMX206CQC_H
#define IQS_SETTINGTABLE_IMX206CQC_H

typedef enum
{
    PRV1ISO0050,
    PRV1ISO0100,
    PRV1ISO0200,
    PRV1ISO0400,
    PRV1ISO0800,
    PRV1ISO1600,
    PRV1ISO3200,
    PRV1ISO6400,

    PRV1SISO0050,
    PRV1SISO0100,
    PRV1SISO0200,
    PRV1SISO0400,
    PRV1SISO0800,
    PRV1SISO1600,
    PRV1SISO3200,
    PRV1SISO6400,

    CAP1ISO0050,
    CAP1ISO0100,
    CAP1ISO0200,
    CAP1ISO0400,
    CAP1ISO0800,
    CAP1ISO1600,
    CAP1ISO3200,
    CAP1ISO6400,

    PRV2ISO0050,
    PRV2ISO0100,
    PRV2ISO0200,
    PRV2ISO0400,
    PRV2ISO0800,
    PRV2ISO1600,
    PRV2ISO3200,
    PRV2ISO6400,
//#NT#2016/03/01#Ted Lin -begin
//#FIXED, correct ISO mapping
    CAP2ISO0050,
    CAP2ISO0100,
//#NT#2016/03/01#Ted Lin -end
    CAP2ISO0200,
    CAP2ISO0400,
    CAP2ISO0800,
    CAP2ISO1600,
    CAP2ISO3200,
    CAP2ISO6400,

    Total_ISONum
}IQS_ISONumEnum;

#define IQS_PRV1_ISOSTART PRV1ISO0050
#define IQS_PRV1_ISOEND   PRV1ISO6400
#define IQS_PRV1_ISOEND_VALUE 6400

#define IQS_CAP1_ISOSTART CAP1ISO0050
#define IQS_CAP1_ISOEND   CAP1ISO6400
#define IQS_CAP1_ISOEND_VALUE 6400

#define IQS_PRV1S_ISOSTART PRV1SISO0050
#define IQS_PRV1S_ISOEND   PRV1SISO6400
#define IQS_PRV1S_ISOEND_VALUE 6400

#define IQS_CAP1S_ISOSTART CAP1ISO0050
#define IQS_CAP1S_ISOEND   CAP1ISO6400
#define IQS_CAP1S_ISOEND_VALUE 6400

#define IQS_PRV2_ISOSTART PRV2ISO0050
#define IQS_PRV2_ISOEND   PRV2ISO6400
#define IQS_PRV2_ISOEND_VALUE 6400

#define IQS_CAP2_ISOSTART CAP2ISO0050
#define IQS_CAP2_ISOEND   CAP2ISO6400
#define IQS_CAP2_ISOEND_VALUE 6400

//#NT#2016/11/21#Wendy Liao -begin
//#NT#Define 3DNR parameters in array
typedef enum
{
    H264_UI_3DNR_OFF,
    H264_UI_3DNR_LV1,
    H264_UI_3DNR_LV2,
    H264_UI_3DNR_LV3,
    H264_UI_3DNR_LV4,
    H264_UI_3DNR_LV5,

    Total_UI_3DNR
}IQS_UI3dnrEnum;
//#NT#2016/11/21#Wendy Liao -end

typedef enum
{
    SENID1,
    SENID2,
    Total_SENIDNum
}IQS_SENIDNumEnum;

typedef struct _ISOMAP
{
    UINT32  ISO;
    UINT32  Index;
}ISOMAP;

typedef enum
{
    VIG_TAB_GAIN_1X,
    VIG_TAB_GAIN_2X
} IQS_VIG_TAB_GAIN;

#define IQS_ISOMAPTAB_NUM 10
#define IQS_ISOMAP_END    999999

extern ISOMAP IQS1_ISOMapTab[2][IQS_ISOMAPTAB_NUM];
extern ISOMAP IQS1S_ISOMapTab[2][IQS_ISOMAPTAB_NUM];
extern ISOMAP IQS2_ISOMapTab[2][IQS_ISOMAPTAB_NUM];

extern UINT16 IQS_OBSub[Total_ISONum];
extern UINT16 IQS_LA_GammaLut[Total_SENIDNum][65];
extern SIE_EmbeddedCS IQS_ECSSet;
extern UINT32 IQS_ECSTab[Total_SENIDNum][4225];
extern SIE_CA_VIG IQS_CA_Vig[Total_SENIDNum];
extern SIE_EDGETHRE Eth_Thres[Total_ISONum];

extern UINT32 IQS_CGain[Total_SENIDNum][4];
extern IFE_CG IQS_ColorGain[Total_SENIDNum];
extern IQS_VIG_SET IQS_Vig_Set[Total_SENIDNum];
extern IFE_VIG IQS_Vig[Total_SENIDNum];
extern UINT32 IQS_Outl_Bri[Total_ISONum][5];
extern UINT32 IQS_Outl_Dark[Total_ISONum][5];
extern IFE_OUTL IQS_Outl[Total_SENIDNum];
extern UINT32 IQS_SF_Weight[Total_SENIDNum][6];
extern Spatial_Filter IQS_SpatialFilter[Total_SENIDNum];
extern UINT32 IQS_RangeA_Thr[Total_ISONum][6];
extern UINT32 IQS_RangeA_Lut[Total_ISONum][17];
extern UINT32 IQS_RangeB_Thr[Total_ISONum][6];
extern UINT32 IQS_RangeB_Lut[Total_ISONum][17];
extern RangeCFGA IQS_RangeA[Total_SENIDNum];
extern RangeCFGB IQS_RangeB[Total_ISONum];
extern IFE_FILT IQS_Filter[Total_ISONum];
extern Range_Filter IQS_RangeFilter[Total_SENIDNum];
extern UINT8 IQS_Gbal_Stab[Total_ISONum][16];
extern UINT8 IQS_Gbal_Dtab[Total_ISONum][16];
extern IFE_GBAL IQS_Gbal[Total_ISONum];

extern DCE_Distortion  IQS_Distortion[Total_SENIDNum];
extern Fact_Norm IQS_DCE_Radious[Total_SENIDNum];
extern DIST IQS_DCE_Dist[Total_SENIDNum];
extern FOV IQS_DCE_Fov[Total_SENIDNum];
extern EnH IQS_DCE_Enh[Total_SENIDNum];
extern Aberation IQS_DCE_ColorAbbration[Total_SENIDNum];
extern UINT32 IQS_DCE_GLUT_Init[65];
extern UINT32 IQS_DCE_GLUT[Total_SENIDNum][65];
extern INT32 IQS_DCE_RLUT[Total_SENIDNum][65];
extern INT32 IQS_DCE_BLUT[Total_SENIDNum][65];
extern DCE_CFAPARAM IQS_CFAInter[Total_SENIDNum];

extern IPE_EDGEMAP IQS_EdgeMap[Total_SENIDNum];
extern IPE_EDGEEXT IQS_EdgeExt[Total_SENIDNum];
extern INT16 IQS_EdgeKernel[Total_SENIDNum][12];
extern UINT8 IQS_EDirTab[Total_SENIDNum][8];
extern IPE_DEE IQS_DirEdgeExt[Total_ISONum];
extern IPE_DirScorCal IQS_DirScoreTab[Total_SENIDNum];
extern IPE_DirScorCalTh IQS_DirScoreTh[Total_ISONum];
extern IPE_DEConParam IQS_DECon[Total_SENIDNum];
extern IPE_KAEdgeW IQS_EWA[Total_SENIDNum];
extern IPE_KBEdgeW IQS_EWB[Total_SENIDNum];
extern IPE_EdgeW IQS_EWParam[Total_ISONum];
extern IPE_EDGEENH IQS_EdgeEnh[Total_ISONum];
extern UINT8 IQS_EDTab[Total_ISONum][16];
extern UINT8 IQS_EStab[Total_ISONum][16];
extern EdgeMap IQS_EDMap[Total_ISONum];
extern EdgeMapS IQS_ESMap[Total_ISONum];
extern LPF IQS_RGBLpf[Total_ISONum];
extern UINT8 IQS_FStab[Total_ISONum][16];
extern UINT8 IQS_FDtab[Total_ISONum][16];
extern IPE_CC IQS_ColorCorrection[Total_SENIDNum];
extern UINT16 Normal_CC[Total_SENIDNum][9];
extern UINT16 IQS_CC_Day[Total_SENIDNum][9];
extern UINT16 IQS_CC_Night[Total_SENIDNum][9];
extern UINT8 IQS_hueTab[Total_SENIDNum][24];
extern INT32 IQS_satTab[Total_SENIDNum][24];
extern INT32 IQS_intTab[Total_SENIDNum][24];
extern UINT8 IQS_edgTab[Total_SENIDNum][24];
extern UINT8 IQS_ddsTab[Total_SENIDNum][8];
extern INT16 IQS_CSTCoef[Total_SENIDNum][9];
extern IPE_CCTRL IQS_Cctrl[Total_SENIDNum];
extern IPE_CCOFS IQS_CCOfs[Total_SENIDNum];
extern IPE_CCON IQS_YCCcon[Total_SENIDNum];
extern IPE_CST IQS_CSTParam[Total_SENIDNum];
extern IPE_RAND_NR IQS_NoiseParam[Total_SENIDNum];
extern IPE_GamRand IQS_GammaRand[Total_SENIDNum];
extern UINT32 IQS_EdgeGammaLut[Total_SENIDNum][65];
extern UINT32 IQS_YCurveLut[Total_SENIDNum][130];
extern UINT32 IQS_WDR_LumaMapingLut[Total_SENIDNum][256];
//#NT#2016/02/19#Kristin Huang -begin
//#NT# Add WDR ContratOBMap
extern INT32 IQS_WDR_ContratOBMap[Total_SENIDNum][256];
//#NT#2016/02/19#Kristin Huang -end
//#NT#2016/05/19#Jarkko Chang -begin
//#NT# for ExtFunc using
extern UINT32 IQS_WDR_LocalGain[Total_SENIDNum][256];
//#NT#2016/05/19#Jarkko Chang -end

//#NT#2017/03/13#Wendy Liao -begin
//#NT# for WDR2.5
extern UINT32 IQS_WDR_DarkGainMap[Total_SENIDNum][256];
extern UINT32 IQS_WDR_BrightGainMap[Total_SENIDNum][256];
extern UINT32 IQS_WDR_BlackLevelMap[Total_SENIDNum][256];
extern UINT32 IQS_WDR_SharpnessMap[Total_SENIDNum][256];
//#NT#2017/03/13#Wendy Liao -end

//#NT#2017/03/13#Wendy Liao -begin
//#Add WDR maxlevel by ISO index
extern AUTO_WDR_OVEREXP AutoWDRLevel[Total_ISONum];
//#NT#2017/03/13#Wendy Liao -end
extern UINT32 IQS_GammaLut[Total_SENIDNum][130];
extern UINT32 IQS_GammaLut_Day[Total_SENIDNum][130];
extern UINT32 IQS_GammaLut_Night[Total_SENIDNum][130];
extern IPE_YCCFIX IQS_YCCFixBuf[Total_SENIDNum];
extern IPE_YCCFIX IQS_YCCFix[Total_SENIDNum];
extern IPE_3DCC IQS_3DColorCtrl;
extern INT8  Saturation[Total_ISONum];
extern UINT8 Sharpness[Total_ISONum];
extern INT8  Contrast[Total_ISONum];

extern IFE2_GRAY_Statal IQS_GrayStatical;
extern IFE2_Refcent_Param IQS_RefCenter[Total_SENIDNum];
extern UINT32 IQS_RefCenter_YTh[Total_ISONum][3];
extern UINT32 IQS_RefCenter_YWt[Total_ISONum][4];
extern IFE2_REFCENT_Set IQS_RefCenter_Y[Total_ISONum];
extern UINT32 IQS_RefCenter_UVTh[Total_ISONum][3];
extern UINT32 IQS_RefCenter_UVWt[Total_ISONum][4];
extern IFE2_REFCENT_Set IQS_RefCenter_UV[Total_ISONum];

extern UINT32 IQS_IFE2Filter_YTh[Total_ISONum][5];
extern UINT32 IQS_IFE2Filter_YWt[Total_ISONum][6];
extern UINT32 IQS_IFE2Filter_UVTh[Total_ISONum][5];
extern UINT32 IQS_IFE2Filter_UVWt[Total_ISONum][6];
extern IQS_IFE2_FILTER IQS_IFE2Filter[Total_ISONum];

extern IME_CHRA_ADAP IQS_ChromaAdapt[Total_ISONum];
extern IME_CHRA_LADAP IQS_ChromaAdapt_Lum[Total_ISONum];
extern IME_CHRA_CADAP IQS_ChromaAdapt_Color[Total_ISONum];
extern UINT32 IQS_ChromaSupp_WeightY[Total_SENIDNum][16];
extern UINT32 IQS_ChromaSupp_WeightC[Total_SENIDNum][16];
extern IME_ChromaSupp IQS_ChromaSupp[Total_SENIDNum];
extern IME_SuperRes IQS_SuperRes;

extern SIE_GAMMA SieRawEncTab;
extern UINT32 RawDecLut[33];

#define IQS_MAX_DGMASK_NUM 1
#define IQS_MAX_MSNR_NR_NUM 20
extern DRAM_DG_INFO IQS_DRE_DGMask[DRE_DG_PROC_MAX_NUM];
extern IQS_DRE_NR_IQPARAM *IQS_MSNR_NR[IQS_MAX_MSNR_NR_NUM];
extern IQS_DRE_IQPARAM IQS_MSNR[Total_ISONum];

//#NT#2016/11/10#Wendy Liao -begin
//#Add 3DNRlevel by ISO index
extern INT8 IQS_3DNR_Level[Total_ISONum];
extern H264Enc3dnrCfg* IQS_3DNR_Param[Total_UI_3DNR];
//#NT#2016/11/16#Wendy Liao -begin
//#NT#Define 3DNR parameters in LibExt
extern H264Enc3dnrCfg IQS_3DNR_Param_OFF;
extern H264Enc3dnrCfg IQS_3DNR_Param_LV1;
extern H264Enc3dnrCfg IQS_3DNR_Param_LV2;
extern H264Enc3dnrCfg IQS_3DNR_Param_LV3;
extern H264Enc3dnrCfg IQS_3DNR_Param_LV4;
extern H264Enc3dnrCfg IQS_3DNR_Param_LV5;
//#NT#2016/11/16#Wendy Liao -end
//#NT#2016/11/01#Wendy Liao -end
#endif
