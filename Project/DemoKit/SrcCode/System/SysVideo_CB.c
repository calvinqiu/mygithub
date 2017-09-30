/**
    DevMan, Service command function implementation

    @file       DeviceUsbMan.c
    @ingroup    mDEVMAN

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysVideoExe
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#include "SysKer.h"
#include "Debug.h"
#include "NvtUser.h"

//#include "NvtSystem.h"
#include "GxVideo.h"
#include "GxSystem.h"
#include "AppControl.h"
//#NT#2016/06/21#Jeah Yen -begin
//#NT#Support display fast switch
#include "ImageUnit_CamDisp.h"
//#NT#2016/06/21#Jeah Yen -end

/////////////////////////////////////////////////////////////////////////////
// DISPLAY
#include "GxDisplay.h"

UINT32 g_Disp_Device1 = 0;
UINT32 g_Disp_Device2 = 0;

void Video_CB(UINT32 event, UINT32 param1, UINT32 param2);

void Video_CB(UINT32 event, UINT32 param1, UINT32 param2)
{
    switch(event)
    {
// part-1
///////////////////////////////////////////////////////
// part-3
    case DISPLAY_CB_PLUG:
        //This CB will be trigger when TV or HDMI plug
        DBG_IND("DISPLAY_CB_PLUG\r\n");
        {
            Ux_PostEvent(NVTEVT_VIDEO_INSERT, 1, param1);
        }
        break;
    case DISPLAY_CB_UNPLUG:
        //This CB will be trigger when TV or HDMI unplug
        DBG_IND("DISPLAY_CB_UNPLUG\r\n");
        {
            Ux_PostEvent(NVTEVT_VIDEO_REMOVE, 1, param1);
        }
        break;
    case DISPLAY_CB_DIR:
        //This CB will be trigger when LCD rotate
        DBG_IND("DISPLAY_CB_DIR\r\n");
        {
            Ux_PostEvent(NVTEVT_VIDEO_DIR, 2, param1, param2);
        }
        break;
    }
}


//#NT#2016/06/21#Jeah Yen -begin
//#NT#Support display fast switch
void AppDisp_CB(UINT32 event, UINT32 param1, UINT32 param2);

void AppDisp_CB(UINT32 event, UINT32 param1, UINT32 param2)
{
    switch(event)
    {
    case APPDISP_CB_FRAMESTABLE:
        //This CB will be trigger when after change config of DispSrv and first-time OnDraw finish
        DBG_IND("APPDISP_CB_FRAMESTABLE\r\n");
        {
            #if (DISPLAY_FAST_SWITCH == ENABLE)
            Ux_SendEvent(0, NVTEVT_VIDEO_ATTACH, 1, 0); //end of video attach
            //DBGD(Perf_GetCurrent());
            #endif
        }
        break;
    }
}
//#NT#2016/06/21#Jeah Yen -end

