
#include "SysKer.h"
#include "timer.h"
#include "Consts.h"
#include "Flag.h"
#include "interrupt.h"
#include "stdio.h"
#include "gpio.h"
#include "LensDrv_Common.h"
#include "lens_drv_ff_c10.h"
#include "DxLens.h"
#include "Utility.h"
#include "adc.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          LENSDRVFF_C10
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#define LD_DELAYMS(x)   Delay_DelayMs(x)
#define LD_DELAYUS(x)   Delay_DelayUs(x)


//static UINT32 Aperture_Pos_Tbl[IRIS_TOTAL_SEGMENT] = {5, 7, 10, 13, 16, 19};
static LENSDRV_FF_C10_PARA FF_c10_Para={0};

/* Focal length */
static UINT32 g_uiFFFocalLength = 0;

//Prototype declaration
static void        lens_ff_c10_init(PLENS_DEVICE_OBJ pLensDev, PMOTOR_TAB pMotorTab);
static UINT32      lens_ff_c10_getSignal(LD_SIGNAL signal);
static void        lens_ff_c10_Zoom_InitPart1(void);
static INT32       lens_ff_c10_Zoom_InitPart2(void);
static void        lens_ff_c10_Zoom_SetBoundaryCheck(BOOL en);
static void        lens_ff_c10_Zoom_SetSpeed(ZOOM_SPEED_CATEGORY category);
static UINT32      lens_ff_c10_Zoom_GetMinSection(void);
static UINT32      lens_ff_c10_Zoom_GetMaxSection(void);
static UINT32      lens_ff_c10_Zoom_GetSection(void);
static UINT32      lens_ff_c10_Zoom_GetStep(void);
static UINT32      lens_ff_c10_Zoom_GetSectionStep(void);
static void        lens_ff_c10_Zoom_GoSection(UINT32 section);
static void        lens_ff_c10_Zoom_GoStep(INT32 position);
static INT32       lens_ff_c10_Zoom_Press(UINT32 zoom);
static UINT32      lens_ff_c10_Zoom_Release(void);
static INT32       lens_ff_c10_Zoom_Retract(void);
static INT32       lens_ff_c10_Focus_Init(void);
static UINT32      lens_ff_c10_Focus_GetLength(UINT32 ZoomSection);
static void        lens_ff_c10_Focus_SetFocalLength(UINT32 ZoomSection, UINT32 FocalLength);
static INT32       lens_ff_c10_Focus_GetPosition(void);
static UINT32      lens_ff_c10_Focus_GetDistTable(UINT32 section);
static INT32       lens_ff_c10_Focus_GetDefultTable(UINT32 section, UINT32 FocusDistance);
static INT32       lens_ff_c10_Focus_GetTable(UINT32 section, UINT32 FocusDistance);
static INT32       lens_ff_c10_Focus_GetRange(FOCUS_RANGE range);
static void        lens_ff_c10_Focus_DoEXC(BOOL on,UINT32 dir);
static void        lens_ff_c10_Focus_Go2(INT32 Position);
static INT32       lens_ff_c10_Focus_Reset(FOCUS_RETRACT_POSITION position);
static void        lens_ff_c10_Aperture_Init(void);
static IRIS_POS    lens_ff_c10_Aperture_GetPosition(void);
static UINT32      lens_ff_c10_Aperture_GetFNO(UINT32 ZoomSection,UINT32 IrisPos);
static void        lens_ff_c10_Aperture_Go2Pos(IRIS_POS position);
static void        lens_ff_c10_shutter_setState(MOTOR_SHUTTER_ACT state, UINT32 param1);
static void        lens_ff_c10_ircut_setState(MOTOR_IRCUT_ACT state, UINT32 param1);
static INT32       lens_ff_c10_LensRetract(void);
static INT32       lens_ff_c10_LensReset(void);
static INT32       lens_ff_c10_LensPowerOff(void);
static void        lens_ff_c10_MakeAFTable(void);
static INT32       lens_ff_c10_SetCalData(INT16 *pCalTbl, UINT32 uiTblSize);
static void        lens_ff_c10_Focus_AfterZoomMoving(UINT32 uiCmd);

static LENSCTRL_TASKAPIOBJ g_FF_c10_LensCtrlTskAPIObj = {NULL};

