#ifndef _IPL_MODE_IMX078CQK_VR360_FF_INT_H
#define _IPL_MODE_IMX078CQK_VR360_FF_INT_H

/**
    IPL_ModeSample_Int.h


    @file       IPL_ModeSample_Int.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.
*/

IPL_CMD_CHGMODE_FP IPL_GetModeFp(IPL_MODE CurMode, IPL_MODE NextMode);
UINT32 IPL_SetSensorMode(IPL_PROC_ID Id, IPL_MODE Mode, BOOL SetFlg);

#endif //_IPL_MODE_SAMPLE_INT_H_