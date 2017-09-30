/*
    Sensor HDR Ctrl

    This file is the API of the SHDR control.

    @file       SHDRCtrl.h
    @ingroup    mISYSAlg
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#ifndef _SENSOR_HDR_CTRL_H_
#define _SENSOR_HDR_CTRL_H_

#include "SensorHDRLib.h"

extern ER SHDRCtrl_Reset(void);
extern ER SHDRCtrl_Init(SHDR_OPEN_INFO *pHdrOpenInfo);
extern UINT32 SHDRCtrl_GetParam(INT32 frameIdx);
extern UINT32 SHDRCtrl_GetUpdateBits(void);
ER SHDRCtrl_Proc(SHDR_CAL_INFO *pHdrCalInfo, UINT32 frameIdx);

#endif  //_SENSOR_HDR_CTRL_H_

