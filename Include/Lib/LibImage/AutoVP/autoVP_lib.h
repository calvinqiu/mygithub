/**
    AutoVP detection library.

    @file       autoVP_lib.h
    @ingroup    mILibAutoVP_Nvt

    Copyright   Novatek Microelectronics Corp. 2015.  All rights reserved.
*/
#ifndef _AUTO_VP_LIB_H
#define _AUTO_VP_LIB_H

/**
    @addtogroup mILibAutoVP_Nvt
*/

/**
     @name AutoVPreturn status
*/
//@{
#define AUTO_VPALG_STA_OK                  (0)                   ///<  the status is ok, for normal case
#define AUTO_VPALG_STA_ERROR               (-1)                  ///<  some error occurred
//@}

#ifndef bool
#define bool    int
#endif
#ifndef true
#define true    1
#endif
#ifndef false
#define false   0
#endif

typedef struct _AVP_image
{
    UINT8* adress;
    UINT32 width;
    UINT32 height;
    UINT32 lineOffset;
}AVP_image;

typedef struct _AVP_point
{
    int x;
    int y;
}AVP_point;

typedef struct _AVP_line
{
    AVP_point sP;
    AVP_point eP;
}AVP_line;

typedef struct _AVP_rect
{
    int x;
    int y;
    int width;
    int height;
}AVP_rect;


/**
    AutoVP failure info
*/
//@{
typedef enum
{
    AVP_FAILURE_TRUE        = 1,        ///<  Enter failure mode
    AVP_FAILURE_FALSE       = 0         ///<  Not in failure mode
} AVP_FAILURE_STATUS;
//@}


/**
     AutoVP detection input structure.
*/
//@{
typedef struct _AVP_SRCIMG_INFO
{
    UINT32 uiVPSrcBufAddr;      ///<  Auto VP input buffer address
    UINT32 uiVPSrcWidth;        ///<  width of Auto VP input image
    UINT32 uiVPSrcHeight;       ///<  height of Auto VP input image
    UINT32 uiVPSrcLineofs;      ///<  lineofset of Auto VP input image
    UINT32 uiFrameWidth;        ///<  width of IME image
    UINT32 uiFrameHeight;       ///<  height of IME image
    UINT32 uiRoiWidth;          ///<  width of ROI region of IME image
    UINT32 uiRoiHeight;         ///<  height of ROI region of IME image
    UINT32 uiRoiSx;             ///<  start x coordinate of ROI region of IME image
    UINT32 uiRoiSy;             ///<  start y coordinate of ROI region of IME image
    UINT32 uiScaleUp;           ///<  IME image scale up
    UINT32 uiScaleDown;         ///<  IME image scale down
    UINT32 uiScaleRsl;          ///<  IME image scale resolution
} AVP_SRCIMG_INFO;
//@}

//@{
typedef struct _AVP_SRCDATA_INFO
{
    AVP_SRCIMG_INFO AutoVPSrcImgInfo;
    UINT32 uiWorkBufAddr;
    UINT32 uiCacheBufAddr;
} AVP_SRCDATA_INFO;
//@}

/**
                 uiVPSrcWidth
 -------------------------------------------------
 - - (uiVPRoiSx, uiVPRoiSy)                      -
 -   -                                           -
 -     ------------uiVPRoiWidth-----------       -
 -     -                                 -       -
 -     -   (iLx1,iLy1)*    * (iRx1,iRy1) -       -
 -     -                          uiVPRoiHeight  - uiVPSrcHeight
 -     -                                 -       -
 -     -(iLx2,iLy2)*           * (iRx2,iRy2)-    -
 -     ----------------------------------        -
 -                                               -
 -                                               -
 -------------------------------------------------
*/
//@{
typedef struct _AVP_RESULT_INFO
{
    AVP_line                HoughLines[8];
    UINT32                  HoughCount;
    UINT32                  HoughFlag[8];       /// flag: 1~4
    AVP_line                ChosedLine[4];
    AVP_FAILURE_STATUS      Failure;            ///<  Failure mode
    AVP_point               uiVPNew;
    UINT32                  ISETime;
    UINT32                  GraphicTime;
} AVP_RESULT_INFO;
//@}


typedef struct _AVP_DEBUG_INFO
{
    BOOL isECosHoughEnable;
    BOOL isCPU2Open;
} AVP_DEBUG_INFO;

/**
    Set initial parameters into AutoVP Lib.
*/
extern void AVP_setInitParms(void);

/**
    Set unInitial parameters into AutoVP Lib.
*/
extern void AVP_setUnInitParms(void);

/**
    AutoVP Detection
*/
extern UINT32 AVP_Detection(AVP_SRCIMG_INFO Src_Img_info, UINT32 InputUsableMemAddr, UINT32 InputUsableCacheMemAddr, AVP_RESULT_INFO *Auto_VP_Info, AVP_DEBUG_INFO *Auto_VP_Dbg_Info);

/**
    Set AutoVP initial flag
*/
extern void AVP_setInitFlag(UINT32 Flag);

/**
    Check AutoVP initial flag
*/
extern UINT32 AVP_checkInitFlag(void);

/**
    Get AutoVP results info
*/
extern AVP_RESULT_INFO ADAS_GetAVPRstInfo(void);

/**
    Get AutoVP source info
*/
extern AVP_SRCIMG_INFO ADAS_GetAVPSrcInfo(void);

/**
    Get AutoVP debug info
*/
extern AVP_DEBUG_INFO ADAS_GetAVPDbgInfo(void);

#endif

