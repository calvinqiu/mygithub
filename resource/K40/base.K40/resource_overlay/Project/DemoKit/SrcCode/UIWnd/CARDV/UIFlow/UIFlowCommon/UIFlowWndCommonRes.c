//This source code is generated by UI Designer Studio.

#ifndef UIFLOWWNDCOMMONRES_H
#define UIFLOWWNDCOMMONRES_H

#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIGraphics.h"
#include "NVTToolCommand.h"
#include "UIDisplayUtil.h"
//------------------------------------------------------------

SHOW_MAP_BEGIN(Skin1)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin1,0,0,799,479)

SHOW_ITEM_RECT(CMD_Rectangle,Skin2Rect,0,0,399,239,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin2)
SHOW_MAP_BODY(Skin2Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin2,0,0,399,239)

SHOW_MAP_BEGIN(UIFlowWndCommon_TabHistogram)
SHOW_MAP_BODY(Skin2)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndCommon_TabHistogram)
CTRL_LIST_BEGIN(UIFlowWndCommon_TabHistogram)
CTRL_LIST_END

CREATE_TAB_DATA(UIFlowWndCommon_TabHistogram,0)

CREATE_TAB_CTRL(UIFlowWndCommon_TabHistogram,UIFlowWndCommon_TabHistogram,0,200,120,599,359)
SHOW_MAP_BEGIN(UIFlowWndCommon)
SHOW_MAP_BODY(Skin1)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndCommon)
DECLARE_CTRL_LIST(UIFlowWndCommon)
CREATE_CTRL(UIFlowWndCommon,UIFlowWndCommon,CTRL_WND,NULL,0 ,0,0,799,479)
#endif
