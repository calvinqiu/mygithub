//This source code is generated by UI Designer Studio.

#ifndef UIMENUWNDSETUPDATETIMERES_H
#define UIMENUWNDSETUPDATETIMERES_H

#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIGraphics.h"
#include "NVTToolCommand.h"
#include "UIDisplayUtil.h"
//------------------------------------------------------------

SHOW_MAP_BEGIN(Skin1)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin1,0,0,319,239)

SHOW_ITEM_RECT(CMD_Rectangle,Skin2Rect,0,0,299,189,0,0,14,14,0,0,0)
SHOW_MAP_BEGIN(Skin2)
SHOW_MAP_BODY(Skin2Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin2,0,0,299,189)

SHOW_MAP_BEGIN(Skin3)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin3,0,0,239,146)

SHOW_MAP_BEGIN(Skin5)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin5,0,0,20,29)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text7,1,0,20,29,STRID_NULL,0,0,0,0,2,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Normal4)
SHOW_MAP_BODY(Skin5)
SHOW_MAP_BODY(Text7)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal4,106,70,126,99)

SHOW_MAP_BEGIN(UIMenuWndSetupDateTime_YMD_VALUE_Other0)
SHOW_MAP_BODY(Normal4)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndSetupDateTime_YMD_VALUE_Other0)
CREATE_STATIC_DATA(UIMenuWndSetupDateTime_YMD_VALUE_Other0,STRID_L)
CREATE_STATIC_CTRL(UIMenuWndSetupDateTime_YMD_VALUE_Other0,0,98,30,118,59)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_MAP_BEGIN(Skin10)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin10,0,0,20,29)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text12,1,0,20,29,STRID_NULL,0,0,0,0,2,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Normal9)
SHOW_MAP_BODY(Skin10)
SHOW_MAP_BODY(Text12)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal9,161,70,181,99)

SHOW_MAP_BEGIN(UIMenuWndSetupDateTime_YMD_VALUE_Other1)
SHOW_MAP_BODY(Normal9)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndSetupDateTime_YMD_VALUE_Other1)
CREATE_STATIC_DATA(UIMenuWndSetupDateTime_YMD_VALUE_Other1,STRID_L)
CREATE_STATIC_CTRL(UIMenuWndSetupDateTime_YMD_VALUE_Other1,0,153,30,173,59)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_MAP_BEGIN(Skin15)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin15,0,0,20,29)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text17,1,0,20,29,STRID_NULL,0,0,0,0,2,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Normal14)
SHOW_MAP_BODY(Skin15)
SHOW_MAP_BODY(Text17)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal14,131,140,151,169)

SHOW_MAP_BEGIN(UIMenuWndSetupDateTime_YMD_VALUE_Other2)
SHOW_MAP_BODY(Normal14)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndSetupDateTime_YMD_VALUE_Other2)
CREATE_STATIC_DATA(UIMenuWndSetupDateTime_YMD_VALUE_Other2,STRID_L)
CREATE_STATIC_CTRL(UIMenuWndSetupDateTime_YMD_VALUE_Other2,0,123,100,143,129)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_MAP_BEGIN(UIMenuWndSetupDateTime_Tab_YMD_VALUE)
SHOW_MAP_BODY(Skin3)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndSetupDateTime_Tab_YMD_VALUE)
CTRL_LIST_BEGIN(UIMenuWndSetupDateTime_Tab_YMD_VALUE)
CTRL_LIST_ITEM(UIMenuWndSetupDateTime_YMD_VALUE_Other0)
CTRL_LIST_ITEM(UIMenuWndSetupDateTime_YMD_VALUE_Other1)
CTRL_LIST_ITEM(UIMenuWndSetupDateTime_YMD_VALUE_Other2)
CTRL_LIST_END

CREATE_TAB_DATA(UIMenuWndSetupDateTime_Tab_YMD_VALUE,0)

