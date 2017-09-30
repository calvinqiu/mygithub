#ifndef _IQUC_CTRL_IF_H_
#define _IQUC_CTRL_IF_H_

#include "Type.h"
#include "ErrorNo.h"
#include "IPL_Hal.h"

#define IQUC_ITEM_END_TAG 0x99999999

#define IQUC_ID_MAX_NUM 4
#define IQUC_CTRLITEM_MAX_NUM 50

#define IQUC_CLAMP(in,lb,ub) (((INT32)(in) < (INT32)(lb)) ? (INT32)(lb) : (((INT32)(in) > (INT32)(ub)) ? (INT32)(ub) : (INT32)(in)))


typedef enum _IQUC_ADJ_ITEM
{
    ///////////////////////////
    // scene control
    ///////////////////////////
    ADJ_IMAGEFLIP,
    ADJ_APERTURE,
    ADJ_EXPOSURE,
    ADJ_GAIN,
    ADJ_BACKLIGHT,   //WDR control
    ADJ_IR,
    ADJ_AWB_RGAIN,
    ADJ_AWB_GGAIN,
    ADJ_AWB_BGAIN,

    ///////////////////////////
    // IQ control
    ///////////////////////////
// Lens
    ADJ_VIG_R,
    ADJ_VIG_G,
    ADJ_VIG_B,
    ADJ_DISTORTION,
    ADJ_CAC_R,
    ADJ_CAC_B,

// IFE
    ADJ_RANGEA_THR,   // IQS_RangeA_Thr
    ADJ_RANGEB_THR,   // IQS_RangeB_Thr
    ADJ_RANGEA_LUT,   // IQS_RangeA_Lut
    ADJ_RANGEB_LUT,   // IQS_RangeB_Lut

    ADJ_CLAMP_TH,     // IQS_Filter.Clamp.Th
    ADJ_CLAMP_MUL,    // IQS_Filter.Clamp.Mul
    ADJ_CLAMP_DLT,    // IQS_Filter.Clamp.Dlt
    ADJ_BILAT_W,      // IQS_Filter.Bilat_w
    ADJ_RTH_W,        // IQS_Filter.Rth_W

    ADJ_GBAL_DTAB,    // IQS_Gbal_Dtab
    ADJ_GBALTHR,      // IQS_Gbal.GbalThr

    ADJ_OUTL_BRI,     // IQS_Outl_Bri
    ADJ_OUTL_DARK,    // IQS_Outl_Dark

// IPE
    ADJ_EWDIR0,       // IQS_EWParam.uiEWDir0 //vertical or horizontal edge
    ADJ_EWDIR1,       // IQS_EWParam.uiEWDir1 //bevel edge
    ADJ_EWDIR2,       // IQS_EWParam.uiEWDir2 //flat area
    ADJ_DYNEMAP,

    ADJ_SCORETH0,     // IQS_DirScoreTh.uiScoreTh0
    ADJ_SCORETH1,     // IQS_DirScoreTh.uiScoreTh1
    ADJ_SCORETH2,     // IQS_DirScoreTh.uiScoreTh2
    ADJ_SCORETH3,     // IQS_DirScoreTh.uiScoreTh3

    ADJ_ENHP,         // IQS_EdgeEnh.EnhP
    ADJ_ENHN,         // IQS_EdgeEnh.EnhN

    ADJ_EDTAB_DARK,   // IQS_EDtab
    ADJ_EDTAB_LIGHT,  // IQS_EDtab
    ADJ_ESTAB_DARK,   // IQS_EStab
    ADJ_ESTAB_LIGHT,  // IQS_EStab
    ADJ_EDMAP_ETHRL,  // IQS_EDMap.EthrL
    ADJ_EDMAP_ETABL,  // IQS_EDMap.EtabL

    ADJ_CONTRAST,     // Contrast

    ADJ_FSTAB_DARK,   // IQS_FStab
    ADJ_FSTAB_LIGHT,  // IQS_FStab
    ADJ_SATURATION,   // Saturation
    ADJ_VDETDIV,      // IQS_Cctrl.uiVdetDiv

    ADJ_RGBLPF_LPFW,  // IQS_RGBLpf.lpfw

// IFE2
    ADJ_IFE2_Y_CWT,   // IQS_RefCenter_Y.uiCntWt
    ADJ_IFE2_UV_CWT,  // IQS_RefCenter_UV.uiCntWt
    ADJ_IFE2_ED_HV_TH,// IQS_IFE2Filter.uiEdthHV
    ADJ_IFE2_ED_PN_TH,// IQS_IFE2Filter.uiEdthPN

// IME
    ADJ_CHRA_OUT_WT,  // IQS_ChromaAdapt.LcaOutUV
    ADJ_CHRA_CA_TH,   // IQS_ChromaAdapt_Color.LumTh

// MCTF
    ADJ_TNR,

    //ADJ_MFR1,           //mid Y
    //ADJ_MFR2,
    //ADJ_MFR3,
    //ADJ_MFR_TH,

    ADJ_ITEM_MAX_CNT,
    ENUM_DUMMY4WORD(IQUC_ADJ_ITEM)
} IQUC_ADJ_ITEM;

#define IQUC_ITEM_POOL_MAX_CNT (ADJ_ITEM_MAX_CNT + 1)

