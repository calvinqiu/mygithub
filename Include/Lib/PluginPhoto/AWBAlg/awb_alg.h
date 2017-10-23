/**
    Auto White Balance Algorithm.

    depend on current image information to calculate white balance gain

    @file       awb_alg.h
    @ingroup    mILibAWBAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _AWB_ALG_H_
#define _AWB_ALG_H_
#ifdef WIN32
#define ENUM_DUMMY4WORD(a)
#endif

#include "awb_task.h"

#define AWB_FUNCCTRL_FACEMATCH_EN      0x00000001    //Enable to match the face color,

#define AWB_FUNCCTRL_R2_PPROC_EN       0x20000001    //Enable high\low color temperature postprocess
#define AWB_FUNCCTRL_R2_CTTAB_EN       0x20000002    //Enable color temperature table
#define AWB_FUNCCTRL_R2_STABLE_EN      0x20000004    //Enable gain stable
#define AWB_FUNCCTRL_R2_MULTICT_EN     0x20000008    //Enable to check multi color temperature
#define AWB_FUNCCTRL_R2_REMAP_EN       0x20000010    //Enable to remap to check white block
#define AWB_FUNCCTRL_R2_MULTI_KGAIN_EN 0x20000020    //Enable advance kgain
#define AWB_FUNCCTRL_R2_COLORMATCH_EN  0x20000040    //Enable color match

#define AWB_ACC_BIT         12   //Define CA ACC Bit Cnt

#define AWB_CABUFFER_SIZE 1024


#define AWB_BASE_KGAIN      100

/**
     @name AWB Alg. default value
     @note
*/
typedef enum _AWBALG_MODE
{
    AWBALG_MODE_NVTALG_R1=0,
    AWBALG_MODE_NVTALG_R2,
    AWBALG_MODE_GRAYWORLD,
    ENUM_DUMMY4WORD(AWBALG_MODE)
}AWBALG_MODE;
#define AWBALG_MODE_NVTALG AWBALG_MODE_NVTALG_R1 //define for old version

/**
     @name AWB Alg. default value
     @note
*/
typedef enum _AWBALG_CH
{
    AWBALG_CH_R = 0,
    AWBALG_CH_G,
    AWBALG_CH_B,
    AWBALG_CH_MAX,
    ENUM_DUMMY4WORD(AWBALG_CH)
}AWBALG_CH;

#define AWBALG_CH_RGAIN AWBALG_CH_R
#define AWBALG_CH_BGAIN AWBALG_CH_B
#define AWBALG_CH_CT    AWBALG_CH_G

/**
     CA PreElement Info.

     @note
*/
typedef struct _AWB_CAINFO
{
    UINT32 WinNumX;
    UINT32 WinNumY;
    UINT32 RGu;
    UINT32 RGl;
    UINT32 GGu;
    UINT32 GGl;
    UINT32 BGu;
    UINT32 BGl;
    UINT32 RBGu;
    UINT32 RBGl;
}AWB_CAINFO;

/**
     check type.

     @note
*/
typedef enum _AWBALG_TYPE
{
    AWBALG_TYPE_Y = 0,
    AWBALG_TYPE_RG,
    AWBALG_TYPE_BG,
    AWBALG_TYPE_RBG,
    AWBALG_TYPE_SAT,
    AWBALG_TYPE_RBGSUM,
    AWBALG_TYPE_RXBG,
    AWBALG_TYPE_RBGDIF,
    AWBALG_TYPE_MAX,
    AWBALG_TAB_END = 0xffffffff,
    ENUM_DUMMY4WORD(AWBALG_TYPE)
}AWBALG_TYPE;


/**
     check type error.

     @note
*/
typedef enum _AWBALG_ER
{
    AWBALG_FAIL = 0,
    AWBALG_OK = 1,
    AWBALG_PAR = 2,
    AWBALG_GAIN_ADJ = 3,
    AWBALG_PDLIGHT = 4,
    AWBALG_Y = 100,
    AWBALG_RG = 110,
    AWBALG_BG = 120,
    AWBALG_RBG = 130,
    AWBALG_SAT = 140,
    AWBALG_RBGSUM = 150,
    AWBALG_RXBG = 160,
    AWBALG_RBGDIF = 170,
    AWBALG_USER = 180,
    AWBALG_OUTL = 190,
    AWBALG_GREEN = 200,
    AWBALG_CTTAB = 210,
    AWBALG_MULTICT = 220,
    AWBALG_MAX,
    ENUM_DUMMY4WORD(AWBALG_ER)
}AWBALG_ER;

