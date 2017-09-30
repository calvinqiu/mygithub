/*
    Public header file for DRE module.

    This file is the header file that define the API and data type for DRE
    module.

    @file       dre_lib.h
    @ingroup    mIDrvIPPDRE
    @note

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _DRE_LIB_H
#define _DRE_LIB_H

#ifdef __cplusplus
extern "C" {
#endif

#include    "Type.h"//a header for basic variable type

/**
    @addtogroup mIDrvIPPDRE
*/
//@{

//please do not modify the definition here
#define PROC_MAX_NUM    10
#define F5X5_WT         6
#define HDR_IN_NUM      2
#define NR_IN_NUM       1
#define F3X3_WT         3
#define COLOR3          3
#define RTH_TAP         8
#define HDR_WT_TAP      17
#define GAMLUT_TAP      128
#define YCMOD_TAP       16

//------enum------

/**
    Process type of DRE engine
*/
//@{
typedef enum
{
    DRE_NR  = 0,
    DRE_HDR = 1
} PROC_TYPE;
//@}

/**
    Input image type of DRE engine
*/
//@{
typedef enum
{
    DRE_YUV422_COMP = 0,    ///<Complete process of YUV422 pack input
    DRE_YUV422_SIMP = 1,    ///<Simplified process of YUV422 pack input
    DRE_YUV420      = 2     ///<Complete process of YUV420 pack input
} DRE_IMG_TYPE;
//@}

/**
    DRE input image ID
*/
typedef enum
{
    IN_0 = 0,
    IN_1 = 1
} DRE_INIMG_ID;

//------struct-------

/**
    DRE image size parameter

    Structure of input image size
*/
//@{
typedef struct _DRE_IMG_SIZE_STRUCT
{
    UINT32 uiWidth;
    UINT32 uiHeight;
} DRE_IMG_SIZE;
//@}

/**
    Dram info
*/
//@{
typedef struct _DRAM_INFO_STRUCT
{
    UINT32 uiSAddr;
    UINT32 uiLofst;
} DRAM_INFO;
//@}

/**
    DRE input image DMA info

    Structure of input image DMA info
*/
//@{
typedef struct _DRE_IMG_DRAM_PARAM_STRUCT
{
    DRAM_INFO ChY;
    DRAM_INFO ChUVp;
} DRE_IMG_DRAM_PARAM;
//@}

/**
    DRE HDR deghost function control

    Structure of DRE HDR deghost function
*/
//@{
typedef struct _DRE_HDR_DG_CTRL_STRUCT
{
    BOOL bDgEn;                         ///<Enable of HDR deghost function
    BOOL bDgImgRef;                     ///<Reference image of deghost function, only effective when bDgEn = ENABLE
    UINT32 uiDgLyrNum;                  ///<Deghost layer number
    DRAM_INFO DgMskIn[PROC_MAX_NUM];    ///<Deghost mask address, only effective when bDgEn = ENABLE
} DRE_HDR_DG_CTRL;
//@}

/**
    DRE NR joint-bilateral filter info

    Structure of one image NR spatial filter

    5x5:
    w5 w4 w3 w4 w5
    w4 w2 w1 w2 w4
    w3 w1 w0 w1 w3
    w4 w2 w1 w2 w4
    w5 w4 w3 w4 w5
*/
//@{
typedef struct _DRE_NR_FILT_STRUCT
{
    UINT32 uiWt[F5X5_WT];   ///< Spatial weight
    UINT32 *puiRngThrY;     ///< Table of range filter threshold, please assign a UINT32[RTH_TAP] array
    UINT32 *puiRngThrU;     ///< Table of range filter threshold, please assign a UINT32[RTH_TAP] array
    UINT32 *puiRngThrV;     ///< Table of range filter threshold, please assign a UINT32[RTH_TAP] array
} DRE_NR_FILT;
//@}

