/**
    IPH module (RHE sub function type define)

    @file       IPL_Hal_RHE_Info.h NT96660
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _IPL_Hal_RHE_Info_H
#define _IPL_Hal_RHE_Info_H
#include "Type.h"
#include "IPL_Utility.h"
/** \addtogroup mILibIPH */
//@{

/**
    rhe engine supported clk
*/
typedef enum _IPH_RHECLK
{
    IPH_RHECLK0 = 192,  ///< -192 mhz
    IPH_RHECLK1 = 240,  ///< -240 mhz
    IPH_RHECLK2 = 270,  ///< -280 mhz
    IPH_RHECLK3 = 300,  ///< -320 mhz
    ENUM_DUMMY4WORD(IPH_RHECLK)
}IPH_RHECLK;

/**
    rhe interrupt enable
*/
typedef enum _RHE_INTE_EN
{
    RHE__INT_FLDEND          = 0x00000001, ///< Enable function input-raw-decode
    RHE__INT_DPC_WARN        = 0x00000002, ///< Interrupt - Defect Concealment warning
    RHE__INT_ALL             = 0xffffffff, ///< Interrupt - All
    ENUM_DUMMY4WORD(RHE_INTE_EN)
}RHE_INTE_EN;
/**
     RHE sub fucntion enable
*/
typedef enum _RheFuncEn
{
    RawDecEn     = 0x00000008,   ///< Enable function input-raw-decode
    FlickerDetEN = 0x00000010,   ///< Enable function flicker-detection (only in Staggered-HDR mode)
    ENUM_DUMMY4WORD(RheFuncEn)
}RheFuncEn;
/**
    rhe out buffer number
*/
typedef enum _RHE_PPBNum
{
    RHE_BUF_1 = 0,  ///< -
    RHE_BUF_2 = 1,  ///< -
    RHE_BUF_3 = 2,  ///< -
    ENUM_DUMMY4WORD(RHE_PPBNum)
}RHE_PPBNum;


/**
    RHE data bitdepth
*/
typedef IPL_RAW_BIT RHE_PackBus;
/**
    rhe dma input
*/
typedef struct _RHE_Dma_In
{
    UINT32  In0_Addr;      ///< -
    UINT32  In1_Addr;      ///< -
    UINT32  In2_Addr;

    RHE_PackBus packBus;    ///< data bit depth
    UINT32  In0_LineOffset;     ///< -
    UINT32  In1_LineOffset;     ///< -

}RHE_Dma_In;
/**
    rhe dma output
*/
typedef struct _RHE_Dma_Out
{
    UINT32  Out_Addr;      ///< -
    RHE_PackBus packBus;    ///< data bit depth
    UINT32  LineOffset;     ///< -
}RHE_Dma_Out;

/**
    RHE input data format and signal
*/
typedef struct _IPP_RHE_SIGNAL
{
    struct
    {
        BOOL    HD_ini;     ///< HD inverse
        BOOL    VD_ini;     ///< VD inverse
    }Rhe_HDVD;              ///< HD/VD signal
}IPP_RHE_SIGNAL;

typedef struct _DFS_INFO
{
    UINT32              uiDefupTh;     ///< Defect upper threshold
    enum
    {
        DFS_DP_BY_IN0  = 0,     ///< DP threshold on input0
        DFS_DP_BY_IN1 ,         ///< DP threshold on input1
        ENUM_DUMMY4WORD(ThresholdSel)
    } ThresholdSel;           ///< Defect thresholding source
    enum
    {
        DFS_OP_B_MINUS_A  = 0,  ///< Input-1 minus Input-0
        DFS_OP_A_MINUS_B     ,  ///< Input-0 minus Input-1
        DFS_OP_A_PLUS_B      ,  ///< Input-0 plus  Input-1
        ENUM_DUMMY4WORD(OP_SEL)
    } OP_SEL;               ///< Operation type
    enum
    {
        DEF_WGT_50  = 0, ///< Defect concealment cross-channel weight = 50%
        DEF_WGT_25  = 1, ///< Defect concealment cross-channel weight = 25%
        DEF_WGT_0   = 2, ///< Defect concealment cross-channel weight =  0% (disabled)
        ENUM_DUMMY4WORD(WeightSel)
    } WeightSel;     ///< Defect concealment cross-channel weight
} DFS_INFO;

typedef struct
{
    BOOL DeGamma;                  ///< enable De-Gamma
    UINT16 *pDgmaTbl;             ///< Pointer of degamma table
} RHE_RAW_DECODE_INFO;


/**
    Struct RHE Crop Point
*/
typedef struct _RHE_CROP
{
    UINT32 y;          ///< Left point Y of Crop image
}RHE_CROP;

