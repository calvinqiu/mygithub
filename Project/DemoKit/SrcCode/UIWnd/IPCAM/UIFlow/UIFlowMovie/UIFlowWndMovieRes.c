//This source code is generated by UI Designer Studio.

#ifndef UIFLOWWNDMOVIERES_H
#define UIFLOWWNDMOVIERES_H

#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIGraphics.h"
#include "NVTToolCommand.h"
#include "UIDisplayUtil.h"
//------------------------------------------------------------

SHOW_ITEM_RECT(CMD_Rectangle,Skin1Rect,0,0,319,239,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin1)
SHOW_MAP_BODY(Skin1Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin1,0,0,319,239)

#if 0
SHOW_MAP_BEGIN(Skin2)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin2,0,0,319,239)

SHOW_MAP_BEGIN(Skin4)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin4,0,0,29,29)

SHOW_ITEM_IMAGE(CMD_Image|CMD_ITEM,Icon5, 1, 1, 11,15, ICONID_NULL,83886080,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Normal3)
SHOW_MAP_BODY(Skin4)
SHOW_MAP_BODY(Icon5)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal3,12,5,41,34)

SHOW_MAP_BEGIN(UIFlowWndMovie_Static_camera)
SHOW_MAP_BODY(Normal3)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndMovie_Static_camera)
CREATE_STATIC_DATA(UIFlowWndMovie_Static_camera,ICON_MODE_VIDEO)
CREATE_STATIC_CTRL(UIFlowWndMovie_Static_camera,0,12,5,41,34)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle,Skin8Rect,0,0,27,27,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin8)
SHOW_MAP_BODY(Skin8Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin8,0,0,27,27)

SHOW_ITEM_IMAGE(CMD_Image|CMD_ITEM,Icon9, 0, 0, 27,27, ICONID_NULL,83886080,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Normal7)
SHOW_MAP_BODY(Skin8)
SHOW_MAP_BODY(Icon9)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal7,12,38,39,65)

CREATE_STATE_ITEM_DATA(ICONID_NULL10,STRID_NULL,ICONID_NULL,NULL) 
CREATE_STATE_ITEM_DATA(ICON_CYCLIC_REC_3MIN11,STRID_NULL,ICON_CYCLIC_REC_3MIN,NULL) 
CREATE_STATE_ITEM_DATA(ICON_CYCLIC_REC_5MIN12,STRID_NULL,ICON_CYCLIC_REC_5MIN,NULL) 
CREATE_STATE_ITEM_DATA(ICON_CYCLIC_REC_10MIN13,STRID_NULL,ICON_CYCLIC_REC_10MIN,NULL) 
SHOW_MAP_BEGIN(UIFlowWndMovie_Status_CyclicRec)
SHOW_MAP_BODY(Normal7)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndMovie_Status_CyclicRec)
STATE_LIST_BEGIN(UIFlowWndMovie_Status_CyclicRec)
STATE_LIST_ITEM(ICONID_NULL10)
STATE_LIST_ITEM(ICON_CYCLIC_REC_3MIN11)
STATE_LIST_ITEM(ICON_CYCLIC_REC_5MIN12)
STATE_LIST_ITEM(ICON_CYCLIC_REC_10MIN13)
STATE_LIST_END

CREATE_STATE_DATA(UIFlowWndMovie_Status_CyclicRec,0)
CREATE_STATE_CTRL(UIFlowWndMovie_Status_CyclicRec,0,12,38,39,65)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle,Skin15Rect,0,0,27,27,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin15)
SHOW_MAP_BODY(Skin15Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin15,0,0,27,27)

SHOW_ITEM_IMAGE(CMD_Image|CMD_ITEM,Icon16, 0, 0, 27,27, ICONID_NULL,83886080,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Normal14)
SHOW_MAP_BODY(Skin15)
SHOW_MAP_BODY(Icon16)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal14,12,71,39,98)

CREATE_STATE_ITEM_DATA(ICONID_NULL17,STRID_NULL,ICONID_NULL,NULL) 
CREATE_STATE_ITEM_DATA(ICON_MOTION_DET_ON18,STRID_NULL,ICON_MOTION_DET_ON,NULL) 
SHOW_MAP_BEGIN(UIFlowWndMovie_Status_MotionDet)
SHOW_MAP_BODY(Normal14)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndMovie_Status_MotionDet)
STATE_LIST_BEGIN(UIFlowWndMovie_Status_MotionDet)
STATE_LIST_ITEM(ICONID_NULL17)
STATE_LIST_ITEM(ICON_MOTION_DET_ON18)
STATE_LIST_END

