////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIFramework.h"
#include "GxImage.h"
#include "UIAppPhoto.h"
#include "NVTUserCommand.h"
#include "ImageApp_CamPhoto.h"
#include "UIPhotoMapping.h"
#include "UIPhotoInfo.h"
#include "UIDateImprint.h"
#include "UIAppCommon.h"
#include "UIAppPhoto.h"
#include "UIAppMovie.h" //for NVTEVT_EXE_MOVIEDIS, NVTEVT_EXE_MOVIEGDC, VTEVT_EXE_MOVIESMEAR
#include "UIInfo.h"
#include "UIMode.h"
#include "dis_alg_fw.h"

#include "UIStorageCheck.h"
//nclude "Demo_Sound.h"
//#include "FlowPanInit.h"
//#include "MenuDatetime.h"
#include "DateTimeInfo.h"
//#NT#2010/09/02#Niven Cho -begin
//#NT#NEW_DATE_IMPRINT
#include "UIDateImprint.h"
//#NT#2010/09/02#Niven Cho -end

#include "PStore.h"
#include "Sensor.h"
#include "DxSensor.h"
#include "UIControlWnd.h"
#include "UIBackgroundObj.h"


#include "DxOutput.h"
#include "FileSysTsk.h"
#include "GxFlash.h"
#include "DeviceCtrl.h"
#include "GxFlash.h"
#include "ae_task.h"
#include "awb_task.h"
#include "PrjCfg.h"
#include "SysMain.h"

//for IPL DZoom - end
#if LENS_FUNCTION
#include "Lens.h"
#include "af_api.h"
#include "AF_Lib.h"
#include "af_task.h"
#endif

#include "SoundData.h"
#include "SizeConvert.h"

#if (PIP_VIEW_FUNC == ENABLE)
#include "AppDisp_PipView.h"
#endif
#if (WIFI_AP_FUNC==ENABLE)
#include "WifiAppCmd.h"
#include "WifiAppXML.h"
#endif
#include "UIPhotoFuncInfo.h"
#include "LviewNvtAPI.h"
#include "lviewd.h"
#include "rsc_common.h"
#include "IPL_Display.h"

#include "ImageUnit_CamDisp.h"
#include "ImageUnit_NetHTTP.h"
#include "ImageUnit_UserDraw.h"
#include "ImageApp_NetPhoto.h"
#include "UCtrlAppPhoto.h"
#include "dma.h"
#include "MovieStamp.h"
#include "PrjCfg.h"
//#NT#2016/11/10#Niven Cho -begin
//#NT#AUTO_TEST
#include "MsdcNvtCb_Exam.h"
//#NT#2016/11/10#Niven Cho -end

#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UiAppPhoto
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
//#define __DBGFLT__          "[cap]"
//#define __DBGFLT__          "[cb]"
//#define __DBGFLT__          "[set]"
//#define __DBGFLT__          "[zoom]"
//#define __DBGFLT__          "[af]"
#include "DebugModule.h"

//---------------------UIAPP_PHOTO Global Variables----------

BOOL _g_bFirstPhoto = TRUE;

extern void System_DispSourceByTarget(ISIZE* pImgSize, USIZE* pAspectRatio);
extern UINT32 System_GetEnableDisp(void);
extern UINT32 System_GetEnableSensor(void);

#ifndef PHOTODISTSK_ENABLE
#define PHOTODISTSK_ENABLE   0
#endif

#define CFG_JPG_HEADER_SIZE (64*1024)       //16K
#define CFG_JPG_SCREENNAIL_SIZE (128*1024)  //128K

#if (_SCREENNAIL_SIZE_ == _SCREENNAIL_SIZE_VGA_)
#define    CFG_SCREENNAIL_SIZE  PHOTO_SIZE_VGA
#define    CFG_SCREENNAIL_W     640
#define    CFG_SCREENNAIL_H     480
#else
#define    CFG_SCREENNAIL_SIZE  PHOTO_SIZE_3M
#define    CFG_SCREENNAIL_W     2048
#define    CFG_SCREENNAIL_H     1536
#endif

static UINT32 gPhotoDzoomStop = 0;


UIAppPhotoExeInfoType UIAppPhotoExeInfoInitVal =
{
    0,                                //FreePicNum;
    FALSE,                            //isFolderFull;
    FALSE,                            //isCardFull;
    FALSE,                           //isFocusEnd
    FALSE,                           //isStartCapture
    FALSE,                           //isContShotStateSetOn;
    FALSE,                           //isStopingContShot
    FALSE,                           //isDoingContShot;
    FALSE,                           //isStartSmileDetect;
    0,                               //uiTakePicNum;
    FALSE,                           // isAscnOn;
    FALSE,                           // isCafOn;
    FALSE,                           // isSdOn;
    FALSE,                           // isFdOn;
    FALSE,                           // IsJPGok
    FALSE,                           // IsAFProcess;              //
    FALSE,                           //BDstatus
    FALSE,                           //IsCallBack
    UIAPPPHOTO_CB_MAX,               //CallBackEvent
    FALSE,                           //isPhotoFrameOn
    FALSE,                           //isFlashTrigger
    FALSE,                           //isSingleViewOpen
    FALSE,                           //isPrvPostProcess
    FALSE,                           //isAFBeam
    0,                               //uiMaxImageSize
    FALSE,                           //isDzoomStart
    UI_ZOOM_CTRL_STOP,               //ZoomCtrl
    FALSE,                           //isStartHighSpeedTone
    {0,0,640,480},                   //FdDispCoord
    0,                               //sensorCount
    0,                               //DualCam
};

static UIAppPhotoExeInfoType UIAppPhotoExeInfo;
static UIAppPhotoExeInfoType *localInfo = &UIAppPhotoExeInfo;
UIAppPhotoExeInfoType *pPhotoExeInfo = &UIAppPhotoExeInfo;

PhotoFrameInfoType      gPFInfo = {0};

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
#if (HTTP_LIVEVIEW_FUNC == ENABLE)
static USIZE HTTPLVIEW_SIZE[IMAGERATIO_MAX_CNT]=
{
    {HTTP_MJPG_W_4,HTTP_MJPG_H_3}, //IMAGERATIO_9_16
    {HTTP_MJPG_W_4,HTTP_MJPG_H_3}, //IMAGERATIO_2_3
    {HTTP_MJPG_W_4,HTTP_MJPG_H_3}, //IMAGERATIO_3_4
    {HTTP_MJPG_W_4,HTTP_MJPG_H_3}, //IMAGERATIO_1_1
    {HTTP_MJPG_W_4,HTTP_MJPG_H_3}, //IMAGERATIO_4_3
    {HTTP_MJPG_W_4,HTTP_MJPG_H_3}, //IMAGERATIO_3_2
    {HTTP_MJPG_W_16,HTTP_MJPG_H_9}, //IMAGERATIO_16_9
};
#endif
//static IPL_IME_INFOR         gImeInfo={0};
//static IPL_SET_IMGRATIO_DATA gIPLRatioInfo = {0};

//---------------------UIAPP_PHOTO Local Functions-------------------------
//return Primary Size or Header+Primary+Screennail Size
static UINT32 PhotoExe_GetExpectSize_RhoBRCrtl(UINT32 ImgIdx,BOOL bPrimaryOnly);

//#NT#2016/10/06#Lincy Lin -begin
//#NT# AE,AWB lock will auto control by capture flow or AF flow
#if 0
void AE_LOCK(BOOL bLock)
{
    if (localInfo->DualCam == DUALCAM_FRONT)
    {
        ImageUnit_SetParam(&ISF_ImagePipe1, IMAGEPIPE_PARAM_LOCKAE, (bLock));
    }
    else if(localInfo->DualCam == DUALCAM_BEHIND)
    {
        ImageUnit_SetParam(&ISF_ImagePipe2, IMAGEPIPE_PARAM_LOCKAE, (bLock));
    }
    else
    {
        ImageUnit_SetParam(&ISF_ImagePipe1, IMAGEPIPE_PARAM_LOCKAE, (bLock));
        ImageUnit_SetParam(&ISF_ImagePipe2, IMAGEPIPE_PARAM_LOCKAE, (bLock));
    }
}

void AWB_LOCK(BOOL bLock)
{
    if (localInfo->DualCam == DUALCAM_FRONT)
    {
        ImageUnit_SetParam(&ISF_ImagePipe1, IMAGEPIPE_PARAM_LOCKAWB, (bLock));
    }
    else if(localInfo->DualCam == DUALCAM_BEHIND)
    {
        ImageUnit_SetParam(&ISF_ImagePipe2, IMAGEPIPE_PARAM_LOCKAWB, (bLock));
    }
    else
    {
        ImageUnit_SetParam(&ISF_ImagePipe1, IMAGEPIPE_PARAM_LOCKAWB, (bLock));
        ImageUnit_SetParam(&ISF_ImagePipe2, IMAGEPIPE_PARAM_LOCKAWB, (bLock));
    }
}
#endif
//#NT#2016/10/06#Lincy Lin -end

void AF_LOCK(BOOL bLock)
{
    // AF just support AF_ID_1
    if(System_GetEnableSensor() & SENSOR_1)
        ImageUnit_SetParam(&ISF_ImagePipe1, IMAGEPIPE_PARAM_LOCKAF, (bLock));
}

void CAF_LOCK(BOOL bLock)
{
    // AF just support AF_ID_1
    if(System_GetEnableSensor() & SENSOR_1)
        ImageUnit_SetParam(&ISF_ImagePipe1, IMAGEPIPE_PARAM_LOCKCAF, (bLock));
}

BOOL CAF_ISLOCK(void)
{
    // AF just support AF_ID_1
    if(System_GetEnableSensor() & SENSOR_1)
        return ImageUnit_GetParam(&ISF_ImagePipe1, IMAGEPIPE_PARAM_LOCKCAF);
    else
        return FALSE;
}

void AF_WAIT_IDLE(void)
{
    // AF just support AF_ID_1
    if(System_GetEnableSensor() & SENSOR_1)
    {
        if (ImageUnit_GetParam(&ISF_ImagePipe1, IMAGEPIPE_PARAM_WAITAF) !=0)
			DBG_ERR("WAITAF\r\n");
    }
}
void PhotoExe_IPL_SetIInfo(UINT32 index, UINT32 value)
{
    if(System_GetEnableSensor() & SENSOR_1)
    {
        ImageUnit_SetParam(&ISF_ImagePipe1, index, value);
    }
    if(System_GetEnableSensor() & SENSOR_2)
    {
        ImageUnit_SetParam(&ISF_ImagePipe2, index, value);
    }
    if (index == IPL_SEL_SHDR)
    {
        DBG_IND("HDR %d\r\n",value);
    }
    else if (index == IPL_SEL_WDR)
    {
        DBG_IND("WDR %d\r\n",value);
    }
}
void PhotoExe_Cap_SetIInfo(UINT32 index, UINT32 value)
{
#if 0
    ImgCap_SetUIInfo(index,value);
#else
    ImageUnit_SetParam(&ISF_Cap, index, value);
#endif
}
void PhotoExe_RSC_SetSwitch(UINT32 index, UINT32 value)
{
#if (_IPPLIB_ != _IPL_FAKE_)
//#NT#2016/10/18#Jeah Yen -begin
//#NT#support sensor map
    // RSC just support IPL 1
    if(System_GetEnableSensor() & SENSOR_1)
        RSC_SetSwitch(IPL_PATH_1, index, value);
    //DBG_IND("RSC index = %d , value %d\r\n", index, value);
//#NT#2016/10/18#Jeah Yen -end
#endif
}

void PhotoExe_IPL_AlgSetFD_AEInfo(UINT32 value)
{
    if(System_GetEnableSensor() & SENSOR_1)
        ImageUnit_SetParam(&ISF_ImagePipe1,  IPL_SEL_FD, SEL_FD_OFF);
    if(System_GetEnableSensor() & SENSOR_2)
        ImageUnit_SetParam(&ISF_ImagePipe2,  IPL_SEL_FD, SEL_FD_OFF);
    if (localInfo->DualCam == DUALCAM_FRONT || localInfo->DualCam == DUALCAM_BOTH)
        ImageUnit_SetParam(&ISF_ImagePipe1,  IPL_SEL_FD, value);
    else
        ImageUnit_SetParam(&ISF_ImagePipe2,  IPL_SEL_FD, value);

}
//---------------------UIAPP_PHOTO MISC-------------------------


static ER PhotoExe_LensZoomIn(void)
{
    DBG_IND("[zoom]\r\n");
    #if LENS_FUNCTION
    return Lens_Zoom_In();
    #else
    return E_OK;
    #endif

}

static ER PhotoExe_LensZoomOut(void)
{
    DBG_IND("[zoom]\r\n");
    #if LENS_FUNCTION
    return Lens_Zoom_Out();
    #else
    return E_OK;
    #endif
}

static ER PhotoExe_LensZoomStop(void)
{
    DBG_IND("[zoom]\r\n");
    #if LENS_FUNCTION
    return Lens_Zoom_Stop();
    #else
    return E_OK;
    #endif
}

static ER PhotoExe_LensReset(void)
{
    DBG_IND("[zoom]\r\n");
    #if LENS_FUNCTION
    return Lens_Reset();
    #else
    return E_OK;
    #endif
}

UINT32 PhotoExe_GetMaxOzoomIdx(void)
{
    #if LENS_FUNCTION
    return Lens_Zoom_GetSection(ZOOM_MAX_SECTION);
    #else
    return 1;
    #endif
}

UINT32 PhotoExe_GetMinOzoomIdx(void)
{
    #if LENS_FUNCTION
    return Lens_Zoom_GetSection(ZOOM_MIN_SECTION);
    #else
    return 1;
    #endif
}


UINT32 PhotoExe_GetCurOzoomIdx(void)
{
    #if LENS_FUNCTION
    return Lens_Zoom_GetSection(ZOOM_CUR_SECTION);
    #else
    return 1;
    #endif
}

UINT32 PhotoExe_GetMaxDzoomIdx(void)
{
    UINT32 max;

    if (localInfo->DualCam == DUALCAM_FRONT || localInfo->DualCam == DUALCAM_BOTH)
    {
        ImageUnit_GetZoom_MaxIdx(&ISF_ImagePipe1, &max);
    }
    else
    {
        ImageUnit_GetZoom_MaxIdx(&ISF_ImagePipe2, &max);
    }
    return max;
}

UINT32 PhotoExe_GetDZoomIdx(void)
{
    UINT32 index;
    if (localInfo->DualCam == DUALCAM_FRONT || localInfo->DualCam == DUALCAM_BOTH)
    {
        ImageUnit_GetZoom_CurIdx(&ISF_ImagePipe1, &index);
    }
    else
    {
        ImageUnit_GetZoom_CurIdx(&ISF_ImagePipe2, &index);
    }
    return index;
}

UINT32 PhotoExe_GetDZoomRatio(void)
{
    UINT32 ratio;
    if (localInfo->DualCam == DUALCAM_FRONT || localInfo->DualCam == DUALCAM_BOTH)
    {
        ImageUnit_GetZoom_Ratio(&ISF_ImagePipe1, &ratio);
    }
    else
    {
        ImageUnit_GetZoom_Ratio(&ISF_ImagePipe2, &ratio);
    }
    return ratio;
}


static void PhotoExe_DZoomIn(void)
{
    gPhotoDzoomStop = FALSE;
    AppBKW_SetData(BKW_DZOOM_STOP,(UINT32)&gPhotoDzoomStop);
    BKG_PostEvent(NVTEVT_BKW_DZOOM_IN);
}

static void PhotoExe_DZoomOut(void)
{
    gPhotoDzoomStop = FALSE;
    AppBKW_SetData(BKW_DZOOM_STOP,(UINT32)&gPhotoDzoomStop);
    BKG_PostEvent(NVTEVT_BKW_DZOOM_OUT);
}

static void PhotoExe_DZoomStop(void)
{
    gPhotoDzoomStop = TRUE;
}


void PhotoExe_DZoomReset(void)
{
    if (localInfo->DualCam == DUALCAM_FRONT || localInfo->DualCam == DUALCAM_BOTH)
    {
        ImageUnit_SetZoom_CurIdx(&ISF_ImagePipe1, DZOOM_IDX_MIN());
    }
    else
    {
        ImageUnit_SetZoom_CurIdx(&ISF_ImagePipe2, DZOOM_IDX_MIN());
    }
}

void PhotoExe_DZoomInBK(UINT32* pIsDzoomStop)
{
    UINT32    index;
    index = DZOOM_IDX_GET();
    while (!(*pIsDzoomStop) && index < DZOOM_IDX_MAX())
    {
        if (localInfo->DualCam == DUALCAM_FRONT || localInfo->DualCam == DUALCAM_BOTH)
        {
            ImageUnit_SetZoom_CurIdx(&ISF_ImagePipe1, ++index);
        }
        else
        {
            ImageUnit_SetZoom_CurIdx(&ISF_ImagePipe2, ++index);
        }
    }
}

