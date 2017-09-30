#include "GxImage.h"
#include "Color.h"
#include <stdio.h>
#include "SxCmd.h"
#include "UIInfo.h"
#include "SysCfg.h"
#include "AppDisp_PipView.h"
#include "ImageUnit_UserDraw.h"
#include "display.h"
#include "GxVideo.h"
#include "ImageUnit_ImagePipe.h"
#include "UIPhotoInfo.h"
#include "SysMain.h"
#include "UIFlow.h"

#define THIS_DBGLVL         1       //0=OFF, 1=ERROR, 2=TRACE
#define __MODULE__          Pip
#define __DBGLVL__          THIS_DBGLVL
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
//#define __DBGFLT__          "[buf]"
#include "DebugModule.h"

//#NT#2016/10/18#Jeah Yen -begin
//#NT#support sensor map
extern INT32 System_MapSensorID(INT32 i);
//#NT#2016/10/18#Jeah Yen -end

#if (OSD_USE_ROTATE_BUFFER==ENABLE)
// for Pip small picture, at 4:3 ratio, for if w cannot be 16 bytes alignment, don't enable it.
#define PRJ_HANDLE_VDO1_ROTATION DISABLE
#else
#define PRJ_HANDLE_VDO1_ROTATION DISABLE
#endif

static INT32 m_uiStyle = DUALCAM_BOTH;

#if (DUALCAM_PIP_BEHIND_FLIP == ENABLE)
//#NT#2016/02/01#Flag to set behind video flip by IDE#KCHong
static BOOL uiStyleChanged = FALSE;
#endif
//#NT#2016/06/17#Brain Yen -begin
//#NT# for Pipview LR function
#if (PIP_VIEW_LR == ENABLE)
static INT32 m_iCleanCnt = 2; //indicate the times of cleaning screen, set 1 as default, for the first time clean screen
static INT32 m_ImageRatioIdx = 6;
static USIZE IMAGERATIO_SIZE[IMAGERATIO_MAX_CNT]=
{
    {9,16},//IMAGERATIO_9_16
    {2,3}, //IMAGERATIO_2_3
    {3,4}, //IMAGERATIO_3_4
    {1,1}, //IMAGERATIO_1_1
    {4,3}, //IMAGERATIO_4_3
    {3,2}, //IMAGERATIO_3_2
    {16,9},//IMAGERATIO_16_9
};
#endif
//#NT#2016/06/17#Brain Yen -end
//#NT#2016/03/24#Niven Cho -begin
//#NT#FIX Failed PIP on TV with OSD_USE_ROTATE_BUFFER ENABLE
#if (PRJ_HANDLE_VDO1_ROTATION==ENABLE)
extern DX_HANDLE gDevLCDObj;
#endif
//#NT#2016/03/24#Niven Cho -end

