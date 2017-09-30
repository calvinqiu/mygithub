////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "AppLib.h"

#include "FileSysTsk.h"

//Mode
#include "UIModeMain.h"
#if (PHOTO_MODE == ENABLE)
#include "UIModePhoto.h"
#endif
#include "UIModeMovie.h"
#if (PLAY_MODE == ENABLE)
#include "UIModePlayback.h"
#endif
#if (USB_MODE == ENABLE)
#include "UIModeUsbDisk.h"
#include "UIModeUsbPTP.h"
#include "UIModeUsbCam.h"
#include "UIModeUsbMenu.h"
#endif
#include "UIModeSleep.h"
#if (UCTRL_FUNC)
#include "UCtrlMain.h"
#endif
//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysMain
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

extern void System_BootStart(void);
extern void System_BootEnd(void);
extern void System_ShutDownStart(void);
extern void System_ShutDownEnd(void);

BOOL g_bIsInitSystemFinish = TRUE;

int SX_TIMER_BURNIN_AUTORUN_ID = -1;

//////////////////////////////////////
//#NT#2016/05/31#Jeah Yen -begin
//#NT#add event trace
UINT32 UI_EventTraceLvl = 0;
//#NT#2016/05/31#Jeah Yen -end

int UserMainProc_Init(void);
int UserMainProc_Exit(void);

void UserWaitEvent(NVTEVT wait_evt, UINT32 *wait_paramNum, UINT32 *wait_paramArray)
{
    NVTEVT evt = 0; //fix for CID 45083
    UINT32 paramNum = 0; //fix for CID 45084
    UINT32 paramArray[MAX_MESSAGE_PARAM_NUM] = {0}; //fix for CID 45085
    BOOL bQuit = FALSE;

    while(!bQuit)
    {
        //#NT#2016/05/12#Jeah Yen -begin
        //#NT#add profile macro
        PROFILE_TASK_IDLE();
        //#NT#2016/05/12#Jeah Yen -end
        Ux_WaitEvent(&evt, &paramNum, paramArray);
        if(evt)
        {
            INT32 result = NVTEVT_PASS;
            #if (UCTRL_FUNC)
            BOOL  isUctrlEvent = FALSE;
            #endif
            //#NT#2016/05/12#Jeah Yen -begin
            //#NT#add profile macro
            PROFILE_TASK_BUSY();
            //#NT#2016/05/12#Jeah Yen -end
            //#NT#2016/05/31#Jeah Yen -begin
            //#NT#add event trace
            //fix for CID 60977 - begin
            if(UI_EventTraceLvl >= 5)
            {
                DBG_DUMP("MSG: get event 0x%08x!\r\n", evt);
            }
            //fix for CID 60977 - end
            //#NT#2016/05/31#Jeah Yen -3nd

            #if (UCTRL_FUNC)
            if (evt & UCTRL_EVENT_MASK)
            {
                isUctrlEvent = TRUE;
                evt &= ~UCTRL_EVENT_MASK;
            }
            #endif

            if(evt == wait_evt)
            {
                if(wait_paramNum)
                    (*wait_paramNum) = paramNum;
                if(wait_paramArray)
                    memcpy(wait_paramArray, paramArray, sizeof(UINT32)*MAX_MESSAGE_PARAM_NUM);
                result = NVTEVT_CONSUME;
                bQuit = TRUE;
            }
            if(IS_APP_EVENT(evt))
            {
                result = Ux_AppDispatchMessage(evt, paramNum, paramArray);
            }
            /*
            {
            VControl* pObj;
            if(Ux_GetRootWindow(&pObj) != NVTRET_OK)
                continue;
            }
            */
            if(result != NVTEVT_CONSUME)
            {
                if(System_GetState(SYS_STATE_CURRMODE) != SYS_MODE_UNKNOWN)
                    result = Ux_WndDispatchMessage(evt, paramNum, paramArray);
            }
            //#NT#2016/05/31#Jeah Yen -begin
            //#NT#add event trace
            //fix for CID 60977 - begin
            if((UI_EventTraceLvl > 0) && (result != NVTEVT_CONSUME))
            {
                DBG_DUMP("^YWRN: MSG: skip event 0x%08x!\r\n", evt);
            }
            //fix for CID 60977 - end
            //#NT#2016/05/31#Jeah Yen -end

            if(System_GetState(SYS_STATE_CURRMODE) != SYS_MODE_UNKNOWN)
                Ux_Redraw();

            #if (UCTRL_FUNC)
            // run uctrl callback
            if (isUctrlEvent)
            {
                UctrlMain_EventEnd(0, evt);
            }
            #endif
        }
    }
}

