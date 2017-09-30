//This source code is generated by UI Designer Studio.

#ifndef UIFLOWWNDPHOTORES_H
#define UIFLOWWNDPHOTORES_H

#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIGraphics.h"
#include "NVTToolCommand.h"
#include "UIDisplayUtil.h"
//------------------------------------------------------------

SHOW_ITEM_RECT(CMD_Rectangle,Skin1Rect,0,0,1279,399,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin1)
SHOW_MAP_BODY(Skin1Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin1,0,0,1279,399)

SHOW_ITEM_RECT(CMD_Rectangle,Skin3Rect,0,0,27,27,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin3)
SHOW_MAP_BODY(Skin3Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin3,0,0,27,27)

SHOW_ITEM_IMAGE(CMD_Image|CMD_ITEM,Icon4, 0, 0, 27,27, ICONID_NULL,0,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Normal2)
SHOW_MAP_BODY(Skin3)
SHOW_MAP_BODY(Icon4)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal2,12,5,39,32)

SHOW_MAP_BEGIN(UIFlowWndPhoto_StaticICN_DSCMode)
SHOW_MAP_BODY(Normal2)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndPhoto_StaticICN_DSCMode)
CREATE_STATIC_DATA(UIFlowWndPhoto_StaticICN_DSCMode,ICON_MODE_CAPTURE)
CREATE_STATIC_CTRL(UIFlowWndPhoto_StaticICN_DSCMode,0,12,5,39,32)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle,Skin7Rect,0,0,27,27,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin7)
SHOW_MAP_BODY(Skin7Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin7,0,0,27,27)

SHOW_ITEM_IMAGE(CMD_Image|CMD_ITEM,Icon8, 0, 0, 27,27, ICONID_NULL,0,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Normal6)
SHOW_MAP_BODY(Skin7)
SHOW_MAP_BODY(Icon8)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal6,50,5,77,32)

CREATE_STATE_ITEM_DATA(ICONID_NULL9,STRID_NULL,ICONID_NULL,NULL) 
CREATE_STATE_ITEM_DATA(ICON_SELFTIMER_2SEC10,STRID_NULL,ICON_SELFTIMER_2SEC,NULL) 
CREATE_STATE_ITEM_DATA(ICON_SELFTIMER_5SEC11,STRID_NULL,ICON_SELFTIMER_5SEC,NULL) 
CREATE_STATE_ITEM_DATA(ICON_SELFTIMER_10SEC12,STRID_NULL,ICON_SELFTIMER_10SEC,NULL) 
SHOW_MAP_BEGIN(UIFlowWndPhoto_StatusICN_Selftimer)
SHOW_MAP_BODY(Normal6)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndPhoto_StatusICN_Selftimer)
STATE_LIST_BEGIN(UIFlowWndPhoto_StatusICN_Selftimer)
STATE_LIST_ITEM(ICONID_NULL9)
STATE_LIST_ITEM(ICON_SELFTIMER_2SEC10)
STATE_LIST_ITEM(ICON_SELFTIMER_5SEC11)
STATE_LIST_ITEM(ICON_SELFTIMER_10SEC12)
STATE_LIST_END

CREATE_STATE_DATA(UIFlowWndPhoto_StatusICN_Selftimer,0)
CREATE_STATE_CTRL(UIFlowWndPhoto_StatusICN_Selftimer,0,50,5,77,32)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle,Skin14Rect,0,0,107,27,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin14)
SHOW_MAP_BODY(Skin14Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin14,0,0,107,27)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text16,0,0,107,27,STRID_NULL,0,1280,0,0,2,1,1,0,0,18,0,0,0,0)
SHOW_MAP_BEGIN(Normal13)
SHOW_MAP_BODY(Skin14)
SHOW_MAP_BODY(Text16)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal13,200,5,307,32)

