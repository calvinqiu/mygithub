
#include "SysKer.h"

#include "PlaybackTsk.h"
//#include "SlideEffect.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          MDSCPLAYBK
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////


//=============================================================================
//#include "NvtSystem.h"

#include "UIFramework.h"
#include "UIAppPlay.h"

#include "PlaybackTsk.h"
#include "SMediaPlayAPI.h"

#include "AppDisp_ScaleView.h"

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


#define MoviePlay_GetMediaInfo(x)                SMediaPlay_GetMediaInfo(x)
#define MoviePlay_GetNowDisplayFrame(x,y,z)      SMediaPlay_GetNowDisplayFrame(x,y,z)
#define MoviePlay_GetVideoFrameRate(x)           SMediaPlay_GetVideoFrameRate(x)
#define MoviePlay_GetVideoFrameData(x)           SMediaPlay_GetVideoFrameData((SMEDIAPLAY_FARMEPOS *)x)
#define MoviePlay_GetAudioFrameData(x)           SMediaPlay_GetAudioFrameData((SMEDIAPLAY_FARMEPOS *)x)
#define MoviePlay_GetDispRatioScaleBuffer(x, y)  SMediaPlay_GetDispRatioScaleBuffer(x, y)
#define MoviePlay_GetDispScaleBuffer(x, y)       SMediaPlay_GetDispScaleBuffer(x, y)

static IMG_BUF  gMoviePlay_DispSrv_Src = {0};
static UINT32   gMoviePlay_DisplayCnt = 1;

MEM_RANGE Play_GetBuf(UINT32 id)
{
    MEM_RANGE ScaleBuf = {0};
    if(id == 0)
    {
    // Get the display ratio scale buffer from Media Play
    MoviePlay_GetDispRatioScaleBuffer(&ScaleBuf.Addr, &ScaleBuf.Size);
    }
    if(id == 1)
    {
    // Get the display scale buffer from Media Play
    MoviePlay_GetDispScaleBuffer(&ScaleBuf.Addr, &ScaleBuf.Size);
    }
    return ScaleBuf;
}

void Play_MovieCB(UINT32 uiEventID)
{
    DBG_IND("+PlayCB=%d\r\n",uiEventID);

    switch(uiEventID)
    {
        case SMEDIAPLAY_EVENT_MEDIAINFO_READY:
        {
            UINT32             uiLineOff[3] = {0};
            UINT32             PxlAddr[3] = {0};
            MEDIA_FIRST_INFO   *pMediaInfo = NULL;
            USIZE               ImgSize;
            USIZE               DispAR;

            pMediaInfo = MoviePlay_GetMediaInfo();
            // Get Image Size
            ImgSize.w = pMediaInfo->wid;
            ImgSize.h = pMediaInfo->hei;
            // Get Display AR
            DispAR.w = pMediaInfo->tkwid;
            DispAR.h = pMediaInfo->tkhei;

            // Init Source Buffer for DispSrv
            uiLineOff[0] = uiLineOff[1] = uiLineOff[2] = ALIGN_CEIL_32(ImgSize.w); // Algien 32-lines base for H.264 HW limitation
            PxlAddr[0]   = PxlAddr[1]   = PxlAddr[2]   = 0; // Set Null first
            GxImg_InitBufEx(&gMoviePlay_DispSrv_Src,
                            ImgSize.w,
                            ImgSize.h,
                            GX_IMAGE_PIXEL_FMT_YUV420_PACKED,
                            uiLineOff,
                            PxlAddr);

            // Configure the Source Buffer
            MoviePlay_DispSrv_Cfg(&gMoviePlay_DispSrv_Src, &DispAR, Play_GetBuf);

        }
        break;

        case SMEDIAPLAY_EVENT_ONE_DISPLAYFRAME:
        {
            UINT32 py, pu, pv;
            // New display frame is presented, get now display frame
            MoviePlay_GetNowDisplayFrame(&py, &pu, &pv);

            gMoviePlay_DispSrv_Src.PxlAddr[0] = py;
            gMoviePlay_DispSrv_Src.PxlAddr[1] = pu;
            gMoviePlay_DispSrv_Src.PxlAddr[2] = pv;

            // Display trigger
            MoviePlay_DispSrv_Trigger();

            break;
        }

        case SMEDIAPLAY_EVENT_FINISH:
        MoviePlay_DispSrv_Stop();
        Ux_PostEvent(NVTEVT_CB_MOVIE_FINISH, 0);
        break;

        case SMEDIAPLAY_EVENT_ONE_SECOND:
        Ux_PostEvent(NVTEVT_CB_MOVIE_ONE_SEC, 0);
        break;

        case SMEDIAPLAY_EVENT_START:
        gMoviePlay_DisplayCnt = 0;
        MoviePlay_DispSrv_Start();
        Ux_PostEvent(NVTEVT_CB_MOVIE_START, 0);
        break;

        case SMEDIAPLAY_EVENT_ONE_VIDEOFRAME:
        {
// This callback is used for playback progressive bar, however NT96660 doesn't implement it, so close this block now.  ( reduce unnecessary event post )        
#if 0       
            UINT32 uPollCnt = 2;

            if (MoviePlay_GetVideoFrameRate() > 120)        // for    240 fps to fix post event table overflow issue
                uPollCnt = 8;
            else if (MoviePlay_GetVideoFrameRate() >= 60)   // for 60/120 fps to fix post event table overflow issue
                uPollCnt = 4;
            gMoviePlay_DisplayCnt++;
            if (gMoviePlay_DisplayCnt == uPollCnt)
            {
                Ux_PostEvent(NVTEVT_CB_MOVIE_ONE_VIDEOFRAME, 0);
                gMoviePlay_DisplayCnt = 0;
            }
#endif            
        }
        break;

        case SMEDIAPLAY_EVENT_CURR_VIDFRAME:
        {
            SMEDIAPLAY_FARMEPOS VFrmPos;
            MoviePlay_GetVideoFrameData(&VFrmPos);
            DBG_IND("V:a=0x%X, s=%d\r\n",VFrmPos.Addr, VFrmPos.Size);
        }
        break;

        case SMEDIAPLAY_EVENT_CURR_AUDFRAME:
        {
            SMEDIAPLAY_FARMEPOS AFrmPos;
            MoviePlay_GetAudioFrameData(&AFrmPos);
            DBG_IND("A:a=0x%X, s=%d\r\n",AFrmPos.Addr, AFrmPos.Size);
        }
        break;

        default:
        break;
    }

}

