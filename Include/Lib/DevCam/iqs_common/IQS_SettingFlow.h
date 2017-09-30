/**
    Internal header file for IPP_sensor

    Header files for differnt IPP_sensor

    @file       IQS_SettingFlow.h
    @ingroup    mILibIPLCom
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _IQS_SETTINGFLOW_H
#define _IQS_SETTINGFLOW_H

#include <string.h>
#include <stdio.h>

#include "IPL_AlgInfor.h"
#include "IPL_Hal_SIE_Com.h"
#include "IPL_Hal_IFE.h"
#include "IPL_Hal_DCE.h"
#include "IPL_Hal_IPE.h"
#include "IPL_Hal_IFE2.h"
#include "IPL_Hal_IME.h"

#include "IPL_Hal_SIE_Com_Info.h"
#include "IPL_Hal_IFE_Info.h"
#include "IPL_Hal_DCE_Info.h"
#include "IPL_Hal_IPE_Info.h"
#include "IPL_Hal_IFE2_Info.h"
#include "IPL_Hal_IME_Info.h"
#include "IPL_Hal_DRE_Info.h"

#include "Type.h"
#include "IPL_Hal.h"
#include "h264enc_api.h"

typedef struct _IQS_REFIMG_INFO
{
    UINT32 width;
    UINT32 height;
    UINT32 vratio;
    UINT32 hratio;
}IQS_REFIMG_INFO;

typedef struct
{
    UINT32 TabGain;
    UINT32 Tab[17];
    UINT32 Tab_R[17];
    UINT32 Tab_G[17];
    UINT32 Tab_B[17];
} IQS_VIG_SET;

typedef struct
{
    UINT8 CbOfs;    ///< Legal range : 0~255, Cb color offset, 128 : NO color offset
    UINT8 CrOfs;    ///< Legal range : 0~255, Cr color offset, 128 : NO color offset
    UINT8 YCon;    ///< Legal range : 0~255, Y contrast adjust, 128 : NO contrst adjust, Y' = (Y * Ycon) >> 7
    UINT8 CCon;    ///< Legal range : 0~255, CbCr contrast adjust, 128 : NO contrst adjust, C' = (C * CCon) >> 7
    INT16 IntOfs;    ///< Legal range : -128~127, Y offset, Y' = Y + Intofs
    INT16 SatOfs;   ///< Legal range : -128~127, color saturation adjust, 0 : NO color saturation adjust
    BOOL  HueRotateEn; ///< Hue adjust rotation option, 0 : NO rotation, 1 : CHUEM[n] LSB 2 bit = 0 : 0 degree rotation, 1 : 90 degree rotation, 2 bit = 2 : 180 degree rotation, 3 : 270 degree rotation
} IQS_COLOR_EFFECT_PARAM;

typedef struct
{
    UINT32 RepNum;
    IFE2_Filter_Param Filter;
} IQS_IFE2_FILTER;

typedef struct _DRE_NR_IQPARAM
{
    UINT32 IQS_Msnr_Filt_Weight[6];
    UINT32 IQS_Msnr_Filt_RngThr_Y[8];
    UINT32 IQS_Msnr_Filt_RngThr_U[8];
    UINT32 IQS_Msnr_Filt_RngThr_V[8];
    UINT32 IQS_Msnr_GamLutY_En;
    UINT32 IQS_Msnr_GamLutU_En;
    UINT32 IQS_Msnr_GamLutV_En;
    UINT32 IQS_Msnr_GamLut_Y[128];
    UINT32 IQS_Msnr_GamLut_U[128];
    UINT32 IQS_Msnr_GamLut_V[128];
    UINT32 IQS_Msnr_YCMLut_Y[16];
    UINT32 IQS_Msnr_YCMLut_C[16];
    DRE_YCMOD_CTRL_INFO IQS_Msnr_YCMCtrl;
}IQS_DRE_NR_IQPARAM;

typedef struct _DRE_IQPARAM
{
    //NR
    UINT32 uiProcRepNum;
    UINT32 uiProcRepIdx[DRE_NR_PROC_MAX_NUM];
    BOOL bDgEn;
    BOOL bDgImgRef;
    UINT32 uiDgLyrNum;
    UINT32 DgMskInIdx[DRE_DG_PROC_MAX_NUM];
}IQS_DRE_IQPARAM;

typedef enum
{
    IQCB_PRV_START = 0x00000000,
    IQCB_PRV_SIE_INIT,
    IQCB_PRV_IPE_INIT,
    IQCB_PRV_IFE_INIT,
    IQCB_PRV_DCE_INIT,
    IQCB_PRV_IFE2_INIT,
    IQCB_PRV_IME_INIT,
    IQCB_PRV_ISE_INIT,
    IQCB_PRV_RDE_INIT,
    IQCB_PRV_RHE_INIT,
    IQCB_PRV_END,

    IQCB_CAP_SUB_START = 0x00000100,
    IQCB_PREIME_D2D_SIE_SUB,
    IQCB_PREIME_D2D_DCE_SUB,
    IQCB_PREIME_D2D_IFE_SUB,
    IQCB_PREIME_D2D_IPE_SUB,
    IQCB_PREIME_D2D_IME_SUB,
    IQCB_D2D_IFE2_SUB,
    IQCB_CAP_SUB_END,

    IQCB_CAP_PRI_START = 0x00000200,
    IQCB_PREIME_D2D_SIE_PRI,
    IQCB_PREIME_D2D_DCE_PRI,
    IQCB_PREIME_D2D_IFE_PRI,
    IQCB_PREIME_D2D_IPE_PRI,
    IQCB_PREIME_D2D_IME_PRI, //enable LCA
    IQCB_D2D_IFE2_PRI,
    IQCB_D2D_DRE_PRI,
    IQCB_CAP_PRI_END,

    IQS_FLOW_START = 0x00000400,
    IQS_AE_END,
    IQS_AWB_END,
    IQS_OZOOM_END,
    IQS_DZOOM_END,
    IQS_IMAGE_EFFECT,
    IQS_WDR_END,
    IQS_MANUAL_TRIG,
    IQS_AUTO_TRIG,
    IQS_REORG_APPINFO,
    IQS_FLOW_MSG_NUM
}IQS_FLOW_MSG;

void IQS_GetRefImageInfo(UINT32 Id, INT32 SensorMode, IQS_REFIMG_INFO *Info);
extern ER IQS_SettingFlowFunc(UINT32 Id, IQS_FLOW_MSG MsgID);

//IQ parameter

typedef enum  {
	IQ_MODE = 0,
	IQ_ISOIDX,
	IQ_ISOIDX_OFFSET,

	IQ_OB_HDR,
	IQ_ECS_SET,
	IQ_ECS_TAB,

	IQ_VIG_SET,

	IQ_DCE_CA,
	IQ_GDC_GLUT,
	IQ_CFAINTER,

	IQ_EDGEKER,
	IQ_CST_COEF,
	IQ_YCCFIX,

	IQS_CCM_L,
	IQS_CCM_M,
	IQS_CCM_H,

	IQ_GAMMA_DAY,
	IQ_GAMMA_NIGHT,

	IQ_HUETAB_L,
	IQ_HUETAB_M,
	IQ_HUETAB_H,
	IQ_SATTAB_L,
	IQ_SATTAB_M,
	IQ_SATTAB_H,
	IQ_INTTAB_L,
	IQ_INTTAB_M,
	IQ_INTTAB_H,
	IQ_EDGTAB_L,
	IQ_EDGTAB_M,
	IQ_EDGTAB_H,
    IQ_EDGTAB,


	IQ_3DCC,

	IQ_SUPER_RES,
	IQS_3DNR_PARAM,

	IQ_OBSUB,
	IQ_OUTL_BRI,
	IQ_OUTL_DARK,
	IQ_RANGEA_THR,
	IQ_RANGEA_LUT,
	IQ_RANGEB_THR,
	IQ_RANGEB_LUT,
	IQ_RANGEB,
	IQ_FILTER,
	IQ_GBAL_STAB,
	IQ_GBAL_DTAB,
	IQ_GBAL,

	IQ_DCE_ENH,
	IQ_DIREDGE_EXT,
	IQ_EWA,
	IQ_DIRSCORETH,
	IQ_EWPARAM,
	IQ_EDGEENH,
	IQ_EDMAP,
	IQ_EDTAB,
	IQ_ESMAP,
	IQ_ESTAB,
	IQ_RGBLPF,
	IQ_FSTAB,
	IQ_FDTAB,
	IQ_SATURATION,
	IQ_CONTRAST,

	IQ_REFCENTER_YTH,
	IQ_REFCENTER_YWT,
	IQ_REFCENTER_Y,
	IQ_REFCENTER_UVTH,
	IQ_REFCENTER_UVWT,
	IQ_REFCENTER_UV,

	IQ_IFE2FILTER_YTH,
	IQ_IFE2FILTER_YWT,
	IQ_IFE2FILTER_UVTH,
	IQ_IFE2FILTER_UVWT,
	IQ_IFE2FILTER,

	IQ_CHROMAADAPT,
	IQ_CHROMAADAPT_COLOR,
	IQ_CHROMAADAPT_LUM,

	IQ_CHROMASUPP_WEIGHTY,
	IQ_CHROMASUPP_WEIGHTC,
	IQ_CHROMASUPP,
	IQ_3DNR_LEVEL,
	IQ_3DNR_PARAM,
	IQ_NUM_3DNR_PARAM,
	IQ_AWDRLV,
    IQ_PARAM_MAX
} IQ_PARAM_ITEM;

void iqs_set_addr(UINT32 id, UINT32** addr);

//Auto WDR OverExp
typedef struct _WDR_TH
{
    UINT32 l;
    UINT32 h;
}WDR_TH;

typedef struct _AUTO_WDR_OVEREXP
{
    WDR_TH DarkGainTH;
    WDR_TH BrightGainTH;
    WDR_TH BlackLevelTH;
    WDR_TH SharpnessTH;
    UINT32 OverExp_l;
    UINT32 OverExp_h;
}AUTO_WDR_OVEREXP;

typedef struct _AUTO_WDR_SET
{
    UINT32 DarkGain;
    UINT32 BrightGain;
    UINT32 BlackLevel;
    UINT32 Sharpness;
    UINT32 OverExp_l;
    UINT32 OverExp_h;
}AUTO_WDR_SET;

/** IQS Tuning API struct **/

