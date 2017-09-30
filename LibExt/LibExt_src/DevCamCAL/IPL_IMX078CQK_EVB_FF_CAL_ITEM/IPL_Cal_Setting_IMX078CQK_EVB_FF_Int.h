#ifndef _IPL_CAL_SETTING_IMX078CQK_EVB_FF_INT_H_
#define _IPL_CAL_SETTING_IMX078CQK_EVB_FF_INT_H_
#include "IPL_Cal_DataPassing_IMX078CQK_EVB_FF_Int.h"
#include "IPL_Calibration_3A.h"
#include "IPL_Hal.h"
#include "IPL_Ctrl_flow_Int.h"
#include "IPL_CalRW.h"
#include "IPL_Calibration.h"

#define BYPASS  0xffffffff
#define CFGEND  (BYPASS-1)
typedef struct
{
    UINT32 SenMode;
    UINT32 HDinv;
    UINT32 VDinv;
    UINT32 VDPhase;
    UINT32 HDPhase;
    UINT32 DataPhase;
    UINT32 CropShrinkX; ///< offset crop size according sensor signal,must be multiple of 4, avoid bayer start pixel shifted
    UINT32 CropShrinkY; ///< offset crop size according sensor signal,must be multiple of 4, avoid bayer start pixel shifted
}Cal_SENSOR_INFO;

/**
    AE information (include condition)
*/
typedef struct
{
    CAL_AE_INFO   CalAeInfo;    ///< AE information
    CAL_CONDITION CalConditin;    ///< other condition
}CAL_AE_INFO_CONDITION;

/*
    calibration per station information
*/
typedef struct
{
    BOOL   bDP;           ///<TRUE for bright, FALSE for dark
    UINT32 Threshold;   ///< DP detect threshold
    UINT32 Block_X;     ///< DP search block X,(must be even,because of bayer fmt)
    UINT32 Block_Y;     ///< DP search block Y,(must be even,because of bayer fmt)
}CAL_DPC_THRES;

typedef struct
{
    UINT32          GS_GainBase;    //GS Gain Base
    UINT32          Threshold;      //iso base threshold, range = 0~100 (%);
}CAL_ISO_INFO;

typedef struct
{
    UINT32          awb_bit;        //d2f8 or d3f7
    UINT32          Threshold;      //awb cal threshold, range = 0~100 (%);
    UINT32          RepeatTime;     //repeat for calculate average R/B gain;
}CAL_AWB_INFO;

typedef struct
{
    ECS_BitRatio    Ecs_bit;        //d2f8 or d3f7
    ECSMAP_ENUM     Ecs_map;        //ECS color table size select
    UINT32          CompenRatio;    //ECS Compensation ratio, range = 0~100 (%)
}CAL_ECS_INFO;

typedef struct
{
    UINT32  ValidRadius;        // in pixel units
    UINT32  ScanRadius;         // in pixel units
    UINT32  AvgCnt;             // in pixel units (noise filter)
    UINT32  EdgeThr;            // edge threshold, range = 0~100 (%);
    UINT32  MinReliability;     // Minimum Reliability (success rate), range = 0~100 (%);
}CAL_LENSCEN_INFO;

/*
    calibration station information
*/
typedef struct
{
    CAL_SECMAP CalItem;
    Cal_SENSOR_INFO* SenModeInfo;
    UINT32 SecSize;
    CAL_AE_INFO_CONDITION* CalAeInfoCon;
    BOOL bDP; //TRUE for DP bright, FALSE for DP dark
}Cal_SECTION;

/*
    calibration all station information (include cal item)
*/
typedef struct
{
    CAL_ISO_INFO* IsoInfor;
    CAL_AWB_INFO* AwbInfor;
    CAL_ECS_INFO* EcsInfor;
    CAL_DPC_THRES* DPBrightTH;
    CAL_DPC_THRES* DPDarkTH;
    CAL_LENSCEN_INFO* LensCenInfor;
}CAL_INFO;


/*
    ALL calibration information
*/
typedef struct
{
    Cal_SECTION* CalSecInfo;
    CAL_INFO* CalInfo;
}Cal_SECTION_INFO;

//#define DP_MAX_CNT  (40000) // 1/100
#define MaxSectionNum  20 // max section number for per IPL
#define MaxSenModeNum  10 // max sensor mode number for per calItem in per IPL

