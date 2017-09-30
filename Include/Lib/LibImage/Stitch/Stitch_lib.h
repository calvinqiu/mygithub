/**
    Dual camera image stitching library.

    @file    Stitch_lib.h

    Usage flow:

    NOTE: Stitch_CalMemoryRequired() must be called (to set input image info) before other APIs

    Example code:  \n

    Copyright   Novatek Microelectronics Corp. 2015.  All rights reserved.
*/
#ifndef _STITCH_LIB_H_
#define _STITCH_LIB_H_

#include "Type.h"
#include "IPL_Utility.h"
#include "Stitch_defs.h"


// Application settings (USER OPTIONS) ------------------------------------------------------------------------
#define STITCH_REF_IMG_WIDTH            1920        // -> 960
#define STITCH_REF_IMG_HEIGHT           1080        // -> 240
#define STITCH_REF_IMG_SEAM_L           (380*2)     // -> 380
#define STITCH_REF_IMG_SEAM_R           (258*2)     // -> 258
#define STITCH_REF_IMG_EST_RADIUS       184         // -> 92
#define STITCH_REF_IMG_BLEND_RADIUS     100         // -> 50

#define STITCH_EST_DOWNSAMPLE           2

#define STITCH_PRMS_AUTO                TRUE
#define STITCH_PRMS_YBAND_H             32      // disable if STITCH_PRMS_AUTO
#define STITCH_PRMS_YBAND_W             72      // disable if STITCH_PRMS_AUTO
#define STITCH_PRMS_IMGDIFF_TH          20      // disable if STITCH_PRMS_AUTO
#define STITCH_PRMS_IMGDIFF_PXLNUM_TH   45      // disable if STITCH_PRMS_AUTO

#define STITCH_RING_BUF_NUM             8
// End applcation settions (USER OPTIONS) ---------------------------------------------------------------------


/**
    Calculate required memory size (including the buffer size used for estimation result)
*/
extern UINT32 Stitch_CalMemoryRequired(UINT32 uiImgWidth, UINT32 uiImgHeight);

/**
    Stitch initialization function.
*/
extern STITCH_STAT Stitch_Initial(MEM_RANGE *MemRange);

/**
    Stitch estimation.

    This function estimates a vector which describes how to stitch two images.

    @param[out] uiAddrEstResult: Estimation result (INT16, dim: 1 x imgRows). 
    @param[in] pImgL: Left sub-image.
    @param[in] pImgR: Right sub-image.
    
    @return estimation status.
*/
extern STITCH_STAT Stitch_Estimate(UINT32 uiAddrEstResult, STITCH_IM *pImgL, STITCH_IM *pImgR);

/**
    Image stitching.

    This function stitches two images according to estimation results.

    @param[out] pDstImg: Destination sub-image.
    @param[in] pImgL: Left sub-image.
    @param[in] pImgR: Right sub-image.
    @param[in] uiAddrEstResult: Stitch estimation result.
*/
extern STITCH_STAT Stitch_Process(STITCH_IM *pDstImg, STITCH_IM *pImgL, STITCH_IM *pImgRm, UINT32 uiAddrEstResult, IPL_YUV_IMG_TYPE imgFmt);

/**
    Stitch de-initialization.
*/
extern STITCH_STAT Stitch_DeInitial(void);

/**
   Get the status of stitch initialization.
   TRUE is returned if initialization is done else FALSE is returned instead.
*/
extern STITCH_STAT Stitch_InitialStatus(void);

/**
   Get the starting address of working buffer.
   Please check g_StitchInitialized before using the returned address.
*/
extern UINT32 Stitch_GetWrkBufAddr(void);   // including IPC prm buf

/**
   Get the starting address of the buffers for storing estimation results.
   Please check g_StitchInitialized before using the returned address.
*/
extern UINT32 Stitch_GetEstRltBufsAddr(void);

/**
   Get the memory size required to store estimation results (in bytes)
*/ 
extern UINT32 Stith_GetOneEstRltBufSize(void);

/**
   Get the starting address of stitch parameter structure.
*/
extern UINT32 Stitch_GetStitchPrmsAddr(void);

/**
   Get the offset of right stitch sub-image. This offset is used to define the exact x-range as well as the left sub-image for stitching.
*/ 
INT32 Stitch_GetRightStitchSubImgXOffs(void);

/**
   Get the cut point of left image in percentage (w.r.t. uiImgWidth, 0~100, precision 4 bits).
*/
extern UINT32 Stitch_GetLeftImgCutPointInPercent(UINT32 uiImgWidth);    // NOTE: uiImgWidth==STITCH_IMG_WIDTH

/**
  Get the cut point of left image (in pixel).
*/ 
extern UINT32 Stitch_GetLeftImgCutPointInPixel(UINT32 uiImgWidth);      // NOTE: uiImgWidth==STITCH_IMG_WIDTH

/**
  Get the cut point of right image in percentage (w.r.t. uiImgWidth, 0~100, precision 4 bits).
*/
extern UINT32 Stitch_GetRightImgCutPointInPercent(UINT32 uiImgWidth);   // NOTE: uiImgWidth==STITCH_IMG_WIDTH

/**
  Get the cut point of right image (in pixel).
*/
extern UINT32 Stitch_GetRightImgCutPointInPixel(UINT32 uiImgWidth); // NOTE: uiImgWidth==STITCH_IMG_WIDTH

/**
    Calculate the width of stitching area in percentage (w.r.t. uiImgWidth, 0~100, precision 4 bits).
*/
extern UINT32 Stitch_GetStitchAreaWidthInPercent(UINT32 uiImgWidth);    // NOTE: uiImgWidth==STITCH_IMG_WIDTH

/**
    Calculate the width of stitching area (in pixel).
*/
extern UINT32 Stitch_GetStitchAreaWidthInPixel(UINT32 uiImgWidth);      // NOTE: uiImgWidth==STITCH_IMG_WIDTH

/**
   Get estimation radius
*/
extern UINT32 Stitch_GetEstimateRadius(void);

/**
   Get blending radius
*/
extern UINT32 Stitch_GetBlendRadius(void);

/**
   Get the width of stitching (output) area
*/
extern UINT32 Stitch_GetSubImgWidth(void);

/**
   Get the height of stitching (output) area
*/
extern UINT32 Stitch_GetSubImgHeight(void);


#endif