/**
     AWB AppMode.

     @note
*/
typedef enum _AWB_APPMODE
{
    AWB_APPMODE_NORMAL = 0,
    AWB_APPMODE_STITCH,
    AWB_APPMODE_SHDR,
    AWB_APPMODE_END = 0xffffffff,
    ENUM_DUMMY4WORD(AWB_APPMODE)
}AWB_APPMODE;

/**
     AWB element.

     @note
*/
typedef struct _AWBALG_ELEMET
{
    AWBALG_TYPE Type;
    UINT32 ValH;
    UINT32 ValL;
    UINT32 ValHR[2];
    UINT32 ValLR[2];
}AWBALG_ELEMET;

/**
     AWB Path.

     @note
*/
typedef enum _AWBALG_ENV
{
    AWBALG_ENV_Night = 1,
    AWBALG_ENV_InDoor = 2,
    AWBALG_ENV_OutDoor = 3,
    AWBALG_ENV_Other_A = 4,
    AWBALG_ENV_Other_B = 5,
    AWBALG_ENV_NULL = 6,
    AWBALG_ENV_MAX,
    ENUM_DUMMY4WORD(AWBALG_ENV)
}AWBALG_ENV;


/**
     AWB Table.

     @note
*/
typedef struct _AWBALG_TAB
{
    UINT32 EvH;
    UINT32 EvL;
    AWBALG_ELEMET *Element;
    AWBALG_ENV Env;
}AWBALG_TAB;


/**
     AWB PreDefined Light Table.
     @note
*/
typedef struct _AWBALG_PDLTAB
{
    UINT32 RGain;
    UINT32 BGain;
    UINT32 Range;
}AWBALG_PDLTAB;

typedef UINT32 (*USER_CHK_WHITE)(UINT32 id, UINT32 R, UINT32 G, UINT32 B);
typedef UINT32 (*USER_ADJ_GAIN)(UINT32 id, UINT32 *nRgain, UINT32 *nBgain);

/**
     AWB algorithm event.
     @note
*/
typedef struct _AWBALG_EVENT_TAB
{
    //AWBALG_ChkWhite event
    USER_CHK_WHITE UserChkWhite;    ///< SUCCESS/FAIL return AWBALG_OK/AWBALG_USER
    USER_ADJ_GAIN  UserAdjGain;

}AWBALG_EVENT_TAB;

/**
     AWB CT Table
     @note
*/
typedef struct _AWBALG_GAIN
{
    UINT32 RGain;
    UINT32 GGain;
    UINT32 BGain;
}AWBALG_GAIN;

typedef struct _AWB_CTINFO
{
    UINT32 Temperature;
    AWBALG_GAIN Gain;
}AWB_CTINFO;

typedef struct
{
    UINT16 R;
    UINT16 G;
    UINT16 B;
} AWBALG_RGB_INFO;

typedef struct _AWBALG_REMAP_INFO
{
    UINT32 CT_VectorAngle;
    UINT32 HCT_RotateAngle;
    UINT32 LCT_RotateAngle;

    AWBALG_RGB_INFO HCTInfo;
    AWBALG_RGB_INFO MCTInfo;
    AWBALG_RGB_INFO LCTInfo;
} AWBALG_REMAP_INFO;

typedef struct _AWBALG_COLORBLOCK
{
    UINT16  x;               ///< result start x
    UINT16  y;               ///< result start y
    UINT16  w;               ///< result width
    UINT16  h;               ///< result height
} AWBALG_COLORBLOCK, *P_AWBALG_COLORBLOCK;


typedef struct _AWBALG_COLORMATCH_INFO
{
    UINT32 wImgWidth;
    UINT32 wImgHeight;
    UINT32 TargetR;
    UINT32 TargetG;
    UINT32 TargetB;
    UINT32 ColorBlockCnt;
    AWBALG_COLORBLOCK ColorBlockTab[32];
} AWBALG_COLORMATCH_INFO, *P_AWBALG_COLORMATCH_INFO;


typedef struct _AWBALG_STABLE_GAIN
{
    UINT32 RGain;
    UINT32 GGain;
    UINT32 BGain;
    UINT32 Score;
    UINT32 Tolerance;   ///< 256 is 100%
}AWBALG_STABLE_GAIN;


#define AWB_KGAIN_MAPTAB_SIZE 64

typedef struct _AWBALG_KGAIN
{
    UINT16 RGain;  //awb gain of golden sample
    UINT16 BGain;
    UINT16 KRGain; //calibration gain of test sample
    UINT16 KBGain;
}AWBALG_KGAIN;

