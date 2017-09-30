////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "AppLib.h"
#include "UIMode.h"
#include "UIModePhoto.h"
#include "UIAppPhoto.h"
#include "UISetup.h"
//UIControl
#include "UIFrameworkExt.h"

//UIInfo
#include "UIInfo.h"
#include "UIFlow.h"
#include "UIModeWifi.h"
//#include "FlowPanInit.h"
#include "DeviceCtrl.h"

#if (CALIBRATION_FUNC == ENABLE)
#include "EngineerMode.h"
#endif

extern INT32 PhotoExe_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);

int PRIMARY_MODE_PHOTO = -1;      ///< Photo
void ModePhoto_Open(void);
void ModePhoto_Close(void);

void ModePhoto_Open(void)
{
    Input_ResetMask();
    Ux_SetActiveApp(&CustomPhotoObjCtrl);
    Ux_SendEvent(0, NVTEVT_EXE_OPEN, 0);

    UI_SetData(FL_ModeIndex, UI_GetData(FL_NextMode));
/*
    //disable video1
    UI_Show(UI_SHOW_PREVIEW, TRUE);
    //enable video2
    UI_Show(UI_SHOW_QUICKVIEW, FALSE);
*/
#if( _UI_STYLE_ != _UI_STYLE_IPCAM_)
    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
    {
        UI_OpenWifiWnd();
        FlowPhoto_InitCfgSetting(); // Init Photo mode related parameters.
    }
    #if (CALIBRATION_FUNC == ENABLE)
    else if (IsEngineerModeOpened())
    {
        EngineerMode_WndOpen();
    }
    #endif
    else
    {
        Ux_OpenWindow((VControl *)(&UIFlowWndPhotoCtrl), 0);
    }
#endif
}
void ModePhoto_Close(void)
{
#if( _UI_STYLE_ != _UI_STYLE_IPCAM_)

    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
    {
        UI_CloseWifiWnd();
    }
    #if (CALIBRATION_FUNC == ENABLE)
    else if (IsEngineerModeOpened())
    {
        EngineerMode_WndClose();
    }
    #endif
    else
    {
        Ux_CloseWindow((VControl *)(&UIFlowWndPhotoCtrl), 0);
    }
#endif
    Ux_SendEvent(0, NVTEVT_EXE_CLOSE, 0);
}

SYS_MODE gModePhoto =
{
    "PHOTO",
    ModePhoto_Open,
    ModePhoto_Close,
    NULL,
    NULL,
    NULL,
    NULL
};
