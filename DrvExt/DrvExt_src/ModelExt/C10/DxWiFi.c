#include "DxCfg.h"
#include "IOCfg.h"
#include "Debug.h"
#include "DxWiFi.h"
#include "Delay.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxWiFi
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////
void DrvWiFi_PowerOn(void)
{
    DBG_FUNC_BEGIN("[API]\r\n");
    gpio_setDir(C_GPIO_6, GPIO_DIR_OUTPUT);
    gpio_clearPin(C_GPIO_6);
    Delay_DelayMs(10);
    gpio_setDir(P_GPIO_8, GPIO_DIR_INPUT);
    gpio_setDir(P_GPIO_7, GPIO_DIR_OUTPUT);
    gpio_setPin(P_GPIO_7);
    DBG_FUNC_END("[API]\r\n");
}

void DrvWiFi_PowerOff(void)
{
    DBG_FUNC_BEGIN("[API]\r\n");
    gpio_clearPin(P_GPIO_7);
    DBG_FUNC_END("[API]\r\n");
}