#define ISP_IQ_MAX_NUM 2
#define ISP_MAX_ISONUM 38

typedef enum  {
    ISP_ECS = 0,
    ISP_OUTLIER,
    ISP_RANGE,
    ISP_OB,
    ISP_GBAL,
    ISP_LDC,
    ISP_EDGE,
    ISP_CCTRL,
    ISP_COLORNF,
    ISP_TNR,
    ISP_GAMMA,
    ISP_CC,
    ISP_ALL,
    ISP_ISOINDEX,
    ISP_ALL_NONISO,
    ISP_AWDR,
    ISP_TNR_CONFIG,
    ISP_MAX
} ISP_IQ_ITEM;

typedef enum{
    ISP_UPDATE_ECS      =   FLGPTN_BIT(ISP_ECS),
    ISP_UPDATE_OUTLIER  =   FLGPTN_BIT(ISP_OUTLIER),
    ISP_UPDATE_RANGE    =   FLGPTN_BIT(ISP_RANGE),
    ISP_UPDATE_OB       =   FLGPTN_BIT(ISP_OB),
    ISP_UPDATE_GBAL     =   FLGPTN_BIT(ISP_GBAL),
    ISP_UPDATE_LDC      =   FLGPTN_BIT(ISP_LDC),
    ISP_UPDATE_EDGE     =   FLGPTN_BIT(ISP_EDGE),
    ISP_UPDATE_CCTRL    =   FLGPTN_BIT(ISP_CCTRL),
    ISP_UPDATE_COLORNF  =   FLGPTN_BIT(ISP_COLORNF),
    ISP_UPDATE_TNR      =   FLGPTN_BIT(ISP_TNR),
    ISP_UPDATE_GAMMA    =   FLGPTN_BIT(ISP_GAMMA),
    ISP_UPDATE_CC       =   FLGPTN_BIT(ISP_CC),
    ISP_UPDATE_ALL      =   FLGPTN_BIT_ALL,
} ISP_IQ_UPDATE;