extern Cal_SENSOR_INFO SenModeSection_End;
extern Cal_SENSOR_INFO SensorMode_PRV_IPL1[];
extern Cal_SENSOR_INFO SensorMode_AWB_IPL1[];
extern Cal_SENSOR_INFO SensorMode_ECS_IPL1[];
extern Cal_SENSOR_INFO SensorMode_DP_IPL1[];
extern Cal_SENSOR_INFO SensorMode_ISO_IPL1[];
extern Cal_SENSOR_INFO SensorMode_AE_Check_IPL1[];
extern Cal_SENSOR_INFO SensorMode_LENSCEN_IPL1[];
extern Cal_SENSOR_INFO SensorMode_PRV_IPL2[];
extern Cal_SENSOR_INFO SensorMode_AWB_IPL2[];
extern Cal_SENSOR_INFO SensorMode_ECS_IPL2[];
extern Cal_SENSOR_INFO SensorMode_DP_IPL2[];
extern Cal_SENSOR_INFO SensorMode_ISO_IPL2[];
extern Cal_SENSOR_INFO SensorMode_AE_Check_IPL2[];
extern Cal_SENSOR_INFO SensorMode_LENSCEN_IPL2[];

extern CAL_INFO Cal_info_IPL1;
extern CAL_INFO Cal_info_IPL2;
extern CAL_INFO Cal_info_IPL3;
extern CAL_INFO Cal_info_IPL4;

extern CAL_AE_INFO_CONDITION AeInfo_PRV_IPL1[];
extern CAL_AE_INFO_CONDITION AeInfo_ISO_IPL1[];
extern CAL_AE_INFO_CONDITION AeInfo_AWB_IPL1[];
extern CAL_AE_INFO_CONDITION AeInfo_ECS_IPL1[];
extern CAL_AE_INFO_CONDITION AeInfo_DPB_IPL1[];
extern CAL_AE_INFO_CONDITION AeInfo_DPD_IPL1[];
extern CAL_AE_INFO_CONDITION AeInfo_LENSCEN_IPL1[];

extern CAL_AE_INFO_CONDITION AeInfo_PRV_IPL2[];
extern CAL_AE_INFO_CONDITION AeInfo_ISO_IPL2[];
extern CAL_AE_INFO_CONDITION AeInfo_AWB_IPL2[];
extern CAL_AE_INFO_CONDITION AeInfo_ECS_IPL2[];
extern CAL_AE_INFO_CONDITION AeInfo_DPB_IPL2[];
extern CAL_AE_INFO_CONDITION AeInfo_DPD_IPL2[];
extern CAL_AE_INFO_CONDITION AeInfo_LENSCEN_IPL2[];


extern CAL_ISO_INFO Cal_iso_info_1;
extern CAL_AWB_INFO Cal_awb_info_1;
extern CAL_ECS_INFO Cal_ecs_info_1;
extern CAL_DPC_THRES DPC_THRES_BRIGHT_1;
extern CAL_DPC_THRES DPC_THRES_DARK_1;
extern CAL_LENSCEN_INFO Cal_lenscen_info_1;

extern CAL_ISO_INFO Cal_iso_info_2;
extern CAL_AWB_INFO Cal_awb_info_2;
extern CAL_ECS_INFO Cal_ecs_info_2;
extern CAL_DPC_THRES DPC_THRES_BRIGHT_2;
extern CAL_DPC_THRES DPC_THRES_DARK_2;
extern CAL_LENSCEN_INFO Cal_lenscen_info_2;

extern UINT32 DPLimitCnt[IPL_ID_MAX_NUM][MaxSenModeNum];

extern Cal_SECTION CalSection_IPL1[];
extern Cal_SECTION CalSection_IPL2[];
extern Cal_SECTION CalSection_IPL3[];
extern Cal_SECTION CalSection_IPL4[];
extern Cal_SECTION_INFO CalSectionInfo[IPL_ID_MAX_NUM];

extern CAL_DEBUG_INFO_DP CalDebugDP_IPL1;
extern CAL_DEBUG_INFO_ISO CalDebugISO_IPL1;
extern CAL_DEBUG_INFO_ECS CalDebugECS_IPL1;
extern CAL_DEBUG_INFO_AWB CalDebugAWB_IPL1;
extern CAL_DEBUG_INFO_LENSCEN CalDebugLENSCEN_IPL1;
extern CAL_DEBUG_INFO_DP CalDebugDP_IPL2;
extern CAL_DEBUG_INFO_ISO CalDebugISO_IPL2;
extern CAL_DEBUG_INFO_ECS CalDebugECS_IPL2;
extern CAL_DEBUG_INFO_AWB CalDebugAWB_IPL2;
extern CAL_DEBUG_INFO_LENSCEN CalDebugLENSCEN_IPL2;

