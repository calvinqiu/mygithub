#include "SysKer.h"
#include "DispSrvApi.h"
#include "display.h"
#include "Color.h"
#include "SizeConvert.h"
#include "AppDisp_ScaleView.h"
#include "PlaybackTsk.h"
#include "SysCfg.h"

#include "GxVideo.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SCALEVIEW
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#define PLAY_IMETMP_WIDTH               640
#define PLAY_IMETMP_HEIGHT              540

static BOOL       gMoviePlay_StartPlay = FALSE;
static MoviePlay_GetBuf g_fpGetBuf = NULL;
static USIZE gDispAR = {0};
static MEM_RANGE gScaleBuf1 = {0};
static MEM_RANGE gScaleBuf2 = {0};

//#NT#2017/02/24#Adam Su -begin
//#NT#Support sigle region display
VIDEO_WIN_ATTR gSingleRegionAttr = {0};
//#NT#2017/02/24#Adam Su -end

static ER MoviePlay_DispSrv_OnLock(UINT32* puiLocked, UINT32 uiIdx)
{
    *puiLocked = 0;
    return E_OK;
}

static ER MoviePlay_DispSrv_OnUnlock(UINT32 uiIdx)
{
    // Do nothing
    return E_OK;
}

void MoviePlay_DispSrv_Scalar2Display(PIMG_BUF pSrc, PIMG_BUF pDst, BOOL bDualHandle)
{
    IRECT             SrcRegion = {0};
    IRECT             DstRegion = {0};
    SIZECONVERT_INFO  ScaleInfo = {0};
    IMG_BUF           ImgBufScale2PDisp;
    UINT32            ImeBufWidth, ImeBufHeight;
    PIMG_BUF          pScaleDst;
    UINT32            DstWidth, DstHeight;
    INT32             temp_w, temp_h;
    //#NT#2013/10/18#Calvin Chang#Get track width and height for sample aspect ratio -begin
    PIMG_BUF          pDRatioSrc;
    IMG_BUF           ImgBufScaleDispRatio;

    DstWidth  = pDst->Width;
    DstHeight = pDst->Height;

    if (gDispAR.w == pSrc->Width)
    {
        // Display ratio is 1:1
        pDRatioSrc = pSrc;
    }
    else
    {
        ImeBufWidth  = gDispAR.w;
        ImeBufHeight = gDispAR.h;

        ImeBufWidth  = ALIGN_CEIL_8(ImeBufWidth);
        ImeBufHeight = ALIGN_CEIL_8(ImeBufHeight);

        gScaleBuf1 = (*g_fpGetBuf)(0);
        if (!gScaleBuf1.Addr)
        {
            DBG_ERR("ScaleBuf1 is NOT allocated!!\r\n");
            return;
        }

        GxImg_InitBuf(&ImgBufScaleDispRatio,
                      ImeBufWidth,
                      ImeBufHeight,
                      GX_IMAGE_PIXEL_FMT_YUV420_PACKED,
                      GXIMAGE_LINEOFFSET_ALIGN(8),
                      gScaleBuf1.Addr,
                      gScaleBuf1.Size);

        pDRatioSrc = &ImgBufScaleDispRatio;

        // RAW size To Display size
        SrcRegion.x = 0;
        SrcRegion.y = 0;
        SrcRegion.w = pSrc->Width;
        SrcRegion.h = pSrc->Height;

        DstRegion.x = 0;
        DstRegion.y = 0;
        DstRegion.w = pDRatioSrc->Width;
        DstRegion.h = pDRatioSrc->Height;

        GxImg_ScaleDataFine(pSrc, &SrcRegion, pDRatioSrc, &DstRegion);
    }
    //#NT#2013/10/18#Calvin Chang -end


    // 2-Pass Scale To Display (Reference by PB_Scale2Display() function, use the same scale solution)
    //
    // 1st Pass (Source Size -> (ImeBufWidth x ImeBufHeight))
    //
    if((pDst->Width > PLAY_IMETMP_WIDTH) || (pDst->Height > PLAY_IMETMP_HEIGHT) )
    {
        // Do 1st pass only on Large LCD Panel
        pScaleDst = pDst;
    }
    else
    {
        ImeBufWidth  = PLAY_IMETMP_WIDTH;
        ImeBufHeight = PLAY_IMETMP_HEIGHT;

        ImeBufWidth  = ALIGN_CEIL_8(ImeBufWidth);
        ImeBufHeight = ALIGN_CEIL_8(ImeBufHeight);

        gScaleBuf2 = (*g_fpGetBuf)(1);
        if (!gScaleBuf2.Addr)
        {
            DBG_ERR("ScaleBuf2 is NOT allocated!!\r\n");
            return;
        }

        GxImg_InitBuf(&ImgBufScale2PDisp,
                      ImeBufWidth,
                      ImeBufHeight,
                      GX_IMAGE_PIXEL_FMT_YUV420_PACKED,
                      GXIMAGE_LINEOFFSET_ALIGN(8),
                      gScaleBuf2.Addr,
                      gScaleBuf2.Size);

        pScaleDst = &ImgBufScale2PDisp;
    }

    if(gSingleRegionAttr.bEnable && gSingleRegionAttr.typeID == PB_SIGDISP_CROP)
    {
        SrcRegion.w = gSingleRegionAttr.iOutWidth;
        SrcRegion.h = gSingleRegionAttr.iOutHeight;
        SrcRegion.x = gSingleRegionAttr.iOutStartX;
        SrcRegion.y = gSingleRegionAttr.iOutStartY;
    }
    else
    {
        SrcRegion.x = 0;
        SrcRegion.y = 0;
        SrcRegion.w = pDRatioSrc->Width;
        SrcRegion.h = pDRatioSrc->Height;
    }


    ScaleInfo.uiSrcWidth  = pDRatioSrc->Width;
    ScaleInfo.uiSrcHeight = pDRatioSrc->Height;
    ScaleInfo.uiDstWidth  = pScaleDst->Width;
    ScaleInfo.uiDstHeight = pScaleDst->Height;
    ScaleInfo.alignType   = SIZECONVERT_ALIGN_FLOOR_32;

    //#NT#2017/02/24#Adam Su -begin
    //#NT#Support sigle region display
    // Single Region Display
    if (gSingleRegionAttr.bEnable)
    {
		INT32 StartX = ALIGN_CEIL_4((gSingleRegionAttr.iOutStartX)*DstWidth/OSD_W);
        INT32 StartY = ALIGN_CEIL_4((gSingleRegionAttr.iOutStartY)*DstHeight/OSD_H);
        if(gSingleRegionAttr.typeID == PB_SIGDISP_CROP)
        {
           ISIZE DeviceSize = GxVideo_GetDeviceSize(DOUT1);

           DstRegion.x = 0;
           DstRegion.y = 0;
           DstRegion.w = DeviceSize.w;
           DstRegion.h = DeviceSize.h;
        }
        else
        {
            // Check boundary conditions
            if (StartX >= (INT32)DstWidth || StartX < 0)
            {
                DBG_ERR("Assigned x is out of screen.(x=%d)\r\n", StartX);
                gSingleRegionAttr.bEnable = FALSE;
            }
            else if (StartY >= (INT32)DstHeight || StartY < 0)
            {
                DBG_ERR("Assigned y is out of screen.(y=%d)\r\n", StartY);
                gSingleRegionAttr.bEnable = FALSE;
            }
            else if (gSingleRegionAttr.iOutWidth < 0 || gSingleRegionAttr.iOutHeight < 0)
            {
                DBG_ERR("Size < 0.(w=%d, h=%d)\r\n", gSingleRegionAttr.iOutWidth, gSingleRegionAttr.iOutHeight);
                gSingleRegionAttr.bEnable = FALSE;
            }

            if (gSingleRegionAttr.typeID == PB_SIGDISP_COORDINATE)
            {
                temp_w = (INT32)DstWidth - StartX;
                temp_h = (INT32)DstHeight - StartY;

                ScaleInfo.uiDstWidth  = temp_w;
                ScaleInfo.uiDstHeight = temp_h;
                ScaleInfo.uiDstWRatio = temp_w*OSD_W/DstWidth;;
                ScaleInfo.uiDstHRatio = temp_h*OSD_H/DstHeight;
                ScaleInfo.alignType   = SIZECONVERT_ALIGN_FLOOR_32;

                DisplaySizeConvert(&ScaleInfo);

                DstRegion.x = StartX;
                DstRegion.y = StartY;
                DstRegion.w = ScaleInfo.uiOutWidth;
                DstRegion.h = ScaleInfo.uiOutHeight;
            }
            else if (gSingleRegionAttr.typeID == PB_SIGDISP_CUSTOMIZE)
            {
                DstRegion.x = StartX;
    	        DstRegion.y = StartY;
    	        DstRegion.w = ALIGN_CEIL_4(gSingleRegionAttr.iOutWidth*DstWidth/gSingleRegionAttr.iOSDWidth);
                DstRegion.h = ALIGN_CEIL_4(gSingleRegionAttr.iOutHeight*DstHeight/gSingleRegionAttr.iOSDHeight);

    		if (DstRegion.x + DstRegion.w > (INT32)DstWidth)
                    {
    				DBG_WRN("Width is out of screen, set to boundary.(x=%d, w=%d)\r\n", StartX, gSingleRegionAttr.iOutWidth);
    				DstRegion.w = ALIGN_CEIL_4((INT32)DstWidth - StartX);

    			}
    			if (DstRegion.y + DstRegion.h > (INT32)DstHeight)
                    {
    				DBG_WRN("Height is out of screen, set to boundary.(y=%d, h=%d)\r\n", StartY, gSingleRegionAttr.iOutHeight);
    				DstRegion.h = ALIGN_CEIL_4((INT32)DstHeight - StartY);
    			}
            }
        }
        GxImg_FillData(pScaleDst, NULL, COLOR_YUV_BLACK); // Single region display always needs to fill dark screen
        GxImg_ScaleDataFine(pDRatioSrc, &SrcRegion, pScaleDst, &DstRegion);
    }
    // Global region display
    else
    {
        // Get display device apsec-ratio
        if (!bDualHandle)
        {
            USIZE sz1 = GxVideo_GetDeviceAspect(DOUT1);
            ScaleInfo.uiDstWRatio = sz1.w;
            ScaleInfo.uiDstHRatio = sz1.h;
        }
        else // Support dual play
        {
            USIZE sz2 = GxVideo_GetDeviceAspect(DOUT2);
            ScaleInfo.uiDstWRatio = sz2.w;
            ScaleInfo.uiDstHRatio = sz2.h;
        }

        DisplaySizeConvert(&ScaleInfo);

        DstRegion.x = ScaleInfo.uiOutX;
        DstRegion.y = ScaleInfo.uiOutY;
        DstRegion.w = ScaleInfo.uiOutWidth;
        DstRegion.h = ScaleInfo.uiOutHeight;

        if ((DstRegion.w * pScaleDst->Height) != (DstRegion.h * pScaleDst->Width)) // If the ratio is not equal, need to fill dark screen
            GxImg_FillData(pScaleDst, NULL, COLOR_YUV_BLACK);
        GxImg_ScaleDataFine(pDRatioSrc, &SrcRegion, pScaleDst, &DstRegion);
    }
    //#NT#2017/02/24#Adam Su -end

    //
    // 2nd pass ((ImeBufWidth x ImeBufHeight)->Display Size)
    //
    if (pScaleDst != pDst)
    {
        SrcRegion.x = 0;
        SrcRegion.y = 0;
        SrcRegion.w = pScaleDst->Width;
        SrcRegion.h = pScaleDst->Height;

        if ((DstRegion.w * pScaleDst->Height) != (DstRegion.h * pScaleDst->Width)) // If the ratio is not equal, need to fill dark screen
            GxImg_FillData(pDst, NULL, COLOR_YUV_BLACK);
        GxImg_ScaleDataFine(pScaleDst, &SrcRegion ,pDst, NULL);
    }
}

