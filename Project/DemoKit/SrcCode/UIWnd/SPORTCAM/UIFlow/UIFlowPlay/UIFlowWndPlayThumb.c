//This source code is generated by UI Designer Studio.

#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "NVTToolCommand.h"
#include "UIFlowWndPlayThumbRes.c"
#include "UIFlow.h"
#include "UIPlayComm.h"
#include "ImageUnit_CamDisp.h"

#define PBX_SLIDE_EFFECT_NONE                       0

//---------------------UIFlowWndPlayThumbCtrl Control List---------------------------
CTRL_LIST_BEGIN(UIFlowWndPlayThumb)
CTRL_LIST_ITEM(UIFlowWndPlayThumb_Panel0)
CTRL_LIST_ITEM(UIFlowWndPlayThumb_Panel1)
CTRL_LIST_ITEM(UIFlowWndPlayThumb_Panel2)
CTRL_LIST_ITEM(UIFlowWndPlayThumb_Panel3)
CTRL_LIST_ITEM(UIFlowWndPlayThumb_Panel4)
CTRL_LIST_ITEM(UIFlowWndPlayThumb_Panel5)
CTRL_LIST_ITEM(UIFlowWndPlayThumb_Panel6)
CTRL_LIST_ITEM(UIFlowWndPlayThumb_Panel7)
CTRL_LIST_ITEM(UIFlowWndPlayThumb_Panel8)
CTRL_LIST_ITEM(UIFlowWndPlayThumb_StaticICN_Protect0)
CTRL_LIST_ITEM(UIFlowWndPlayThumb_StaticICN_Protect1)
CTRL_LIST_ITEM(UIFlowWndPlayThumb_StaticICN_Protect2)
CTRL_LIST_ITEM(UIFlowWndPlayThumb_StaticICN_Protect3)
CTRL_LIST_ITEM(UIFlowWndPlayThumb_StaticICN_Protect4)
CTRL_LIST_ITEM(UIFlowWndPlayThumb_StaticICN_Protect5)
CTRL_LIST_ITEM(UIFlowWndPlayThumb_StaticICN_Protect6)
CTRL_LIST_ITEM(UIFlowWndPlayThumb_StaticICN_Protect7)
CTRL_LIST_ITEM(UIFlowWndPlayThumb_StaticICN_Protect8)
CTRL_LIST_ITEM(UIFlowWndPlayThumb_StaticICN_FileFilm0)
CTRL_LIST_ITEM(UIFlowWndPlayThumb_StaticICN_FileFilm1)
CTRL_LIST_ITEM(UIFlowWndPlayThumb_StaticICN_FileFilm2)
CTRL_LIST_ITEM(UIFlowWndPlayThumb_StaticICN_FileFilm3)
CTRL_LIST_ITEM(UIFlowWndPlayThumb_StaticICN_FileFilm4)
CTRL_LIST_ITEM(UIFlowWndPlayThumb_StaticICN_FileFilm5)
CTRL_LIST_ITEM(UIFlowWndPlayThumb_StaticICN_FileFilm6)
CTRL_LIST_ITEM(UIFlowWndPlayThumb_StaticICN_FileFilm7)
CTRL_LIST_ITEM(UIFlowWndPlayThumb_StaticICN_FileFilm8)
CTRL_LIST_ITEM(UIFlowWndPlayThumb_ThumbID)
CTRL_LIST_END

