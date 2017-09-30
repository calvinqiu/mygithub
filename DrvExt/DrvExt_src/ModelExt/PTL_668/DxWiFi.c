#include "DxCfg.h"
#include "IOCfg.h"
#include "Debug.h"
#include "DxWiFi.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxWiFi
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////
void DrvWiFi_PowerOn(void)
{
    gpio_setDir(P_GPIO_19, GPIO_DIR_OUTPUT);
    gpio_setPin(P_GPIO_19);

}

void DrvWiFi_PowerOff(void)
{
    gpio_clearPin(P_GPIO_19);
}