SHOW_MAP_BEGIN(UIFlowWndPhoto_StaticTXT_FreePic)
SHOW_MAP_BODY(Normal13)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndPhoto_StaticTXT_FreePic)
CREATE_STATIC_DATA(UIFlowWndPhoto_StaticTXT_FreePic,STRID_MODE)
CREATE_STATIC_CTRL(UIFlowWndPhoto_StaticTXT_FreePic,0,200,5,307,32)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle,Skin19Rect,0,0,27,27,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin19)
SHOW_MAP_BODY(Skin19Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin19,0,0,27,27)

SHOW_ITEM_IMAGE(CMD_Image|CMD_ITEM,Icon20, 0, 0, 27,27, ICONID_NULL,0,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Normal18)
SHOW_MAP_BODY(Skin19)
SHOW_MAP_BODY(Icon20)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal18,12,38,39,65)

CREATE_STATE_ITEM_DATA(ICON_EV_P2P021,STRID_NULL,ICON_EV_P2P0,NULL) 
CREATE_STATE_ITEM_DATA(ICON_EV_P1P622,STRID_NULL,ICON_EV_P1P6,NULL) 
CREATE_STATE_ITEM_DATA(ICON_EV_P1P323,STRID_NULL,ICON_EV_P1P3,NULL) 
CREATE_STATE_ITEM_DATA(ICON_EV_P1P024,STRID_NULL,ICON_EV_P1P0,NULL) 
CREATE_STATE_ITEM_DATA(ICON_EV_P0P625,STRID_NULL,ICON_EV_P0P6,NULL) 
CREATE_STATE_ITEM_DATA(ICON_EV_P0P326,STRID_NULL,ICON_EV_P0P3,NULL) 
CREATE_STATE_ITEM_DATA(ICON_EV_P0P027,STRID_NULL,ICON_EV_P0P0,NULL) 
CREATE_STATE_ITEM_DATA(ICON_EV_M0P328,STRID_NULL,ICON_EV_M0P3,NULL) 
CREATE_STATE_ITEM_DATA(ICON_EV_M0P629,STRID_NULL,ICON_EV_M0P6,NULL) 
CREATE_STATE_ITEM_DATA(ICON_EV_M1P030,STRID_NULL,ICON_EV_M1P0,NULL) 
CREATE_STATE_ITEM_DATA(ICON_EV_M1P331,STRID_NULL,ICON_EV_M1P3,NULL) 
CREATE_STATE_ITEM_DATA(ICON_EV_M1P632,STRID_NULL,ICON_EV_M1P6,NULL) 
CREATE_STATE_ITEM_DATA(ICON_EV_M2P033,STRID_NULL,ICON_EV_M2P0,NULL) 
SHOW_MAP_BEGIN(UIFlowWndPhoto_StatusICN_EV)
SHOW_MAP_BODY(Normal18)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndPhoto_StatusICN_EV)
STATE_LIST_BEGIN(UIFlowWndPhoto_StatusICN_EV)
STATE_LIST_ITEM(ICON_EV_P2P021)
STATE_LIST_ITEM(ICON_EV_P1P622)
STATE_LIST_ITEM(ICON_EV_P1P323)
STATE_LIST_ITEM(ICON_EV_P1P024)
STATE_LIST_ITEM(ICON_EV_P0P625)
STATE_LIST_ITEM(ICON_EV_P0P326)
STATE_LIST_ITEM(ICON_EV_P0P027)
STATE_LIST_ITEM(ICON_EV_M0P328)
STATE_LIST_ITEM(ICON_EV_M0P629)
STATE_LIST_ITEM(ICON_EV_M1P030)
STATE_LIST_ITEM(ICON_EV_M1P331)
STATE_LIST_ITEM(ICON_EV_M1P632)
STATE_LIST_ITEM(ICON_EV_M2P033)
STATE_LIST_END

CREATE_STATE_DATA(UIFlowWndPhoto_StatusICN_EV,0)
CREATE_STATE_CTRL(UIFlowWndPhoto_StatusICN_EV,0,12,38,39,65)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle,Skin35Rect,0,0,27,27,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin35)
SHOW_MAP_BODY(Skin35Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin35,0,0,27,27)