CREATE_TAB_CTRL(UIMenuWndSetupDateTime_Tab_YMD_VALUE,UIMenuWndSetupDateTime_Tab_YMD_VALUE,0,-2,0,237,146)
SHOW_MAP_BEGIN(Skin20)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin20,0,0,39,39)

SHOW_ITEM_IMAGE(CMD_Image|CMD_ITEM,Icon21, 6, 6, 33,33, ICONID_NULL,83886080,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Normal19)
SHOW_MAP_BODY(Skin20)
SHOW_MAP_BODY(Icon21)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal19,260,180,299,219)

SHOW_MAP_BEGIN(Skin23)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin23,0,0,39,39)

SHOW_ITEM_IMAGE(CMD_Image|CMD_ITEM,Icon24, 6, 6, 33,33, ICONID_NULL,83886080,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Focused22)
SHOW_MAP_BODY(Skin23)
SHOW_MAP_BODY(Icon24)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Focused22,260,180,299,219)

SHOW_MAP_BEGIN(Skin26)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin26,0,0,39,39)

SHOW_ITEM_IMAGE(CMD_Image|CMD_ITEM,Icon27, 6, 6, 33,33, ICONID_NULL,83886080,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Normal_Disable25)
SHOW_MAP_BODY(Skin26)
SHOW_MAP_BODY(Icon27)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal_Disable25,260,180,299,219)

SHOW_MAP_BEGIN(Skin29)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin29,0,0,39,39)

SHOW_ITEM_IMAGE(CMD_Image|CMD_ITEM,Icon30, 6, 6, 33,33, ICONID_NULL,83886080,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Focused_Disable28)
SHOW_MAP_BODY(Skin29)
SHOW_MAP_BODY(Icon30)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Focused_Disable28,260,180,299,219)

SHOW_ITEM_ROUNDRECT(CMD_RoundRect,RoundRect34,0,0,39,39,16,2,0,211,0,0,0,2,6)
SHOW_MAP_BEGIN(Skin32)
SHOW_MAP_BODY(RoundRect34)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin32,0,0,39,39)

SHOW_ITEM_IMAGE(CMD_Image|CMD_ITEM,Icon35, 7, 7, 34,34, ICONID_NULL,83886080,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Pressed31)
SHOW_MAP_BODY(Skin32)
SHOW_MAP_BODY(Icon35)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Pressed31,260,180,299,219)

CREATE_BUTTON_ITEM_DATA(ICON_OK236,STRID_NULL,ICON_OK2,STATUS_ENABLE,NULL) 
SHOW_MAP_BEGIN(UIMenuWndSetupDateTime_YMD_OK)
SHOW_MAP_BODY(Normal19)
SHOW_MAP_BODY(Focused22)
SHOW_MAP_BODY(Normal_Disable25)
SHOW_MAP_BODY(Focused_Disable28)
SHOW_MAP_BODY(Pressed31)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndSetupDateTime_YMD_OK)
BUTTON_LIST_BEGIN(UIMenuWndSetupDateTime_YMD_OK)
BUTTON_LIST_ITEM(ICON_OK236)
BUTTON_LIST_END

CREATE_BUTTON_DATA(UIMenuWndSetupDateTime_YMD_OK,BTN_DRAW_IMAGE)

CREATE_BUTTON_CTRL(UIMenuWndSetupDateTime_YMD_OK,UIMenuWndSetupDateTime_YMD_OK,0,250,140,289,179)
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_ROUNDRECT(CMD_RoundRect,RoundRect40,0,0,39,39,16,1,2,0,0,0,0,8,16)
SHOW_MAP_BEGIN(Skin38)
SHOW_MAP_BODY(RoundRect40)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin38,0,0,39,39)

SHOW_ITEM_IMAGE(CMD_Image|CMD_ITEM,Icon41, 6, 6, 33,33, ICONID_NULL,83886080,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Normal37)
SHOW_MAP_BODY(Skin38)
SHOW_MAP_BODY(Icon41)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal37,260,60,299,99)