typedef struct{
    UINT32 Id;
    UINT32 ECS_tab[4225];
} sISP_ECS;

typedef struct{
    UINT32 Id;
    UINT16 ISO_index;
    UINT32 Outl_Bri[5];
    UINT32 Outl_dark[5];
} sISP_OUTLIER;

typedef struct{
    UINT32 Id;
    UINT16 ISO_index;
    UINT32 RangeA_Thr[6];
    UINT32 RangeA_Lut[17];
    UINT32 RangeB_Thr[6];
    UINT32 RangeB_Lut[17];
    UINT32 Clamp;
    UINT32 Mul;
    UINT32 A_B_WEIGHT;
    UINT32 RthW;
} sISP_RANGE;


typedef struct{
    UINT32 Id;
    UINT16 ISO_index;
    UINT16 OB;
} sISP_OB;


typedef struct{
    UINT32 Id;
    UINT16 ISO_index;
    UINT32 Gbal_thr;
    UINT8  Gbal_Stab[16];
    UINT8  Gbal_Dtab[16];
} sISP_GBAL;


typedef struct{
    UINT32 Id;
    UINT16 ISO_index;
    UINT32 LDC_Enh[2];
    UINT32 LDC_tab[65];
    UINT32 CAC[2];
} sISP_LDC;

