////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "AppLib.h"
#include "UIMode.h"
#include "UIModeUsbCam.h"
#include "UIAppUsbCam.h"
//UIControl
#include "UIFrameworkExt.h"

#include "DeviceCtrl.h"
#include "UIFlow.h"

int PRIMARY_MODE_USBPCC = -1;     ///< USB PC camera

void ModeUsbCam_Open(void);
void ModeUsbCam_Close(void);

void ModeUsbCam_Open(void)
{
    Input_ResetMask();
    USB_PlugInSetFunc();
    Ux_SetActiveApp(&CustomUSBPCCObjCtrl);
    Ux_SendEvent(0, NVTEVT_EXE_OPEN, 0);
#if( _UI_STYLE_ != _UI_STYLE_IPCAM_)
    Ux_OpenWindow((VControl *)(&UIFlowWndUSBCtrl), 1, UIFlowWndUSB_PCC_MODE);
#endif
}
void ModeUsbCam_Close(void)
{
    Ux_SendEvent(0, NVTEVT_EXE_CLOSE, 0);
}

SYS_MODE gModeUsbCam =
{
    "USBCAM",
    ModeUsbCam_Open,
    ModeUsbCam_Close,
    NULL,
    NULL,
    NULL,
    NULL
};
