#include <stdio.h>
#include "UIFlow.h"

// --------------------------------------------------------------------------
// OPTIONS
// --------------------------------------------------------------------------
// MOVIE_SIZE_TAG
// Movie Size Options
TMDEF_BEGIN_OPTIONS(MOVIE_SIZE)
    TMDEF_OPTION_TEXT_S(MOVIE_SIZE_FRONT_2880x2160P24,TM_OPTION_ENABLE) //UHD P24
    TMDEF_OPTION_TEXT_S(MOVIE_SIZE_FRONT_2560x1440P30,TM_OPTION_ENABLE) //QHD P30
    TMDEF_OPTION_TEXT_S(MOVIE_SIZE_FRONT_2304x1296P30,TM_OPTION_NOT_SUPPORT) //3MHD P30
    TMDEF_OPTION_TEXT_S(MOVIE_SIZE_FRONT_1920x1080P96,TM_OPTION_NOT_SUPPORT) //FHD
    TMDEF_OPTION_TEXT_S(MOVIE_SIZE_FRONT_1920x1080P60,TM_OPTION_ENABLE)  //FHD
    TMDEF_OPTION_TEXT_S(MOVIE_SIZE_FRONT_1920x1080P30,TM_OPTION_ENABLE)  //FHD
    TMDEF_OPTION_TEXT_S(MOVIE_SIZE_FRONT_1280x720P120,TM_OPTION_ENABLE) //HD
    TMDEF_OPTION_TEXT_S(MOVIE_SIZE_FRONT_1280x720P60,TM_OPTION_ENABLE) //HD
    TMDEF_OPTION_TEXT_S(MOVIE_SIZE_FRONT_1280x720P30,TM_OPTION_ENABLE)  //HD
    TMDEF_OPTION_TEXT_S(MOVIE_SIZE_FRONT_848x480P30,TM_OPTION_ENABLE)   //WVGA
    TMDEF_OPTION_TEXT_S(MOVIE_SIZE_FRONT_640x480P240,TM_OPTION_ENABLE)   //VGA
    TMDEF_OPTION_TEXT_S(MOVIE_SIZE_FRONT_640x480P30,TM_OPTION_ENABLE)   //VGA
    TMDEF_OPTION_TEXT_S(MOVIE_SIZE_FRONT_320x240P30,TM_OPTION_ENABLE)   //QVGA
    TMDEF_OPTION_TEXT_S(MOVIE_SIZE_DUAL_1920x1080P30_1920x1080P30,TM_OPTION_ENABLE) //FHD FHD
TMDEF_END_OPTIONS()

// Cyclic recroding Options
TMDEF_BEGIN_OPTIONS(MOVIE_CYCLIC_REC)
    TMDEF_OPTION_TEXT(MOVIE_CYCLIC_REC_OFF)
    TMDEF_OPTION_TEXT(MOVIE_CYCLIC_REC_3MIN)
    TMDEF_OPTION_TEXT(MOVIE_CYCLIC_REC_5MIN)
    TMDEF_OPTION_TEXT(MOVIE_CYCLIC_REC_10MIN)
TMDEF_END_OPTIONS()

TMDEF_BEGIN_OPTIONS(MOVIE_MOTION_DET)
    TMDEF_OPTION_TEXT(MOVIE_MOTION_DET_OFF)
    TMDEF_OPTION_TEXT(MOVIE_MOTION_DET_ON)
TMDEF_END_OPTIONS()

TMDEF_BEGIN_OPTIONS(MOVIE_HDR)
    TMDEF_OPTION_TEXT(MOVIE_HDR_OFF)
    TMDEF_OPTION_TEXT(MOVIE_HDR_ON)
TMDEF_END_OPTIONS()

TMDEF_BEGIN_OPTIONS(MOVIE_AUDIO)
    TMDEF_OPTION_TEXT(MOVIE_AUDIO_OFF)
    TMDEF_OPTION_TEXT(MOVIE_AUDIO_ON)
TMDEF_END_OPTIONS()

TMDEF_BEGIN_OPTIONS(MOVIE_DATEIMPRINT)
    TMDEF_OPTION_TEXT(MOVIE_DATEIMPRINT_OFF)
    TMDEF_OPTION_TEXT(MOVIE_DATEIMPRINT_ON)
TMDEF_END_OPTIONS()

TMDEF_BEGIN_OPTIONS(MOVIE_LDWS)
    TMDEF_OPTION_TEXT_S(MOVIE_LDWS_OFF,TM_OPTION_ENABLE)
    TMDEF_OPTION_TEXT_S(MOVIE_LDWS_ON,TM_OPTION_ENABLE)
TMDEF_END_OPTIONS()

TMDEF_BEGIN_OPTIONS(MOVIE_FCW)
    TMDEF_OPTION_TEXT_S(MOVIE_FCW_OFF,TM_OPTION_ENABLE)
    TMDEF_OPTION_TEXT_S(MOVIE_FCW_ON,TM_OPTION_ENABLE)