void PhotoExe_DZoomOutBK(UINT32* pIsDzoomStop)
{
    UINT32    index;
    index = DZOOM_IDX_GET();
    while(!(*pIsDzoomStop) && index > DZOOM_IDX_MIN())
    {
        if (localInfo->DualCam == DUALCAM_FRONT || localInfo->DualCam == DUALCAM_BOTH)
        {
            ImageUnit_SetZoom_CurIdx(&ISF_ImagePipe1, --index);
        }
        else
        {
            ImageUnit_SetZoom_CurIdx(&ISF_ImagePipe2, --index);
        }
    }
}


UINT32 PhotoExe_GetFdFaceNum(void)
{
    #if _FD_FUNC_
    return FD_GetRsltFaceNum();
    #else
    return 0;
    #endif
}


static USIZE PhotoExe_RatioSizeConvert(USIZE* devSize, USIZE* devRatio, USIZE* Imgratio)
{
    USIZE  resultSize=*devSize;

    if ((!devRatio->w) || (!devRatio->h))
    {
        DBG_ERR("devRatio w=%d, h=%d\r\n",devRatio->w,devRatio->h);
    }
    else if((!Imgratio->w) || (!Imgratio->h))
    {
        DBG_ERR("Imgratio w=%d, h=%d\r\n",Imgratio->w,Imgratio->h);
    }
    else
    {
        if (((float)Imgratio->w/Imgratio->h) >= ((float)devRatio->w/devRatio->h))
        {
            resultSize.w = devSize->w;
            resultSize.h = ALIGN_ROUND_4(devSize->h * Imgratio->h/Imgratio->w* devRatio->w/devRatio->h);
        }
        else
        {
            resultSize.h = devSize->h;
            resultSize.w = ALIGN_ROUND_16(devSize->w * Imgratio->w/Imgratio->h * devRatio->h/devRatio->w);
        }
    }
    return resultSize;
}

static UINT32 PhotoExe_GetScreenNailSize(void)
{
    UINT32           uiImageSize, ScreenNailSize;
    UINT32           BitStreamSize;


    ScreenNailSize = CFG_SCREENNAIL_SIZE;
    uiImageSize = UI_GetData(FL_PHOTO_SIZE);
    if(uiImageSize < ScreenNailSize)
    {
        BitStreamSize = CFG_SCREENNAIL_W * CFG_SCREENNAIL_H / 4;
        /*
        if (uiImageSize == SIZE_12M32)
        {
            BitStreamSize = BitStreamSize*2/3*4/3;

        }
        */
        #if 0
        if (uiImageSize == PHOTO_SIZE_2MHD)
        {
            BitStreamSize = BitStreamSize*9/16*4/3;
        }
        #endif
    }
    else
    {
        BitStreamSize = 0;
    }
    //#NT#2016/04/26#Lincy Lin -begin
    //#NT#Support sidebyside capture mode
    if (UI_GetData(FL_CONTINUE_SHOT) == CONTINUE_SHOT_SIDE)
    {
        BitStreamSize = BitStreamSize << 1;
    }
    //#NT#2016/04/26#Lincy Lin -end
    DBG_IND("[cap]ScreenNail BitStreamSize =%d K\r\n",BitStreamSize/1024);
    return BitStreamSize;
}


static void PhotoExe_SetScreenNailSize(void)
{
    UINT32           uiImageSize, ScreenNailSize;
    USIZE            devRatio={4,3};
    USIZE            ImageRatioSize={0},BufferSize={0};
    USIZE            ImgSize={0};
    UINT32           ImageRatioIdx,BitStreamSize;

    ScreenNailSize = CFG_SCREENNAIL_SIZE;
    uiImageSize = UI_GetData(FL_PHOTO_SIZE);
    if(uiImageSize < ScreenNailSize)
    {
        //BufferSize.w = GetPhotoSizeWidth(ScreenNailSize);
        //BufferSize.h = GetPhotoSizeHeight(ScreenNailSize);
        BufferSize.w = CFG_SCREENNAIL_W;
        BufferSize.h = BufferSize.w*3/4;

        ImageRatioIdx = GetPhotoSizeRatio(UI_GetData(FL_PHOTO_SIZE));
        ImageRatioSize = IMAGERATIO_SIZE[ImageRatioIdx];
        ImgSize = PhotoExe_RatioSizeConvert(&BufferSize,&devRatio,&ImageRatioSize);
        BitStreamSize = PhotoExe_GetScreenNailSize();

        PhotoExe_Cap_SetIInfo(CAP_SEL_SCREEN_IMG,SEL_SCREEN_IMG_ON);
        //#NT#2016/04/26#Lincy Lin -begin
        //#NT#Support sidebyside capture mode
        if (UI_GetData(FL_CONTINUE_SHOT) == CONTINUE_SHOT_SIDE)
        {
            ImgSize.w = ImgSize.w << 1;
        }
        //#NT#2016/04/26#Lincy Lin -end
        PhotoExe_Cap_SetIInfo(CAP_SEL_SCREEN_IMG_H_SIZE,ImgSize.w);
        PhotoExe_Cap_SetIInfo(CAP_SEL_SCREEN_IMG_V_SIZE,ImgSize.h);
        PhotoExe_Cap_SetIInfo(CAP_SEL_SCREEN_FMT, IPL_IMG_Y_PACK_UV422);
        PhotoExe_Cap_SetIInfo(CAP_SEL_SCREENBUFSIZE, BitStreamSize);
        DBG_IND("[cap]ScreenNail w=%d,h=%d, buffSize=%d k\r\n",ImgSize.w,ImgSize.h,BitStreamSize/1024);

    }
    else
    {
        PhotoExe_Cap_SetIInfo(CAP_SEL_SCREEN_IMG,SEL_SCREEN_IMG_OFF);
        PhotoExe_Cap_SetIInfo(CAP_SEL_SCREENBUFSIZE, 0);
        DBG_IND("[cap]ScreenNail None\r\n");
    }
}

static void PhotoExe_SetQuickViewSize(void)
{
    USIZE            devRatio={4,3};
    USIZE            ImageRatioSize={0},BufferSize={0};
    USIZE            ImgSize={0};
    UINT32           ImageRatioIdx;
    ISIZE            DevSize={0};

    DevSize = GxVideo_GetDeviceSize(DOUT1);
    #if (_QUICKVIEW_SIZE_ == _QUICKVIEW_SIZE_VGA_)
    {
        #if 0
        UINT32 QuickViewSize = SIZE_VGA;

        BufferSize.w = GetPhotoSizeWidth(QuickViewSize);
        BufferSize.h = GetPhotoSizeHeight(QuickViewSize);
        #else
        // fix bug: w/h size will be wrong if no VGA/3M resolution!
        BufferSize.w = CFG_SCREENNAIL_W;
        BufferSize.h = CFG_SCREENNAIL_H;
        #endif

        if (BufferSize.w > (UINT32)DevSize.w)
        {
            BufferSize.w = DevSize.w;
            BufferSize.h = BufferSize.w * devRatio.h/devRatio.w;
        }
    }
    #else
    {
        BufferSize.w = DevSize.w;
        BufferSize.h = BufferSize.w * devRatio.h/devRatio.w;
    }
    #endif

    ImageRatioIdx = GetPhotoSizeRatio(UI_GetData(FL_PHOTO_SIZE));
    ImageRatioSize = IMAGERATIO_SIZE[ImageRatioIdx];
    ImgSize = PhotoExe_RatioSizeConvert(&BufferSize,&devRatio,&ImageRatioSize);


    #if (_QUICKVIEW_SIZE_ != _QUICKVIEW_SIZE_VGA_)
    //#NT#2013/07/19#Lincy Lin -begin
    //#NT#Fine tune TV NTSC/PAL quick view display quality
    {

        SIZECONVERT_INFO     CovtInfo = {0};
        USIZE                dev1Ratio;
        USIZE                tmpImgSize={0};

        dev1Ratio = GxVideo_GetDeviceAspect(DOUT1);

        CovtInfo.uiSrcWidth  = ImgSize.w;
        CovtInfo.uiSrcHeight = ImgSize.h;
        CovtInfo.uiDstWidth  = DevSize.w;
        CovtInfo.uiDstHeight = DevSize.h;
        CovtInfo.uiDstWRatio = dev1Ratio.w;
        CovtInfo.uiDstHRatio = dev1Ratio.h;
        //#NT#2016/03/29#Lincy Lin -begin
        //#NT#Fix the photo quick view display error when image size is 16:9 with rotate panel
        CovtInfo.alignType = SIZECONVERT_ALIGN_FLOOR_16;
        //#NT#2016/03/29#Lincy Lin -end
        DisplaySizeConvert(&CovtInfo);

        DBG_IND("[cap]CovtInfo Srcw=%d,h=%d, Dstw=%d ,h=%d, Ratiow=%d ,h=%d, OutW=%d,h=%d\r\n",CovtInfo.uiSrcWidth,CovtInfo.uiSrcHeight,CovtInfo.uiDstWidth,CovtInfo.uiDstHeight,CovtInfo.uiDstWRatio,CovtInfo.uiDstHRatio,CovtInfo.uiOutWidth,CovtInfo.uiOutHeight);
        DBG_IND("[cap]QuickView w=%d,h=%d, DevSize w=%d ,h=%d\r\n",ImgSize.w,ImgSize.h,DevSize.w,DevSize.h);
        //#NT#2016/03/29#Lincy Lin -begin
        //#NT#Fix the photo quick view display error when image size is 16:9 with rotate panel
        if (ImgSize.h != CovtInfo.uiOutHeight)
        {
            tmpImgSize.h = CovtInfo.uiOutHeight;
            // image size should not exceed video size
            if (tmpImgSize.h > ImgSize.h)
            {
                tmpImgSize.h = ImgSize.h;
            }
            tmpImgSize.w = tmpImgSize.h * ImgSize.w/ImgSize.h;
            tmpImgSize.w = ALIGN_CEIL_16(tmpImgSize.w);
            ImgSize = tmpImgSize;
        }
        //#NT#2016/03/29#Lincy Lin -end
    }
    #endif
    //#NT#2013/07/19#Lincy Lin -end

    //#NT#2016/04/26#Lincy Lin -begin
    //#NT#Support sidebyside capture mode
    if (UI_GetData(FL_CONTINUE_SHOT) == CONTINUE_SHOT_SIDE)
    {
        ImgSize.w = ImgSize.w << 1;
    }
    //#NT#2016/04/26#Lincy Lin -end
    PhotoExe_Cap_SetIInfo(CAP_SEL_QV_IMG_H_SIZE,ImgSize.w);
    PhotoExe_Cap_SetIInfo(CAP_SEL_QV_IMG_V_SIZE,ImgSize.h);
    PhotoExe_Cap_SetIInfo(CAP_SEL_QV_FMT, IPL_IMG_Y_PACK_UV422);
    DBG_IND("[cap]QuickView w=%d,h=%d, DevSize w=%d ,h=%d\r\n",ImgSize.w,ImgSize.h,DevSize.w,DevSize.h);
}

//return Primary Size or Header+Primary+Screennail Size
static UINT32 PhotoExe_GetExpectSize_RhoBRCrtl(UINT32 ImgIdx,BOOL bPrimaryOnly)
{
    //#NT#2016/04/26#Lincy Lin -begin
    //#NT#Support sidebyside capture mode
    UINT32 BitstreamSize;

    if (UI_GetData(FL_CONTINUE_SHOT) == CONTINUE_SHOT_SIDE )
    {
        BitstreamSize = GetPhotoSizeWidth(ImgIdx) * 2 * GetPhotoSizeHeight(ImgIdx);
    }
    else
    {
        BitstreamSize = GetPhotoSizeWidth(ImgIdx) * GetPhotoSizeHeight(ImgIdx);
    }
    //#NT#2016/04/26#Lincy Lin -end
    switch(UI_GetData(FL_QUALITY))
    {
        case QUALITY_SUPER:
            BitstreamSize = (UINT32)(BitstreamSize*30/100);
            break;

        case QUALITY_FINE:
            BitstreamSize = (UINT32)(BitstreamSize*25/100);
            break;

        case QUALITY_NORMAL:
        default:
            BitstreamSize = (UINT32)(BitstreamSize*20/100);
    }

    if(!bPrimaryOnly)
        BitstreamSize += CFG_JPG_HEADER_SIZE+PhotoExe_GetScreenNailSize();

    return BitstreamSize;

}

//
UINT32 PhotoExe_GetFreePicNum(void)
{
    #if 1
    UINT64              uiFreeSpace;
    UINT32              uiMaxImageSize;
    UINT32              freeImgNum;
    UINT32              reserveSize = 0x80000; //  500KB
    UINT32              CaptureSize, showFreeSize;
    UINT32              size;

    #if (FILESIZE_ALIGN_FUNC)
    reserveSize += FS_ALIGN_RESERVED_SIZE;
    #endif
    uiFreeSpace = FileSys_GetDiskInfo(FST_INFO_FREE_SPACE);
    showFreeSize = uiFreeSpace/1024;

    DBG_IND("[cap]Free Space = %d KB\r\n", showFreeSize);

    size = UI_GetData(FL_PHOTO_SIZE);

    CaptureSize = PhotoExe_GetExpectSize_RhoBRCrtl(size,FALSE);
    DBG_IND("[cap]CaptureSize = %d K \r\n", CaptureSize/1024);

    if(uiFreeSpace < reserveSize)
        freeImgNum = 0;
    else
        freeImgNum = (uiFreeSpace - reserveSize)/CaptureSize;

    DBG_IND("[cap]Free ImgNum = %d\r\n", freeImgNum);

    uiMaxImageSize = CaptureSize + reserveSize;
    //uiMaxImageSize = CaptureSize+(CaptureSize*15/100); //+15%

    localInfo->uiMaxImageSize = uiMaxImageSize;

    DBG_IND("[cap]uiMaxImageSize = %ld k\r\n", uiMaxImageSize/1024);

    //#NT#store in info
    localInfo->FreePicNum = freeImgNum;
    if (localInfo->FreePicNum > 0)
    {
        localInfo->isCardFull = FALSE;
    }
    return freeImgNum;
    #else
    return 100;
    #endif
}



//#NT#2010/09/03#Niven Cho -begin
//#NT#NEW_DATE_IMPRINT
//void PhotoExe_DateImprint(void)
//#NT#2010/09/03#Niven Cho -end


static BOOL PhotoExe_CheckBD(void)
{
    return FALSE;
}


static void PhotoExe_CallBackUpdateInfo(UINT32 callBackEvent)
{
    VControl *pCurrnetWnd;

    Ux_GetFocusedWindow(&pCurrnetWnd);
    localInfo->IsCallBack = TRUE;
    localInfo->CallBackEvent = callBackEvent;
    //NVTEVT_UPDATE_INFO
    Ux_SendEvent(pCurrnetWnd,NVTEVT_UPDATE_INFO, 1, localInfo->CallBackEvent);
    // callback to uctrl
    #if(UCTRL_FUNC==ENABLE)
    UCtrlAppPhoto_CallBack(callBackEvent);
    #endif
    localInfo->IsCallBack = FALSE;
}


static void PhotoExe_StopContShot(void)
{
    if (localInfo->isStopingContShot == FALSE)
    {
        localInfo->isStopingContShot = TRUE;
        ImageApp_CamPhoto_CapStop();
        PhotoExe_CallBackUpdateInfo(UIAPPPHOTO_CB_STOP_CONTSHOT);
    }
}


ISIZE Photo_GetBufferSize(void) //return USER CUSTOM buffer size
{
    return GxVideo_GetDeviceSize(DOUT1);
}


void PhotoExe_CfgNetHttpPIP(USIZE HttpLviewSize,UINT32 dualCam)
{
    #if (PIP_VIEW_FUNC == ENABLE)
    // config pip view parameter
    {
        PIP_VIEW_INFO   PipviewInfo = {0};

        if(System_GetEnableSensor() == (SENSOR_1|SENSOR_2))
        {
            PipviewInfo.isEnablePIP = TRUE;
            PipviewInfo.rect2nd.x = 0;
            PipviewInfo.rect2nd.y = 0;
            PipviewInfo.rect2nd.w = HttpLviewSize.w/2;
            PipviewInfo.rect2nd.h = HttpLviewSize.h/2;
            #if (PIP_VIEW_LR == ENABLE)
            if (dualCam > DUALCAM_BOTH2)
            {
                PipviewInfo.dualcam = DUALCAM_BOTH;
            }
            else
            #endif
            {
                PipviewInfo.dualcam = dualCam;
            }
        }
        ImageUnit_SetParam(&ISF_NetHTTP, NETHTTP_PARAM_PIP_VIEW, (UINT32)&PipviewInfo);
    }
    #endif

}

