#ifndef _CAL_HEADER_INT_H_
#define _CAL_HEADER_INT_H_
#include "IPL_Calibration.h"

extern BOOL CAL_ReadDebugConfig (void)_CALLBACK();
extern ER Set2Preview(IPL_PROC_ID Id, UINT32 iplNum, UINT32 BufAddr,UINT32 BufSize) _CALLBACK();
extern pCAL_ITEM_OBJ CAL_ITEM(IPL_PROC_ID Id) _CALLBACK();
#endif