SHOW_ITEM_IMAGE(CMD_Image|CMD_ITEM,Icon36, 0, 0, 27,27, ICONID_NULL,0,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Normal34)
SHOW_MAP_BODY(Skin35)
SHOW_MAP_BODY(Icon36)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal34,12,106,39,133)

CREATE_STATE_ITEM_DATA(ICON_ISO_AUTO37,STRID_NULL,ICON_ISO_AUTO,NULL) 
CREATE_STATE_ITEM_DATA(ICON_ISO_10038,STRID_NULL,ICON_ISO_100,NULL) 
CREATE_STATE_ITEM_DATA(ICON_ISO_20039,STRID_NULL,ICON_ISO_200,NULL) 
CREATE_STATE_ITEM_DATA(ICON_ISO_40040,STRID_NULL,ICON_ISO_400,NULL) 
CREATE_STATE_ITEM_DATA(ICON_ISO_80041,STRID_NULL,ICON_ISO_800,NULL) 
CREATE_STATE_ITEM_DATA(ICON_ISO_160042,STRID_NULL,ICON_ISO_1600,NULL) 
SHOW_MAP_BEGIN(UIFlowWndPhoto_StatusICN_ISO)
SHOW_MAP_BODY(Normal34)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndPhoto_StatusICN_ISO)
STATE_LIST_BEGIN(UIFlowWndPhoto_StatusICN_ISO)
STATE_LIST_ITEM(ICON_ISO_AUTO37)
STATE_LIST_ITEM(ICON_ISO_10038)
STATE_LIST_ITEM(ICON_ISO_20039)
STATE_LIST_ITEM(ICON_ISO_40040)
STATE_LIST_ITEM(ICON_ISO_80041)
STATE_LIST_ITEM(ICON_ISO_160042)
STATE_LIST_END

CREATE_STATE_DATA(UIFlowWndPhoto_StatusICN_ISO,0)
CREATE_STATE_CTRL(UIFlowWndPhoto_StatusICN_ISO,0,12,106,39,133)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle,Skin44Rect,0,0,107,27,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin44)
SHOW_MAP_BODY(Skin44Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin44,0,0,107,27)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text46,0,0,107,27,STRID_NULL,0,1280,0,0,2,1,1,0,0,18,0,0,0,0)
SHOW_MAP_BEGIN(Normal43)
SHOW_MAP_BODY(Skin44)
SHOW_MAP_BODY(Text46)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal43,200,38,307,65)

SHOW_MAP_BEGIN(UIFlowWndPhoto_StaticTXT_Size)
SHOW_MAP_BODY(Normal43)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndPhoto_StaticTXT_Size)
CREATE_STATIC_DATA(UIFlowWndPhoto_StaticTXT_Size,STRID_MODE)
CREATE_STATIC_CTRL(UIFlowWndPhoto_StaticTXT_Size,0,200,38,307,65)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle,Skin49Rect,0,0,27,27,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin49)
SHOW_MAP_BODY(Skin49Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin49,0,0,27,27)

SHOW_ITEM_IMAGE(CMD_Image|CMD_ITEM,Icon50, 0, 0, 27,27, ICONID_NULL,0,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Normal48)
SHOW_MAP_BODY(Skin49)
SHOW_MAP_BODY(Icon50)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal48,88,5,115,32)

CREATE_STATE_ITEM_DATA(ICON_SHAKE_ON51,STRID_NULL,ICON_SHAKE_ON,NULL) 
CREATE_STATE_ITEM_DATA(ICON_SHAKE_OFF52,STRID_NULL,ICON_SHAKE_OFF,NULL) 
SHOW_MAP_BEGIN(UIFlowWndPhoto_StatusICN_AntiShaking)
SHOW_MAP_BODY(Normal48)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndPhoto_StatusICN_AntiShaking)
STATE_LIST_BEGIN(UIFlowWndPhoto_StatusICN_AntiShaking)
STATE_LIST_ITEM(ICON_SHAKE_ON51)
STATE_LIST_ITEM(ICON_SHAKE_OFF52)
STATE_LIST_END