static LENS_TAB g_pFF_c10_Obj =
{
    //Lens
    lens_ff_c10_init,
    lens_ff_c10_getSignal,
    //Zoom
    lens_ff_c10_Zoom_InitPart1,
    lens_ff_c10_Zoom_InitPart2,
    lens_ff_c10_Zoom_SetBoundaryCheck,
    lens_ff_c10_Zoom_SetSpeed,
    lens_ff_c10_Zoom_GetMinSection,
    lens_ff_c10_Zoom_GetMaxSection,
    lens_ff_c10_Zoom_GetSection,
    lens_ff_c10_Zoom_GetStep,
    lens_ff_c10_Zoom_GetSectionStep,
    lens_ff_c10_Zoom_GoSection,
    NULL,
    lens_ff_c10_Zoom_GoStep,
    lens_ff_c10_Zoom_Press,
    lens_ff_c10_Zoom_Release,
    lens_ff_c10_Zoom_Retract,
    //Focus
    lens_ff_c10_Focus_Init,
    lens_ff_c10_Focus_GetLength,
    lens_ff_c10_Focus_SetFocalLength,
    lens_ff_c10_Focus_GetPosition,
    lens_ff_c10_Focus_GetDistTable,
    lens_ff_c10_Focus_GetDefultTable,
    lens_ff_c10_Focus_GetTable,
    lens_ff_c10_Focus_GetRange,
    lens_ff_c10_Focus_DoEXC,
    lens_ff_c10_Focus_Go2,
    lens_ff_c10_Focus_Reset,
    //Iris
    lens_ff_c10_Aperture_Init,
    lens_ff_c10_Aperture_GetPosition,
    lens_ff_c10_Aperture_GetFNO,
    lens_ff_c10_Aperture_Go2Pos,
    //Shutter
    lens_ff_c10_shutter_setState,
    //IRCUT
    lens_ff_c10_ircut_setState,
    //Misc
    lens_ff_c10_LensRetract,
    lens_ff_c10_LensReset,
    lens_ff_c10_MakeAFTable,
    lens_ff_c10_SetCalData,
    lens_ff_c10_Focus_AfterZoomMoving,
    lens_ff_c10_LensPowerOff,
    NULL,
    &g_FF_c10_LensCtrlTskAPIObj // PLENSCTRL_TASKAPIOBJ

};

//--------------------------------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//This zone is for lens control.
//--------------------------------------------------------------------------------------------------------------------------

static void lens_ff_c10_init(PLENS_DEVICE_OBJ pLensDev, PMOTOR_TAB pMotorTab)
{
    DBG_MSG("%s\r\n", __func__);
    g_pFF_c10_Obj.pMotor = pMotorTab;
}

/**
    Get FF_C10 lens object

    @return FF_C10 lens object pointer

*/
PLENS_TAB lens_getObject(void)
{
    DBG_MSG("%s\r\n", __func__);

    return &g_pFF_c10_Obj;
}
static UINT32 lens_ff_c10_getSignal(LD_SIGNAL signal)
{
    return 0;
}

//--------------------------------------------------------------------------------------------------------------------------
//Part of Zoom
//--------------------------------------------------------------------------------------------------------------------------
static void lens_ff_c10_Zoom_InitPart1(void)
{

}

static INT32 lens_ff_c10_Zoom_InitPart2(void)
{
    return ERR_OK;
}

static void lens_ff_c10_Zoom_SetBoundaryCheck(BOOL en)
{

}

static void lens_ff_c10_Zoom_SetSpeed(ZOOM_SPEED_CATEGORY category)
{

}

static UINT32 lens_ff_c10_Zoom_GetMinSection(void)
{
    return 0;
}

static UINT32 lens_ff_c10_Zoom_GetMaxSection(void)
{
    return 0;
}

static UINT32 lens_ff_c10_Zoom_GetSection(void)
{
    return 0;
}

static UINT32 lens_ff_c10_Zoom_GetStep(void)
{
    return 0;
}

static UINT32 lens_ff_c10_Zoom_GetSectionStep(void)
{
    return 0;
}

static void lens_ff_c10_Zoom_GoSection(UINT32 section)
{

}

/* position: absolute position. */
static void lens_ff_c10_Zoom_GoStep(INT32 position)
{

}

static INT32 lens_ff_c10_Zoom_Press(UINT32 zoom)
{
    return ERR_OK;
}

static UINT32 lens_ff_c10_Zoom_Release(void)
{

    return 0;
}

static INT32 lens_ff_c10_Zoom_Retract(void)
{
    return ERR_OK;
}


//--------------------------------------------------------------------------------------------------------------------------
//Part of Focus
//--------------------------------------------------------------------------------------------------------------------------
static INT32 lens_ff_c10_Focus_Init(void)
{
    return ERR_OK;
}

