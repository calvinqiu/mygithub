//This source code is generated by UI Designer Studio.

#ifndef UIMENUWNDSETUPCARNUMBERRES_H
#define UIMENUWNDSETUPCARNUMBERRES_H

#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIGraphics.h"
#include "NVTToolCommand.h"
#include "UIDisplayUtil.h"
//------------------------------------------------------------

SHOW_MAP_BEGIN(Skin1)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin1,0,0,799,479)

SHOW_ITEM_RECT(CMD_Rectangle,Rect4,0,0,665,311,272,0,5,15,0,0,0)
SHOW_ITEM_RECT(CMD_Rectangle,Skin2Rect,0,0,665,311,0,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Skin2)
SHOW_MAP_BODY(Skin2Rect)
SHOW_MAP_BODY(Rect4)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin2,0,0,665,311)

SHOW_MAP_BEGIN(Skin6)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin6,0,0,39,91)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text8,0,22,39,69,STRID_NULL,0,0,0,0,2,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Normal5)
SHOW_MAP_BODY(Skin6)
SHOW_MAP_BODY(Text8)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal5,277,160,316,251)

SHOW_ITEM_IMAGE(CMD_Image,Image_Sel_Bar11, 4, 25, 35,72, ICON_SEL_BAR_DATE,83886080,0,0,0,0,0,16,0)
SHOW_ITEM_IMAGE(CMD_Image,Image_Up12, 3, 0, 36,21, ICON_DIR_UP,83886080,0,0,0,0,0,0,0)
SHOW_ITEM_IMAGE(CMD_Image,Image_Down13, 3, 70, 36,91, ICON_DIR_DOWN,83886080,0,0,0,0,0,0,0)
SHOW_ITEM_RECT(CMD_Rectangle,Skin10Rect,0,0,39,91,0,0,15,15,0,0,0)
SHOW_MAP_BEGIN(Skin10)
SHOW_MAP_BODY(Skin10Rect)
SHOW_MAP_BODY(Image_Sel_Bar11)
SHOW_MAP_BODY(Image_Up12)
SHOW_MAP_BODY(Image_Down13)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin10,0,0,39,91)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text15,0,22,39,69,STRID_NULL,0,0,0,0,14,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Focused9)
SHOW_MAP_BODY(Skin10)
SHOW_MAP_BODY(Text15)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Focused9,277,160,316,251)

SHOW_ITEM_RECT(CMD_Rectangle,Skin17Rect,0,0,39,91,0,0,15,15,0,0,0)
SHOW_MAP_BEGIN(Skin17)
SHOW_MAP_BODY(Skin17Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin17,0,0,39,91)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text19,2,13,36,42,STRID_NULL,0,0,0,0,2,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Normal_Disable16)
SHOW_MAP_BODY(Skin17)
SHOW_MAP_BODY(Text19)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal_Disable16,277,160,316,251)

SHOW_ITEM_RECT(CMD_Rectangle,Skin21Rect,0,0,39,91,0,0,15,15,0,0,0)
SHOW_MAP_BEGIN(Skin21)
SHOW_MAP_BODY(Skin21Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin21,0,0,39,91)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text23,2,13,36,42,STRID_NULL,0,0,0,0,2,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Focused_Disable20)
SHOW_MAP_BODY(Skin21)
SHOW_MAP_BODY(Text23)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Focused_Disable20,277,160,316,251)

SHOW_ITEM_RECT(CMD_Rectangle,Skin25Rect,0,0,39,91,0,0,15,15,0,0,0)
SHOW_MAP_BEGIN(Skin25)
SHOW_MAP_BODY(Skin25Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin25,0,0,39,91)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text27,2,13,36,42,STRID_NULL,0,0,0,0,2,1,1,256,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Pressed24)
SHOW_MAP_BODY(Skin25)
SHOW_MAP_BODY(Text27)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Pressed24,277,160,316,251)

CREATE_BUTTON_ITEM_DATA(STRID_A128,STRID_A1,ICONID_NULL,STATUS_ENABLE,NULL) 
SHOW_MAP_BEGIN(UIMenuWndSetupCarNumber_Button1)
SHOW_MAP_BODY(Normal5)
SHOW_MAP_BODY(Focused9)
SHOW_MAP_BODY(Normal_Disable16)
SHOW_MAP_BODY(Focused_Disable20)
SHOW_MAP_BODY(Pressed24)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndSetupCarNumber_Button1)
BUTTON_LIST_BEGIN(UIMenuWndSetupCarNumber_Button1)
BUTTON_LIST_ITEM(STRID_A128)
BUTTON_LIST_END