TMDEF_END_OPTIONS()

TMDEF_BEGIN_OPTIONS(MOVIE_DDD)
    TMDEF_OPTION_TEXT_S(MOVIE_DDD_OFF,TM_OPTION_ENABLE)
    TMDEF_OPTION_TEXT_S(MOVIE_DDD_ON,TM_OPTION_ENABLE)
TMDEF_END_OPTIONS()

TMDEF_BEGIN_OPTIONS(MOVIE_ADAS_CAL)
    TMDEF_OPTION_TEXT_S(MOVIE_ADAS_CAL_OFF,TM_OPTION_ENABLE)
    TMDEF_OPTION_TEXT_S(MOVIE_ADAS_CAL_ON,TM_OPTION_ENABLE)
TMDEF_END_OPTIONS()



// --------------------------------------------------------------------------
// ITEMS
// --------------------------------------------------------------------------
// Movie Menu Items
TMDEF_BEGIN_ITEMS(MOVIE)
    TMDEF_ITEM_TEXTID(MOVIE_SIZE)
    TMDEF_ITEM_TEXTID(MOVIE_CYCLIC_REC)
#if (SHDR_FUNC == ENABLE)
    TMDEF_ITEM_TEXTID(MOVIE_HDR)
#endif
    //TMDEF_ITEM_TEXTID(MOVIE_EV)
    TMDEF_ITEM_TEXTID(EV)
    TMDEF_ITEM_TEXTID(MOVIE_MOTION_DET)
    TMDEF_ITEM_TEXTID(MOVIE_AUDIO)
    TMDEF_ITEM_TEXTID(MOVIE_DATEIMPRINT)
    #if 0 // To do: need ADAS related ICONs.
    TMDEF_ITEM_TEXTID(MOVIE_LDWS)
    TMDEF_ITEM_TEXTID(MOVIE_FCW)
    TMDEF_ITEM_TEXTID(MOVIE_DDD)
    TMDEF_ITEM_TEXTID(MOVIE_ADAS_CAL)
    #endif
TMDEF_END_ITEMS()

// --------------------------------------------------------------------------
// PAGES
// --------------------------------------------------------------------------
// Movie Menu Pages
TMDEF_BEGIN_PAGES(MOVIE)
    TMDEF_PAGE_TEXT_ICON(MOVIE)
    TMDEF_PAGE_TEXT_ICON(MOVIE) //reserved for setup menu,replace dynamic in MenuCommonItem
TMDEF_END_PAGES()

TMDEF_EMNU(gMovieMenu, MOVIE, Movie_MenuCallback)

// --------------------------------------------------------------------------
// Menu Callback
// --------------------------------------------------------------------------
int Movie_MenuCallback(UINT32 uiMessage, UINT32 uiParam)
{
    UINT16  uwItemId;
    UINT16  uwOption;

    if (uiMessage == TMM_CONFIRM_OPTION)
    {
        uwItemId = LO_WORD(uiParam);
        uwOption = HI_WORD(uiParam);

        switch (uwItemId)
        {
        case IDM_MOVIE_SIZE:
            //Set Movie size event until menu is closed.
            //Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIESIZE, 1, uwOption);
            //Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_IMAGE_RATIO, 1, GetMovieSizeRatio(UI_GetData(FL_MOVIE_SIZE)));
            break;

        case IDM_EV:
            Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_EV, 1, uwOption);
            break;

        case IDM_MOVIE_HDR:
            //Set Movie HDR event until menu is closed.
            //Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_SHDR, 1, uwOption);
            break;

        case IDM_MOVIE_CYCLIC_REC:
            Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_CYCLIC_REC, 1, uwOption);
            break;

        case IDM_MOVIE_MOTION_DET:
            Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOTION_DET, 1, uwOption);
            break;

        case IDM_MOVIE_AUDIO:
            Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIE_AUDIO, 1, uwOption);
            break;

        case IDM_MOVIE_DATEIMPRINT:
            Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIE_DATE_IMPRINT, 1, uwOption);
            break;

        case IDM_MOVIE_LDWS:
			//#NT#2016/10/12#KCHong#[0111054] -begin
			//#NT#Don't set flag here if use both FL_XXX_MENU and FL_XXX to store status.
            //Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIE_LDWS, 1, uwOption);
			//#NT#2016/10/12#KCHong#[0111054] -end
            break;

        case IDM_MOVIE_FCW:
			//#NT#2016/10/12#KCHong#[0111054] -begin
			//#NT#Don't set flag here if use both FL_XXX_MENU and FL_XXX to store status.
            //Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIE_FCW, 1, uwOption);
			//#NT#2016/10/12#KCHong#[0111054] -end
            break;

        case IDM_MOVIE_DDD:
            break;

        case IDM_MOVIE_ADAS_CAL:

            break;

        default:
            Setup_MenuCallback(uiMessage, uiParam);
            break;
        }
    }

    return TMF_PROCESSED;
}