void PipView_SetImg(ISIZE* pImgSize, USIZE* pAspectRatio)
{
    #if (PIP_VIEW_FASTSWITCH == ENABLE)
    #if 1
    if (UI_GetData(FL_DUAL_CAM) == DUALCAM_FRONT)
    {
        ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_WIDTH, 0x00010001); // 1x
        ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_HEIGHT, 0x00010001); // 1x
        ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_WIDTH2, 0);
        ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_HEIGHT2, 0);
    }
    if (UI_GetData(FL_DUAL_CAM) == DUALCAM_BEHIND)
    {
        ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_WIDTH, 0);
        ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_HEIGHT, 0);
        ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_WIDTH2, 0x00010001); // 1x
        ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_HEIGHT2, 0x00010001); // 1x
    }
    if (UI_GetData(FL_DUAL_CAM) == DUALCAM_BOTH)
    {
        ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_WIDTH, 0x00010001); // 1x
        ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_HEIGHT, 0x00010001); // 1x
        ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_WIDTH2, 0x00010002); // 1/2x
        ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_HEIGHT2, 0x00010002); // 1/2x
    }
    //#NT#2016/02/17#Niven Cho -begin
    //#NT#DUALCAM BOTH2 VIEW
    if (UI_GetData(FL_DUAL_CAM) == DUALCAM_BOTH2)
    {
        ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_WIDTH, 0x00010002); // 1/2x
        ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_HEIGHT, 0x00010002); // 1/2x
        ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_WIDTH2, 0x00010001); // 1x
        ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_HEIGHT2, 0x00010001); // 1x
    }
    //#NT#2016/02/17#Niven Cho -end

    //#NT#2016/09/21#Brain Yen -begin
    //#NT#add for special aspect ratio panel
    #if (PIP_VIEW_LR == ENABLE)
    {
        USIZE DeviceRatioSize= GxVideo_GetDeviceAspect(DOUT1);//16:5
        USIZE newImgSize;

        //switch(m_uiStyle)
        switch(UI_GetData(FL_DUAL_CAM))
        {
        case DUALCAM_LR_FRONT_FULL: //for front sensor
            //pAspectRatio->16:9
            //pImgSize->1280,400
            newImgSize.w = pImgSize->w;
            newImgSize.h = pImgSize->h *pAspectRatio->h*DeviceRatioSize.w/(DeviceRatioSize.h*pAspectRatio->w);

            ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_WIDTH, USERDRAW_CROP_X(0, newImgSize.w)); // set crop x,w
            ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_HEIGHT, USERDRAW_CROP_Y((newImgSize.h-pImgSize->h)>>1, newImgSize.h)); // set crop y,h
            //#NT#2017/01/24#Brain Yen -begin
            //#NT#Modify for saving memory
            //ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_WIDTH2, 0x00010001); // 1x
            //ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_HEIGHT2, 0x00010001); // 1x
            ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_WIDTH2, 0);
            ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_HEIGHT2, 0);
            //#NT#2017/01/24#Brain Yen -end
            break;
        case DUALCAM_LR_BEHIND_FULL: //for rear sensor
            //#NT#2017/01/24#Brain Yen -begin
            //#NT#Modify for saving memory
            //ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_WIDTH, 0x00010001); // 1x
            //ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_HEIGHT, 0x00010001); // 1x
            ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_WIDTH, 0); // 1x
            ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_HEIGHT, 0); // 1x
            //#NT#2017/01/24#Brain Yen -end
            newImgSize.w = pImgSize->w;
            newImgSize.h = pImgSize->h *pAspectRatio->h*DeviceRatioSize.w/(DeviceRatioSize.h*pAspectRatio->w);
            ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_WIDTH2, USERDRAW_CROP_X(0, newImgSize.w)); // set crop x,w
            ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_HEIGHT2, USERDRAW_CROP_Y((newImgSize.h-pImgSize->h)>>1,newImgSize.h)); // set crop y,h
            break;
        //#NT#2016/11/22#Brain Yen -begin
        //#NT#Modify for sawtooth bug, only for 16:9 image size
        case DUALCAM_LR_16_9:
            DeviceRatioSize.w=DeviceRatioSize.w>>1;//16->8
            if (((float)pAspectRatio->w/pAspectRatio->h) >= ((float)DeviceRatioSize.w/DeviceRatioSize.h))
            {
                ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_WIDTH, 0x00010002);
                ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_HEIGHT, (((pAspectRatio->h*DeviceRatioSize.w)<<16) | (pAspectRatio->w*DeviceRatioSize.h)) );
                ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_WIDTH2, 0x00010002);
                ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_HEIGHT2, (((pAspectRatio->h*DeviceRatioSize.w)<<16) | (pAspectRatio->w*DeviceRatioSize.h)) );
            }
            else
            {
                ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_WIDTH, (((pAspectRatio->w*DeviceRatioSize.h)<<16) | 2*(pAspectRatio->h *DeviceRatioSize.w)));
                ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_HEIGHT, 0x00010001);
                ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_WIDTH2, (((pAspectRatio->w*DeviceRatioSize.h)<<16) | 2*(pAspectRatio->h *DeviceRatioSize.w)));
                ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_HEIGHT2, 0x00010001);
            }
            break;
        case DUALCAM_LR_FULL:
            DeviceRatioSize.w=DeviceRatioSize.w>>1;
            if (((float)pAspectRatio->w/pAspectRatio->h) >= ((float)DeviceRatioSize.w/DeviceRatioSize.h))
            {
                ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_WIDTH, (((pAspectRatio->w*pImgSize->h)<<16) | (pAspectRatio->h*pImgSize->w)));
                ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_HEIGHT, 0x00010001);
                ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_WIDTH2, (((pAspectRatio->w*pImgSize->h)<<16) | (pAspectRatio->h*pImgSize->w)));
                ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_HEIGHT2, 0x00010001);
            }
            else
            {
                ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_WIDTH, 0x00010002);
                ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_HEIGHT, (((pAspectRatio->h*pImgSize->w)<<16) | 2*(pAspectRatio->w*pImgSize->h)));
                ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_WIDTH2, 0x00010002);
                ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_HEIGHT2, (((pAspectRatio->h*pImgSize->w)<<16) | 2*(pAspectRatio->w*pImgSize->h)));
            }
            break;
        //#NT#2016/11/03#Brain Yen -end
        default: //others
            ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_WIDTH, 0x00010001); // 1x
            ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_HEIGHT, 0x00010001); // 1x
            ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_WIDTH2, 0x00010001); // 1x
            ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_HEIGHT2, 0x00010001); // 1x
            break;
        }
    }
    #endif // #if (PIP_VIEW_LR == ENABLE)
    //#NT#2016/09/21#Brain Yen -end

    #else
    if (UI_GetData(FL_DUAL_CAM) == DUALCAM_FRONT)
    {
        ImageUnit_Begin(&ISF_UserDraw, 0);
            ImageUnit_CfgImgSize(ISF_IN1, pImgSize->w, pImgSize->h);
            ImageUnit_CfgAspect(ISF_IN1, pAspectRatio->w, pAspectRatio->h);
            ImageUnit_CfgWindow(ISF_IN1, 0, 0, 0, 0);  //window range = full device range
        ImageUnit_End();

        ImageUnit_Begin(&ISF_UserDraw, 0);
            ImageUnit_CfgImgSize(ISF_IN2, 80, 60); //very small size
            ImageUnit_CfgAspect(ISF_IN2, pAspectRatio->w, pAspectRatio->h);
            ImageUnit_CfgWindow(ISF_IN2, 0, 0, 0, 0);  //window range = full device range
        ImageUnit_End();
    }
    if (UI_GetData(FL_DUAL_CAM) == DUALCAM_BEHIND)
    {
        ImageUnit_Begin(&ISF_UserDraw, 0);
            ImageUnit_CfgImgSize(ISF_IN1, 80, 60); //very small size
            ImageUnit_CfgAspect(ISF_IN1, pAspectRatio->w, pAspectRatio->h);
            ImageUnit_CfgWindow(ISF_IN1, 0, 0, 0, 0);  //window range = full device range
        ImageUnit_End();

        ImageUnit_Begin(&ISF_UserDraw, 0);
            ImageUnit_CfgImgSize(ISF_IN2, pImgSize->w, pImgSize->h);
            ImageUnit_CfgAspect(ISF_IN2, pAspectRatio->w, pAspectRatio->h);
            ImageUnit_CfgWindow(ISF_IN2, 0, 0, 0, 0);  //window range = full device range
        ImageUnit_End();
    }
    if (UI_GetData(FL_DUAL_CAM) == DUALCAM_BOTH)
    {
        ImageUnit_Begin(&ISF_UserDraw, 0);
            ImageUnit_CfgImgSize(ISF_IN1, pImgSize->w, pImgSize->h);
            ImageUnit_CfgAspect(ISF_IN1, pAspectRatio->w, pAspectRatio->h);
            ImageUnit_CfgWindow(ISF_IN1, 0, 0, 0, 0);  //window range = full device range
        ImageUnit_End();

        ImageUnit_Begin(&ISF_UserDraw, 0);
            ImageUnit_CfgImgSize(ISF_IN2, pImgSize->w/2, pImgSize->h/2);
            ImageUnit_CfgAspect(ISF_IN2, pAspectRatio->w, pAspectRatio->h);
            ImageUnit_CfgWindow(ISF_IN2, 0, 0, 0, 0);  //window range = full device range
        ImageUnit_End();
    }
    #endif
    #endif
}

