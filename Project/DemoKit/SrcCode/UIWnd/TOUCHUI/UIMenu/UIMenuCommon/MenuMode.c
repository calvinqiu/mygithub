#include <stdio.h>
#include "UIFlow.h"

BOOL g_NeedOpenSetupMode = FALSE;

// --------------------------------------------------------------------------
// OPTIONS
// --------------------------------------------------------------------------
// --------------------------------------------------------------------------
// ITEMS
// --------------------------------------------------------------------------

int MenuCustom_Movie(UINT32 uiMessage, UINT32 uiParam)
{
    if (System_GetState(SYS_STATE_CURRMODE) != PRIMARY_MODE_MOVIE)
    {
        Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_MOVIE);
    }
    else
    {
        Ux_CloseWindow(&MenuCommonItemCtrl, 0);
    }
    return TMF_PROCESSED;
}
int MenuCustom_Photo(UINT32 uiMessage, UINT32 uiParam)
{
    if (System_GetState(SYS_STATE_CURRMODE) != PRIMARY_MODE_PHOTO)
    {
        Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_PHOTO);
    }
    else
    {
        Ux_CloseWindow(&MenuCommonItemCtrl,0);
    }
    return TMF_PROCESSED;
}
int MenuCustom_Playback(UINT32 uiMessage, UINT32 uiParam)
{
    if (System_GetState(SYS_STATE_CURRMODE) != PRIMARY_MODE_PLAYBACK)
    {
        Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_PLAYBACK);
    }
    else
    {
        Ux_CloseWindow(&MenuCommonItemCtrl,0);
    }
    return TMF_PROCESSED;
}

int MenuCustom_Setup(UINT32 uiMessage, UINT32 uiParam)
{
    g_NeedOpenSetupMode = TRUE;
    Ux_CloseWindow(&MenuCommonItemCtrl,0);
    return TMF_PROCESSED;
}

// Movie Menu Items
TMDEF_BEGIN_ITEMS(MODETOUCH)
    TMDEF_ITEM_CUSTOM(MODETOUCH_MOVIE, MenuCustom_Movie)
    TMDEF_ITEM_CUSTOM(MODETOUCH_PHOTO, MenuCustom_Photo)
    TMDEF_ITEM_CUSTOM(MODETOUCH_PLAYBACK, MenuCustom_Playback)
    TMDEF_ITEM_CUSTOM(MODETOUCH_SETUP, MenuCustom_Setup)
TMDEF_END_ITEMS()

// --------------------------------------------------------------------------
// PAGES
// --------------------------------------------------------------------------
// Common Menu Pages
TMDEF_BEGIN_PAGES(MODETOUCH)
    TMDEF_PAGE_TEXT_ICON(MODETOUCH)
TMDEF_END_PAGES()

TMDEF_EMNU(gMenuModeTouch, MODETOUCH, Mode_MenuCallback)

// --------------------------------------------------------------------------
// Menu Callback
// --------------------------------------------------------------------------
int Mode_MenuCallback(UINT32 uiMessage, UINT32 uiParam)
{
    return TMF_PROCESSED;
}