//#NT#2016/03/16#Lincy Lin -begin
//#NT#Support HTTP liveview datastamp function
#if (HTTP_LIVEVIEW_DATASTAMP_FUNC == ENABLE)
#define HTTP_PATH_ID  2

static UINT32 NetHTTP_DataStampCB(UINT32 yAddr, UINT32 cbAddr, UINT32 crAddr, UINT32 lineY)
{
    MovieStamp_CopyData(HTTP_PATH_ID, yAddr, cbAddr, crAddr, lineY, lineY);
    return 0;
}
#endif
//#NT#2016/03/16#Lincy Lin -end

void PhotoExe_CfgNetHttp(USIZE HttpLviewSize)
{
    // config encoder parameter
    {
        LVIEWNVT_INFO      LViewEncodeInfo = {0};

        LViewEncodeInfo.targetSize      = (1000*1024);
        LViewEncodeInfo.frameRate       = 30;
        LViewEncodeInfo.imgWidth        = HttpLviewSize.w;
        LViewEncodeInfo.imgHeight       = HttpLviewSize.h;
        LViewEncodeInfo.MJPGQueueFrameNum = 2;
        //#NT#2016/03/16#Lincy Lin -begin
        //#NT#Support HTTP liveview datastamp function
        #if (HTTP_LIVEVIEW_DATASTAMP_FUNC == ENABLE)
        LViewEncodeInfo.fpDrawCB        = NetHTTP_DataStampCB;
        #endif
        ImageUnit_SetParam(&ISF_NetHTTP, NETHTTP_PARAM_ENCODER, (UINT32)&LViewEncodeInfo);
    }
    // config daemon parameter
    {
        LVIEWNVT_DAEMON_INFO   DaemonInfo = {0};

        DaemonInfo.portNum = 8192;
        // set http live view server thread priority
        DaemonInfo.threadPriority = 6;
        // set the maximum JPG size that http live view server to support
        DaemonInfo.maxJpgSize = 204800;
        // live view streaming frame rate
        DaemonInfo.frameRate = 30;
        // socket buffer size
        DaemonInfo.sockbufSize = 40960;
		// if want to use https
        DaemonInfo.is_ssl = 0;
        ImageUnit_SetParam(&ISF_NetHTTP, NETHTTP_PARAM_DAEMON, (UINT32)&DaemonInfo);
    }
    //#NT#2016/03/16#Lincy Lin -begin
    //#NT#Support HTTP liveview datastamp function
    #if (HTTP_LIVEVIEW_DATASTAMP_FUNC == ENABLE)
    // config data stamp
    if (UI_GetData(FL_MOVIE_DATEIMPRINT) == MOVIE_DATEIMPRINT_ON)
    {
        UINT32      uiStampAddr;
        UINT32      uiWidth  = HttpLviewSize.w;
        UINT32      uiHeight = HttpLviewSize.h;
        STAMP_COLOR StampColorBg = {RGB_GET_Y( 16,  16,  16), RGB_GET_U( 16,  16,  16), RGB_GET_V( 16,  16,  16)}; // date stamp background color
        STAMP_COLOR StampColorFr = {RGB_GET_Y( 16,  16,  16), RGB_GET_U( 16,  16,  16), RGB_GET_V( 16,  16,  16)}; // date stamp frame color
        STAMP_COLOR StampColorFg = {RGB_GET_Y(224, 224, 192), RGB_GET_U(224, 224, 192), RGB_GET_V(224, 224, 192)}; // date stamp foreground color

        // use POOL_ID_DATEIMPRINT as movie data stamp buffer
        uiStampAddr = OS_GetMempoolAddr(POOL_ID_DATEIMPRINT);

        //#NT#2016/07/22#YongChang Qui -begin
        //#NT#MovieStamp uses unified memory as IPC & MovieStamp buffer for IPCam with AE_LOG off
        #if !(IPCAM_FUNC && !MOVIE_AE_LOG)
        MovieStamp_SetBuffer(HTTP_PATH_ID, uiStampAddr + POOL_SIZE_DATEIMPRINT*3/4, POOL_SIZE_DATEIMPRINT/4);
        #endif
        //#NT#2016/07/22#YongChang Qui -end

        MovieStamp_SetColor(HTTP_PATH_ID, &StampColorBg, &StampColorFr, &StampColorFg);


        MovieStamp_Setup(
            HTTP_PATH_ID,
            STAMP_ON |
            STAMP_AUTO |
            STAMP_DATE_TIME |
#if MOVIE_AE_LOG || MOVIE_ADAS_LOG
            STAMP_TOP_LEFT |
#else
            STAMP_BOTTOM_RIGHT |
#endif
            STAMP_POS_NORMAL |
            STAMP_BG_TRANSPARENT |
            STAMP_YY_MM_DD |
            STAMP_IMG_420UV,
            uiWidth,
            uiHeight,
#if defined (WATERLOGO_FUNCTION) && (WATERLOGO_FUNCTION == ENABLE)
            (WATERLOGO_BUFFER *)&g_WaterLogo);
#else
            NULL);
#endif
       // enable movie stamp
       MovieStamp_Enable();
    }
    #endif
    //#NT#2016/03/16#Lincy Lin -end
    PhotoExe_CfgNetHttpPIP(HttpLviewSize,UI_GetData(FL_DUAL_CAM));

	//#NT#2016/09/21#Hideo Lin -begin
	//#NT#Set HTTP live view streaming maximum size
	ImageUnit_SetParam(&ISF_NetHTTP, NETHTTP_PARAM_MAX_IMAGE_WIDTH1, (UINT32)HttpLviewSize.w);
	ImageUnit_SetParam(&ISF_NetHTTP, NETHTTP_PARAM_MAX_IMAGE_HEIGHT1, (UINT32)HttpLviewSize.h);
	ImageUnit_SetParam(&ISF_NetHTTP, NETHTTP_PARAM_MAX_IMAGE_WIDTH2, (UINT32)HttpLviewSize.w);
	ImageUnit_SetParam(&ISF_NetHTTP, NETHTTP_PARAM_MAX_IMAGE_HEIGHT2, (UINT32)HttpLviewSize.h);
	//#NT#2016/09/21#Hideo Lin -end
}




/**
  Initialize application for Photo mode

  Initialize application for Photo mode.

  @param void
  @return void
*/
INT32 PhotoExe_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    MEM_RANGE Pool;
#if(WIFI_AP_FUNC==ENABLE)
#if (XML_USE_APP_BUFFER==ENABLE)
    MEM_RANGE xmlPool;
#endif
#endif
    USIZE            ImageRatioSize={0};
    UINT32           ImageRatioIdx;
    UINT32 useFileDB=0;
    PHOTO_FUNC_INFO* phead = NULL;
    //#NT#2016/06/17#Brain Yen -begin
    //#NT# for special aspect panel and pipview
    #if (PIP_VIEW_LR == ENABLE || _LCDTYPE_ == _LCDTYPE_TG078UW006A0_DSI_)
    USIZE DeviceRatioSize= GxVideo_GetDeviceAspect(DOUT1);
    #endif
    //#NT#2016/06/17#Brain Yen -end
    DBG_IND("\r\n");

    //call default
    Ux_DefaultEvent(pCtrl,NVTEVT_EXE_OPEN,paramNum,paramArray);

#if(WIFI_AP_FUNC==ENABLE)
    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
    {
        #if (XML_USE_APP_BUFFER==ENABLE)
        xmlPool = AppMem_Alloc("XML", POOL_SIZE_TEMP);
        XML_SetTempMem(xmlPool.Addr,xmlPool.Size);
        #endif
        Pool = AppMem_Alloc("ImageStream", APPMEM_ALL);
        ImageApp_NetPhoto_Config(NETPHOTO_CFG_POOL, (UINT32)&Pool);
    }
    else
#endif
    {
        Pool = AppMem_Alloc("ImageStream", APPMEM_ALL);
        ImageApp_CamPhoto_Config(PHOTO_CFG_POOL, (UINT32)&Pool);
    }

    phead = &PhotoFuncInfo_dummy;
    phead->pNext = NULL; //mark end
    if(1)
    {
        PHOTO_FUNC_INFO* pfunc = &PhotoFuncInfo_dis;
        PHOTO_FUNC_INFO* pcurr = 0;
        pcurr = phead; while(pcurr->pNext) pcurr = pcurr->pNext; //find last func
        pcurr->pNext = pfunc; //append this func
        pfunc->pNext = NULL; //mark end
    }
    #if _FD_FUNC_
    {
        UINT32      myDualCam;
        PHOTO_FUNC_INFO* pfunc = &PhotoFuncInfo_fd;
        PHOTO_FUNC_INFO* pcurr = 0;
        pcurr = phead; while(pcurr->pNext) pcurr = pcurr->pNext; //find last func
        pcurr->pNext = pfunc; //append this func
        pfunc->pNext = NULL; //mark end

        myDualCam = Photo_getDualCamSetting();

        //#NT#2016/10/18#Jeah Yen -begin
        //#NT#support sensor map
        if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH)
            PhotoFuncInfo_fd.Info.IPLId = IPL_PATH_1;
        else
            PhotoFuncInfo_fd.Info.IPLId = IPL_PATH_2;
        //#NT#2016/10/18#Jeah Yen -end

        DBG_IND("myDualCam = %d, FD IPLID = %d\r\n",myDualCam,PhotoFuncInfo_fd.Info.IPLId);
    }
    // Register FD callback func
    {
        URECT fdDispCoord={0};
        //FD_RegisterCB(Photo_FdCB);
        ISIZE dev1size;
        //1.get current device size (current mode)
        dev1size = GxVideo_GetDeviceSize(DOUT1);
        // Set FD display coordinate
        fdDispCoord.w = dev1size.w;
        fdDispCoord.h = dev1size.h;
        //FD_SetDispCoord(&fdDispCoord);
        localInfo->FdDispCoord = fdDispCoord;
    }
    #endif
    #if _SD_FUNC_
    {
        PHOTO_FUNC_INFO* pfunc = &PhotoFuncInfo_sd;
        PHOTO_FUNC_INFO* pcurr = 0;
        pcurr = phead; while(pcurr->pNext) pcurr = pcurr->pNext; //find last func
        pcurr->pNext = pfunc; //append this func
        pfunc->pNext = NULL; //mark end
    }
    //SD_RegisterCB(Photo_SdCB);
    #endif
    #if _FD_FUNC_
    ImageUnit_SetParam(&ISF_ImagePipe, PHOTO_PARAM_FUNCINFO, (UINT32)phead);
    ImageUnit_SetParam(&ISF_ImagePipe, PHOTO_PARAM_CACHESIZE, 2000*1024);
    #else
    ImageUnit_SetParam(&ISF_ImagePipe, PHOTO_PARAM_FUNCINFO, (UINT32)phead);
    ImageUnit_SetParam(&ISF_ImagePipe, PHOTO_PARAM_CACHESIZE, 0);
    #endif

    //set photo function
    {
        IPL_POST_PROC_FUNC_EN Func1_En = IPL_FUNC_NONE;
        IPL_POST_PROC_FUNC_EN Func2_En = IPL_FUNC_NONE;
        // NOTE: WDR & SHDR is only one can active at the same time
        #if WDR_FUNC
        if (UI_GetData(FL_WDR) == WDR_ON || UI_GetData(FL_SHDR) == SHDR_OFF)
        {
            Func1_En |= IPL_FUNC_WDR;
            Func2_En |= IPL_FUNC_WDR;
        }
        PhotoExe_IPL_SetIInfo(IPL_SEL_WDR,Get_WDRValue(UI_GetData(FL_WDR)));
        #endif
        #if SHDR_FUNC
        if (UI_GetData(FL_SHDR) == SHDR_ON && UI_GetData(FL_WDR) == WDR_OFF)
        {
            Func1_En |= IPL_FUNC_SHDR;
            Func2_En |= IPL_FUNC_SHDR;
        }
        else
        {
            UI_SetData(FL_SHDR,SHDR_OFF);
        }
        PhotoExe_IPL_SetIInfo(IPL_SEL_SHDR,Get_SHDRValue(UI_GetData(FL_SHDR)));
        #endif
        ImageUnit_SetParam(&ISF_ImagePipe, PHOTO_PARAM_FUNC1, Func1_En);
        ImageUnit_SetParam(&ISF_ImagePipe, PHOTO_PARAM_FUNC2, Func2_En);
    }

	#if (_MODEL_DSC_ == _MODEL_TWIN_360_)
	IPL_AlgSetUIInfo(0, IPL_SEL_SBS, SEL_SBS_LR);
	IPL_AlgSetUIInfo(1, IPL_SEL_SBS, SEL_SBS_LR);
	#endif

    #if USE_FILEDB
    UI_SetData(FL_IsUseFileDB, 1);
    #else
    UI_SetData(FL_IsUseFileDB, 0);
    #endif

    useFileDB = UI_GetData(FL_IsUseFileDB);
    if (useFileDB)
    {
        Pool.Addr = dma_getCacheAddr(OS_GetMempoolAddr(POOL_ID_FILEDB));
        Pool.Size = POOL_SIZE_FILEDB;
        ImageUnit_SetParam(&ISF_CamFile, CAMFILE_PARAM_USE_FILEDB, (UINT32)&Pool);
        // config FDB root
#if (CUSTOM_NAMING_RULE == DISABLE)
        // sample code for FileDB Naming Rule

        // disable custom naming
        ImageUnit_SetParam(&ISF_CamFile, CAMFILE_PARAM_CUSTOM_NAMING, FALSE);

        // set root folder
        ImageApp_CamFile_SetFolder(FILEDB_CARDV_ROOT);

        // set folder name of movie, RO, and photo
        //ImageApp_CamFile_SetPhotoFolder(0, "PHOTO");

#else // sample code for Custom Naming Rule

        //////////////////////////////////////////////////////////////////////////////////////
        // NOTE! IMPORTANT! Custom Naming Rule only support 1 sensor for Photo Capture!     //
        // Since Photo Capture cannot distinguish path 1 and path 2 now, so the file name   //
        // of path 1 and path 2 may be the same, that's why it cannot suport 2 sensors now! //
        //////////////////////////////////////////////////////////////////////////////////////

        // enable custom naming
        ImageUnit_SetParam(&ISF_CamFile, CAMFILE_PARAM_CUSTOM_NAMING, TRUE);

        #if 1

        // set root folder
        #if (FS_MULTI_STRG_FUNC == DISABLE)
        ImageApp_CamFile_SetRootFolder("A:\\CarDV\\");
        #else
        ImageApp_CamFile_SetRootFolderByPath(0, "A:\\CarDV\\");
        ImageApp_CamFile_SetRootFolderByPath(1, "B:\\CarDV\\");
        #endif

        // set folder name of 1st photo path
        ImageApp_CamFile_SetPhotoFolder(0, "Photo");

        // set folder name of 2nd photo path
        ImageApp_CamFile_SetPhotoFolder(1, "Photo");

        ImageApp_CamFile_SetFileNamingMethod(FILE_NAMING_SEMIAUTO);
        ImageApp_CamFile_SetFileNameEndChar(0, ENABLE, "A");
        ImageApp_CamFile_SetFileNameEndChar(1, ENABLE, "B");

        #else

        // set root folder
        ImageApp_CamFile_SetRootFolder("A:\\CarDV\\");

        // set folder name of 1st photo path
        ImageApp_CamFile_SetPhotoFolder(0, "Image_Front");

        // set folder name of 2nd photo path
        ImageApp_CamFile_SetPhotoFolder(1, "Image_Rear");

        // set custom file name
        #if 0 // manual naming method
        {
            ImageApp_CamFile_SetFileNamingMethod(FILE_NAMING_MANUAL);
            // register callback function for manual file naming method
            ImageApp_CamFile_RegPhotoFileNamingCB(UIMovie_PhotoFileNamingCB);
        }
        #elif 1 // semi-auto naming method
        {
            switch(SysGetFlag(FL_DATE_FORMAT))
            {
            case DATE_FORMAT_DMY:
                ImageApp_CamFile_SetFileNamingYMD(FILE_NAMING_DATEFMT_DMY);
                break;
            case DATE_FORMAT_MDY:
                ImageApp_CamFile_SetFileNamingYMD(FILE_NAMING_DATEFMT_MDY);
                break;
            case DATE_FORMAT_YMD:
            default:
                ImageApp_CamFile_SetFileNamingYMD(FILE_NAMING_DATEFMT_YMD);
                break;
            }
            ImageApp_CamFile_SetFileNamingMethod(FILE_NAMING_SEMIAUTO);
            ImageApp_CamFile_SetFileNamingType(FILE_NAMING_SEMIAUTO_TYPE2);
            ImageApp_CamFile_SetFileNameEndChar(0, ENABLE, "_FR");
            ImageApp_CamFile_SetFileNameEndChar(1, ENABLE, "_RE");
            ImageApp_CamFile_SetFileNameSerialNum(0, DISABLE);
            ImageApp_CamFile_SetFileNameSerialNum(1, DISABLE);
        }
        #else // auto naming method => need to do nothing
        #endif

        #endif

#endif
    }
    else
    {
        ImageUnit_SetParam(&ISF_CamFile, CAMFILE_PARAM_USE_FILEDB, 0);
    }
    //#NT#2016/05/31#Lincy Lin -begin
    //#NT#Support fileSize align function
    #if FILESIZE_ALIGN_FUNC
    ImageUnit_SetParam(&ISF_CamFile, CAMFILE_PARAM_FILESIZE_ALIGN, PHOTO_FILESIZE_ALIGN_VALUE);
    #endif
    //#NT#2016/05/31#Lincy Lin -end
    // register Callback function
    Photo_RegCB();
    {
        if(System_GetEnableSensor() == (SENSOR_1))
            localInfo->sensorCount = 1;
        else if(System_GetEnableSensor() == (SENSOR_2))
            localInfo->sensorCount = 1;
        else if(System_GetEnableSensor() == (SENSOR_1|SENSOR_2))
            localInfo->sensorCount = 2;
        else if(System_GetEnableSensor() == (SENSOR_1|SENSOR_2|SENSOR_3|SENSOR_4))
            localInfo->sensorCount = 4;
        DBG_IND("sensorCount = %d\r\n",localInfo->sensorCount);
        // load sensor display option
        #if (SENSOR_CAPS_COUNT == 1)
        if(System_GetEnableSensor() == (SENSOR_1) )
            localInfo->DualCam = DUALCAM_FRONT;
        else
            localInfo->DualCam = DUALCAM_BEHIND;
        #endif
        #if (SENSOR_CAPS_COUNT == 2)
        if(localInfo->sensorCount < 2)
        {
            if(System_GetEnableSensor() == (SENSOR_1) )
                localInfo->DualCam = DUALCAM_FRONT;
            else
                localInfo->DualCam = DUALCAM_BEHIND;
        }
        else
        {
            localInfo->DualCam = UI_GetData(FL_DUAL_CAM);
        }
        #endif
		#if (PIP_VIEW_FASTSWITCH==ENABLE)
        PipView_SetStyle(UI_GetData(FL_DUAL_CAM));
		#endif
        DBG_IND("localInfo->DualCam = %d\r\n",localInfo->DualCam);
    }
    ImageRatioIdx = GetPhotoSizeRatio(UI_GetData(FL_PHOTO_SIZE));
    ImageRatioSize = IMAGERATIO_SIZE[ImageRatioIdx];