CREATE_STATE_DATA(UIFlowWndMovie_Status_MotionDet,0)
CREATE_STATE_CTRL(UIFlowWndMovie_Status_MotionDet,0,12,71,39,98)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle,Skin20Rect,0,0,27,27,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin20)
SHOW_MAP_BODY(Skin20Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin20,0,0,27,27)

SHOW_ITEM_IMAGE(CMD_Image|CMD_ITEM,Icon21, 0, -1, 27,26, ICONID_NULL,0,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Normal19)
SHOW_MAP_BODY(Skin20)
SHOW_MAP_BODY(Icon21)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal19,280,208,307,235)

CREATE_STATE_ITEM_DATA(ICON_BATTERY_FULL22,STRID_NULL,ICON_BATTERY_FULL,NULL) 
CREATE_STATE_ITEM_DATA(ICON_BATTERY_MED23,STRID_NULL,ICON_BATTERY_MED,NULL) 
CREATE_STATE_ITEM_DATA(ICON_BATTERY_LOW24,STRID_NULL,ICON_BATTERY_LOW,NULL) 
CREATE_STATE_ITEM_DATA(ICON_BATTERY_EMPTY25,STRID_NULL,ICON_BATTERY_EMPTY,NULL) 
CREATE_STATE_ITEM_DATA(ICON_BATTERY_ZERO26,STRID_NULL,ICON_BATTERY_ZERO,NULL) 
CREATE_STATE_ITEM_DATA(ICON_BATTERY_CHARGE27,STRID_NULL,ICON_BATTERY_CHARGE,NULL) 
SHOW_MAP_BEGIN(UIFlowWndMovie_Status_battery)
SHOW_MAP_BODY(Normal19)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndMovie_Status_battery)
STATE_LIST_BEGIN(UIFlowWndMovie_Status_battery)
STATE_LIST_ITEM(ICON_BATTERY_FULL22)
STATE_LIST_ITEM(ICON_BATTERY_MED23)
STATE_LIST_ITEM(ICON_BATTERY_LOW24)
STATE_LIST_ITEM(ICON_BATTERY_EMPTY25)
STATE_LIST_ITEM(ICON_BATTERY_ZERO26)
STATE_LIST_ITEM(ICON_BATTERY_CHARGE27)
STATE_LIST_END

CREATE_STATE_DATA(UIFlowWndMovie_Status_battery,0)
CREATE_STATE_CTRL(UIFlowWndMovie_Status_battery,0,280,208,307,235)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle,Skin29Rect,0,0,120,20,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin29)
SHOW_MAP_BODY(Skin29Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin29,0,0,120,20)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text31,2,-1,119,19,STRID_NULL,0,1280,0,0,2,1,1,0,0,16,0,0,0,0)
SHOW_MAP_BEGIN(Normal28)
SHOW_MAP_BODY(Skin29)
SHOW_MAP_BODY(Text31)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal28,56,214,176,234)

SHOW_MAP_BEGIN(UIFlowWndMovie_YMD_Static)
SHOW_MAP_BODY(Normal28)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndMovie_YMD_Static)
CREATE_STATIC_DATA(UIFlowWndMovie_YMD_Static,STRID_MODE)
CREATE_STATIC_CTRL(UIFlowWndMovie_YMD_Static,0,56,214,176,234)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle,Skin34Rect,0,0,86,20,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin34)
SHOW_MAP_BODY(Skin34Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin34,0,0,86,20)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text36,1,2,84,18,STRID_NULL,0,1280,0,0,2,1,1,0,0,16,0,0,0,0)
SHOW_MAP_BEGIN(Normal33)
SHOW_MAP_BODY(Skin34)
SHOW_MAP_BODY(Text36)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal33,182,214,268,234)

SHOW_MAP_BEGIN(UIFlowWndMovie_HMS_Static)
SHOW_MAP_BODY(Normal33)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndMovie_HMS_Static)
CREATE_STATIC_DATA(UIFlowWndMovie_HMS_Static,STRID_MODE)
CREATE_STATIC_CTRL(UIFlowWndMovie_HMS_Static,0,182,214,268,234)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_MAP_BEGIN(Skin39)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin39,0,0,17,17)