/**
    DRE NR Gamma LUT info

    Structure of NR Gamma LUT parameters
*/
//@{
typedef struct _DRE_NR_GAM_LUT_STRUCT
{
    UINT32 *puiLutY;    ///< NR gamma table for Y, please assign a UINT32[GAMLUT_TAP] array
    UINT32 *puiLutU;    ///< NR gamma table for U, please assign a UINT32[GAMLUT_TAP] array
    UINT32 *puiLutV;    ///< NR gamma table for V, please assign a UINT32[GAMLUT_TAP] array
} DRE_NR_GAM_LUT;
//@}

/**
    DRE YC modify LUT info

    Structure of YC modify table
*/
//@{
typedef struct _DRE_YCMOD_TBL_STRUCT
{
    UINT32 *puiYcLutY;      ///< Table of YC modify LUT for Y, please assign a UINT32[YCMOD_TAP] array
    UINT32 *puiYcLutC;      ///< Table of YC modify LUT for C, please assign a UINT32[YCMOD_TAP] array
} DRE_YCMOD_TBL;
//@}

/**
    DRE YC modify control info

    Structure of YC modify control parameters
*/
//@{
typedef struct _DRE_YCMOD_CTRL_STRUCT
{
    UINT32 uiCbOfs;
    UINT32 uiCrOfs;
    UINT32 uiStepY;
    UINT32 uiStepC;
    BOOL bModSel;
    BOOL bModLutSel;
} DRE_YCMOD_CTRL;
//@}

/**
    DRE engine parameter for HDR library

    Structure of all avaliable HDR parameter
*/
//@{
typedef struct _DRE_HDR_PARAM_STRUCT
{
    DRE_IMG_TYPE ImgType;
    DRE_IMG_SIZE ImgInSz;
    UINT32 uiWorkBufSAddr;
    DRE_IMG_DRAM_PARAM In[HDR_IN_NUM];
    DRE_IMG_DRAM_PARAM Out;
    DRE_HDR_DG_CTRL DgCtrl;
    BOOL bHardWtEn;
} DRE_HDR_PARAM;
//@}

/**
    DRE engine parameter for NR library

    Structure of all avaliable NR parameter
*/
//@{
typedef struct _DRE_NR_PARAM_STRUCT
{
    DRE_IMG_TYPE ImgType;
    DRE_IMG_SIZE ImgInSz;
    UINT32 uiWorkBufSAddr;
    UINT32 uiProcRepNum;//min=1, max=9
    DRE_IMG_DRAM_PARAM In;
    DRE_IMG_DRAM_PARAM Out;
    BOOL bNrGamYEn[PROC_MAX_NUM];
    BOOL bNrGamUEn[PROC_MAX_NUM];
    BOOL bNrGamVEn[PROC_MAX_NUM];
    DRE_NR_FILT NrJBFilt[PROC_MAX_NUM];
    DRE_NR_GAM_LUT NrGam[PROC_MAX_NUM];
    DRE_YCMOD_TBL YCMTbl[PROC_MAX_NUM];
    DRE_YCMOD_CTRL YCMCtrl[PROC_MAX_NUM];
} DRE_NR_PARAM;
//@}

/**
    function declaration
*/
extern UINT32 dre_getHdrWorkBufSize(DRE_IMG_SIZE *pImgInSz, DRE_IMG_TYPE ImgInType);
extern ER dre_HdrProcess(DRE_HDR_PARAM *pDreHdrParam);

extern UINT32 dre_getNrWorkBufSize(DRE_IMG_SIZE *pImgInSz, UINT32 uiProcRepNum, DRE_IMG_TYPE ImgInType);
extern ER dre_NrProcess(DRE_NR_PARAM *pDreNrParam);

extern VOID dre_setDbgDumpReg(BOOL bDbgDumpEn, UINT32 uiRegSAddr);//for simulation tool
extern UINT32 dre_getClockRate(void);


#ifdef __cplusplus
}
#endif

#endif

//@}
