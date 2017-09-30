
#ifndef _SCENE_RENDER_LIB_H_
#define _SCENE_RENDER_LIB_H_


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

/*
    Scene Render enum - processing ID
*/
typedef enum
{
    SR_WDR_PROCID1 = 0,  ///< processing ID 1
    SR_WDR_PROCID2 = 1,  ///< processing ID 2
    SR_WDR_TOTAL
} SR_WDR_PROCID;


typedef enum
{   
    SR_WDR_COLORSAT_LEVEL01 = 1,
    SR_WDR_COLORSAT_LEVEL02 = 2,
    SR_WDR_COLORSAT_LEVEL03 = 3,
    SR_WDR_COLORSAT_LEVEL04 = 4,
    SR_WDR_COLORSAT_LEVEL05 = 5,
    SR_WDR_COLORSAT_LEVEL06 = 6,
    SR_WDR_COLORSAT_LEVEL07 = 7,
    SR_WDR_COLORSAT_LEVEL08 = 8,
    SR_WDR_COLORSAT_LEVEL09 = 9,
    SR_WDR_COLORSAT_LEVEL10 = 10,
} SR_WDR_COLORSAT_LEVEL;


typedef enum
{   
    SR_WDR_CONTRAST_LEVEL01 = 1,
    SR_WDR_CONTRAST_LEVEL02 = 2,
    SR_WDR_CONTRAST_LEVEL03 = 3,
    SR_WDR_CONTRAST_LEVEL04 = 4,
    SR_WDR_CONTRAST_LEVEL05 = 5,
    SR_WDR_CONTRAST_LEVEL06 = 6,
    SR_WDR_CONTRAST_LEVEL07 = 7,
    SR_WDR_CONTRAST_LEVEL08 = 8,
    SR_WDR_CONTRAST_LEVEL09 = 9,
    SR_WDR_CONTRAST_LEVEL10 = 10,
} SR_WDR_CONTRAST_LEVEL;


typedef enum
{   
	SR_WDR_BLACK_LEVEL00 = 0,
    SR_WDR_BLACK_LEVEL01 = 1,
    SR_WDR_BLACK_LEVEL02 = 2,
    SR_WDR_BLACK_LEVEL03 = 3,
    SR_WDR_BLACK_LEVEL04 = 4,
    SR_WDR_BLACK_LEVEL05 = 5,
    SR_WDR_BLACK_LEVEL06 = 6,
    SR_WDR_BLACK_LEVEL07 = 7,
    SR_WDR_BLACK_LEVEL08 = 8,
    SR_WDR_BLACK_LEVEL09 = 9,
    SR_WDR_BLACK_LEVEL10 = 10,
} SR_WDR_BLACK_LEVEL;


typedef enum
{
	SR_WDR_EDGE_LEVEL00 = 0,
	SR_WDR_EDGE_LEVEL01 = 1,
	SR_WDR_EDGE_LEVEL02 = 2,
	SR_WDR_EDGE_LEVEL03 = 3,
	SR_WDR_EDGE_LEVEL04 = 4,
	SR_WDR_EDGE_LEVEL05 = 5,
	SR_WDR_EDGE_LEVEL06 = 6,
	SR_WDR_EDGE_LEVEL07 = 7,
	SR_WDR_EDGE_LEVEL08 = 8,
	SR_WDR_EDGE_LEVEL09 = 9,
	SR_WDR_EDGE_LEVEL10 = 10,
} SR_WDR_EDGE_LEVEL;


typedef enum
{
    SR_WDR_CHG_LUMA_WEIGHT_MAP      = 0x00000001,  ///< change luma weighting map selection
    SR_WDR_CHG_LUMA_GBL_WEIGHT_RTO  = 0x00000002,  ///< change luma global weighting ratio
    SR_WDR_CHG_CONTRAST_LEVEL       = 0x00000004,  ///< change contrast level selection
    SR_WDR_CHG_COLORSAT_LEVEL       = 0x00000008,  ///< change color saturation level selection
    SR_WDR_CHG_LUMA_DARK            = 0x00000010,  ///< chgange luma of dark part
    SR_WDR_CHG_LUMA_BRIGHT          = 0x00000020,  ///< change luma of bright part
    SR_WDR_CHG_CONTRAST_OB_MAP      = 0x00000040,  ///< change contrast OB map selection
    SR_WDR_CHG_LOCAL_GAIN           = 0x00000080,  ///< change local contrast gain
    SR_WDR_CHG_LUMA_DARK_LEVEL      = 0x00000100,  ///< chgange dark luma level
    SR_WDR_CHG_LUMA_BRIGHT_LEVEL    = 0x00000200,  ///< change bright luma level
    SR_WDR_CHG_BLACK_LEVEL          = 0x00000400,  ///< change black level
    SR_WDR_CHG_EDGE_LEVEL           = 0x00000800,  ///< change edge level
} SR_WDR_CHG_SEL;


typedef enum
{
    SR_WDR_PROC_20    = 0,    ///< WDR2.0
    SR_WDR_PROC_25    = 1,    ///< WDR2.5
} SR_WDR_PROC_SEL;


typedef enum
{
    SR_DEFOG_PROC_OFF   = 0,    ///< enable defog function
    SR_DEFOG_PROC_ON    = 1,    ///< disable defog function
} SR_DEFOG_PROC_SEL;



/*
    Scene Render structure - open WDR operation
*/
typedef struct _SR_WDR_OPEN_INFO
{
    UINT32 *puiRGBGammaLut;   ///< fixed gamma LUT, 130 elements
    SR_WDR_PROC_SEL WdrProcSel; ///< operation model selection for WDR
    SR_DEFOG_PROC_SEL DefogProcEn;	///< defog process enable
} SR_WDR_OPEN_INFO;