CREATE_BUTTON_DATA(UIMenuWndSetupCarNumber_Button1,BTN_DRAW_IMAGE)

CREATE_BUTTON_CTRL(UIMenuWndSetupCarNumber_Button1,UIMenuWndSetupCarNumber_Button1,0,193,66,232,157)
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_MAP_BEGIN(Skin30)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin30,0,0,39,91)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text32,0,22,39,69,STRID_NULL,0,0,0,0,2,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Normal29)
SHOW_MAP_BODY(Skin30)
SHOW_MAP_BODY(Text32)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal29,317,160,356,251)

SHOW_ITEM_IMAGE(CMD_Image,Image_Sel_Bar35, 4, 25, 35,72, ICON_SEL_BAR_DATE,83886080,0,0,0,0,0,16,0)
SHOW_ITEM_IMAGE(CMD_Image,Image_Up36, 3, 0, 36,21, ICON_DIR_UP,83886080,0,0,0,0,0,0,0)
SHOW_ITEM_IMAGE(CMD_Image,Image_Down37, 3, 70, 36,91, ICON_DIR_DOWN,83886080,0,0,0,0,0,0,0)
SHOW_ITEM_RECT(CMD_Rectangle,Skin34Rect,0,0,39,91,0,0,15,15,0,0,0)
SHOW_MAP_BEGIN(Skin34)
SHOW_MAP_BODY(Skin34Rect)
SHOW_MAP_BODY(Image_Sel_Bar35)
SHOW_MAP_BODY(Image_Up36)
SHOW_MAP_BODY(Image_Down37)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin34,0,0,39,91)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text39,0,22,39,69,STRID_NULL,0,0,0,0,14,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Focused33)
SHOW_MAP_BODY(Skin34)
SHOW_MAP_BODY(Text39)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Focused33,317,160,356,251)

SHOW_ITEM_RECT(CMD_Rectangle,Skin41Rect,0,0,39,91,0,0,15,15,0,0,0)
SHOW_MAP_BEGIN(Skin41)
SHOW_MAP_BODY(Skin41Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin41,0,0,39,91)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text43,2,13,36,42,STRID_NULL,0,0,0,0,2,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Normal_Disable40)
SHOW_MAP_BODY(Skin41)
SHOW_MAP_BODY(Text43)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal_Disable40,317,160,356,251)

SHOW_ITEM_RECT(CMD_Rectangle,Skin45Rect,0,0,39,91,0,0,15,15,0,0,0)
SHOW_MAP_BEGIN(Skin45)
SHOW_MAP_BODY(Skin45Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin45,0,0,39,91)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text47,2,13,36,42,STRID_NULL,0,0,0,0,2,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Focused_Disable44)
SHOW_MAP_BODY(Skin45)
SHOW_MAP_BODY(Text47)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Focused_Disable44,317,160,356,251)

SHOW_ITEM_RECT(CMD_Rectangle,Skin49Rect,0,0,39,91,0,0,15,15,0,0,0)
SHOW_MAP_BEGIN(Skin49)
SHOW_MAP_BODY(Skin49Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin49,0,0,39,91)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text51,2,13,36,42,STRID_NULL,0,0,0,0,2,1,1,256,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Pressed48)
SHOW_MAP_BODY(Skin49)
SHOW_MAP_BODY(Text51)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Pressed48,317,160,356,251)

CREATE_BUTTON_ITEM_DATA(STRID_A152,STRID_A1,ICONID_NULL,STATUS_ENABLE,NULL) 
SHOW_MAP_BEGIN(UIMenuWndSetupCarNumber_Button2)
SHOW_MAP_BODY(Normal29)
SHOW_MAP_BODY(Focused33)
SHOW_MAP_BODY(Normal_Disable40)
SHOW_MAP_BODY(Focused_Disable44)
SHOW_MAP_BODY(Pressed48)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndSetupCarNumber_Button2)
BUTTON_LIST_BEGIN(UIMenuWndSetupCarNumber_Button2)
BUTTON_LIST_ITEM(STRID_A152)
BUTTON_LIST_END

CREATE_BUTTON_DATA(UIMenuWndSetupCarNumber_Button2,BTN_DRAW_IMAGE)

