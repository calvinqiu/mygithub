/***************************************************************************
* Copyright  Novatek Microelectronics Corp. 2015.  All rights reserved.    *
*--------------------------------------------------------------------------*
* Name: DIS Library Module                                                  *
* Description:                                                             *
* Author: Yang Jin                                                         *
****************************************************************************/

/**
    DIS lib

    Sample module detailed description.

    @file       dis_alg_dsp.h
    @ingroup    mILibDIS
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2015.  All rights reserved.
*/

/** \addtogroup DISLib */
//@{

#ifndef _DIS_ALG_DSP_H_
#define _DIS_ALG_DSP_H_

#include "Type.h"
#include "dis_lib.h"
#include "NvtCv_lib.h"


typedef struct _DIS_DSP_MV_INFO_STRUCT
{
    MOTION_INFOR *motVec;
    UINT32 uiNumH;
    UINT32 uiNumV;
} DIS_DSP_MV_INFO;

typedef struct _DIS_DSP_ROIGMV_IN_STRUCT
{
    UINT8 uiStartX;                  ///< input x coordinate, 0~100%
    UINT8 uiStartY;                  ///< input y coordinate, 0~100%
    UINT8 uiSizeX;                   ///< input width, 0~100%
    UINT8 uiSizeY;                   ///< input height, 0~100%
} DIS_DSP_ROIGMV_IN;

typedef struct _DISALG_DSP_MOTION_VECTOR
{
    INT32 iX;                        ///< x component
    INT32 iY;                        ///< y component
} DISALG_DSP_MOTION_VECTOR;


typedef struct _DIS_DSP_ROIGMV_OUT_STRUCT
{
    DISALG_DSP_MOTION_VECTOR vector; ///< output result vector
    BOOL  bValidVec;                 ///< if the result vector is valid
} DIS_DSP_ROIGMV_OUT;

typedef struct
{
    UINT32 width;                    ///< Input Img width
    UINT32 height;                   ///< Input Img height
    UINT32 lineoffset;               ///< Input Img lineoffset
    UINT32 addr;                     ///< Input Img addr
}UIMGSIZE;

typedef struct
{
    UINT32 Threshold0;             ///< ETH threshold0 default:2
    UINT32 Threshold1;             ///< ETH threshold1 default:8
    UINT32 Threshold2;             ///< ETH threshold2 default:32
}DisParam;

typedef struct _NVTCV_DISDSP_PRMS
{
    UINT32  ImgInBufAddr;
    UINT32  DisBufAddr;
    UINT32  EdgeMapAddr;
    UINT32  ImgSizeAddr;
    UINT32  TheBufAddr;
}NVTCV_DISDSP_PRMS;

extern UINT32 DisDsp_CalcBuffSize(void);
extern BOOL DisDsp_Init(MEM_RANGE *buf);

extern void DisDsp_ImgSize_SetInputInfo( UIMGSIZE* pInputImgSize);
extern void DisDsp_Roi_SetInputInfo(DIS_DSP_ROIGMV_IN* roiIn);
extern void DisDsp_Threshold_SetInputInfo(DisParam* Threshold);
//#NT#02724/12/09#JS Liu -begin
extern void DisDsp_ZeroStable_SetInputInfo(UINT32 uiThreshold);
//#NT#2016/12/09#JS Liu -end
//#NT#02724/12/26#JS Liu -begin
extern void DisDsp_GetReadyMotionVec(DIS_DSP_MV_INFO *mvInfo);
//#NT#2016/12/26#JS Liu -end
//#NT#02724/01/11#JS Liu -begin
extern void DisDsp_GetReadyMotionVecL2(DIS_DSP_MV_INFO *mvInfo);
//#NT#2017/01/11#JS Liu -end

void DisDsp_GetReadyMotionVecAll(DIS_DSP_MV_INFO *mvInfo);


extern UIMGSIZE DisDsp_ImgSize_GetConfig(void);
extern DIS_DSP_ROIGMV_IN DisDsp_Roi_GetConfig(void);
extern DisParam DisDsp_Threshold_GetConfig(void);

extern DIS_DSP_ROIGMV_OUT DisDsp_Roi_GetResult(void);

extern BOOL DisDsp_START(VOID);
extern BOOL DisDsp_END(VOID);

//for NvtCV;
extern NVTCV_STA NvtCv_disdsp_Preprocess(NVTCV_FUN_INFO *pInfo);
extern VOID NvtCv_disdsp_process(VOID);
extern NVTCV_STA NvtCv_disdsp_Postprocess(NVTCV_FUN_INFO *pInfo);

#endif

