////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "AppLib.h"
#include "UIMode.h"
#include "UIModeMain.h"
#include "UIModeWifi.h"
#include "UISetup.h"
//UIControl
#include "UIFrameworkExt.h"

//UIWnd
//#include "UIMainWnd.h"
//UIInfo
#include "UIInfo.h"

#if (CALIBRATION_FUNC == ENABLE)
#include "EngineerMode.h"
#endif

int PRIMARY_MODE_MAIN = -1;      ///< Main

//#NT#2016/12/02#Niven Cho -begin
//#NT#MAIN mode use whole POOL_ID_APP as temp buffer
extern UINT32 User_GetTempBuffer(UINT32 uiSize);
//#NT#2016/12/02#Niven Cho -end

void ModeMain_Open(void);
void ModeMain_Close(void);

void ModeMain_Open(void)
{
	//#NT#2016/12/02#Niven Cho -begin
    //#NT#MAIN mode use whole POOL_ID_APP as temp buffer
	SxCmd_RegTempMemFunc(User_GetTempBuffer);
	//#NT#2016/12/02#Niven Cho -end
    Ux_SetActiveApp(&UISetupObjCtrl);
    Ux_SendEvent(0, NVTEVT_EXE_OPEN, 0);
    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
    {
        #if(IPCAM_FUNC!=ENABLE)
        UI_OpenWifiWnd();
        #endif
    }
//#NT#2016/05/31#Ben Wang -begin
//#NT#Add UVC multimedia function.
#if(UVC_MULTIMEDIA_FUNC == ENABLE)
    else if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_UVC)
    {
        UI_OpenWifiWnd();
    }
#endif
//#NT#2016/05/31#Ben Wang -end
    #if (CALIBRATION_FUNC == ENABLE)
    else if (EngineerMode_CheckEng())
    {
        EngineerMode_Open();

        // check if enter engineer mode
        if (IsEngineerModeOpened())
        {
            EngineerMode_WndOpen();
        }
    }
    #endif
    //Ux_OpenWindow((VControl *)(&UIMainWndCtrl), 0);
}
void ModeMain_Close(void)
{

    //Ux_CloseWindowClear((VControl *)(&UIMainWndCtrl), 0);
    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
    {
        #if(IPCAM_FUNC!=ENABLE)
        UI_CloseWifiWnd();
        #endif
    }
//#NT#2016/05/31#Ben Wang -begin
//#NT#Add UVC multimedia function.
#if(UVC_MULTIMEDIA_FUNC == ENABLE)
    else if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_UVC)
    {
        UI_CloseWifiWnd();
    }
#endif
//#NT#2016/05/31#Ben Wang -end
    #if (CALIBRATION_FUNC == ENABLE)
    else if (IsEngineerModeOpened())
    {
        EngineerMode_WndClose();
    }
    #endif


    Ux_SendEvent(0, NVTEVT_EXE_CLOSE, 0);

}

SYS_MODE gModeMain =
{
    "MAIN",
    ModeMain_Open,
    ModeMain_Close,
    NULL,
    NULL,
    NULL,
    NULL
};
