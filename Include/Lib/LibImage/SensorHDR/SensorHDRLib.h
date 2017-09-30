
#ifndef _SENSOR_HDR_LIB_H_
#define _SENSOR_HDR_LIB_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Type.h"
#include "ErrorNo.h"
#include "Debug.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef enum
{
    SHDR_STAGGERED_MODE = 0, ///< Staggered Mode
    SHDR_CURVED         = 1, ///< Curved Mode
    ENUM_DUMMY4WORD(SHDR_MODE_SELECTION)
} SHDR_MODE_SELECTION;

typedef enum
{
    SHDR_RAD_WGT_BY_IN0  = 0, ///< Radiance weight table source = input-0
    SHDR_RAD_WGT_BY_IN1  = 1, ///< Radiance weight table source = input-1
    ENUM_DUMMY4WORD(SHDR_RAD_WGT_SRC_SEL)
} SHDR_RAD_WGT_SRC_SEL;

typedef enum
{
    SHDR_RATIO_GAIN_1X  = 0, ///< 1x ratio gap
    SHDR_RATIO_GAIN_2X  = 1, ///< 2x ratio gap
    SHDR_RATIO_GAIN_4X  = 2, ///< 4x ratio gap
    SHDR_RATIO_GAIN_8X  = 3, ///< 8x ratio gap
    SHDR_RATIO_GAIN_16X = 4, ///< 16x ratio gap
    ENUM_DUMMY4WORD(SHDR_RATIO_GAIN)
} SHDR_RATIO_GAIN;


typedef struct
{
    UINT8       uiLpf[8];    ///< low pass filter A/B/C/D/E/F/G/H
    UINT32      uiLpfNormM;  ///< low-pass-filter normalization factor M. No need to configure.
    UINT32      uiLpfNormS;  ///< low-pass-filter normalization factor S. No need to configure.
} SHDR_REF_LA_LPF_INFO;

typedef struct
{
    UINT16 uiGblTnTbl[41];  ///< global tone table, 41taps x 12bits
} SHDR_GLOBAL_TONE_INFO;

typedef struct
{
    UINT32  uiBand0Gain;    ///< gain for band-0, 10 bits as 1.9 bits
    UINT32  uiBand1Gain;    ///< gain for band-1, 10 bits as 1.9 bits
} SHDR_LOCAL_TONE_INFO;


typedef struct
{
    UINT8   uiARto[9];    ///< regulation on ratio, considering data A, 9taps x 8bits
    UINT8   uiAMax[9];    ///< regulation on max. , considering data A, 9taps x 8bits
    UINT8   uiAMin[9];    ///< regulation on min. , considering data A, 9taps x 8bits
    UINT8   uiBMax[9];    ///< regulation on max. , considering data B, 9taps x 8bits
    UINT8   uiBMin[9];    ///< regulation on min. , considering data B, 9taps x 8bits
} SHDR_REGULATION_INFO;


typedef struct
{
    UINT16  uiPointX[4];
    UINT16  uiPointY[4];
    UINT16  uiSlopeM[4];
    UINT8   uiSlopeS[4];
} SHDR_RECURVEN_INFO;


typedef struct
{
    UINT32      uiIn0EvGap;     ///< The EV-gap for input-0, for the bright one
    UINT32      uiIn1EvGap;     ///< The EV-gap for input-1, for the dark one
    SHDR_RAD_WGT_SRC_SEL RadWgtSrc;  ///< Radiance weight table source
    UINT8       uiWgtTbl[17];   ///< radiance weight table
} SHDR_RAD_INFO;


typedef struct
{
    SHDR_RAD_INFO            RadInfo;         ///<
    SHDR_REF_LA_LPF_INFO     RefLaLpfInfo;    ///<
    SHDR_GLOBAL_TONE_INFO    GblTnInfo;       ///<
    SHDR_LOCAL_TONE_INFO     LocTnInfo;       ///<
    SHDR_REGULATION_INFO     ReguInfo;        ///<
    SHDR_RECURVEN_INFO       ReCrvInfo;       ///<
} SHDR_PARAM;


typedef struct _SHDR_AE_LV_INFO
{
    UINT32 uiAeLvMin;   ///< min. value of LV
    UINT32 uiAeLvMax;   ///< max. value of LV
    UINT32 uiAeLvTor;   ///< tolerance value of LV
    UINT32 uiAeLvCur;   ///< current value of LV
} SHDR_AE_LV_INFO;


typedef struct _SHDR_OPEN_INFO
{
    SHDR_MODE_SELECTION  OperationMode;   ///< sensor-HDR, mode selection
    UINT32 *puiRGBGammaLut;   ///< IPE rgb gamma LUT, 130 elements
} SHDR_OPEN_INFO;


typedef struct _SHDR_CAL_INFO
{
    UINT16  *puiCurBrightLA32x32;   ///< current 32x32 LA window for bright image, 12bits data and post color-gain
    UINT16  *puiCurDarkLA32x32;     ///< current 32x32 LA window for dark image, 12bits data and post color-gain
    UINT32  *puiAeMatrix8x8;        ///< 8x8 AE matrix
    BOOL    bAeMatrixUpdate;        ///< AE matrix update, TRUE: enable update / FALSE: disable update
    INT32   uiGlobalToneGain;       ///< global tone gain;  1x = 1024;
    SHDR_AE_LV_INFO *pAeLvInfo;     ///< LV information of AE


	UINT32  uiMaxTagLum;               ///< user define max target luminance, 8bit value; range: 0 ~ 255
    UINT32  uiContrastGain;         ///< contrast gain, range: 0 ~ 100
    UINT32  uiLumaUprBnd;           ///< upper bound luminance for processing, 12bit value
    UINT32  uiLumaLorBnd;           ///< lower bound luminance for processing, 12bit value
    UINT32  uiWt;                   ///< gamma weighting, max = 16
} SHDR_CAL_INFO;
//----------------------------------------------------------

extern UINT32 SensorHDR_GetAeRatio(VOID);
//----------------------------------------------------------


extern ER SensorHDR_Init(SHDR_OPEN_INFO *pOpenInfo);
//----------------------------------------------------------


extern ER SensorHDR_Proc(SHDR_CAL_INFO *pHdrCalInfo);
//----------------------------------------------------------


extern VOID SensorHDR_GetParam(SHDR_PARAM *pHdrParams);
//----------------------------------------------------------



#ifdef __cplusplus
}
#endif


#endif




