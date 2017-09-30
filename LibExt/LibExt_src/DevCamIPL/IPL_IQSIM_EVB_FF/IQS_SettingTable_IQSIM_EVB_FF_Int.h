#ifndef IQS_SETTINGTABLE_H
#define IQS_SETTINGTABLE_H
#include "IQS_SettingFlow.h"

typedef enum
{
    CAP1,
    Total_ISONum
}IQS_ISONumEnum;

typedef enum
{
    ZOOM1,
    Total_ZOOMNum
}IQS_ZOOMNumEnum;

typedef enum
{
    SENID1,
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

extern ISOMAP IQS_CapISOMapTab[Total_SENIDNum][IQS_ISOMAPTAB_NUM];
extern ISOMAP IQS_PrvISOMapTab[Total_SENIDNum][IQS_ISOMAPTAB_NUM];

extern SIE_OBSUB IQS_OBSub[Total_SENIDNum];
extern UINT16 IQS_LA_GammaLut[Total_SENIDNum][65];
extern SIE_EmbeddedCS IQS_ECSSet;
extern UINT32 IQS_ECSTab[Total_ZOOMNum][4225];
extern SIE_CA_VIG IQS_CA_Vig[Total_SENIDNum];

extern UINT32 IQS_CGain[Total_SENIDNum][4];
extern UINT32 IQS_CGain_Offset[Total_ISONum][4];
extern IFE_CG IQS_ColorGain[Total_SENIDNum];
//extern IQS_VIG_SET IQS_Vig_Set[Total_ZOOMNum];
extern UINT32 IQS_Vig_RLut[Total_ZOOMNum][17];
extern UINT32 IQS_Vig_GLut[Total_ZOOMNum][17];
extern UINT32 IQS_Vig_BLut[Total_ZOOMNum][17];
extern IFE_VIG IQS_Vig[Total_SENIDNum];
extern UINT32 IQS_Outl_Bri[Total_ISONum][5];
extern UINT32 IQS_Outl_Dark[Total_ISONum][5];
extern IFE_OUTL IQS_Outl[Total_ISONum];
extern UINT32 IQS_SF_Weight[Total_ISONum][6];
extern UINT32 IQS_RangeA_Thr[Total_ISONum][6];
extern UINT32 IQS_RangeA_Lut[Total_ISONum][17];
extern RangeCFGA IQS_RangeA[Total_ISONum];
extern UINT32 IQS_RangeB_Thr[Total_ISONum][6];
extern UINT32 IQS_RangeB_Lut[Total_ISONum][17];
extern RangeCFGB IQS_RangeB[Total_ISONum];
extern IFE_FILT IQS_Filter[Total_ISONum];
extern UINT8 IQS_Gbal_Stab[Total_ISONum][16];
extern UINT8 IQS_Gbal_Dtab[Total_ISONum][16];
extern IFE_GBAL IQS_Gbal[Total_ISONum];
extern Spatial_Filter  IQS_SpatialFilter[Total_ISONum];
extern Range_Filter    IQS_RangeFilter[Total_ISONum];

extern DCE_Distortion  IQS_Distortion[Total_SENIDNum];
extern Coordinate IQS_DCECenter[Total_ZOOMNum];
extern Fact_Norm   IQS_DCE_Radious[Total_ZOOMNum];
extern DIST IQS_DCE_Dist[Total_ZOOMNum];
extern FOV IQS_DCE_Fov[Total_ZOOMNum];
extern EnH IQS_DCE_Enh[Total_ZOOMNum];
extern Aberation IQS_DCE_ColorAbbration[Total_ZOOMNum];
extern UINT32 IQS_DCE_GLUT[Total_ZOOMNum][65];
extern INT32 IQS_DCE_RLUT[Total_ZOOMNum][65];
extern INT32 IQS_DCE_BLUT[Total_ZOOMNum][65];
extern DCE_CFAPARAM IQS_CFAInter[Total_ISONum];

extern UINT32 IQS_EdgeGammaLut[65];
extern IPE_EDGEMAP IQS_EdgeMap[Total_SENIDNum];
extern IPE_EDGEEXT IQS_EdgeExt[Total_SENIDNum];
extern IPE_CCTRL IQS_Cctrl[Total_SENIDNum];
extern IPE_CCOFS IQS_CCOfs[Total_SENIDNum];
extern IPE_CCON IQS_YCCcon[Total_SENIDNum];
extern INT16 IQS_EdgeKernel[Total_SENIDNum][12];
extern UINT8 IQS_EDirTab[Total_ISONum][8];
extern IPE_DEE IQS_DirEdgeExt[Total_ISONum];
extern IPE_DirScorCal IQS_DirScoreTab[Total_ISONum];
extern IPE_DirScorCalTh IQS_DirScoreTh[Total_ISONum];
extern IPE_DEConParam IQS_DECon[Total_ISONum];
extern IPE_KAEdgeW IQS_EWA[Total_ISONum];
extern IPE_KBEdgeW IQS_EWB[Total_ISONum];
extern IPE_EdgeW IQS_EWParam[Total_ISONum];
extern IPE_EDGEENH IQS_EdgeEnh[Total_ISONum];
extern UINT8 IQS_EDTab[Total_ISONum][16];
extern UINT8 IQS_EStab[Total_ISONum][16];
extern EdgeMap IQS_EDMap[Total_ISONum];
extern EdgeMapS IQS_ESMap[Total_ISONum];
extern IPE_RGBLPF IQS_RGBLpf[Total_ISONum];
extern UINT8 IQS_FStab[Total_ISONum][16];
extern UINT8 IQS_FDtab[Total_ISONum][16];
extern UINT16 IQS_ColorMatrix[Total_SENIDNum][9];
extern IPE_CC IQS_ColorCorrection[Total_SENIDNum];
extern UINT8 IQS_hueTab[Total_SENIDNum][24];
extern INT32 IQS_satTab[Total_SENIDNum][24];
extern INT32 IQS_intTab[Total_SENIDNum][24];
extern UINT8 IQS_edgTab[Total_SENIDNum][24];
extern UINT8 IQS_ddsTab[Total_SENIDNum][8];
extern INT16 IQS_CSTCoef[Total_SENIDNum][9];
extern IPE_CST IQS_CSTParam[Total_SENIDNum];
extern IPE_RAND_NR IQS_NoiseParam[Total_SENIDNum];
extern IPE_GamRand IQS_GammaRand[Total_SENIDNum];
extern UINT32 IQS_YCurveLut[Total_SENIDNum][130];
extern UINT32 IQS_GammaLut[Total_SENIDNum][130];
extern INT8  Saturation[Total_ISONum][SEL_SATURATION_MAX_CNT];
extern UINT8 Sharpness[Total_ISONum][SEL_SHARPNESS_MAX_CNT];
extern IPE_YCCFIX IQS_YCCFix[Total_SENIDNum];
extern IPE_3DCC IQS_3DColorCtrl;
extern UINT32 IQS_3DCCLut[900];

extern IFE2_GRAY_Statal IQS_GrayStatical;
extern IFE2_Refcent_Param IQS_RefCenter;
extern UINT32 IQS_RefCenter_YTh[3];
extern UINT32 IQS_RefCenter_YWt[4];
extern IFE2_REFCENT_Set IQS_RefCenter_Y;
extern UINT32 IQS_RefCenter_UVTh[3];
extern UINT32 IQS_RefCenter_UVWt[4];
extern IFE2_REFCENT_Set IQS_RefCenter_UV;

extern UINT32 IQS_IFE2Filter_YTh[Total_ISONum][5];
extern UINT32 IQS_IFE2Filter_YWt[Total_ISONum][6];
extern UINT32 IQS_IFE2Filter_UTh[Total_ISONum][5];
extern UINT32 IQS_IFE2Filter_UWt[Total_ISONum][6];
extern UINT32 IQS_IFE2Filter_VTh[Total_ISONum][5];
extern UINT32 IQS_IFE2Filter_VWt[Total_ISONum][6];
extern IQS_IFE2_FILTER IQS_IFE2Filter[Total_ISONum];


extern SIE_GAMMA SieRawEncTab;
extern UINT32 RawDecLut[33];

extern IME_CHRA_ADAP IQS_ChromaAdapt[Total_ISONum];
extern IME_CHRA_LADAP IQS_ChromaAdapt_Lum[Total_ISONum];
extern IME_CHRA_CADAP IQS_ChromaAdapt_Color[Total_ISONum];
extern UINT32 IQS_ChromaSupp_WeightY[16];
extern UINT32 IQS_ChromaSupp_WeightC[16];
extern IME_ChromaSupp IQS_ChromaSupp;
extern IME_SuperRes IQS_SuperRes;


#define IQS_MAX_DGMASK_NUM 1
#define IQS_MAX_MSNR_NR_NUM 20
extern DRAM_DG_INFO IQS_DRE_DGMask[DRE_DG_PROC_MAX_NUM];
extern IQS_DRE_NR_IQPARAM *IQS_MSNR_NR[IQS_MAX_MSNR_NR_NUM];
extern IQS_DRE_IQPARAM IQS_MSNR[Total_ISONum];

#endif