extern CAL_DEBUG_INFO CalDebugInfo[IPL_ID_MAX_NUM];
extern ER CAL_SetDebugLevel (CalDebugIdx idx, UINT32 value);
extern UINT32 CAL_GetDebugLevel (CalDebugIdx idx);


/**
    CAL IQ parameter
*/
extern DIST CAL_DCE_Dist;

#define LutTblSize  65
extern UINT32 CAL_DCE_2D_LUT[LutTblSize];
extern DCE_Distortion Cal_DCE_Distortion;
extern DCE_CFAPARAM CAL_DCE_IQS_CFAInter;
extern UINT32 CAL_IQS_CGain[4];
extern UINT32 CAL_IQS_CGain_PRV[4];

extern IFE_FILT pFilter1;
extern Spatial_Filter Spatial1;
extern UINT32 IFE_spatial[6];
extern UINT32 IFE_A_pThres[6];
extern UINT32 IFE_A_pThLut[17];
extern UINT32 IFE_B_pThres[6];
extern UINT32 IFE_B_pThLut[17];
extern UINT32 GainMask;
extern Range_Filter Range1;
extern RangeCFGA A11;
extern RangeCFGB B11;
extern UINT32 A_pThres1[6];
extern UINT32 A_pThLut1[17];
extern UINT32 B_pThres1[6];
extern UINT32 B_pThLut1[17];

extern IPC_IQparam     Ipc_IQ;
extern IFE_SUBFUNC     Ife_sub1;
extern IPL_HAL_GROUP   Grp1;
extern UINT32   pWeight1[6];
extern UINT32 CAL_IQS_CGain_Offset[4];
extern IFE_CG CAL_IQS_ColorGain;
extern IPE_RGBLPF CAL_IQS_RGBLpf;
extern INT16 CAL_IQS_CSTCoef[9];
extern IPE_CST CAL_IQS_CSTParam;
extern UINT32 CAL_IQS_GammaLut[130];
extern UINT16 CAL_Normal_CC[9];
extern UINT8 CAL_IQS_FStab[16];
extern UINT8 CAL_IQS_FDtab[16];
extern IPE_CC CAL_IQS_ColorCorrection;
extern IME_CHRA_ADAP CAL_IQS_ChromaAdapt;
//extern IME_CHRA_FILT CAL_IQS_ChromaFilter;
extern IME_CHRA_LADAP CAL_IQS_ChromaAdapt_Lum;
extern IME_CHRA_CADAP CAL_IQS_ChromaAdapt_Color;
extern IFE2_GRAY_Statal CAL_IQS_GrayStatical;

extern UINT32 CAL_IQS_RefCenter_YTh[3];
extern UINT32 CAL_IQS_RefCenter_YWt[4];
extern IFE2_REFCENT_Set CAL_IQS_RefCenter_Y;
extern UINT32 CAL_IQS_RefCenter_UVTh[3];
extern UINT32 CAL_IQS_RefCenter_UVWt[4];
extern IFE2_REFCENT_Set CAL_IQS_RefCenter_UV;

extern IFE2_Refcent_Param CAL_IQS_RefCenter;

extern UINT32 CAL_IQS_IFE2Filter_YTh[5];
extern UINT32 CAL_IQS_IFE2Filter_YWt[6];
extern UINT32 CAL_IQS_IFE2Filter_UTh[5];
extern UINT32 CAL_IQS_IFE2Filter_UWt[6];
extern UINT32 CAL_IQS_IFE2Filter_VTh[5];
extern UINT32 CAL_IQS_IFE2Filter_VWt[6];
extern IFE2_Filter_Param CAL_IQS_IFE2Filter;

extern UINT32 CAL_IQS_Outl_Bri[5];
extern UINT32 CAL_IQS_Outl_Dark[5];
extern IFE_OUTL CAL_IQS_Outl;
extern UINT32 CAL_IQS_SF_Weight[6];
extern Spatial_Filter CAL_IQS_SpatialFilter;
extern UINT32 CAL_IQS_RangeA_Thr[6];
extern UINT32 CAL_IQS_RangeA_Lut[17];
extern RangeCFGA CAL_IQS_RangeA;
extern UINT32 CAL_IQS_RangeB_Thr[6];
extern UINT32 CAL_IQS_RangeB_Lut[17];
extern RangeCFGB CAL_IQS_RangeB;
extern Range_Filter CAL_IQS_RangeFilter;
extern IFE_FILT CAL_IQS_Filter;
extern UINT8 CAL_IQS_Gbal_Stab[16];
extern UINT8 CAL_IQS_Gbal_Dtab[16];
extern IFE_GBAL CAL_IQS_Gbal;
#endif