//----------------------UIFlowWndPlayThumbCtrl Event---------------------------
INT32 UIFlowWndPlayThumb_OnOpen(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndPlayThumb_OnClose(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndPlayThumb_OnKeyUp(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndPlayThumb_OnKeyDown(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndPlayThumb_OnKeyLeft(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndPlayThumb_OnKeyRight(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndPlayThumb_OnKeyEnter(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndPlayThumb_OnKeyMenu(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndPlayThumb_OnKeyMode(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndPlayThumb_OnKeyZoomin(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndPlayThumb_OnKeyShutter2(VControl *, UINT32, UINT32 *);
EVENT_BEGIN(UIFlowWndPlayThumb)
EVENT_ITEM(NVTEVT_OPEN_WINDOW,UIFlowWndPlayThumb_OnOpen)
EVENT_ITEM(NVTEVT_CLOSE_WINDOW,UIFlowWndPlayThumb_OnClose)
EVENT_ITEM(NVTEVT_KEY_UP,UIFlowWndPlayThumb_OnKeyUp)
EVENT_ITEM(NVTEVT_KEY_DOWN,UIFlowWndPlayThumb_OnKeyDown)
EVENT_ITEM(NVTEVT_KEY_LEFT,UIFlowWndPlayThumb_OnKeyLeft)
EVENT_ITEM(NVTEVT_KEY_RIGHT,UIFlowWndPlayThumb_OnKeyRight)
EVENT_ITEM(NVTEVT_KEY_ENTER,UIFlowWndPlayThumb_OnKeyEnter)
EVENT_ITEM(NVTEVT_KEY_MENU,UIFlowWndPlayThumb_OnKeyMenu)
EVENT_ITEM(NVTEVT_KEY_MODE,UIFlowWndPlayThumb_OnKeyMode)
EVENT_ITEM(NVTEVT_KEY_ZOOMIN,UIFlowWndPlayThumb_OnKeyZoomin)
EVENT_ITEM(NVTEVT_KEY_SHUTTER2,UIFlowWndPlayThumb_OnKeyShutter2)
EVENT_END

extern PURECT FlowPB_ThumbDisplay(void);
static void UIFlowWndPlayThumb_NaviKey(UINT32 key)
{
    BROWSE_NAVI_INFO BrowseNavi;
    FlowPB_ClearAllThumbIcon();
    SxTimer_SetFuncActive(SX_TIMER_DET_KEY_ID, FALSE);
    BrowseNavi.NaviKey = key;
    BrowseNavi.HorNums = FLOWPB_THUMB_H_NUM2;
    BrowseNavi.VerNums = FLOWPB_THUMB_V_NUM2;

    BrowseThumbNaviKey(&BrowseNavi);

    FlowPB_ShowAllThumbIcon();
    SxTimer_SetFuncActive(SX_TIMER_DET_KEY_ID, TRUE);
}
INT32 UIFlowWndPlayThumb_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    PURECT          pThumbRect;
    PLAY_BROWSER_OBJ     FlowPBBrowserObj={0};
    UINT32 FileNumsInDir=0, NumsPerPage;
    UINT8   HorNums, VerNums;

    FlowPB_ClearAllThumbIcon();
    pThumbRect = FlowPB_ThumbDisplay();
    HorNums = FLOWPB_THUMB_H_NUM2;
    VerNums = FLOWPB_THUMB_V_NUM2;
    PB_SetParam(PBPRMID_THUMB_LAYOUT_ARRAY, (UINT32)pThumbRect);
//#NT#2012/11/27#Scottie -begin
//#NT#Support Dual Display for PB
    if(ImageUnit_GetParam(&ISF_CamDisp, CAMDISP_PARAM_DISPCOUNT) == 2)
    {
        PB_SetParam(PBPRMID_THUMB_LAYOUT_ARRAY2, (UINT32)pThumbRect);
    }
//#NT#2012/11/27#Scottie -end
    NumsPerPage = VerNums * VerNums;
    PB_GetParam(PBPRMID_TOTAL_FILE_COUNT, &FileNumsInDir);
    FlowPBBrowserObj.BrowserCommand = PB_BROWSER_CURR;
    FlowPBBrowserObj.HorNums        = HorNums;
    FlowPBBrowserObj.VerNums        = VerNums;
    FlowPBBrowserObj.slideEffectFunc = PBX_SLIDE_EFFECT_NONE;
    if(FileNumsInDir <= NumsPerPage)// means only one page
        FlowPBBrowserObj.bReDecodeImages = FALSE;
    else
        FlowPBBrowserObj.bReDecodeImages = TRUE;
    PB_PlayBrowserMode(&FlowPBBrowserObj);
    PB_WaitCommandFinish(PB_WAIT_INFINITE);

    FlowPB_ShowAllThumbIcon();
    Ux_DefaultEvent(pCtrl,NVTEVT_OPEN_WINDOW,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndPlayThumb_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    Ux_DefaultEvent(pCtrl,NVTEVT_CLOSE_WINDOW,paramNum,paramArray);
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndPlayThumb_OnKeyUp(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    INT32 uiActKey;

    uiActKey = paramArray[0];
    switch(uiActKey)
    {

    case NVTEVT_KEY_PRESS:
        UIFlowWndPlayThumb_NaviKey(NVTEVT_KEY_UP);
        break;

    }

    return NVTEVT_CONSUME;
}
INT32 UIFlowWndPlayThumb_OnKeyDown(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    INT32 uiActKey;

    uiActKey = paramArray[0];
    switch(uiActKey)
    {

    case NVTEVT_KEY_PRESS:
        UIFlowWndPlayThumb_NaviKey(NVTEVT_KEY_DOWN);
        break;

    }

    return NVTEVT_CONSUME;
}
INT32 UIFlowWndPlayThumb_OnKeyLeft(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    INT32 uiActKey;

    uiActKey = paramArray[0];
    switch(uiActKey)
    {

    case NVTEVT_KEY_PRESS:
        UIFlowWndPlayThumb_NaviKey(NVTEVT_KEY_LEFT);
        break;

    }

    return NVTEVT_CONSUME;
}
INT32 UIFlowWndPlayThumb_OnKeyRight(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    INT32 uiActKey;

    uiActKey = paramArray[0];
    switch(uiActKey)
    {

    case NVTEVT_KEY_PRESS:
        UIFlowWndPlayThumb_NaviKey(NVTEVT_KEY_RIGHT);
        break;

    }

    return NVTEVT_CONSUME;
}
INT32 UIFlowWndPlayThumb_OnKeyEnter(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    FlowPB_ClearAllThumbIcon();
    Ux_Redraw();
    Ux_CloseWindow((VControl *)(&UIFlowWndPlayThumbCtrl), 1, NVTRET_THUMBNAIL);
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndPlayThumb_OnKeyMenu(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndPlayThumb_OnKeyMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //Ux_SendEvent(&UISetupObjCtrl,NVTEVT_EXE_CHANGEDSCMODE,1,DSCMODE_CHGTO_NEXT);
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndPlayThumb_OnKeyZoomin(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    FlowPB_ClearAllThumbIcon();
    Ux_Redraw();
    Ux_CloseWindow((VControl *)(&UIFlowWndPlayThumbCtrl), 1, NVTRET_THUMBNAIL);
    return NVTEVT_CONSUME;
}
INT32 UIFlowWndPlayThumb_OnKeyShutter2(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //Ux_SendEvent(&UISetupObjCtrl,NVTEVT_FORCETO_LIVEVIEW_MODE,0);
    return NVTEVT_CONSUME;
}
//---------------------UIFlowWndPlayThumb_Panel0Ctrl Control List---------------------------
CTRL_LIST_BEGIN(UIFlowWndPlayThumb_Panel0)
CTRL_LIST_END

//----------------------UIFlowWndPlayThumb_Panel0Ctrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlayThumb_Panel0)
EVENT_END

//---------------------UIFlowWndPlayThumb_Panel1Ctrl Control List---------------------------
CTRL_LIST_BEGIN(UIFlowWndPlayThumb_Panel1)
CTRL_LIST_END

//----------------------UIFlowWndPlayThumb_Panel1Ctrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlayThumb_Panel1)
EVENT_END

//---------------------UIFlowWndPlayThumb_Panel2Ctrl Control List---------------------------
CTRL_LIST_BEGIN(UIFlowWndPlayThumb_Panel2)
CTRL_LIST_END

//----------------------UIFlowWndPlayThumb_Panel2Ctrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlayThumb_Panel2)
EVENT_END

//---------------------UIFlowWndPlayThumb_Panel3Ctrl Control List---------------------------
CTRL_LIST_BEGIN(UIFlowWndPlayThumb_Panel3)
CTRL_LIST_END

//----------------------UIFlowWndPlayThumb_Panel3Ctrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlayThumb_Panel3)
EVENT_END

//---------------------UIFlowWndPlayThumb_Panel4Ctrl Control List---------------------------
CTRL_LIST_BEGIN(UIFlowWndPlayThumb_Panel4)
CTRL_LIST_END

//----------------------UIFlowWndPlayThumb_Panel4Ctrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlayThumb_Panel4)
EVENT_END

//---------------------UIFlowWndPlayThumb_Panel5Ctrl Control List---------------------------
CTRL_LIST_BEGIN(UIFlowWndPlayThumb_Panel5)
CTRL_LIST_END

//----------------------UIFlowWndPlayThumb_Panel5Ctrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlayThumb_Panel5)
EVENT_END

//---------------------UIFlowWndPlayThumb_Panel6Ctrl Control List---------------------------
CTRL_LIST_BEGIN(UIFlowWndPlayThumb_Panel6)
CTRL_LIST_END

//----------------------UIFlowWndPlayThumb_Panel6Ctrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlayThumb_Panel6)
EVENT_END

//---------------------UIFlowWndPlayThumb_Panel7Ctrl Control List---------------------------
CTRL_LIST_BEGIN(UIFlowWndPlayThumb_Panel7)
CTRL_LIST_END

//----------------------UIFlowWndPlayThumb_Panel7Ctrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlayThumb_Panel7)
EVENT_END

//---------------------UIFlowWndPlayThumb_Panel8Ctrl Control List---------------------------
CTRL_LIST_BEGIN(UIFlowWndPlayThumb_Panel8)
CTRL_LIST_END

//----------------------UIFlowWndPlayThumb_Panel8Ctrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlayThumb_Panel8)
EVENT_END

//----------------------UIFlowWndPlayThumb_StaticICN_Protect0Ctrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlayThumb_StaticICN_Protect0)
EVENT_END

//----------------------UIFlowWndPlayThumb_StaticICN_Protect1Ctrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlayThumb_StaticICN_Protect1)
EVENT_END

//----------------------UIFlowWndPlayThumb_StaticICN_Protect2Ctrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlayThumb_StaticICN_Protect2)
EVENT_END

//----------------------UIFlowWndPlayThumb_StaticICN_Protect3Ctrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlayThumb_StaticICN_Protect3)
EVENT_END

//----------------------UIFlowWndPlayThumb_StaticICN_Protect4Ctrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlayThumb_StaticICN_Protect4)
EVENT_END

//----------------------UIFlowWndPlayThumb_StaticICN_Protect5Ctrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlayThumb_StaticICN_Protect5)
EVENT_END

//----------------------UIFlowWndPlayThumb_StaticICN_Protect6Ctrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlayThumb_StaticICN_Protect6)
EVENT_END

//----------------------UIFlowWndPlayThumb_StaticICN_Protect7Ctrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlayThumb_StaticICN_Protect7)
EVENT_END

//----------------------UIFlowWndPlayThumb_StaticICN_Protect8Ctrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlayThumb_StaticICN_Protect8)
EVENT_END

//----------------------UIFlowWndPlayThumb_StaticICN_FileFilm0Ctrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlayThumb_StaticICN_FileFilm0)
EVENT_END

//----------------------UIFlowWndPlayThumb_StaticICN_FileFilm1Ctrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlayThumb_StaticICN_FileFilm1)
EVENT_END

//----------------------UIFlowWndPlayThumb_StaticICN_FileFilm2Ctrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlayThumb_StaticICN_FileFilm2)
EVENT_END

//----------------------UIFlowWndPlayThumb_StaticICN_FileFilm3Ctrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlayThumb_StaticICN_FileFilm3)
EVENT_END

//----------------------UIFlowWndPlayThumb_StaticICN_FileFilm4Ctrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlayThumb_StaticICN_FileFilm4)
EVENT_END

//----------------------UIFlowWndPlayThumb_StaticICN_FileFilm5Ctrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlayThumb_StaticICN_FileFilm5)
EVENT_END

//----------------------UIFlowWndPlayThumb_StaticICN_FileFilm6Ctrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlayThumb_StaticICN_FileFilm6)
EVENT_END

//----------------------UIFlowWndPlayThumb_StaticICN_FileFilm7Ctrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlayThumb_StaticICN_FileFilm7)
EVENT_END

//----------------------UIFlowWndPlayThumb_StaticICN_FileFilm8Ctrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlayThumb_StaticICN_FileFilm8)
EVENT_END

//----------------------UIFlowWndPlayThumb_ThumbIDCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndPlayThumb_ThumbID)
EVENT_END