SHOW_MAP_BEGIN(Skin43)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin43,0,0,39,39)

SHOW_ITEM_IMAGE(CMD_Image|CMD_ITEM,Icon44, 6, 6, 33,33, ICONID_NULL,83886080,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Focused42)
SHOW_MAP_BODY(Skin43)
SHOW_MAP_BODY(Icon44)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Focused42,260,60,299,99)

SHOW_MAP_BEGIN(Skin46)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin46,0,0,39,39)

SHOW_ITEM_IMAGE(CMD_Image|CMD_ITEM,Icon47, 6, 6, 33,33, ICONID_NULL,83886080,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Normal_Disable45)
SHOW_MAP_BODY(Skin46)
SHOW_MAP_BODY(Icon47)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal_Disable45,260,60,299,99)

SHOW_MAP_BEGIN(Skin49)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin49,0,0,39,39)

SHOW_ITEM_IMAGE(CMD_Image|CMD_ITEM,Icon50, 6, 6, 33,33, ICONID_NULL,83886080,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Focused_Disable48)
SHOW_MAP_BODY(Skin49)
SHOW_MAP_BODY(Icon50)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Focused_Disable48,260,60,299,99)

SHOW_ITEM_ROUNDRECT(CMD_RoundRect,RoundRect54,0,0,39,39,16,2,0,211,0,0,0,2,6)
SHOW_MAP_BEGIN(Skin52)
SHOW_MAP_BODY(RoundRect54)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin52,0,0,39,39)

SHOW_ITEM_IMAGE(CMD_Image|CMD_ITEM,Icon55, 7, 7, 34,34, ICONID_NULL,83886080,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Pressed51)
SHOW_MAP_BODY(Skin52)
SHOW_MAP_BODY(Icon55)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Pressed51,260,60,299,99)

CREATE_BUTTON_ITEM_DATA(ICON_DIR_UP56,STRID_NULL,ICON_DIR_UP,STATUS_ENABLE,NULL) 
SHOW_MAP_BEGIN(UIMenuWndSetupDateTime_YMD_INC)
SHOW_MAP_BODY(Normal37)
SHOW_MAP_BODY(Focused42)
SHOW_MAP_BODY(Normal_Disable45)
SHOW_MAP_BODY(Focused_Disable48)
SHOW_MAP_BODY(Pressed51)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndSetupDateTime_YMD_INC)
BUTTON_LIST_BEGIN(UIMenuWndSetupDateTime_YMD_INC)
BUTTON_LIST_ITEM(ICON_DIR_UP56)
BUTTON_LIST_END

CREATE_BUTTON_DATA(UIMenuWndSetupDateTime_YMD_INC,BTN_DRAW_IMAGE)

CREATE_BUTTON_CTRL(UIMenuWndSetupDateTime_YMD_INC,UIMenuWndSetupDateTime_YMD_INC,0,250,20,289,59)
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_ITEM_ROUNDRECT(CMD_RoundRect,RoundRect60,0,0,39,39,16,1,2,0,0,0,0,8,16)
SHOW_MAP_BEGIN(Skin58)
SHOW_MAP_BODY(RoundRect60)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin58,0,0,39,39)

SHOW_ITEM_IMAGE(CMD_Image|CMD_ITEM,Icon61, 6, 6, 33,33, ICONID_NULL,83886080,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Normal57)
SHOW_MAP_BODY(Skin58)
SHOW_MAP_BODY(Icon61)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal57,260,120,299,159)

SHOW_MAP_BEGIN(Skin63)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin63,0,0,39,39)

SHOW_ITEM_IMAGE(CMD_Image|CMD_ITEM,Icon64, 6, 6, 33,33, ICONID_NULL,83886080,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Focused62)
SHOW_MAP_BODY(Skin63)
SHOW_MAP_BODY(Icon64)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Focused62,260,120,299,159)

SHOW_MAP_BEGIN(Skin66)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin66,0,0,39,39)

