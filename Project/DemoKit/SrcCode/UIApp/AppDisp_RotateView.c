#include "SysKer.h"
#include "DispSrvApi.h"
#include "display.h"
#include "Color.h"
#include "SizeConvert.h"
#include "AppDisp_RotateView.h"
#include "ImageUnit_UserDraw.h"
#include "PrjCfg.h"
#include "Dx.h"
#include "GxVideo.h"
#include "UIPhotoInfo.h"
#include "SysMain.h"
#include "UIFlow.h"

#define THIS_DBGLVL         1       //0=OFF, 1=ERROR, 2=TRACE
#define __MODULE__          Rot
#define __DBGLVL__          THIS_DBGLVL
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
//#define __DBGFLT__          "[buf]"
#include "DebugModule.h"

//#NT#2016/10/18#Jeah Yen -begin
//#NT#support sensor map
extern INT32 System_MapSensorID(INT32 i);
//#NT#2016/10/18#Jeah Yen -end

#if (OSD_USE_ROTATE_BUFFER==ENABLE)
#define PRJ_HANDLE_VDO1_ROTATION ENABLE
#else
#define PRJ_HANDLE_VDO1_ROTATION DISABLE
#endif

//#NT#2016/06/13#Niven Cho -begin
//#NT#DUAL-IDE Display
#if (PRJ_HANDLE_VDO1_ROTATION==ENABLE)
extern DX_HANDLE gDevLCDObj;
#endif
//#NT#2016/06/13#Niven Cho -end

//#NT#2016/10/18#Jeah Yen -begin
//#NT#support sensor map
#define ROTATEVIEW_SRC_IDX System_MapSensorID(0)
//#NT#2016/10/18#Jeah Yen -end

void RotateView_SetImg(ISIZE* pImgSize, USIZE* pAspectRatio)
{
    #if 1
    {
        ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_WIDTH, 0x00010001); // 1x
        ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_HEIGHT, 0x00010001); // 1x
    }
    #else
    {
        ImageUnit_Begin(&ISF_UserDraw, 0);
            ImageUnit_CfgImgSize(ISF_IN1, pImgSize->w, pImgSize->h);
            ImageUnit_CfgAspect(ISF_IN1, pAspectRatio->w, pAspectRatio->h);
            ImageUnit_CfgWindow(ISF_IN1, 0, 0, 0, 0);  //window range = full device range
        ImageUnit_End();
    }
    #endif
}