typedef enum _ADJIQ_ITEM
{
    ADJ_IQRatio_OB,         // 0  //  -n: value-n, 0:value, n: value+n

    ADJ_IQRatio_OUTL_BRI,   // 1  //  0: min, 100: 1x, 200: 2x, 300: 3x...
    ADJ_IQRatio_OUTL_DARK,  // 2  //  0: min, 100: 1x, 200: 2x, 300: 3x...

    ADJ_IQRatio_NR,         // 3  //  0: min, 100: 1x, 200: 2x, 300: 3x...
    ADJ_IQRatio_NR_FP,      // 4  //  0: min, 100: 1x, 200: max
    ADJ_IQRatio_NR_SPATIAL, // 5  //  0: min, 100: 1x, 200: max

    ADJ_IQRatio_GBAL,       // 6  //  0: min, 100: 1x, 200: max

    ADJ_IQRatio_EDGE_WHITE, // 7  //  0: min, 100: 1x, 200: 2x, 300: 3x...
    ADJ_IQRatio_EDGE_BLACK, // 8  //  0: min, 100: 1x, 200: 2x, 300: 3x...
    ADJ_IQRatio_EDGE_THICK, // 9  //  0: min, 100: 1x, 200: max
    ADJ_IQRatio_EDGE_START, // 10 //  0: min, 100: 1x, 200: 2x, 300: 3x...
    ADJ_IQRatio_EDGE_STEP,  // 11 //  0: min, 100: 1x, 200: max

    ADJ_IQRatio_SAT,        // 12 //  0: min, 100: 1x, 200: max
    ADJ_IQRatio_CONT,       // 13 //  0: min, 100: 1x, 200: max

    ADJ_IQRatio_LCA,        // 14 //  0: min, 100: 1x, 200: max

    ADJ_IQRatio_CLAMP_MUL,  // 15 //  0: min, 100: 1x, 200: max
    ADJ_IQRatio_CLAMP_TH,   // 16 //  0: min, 100: 1x, 200: max

    ADJ_IQRatio_MAX_CNT,

    ENUM_DUMMY4WORD(ADJIQ_ITEM)
}IQT_ADJ_ITEM;

#define IQT_ADJ_ITEM_POOL_MAX_CNT (ADJ_IQRatio_MAX_CNT + 1)

typedef enum _ADJCAC_ITEM
{
    ADJ_IQRatio_CAC_R,      // -n: value-n, 0:value, n: value+n
    ADJ_IQRatio_CAC_B,      // -n: value-n, 0:value, n: value+n
    ADJ_IQRatio_CAC_MAX_CNT,
}IQT_ADJ_CAC_ITEM;

#define IQT_ADJ_CAC_ITEM_POOL_MAX_CNT (ADJ_IQRatio_CAC_MAX_CNT + 1)

typedef struct
{
    BOOL GDC;   // Geometric Distortion Correction
    BOOL CAC;   // Color Abberation Correction
    BOOL LSC;   // Lens Shading Correction (Vignette)
} IQS_SET_ENABLE;

typedef struct
{
    UINT32 Cmd;
    UINT32 CmdData[8];
    UINT32 Id;
    UINT32 IQIndex;   //if IQIndex chnage, ask user reload
    UINT32 Status;
    INT32  ADJ_ITEM[IQUC_ITEM_POOL_MAX_CNT];
    INT32  ADJ_RATIO[IQT_ADJ_ITEM_POOL_MAX_CNT];
    INT32  ADJ_CAC_VALUE[IQT_ADJ_CAC_ITEM_POOL_MAX_CNT];
} IQUC_CTRL_ITEM;


typedef struct _IQUC_IF_
{
    IQUC_CTRL_ITEM* (*GetCtrlItem)(UINT32 Id, UINT32 Index);
    UINT32 (*SetCtrlIndex)(UINT32 Id, UINT32 Index);
    UINT32 (*GetCtrlIndex)(UINT32 Id);
    UINT32 (*SetCtrlItem)(UINT32 Id, UINT32 Index, IQUC_CTRL_ITEM *Item);
    UINT32 (*Set2ACtrlItem)(UINT32 Id, UINT32 Index, IQUC_CTRL_ITEM *Item);
    UINT32 (*SettingFlow)(UINT32 Id, UINT32 EngID, ENG_SUBFUNC *pEngSubF);
    UINT32 (*DumpInfo)(UINT32 Id,  CHAR* params, CHAR* file_name);
} IQUC_IF;


void IQUCIF_InitCtrlItem(UINT32 Id);
IQUC_CTRL_ITEM* IQUCIF_GetCtrlItem(UINT32 Id, UINT32 Index);
UINT32 IQUCIF_GetCtrlIndex(UINT32 Id);
UINT32 IQUCIF_SetCtrlIndex(UINT32 Id, UINT32 Index);
UINT32 IQUCIF_Set2ACtrlItem(UINT32 Id, UINT32 Index, IQUC_CTRL_ITEM *Item);
UINT32 IQUCIF_SetCtrlItem(UINT32 Id, UINT32 Index, IQUC_CTRL_ITEM *Item);
UINT32 IQUCIF_SettingFlow(UINT32 Id, UINT32 EngID, ENG_SUBFUNC *pEngSubF);
UINT32 IQUCIF_SetBufferInfo(UINT32 Id, UINT32 Addr, UINT32 Size);
UINT32 IQUCIF_GetBufferInfo(UINT32 Id, UINT32* Addr, UINT32* Size);
INT32 IQUCIF_GetCtrlItemToChar(UINT32 Id, CHAR *buf, UINT32 Buf_Max_Cnt);
INT32 IQUCIF_SetCtrlItemFromChar(UINT32 Id, CHAR *buf);
INT32 IQUCIF_Set2ACtrlItemFromChar(UINT32 Id, CHAR *buf);
UINT32 IQUCIF_DumpInfo(UINT32 Id,  CHAR* params, CHAR* file_name);
#endif //_IQS_CTRL_H_