SHOW_ITEM_IMAGE(CMD_Image|CMD_ITEM,Icon67, 6, 6, 33,33, ICONID_NULL,83886080,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Normal_Disable65)
SHOW_MAP_BODY(Skin66)
SHOW_MAP_BODY(Icon67)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal_Disable65,260,120,299,159)

SHOW_MAP_BEGIN(Skin69)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin69,0,0,39,39)

SHOW_ITEM_IMAGE(CMD_Image|CMD_ITEM,Icon70, 6, 6, 33,33, ICONID_NULL,83886080,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Focused_Disable68)
SHOW_MAP_BODY(Skin69)
SHOW_MAP_BODY(Icon70)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Focused_Disable68,260,120,299,159)

SHOW_ITEM_ROUNDRECT(CMD_RoundRect,RoundRect74,0,0,39,39,16,2,0,211,0,0,0,2,6)
SHOW_MAP_BEGIN(Skin72)
SHOW_MAP_BODY(RoundRect74)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin72,0,0,39,39)

SHOW_ITEM_IMAGE(CMD_Image|CMD_ITEM,Icon75, 7, 7, 34,34, ICONID_NULL,83886080,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Pressed71)
SHOW_MAP_BODY(Skin72)
SHOW_MAP_BODY(Icon75)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Pressed71,260,120,299,159)

CREATE_BUTTON_ITEM_DATA(ICON_DIR_DOWN76,STRID_NULL,ICON_DIR_DOWN,STATUS_ENABLE,NULL) 
SHOW_MAP_BEGIN(UIMenuWndSetupDateTime_YMD_DEC)
SHOW_MAP_BODY(Normal57)
SHOW_MAP_BODY(Focused62)
SHOW_MAP_BODY(Normal_Disable65)
SHOW_MAP_BODY(Focused_Disable68)
SHOW_MAP_BODY(Pressed71)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndSetupDateTime_YMD_DEC)
BUTTON_LIST_BEGIN(UIMenuWndSetupDateTime_YMD_DEC)
BUTTON_LIST_ITEM(ICON_DIR_DOWN76)
BUTTON_LIST_END

CREATE_BUTTON_DATA(UIMenuWndSetupDateTime_YMD_DEC,BTN_DRAW_IMAGE)

CREATE_BUTTON_CTRL(UIMenuWndSetupDateTime_YMD_DEC,UIMenuWndSetupDateTime_YMD_DEC,0,250,80,289,119)
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_MAP_BEGIN(Skin77)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin77,0,0,64,39)

SHOW_MAP_BEGIN(Skin79)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin79,0,0,19,29)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text81,0,0,19,29,STRID_NULL,0,0,0,0,2,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Normal78)
SHOW_MAP_BODY(Skin79)
SHOW_MAP_BODY(Text81)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal78,39,70,58,99)

SHOW_MAP_BEGIN(UIMenuWndSetupDateTime_YMD_Y)
SHOW_MAP_BODY(Normal78)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndSetupDateTime_YMD_Y)
CREATE_STATIC_DATA(UIMenuWndSetupDateTime_YMD_Y,STRID_0)
CREATE_STATIC_CTRL(UIMenuWndSetupDateTime_YMD_Y,0,0,5,19,34)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_MAP_BEGIN(Skin84)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin84,0,0,19,29)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text86,0,0,19,29,STRID_NULL,0,0,0,0,2,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Normal83)
SHOW_MAP_BODY(Skin84)
SHOW_MAP_BODY(Text86)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal83,54,70,73,99)

SHOW_MAP_BEGIN(UIMenuWndSetupDateTime_YMD_Y2)
SHOW_MAP_BODY(Normal83)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndSetupDateTime_YMD_Y2)
CREATE_STATIC_DATA(UIMenuWndSetupDateTime_YMD_Y2,STRID_0)
CREATE_STATIC_CTRL(UIMenuWndSetupDateTime_YMD_Y2,0,15,5,34,34)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_MAP_BEGIN(Skin89)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin89,0,0,19,29)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text91,0,0,19,29,STRID_NULL,0,0,0,0,2,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Normal88)
SHOW_MAP_BODY(Skin89)
SHOW_MAP_BODY(Text91)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal88,69,70,88,99)