static ER MoviePlay_DispSrv_OnDraw(DISPSRV_DRAW* pDraw)
{
    ER       er;
    UINT32   uiLockIdx;
    IMG_BUF* pSrc=NULL;
    IMG_BUF* pDst=pDraw->pImg[0];

    if (gMoviePlay_StartPlay == FALSE)
        return E_OK;     // Media Play has been closed on dual-view mode

    if((er=pDraw->fpLock[DISPSRV_SRC_IDX_PRIMARY](&uiLockIdx,DISPSRV_LOCK_NEWEST))!=E_OK)
    {//locked fail indicate skip this draw
        return er;
    }

    pSrc = &pDraw->pDS[DISPSRV_SRC_IDX_PRIMARY]->pSrc[uiLockIdx];

    //DBG_ERR("pSrc->PxlAddr[0] = 0x%x\r\n", pSrc->PxlAddr[0]);

    // Scale source to destination for primary (display)
    MoviePlay_DispSrv_Scalar2Display(pSrc, pDst, FALSE);

    // Support Dual display
    if(pDraw->bDualHandle)
    {
        pDst=pDraw->pImg[1];
        MoviePlay_DispSrv_Scalar2Display(pSrc, pDst, pDraw->bDualHandle);
    }

    pDraw->fpFlip(TRUE);

    pDraw->fpUnlock[DISPSRV_SRC_IDX_PRIMARY](uiLockIdx);

    return E_OK;

}