/*
    Scene Render structure - AE LV information
*/
typedef struct _SR_WDR_AE_LV_INFO
{
    UINT32 uiAeLvMin;   ///< min. value of LV
    UINT32 uiAeLvMax;   ///< max. value of LV    
    UINT32 uiAeLvCur;   ///< current value of LV
    UINT32 uiAeLvTolDark;   ///< LV tolerance value for dark
    UINT32 uiAeLvTolBright;   ///< LV tolerance value for bright
} SR_WDR_AE_LV_INFO;


/*
    Scene Render structure - Input parameters of WDR computation
*/
typedef struct _SR_WDR_CAL_INFO
{
    UINT16                  *puiLA32x32;            ///< 32x32 LA window, 10bit/each pixel       
    SR_WDR_AE_LV_INFO       *pAeLvInfo;             ///< AE LV information  
    UINT32                  uiVdoFrameRate;         ///< video frame rate
    UINT32                  uiCalFrameRate;         ///< computing skip frame rate
} SR_WDR_CAL_INFO;

/*
    Scene Render structure - Change parameters for WDR effect
*/
typedef struct _SR_WDR_CHG_PARAMS_
{
    UINT32                  uiChgSel;               ///< change parameter selection, for example: uiChgSel = (SR_WDR_CHG_LUMA_WETMAP | SR_WDR_CHG_CONTRAST_LEVEL). please refer to "SR_WDR_CHG_SEL"
    UINT32                  *puiLumaWeightMap;      ///< curve weighting map with 256 elements;, value range = [0, 256]; value->0: system gamma curve, value->256: WDR curve
    UINT32                  uiLumaGblWeightRatio;   ///< luma global weighting ratio, 1x = 128, default = 128
    SR_WDR_CONTRAST_LEVEL   uiContrastLevel;        ///< contrast level control, default = SR_WDR_CONTRAST_LEVEL03
    INT32                   *piContratOBMap;        ///< contrast OB control with 256 elements; value range = [0, 255], value -> 0:  low contrast, value ->256: high contrast
    SR_WDR_COLORSAT_LEVEL   uiColorSatLevel;        ///< color saturation control, default = SR_WDR_COLORSAT_LEVEL06;
    UINT32                  uiBrightLumaRatio;      ///< luma ratio of bright part, range: [0, 1024]; 0 -> 1024: increasing luminance, less visual details; 1024 -> 0: decreasing luminance, more visual details; default = 1024
    UINT32                  uiDarkLumaRatio;        ///< luma ratio of dark part, range: [0, 1024]; 0 -> 1024: increasing luminance, more visuall details(including noise); 1024 -> 0: decreasing luminance, less visual details; default = 742
    UINT32                  *puiLocalGain;          ///< local gain with 256 elements, range: [512, 1023];  512 -> 1024, increasing local contrast
    UINT32                  uiDarkLumaLevel;        ///< change dark luma level, range: [100, 400], default = 200
    UINT32                  uiBrightLumaLevel;      ///< change bright luma level, range: [100, 200], default = 105
    SR_WDR_BLACK_LEVEL      uiBlackLevel;           ///< change black level,  range: [0, 10]
    SR_WDR_EDGE_LEVEL		uiEdgeLevel;			///< change edge level,  range: [0, 10]
} SR_WDR_CHG_PARAMS;

/*
    Scene Render structure - Get computed WDR parameters
*/
typedef struct _SR_WDR_PARAM
{
     UINT32 *puiGetToneCurve;   ///< output WDR tone curve, 130 elements
     UINT32 *puiGetEdgeCurve;   ///< output edge curve, 65 elements
     INT16  *piGetSat;          ///< output to adjust color saturation, 1 elements
     UINT32 *puiGetBandGain;    ///< output to local enhancement, 2 elements
} SR_WDR_PARAM;

//----------------------------------------------------------

/**
    Parameter initialization

    @param[in] ProcId       process ID
    @param[in] pOpenInfo    initial parameter for opening WDR lib.

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER SceneRender_WDR_Init(SR_WDR_PROCID ProcId, SR_WDR_OPEN_INFO *pOpenInfo);
//----------------------------------------------------------

/**
    Change paremeters for the global effect of WDR

    @param[in] ProcId       process ID
    @param[in] pChgParams   parameters for adjustment

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER SceneRender_WDR_ChgParams(SR_WDR_PROCID ProcId, SR_WDR_CHG_PARAMS *pChgParams);
//----------------------------------------------------------

/**
    Parameter computation for WDR effect

    @param[in] ProcId        process ID
    @param[in] pWdrCalInfo   parameters for computation

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER SceneRender_WDR_Proc(SR_WDR_PROCID ProcId, SR_WDR_CAL_INFO *pWdrCalInfo);
//----------------------------------------------------------

/**
    Get computed parameters of WDR for setting

    @param[in] ProcId          process ID
    @param[in] pGetWdrParams   get computed parameters

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern VOID SceneRender_WDR_GetParam(SR_WDR_PROCID ProcId, SR_WDR_PARAM *pGetWdrParams);
//----------------------------------------------------------

/**
    Get Min Value of Histogram

    @param[in] ProcId          process ID

    @return UINT32      minima value\n
*/
extern UINT32 SceneRender_WDR_GetHistMinBin(SR_WDR_PROCID ProcId);
//----------------------------------------------------------



#ifdef __cplusplus
}
#endif


#endif