SHOW_MAP_BEGIN(UIMenuWndSetupDateTime_YMD_Y3)
SHOW_MAP_BODY(Normal88)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndSetupDateTime_YMD_Y3)
CREATE_STATIC_DATA(UIMenuWndSetupDateTime_YMD_Y3,STRID_0)
CREATE_STATIC_CTRL(UIMenuWndSetupDateTime_YMD_Y3,0,30,5,49,34)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_MAP_BEGIN(Skin94)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin94,0,0,19,29)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text96,0,0,19,29,STRID_NULL,0,0,0,0,2,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Normal93)
SHOW_MAP_BODY(Skin94)
SHOW_MAP_BODY(Text96)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal93,84,70,103,99)

SHOW_MAP_BEGIN(UIMenuWndSetupDateTime_YMD_Y4)
SHOW_MAP_BODY(Normal93)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndSetupDateTime_YMD_Y4)
CREATE_STATIC_DATA(UIMenuWndSetupDateTime_YMD_Y4,STRID_0)
CREATE_STATIC_CTRL(UIMenuWndSetupDateTime_YMD_Y4,0,45,5,64,34)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_MAP_BEGIN(UIMenuWndSetupDateTime_YMD_PNL_Y)
SHOW_MAP_BODY(Skin77)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndSetupDateTime_YMD_PNL_Y)
DECLARE_CTRL_LIST(UIMenuWndSetupDateTime_YMD_PNL_Y)
CREATE_CTRL(UIMenuWndSetupDateTime_YMD_PNL_Y,UIMenuWndSetupDateTime_YMD_PNL_Y,CTRL_WND,NULL,0 ,29,25,93,64)
SHOW_MAP_BEGIN(Skin98)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin98,0,0,34,39)

SHOW_MAP_BEGIN(Skin100)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin100,0,0,19,29)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text102,0,0,19,29,STRID_NULL,0,0,0,0,2,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Normal99)
SHOW_MAP_BODY(Skin100)
SHOW_MAP_BODY(Text102)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal99,124,70,143,99)

SHOW_MAP_BEGIN(UIMenuWndSetupDateTime_YMD_M)
SHOW_MAP_BODY(Normal99)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndSetupDateTime_YMD_M)
CREATE_STATIC_DATA(UIMenuWndSetupDateTime_YMD_M,STRID_0)
CREATE_STATIC_CTRL(UIMenuWndSetupDateTime_YMD_M,0,0,5,19,34)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_MAP_BEGIN(Skin105)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin105,0,0,19,29)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text107,0,0,19,29,STRID_NULL,0,0,0,0,2,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Normal104)
SHOW_MAP_BODY(Skin105)
SHOW_MAP_BODY(Text107)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal104,139,70,158,99)

SHOW_MAP_BEGIN(UIMenuWndSetupDateTime_YMD_M2)
SHOW_MAP_BODY(Normal104)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndSetupDateTime_YMD_M2)
CREATE_STATIC_DATA(UIMenuWndSetupDateTime_YMD_M2,STRID_0)
CREATE_STATIC_CTRL(UIMenuWndSetupDateTime_YMD_M2,0,15,5,34,34)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_MAP_BEGIN(UIMenuWndSetupDateTime_YMD_PNL_M)
SHOW_MAP_BODY(Skin98)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndSetupDateTime_YMD_PNL_M)
DECLARE_CTRL_LIST(UIMenuWndSetupDateTime_YMD_PNL_M)
CREATE_CTRL(UIMenuWndSetupDateTime_YMD_PNL_M,UIMenuWndSetupDateTime_YMD_PNL_M,CTRL_WND,NULL,0 ,114,25,148,64)
SHOW_MAP_BEGIN(Skin109)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin109,0,0,34,39)

