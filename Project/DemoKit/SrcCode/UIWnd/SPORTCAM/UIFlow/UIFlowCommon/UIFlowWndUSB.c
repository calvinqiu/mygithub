//This source code is generated by UI Designer Studio.

#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "NVTToolCommand.h"
#include "UIFlowWndUSBRes.c"
#include "UIFlow.h"
#if defined(YQCONFIG_UART_TO_MTK_SUPPORT)
#include "UART_TO_MTK.h"
#endif
//---------------------UIFlowWndUSBCtrl Control List---------------------------
CTRL_LIST_BEGIN(UIFlowWndUSB)
CTRL_LIST_ITEM(UIFlowWndMSDC_StaticTxt_MassStorage)
CTRL_LIST_END

//----------------------UIFlowWndUSBCtrl Event---------------------------
INT32 UIFlowWndUSB_OnOpen(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndUSB_OnClose(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndUSB_OnTimer(VControl *, UINT32, UINT32 *);
#if (SDHOTPLUG_FUNCTION == ENABLE)
INT32 UIFlowWndUSB_OnStorageChange(VControl *, UINT32, UINT32 *);
#endif
#if defined(YQCONFIG_UART_TO_MTK_SUPPORT)
INT32 UIFlowWndUSB_OnMTKGpsInfo(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndUSB_OnChangeMode(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndUSB_OnGetCurStatus(VControl *, UINT32, UINT32 *);
#endif
EVENT_BEGIN(UIFlowWndUSB)
EVENT_ITEM(NVTEVT_OPEN_WINDOW,UIFlowWndUSB_OnOpen)
EVENT_ITEM(NVTEVT_CLOSE_WINDOW,UIFlowWndUSB_OnClose)
EVENT_ITEM(NVTEVT_TIMER,UIFlowWndUSB_OnTimer)
#if (SDHOTPLUG_FUNCTION == ENABLE)
EVENT_ITEM(NVTEVT_STORAGE_INIT,UIFlowWndUSB_OnStorageChange)
EVENT_ITEM(NVTEVT_STORAGE_CHANGE,UIFlowWndUSB_OnStorageChange)
#endif
#if defined(YQCONFIG_UART_TO_MTK_SUPPORT)
EVENT_ITEM(NVTEVT_MTK_GPSINFO,UIFlowWndUSB_OnMTKGpsInfo)
EVENT_ITEM(NVTEVT_MTK_CHANGE_MODE,UIFlowWndUSB_OnChangeMode)
EVENT_ITEM(NVTEVT_MTK_OTG_TO_MTK, UIFlowWndUSB_OnGetCurStatus)
#endif
EVENT_END

INT32 UIFlowWndUSB_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    if (paramNum==1)
    {
        if (paramArray[0]==UIFlowWndUSB_PCC_MODE)
        { // show pcc string
            UxStatic_SetData(&UIFlowWndMSDC_StaticTxt_MassStorageCtrl,
                         STATIC_VALUE,STRID_PCC);
        }
        else
        {
        	#if (SDHOTPLUG_FUNCTION == ENABLE)
        	if(UIStorageCheck(STORAGE_CHECK_ERROR, NULL) == TRUE)
		    {
		       Ux_OpenWindow(&UIFlowWndWrnMsgCtrl,2,UIFlowWndWrnMsg_StatusTXT_Msg_STRID_PLEASE_INSERT_SD, FLOWWRNMSG_TIMER_KEEP);
		       return NVTEVT_CONSUME;
		    }
		    #endif
            // show msdc string
            UxStatic_SetData(&UIFlowWndMSDC_StaticTxt_MassStorageCtrl,
                         STATIC_VALUE,STRID_MSDC);
        }
    }
    Ux_DefaultEvent(pCtrl,NVTEVT_OPEN_WINDOW,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndUSB_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_DefaultEvent(pCtrl,NVTEVT_CLOSE_WINDOW,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndUSB_OnTimer(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
#if (SDHOTPLUG_FUNCTION == ENABLE)
INT32 UIFlowWndUSB_OnStorageChange(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	if(STRID_MSDC == UxStatic_GetData(&UIFlowWndMSDC_StaticTxt_MassStorageCtrl,STATIC_VALUE))
	{
    	Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, System_GetState(SYS_STATE_CURRMODE));
   	}
    return NVTEVT_CONSUME;
}
#endif
#if defined(YQCONFIG_UART_TO_MTK_SUPPORT)
INT32 UIFlowWndUSB_OnMTKGpsInfo(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT8   uiResqData[16]= {0};
    UINT32 FrameID;
    if(paramNum>0)
        FrameID=paramArray[0];
    uiResqData[0] =( (0x03<<1)|uiResqData[0]);
    if(System_GetSensor2Plug())
    {
        uiResqData[1] = 0x01;
    }
    else
    {
        uiResqData[1] = 0x0;
    }
   debug_msg("UIFlowWndUSB_OnMTKGpsInfo:--%x--%x--\r\n",uiResqData[0],uiResqData[1]);
    MTKComposeCMDRspFrame(FrameID, CMD_GPS,(UINT8 *)&uiResqData, 2/*1*/);	//pgl modify 20160905
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndUSB_OnChangeMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 FrameID;
    UIFlowwndMovieSetNeedToRecordFlag(TRUE);
    debug_msg("QIUHAN======================UIFlowWndUSB_OnChangeMode\r\n");
    Ux_SendEvent(&UISetupObjCtrl,NVTEVT_EXE_CHANGEDSCMODE,1,DSCMODE_CHGTO_NEXT);

    return NVTEVT_CONSUME;
}


INT32 UIFlowWndUSB_OnGetCurStatus(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray){
   UINT8   uiResqData[16]= {0};
    UINT32 FrameID;
    if(paramNum>0)
        FrameID=paramArray[0];
   uiResqData[0]=0x00;
   MTKComposeCMDRspFrame(FrameID, CMD_OTG_TO_MTK,&uiResqData, 1);
   return NVTEVT_CONSUME;
}
#endif
//----------------------UIFlowWndMSDC_StaticTxt_MassStorageCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndMSDC_StaticTxt_MassStorage)
EVENT_END

