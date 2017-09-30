#include "PrjCfg.h"
#include "UIDisplay.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          PBVIEW
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

//VDO1
BOOL View_Playback(UINT32 cmd, UINT32 param1, UINT32 param2)
{
    if(cmd == SHOWCMD_BEGINDRAW)
    {
        DBG_MSG("^Gbegin VDO1 src=PLAYBACK\r\n");
        GxDisplay_SetSwapEffect(LAYER_VDO1, SWAPEFFECT_DISCARD); //disable double buffer
        return TRUE;
    }
    if(cmd == SHOWCMD_ENDDRAW)
    {
        DBG_MSG("^Gend VDO1 src=PLAYBACK\r\n");
        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO1COMPARE, COMPARE_KEY_NEVER);
        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO1COLORKEY, 0);
        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO1ALPHA, OSD_OPAQUE_100);
        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO1BLEND, BLEND_DISABLE);
        return TRUE;
    }
    if(cmd == SHOWCMD_SETSIZE)
    {
        //ISIZE *pSize = (ISIZE*)param2;
        //Display_SetWindow(LAYER_VDO1, 0, 0, pSize->w, pSize->h); //change window size
        //FlowPlay_OnChangeSize(pSize, 0);
        return TRUE;
    }
    return FALSE;
}




