#ifndef _IPL_MODE_IMX322LQJ_AHD_INT_H_
#define _IPL_MODE_IMX322LQJ_AHD_INT_H_
/**
    IPL_Mode_IMX322LQJ_AHD_Int.h


    @file       IPL_Mode_IMX322LQJ_AHD_Int.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

IPL_CMD_CHGMODE_FP IPL_GetModeFp(IPL_MODE CurMode, IPL_MODE NextMode);
UINT32 IPL_SetSensorMode(IPL_PROC_ID Id, IPL_MODE Mode, BOOL SetFlg);

#endif //_IPL_MODE_IMX322LQJ_AHD_INT_H_