void PipView_SetStyle(UINT32 uiStyle)
{
    m_uiStyle = uiStyle;

#if (DUALCAM_PIP_BEHIND_FLIP == ENABLE)
    //#NT#2016/02/01#Flag to set behind video flip by IDE#KCHong
    uiStyleChanged = TRUE;
#endif
//#NT#2016/06/03#Brain Yen -begin
//#NT#for clean screen
#if (PIP_VIEW_LR == ENABLE)
    m_iCleanCnt =2;
#endif // #if (PIP_VIEW_LR == ENABLE)
//#NT#2016/06/03#Brain Yen -end
}

//#NT#2016/06/13#Niven Cho -begin
//#NT#DUAL-IDE Display
BOOL xPipView_MakeFirstImage(IMG_BUF* pDst, IMG_BUF* pSrc, DISPSRV_DRAW* pDraw, BOOL bIDE2)
{
    if(pSrc!=NULL)
    {
#if (PRJ_HANDLE_VDO1_ROTATION==ENABLE)
        //#NT#2016/03/24#Niven Cho -begin
        //#NT#FIX Failed PIP on TV with OSD_USE_ROTATE_BUFFER ENABLE
        if(!bIDE2 && (DX_HANDLE)GxVideo_GetDevice(DOUT1) == gDevLCDObj)
        {
            IMG_BUF OutImg={0};
            GxImg_RotateDataEx(pSrc
                ,pDraw->RawMem[0].Addr
                ,pDraw->RawMem[0].Size
                ,GX_IMAGE_ROTATE_270
                ,&OutImg
                ,GX_IMAGE_ROTATE_ENG2);
            if(memcmp(&OutImg,pDraw->pRawImg[0],sizeof(IMG_BUF))!=0)
            {
                DBG_ERR("OutImg and pRawImg[0] are not matched.\r\n");
                DBG_DUMP("f: %08X, %08X\r\n",OutImg.flag,pDraw->pRawImg[0]->flag);
                DBG_DUMP("w: %08X, %08X\r\n",OutImg.Width,pDraw->pRawImg[0]->Width);
                DBG_DUMP("h: %08X, %08X\r\n",OutImg.Height,pDraw->pRawImg[0]->Height);
                DBG_DUMP("pf: %08X, %08X\r\n",OutImg.PxlFmt,pDraw->pRawImg[0]->PxlFmt);
                DBG_DUMP("addr[0]: %08X, %08X\r\n",OutImg.PxlAddr[0],pDraw->pRawImg[0]->PxlAddr[0]);
                DBG_DUMP("addr[1]: %08X, %08X\r\n",OutImg.PxlAddr[1],pDraw->pRawImg[0]->PxlAddr[1]);
                DBG_DUMP("addr[2]: %08X, %08X\r\n",OutImg.PxlAddr[2],pDraw->pRawImg[0]->PxlAddr[2]);
                DBG_DUMP("ln[0]: %08X, %08X\r\n",OutImg.LineOffset[0],pDraw->pRawImg[0]->LineOffset[0]);
                DBG_DUMP("ln[1]: %08X, %08X\r\n",OutImg.LineOffset[1],pDraw->pRawImg[0]->LineOffset[1]);
                DBG_DUMP("ln[2]: %08X, %08X\r\n",OutImg.LineOffset[2],pDraw->pRawImg[0]->LineOffset[2]);
                DBG_DUMP("sr.x: %08X, %08X\r\n",OutImg.ScaleRatio.x,pDraw->pRawImg[0]->ScaleRatio.x);
                DBG_DUMP("sr.y: %08X, %08X\r\n",OutImg.ScaleRatio.y,pDraw->pRawImg[0]->ScaleRatio.y);
                DBG_DUMP("vc.x: %08X, %08X\r\n",OutImg.VirtCoord.x,pDraw->pRawImg[0]->VirtCoord.x);
                DBG_DUMP("vc.y: %08X, %08X\r\n",OutImg.VirtCoord.y,pDraw->pRawImg[0]->VirtCoord.y);
                DBG_DUMP("vc.w: %08X, %08X\r\n",OutImg.VirtCoord.w,pDraw->pRawImg[0]->VirtCoord.w);
                DBG_DUMP("vc.h: %08X, %08X\r\n",OutImg.VirtCoord.h,pDraw->pRawImg[0]->VirtCoord.h);
                DBG_DUMP("rc.x: %08X, %08X\r\n",OutImg.RealCoord.x,pDraw->pRawImg[0]->RealCoord.x);
                DBG_DUMP("rc.y: %08X, %08X\r\n",OutImg.RealCoord.y,pDraw->pRawImg[0]->RealCoord.y);
                DBG_DUMP("rc.w: %08X, %08X\r\n",OutImg.RealCoord.w,pDraw->pRawImg[0]->RealCoord.w);
                DBG_DUMP("rc.h: %08X, %08X\r\n",OutImg.RealCoord.h,pDraw->pRawImg[0]->RealCoord.h);
                return FALSE;
            }
        }
        else
        {
            GxImg_ScaleData(pSrc,NULL,pDst,NULL);
        }
        //#NT#2016/03/24#Niven Cho -end
#else
        //#NT#2016/06/17#Brain Yen -begin
        //#NT#for special aspect panel and pipview
        #if (PIP_VIEW_LR == ENABLE)
        IRECT rcDst={0};
        IRECT rcSrc={0};
        ISIZE DeviceSize = {0,0};
        //USIZE ImageRatioSize={16,9};
        USIZE ImageRatioSize=IMAGERATIO_SIZE[m_ImageRatioIdx];
        USIZE DeviceRatioSize = {0,0};
        DeviceSize = GxVideo_GetDeviceSize(DOUT1);
        DeviceRatioSize= GxVideo_GetDeviceAspect(DOUT1);
        UINT32 nSensor= 0;

        if(DUALCAM_LR_16_9==m_uiStyle)//16:9 half screen
        {
            DeviceSize.w=DeviceSize.w>>1;
            DeviceRatioSize.w=DeviceRatioSize.w>>1;
            if (((float)ImageRatioSize.w/ImageRatioSize.h) >= ((float)DeviceRatioSize.w/DeviceRatioSize.h))
            {
                rcDst.w = DeviceSize.w;
                rcDst.h = ALIGN_ROUND(DeviceSize.h * ImageRatioSize.h/ImageRatioSize.w* DeviceRatioSize.w/DeviceRatioSize.h, 2);
            }
            else
            {
                rcDst.h = DeviceSize.h;
                rcDst.w = ALIGN_ROUND(DeviceSize.w * ImageRatioSize.w/ImageRatioSize.h * DeviceRatioSize.h/DeviceRatioSize.w, 2);
            }
            rcDst.x = (DeviceSize.w-rcDst.w)>>1;
            rcDst.y = (DeviceSize.h-rcDst.h)>>1;
            GxImg_ScaleData(pSrc,NULL,pDst,&rcDst);
        }
        else if(DUALCAM_LR_FULL==m_uiStyle)//full half screen
        {
            rcDst.x = 0;
            rcDst.y = 0;
            rcDst.w = DeviceSize.w>>1;
            rcDst.h = DeviceSize.h;

            DeviceSize.w=DeviceSize.w>>1;
            DeviceRatioSize.w=DeviceRatioSize.w>>1;
            if (((float)ImageRatioSize.w/ImageRatioSize.h) >= ((float)DeviceRatioSize.w/DeviceRatioSize.h))
            {
                rcSrc.h = pSrc->Height;
                rcSrc.w = ALIGN_ROUND(pSrc->Width*DeviceSize.w*ImageRatioSize.h/(ImageRatioSize.w*DeviceSize.h),2);
            }
            else
            {
                rcSrc.w = pSrc->Width;
                rcSrc.h = ALIGN_ROUND(pSrc->Height* DeviceSize.h* ImageRatioSize.w/(DeviceSize.w*ImageRatioSize.h),2);
            }
            rcSrc.x = (pSrc->Width-rcSrc.w)>>1;
            rcSrc.y = (pSrc->Height-rcSrc.h)>>1;
            GxImg_ScaleData(pSrc,&rcSrc,pDst,&rcDst);
        }
        else if(DUALCAM_LR_FRONT_FULL==m_uiStyle) //front sensor full screen
        {
            //#NT#2017/01/24#Brain Yen -begin
            //#NT#Modify for bandwith issue
            nSensor= ImageUnit_GetParam(&ISF_ImagePipe, IMAGEPIPE_PARAM_SENSORCOUNT); //return 0/1/2/4
            if(nSensor ==2)//behind path
            {
                //Do nothing.
            }
            else //front path
            {
                GxImg_ScaleData(pSrc,NULL,pDst,NULL);
            }
            //#NT#2017/01/24#Brain Yen -end
        }

        #else
        GxImg_ScaleData(pSrc,NULL,pDst,NULL);
        #endif // #if (PIP_VIEW_LR == ENABLE)
        //#NT#2016/06/17#Brain Yen -end
#endif
        return TRUE;
    }
    return FALSE;
}
//#NT#2016/06/13#Niven Cho -end

