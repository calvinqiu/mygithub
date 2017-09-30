#include <stdio.h>
#include "UIFlow.h"

// --------------------------------------------------------------------------
// OPTIONS
// --------------------------------------------------------------------------
// Menu Common Options
TMDEF_BEGIN_OPTIONS(COMMON_MODE)
TMDEF_END_OPTIONS()

TMDEF_BEGIN_OPTIONS(COMMON_MENU)
TMDEF_END_OPTIONS()

TMDEF_BEGIN_OPTIONS(COMMON_LOCAL)
    TMDEF_OPTION_TEXT_S(COMMON_LOCAL_OFF,TM_OPTION_ENABLE)
    TMDEF_OPTION_TEXT_S(COMMON_LOCAL_ON,TM_OPTION_ENABLE)
TMDEF_END_OPTIONS()

TMDEF_BEGIN_OPTIONS(COMMON_CLOUD)
    TMDEF_OPTION_TEXT_S(COMMON_CLOUD_OFF,TM_OPTION_ENABLE)
    TMDEF_OPTION_TEXT_S(COMMON_CLOUD_ON,TM_OPTION_ENABLE)
TMDEF_END_OPTIONS()

TMDEF_BEGIN_OPTIONS(COMMON_SETUP)
TMDEF_END_OPTIONS()

TMDEF_BEGIN_OPTIONS(COMMON_EXT_SETUP)
TMDEF_END_OPTIONS()

// --------------------------------------------------------------------------
// ITEMS
// --------------------------------------------------------------------------
// Movie Menu Items
TMDEF_BEGIN_ITEMS(COMMON)
#if (_MODEL_DSC_ == _MODEL_DEMO1_)
    TMDEF_ITEM_TEXTID(COMMON_MODE)
    TMDEF_ITEM_TEXTID(COMMON_MENU)
    #if (_NETWORK_ != _NETWORK_NONE_)
    TMDEF_ITEM_TEXTID(COMMON_LOCAL)
    #endif
    TMDEF_ITEM_TEXTID(COMMON_SETUP)
    #if (EXTRA_MENU==ENABLE)
    TMDEF_ITEM_TEXTID(COMMON_EXT_SETUP)
    #endif
#else
    TMDEF_ITEM_TEXTID(COMMON_MODE)
    TMDEF_ITEM_TEXTID(COMMON_MENU)
    #if (_NETWORK_ != _NETWORK_NONE_)
    TMDEF_ITEM_TEXTID(COMMON_LOCAL)
    TMDEF_ITEM_TEXTID(COMMON_CLOUD)
    #endif
    TMDEF_ITEM_TEXTID(COMMON_SETUP)
#endif
TMDEF_END_ITEMS()

// --------------------------------------------------------------------------
// PAGES
// --------------------------------------------------------------------------
// Common Menu Pages
TMDEF_BEGIN_PAGES(COMMON)
    TMDEF_PAGE_TEXT_ICON(COMMON)
TMDEF_END_PAGES()

TMDEF_EMNU(gCommonMenu, COMMON, Common_MenuCallback)

// --------------------------------------------------------------------------
// Menu Callback
// --------------------------------------------------------------------------
int Common_MenuCallback(UINT32 uiMessage, UINT32 uiParam)
{
    UINT16  uwItemId;
    UINT16  uwOption;

    if (uiMessage == TMM_CONFIRM_OPTION)
    {
        uwItemId = LO_WORD(uiParam);
        uwOption = HI_WORD(uiParam);

        switch (uwItemId)
        {
        case IDM_COMMON_MODE:

            break;

        case IDM_COMMON_MENU:

            break;
        #if(WIFI_AP_FUNC==ENABLE)
        case IDM_COMMON_LOCAL:
            if(uwOption ==WIFI_ON)
            {
                #if 0
                UI_SetData(FL_NetWorkMode,NET_AP_MODE); // flag for TUTK on in
                Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_MOVIE);
                #else

                if(CheckWiFiMapTbl()!=E_OK)
                {
                    UI_SetData(FL_WIFI_LINK,WIFI_LINK_NG);
                    return TMF_PROCESSED;
                }

                Ux_OpenWindow(&UIMenuWndWiFiWaitCtrl, 0);
                BKG_PostEvent(NVTEVT_BKW_WIFI_ON);
                #endif
            }
            else
            {
                Ux_SendEvent(0,NVTEVT_EXE_WIFI_STOP,0);
                Ux_CloseWindow(&MenuCommonItemCtrl,0);
            }
            break;

        case IDM_COMMON_CLOUD:

            break;
        #endif
        case IDM_COMMON_SETUP:

            break;
        }
    }

    return TMF_PROCESSED;
}