void MoviePlay_DispSrv_Start(void)
{
    gMoviePlay_StartPlay = TRUE;
}
void MoviePlay_DispSrv_Stop(void)
{
    gMoviePlay_StartPlay = FALSE;
}

void MoviePlay_DispSrv_Cfg(IMG_BUF* pSrc, USIZE* pDispAR,MoviePlay_GetBuf fpGetBuf)
{
    DISPSRV_CMD     Cmd={0};
    DISPSRV_SRC_CFG CfgSrc = {0};
    UINT32          fpDrawCb;

    if(pDispAR)
    {
        gDispAR = *pDispAR;
    }
    else
    {
        gDispAR.w = 1; gDispAR.h = 1;
    }
    g_fpGetBuf = fpGetBuf;

    // Configure
    CfgSrc.fpLockCb[DISPSRV_SRC_IDX_PRIMARY] = (DISPSRV_FP_LOCK_CB) MoviePlay_DispSrv_OnLock;
    CfgSrc.fpUnlockCb[DISPSRV_SRC_IDX_PRIMARY] = (DISPSRV_FP_UNLOCK_CB) MoviePlay_DispSrv_OnUnlock;
    CfgSrc.Desc[DISPSRV_SRC_IDX_PRIMARY].uiNumSrc = 1;
    CfgSrc.Desc[DISPSRV_SRC_IDX_PRIMARY].pSrc = pSrc;
    CfgSrc.Desc[DISPSRV_SRC_IDX_PRIMARY].pSrcRect = NULL;  //No DIS

    //Set the source buffer
    memset(&Cmd,0,sizeof(DISPSRV_CMD));
    Cmd.Prop.bExitCmdFinish = TRUE;
    Cmd.Idx = DISPSRV_CMD_IDX_SET_SRC;
    Cmd.In.uiNumByte = sizeof(CfgSrc);
    Cmd.In.pData = &CfgSrc;
    DispSrv_Cmd(&Cmd);

    // Set the Draw Callback
    fpDrawCb = (UINT32) MoviePlay_DispSrv_OnDraw;
    memset(&Cmd,0,sizeof(DISPSRV_CMD));
    Cmd.Prop.bExitCmdFinish = TRUE;
    Cmd.Idx = DISPSRV_CMD_IDX_SET_DRAW_CB;
    Cmd.In.uiNumByte = sizeof(fpDrawCb);
    Cmd.In.pData = &fpDrawCb;
    DispSrv_Cmd(&Cmd);
}

