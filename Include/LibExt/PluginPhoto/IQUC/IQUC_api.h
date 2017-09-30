/**
    IQUC api.


    @file       IQUC_api.h
    @ingroup    mILibAEAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _IQUC_API_H_
#define _IQUC_API_H_

#include "IQUC_Ctrl.h"

#define IQUC_CLAMP(in,lb,ub) (((INT32)(in) < (INT32)(lb)) ? (INT32)(lb) : (((INT32)(in) > (INT32)(ub)) ? (INT32)(ub) : (INT32)(in)))

extern UINT32 IQUC_InitCtrlItem(UINT32 Id, IQUC_IF **pfpIQUC);
extern void IQUC_GetTuningRatio(UINT32 Id, UINT32* IQUC_TuningIQRatio, UINT32* IQUC_TuningCACValue);


#endif //_IQUC_API_H_