SHOW_ITEM_IMAGE(CMD_Image|CMD_ITEM,Icon40, 0, 0, 17,17, ICONID_NULL,0,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Normal38)
SHOW_MAP_BODY(Skin39)
SHOW_MAP_BODY(Icon40)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal38,52,11,69,28)

CREATE_STATE_ITEM_DATA(ICON_REC_TRANSPAENT41,STRID_NULL,ICON_REC_TRANSPAENT,NULL) 
CREATE_STATE_ITEM_DATA(ICON_REC_ELLIPSE42,STRID_NULL,ICON_REC_ELLIPSE,NULL) 
SHOW_MAP_BEGIN(UIFlowWndMovie_Status_REC)
SHOW_MAP_BODY(Normal38)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndMovie_Status_REC)
STATE_LIST_BEGIN(UIFlowWndMovie_Status_REC)
STATE_LIST_ITEM(ICON_REC_TRANSPAENT41)
STATE_LIST_ITEM(ICON_REC_ELLIPSE42)
STATE_LIST_END

CREATE_STATE_DATA(UIFlowWndMovie_Status_REC,0)
CREATE_STATE_CTRL(UIFlowWndMovie_Status_REC,0,52,11,69,28)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle,Skin44Rect,0,0,27,27,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin44)
SHOW_MAP_BODY(Skin44Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin44,0,0,27,27)

SHOW_ITEM_IMAGE(CMD_Image|CMD_ITEM,Icon45, 0, 0, 27,27, ICONID_NULL,83886080,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Normal43)
SHOW_MAP_BODY(Skin44)
SHOW_MAP_BODY(Icon45)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal43,280,170,307,197)

CREATE_STATE_ITEM_DATA(ICON_INTERNAL_FLASH46,STRID_NULL,ICON_INTERNAL_FLASH,NULL) 
CREATE_STATE_ITEM_DATA(ICON_SD_CARD47,STRID_NULL,ICON_SD_CARD,NULL) 
CREATE_STATE_ITEM_DATA(ICON_SD_LOCK48,STRID_NULL,ICON_SD_LOCK,NULL) 
SHOW_MAP_BEGIN(UIFlowWndMovie_Status_Storage)
SHOW_MAP_BODY(Normal43)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndMovie_Status_Storage)
STATE_LIST_BEGIN(UIFlowWndMovie_Status_Storage)
STATE_LIST_ITEM(ICON_INTERNAL_FLASH46)
STATE_LIST_ITEM(ICON_SD_CARD47)
STATE_LIST_ITEM(ICON_SD_LOCK48)
STATE_LIST_END

CREATE_STATE_DATA(UIFlowWndMovie_Status_Storage,0)
CREATE_STATE_CTRL(UIFlowWndMovie_Status_Storage,0,280,170,307,197)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle,Skin50Rect,0,0,81,29,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin50)
SHOW_MAP_BODY(Skin50Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin50,0,0,81,29)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text52,1,0,80,29,STRID_NULL,0,1280,0,0,2,1,0,0,0,16,0,0,0,0)
SHOW_MAP_BEGIN(Normal49)
SHOW_MAP_BODY(Skin50)
SHOW_MAP_BODY(Text52)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal49,227,5,308,34)

SHOW_MAP_BEGIN(UIFlowWndMovie_Static_time)
SHOW_MAP_BODY(Normal49)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndMovie_Static_time)
CREATE_STATIC_DATA(UIFlowWndMovie_Static_time,STRID_MODE)
CREATE_STATIC_CTRL(UIFlowWndMovie_Static_time,0,227,5,308,34)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle,Skin55Rect,0,0,89,29,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin55)
SHOW_MAP_BODY(Skin55Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin55,0,0,89,29)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text57,1,0,90,29,STRID_NULL,0,1280,0,0,2,1,1,0,0,16,0,0,0,0)
SHOW_MAP_BEGIN(Normal54)
SHOW_MAP_BODY(Skin55)
SHOW_MAP_BODY(Text57)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal54,227,5,316,34)

SHOW_MAP_BEGIN(UIFlowWndMovie_Static_maxtime)
SHOW_MAP_BODY(Normal54)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndMovie_Static_maxtime)
CREATE_STATIC_DATA(UIFlowWndMovie_Static_maxtime,STRID_MODE)
CREATE_STATIC_CTRL(UIFlowWndMovie_Static_maxtime,0,227,5,316,34)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle,Skin60Rect,0,0,107,29,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin60)
SHOW_MAP_BODY(Skin60Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin60,0,0,107,29)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text62,1,0,106,29,STRID_NULL,0,1280,0,0,2,1,0,0,0,18,0,0,0,0)
SHOW_MAP_BEGIN(Normal59)
SHOW_MAP_BODY(Skin60)
SHOW_MAP_BODY(Text62)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal59,201,40,308,69)