CREATE_STATE_DATA(UIFlowWndPhoto_StatusICN_AntiShaking,0)
CREATE_STATE_CTRL(UIFlowWndPhoto_StatusICN_AntiShaking,0,88,5,115,32)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle,Skin54Rect,0,0,27,27,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin54)
SHOW_MAP_BODY(Skin54Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin54,0,0,27,27)

SHOW_ITEM_IMAGE(CMD_Image|CMD_ITEM,Icon55, 0, 0, 27,27, ICONID_NULL,0,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Normal53)
SHOW_MAP_BODY(Skin54)
SHOW_MAP_BODY(Icon55)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal53,12,72,39,99)

CREATE_STATE_ITEM_DATA(ICON_WB_AUTO56,STRID_NULL,ICON_WB_AUTO,NULL) 
CREATE_STATE_ITEM_DATA(ICON_WB_DAYLIGHT57,STRID_NULL,ICON_WB_DAYLIGHT,NULL) 
CREATE_STATE_ITEM_DATA(ICON_WB_CLOUDY58,STRID_NULL,ICON_WB_CLOUDY,NULL) 
CREATE_STATE_ITEM_DATA(ICON_WB_TUNGSTEN59,STRID_NULL,ICON_WB_TUNGSTEN,NULL) 
CREATE_STATE_ITEM_DATA(ICON_WB_FLUORESCENT60,STRID_NULL,ICON_WB_FLUORESCENT,NULL) 
SHOW_MAP_BEGIN(UIFlowWndPhoto_StatusICN_WB)
SHOW_MAP_BODY(Normal53)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndPhoto_StatusICN_WB)
STATE_LIST_BEGIN(UIFlowWndPhoto_StatusICN_WB)
STATE_LIST_ITEM(ICON_WB_AUTO56)
STATE_LIST_ITEM(ICON_WB_DAYLIGHT57)
STATE_LIST_ITEM(ICON_WB_CLOUDY58)
STATE_LIST_ITEM(ICON_WB_TUNGSTEN59)
STATE_LIST_ITEM(ICON_WB_FLUORESCENT60)
STATE_LIST_END

CREATE_STATE_DATA(UIFlowWndPhoto_StatusICN_WB,0)
CREATE_STATE_CTRL(UIFlowWndPhoto_StatusICN_WB,0,12,72,39,99)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle,Skin62Rect,0,0,27,27,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin62)
SHOW_MAP_BODY(Skin62Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin62,0,0,27,27)

SHOW_ITEM_IMAGE(CMD_Image|CMD_ITEM,Icon63, 0, 0, 27,27, ICONID_NULL,0,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Normal61)
SHOW_MAP_BODY(Skin62)
SHOW_MAP_BODY(Icon63)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal61,280,104,307,131)

CREATE_STATE_ITEM_DATA(ICON_FACE_ON64,STRID_NULL,ICON_FACE_ON,NULL) 
CREATE_STATE_ITEM_DATA(ICON_FACE_OFF65,STRID_NULL,ICON_FACE_OFF,NULL) 
SHOW_MAP_BEGIN(UIFlowWndPhoto_StatusICN_FD)
SHOW_MAP_BODY(Normal61)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndPhoto_StatusICN_FD)
STATE_LIST_BEGIN(UIFlowWndPhoto_StatusICN_FD)
STATE_LIST_ITEM(ICON_FACE_ON64)
STATE_LIST_ITEM(ICON_FACE_OFF65)
STATE_LIST_END

CREATE_STATE_DATA(UIFlowWndPhoto_StatusICN_FD,0)
CREATE_STATE_CTRL(UIFlowWndPhoto_StatusICN_FD,0,280,104,307,131)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle,Skin67Rect,0,0,27,27,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin67)
SHOW_MAP_BODY(Skin67Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin67,0,0,27,27)