CREATE_BUTTON_CTRL(UIMenuWndSetupCarNumber_Button2,UIMenuWndSetupCarNumber_Button2,0,233,66,272,157)
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_MAP_BEGIN(Skin54)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin54,0,0,39,91)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text56,0,22,39,69,STRID_NULL,0,0,0,0,2,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Normal53)
SHOW_MAP_BODY(Skin54)
SHOW_MAP_BODY(Text56)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal53,357,160,396,251)

SHOW_ITEM_IMAGE(CMD_Image,Image_Sel_Bar59, 4, 25, 35,72, ICON_SEL_BAR_DATE,83886080,0,0,0,0,0,16,0)
SHOW_ITEM_IMAGE(CMD_Image,Image_Up60, 3, 0, 36,21, ICON_DIR_UP,83886080,0,0,0,0,0,0,0)
SHOW_ITEM_IMAGE(CMD_Image,Image_Down61, 3, 70, 36,91, ICON_DIR_DOWN,83886080,0,0,0,0,0,0,0)
SHOW_ITEM_RECT(CMD_Rectangle,Skin58Rect,0,0,39,91,0,0,15,15,0,0,0)
SHOW_MAP_BEGIN(Skin58)
SHOW_MAP_BODY(Skin58Rect)
SHOW_MAP_BODY(Image_Sel_Bar59)
SHOW_MAP_BODY(Image_Up60)
SHOW_MAP_BODY(Image_Down61)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin58,0,0,39,91)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text63,0,22,39,69,STRID_NULL,0,0,0,0,14,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Focused57)
SHOW_MAP_BODY(Skin58)
SHOW_MAP_BODY(Text63)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Focused57,357,160,396,251)

SHOW_ITEM_RECT(CMD_Rectangle,Skin65Rect,0,0,39,91,0,0,15,15,0,0,0)
SHOW_MAP_BEGIN(Skin65)
SHOW_MAP_BODY(Skin65Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin65,0,0,39,91)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text67,2,13,36,42,STRID_NULL,0,0,0,0,2,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Normal_Disable64)
SHOW_MAP_BODY(Skin65)
SHOW_MAP_BODY(Text67)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal_Disable64,357,160,396,251)

SHOW_ITEM_RECT(CMD_Rectangle,Skin69Rect,0,0,39,91,0,0,15,15,0,0,0)
SHOW_MAP_BEGIN(Skin69)
SHOW_MAP_BODY(Skin69Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin69,0,0,39,91)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text71,2,13,36,42,STRID_NULL,0,0,0,0,2,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Focused_Disable68)
SHOW_MAP_BODY(Skin69)
SHOW_MAP_BODY(Text71)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Focused_Disable68,357,160,396,251)

SHOW_ITEM_RECT(CMD_Rectangle,Skin73Rect,0,0,39,91,0,0,15,15,0,0,0)
SHOW_MAP_BEGIN(Skin73)
SHOW_MAP_BODY(Skin73Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin73,0,0,39,91)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text75,2,13,36,42,STRID_NULL,0,0,0,0,2,1,1,256,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Pressed72)
SHOW_MAP_BODY(Skin73)
SHOW_MAP_BODY(Text75)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Pressed72,357,160,396,251)

CREATE_BUTTON_ITEM_DATA(STRID_A176,STRID_A1,ICONID_NULL,STATUS_ENABLE,NULL) 
SHOW_MAP_BEGIN(UIMenuWndSetupCarNumber_Button3)
SHOW_MAP_BODY(Normal53)
SHOW_MAP_BODY(Focused57)
SHOW_MAP_BODY(Normal_Disable64)
SHOW_MAP_BODY(Focused_Disable68)
SHOW_MAP_BODY(Pressed72)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndSetupCarNumber_Button3)
BUTTON_LIST_BEGIN(UIMenuWndSetupCarNumber_Button3)
BUTTON_LIST_ITEM(STRID_A176)
BUTTON_LIST_END

CREATE_BUTTON_DATA(UIMenuWndSetupCarNumber_Button3,BTN_DRAW_IMAGE)

CREATE_BUTTON_CTRL(UIMenuWndSetupCarNumber_Button3,UIMenuWndSetupCarNumber_Button3,0,273,66,312,157)
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_MAP_BEGIN(Skin78)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin78,0,0,39,91)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text80,0,22,39,69,STRID_NULL,0,0,0,0,2,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Normal77)
SHOW_MAP_BODY(Skin78)
SHOW_MAP_BODY(Text80)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal77,397,160,436,251)