SHOW_MAP_BEGIN(UIFlowWndMovie_Static_resolution)
SHOW_MAP_BODY(Normal59)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndMovie_Static_resolution)
CREATE_STATIC_DATA(UIFlowWndMovie_Static_resolution,STRID_MODE)
CREATE_STATIC_CTRL(UIFlowWndMovie_Static_resolution,0,201,40,308,69)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle,Skin65Rect,0,0,59,27,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin65)
SHOW_MAP_BODY(Skin65Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin65,0,0,59,27)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text67,0,0,59,27,STRID_NULL,0,1280,0,0,2,1,1,0,0,16,0,0,0,0)
SHOW_MAP_BEGIN(Normal64)
SHOW_MAP_BODY(Skin65)
SHOW_MAP_BODY(Text67)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal64,76,168,135,195)

SHOW_MAP_BEGIN(UIFlowWndMovie_Zoom_Static)
SHOW_MAP_BODY(Normal64)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndMovie_Zoom_Static)
CREATE_STATIC_DATA(UIFlowWndMovie_Zoom_Static,STRID_MODE)
CREATE_STATIC_CTRL(UIFlowWndMovie_Zoom_Static,0,76,168,135,195)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle,Skin70Rect,0,0,27,27,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin70)
SHOW_MAP_BODY(Skin70Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin70,0,0,27,27)

SHOW_ITEM_IMAGE(CMD_Image|CMD_ITEM,Icon71, 0, 0, 27,27, ICONID_NULL,0,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Normal69)
SHOW_MAP_BODY(Skin70)
SHOW_MAP_BODY(Icon71)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal69,12,104,39,131)

CREATE_STATE_ITEM_DATA(ICON_EV_P2P072,STRID_NULL,ICON_EV_P2P0,NULL) 
CREATE_STATE_ITEM_DATA(ICON_EV_P1P673,STRID_NULL,ICON_EV_P1P6,NULL) 
CREATE_STATE_ITEM_DATA(ICON_EV_P1P374,STRID_NULL,ICON_EV_P1P3,NULL) 
CREATE_STATE_ITEM_DATA(ICON_EV_P1P075,STRID_NULL,ICON_EV_P1P0,NULL) 
CREATE_STATE_ITEM_DATA(ICON_EV_P0P676,STRID_NULL,ICON_EV_P0P6,NULL) 
CREATE_STATE_ITEM_DATA(ICON_EV_P0P377,STRID_NULL,ICON_EV_P0P3,NULL) 
CREATE_STATE_ITEM_DATA(ICON_EV_P0P078,STRID_NULL,ICON_EV_P0P0,NULL) 
CREATE_STATE_ITEM_DATA(ICON_EV_M0P379,STRID_NULL,ICON_EV_M0P3,NULL) 
CREATE_STATE_ITEM_DATA(ICON_EV_M0P680,STRID_NULL,ICON_EV_M0P6,NULL) 
CREATE_STATE_ITEM_DATA(ICON_EV_M1P081,STRID_NULL,ICON_EV_M1P0,NULL) 
CREATE_STATE_ITEM_DATA(ICON_EV_M1P382,STRID_NULL,ICON_EV_M1P3,NULL) 
CREATE_STATE_ITEM_DATA(ICON_EV_M1P683,STRID_NULL,ICON_EV_M1P6,NULL) 
CREATE_STATE_ITEM_DATA(ICON_EV_M2P084,STRID_NULL,ICON_EV_M2P0,NULL) 
SHOW_MAP_BEGIN(UIFlowWndMovie_StatusICN_EV)
SHOW_MAP_BODY(Normal69)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndMovie_StatusICN_EV)
STATE_LIST_BEGIN(UIFlowWndMovie_StatusICN_EV)
STATE_LIST_ITEM(ICON_EV_P2P072)
STATE_LIST_ITEM(ICON_EV_P1P673)
STATE_LIST_ITEM(ICON_EV_P1P374)
STATE_LIST_ITEM(ICON_EV_P1P075)
STATE_LIST_ITEM(ICON_EV_P0P676)
STATE_LIST_ITEM(ICON_EV_P0P377)
STATE_LIST_ITEM(ICON_EV_P0P078)
STATE_LIST_ITEM(ICON_EV_M0P379)
STATE_LIST_ITEM(ICON_EV_M0P680)
STATE_LIST_ITEM(ICON_EV_M1P081)
STATE_LIST_ITEM(ICON_EV_M1P382)
STATE_LIST_ITEM(ICON_EV_M1P683)
STATE_LIST_ITEM(ICON_EV_M2P084)
STATE_LIST_END

