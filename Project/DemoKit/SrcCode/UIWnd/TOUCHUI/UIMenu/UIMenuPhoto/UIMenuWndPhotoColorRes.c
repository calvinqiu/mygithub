//This source code is generated by UI Designer Studio.

#ifndef UIMENUWNDPHOTOCOLORRES_H
#define UIMENUWNDPHOTOCOLORRES_H

#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIGraphics.h"
#include "NVTToolCommand.h"
#include "UIDisplayUtil.h"
//------------------------------------------------------------

SHOW_MAP_BEGIN(Skin1)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin1,0,0,319,239)

SHOW_ITEM_RECT(CMD_Rectangle,Rect4,0,0,294,168,272,0,5,0,0,0,0)
SHOW_MAP_BEGIN(Skin2)
SHOW_MAP_BODY(Rect4)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin2,0,0,294,168)

SHOW_MAP_BEGIN(Skin6)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin6,0,0,215,37)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text8,0,0,215,37,STRID_NULL,0,0,0,0,2,1,1,0,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Normal5)
SHOW_MAP_BODY(Skin6)
SHOW_MAP_BODY(Text8)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal5,52,53,267,90)

SHOW_ITEM_IMAGE(CMD_Image,Image11, 0, 0, 31,37, ICON_SEL_BAR_BEGIN,83886080,0,0,0,0,0,17,0)
SHOW_ITEM_IMAGE(CMD_Image,Image12, 32, 0, 63,37, ICON_SEL_BAR_MED,83886080,0,0,0,0,0,17,0)
SHOW_ITEM_IMAGE(CMD_Image,Image13, 64, 0, 95,37, ICON_SEL_BAR_MED,83886080,0,0,0,0,0,17,0)
SHOW_ITEM_IMAGE(CMD_Image,Image14, 96, 0, 127,37, ICON_SEL_BAR_MED,83886080,0,0,0,0,0,17,0)
SHOW_ITEM_IMAGE(CMD_Image,Image15, 128, 0, 159,37, ICON_SEL_BAR_MED,83886080,0,0,0,0,0,17,0)
SHOW_ITEM_IMAGE(CMD_Image,Image16, 160, 0, 191,37, ICON_SEL_BAR_MED,83886080,0,0,0,0,0,17,0)
SHOW_ITEM_IMAGE(CMD_Image,Image17, 184, 0, 215,37, ICON_SEL_BAR_END,83886080,0,0,0,0,0,16,0)
SHOW_MAP_BEGIN(Skin10)
SHOW_MAP_BODY(Image11)
SHOW_MAP_BODY(Image12)
SHOW_MAP_BODY(Image13)
SHOW_MAP_BODY(Image14)
SHOW_MAP_BODY(Image15)
SHOW_MAP_BODY(Image16)
SHOW_MAP_BODY(Image17)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin10,0,0,215,37)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text19,0,0,215,37,STRID_NULL,0,0,0,0,14,1,1,0,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Focused9)
SHOW_MAP_BODY(Skin10)
SHOW_MAP_BODY(Text19)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Focused9,52,53,267,90)

SHOW_ITEM_RECT(CMD_Rectangle,Skin21Rect,0,0,215,37,0,0,98,98,0,0,0)
SHOW_MAP_BEGIN(Skin21)
SHOW_MAP_BODY(Skin21Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin21,0,0,215,37)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text23,0,0,203,31,STRID_NULL,0,0,0,0,1,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Normal_Disable20)
SHOW_MAP_BODY(Skin21)
SHOW_MAP_BODY(Text23)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Normal_Disable20,52,53,267,90)

SHOW_ITEM_RECT(CMD_Rectangle,Skin25Rect,0,0,215,37,0,0,98,98,0,0,0)
SHOW_MAP_BEGIN(Skin25)
SHOW_MAP_BODY(Skin25Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin25,0,0,215,37)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text27,0,0,203,31,STRID_NULL,0,0,0,0,1,1,1,256,0,17,0,0,0,0)
SHOW_MAP_BEGIN(Focused_Disable24)
SHOW_MAP_BODY(Skin25)
SHOW_MAP_BODY(Text27)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Focused_Disable24,52,53,267,90)

SHOW_ITEM_RECT(CMD_Rectangle,Skin29Rect,0,0,215,37,0,0,98,98,0,0,0)
SHOW_MAP_BEGIN(Skin29)
SHOW_MAP_BODY(Skin29Rect)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Skin29,0,0,215,37)

SHOW_ITEM_TEXT(CMD_Text|CMD_ITEM,Text31,0,0,203,31,STRID_NULL,0,0,0,0,1,1,1,256,0,0,0,0,0,0)
SHOW_MAP_BEGIN(Pressed28)
SHOW_MAP_BODY(Skin29)
SHOW_MAP_BODY(Text31)
SHOW_MAP_END

SHOW_ITEM_GROUP(CMD_Group,Pressed28,52,53,267,90)

CREATE_MENU_ITEM_DATA(STRID_COLOR32,STRID_COLOR,ICONID_NULL,STATUS_ENABLE,NULL,NULL) 
CREATE_MENU_ITEM_DATA(STRID_COLOR_BW33,STRID_COLOR_BW,ICONID_NULL,STATUS_ENABLE,NULL,NULL) 
CREATE_MENU_ITEM_DATA(STRID_COLOR_SEPIA34,STRID_COLOR_SEPIA,ICONID_NULL,STATUS_ENABLE,NULL,NULL) 
SHOW_MAP_BEGIN(UIMenuWndPhotoColor_Menu)
SHOW_MAP_BODY(Normal5)
SHOW_MAP_BODY(Focused9)
SHOW_MAP_BODY(Normal_Disable20)
SHOW_MAP_BODY(Focused_Disable24)
SHOW_MAP_BODY(Pressed28)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndPhotoColor_Menu)
MENU_LIST_BEGIN(UIMenuWndPhotoColor_Menu)
MENU_LIST_ITEM(STRID_COLOR32)
MENU_LIST_ITEM(STRID_COLOR_BW33)
MENU_LIST_ITEM(STRID_COLOR_SEPIA34)
MENU_LIST_END

CREATE_MENU_DATA(UIMenuWndPhotoColor_Menu,0,3,MENU_LAYOUT_VERTICAL|MENU_DISABLE_SHOW|MENU_SCROLL_CYCLE|MENU_SCROLL_NEXT_PAGE|MENU_DISABLE_NORMAL|MENU_DRAW_IMAGE)

CREATE_MENU_CTRL(UIMenuWndPhotoColor_Menu,UIMenuWndPhotoColor_Menu,0,39,8,254,45)
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SHOW_MAP_BEGIN(UIMenuWndPhotoColor_Panel)
SHOW_MAP_BODY(Skin2)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndPhotoColor_Panel)
DECLARE_CTRL_LIST(UIMenuWndPhotoColor_Panel)
CREATE_CTRL(UIMenuWndPhotoColor_Panel,UIMenuWndPhotoColor_Panel,CTRL_WND,NULL,0 ,13,45,307,213)
SHOW_MAP_BEGIN(UIMenuWndPhotoColor)
SHOW_MAP_BODY(Skin1)
SHOW_MAP_END

DECLARE_EVENT(UIMenuWndPhotoColor)
DECLARE_CTRL_LIST(UIMenuWndPhotoColor)
CREATE_CTRL(UIMenuWndPhotoColor,UIMenuWndPhotoColor,CTRL_WND,NULL,0 ,0,0,319,239)
#endif
