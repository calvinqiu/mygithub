////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "AppLib.h"
#include "UIMode.h"
#include "UIModeSleep.h"
#include "SysCommon.h"
//UIControl
#include "UIFrameworkExt.h"
#include "UIInfo.h"

#include "UISetup.h"

#include "Dx.h"
#include "DxApi.h"
#include "DxStorage.h"
#include "IrRx.h"
#include "FwSrvApi.h"
#include "rtc.h"
#include "clock.h"
#include "uart.h"
#include "vx1.h"
#include "GxLinuxApi.h"

#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UIModeSleep
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

int PRIMARY_MODE_SLEEP = -1;      ///< Sleep


static DX_HANDLE hDxVdo1,hDxVdo2;


static void ModeSleep_Open(void)
{
    Ux_SetActiveApp(&UISetupObjCtrl);
    Ux_SendEvent(0, NVTEVT_EXE_OPEN, 0);

#if (_CPU2_TYPE_ == _CPU2_ECOS_ && ECOS_POWER_OFF_FLOW == DISABLE)
    DBG_ERR("ECOS_POWER_OFF_FLOW must set ENABLE\r\n");
#endif

    SxTimer_SetFuncActive(SX_TIMER_DET_TIMER_ID,FALSE);
    SxTimer_SetFuncActive(SX_TIMER_DET_TV_ID,FALSE);
    SxTimer_SetFuncActive(SX_TIMER_DET_STRG_ID,FALSE);
    SxTimer_SetFuncActive(SX_TIMER_DET_USB_ID,FALSE);
    SxTimer_SetFuncActive(SX_TIMER_DET_SLEEP_ID,FALSE);
    SxTimer_SetFuncActive(SX_TIMER_DET_AUTOPOWEROFF_ID,FALSE);
    SxTimer_SetFuncActive(SX_TIMER_DET_KEY_ID,FALSE);
    SxTimer_SetFuncActive(SX_TIMER_DET_PWR_ID,FALSE);

    //STORAGE
    FwSrv_Close();
    System_OnStrgExit_PS();
    GxStrg_CloseDevice(0);

    //LCD
    GxVideo_SetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_BACKLIGHT, FALSE);
    hDxVdo1 = (DX_HANDLE)GxVideo_GetDevice(DOUT1);
    if(hDxVdo1)
    {
        GxVideo_CloseDevice(DOUT1);
    }

    hDxVdo2 = (DX_HANDLE)GxVideo_GetDevice(DOUT2);
    if(hDxVdo2)
    {
        GxVideo_CloseDevice(DOUT2);
    }

	#if (_CPU2_TYPE_ == _CPU2_LINUX_)
    GxLinux_Suspend();
	#endif

#if 1 //set it as 0 for debug
	//#NT#2017/04/12#Ben Wang -begin
    //#NT#Addd more wake-up source for suspend/resume test.
    rtc_setConfig(RTC_CONFIG_ID_WAKEUP_ENABLE, RTC_WAKEUP_SRC_ALARM|RTC_WAKEUP_SRC_PWR_SW|RTC_WAKEUP_SRC_PWR_SW2);
	//#NT#2017/04/12#Ben Wang -end

    //enter to sleep
    clk_powerdown(CLK_PDN_MODE_SLEEP3);
    //leave sleeping

    UINT32 iCurrMode = System_GetState(SYS_STATE_CURRMODE);
    UINT32 iNextMode = System_GetState(SYS_STATE_PREVMODE);
    UI_SetData(FL_PreMode,iCurrMode);
    UI_SetData(FL_NextMode,iNextMode);
    Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, iNextMode);
#endif
}

static void ModeSleep_Close(void)
{
    Ux_SendEvent(0, NVTEVT_EXE_CLOSE, 0);

	#if (_CPU2_TYPE_ == _CPU2_LINUX_)
    GxLinux_Resume();
	#endif

    //LCD
    if(hDxVdo1)
    {
        GxVideo_OpenDevice(DOUT1,(UINT32)hDxVdo1,DISP_LASTMODE);
    }

    if(hDxVdo2)
    {
        GxVideo_OpenDevice(DOUT2,(UINT32)hDxVdo2,DISP_LASTMODE);
    }
    GxVideo_SetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_BACKLIGHT, TRUE);

    //STORAGE
    if(System_OnStrgInit_EMBMEM_GetGxStrgType() == GXSTRG_FS_TYPE_UITRON)
    {
        DX_HANDLE pStrgDev = Dx_GetObject(DX_CLASS_STORAGE_EXT|DX_TYPE_CARD1);
        if (GxStrg_OpenDevice(0, pStrgDev) != TRUE)
        {
            char* pDxName="unknown";
            Dx_GetInfo(pStrgDev, DX_INFO_NAME,&pDxName);
            DBG_ERR("Storage mount %s fail\r\n",pDxName);
        }
    }
    System_OnStrgInit_PS();
    FwSrv_Open();

    SxTimer_SetFuncActive(SX_TIMER_DET_TIMER_ID,TRUE);
    SxTimer_SetFuncActive(SX_TIMER_DET_TV_ID,TRUE);
    SxTimer_SetFuncActive(SX_TIMER_DET_STRG_ID,TRUE);
    SxTimer_SetFuncActive(SX_TIMER_DET_USB_ID,TRUE);
    SxTimer_SetFuncActive(SX_TIMER_DET_SLEEP_ID,TRUE);
    SxTimer_SetFuncActive(SX_TIMER_DET_AUTOPOWEROFF_ID,TRUE);
    SxTimer_SetFuncActive(SX_TIMER_DET_KEY_ID,TRUE);
    SxTimer_SetFuncActive(SX_TIMER_DET_PWR_ID,TRUE);
}

SYS_MODE gModeSleep =
{
    "SLEEP",
    ModeSleep_Open,
    ModeSleep_Close,
    NULL,
    NULL,
    NULL,
    NULL
};