typedef struct{
    UINT32 Id;
    UINT16 ISO_index;
    UINT8  SCORETH[4];
    UINT8  EWparam[3];
    UINT32 ED_Enh[2];
    UINT16 ED_tab[16];
    UINT32 ED_map[4];
    UINT16 ES_tab[16];
    UINT32 ES_map[4];
} sISP_EDGE;

typedef struct{
    UINT32 Id;
    UINT16 ISO_index;
    UINT32 FS_tab[16];
    UINT32 FD_tab[16];
    UINT8  hue_tab[3][24];
    INT8   sat_tab[3][24];
    INT8   int_tab[3][24];
    INT16  saturation;
    INT16  contrast;
} sISP_CCTRL;

typedef struct{
    UINT32 Id;
    UINT16 ISO_index;
    UINT16 cnr_ratio;
} sISP_COLORNF;

typedef struct{
    UINT32 Id;
    UINT16 ISO_index;
    UINT16 TNR_level;
} sISP_TNR;

typedef struct{
    UINT32 Id;
    UINT32 gamma_tab[129];
	UINT32 gamma_tab_night[129];
} sISP_GAMMA;

typedef struct{
    UINT32 Id;
    UINT16 cc_tab[3][9];
} sISP_CC;

typedef struct{
	UINT32 Id;
	UINT32 ISO_index;
	UINT32 dark_gain_min;
	UINT32 dark_gain_max;
	UINT32 bright_gain_min;
	UINT32 bright_gain_max;
	UINT32 black_level_min;
	UINT32 black_level_max;
	UINT32 sharpness_level_min;
	UINT32 sharpness_level_max;
	UINT32 overexp_th_min;
	UINT32 overexp_th_max;
} sISP_AWDR;

typedef struct{
	UINT32 Id;
	UINT32 ISO_index;

	BOOL TNREn;
	BOOL PixelBlendEn;
	//  Threshold and Weight Config //
	UINT32 TH[3];
	UINT32 TH_Weight[3];

	//  p2p			//
	//  Luma Config //
	UINT32 p2p_TY_Weight[4][3];    ///< Temporal Filter
	UINT32 p2p_RY_TH[3];           ///< Range Filter

	// Chroma Config //
	UINT32 p2p_TC_Weight[4][3];    ///< Temporal Filter
	UINT32 p2p_RC_TH[3];           ///< Range Filter

	// Clamp Config //
	UINT32 p2p_ClampY_TH;
	UINT32 p2p_ClampY_Div;
	UINT32 p2p_ClampUV_TH;
	UINT32 p2p_ClampUV_Div;

	//  CMTF		//
	//  Luma Config //
	UINT32 mctf_TY_Weight[4][3];    ///< Temporal Filter
	UINT32 mctf_RY_TH[3];           ///< Range Filter

	// Chroma Config //
	UINT32 mctf_TC_Weight[4][3];    ///< Temporal Filter
	UINT32 mctf_RC_TH[3];           ///< Range Filter

	// Clamp Config //
	UINT32 mctf_ClampY_TH;
	UINT32 mctf_ClampY_Div;
	UINT32 mctf_ClampUV_TH;
	UINT32 mctf_ClampUV_Div;
} sISP_TNR_CONFIG;