SHOW_ITEM_IMAGE(CMD_Image|CMD_ITEM,Icon68, 0, 0, 27,27, ICONID_NULL,0,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Normal66)
SHOW_MAP_BODY(Skin67)
SHOW_MAP_BODY(Icon68)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal66,280,71,307,98)

CREATE_STATE_ITEM_DATA(ICON_QUALITY_FINE69,STRID_NULL,ICON_QUALITY_FINE,NULL) 
CREATE_STATE_ITEM_DATA(ICON_QUALITY_NORMAL70,STRID_NULL,ICON_QUALITY_NORMAL,NULL) 
CREATE_STATE_ITEM_DATA(ICON_QUALITY_BASIC71,STRID_NULL,ICON_QUALITY_BASIC,NULL) 
SHOW_MAP_BEGIN(UIFlowWndPhoto_StatusICN_Quality)
SHOW_MAP_BODY(Normal66)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndPhoto_StatusICN_Quality)
STATE_LIST_BEGIN(UIFlowWndPhoto_StatusICN_Quality)
STATE_LIST_ITEM(ICON_QUALITY_FINE69)
STATE_LIST_ITEM(ICON_QUALITY_NORMAL70)
STATE_LIST_ITEM(ICON_QUALITY_BASIC71)
STATE_LIST_END

CREATE_STATE_DATA(UIFlowWndPhoto_StatusICN_Quality,0)
CREATE_STATE_CTRL(UIFlowWndPhoto_StatusICN_Quality,0,280,71,307,98)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle,Skin73Rect,0,0,27,27,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin73)
SHOW_MAP_BODY(Skin73Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin73,0,0,27,27)

SHOW_ITEM_IMAGE(CMD_Image|CMD_ITEM,Icon74, 0, 0, 27,27, ICONID_NULL,0,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Normal72)
SHOW_MAP_BODY(Skin73)
SHOW_MAP_BODY(Icon74)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal72,280,170,307,197)

CREATE_STATE_ITEM_DATA(ICON_INTERNAL_FLASH75,STRID_NULL,ICON_INTERNAL_FLASH,NULL) 
CREATE_STATE_ITEM_DATA(ICON_SD_CARD76,STRID_NULL,ICON_SD_CARD,NULL) 
CREATE_STATE_ITEM_DATA(ICON_SD_LOCK77,STRID_NULL,ICON_SD_LOCK,NULL) 
SHOW_MAP_BEGIN(UIFlowWndPhoto_StatusICN_Storage)
SHOW_MAP_BODY(Normal72)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndPhoto_StatusICN_Storage)
STATE_LIST_BEGIN(UIFlowWndPhoto_StatusICN_Storage)
STATE_LIST_ITEM(ICON_INTERNAL_FLASH75)
STATE_LIST_ITEM(ICON_SD_CARD76)
STATE_LIST_ITEM(ICON_SD_LOCK77)
STATE_LIST_END

CREATE_STATE_DATA(UIFlowWndPhoto_StatusICN_Storage,0)
CREATE_STATE_CTRL(UIFlowWndPhoto_StatusICN_Storage,0,280,170,307,197)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle,Skin79Rect,0,0,59,27,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin79)
SHOW_MAP_BODY(Skin79Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin79,0,0,59,27)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text81,0,0,59,27,STRID_NULL,0,1280,0,0,2,1,1,0,0,16,0,0,0,0)
SHOW_MAP_BEGIN(Normal78)
SHOW_MAP_BODY(Skin79)
SHOW_MAP_BODY(Text81)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal78,41,207,100,234)

SHOW_MAP_BEGIN(UIFlowWndPhoto_StaticTXT_DZoom)
SHOW_MAP_BODY(Normal78)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndPhoto_StaticTXT_DZoom)
CREATE_STATIC_DATA(UIFlowWndPhoto_StaticTXT_DZoom,STRID_MODE)
CREATE_STATIC_CTRL(UIFlowWndPhoto_StaticTXT_DZoom,0,41,207,100,234)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle,Skin84Rect,0,0,27,27,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin84)
SHOW_MAP_BODY(Skin84Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin84,0,0,27,27)