//#NT#2016/06/13#Niven Cho -begin
//#NT#DUAL-IDE Display
INT32 PipView_OnDraw(DISPSRV_DRAW* pDraw) //PIP = Picture In Picture
{
    ER er;
    UINT32 i,n=(pDraw->bDualSrc==TRUE)?2:1;
    UINT32 uiLockIdx[2]={0};
    IMG_BUF* pSrc=NULL;
    IMG_BUF* pSrcImg[2]={0};
    IMG_BUF* pDst=pDraw->pImg[0];
    IMG_BUF* pDst2=pDraw->pImg[1];
    IRECT rcDst={0};
    BOOL bFlip = FALSE;
    //#NT#2016/06/17#Brain Yen -begin
    //#NT#for special aspect panel and pipview
    #if (PIP_VIEW_LR == ENABLE)
    IRECT rcSrc={0};
    ISIZE DeviceSize = {0,0};
    //INT32 DeviceSizeW=0;
    //USIZE ImageRatioSize={16,9};
    USIZE ImageRatioSize=IMAGERATIO_SIZE[m_ImageRatioIdx];
    USIZE DeviceRatioSize = {0,0};
    //UINT32  DeviceRatioSizeW=0;
    //#NT#2016/12/22#Brain Yen -begin
    //#NT#for chg mode(movie<->photo) bug
    if(UI_IsForceLock())
    {
        m_iCleanCnt=10;
        return E_OK;
    }
    //#NT#2016/12/22#Brain Yen -end

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
    else//mode changing
    {
        if(DUALCAM_LR_16_9==m_uiStyle)
            m_iCleanCnt=10;
    }
    #endif
    //#NT#2016/06/17#Brain Yen -end
    //#NT#2016/10/18#Jeah Yen -begin
    //#NT#support sensor map
    for(i=0;i<n;i++)
    {
        INT32 id = System_MapSensorID(i);
        if(pDraw->uiMaskSrc&(1<<id))
        {
            if((er=pDraw->fpLock[id](&uiLockIdx[i],DISPSRV_LOCK_NEWEST))!=E_OK)
            {
                DBG_ERR("failed to lock %d\r\n",i);
                return er;
            }
            else
            {
                 pSrcImg[i] = &pDraw->pDS[id]->pSrc[uiLockIdx[i]];
            }
        }
    }
    //#NT#2016/10/18#Jeah Yen -end

    //--------------------Customer Draw Begin-----------------------------------

    switch(m_uiStyle)
    {
        case DUALCAM_FRONT:
            //Process 1st draw
            bFlip = xPipView_MakeFirstImage(pDst,pSrcImg[0],pDraw,FALSE);
            if(bFlip && pDraw->bDualHandle)
            {
                bFlip = xPipView_MakeFirstImage(pDst2,pSrcImg[0],pDraw,TRUE);
            }
            break;

        case DUALCAM_BEHIND:
            //Process 1st draw
            bFlip = xPipView_MakeFirstImage(pDst,pSrcImg[1],pDraw,FALSE);
            if(bFlip && pDraw->bDualHandle)
            {
                bFlip = xPipView_MakeFirstImage(pDst2,pSrcImg[1],pDraw,TRUE);
            }
            break;

        //#NT#2016/02/17#Niven Cho -begin
        //#NT#DUALCAM BOTH2 VIEW
        case DUALCAM_BOTH:
        case DUALCAM_BOTH2:
//#NT#2016/06/17#Brain Yen -begin
//#NT# Modify for Pipview LR function
#if (PIP_VIEW_LR == ENABLE)
        case DUALCAM_LR_16_9:
        case DUALCAM_LR_FULL:
        case DUALCAM_LR_FRONT_FULL:
        case DUALCAM_LR_BEHIND_FULL:
            if(m_iCleanCnt>0)
            {
                GxImg_FillData(pDst, NULL, COLOR_YUV_BLACK) ;
                m_iCleanCnt--;
            }
            if(DUALCAM_LR_FRONT_FULL==m_uiStyle)
            {
                UINT32 nSensor= ImageUnit_GetParam(&ISF_ImagePipe, IMAGEPIPE_PARAM_SENSORCOUNT); //return 0/1/2/4
                if(nSensor ==2)
                {
                    //#NT#2017/01/24#Brain Yen -begin
                    //#NT# Modify for 2 sensors and IPL2 is auto closed(EX: IPL1 FHDp60)
                    if(pSrcImg[1])
                    {
                        bFlip = xPipView_MakeFirstImage(pDst,pSrcImg[1],pDraw,FALSE);
                        if(bFlip && pDraw->bDualHandle)
                        {
                            bFlip = xPipView_MakeFirstImage(pDst2,pSrcImg[1],pDraw,TRUE);
                        }
                    }
                    else
                    {
                        bFlip=TRUE;
                    }
                    //#NT#2017/01/24#Brain Yen -end
                    //Process 2nd draw//up, show
                    pSrc = pSrcImg[0];
                }
                else
                {
                    //Process 1st draw
                    bFlip = xPipView_MakeFirstImage(pDst,pSrcImg[0],pDraw,FALSE);
                    if(bFlip && pDraw->bDualHandle)
                    {
                        bFlip = xPipView_MakeFirstImage(pDst2,pSrcImg[0],pDraw,TRUE);
                    }
                    break;
                }
            }
            else //(m_uiStyle == DUALCAM_BOTH2)
            {
                //Process 1st draw
                bFlip = xPipView_MakeFirstImage(pDst,pSrcImg[0],pDraw,FALSE);
                if(bFlip && pDraw->bDualHandle)
                {
                    bFlip = xPipView_MakeFirstImage(pDst2,pSrcImg[0],pDraw,TRUE);
                }
                //Process 2nd draw
                pSrc = pSrcImg[1];
            }
#else
            if(m_uiStyle == DUALCAM_BOTH)
            {
                //Process 1st draw
                bFlip = xPipView_MakeFirstImage(pDst,pSrcImg[0],pDraw,FALSE);
                if(bFlip && pDraw->bDualHandle)
                {
                    bFlip = xPipView_MakeFirstImage(pDst2,pSrcImg[0],pDraw,TRUE);
                }
                //Process 2nd draw
                pSrc = pSrcImg[1];
            }
            else //(m_uiStyle == DUALCAM_BOTH2)
            {
                //Process 1st draw
                bFlip = xPipView_MakeFirstImage(pDst,pSrcImg[1],pDraw,FALSE);
                if(bFlip && pDraw->bDualHandle)
                {
                    bFlip = xPipView_MakeFirstImage(pDst2,pSrcImg[1],pDraw,TRUE);
                }
                //Process 2nd draw
                pSrc = pSrcImg[0];
            }
#endif
//#NT#2016/06/17#Brain Yen -end
        //#NT#2016/02/17#Niven Cho -end
            if(bFlip && pSrc!=NULL)
            {
            //#NT#2016/06/17#Brain Yen -begin
            //#NT#for special aspect panel and pipview
#if (PIP_VIEW_LR == ENABLE)
                if(DUALCAM_LR_16_9==m_uiStyle) //16:9 half screen
                {
                    DeviceSize.w=DeviceSize.w>>1;
                    DeviceRatioSize.w=DeviceRatioSize.w>>1;
                    if (((float)ImageRatioSize.w/ImageRatioSize.h) >= ((float)DeviceRatioSize.w/DeviceRatioSize.h))
                    {
                        rcDst.w = DeviceSize.w;
                        rcDst.h = ALIGN_ROUND(DeviceSize.h * ImageRatioSize.h/ImageRatioSize.w* DeviceRatioSize.w/DeviceRatioSize.h, 2);
                    }
                    else
                    {
                        rcDst.h = DeviceSize.h;
                        rcDst.w = ALIGN_ROUND(DeviceSize.w * ImageRatioSize.w/ImageRatioSize.h * DeviceRatioSize.h/DeviceRatioSize.w, 2);
                    }
                    #if (DUALCAM_PIP_BEHIND_FLIP == ENABLE)
                    //for rotate function
                    rcDst.x = 0;
                    rcDst.y = 0;
                    #else
                    rcDst.x = ((DeviceSize.w-rcDst.w)>>1) + DeviceSize.w;
                    rcDst.y = (DeviceSize.h-rcDst.h)>>1;
                    #endif
                }
                else if(DUALCAM_LR_FULL==m_uiStyle) //full half screen
                {
                    rcDst.w = DeviceSize.w>>1;
                    rcDst.h = DeviceSize.h;
                    #if (DUALCAM_PIP_BEHIND_FLIP == ENABLE)
                    //for rotate function
                    rcDst.x = 0;
                    rcDst.y = 0;
                    #else
                    rcDst.x = DeviceSize.w>>1;
                    rcDst.y = 0;
                    #endif
                }

                else if(DUALCAM_LR_FRONT_FULL==m_uiStyle) //front sensor full screen
                {
                    GxImg_ScaleData(pSrc,NULL,pDst,NULL);
                    break;
                }
                else if(DUALCAM_LR_BEHIND_FULL==m_uiStyle) //rear sensor full screen
                {
                    GxImg_ScaleData(pSrc,NULL,pDst,NULL);
                    break;
                }
                //#NT#2016/12/22#Brain Yen -begin
                //#NT#for WIFI change style to normal pip view and error msg print
                else
                {
                    rcDst.x = 0;
                    rcDst.y = 0;
                    rcDst.w = pDst->Width >> 1;
                    rcDst.h = pDst->Height >> 1;
                }
                //#NT#2016/12/22#Brain Yen -end
#else
                rcDst.x = 0;
                rcDst.y = 0;
                rcDst.w = pDst->Width >> 1;
                rcDst.h = pDst->Height >> 1;
#endif // #if (PIP_VIEW_LR == ENABLE)
           //#NT#2016/06/17#Brain Yen -end
#if (DUALCAM_PIP_BEHIND_FLIP == ENABLE)
                IMG_BUF  TempImgS = {0};
                IMG_BUF  TempImgR = {0};
                UINT32   TempPoolAddr = 0;
                UINT32   TempPoolSize = 0;
                UINT32   uiLineOffset[2] = {0};
                UINT32   uiPxlAddr[2] = {0};
                TempPoolAddr = OS_GetMempoolAddr(POOL_ID_DISP_VDO1TEMP3);
                TempPoolSize = OS_GetMempoolSize(POOL_ID_DISP_VDO1TEMP3);

                //#NT#2016/01/11#KCHong#[0092404] -begin
                //#NT#Fixed behind cam display error in photo mode.
                uiLineOffset[0] = ALIGN_CEIL_16(rcDst.w);
                uiLineOffset[1] = ALIGN_CEIL_16(rcDst.w);
                uiPxlAddr[0]    = TempPoolAddr;
                uiPxlAddr[1]    = TempPoolAddr + uiLineOffset[0] * rcDst.h;
                //#NT#2016/01/11#KCHong#[0092404] -end
//#NT#2016/11/28#Brain Yen -begin
//#NT#Add for PIP_VIEW_LR behind path flip
                #if (PIP_VIEW_LR == ENABLE)
                if(DUALCAM_LR_16_9==m_uiStyle)
                {
                    GxImg_InitBufEx(&TempImgS,
                                    rcDst.w,
                                    rcDst.h,
                                    pDst->PxlFmt,
                                    uiLineOffset,
                                    uiPxlAddr);
                    GxImg_ScaleData(pSrc, NULL, &TempImgS, &rcDst);
                }
                else if(DUALCAM_LR_FULL==m_uiStyle)
                {
                    GxImg_InitBufEx(&TempImgS,
                                    rcDst.w,
                                    rcDst.h,
                                    pDst->PxlFmt,
                                    uiLineOffset,
                                    uiPxlAddr);
                    //calc SrcRegion
                    //==============================
                    //DeviceSize.w=DeviceSize.w>>1;
                    DeviceSizeW=DeviceSize.w>>1;
                    //DeviceRatioSize.w=DeviceRatioSize.w>>1;
                    DeviceRatioSizeW=DeviceRatioSize.w>>1;
                    if (((float)ImageRatioSize.w/ImageRatioSize.h) >= ((float)DeviceRatioSizeW/DeviceRatioSize.h))
                    {
                        rcSrc.h = pSrc->Height;
                        rcSrc.w = ALIGN_ROUND(pSrc->Width*DeviceSizeW*ImageRatioSize.h/(ImageRatioSize.w*DeviceSize.h),2);
                    }
                    else
                    {
                        rcSrc.w = pSrc->Width;
                        rcSrc.h = ALIGN_ROUND(pSrc->Height* DeviceSize.h* ImageRatioSize.w/(DeviceSizeW*ImageRatioSize.h),2);
                    }
                    rcSrc.x = (pSrc->Width-rcSrc.w)>>1;
                    rcSrc.y = (pSrc->Height-rcSrc.h)>>1;
                    //==============================
                    GxImg_ScaleData(pSrc, &rcSrc, &TempImgS, &rcDst);
                }
                else
                #endif //#if (PIP_VIEW_LR == ENABLE)
//#NT#2016/11/28#Brain Yen -end
                {
                    GxImg_InitBufEx(&TempImgS,
                                   (pDst->Width >> 1),
                                   (pDst->Height >> 1),
                                    pDst->PxlFmt,
                                    uiLineOffset,
                                    uiPxlAddr);
                    GxImg_ScaleData(pSrc, NULL, &TempImgS, &rcDst);
                }
                GxImg_RotateDataEx(&TempImgS,
                    TempPoolAddr + TempPoolSize / 2,            // The first 1/2 buffer is located for scaled-down data
                    TempPoolSize / 2,                           // The rest 1/2 buffer is located for flipped data
                    GX_IMAGE_ROTATE_HRZ,
                    &TempImgR,
                    GX_IMAGE_ROTATE_ENG2);
                #if (PRJ_HANDLE_VDO1_ROTATION==ENABLE)
                //#NT#2016/03/24#Niven Cho -begin
                //#NT#FIX Failed PIP on TV with OSD_USE_ROTATE_BUFFER ENABLE
                if((DX_HANDLE)GxVideo_GetDevice(DOUT1) == gDevLCDObj)
                {
                    if(pSrc->Width==(UINT32)rcDst.w && pSrc->Height==(UINT32)rcDst.h)
                    {
                        IPOINT pt = {0};
                        pt.x = 0;
                        pt.y = 0;
                        GxImg_RotatePasteData(&TempImgR
                            ,NULL
                            ,pDraw->pRawImg[0]
                            ,&pt
                            ,GX_IMAGE_ROTATE_270
                            ,GX_IMAGE_ROTATE_ENG2);
                    }
                }
                else
                {
                    GxImg_ScaleData(&TempImgR, NULL, pDst, &rcDst);
                }
                //#NT#2016/03/24#Niven Cho -end
                #else  //#if (PRJ_HANDLE_VDO1_ROTATION==ENABLE)
//#NT#2016/11/28#Brain Yen -begin
//#NT#Add for PIP_VIEW_LR behind path flip
                #if (PIP_VIEW_LR == ENABLE)
                if(DUALCAM_LR_16_9==m_uiStyle)
                {
                    rcDst.x = ((DeviceSize.w-rcDst.w)>>1) + DeviceSize.w;
                    rcDst.y = (DeviceSize.h-rcDst.h)>>1;
                    GxImg_ScaleData(&TempImgR,NULL,pDst,&rcDst);
                }
                else if(DUALCAM_LR_FULL==m_uiStyle)
                {
                    rcDst.x = DeviceSize.w>>1;
                    rcDst.y = 0;
                    GxImg_ScaleData(&TempImgR,NULL,pDst,&rcDst);
                }
                else
                #endif //#if (PIP_VIEW_LR == ENABLE)
//#NT#2016/11/28#Brain Yen -end
                {
                    GxImg_ScaleData(&TempImgR, NULL, pDst, &rcDst);
                }
                #endif //#if (PRJ_HANDLE_VDO1_ROTATION==ENABLE)
#else //#if (DUALCAM_PIP_BEHIND_FLIP == ENABLE)
                #if (PRJ_HANDLE_VDO1_ROTATION==ENABLE)
                //#NT#2016/03/24#Niven Cho -begin
                //#NT#FIX Failed PIP on TV with OSD_USE_ROTATE_BUFFER ENABLE
                if((DX_HANDLE)GxVideo_GetDevice(DOUT1) == gDevLCDObj)
                {
                    if(pSrc->Width==(UINT32)rcDst.w && pSrc->Height==(UINT32)rcDst.h)
                    {
                        IPOINT pt = {0};
                        pt.x = 0;
                        pt.y = rcDst.w;
                        GxImg_RotatePasteData(pSrc
                            ,NULL
                            ,pDraw->pRawImg[0]
                            ,&pt
                            ,GX_IMAGE_ROTATE_270
                            ,GX_IMAGE_ROTATE_ENG2);
                    }
                }
                else
                {
                    GxImg_ScaleData(pSrc,NULL,pDst,&rcDst);
                }
                //#NT#2016/03/24#Niven Cho -end
                #else  //#if (PRJ_HANDLE_VDO1_ROTATION==ENABLE)
                //#NT#2016/06/17#Brain Yen -begin
                //#NT#for special aspect panel and pipview
                #if (PIP_VIEW_LR == ENABLE)
                if(DUALCAM_LR_16_9==m_uiStyle)//16:9 half screen
                {
                    GxImg_ScaleData(pSrc,NULL,pDst,&rcDst);
                }
                else if(DUALCAM_LR_FULL==m_uiStyle)//full half screen
                {
                    DeviceSize.w=DeviceSize.w>>1;
                    DeviceRatioSize.w=DeviceRatioSize.w>>1;
                    if (((float)ImageRatioSize.w/ImageRatioSize.h) >= ((float)DeviceRatioSize.w/DeviceRatioSize.h))
                    {
                        rcSrc.h = pSrc->Height;
                        rcSrc.w = ALIGN_ROUND(pSrc->Width*DeviceSize.w*ImageRatioSize.h/(ImageRatioSize.w*DeviceSize.h),2);
                    }
                    else
                    {
                        rcSrc.w = pSrc->Width;
                        rcSrc.h = ALIGN_ROUND(pSrc->Height* DeviceSize.h* ImageRatioSize.w/(DeviceSize.w*ImageRatioSize.h),2);
                    }
                    rcSrc.x = (pSrc->Width-rcSrc.w)>>1;
                    rcSrc.y = (pSrc->Height-rcSrc.h)>>1;
                    GxImg_ScaleData(pSrc,&rcSrc,pDst,&rcDst);
                }
                #else
                GxImg_ScaleData(pSrc,NULL,pDst,&rcDst);
                #endif // #if (PIP_VIEW_LR == ENABLE)
                //#NT#2016/06/17#Brain Yen -end
                #endif //#if (PRJ_HANDLE_VDO1_ROTATION==ENABLE)
#endif //#if (DUALCAM_PIP_BEHIND_FLIP == ENABLE)
                //It's impossible that IDE2 device connecting a DSI panel, so we don't consider rotation case.
                if(pDraw->bDualHandle)
                {
                    rcDst.x = 0;
                    rcDst.y = 0;
                    rcDst.w = pDst2->Width >> 1;
                    rcDst.h = pDst2->Height >> 1;
                    GxImg_ScaleData(pSrc,NULL,pDst2,&rcDst);
                }

            }
            else
            {
                bFlip = FALSE;
            }
            break;
    }

    for(i=0;i<n;i++)
    {
        if(pDraw->uiMaskSrc&(1<<i))
        {
            if((er=pDraw->fpUnlock[i](uiLockIdx[i]))!=E_OK)
            {
                DBG_ERR("failed to unlock %d\r\n",i);
                return er;
            }
        }
    }

    //--------------------Customer Draw End-------------------------------------
    if(bFlip)
    {
#if (PRJ_HANDLE_VDO1_ROTATION==ENABLE)
        pDraw->fpRawFlip(TRUE);
#else
        pDraw->fpFlip(TRUE);
#endif
    }

//#NT#2016/02/01#KCHong -begin
//#NT#Filp behind video by IDE
#if (DUALCAM_PIP_BEHIND_FLIP == ENABLE)
    if (uiStyleChanged)
    {
        DISPLAYER_PARAM LyrParam = {0};
        PDISP_OBJ pDispObj = disp_getDisplayObject(DISP_1);
        if (m_uiStyle == DUALCAM_BEHIND
//#NT#2016/11/28#Brain Yen -begin
//#NT#Add for PIP_VIEW_LR behind path flip
            || DUALCAM_LR_BEHIND_FULL==m_uiStyle
//#NT#2016/11/28#Brain Yen -end
            )
        {
            #if (VDO2_USE_ROTATE_BUFFER == ENABLE)
            LyrParam.SEL.SET_OUTDIR.BufOutDir = DISPOUTDIR_VTC_FLIP;
            #else
            LyrParam.SEL.SET_OUTDIR.BufOutDir = DISPOUTDIR_HRZ_FLIP;
            #endif
        }
        else
        {
            LyrParam.SEL.SET_OUTDIR.BufOutDir = DISPOUTDIR_NORMAL;
        }
        pDispObj->dispLyrCtrl(DISPLAYER_VDO1, DISPLAYER_OP_SET_OUTDIR, &LyrParam);
        pDispObj->load(TRUE);
        uiStyleChanged = FALSE;
    }
#endif
//#NT#2016/02/01#KCHong -end

    return E_OK;
}
//#NT#2016/06/13#Niven Cho -end

