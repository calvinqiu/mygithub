/*
    System Lens Callback

    System Callback for Lens Module.

    @file       SysLens_Exe.c
    @ingroup    mIPRJSYS

    @note

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIFrameworkExt.h"
#include "AppLib.h"
#include "DeviceCtrl.h"

#include "UIInfo.h" //for UI_SetData(), UI_GetData()
#include "UIMode.h"
#include "UIModePhoto.h"
#include "UIAppMovie.h"
#include "ImageApp_CamPhoto.h" //for NVTEVT_CB_ZOOM

#include "Lens.h"
#include "LensInfo.h"
#include "GxFlash.h"
#include "SwTimer.h"


//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysLensExe
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////


extern SX_CMD_ENTRY Cmd_lens[]; //device (lens)

int SX_TIMER_DET_CLOSE_LENS_ID = -1;

#if LENS_FUNCTION
SX_TIMER_ITEM(System_DetCloseLens, System_DetCloseLens, 50, TRUE)
#endif

extern void DrvLens_PowerOn(void);
extern void DrvLens_PowerOff(void);
extern void Photo_LensCB(UINT32 MessageID, UINT32 *Parameter);
extern void LensAPICB(UINT32 MessageID, UINT32 *Parameter);

void System_OnLensInit(void)
{
    //PHASE-1 : Init & Open Drv or DrvExt
    {
#if LENS_FUNCTION
        DrvLens_RegCtrlCB((void*)Photo_LensCB, (void*)NULL, (void*)LensAPICB);
#endif
    }
    //PHASE-2 : Init & Open Lib or LibExt
    {

    if(System_GetState(SYS_STATE_POWERON) != SYS_POWERON_LOWPOWER)
    {
        DrvLens_PowerOn();
        if(DrvLens_GetLensCtrlObj()) // Init lens module if it's not a null object.
        {
            Lens_Module_Init(DrvLens_GetLensCtrlObj());
            /* Focal length setup. */
            Lens_Focus_SetFocalLength_um(0, DrvLens_GetFFFocalLength());
        }
    }

#if LENS_FUNCTION
    SX_TIMER_DET_CLOSE_LENS_ID = SxTimer_AddItem(&Timer_System_DetCloseLens);
#endif

    }
}

void System_OnLensExit(void)
{
    //PHASE-2 : Close Lib or LibExt
    {
    DBG_IND("power off state=%d\r\n",System_GetState(SYS_STATE_POWEROFF));
#if LENS_FUNCTION
    //if(System_GetState(SYS_STATE_POWEROFF) != SYS_POWEROFF_LOWPOWER)
    {
        if(DevCtrl_Lens_PowerOff_IsNeedRetract())
        {
            DevCtrl_Lens_PowerOff_Retract();
        }
    }
#endif
    }
    //PHASE-1 : Close Drv or DrvExt
    {
    }
}

static UINT32 g_uiPBCloseLensCnt = 0;

void System_DetCloseLens(void)
{
    #define CLOSE_LENS_MAX_COUNT  10   //10secs
    g_uiPBCloseLensCnt++;

    DBG_IND("Cnt=%d\r\n", g_uiPBCloseLensCnt);

    if( (UI_GetData(FL_ModeIndex) == DSC_MODE_PLAYBACK) && (UI_GetData(FL_NextMode) == DSC_MODE_PLAYBACK)  && UI_GetData(FL_LensOpen)&&(g_uiPBCloseLensCnt >=CLOSE_LENS_MAX_COUNT))
    {
        Ux_FlushEventByRange(NVTEVT_EXE_PB_RETRACT_LENS,NVTEVT_EXE_PB_RETRACT_LENS);
        Ux_PostEvent(NVTEVT_EXE_PB_RETRACT_LENS,0);
    }

    if((g_uiPBCloseLensCnt >=CLOSE_LENS_MAX_COUNT)||(UI_GetData(FL_ModeIndex) != DSC_MODE_PLAYBACK) || UI_GetData(FL_NextMode) != DSC_MODE_PLAYBACK) //10secs
        g_uiPBCloseLensCnt = 0;
}

void System_ResetDetCloseLenCount(void)
{
    g_uiPBCloseLensCnt = 0;
}