extern void System_InstallAppObj(void);

BOOL bUI_Quit = FALSE;
void UI_Quit(void){bUI_Quit = TRUE;}

/////////////////////////////////////////////////////////////////////////////

void System_InstallModeObj(void)
{
    //register project mode
    PRIMARY_MODE_MAIN = System_AddMode(&gModeMain); //an empty mode for calibration
#if (PLAY_MODE == ENABLE)
    PRIMARY_MODE_PLAYBACK = System_AddMode(&gModePlay);
#endif
#if (PHOTO_MODE == ENABLE)
    PRIMARY_MODE_PHOTO = System_AddMode(&gModePhoto);
#endif
    PRIMARY_MODE_MOVIE = System_AddMode(&gModeMovie);
#if (USB_MODE == ENABLE)
    PRIMARY_MODE_USBMSDC = System_AddMode(&gModeUsbDisk);
    PRIMARY_MODE_USBMENU = System_AddMode(&gModeUsbMenu);
    PRIMARY_MODE_USBPCC = System_AddMode(&gModeUsbCam);
#endif
    PRIMARY_MODE_SLEEP = System_AddMode(&gModeSleep);
}

/////////////////////////////////////////////////////////////////////////////
INT32 System_UserKeyFilter(NVTEVT evt, UINT32 paramNum, UINT32 *paramArray);
INT32 System_UserTouchFilter(NVTEVT evt, UINT32 paramNum, UINT32 *paramArray);

#if (USB_MODE == ENABLE)
BOOL gIsUSBChargePreCheck = FALSE;
#endif

