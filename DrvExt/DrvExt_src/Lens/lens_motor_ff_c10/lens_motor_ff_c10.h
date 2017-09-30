#ifndef _LENSMOTOR_FF_C10_H
#define _LENSMOTOR_FF_C10_H
#include "Type.h"
#include "pad.h"
#include "pwm.h"

#define TOTAL_IRIS_PHASE 2
#define TOTAL_SHUTTER_PHASE 2

#define MOTOR_MAX_INPUT_PIN                      2
#define MOTOR_CTRL_PIN_IDX_IRCUT_P               0 //IN6A
#define MOTOR_CTRL_PIN_IDX_IRCUT_N               1 //IN6B

/**
    Motor object.
    All motor controls will be process via motor object.
*/
typedef struct _MOTOR_PVT_OBJ
{
    //GPIO related var
    UINT32        uiPinIN[MOTOR_MAX_INPUT_PIN];
}MOTOR_PVT_OBJ, *PMOTOR_PVT_OBJ;

#endif