#define AWB_KGAIN_CALICNT_MAX 2
typedef struct _AWBALG_KGAIN_INFO
{
#if 0
    //single color temperature
    UINT16 KRGain;
    UINT16 KBGain;

    //multiple color temperature
    AWBALG_KGAIN Gain[2];

    //Use by kernel
    UINT16 KRGainTab[AWB_KGAIN_MAPTAB_SIZE];
    UINT16 KBGainTab[AWB_KGAIN_MAPTAB_SIZE];
#endif
    AWBALG_KGAIN KGain[AWB_KGAIN_CALICNT_MAX];

    //Use by kernel
    UINT16 KRGainTab[AWB_KGAIN_MAPTAB_SIZE];
    UINT16 KBGainTab[AWB_KGAIN_MAPTAB_SIZE];

}AWBALG_KGAIN_INFO;

/**
     AWB CT Parameter
     @note
*/
typedef struct _AWBALG_CT_PARAM
{
    UINT32 HCTMaxEV;
    UINT32 HCTMinEV;
    AWBALG_GAIN HCTMaxGain;
    AWBALG_GAIN HCTMinGain;
    UINT32 LCTMaxEV;
    UINT32 LCTMinEV;
    AWBALG_GAIN LCTMaxGain;
    AWBALG_GAIN LCTMinGain;
}AWBALG_CT_PARAM;

/**
     AWB Postprocess
     @note
*/
typedef struct _AWBALG_POSTPROC_PARAM
{
    UINT32 HCTRRatio; //100~200
    UINT32 HCTBRatio; //100~200
    AWBALG_GAIN HCTMaxGain;
    AWBALG_GAIN HCTMinGain;
    UINT32 LCTRRatio;  //100~200
    UINT32 LCTBRatio;  //100~200
    AWBALG_GAIN LCTMaxGain;
    AWBALG_GAIN LCTMinGain;
}AWBALG_POSTPROC_PARAM;

/**
     AWB input information.

     @note
*/
typedef struct _AWBALG_INFO
{
    UINT32 Id;
    AWBALG_MODE AwbAlgMode;
    UINT32 CurEV;
    UINT32 WinNumX;
    UINT32 WinNumY;
    UINT16 *AccTab[AWBALG_CH_MAX];
    UINT16 *AccCnt;
    UINT16 *CalTab[AWBALG_CH_MAX];
    UINT32 MinWTotal;
    UINT16 *WTab;
    UINT16 *FlagTab;
    AWBALG_TAB *Tab;
    UINT32 TabCnt;
    AWBALG_PDLTAB *PDLightTab;
    UINT32 PDLightTabCnt;
    AWBALG_CT_PARAM *CTParam;
    AWBALG_POSTPROC_PARAM *PostParam;
    AWBALG_EVENT_TAB EventFp;
    UINT32 FuncCtrl;
    AWBALG_KGAIN_INFO  KGainInfo;
    AWBALG_REMAP_INFO AwbRemapInfo;
    AWBALG_STABLE_GAIN StableGain;

    AWBALG_GAIN DefGain;
    AWBALG_GAIN PreGain;
    AWBALG_GAIN AWBGain;
    AWBALG_GAIN MWBGain;
    UINT32 AwbConvSpeed;
    UINT32 AwbConvStep;
    UINT32 AWBCnt;
    //UINT32 AWBEnter;
    UINT32 AWBEnterCnt;

    AWBALG_COLORMATCH_INFO ColorMatchInfo;

    BOOL AwbWeightEn;

} AWBALG_INFO;

/**
     AWB debug information.

     @note
*/
typedef struct _AWBALG_DEBUG_INFO
{
    UINT32 StartTag;
    UINT32 ImgWidth;
    UINT32 ImgHeight;
    UINT32 ImgBitDepth;
    UINT32 CurRgain;
    UINT32 CurGgain;
    UINT32 CurBgain;
    UINT32 PrevRgain;
    UINT32 PrevGgain;
    UINT32 PrevBgain;
    UINT32 CapRgain;
    UINT32 CapGgain;
    UINT32 CapBgain;
    UINT32 EV;
    UINT32 LV;
    UINT32 CABits;
    UINT16 CA_R[AWB_CABUFFER_SIZE];
    UINT16 CA_G[AWB_CABUFFER_SIZE];
    UINT16 CA_B[AWB_CABUFFER_SIZE];
    UINT16 CACnt[AWB_CABUFFER_SIZE];
    UINT32 FaceTabCnt;
    AWBALG_COLORBLOCK FaceTab[32];
} AWBALG_DEBUG_INFO;