INT32 RotateView_OnDraw(DISPSRV_DRAW* pDraw)
{
    ER er;
    UINT32 uiLockIdx;
    IMG_BUF* pSrc=NULL;
    IMG_BUF* pDst = pDraw->pImg[0];
    //#NT#2016/06/13#Niven Cho -begin
    //#NT#DUAL-IDE Display
    IMG_BUF* pDst2 = pDraw->pImg[1];
    //#NT#2016/06/13#Niven Cho -end

    //#NT#2016/06/27#Niven Cho -begin
    //#NT#Consider only sensor 2 case.
    if((er=pDraw->fpLock[ROTATEVIEW_SRC_IDX](&uiLockIdx,DISPSRV_LOCK_NEWEST))!=E_OK)
    {//locked fail indicate skip this draw
        return er;
    }

    pSrc = &pDraw->pDS[ROTATEVIEW_SRC_IDX]->pSrc[uiLockIdx];

//#NT#2017/03/23#Niven Cho -begin
//#NT# use GxImg_RotatePasteData instead of GxImg_RotateData to fix src_w%16!=0 with 90 degree rotation.
#if (PRJ_HANDLE_VDO1_ROTATION==ENABLE)
    pDst = pDraw->pRawImg[0];
    //#NT#2016/06/13#Niven Cho -begin
    //#NT#DUAL-IDE Display
    if((DX_HANDLE)GxVideo_GetDevice(DOUT1) == gDevLCDObj)
    {
		IPOINT pt = {0};
        if(GxImg_RotatePasteData(pSrc
						,NULL
                        ,pDraw->pRawImg[0]
                        ,&pt
                        ,GX_IMAGE_ROTATE_270
                        ,GX_IMAGE_ROTATE_ENG2)==E_OK)
        {

#if 0
            INT32 m_ImageRatioIdx = 6;
            USIZE IMAGERATIO_SIZE[IMAGERATIO_MAX_CNT]=
            {
                {9,16},//IMAGERATIO_9_16
                {2,3}, //IMAGERATIO_2_3
                {3,4}, //IMAGERATIO_3_4
                {1,1}, //IMAGERATIO_1_1
                {4,3}, //IMAGERATIO_4_3
                {3,2}, //IMAGERATIO_3_2
                {16,9},//IMAGERATIO_16_9
            };
            USIZE ImageRatioSize=IMAGERATIO_SIZE[m_ImageRatioIdx];
            IRECT rcDst={0};
            IRECT rcSrc={0};
            ISIZE DeviceSize = {0,0};
            USIZE DeviceRatioSize = {0,0};
            DeviceSize = GxVideo_GetDeviceSize(DOUT1);
            DeviceRatioSize= GxVideo_GetDeviceAspect(DOUT1);
            if(System_GetState(SYS_STATE_CURRMODE) !=SYS_MODE_UNKNOWN)
            {
                if (System_GetState(SYS_STATE_CURRMODE)==PRIMARY_MODE_MOVIE)
                    m_ImageRatioIdx=GetMovieSizeRatio(UI_GetData(FL_MOVIE_SIZE));
                else
                    m_ImageRatioIdx=GetPhotoSizeRatio(UI_GetData(FL_PHOTO_SIZE));

                ImageRatioSize=IMAGERATIO_SIZE[m_ImageRatioIdx];
            }
            rcDst.x = 0;
            rcDst.y = 0;
            rcDst.w = DeviceSize.h;
            rcDst.h = DeviceSize.w;

            rcSrc.w = OutImg.Width*DeviceRatioSize.h*ImageRatioSize.w/(ImageRatioSize.h*DeviceRatioSize.w);
            rcSrc.x =  (OutImg.Width-rcSrc.w)>>1;

            rcSrc.h = OutImg.Height;
            rcSrc.y =0;
            GxImg_ScaleData(&OutImg,&rcSrc,pDst,&rcDst);
#endif
//#NT#2017/03/23#Niven Cho -end
            //It's impossible that IDE2 device connecting a DSI panel, so we don't consider rotation case.
            if(pDraw->bDualHandle)
            {
                GxImg_ScaleData(pSrc,NULL,pDst2,NULL);
            }
            pDraw->fpUnlock[ROTATEVIEW_SRC_IDX](uiLockIdx);
            pDraw->fpRawFlip(TRUE);
        }
    }
    else
    {
        GxImg_ScaleData(pSrc,NULL,pDst,NULL);
        //It's impossible that IDE2 device connecting a DSI panel, so we don't consider rotation case.
        if(pDraw->bDualHandle)
        {
            GxImg_ScaleData(pSrc,NULL,pDst2,NULL);
        }
        pDraw->fpUnlock[ROTATEVIEW_SRC_IDX](uiLockIdx);
        pDraw->fpFlip(TRUE);
    }
    //#NT#2016/06/13#Niven Cho -end
#else
    GxImg_ScaleData(pSrc,NULL,pDst,NULL);
    //#NT#2016/06/13#Niven Cho -begin
    //#NT#DUAL-IDE Display
    //It's impossible that IDE2 device connecting a DSI panel, so we don't consider rotation case.
    if(pDraw->bDualHandle)
    {
        GxImg_ScaleData(pSrc,NULL,pDst2,NULL);
    }
    //#NT#2016/06/13#Niven Cho -end
    pDraw->fpUnlock[ROTATEVIEW_SRC_IDX](uiLockIdx);
    pDraw->fpFlip(TRUE);
#endif
    //#NT#2016/06/27#Niven Cho -end

#if 0
    DISPLAYER_PARAM LyrParam = {0};
    PDISP_OBJ pDispObj = disp_getDisplayObject(DISP_1);
    LyrParam.SEL.SET_OUTDIR.BufOutDir = DISPOUTDIR_VTC_FLIP;
    pDispObj->dispLyrCtrl(DISPLAYER_VDO1, DISPLAYER_OP_SET_OUTDIR, &LyrParam);
    pDispObj->load(TRUE);
#endif
    return E_OK;
}