SHOW_ITEM_IMAGE(CMD_Image|CMD_ITEM,Icon85, 0, 0, 31,27, ICONID_NULL,0,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Normal83)
SHOW_MAP_BODY(Skin84)
SHOW_MAP_BODY(Icon85)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal83,280,207,307,234)

CREATE_STATE_ITEM_DATA(ICON_BATTERY_FULL86,STRID_NULL,ICON_BATTERY_FULL,NULL) 
CREATE_STATE_ITEM_DATA(ICON_BATTERY_MED87,STRID_NULL,ICON_BATTERY_MED,NULL) 
CREATE_STATE_ITEM_DATA(ICON_BATTERY_LOW88,STRID_NULL,ICON_BATTERY_LOW,NULL) 
CREATE_STATE_ITEM_DATA(ICON_BATTERY_EMPTY89,STRID_NULL,ICON_BATTERY_EMPTY,NULL) 
CREATE_STATE_ITEM_DATA(ICON_BATTERY_ZERO90,STRID_NULL,ICON_BATTERY_ZERO,NULL) 
CREATE_STATE_ITEM_DATA(ICON_BATTERY_CHARGE91,STRID_NULL,ICON_BATTERY_CHARGE,NULL) 
SHOW_MAP_BEGIN(UIFlowWndPhoto_StatusICN_Battery)
SHOW_MAP_BODY(Normal83)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndPhoto_StatusICN_Battery)
STATE_LIST_BEGIN(UIFlowWndPhoto_StatusICN_Battery)
STATE_LIST_ITEM(ICON_BATTERY_FULL86)
STATE_LIST_ITEM(ICON_BATTERY_MED87)
STATE_LIST_ITEM(ICON_BATTERY_LOW88)
STATE_LIST_ITEM(ICON_BATTERY_EMPTY89)
STATE_LIST_ITEM(ICON_BATTERY_ZERO90)
STATE_LIST_ITEM(ICON_BATTERY_CHARGE91)
STATE_LIST_END

CREATE_STATE_DATA(UIFlowWndPhoto_StatusICN_Battery,0)
CREATE_STATE_CTRL(UIFlowWndPhoto_StatusICN_Battery,0,280,207,307,234)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle,Skin93Rect,0,0,27,27,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin93)
SHOW_MAP_BODY(Skin93Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin93,0,0,27,27)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text95,0,0,27,27,STRID_NULL,0,1280,0,0,2,1,1,0,0,16,0,0,0,0)
SHOW_MAP_BEGIN(Normal92)
SHOW_MAP_BODY(Skin93)
SHOW_MAP_BODY(Text95)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal92,50,5,77,32)

SHOW_MAP_BEGIN(UIFlowWndPhoto_StaticTXT_SelftimerCnt)
SHOW_MAP_BODY(Normal92)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndPhoto_StaticTXT_SelftimerCnt)
CREATE_STATIC_DATA(UIFlowWndPhoto_StaticTXT_SelftimerCnt,STRID_DUMMY)
CREATE_STATIC_CTRL(UIFlowWndPhoto_StaticTXT_SelftimerCnt,0,50,5,77,32)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle,Skin97Rect,0,0,127,49,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin97)
SHOW_MAP_BODY(Skin97Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin97,0,0,127,49)

SHOW_MAP_BEGIN(UIFlowWndPhoto_PNL_Histogram)
SHOW_MAP_BODY(Skin97)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndPhoto_PNL_Histogram)
DECLARE_CTRL_LIST(UIFlowWndPhoto_PNL_Histogram)
CREATE_CTRL(UIFlowWndPhoto_PNL_Histogram,UIFlowWndPhoto_PNL_Histogram,CTRL_WND,NULL,0 ,127,185,254,234)
SHOW_MAP_BEGIN(Skin98)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin98,0,0,319,239)