SHOW_ITEM_IMAGE(CMD_Image,Image_Sel_Bar83, 4, 25, 35,72, ICON_SEL_BAR_DATE,83886080,0,0,0,0,0,16,0)
SHOW_ITEM_IMAGE(CMD_Image,Image_Up84, 3, 0, 36,21, ICON_DIR_UP,83886080,0,0,0,0,0,0,0)
SHOW_ITEM_IMAGE(CMD_Image,Image_Down85, 3, 70, 36,91, ICON_DIR_DOWN,83886080,0,0,0,0,0,0,0)
SHOW_ITEM_RECT(CMD_Rectangle,Skin82Rect,0,0,39,91,0,0,15,15,0,0,0)
SHOW_MAP_BEGIN(Skin82)
SHOW_MAP_BODY(Skin82Rect)
SHOW_MAP_BODY(Image_Sel_Bar83)
SHOW_MAP_BODY(Image_Up84)
SHOW_MAP_BODY(Image_Down85)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin82,0,0,39,91)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text87,0,22,39,69,STRID_NULL,0,0,0,0,14,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Focused81)
SHOW_MAP_BODY(Skin82)
SHOW_MAP_BODY(Text87)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Focused81,397,160,436,251)

SHOW_ITEM_RECT(CMD_Rectangle,Skin89Rect,0,0,39,91,0,0,15,15,0,0,0)
SHOW_MAP_BEGIN(Skin89)
SHOW_MAP_BODY(Skin89Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin89,0,0,39,91)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text91,2,13,36,42,STRID_NULL,0,0,0,0,2,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Normal_Disable88)
SHOW_MAP_BODY(Skin89)
SHOW_MAP_BODY(Text91)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal_Disable88,397,160,436,251)

SHOW_ITEM_RECT(CMD_Rectangle,Skin93Rect,0,0,39,91,0,0,15,15,0,0,0)
SHOW_MAP_BEGIN(Skin93)
SHOW_MAP_BODY(Skin93Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin93,0,0,39,91)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text95,2,13,36,42,STRID_NULL,0,0,0,0,2,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Focused_Disable92)
SHOW_MAP_BODY(Skin93)
SHOW_MAP_BODY(Text95)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Focused_Disable92,397,160,436,251)

SHOW_ITEM_RECT(CMD_Rectangle,Skin97Rect,0,0,39,91,0,0,15,15,0,0,0)
SHOW_MAP_BEGIN(Skin97)
SHOW_MAP_BODY(Skin97Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin97,0,0,39,91)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text99,2,13,36,42,STRID_NULL,0,0,0,0,2,1,1,256,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Pressed96)
SHOW_MAP_BODY(Skin97)
SHOW_MAP_BODY(Text99)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Pressed96,397,160,436,251)

CREATE_BUTTON_ITEM_DATA(STRID_A1100,STRID_A1,ICONID_NULL,STATUS_ENABLE,NULL) 
SHOW_MAP_BEGIN(UIMenuWndSetupCarNumber_Button4)
SHOW_MAP_BODY(Normal77)
SHOW_MAP_BODY(Focused81)
SHOW_MAP_BODY(Normal_Disable88)
SHOW_MAP_BODY(Focused_Disable92)
SHOW_MAP_BODY(Pressed96)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndSetupCarNumber_Button4)
BUTTON_LIST_BEGIN(UIMenuWndSetupCarNumber_Button4)
BUTTON_LIST_ITEM(STRID_A1100)
BUTTON_LIST_END

CREATE_BUTTON_DATA(UIMenuWndSetupCarNumber_Button4,BTN_DRAW_IMAGE)

CREATE_BUTTON_CTRL(UIMenuWndSetupCarNumber_Button4,UIMenuWndSetupCarNumber_Button4,0,313,66,352,157)
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_MAP_BEGIN(Skin102)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin102,0,0,39,91)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text104,0,22,39,69,STRID_NULL,0,0,0,0,2,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Normal101)
SHOW_MAP_BODY(Skin102)
SHOW_MAP_BODY(Text104)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal101,437,160,476,251)