void UserMainProc(void)
{
    NVTEVT evt = 0; //fix for CID 45081
    UINT32 paramNum = 0; //fix for CID 45082
    UINT32 paramArray[MAX_MESSAGE_PARAM_NUM] = {0};
    //debug_msg("event loop - begin!\r\n");
    TM_BOOT_BEGIN("flow","preboot");
    //////////////////////////////////////////////////////////////
    UserMainProc_Init();
    //////////////////////////////////////////////////////////////
    #if(UCTRL_FUNC == ENABLE)
    System_OnUctrl();
    #endif
    System_InstallAppObj(); //install VControl type list of App Object and UIControl Object
    System_InstallModeObj(); //install SYS_MODE objects
    TM_BOOT_END("flow","preboot");
    Ux_SendEvent(0, NVTEVT_SYSTEM_BOOT, 1, 1);

    while(!bUI_Quit)
    {
        //#NT#2016/05/12#Jeah Yen -begin
        //#NT#add profile macro
        PROFILE_TASK_IDLE();
        //#NT#2016/05/12#Jeah Yen -end
        Ux_WaitEvent(&evt, &paramNum, paramArray);
        if(evt)
        {
            INT32 result = NVTEVT_PASS;
            #if (UCTRL_FUNC)
            BOOL  isUctrlEvent = FALSE;
            #endif
            //#NT#2016/05/12#Jeah Yen -begin
            //#NT#add profile macro
            PROFILE_TASK_BUSY();
            //#NT#2016/05/12#Jeah Yen -end
            //#NT#2016/05/31#Jeah Yen -begin
            //#NT#add event trace
            //fix for CID 60977 - begin
            if(UI_EventTraceLvl >= 5)
            {
                DBG_DUMP("MSG: get event 0x%08x!\r\n", evt);
            }
            //fix for CID 60977 - end
            //#NT#2016/05/31#Jeah Yen -end

            #if (UCTRL_FUNC)
            if (evt & UCTRL_EVENT_MASK)
            {
                isUctrlEvent = TRUE;
                evt &= ~UCTRL_EVENT_MASK;
            }
            #endif
            if(IS_KEY_EVENT(evt))
                result = System_UserKeyFilter(evt, paramNum, paramArray);

            #if (_TOUCH_FUNC_ == _TOUCH_ENABLE_)
            if(result != NVTEVT_CONSUME)
            {
                if(IS_TOUCH_EVENT(evt))
                    result = System_UserTouchFilter(evt, paramNum, paramArray);
            }
            #endif

            if(result != NVTEVT_CONSUME)
            {
                if(IS_APP_EVENT(evt))
                    result = Ux_AppDispatchMessage(evt, paramNum, paramArray);
            }
            if(result != NVTEVT_CONSUME)
            {
                if(System_GetState(SYS_STATE_CURRMODE) != SYS_MODE_UNKNOWN)
                    result = Ux_WndDispatchMessage(evt, paramNum, paramArray);
            }
            //#NT#2016/05/31#Jeah Yen -begin
            //#NT#add event trace
            //fix for CID 60978 - begin
            if((UI_EventTraceLvl > 0) && (result != NVTEVT_CONSUME))
            {
                DBG_DUMP("^YWRN: MSG: skip event 0x%08x!\r\n", evt);
            }
            //fix for CID 60978 - end
            //#NT#2016/05/31#Jeah Yen -end

            if(System_GetState(SYS_STATE_CURRMODE) != SYS_MODE_UNKNOWN)
                Ux_Redraw();
#if 0 //background task wait done flag,need to remove
            // A special case.
            // UI background is busy until finishing the event handler of BACKGROUND_DONE
            if (evt == NVTEVT_BACKGROUND_DONE)
            {
                BKG_Done();
            }
#endif
            #if (UCTRL_FUNC)
            // run uctrl callback
            if (isUctrlEvent)
            {
                UctrlMain_EventEnd(0, evt);
            }
            #endif
            //return result;
        }
    }

    //////////////////////////////////////////////////////////////
    UserMainProc_Exit();
    //////////////////////////////////////////////////////////////
    GxSystem_PowerOff();
    //debug_msg("event loop - end!\r\n");
}



//////////////////////////////////////////////////////////////

//#NT#2016/03/30#Jeah Yen -begin
//#NT#isolate fast boot code
#if (POWERON_FAST_BOOT == ENABLE)

// Services
extern void UserMainProc2(void);
extern void UserMainProc3(void);
extern void UserMainProc4(void);
extern void UserMainProc5(void);
extern void UserMainProc6(void);
extern void UserMainProc7(void);
extern void UserMainProc8(void);
UINT32 USERINIT_FLG_ID = 0;
UINT32 USEREXIT_FLG_ID = 0;
UINT32 USERPROC2_ID = 0;
UINT32 USERPROC3_ID = 0;
UINT32 USERPROC4_ID = 0;
UINT32 USERPROC5_ID = 0;
UINT32 USERPROC6_ID = 0;
UINT32 USERPROC7_ID = 0;
UINT32 USERPROC8_ID = 0;
//#define PRI_NVTUSER                 10
//#define PRI_FILESYS                 10
//#define PRI_FSBACKGROUND            9
#define PRI_USERPROC2               12 //ensure its priority is lower then PRI_NVTUSER
#define PRI_USERPROC3               12 //ensure its priority is lower then PRI_NVTUSER
#define PRI_USERPROC4               12 //ensure its priority is lower then PRI_NVTUSER
#define PRI_USERPROC5               12 //ensure its priority is lower then PRI_NVTUSER
#define PRI_USERPROC6               12 //ensure its priority is lower then PRI_NVTUSER
#define PRI_USERPROC7               12 //ensure its priority is lower then PRI_NVTUSER
#define PRI_USERPROC8               12 //ensure its priority is higher then PRI_NVTUSER (for network IPC)
#define STKSIZE_USERPROC2           2048
#define STKSIZE_USERPROC3           2048
#define STKSIZE_USERPROC4           2048
#define STKSIZE_USERPROC5           2048
#define STKSIZE_USERPROC6           2048
#define STKSIZE_USERPROC7           2048
#define STKSIZE_USERPROC8           2048
#endif
//#NT#2016/03/30#Jeah Yen -end