//#NT#2016/02/22#Jeah Yen -begin
//#NT#Support IPL direction function
    if(System_GetEnableSensor() & SENSOR_1)
    {
        ImageUnit_Begin(&ISF_ImagePipe1, 0);
            ImageUnit_CfgAspect(ISF_IN1, ImageRatioSize.w, ImageRatioSize.h);
            //#NT#2016/04/06#Lincy Lin -begin
            //#NT#Use chip flip instead of sensor flip
            ImageUnit_CfgDirect(ISF_IN1, (UI_GetData(FL_MOVIE_SENSOR_ROTATE) == SEN_ROTATE_ON)? FLIP_V : FLIP_NONE);
            //#NT#2016/04/06#Lincy Lin -end
        ImageUnit_End();
    }
    if(System_GetEnableSensor() & SENSOR_2)
    {
        ImageUnit_Begin(&ISF_ImagePipe2, 0);
            ImageUnit_CfgAspect(ISF_IN1, ImageRatioSize.w, ImageRatioSize.h);
            ImageUnit_CfgDirect(ISF_IN1, 0);
        ImageUnit_End();
    }
//#NT#2016/02/22#Jeah Yen -end

    if (System_GetEnableDisp() & DISPLAY_1)
    {
        ImageUnit_Begin(&ISF_CamDisp, 0);
            ImageUnit_CfgImgSize(ISF_IN1, 0, 0); //buffer size = full device size
            //#NT#2016/06/17#Brain Yen -begin
            //#NT#for special aspect panel and pipview
            #if (PIP_VIEW_LR == ENABLE || _LCDTYPE_ == _LCDTYPE_TG078UW006A0_DSI_)
            ImageUnit_CfgAspect(ISF_IN1, DeviceRatioSize.w, DeviceRatioSize.h);
			#elif (SBS_VIEW_FUNC == ENABLE)
            ImageUnit_CfgAspect(ISF_IN1, ImageRatioSize.w << 1, ImageRatioSize.h);
            #else
            ImageUnit_CfgAspect(ISF_IN1, ImageRatioSize.w, ImageRatioSize.h);
            #endif
            //#NT#2016/06/17#Brain Yen -end
            ImageUnit_CfgWindow(ISF_IN1, 0, 0, 0, 0);  //window range = full device range
        ImageUnit_End();
    }
    if (System_GetEnableDisp() & DISPLAY_2)
    {
        ImageUnit_Begin(&ISF_CamDisp, 0);
            ImageUnit_CfgImgSize(ISF_IN2, 0, 0); //buffer size = full device size
			#if (SBS_VIEW_FUNC == ENABLE)
            ImageUnit_CfgAspect(ISF_IN2, ImageRatioSize.w << 1, ImageRatioSize.h);
			#else
            ImageUnit_CfgAspect(ISF_IN2, ImageRatioSize.w, ImageRatioSize.h);
			#endif
            ImageUnit_CfgWindow(ISF_IN2, 0, 0, 0, 0);  //window range = full device range
        ImageUnit_End();
    }
    //Set UserDraw Size
    {
        ISIZE devSize = GxVideo_GetDeviceSize(DOUT1);
        System_DispSourceByTarget(&devSize, &ImageRatioSize);
    }

    #if (HTTP_LIVEVIEW_FUNC == ENABLE)
    {
        USIZE HttpLviewSize;
        HttpLviewSize = HTTPLVIEW_SIZE[ImageRatioIdx];
        ImageUnit_Begin(&ISF_NetHTTP, 0);
            ImageUnit_CfgAspect(ISF_IN1, ImageRatioSize.w, ImageRatioSize.h);
            ImageUnit_CfgImgSize(ISF_IN1, HttpLviewSize.w, HttpLviewSize.h);
            ImageUnit_CfgAspect(ISF_IN2, ImageRatioSize.w, ImageRatioSize.h);
            ImageUnit_CfgImgSize(ISF_IN2, HttpLviewSize.w, HttpLviewSize.h);
        ImageUnit_End();
        PhotoExe_CfgNetHttp(HttpLviewSize);
    }
    #endif

    //open
#if (MOVIE_DIS == ENABLE)
    // force to disable DIS
    PhotoExe_IPL_SetIInfo(IPL_SEL_DISCOMP, SEL_DISCOMP_OFF);
#endif

    #if (_ADAS_FUNC_)
    //dis_setDisViewRatio(1000);
    //PhotoExe_IPL_SetIInfo(IPL_SEL_DISCOMP, SEL_DISCOMP_ON);
    #endif

    if(UI_GetData(FL_RSC) == RSC_ON )
        PhotoExe_RSC_SetSwitch(SEL_RSC_CHANGEMODE,SEL_RSC_ON);
    else
        PhotoExe_RSC_SetSwitch(SEL_RSC_CHANGEMODE,SEL_RSC_OFF);

    #if HTTP_LIVEVIEW_FUNC
    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
    {
        if(_g_bFirstPhoto){TM_BOOT_BEGIN("sensor","ipl_open");}
        ImageApp_NetPhoto_Open();
        //WifiCmd_Done(WIFIFLAG_MODE_DONE,E_OK);
        Ux_PostEvent(NVTEVT_WIFI_EXE_MODE_DONE,1,E_OK);

    }
    else
    #endif
    {
        if(_g_bFirstPhoto){TM_BOOT_BEGIN("sensor","ipl_open");}
        ImageApp_CamPhoto_Open();
    }
    #if UCTRL_FUNC
    UCtrlAppPhoto_Open();
    #endif
    DBG_FUNC_END("");

//#NT#2016/03/18#Niven Cho -begin
//#NT#improve the using of gprof
#if (_GPROF_TOOL_ == _GPROF_TOOL_ON_)
    //take the place of SxCmd Temp Buffer after ImageApp_Open, becasue it has changed in Open()
    SxCmd_RegTempMemFunc(System_GetGprofBuffer);
#endif
//#NT#2016/03/18#Niven Cho -end

    return NVTEVT_CONSUME;
}