/**

     calculate Rgain, Bgain

     @note

     @param[in] AWBALG_INFO
     @param[out] Rgain
     @param[out] Bgain
     @return
         - @b AWBALG_OK: SUCCESS
*/
extern AWBALG_ER AWBSim(AWBALG_INFO *pAWBAlgInfo, UINT32 *RGain, UINT32 *GGain, UINT32 *BGain);

extern void AWB_SetAlgMode(AWBALG_MODE mode);
extern AWBALG_MODE AWB_GetAlgMode(void);

extern AWBALG_ER AWBColorMatchSim(AWBALG_INFO *pAWBInfo, UINT32 *RGain, UINT32 *GGain, UINT32 *BGain,
                             UINT32 RValue, UINT32 GValue, UINT32 BValue,
                             AWBALG_COLORBLOCK *pColorBlock,
                             UINT32 wImgWidth, UINT32 wImgHeight);

extern void AWBALG_Init(AWBALG_INFO *pAWBInfo);
extern UINT32 AWBALG_GetKGain(AWBALG_INFO *pAWBInfo, UINT32 RGain, UINT32 BGain, UINT32 *KRGain, UINT32 *KBGain);



/**** AWB TUNING API ****/

#if 0
#endif

#define AWB_TUNING_CT_MAX 6
#define AWB_TUNING_W_MAX  5

typedef enum _AWB_TUNING_SET_ITEM
{
    AWB_TUNING_SET_CTTABLE  =   0,
    AWB_TUNING_SET_WTABLE,
    AWB_TUNING_SET_MANUAL,
    AWB_TUNING_SET_ALL,
    AWB_TUNING_SET_MAX
} AWB_TUNING_SET_ITEM;

typedef enum _AWB_TUNING_GET_ITEM
{
    AWB_TUNING_GET_CTTABLE  =   0,
    AWB_TUNING_GET_WTABLE,
    AWB_TUNING_GET_MANUAL,
    AWB_TUNING_GET_STATUS,
    AWB_TUNING_GET_CA,
    AWB_TUNING_GET_FLAG,
    AWB_TUNING_GET_ALL,
    AWB_TUNING_GET_MAX
} AWB_TUNING_GET_ITEM;

typedef enum _AWB_TUNING_UPDATE
{
    AWB_TUNING_UPDATE_CTTABLE       = FLGPTN_BIT(AWB_TUNING_SET_CTTABLE),
    AWB_TUNING_UPDATE_WTABLE        = FLGPTN_BIT(AWB_TUNING_SET_WTABLE),
    AWB_TUNING_UPDATE_MANUAL        = FLGPTN_BIT(AWB_TUNING_SET_MANUAL),
    AWB_TUNING_UPDATE_ALL           = FLGPTN_BIT_ALL
} AWB_TUNING_UPDATE;

// CT Table
typedef struct _AWB_TUNING_CT_ELEMENT
{
    UINT16 CT;
    UINT16 Rgain;
    UINT16 Bgain;
} AWB_TUNING_CT_ELEMENT;

typedef struct _AWB_TUNING_CTTABLE
{
    AWB_TUNING_CT_ELEMENT tab[AWB_TUNING_CT_MAX];
} AWB_TUNING_CTTABLE;

// W Table
typedef struct _AWB_TUNING_BOUNDARY
{
    INT16 UB;
    INT16 LB;
} AWB_TUNING_BOUNDARY;

typedef struct _AWB_TUNING_W_ELEMENT
{
    AWB_TUNING_BOUNDARY LV;
    AWB_TUNING_BOUNDARY Y;
    AWB_TUNING_BOUNDARY RG;
    AWB_TUNING_BOUNDARY BG;
    AWB_TUNING_BOUNDARY RBG;
    AWB_TUNING_BOUNDARY RBGSUM;
    AWB_TUNING_BOUNDARY RXBG;
} AWB_TUNING_W_ELEMENT;

typedef struct _AWB_TUNING_WTABLE
{
    AWB_TUNING_W_ELEMENT tab[AWB_TUNING_W_MAX];
} AWB_TUNING_WTABLE;

// Manual set gain
typedef struct _AWB_TUNING_MANUAL
{
    UINT32 en;
    UINT32 Rgain;
    UINT32 Bgain;
    UINT32 Ggain;
} AWB_TUNING_MANUAL;

// Current Status
typedef struct _AWB_TUNING_STATUS
{
    UINT32 curRgain;
    UINT32 curBgain;
    UINT32 curGgain;
} AWB_TUNING_STATUS;

// CA Info
typedef struct _AWB_TUNING_CA
{
    UINT32 winNum_x;
    UINT32 winNum_y;
    UINT16 tab[AWBALG_CH_MAX][32][32];
} AWB_TUNING_CA;