void UserMainProc_InstallID(void)
{
//#NT#2016/03/30#Jeah Yen -begin
//#NT#isolate fast boot code
#if (POWERON_FAST_BOOT == ENABLE)
    OS_CONFIG_FLAG(USERINIT_FLG_ID);
    OS_CONFIG_FLAG(USEREXIT_FLG_ID);
    OS_CONFIG_TASK(USERPROC2_ID,     PRI_USERPROC2,       STKSIZE_USERPROC2,        UserMainProc2);
    OS_CONFIG_TASK(USERPROC3_ID,     PRI_USERPROC3,       STKSIZE_USERPROC3,        UserMainProc3);
    OS_CONFIG_TASK(USERPROC4_ID,     PRI_USERPROC4,       STKSIZE_USERPROC4,        UserMainProc4);
    OS_CONFIG_TASK(USERPROC5_ID,     PRI_USERPROC5,       STKSIZE_USERPROC5,        UserMainProc5);
    OS_CONFIG_TASK(USERPROC6_ID,     PRI_USERPROC6,       STKSIZE_USERPROC6,        UserMainProc6);
    OS_CONFIG_TASK(USERPROC7_ID,     PRI_USERPROC7,       STKSIZE_USERPROC7,        UserMainProc7);
    OS_CONFIG_TASK(USERPROC8_ID,     PRI_USERPROC8,       STKSIZE_USERPROC8,        UserMainProc8);
#endif
//#NT#2016/03/30#Jeah Yen -end
}

BOOL bSystemFlow = 1;

int UserMainProc_Init(void)
{
    System_BootStart();
    bSystemFlow = 1;
//#NT#2016/03/30#Jeah Yen -begin
//#NT#isolate fast boot code
#if (POWERON_FAST_BOOT == ENABLE)
    INIT_CLRFLAG(FLGINIT_MASKALL);

    INIT_SETFLAG(FLGINIT_BEGIN); //set by itself
    sta_tsk(USERPROC2_ID, 0);
    sta_tsk(USERPROC3_ID, 0);
    sta_tsk(USERPROC4_ID, 0);
    sta_tsk(USERPROC5_ID, 0);
    sta_tsk(USERPROC6_ID, 0);
    sta_tsk(USERPROC7_ID, 0);
    sta_tsk(USERPROC8_ID, 0);
#endif
//#NT#2016/03/30#Jeah Yen -end
    SystemInit();

//#NT#2016/03/30#Jeah Yen -begin
//#NT#isolate fast boot code
#if (POWERON_FAST_BOOT == ENABLE)
    INIT_SETFLAG(FLGINIT_END);
#endif
//#NT#2016/03/30#Jeah Yen -end
    System_BootEnd();
    return 0;
}

int UserMainProc_Exit(void)
{
    System_ShutDownStart();
    bSystemFlow = 0;
//#NT#2016/03/30#Jeah Yen -begin
//#NT#isolate fast boot code
#if (POWERON_FAST_BOOT == ENABLE)
    EXIT_CLRFLAG(FLGEXIT_MASKALL);

    EXIT_SETFLAG(FLGEXIT_BEGIN); //set by itself
    sta_tsk(USERPROC2_ID, 0);
    sta_tsk(USERPROC3_ID, 0);
    sta_tsk(USERPROC4_ID, 0);
    sta_tsk(USERPROC5_ID, 0);
    sta_tsk(USERPROC6_ID, 0);
    sta_tsk(USERPROC7_ID, 0);
    sta_tsk(USERPROC8_ID, 0);
#endif
//#NT#2016/03/30#Jeah Yen -end
    SystemExit();

//#NT#2016/03/30#Jeah Yen -begin
//#NT#isolate fast boot code
#if (POWERON_FAST_BOOT == ENABLE)
    EXIT_SETFLAG(FLGEXIT_END);
#endif
//#NT#2016/03/30#Jeah Yen -end
    System_ShutDownEnd();
    return 0;
}