CREATE_STATE_DATA(UIFlowWndMovie_StatusICN_EV,0)
CREATE_STATE_CTRL(UIFlowWndMovie_StatusICN_EV,0,12,104,39,131)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle,Skin86Rect,0,0,27,27,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin86)
SHOW_MAP_BODY(Skin86Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin86,0,0,27,27)

SHOW_ITEM_IMAGE(CMD_Image|CMD_ITEM,Icon87, 0, 0, 27,27, ICONID_NULL,83886080,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Normal85)
SHOW_MAP_BODY(Skin86)
SHOW_MAP_BODY(Icon87)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal85,80,5,107,32)

CREATE_STATE_ITEM_DATA(ICONID_NULL88,STRID_NULL,ICONID_NULL,NULL) 
CREATE_STATE_ITEM_DATA(ICON_HDR89,STRID_NULL,ICON_HDR,NULL) 
SHOW_MAP_BEGIN(UIFlowWndMovie_Status_HDR)
SHOW_MAP_BODY(Normal85)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndMovie_Status_HDR)
STATE_LIST_BEGIN(UIFlowWndMovie_Status_HDR)
STATE_LIST_ITEM(ICONID_NULL88)
STATE_LIST_ITEM(ICON_HDR89)
STATE_LIST_END

CREATE_STATE_DATA(UIFlowWndMovie_Status_HDR,0)
CREATE_STATE_CTRL(UIFlowWndMovie_Status_HDR,0,80,5,107,32)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle,Skin91Rect,0,0,27,27,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin91)
SHOW_MAP_BODY(Skin91Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin91,0,0,27,27)

SHOW_ITEM_IMAGE(CMD_Image|CMD_ITEM,Icon92, 0, 0, 27,27, ICONID_NULL,83886080,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Normal90)
SHOW_MAP_BODY(Skin91)
SHOW_MAP_BODY(Icon92)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal90,115,5,142,32)

CREATE_STATE_ITEM_DATA(ICON_WIFI_OFF93,STRID_NULL,ICON_WIFI_OFF,NULL) 
CREATE_STATE_ITEM_DATA(ICON_WIFI_ON94,STRID_NULL,ICON_WIFI_ON,NULL) 
CREATE_STATE_ITEM_DATA(ICON_CLOUD_ON95,STRID_NULL,ICON_CLOUD_ON,NULL) 
SHOW_MAP_BEGIN(UIFlowWndMovie_Status_WIFI)
SHOW_MAP_BODY(Normal90)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndMovie_Status_WIFI)
STATE_LIST_BEGIN(UIFlowWndMovie_Status_WIFI)
STATE_LIST_ITEM(ICON_WIFI_OFF93)
STATE_LIST_ITEM(ICON_WIFI_ON94)
STATE_LIST_ITEM(ICON_CLOUD_ON95)
STATE_LIST_END

CREATE_STATE_DATA(UIFlowWndMovie_Status_WIFI,0)
CREATE_STATE_CTRL(UIFlowWndMovie_Status_WIFI,0,115,5,142,32)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_RECT(CMD_Rectangle,Skin97Rect,0,0,27,27,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin97)
SHOW_MAP_BODY(Skin97Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin97,0,0,27,27)

SHOW_ITEM_IMAGE(CMD_Image|CMD_ITEM,Icon98, 1, 1, 28,28, ICONID_NULL,83886080,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Normal96)
SHOW_MAP_BODY(Skin97)
SHOW_MAP_BODY(Icon98)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal96,280,132,307,159)

SHOW_MAP_BEGIN(UIFlowWndMovie_StaticIcon_PIMC)
SHOW_MAP_BODY(Normal96)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndMovie_StaticIcon_PIMC)
CREATE_STATIC_DATA(UIFlowWndMovie_StaticIcon_PIMC,ICON_PIM_ON)
CREATE_STATIC_CTRL(UIFlowWndMovie_StaticIcon_PIMC,0,280,132,307,159)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_MAP_BEGIN(Skin100)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin100,0,0,319,239)

