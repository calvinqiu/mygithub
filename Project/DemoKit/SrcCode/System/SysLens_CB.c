////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "Lens.h"
#include "GxFlash.h"
#include "ImageApp_CamPhoto.h" //for UIMovie_UpdateLensState
#include "UIAppMovie.h"

//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysLensExe
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

BOOL    bIsShuterPseudoopen = FALSE;

void Photo_LensCB(UINT32 MessageID, UINT32 *Parameter)
{
#if LENS_FUNCTION
    switch (MessageID)
    {
        case LENS_CB_CHGOZOOMSTEP:
            Ux_PostEvent(NVTEVT_CB_ZOOM, 0);
            break;
        case LENS_CB_SHUTTER_PSEUDO_OPEN:
            DBG_IND("===LENS_CB_SHUTTER_PSEUDO_OPEN===\r\n");
            //GxFlash_SuspendCharge();
            bIsShuterPseudoopen = TRUE;
            break;
        case LENS_CB_SHUTTER_PSEUDO_CLOSE:
            DBG_IND("===LENS_CB_SHUTTER_PSEUDO_CLOSE===\r\n");
            //GxFlash_ResumeCharge();
            bIsShuterPseudoopen = FALSE;
            break;
    }
#endif
}

void LensAPICB(UINT32 MessageID, UINT32 *Parameter)
{
#if LENS_FUNCTION
    DBG_IND("MessageID=%d\r\n",MessageID);
    switch(MessageID)
    {
        case LENS_CB_INITZOOM1_START:
        case LENS_CB_INITZOOM2_START:
        case LENS_CB_INITFOCUS_START:
        case LENS_CB_INITAPERTURE_START:
        case LENS_CB_ZOOM_START:
        case LENS_CB_FOCUS_START:
        case LENS_CB_APERTURE_START:
        case LENS_CB_SHUTTER_START:
        case LENS_CB_IRCUT_START:
            GxFlash_SuspendCharge();
            //#NT#2012/11/07#Calvin Chang#API function for Lens state updating -begin
            UIMovie_UpdateLensState(MessageID);
            //#NT#2012/11/07#Calvin Chang -end
            break;
        case LENS_CB_INITZOOM1_END:
        case LENS_CB_INITZOOM2_END:
        case LENS_CB_INITFOCUS_END:
        case LENS_CB_INITAPERTURE_END:
        case LENS_CB_ZOOM_END:
        case LENS_CB_FOCUS_END:
        case LENS_CB_APERTURE_END:
        case LENS_CB_SHUTTER_END:
        case LENS_CB_IRCUT_END:
            GxFlash_ResumeCharge();
            //#NT#2012/11/07#Calvin Chang#API function for Lens state updating -begin
            UIMovie_UpdateLensState(MessageID);
            //#NT#2012/11/07#Calvin Chang -end
            break;
        case LENS_CB_RESET_START:
            GxFlash_SuspendCharge();
            #if (LENS_PATCH_ZOOM_RESET == ENABLE)
            #endif

            break;
        case LENS_CB_RESET_END:
            #if (LENS_PATCH_ZOOM_RESET == ENABLE)
            #endif
            GxFlash_ResumeCharge();
            break;
        default:
            DBG_ERR("Parameter error in LensAPICB() (%d)\r\n",MessageID);
            break;
    }
#endif
}