//#NT#2016/03/30#Jeah Yen -begin
//#NT#isolate fast boot code
#if (POWERON_FAST_BOOT == ENABLE)
void UserMainProc2(void)
{
    kent_tsk();
    if(bSystemFlow == 1)
    {
    INIT_WAITFLAG(FLGINIT_BEGIN2); //wait until trigger begin
    SystemInit2();
    INIT_SETFLAG(FLGINIT_END2);
    }
    else
    {
    EXIT_WAITFLAG(FLGEXIT_BEGIN2); //wait until trigger begin
    SystemExit2();
    EXIT_SETFLAG(FLGEXIT_END2);
    }
    ext_tsk();
}

void UserMainProc3(void)
{
    kent_tsk();
    if(bSystemFlow == 1)
    {
    INIT_WAITFLAG(FLGINIT_BEGIN3); //wait until trigger begin
    SystemInit3();
    INIT_SETFLAG(FLGINIT_END3);
    }
    else
    {
    EXIT_WAITFLAG(FLGEXIT_BEGIN3); //wait until trigger begin
    SystemExit3();
    EXIT_SETFLAG(FLGEXIT_END3);
    }
    ext_tsk();
}

void UserMainProc4(void)
{
    kent_tsk();
    if(bSystemFlow == 1)
    {
    INIT_WAITFLAG(FLGINIT_BEGIN4); //wait until trigger begin
    SystemInit4();
    INIT_SETFLAG(FLGINIT_END4);
    }
    else
    {
    EXIT_WAITFLAG(FLGEXIT_BEGIN4); //wait until trigger begin
    SystemExit4();
    EXIT_SETFLAG(FLGEXIT_END4);
    }
    ext_tsk();
}

void UserMainProc5(void)
{
    kent_tsk();
    if(bSystemFlow == 1)
    {
    INIT_WAITFLAG(FLGINIT_BEGIN5); //wait until trigger begin
    SystemInit5();
    INIT_SETFLAG(FLGINIT_END5);
    }
    else
    {
    EXIT_WAITFLAG(FLGEXIT_BEGIN5); //wait until trigger begin
    SystemExit5();
    EXIT_SETFLAG(FLGEXIT_END5);
    }
    ext_tsk();
}

void UserMainProc6(void)
{
    kent_tsk();
    if(bSystemFlow == 1)
    {
    INIT_WAITFLAG(FLGINIT_BEGIN6); //wait until trigger begin
    SystemInit6();
    INIT_SETFLAG(FLGINIT_END6);
    }
    else
    {
    EXIT_WAITFLAG(FLGEXIT_BEGIN6); //wait until trigger begin
    SystemExit6();
    EXIT_SETFLAG(FLGEXIT_END6);
    }
    ext_tsk();
}

void UserMainProc7(void)
{
    kent_tsk();
    if(bSystemFlow == 1)
    {
    INIT_WAITFLAG(FLGINIT_BEGIN7); //wait until trigger begin
    SystemInit7();
    INIT_SETFLAG(FLGINIT_END7);
    }
    else
    {
    EXIT_WAITFLAG(FLGEXIT_BEGIN7); //wait until trigger begin
    SystemExit7();
    EXIT_SETFLAG(FLGEXIT_END7);
    }
    ext_tsk();
}

void UserMainProc8(void)
{
    kent_tsk();
    if(bSystemFlow == 1)
    {
    INIT_WAITFLAG(FLGINIT_BEGIN8); //wait until trigger begin
    SystemInit8();
    INIT_SETFLAG(FLGINIT_END8);
    }
    else
    {
    EXIT_WAITFLAG(FLGEXIT_BEGIN8); //wait until trigger begin
    SystemExit8();
    EXIT_SETFLAG(FLGEXIT_END8);
    }
    ext_tsk();
}

#endif
//#NT#2016/03/30#Jeah Yen -end