INT32 PipView_OnDraw_4in1(DISPSRV_DRAW* pDraw)
{
    ER er;
    UINT32 uiLockIdx;
    IMG_BUF* pSrc=NULL;
    IMG_BUF* pDst=pDraw->pImg[0];

    IRECT rcSrc={0},rcDst={0};

    //Draw 0
    if(pDraw->bDualSrc==FALSE || (pDraw->uiMaskSrc&0x1))
    {
        if((er=pDraw->fpLock[0](&uiLockIdx,DISPSRV_LOCK_NEWEST))!=E_OK)
        {//locked fail indicate skip this draw
            return er;
        }
        pSrc = &pDraw->pDS[0]->pSrc[uiLockIdx];
        rcSrc.x = 0;
        rcSrc.y = 0;
        rcSrc.w = pSrc->Width;
        rcSrc.h = pSrc->Height;
        rcDst.x = 0;
        rcDst.y = 0;
        rcDst.w = pDst->Width/2;
        rcDst.h = pDst->Height/2;
        GxImg_ScaleData(pSrc,&rcSrc,pDst,&rcDst);
        pDraw->fpUnlock[0](uiLockIdx);
    }

    //Draw 1
    if(pDraw->uiMaskSrc&0x2)
    {
        if((er=pDraw->fpLock[1](&uiLockIdx,DISPSRV_LOCK_NEWEST))!=E_OK)
        {//locked fail indicate skip this draw
            return er;
        }
        pSrc = &pDraw->pDS[1]->pSrc[uiLockIdx];
        rcSrc.x = 0;
        rcSrc.y = 0;
        rcSrc.w = pSrc->Width;
        rcSrc.h = pSrc->Height;
        rcDst.x = pDst->Width/2;
        rcDst.y = 0;
        rcDst.w = pDst->Width/2;
        rcDst.h = pDst->Height/2;
        GxImg_ScaleData(pSrc,&rcSrc,pDst,&rcDst);
        pDraw->fpUnlock[1](uiLockIdx);
    }

    //Draw 2
    if(pDraw->uiMaskSrc&0x4)
    {
        if((er=pDraw->fpLock[2](&uiLockIdx,DISPSRV_LOCK_NEWEST))!=E_OK)
        {//locked fail indicate skip this draw
            return er;
        }
        pSrc = &pDraw->pDS[2]->pSrc[uiLockIdx];
        rcSrc.x = 0;
        rcSrc.y = 0;
        rcSrc.w = pSrc->Width;
        rcSrc.h = pSrc->Height;
        rcDst.x = 0;
        rcDst.y = pDst->Height/2;
        rcDst.w = pDst->Width/2;
        rcDst.h = pDst->Height/2;
        GxImg_ScaleData(pSrc,&rcSrc,pDst,&rcDst);
        pDraw->fpUnlock[2](uiLockIdx);
    }

    //Draw 3
    if(pDraw->uiMaskSrc&0x8)
    {
        if((er=pDraw->fpLock[3](&uiLockIdx,DISPSRV_LOCK_NEWEST))!=E_OK)
        {//locked fail indicate skip this draw
            return er;
        }
        pSrc = &pDraw->pDS[3]->pSrc[uiLockIdx];
        rcSrc.x = 0;
        rcSrc.y = 0;
        rcSrc.w = pSrc->Width;
        rcSrc.h = pSrc->Height;
        rcDst.x = pDst->Width/2;
        rcDst.y = pDst->Height/2;
        rcDst.w = pDst->Width/2;
        rcDst.h = pDst->Height/2;
        GxImg_ScaleData(pSrc,&rcSrc,pDst,&rcDst);
        pDraw->fpUnlock[3](uiLockIdx);
    }


    //--------------------Customer Draw End-------------------------------------

    pDraw->fpFlip(TRUE);

    return E_OK;
}


void PipView_InstallCmd(void)
{
}