SHOW_ITEM_IMAGE(CMD_Image,Image_Sel_Bar107, 4, 25, 35,72, ICON_SEL_BAR_DATE,83886080,0,0,0,0,0,16,0)
SHOW_ITEM_IMAGE(CMD_Image,Image_Up108, 3, 0, 36,21, ICON_DIR_UP,83886080,0,0,0,0,0,0,0)
SHOW_ITEM_IMAGE(CMD_Image,Image_Down109, 3, 70, 36,91, ICON_DIR_DOWN,83886080,0,0,0,0,0,0,0)
SHOW_ITEM_RECT(CMD_Rectangle,Skin106Rect,0,0,39,91,0,0,15,15,0,0,0)
SHOW_MAP_BEGIN(Skin106)
SHOW_MAP_BODY(Skin106Rect)
SHOW_MAP_BODY(Image_Sel_Bar107)
SHOW_MAP_BODY(Image_Up108)
SHOW_MAP_BODY(Image_Down109)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin106,0,0,39,91)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text111,0,22,39,69,STRID_NULL,0,0,0,0,14,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Focused105)
SHOW_MAP_BODY(Skin106)
SHOW_MAP_BODY(Text111)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Focused105,437,160,476,251)

SHOW_ITEM_RECT(CMD_Rectangle,Skin113Rect,0,0,39,91,0,0,15,15,0,0,0)
SHOW_MAP_BEGIN(Skin113)
SHOW_MAP_BODY(Skin113Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin113,0,0,39,91)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text115,2,13,36,42,STRID_NULL,0,0,0,0,2,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Normal_Disable112)
SHOW_MAP_BODY(Skin113)
SHOW_MAP_BODY(Text115)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal_Disable112,437,160,476,251)

SHOW_ITEM_RECT(CMD_Rectangle,Skin117Rect,0,0,39,91,0,0,15,15,0,0,0)
SHOW_MAP_BEGIN(Skin117)
SHOW_MAP_BODY(Skin117Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin117,0,0,39,91)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text119,2,13,36,42,STRID_NULL,0,0,0,0,2,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Focused_Disable116)
SHOW_MAP_BODY(Skin117)
SHOW_MAP_BODY(Text119)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Focused_Disable116,437,160,476,251)

SHOW_ITEM_RECT(CMD_Rectangle,Skin121Rect,0,0,39,91,0,0,15,15,0,0,0)
SHOW_MAP_BEGIN(Skin121)
SHOW_MAP_BODY(Skin121Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin121,0,0,39,91)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text123,2,13,36,42,STRID_NULL,0,0,0,0,2,1,1,256,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Pressed120)
SHOW_MAP_BODY(Skin121)
SHOW_MAP_BODY(Text123)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Pressed120,437,160,476,251)

CREATE_BUTTON_ITEM_DATA(STRID_A1124,STRID_A1,ICONID_NULL,STATUS_ENABLE,NULL) 
SHOW_MAP_BEGIN(UIMenuWndSetupCarNumber_Button5)
SHOW_MAP_BODY(Normal101)
SHOW_MAP_BODY(Focused105)
SHOW_MAP_BODY(Normal_Disable112)
SHOW_MAP_BODY(Focused_Disable116)
SHOW_MAP_BODY(Pressed120)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndSetupCarNumber_Button5)
BUTTON_LIST_BEGIN(UIMenuWndSetupCarNumber_Button5)
BUTTON_LIST_ITEM(STRID_A1124)
BUTTON_LIST_END

CREATE_BUTTON_DATA(UIMenuWndSetupCarNumber_Button5,BTN_DRAW_IMAGE)

CREATE_BUTTON_CTRL(UIMenuWndSetupCarNumber_Button5,UIMenuWndSetupCarNumber_Button5,0,353,66,392,157)
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_MAP_BEGIN(Skin126)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin126,0,0,39,91)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text128,0,22,39,69,STRID_NULL,0,0,0,0,2,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Normal125)
SHOW_MAP_BODY(Skin126)
SHOW_MAP_BODY(Text128)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal125,477,160,516,251)