// All parameters which will change by ISO Index
typedef struct{
    UINT32 Id;
    UINT16 ISO_index;

    UINT32 Outl_Bri[5];
    UINT32 Outl_dark[5];

    UINT32 RangeA_Thr[6];
    UINT32 RangeA_Lut[17];
    UINT32 RangeB_Thr[6];
    UINT32 RangeB_Lut[17];
    UINT32 Clamp;
    UINT32 Mul;
    UINT32 A_B_WEIGHT;
    UINT32 RthW;

    UINT16 OB;

    UINT32 Gbal_thr;
    UINT8  Gbal_Stab[16];
    UINT8  Gbal_Dtab[16];

    UINT32 LDC_Enh[2];

    UINT8  SCORETH[4];
    UINT8  EWparam[3];
    UINT32 ED_Enh[2];
    UINT16 ED_tab[16];
    UINT32 ED_map[4];
    UINT16 ES_tab[16];
    UINT32 ES_map[4];

    UINT32 FS_tab[16];
    UINT32 FD_tab[16];
    INT16  saturation;
    INT16  contrast;

    UINT16 cnr_ratio;
    UINT16 TNR_level;

	UINT32 dark_gain_min;
	UINT32 dark_gain_max;
	UINT32 bright_gain_min;
	UINT32 bright_gain_max;
	UINT32 black_level_min;
	UINT32 black_level_max;
	UINT32 sharpness_level_min;
	UINT32 sharpness_level_max;
	UINT32 overexp_th_min;
	UINT32 overexp_th_max;

	BOOL TNREn;
	BOOL PixelBlendEn;
	UINT32 TH[3];
	UINT32 TH_Weight[3];
	UINT32 p2p_TY_Weight[4][3];    ///< Temporal Filter
	UINT32 p2p_RY_TH[3];           ///< Range Filter
	UINT32 p2p_TC_Weight[4][3];    ///< Temporal Filter
	UINT32 p2p_RC_TH[3];           ///< Range Filter
	UINT32 p2p_ClampY_TH;
	UINT32 p2p_ClampY_Div;
	UINT32 p2p_ClampUV_TH;
	UINT32 p2p_ClampUV_Div;
	UINT32 mctf_TY_Weight[4][3];    ///< Temporal Filter
	UINT32 mctf_RY_TH[3];           ///< Range Filter
	UINT32 mctf_TC_Weight[4][3];    ///< Temporal Filter
	UINT32 mctf_RC_TH[3];           ///< Range Filter
	UINT32 mctf_ClampY_TH;
	UINT32 mctf_ClampY_Div;
	UINT32 mctf_ClampUV_TH;
	UINT32 mctf_ClampUV_Div;
} sISP_ALL;

// All parameters which will not change by ISO Index
typedef struct{
    UINT32 Id;
    UINT32 LDC_tab[65];
    UINT32 CAC[2];
    UINT8  hue_tab[3][24];
    INT8   sat_tab[3][24];
    INT8   int_tab[3][24];
    UINT32 GammaLut[129];
	UINT32 GammaLut_Night[129];
    UINT16 CC[3][9];
    UINT32 ECS_tab[4225];
} sISP_ALL_NONISO;

typedef struct{
    UINT32 Id;
    UINT32 num;
    UINT32 curIdx;
	UINT32 offset;
} sISP_ISOINDEX;

extern void iqs_set_addr(UINT32 id, UINT32** addr);
extern void iqs_get_addr(UINT32 id, UINT32** addr);