INT32 PhotoExe_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    if (ImageUnit_GetParam(&ISF_Cap, CAP_PARAM_WAITTSK) != 0)
        DBG_ERR("WAITTSK\r\n");
    #if HTTP_LIVEVIEW_FUNC
    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
    {
        ImageApp_NetPhoto_Close();
    }
    else
    #endif
    {
        ImageApp_CamPhoto_Close();
    }
    #if _SD_FUNC_
    SD_UnInit();
    #endif
    #if _FD_FUNC_
    FD_Lock(TRUE);
    FD_UnInit();
    #endif

    //call default
    Ux_DefaultEvent(pCtrl,NVTEVT_EXE_CLOSE,paramNum,paramArray);
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnMacro(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    DBG_IND("%d \r\n",paramArray[0]);
    if(paramNum>0)
        uhSelect= paramArray[0];

    UI_SetData(FL_MACRO,uhSelect);
    PhotoExe_IPL_SetIInfo(IPL_SEL_AFMODE,Get_MacroValue(uhSelect));
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnSelftimer(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    DBG_IND("%d \r\n",paramArray[0]);
    if(paramNum>0)
        uhSelect= paramArray[0];

    UI_SetData(FL_SELFTIMER,uhSelect);
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnFlash(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;

    DBG_IND("%d \r\n",paramArray[0]);
    if(paramNum>0)
        uhSelect= paramArray[0];

    UI_SetData(FL_FLASH_MODE,uhSelect);
    Photo_SetUserIndex(PHOTO_USR_FLASH,uhSelect);
    if (UI_GetData(FL_IsInitSettingFlag) == FALSE)
    {
        if(uhSelect != FLASH_OFF)
        {
            //#NT#Hera14 HW bug , no battery insert can't charge for flash
            if (!UI_GetData(FL_IsStopCharge))
            {
                GxFlash_StartCharge();
            }
        }
        else
        {
            GxFlash_StopCharge();
        }
    }
    PhotoExe_IPL_SetIInfo(IPL_SEL_FLASHMODE,Get_FlashValue(uhSelect));
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnEV(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    DBG_IND("%d \r\n",paramArray[0]);
    if(paramNum>0)
        uhSelect= paramArray[0];

    UI_SetData(FL_EV,uhSelect);
    Photo_SetUserIndex(PHOTO_USR_EV,uhSelect);
    PhotoExe_IPL_SetIInfo(IPL_SEL_AEEV,Get_EVValue(uhSelect));
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnCaptureSize(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    DBG_IND("%d \r\n",paramArray[0]);

    if(paramNum>0)
        uhSelect= paramArray[0];

    UI_SetData(FL_PHOTO_SIZE,uhSelect);
    Photo_SetUserIndex(PHOTO_USR_SIZE,uhSelect);
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnQuality(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    DBG_IND("%d \r\n",paramArray[0]);
    if(paramNum>0)
        uhSelect= paramArray[0];
    UI_SetData(FL_QUALITY,uhSelect);
    Photo_SetUserIndex(PHOTO_USR_QUALITY,uhSelect);
    PhotoExe_Cap_SetIInfo(CAP_SEL_QUALITY,Get_QualityValue(uhSelect));
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnWB(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    DBG_IND("%d \r\n",paramArray[0]);

    uhSelect= paramArray[0];
    UI_SetData(FL_WB,uhSelect);
    Photo_SetUserIndex(PHOTO_USR_WB,uhSelect);
    PhotoExe_IPL_SetIInfo(IPL_SEL_WBMODE,Get_WBValue(uhSelect));
    return NVTEVT_CONSUME;
}


INT32 PhotoExe_OnColor(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    DBG_IND("%d \r\n",paramArray[0]);

    if(paramNum>0)
        uhSelect= paramArray[0];

    if (UI_GetData(FL_ModeIndex) != DSC_MODE_PHOTO_SCENE)
    {
        UI_SetData(FL_COLOR_EFFECT,uhSelect);
        Photo_SetUserIndex(PHOTO_USR_COLOR,uhSelect);
        PhotoExe_IPL_SetIInfo(IPL_SEL_IMAGEEFFECT,Get_ColorValue(uhSelect));
    }
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnISO(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    DBG_IND("%d \r\n",paramArray[0]);

    if(paramNum>0)
        uhSelect= paramArray[0];

    UI_SetData(FL_ISO,uhSelect);
    Photo_SetUserIndex(PHOTO_USR_ISO,uhSelect);
    PhotoExe_IPL_SetIInfo(IPL_SEL_ISOINDEX,Get_ISOValue(uhSelect));
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnAFWindow(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    DBG_IND("%d \r\n",paramArray[0]);

    if(paramNum>0)
        uhSelect= paramArray[0];

    UI_SetData(FL_AFWindowIndex,uhSelect);
    Photo_SetUserIndex(PHOTO_USR_AFWINDOW,uhSelect);
    PhotoExe_IPL_SetIInfo(IPL_SEL_AFWINDOW,Get_AFWindowValue(uhSelect));
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnAFBeam(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    DBG_IND("%d \r\n",paramArray[0]);
    if(paramNum>0)
        uhSelect= paramArray[0];

    UI_SetData(FL_AFBeamIndex,uhSelect);
    Photo_SetUserIndex(PHOTO_USR_AFBEAM,uhSelect);
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnContAF(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    DBG_IND("%d \r\n",paramArray[0]);
    if(paramNum>0)
        uhSelect= paramArray[0];

    UI_SetData(FL_ContAFIndex,uhSelect);
    Photo_SetUserIndex(PHOTO_USR_CONTAF,uhSelect);
    if (uhSelect == CONT_AF_ON)
    {
        localInfo->isCafOn = TRUE;
    }
    else
        localInfo->isCafOn = FALSE;
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnMetering(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    DBG_IND("%d \r\n",paramArray[0]);

    if(paramNum>0)
        uhSelect= paramArray[0];

    UI_SetData(FL_METERING,uhSelect);
    Photo_SetUserIndex(PHOTO_USR_METERING,uhSelect);
    PhotoExe_IPL_SetIInfo(IPL_SEL_AEMODE,Get_MeteringValue(uhSelect));
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnCaptureMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];

    UI_SetData(FL_CapModeIndex,uhSelect);
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnDatePrint(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    DBG_IND("%d \r\n",paramArray[0]);

    if(paramNum>0)
        uhSelect= paramArray[0];

    UI_SetData(FL_DATE_STAMP,uhSelect);
    Photo_SetUserIndex(PHOTO_USR_DATEIMPRINT,uhSelect);
    PhotoExe_Cap_SetIInfo(CAL_SEL_DATASTAMP, Get_DatePrintValue(uhSelect));
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnPreview(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;

    DBG_IND("%d \r\n",paramArray[0]);
    if(paramNum>0)
        uhSelect= paramArray[0];

    UI_SetData(FL_QUICK_REVIEW,uhSelect);
    Photo_SetUserIndex(PHOTO_USR_QREVIEW,uhSelect);
    return NVTEVT_CONSUME;
}


INT32 PhotoExe_OnDigitalZoom(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];

    UI_SetData(FL_Dzoom,uhSelect);
    Photo_SetUserIndex(PHOTO_USR_DZOOMSTATUS,uhSelect);
    if (uhSelect == DZOOM_OFF)
    {
        UI_SetData(FL_DzoomIndex,DZOOM_10X);
    }
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnFD(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];

    DBG_IND("%d \r\n",paramArray[0]);

    UI_SetData(FL_FD,uhSelect);
    //Photo_SetUserIndex(PHOTO_USR_FD,uhSelect);
    if(uhSelect == FD_ON)
    {
        PhotoExe_IPL_AlgSetFD_AEInfo(SEL_FD_ON);
        localInfo->isSdOn = FALSE;
        localInfo->isFdOn = TRUE;
    #if _FD_FUNC_
        FD_Lock(FALSE);
    #endif
    }
    else
    {
        PhotoExe_IPL_AlgSetFD_AEInfo(SEL_FD_OFF);
        localInfo->isSdOn = FALSE;
        localInfo->isFdOn = FALSE;
        if (!localInfo->isAscnOn)
        {
        #if _FD_FUNC_
        FD_Lock(TRUE);
        #endif
        }
    }
    return NVTEVT_CONSUME;
}


INT32 PhotoExe_OnContShot(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;

    DBG_IND("%d \r\n",paramArray[0]);
    if(paramNum>0)
        uhSelect= paramArray[0];

    UI_SetData(FL_CONTINUE_SHOT,uhSelect);
    return NVTEVT_CONSUME;
}


INT32 PhotoExe_OnSceneMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;

    DBG_IND("%d \r\n",paramArray[0]);

    if(paramNum)
    {
        uhSelect= paramArray[0];

        // scene select by menu
        if (paramNum == 1)
        {
            UI_SetData(FL_SCENE,uhSelect);
            PhotoExe_IPL_SetIInfo(IPL_SEL_SCENEMODE, Get_SceneModeValue(uhSelect));
        }
        else
        // scene set by photo init
        {
            PhotoExe_IPL_SetIInfo(IPL_SEL_SCENEMODE, Get_SceneModeValue(uhSelect));
        }
    }
    return NVTEVT_CONSUME;
}

//#NT#2016/10/18#Jeah Yen -begin
//#NT#support sensor map
extern INT32 System_MapSensorID(INT32 i);
//#NT#2016/10/18#Jeah Yen -end

INT32 PhotoExe_OnCaptureStart(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 CaptureParm = ON_CAPTURE_SINGLE;
    BOOL bAllowImagePostProcess = TRUE;


    if (localInfo->isStartCapture)
    {
        DBG_ERR("Capture start in Capturing\r\n");
        return NVTEVT_CONSUME;
    }
    if (localInfo->IsAFProcess)
    {
        DBG_ERR("Capture start in AF\r\n");
        return NVTEVT_CONSUME;
    }
    if (UI_GetData(FL_SHDR) == SHDR_ON)
    {
        DBG_WRN("HDR not support Capture\r\n");
        return NVTEVT_CONSUME;
    }
    if (paramNum == 1)
    {
        CaptureParm = paramArray[0];
    }
    DBG_IND("[cap] CaptureParm = %d\r\n",CaptureParm);
    DBG_IND("[cap] ContSot En %d\r\n",UI_GetData(FL_CONTINUE_SHOT));
    //DBG_IND("[cap] ContSot index %d\r\n",UI_GetData(FL_ContShotIndex));

    if( UI_GetData(FL_FLASH_MODE) != FLASH_OFF)
    {
        #if (FLASHLIGHT_FUNCTION == ENABLE)
        SxTimer_SetFuncActive(SX_TIMER_DET_RECHARGE_ID,FALSE);
        #endif
        GxFlash_StopCharge();
    }
    //Disable USB detection
    SxTimer_SetFuncActive(SX_TIMER_DET_USB_ID,FALSE);
    //Disable USB detection
    SxTimer_SetFuncActive(SX_TIMER_DET_TV_ID,FALSE);
    //Disable Mode detection
    SxTimer_SetFuncActive(SX_TIMER_DET_MODE_ID,FALSE);

    //#NT#2016/10/06#Lincy Lin -begin
    //#NT# AE,AWB lock will auto control by capture flow or AF flow
    //AE_LOCK(TRUE);
    //AWB_LOCK(TRUE);
    //#NT#2016/10/06#Lincy Lin -end
    AF_LOCK(TRUE);
    #if _FD_FUNC_
    FD_Lock(TRUE);
    #endif
    #if _SD_FUNC_
    SD_Lock(TRUE);
    #endif
    AF_WAIT_IDLE();
    if (ImageUnit_GetParam(&ISF_ImagePipe, PHOTO_PARAM_WAITTSK1) != 0)
    	DBG_ERR("WAITTSK\r\n");

    Ux_FlushEventByRange(NVTEVT_EXE_FDEND,NVTEVT_EXE_FDEND);

    //idec_setV1En(IDE_ID_1, FALSE);
    if (localInfo->isSingleViewOpen)
    {
        //idec_setV1En(IDE_ID_1, FALSE);
        //Photo_SingleView_Close(PHOTO_VIEWTYPE_NORMAL);
        localInfo->isSingleViewOpen = FALSE;
    }
    if(UI_GetData(FL_DATE_STAMP)==DATEIMPRINT_OFF)
    {
        UiDateImprint_SetEnable(FALSE);
    }
    else
    {
        //Avoid seeing garbage screen
        //idec_setV1En(IDE_ID_1, FALSE);
        // 1. should enable first then can init buff
        UiDateImprint_SetEnable(TRUE);
        // 2. Init Date Imprint buff
        Ux_SendEvent(&CustomPhotoObjCtrl,NVTEVT_EXE_INIT_DATE_BUF, 0);
    }
#if (_FPGA_EMULATION_ == DISABLE)
    PhotoExe_IPL_SetIInfo(IPL_SEL_FLASHMODE,Get_FlashValue(UI_GetData(FL_FLASH_MODE)));
#else
    PhotoExe_IPL_SetIInfo(IPL_SEL_FLASHMODE,SEL_FLASHMODE_FORCEOFF);
#endif
    //#NT#2016/04/26#Lincy Lin -begin
    //#NT#Support sidebyside capture mode
    if (UI_GetData(FL_CONTINUE_SHOT) == CONTINUE_SHOT_SIDE )
    {
        PhotoExe_Cap_SetIInfo(CAP_SEL_JPG_IMG_H_SIZE, 2*GetPhotoSizeWidth(UI_GetData(FL_PHOTO_SIZE)));
    }
    else
    {
        PhotoExe_Cap_SetIInfo(CAP_SEL_JPG_IMG_H_SIZE, GetPhotoSizeWidth(UI_GetData(FL_PHOTO_SIZE)));
    }
    //#NT#2016/04/26#Lincy Lin -end
    PhotoExe_Cap_SetIInfo(CAP_SEL_JPG_IMG_V_SIZE, GetPhotoSizeHeight(UI_GetData(FL_PHOTO_SIZE)));
    if (CaptureParm == ON_CAPTURE_SD_CAPTURE)
    {
        localInfo->uiTakePicNum++;

    }
    bAllowImagePostProcess = TRUE;
    //#NT#2016/04/26#Lincy Lin -begin
    //#NT#Support sidebyside capture mode

    if (UI_GetData(FL_CONTINUE_SHOT) == CONTINUE_SHOT_SIDE )
    {
        if (System_GetEnableSensor() != (SENSOR_1|SENSOR_2))
        {
            DBG_ERR("SidebySide capture needs dual sensors\r\n");
            PhotoExe_CallBackUpdateInfo(UIAPPPHOTO_CB_CAPSTART);
            PhotoExe_CallBackUpdateInfo(UIAPPPHOTO_CB_CAPTUREEND);
            return NVTEVT_CONSUME;
        }
		#if (_MODEL_DSC_ != _MODEL_TWIN_360_) // TWIN360 allows 12Mx2 capture
        if (UI_GetData(FL_PHOTO_SIZE) == PHOTO_SIZE_12M)
        {
            DBG_ERR("SidebySide not support 12M\r\n");
            PhotoExe_CallBackUpdateInfo(UIAPPPHOTO_CB_CAPSTART);
            PhotoExe_CallBackUpdateInfo(UIAPPPHOTO_CB_CAPTUREEND);
            return NVTEVT_CONSUME;
        }
		#endif
    }
    //#NT#2016/04/26#Lincy Lin -end
    //if(CaptureParm == ON_CAPTURE_CONT)
    {
        switch(UI_GetData(FL_CONTINUE_SHOT))
        {
            case CONTINUE_SHOT_BURST:
                //In cont shot (except for timelapse cont shot) case ,must disable blink
                PhotoExe_Cap_SetIInfo(CAP_SEL_DRIVEMODE, SEL_DRIVEMODE_INFBURST);
                PhotoExe_IPL_SetIInfo(IPL_SEL_FLASHMODE,SEL_FLASHMODE_FORCEOFF);
                PhotoExe_Cap_SetIInfo(CAP_SEL_RAW_BUF_NUM, 2);
                PhotoExe_Cap_SetIInfo(CAP_SEL_JPG_BUF_NUM, 3);
                PhotoExe_Cap_SetIInfo(CAP_SEL_FILEFMT, SEL_FILEFMT_JPG);
                PhotoExe_Cap_SetIInfo(CAP_SEL_JPGFMT, SEL_JPGFMT_422);
                UI_SetData(FL_IsSingleCapture,FALSE);
                localInfo->isDoingContShot = TRUE;
                localInfo->isStopingContShot = FALSE;
                localInfo->uiTakePicNum = 0;
                bAllowImagePostProcess = FALSE;//not support image post-process
                // set s2 pressed status to true
                Photo_setS2Status(TRUE);
                break;

            case CONTINUE_SHOT_BURST_3:
                PhotoExe_Cap_SetIInfo(CAP_SEL_DRIVEMODE, SEL_DRIVEMODE_BURST);
                PhotoExe_IPL_SetIInfo(IPL_SEL_FLASHMODE,SEL_FLASHMODE_FORCEOFF);
                PhotoExe_Cap_SetIInfo(CAP_SEL_PICNUM, 3);
                PhotoExe_Cap_SetIInfo(CAP_SEL_JPG_BUF_NUM, 2);
                PhotoExe_Cap_SetIInfo(CAP_SEL_RAW_BUF_NUM, 2);
                PhotoExe_Cap_SetIInfo(CAP_SEL_FILEFMT, SEL_FILEFMT_JPG);
                PhotoExe_Cap_SetIInfo(CAP_SEL_JPGFMT, SEL_JPGFMT_422);
                UI_SetData(FL_IsSingleCapture,FALSE);
                localInfo->isDoingContShot = TRUE;
                localInfo->isStopingContShot = FALSE;
                localInfo->uiTakePicNum = 0;
                bAllowImagePostProcess = FALSE;//not support image post-process
                break;
           //#NT#2016/04/26#Lincy Lin -begin
           //#NT#Support sidebyside capture mode
           case CONTINUE_SHOT_SIDE:
                //#NT#2016/11/18#KS Hung -begin
                //#NT#For Side By Side calibration from APP
                #if (SBS_VIEW_FUNC == ENABLE)
                PhotoExe_Cap_SetIInfo(CAP_SEL_MSGBUFSIZE, OS_GetMempoolSize(POOL_ID_SBS_CAL));
                #endif
                //#NT#2016/11/18#KS Hung -begin
                PhotoExe_Cap_SetIInfo(CAP_SEL_DRIVEMODE, SEL_DRIVEMODE_DUALSBS);
                PhotoExe_Cap_SetIInfo(CAP_SEL_PICNUM, 1);
                PhotoExe_Cap_SetIInfo(CAP_SEL_RAW_BUF_NUM, 1);
                PhotoExe_Cap_SetIInfo(CAP_SEL_JPG_BUF_NUM, 1);
                PhotoExe_Cap_SetIInfo(CAP_SEL_FILEFMT, SEL_FILEFMT_JPG);
                PhotoExe_Cap_SetIInfo(CAP_SEL_JPGFMT, SEL_JPGFMT_422);
                UI_SetData(FL_IsSingleCapture,TRUE);
                localInfo->isDoingContShot = FALSE;
                break;
            //#NT#2016/04/26#Lincy Lin -end

            //case CONT_SHOT_TIMELAPSE:
            default:
                //#NT#2016/11/18#KS Hung -begin
                //#NT#For Side By Side calibration from APP
                #if (SBS_VIEW_FUNC == ENABLE)
                PhotoExe_Cap_SetIInfo(CAP_SEL_MSGBUFSIZE, OS_GetMempoolSize(POOL_ID_SBS_CAL));
                #endif
                //#NT#2016/11/18#KS Hung -end
                PhotoExe_Cap_SetIInfo(CAP_SEL_DRIVEMODE, SEL_DRIVEMODE_SINGLE);
                PhotoExe_Cap_SetIInfo(CAP_SEL_PICNUM, 1);
                PhotoExe_Cap_SetIInfo(CAP_SEL_RAW_BUF_NUM, 1);
                PhotoExe_Cap_SetIInfo(CAP_SEL_JPG_BUF_NUM, 1);
                PhotoExe_Cap_SetIInfo(CAP_SEL_FILEFMT, SEL_FILEFMT_JPG);
                PhotoExe_Cap_SetIInfo(CAP_SEL_JPGFMT, SEL_JPGFMT_422);
                UI_SetData(FL_IsSingleCapture,TRUE);
                localInfo->isDoingContShot = FALSE;
                bAllowImagePostProcess = TRUE;//support image post-process
                break;
        }
    }

    // BRC setting
    {
        UINT32 TargetBytes = PhotoExe_GetExpectSize_RhoBRCrtl(UI_GetData(FL_PHOTO_SIZE),TRUE);
        UINT32 UBoundBytes = TargetBytes+(TargetBytes*15/100); //+15%
        UINT32 LBoundBytes = TargetBytes-(TargetBytes*15/100); //-15%
        PhotoExe_Cap_SetIInfo(CAP_SEL_REENCTYPE, SEL_REENCTYPE_RHO);
        PhotoExe_Cap_SetIInfo(CAP_SEL_RHO_INITQF,128);
        PhotoExe_Cap_SetIInfo(CAP_SEL_RHO_TARGETSIZE,TargetBytes);
        PhotoExe_Cap_SetIInfo(CAP_SEL_RHO_LBOUNDSIZE,LBoundBytes);
        PhotoExe_Cap_SetIInfo(CAP_SEL_RHO_HBOUNDSIZE,UBoundBytes);
        PhotoExe_Cap_SetIInfo(CAP_SEL_RHO_RETRYCNT,4);
        DBG_IND("[cap]TargetBytes=%d k, H=%d k, L=%d K\r\n",TargetBytes/1024,UBoundBytes/1024,LBoundBytes/1024);
    }

    if (localInfo->uiMaxImageSize)
    {
        PhotoExe_Cap_SetIInfo(CAP_SEL_FILEBUFMODE,SEL_FILEBUFMODE_MANUAL);
        PhotoExe_Cap_SetIInfo(CAP_SEL_FILEBUFSIZE,localInfo->uiMaxImageSize);
        DBG_IND("[cap]FileBufSize=%d k\r\n",localInfo->uiMaxImageSize/1024);
    }
    else
    {
        PhotoExe_Cap_SetIInfo(CAP_SEL_FILEBUFMODE,SEL_FILEBUFMODE_AUTO);
    }
    PhotoExe_SetScreenNailSize();
    PhotoExe_SetQuickViewSize();

    localInfo->isStartCapture = TRUE;

    /* Start to capture */
    localInfo->IsJPGok = FALSE;
    PhotoExe_CallBackUpdateInfo(UIAPPPHOTO_CB_CAPSTART);
    //#NT#2016/10/18#Jeah Yen -begin
    //#NT#support sensor map
    //#NT#2016/04/26#Lincy Lin -begin
    //#NT#Support sidebyside capture mode
    if (UI_GetData(FL_CONTINUE_SHOT) == CONTINUE_SHOT_SIDE)
    {
        DBG_IND("[cap]sidebyside\r\n");
        PhotoExe_Cap_SetIInfo(CAP_SEL_IPL_ID, 0);
    }
    // set capture id
    else if (localInfo->DualCam < DUALCAM_BOTH)
    //#NT#2016/04/26#Lincy Lin -end
    {
        UINT32 SensorID = localInfo->DualCam; //active sensor (0=1st sensor; 1=2nd sensor)
        UINT32 CaptureID;
        CaptureID = System_MapSensorID(SensorID); //get active sensor
        PhotoExe_Cap_SetIInfo(CAP_SEL_IPL_ID, CaptureID);
        DBG_IND("[cap]single CaptureID=%d \r\n",CaptureID);
    }
    //  dual cam capture
    else
    {
        extern UINT32 Cap_EncodeSensorIDInfo(UINT32 CapNum, UINT32 *ID_Array);
        UINT32 SensorID[2], CaptureID, ID_Num;

        SensorID[0] = System_MapSensorID(0); //get 1st sensor
        SensorID[1] = System_MapSensorID(1); //get 2nd sensor
        ID_Num = localInfo->sensorCount;
        CaptureID = Cap_EncodeSensorIDInfo(ID_Num,SensorID);
        DBG_IND("[cap]dual CaptureID=%d\r\n",CaptureID);
        PhotoExe_Cap_SetIInfo(CAP_SEL_IPL_ID, CaptureID);
        PhotoExe_Cap_SetIInfo(CAP_SEL_DRIVEMODE, SEL_DRIVEMODE_MULSENSOR);
        PhotoExe_Cap_SetIInfo(CAP_SEL_RAW_BUF_NUM, 2);
        PhotoExe_Cap_SetIInfo(CAP_SEL_JPG_BUF_NUM, 3);
    }
    //#NT#2016/10/18#Jeah Yen -end
    if (ImageApp_CamPhoto_CapStart() != 0)
    {
        if (ImageUnit_GetParam(&ISF_Cap, CAP_PARAM_WAITTSK) != 0)
        	DBG_ERR("WAITTSK\r\n");
        ImageApp_CamPhoto_CapStart();
    }
    return NVTEVT_CONSUME;
}


INT32 PhotoExe_OnCaptureStop(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    DBG_IND("[cap]\r\n");
    if(localInfo->isDoingContShot)
    {
        Photo_setS2Status(FALSE);
        ImageApp_CamPhoto_CapStop();
    }
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnCaptureSmile(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 sdState;

    DBG_IND("[cap]\r\n");
    if (paramNum == 1)
    {
        sdState = paramArray[0];
    }
    else
    {
       DBG_ERR("Wrong param Num %d\r\n",paramNum);
       return NVTEVT_CONSUME;
    }

    switch(sdState)
    {
        case SD_START:
        {
            /* Set smile detection start flag */
            if (!localInfo->isStartSmileDetect)
            {
                localInfo->isStartSmileDetect = TRUE;
                localInfo->uiTakePicNum = 0;

                /* Smile detecting should unlock FD and SD */
                #if _FD_FUNC_
                FD_Lock(FALSE);
                #endif
                #if _SD_FUNC_
                SD_Lock(FALSE);
                #endif

                /* Smile detecting should unlock AE/AWB */
                #if 0
                AE_LOCK(FALSE);
                AWB_LOCK(FALSE);
                #endif
            }
            break;
        }

        case SD_CONTINUE:
        {
            /* Continue next smile detection need to only unlock SD again (FD/AE/AWB is already unlocked after capture completed) */
            if (localInfo->isStartSmileDetect && (localInfo->isStartCapture != TRUE))
            {
                #if _FD_FUNC_
                FD_Lock(FALSE);
                #endif
                #if _SD_FUNC_
                SD_Lock(FALSE);
                #endif
            }

            break;
        }

        case SD_STOP:
        {
            // Lock SD
            #if _SD_FUNC_
            SD_Lock(TRUE);
            #endif
            if (ImageUnit_GetParam(&ISF_ImagePipe, PHOTO_PARAM_WAITTSK1) != 0)
        		DBG_ERR("WAITTSK\r\n");

            Ux_FlushEventByRange(NVTEVT_EXE_SDEND,NVTEVT_EXE_SDEND);

            if (localInfo->isStartCapture != TRUE)
            {
                // Unlock AE/AWB if locked
                //unlock FD when
                /* Resume FD action if early in FD process */
                if(UI_GetData(FL_FD) != FD_OFF)
                {
                    #if _FD_FUNC_
                    FD_Lock(FALSE);
                    #endif
                }
            }
            /* Reset smile detection start flag */
            localInfo->isStartSmileDetect = FALSE;

            break;
        }

        default:
            break;
    }
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnCaptureEnd(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    DBG_IND("[cap]\r\n");
    localInfo->isDoingContShot = FALSE;
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnZoom(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{

    UINT32 uiZoomCtrl=UI_ZOOM_CTRL_STOP;
    BOOL   bDZoomEnable = FALSE;
    static BOOL bIsCAFLocked = FALSE;
    static UINT32 uiZoomCnt = 0;
    static UINT32 uiLastSection = 1;
    UINT32 TimeOutCnt = 0;
    UINT32 TimeOutMaxCnt = 20;

    DBG_IND("paramArray[0]=%d, paramArray[1]=%d\r\n",paramArray[0],paramArray[1]);
    if (localInfo->IsAFProcess)
    {
        DBG_ERR("Wait AF Idle\r\n");
        AF_WAIT_IDLE();
    }
    if (paramNum == 2)
    {
        uiZoomCtrl = paramArray[0];
        bDZoomEnable = paramArray[1];
    }
    else
    {
        DBG_ERR("ParamNum %d\r\n",paramNum);
    }
    UI_SetData(FL_ZoomOzmDzmChange,FALSE);
    switch(uiZoomCtrl)
    {
        case UI_ZOOM_CTRL_STOP:

            if(UI_GetData(FL_ZoomIsDigital) == FALSE)
            {
                DBG_IND("Photo_OpticalZoom_Stop\r\n");
                if (OZOOM_IDX_GET() == OZOOM_IDX_MIN() &&
                   (OZOOM_IDX_GET() != uiLastSection))
                {
                    uiZoomCnt++;
                }

                uiLastSection = OZOOM_IDX_GET();
                Ux_FlushEventByRange(NVTEVT_CB_ZOOM, NVTEVT_CB_ZOOM);
                UI_SetData(FL_IslensStopping, TRUE);
                if(PhotoExe_LensZoomStop() != E_OK)
                {
                    if(PhotoExe_LensReset()!=E_OK)
                    {
                        DBG_ERR("lens stop and reset error!!!\r\n");
                        #if (LENSERROFF_FUNCTION == ENABLE)
                        System_PowerOff(SYS_POWEROFF_LENSERROR);
                        #endif
                    }
                    else
                    {
                        UI_SetData(FL_ZoomOzmDzmChange,TRUE);
                        Ux_PostEvent(NVTEVT_CB_ZOOM, 1, ZOOM_IF_OPTICAL);
                    }
                }
                else
                {
                    #if 0
                    if (uiZoomCnt == 10)
                    {
                        uiZoomCnt = 0;

                        if(PhotoExe_LensReset()!=E_OK)
                        {
                            DBG_ERR("lens reset error!!!\r\n");
                            #if (LENSERROFF_FUNCTION == ENABLE)
                            System_PowerOff(SYS_POWEROFF_LENSERROR);
                            #endif
                        }

                    }
                    #endif
                }
                UI_SetData(FL_IslensStopping, FALSE);

            }
            else
            {
                DBG_IND("Photo_DZoom_Stop\r\n");
                PhotoExe_DZoomStop();
            }
            if (!bIsCAFLocked)
                CAF_LOCK(FALSE);
            break;

        case UI_ZOOM_CTRL_IN:
            bIsCAFLocked = CAF_ISLOCK();
            if (!bIsCAFLocked)
                CAF_LOCK(TRUE);
            //only CAF or ASC need to wait idle
            if (!bIsCAFLocked)
            {
                AF_WAIT_IDLE();
            }
            // wait image effect
            SwTimer_DelayMs(60);
            if (UI_GetData(FL_ZoomIFIndex) == ZOOM_IF_DIGITAL)
            {
                PhotoExe_DZoomIn();
                UI_SetData(FL_ZoomIsDigital,TRUE);
                if (DZOOM_IDX_GET() == DZOOM_IDX_MIN())
                {
                    UI_SetData(FL_ZoomOzmDzmChange,TRUE);
                }

            }
            else if(UI_GetData(FL_ZoomIFIndex) == ZOOM_IF_OPTICAL)
            {
                TimeOutCnt = 0;
                while (TimeOutCnt != TimeOutMaxCnt)
                {
                    if (PhotoExe_LensZoomIn() == E_OK)
                    {
                        break;
                    }
                    else
                    {
                        TimeOutCnt++;
                        SwTimer_DelayMs(20);
                        DBG_IND("retry\r\n");
                    }
                }
                if(TimeOutCnt == TimeOutMaxCnt)
                {
                    DBG_ERR("time out\r\n");
                }
                UI_SetData(FL_ZoomIsDigital,FALSE);
            }
            else
            {
                if(OZOOM_IDX_GET() >= OZOOM_IDX_MAX())
                {
                    if(bDZoomEnable)
                    {
                        DBG_IND("\r\n");
                        PhotoExe_DZoomIn();
                        UI_SetData(FL_ZoomIsDigital,TRUE);
                        UI_SetData(FL_ZoomOzmDzmChange,TRUE);
                    }
                }
                else
                {
                    TimeOutCnt = 0;
                    while (TimeOutCnt != TimeOutMaxCnt)
                    {
                        if (PhotoExe_LensZoomIn() == E_OK)
                        {
                            break;
                        }
                        else
                        {
                            TimeOutCnt++;
                            SwTimer_DelayMs(20);
                            DBG_IND("retry \r\n");
                        }
                    }

                    if(TimeOutCnt == TimeOutMaxCnt)
                    {
                        DBG_ERR("time out\r\n");
                    }
                    UI_SetData(FL_ZoomIsDigital,FALSE);
                }
            }
            break;

        case UI_ZOOM_CTRL_OUT:

            bIsCAFLocked = CAF_ISLOCK();
            if (!bIsCAFLocked)
                CAF_LOCK(TRUE);

            if (!bIsCAFLocked)
            {
                AF_WAIT_IDLE();
            }
            SwTimer_DelayMs(60);
            if (UI_GetData(FL_ZoomIFIndex) == ZOOM_IF_DIGITAL)
            {
                PhotoExe_DZoomOut();
                UI_SetData(FL_ZoomIsDigital,TRUE);
                if(DZOOM_IDX_GET() == DZOOM_IDX_MIN())
                {
                    UI_SetData(FL_ZoomOzmDzmChange,TRUE);
                    Ux_PostEvent(NVTEVT_CB_ZOOM, 1, ZOOM_IF_DIGITAL);
                }
            }
            else if(UI_GetData(FL_ZoomIFIndex) == ZOOM_IF_OPTICAL)
            {
                TimeOutCnt = 0;
                while (TimeOutCnt != TimeOutMaxCnt)
                {
                    if (PhotoExe_LensZoomOut() == E_OK)
                    {
                        break;
                    }
                    else
                    {
                        TimeOutCnt++;
                        SwTimer_DelayMs(20);
                        DBG_IND("retry \r\n");
                    }
                }

                if(TimeOutCnt == TimeOutMaxCnt)
                {
                    DBG_ERR("time out\r\n");
                }
                //#NT#2010/11/19#Photon Lin -end
                UI_SetData(FL_ZoomIsDigital,FALSE);
            }
            else
            {
                if(DZOOM_IDX_GET() <= DZOOM_IDX_MIN())
                {
                    DBG_IND("Lens_Zoom_Out\r\n");
                    TimeOutCnt = 0;
                    while (TimeOutCnt != TimeOutMaxCnt)
                    {
                        if (PhotoExe_LensZoomOut() == E_OK)
                        {
                            break;
                        }
                        else
                        {
                            TimeOutCnt++;
                            SwTimer_DelayMs(20);
                            DBG_MSG("Lens_Zoom_Out() retry \r\n");
                        }
                    }

                    if(TimeOutCnt == TimeOutMaxCnt)
                    {
                        DBG_MSG("time out Lens_Zoom_Out()\r\n");
                    }
                    UI_SetData(FL_ZoomIsDigital,FALSE);
                    UI_SetData(FL_ZoomOzmDzmChange,TRUE);
                }
                else
                {
                    DBG_IND("Photo_DZoom_Out\r\n");
                    PhotoExe_DZoomOut();
                    UI_SetData(FL_ZoomIsDigital,TRUE);
                }
            }
            break;

        case UI_ZOOM_CTRL_RESET_DZOOM:
            if((UI_GetData(FL_ZoomIsDigital) == TRUE) && (DZOOM_IDX_GET() > DZOOM_IDX_MIN()) /*&& (UI_GetData(UI_MovieHotKeyRec) != TRUE)*/)
            {
                DBG_IND("DZoom Reset\r\n");
                UI_SetData(FL_DzoomReset,TRUE);
                PhotoExe_DZoomReset();
                UI_SetData(FL_ZoomIsDigital,FALSE);
            }
            break;
        case UI_ZOOM_CTRL_RESET_OZOOM:
            uiZoomCnt = 0;
            break;
        default:
            DBG_ERR("Unknown zoom control 0x%x\r\n", uiZoomCtrl);
            break;
    }
    return NVTEVT_CONSUME;
}


INT32 PhotoExe_OnStartFunc(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{

    UINT32   func= 0;
    UINT32   waitIdle = FALSE;

    DBG_IND("func=0x%x\r\n",paramArray[0]);
    if (localInfo->isStartCapture)
    {
        DBG_ERR("in capturing\r\n");
    }
    else if (paramNum == 2)
    {
        func = paramArray[0];
        waitIdle = paramArray[1];

        //#NT#2016/10/06#Lincy Lin -begin
        //#NT# AE,AWB lock will auto control by capture flow or AF flow
        #if 0
        if(func & UIAPP_PHOTO_AE)
            AE_LOCK(FALSE);
        if(func & UIAPP_PHOTO_AWB)
            AWB_LOCK(FALSE);
        #endif
		//#NT#2016/10/06#Lincy Lin -end
        #if _FD_FUNC_
        if(func & UIAPP_PHOTO_FD)
            FD_Lock(FALSE);
        #endif
        if(func & UIAPP_PHOTO_CAF)
            CAF_LOCK(FALSE);
        if (waitIdle == TRUE)
        {
            if (ImageUnit_GetParam(&ISF_ImagePipe, PHOTO_PARAM_WAITTSK1) != 0)
        		DBG_ERR("WAITTSK\r\n");
        }

    }
    else
    {
        DBG_ERR("wrong param 0x%x\r\n",paramNum);
    }
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnStopFunc(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{

    //UINT32   lock= FALSE;
    UINT32   func= 0;
    UINT32   waitIdle = FALSE;

    DBG_IND("func=0x%x\r\n",paramArray[0]);
    if (paramNum == 2)
    {
        func = paramArray[0];
        waitIdle = paramArray[1];

        #if _FD_FUNC_
        if(func & UIAPP_PHOTO_FD)
            FD_Lock(TRUE);
        #endif
        if(func & UIAPP_PHOTO_CAF)
            CAF_LOCK(TRUE);
        if (waitIdle == TRUE)
        {
            AF_WAIT_IDLE();
            if (ImageUnit_GetParam(&ISF_ImagePipe, PHOTO_PARAM_WAITTSK1) != 0)
        		DBG_ERR("WAITTSK\r\n");
        }

    }
    else
    {
        DBG_ERR("wrong param 0x%x\r\n",paramNum);
    }
    return NVTEVT_CONSUME;
}

////////////////////////////////////////////////////////////
INT32 PhotoExe_OnAFProcess(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if LENS_FUNCTION
    UINT32 isAFBeam;
    //UINT32 isSupportPunchThrough;
    UINT32 currEV, EV_Threshold , LV;

    isAFBeam = paramArray[0];
    //#NT#2010/11/17#Lincy Lin -begin
    #if (ASSIST_BEAM_FUNC == DISABLE)
    isAFBeam = FALSE;
    #endif
    //#NT#2010/11/17#Lincy Lin -end
    //isSupportPunchThrough = paramArray[1];
    DBG_FUNC_BEGIN("[af]\r\n");
	//#NT#2016/10/06#Lincy Lin -begin
    //#NT# AE,AWB lock will auto control by capture flow or AF flow
    //AE_LOCK(TRUE);
    //AWB_LOCK(TRUE);
    //#NT#2016/10/06#Lincy Lin -end
    AF_LOCK(TRUE);
    #if _FD_FUNC_
    FD_Lock(TRUE);
    #endif
    #if _SD_FUNC_
    SD_Lock(TRUE);
    #endif
    Ux_FlushEventByRange(NVTEVT_ALGMSG_FOCUSEND, NVTEVT_ALGMSG_FOCUSEND);
    Ux_FlushEventByRange(NVTEVT_EXE_FDEND,NVTEVT_EXE_FDEND);

    currEV = CURR_EV();
    LV     =  currEV/10;
    EV_Threshold = AF_BEAM_EV_THRESHOLD;
    DBG_IND("[af]EV = %d, LV=%d\r\n",currEV,LV);
    if(isAFBeam && (currEV < EV_Threshold))//LV 5.6~6.1
    {
        //If AF beam is on ,then turn on the focus LED before AF_Process().
        if (!localInfo->isAFBeam)
        {
            if (LV >6)
            {
                DBG_ERR("[af]AF LV %d\r\n",LV);
                LED_SetFcsLevel(0);
            }
            else
            {
                LED_SetFcsLevel(LCSBRT_LVL_06 - LV);
            }
            LED_TurnOnLED(GPIOMAP_LED_FCS);
            localInfo->isAFBeam = TRUE;
        }
        /* AF_Run no wait. */
        AF_Run(AF_ID_1, FALSE);
        localInfo->IsAFProcess = TRUE;
    }
    else
    {
        /* AF_Run no wait. */
        AF_Run(AF_ID_1, FALSE);
        localInfo->IsAFProcess = TRUE;
    }
    DBG_FUNC_END("[af]\r\n");
#endif
    return NVTEVT_CONSUME;
}
INT32 PhotoExe_OnAFRelease(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if LENS_FUNCTION
    DBG_IND("[af]\r\n");
    AF_Release(AF_ID_1,TRUE);
    if (localInfo->isAFBeam)
    {
        LED_TurnOffLED(GPIOMAP_LED_FCS);   //If AF beam is on ,then turn off the focus LED after AF_Process().
        LED_SetFcsLevel(LCSBRT_LVL_03);
        localInfo->isAFBeam = FALSE;
    }
    DBG_FUNC_END("[af]\r\n");
#endif
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnAFWaitEnd(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if LENS_FUNCTION
    DBG_IND("[af]\r\n");
    if (localInfo->IsAFProcess)
    {
        AF_WAIT_IDLE();
        localInfo->IsAFProcess = FALSE;
        if (localInfo->isAFBeam)
        {
            LED_TurnOffLED(GPIOMAP_LED_FCS);   //If AF beam is on ,then turn off the focus LED after AF_Process().
            LED_SetFcsLevel(LCSBRT_LVL_03);
            localInfo->isAFBeam = FALSE;
        }
    }
    DBG_FUNC_END("[af]\r\n");
#endif
    return NVTEVT_CONSUME;
}
#if _FD_FUNC_
void PhotoExe_GetFDDispCord(URECT *dispCord)
{
    UINT32 ImageRatioIdx = 0;
    USIZE  ImageRatioSize={0};
    URECT  fdDispCoord;
    ISIZE  dev1size;
    USIZE  dev1Ratio;
    USIZE  finalSize={0};

    ImageRatioIdx = GetPhotoSizeRatio(UI_GetData(FL_PHOTO_SIZE));
    ImageRatioSize = IMAGERATIO_SIZE[ImageRatioIdx];

    //1.get current device size (current mode)
    dev1size = GxVideo_GetDeviceSize(DOUT1);
    //2.get current device aspect Ratio
    dev1Ratio = GxVideo_GetDeviceAspect(DOUT1);
    finalSize = PhotoExe_RatioSizeConvert((USIZE *)&dev1size,&dev1Ratio,&ImageRatioSize);
    fdDispCoord.w = finalSize.w;
    fdDispCoord.h = finalSize.h;
    if (finalSize.w == (UINT32)dev1size.w)
    {
        fdDispCoord.x = 0;
        fdDispCoord.y = (dev1size.h - finalSize.h)>>1;
    }
    else
    {
        fdDispCoord.y = 0;
        fdDispCoord.x = (dev1size.w - finalSize.w)>>1;

    }
    *dispCord = fdDispCoord;
}
#endif

INT32 PhotoExe_OnImageRatio(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32           ImageRatioIdx = 0;
    USIZE            ImageRatioSize={0};
    BOOL   isCAFLocked;
    //BOOL   isASCLocked;
    BOOL   isWaitIdle = FALSE;
    //#NT#2016/06/17#Brain Yen -begin
    //#NT#for special aspect panel and pipview
    #if (PIP_VIEW_LR == ENABLE || _LCDTYPE_ == _LCDTYPE_TG078UW006A0_DSI_)
    USIZE DeviceRatioSize= GxVideo_GetDeviceAspect(DOUT1);
    #endif
    //#NT#2016/06/17#Brain Yen -end
    DBG_IND("\r\n");

    if (paramNum > 0)
        ImageRatioIdx = paramArray[0];
    DBG_MSG("ImageRatioIdx =%d\r\n", ImageRatioIdx);
    if (ImageRatioIdx >=IMAGERATIO_MAX_CNT)
    {
        DBG_ERR("ImageRatioIdx =%d\r\n", ImageRatioIdx);
        return NVTEVT_CONSUME;
    }

    isCAFLocked = CAF_ISLOCK();
    if(!isCAFLocked)
    {
        CAF_LOCK(TRUE);
        isWaitIdle = TRUE;
    }
    if (isWaitIdle == TRUE)
    {
        AF_WAIT_IDLE();
        //Photo_WaitPhotoIdle(TRUE, (PHOTO_1ST|PHOTO_2ND|PHOTO_3RD));
        //ImageUnit_GetParam(&ISF_ImagePipe, PHOTO_PARAM_WAITTSK1);
        //ImageUnit_GetParam(&ISF_ImagePipe, PHOTO_PARAM_WAITTSK2);
        //ImageUnit_GetParam(&ISF_ImagePipe, PHOTO_PARAM_WAITTSK3);
    }
    ImageRatioSize = IMAGERATIO_SIZE[ImageRatioIdx];
    DBG_MSG("ImageRatioSize=%d:%d\r\n", ImageRatioSize.w, ImageRatioSize.h);

    //1. Set Display image ratio
    if (System_GetEnableDisp() & DISPLAY_1)
    {
        ImageUnit_Begin(&ISF_CamDisp, 0);
            ImageUnit_CfgImgSize(ISF_IN1, 0, 0); //buffer size = full device size
            //#NT#2016/06/17#Brain Yen -begin
            //#NT#for special aspect panel and pipview
            #if (PIP_VIEW_LR == ENABLE || _LCDTYPE_ == _LCDTYPE_TG078UW006A0_DSI_)
            ImageUnit_CfgAspect(ISF_IN1, DeviceRatioSize.w, DeviceRatioSize.h);
			#elif (SBS_VIEW_FUNC == ENABLE)
            ImageUnit_CfgAspect(ISF_IN1, ImageRatioSize.w << 1, ImageRatioSize.h);
            #else
            ImageUnit_CfgAspect(ISF_IN1, ImageRatioSize.w, ImageRatioSize.h);
            #endif
            //#NT#2016/06/17#Brain Yen -end
            ImageUnit_CfgWindow(ISF_IN1, 0, 0, 0, 0);  //window range = full device range
        ImageUnit_End();
    }
    if (System_GetEnableDisp() & DISPLAY_2)
    {
        ImageUnit_Begin(&ISF_CamDisp, 0);
            ImageUnit_CfgImgSize(ISF_IN2, 0, 0); //buffer size = full device size
			#if (SBS_VIEW_FUNC == ENABLE)
            ImageUnit_CfgAspect(ISF_IN2, ImageRatioSize.w << 1, ImageRatioSize.h);
			#else
            ImageUnit_CfgAspect(ISF_IN2, ImageRatioSize.w, ImageRatioSize.h);
			#endif
            ImageUnit_CfgWindow(ISF_IN2, 0, 0, 0, 0);  //window range = full device range
        ImageUnit_End();
    }
    //Set UserDraw Size
    {
        ISIZE devSize = GxVideo_GetDeviceSize(DOUT1);
        System_DispSourceByTarget(&devSize, &ImageRatioSize);
    }

    #if (HTTP_LIVEVIEW_FUNC == ENABLE)
    {
        USIZE HttpLviewSize;
        HttpLviewSize = HTTPLVIEW_SIZE[ImageRatioIdx];
        ImageUnit_Begin(&ISF_NetHTTP, 0);
            ImageUnit_CfgAspect(ISF_IN1, ImageRatioSize.w, ImageRatioSize.h);
            ImageUnit_CfgImgSize(ISF_IN1, HttpLviewSize.w, HttpLviewSize.h);
        ImageUnit_End();
    }
    #endif

    //2. Set Preview image ratio, and direction
//#NT#2016/02/22#Jeah Yen -begin
//#NT#Support IPL direction function
    if(System_GetEnableSensor() & SENSOR_1)
    {
        ImageUnit_Begin(&ISF_ImagePipe1, 0);
            ImageUnit_CfgAspect(ISF_IN1, ImageRatioSize.w, ImageRatioSize.h);
            //#NT#2016/04/06#Lincy Lin -begin
            //#NT#Use chip flip instead of sensor flip
            ImageUnit_CfgDirect(ISF_IN1, (UI_GetData(FL_MOVIE_SENSOR_ROTATE) == SEN_ROTATE_ON)? FLIP_V : FLIP_NONE);
            //#NT#2016/04/06#Lincy Lin -end
        ImageUnit_End();
    }
    if(System_GetEnableSensor() & SENSOR_2)
    {
        ImageUnit_Begin(&ISF_ImagePipe2, 0);
            ImageUnit_CfgAspect(ISF_IN1, ImageRatioSize.w, ImageRatioSize.h);
            ImageUnit_CfgDirect(ISF_IN1, 0);
        ImageUnit_End();
    }
//#NT#2016/02/22#Jeah Yen -end
    ImageStream_UpdateSize();

    //3. Set Capture image ratio
    PhotoExe_Cap_SetIInfo(CAP_PARAM_IMGASPECT_W, ImageRatioSize.w);
    PhotoExe_Cap_SetIInfo(CAP_PARAM_IMGASPECT_H, ImageRatioSize.h);

    //4. Set Fd image ratio
    #if _FD_FUNC_
    {
        PhotoExe_GetFDDispCord(&localInfo->FdDispCoord);
    }
    #endif

    if(!isCAFLocked)
    {
        CAF_LOCK(FALSE);

    }
    return NVTEVT_CONSUME;
}


IMG_CAP_QV_DATA gPhoto_QvData = {0};

INT32 PhotoExe_OnCallback(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    NVTEVT event;

    event = paramArray[0];

    DBG_IND("[cb] event=0x%x\r\n",event);
    switch(event)
    {
        case NVTEVT_ALGMSG_FLASH:
            DBG_IND("[cb]NVTEVT_ALGMSG_FLASH\r\n");
            PhotoExe_CallBackUpdateInfo(UIAPPPHOTO_CB_FLASH);
            break;

        case NVTEVT_ALGMSG_QVSTART:

            DBG_IND("[cb]NVTEVT_ALGMSG_QVSTART\r\n");
            //copy current QV data
            memcpy(&gPhoto_QvData, (IMG_CAP_QV_DATA*)(paramArray[1]), sizeof(IMG_CAP_QV_DATA));
            //Charge flash
            if( UI_GetData(FL_FLASH_MODE) != FLASH_OFF)
            {
                //#NT#2011/04/15#Lincy Lin -begin
                //#NT#Hera14 HW bug , no battery insert can't charge for flash
                if (!UI_GetData(FL_IsStopCharge))
                //#NT#2011/04/15#Lincy Lin -end
                {
                    #if (FLASHLIGHT_FUNCTION == ENABLE)
                    SxTimer_SetFuncActive(SX_TIMER_DET_RECHARGE_ID,TRUE);
                    #endif
                    GxFlash_StartCharge();
                }
            }
            PhotoExe_CallBackUpdateInfo(UIAPPPHOTO_CB_QVSTART);
            break;

        case NVTEVT_ALGMSG_JPGOK:

            DBG_IND("[cb]NVTEVT_ALGMSG_JPGOK\r\n");

            // add picture count 1
            UI_SetData(FL_TakePictCnt,UI_GetData(FL_TakePictCnt)+1);
            localInfo->IsJPGok = TRUE;

            localInfo->isFolderFull = UIStorageCheck(STORAGE_CHECK_FOLDER_FULL, NULL);
            localInfo->isCardFull = UIStorageCheck(STORAGE_CHECK_FULL, &localInfo->FreePicNum);
            if(localInfo->isDoingContShot)
            {
                pPhotoExeInfo->uiTakePicNum++;
                /*
                if((GxKey_GetData(GXKEY_NORMAL_KEY) & FLGKEY_SHUTTER2) == 0 )
                {
                    if (UI_GetData(FL_ContShotIndex) != CONT_SHOT_10)
                    {
                        PhotoExe_StopContShot();
                    }
                }
                else */if(localInfo->isFolderFull || localInfo->isCardFull)
                {
                    PhotoExe_StopContShot();
                }
                #if 0
                else if (UI_GetData(FL_ContShotIndex) == CONT_SHOT_10 && pPhotoExeInfo->uiTakePicNum >=10)
                {
                    PhotoExe_StopContShot();
                }
                #endif
            }
            else
            {
                localInfo->BDstatus = PhotoExe_CheckBD();
            }
            PhotoExe_CallBackUpdateInfo(UIAPPPHOTO_CB_JPGOK);
            break;

        case NVTEVT_ALGMSG_CAPFSTOK:
        {
            INT32  FSsts = paramArray[1];
            DBG_IND("[cb]NVTEVT_ALGMSG_CAPFSTOK\r\n");
            localInfo->isFolderFull = UIStorageCheck(STORAGE_CHECK_FOLDER_FULL, NULL);
            localInfo->isCardFull = UIStorageCheck(STORAGE_CHECK_FULL, &localInfo->FreePicNum);

            if (FSsts != FST_STA_OK)
            {
               UI_SetData(FL_FSStatus, FS_DISK_ERROR);
            }
            if (localInfo->isDoingContShot == TRUE)
            {
                if(localInfo->isFolderFull || localInfo->isCardFull)
                {
                    PhotoExe_StopContShot();
                }
            }
            #if (WIFI_AP_FUNC==ENABLE)
            WifiCmd_Done(WIFIFLAG_CAPTURE_DONE,E_OK);
            #endif

            PhotoExe_CallBackUpdateInfo(UIAPPPHOTO_CB_FSTOK);
        }
            break;


        case NVTEVT_ALGMSG_CAPTUREEND:
            DBG_IND("[cb]NVTEVT_ALGMSG_CAPTUREEND \r\n");
            localInfo->isStartCapture = FALSE;
            localInfo->isStopingContShot = FALSE;
            //Disable USB detection
            SxTimer_SetFuncActive(SX_TIMER_DET_USB_ID,TRUE);
            //Disable USB detection
            SxTimer_SetFuncActive(SX_TIMER_DET_TV_ID,TRUE);
            //Disable Mode detection
            SxTimer_SetFuncActive(SX_TIMER_DET_MODE_ID,TRUE);
            //clear fd number
            #if _FD_FUNC_
            FD_ClrRsltFaceNum();
            #endif

            PhotoExe_CallBackUpdateInfo(UIAPPPHOTO_CB_CAPTUREEND);
            localInfo->isDoingContShot = FALSE;
            DBG_TEST("{OnCaptureEnd}\r\n");
			//#NT#2016/11/10#Niven Cho -begin
            //#NT#AUTO_TEST
			MsdcNvtCb_ExamMsg("{OnCaptureEnd}\r\n");
			//#NT#2016/11/10#Niven Cho -end
            break;
        case NVTEVT_ALGMSG_SLOWSHUTTER:
            PhotoExe_CallBackUpdateInfo(UIAPPPHOTO_CB_SLOWSHUTTER);
            break;

        case NVTEVT_ALGMSG_PREVIEW_STABLE:
            DBG_IND("[cb]NVTEVT_ALGMSG_PREVIEW_STABLE \r\n");
            PhotoExe_CallBackUpdateInfo(UIAPPPHOTO_CB_PREVIEW_STABLE);
            break;

        default:

            break;
    }
    return NVTEVT_PASS;
}


INT32 PhotoExe_OnFDEnd(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    DBG_IND("[cb]\r\n");

    if((!localInfo->isStartCapture) && (UI_GetData(FL_FD) != FD_OFF))
    {
        //Flush FD event before draw
        Ux_FlushEventByRange(NVTEVT_EXE_FDEND,NVTEVT_EXE_FDEND);
        PhotoExe_CallBackUpdateInfo(UIAPPPHOTO_CB_FDEND);
    }
    return NVTEVT_CONSUME;

}

#if 0
INT32 PhotoExe_OnSDEnd(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    DBG_IND("[cb]\r\n");
    /* Set start capture flag */
    if (localInfo->isStartSmileDetect)
    {
        if ((!localInfo->isFolderFull) && (!localInfo->isCardFull))
        {
            //fix SD cap flash sometimes not trigger
            if( (UI_GetData(FL_FLASH_MODE)!=FLASH_OFF) &&  (GxFlash_IsCharging() || GxFlash_IsChargeSuspended()))
            {
                DBG_ERR("Flash is charging!  %d,%d\r\n",GxFlash_IsCharging(),GxFlash_IsChargeSuspended());
            }
            else
            {
                PhotoExe_CallBackUpdateInfo(UIAPPPHOTO_CB_SDEND);
                Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_CAPTURE_START, 1, ON_CAPTURE_SD_CAPTURE);
            }
        }
    }
    return NVTEVT_CONSUME;
}
#endif

INT32 PhotoExe_OnSHDR(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    DBG_IND("%d \r\n",paramArray[0]);
    if(paramNum>0)
        uhSelect= paramArray[0];

    UI_SetData(FL_SHDR,uhSelect);

    #if SHDR_FUNC
    PhotoExe_IPL_SetIInfo(IPL_SEL_SHDR,Get_SHDRValue(uhSelect));

    if (UI_GetData(FL_SHDR) == SHDR_ON)
    {
        UI_SetData(FL_WDR, WDR_OFF);
        UI_SetData(FL_RSC, RSC_OFF);
    }
    // SHDR on/off need to re-arrange IPP buffer , so need to change mode
    // need to off to preview again
    //#NT#2016/08/19#Lincy Lin#[0106935] -begin
    //#NT# Support change WDR, SHDR, RSC setting will change mode after exit menu
    //Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_PHOTO);
    //#NT#2016/08/19#Lincy Lin -end
    #endif
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnWDR(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    DBG_IND("%d \r\n",paramArray[0]);
    if(paramNum>0)
        uhSelect= paramArray[0];

    UI_SetData(FL_WDR,uhSelect);
    #if WDR_FUNC
    if (UI_GetData(FL_WDR) == WDR_ON && UI_GetData(FL_SHDR) == SHDR_ON)
    {
        Ux_SendEvent(&CustomPhotoObjCtrl,NVTEVT_EXE_SHDR, 1,SHDR_OFF);
    }
    else
    {
        PhotoExe_IPL_SetIInfo(IPL_SEL_WDR,Get_WDRValue(uhSelect));
    }
    #endif
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnAntishake(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    DBG_IND("%d \r\n",paramArray[0]);
    if(paramNum>0)
        uhSelect= paramArray[0];

    UI_SetData(FL_ANTISHAKE,uhSelect);
    PhotoExe_IPL_SetIInfo(IPL_SEL_ANTISHAKE,Get_AntishakeValue(uhSelect));
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnEdge(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;
    DBG_IND("%d \r\n",paramArray[0]);
    if (paramNum)
        uiSelect = paramArray[0];

    UI_SetData(FL_EDGE, uiSelect);

    if (uiSelect == MOVIE_EDGE_ON)
    {
        PhotoExe_IPL_SetIInfo(IPL_SEL_IPPEDGE, SEL_IPPEDGE_ON);
    }
    else
    {
        PhotoExe_IPL_SetIInfo(IPL_SEL_IPPEDGE, SEL_IPPEDGE_OFF);
    }

    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnNR(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;
    DBG_IND("%d \r\n",paramArray[0]);
    if (paramNum)
        uiSelect = paramArray[0];

    UI_SetData(FL_NR, uiSelect);

    if (uiSelect == MOVIE_NR_ON)
    {
        PhotoExe_IPL_SetIInfo(IPL_SEL_IPPNR, SEL_IPPNR_ON);
    }
    else
    {
        PhotoExe_IPL_SetIInfo(IPL_SEL_IPPNR, SEL_IPPNR_OFF);
    }

    return NVTEVT_CONSUME;
}


INT32 PhotoExe_OnFocusEnd(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if LENS_FUNCTION
//#NT#2010/09/03#Jeffery Chuang -begin
//#NT#2010/09/03#Jeffery Chuang -end
    DBG_IND("[cb]\r\n");
    localInfo->IsAFProcess = FALSE;
    if (localInfo->isAFBeam)
    {
        LED_TurnOffLED(GPIOMAP_LED_FCS);   //If AF beam is on ,then turn off the focus LED after AF_Process().
        LED_SetFcsLevel(LCSBRT_LVL_03);
        localInfo->isAFBeam = FALSE;
    }
    PhotoExe_CallBackUpdateInfo(UIAPPPHOTO_CB_FOCUSEND);
#endif
    return NVTEVT_PASS;
}


INT32 PhotoExe_OnInitDateBuf(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    if (DATEIMPRINT_OFF != UI_GetData(FL_DATE_STAMP))
    {
        UiDateImprint_InitBuff();
    }
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnGenDateStr(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    if (DATEIMPRINT_OFF != UI_GetData(FL_DATE_STAMP))
    {
        UiDateImprint_UpdateDate();
    }
    return NVTEVT_CONSUME;
}



INT32 PhotoExe_OnGenDatePic(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    if(paramNum!=1)
    {
        //error parameter
        return NVTEVT_CONSUME;
    }
    if (DATEIMPRINT_OFF != UI_GetData(FL_DATE_STAMP))
    {
        UiDateImprint_GenData((IMG_CAP_DATASTAMP_INFO*)paramArray[0]);
    }
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnRSC(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    DBG_IND("%d \r\n",paramArray[0]);
    if (paramNum)
        uiSelect = paramArray[0];

    UI_SetData(FL_RSC, uiSelect);

    if(uiSelect == RSC_ON )
    {
        PhotoExe_RSC_SetSwitch(SEL_RSC_RUNTIME,SEL_RSC_ON);
        if (UI_GetData(FL_SHDR) == SHDR_ON)
        {
            Ux_SendEvent(&CustomPhotoObjCtrl,NVTEVT_EXE_SHDR, 1,SHDR_OFF);
        }
    }
    else
        PhotoExe_RSC_SetSwitch(SEL_RSC_RUNTIME,SEL_RSC_OFF);
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnGdc(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];

    DBG_IND("%d \r\n",uhSelect);
    if (uhSelect == TRUE)
    {
        if (UI_GetData(FL_RSC) == RSC_ON )
        {
            PhotoExe_RSC_SetSwitch(SEL_RSC_RUNTIME,SEL_RSC_OFF);
            DBG_WRN("GDC is enable, so RSC is forced off\r\n");
        }
        PhotoExe_IPL_SetIInfo(IPL_SEL_GDCCOMP,SEL_GDCCOMP_ON);
    }
    else
        PhotoExe_IPL_SetIInfo(IPL_SEL_GDCCOMP,SEL_GDCCOMP_OFF);
    return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnSharpness(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];

    DBG_IND("%d \r\n",uhSelect);
    UI_SetData(FL_SHARPNESS,uhSelect);
    PhotoExe_IPL_SetIInfo(IPL_SEL_SHARPNESS,Get_SharpnessValue(uhSelect));
    Photo_SetUserIndex(PHOTO_USR_SHARPNESS,uhSelect);
    return NVTEVT_CONSUME;
}


INT32 PhotoExe_OnSaturation(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];

    DBG_IND("%d \r\n",uhSelect);
    UI_SetData(FL_SATURATION,uhSelect);
    PhotoExe_IPL_SetIInfo(IPL_SEL_SATURATION,Get_SaturationValue(uhSelect));
    return NVTEVT_CONSUME;
}


INT32 PhotoExe_OnPlayShutterSound(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    GxSound_Stop();
    UISound_Play(DEMOSOUND_SOUND_SHUTTER_TONE);
    return NVTEVT_CONSUME;
}

#if 0
INT32 PhotoExe_OnCaptureID(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];

    DBG_IND("%d \r\n",uhSelect);

    if (uhSelect >= localInfo->sensorCount)
    {
        DBG_ERR("uhSelect %d > sensorCount\r\n");
        return NVTEVT_CONSUME;
    }
    localInfo->CaptureID = uhSelect;
    return NVTEVT_CONSUME;
}
#endif

//#NT#2016/06/21#Jeah Yen -begin
//#NT#Support display fast switch
INT32 PhotoExe_OnVideoChange(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	#if (PIP_VIEW_FASTSWITCH==ENABLE)
    PipView_SetStyle(UI_GetData(FL_DUAL_CAM));
	#endif
    //DBGD(Perf_GetCurrent());
    if(System_GetState(SYS_STATE_CURRMODE)==PRIMARY_MODE_PHOTO)
    {
        Ux_SendEvent(0,NVTEVT_EXE_IMAGE_RATIO,1,GetPhotoSizeRatio(UI_GetData(FL_PHOTO_SIZE)));
    }
    //DBGD(Perf_GetCurrent());
    return NVTEVT_PASS; //PASS this event to UIWnd!
}
//#NT#2016/06/21#Jeah Yen -end

INT32 PhotoExe_OnDualcam(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];

    DBG_IND("%d \r\n",uhSelect);

    #if (SENSOR_CAPS_COUNT == 1)
    DBG_IND("single sensor, not support this option!\r\n");
    #endif
    #if (SENSOR_CAPS_COUNT == 2)

    if(localInfo->sensorCount >= 2)
    {
        localInfo->DualCam = uhSelect;
    }
    UI_SetData(FL_DUAL_CAM, uhSelect);

    // also change the capture id
    //PhotoExe_OnCaptureID(pCtrl, paramNum, paramArray);
    //if (localInfo->DualCam != uhSelect)
    #if (PIP_VIEW_FASTSWITCH==ENABLE)
    {
        Ux_SendEvent(0, NVTEVT_SENSOR_DISPLAY, 1, (SENSOR_1|SENSOR_2)); //for Always trigger PIP View
        //#NT#2015/11/25#Niven Cho#[87393] -begin
        //Here be invoked at startup without in any mode
        if(System_GetState(SYS_STATE_CURRMODE)==PRIMARY_MODE_PHOTO)
        {
            Ux_SendEvent(0,NVTEVT_EXE_IMAGE_RATIO,1,GetPhotoSizeRatio(UI_GetData(FL_PHOTO_SIZE)));
        }
        PipView_SetStyle(UI_GetData(FL_DUAL_CAM));
        //#NT#2015/11/25#Niven Cho -end
    }
    #else
    {
        // set display display
        if(uhSelect == DUALCAM_FRONT)
        {
            Ux_SendEvent(0, NVTEVT_SENSOR_DISPLAY, 1, SENSOR_1);
        }
        if(uhSelect == DUALCAM_BEHIND)
        {
            Ux_SendEvent(0, NVTEVT_SENSOR_DISPLAY, 1, SENSOR_2);
        }
        if (uhSelect == DUALCAM_BOTH)
        {
            Ux_SendEvent(0, NVTEVT_SENSOR_DISPLAY, 1, (SENSOR_1|SENSOR_2));
        }
    }
    #endif //(PIP_VIEW_FASTSWITCH==ENABLE)

    #if (HTTP_LIVEVIEW_FUNC == ENABLE)
    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
    {
        // config pip view parameter
        {
            USIZE   HttpLviewSize;
            UINT32  ImageRatioIdx;

            ImageRatioIdx = GetPhotoSizeRatio(UI_GetData(FL_PHOTO_SIZE));
            HttpLviewSize = HTTPLVIEW_SIZE[ImageRatioIdx];
            PhotoExe_CfgNetHttpPIP(HttpLviewSize,UI_GetData(FL_DUAL_CAM));

        }
    }
    #endif


    #endif
    return NVTEVT_CONSUME;
}




EVENT_ENTRY CustomPhotoObjCmdMap[] =
{
    {NVTEVT_EXE_OPEN,               PhotoExe_OnOpen                 },
    {NVTEVT_EXE_CLOSE,              PhotoExe_OnClose                },
    {NVTEVT_EXE_MACRO,              PhotoExe_OnMacro                },
    {NVTEVT_EXE_SELFTIMER,          PhotoExe_OnSelftimer            },
    {NVTEVT_EXE_FLASH,              PhotoExe_OnFlash                },
    {NVTEVT_EXE_EV,                 PhotoExe_OnEV                   },
    {NVTEVT_EXE_CAPTURE_SIZE,       PhotoExe_OnCaptureSize          },
    {NVTEVT_EXE_QUALITY,            PhotoExe_OnQuality              },
    {NVTEVT_EXE_WB,                 PhotoExe_OnWB                   },
    {NVTEVT_EXE_COLOR,              PhotoExe_OnColor                },
    {NVTEVT_EXE_ISO,                PhotoExe_OnISO                  },
    {NVTEVT_EXE_AFWINDOW,           PhotoExe_OnAFWindow             },
    {NVTEVT_EXE_AFBEAM,             PhotoExe_OnAFBeam               },
    {NVTEVT_EXE_CONTAF,             PhotoExe_OnContAF               },
    {NVTEVT_EXE_METERING,           PhotoExe_OnMetering             },
    {NVTEVT_EXE_CAPTURE_MODE,       PhotoExe_OnCaptureMode          },
    {NVTEVT_EXE_DATE_PRINT,         PhotoExe_OnDatePrint            },
    {NVTEVT_EXE_PREVIEW,            PhotoExe_OnPreview              },
    {NVTEVT_EXE_DIGITAL_ZOOM,       PhotoExe_OnDigitalZoom          },
    {NVTEVT_EXE_FD,                 PhotoExe_OnFD                   },
    {NVTEVT_EXE_CONTSHOT,           PhotoExe_OnContShot             },
    {NVTEVT_EXE_SCENEMODE,          PhotoExe_OnSceneMode            },
    {NVTEVT_EXE_CAPTURE_START,      PhotoExe_OnCaptureStart         },
    {NVTEVT_EXE_CAPTURE_STOP,       PhotoExe_OnCaptureStop          },
    {NVTEVT_EXE_CAPTURE_END,        PhotoExe_OnCaptureEnd           },
    {NVTEVT_EXE_CAPTURE_SMILE,      PhotoExe_OnCaptureSmile         },
    {NVTEVT_EXE_ZOOM,               PhotoExe_OnZoom                 },
    {NVTEVT_EXE_RSC,                PhotoExe_OnRSC                  },
    {NVTEVT_EXE_GDC,                PhotoExe_OnGdc                  },
    {NVTEVT_EXE_START_FUNC,         PhotoExe_OnStartFunc            },
    {NVTEVT_EXE_STOP_FUNC,          PhotoExe_OnStopFunc             },
    {NVTEVT_EXE_AF_PROCESS,         PhotoExe_OnAFProcess            },
    {NVTEVT_EXE_AF_RELEASE,         PhotoExe_OnAFRelease            },
    {NVTEVT_EXE_AF_WAITEND,         PhotoExe_OnAFWaitEnd            },
    {NVTEVT_EXE_IMAGE_RATIO,        PhotoExe_OnImageRatio           },
    {NVTEVT_EXE_INIT_DATE_BUF,      PhotoExe_OnInitDateBuf          },
    {NVTEVT_EXE_GEN_DATE_STR,       PhotoExe_OnGenDateStr           },
    {NVTEVT_EXE_GEN_DATE_PIC,       PhotoExe_OnGenDatePic           },
    {NVTEVT_EXE_SHARPNESS,          PhotoExe_OnSharpness            },
    {NVTEVT_EXE_SATURATION,         PhotoExe_OnSaturation           },
    {NVTEVT_EXE_PLAY_SHUTTER_SOUND, PhotoExe_OnPlayShutterSound     },
    //#NT#2012/07/31#Hideo Lin -end
    //#NT#2016/06/21#Jeah Yen -begin
    //#NT#Support display fast switch
    {NVTEVT_VIDEO_CHANGE,           PhotoExe_OnVideoChange          },
    //#NT#2016/06/21#Jeah Yen -end
    {NVTEVT_EXE_DUALCAM,            PhotoExe_OnDualcam              },
    {NVTEVT_EXE_FDEND,              PhotoExe_OnFDEnd                },
    //{NVTEVT_EXE_SDEND,              PhotoExe_OnSDEnd                },
    {NVTEVT_EXE_SHDR,               PhotoExe_OnSHDR                  },
    {NVTEVT_EXE_WDR,                PhotoExe_OnWDR                  },
    {NVTEVT_EXE_ANTISHAKING,        PhotoExe_OnAntishake            },
    {NVTEVT_EXE_EDGE,               PhotoExe_OnEdge                 },
    {NVTEVT_EXE_NR,                 PhotoExe_OnNR                   },
    {NVTEVT_CALLBACK,               PhotoExe_OnCallback             },
    {NVTEVT_ALGMSG_FOCUSEND,        PhotoExe_OnFocusEnd             },
    {NVTEVT_NULL,                   0},
};

CREATE_APP(CustomPhotoObj,APP_SETUP)


//#NT#2016/08/19#Lincy Lin#[0106935] -begin
//#NT# Support change WDR, SHDR, RSC setting will change mode after exit menu
BOOL FlowPhoto_CheckReOpenItem(void)
{
    BOOL bReOpen = FALSE;

    #if SHDR_FUNC
    if (UI_GetData(FL_SHDR_MENU) != UI_GetData(FL_SHDR))
    {
        if (UI_GetData(FL_SHDR_MENU) == SHDR_ON)
        {
            UI_SetData(FL_WDR, WDR_OFF);
            UI_SetData(FL_WDR_MENU, WDR_OFF);
            UI_SetData(FL_RSC, RSC_OFF);
            UI_SetData(FL_RSC_MENU, RSC_OFF);
        }
        UI_SetData(FL_SHDR, UI_GetData(FL_SHDR_MENU));
        bReOpen = TRUE;
    }
    #endif

    #if WDR_FUNC
    if (UI_GetData(FL_WDR_MENU) != UI_GetData(FL_WDR))
    {
        if(UI_GetData(FL_WDR_MENU)==WDR_ON)
        {
            UI_SetData(FL_SHDR, SHDR_OFF);
            UI_SetData(FL_SHDR_MENU, SHDR_OFF);
        }
        UI_SetData(FL_WDR, UI_GetData(FL_WDR_MENU));
        bReOpen = TRUE;
    }
    #endif

    if(UI_GetData(FL_RSC_MENU) != UI_GetData(FL_RSC))
    {
        if(UI_GetData(FL_RSC_MENU) == RSC_ON)
        {
            UI_SetData(FL_SHDR, SHDR_OFF);
            UI_SetData(FL_SHDR_MENU, SHDR_OFF);
        }
        UI_SetData(FL_RSC, UI_GetData(FL_RSC_MENU));
        bReOpen = TRUE;
    }
    return bReOpen;
}
//#NT#2016/08/19#Lincy Lin -end