SHOW_ITEM_IMAGE(CMD_Image,Image_Sel_Bar131, 4, 25, 35,72, ICON_SEL_BAR_DATE,83886080,0,0,0,0,0,16,0)
SHOW_ITEM_IMAGE(CMD_Image,Image_Up132, 3, 0, 36,21, ICON_DIR_UP,83886080,0,0,0,0,0,0,0)
SHOW_ITEM_IMAGE(CMD_Image,Image_Down133, 3, 70, 36,91, ICON_DIR_DOWN,83886080,0,0,0,0,0,0,0)
SHOW_ITEM_RECT(CMD_Rectangle,Skin130Rect,0,0,39,91,0,0,15,15,0,0,0)
SHOW_MAP_BEGIN(Skin130)
SHOW_MAP_BODY(Skin130Rect)
SHOW_MAP_BODY(Image_Sel_Bar131)
SHOW_MAP_BODY(Image_Up132)
SHOW_MAP_BODY(Image_Down133)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin130,0,0,39,91)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text135,0,22,39,69,STRID_NULL,0,0,0,0,14,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Focused129)
SHOW_MAP_BODY(Skin130)
SHOW_MAP_BODY(Text135)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Focused129,477,160,516,251)

SHOW_ITEM_RECT(CMD_Rectangle,Skin137Rect,0,0,39,91,0,0,15,15,0,0,0)
SHOW_MAP_BEGIN(Skin137)
SHOW_MAP_BODY(Skin137Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin137,0,0,39,91)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text139,2,13,36,42,STRID_NULL,0,0,0,0,2,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Normal_Disable136)
SHOW_MAP_BODY(Skin137)
SHOW_MAP_BODY(Text139)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal_Disable136,477,160,516,251)

SHOW_ITEM_RECT(CMD_Rectangle,Skin141Rect,0,0,39,91,0,0,15,15,0,0,0)
SHOW_MAP_BEGIN(Skin141)
SHOW_MAP_BODY(Skin141Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin141,0,0,39,91)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text143,2,13,36,42,STRID_NULL,0,0,0,0,2,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Focused_Disable140)
SHOW_MAP_BODY(Skin141)
SHOW_MAP_BODY(Text143)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Focused_Disable140,477,160,516,251)

SHOW_ITEM_RECT(CMD_Rectangle,Skin145Rect,0,0,39,91,0,0,15,15,0,0,0)
SHOW_MAP_BEGIN(Skin145)
SHOW_MAP_BODY(Skin145Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin145,0,0,39,91)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text147,2,13,36,42,STRID_NULL,0,0,0,0,2,1,1,256,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Pressed144)
SHOW_MAP_BODY(Skin145)
SHOW_MAP_BODY(Text147)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Pressed144,477,160,516,251)

CREATE_BUTTON_ITEM_DATA(STRID_A1148,STRID_A1,ICONID_NULL,STATUS_ENABLE,NULL) 
SHOW_MAP_BEGIN(UIMenuWndSetupCarNumber_Button6)
SHOW_MAP_BODY(Normal125)
SHOW_MAP_BODY(Focused129)
SHOW_MAP_BODY(Normal_Disable136)
SHOW_MAP_BODY(Focused_Disable140)
SHOW_MAP_BODY(Pressed144)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndSetupCarNumber_Button6)
BUTTON_LIST_BEGIN(UIMenuWndSetupCarNumber_Button6)
BUTTON_LIST_ITEM(STRID_A1148)
BUTTON_LIST_END

CREATE_BUTTON_DATA(UIMenuWndSetupCarNumber_Button6,BTN_DRAW_IMAGE)

CREATE_BUTTON_CTRL(UIMenuWndSetupCarNumber_Button6,UIMenuWndSetupCarNumber_Button6,0,393,66,432,157)
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_MAP_BEGIN(Skin150)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin150,0,0,39,91)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text152,0,22,39,69,STRID_NULL,0,0,0,0,2,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Normal149)
SHOW_MAP_BODY(Skin150)
SHOW_MAP_BODY(Text152)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal149,517,160,556,251)

SHOW_ITEM_IMAGE(CMD_Image,Image_Sel_Bar155, 4, 25, 35,72, ICON_SEL_BAR_DATE,83886080,0,0,0,0,0,16,0)
SHOW_ITEM_IMAGE(CMD_Image,Image_Up156, 3, 0, 36,21, ICON_DIR_UP,83886080,0,0,0,0,0,0,0)
SHOW_ITEM_IMAGE(CMD_Image,Image_Down157, 3, 70, 36,91, ICON_DIR_DOWN,83886080,0,0,0,0,0,0,0)
SHOW_ITEM_RECT(CMD_Rectangle,Skin154Rect,0,0,39,91,0,0,15,15,0,0,0)
SHOW_MAP_BEGIN(Skin154)
SHOW_MAP_BODY(Skin154Rect)
SHOW_MAP_BODY(Image_Sel_Bar155)
SHOW_MAP_BODY(Image_Up156)
SHOW_MAP_BODY(Image_Down157)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin154,0,0,39,91)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text159,0,22,39,69,STRID_NULL,0,0,0,0,14,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Focused153)
SHOW_MAP_BODY(Skin154)
SHOW_MAP_BODY(Text159)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Focused153,517,160,556,251)