typedef struct _RHE_SIZE
{
    UINT32      CropSizeX; ///< horizontal size
    UINT32      CropSizeY; ///< vertical size
    UINT32      OrgSizeX; ///< horizontal size
    UINT32      OrgSizeY; ///< vertical size
} RHE_SIZE;

typedef struct _FLICKER_DETECT_INFO
{
    Coordinate  StartPosition;
    struct{
        UINT32      uiWinSizeX;  ///< window size X
        UINT32      uiWinSizeY;  ///< window size Y
        UINT32      uiWinSpacingY;  ///< window skip(spacing) Y
    }WinInfo;
    struct{
        UINT32      uiRatioX;   ///< subsample ratio X (inside window)
        UINT32      uiRatioY;   ///< subsample ratio Y (inside window)
    }subsampRatio;
    BOOL        bChannelSel[4];  ///< channel selection
    UINT32      uiUpperTh;    ///< upper thershold
    UINT32      uiLowerTh;    ///< lower thershold
} FLICKER_DETECT_INFO;

typedef struct _HDR_RAD_INFO
{
    UINT32      uiIn0EvGap;     ///< The EV-gap for input-0
    UINT32      uiIn1EvGap;     ///< The EV-gap for input-1
    enum
    {
        HDR_RAD_WGT_BY_IN0  = 0, ///< Radiance weight table source = input-0
        HDR_RAD_WGT_BY_IN1  = 1, ///< Radiance weight table source = input-1
        ENUM_DUMMY4WORD(Weight_SoruceSel)
    } Weight_SoruceSel;;  ///< Radiance weight table source
    UINT8       *puiWeightTbl;   ///< radiance weight table 17tab x 8bits.
} HDR_RAD_INFO;

typedef struct _HDR_REF_LA_LPF_INFO
{
    UINT8       *puiReflaLpf;         ///< low pass filter A/B/C/D/E/F/G/H 8tab x 8bits
    UINT32      uiReflaLpfNormM;      ///< low-pass-filter normalization factor M
    UINT32      uiReflaLpfNormS;      ///< low-pass-filter normalization factor S
} HDR_REF_LA_LPF_INFO;

typedef struct _HDR_GLOBAL_TONE_INFO
{
    UINT16 *puiGblToneTbl;  ///< global tone table, 41taps x 12bits
}HDR_GLOBAL_TONE_INFO;

typedef struct _HDR_LOCAL_TONE_INFO
{
    UINT32  uiBand0Gain;    ///< gain for band-0, 10 bits as 1.9 bits
    UINT32  uiBand1Gain;    ///< gain for band-1, 10 bits as 1.9 bits
} HDR_LOCAL_TONE_INFO;

typedef struct _HDR_REGULATION_INFO
{
    UINT8   *puiDataARto;    ///< reaulation on ratio, considering data A, 9taps x 8bits
    UINT8   *puiDataAMax;    ///< reaulation on max. , considering data A, 9taps x 8bits
    UINT8   *puiDataAMin;    ///< reaulation on min. , considering data A, 9taps x 8bits
    UINT8   *puiDataBMax;    ///< reaulation on max. , considering data B, 9taps x 8bits
    UINT8   *puiDataBMin;    ///< reaulation on min. , considering data B, 9taps x 8bits
} HDR_REGULATION_INFO;


typedef struct _HDR_RECURVEN_INFO
{
    UINT16  *puiPointX;     ///< 4taps x 16 bits
    UINT16  *puiPointY;      ///< 4taps x 16 bits
    UINT16  *puiSlopeM;      ///< 4taps x 16 bits
    UINT8   *puiSlopeS;      ///< 4taps x 8 bits
} HDR_RECURVEN_INFO;

/**
     RHE sub function configuration.
*/
typedef struct _RHE_SUBFUNC
{
    RHE_INTE_EN InteEn;     ///< interrupt enable
    RheFuncEn   rheFuncEn;  ///< function enable

    FLICKER_DETECT_INFO         FlickerDetectInfo;          ///<
    HDR_RAD_INFO                HdrRadianceInfo;            ///<
    HDR_REF_LA_LPF_INFO         HdrRefLaLowPassfilterInfo;  ///<
    HDR_GLOBAL_TONE_INFO        HdrGlobalToneInfo;          ///<
    HDR_LOCAL_TONE_INFO         HdrLocalToneInfo;           ///<
    HDR_REGULATION_INFO         HdrRegulationInfo;          ///<
    HDR_RECURVEN_INFO           HdrRecurvenInfo;            ///<
    DFS_INFO                    DFSInfo;                    ///< RHE dark frame substraction information
}RHE_SUBFUNC;
//@}
#endif
