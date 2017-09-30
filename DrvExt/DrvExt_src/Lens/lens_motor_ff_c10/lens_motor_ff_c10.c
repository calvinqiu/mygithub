#include "LensMotor_Common.h"
#include "lens_motor_ff_c10.h"
#include "Utility.h"
#include "gpio.h"
#include "sif.h"
#include "top.h"
#include "ErrorNo.h"
#include "DxLens.h"
#include "Debug.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          LENSmotor_ff_c10
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#define MD_DELAYMS(x)   Delay_DelayMs(x)
#define MD_DELAYUS(x)   Delay_DelayUs(x)
#define LENS_BINARY_PHASE(b3, b2, b1, b0)       ((b0)|((b1)<<1)|((b2)<<2)|((b3)<<3))

#define SET_GPIO            1
#define CLEAR_GPIO          0


static void     motor_ff_c10_init(PLENS_DEVICE_OBJ pLensDev);
static void     motor_ff_c10_setSignal(MD_SIGNAL signal, UINT32 on);
static BOOL     motor_ff_c10_zoom_checkPI(void);
static void     motor_ff_c10_zoom_setState(MOTOR_ZOOM_ACT state, UINT32 param1);
static void     motor_ff_c10_zoom_setSpeed(UINT32 uiSpeed);
static UINT32   motor_ff_c10_zoom_getSpeed(void);
static BOOL     motor_ff_c10_focus_setState(MOTOR_FOCUS_ACT state, UINT32 param1);
static void     motor_ff_c10_aperture_setState(MOTOR_APERTURE_ACT state, UINT32 param1);
static void     motor_ff_c10_shutter_setState(MOTOR_SHUTTER_ACT state, UINT32 param1);
static void     motor_ff_c10_ircut_setState(MOTOR_IRCUT_ACT state, UINT32 param1);
static void     motor_ff_c10_signal_setGPIO(BOOL set, UINT32 gpio);

static MOTOR_TAB gmotor_ff_c10Obj =
{
    motor_ff_c10_init,
    motor_ff_c10_setSignal,
    motor_ff_c10_zoom_setSpeed,
    motor_ff_c10_zoom_getSpeed,
    motor_ff_c10_zoom_setState,
    motor_ff_c10_zoom_checkPI,
    motor_ff_c10_focus_setState,
    motor_ff_c10_aperture_setState,
    motor_ff_c10_shutter_setState,
    motor_ff_c10_ircut_setState
};

static MOTOR_PVT_OBJ gFF_c10_PvtObj =
{
    //GPIO related var
    {0}, //uiPinIN[MOTOR_MAX_INPUT_PIN];
};

static void motor_ff_c10_initGPIO(PLENS_DEVICE_OBJ pLensDev)
{
    UINT32 i, ui8InPinIdx = LENS_IO_IN_0;

    for(i = 0; i < MOTOR_MAX_INPUT_PIN; i++)
    {
        gFF_c10_PvtObj.uiPinIN[i]   =  pLensDev->pGPIOArray[ui8InPinIdx + i];
    }
}

/**
    motor init

    Init FF_c10 pin assignment

    @param None.

    @return None.
*/
static void motor_ff_c10_init(PLENS_DEVICE_OBJ pLensDev)
{
    motor_ff_c10_initGPIO(pLensDev);

    return;
}

/**
    Get FF_c10 motor object

    @return FF_c10 motor object pointer

*/
PMOTOR_TAB motor_getObject(void)
{
    DBG_MSG("%s\r\n", __func__);

    return &gmotor_ff_c10Obj;
}

static void motor_ff_c10_setSignal(MD_SIGNAL signal, UINT32 on)
{

}

static BOOL motor_ff_c10_zoom_checkPI(void)
{
    return FALSE;
}

static void motor_ff_c10_zoom_setState(MOTOR_ZOOM_ACT state, UINT32 param1)
{

}
static void motor_ff_c10_zoom_setSpeed(UINT32 uiSpeed)
{

}
static UINT32 motor_ff_c10_zoom_getSpeed(void)
{
    return 0;
}

static BOOL motor_ff_c10_focus_setState(MOTOR_FOCUS_ACT state, UINT32 param1)
{
    return TRUE;
}

static void motor_ff_c10_aperture_setNormalState(UINT32 phase)
{

}

static void motor_ff_c10_aperture_dummyLoadState(UINT32 phase)
{

}

static void motor_ff_c10_aperture_setState(MOTOR_APERTURE_ACT state, UINT32 param1)
{
    DBG_MSG("%s, state=%d, param1=%d\n\r", __func__, state, param1);

    switch(state)
    {
        case MOTOR_APERTURE_NORMAL:
            motor_ff_c10_aperture_setNormalState(param1);
            break;
        case MOTOR_APERTURE_DUMMYLOAD:
            motor_ff_c10_aperture_dummyLoadState(param1);
            break;
        default:
            DBG_ERR("%s,UNKNOW\n\r", __func__);
            break;
    }
}

static void motor_ff_c10_ircut_setNormalState(UINT32 phase)
{
    if(phase>=IRCUT_POS_MAX)
    {
        DBG_ERR("%s, Shutter phase error (%d)\n\r", __func__,phase);
        return;
    }
    DBG_MSG("motor_ff_c10_shutter_setNormalState %d\r\n",phase);

    if (phase == IRCUT_POS_OPEN)  //IRCUT open
    {
        motor_ff_c10_signal_setGPIO(SET_GPIO,gFF_c10_PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_IRCUT_P]);
        motor_ff_c10_signal_setGPIO(CLEAR_GPIO,gFF_c10_PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_IRCUT_N]);
    }
    else   //IRCUT close
    {
        motor_ff_c10_signal_setGPIO(SET_GPIO,gFF_c10_PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_IRCUT_N]);
        motor_ff_c10_signal_setGPIO(CLEAR_GPIO,gFF_c10_PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_IRCUT_P]);
    }

    MD_DELAYMS(15);

    motor_ff_c10_signal_setGPIO(CLEAR_GPIO,gFF_c10_PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_IRCUT_P]);
    motor_ff_c10_signal_setGPIO(CLEAR_GPIO,gFF_c10_PvtObj.uiPinIN[MOTOR_CTRL_PIN_IDX_IRCUT_N]);
}

static void motor_ff_c10_shutter_setState(MOTOR_SHUTTER_ACT state, UINT32 param1)
{

}

static void motor_ff_c10_ircut_setState(MOTOR_IRCUT_ACT state, UINT32 param1)
{
    DBG_MSG("%s, state=%d, param1=%d\n\r", __func__, state, param1);
    switch(param1)
    {
        case IRCUT_POS_OPEN:
            motor_ff_c10_ircut_setNormalState(IRCUT_POS_OPEN);
            break;
        case IRCUT_POS_CLOSE:
            motor_ff_c10_ircut_setNormalState(IRCUT_POS_CLOSE);
            break;
        default:
            DBG_ERR("%s,UNKNOW\n\r", __func__);
            break;
    }

}

static void motor_ff_c10_signal_setGPIO(BOOL set, UINT32 gpio)
{
    DBG_MSG("%s , %s %d\r\n", __func__,(set == SET_GPIO)?"SET":"CLEAR",gpio);

    gpio_setDir(gpio, GPIO_DIR_OUTPUT);
    switch(set)
    {
        case SET_GPIO:
            gpio_setPin(gpio);
        break;
        case CLEAR_GPIO:
            gpio_clearPin(gpio);
        break;
    }
}