SHOW_MAP_BEGIN(UIFlowWndMovie_PNL_FDFrame)
SHOW_MAP_BODY(Skin100)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndMovie_PNL_FDFrame)
DECLARE_CTRL_LIST(UIFlowWndMovie_PNL_FDFrame)
CREATE_CTRL(UIFlowWndMovie_PNL_FDFrame,UIFlowWndMovie_PNL_FDFrame,CTRL_WND,NULL,0 ,0,0,319,239)
SHOW_MAP_BEGIN(UIFlowWndMovie_Panel_Normal_Display)
SHOW_MAP_BODY(Skin2)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndMovie_Panel_Normal_Display)
DECLARE_CTRL_LIST(UIFlowWndMovie_Panel_Normal_Display)
CREATE_CTRL(UIFlowWndMovie_Panel_Normal_Display,UIFlowWndMovie_Panel_Normal_Display,CTRL_WND,NULL,0 ,0,0,319,239)
SHOW_MAP_BEGIN(Skin101)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin101,0,0,319,239)

SHOW_MAP_BEGIN(UIFlowWndMovie_ADAS_DrawingLine)
SHOW_MAP_BODY(Skin101)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndMovie_ADAS_DrawingLine)
DECLARE_CTRL_LIST(UIFlowWndMovie_ADAS_DrawingLine)
CREATE_CTRL(UIFlowWndMovie_ADAS_DrawingLine,UIFlowWndMovie_ADAS_DrawingLine,CTRL_WND,NULL,0 ,0,0,319,239)
SHOW_ITEM_RECT(CMD_Rectangle,Skin102Rect,0,0,319,239,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin102)
SHOW_MAP_BODY(Skin102Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin102,0,0,319,239)

SHOW_MAP_BEGIN(Skin104)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin104,0,0,319,239)

SHOW_ITEM_IMAGE(CMD_Image|CMD_ITEM,Icon105, 0, 0, 319,239, ICONID_NULL,0,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Normal103)
SHOW_MAP_BODY(Skin104)
SHOW_MAP_BODY(Icon105)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal103,0,0,319,239)

SHOW_MAP_BEGIN(UIFlowWndMovie_StaticICN_LDWS_Alert)
SHOW_MAP_BODY(Normal103)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndMovie_StaticICN_LDWS_Alert)
CREATE_STATIC_DATA(UIFlowWndMovie_StaticICN_LDWS_Alert,ICON_LDWS_ALERT)
CREATE_STATIC_CTRL(UIFlowWndMovie_StaticICN_LDWS_Alert,0,0,0,319,239)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_MAP_BEGIN(Skin108)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin108,0,0,319,239)

SHOW_ITEM_IMAGE(CMD_Image|CMD_ITEM,Icon109, 0, 0, 319,239, ICONID_NULL,0,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Normal107)
SHOW_MAP_BODY(Skin108)
SHOW_MAP_BODY(Icon109)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal107,0,0,319,239)

SHOW_MAP_BEGIN(UIFlowWndMovie_StaticICN_FCWS_Alert)
SHOW_MAP_BODY(Normal107)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndMovie_StaticICN_FCWS_Alert)
CREATE_STATIC_DATA(UIFlowWndMovie_StaticICN_FCWS_Alert,ICON_FCW_ALERT)
CREATE_STATIC_CTRL(UIFlowWndMovie_StaticICN_FCWS_Alert,0,0,0,319,239)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_MAP_BEGIN(UIFlowWndMovie_ADAS_Alert_Display)
SHOW_MAP_BODY(Skin102)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndMovie_ADAS_Alert_Display)
DECLARE_CTRL_LIST(UIFlowWndMovie_ADAS_Alert_Display)
CREATE_CTRL(UIFlowWndMovie_ADAS_Alert_Display,UIFlowWndMovie_ADAS_Alert_Display,CTRL_WND,NULL,0 ,0,0,319,239)
#endif
SHOW_MAP_BEGIN(UIFlowWndMovie)
SHOW_MAP_BODY(Skin1)
SHOW_MAP_END

DECLARE_EVENT(UIFlowWndMovie)
DECLARE_CTRL_LIST(UIFlowWndMovie)
CREATE_CTRL(UIFlowWndMovie,UIFlowWndMovie,CTRL_WND,NULL,0 ,0,0,319,239)
#endif
