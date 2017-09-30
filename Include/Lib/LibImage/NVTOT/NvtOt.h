#ifndef __NvtOt_H__
#define __NvtOt_H__

#include <Type.h>
#include <Perf.h>
#include "FileSysTsk.h"

#include "ife_lib.h"
#include "ime_lib.h"
#include "grph.h"
#include "ise_lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "clock.h"
#include "Debug.h"
#include "dma.h"
#include "SxCmd.h"
#include "kernel.h"
#include "pll.h"
#include "sdio.h"
#include "top.h"
#include "Type.h"
#include "uart.h"
#include "Utility.h"
#include "IPL_Hal_IME_Info.h"
#include "fde_lib.h"
#include "GxImage.h"
#include "NvtCv_lib.h"

#ifdef __cplusplus
extern "C" {
#endif


//	#define OT_SAVE_TRACK_RECT
//	#define OT_PROFILE_TIME_MAIN

#define OT_SHOW_PROFILE        1   //profile switch 0 = OFF, 1 = ON


typedef enum
{
    OT_CFG_INTERP_FACTOR,      ///< config inter_factor 0~128, the more larger number,the more fast model update
    OT_CFG_PADDINGSTYLE,       ///< config padding style ,0 for simple,1 for replicate
    OT_CFG_FRAME_INTERVALS,    ///< config frame_intervals, 1~5 advised;
    OT_CFG_TARGET,             ///< config target
    OT_CFG_ALG_VERSION,        ///< config algorithm version, 0 for SingleScale, 1 for MultiScale
    OT_CFG_FEATURE_TYPE,       ///< config feature type, 0 or 1
    ENUM_DUMMY4WORD(OT_CFG)
} OT_CFG;

typedef enum
{
    OT_OUT_RECT//OUT_RECT
}OT_TST_BUF_CFG;

typedef enum
{
    OT_PATH_OUT_DATA_DIR,
    OT_PATH_OUT_IMG_DIR
}OT_TEST_PATH;

typedef struct
{
    UINT32 width;                    ///< Input Img width
    UINT32 height;                   ///< Input Img height
    UINT32 lineoffset;               ///< Input Img lineoffset
    UINT32 addr;                     ///< Input Img addr
    UINT32 imgRatio;                 ///< Input Img ratio
}OT_UIMGSIZE;

typedef enum
{
    OT_OBJ_FACE = 0,
    OT_OBJ_BODY = 1,
    OT_OBJ_CAR,
    ENUM_DUMMY4WORD(OT_TARGET_TYPE)
}OT_TARGET_TYPE;

typedef enum
{
    OT_SEARCH_PROPORTIONAL = 0,
    OT_SEARCH_CUSTOM,
    ENUM_DUMMY4WORD(OT_SEARCH_AREA_SHAPE)
}OT_SEARCH_AREA_SHAPE;

////////////////////////////////////////////////////////////////////////////////
typedef enum
{
    OT_VERSION_SINGLE_SCALE = 0,
    OT_VERSION_MULTI_SCALE = 1,
    ENUM_DUMMY4WORD(OT_ALG_VERSION)
} OT_ALG_VERSION;

typedef enum
{
    OT_CELL4_BIN9 = 0,
    OT_CELL4_BIN4 = 1,
    OT_CELL8_BIN9 = 2,
    OT_CELL8_BIN4 = 3,
     ENUM_DUMMY4WORD(OT_FEATURE_TYPE)
} OT_FEATURE_TYPE;

typedef struct
{
    UINT32  ImgInBufAddr;
    UINT32  KcfBufAddr;
    UINT32  FlagAddr;
    UINT32  ParamAddr;
    UINT32  FeedBackAddr;
} NVTCV_OT_MULTI_PRMS;

typedef struct
{
    UINT32  ImgInBufAddr;
    UINT32  KcfBufAddr;
    UINT32  PatchAddr;
    UINT32  ParamAddr;
    UINT32  FeedBackAddr;
} NVTCV_OT_SINGLE_PRMS;

typedef struct _NvtCvOTStruct
{
    //input parameter
    OT_ALG_VERSION        TType;
    OT_FEATURE_TYPE       FType;
    NVTCV_OT_SINGLE_PRMS  SParam;
    NVTCV_OT_MULTI_PRMS   MParam;
} NvtCvOTStruct;


typedef struct
{
    UINT32 ImgInBufAddr[3];

} NVTCV_OT_IMG_IN;


extern BOOL NvtOt_Init(MEM_RANGE *buf);
extern void OT_SetTarget(IRECT Target);
extern BOOL OT_SetImgSize( OT_UIMGSIZE* pInputImgSize);
extern UINT32 OT_GetConfig(OT_CFG config_id);


extern void OT_SetConfig(OT_CFG config_id, INT32 Value);
extern UINT32 OT_START(VOID);
extern UINT32 OT_END(VOID);
extern UINT32 OT_CalcCacheBuffSize(void);
extern UINT32 OT_CalcBuffSize(void);
extern UINT32 OT_GetTarget(URECT* Target, URECT* TargetCoord);
extern OT_UIMGSIZE OT_GetInputImgBuf(void);

extern NVTCV_STA NvtCv_ot_Preprocess(NVTCV_FUN_INFO *pInfo);
extern NVTCV_STA NvtCv_ot_Postprocess(NVTCV_FUN_INFO *pInfo);
extern VOID NvtCv_ot_process(VOID);

//just for ot - test
extern void OT_GetFileBuf(OT_TST_BUF_CFG config_id, UINT32 *pBufAddr);
extern void OT_GetTrackRes(IRECT *targetRect);
extern VOID OT_SetTestDataPath(OT_TEST_PATH path_id, CHAR *str);

VOID OT_SetShowProflieState(BOOL flag);
BOOL OT_GetShowProflieState(VOID);


#ifdef __cplusplus
}
#endif

#endif