INT32 System_OnLensAttach(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    int iCurrMode = System_GetState(SYS_STATE_NEXTMODE);
    DBG_IND("\r\n");

    if(iCurrMode == PRIMARY_MODE_PLAYBACK)
    {
        //DevCtrl_Lens(FALSE);  //delay this after enter Photo MODE!
    }
    else
    if(iCurrMode == PRIMARY_MODE_PHOTO)
    {
        DevCtrl_Lens(TRUE);
    }
    else
    if(iCurrMode == PRIMARY_MODE_MOVIE)
    {
        DevCtrl_Lens(TRUE);
    }
    else
    if(iCurrMode == PRIMARY_MODE_USBPCC)
    {
        DevCtrl_Lens(TRUE);
    }
    else
    {
        DevCtrl_Lens(FALSE);
    }
    return NVTEVT_CONSUME;
}

INT32 System_OnLensDetach(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}



/////////////////////////////////////////////////////////////////////////////
// Power On Flow
/////////////////////////////////////////////////////////////////////////////

void Lens_PowerOn_Load(void)
{
    #if (LENS_FUNCTION == ENABLE)
    //  Read lens init flag and set it.
    UINT32 lensFlag=0;
    lensFlag = Load_LensInfo();
    Set_LensFlag(lensFlag);
    #endif
}

void Lens_PowerOn_Apply(void)
{
}

/////////////////////////////////////////////////////////////////////////////



BOOL gLensNeedAdvance = FALSE;

BOOL DevCtrl_Lens_PowerOn_IsUnderAdvance(void)
{
    #if (LENS_FUNCTION == ENABLE)
    return gLensNeedAdvance;
    #else
    return 0;
    #endif
}

void DevCtrl_Lens_PowerOn_Advance(void)
{
    #if (LENS_FUNCTION == ENABLE)
    Lens_Init(LENS_INIT_ZOOM_PART1);
    gLensNeedAdvance = TRUE;
    #endif
}

/////////////////////////////////////////////////////////////////////////////
// Power Off Flow
/////////////////////////////////////////////////////////////////////////////

BOOL DevCtrl_Lens_PowerOff_IsNeedRetract(void)
{
    #if (LENS_FUNCTION == ENABLE)
    DBG_IND("\r\n");
    return ((Lens_Module_GetState() != LENS_STATE_NONE) || (Get_LensFlag() == ZOOM_FLAG_OUTSIDE));
    #else
    return 0;
    #endif
}

void DevCtrl_Lens_PowerOff_Retract(void)
{
    #if (LENS_FUNCTION == ENABLE)
    DBG_IND("\r\n");
    if (Lens_Retract()!=ERR_OK)
    {
        DBG_ERR("Lens error\r\n");
    }
    else if(!Get_LensFlag() && (!GxPower_GetControl(GXPWR_CTRL_IS_DUMMUYLOAD_POWEROFF)))
    {
        Lens_Init(LENS_INIT_ZOOM_PART1);
        Lens_Init(LENS_INIT_ZOOM_PART2);
        Lens_Init(LENS_INIT_APERTURE);
        if (Lens_Retract()!=ERR_OK)
        {
            DBG_ERR("Lens error2\r\n");
        }

    }
    #endif
}

void DevCtrl_Lens_PowerOff_Exit(void)
{
    #if (LENS_FUNCTION == ENABLE)
    #endif
}

/////////////////////////////////////////////////////////////////////////////
// Playback Mode Control Flow
/////////////////////////////////////////////////////////////////////////////

void DevCtrl_Lens_Playback_Retract(void)
{
    #if (LENS_FUNCTION == ENABLE)
    BOOL bLensInitErr = FALSE;

    // During lens retract, does not detect key
    SxTimer_SetFuncActive(SX_TIMER_DET_KEY_ID,FALSE);
    bLensInitErr = Lens_Retract();
    SxTimer_SetFuncActive(SX_TIMER_DET_KEY_ID,TRUE);
    if(bLensInitErr)
    {
        DBG_ERR("\r\n");
        #if (LENSERROFF_FUNCTION == ENABLE)
        System_PowerOff(SYS_POWEROFF_LENSERROR);
        #endif
    }
    UI_SetData(FL_LensOpen, FALSE);
    SxTimer_SetFuncActive(SX_TIMER_DET_CLOSE_LENS_ID,FALSE);
    #endif
}

/////////////////////////////////////////////////////////////////////////////
// Photo Mode Control Flow
/////////////////////////////////////////////////////////////////////////////
BOOL    g_bLensInitErr  = FALSE;