// FLAG Info
typedef struct _AWB_TUNING_FLAG
{
    UINT32 winNum_x;
    UINT32 winNum_y;
    UINT16 tab[32][32];
} AWB_TUNING_FLAG;

// Tuning All
typedef struct _AWB_TUNING_ALL
{
    AWB_TUNING_CTTABLE ct_tab;
    AWB_TUNING_WTABLE w_tab;
    AWB_TUNING_MANUAL manual;

    // read only
    AWB_TUNING_STATUS status;
    AWB_TUNING_CA ca;
    AWB_TUNING_FLAG flag;
} AWB_TUNING_ALL;

// AWB Tuning API
extern UINT32 AWBTuning_Set_Cmd(UINT32 Id, AWB_TUNING_SET_ITEM item, UINT32 addr, UINT32 size);
extern UINT32 AWBTuning_Get_Cmd(UINT32 Id, AWB_TUNING_GET_ITEM item, UINT32 addr, UINT32 size);

// Internel used for awb_ext
void AWBTuning_getPtr(UINT32 Id, UINT32 addr);
void AWBTuning_setInit(UINT32 Id, UINT32 addr, UINT32 size);
void AWBTuning_getUpdate(UINT32 Id, UINT32 addr);
void AWBTuning_setUpdate(UINT32 Id, UINT32 up);
INT16 AWBTuning_ev2lv(UINT32 ev);
UINT32 AWBTuning_lv2ev(INT16 lv);
void AWBTuning_setAWBInfo(UINT32 Id, AWBALG_INFO* awbInfo);

/************************/

/**** AWB Common def ****/

/**
    Color temperature range.
*/
typedef enum _AWB_CT_RANGE
{
    AWB_CT_RANGE_NONE   = 0x00000000,     ///< not in range
    AWB_CT_RANGE_1      = 0x00000001,     ///< awb ct range 1
    AWB_CT_RANGE_2      = 0x00000002,     ///< awb ct range 2
    AWB_CT_RANGE_3      = 0x00000004,     ///< awb ct range 3
    AWB_CT_RANGE_4      = 0x00000008,     ///< awb ct range 4
    AWB_CT_RANGE_5      = 0x00000010,     ///< awb ct range 5
    AWB_CT_RANGE_6      = 0x00000020,     ///< awb ct range 6
    AWB_CT_RANGE_7      = 0x00000040,     ///< awb ct range 6
    AWB_CT_RANGE_MAX    = 8,
    ENUM_DUMMY4WORD(AWB_CT_RANGE)
} AWB_CT_RANGE;

typedef struct _AWB_PARAM_Pr
{
    AWBALG_INFO*            Now_AWB_Info;
    AWB_CAINFO*             Now_CA_Info;
    AWB_CTINFO*             Now_CT_Tab;
    AWBALG_TAB*             Now_W_Tab;
    AWBALG_CT_PARAM*        Now_CT_Param;
    AWBALG_POSTPROC_PARAM*  Now_Post_Param;
    UINT32*                 Now_MWB_Tab;
}AWB_PARAM_Pr;

extern AWB_PARAM_Pr All_Param_Pr[AWB_ID_MAX_NUM];

typedef UINT32 (*CA_DUMP_FP)(UINT32 Id);
extern void CA_DumpFP(CA_DUMP_FP fp);

typedef UINT32 (*Flag_DUMP_FP)(UINT32 Id);
extern void Flag_DumpFP(Flag_DUMP_FP fp);

extern UINT32 AWB_DumpCAInfo(UINT32 id);
extern UINT32 AWB_DumpFlagInfo(UINT32 id);

/**
    Color temperature calculation type.
*/
typedef enum _AWBALG_CT_CALMODE
{
    AWBALG_CT_CAL_R_GAIN,
    AWBALG_CT_CAL_B_GAIN,
    AWBALG_CT_CAL_RB_GAIN,
    ENUM_DUMMY4WORD(AWBALG_CT_CALMODE)
}AWBALG_CT_CALMODE;

void AWB_GetCurCT_R(UINT32 id, UINT32 Rgain, UINT32 Bgain, UINT32 *CT);
void AWB_GetCurCT_B(UINT32 id, UINT32 Rgain, UINT32 Bgain, UINT32 *CT);
void AWB_GetCurCT_RB(UINT32 id, UINT32 Rgain, UINT32 Bgain, UINT32 *CT);
//void AWB_GetCurCT(UINT32 id, AWBALG_CT_CALMODE CTCalMode, UINT32 CurRgain, UINT32 CurBgain, UINT32 *CT);

#endif //_AWB_ALG_H_