SHOW_MAP_BEGIN(UIFlowWndPhoto_PNL_FDFrame)
SHOW_MAP_BODY(Skin98)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndPhoto_PNL_FDFrame)
DECLARE_CTRL_LIST(UIFlowWndPhoto_PNL_FDFrame)
CREATE_CTRL(UIFlowWndPhoto_PNL_FDFrame,UIFlowWndPhoto_PNL_FDFrame,CTRL_WND,NULL,0 ,0,0,319,239)
SHOW_ITEM_RECT(CMD_Rectangle,Skin100Rect,0,0,27,27,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin100)
SHOW_MAP_BODY(Skin100Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin100,0,0,27,27)

SHOW_ITEM_IMAGE(CMD_Image|CMD_ITEM,Icon101, 0, 0, 27,27, ICONID_NULL,0,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Normal99)
SHOW_MAP_BODY(Skin100)
SHOW_MAP_BODY(Icon101)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal99,126,5,153,32)

CREATE_STATE_ITEM_DATA(ICON_BUST_OFF102,STRID_NULL,ICON_BUST_OFF,NULL) 
CREATE_STATE_ITEM_DATA(ICON_BUST_3SHOT103,STRID_NULL,ICON_BUST_3SHOT,NULL) 
CREATE_STATE_ITEM_DATA(ICON_BUST_CONTINUE104,STRID_NULL,ICON_BUST_CONTINUE,NULL) 
SHOW_MAP_BEGIN(UIFlowWndPhoto_StatusICN_ContinueShot)
SHOW_MAP_BODY(Normal99)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndPhoto_StatusICN_ContinueShot)
STATE_LIST_BEGIN(UIFlowWndPhoto_StatusICN_ContinueShot)
STATE_LIST_ITEM(ICON_BUST_OFF102)
STATE_LIST_ITEM(ICON_BUST_3SHOT103)
STATE_LIST_ITEM(ICON_BUST_CONTINUE104)
STATE_LIST_END

CREATE_STATE_DATA(UIFlowWndPhoto_StatusICN_ContinueShot,0)
CREATE_STATE_CTRL(UIFlowWndPhoto_StatusICN_ContinueShot,0,126,5,153,32)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle,Skin106Rect,0,0,29,29,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin106)
SHOW_MAP_BODY(Skin106Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin106,0,0,29,29)

SHOW_ITEM_IMAGE(CMD_Image|CMD_ITEM,Icon107, 0, 0, 29,29, ICONID_NULL,83886080,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Normal105)
SHOW_MAP_BODY(Skin106)
SHOW_MAP_BODY(Icon107)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal105,50,5,79,34)

CREATE_STATE_ITEM_DATA(ICON_HDR_OFF108,STRID_NULL,ICON_HDR_OFF,NULL) 
CREATE_STATE_ITEM_DATA(ICON_HDR109,STRID_NULL,ICON_HDR,NULL) 
SHOW_MAP_BEGIN(UIFlowWndPhoto_StatusICN_HDR)
SHOW_MAP_BODY(Normal105)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndPhoto_StatusICN_HDR)
STATE_LIST_BEGIN(UIFlowWndPhoto_StatusICN_HDR)
STATE_LIST_ITEM(ICON_HDR_OFF108)
STATE_LIST_ITEM(ICON_HDR109)
STATE_LIST_END

CREATE_STATE_DATA(UIFlowWndPhoto_StatusICN_HDR,0)
CREATE_STATE_CTRL(UIFlowWndPhoto_StatusICN_HDR,0,50,5,79,34)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_MAP_BEGIN(UIFlowWndPhoto)
SHOW_MAP_BODY(Skin1)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndPhoto)
DECLARE_CTRL_LIST(UIFlowWndPhoto)
CREATE_CTRL(UIFlowWndPhoto,UIFlowWndPhoto,CTRL_WND,NULL,0 ,0,0,1279,399)
#endif