/**
  Control the devices for Photo mode

  Control the devices before entering Photo mode.

  @param void
  @return void
*/
BOOL DevCtrl_Lens(BOOL bIsOpen)
{
    #if (LENS_FUNCTION == ENABLE)
    BOOL returnValue = TRUE;
    GxFlash_SuspendCharge();
    if (bIsOpen)
    {
        if (!UI_GetData(FL_LensOpen))
        {
            Lens_Init(LENS_INIT_ZOOM_PART1);
            UI_SetData(FL_LensOpen, TRUE);
            SxTimer_SetFuncActive(SX_TIMER_DET_CLOSE_LENS_ID,TRUE);
            Ux_SendEvent(&CustomPhotoObjCtrl,NVTEVT_EXE_ZOOM,2, UI_ZOOM_CTRL_RESET_DZOOM, FALSE);

        }
        if(Lens_Module_GetState()!=LENS_STATE_IDLE)
        {
            BOOL    bLensInitErr  = TRUE;
            INT32   retValue = ERR_OK;
            retValue = Lens_Init(LENS_INIT_ZOOM_PART2);

            if (retValue == ERR_OK)
            {
                Lens_Init(LENS_INIT_APERTURE);

                if(Lens_Init(LENS_INIT_FOCUS)==ERR_OK)
                {
                    bLensInitErr = FALSE;
                }
            }
            else if(retValue == ERR_LENS_INIT_FAIL)
            {
                DBG_ERR("Initiate zoom part 2 fail, try again\r\n");

                DBG_IND("Lens retract\r\n");
                Lens_Retract();

                DBG_IND("Initiate zoom part 1\r\n");
                Lens_Init(LENS_INIT_ZOOM_PART1);
                UI_SetData(FL_LensOpen, TRUE);
                SxTimer_SetFuncActive(SX_TIMER_DET_CLOSE_LENS_ID,TRUE);
                DBG_IND("Initiate zoom part 2\r\n");

                if (Lens_Init(LENS_INIT_ZOOM_PART2) == ERR_OK)
                {
                    DBG_IND("Initiate iris\r\n");

                    Lens_Init(LENS_INIT_APERTURE);

                    DBG_IND("Initiate focus\r\n");
                    if (Lens_Init(LENS_INIT_FOCUS) == ERR_OK)
                    {
                        bLensInitErr = FALSE;
                    }
                    else
                    {
                        bLensInitErr = TRUE;
                    }
                }
                else
                {
                    bLensInitErr = TRUE;
                }

            }
            if(bLensInitErr)
            {
                #if (LENSERROFF_FUNCTION == ENABLE)
                DBG_ERR("Initiation fail -> power off \r\n");
                g_bLensInitErr = TRUE;
                System_PowerOff(SYS_POWEROFF_LENSERROR);
                returnValue = FALSE;
                #endif
            }
        }
    }
    else
    {
        //#NT#Modified., whiling Startup with Photo but usb plugin quickly, lens not retract.
        if (UI_GetData(FL_LensOpen))
        {
            if(Lens_Module_GetState()==LENS_STATE_INIT_PART1)
            {
                INT32 nRetry = 20; //Retry 2 sec
                while((Lens_Module_GetState()==LENS_STATE_INIT_PART1) && (nRetry-- >= 0))
                {
                    SwTimer_DelayMs(10);
                }

                if(nRetry<0)
                    DBG_ERR("lens init time out!\r\n");
            }
            if(Lens_Retract()!=ERR_OK)
            {
                DBG_ERR("Lens error3\r\n");
                #if (LENSERROFF_FUNCTION == ENABLE)
                System_PowerOff(SYS_POWEROFF_LENSERROR);
                returnValue = FALSE;
                #endif
            }

            UI_SetData(FL_LensOpen, FALSE);
            SxTimer_SetFuncActive(SX_TIMER_DET_CLOSE_LENS_ID,FALSE);

        }
        // Fix USB power on can't retract lens if previous lens is outside.
        else if(!Get_LensFlag() && (!GxPower_GetControl(GXPWR_CTRL_IS_DUMMUYLOAD_POWEROFF)))
        {
            Lens_Init(LENS_INIT_ZOOM_PART1);
            Lens_Init(LENS_INIT_ZOOM_PART2);
            Lens_Init(LENS_INIT_APERTURE);

            if(Lens_Retract()!=ERR_OK)
            {
                DBG_ERR("Lens error4\r\n");
            }
        }
    }
    GxFlash_ResumeCharge();
    return returnValue;

    #else
    return TRUE;
    #endif
}