//um in unit.
static UINT32 lens_ff_c10_Focus_GetLength(UINT32 ZoomSection)
{
    // Focal Length = 2.57mm
    //return 2570;
    return g_uiFFFocalLength;
}

//um in unit
static void lens_ff_c10_Focus_SetFocalLength(UINT32 ZoomSection, UINT32 FocalLength)
{
    g_uiFFFocalLength = FocalLength;
}

static INT32 lens_ff_c10_Focus_GetPosition(void)
{
    return 0;
}

static UINT32 lens_ff_c10_Focus_GetDistTable(UINT32 section)
{
    return 0;
}

static INT32 lens_ff_c10_Focus_GetDefultTable(UINT32 section, UINT32 FocusDistance)
{
    return 0;
}

#if 0
UINT32 lens_ff_c10_Focus_GetDist(void)
{
   return 0;
}
#endif

static INT32 lens_ff_c10_Focus_GetTable(UINT32 section, UINT32 FocusDistance)
{
    return 0;
}

static INT32 lens_ff_c10_Focus_GetRange(FOCUS_RANGE range)
{
    return 0;
}

static void lens_ff_c10_Focus_DoEXC(BOOL on, UINT32 dir)
{

}

static void lens_ff_c10_Focus_Go2(INT32 position)
{

}

static INT32 lens_ff_c10_Focus_Reset(FOCUS_RETRACT_POSITION position)
{
    return ERR_OK;
}


//--------------------------------------------------------------------------------------------------------------------------
//Part of Aperture
//--------------------------------------------------------------------------------------------------------------------------
static void lens_ff_c10_Aperture_Init(void)
{
    FF_c10_Para.AperturePos = IRIS_POS_BIG;
    lens_ff_c10_Aperture_Go2Pos(IRIS_POS_BIG);
}

static IRIS_POS lens_ff_c10_Aperture_GetPosition(void)
{
    return FF_c10_Para.AperturePos;
}

static UINT32 lens_ff_c10_Aperture_GetFNO(UINT32 ZoomSection,UINT32 IrisPos)
{
    //return Aperture_Zoom_Tbl[ZoomSection][IrisPos];
    return 0;
}

static void lens_ff_c10_Aperture_Go2Pos(IRIS_POS position)
{
    if ((position >= IRIS_POS_MAX))
    {
        DBG_ERR("%s, parameters error, (%d)\r\n",__func__,position);
    }
    else if(FF_c10_Para.AperturePos != position)
    {
        g_FF_c10_LensCtrlTskAPIObj.lensctrltsk_aperture_setstate(MOTOR_APERTURE_NORMAL, position);//LensCtrl_Aperture_SetState(MOTOR_APERTURE_NORMAL,position);

        FF_c10_Para.AperturePos = position;
    }
}

//--------------------------------------------------------------------------------------------------------------------------
//Part of Shutter
//--------------------------------------------------------------------------------------------------------------------------
static void lens_ff_c10_shutter_setState(MOTOR_SHUTTER_ACT state, UINT32 param1)
{
    g_FF_c10_LensCtrlTskAPIObj.lensctrltsk_shutter_setstate(state, param1);//LensCtrl_Shutter_SetState(state, param1);
}

//--------------------------------------------------------------------------------------------------------------------------
//Part of IRCUT
//--------------------------------------------------------------------------------------------------------------------------
static void lens_ff_c10_ircut_setState(MOTOR_IRCUT_ACT state, UINT32 param1)
{
    g_FF_c10_LensCtrlTskAPIObj.lensctrltsk_ircut_setstate(state, param1);//LensCtrl_IRCUT_SetState(state, param1);
}

//--------------------------------------------------------------------------------------------------------------------------
//Part of misc.
//--------------------------------------------------------------------------------------------------------------------------
static INT32 lens_ff_c10_LensRetract(void)
{
    return lens_ff_c10_Zoom_Retract();
}

static INT32 lens_ff_c10_LensReset(void)
{
    return ERR_OK;
}

static void lens_ff_c10_MakeAFTable(void)
{

}

static INT32 lens_ff_c10_SetCalData(INT16 *pCalTbl, UINT32 uiTblSize)
{
    return ERR_OK;
}

static void  lens_ff_c10_Focus_AfterZoomMoving(UINT32 uiCmd)
{
}

static INT32 lens_ff_c10_LensPowerOff(void)
{
    return ERR_OK;
}
