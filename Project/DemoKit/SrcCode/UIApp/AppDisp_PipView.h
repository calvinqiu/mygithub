#ifndef _PIPVIEW_H
#define _PIPVIEW_H

#include "DispSrvApi.h"

extern void PipView_SetImg(ISIZE* pImgSize, USIZE* pAspectRatio);
extern void PipView_SetStyle(UINT32 uiStyle); //DUALCAM_FRONT,DUALCAM_BOTH,DUALCAM_BEHIND
extern INT32 PipView_OnDraw(DISPSRV_DRAW* pDraw); //PIP = Picture In Picture
extern INT32 PipView_OnDraw_4in1(DISPSRV_DRAW* pDraw);
extern void PipView_InstallCmd(void);

#endif