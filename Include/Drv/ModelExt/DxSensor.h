#ifndef _DXSENSOR_H_
#define _DXSENSOR_H_
/**
    DxSensor.h

    Sensor common interface
    @file       DxSensor.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include "Sensor.h"

extern SENSOR_DRVTAB* DrvSensor_GetTab(SENSOR_ID Id);
extern SENSOR_INIT_OBJ DrvSensor_GetObj(SENSOR_ID Id);
extern void DrvSensor_PowerTurnOn(SENSOR_ID Id);
extern void DrvSensor_PowerTurnOff(SENSOR_ID Id);
extern void DrvSensor_PowerSaveOff(SENSOR_ID Id);
extern void DrvSensor_PowerSaveOn(SENSOR_ID Id);
extern BOOL DrvSensor_DetPlugIn(SENSOR_ID Id);
#endif //_DXSENSOR_H_