SHOW_ITEM_RECT(CMD_Rectangle,Skin161Rect,0,0,39,91,0,0,15,15,0,0,0)
SHOW_MAP_BEGIN(Skin161)
SHOW_MAP_BODY(Skin161Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin161,0,0,39,91)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text163,2,13,36,42,STRID_NULL,0,0,0,0,2,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Normal_Disable160)
SHOW_MAP_BODY(Skin161)
SHOW_MAP_BODY(Text163)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal_Disable160,517,160,556,251)

SHOW_ITEM_RECT(CMD_Rectangle,Skin165Rect,0,0,39,91,0,0,15,15,0,0,0)
SHOW_MAP_BEGIN(Skin165)
SHOW_MAP_BODY(Skin165Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin165,0,0,39,91)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text167,2,13,36,42,STRID_NULL,0,0,0,0,2,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Focused_Disable164)
SHOW_MAP_BODY(Skin165)
SHOW_MAP_BODY(Text167)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Focused_Disable164,517,160,556,251)

SHOW_ITEM_RECT(CMD_Rectangle,Skin169Rect,0,0,39,91,0,0,15,15,0,0,0)
SHOW_MAP_BEGIN(Skin169)
SHOW_MAP_BODY(Skin169Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin169,0,0,39,91)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text171,2,13,36,42,STRID_NULL,0,0,0,0,2,1,1,256,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Pressed168)
SHOW_MAP_BODY(Skin169)
SHOW_MAP_BODY(Text171)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Pressed168,517,160,556,251)

CREATE_BUTTON_ITEM_DATA(STRID_A1172,STRID_A1,ICONID_NULL,STATUS_ENABLE,NULL) 
SHOW_MAP_BEGIN(UIMenuWndSetupCarNumber_Button7)
SHOW_MAP_BODY(Normal149)
SHOW_MAP_BODY(Focused153)
SHOW_MAP_BODY(Normal_Disable160)
SHOW_MAP_BODY(Focused_Disable164)
SHOW_MAP_BODY(Pressed168)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndSetupCarNumber_Button7)
BUTTON_LIST_BEGIN(UIMenuWndSetupCarNumber_Button7)
BUTTON_LIST_ITEM(STRID_A1172)
BUTTON_LIST_END

CREATE_BUTTON_DATA(UIMenuWndSetupCarNumber_Button7,BTN_DRAW_IMAGE)

CREATE_BUTTON_CTRL(UIMenuWndSetupCarNumber_Button7,UIMenuWndSetupCarNumber_Button7,0,433,66,472,157)
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_MAP_BEGIN(UIMenuWndSetupCarNumber_Tab)
SHOW_MAP_BODY(Skin2)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndSetupCarNumber_Tab)
CTRL_LIST_BEGIN(UIMenuWndSetupCarNumber_Tab)
CTRL_LIST_ITEM(UIMenuWndSetupCarNumber_Button1)
CTRL_LIST_ITEM(UIMenuWndSetupCarNumber_Button2)
CTRL_LIST_ITEM(UIMenuWndSetupCarNumber_Button3)
CTRL_LIST_ITEM(UIMenuWndSetupCarNumber_Button4)
CTRL_LIST_ITEM(UIMenuWndSetupCarNumber_Button5)
CTRL_LIST_ITEM(UIMenuWndSetupCarNumber_Button6)
CTRL_LIST_ITEM(UIMenuWndSetupCarNumber_Button7)
CTRL_LIST_END

CREATE_TAB_DATA(UIMenuWndSetupCarNumber_Tab,0)

CREATE_TAB_CTRL(UIMenuWndSetupCarNumber_Tab,UIMenuWndSetupCarNumber_Tab,0,84,94,749,405)
SHOW_MAP_BEGIN(UIMenuWndSetupCarNumber)
SHOW_MAP_BODY(Skin1)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndSetupCarNumber)
DECLARE_CTRL_LIST(UIMenuWndSetupCarNumber)
CREATE_CTRL(UIMenuWndSetupCarNumber,UIMenuWndSetupCarNumber,CTRL_WND,NULL,0 ,0,0,799,479)
#endif