SHOW_MAP_BEGIN(Skin111)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin111,0,0,19,29)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text113,0,0,19,29,STRID_NULL,0,0,0,0,2,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Normal110)
SHOW_MAP_BODY(Skin111)
SHOW_MAP_BODY(Text113)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal110,179,70,198,99)

SHOW_MAP_BEGIN(UIMenuWndSetupDateTime_YMD_D)
SHOW_MAP_BODY(Normal110)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndSetupDateTime_YMD_D)
CREATE_STATIC_DATA(UIMenuWndSetupDateTime_YMD_D,STRID_0)
CREATE_STATIC_CTRL(UIMenuWndSetupDateTime_YMD_D,0,0,5,19,34)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_MAP_BEGIN(Skin116)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin116,0,0,19,29)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text118,0,0,19,29,STRID_NULL,0,0,0,0,2,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Normal115)
SHOW_MAP_BODY(Skin116)
SHOW_MAP_BODY(Text118)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal115,194,70,213,99)

SHOW_MAP_BEGIN(UIMenuWndSetupDateTime_YMD_D2)
SHOW_MAP_BODY(Normal115)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndSetupDateTime_YMD_D2)
CREATE_STATIC_DATA(UIMenuWndSetupDateTime_YMD_D2,STRID_0)
CREATE_STATIC_CTRL(UIMenuWndSetupDateTime_YMD_D2,0,15,5,34,34)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_MAP_BEGIN(UIMenuWndSetupDateTime_YMD_PNL_D)
SHOW_MAP_BODY(Skin109)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndSetupDateTime_YMD_PNL_D)
DECLARE_CTRL_LIST(UIMenuWndSetupDateTime_YMD_PNL_D)
CREATE_CTRL(UIMenuWndSetupDateTime_YMD_PNL_D,UIMenuWndSetupDateTime_YMD_PNL_D,CTRL_WND,NULL,0 ,169,25,203,64)
SHOW_MAP_BEGIN(Skin120)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin120,0,0,34,39)

SHOW_MAP_BEGIN(Skin122)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin122,0,0,19,29)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text124,0,0,19,29,STRID_NULL,0,0,0,0,2,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Normal121)
SHOW_MAP_BODY(Skin122)
SHOW_MAP_BODY(Text124)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal121,94,142,113,171)

SHOW_MAP_BEGIN(UIMenuWndSetupDateTime_YMD_HR)
SHOW_MAP_BODY(Normal121)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndSetupDateTime_YMD_HR)
CREATE_STATIC_DATA(UIMenuWndSetupDateTime_YMD_HR,STRID_0)
CREATE_STATIC_CTRL(UIMenuWndSetupDateTime_YMD_HR,0,0,5,19,34)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_MAP_BEGIN(Skin127)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin127,0,0,19,29)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text129,0,0,19,29,STRID_NULL,0,0,0,0,2,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Normal126)
SHOW_MAP_BODY(Skin127)
SHOW_MAP_BODY(Text129)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal126,109,142,128,171)

SHOW_MAP_BEGIN(UIMenuWndSetupDateTime_YMD_HR2)
SHOW_MAP_BODY(Normal126)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndSetupDateTime_YMD_HR2)
CREATE_STATIC_DATA(UIMenuWndSetupDateTime_YMD_HR2,STRID_0)
CREATE_STATIC_CTRL(UIMenuWndSetupDateTime_YMD_HR2,0,15,5,34,34)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_MAP_BEGIN(UIMenuWndSetupDateTime_YMD_PNL_HR)
SHOW_MAP_BODY(Skin120)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndSetupDateTime_YMD_PNL_HR)
DECLARE_CTRL_LIST(UIMenuWndSetupDateTime_YMD_PNL_HR)
CREATE_CTRL(UIMenuWndSetupDateTime_YMD_PNL_HR,UIMenuWndSetupDateTime_YMD_PNL_HR,CTRL_WND,NULL,0 ,84,97,118,136)
SHOW_MAP_BEGIN(Skin131)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin131,0,0,34,39)

