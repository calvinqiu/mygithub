#ifndef _IPL_CALUI_INT_H_
#define _IPL_CALUI_INT_H_
#include <stdio.h>
#include "IPL_CAL_Debug.h"
#include "IPL_Utility.h"
#include "Type.h"

extern UINT32 LenCenter(IPL_RAW_IMG_INFO* pRaw, UINT32 ScanCenterX, UINT32 ScanCenterY, UINT32 ScanRadius, UINT32 AvgCnt, UINT32 EdgeThr, UINT32 ValidRadius, iCoordinate *out_cent);

#endif
