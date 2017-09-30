#ifndef _IQSIM_H_
#define _IQSIM_H_
/**
    IQSim.h


    @file       IQSim.h
    @ingroup    mILibIPLCom
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include "Type.h"
#include "ErrorNo.h"

#include "IQS_SettingFlow.h"

typedef struct
{
    ISIZE Size;
    UINT32 BitNum;
    IPL_RAW_PIX cfa;
} IQSIM_RAW_INFO;


typedef struct
{
    ISIZE Size;
    UINT32 BitRate;
} IQSIM_JPG_INFO;

typedef struct
{
    IQSIM_RAW_INFO  RawInfo;

    SieFuncEn       SIE_FuncEn;
    IfeFuncEn       IFE_FuncEn;
    DceFuncEn       DCE_FuncEn;
    IPEFUNCEN       IPE_FuncEn;
    IFE2_FUNC_En    IFE2_FuncEn;
    ImeFuncEn       IME_FuncEn;

    SIE_OBSUB       IQS_OBSub;
    ECSMAP_ENUM     IQS_ECSMap;
    SIE_EmbeddedCS  IQS_ECSSet;
    UINT32          IQS_ECSTab[4225];

    UINT32          IQS_CGain[4];
    UINT32          IQS_CGain_Offset[4];
    IFE_CG          IQS_ColorGain;
    UINT32          IQS_Vig_RLut[17];
    UINT32          IQS_Vig_GLut[17];
    UINT32          IQS_Vig_BLut[17];
    IFE_VIG         IQS_Vig;
    UINT32          IQS_Outl_Bri[5];
    UINT32          IQS_Outl_Dark[5];
    IFE_OUTL        IQS_Outl;
    UINT32          IQS_SF_Weight[6];
    Spatial_Filter  IQS_SpatialFilter;
    UINT32          IQS_RangeA_Thr[6];
    UINT32          IQS_RangeA_Lut[17];
    RangeCFGA       IQS_RangeA;
    UINT32          IQS_RangeB_Thr[6];
    UINT32          IQS_RangeB_Lut[17];
    RangeCFGB       IQS_RangeB;
    Range_Filter    IQS_RangeFilter;
    IFE_FILT        IQS_Filter;
    UINT8           IQS_Gbal_Stab[16];
    UINT8           IQS_Gbal_Dtab[16];
    IFE_GBAL        IQS_Gbal;

    DCE_Distortion  IQS_Distortion;
    Fact_Norm       IQS_DCE_Radious;
    DIST            IQS_DCE_Dist;
    UINT32          IQS_DCE_GLUT[65];
    INT32           IQS_DCE_RLUT[65];
    INT32           IQS_DCE_BLUT[65];
    DCE_CFAPARAM    IQS_CFAInter;

    UINT32          IQS_EdgeGammaLut[65];
    IPE_EDGEMAP     IQS_EdgeMap;
    IPE_EDGEEXT     IQS_EdgeExt;
    IPE_CCTRL       IQS_Cctrl;
    IPE_CCOFS       IQS_CCOfs;
    IPE_CCON        IQS_YCCcon;
    INT16           IQS_EdgeKernel[12];
    UINT8           IQS_EDirTab[8];
    IPE_DEE         IQS_DirEdgeExt;
    IPE_DirScorCal  IQS_DirScoreTab;
    IPE_DirScorCalTh IQS_DirScoreTh;
    IPE_DEConParam  IQS_DECon;
    IPE_KAEdgeW     IQS_EWA;
    IPE_KBEdgeW     IQS_EWB;
    IPE_EdgeW       IQS_EWParam;
    IPE_EDGEENH     IQS_EdgeEnh;
    UINT8           IQS_EDTab[16];
    UINT8           IQS_EStab[16];
    EdgeMap         IQS_EDMap;
    EdgeMapS        IQS_ESMap;
    IPE_RGBLPF      IQS_RGBLpf;
    UINT8           IQS_FStab[16];
    UINT8           IQS_FDtab[16];
    UINT16          IQS_ColorMatrix[9];
    IPE_CC          IQS_ColorCorrection;
    UINT8           IQS_hueTab[24];
    INT32           IQS_satTab[24];
    INT32           IQS_intTab[24];
    UINT8           IQS_edgTab[24];
    UINT8           IQS_ddsTab[8];
    INT16           IQS_CSTCoef[9];
    IPE_CST         IQS_CSTParam;
    IPE_RAND_NR     IQS_NoiseParam;
    IPE_GamRand     IQS_GammaRand;
    UINT32          IQS_YCurveLut[130];
    UINT32          IQS_GammaLut[130];
    IPE_YCCFIX      IQS_YCCFix;
    IPE_3DCCRound   IQS_CC3dRnd;
    IPE_3DCC        IQS_3DColorCtrl;

    IFE2_GRAY_Statal    IQS_GrayStatical;
    IFE2_Refcent_Param  IQS_RefCenter;
    UINT32              IQS_RefCenter_YTh[3];
    UINT32              IQS_RefCenter_YWt[4];
    IFE2_REFCENT_Set    IQS_RefCenter_Y;
    UINT32              IQS_RefCenter_UVTh[3];
    UINT32              IQS_RefCenter_UVWt[4];
    IFE2_REFCENT_Set    IQS_RefCenter_UV;

    UINT32      IQS_IFE2Filter_YTh[5];
    UINT32      IQS_IFE2Filter_YWt[6];
    UINT32      IQS_IFE2Filter_UTh[5];
    UINT32      IQS_IFE2Filter_UWt[6];
    UINT32      IQS_IFE2Filter_VTh[5];
    UINT32      IQS_IFE2Filter_VWt[6];
    IQS_IFE2_FILTER IQS_IFE2Filter;

    IME_CHRA_ADAP   IQS_ChromaAdapt;
    IME_CHRA_LADAP  IQS_ChromaAdapt_Lum;
    IME_CHRA_CADAP  IQS_ChromaAdapt_Color;
    UINT32          IQS_ChromaSupp_WeightY[16];
    UINT32          IQS_ChromaSupp_WeightC[16];
    IME_ChromaSupp  IQS_ChromaSupp;
    IME_SuperRes    IQS_SuperRes;

    DRAM_DG_INFO        IQS_DRE_DGMask[DRE_DG_PROC_MAX_NUM];
    IQS_DRE_NR_IQPARAM  IQS_MSNR_NR[DRE_NR_PROC_MAX_NUM];
    IQS_DRE_IQPARAM     IQS_MSNR;

    UINT32          IQS_3DCCLut[900];
} IQSIM_CHINFO;


typedef struct
{
    char version[32];
    IQSIM_JPG_INFO IQSimJpgInfo;
    IQSIM_CHINFO IQSimPriInfo;
    //IQSIM_CHINFO IQSimQVInfo;
} IQSIM_APPINFO;

IQSIM_CHINFO* IQSIM_GetChInfo(IQS_FLOW_MSG MsgID);
IQSIM_APPINFO* IQSIM_GetAppInfo(void);
void IQSIM_SetAppInfo(UINT32 BinAddr);
void IQSIM_ReorgAppInfo(void);
void IQSIM_SetRawAddr(UINT32 ch0, UINT32 ch2);
void IQSIM_GetRawAddr(UINT32 *ch0, UINT32 *ch2);


typedef void (*IQSIM_SETCHDATA_FP)(IQSIM_CHINFO* ChInfo, UINT32 ISOIdx);
void IQSIM_SetChDataFP(IQSIM_SETCHDATA_FP fp);
void SetTestAppInfo(void);

#endif //_IQS_UTILITY_H_