SHOW_MAP_BEGIN(Skin133)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin133,0,0,19,29)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text135,0,0,19,29,STRID_NULL,0,0,0,0,2,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Normal132)
SHOW_MAP_BODY(Skin133)
SHOW_MAP_BODY(Text135)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal132,154,142,173,171)

SHOW_MAP_BEGIN(UIMenuWndSetupDateTime_YMD_MIN)
SHOW_MAP_BODY(Normal132)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndSetupDateTime_YMD_MIN)
CREATE_STATIC_DATA(UIMenuWndSetupDateTime_YMD_MIN,STRID_0)
CREATE_STATIC_CTRL(UIMenuWndSetupDateTime_YMD_MIN,0,0,5,19,34)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_MAP_BEGIN(Skin138)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin138,0,0,19,29)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text140,0,0,19,29,STRID_NULL,0,0,0,0,2,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Normal137)
SHOW_MAP_BODY(Skin138)
SHOW_MAP_BODY(Text140)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal137,169,142,188,171)

SHOW_MAP_BEGIN(UIMenuWndSetupDateTime_YMD_MIN2)
SHOW_MAP_BODY(Normal137)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndSetupDateTime_YMD_MIN2)
CREATE_STATIC_DATA(UIMenuWndSetupDateTime_YMD_MIN2,STRID_0)
CREATE_STATIC_CTRL(UIMenuWndSetupDateTime_YMD_MIN2,0,15,5,34,34)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_MAP_BEGIN(UIMenuWndSetupDateTime_YMD_PNL_MIN)
SHOW_MAP_BODY(Skin131)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndSetupDateTime_YMD_PNL_MIN)
DECLARE_CTRL_LIST(UIMenuWndSetupDateTime_YMD_PNL_MIN)
CREATE_CTRL(UIMenuWndSetupDateTime_YMD_PNL_MIN,UIMenuWndSetupDateTime_YMD_PNL_MIN,CTRL_WND,NULL,0 ,144,97,178,136)
SHOW_MAP_BEGIN(UIMenuWndSetupDateTime_Tab)
SHOW_MAP_BODY(Skin2)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndSetupDateTime_Tab)
CTRL_LIST_BEGIN(UIMenuWndSetupDateTime_Tab)
CTRL_LIST_ITEM(UIMenuWndSetupDateTime_Tab_YMD_VALUE)
CTRL_LIST_ITEM(UIMenuWndSetupDateTime_YMD_OK)
CTRL_LIST_ITEM(UIMenuWndSetupDateTime_YMD_INC)
CTRL_LIST_ITEM(UIMenuWndSetupDateTime_YMD_DEC)
CTRL_LIST_ITEM(UIMenuWndSetupDateTime_YMD_PNL_Y)
CTRL_LIST_ITEM(UIMenuWndSetupDateTime_YMD_PNL_M)
CTRL_LIST_ITEM(UIMenuWndSetupDateTime_YMD_PNL_D)
CTRL_LIST_ITEM(UIMenuWndSetupDateTime_YMD_PNL_HR)
CTRL_LIST_ITEM(UIMenuWndSetupDateTime_YMD_PNL_MIN)
CTRL_LIST_END

CREATE_TAB_DATA(UIMenuWndSetupDateTime_Tab,0)

CREATE_TAB_CTRL(UIMenuWndSetupDateTime_Tab,UIMenuWndSetupDateTime_Tab,0,10,40,309,229)
SHOW_MAP_BEGIN(UIMenuWndSetupDateTime)
SHOW_MAP_BODY(Skin1)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndSetupDateTime)
DECLARE_CTRL_LIST(UIMenuWndSetupDateTime)
CREATE_CTRL(UIMenuWndSetupDateTime,UIMenuWndSetupDateTime,CTRL_WND,NULL,0 ,0,0,319,239)
#endif