void MoviePlay_DispSrv_Trigger(void)
{
    DispSrv_TriggerNext();
}

//#NT#2017/03/03#Adam Su -begin
//#NT#Support single region display
void MoviePlay_ChangeSingleDstRegion(PB_SIGDISP_TYPE TypeID, PIRECT pDstRegion)
{
        DBG_IND("TypeID=%d, (x,y,w,h)=(%d,%d,%d,%d)\r\n",
                 TypeID, pDstRegion->x, pDstRegion->y, pDstRegion->w, pDstRegion->h);

        switch(TypeID)
        {
        case PB_SIGDISP_DISABLE:
            DBG_WRN("Single Region display off\r\n");
            gSingleRegionAttr.bEnable = FALSE;
			gSingleRegionAttr.typeID = TypeID;

			PB_SetParam(PBPRMID_SINGLE_DSTREGION, (UINT32)&gSingleRegionAttr);
            break;

        case PB_SIGDISP_COORDINATE:
            //#NT#2017/03/01#HM Tseng#[0116551] -begin
            //#NT#Support assigning display coordinate in single view
            if (pDstRegion->x < 0 || pDstRegion->y < 0)
            {
    			DBG_WRN("Coordinate < 0, X=%d, Y=%d, Set to 0\r\n", pDstRegion->x, pDstRegion->y);
    			pDstRegion->x = 0;
    			pDstRegion->y = 0;
		    }

            gSingleRegionAttr.bEnable 	 = TRUE;
			gSingleRegionAttr.typeID 	 = TypeID;
            gSingleRegionAttr.iOutStartX = pDstRegion->x;
            gSingleRegionAttr.iOutStartY = pDstRegion->y;
            gSingleRegionAttr.iOutWidth  = 0;
            gSingleRegionAttr.iOutHeight = 0;
            gSingleRegionAttr.iOSDWidth  = OSD_W;
            gSingleRegionAttr.iOSDHeight = OSD_H;

            PB_SetParam(PBPRMID_SINGLE_DSTREGION, (UINT32)&gSingleRegionAttr);
		    //#NT#2017/03/01#HM Tseng -end
            break;

        case PB_SIGDISP_CUSTOMIZE:
			if (pDstRegion->x < 0 || pDstRegion->y < 0)
            {
    			DBG_WRN("Coordinate < 0, X=%d, Y=%d, Set to 0\r\n", pDstRegion->x, pDstRegion->y);
    			pDstRegion->x = 0;
    			pDstRegion->y = 0;
		    }
            if (pDstRegion->w <= 0 || pDstRegion->h <= 0)
            {
                DBG_ERR("Input size error, W=%d, H=%d\r\n", pDstRegion->w, pDstRegion->h);
                return;
            }

            gSingleRegionAttr.bEnable    = TRUE;
			gSingleRegionAttr.typeID 	 = TypeID;
            gSingleRegionAttr.iOutStartX = pDstRegion->x;
            gSingleRegionAttr.iOutStartY = pDstRegion->y;
            gSingleRegionAttr.iOutWidth  = pDstRegion->w;
            gSingleRegionAttr.iOutHeight = pDstRegion->h;
            gSingleRegionAttr.iOSDWidth  = OSD_W;
            gSingleRegionAttr.iOSDHeight = OSD_H;

            PB_SetParam(PBPRMID_SINGLE_DSTREGION, (UINT32)&gSingleRegionAttr);
            break;
//#NT#2017/04/20#Brain Yen -begin
//#NT#Add for E-Mirror (wide panel) crop playback
        case PB_SIGDISP_CROP:
            gSingleRegionAttr.bEnable    = TRUE;
            gSingleRegionAttr.typeID 	 = TypeID;
            gSingleRegionAttr.iOutStartX = pDstRegion->x;
            gSingleRegionAttr.iOutStartY = pDstRegion->y;
            gSingleRegionAttr.iOutWidth  = pDstRegion->w;
            gSingleRegionAttr.iOutHeight = pDstRegion->h;
            gSingleRegionAttr.iOSDWidth  = OSD_W;
            gSingleRegionAttr.iOSDHeight = OSD_H;
            PB_SetParam(PBPRMID_SINGLE_DSTREGION, (UINT32)&gSingleRegionAttr);
            break;
//#NT#2017/04/20#Brain Yen -end
        default:
            DBG_ERR("Unsupported TypeID!\r\n");
            break;
        }
}
//#NT#2017/03/03#Adam Su -end



