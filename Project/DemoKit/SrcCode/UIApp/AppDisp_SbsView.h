#ifndef _SBSVIEW_H
#define _SBSVIEW_H

#include "DispSrvApi.h"

extern void SbsView_SetImg(ISIZE* pImgSize, USIZE* pAspectRatio);
extern INT32 SbsView_OnDraw(DISPSRV_DRAW* pDraw);

#endif
