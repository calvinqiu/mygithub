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
    gpio_setPin(GPIO_WIFI_CS);
    //gpio_clearPin(GPIO_WIFI_PWREN); // useless in C11
    DBG_FUNC_END("[API]\r\n");
}

void DrvWiFi_PowerOff(void)
{
    DBG_FUNC_BEGIN("[API]\r\n");
    //gpio_setPin(GPIO_WIFI_PWREN); // useless in C11
    gpio_clearPin(GPIO_WIFI_CS);
    DBG_FUNC_END("[API]\r\n");
}

