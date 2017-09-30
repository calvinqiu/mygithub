/**
    IPL Display control

    @file       IPL_Display_int.h
    @ingroup    mISYSAlg
    @note       This moudle is used to control the relationship of IME/display survice/video encoder/DIS

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _IPL_STITCH_H_
#define _IPL_STITCH_H_

#include "IPL_Utility.h"

#if 0
 - Display Ctrl task
#endif
#define FLGPTN_STITCHEST_IDLE         FLGPTN_BIT(0)
#define FLGPTN_STITCHEST_TRIG         FLGPTN_BIT(1)
#define FLGPTN_STITCHEST_READY        FLGPTN_BIT(2)

#define FLGPTN_STITCHIMG_IDLE         FLGPTN_BIT(0)
#define FLGPTN_STITCHIMG_TRIG         FLGPTN_BIT(1)
#define FLGPTN_STITCHIMG_READY        FLGPTN_BIT(2)


extern void IPL_StitchEst_OpenTsk(void);
extern void IPL_StitchEst_CloseTsk(void);
extern void IPL_StitchEst_Pause(void);
extern void IPL_StitchEst_Resume(void);
extern void IPL_StitchEst_Tsk(void);

extern void IPL_StitchImg_OpenTsk(void);
extern void IPL_StitchImg_CloseTsk(void);
extern void IPL_StitchImg_Pause(void);
extern void IPL_StitchImg_Resume(void);
extern void IPL_StitchImg_Tsk(void);

extern UINT32 IPL_TriggerStitch(UINT32 ImgBufIdx, IPL_YCC_IMG_INFO StitchBuf, IPL_YCC_IMG_INFO LeftBuf, IPL_YCC_IMG_INFO RightBuf);
#endif
