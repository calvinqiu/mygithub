////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "AppLib.h"
#include "UIMode.h"
#include "UIModeMovie.h"
#include "UIAppMovie.h"
#include "UISetup.h"
//UIControl
#include "UIFrameworkExt.h"

//UIWnd
#include "UIFlow.h"
#include "UIModeWifi.h"

//UIInfo
#include "UIInfo.h"

#if (CALIBRATION_FUNC == ENABLE)
#include "EngineerMode.h"
#endif
#if (IPCAM_FUNC == ENABLE)
#include "FlowIPCam.h"
#endif

////////////////////////////////////////////////////////////////////////////////

#include "DeviceCtrl.h"

extern INT32 MovieExe_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);

int PRIMARY_MODE_MOVIE = -1;      ///< Movie

void ModeMovie_Open(void);
void ModeMovie_Close(void);

void ModeMovie_Open(void)
{
    Input_ResetMask();
    Ux_SetActiveApp(&CustomMovieObjCtrl);
    Ux_SendEvent(0, NVTEVT_EXE_OPEN, 0);

    UI_SetData(FL_ModeIndex, UI_GetData(FL_NextMode));
/*
    //disable video1
    UI_Show(UI_SHOW_PREVIEW, TRUE);
    //enable video2
    UI_Show(UI_SHOW_QUICKVIEW, FALSE);
*/
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
    else if (IsEngineerModeOpened())
    {
        EngineerMode_WndOpen();
    }
    #endif
    else
    {
        gMovData.State = MOV_ST_VIEW;
        #if (IPCAM_FUNC == ENABLE)
        Ux_OpenWindow((VControl *)(&FlowIPCamCtrl), 0);
        #else
        Ux_OpenWindow((VControl *)(&UIFlowWndMovieCtrl), 0);
        #endif
        //#NT#2016/03/07#KCHong -begin
        //#NT#Low power timelapse function
        #if (TIMELAPSE_LPR_FUNCTION == ENABLE)
        if (MovieTLLPR_CheckHWRTStatus() == TL_HWRT_BOOT_ALARM)
            MovieTLLPR_Process(TIMELAPSE_FROM_HWRT);
        #endif
        //#NT#2016/03/07#KCHong -end
    }
}
void ModeMovie_Close(void)
{
    UINT32 i;

    //Ux_CloseWindowClear((VControl *)(&FlowMovieCtrl), 0);
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
    else
    {
        #if (IPCAM_FUNC == ENABLE)
        Ux_CloseWindow((VControl *)(&FlowIPCamCtrl), 0);
        #else
        Ux_CloseWindow((VControl *)(&UIFlowWndMovieCtrl), 0);
        #endif
    }

    #if (MOVIE_DIS == ENABLE)
    Ux_SendEvent(&CustomMovieObjCtrl,NVTEVT_EXE_MOVIEDIS_ENABLE,1,0);
    #endif

    {
        for(i=0; i<500; i++)
        {
            if (!BKG_GetTskBusy())
            {
                break;
            }
            SwTimer_DelayMs(100);
        }
        if (i==500)
        {
            debug_err(("^R Wait background task idle timeout \r\n"));
        }
    }
    Ux_SendEvent(0, NVTEVT_EXE_CLOSE, 0);
}

SYS_MODE gModeMovie =
{
    "MOVIE",
    ModeMovie_Open,
    ModeMovie_Close,
    NULL,
    NULL,
    NULL,
    NULL
};