// Pointer of all parameters
typedef struct{
    UINT32* Outl_Bri;       // array, size=5
    UINT32* Outl_dark;      // array, size=5

    UINT32* RangeA_Thr;     // array, size=6
    UINT32* RangeA_Lut;     // array, size=17
    UINT32* RangeB_Thr;     // array, size=6
    UINT32* RangeB_Lut;     // array, size=17
    UINT32* Clamp;          // variable
    UINT32* Mul;            // variable
    UINT32* A_B_WEIGHT;     // variable
    UINT32* RthW;           // variable

    UINT16* OB;             // variable

    UINT32* Gbal_thr;       // variable
    UINT8*  Gbal_Stab;      // array, size=16
    UINT8*  Gbal_Dtab;      // array, size=16

    EnH*    LDC_Enh;        // variable
    UINT32* LDC_tab;        // array, size=65, NONISO
    Aberation* CAC;         // variable, NONISO

    IPE_DirScorCalTh* SCORETH;  // variable
    IPE_EdgeW* EWparam;     // variable
    IPE_EDGEENH* ED_Enh;    // variable
    EdgeMap*  ED_map;        // variable
    EdgeMapS* ES_map;        // variable
    UINT8*  ED_tab;         // array, size=16
    UINT8*  ES_tab;         // array, size=16

    UINT8*  FS_tab;         // array, size=16
    UINT8*  FD_tab;         // array, size=16
    UINT8*  hue_tab[3];     // array, size=24, NONISO
    INT32*  sat_tab[3];     // array, size=24, NONISO
    INT32*  int_tab[3];     // array, size=24, NONISO
    INT8*   saturation;     // variable
    INT8*   contrast;       // variable

    UINT32* cnr_ratio;      // variable

    INT8*	TNR_level;      // variable

    UINT32* GammaLut;       // array, size=129, NONISO
    UINT32* GammaLut_Night; // array, size=129, NONISO
    UINT16* CC[3];          // array, size=9, NONISO
    UINT32* ECS_tab;        // array, size=4225, NONISO

	AUTO_WDR_OVEREXP* awdr;	// variable

    UINT32  night_iso_start;// variable, NONISO, not used

	UINT32	TNR_level_num;	// variable, NONISO
	UINT32* TNR_Config;		// array, pointer of H264Enc3dnrCfg, size depend on TNR_level_num, NONISO
} sISP_PTR;

extern UINT32 ISP_IQ_get_param(ISP_IQ_ITEM item, UINT32 addr, UINT16 size);
extern UINT32 ISP_IQ_set_param(ISP_IQ_ITEM item, UINT32 addr, UINT16 size);

// IQS Internel Used
void ISP_IQ_set_iqsinfo(UINT32 Id, UINT32 iso_idx, sISP_PTR* info);
void ISP_IQ_set_isoidx(UINT32 Id, sISP_ISOINDEX* addr);

// IQST Debug Msg
#define IQS_UART_DUMP_VAR(var) (DBG_DUMP("IQST %s = %d\r\n", #var, var))
#define IQS_UART_DUMP_ARR(arr, len)     \
    do{                                 \
        UINT32 i;                       \
        DBG_DUMP("%s = { ", #arr);      \
        for(i=0; i<len; i++)            \
            DBG_DUMP("%d, ", arr[i]);   \
        DBG_DUMP("}\r\n");              \
    }while(0)

/***************************/

#if 0
typedef struct
{
    Coordinate  R;  ///<center of R
    Coordinate  Gr; ///<center of Gr
    Coordinate  Gb; ///<center of Gb
    Coordinate  B;  ///<center of B
} IQS_IFE_CENTER;

typedef struct
{
    UINT32  T;              ///<disabled area
    UINT32  TabGain;        ///<gain factor
    UINT32  AutoScaleLut;
    UINT32  XDIV;           ///<scaling factor in x direction. unsigned 0.6
    UINT32  YDIV;           ///<scaling factor in y direction. unsigned 0.6
    UINT8   *IFE_VigLut;
    UINT8   *IFE_VigLut;
    UINT8   *IFE_VigLut;
    BOOL    bDither;        ///<dithering enable/disable
    BOOL    DitherRstEn;    ///<dithering reset enable
} IQS_IFE_VIGLUT_PARAM;

typedef struct
{
    IQS_IFE_CENTER Center;
    IQS_IFE_VIGLUT_PARAM VigLutParam;
}IQS_IFE_VIG;

typedef struct
{
    UINT8 OUTLER;
} IQS_IFE_FUNC;
#endif

#endif // EOF
