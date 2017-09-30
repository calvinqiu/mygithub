#ifndef _ROTVIEW_H
#define _ROTVIEW_H

#include "DispSrvApi.h"

extern void RotateView_SetImg(ISIZE* pImgSize, USIZE* pAspectRatio);
extern INT32 RotateView_OnDraw(DISPSRV_DRAW* pDraw);

#endif //_ROTVIEW_H