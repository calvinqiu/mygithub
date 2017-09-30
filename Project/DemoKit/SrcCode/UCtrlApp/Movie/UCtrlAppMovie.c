#include "SysCommon.h"
#include "UIFramework.h"
#include "PrjCfg.h"
#include "uToken.h"
#include "UIInfo.h"
#include "UIAppMovie.h"
#include "UIAppPhoto.h"
#include "UIControl.h"
#include "UIControlWnd.h"
#include "UISetup.h"
#include "UCtrlAppMovie.h"
#include "NvtUctrlAPI.h"
#include "UCtrlMain.h"
#include "UIMovieMapping.h"
#include "UIPhotoFuncInfo.h"
#include "MediaWriteLib.h"
#include "MovieInterface.h"
#include "NvtMem.h"
#include "ImageApp_IPCam.h"
#include "FileSysTsk.h"
#include "md_api.h"
#include "MovieStamp.h"
#if (MEDIA_VASTREAM==ENABLE)
#include "LviewNvtAPI.h"
#endif
//#NT#2016/09/09#HM Tseng -begin
//#NT#Avoid build error when audio play is disable
#include "Audio.h"
#include "WavStudioTsk.h"
//#NT#2016/09/09#HM Tseng -end
//#NT#2016/07/26#Niven Cho -begin
//#NT#IPCAM-DSP
#include "ROI_lib.h"
//#NT#2016/07/26#Niven Cho -end

//#NT#2016/05/23#David Tsai -begin
//#NT# Add tampering detection control for uctrl
#if MOVIE_TD_FUNC
#include "TD_lib.h"
#endif
//#NT#2016/05/23#David Tsai -end
//#NT#2016/08/26#Lincy Lin -begin
//#NT# Add uctrl command for BC/MV/IPL buffer APIs .
#if MOVIE_DIS
#include "dis_alg.h"
#endif
//#NT#2016/08/26#Lincy Lin -end
#if MOVIE_FTG_FUNC
#include "ftg_lib.h"
#include "ftg_fd_lib.h"
#endif
#include "JpgEnc.h"
//#NT#2016/12/22#HM Tseng -begin
//#NT# Support AEC function
#if (AUDIO_AEC_FUNC == ENABLE)
#include "nvtaec_api.h"
#endif
//#NT#2016/12/22#HM Tseng -end
#define THIS_DBGLVL         2 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UCtrlAppMovie
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
//#define __DBGFLT__          "[cap]"
#include "DebugModule.h"


#define SXCMD_TEST          0

#define ON                  1
#define OFF                 0

#if (FTG_SNAPSHOT_FUN==ENABLE)
ftg_face g_ftg_list[FTG_MAX_SNAPSHOT_NUM];
UINT32 g_face_number=0;
BOOL g_bFTGOpen = FALSE;
#endif

#if (MEDIA_VASTREAM==ENABLE)
#define NEW_PATH            ENABLE
#else
#define NEW_PATH            DISABLE
#endif


typedef struct _UCtrlMovieParmType
{
    UINT32 isSet;
    UINT32 index;
} UCtrlMovieParmType;


typedef struct _UCtrlMovieInfoType
{
    BOOL                isErr;

    UCtrlMovieParmType  recsize;
    #if UCTRL_APP_MOVIE_FEATURE_SETGET == ENABLE
    UCtrlMovieParmType  liveview_channel_id;
    UCtrlMovieParmType  liveview_vid_enc_fmt;
    UCtrlMovieParmType  liveview_vid_size;
    UCtrlMovieParmType  liveview_vid_framerate;
    UCtrlMovieParmType  liveview_vid_bitrate;
    UCtrlMovieBitrateParmType  liveview_vid_advanced_bitrate;
    UCtrlMovieParmType  liveview_vid_gop;
    UCtrlMovieParmType  liveview_vid_svc_t;
    UCtrlMovieCropPosParmType  liveview_crop;
    UCtrlMovieParmType  liveview_aud_enc_fmt;
    UCtrlMovieParmType  liveview_aud_channel;
    UCtrlMovieParmType  liveview_aud_samplerate;
    UCtrlMovieParmType  vratio;
	UCtrlMovieParmType  liveview_source_id;
    #endif
    UCtrlMovieParmType  cycrec;
    UCtrlMovieParmType  wdr;
    UCtrlMovieParmType  hdr;
    UCtrlMovieParmType  ev;
    UCtrlMovieParmType  motion;
    UCtrlMovieParmType  audio;
    UCtrlMovieParmType  dstamp;
    UCtrlMovieParmType  rsc;
    UCtrlMovieParmType  gsensor;
    UCtrlMovieParmType  timelap;
    UCtrlMovieParmType  ircut;
    UCtrlMovieParmType  rotate;
    UCtrlMovieParmType  murgpro;
    UCtrlMovieParmType  aurgpro;
    UCtrlMovieParmType  pim;
    UCtrlMovieParmType  dualcam;
    UCtrlMovieParmType  start;
    UCtrlMovieParmType  stop;
    UCtrlMovieParmType  snapshot;
    UCtrlMovieParmType  fdstart;
    UCtrlMovieParmType  fdcfgset;
    UCtrlMovieParmType  fdcfgget;
    UCtrlMovieParmType  fddataget;
    UCtrlMovieParmType  aout_channel;


} UCtrlMovieInfoType;

#if (NEW_PATH == ENABLE)
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

#if(IPCAM_FUNC == ENABLE)
static BOOL bMovieModeRestart = FALSE;
#endif
static UCtrlMovieInfoType   movieInfo;
static UCtrlMovieInfoType*  pMovieInfo     = &movieInfo;
#if UCTRL_APP_MOVIE_FEATURE_SETGET == ENABLE
static BOOL                 bUctrlRecStart = TRUE;
#endif
#if (MEDIA_VASTREAM==ENABLE)
//static BOOL                 IsSnapShotFinish = FALSE;
static UINT32               snapShotPathID = 0;
static MEM_RANGE            snapShotData = {0};
#endif

#if (MEDIA_VASTREAM==ENABLE)

//#NT#2016/08/31#Lincy Lin -begin
//#NT#Fix snapshot sometimes fail because of image width*height to large
#if 1
UINT32 SnapShot_EncJPG(UINT32 BufAddr, UINT32 BufSize, PIMG_BUF pSrcImg)
{
    ER     rt;
    UINT32 BSSize,      QValue;
    UINT32 RetryCnt,    EnterRetryMode;
    JPG_ENC_CFG         JpgParam = {0};
    JPG_BRC_CFG         BrcCfg;

    QValue = 0;
    JpgParam.BrcEn = DISABLE;
    EnterRetryMode = FALSE;

    {
        BrcCfg.uiInitialQF = 350;
        BrcCfg.uiWidth = pSrcImg->Width;
        BrcCfg.uiHeight= pSrcImg->Height;

        //target size
        BrcCfg.uiTargetSize = pSrcImg->Width * pSrcImg->Height/10;
        if (BrcCfg.uiTargetSize > BufSize)
        {
            BrcCfg.uiTargetSize = BufSize;
        }

        //low bound size
        BrcCfg.uiLowBoundSize = BrcCfg.uiTargetSize - BrcCfg.uiTargetSize*15/100;
        if (BrcCfg.uiLowBoundSize > BufSize)
        {
            BrcCfg.uiLowBoundSize = BufSize;
        }

        //up bound size
        BrcCfg.uiUpBoundSize = BrcCfg.uiTargetSize + BrcCfg.uiTargetSize*15/100;
        if (BrcCfg.uiUpBoundSize > BufSize)
        {
            BrcCfg.uiUpBoundSize = BufSize;
        }
        BrcCfg.uiMaxReTryCnt = 5;
        Jpg_SetBitRateCtrl(&BrcCfg);
        JpgParam.BrcEn = ENABLE;
    }
    BSSize = BufSize;
    JpgParam.RawImageAddr[0] = pSrcImg->PxlAddr[0];
    JpgParam.RawImageAddr[1] = pSrcImg->PxlAddr[1];
    JpgParam.RawImageAddr[2] = pSrcImg->PxlAddr[2];
    JpgParam.JPGBSAddr = BufAddr;
    JpgParam.JPGBSBufSize = &BSSize;
    JpgParam.Width =  pSrcImg->Width;
    JpgParam.Height = pSrcImg->Height;
    JpgParam.LineOffset[0] = pSrcImg->LineOffset[0];
    JpgParam.LineOffset[1] = pSrcImg->LineOffset[1];
    JpgParam.LineOffset[2] = pSrcImg->LineOffset[2];

    JpgParam.YUVFormat = JPG_FMT_YUV420;
    {
        rt = Jpg_EncOneImg(&JpgParam, QValue);
        if (rt != E_OK)
        {
            EnterRetryMode = TRUE;
            QValue = 95;
            JpgParam.BrcEn = DISABLE;
        }
    }
    if (EnterRetryMode == TRUE)
    {
        RetryCnt = 0;
        while(1)
        {
            BSSize = BufSize;
            if (Jpg_EncOneImg(&JpgParam, QValue) != E_OK)
            {
                if (QValue >= 5)
                {
                    QValue -= 5;
                }
                else
                {
                    BSSize = 0;
                    break;
                }
                //DBG_IND("[flow]retry QVal = %d\r\n", QValue);
            }
            else
            {
                DBG_IND("[flow]FixQ finish QVal = %d\r\n", QValue);
                break;
            }
            RetryCnt ++;
        }
    }
    return BSSize;

}

static void SnapShot_Start(UINT32 channelID)
{
    UINT32  uiLineOffset[MAX_PLANE_NUM];
    UINT32  uiPxlAddr[MAX_PLANE_NUM];
    IMG_BUF SrcImg;
    UINT32  Width = GetMovieSizeWidth_2p(channelID,0);
    UINT32  Height = GetMovieSizeHeight_2p(channelID,0);
    MEDIAREC_YUV_INFO   mediaRecInfo={0};
    UINT32  BufAddr = OS_GetMempoolAddr(POOL_ID_IPCAM_SNAPSHOT);
    UINT32  BufSize = OS_GetMempoolSize(POOL_ID_IPCAM_SNAPSHOT);
    UINT32  jpgSize;

    MovRec_GetLastV_YUV(channelID,&mediaRecInfo);

    //DBG_IND("mediaRecInfo.Yaddr 0x%x, Ylot=0x%x\r\n",mediaRecInfo.Yaddr, mediaRecInfo.Ylot);
    uiPxlAddr[0] = mediaRecInfo.Yaddr;
    uiPxlAddr[1] = mediaRecInfo.UVaddr;
    uiPxlAddr[2] = mediaRecInfo.UVaddr;
    uiLineOffset[0]    = mediaRecInfo.Ylot;
    uiLineOffset[1]    = mediaRecInfo.Ylot;
    uiLineOffset[2]    = mediaRecInfo.Ylot;
    GxImg_InitBufEx(&SrcImg, Width, Height,GX_IMAGE_PIXEL_FMT_YUV420, uiLineOffset, uiPxlAddr);
    jpgSize = SnapShot_EncJPG(BufAddr, BufSize, &SrcImg);
    snapShotData.Addr = BufAddr;
    snapShotData.Size = jpgSize;
    DBG_IND("Snapshot Addr=0x%x, size=0x%x\r\n",snapShotData.Addr, snapShotData.Size);
}


#else
static UINT32 SnapShot_FrameCB(UINT32 frameIdx, MEM_RANGE* frameBuf)
{
    snapShotData = *frameBuf;
    DBG_IND("Addr=0x%x, size=0x%x\r\n",snapShotData.Addr, snapShotData.Size);
    IsSnapShotFinish = TRUE;

    #if 0
    {
        CHAR         tempfilename[30];
        FST_FILE     filehdl = NULL;

        snprintf(tempfilename,sizeof(tempfilename), "A:\\snap_%d.JPG", snapShotPathID);
        filehdl = FileSys_OpenFile(tempfilename,FST_CREATE_ALWAYS|FST_OPEN_WRITE);
        if (filehdl)
        {
            FileSys_WriteFile(filehdl, (UINT8 *)snapShotData.Addr, &snapShotData.Size, 0, NULL);
            FileSys_CloseFile(filehdl);
        }
    }
    #endif
    return 0;
}
static UINT32 SnapShot_GetVideoBufCB(UINT32 *yAddr, UINT32 *cbAddr, UINT32 *crAddr, UINT32 *lineOfsY)
{


    if (IsSnapShotFinish)
    {
        *yAddr = 0;
        *cbAddr = 0;
        *crAddr = 0;
        *lineOfsY = 0;
        return 0;
    }
    #if 0
    {
        IMG_BUF      InputImg;
        if (snapShotPathID == 0)
            ImageUnit_SetParam(&ISF_ImagePipe1,IMAGEPIPE_PARAM_PULLBUF_OUT1, (UINT32)&InputImg);
        else if(snapShotPathID == 1)
            ImageUnit_SetParam(&ISF_ImagePipe1,IMAGEPIPE_PARAM_PULLBUF_OUT2, (UINT32)&InputImg);
        else if(snapShotPathID == 2)
            ImageUnit_SetParam(&ISF_ImagePipe1,IMAGEPIPE_PARAM_PULLBUF_OUT3, (UINT32)&InputImg);
        #if 0
        {
            CHAR tempfilename[30];

            snprintf(tempfilename,sizeof(tempfilename), "A:\\snap_%d.RAW", snapShotPathID);
            GxImg_DumpBuf(tempfilename,&InputImg,GX_IMAGE_DUMP_ALL);
        }
        #endif
        DBG_IND("W=%d, H=%d, lineof=%d\r\n",InputImg.Width, InputImg.Height,InputImg.LineOffset[0]);
        *yAddr = InputImg.PxlAddr[0];
        *cbAddr = InputImg.PxlAddr[1];
        *crAddr = InputImg.PxlAddr[2];
        *lineOfsY = InputImg.LineOffset[0];
    }
    #else
    {
        MEDIAREC_YUV_INFO   mediaRecInfo={0};

        MovRec_GetLastV_YUV(snapShotPathID,&mediaRecInfo);
        *yAddr = mediaRecInfo.Yaddr;
        *cbAddr = mediaRecInfo.UVaddr;
        *crAddr = 0;
        *lineOfsY = mediaRecInfo.Ylot;
    }
    #endif

    return 0;
}


static void SnapShot_Stop(UINT32 channelID)
{
    LviewNvt_Close();
}

static void SnapShot_Start(UINT32 channelID)
{
    LVIEWNVT_INFO      LViewInfo = {0};

    LViewInfo.workMemAdr      = OS_GetMempoolAddr(POOL_ID_IPCAM_SNAPSHOT);
    LViewInfo.workMemSize     = POOL_SIZE_IPCAM_SNAPSHOT*LVIEWNVT_VIDEO_FRAMERATE_15;  // memsize need to be target size *2
    LViewInfo.targetSize      = GetMovieSizeWidth_2p(channelID,0)*GetMovieSizeHeight_2p(channelID,0)/10*LVIEWNVT_VIDEO_FRAMERATE_15;
    LViewInfo.frameRate       = LVIEWNVT_VIDEO_FRAMERATE_15;
//#NT#2016/04/07#David Tsai -begin
//#NT#Cancel alignment for jpeg file to keep the resolution from Linux
    LViewInfo.imgWidth        = GetMovieSizeWidth_2p(channelID,0);//ALIGN_CEIL_32(GetMovieSizeWidth_2p(channelID,0));
    LViewInfo.imgHeight       = GetMovieSizeHeight_2p(channelID,0);//ALIGN_CEIL_16(GetMovieSizeHeight_2p(channelID,0));
//#NT#2016/04/07#David Tsai -end
    DBG_IND("targetSize = %d W=%d, H=%d\r\n",LViewInfo.targetSize,LViewInfo.imgWidth, LViewInfo.imgHeight);

    LViewInfo.MJPGQueueFrameNum = 1;

    LViewInfo.videoCodec   = LVIEWNVT_VIDEO_CODEC_MJPG;
    LViewInfo.fpFrameCB  = SnapShot_FrameCB;

    LViewInfo.fpGetVideoBufCB = SnapShot_GetVideoBufCB;
    // open encode task
    if (LviewNvt_Open(&LViewInfo) != E_OK)
    {
        DBG_ERR("Error open LView video task\r\n");
        return;
    }
    // start encode task
    LviewNvt_Start();
    IsSnapShotFinish = FALSE;
}
#endif
//#NT#2016/08/31#Lincy Lin -end
#endif

static INT32 param_recsize(INT32 argc, char* argv[])
{
    UINT32 value;
    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }
    DBG_IND("get: %d\r\n",atoi(argv[0]));
    value = atoi(argv[0]);
    if (value >= MOVIE_SIZE_ID_MAX)
    {
        DBG_ERR("value =%d\r\n",value);
        return -1;
    }

    pMovieInfo->recsize.index = value;
    pMovieInfo->recsize.isSet = 1;

    return 0;
}

#if UCTRL_APP_MOVIE_FEATURE_SETGET == ENABLE

//#NT#2016/04/13#Charlie Chang -begin
//#NT# add liveview ratio set function
static INT32 param_liveviewratio_set(INT32 argc, char* argv[])
{
    UINT32 value = 0;
    if(argc!=1){
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }
    value = atoi(argv[0]);
    if(value >= IMAGERATIO_MAX_CNT){
        DBG_ERR("ratio value error, value=%d\r\n",value);
        return -1;
    }

    if(ImageApp_IPCam_GetConfig(IPCAM_CFG_RATIO) != value){

        pMovieInfo->vratio.isSet = 1;
        pMovieInfo->vratio.index = value;
    }
    return 0;
}
//#NT#2016/04/13#Charlie Chang -end
static INT32 param_channelid_set(INT32 argc, char* argv[])
{
    UINT32 value;
    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }
    value = atoi(argv[0]);
    if (value >= MOVIE_MAX_CHANNEL)
    {
        DBG_ERR("value =%d\r\n",value);
        return -1;
    }

    pMovieInfo->liveview_channel_id.index = value;
    pMovieInfo->liveview_channel_id.isSet = 1;

    return 0;
}

static INT32 param_liveviewencfmt_set(INT32 argc, char* argv[])
{
    UINT32 value;
    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }
    //DBG_IND("got lv size: %d\r\n",atoi(argv[0]));
    value = atoi(argv[0]);
    if ((value != MEDIAVIDENC_MJPG) && (value != MEDIAVIDENC_H264))
    {
        DBG_ERR("value =%d\r\n",value);
        return -1;
    }
    pMovieInfo->liveview_vid_enc_fmt.index = value;
    pMovieInfo->liveview_vid_enc_fmt.isSet = 1;
    return 0;
}

static INT32 param_liveviewencfmt_get(INT32 argc, char* argv[])
{
    UINT32 uilvencfmt;
    char   retStr[32];

    uilvencfmt = GetMovieVidEncFmt_2p(1);

    snprintf(retStr,sizeof(retStr), "%d", uilvencfmt);
    NvtUctrl_SetRetString(retStr);
    return 0;
}


static INT32 param_liveviewsize_set(INT32 argc, char* argv[])
{
    UINT32 value;
    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }
    //DBG_IND("got lv size: %d\r\n",atoi(argv[0]));
    value = atoi(argv[0]);
    if (value >= WIFI_MOVIE_APP_PREVIEW_SIZE_ID_MAX)
    {
        DBG_ERR("value =%d\r\n",value);
        return -1;
    }
    pMovieInfo->liveview_vid_size.index = value;
    pMovieInfo->liveview_vid_size.isSet = 1;

    return 0;
}

static INT32 param_liveviewsize_get(INT32 argc, char* argv[])
{
    UINT32 uilvsize_w, uilvsize_h;
    char   retStr[32];

    uilvsize_w = GetMovieSizeWidth_2p(1, 0);  // image width
    uilvsize_h = GetMovieSizeHeight_2p(1, 0); // image height

    snprintf(retStr,sizeof(retStr), "%d %d", uilvsize_w, uilvsize_h);
    NvtUctrl_SetRetString(retStr);
    return 0;
}

static INT32 param_liveviewframerate_set(INT32 argc, char* argv[])
{
    UINT32 value;

    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }
    //DBG_IND("got FR: %d\r\n",atoi(argv[0]));
    value = atoi(argv[0]);
    if (value > 30) // Max frame rate is 30FPS.
    {
        DBG_ERR("value =%d\r\n",value);
        return -1;
    }
    #if (MEDIA_VASTREAM==DISABLE)
    // 10/15/20/24/25/30/48/50/60/120/240fps
    if (!((value == 10) || (value == 15) || (value == 20) || (value == 24) || (value == 25) || (value == 30)))
    {
        DBG_ERR("value =%d\r\n",value);
        return -1;
    }
    #endif

    pMovieInfo->liveview_vid_framerate.index = value;
    pMovieInfo->liveview_vid_framerate.isSet = 1;

    return 0;
}

static INT32 param_liveviewframerate_get(INT32 argc, char* argv[])
{
    UINT32 uilvframerate;
    char   retStr[32];

    uilvframerate = GetMovieFrameRate_2p(1, 0);

    snprintf(retStr,sizeof(retStr), "%d", uilvframerate);
    NvtUctrl_SetRetString(retStr);
    return 0;
}

static INT32 param_liveviewbitrate_set(INT32 argc, char* argv[])
{
    UINT32 value;

    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }
    //DBG_IND("got BR: %d\r\n",atoi(argv[0]));
    value = atoi(argv[0]);
    //if (value > (1500 * 1024)) // Max bit rate is 1500 KBytes.
    //{
    //    DBG_ERR("value =%d\r\n",value);
    //    return -1;
    //}
    pMovieInfo->liveview_vid_bitrate.index = value;
    pMovieInfo->liveview_vid_bitrate.isSet = 1;

    return 0;
}

static INT32 param_liveviewbitrate_get(INT32 argc, char* argv[])
{
    UINT32 uilvbitrate;
    char   retStr[32];

    uilvbitrate = GetMovieTargetBitrate_2p(1, 0);

    snprintf(retStr,sizeof(retStr), "%d", uilvbitrate);
    NvtUctrl_SetRetString(retStr);
    return 0;
}

static INT32 param_liveviewgop_set(INT32 argc, char* argv[])
{
    UINT32 value;

    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }
    value = atoi(argv[0]);

    pMovieInfo->liveview_vid_gop.index = value;
    pMovieInfo->liveview_vid_gop.isSet = 1;

    return 0;
}


static INT32 param_liveviewcrop_set(INT32 argc, char* argv[])
{
    if(argc!=3)
    {
        DBG_ERR("argc!=3 (%d)\r\n",argc);
        return -1;
    }
    pMovieInfo->liveview_crop.bCropEnable = atoi(argv[0]);
    pMovieInfo->liveview_crop.uiCoordinateX= atoi(argv[1]);
    pMovieInfo->liveview_crop.uiCoordinateY = atoi(argv[2]);
    pMovieInfo->liveview_crop.isSet = 1;

    DBG_IND("cropen=%d ,x = %d, y=%d\r\n",pMovieInfo->liveview_crop.bCropEnable,pMovieInfo->liveview_crop.uiCoordinateX, pMovieInfo->liveview_crop.uiCoordinateY);
    return 0;
}



static INT32 param_lvaudsamprate_set(INT32 argc, char* argv[])
{
	#if(AUDIO_CMD_UCTRL_ENABLE == ENABLE)
    UINT32 value;

    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }
    //DBG_IND("got BR: %d\r\n",atoi(argv[0]));
    value = atoi(argv[0]);
    if (value > 32) // Max sample rate is 32Khz.
    {
        DBG_ERR("value =%d\r\n",value);
        return -1;
    }

    pMovieInfo->liveview_aud_samplerate.index = value;
    pMovieInfo->liveview_aud_samplerate.isSet = 1;
    #endif
    return 0;
}

static INT32 param_lvaudsamprate_get(INT32 argc, char* argv[])
{
	#if(AUDIO_CMD_UCTRL_ENABLE == ENABLE)
    UINT32 uilvaudsamplingrate;
    char   retStr[32];

    uilvaudsamplingrate = Movie_GetAudSampleRate();

    snprintf(retStr,sizeof(retStr), "%d", uilvaudsamplingrate);
    NvtUctrl_SetRetString(retStr);
    #endif
    return 0;
}

static INT32 param_lvaudenctype_set(INT32 argc, char* argv[])
{
	#if(AUDIO_CMD_UCTRL_ENABLE == ENABLE)
    UINT32 value;

    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }
    //DBG_IND("got BR: %d\r\n",atoi(argv[0]));
    value = atoi(argv[0]);
    #if 0
    if (value != 2 && value != 3) // Only AAC(2) and PPcm(3) can be chosen.
    {
        DBG_ERR("value =%d\r\n",value);
        return -1;
    }
    #endif

    pMovieInfo->liveview_aud_enc_fmt.index = value;
    pMovieInfo->liveview_aud_enc_fmt.isSet = 1;
	#endif
    return 0;

}

static INT32 param_lvaudenctype_get(INT32 argc, char* argv[])
{
	#if(AUDIO_CMD_UCTRL_ENABLE == ENABLE)
    UINT32 uilvaudenctype;
    char   retStr[32];

    uilvaudenctype = Movie_GetAudCodec();

    if(uilvaudenctype == MOVAUDENC_AAC)
        uilvaudenctype = 2;
    else
        uilvaudenctype = 0; // G.711

    snprintf(retStr,sizeof(retStr), "%d", uilvaudenctype);
    NvtUctrl_SetRetString(retStr);
	#endif
    return 0;

}

static INT32 param_venc_get(INT32 argc, char* argv[])
{
    UINT8 venc_channel=0;
    CodecVencSetting sVencSetting={0};
    MOVIE_ENCODE_CROP *pEncodeCrop=NULL;
    MOVIE_BRC_TYPE eMovieBRCType;
    UINT32 uiBrcInfoAddr;

    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }

    if ((venc_channel = atoi(argv[0])) >= MOVIE_MAX_CHANNEL)
    {
        DBG_ERR("invalid venc channel: %u (Max: %d)\r\n",venc_channel , 1);
        return -1;
    }

    //sVencSetting.sVencBitRateCtrl.bDropFrameEnable
    //sVencSetting.sVencBitRateCtrl.eBitrateMode = CODEC_VENC_BIT_RATE_CTRL_CBR;

    #if IPCAM_FUNC
    switch (GetMovieVidEncFmt_2p(venc_channel))
    {
        case MEDIAVIDENC_MJPG:
            sVencSetting.eVencFormat = CODEC_VENC_FORMAT_MJPEG;
            break;
        case MEDIAVIDENC_H264:
        default:
            sVencSetting.eVencFormat = CODEC_VENC_FORMAT_H264;
            break;
    }
    #else
    sVencSetting.eVencFormat = CODEC_VENC_FORMAT_H264;
    #endif

    sVencSetting.sVencBitRateCtrl.uiGOP = GetMovieGOP_2p(venc_channel);
    switch (GetMovieSVCT_2p(venc_channel))
    {
        case VIDENC_SVC_LAYER2:
            sVencSetting.sSVCSetting.eSVCTemporalLayer = CODEC_VENC_SVCT_LAYER_2;
            break;
        case VIDENC_SVC_LAYER3:
            sVencSetting.sSVCSetting.eSVCTemporalLayer = CODEC_VENC_SVCT_LAYER_3;
            break;
        case VIDENC_SVC_DISABLE:
        default:
            sVencSetting.sSVCSetting.eSVCTemporalLayer = CODEC_VENC_SVCT_DISABLE;
            break;
    }

    eMovieBRCType = GetMovieBRC_2p(venc_channel, &uiBrcInfoAddr);
    if (eMovieBRCType == MOVIE_BRC_TYPE_CBR)
    {
        SM_H264_CBR_INFO *pH264CBRInfo=(SM_H264_CBR_INFO*)uiBrcInfoAddr;
		UINT32 uiOptValue;

        sVencSetting.sVencBitRateCtrl.eBitrateMode = CODEC_VENC_BIT_RATE_CTRL_CBR;
        sVencSetting.sVencBitRateCtrl.uiTargetBitrate = pH264CBRInfo->uiBitRate / 1024;
        sVencSetting.sVencBitRateCtrl.fStatisticsTime = pH264CBRInfo->fStatTime;
        sVencSetting.sVencBitRateCtrl.uiQPDelta = pH264CBRInfo->uiQpDelta;
        sVencSetting.sVencBitRateCtrl.uiQPFrameRate = pH264CBRInfo->uiQpFrmRate;
        uiOptValue = ((pH264CBRInfo->uiOpt1 & 0xF00) >> 8);
		if (uiOptValue & 0x8)
			sVencSetting.sVencBitRateCtrl.uiIPWeight = ((uiOptValue & 0x7) / 2) + 4;
		else
			sVencSetting.sVencBitRateCtrl.uiIPWeight = 3 - ((uiOptValue & 0x7) / 2);

        sVencSetting.sVencBitRateCtrl.uiMaxQP = (UINT32)(pH264CBRInfo->iMaxQp);
		sVencSetting.sVencBitRateCtrl.uiBitrateTolerance = ((pH264CBRInfo->uiOpt1 & 0xE0000000) >> 29) - 1;
        sVencSetting.sVencBitRateCtrl.uiMinQP = 7;
        sVencSetting.sVencBitRateCtrl.uiMaxBitrate = 16 * 1024;
        sVencSetting.sVencBitRateCtrl.uiQPGOPDelta = 15;
    }
    else if (eMovieBRCType == MOVIE_BRC_TYPE_VBR)
    {
        SM_H264_VBR_INFO *pH264VBRInfo=(SM_H264_VBR_INFO*)uiBrcInfoAddr;

        sVencSetting.sVencBitRateCtrl.eBitrateMode = CODEC_VENC_BIT_RATE_CTRL_VBR;
        sVencSetting.sVencBitRateCtrl.uiTargetBitrate = 4 * 1024;
        sVencSetting.sVencBitRateCtrl.fStatisticsTime = 1;
        sVencSetting.sVencBitRateCtrl.uiQPDelta = 10;
        sVencSetting.sVencBitRateCtrl.uiQPFrameRate = 15;
        sVencSetting.sVencBitRateCtrl.uiIPWeight = 4;
		sVencSetting.sVencBitRateCtrl.uiBitrateTolerance = ((pH264VBRInfo->uiOpt1 & 0xE0000000) >> 29) - 1;
        sVencSetting.sVencBitRateCtrl.uiMaxQP = (UINT32)(pH264VBRInfo->iMaxQp);
        sVencSetting.sVencBitRateCtrl.uiMinQP = (UINT32)(pH264VBRInfo->iMinQp);
        sVencSetting.sVencBitRateCtrl.uiMaxBitrate = pH264VBRInfo->uiMaxBitrate / 1024;
        sVencSetting.sVencBitRateCtrl.uiQPGOPDelta = pH264VBRInfo->uiQpGopDelta;
    }
    else
    {
        sVencSetting.sVencBitRateCtrl.eBitrateMode = CODEC_VENC_BIT_RATE_CTRL_CBR;
        sVencSetting.sVencBitRateCtrl.uiTargetBitrate = (GetMovieTargetBitrate_2p(venc_channel, 0) * 8) /1024;
    }

    //sVencSetting.eVencInputSource
    sVencSetting.eVencProfile = CODEC_VENC_PROFILE_HIGH;
    if ((pEncodeCrop = GetMovieCrop_2p(venc_channel)) != NULL)
    {
        sVencSetting.sVencCrop.bCropEnable = pEncodeCrop->bCropEnable;
        sVencSetting.sVencCrop.uiCoordinateX = pEncodeCrop->uiCoordinateX;
        sVencSetting.sVencCrop.uiCoordinateY = pEncodeCrop->uiCoordinateY;
    }
    //sVencSetting.uiBufNum
    sVencSetting.uiTargetFrameRate = GetMovieFrameRate_2p(venc_channel, 0);
    sVencSetting.uiTargetWidth = GetMovieSizeWidth_2p(venc_channel, 0);
    sVencSetting.uiTargetHeight = GetMovieSizeHeight_2p(venc_channel, 0);

    /////source ID
	#if(IPCAM_FUNC == ENABLE)

		IPCAM_IPL *pIPL_map=NULL;
		pIPL_map = ImageApp_IPCam_Get_IPL_Mapping();
		sVencSetting.eVencInputSource = pIPL_map[venc_channel];

	#endif
    NvtUctrl_SetConfigData((void *)&sVencSetting, sizeof(CodecVencSetting));

    return 0;
}

static INT32 param_venc_set(INT32 argc, char* argv[])
{
    UINT8 venc_channel=0;
    CodecVencSetting sVencSetting={0};
	UINT32 max_width, max_height;

    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }

    if ((venc_channel = atoi(argv[0])) >= MOVIE_MAX_CHANNEL)
    {
       DBG_ERR("invalid venc channel: %u (Max: %d)\r\n",venc_channel, MOVIE_MAX_CHANNEL);
       return -1;
    }

    NvtUctrl_GetConfigData((void *)&sVencSetting, sizeof(CodecVencSetting));

    // Channel
    pMovieInfo->liveview_channel_id.index = venc_channel;
    pMovieInfo->liveview_channel_id.isSet = 1;

    // Encoded Format
    switch (sVencSetting.eVencFormat)
    {
        case CODEC_VENC_FORMAT_MJPEG:
            pMovieInfo->liveview_vid_enc_fmt.index = MEDIAVIDENC_MJPG;
            break;
        case CODEC_VENC_FORMAT_H264:
        default:
            pMovieInfo->liveview_vid_enc_fmt.index = MEDIAVIDENC_H264;
            break;
    }
    pMovieInfo->liveview_vid_enc_fmt.isSet = 1;

    // FPS
    pMovieInfo->liveview_vid_framerate.index = sVencSetting.uiTargetFrameRate;
    pMovieInfo->liveview_vid_framerate.isSet = 1;

    // Bitrate
    if (sVencSetting.eVencFormat == CODEC_VENC_FORMAT_H264)
    {
	    pMovieInfo->liveview_vid_advanced_bitrate.isSet = 1;
    	pMovieInfo->liveview_vid_advanced_bitrate.eBitrateMode = sVencSetting.sVencBitRateCtrl.eBitrateMode;
	    pMovieInfo->liveview_vid_advanced_bitrate.uiTargetBitrate = sVencSetting.sVencBitRateCtrl.uiTargetBitrate;
    }
	else
	{
		pMovieInfo->liveview_vid_bitrate.isSet = 1;
		pMovieInfo->liveview_vid_bitrate.index = (sVencSetting.sVencBitRateCtrl.uiTargetBitrate * 1024) / 8;
	}

    if (sVencSetting.sVencBitRateCtrl.fStatisticsTime > 10.0)
        pMovieInfo->liveview_vid_advanced_bitrate.fStatisticsTime = 10.0;
    else if (sVencSetting.sVencBitRateCtrl.fStatisticsTime < 1.0)
        pMovieInfo->liveview_vid_advanced_bitrate.fStatisticsTime = 1.0;
    else
        pMovieInfo->liveview_vid_advanced_bitrate.fStatisticsTime = sVencSetting.sVencBitRateCtrl.fStatisticsTime;

    if (sVencSetting.sVencBitRateCtrl.uiQPDelta > 51)
        pMovieInfo->liveview_vid_advanced_bitrate.uiQPDelta = 51;
    else if (sVencSetting.sVencBitRateCtrl.uiQPDelta < 1)
        pMovieInfo->liveview_vid_advanced_bitrate.uiQPDelta = 1;
    else
        pMovieInfo->liveview_vid_advanced_bitrate.uiQPDelta = sVencSetting.sVencBitRateCtrl.uiQPDelta;

    if (sVencSetting.sVencBitRateCtrl.uiQPFrameRate > 16)
        pMovieInfo->liveview_vid_advanced_bitrate.uiQPFrameRate = 16;
    else if (sVencSetting.sVencBitRateCtrl.uiQPFrameRate < 1)
        pMovieInfo->liveview_vid_advanced_bitrate.uiQPFrameRate = 1;
    else
        pMovieInfo->liveview_vid_advanced_bitrate.uiQPFrameRate = sVencSetting.sVencBitRateCtrl.uiQPFrameRate;

	if (sVencSetting.sVencBitRateCtrl.uiIPWeight > 6)
		pMovieInfo->liveview_vid_advanced_bitrate.uiIPWeight = 6;
	else
	    pMovieInfo->liveview_vid_advanced_bitrate.uiIPWeight = sVencSetting.sVencBitRateCtrl.uiIPWeight;

	if (sVencSetting.sVencBitRateCtrl.uiBitrateTolerance > 5)
		pMovieInfo->liveview_vid_advanced_bitrate.uiBitrateTolerance = 5;
	else
	    pMovieInfo->liveview_vid_advanced_bitrate.uiBitrateTolerance = sVencSetting.sVencBitRateCtrl.uiBitrateTolerance;

    if (sVencSetting.sVencBitRateCtrl.uiMaxQP == 0)
        pMovieInfo->liveview_vid_advanced_bitrate.uiMaxQP = 51;
    else if (sVencSetting.sVencBitRateCtrl.uiMaxQP < 15)
        pMovieInfo->liveview_vid_advanced_bitrate.uiMaxQP = 15;
    else if (sVencSetting.sVencBitRateCtrl.uiMaxQP > 51)
        pMovieInfo->liveview_vid_advanced_bitrate.uiMaxQP = 51;
    else
        pMovieInfo->liveview_vid_advanced_bitrate.uiMaxQP = sVencSetting.sVencBitRateCtrl.uiMaxQP;

    if (sVencSetting.sVencBitRateCtrl.uiMinQP == 0)
        pMovieInfo->liveview_vid_advanced_bitrate.uiMinQP = 5;
    else if (sVencSetting.sVencBitRateCtrl.uiMinQP < 5)
        pMovieInfo->liveview_vid_advanced_bitrate.uiMinQP = 5;
    else if (sVencSetting.sVencBitRateCtrl.uiMinQP > (pMovieInfo->liveview_vid_advanced_bitrate.uiMaxQP - 5))
        pMovieInfo->liveview_vid_advanced_bitrate.uiMinQP = pMovieInfo->liveview_vid_advanced_bitrate.uiMaxQP - 5;
    else
        pMovieInfo->liveview_vid_advanced_bitrate.uiMinQP = sVencSetting.sVencBitRateCtrl.uiMinQP;

    pMovieInfo->liveview_vid_advanced_bitrate.uiMaxBitrate = sVencSetting.sVencBitRateCtrl.uiMaxBitrate;

    if (sVencSetting.sVencBitRateCtrl.uiQPGOPDelta > 51)
        pMovieInfo->liveview_vid_advanced_bitrate.uiQPGOPDelta = 51;
    else if (sVencSetting.sVencBitRateCtrl.uiQPGOPDelta < 1)
        pMovieInfo->liveview_vid_advanced_bitrate.uiQPGOPDelta = 1;
    else
        pMovieInfo->liveview_vid_advanced_bitrate.uiQPGOPDelta = sVencSetting.sVencBitRateCtrl.uiQPGOPDelta;

    // GOP
    pMovieInfo->liveview_vid_gop.index = sVencSetting.sVencBitRateCtrl.uiGOP;
    pMovieInfo->liveview_vid_gop.isSet = 1;

    // SVC-T
    switch (sVencSetting.sSVCSetting.eSVCTemporalLayer)
    {
        case CODEC_VENC_SVCT_LAYER_2:
            pMovieInfo->liveview_vid_svc_t.index = VIDENC_SVC_LAYER2;
            break;
        case CODEC_VENC_SVCT_LAYER_3:
            pMovieInfo->liveview_vid_svc_t.index = VIDENC_SVC_LAYER3;
            break;
        case CODEC_VENC_SVCT_DISABLE:
        default:
            pMovieInfo->liveview_vid_svc_t.index = VIDENC_SVC_DISABLE;
            break;
    }

    pMovieInfo->liveview_vid_svc_t.isSet = 1;

	switch (venc_channel)
	{
		case 1:
			max_width = MOVIE_PATH2_MAX_W;
			max_height = MOVIE_PATH2_MAX_H;
			break;
		case 2:
			max_width = MOVIE_PATH3_MAX_W;
			max_height = MOVIE_PATH3_MAX_H;
			break;
		case 0:
		default:
			max_width = MOVIE_PATH1_MAX_W;
			max_height = MOVIE_PATH1_MAX_H;
			break;
	}

    // Resolution
    if (sVencSetting.uiTargetWidth <= max_width && sVencSetting.uiTargetHeight <= max_height)
    {
        switch(sVencSetting.uiTargetWidth)
        {
            case 848:
                pMovieInfo->liveview_vid_size.index = WIFI_MOVIE_APP_PREVIEW_SIZE_WVGA;
                break;
            case 720:
                if(sVencSetting.uiTargetHeight == 480)
                    pMovieInfo->liveview_vid_size.index = WIFI_MOVIE_APP_PREVIEW_SIZE_NTSC;
                else
                    pMovieInfo->liveview_vid_size.index = WIFI_MOVIE_APP_PREVIEW_SIZE_PAL;
                break;
            case 704:
                if(sVencSetting.uiTargetHeight == 480)
                    pMovieInfo->liveview_vid_size.index = WIFI_MOVIE_APP_PREVIEW_SIZE_704x480;
                else
                    pMovieInfo->liveview_vid_size.index = WIFI_MOVIE_APP_PREVIEW_SIZE_704x576;
                break;
            case 640:
                if(sVencSetting.uiTargetHeight == 480)
                    pMovieInfo->liveview_vid_size.index = WIFI_MOVIE_APP_PREVIEW_SIZE_VGA_4_3;
                else
                    pMovieInfo->liveview_vid_size.index = WIFI_MOVIE_APP_PREVIEW_SIZE_VGA_16_9;
                break;
            case 352:
                pMovieInfo->liveview_vid_size.index = WIFI_MOVIE_APP_PREVIEW_SIZE_352x288;
                break;
            case 320:
                pMovieInfo->liveview_vid_size.index = WIFI_MOVIE_APP_PREVIEW_SIZE_QVGA;
                break;
            case 1280:
                if (sVencSetting.uiTargetHeight == 960)
                    pMovieInfo->liveview_vid_size.index = WIFI_MOVIE_APP_PREVIEW_SIZE_960P;
                else
                    pMovieInfo->liveview_vid_size.index = WIFI_MOVIE_APP_PREVIEW_SIZE_720P;
                break;
            case 2048:
                pMovieInfo->liveview_vid_size.index = WIFI_MOVIE_APP_PREVIEW_SIZE_3M_4_3;
                break;
            case 2560:
                if (sVencSetting.uiTargetHeight == 1440)
                    pMovieInfo->liveview_vid_size.index = WIFI_MOVIE_APP_PREVIEW_SIZE_4M_16_9;
                else
                    pMovieInfo->liveview_vid_size.index = WIFI_MOVIE_APP_PREVIEW_SIZE_5M_4_3;
                break;
            case 1920:
            default:
                pMovieInfo->liveview_vid_size.index = WIFI_MOVIE_APP_PREVIEW_SIZE_1080P;
                break;

        }
        pMovieInfo->liveview_vid_size.isSet = 1;
    }
    else
    {
        DBG_WRN("New resolution(%u, %u) is over limitation(%u, %u)\r\n", sVencSetting.uiTargetWidth,
	                                                                     sVencSetting.uiTargetHeight,
		                                                                 max_width,
    		                                                             max_height);
    }

    // Crop
    pMovieInfo->liveview_crop.bCropEnable = (sVencSetting.sVencCrop.bCropEnable == NVT_TRUE) ? 1 : 0;
    pMovieInfo->liveview_crop.uiCoordinateX= sVencSetting.sVencCrop.uiCoordinateX;
    pMovieInfo->liveview_crop.uiCoordinateY = sVencSetting.sVencCrop.uiCoordinateY;
    pMovieInfo->liveview_crop.isSet = 1;

    /////source id
    #if(IPCAM_FUNC == ENABLE)
	IPCAM_IPL *pIPL_map=NULL;
	pIPL_map = ImageApp_IPCam_Get_IPL_Mapping();
	if(pIPL_map[venc_channel]!=sVencSetting.eVencInputSource){
		pMovieInfo->liveview_source_id.isSet = 1;
		pMovieInfo->liveview_source_id.index = sVencSetting.eVencInputSource;
	}
	#endif
    return 0;
}

static INT32 param_vencROI_get(INT32 argc, char* argv[])
{
//#NT#2016/07/26#Niven Cho -begin
//#NT#IPCAM-DSP
#if (_DSP_TYPE_ != _DSP_FREERTOS_)
    DBG_ERR("DSP must enable\r\n");
    return -1;
#else
    UINT8 venc_channel=0;
    CodecVencROISetting sVencROISetting={0};

    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }

    if ((venc_channel = atoi(argv[0])) >= MOVIE_MAX_CHANNEL)
    {
       DBG_ERR("invalid venc channel: %u (Max: %d)\r\n",venc_channel, MOVIE_MAX_CHANNEL);
       return -1;
    }

    /* Get ROI setting */
    switch(venc_channel)
    {
        case 0:
            sVencROISetting.uiX = ROI_GetParam(ROI_PARAM_ROI_X);
            sVencROISetting.uiY = ROI_GetParam(ROI_PARAM_ROI_Y);
            sVencROISetting.uiWidth = ROI_GetParam(ROI_PARAM_ROI_W);
            sVencROISetting.uiHeight = ROI_GetParam(ROI_PARAM_ROI_H);
            sVencROISetting.uiLevel = ROI_GetParam(ROI_PARAM_LEVEL);
            sVencROISetting.bROIEnable = ROI_GetParam(ROI_PARAM_ENABLE);
            break;
        case 1:
            sVencROISetting.uiX = ROI_GetParam(ROI_PARAM_ROI_X2);
            sVencROISetting.uiY = ROI_GetParam(ROI_PARAM_ROI_Y2);
            sVencROISetting.uiWidth = ROI_GetParam(ROI_PARAM_ROI_W2);
            sVencROISetting.uiHeight = ROI_GetParam(ROI_PARAM_ROI_H2);
            sVencROISetting.uiLevel = ROI_GetParam(ROI_PARAM_LEVEL2);
            sVencROISetting.bROIEnable = ROI_GetParam(ROI_PARAM_ENABLE2);
            break;
        case 2:
            sVencROISetting.uiX = ROI_GetParam(ROI_PARAM_ROI_X3);
            sVencROISetting.uiY = ROI_GetParam(ROI_PARAM_ROI_Y3);
            sVencROISetting.uiWidth = ROI_GetParam(ROI_PARAM_ROI_W3);
            sVencROISetting.uiHeight = ROI_GetParam(ROI_PARAM_ROI_H3);
            sVencROISetting.uiLevel = ROI_GetParam(ROI_PARAM_LEVEL3);
            sVencROISetting.bROIEnable = ROI_GetParam(ROI_PARAM_ENABLE3);
            break;
        default:
            DBG_ERR("out of channel.\r\n");
            return -1;
            break;
    }
    NvtUctrl_SetConfigData((void *)&sVencROISetting, sizeof(CodecVencROISetting));
    return 0;
#endif
//#NT#2016/07/26#Niven Cho -end
}

static INT32 param_vencROI_set(INT32 argc, char *argv[])
{
//#NT#2016/07/26#Niven Cho -begin
//#NT#IPCAM-DSP
#if (_DSP_TYPE_ != _DSP_FREERTOS_)
    DBG_ERR("DSP must enable\r\n");
    return -1;
#else
    UINT8 venc_channel=0;
    CodecVencROISetting sVencROISetting={0};

    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }

    if ((venc_channel = atoi(argv[0])) >= MOVIE_MAX_CHANNEL)
    {
       DBG_ERR("invalid venc channel: %u (Max: %d)\r\n",venc_channel, MOVIE_MAX_CHANNEL);
       return -1;
    }

    NvtUctrl_GetConfigData((void *)&sVencROISetting, sizeof(CodecVencROISetting));

    /* Set ROI setting */
    switch(venc_channel)
    {
        case 0:
            ROI_SetParam(ROI_PARAM_ROI_X, sVencROISetting.uiX);
            ROI_SetParam(ROI_PARAM_ROI_Y, sVencROISetting.uiY);
            ROI_SetParam(ROI_PARAM_ROI_W, sVencROISetting.uiWidth);
            ROI_SetParam(ROI_PARAM_ROI_H, sVencROISetting.uiHeight);
            ROI_SetParam(ROI_PARAM_LEVEL, sVencROISetting.uiLevel);
            ROI_SetParam(ROI_PARAM_ENABLE, sVencROISetting.bROIEnable);
            break;
        case 1:
            ROI_SetParam(ROI_PARAM_ROI_X2, sVencROISetting.uiX);
            ROI_SetParam(ROI_PARAM_ROI_Y2, sVencROISetting.uiY);
            ROI_SetParam(ROI_PARAM_ROI_W2, sVencROISetting.uiWidth);
            ROI_SetParam(ROI_PARAM_ROI_H2, sVencROISetting.uiHeight);
            ROI_SetParam(ROI_PARAM_LEVEL2, sVencROISetting.uiLevel);
            ROI_SetParam(ROI_PARAM_ENABLE2, sVencROISetting.bROIEnable);
            break;
        case 2:
            ROI_SetParam(ROI_PARAM_ROI_X2, sVencROISetting.uiX);
            ROI_SetParam(ROI_PARAM_ROI_Y2, sVencROISetting.uiY);
            ROI_SetParam(ROI_PARAM_ROI_W2, sVencROISetting.uiWidth);
            ROI_SetParam(ROI_PARAM_ROI_H2, sVencROISetting.uiHeight);
            ROI_SetParam(ROI_PARAM_LEVEL2, sVencROISetting.uiLevel);
            ROI_SetParam(ROI_PARAM_ENABLE3, sVencROISetting.bROIEnable);
            break;
        default:
            DBG_ERR("out of channel.\r\n");
            return -1;
            break;
    }

    if(ROI_GetParam(ROI_PARAM_ENABLE)
        || ROI_GetParam(ROI_PARAM_ENABLE2)
        || ROI_GetParam(ROI_PARAM_ENABLE3))
    {
        ROI_Start();
    }
    else
    {
        ROI_Stop();
    }

    return 0;
#endif
//#NT#2016/07/26#Niven Cho -end
}

static INT32 param_vstat_get(INT32 argc, char* argv[])
{
    CodecVencStatus sVencStatus={0};
	UINT8 venc_channel=0;

    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }

    if ((venc_channel = atoi(argv[0])) >= MOVIE_MAX_CHANNEL)
    {
       DBG_ERR("invalid venc channel: %u (Max: %d)\r\n",venc_channel, MOVIE_MAX_CHANNEL);
       return -1;
    }

    //if ((venc_channel = atoi(argv[0])) >= IPL_ID_MAX_NUM)
    //{
    //    DBG_ERR("invalid venc channel: %u (Max: %d)\r\n",venc_channel , IPL_ID_MAX_NUM);
    //    return -1;
    //}

    sVencStatus.bStart = bUctrlRecStart;
	switch (venc_channel)
	{
		case 1:
			sVencStatus.uiMaxWidth = MOVIE_PATH2_MAX_W;
			sVencStatus.uiMaxHeight = MOVIE_PATH2_MAX_H;
			break;
		case 2:
			sVencStatus.uiMaxWidth = MOVIE_PATH3_MAX_W;
			sVencStatus.uiMaxHeight = MOVIE_PATH3_MAX_H;
			break;
		case 0:
		default:
			sVencStatus.uiMaxWidth = MOVIE_PATH1_MAX_W;
			sVencStatus.uiMaxHeight = MOVIE_PATH1_MAX_H;
			break;
	}

    NvtUctrl_SetConfigData((void *)&sVencStatus, sizeof(CodecVencStatus));

    return 0;
}

static INT32 param_venc_ireq(INT32 argc, char *argv[])
{
    UINT8 venc_channel=0;

    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }

    if ((venc_channel = atoi(argv[0])) >= MOVIE_MAX_CHANNEL)
    {
       DBG_ERR("invalid venc channel: %u (Max: %d)\r\n",venc_channel, MOVIE_MAX_CHANNEL);
       return -1;
    }
#if MEDIA_VASTREAM
    //#NT#2017/02/22#David Tsai -begin
    //#NT#Use MovRec_ResetI to handle I-frame request(not reset BRC)
	MovRec_ResetI(venc_channel);
    //MovRec_PathN_Start_Stop(venc_channel, 0);
    //MovRec_PathN_Start_Stop(venc_channel, 1);
    //#NT#2017/02/22#David Tsai -end
#endif
    return 0;
}

static INT32 param_aenc_get(INT32 argc, char* argv[])
{
	#if(AUDIO_CMD_UCTRL_ENABLE == ENABLE)
    UINT8 aenc_channel=0;
    CodecAinSetting sAinSetting={0};
    UINT32 volume;

    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }

    if ((aenc_channel = atoi(argv[0])) > 1)
    {
        DBG_ERR("invalid aenc channel: %u (Max: %d)\r\n",aenc_channel , 1);
        return -1;
    }

    switch (Movie_GetAudSampleRate())
    {
        case 8000:
            sAinSetting.eAinSampleRate = CODEC_AIO_SAMPLE_RATE_8000;
            break;
        case 11025:
            sAinSetting.eAinSampleRate = CODEC_AIO_SAMPLE_RATE_11025;
            break;
        case 12000:
            sAinSetting.eAinSampleRate = CODEC_AIO_SAMPLE_RATE_12000;
            break;
        case 16000:
            sAinSetting.eAinSampleRate = CODEC_AIO_SAMPLE_RATE_16000;
            break;
        case 22050:
            sAinSetting.eAinSampleRate = CODEC_AIO_SAMPLE_RATE_22050;
            break;
        case 24000:
            sAinSetting.eAinSampleRate = CODEC_AIO_SAMPLE_RATE_24000;
            break;
        case 32000:
            sAinSetting.eAinSampleRate = CODEC_AIO_SAMPLE_RATE_32000;
            break;
        case 44100:
            sAinSetting.eAinSampleRate = CODEC_AIO_SAMPLE_RATE_44100;
            break;
        case 48000:
        default:
            sAinSetting.eAinSampleRate = CODEC_AIO_SAMPLE_RATE_48000;
            break;
    }

    switch (Movie_GetAudCodec())
    {
        case MOVAUDENC_AAC:
            sAinSetting.eAinFormat = CODEC_AENC_FORMAT_AAC;
            break;
        case MOVAUDENC_ALAW:
            sAinSetting.eAinFormat = CODEC_AENC_FORMAT_ALAW;
            break;
        case MOVAUDENC_PCM:
        case MOVAUDENC_PPCM:
            sAinSetting.eAinFormat = CODEC_AENC_FORMAT_PCM;
            break;
        case MOVAUDENC_ULAW:
        default:
            sAinSetting.eAinFormat = CODEC_AENC_FORMAT_ULAW;
            break;
    }

    switch (Movie_GetAudChannel())
    {
        case MOVREC_AUDTYPE_LEFT:
            sAinSetting.eAinSoundMode = CODEC_AIO_SOUND_MODE_LEFT;
            break;
        case MOVREC_AUDTYPE_RIGHT:
            sAinSetting.eAinSoundMode = CODEC_AIO_SOUND_MODE_RIGHT;
            break;
        case MOVREC_AUDTYPE_MONO:
            sAinSetting.eAinSoundMode = CODEC_AIO_SOUND_MODE_MONO;
            break;
        case MOVREC_AUDTYPE_STEREO:
        default:
            sAinSetting.eAinSoundMode = CODEC_AIO_SOUND_MODE_STEREO;
            break;
    }

    if (System_GetAudioMode() == TRUE)
        sAinSetting.eAinDuplexMode = CODEC_AIO_DUPLEX_MODE_FULL;
    else
        sAinSetting.eAinDuplexMode = CODEC_AIO_DUPLEX_MODE_HALF;

    volume = UI_GetData(FL_MovieAudioRecIndex);
    if (volume == RECMOVIE_AUD_OFF)
        sAinSetting.uiAinVolume = 0;
    else if (volume <= RECMOVIE_AUD_VOL1)
        sAinSetting.uiAinVolume = 1;
    else if (volume <= RECMOVIE_AUD_VOL2)
        sAinSetting.uiAinVolume = 2;
    else if (volume <= RECMOVIE_AUD_VOL3)
        sAinSetting.uiAinVolume = 3;
    else if (volume <= RECMOVIE_AUD_VOL4)
        sAinSetting.uiAinVolume = 4;
    else if (volume <= RECMOVIE_AUD_VOL5)
        sAinSetting.uiAinVolume = 5;
    else if (volume <= RECMOVIE_AUD_VOL6)
        sAinSetting.uiAinVolume = 6;
    else if (volume <= RECMOVIE_AUD_VOL7)
        sAinSetting.uiAinVolume = 7;
    else // if (volume <= RECMOVIE_AUD_ON)
        sAinSetting.uiAinVolume = 8;

    NvtUctrl_SetConfigData((void *)&sAinSetting, sizeof(CodecAinSetting));
    #endif
    return 0;
}

static INT32 param_aenc_set(INT32 argc, char* argv[])
{

    #if(AUDIO_CMD_UCTRL_ENABLE == ENABLE)
    UINT8 aenc_channel=0;
    CodecAinSetting sAinSetting={0};

    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }

    if ((aenc_channel = atoi(argv[0])) > 0)
    {
        DBG_ERR("invalid aenc channel: %u (Max: %d)\r\n",aenc_channel, 1);
        return -1;
    }

    NvtUctrl_GetConfigData((void *)&sAinSetting, sizeof(CodecAinSetting));

    switch (sAinSetting.eAinSampleRate)
    {
        case CODEC_AIO_SAMPLE_RATE_8000:
            Movie_SetAudSampleRate(8000);
            break;
        case CODEC_AIO_SAMPLE_RATE_11025:
            Movie_SetAudSampleRate(11025);
            break;
        case CODEC_AIO_SAMPLE_RATE_12000:
            Movie_SetAudSampleRate(12000);
            break;
        case CODEC_AIO_SAMPLE_RATE_16000:
            Movie_SetAudSampleRate(16000);
            break;
        case CODEC_AIO_SAMPLE_RATE_22050:
            Movie_SetAudSampleRate(22050);
            break;
        case CODEC_AIO_SAMPLE_RATE_24000:
            Movie_SetAudSampleRate(24000);
            break;
        case CODEC_AIO_SAMPLE_RATE_32000:
            Movie_SetAudSampleRate(32000);
            break;
        case CODEC_AIO_SAMPLE_RATE_44100:
            Movie_SetAudSampleRate(44100);
            break;
        case CODEC_AIO_SAMPLE_RATE_48000:
        default:
            Movie_SetAudSampleRate(48000);
            break;
    }

    pMovieInfo->liveview_aud_enc_fmt.isSet = 1;
//#NT#2016/05/13#David Tsai -begin
//#NT#Disable unsupported audio codec
    switch (sAinSetting.eAinFormat)
    {
        case CODEC_AENC_FORMAT_AAC:
            pMovieInfo->liveview_aud_enc_fmt.index = 1;
            break;
        case CODEC_AENC_FORMAT_PCM:
        default:
            pMovieInfo->liveview_aud_enc_fmt.index = 3;
            break;
        /*
        case CODEC_AENC_FORMAT_ALAW:
            pMovieInfo->liveview_aud_enc_fmt.index = 2;
            break;
        case CODEC_AENC_FORMAT_ULAW:
        default:
            pMovieInfo->liveview_aud_enc_fmt.index = 0;
            break;
        */
    }
//#NT#2016/05/13#David Tsai -end
    //channel
    pMovieInfo->liveview_aud_channel.isSet = 1;
    switch (sAinSetting.eAinSoundMode)
    {
        case CODEC_AIO_SOUND_MODE_MONO:
            pMovieInfo->liveview_aud_channel.index = 3;
            break;
        case CODEC_AIO_SOUND_MODE_STEREO:
            pMovieInfo->liveview_aud_channel.index = 2;
            break;
        case CODEC_AIO_SOUND_MODE_LEFT:
            pMovieInfo->liveview_aud_channel.index = 0;
            break;
        case CODEC_AIO_SOUND_MODE_RIGHT:
        default:
            pMovieInfo->liveview_aud_channel.index = 1;
            break;
    }

    //mode = 0:half-duplex or 1:full-duplex
    switch (sAinSetting.eAinDuplexMode)
    {
        case CODEC_AIO_DUPLEX_MODE_HALF:
            Ux_SendEvent(0, NVTEVT_AUDIO_MODE, 1, 0);
            break;
        case CODEC_AIO_DUPLEX_MODE_FULL:
        default:
            Ux_SendEvent(0, NVTEVT_AUDIO_MODE, 1, 1);
            break;
    }

    //vol=0~8
    switch (sAinSetting.uiAinVolume)
    {
        case 0:
            Ux_SendEvent(0, NVTEVT_EXE_MOVIE_AUDIO_VOL, 1, RECMOVIE_AUD_OFF);
            break;
        case 1:
            Ux_SendEvent(0, NVTEVT_EXE_MOVIE_AUDIO_VOL, 1, RECMOVIE_AUD_VOL1);
            break;
        case 2:
            Ux_SendEvent(0, NVTEVT_EXE_MOVIE_AUDIO_VOL, 1, RECMOVIE_AUD_VOL2);
            break;
        case 3:
            Ux_SendEvent(0, NVTEVT_EXE_MOVIE_AUDIO_VOL, 1, RECMOVIE_AUD_VOL3);
            break;
        case 4:
            Ux_SendEvent(0, NVTEVT_EXE_MOVIE_AUDIO_VOL, 1, RECMOVIE_AUD_VOL4);
            break;
        case 5:
            Ux_SendEvent(0, NVTEVT_EXE_MOVIE_AUDIO_VOL, 1, RECMOVIE_AUD_VOL5);
            break;
        case 6:
            Ux_SendEvent(0, NVTEVT_EXE_MOVIE_AUDIO_VOL, 1, RECMOVIE_AUD_VOL6);
            break;
        case 7:
            Ux_SendEvent(0, NVTEVT_EXE_MOVIE_AUDIO_VOL, 1, RECMOVIE_AUD_VOL7);
            break;
        case 8:
        default:
            Ux_SendEvent(0, NVTEVT_EXE_MOVIE_AUDIO_VOL, 1, RECMOVIE_AUD_ON);
            break;
    }

    //Movie_SetAudChannel(channel); // Channel
    #endif
    return 0;
}

static INT32 param_vout_get(INT32 argc, char* argv[])
{
    UINT8 vout_channel=0;
    CodecVoutSetting sVoutSetting={0};

    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }

    if ((vout_channel = atoi(argv[0])) > 1)
    {
        DBG_ERR("invalid vout channel: %u (Max: %d)\r\n",vout_channel , 1);
        return -1;
    }

    switch (KeyScan_GetPlugDev())
    {
        case PLUG_OFF:
            sVoutSetting.bVoutEnable = NVT_FALSE;
            break;
        case PLUG_TV:
            sVoutSetting.bVoutEnable = NVT_TRUE;
            if (UI_GetData(FL_TV_MODE) == VIDEOOUT_NTSC)
                sVoutSetting.eVoutInterface = CODEC_VOUT_INTERFACE_CVBS_NTSC;
            else
                sVoutSetting.eVoutInterface = CODEC_VOUT_INTERFACE_CVBS_PAL;
            break;
        case PLUG_HDMI:
            switch (UI_GetData(FL_HDMI_MODE))
            {
                case VIDEOOUT2_FHD:
                    sVoutSetting.eVoutInterface = CODEC_VOUT_INTERFACE_HDMI_1920x1080I60;
                    break;
                case VIDEOOUT2_HD:
                default:
                    sVoutSetting.eVoutInterface = CODEC_VOUT_INTERFACE_HDMI_720x480P60;
                    break;
            }
            break;
        default:
            break;
    }

    NvtUctrl_SetConfigData((void *)&sVoutSetting, sizeof(CodecVoutSetting));

    return 0;
}

static INT32 param_vout_set(INT32 argc, char* argv[])
{
    UINT8 vout_channel=0;
    CodecVoutSetting sVoutSetting={0};

    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }

    if ((vout_channel = atoi(argv[0])) > 1)
    {
        DBG_ERR("invalid vout channel: %u (Max: %d)\r\n",vout_channel , 1);
        return -1;
    }

    NvtUctrl_GetConfigData((void *)&sVoutSetting, sizeof(CodecVoutSetting));
    DBG_IND("sVoutSetting.bVoutEnable = %u\r\n", sVoutSetting.bVoutEnable);
    if (sVoutSetting.bVoutEnable == NVT_TRUE)
    {
        switch (sVoutSetting.eVoutInterface)
        {
            case CODEC_VOUT_INTERFACE_CVBS_NTSC:
                UctrlMain_SendEvent(NVTEVT_EXE_TVFORMAT, 1, VIDEOOUT_NTSC);
                SxCmd_DoCommand("sys disp tv");
                break;
            case CODEC_VOUT_INTERFACE_CVBS_PAL:
                UctrlMain_SendEvent(NVTEVT_EXE_TVFORMAT, 1, VIDEOOUT_PAL);
                SxCmd_DoCommand("sys disp tv");
                break;
			case CODEC_VOUT_INTERFACE_HDMI_720x480P60:
                UctrlMain_SendEvent(NVTEVT_EXE_HDMIFORMAT, 1, VIDEOOUT2_HD);
                SxCmd_DoCommand("sys disp hd");
                break;
            case CODEC_VOUT_INTERFACE_HDMI_1920x1080I60:
                UctrlMain_SendEvent(NVTEVT_EXE_HDMIFORMAT, 1, VIDEOOUT2_FHD);
                SxCmd_DoCommand("sys disp hd");
                break;
#if (IPCAM_FUNC)
            case CODEC_VOUT_INTERFACE_HDMI_AUTO:
                UctrlMain_SendEvent(NVTEVT_EXE_HDMIFORMAT, 1, VIDEOOUT2_AUTO);
                SxCmd_DoCommand("sys disp hd");
                break;
#endif
            default:
                break;
        }
    }
    else
    {
        DBG_IND("SxCmd_DoCommand : sys disp lcd\r\n");
        SxCmd_DoCommand("sys disp lcd");
    }

    return 0;
}

static INT32 param_aout_get(INT32 argc, char* argv[])
{
	#if(AUDIO_CMD_UCTRL_ENABLE == ENABLE)
    UINT8 aout_channel=0;
    CodecAoutSetting sAoutSetting={0};
    //#NT#2016/09/08#HM Tseng -begin
    //#NT#Support audio channel
    UINT32 aout_audch = 0;
    //#NT#2016/09/08#HM Tseng -end

    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }

    if ((aout_channel = atoi(argv[0])) > 1)
    {
        DBG_ERR("invalid aout channel: %u (Max: %d)\r\n",aout_channel , 1);
        return -1;
    }

    sAoutSetting.eAoutTarget = System_GetAudioOut() == TRUE ? CODEC_AOUT_TARGET_LINEOUT : CODEC_AOUT_TARGET_SPEAKER;
    sAoutSetting.uiAoutVolume = (System_GetAudioVol() * 64) / 100;
    //#NT#2016/09/08#HM Tseng -begin
    //#NT#Support audio channel
    aout_audch = System_GetAudioChannel();

    switch (aout_audch)
    {
        case AUDIO_CH_MONO:
            sAoutSetting.eAoutSoundMode = CODEC_AIO_SOUND_MODE_MONO;
            break;
        case AUDIO_CH_STEREO:
            sAoutSetting.eAoutSoundMode = CODEC_AIO_SOUND_MODE_STEREO;
            break;
        case AUDIO_CH_LEFT:
            sAoutSetting.eAoutSoundMode = CODEC_AIO_SOUND_MODE_LEFT;
            break;
        case AUDIO_CH_RIGHT:
            sAoutSetting.eAoutSoundMode = CODEC_AIO_SOUND_MODE_RIGHT;
            break;
        default:
            sAoutSetting.eAoutSoundMode = CODEC_AIO_SOUND_MODE_LEFT;
            break;
    }
    //#NT#2016/09/08#HM Tseng -end

    //#NT#2016/12/22#HM Tseng -begin
    //#NT# support AEC eanble
    sAoutSetting.bAoutAECEn = System_GetAecEnable();
    //#NT#2016/12/22#HM Tseng -end

    NvtUctrl_SetConfigData((void *)&sAoutSetting, sizeof(CodecAoutSetting));
    #endif
    return 0;
}

static INT32 param_aout_set(INT32 argc, char* argv[])
{
	#if(AUDIO_CMD_UCTRL_ENABLE == ENABLE)
    UINT8 aout_channel=0;
    CodecAoutSetting sAoutSetting={0};
    //#NT#2016/09/08#HM Tseng -begin
    //#NT#Support audio channel
    UINT32 aout_audch = 0;
    //#NT#2016/09/08#HM Tseng -end

    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }

    if ((aout_channel = atoi(argv[0])) > 0)
    {
        DBG_ERR("invalid aout channel: %u (Max: %d)\r\n",aout_channel, 1);
        return -1;
    }

    NvtUctrl_GetConfigData((void *)&sAoutSetting, sizeof(CodecAoutSetting));

    switch (sAoutSetting.eAoutTarget)
    {
        case CODEC_AOUT_TARGET_LINEOUT:
            SxCmd_DoCommand("sys aout line");
            break;
        case CODEC_AOUT_TARGET_SPEAKER:
        default:
            SxCmd_DoCommand("sys aout spk");
            break;
    }

    //#NT#2016/09/08#HM Tseng -begin
    //#NT#Support audio channel
    switch (sAoutSetting.eAoutSoundMode)
    {
        case CODEC_AIO_SOUND_MODE_MONO:
            aout_audch = AUDIO_CH_MONO;
            break;
        case CODEC_AIO_SOUND_MODE_STEREO:
            aout_audch = AUDIO_CH_STEREO;
            break;
        case CODEC_AIO_SOUND_MODE_LEFT:
            aout_audch = AUDIO_CH_LEFT;
            break;
        case CODEC_AIO_SOUND_MODE_RIGHT:
            aout_audch = AUDIO_CH_RIGHT;
            break;
        default:
            aout_audch = AUDIO_CH_LEFT;
            break;
    }

    //#NT#2016/08/29#HM Tseng -begin
    //#NT#For audio play testing mode
    #if (AUDIO_PLAY_TEST == ENABLE)
    UINT32 aout_bitwidth = 0;

    switch (sAoutSetting.eAoutBitWidth)
    {
        case CODEC_AIO_BIT_WIDTH_8BIT:
            aout_bitwidth = 8;
            break;
        case CODEC_AIO_BIT_WIDTH_16BIT:
        default:
            aout_bitwidth = 16;
            break;
    }

    System_SetAudioParam(0, sAoutSetting.eAoutSampleRate, aout_audch, aout_bitwidth);
    #else
    //#NT#2016/12/22#HM Tseng -begin
    //#NT# Only restart when channel is changed
    if (aout_audch != System_GetAudioChannel()) {
        pMovieInfo->aout_channel.index = aout_audch;
        pMovieInfo->aout_channel.isSet = 1;
    }
    //#NT#2016/12/22#HM Tseng -end
    #endif
    //#NT#2016/08/29#HM Tseng -end
    //#NT#2016/09/08#HM Tseng -end

    //vol=0~64, 0 is mute
    Ux_SendEvent(0, NVTEVT_AUDIO_PLAY_VOL, 1, (100 * sAoutSetting.uiAoutVolume) / 64);

    //#NT#2016/12/22#HM Tseng -begin
    //#NT# support AEC eanble
    System_SetAecEnable(sAoutSetting.bAoutAECEn);
    //#NT#2016/12/22#HM Tseng -end
    #endif
    return 0;
}

static INT32 param_aout_switch(INT32 argc, char* argv[])
{
	#if(AUDIO_CMD_UCTRL_ENABLE == ENABLE)
    UINT8 aout_channel=0;

    if(argc!=2)
    {
        DBG_ERR("argc!=2 (%d)\r\n",argc);
        return -1;
    }

    if ((aout_channel = atoi(argv[0])) > 1)
    {
        DBG_ERR("invalid aout channel: %u (Max: %d)\r\n",aout_channel , 1);
        return -1;
    }

/*
    //mode = 0:disable or 1:enable
    if (atoi(argv[1]) == 0)
        Ux_SendEvent(0, NVTEVT_AUDIO_INPUT, 1, 0);
    else
        Ux_SendEvent(0, NVTEVT_AUDIO_INPUT, 1, 1);
*/
    #endif
    return 0;
}

static INT32 param_aout_status(INT32 argc, char* argv[])
{
	#if(AUDIO_CMD_UCTRL_ENABLE == ENABLE)
    UINT8 aout_channel=0;
    char retStr[32];

    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }

    if ((aout_channel = atoi(argv[0])) > 1)
    {
        DBG_ERR("invalid aout channel: %u (Max: %d)\r\n",aout_channel , 1);
        return -1;
    }

    snprintf(retStr, sizeof(retStr), "%lu", System_GetAudioInput() == TRUE ? 1 : 0);
    NvtUctrl_SetRetString(retStr);
    #endif
    return 0;
}

static INT32 param_hdr_get(INT32 argc, char* argv[])
{
    UINT8 ipl_id;
    ISPHDRSetting sHDRSetting={0};

    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }

    if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM)
    {
        DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n",ipl_id, IPL_ID_MAX_NUM);
        return -1;
    }

    switch (UI_GetData(FL_MOVIE_HDR_MENU))
    {
        case MOVIE_HDR_ON:
            sHDRSetting.bHDREnable = NVT_TRUE;
            break;
        case MOVIE_HDR_OFF:
        default:
            sHDRSetting.bHDREnable = NVT_FALSE;
            break;
    }

    NvtUctrl_SetConfigData((void *)&sHDRSetting, sizeof(ISPHDRSetting));

    return 0;
}

static INT32 param_hdr_set(INT32 argc, char* argv[])
{
    UINT8 ipl_id;
    ISPHDRSetting sHDRSetting = {0};

    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }

    if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM)
    {
        DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n",ipl_id, IPL_ID_MAX_NUM);
        return -1;
    }

    NvtUctrl_GetConfigData((void *)&sHDRSetting, sizeof(ISPHDRSetting));

    if (sHDRSetting.bHDREnable == NVT_TRUE)
    {
        pMovieInfo->hdr.index = MOVIE_HDR_ON;
    }
    else
    {
        pMovieInfo->hdr.index = MOVIE_HDR_OFF;
    }

    pMovieInfo->hdr.isSet = 1;

    return 0;
}
//#NT#2016/05/11#David Tsai -begin
//#NT#Add dummy function to make all project compatible to core 2 linux
#else
static INT32 param_dummy_func(INT32 argc, char* argv[])
{
    return 0;
}
//#NT#2016/05/11#David Tsai -end
#endif

static INT32 param_cycrec(INT32 argc, char* argv[])
{
    UINT32 value;
    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }
    DBG_IND("get: %d\r\n",atoi(argv[0]));
    value = atoi(argv[0]);
    if (value >= MOVIE_CYCLICREC_ID_MAX)
    {
        DBG_ERR("value =%d\r\n",value);
        return -1;
    }

    pMovieInfo->cycrec.index = value;
    pMovieInfo->cycrec.isSet = 1;

    return 0;
}

static INT32 param_wdr(INT32 argc, char* argv[])
{
    UINT32 value;
    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }
    DBG_IND("get: %d\r\n",atoi(argv[0]));
    value = atoi(argv[0]);
    if (value > ON)
    {
        DBG_ERR("value =%d\r\n",value);
        return -1;
    }

    if (value == ON)
    {
        pMovieInfo->wdr.index = MOVIE_WDR_ON;
    }
    else
    {
        pMovieInfo->wdr.index = MOVIE_WDR_OFF;
    }

    pMovieInfo->wdr.isSet = 1;

    return 0;
}


static INT32 param_hdr(INT32 argc, char* argv[])
{
    UINT32 value;
    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }
    DBG_IND("get: %d\r\n",atoi(argv[0]));
    value = atoi(argv[0]);
    if (value > ON)
    {
        DBG_ERR("value =%d\r\n",value);
        return -1;
    }

    if (value == ON)
    {
        pMovieInfo->hdr.index = MOVIE_HDR_ON;
    }
    else
    {
        pMovieInfo->hdr.index = MOVIE_HDR_OFF;
    }

    pMovieInfo->hdr.isSet = 1;

    return 0;
}

static INT32 param_ev(INT32 argc, char* argv[])
{
    UINT32 value;
    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }
    DBG_IND("get: %d\r\n",atoi(argv[0]));
    value = atoi(argv[0]);
    if (value >= EV_SETTING_MAX)
    {
        DBG_ERR("value =%d\r\n",value);
        return -1;
    }

    pMovieInfo->ev.index = value;
    pMovieInfo->ev.isSet = 1;

    return 0;
}

static INT32 param_motion(INT32 argc, char* argv[])
{
    UINT32 value;
    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }
    DBG_IND("get: %d\r\n",atoi(argv[0]));
    value = atoi(argv[0]);
    if (value > ON)
    {
        DBG_ERR("value =%d\r\n",value);
        return -1;
    }

    if (value == ON)
    {
        pMovieInfo->motion.index = MOVIE_MOTIONDET_ON;
    }
    else
    {
        pMovieInfo->motion.index = MOVIE_MOTIONDET_OFF;
    }

    pMovieInfo->motion.isSet = 1;

    return 0;
}

static INT32 param_audio(INT32 argc, char* argv[])
{
	#if(AUDIO_CMD_UCTRL_ENABLE == ENABLE)
    UINT32 value;
    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }
    DBG_IND("get: %d\r\n",atoi(argv[0]));
    value = atoi(argv[0]);
    if (value > ON)
    {
        DBG_ERR("value =%d\r\n",value);
        return -1;
    }

    if (value == ON)
    {
        pMovieInfo->audio.index = MOVIE_AUDIO_ON;
        //#NT#2016/08/29#HM Tseng -begin
        //#NT#For audio play testing mode
        #if (AUDIO_PLAY_TEST == ENABLE)
        System_AudioOutStart();
        #endif
        //#NT#2016/08/29#HM Tseng -end
    }
    else
    {
        pMovieInfo->audio.index = MOVIE_AUDIO_OFF;
        //#NT#2016/08/29#HM Tseng -begin
        //#NT#For audio play testing mode
        #if (AUDIO_PLAY_TEST == ENABLE)
        System_AudioOutStop();
        #endif
        //#NT#2016/08/29#HM Tseng -end
    }

    pMovieInfo->audio.isSet = 1;
    #endif
    return 0;
}

static INT32 param_dstamp(INT32 argc, char* argv[])
{
    UINT32 value;
    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }
    DBG_IND("get: %d\r\n",atoi(argv[0]));
    value = atoi(argv[0]);
    if (value > ON)
    {
        DBG_ERR("value =%d\r\n",value);
        return -1;
    }

    if (value == ON)
    {
        pMovieInfo->dstamp.index = MOVIE_DATEIMPRINT_ON;
    }
    else
    {
        pMovieInfo->dstamp.index = MOVIE_DATEIMPRINT_OFF;
    }

    pMovieInfo->dstamp.isSet = 1;

    return 0;
}

static INT32 param_rsc(INT32 argc, char* argv[])
{
    UINT32 value;
    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }
    DBG_IND("get: %d\r\n",atoi(argv[0]));
    value = atoi(argv[0]);
    if (value > ON)
    {
        DBG_ERR("value =%d\r\n",value);
        return -1;
    }

    if (value == ON)
    {
        pMovieInfo->rsc.index = MOVIE_RSC_ON;
    }
    else
    {
        pMovieInfo->rsc.index = MOVIE_RSC_OFF;
    }

    pMovieInfo->rsc.isSet = 1;

    return 0;
}

static INT32 param_gsensor(INT32 argc, char* argv[])
{
    UINT32 value;
    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }
    DBG_IND("get: %d\r\n",atoi(argv[0]));
    value = atoi(argv[0]);
    if (value >= GSENSOR_ID_MAX)
    {
        DBG_ERR("value =%d\r\n",value);
        return -1;
    }

    pMovieInfo->gsensor.index = value;
    pMovieInfo->gsensor.isSet = 1;

    return 0;
}

static INT32 param_timelap(INT32 argc, char* argv[])
{
    UINT32 value;
    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }
    DBG_IND("get: %d\r\n",atoi(argv[0]));
    value = atoi(argv[0]);
    if (value >= MOVIE_TIMELAPSEREC_ID_MAX)
    {
        DBG_ERR("value =%d\r\n",value);
        return -1;
    }

    pMovieInfo->timelap.index = value;
    pMovieInfo->timelap.isSet = 1;

    return 0;
}

static INT32 param_ircut(INT32 argc, char* argv[])
{
    UINT32 value;
    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }
    DBG_IND("get: %d\r\n",atoi(argv[0]));
    value = atoi(argv[0]);
    if (value > ON)
    {
        DBG_ERR("value =%d\r\n",value);
        return -1;
    }

    if (value == ON)
    {
        pMovieInfo->ircut.index = MOVIE_IR_CUT_ON;
    }
    else
    {
        pMovieInfo->ircut.index = MOVIE_IR_CUT_OFF;
    }

    pMovieInfo->ircut.isSet = 1;

    return 0;
}

static INT32 param_rotate(INT32 argc, char* argv[])
{
    UINT32 value;
    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }
    DBG_IND("get: %d\r\n",atoi(argv[0]));
    value = atoi(argv[0]);
    if (value > ON)
    {
        DBG_ERR("value =%d\r\n",value);
        return -1;
    }

    if (value == ON)
    {
        pMovieInfo->rotate.index = SEN_ROTATE_ON;
    }
    else
    {
        pMovieInfo->rotate.index = SEN_ROTATE_OFF;
    }

    pMovieInfo->rotate.isSet = 1;

    return 0;
}

static INT32 param_murgpro(INT32 argc, char* argv[])
{
    UINT32 value;
    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }
    DBG_IND("get: %d\r\n",atoi(argv[0]));
    value = atoi(argv[0]);
    if (value > ON)
    {
        DBG_ERR("value =%d\r\n",value);
        return -1;
    }

    if (value == ON)
    {
        pMovieInfo->murgpro.index = MOVIE_URGENT_PROTECT_MANUAL_ON;
    }
    else
    {
        pMovieInfo->murgpro.index = MOVIE_URGENT_PROTECT_MANUAL_OFF;
    }

    pMovieInfo->murgpro.isSet = 1;

    return 0;
}

static INT32 param_aurgpro(INT32 argc, char* argv[])
{
    UINT32 value;
    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }
    DBG_IND("get: %d\r\n",atoi(argv[0]));
    value = atoi(argv[0]);
    if (value > ON)
    {
        DBG_ERR("value =%d\r\n",value);
        return -1;
    }

    if (value == ON)
    {
        pMovieInfo->aurgpro.index = MOVIE_URGENT_PROTECT_AUTO_ON;
    }
    else
    {
        pMovieInfo->aurgpro.index = MOVIE_URGENT_PROTECT_AUTO_OFF;
    }

    pMovieInfo->aurgpro.isSet = 1;

    return 0;
}

static INT32 param_pim(INT32 argc, char* argv[])
{
    UINT32 value;
    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }
    DBG_IND("get: %d\r\n",atoi(argv[0]));
    value = atoi(argv[0]);
    if (value > ON)
    {
        DBG_ERR("value =%d\r\n",value);
        return -1;
    }

    if (value == ON)
    {
        pMovieInfo->pim.index = MOVIE_PIM_ON;
    }
    else
    {
        pMovieInfo->pim.index = MOVIE_PIM_OFF;
    }

    pMovieInfo->pim.isSet = 1;

    return 0;
}

static INT32 param_dualcam(INT32 argc, char* argv[])
{
    UINT32 value;
    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }
    DBG_IND("get: %d\r\n",atoi(argv[0]));
    value = atoi(argv[0]);
    if (value >= DUALCAM_SETTING_MAX)
    {
        DBG_ERR("value =%d\r\n",value);
        return -1;
    }

    pMovieInfo->dualcam.index = value;
    pMovieInfo->dualcam.isSet = 1;

    return 0;
}

static INT32 param_start(INT32 argc, char* argv[])
{
    pMovieInfo->start.isSet = 1;
    return 0;
}

static INT32 param_stop(INT32 argc, char* argv[])
{
    pMovieInfo->stop.isSet = 1;
    return 0;
}

static INT32 param_snapshot(INT32 argc, char* argv[])
{
    if (argc >= 1)
        pMovieInfo->snapshot.index = atoi(argv[0]);
#if (MEDIA_VASTREAM==ENABLE)
    if (pMovieInfo->snapshot.index >= MOVIE_MAX_CHANNEL)
    {
        DBG_ERR("value =%d\r\n",pMovieInfo->snapshot.index);
        return -1;
    }
#endif
    pMovieInfo->snapshot.isSet = 1;
    return 0;
}


static INT32 param_fdstart(INT32 argc, char* argv[])
{
#if (MOVIE_FD_FUNC_ == ENABLE)
    UINT32 value;
    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }
    DBG_IND("get: %d\r\n",atoi(argv[0]));
    value = atoi(argv[0]);
    if (value == 0)
        FD_Lock(TRUE);
    else
        FD_Lock(FALSE);
    return 0;
#else
    DBG_ERR("not support\r\n");
    return -1;
#endif
}


static INT32 param_fdset(INT32 argc, char* argv[])
{
#if (MOVIE_FD_FUNC_ == ENABLE)
    UINT32 maxFace, faceScale, ProcessRate;
    if(argc!=3)
    {
        DBG_ERR("argc!=3 (%d)\r\n",argc);
        return -1;
    }
    maxFace = atoi(argv[0]);
    FD_Config(FD_CFG_MAX_FACE, maxFace);
    faceScale = atoi(argv[1]);
    FD_Config(FD_CFG_FACE_SCALE, faceScale);
    ProcessRate = atoi(argv[2]);
    FD_Config(FD_CFG_PROCESS_RATE, ProcessRate);
    DBG_IND("%d %d %d\r\n",maxFace,faceScale,ProcessRate);
    return 0;
#else
    DBG_ERR("not support\r\n");
    return -1;
#endif
}

static INT32 param_fdget(INT32 argc, char* argv[])
{
#if (MOVIE_FD_FUNC_ == ENABLE)
    UINT32 maxFace, faceScale, ProcessRate;
    char   retStr[32];

    maxFace = FD_GetConfig(FD_CFG_MAX_FACE);
    faceScale = FD_GetConfig(FD_CFG_FACE_SCALE);
    ProcessRate = FD_GetConfig(FD_CFG_PROCESS_RATE);
    snprintf(retStr, sizeof(retStr), "%d %d %d",maxFace,faceScale,ProcessRate);
    NvtUctrl_SetRetString(retStr);
    return 0;
#else
    DBG_ERR("not support\r\n");
    return -1;
#endif
}

static INT32 param_fdrslt(INT32 argc, char* argv[])
{
#if (MOVIE_FD_FUNC_ == ENABLE)
    char             retStr[32];
    FD_FACE          FdInfo[FD_NUM_MAX];
    UINT32           i,faceNumber;
    URECT            FdDispCoord={0,0,640,480};
    CodecIAObjData   rtnFdInfo[FD_NUM_MAX];
	BOOL bUpdated;

    if(argc!=4)
    {
        DBG_ERR("argc!=4 (%d)\r\n",argc);
        return -1;
    }

	bUpdated = FD_GetRsltFlag();

    FdDispCoord.x = atoi(argv[0]);
    FdDispCoord.y = atoi(argv[1]);
    FdDispCoord.w = atoi(argv[2]);
    FdDispCoord.h = atoi(argv[3]);
    faceNumber = FD_GetRsltFace(FdInfo,&FdDispCoord);
    for (i=0;i<faceNumber;i++)
    {
        rtnFdInfo[i].uiCoordinateX = FdInfo[i].x;
        rtnFdInfo[i].uiCoordinateY = FdInfo[i].y;
        rtnFdInfo[i].uiWidth = FdInfo[i].w;
        rtnFdInfo[i].uiHeight = FdInfo[i].h;
    }
    snprintf(retStr, sizeof(retStr),  "%d %d",faceNumber, bUpdated);
    NvtUctrl_SetConfigData(rtnFdInfo,sizeof(CodecIAObjData)*faceNumber);
    NvtUctrl_SetRetString(retStr);
    return 0;
#else
    DBG_ERR("not support\r\n");
    return -1;
#endif
}

static INT32 param_ftgstart(INT32 argc, char* argv[])
{
#if (MOVIE_FTG_FUNC == ENABLE && MOVIE_DIS == ENABLE)
    UINT32 value;
    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }
    DBG_IND("get: %d\r\n",atoi(argv[0]));
    value = atoi(argv[0]);
	if (value == 0)
    {
		#if (FTG_SNAPSHOT_FUN == ENABLE)
		g_bFTGOpen = FALSE;
		#endif
		Movie_FTGLock(TRUE);
        if (UI_GetData(FL_MovieDisIndex) == MOVIE_DIS_OFF)
        {
            ImageUnit_SetParam(&ISF_ImagePipe1, IPL_SEL_DISCOMP, SEL_DISCOMP_OFF);
        }
    }
    else
    {
	    #if (FTG_SNAPSHOT_FUN == ENABLE)
		g_bFTGOpen = TRUE;
		#endif
		Movie_FTGLock(FALSE);
        // movie dis menu is off, so we need enable ipl dis by ourself
        if (UI_GetData(FL_MovieDisIndex) == MOVIE_DIS_OFF)
        {
            dis_setDisViewRatio(1000);
            ImageUnit_SetParam(&ISF_ImagePipe1, IPL_SEL_DISCOMP, SEL_DISCOMP_ON);
        }
		//#NT#2016/11/22#Charlie Chang -begin
        //#NT# support FTG snapshot
        #if (FTG_SNAPSHOT_FUN == ENABLE)
        FTG_SNAPSHOT *ftg_snapshot_list = (FTG_SNAPSHOT *)OS_GetMempoolAddr(POOL_ID_FTG_SNAPSHOT);
        ////init pool
        memset(ftg_snapshot_list,0,sizeof(FTG_SNAPSHOT)*FTG_MAX_SNAPSHOT_NUM);
        #endif
        //#NT#2016/11/22#Charlie Chang -end
    }



    return 0;
#else
    DBG_ERR("not support\r\n");
    return -1;
#endif
}


static INT32 param_ftgset(INT32 argc, char* argv[])
{
#if (MOVIE_FTG_FUNC == ENABLE)
    UINT32 maxFace, faceScale, ProcessRate, MinFaceScore, drawRate, allInfo;
	UINT32 sensitivity;
    if(argc!=7)
    {
        DBG_ERR("argc!=7 (%d)\r\n",argc);
        return -1;
    }
    maxFace = atoi(argv[0]);
    ftg_config(ftg_fd_cfg_max_face, maxFace);
    faceScale = atoi(argv[1]);
    ftg_config(ftg_fd_cfg_face_scale, faceScale);
    ProcessRate = atoi(argv[2]);
    ftg_config(ftg_fd_cfg_process_rate, ProcessRate);
    MinFaceScore = atoi(argv[3]);
	ftg_config(ftg_cfg_min_face_score,MinFaceScore );
	drawRate= atoi(argv[4]);
    MovieStamp_SetConfig(MOVIESTAMP_FTG_DRAW_DELAY_COUNT,drawRate);
    allInfo = atoi(argv[5]);
	if(allInfo >= ftg_rlst_max_cnt){
		DBG_ERR("ftg allInfo type error allInfo=%d\r\n",allInfo);
		return -1;
	}
    MovieStamp_SetConfig(MOVIESTAMP_FTG_RLST_TYPE, allInfo);
	sensitivity= atoi(argv[6]);
	ftg_config(ftg_fd_cfg_sensitivity, sensitivity);


    DBG_IND("%d %d %d %d %d %d %d\r\n",
		maxFace,faceScale,ProcessRate,MinFaceScore,drawRate,allInfo,sensitivity);
    return 0;
#else
    DBG_ERR("not support\r\n");
    return -1;
#endif
}

static INT32 param_ftgget(INT32 argc, char* argv[])
{
#if (MOVIE_FTG_FUNC == ENABLE)
    UINT32 maxFace = 0, faceScale = 0, ProcessRate = 0, MinFaceScore = 0,drawRate =0, allInfo=0;
	UINT32 sensitivity =0;
    char   retStr[32];

	sensitivity = ftg_getconfig(ftg_fd_cfg_sensitivity);
    maxFace = ftg_getconfig(ftg_fd_cfg_max_face);
    faceScale = ftg_getconfig(ftg_fd_cfg_face_scale);
	ProcessRate = ftg_getconfig(ftg_fd_cfg_process_rate);
	MinFaceScore = ftg_getconfig(ftg_cfg_min_face_score);
	drawRate = MovieStamp_GetConfig(MOVIESTAMP_FTG_DRAW_DELAY_COUNT);
	allInfo = MovieStamp_GetConfig(MOVIESTAMP_FTG_RLST_TYPE);
    snprintf(retStr, sizeof(retStr), "%d %d %d %d %d %d %d",
		   maxFace,faceScale,ProcessRate,MinFaceScore,drawRate,allInfo,sensitivity);
    NvtUctrl_SetRetString(retStr);
    return 0;
#else
    DBG_ERR("not support\r\n");
    return -1;
#endif
}
#if (FTG_SNAPSHOT_FUN == ENABLE)
static INT32 mapping_ftg_snapshot_id(UINT32 ID){

    UINT32 i;
    FTG_SNAPSHOT *ftg_snapshot_list = (FTG_SNAPSHOT *)OS_GetMempoolAddr(POOL_ID_FTG_SNAPSHOT);
    ////find same face id
    for(i=0; i< FTG_MAX_SNAPSHOT_NUM;i++){
        if(ftg_snapshot_list[i].pic_num == 0){
            continue;
		}
        /////if id exist ,return the index
        if(ftg_snapshot_list[i].id == ID){
            return i;
		}

	}
    ////can not find id, assigned new index
    for(i=0; i<FTG_MAX_SNAPSHOT_NUM;i++){
        if(ftg_snapshot_list[i].pic_num == 0){
            return i;
		}
	}
    DBG_ERR("snapshot buffer is max, can not buffer id:%d anymore\r\n",ID);
    return -1;
}


static INT32 find_ftg_snapshot_index(UINT32 ID){

    UINT32 i;
    FTG_SNAPSHOT *ftg_snapshot_list = (FTG_SNAPSHOT *)OS_GetMempoolAddr(POOL_ID_FTG_SNAPSHOT);

    ////find same face id
    for(i=0; i< FTG_MAX_SNAPSHOT_NUM;i++){
        if(ftg_snapshot_list[i].pic_num == 0){
            continue;
		}
        /////if id exist ,return the index
        if(ftg_snapshot_list[i].id == ID){
            return i;
		}

	}

    DBG_ERR("can not find snapshot ID(%d)\r\n",ID);
    return -1;
}

static void ftg_snapshot_insert(UINT32 face_id, UINT32 face_index,UINT32 over_addr,UINT32 over_size,UINT32 face_addr,UINT32 face_size,UINT32 score){

    FTG_SNAPSHOT *ftg_snapshot_list = (FTG_SNAPSHOT *)OS_GetMempoolAddr(POOL_ID_FTG_SNAPSHOT);
    UINT32 i;
	UINT32 min_index=0;
	UINT32 min_score=0;

	for(i=0; i< FTG_MAX_FACE_PIC_NUM ; i++){
		if(i==0){
            min_score = ftg_snapshot_list[face_index].pic_list[i].score;
		}

        if(ftg_snapshot_list[face_index].pic_list[i].score < min_score){
            min_score = ftg_snapshot_list[face_index].pic_list[i].score;
			min_index = i;
		}

	}


	if(over_size > FTG_OVERRALL_MAX_BUF_SIZE){
        DBG_ERR("overrall size(%d) is bigger than FTG_FACE_MAX_BUF_SIZE(%d)\r\n",over_size,FTG_OVERRALL_MAX_BUF_SIZE);
	}
	else{
        memcpy(ftg_snapshot_list[face_index].pic_list[min_index].overrall_pic,(char *)over_addr, over_size);
        ftg_snapshot_list[face_index].pic_list[min_index].overrall_size = over_size;
	}

	if(face_size > FTG_FACE_MAX_BUF_SIZE){
        DBG_ERR("face size(%d) is bigger than FTG_FACE_MAX_BUF_SIZE(%d)\r\n",face_size,FTG_FACE_MAX_BUF_SIZE);
	}
	else{
        memcpy(ftg_snapshot_list[face_index].pic_list[min_index].face_pic,(char *)face_addr, face_size);
	    ftg_snapshot_list[face_index].pic_list[min_index].face_size= face_size;
	}

    ftg_snapshot_list[face_index].pic_list[min_index].score = score;

    ftg_snapshot_list[face_index].id = face_id;
	if(ftg_snapshot_list[face_index].pic_num < FTG_MAX_FACE_PIC_NUM){
    	ftg_snapshot_list[face_index].pic_num ++;
	}

    //charlie test
    DBG_DUMP("insert ftg snapshot=======\r\n");
	DBG_DUMP("ftg_snapshot_list[%d].id=%d\r\n",face_index,ftg_snapshot_list[face_index].id);
	DBG_DUMP("ftg_snapshot_list[%d].pic_num=%d\r\n",face_index,ftg_snapshot_list[face_index].pic_num);

	for(i=0; i < ftg_snapshot_list[face_index].pic_num; i++){
        DBG_DUMP("pic [%d]=======\r\n",i);
		DBG_DUMP("socre =%d\r\n",ftg_snapshot_list[face_index].pic_list[i].score);
        DBG_DUMP("overrall =%x %x %x %x %x %x\r\n",
			ftg_snapshot_list[face_index].pic_list[i].overrall_pic[0],
            ftg_snapshot_list[face_index].pic_list[i].overrall_pic[1],
            ftg_snapshot_list[face_index].pic_list[i].overrall_pic[2],
            ftg_snapshot_list[face_index].pic_list[i].overrall_pic[3],
            ftg_snapshot_list[face_index].pic_list[i].overrall_pic[4],
            ftg_snapshot_list[face_index].pic_list[i].overrall_pic[5]
		);
		DBG_DUMP("overrall size=%d\r\n",ftg_snapshot_list[face_index].pic_list[i].overrall_size);
        DBG_DUMP("face =%x %x %x %x %x %x\r\n",
			ftg_snapshot_list[face_index].pic_list[i].face_pic[0],
            ftg_snapshot_list[face_index].pic_list[i].face_pic[1],
            ftg_snapshot_list[face_index].pic_list[i].face_pic[2],
            ftg_snapshot_list[face_index].pic_list[i].face_pic[3],
            ftg_snapshot_list[face_index].pic_list[i].face_pic[4],
            ftg_snapshot_list[face_index].pic_list[i].face_pic[5]
		);
		DBG_DUMP("face size=%d\r\n",ftg_snapshot_list[face_index].pic_list[i].face_size);
	}
    //end charlie test



    return;

}

#endif
static INT32 param_ftgsnapshotnotify(INT32 argc, char* argv[]){

    #if (FTG_SNAPSHOT_FUN == ENABLE)


    UINT32 face_id = atoi(argv[0]);
	UINT32 score  = atoi(argv[1]);
    UINT32 i;
    ER er;
    UINT32  uiLineOffset[MAX_PLANE_NUM];
    UINT32  uiPxlAddr[MAX_PLANE_NUM];
    IMG_BUF SrcImg, FaceSrcImg;
	UINT32 channelID= 0;
    UINT32  Width = GetMovieSizeWidth_2p(channelID,0);
    UINT32  Height = GetMovieSizeHeight_2p(channelID,0);
   // MEDIAREC_YUV_INFO   mediaRecInfo={0};
    UINT32  BufAddr = OS_GetMempoolAddr(POOL_ID_IPCAM_SNAPSHOT);
    UINT32  BufSize = OS_GetMempoolSize(POOL_ID_IPCAM_SNAPSHOT);
    UINT32  jpgSize;
    INT32 ftg_snapshot_index =-1;

	UINT32 overrall_size, face_size;
	//MEM_RANGE  overrall_ftg_snapShotData = {0};
	//MEM_RANGE  face_ftg_snapShotData = {0};
	UINT32 YUV_size = CROP_SOURCE_W*CROP_SOURCE_H*2;
	UINT32 YUV_addr = OS_GetMempoolAddr(POOL_ID_FTG_SNAPSHOT) + FTG_MAX_SNAPSHOT_SIZE - YUV_size;
    UINT32 overrall_snapshot_addr = OS_GetMempoolAddr(POOL_ID_FTG_SNAPSHOT) + FTG_MAX_SNAPSHOT_SIZE - YUV_size - FTG_OVERRALL_MAX_BUF_SIZE;
	UINT32 face_snapshot_addr = OS_GetMempoolAddr(POOL_ID_FTG_SNAPSHOT) + FTG_MAX_SNAPSHOT_SIZE - YUV_size - FTG_OVERRALL_MAX_BUF_SIZE - FTG_FACE_MAX_BUF_SIZE;
    DBG_DUMP("face_id=%d score=%d\r\n",face_id,score);

    if(argc!=2)
    {
        DBG_ERR("argc!=2 (%d)\r\n",argc);
        return -1;
    }


    if(face_id <= 0){
        DBG_ERR("face_id:%d error\r\n",face_id);
        return -1;
	}

	BOOL check_ftg_list_id = FALSE;
	for(i=0; i < FTG_MAX_SNAPSHOT_NUM; i++ ){
        if(g_ftg_list[i].id == face_id){
            ftg_snapshot_index = i;
			check_ftg_list_id= TRUE;
			break;
		}
	}
	if(check_ftg_list_id == FALSE){
        DBG_ERR("can not find id (%d) in ftg list\r\n",face_id);
		return -1;
	}


    ////need to do, to get face tracking granding YUV buffer
   // MovRec_GetLastV_YUV(channelID,&mediaRecInfo);

    uiPxlAddr[0] = g_ftg_list[ftg_snapshot_index].p_frame->PxlAddr[0];
    uiPxlAddr[1] = g_ftg_list[ftg_snapshot_index].p_frame->PxlAddr[1];
    uiPxlAddr[2] = g_ftg_list[ftg_snapshot_index].p_frame->PxlAddr[2];
    uiLineOffset[0]    = g_ftg_list[ftg_snapshot_index].p_frame->LineOffset[0];
    uiLineOffset[1]    = g_ftg_list[ftg_snapshot_index].p_frame->LineOffset[1];
    uiLineOffset[2]    = g_ftg_list[ftg_snapshot_index].p_frame->LineOffset[2];

    //DBG_IND("mediaRecInfo.Yaddr 0x%x, Ylot=0x%x\r\n",mediaRecInfo.Yaddr, mediaRecInfo.Ylot);
/*
    uiPxlAddr[0] = mediaRecInfo.Yaddr;
    uiPxlAddr[1] = mediaRecInfo.UVaddr;
    uiPxlAddr[2] = mediaRecInfo.UVaddr;
    uiLineOffset[0]    = mediaRecInfo.Ylot;
    uiLineOffset[1]    = mediaRecInfo.Ylot;
    uiLineOffset[2]    = mediaRecInfo.Ylot;
*/
	////snapshot overrall
    er = GxImg_InitBufEx(&SrcImg, Width, Height,GX_IMAGE_PIXEL_FMT_YUV420, uiLineOffset, uiPxlAddr);
	if(er != E_OK){
        DBG_ERR("GxImg_InitBufEx error er=%d\r\n",er);
		return -1;

	}
    jpgSize = SnapShot_EncJPG(BufAddr, BufSize, &SrcImg);
	memcpy((char *)overrall_snapshot_addr,(char *) BufAddr, jpgSize);
	overrall_size = jpgSize;
    //overrall_ftg_snapShotData.Addr = BufAddr;
    //overrall_ftg_snapShotData.Size = jpgSize;
    DBG_IND("Snapshot overrall size=0x%x\r\n", overrall_size);

	//////////////////////snapshot face
    er = GxImg_InitBufEx(&SrcImg, Width, Height,GX_IMAGE_PIXEL_FMT_YUV420, uiLineOffset, uiPxlAddr);
    if(er != E_OK){
        DBG_ERR("GxImg_InitBufEx face error er=%d\r\n",er);
        return -1;
	}
    IRECT face_rec={0};
	face_rec.h = g_ftg_list[ftg_snapshot_index].height;
	face_rec.w = g_ftg_list[ftg_snapshot_index].width;
	face_rec.x = g_ftg_list[ftg_snapshot_index].stx;
	face_rec.y = g_ftg_list[ftg_snapshot_index].sty;


	er = GxImg_InitBuf(&FaceSrcImg, face_rec.w, face_rec.h,GX_IMAGE_PIXEL_FMT_YUV420,
		     GXIMAGE_LINEOFFSET_ALIGN(16), YUV_addr, YUV_size);
    if(er != E_OK){
        DBG_ERR("GxImg_InitBuf error er=%d\r\n");
		return -1;
	}
    er = GxImg_CopyData(&SrcImg, &face_rec, &FaceSrcImg, NULL);
    if(er != E_OK){
        DBG_ERR("GxImg_CopyData error er=%d\r\n");
		return -1;
	}

    jpgSize = SnapShot_EncJPG(BufAddr, BufSize, &FaceSrcImg);
	memcpy((char *)face_snapshot_addr, (char *)BufAddr, jpgSize);
	face_size = jpgSize;

    DBG_IND("Snapshot  size=0x%x\r\n",face_size);

    ////mapping face id
    ftg_snapshot_index = mapping_ftg_snapshot_id(face_id);
    if(ftg_snapshot_index == -1){
        DBG_ERR("assgind FTG snapshot index error!!\r\n");
		return -1;
	}
    DBG_IND("ftg_snapshot_index=%d\r\n",ftg_snapshot_index);
    ftg_snapshot_insert(face_id, ftg_snapshot_index, overrall_snapshot_addr,
		overrall_size, face_snapshot_addr, face_size, score);
	return 0;
	#else
	DBG_ERR("not support \r\n");
	return -1;
	#endif



}

static INT32 param_ftgsetroi(INT32 argc, char* argv[]){

#if (MOVIE_FTG_FUNC == ENABLE)
    URECT roi_rec ={0};
    UINT32 use_roi;
    if(argc!=5)
    {
        DBG_ERR("argc!=5 (%d)\r\n",argc);
        return -1;
    }
    use_roi =  atoi(argv[0]);
    roi_rec.x= atoi(argv[1]);
    roi_rec.y = atoi(argv[2]);
	roi_rec.w = atoi(argv[3]);
	roi_rec.h = atoi(argv[4]);
    ftg_setfdroi(use_roi, &roi_rec);
    DBG_IND("%d %d %d\r\n",use_roi,roi_rec.x,roi_rec.y,roi_rec.w,roi_rec.h);
    return 0;
#else
    DBG_ERR("not support\r\n");
    return -1;
#endif

}


static INT32 param_ftgsnapshotget(INT32 argc, char* argv[]){

    #if (FTG_SNAPSHOT_FUN == ENABLE)

    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }
    UINT32 id = atoi(argv[0]);
	INT32 face_index;
	char   retStr[128];
	FTG_SNAPSHOT *ftg_snapshot_list = (FTG_SNAPSHOT *)OS_GetMempoolAddr(POOL_ID_FTG_SNAPSHOT);
    face_index = find_ftg_snapshot_index(id);
    if(face_index < 0){
        DBG_ERR("find_ftg_snapshot_index error face_index=%d\r\n",face_index);
		return -1;

	}

	snprintf(retStr,sizeof(retStr),"%d %lu",face_index<0?-1:0, NvtMem_GetPhyAddr((UINT32)&ftg_snapshot_list[face_index]));
	DBG_DUMP("snapshot:%s\r\n",retStr);
    //charlie test
    DBG_DUMP("get ftg snapshot=======\r\n");
	DBG_DUMP("ftg_snapshot_list[%d].id=%d\r\n",face_index,ftg_snapshot_list[face_index].id);
	DBG_DUMP("ftg_snapshot_list[%d].pic_num=%d\r\n",face_index,ftg_snapshot_list[face_index].pic_num);
	UINT32 i;
	for(i=0; i < ftg_snapshot_list[face_index].pic_num; i++){
        DBG_DUMP("pic [%d]=======\r\n",i);
		DBG_DUMP("socre =%d\r\n",ftg_snapshot_list[face_index].pic_list[i].score);
        DBG_DUMP("overrall =%x %x %x %x %x %x\r\n",
			ftg_snapshot_list[face_index].pic_list[i].overrall_pic[0],
            ftg_snapshot_list[face_index].pic_list[i].overrall_pic[1],
            ftg_snapshot_list[face_index].pic_list[i].overrall_pic[2],
            ftg_snapshot_list[face_index].pic_list[i].overrall_pic[3],
            ftg_snapshot_list[face_index].pic_list[i].overrall_pic[4],
            ftg_snapshot_list[face_index].pic_list[i].overrall_pic[5]
		);
		DBG_DUMP("overrall size=%d\r\n",ftg_snapshot_list[face_index].pic_list[i].overrall_size);
        DBG_DUMP("face =%x %x %x %x %x %x\r\n",
			ftg_snapshot_list[face_index].pic_list[i].face_pic[0],
            ftg_snapshot_list[face_index].pic_list[i].face_pic[1],
            ftg_snapshot_list[face_index].pic_list[i].face_pic[2],
            ftg_snapshot_list[face_index].pic_list[i].face_pic[3],
            ftg_snapshot_list[face_index].pic_list[i].face_pic[4],
            ftg_snapshot_list[face_index].pic_list[i].face_pic[5]
		);
		DBG_DUMP("face size=%d\r\n",ftg_snapshot_list[face_index].pic_list[i].face_size);
	}
    //end charlie test


    NvtUctrl_SetRetString(retStr);
	return 0;
	#else
	DBG_ERR("not support!\r\n");
	return -1;
	#endif



}
static INT32 param_ftgsnapshotremove(INT32 argc, char* argv[]){

    #if (FTG_SNAPSHOT_FUN == ENABLE)

    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }
    UINT32 id = atoi(argv[0]);
	INT32 face_index;
	FTG_SNAPSHOT *ftg_snapshot_list = (FTG_SNAPSHOT *)OS_GetMempoolAddr(POOL_ID_FTG_SNAPSHOT);
    face_index = find_ftg_snapshot_index(id);
     if(face_index < 0){
        DBG_ERR("find_ftg_snapshot_index error face_index=%d\r\n",face_index);
		return -1;

	}
    memset(&ftg_snapshot_list[face_index],0,sizeof(FTG_SNAPSHOT));
    DBG_DUMP("remove face id:%d\r\n",id);
	return 0;
	#else
	DBG_ERR("not support\r\n");
	return -1;
	#endif



}



static INT32 param_ftggetroi(INT32 argc, char* argv[])
{
#if (MOVIE_FTG_FUNC == ENABLE)
    URECT roi_rec ={0};
    BOOL use_roi;
    char   retStr[128];

    use_roi = ftg_getfdroi(&roi_rec);
    snprintf(retStr, sizeof(retStr), "%d %d %d %d",use_roi,roi_rec.x,roi_rec.y,roi_rec.w,roi_rec.h);
    NvtUctrl_SetRetString(retStr);
    return 0;
#else
    DBG_ERR("not support\r\n");
    return -1;
#endif
}

static INT32 param_ftgrslt(INT32 argc, char* argv[])
{
#if (MOVIE_FTG_FUNC == ENABLE)
    char             retStr[32];
    ftg_face         FtgInfo[FD_NUM_MAX];
    UINT32           i,faceNumber;
    URECT            FdDispCoord={0,0,640,480};
    CodecFTGObjData  rtnFdInfo[FD_NUM_MAX];
	BOOL             bUpdated = TRUE;

    if(argc!=4)
    {
        DBG_ERR("argc!=4 (%d)\r\n",argc);
        return -1;
    }
	//bUpdated = FD_GetRsltFlag();
    FdDispCoord.x = atoi(argv[0]);
    FdDispCoord.y = atoi(argv[1]);
    FdDispCoord.w = atoi(argv[2]);
    FdDispCoord.h = atoi(argv[3]);
	ftg_rlst_type ftg_type;
    ftg_type = MovieStamp_GetConfig(MOVIESTAMP_FTG_RLST_TYPE);
    faceNumber = ftg_getrsltface2(FtgInfo,&FdDispCoord,ftg_type);
	ftg_idmap(FtgInfo, faceNumber);
    for (i=0;i<faceNumber;i++)
    {
        rtnFdInfo[i].stx = FtgInfo[i].stx;
        rtnFdInfo[i].sty = FtgInfo[i].sty;
        rtnFdInfo[i].width = FtgInfo[i].width;
        rtnFdInfo[i].height = FtgInfo[i].height;
		rtnFdInfo[i].id = FtgInfo[i].id;
		rtnFdInfo[i].type = FtgInfo[i].type;
		rtnFdInfo[i].clarity = FtgInfo[i].clarity;
		rtnFdInfo[i].luma = FtgInfo[i].luma;
		rtnFdInfo[i].angle_horizontal = FtgInfo[i].angle_horizontal;
		rtnFdInfo[i].angle_vertical = FtgInfo[i].angle_vertical;
		rtnFdInfo[i].angle_roll = FtgInfo[i].angle_roll;
		rtnFdInfo[i].score = FtgInfo[i].score;

    }

	//#NT#2016/11/21#Charlie Chang -begin
    //#NT# support FTG snapshot
	#if (FTG_SNAPSHOT_FUN == ENABLE)
	    if(faceNumber > FTG_MAX_SNAPSHOT_NUM){
			DBG_ERR("face number (%d) is over FTG_MAX_NUM (%d), only snapshot (%d)\r\n",
				faceNumber,FTG_MAX_SNAPSHOT_NUM,FTG_MAX_SNAPSHOT_NUM);
			faceNumber = FTG_MAX_SNAPSHOT_NUM;
		}
        memcpy( &g_ftg_list, &FtgInfo, sizeof(ftg_face)*faceNumber );
		g_face_number = faceNumber;

        //charlie test
        /*
        DBG_DUMP("FTG detect==============\r\n");
        for (i=0;i<g_face_number;i++)
        {
            DBG_DUMP("index=%d \r\n"
				     "x=%d y=%d w=%d h=%d id=%d type=%d clarity=%d \r\n"
				     "a_h=%d a_v=%d a_r=%d score=%d\r\n",
				i,
				g_ftg_list[i].stx,
				g_ftg_list[i].sty,
				g_ftg_list[i].width,
				g_ftg_list[i].height,
				g_ftg_list[i].id,
				g_ftg_list[i].type,
				g_ftg_list[i].clarity,
				g_ftg_list[i].angle_horizontal,
				g_ftg_list[i].angle_vertical,
				g_ftg_list[i].angle_roll,
				g_ftg_list[i].score

			);
        }
        */
		//end charlie test

	#endif
	//#NT#2016/11/21#Charlie Chang -end
    snprintf(retStr, sizeof(retStr),  "%d %d",faceNumber, bUpdated);
    NvtUctrl_SetConfigData(rtnFdInfo,sizeof(ftg_face)*faceNumber);
    NvtUctrl_SetRetString(retStr);
    return 0;
#else
    DBG_ERR("not support\r\n");
    return -1;
#endif
}
//#NT#2016/05/23#David Tsai -begin
//#NT# Add tampering detection control for uctrl
static INT32 param_tdstart(INT32 argc, char* argv[])
{
#if (MOVIE_TD_FUNC == ENABLE)
    UINT32 TdFgPct=0, TdFgDuration=0, TdFgLifeTime=0, TdBgUpdateSpeed=0;
    UINT32 value;
    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }
    DBG_IND("get: %d\r\n",atoi(argv[0]));
    value = atoi(argv[0]);
    if (value == 0)
    {
        TdFgPct = TDGetPrms(TD_SET_FG_PCT);
        TdFgDuration = TDGetPrms(TD_SET_FG_DURATION);
        TdFgLifeTime = TDGetPrms(TD_SET_FG_LIFE_TIME);
        TdBgUpdateSpeed = TDGetPrms(TD_SET_BG_UPDATE_SPEED);

        TDReset();

        TDSetPrms(TD_SET_FG_PCT , TdFgPct);
        TDSetPrms(TD_SET_FG_DURATION, TdFgDuration);
        TDSetPrms(TD_SET_FG_LIFE_TIME, TdFgLifeTime);
        TDSetPrms(TD_SET_BG_UPDATE_SPEED, TdBgUpdateSpeed);

        Movie_BCLock(TRUE);
    }
    else
    {
        Movie_BCLock(FALSE);
    }
    return 0;
#else
    DBG_ERR("not support\r\n");
    return -1;
#endif
}


static INT32 param_tdset(INT32 argc, char* argv[])
{
#if (MOVIE_TD_FUNC == ENABLE)
    UINT32 TdFgPct, TdFgDuration, TdFgLifeTime, TdBgUpdateSpeed, TdProcessRate, TdSensitivity=100;
    if(argc < 5)
    {
        DBG_ERR("argc < 5 (%d)\r\n",argc);
        return -1;
    }
    else if (argc >= 6)
    {
        TdSensitivity = atoi(argv[5]);
        TDSetPrms(TD_SET_SENSI , TdSensitivity);
    }
    TdFgPct = atoi(argv[0]);
    TDSetPrms(TD_SET_FG_PCT , TdFgPct);
    TdFgDuration = atoi(argv[1]);
    TDSetPrms(TD_SET_FG_DURATION, TdFgDuration);
    TdFgLifeTime = atoi(argv[2]);
    TDSetPrms(TD_SET_FG_LIFE_TIME, TdFgLifeTime);
    TdBgUpdateSpeed = atoi(argv[3]);
    TDSetPrms(TD_SET_BG_UPDATE_SPEED, TdBgUpdateSpeed);
    TdProcessRate = atoi(argv[4]);
    Movie_BCProcessRateSet(TdProcessRate);
    DBG_IND("%d %d %d %d %d %d\r\n", TdFgPct, TdFgDuration, TdFgLifeTime, TdBgUpdateSpeed, TdProcessRate, TdSensitivity);
    return 0;
#else
    DBG_ERR("not support\r\n");
    return -1;
#endif
}

static INT32 param_tdget(INT32 argc, char* argv[])
{
#if (MOVIE_TD_FUNC == ENABLE)
    UINT32 TdFgPct=0, TdFgDuration=0, TdFgLifeTime=0, TdBgUpdateSpeed=0, TdProcessRate=0, TdSensitivity=0;
    char   retStr[32];

    TdFgPct = TDGetPrms(TD_SET_FG_PCT);
    TdFgDuration = TDGetPrms(TD_SET_FG_DURATION);
    TdFgLifeTime = TDGetPrms(TD_SET_FG_LIFE_TIME);
    TdBgUpdateSpeed = TDGetPrms(TD_SET_BG_UPDATE_SPEED);
    TdSensitivity = TDGetPrms(TD_SET_SENSI);

    TdProcessRate = Movie_BCProcessRateGet();
    snprintf(retStr, sizeof(retStr), "%d %d %d %d %d %d", TdFgPct, TdFgDuration, TdFgLifeTime, TdBgUpdateSpeed, TdProcessRate, TdSensitivity);
    NvtUctrl_SetRetString(retStr);
    return 0;
#else
    DBG_ERR("not support\r\n");
    return -1;
#endif
}

static INT32 param_tdrslt(INT32 argc, char* argv[])
{
#if (MOVIE_TD_FUNC == ENABLE)
    char             retStr[32];

    snprintf(retStr, sizeof(retStr), "%d", TDGetAlarm() == TRUE ? 1 : 0);
    NvtUctrl_SetRetString(retStr);
    return 0;
#else
    DBG_ERR("not support\r\n");
    return -1;
#endif
}
//#NT#2016/05/23#David Tsai -end

//#NT#2016/08/26#Lincy Lin -begin
//#NT# Add uctrl command for BC/MV/IPL buffer APIs .
static INT32 param_dspbuf_get(INT32 argc, char* argv[])
{
    IVSBuf  sBuf = {0};

    #if (LINUX_CVHOST_FUNC == ENABLE)
    sBuf.uiAddr = NvtMem_GetPhyAddr(OS_GetMempoolAddr(POOL_ID_CVHOST));
    sBuf.uiSize = OS_GetMempoolSize(POOL_ID_CVHOST);

    //DBGH(sBuf.uiAddr);
    //DBGH(sBuf.uiSize);
    #endif
    NvtUctrl_SetConfigData((void *)&sBuf, sizeof(sBuf));
    return 0;
}

static IVSPixelFormat xGximg_pixelfmt_to_ivs_pixelfmt(GX_IMAGE_PIXEL_FMT  Gximg_pixelfmt)
{
    switch (Gximg_pixelfmt)
    {
        case GX_IMAGE_PIXEL_FMT_YUV422_PLANAR:
            return IVS_PIXEL_FMT_YUV422_PLANAR;

        case GX_IMAGE_PIXEL_FMT_YUV420_PLANAR:
            return IVS_PIXEL_FMT_YUV420_PLANAR;

        case GX_IMAGE_PIXEL_FMT_YUV422_PACKED:
            return IVS_PIXEL_FMT_YUV422_PACKED;

        case GX_IMAGE_PIXEL_FMT_YUV420_PACKED:
            return IVS_PIXEL_FMT_YUV420_PACKED;

        case GX_IMAGE_PIXEL_FMT_Y_ONLY:
            return IVS_PIXEL_FMT_Y_ONLY;

        case GX_IMAGE_PIXEL_FMT_ARGB565:
            return IVS_PIXEL_FMT_ARGB565;

        case GX_IMAGE_PIXEL_FMT_RGB888_PLANAR:
            return IVS_PIXEL_FMT_RGB888_PLANAR;

        case GX_IMAGE_PIXEL_FMT_ARGB888_PACKED:
            return IVS_PIXEL_FMT_ARGB888_PACKED;

        default:
            DBG_ERR("Unknown pxlfmt %d\r\n",Gximg_pixelfmt);
            return IVS_PIXEL_FMT_YUV420_PACKED;

    }
}


static INT32 param_img_get(INT32 argc, char* argv[])
{
    IMG_BUF       InputImg;
    IMG_BUF       tmpImg;
    UINT32        channel;
    BOOL          bIsCopy;
    UINT32        tmpbufAddr, tmpbufSize, i;
    IVSImageBuf   imgbuf={0};
    ISF_PORT*     pSrc;


    if(argc!=4)
    {
        DBG_ERR("argc!=4 (%d)\r\n",argc);
        goto img_get_fail;
    }
    channel = atoi(argv[0]);
    bIsCopy = atoi(argv[1]);
    sscanf_s( argv[2], "%x", &tmpbufAddr);
    sscanf_s( argv[3], "%x", &tmpbufSize);
    DBG_IND("channel= %d, bIsCopy =%d, tmpbufAddr = 0x%x, tmpbufSize = 0x%x\r\n",channel,bIsCopy,tmpbufAddr,tmpbufSize);
    if (channel > 4)
    {
        DBG_ERR("channel %d\r\n",channel);
        goto img_get_fail;
    }
    if (bIsCopy && (tmpbufAddr == 0 || tmpbufSize == 0))
    {
        DBG_ERR("tmpbufAddr 0x%x, tmpbufSize 0x%x\r\n",tmpbufAddr,tmpbufSize);
        goto img_get_fail;
    }
    pSrc = ImageUnit_Out(&ISF_ImagePipe1, ISF_OUT1+channel);
    if(!pSrc || (pSrc->Flag & ISF_PORT_FLAG_PAUSE))
    {
        DBG_ERR("IPL path %d is not enable\r\n",channel);
        goto img_get_fail;
    }
    ImageUnit_SetParam(&ISF_ImagePipe1,IMAGEPIPE_PARAM_PULLBUF_OUT1+channel, (UINT32)&InputImg);
    if (bIsCopy)
    {
        tmpbufAddr = NvtMem_GetNonCacheAddr(tmpbufAddr);
        if (GxImg_InitBuf(&tmpImg, InputImg.Width, InputImg.Height, GX_IMAGE_PIXEL_FMT_Y_ONLY, InputImg.LineOffset[0], tmpbufAddr, tmpbufSize) != E_OK)
        {
            DBG_ERR("GxImg_InitBuf fail\r\n");
            goto img_get_fail;
        }
        GxImg_ScaleData(&InputImg,REGION_MATCH_IMG,&tmpImg,REGION_MATCH_IMG);
        InputImg = tmpImg;
    }
    //GxImg_DumpBuf("A:\\dspimg.raw", &InputImg, GX_IMAGE_DUMP_ALL);
    imgbuf.uiWidth = InputImg.Width;
    imgbuf.uiHeight = InputImg.Height;
    imgbuf.ePxlFmt = xGximg_pixelfmt_to_ivs_pixelfmt(InputImg.PxlFmt);
    for (i=0;i<MAX_PLANE_NUM;i++)
    {
        if (InputImg.PxlAddr[i])
            imgbuf.uiPxlAddr[i] = NvtMem_GetPhyAddr(InputImg.PxlAddr[i]);
    }
    memcpy(imgbuf.uiLineOffset, InputImg.LineOffset, sizeof(UINT32)*MAX_PLANE_NUM);
    NvtUctrl_SetConfigData((void *)&imgbuf, sizeof(imgbuf));
    return 0;

img_get_fail:
    NvtUctrl_SetConfigData((void *)&imgbuf, sizeof(imgbuf));
    return -1;

}

STATIC_ASSERT(sizeof(IVSMVInfor) == sizeof(MOTION_INFOR));


static INT32 param_mvstart(INT32 argc, char* argv[])
{
#if (MOVIE_DIS == ENABLE)
    UINT32 value;
    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }
    DBG_IND("get: %d\r\n",atoi(argv[0]));
    value = atoi(argv[0]);
    if (value == 0)
    {
        if (UI_GetData(FL_MovieDisIndex) == MOVIE_DIS_OFF)
        {
            ImageUnit_SetParam(&ISF_ImagePipe1, IPL_SEL_DISCOMP, SEL_DISCOMP_OFF);
        }
    }
    else
    {
        // movie dis menu is off, so we need enable ipl dis by ourself
        if (UI_GetData(FL_MovieDisIndex) == MOVIE_DIS_OFF)
        {
            dis_setDisViewRatio(1000);
            ImageUnit_SetParam(&ISF_ImagePipe1, IPL_SEL_DISCOMP, SEL_DISCOMP_ON);
        }
    }
    return 0;
#else
    DBG_ERR("not support\r\n");
    return -1;
#endif
}
static INT32 param_mvset(INT32 argc, char* argv[])
{
#if (MOVIE_DIS == ENABLE)
    IVSMVSetting   mvsetting={0};
    DIS_ROIGMV_IN  roiIn;
    DISALG_CFGINFO DisConfigInfo;

    NvtUctrl_GetConfigData((void *)&mvsetting, sizeof(mvsetting));
    DisConfigInfo.iCfgVal = mvsetting.bGlobalMotionEnable;
    dis_SetConfig(DISALG_CFG_GLOBALMOTEN, &DisConfigInfo);
    DisConfigInfo.iCfgVal = mvsetting.bROIMotionEnable;
    dis_SetConfig(DISALG_CFG_ROIMOTEN, &DisConfigInfo);
    roiIn.uiStartX = mvsetting.uiROIStartX;
    roiIn.uiStartY = mvsetting.uiROIStartY;
    roiIn.uiSizeX = mvsetting.uiROISizeX;
    roiIn.uiSizeY = mvsetting.uiROISizeY;
    dis_roi_setInputInfo(&roiIn);
    DBG_IND("GM=%d,ROIM=%d,roi %d,%d,%d,%d, inH %d,inV %d\r\n",mvsetting.bGlobalMotionEnable,mvsetting.bROIMotionEnable,mvsetting.uiROIStartX,mvsetting.uiROIStartY,mvsetting.uiROISizeX,mvsetting.uiROISizeY,mvsetting.uiInputSizeH,mvsetting.uiInputSizeV);
    return 0;
#else
    DBG_ERR("not support\r\n");
    return -1;
#endif
}

static INT32 param_mvget(INT32 argc, char* argv[])
{
#if (MOVIE_DIS == ENABLE)
    IVSMVSetting   mvsetting={0};
    DISALG_PARAM   DisInputInfo;
    DIS_ROIGMV_IN  roiIn={0};
    DISALG_CFGINFO DisConfigInfo;

    dis_GetConfig(DISALG_CFG_GLOBALMOTEN, &DisConfigInfo);
    mvsetting.bGlobalMotionEnable = DisConfigInfo.iCfgVal;
    dis_GetConfig(DISALG_CFG_ROIMOTEN, &DisConfigInfo);
    mvsetting.bROIMotionEnable = DisConfigInfo.iCfgVal;
    dis_roi_getInputInfo(&roiIn);
    mvsetting.uiROIStartX = roiIn.uiStartX;
    mvsetting.uiROIStartY = roiIn.uiStartY;
    mvsetting.uiROISizeX = roiIn.uiSizeX;
    mvsetting.uiROISizeY = roiIn.uiSizeY;
    dis_getInputInfo(&DisInputInfo);
    mvsetting.uiInputSizeH = DisInputInfo.InSizeH;
    mvsetting.uiInputSizeV = DisInputInfo.InSizeV;
    DBG_IND("GM=%d,ROIM=%d,roi %d,%d,%d,%d, inH %d,inV %d\r\n",mvsetting.bGlobalMotionEnable,mvsetting.bROIMotionEnable,mvsetting.uiROIStartX,mvsetting.uiROIStartY,mvsetting.uiROISizeX,mvsetting.uiROISizeY,mvsetting.uiInputSizeH,mvsetting.uiInputSizeV);
    NvtUctrl_SetConfigData((void *)&mvsetting, sizeof(mvsetting));
    return 0;
#else
    DBG_ERR("not support\r\n");
    return -1;
#endif
}


static INT32 param_mvrslt(INT32 argc, char* argv[])
{
#if (MOVIE_DIS == ENABLE)
    static MV_INFO_SIZE   DisMvInfo;
    static IVSMVData      Mvdata;
    DisMvInfo.motVec = (MOTION_INFOR *)&Mvdata.sIVSMVROIResult;
    dis_getReadyMotionVec(&DisMvInfo);
    Mvdata.uiNumH = DisMvInfo.uiNumH;
    Mvdata.uiNumV = DisMvInfo.uiNumV;
    NvtUctrl_SetConfigData((void *)&Mvdata, sizeof(Mvdata));
    #if 0
    DBG_IND("uiNumH %d, uiNumV %d\r\n",DisMvInfo.uiNumH,DisMvInfo.uiNumV);
    {
        UINT32      num,i;
        IVSMVInfor *pMvInfo;

        num = Mvdata.uiNumH*Mvdata.uiNumV;
        for (i=0;i<num;i++)
        {
            pMvInfo = &Mvdata.sIVSMVROIResult[i];
            DBG_IND("pMvInfo[%d]=>x %d, y %d, Sad %d, Cnt %d, blkidx %d, valid %d\r\n",i,pMvInfo->iX,pMvInfo->iY,pMvInfo->uiSad,pMvInfo->uiCnt,pMvInfo->uiIdx,pMvInfo->bValid);
            if (i > 20)
            {
                DBG_IND("====Ignore====\r\n");
                break;
            }
        }
    }
    #endif
    return 0;
#else
    DBG_ERR("not support\r\n");
    return -1;
#endif
}

static INT32 param_blendget(INT32 argc, char* argv[])
{
#if (IPCAM_FUNC)
	MOVIESTAMP_BLENDING_CFG *pstmpBlend;
	MOVIESTAMP_BLENDING_CFG sblendSetting;
	pstmpBlend = (MOVIESTAMP_BLENDING_CFG *)MovieStamp_GetConfig(MOVIESTAMP_BLENDING);
	memcpy(&sblendSetting, pstmpBlend, sizeof(MOVIESTAMP_BLENDING_CFG));
	NvtUctrl_SetConfigData((void *)&sblendSetting, sizeof(sblendSetting));
    return 0;
#else
    DBG_ERR("not support\r\n");
    return -1;
#endif
}

static INT32 param_blendset(INT32 argc, char* argv[])
{
#if (IPCAM_FUNC)

	MOVIESTAMP_BLENDING_CFG sblendSetting={0};
	NvtUctrl_GetConfigData((void *)&sblendSetting, sizeof(sblendSetting));
	DBG_DUMP("====blending ====\r\n");
	DBG_DUMP("enable=	%d\r\n",sblendSetting.bEnable);
	DBG_DUMP("alpha =	%d\r\n",sblendSetting.uiAlpha);
	DBG_DUMP("Des ID=	%d\r\n",sblendSetting.uiDesVideoID);
	DBG_DUMP("Src ID=	%d\r\n",sblendSetting.uiSrcVideoID);
	MovieStamp_SetConfig(MOVIESTAMP_BLENDING,(UINT32)&sblendSetting);
    return 0;
#else
    DBG_ERR("not support\r\n");
    return -1;
#endif
}


static INT32 param_bcstart(INT32 argc, char* argv[])
{
#if (MOVIE_BC_FUNC == ENABLE)
    UINT32 value;
    if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }
    DBG_IND("get: %d\r\n",atoi(argv[0]));
    value = atoi(argv[0]);
    if (value == 0)
    {
        Movie_BCLock(TRUE);
    }
    else
    {
        Movie_BCLock(FALSE);
    }
    return 0;
#else
    DBG_ERR("not support\r\n");
    return -1;
#endif
}

static INT32 param_bcset(INT32 argc, char* argv[])
{
#if (MOVIE_BC_FUNC == ENABLE)
    IVSBCSetting   bcsetting;

    NvtUctrl_GetConfigData((void *)&bcsetting, sizeof(bcsetting));
    BC_SetMinObjAreaPct(bcsetting.uiMinObjectAreaPCT);
    BC_SetMaxFgFrms(bcsetting.uiMaxFGFrame);
    BC_SetNeighborhoodUpdate(bcsetting.bNHUpdateEnable);
    BC_SetBgUpdateSpeed(bcsetting.uiBGUpdateSpeed);
    BC_SetBlkNum(bcsetting.uiBLKNumW, bcsetting.uiBLKNumH);
    BC_SetMinObjBlkNum(bcsetting.uiMinObjectBLKNum);
    BC_SetFgPerBlkPct(bcsetting.uiFGPerBlkPCT);
    BC_SetSensi(bcsetting.uiSensitivy);
    DBG_IND("bc minPCT %d, MaxFG %d, NHU %d, BGUS %d, BLKW %d , BLKH %d, MinBLK %d, FGPCT %d, Sens %d\r\n",
            bcsetting.uiMinObjectAreaPCT,
            bcsetting.uiMaxFGFrame,
            bcsetting.bNHUpdateEnable,
            bcsetting.uiBGUpdateSpeed,
            bcsetting.uiBLKNumW,
            bcsetting.uiBLKNumH,
            bcsetting.uiMinObjectBLKNum,
            bcsetting.uiFGPerBlkPCT,
            bcsetting.uiSensitivy
            );
    return 0;
#else
    DBG_ERR("not support\r\n");
    return -1;
#endif
}


static INT32 param_bcget(INT32 argc, char* argv[])
{
#if (MOVIE_BC_FUNC == ENABLE)
    IVSBCSetting   bcsetting;

    bcsetting.uiMinObjectAreaPCT = BC_GetPrms(BC_SET_MIN_OBJ_AREA_PCT);
    bcsetting.uiMaxFGFrame       = BC_GetPrms(BC_SET_MAX_FG_FRMS);
    bcsetting.bNHUpdateEnable    = BC_GetPrms(BC_SET_NEIGHBORHOOD_UPDATE_EN);
    bcsetting.uiBGUpdateSpeed    = BC_GetPrms(BC_SET_BG_UPDATE_SPEED);
    bcsetting.uiBLKNumW          = BC_GetPrms(BC_SET_BLK_NUM_W);
    bcsetting.uiBLKNumH          = BC_GetPrms(BC_SET_BLK_NUM_H);
    bcsetting.uiMinObjectBLKNum  = BC_GetPrms(BC_SET_MIN_OBJ_BLK_NUM);
    bcsetting.uiFGPerBlkPCT      = BC_GetPrms(BC_SET_FG_PER_BLK_PCT);
    bcsetting.uiSensitivy        = BC_GetPrms(BC_SET_SENSI);
    NvtUctrl_SetConfigData((void *)&bcsetting, sizeof(bcsetting));

    DBG_IND("bc minPCT %d, MaxFG %d, NHU %d, BGUS %d, BLKW %d , BLKH %d, MinBLK %d, FGPCT %d, Sens %d\r\n",
            bcsetting.uiMinObjectAreaPCT,
            bcsetting.uiMaxFGFrame,
            bcsetting.bNHUpdateEnable,
            bcsetting.uiBGUpdateSpeed,
            bcsetting.uiBLKNumW,
            bcsetting.uiBLKNumH,
            bcsetting.uiMinObjectBLKNum,
            bcsetting.uiFGPerBlkPCT,
            bcsetting.uiSensitivy
            );
    return 0;
#else
    DBG_ERR("not support\r\n");
    return -1;
#endif
}


static INT32 param_bcreset(INT32 argc, char* argv[])
{
#if (MOVIE_BC_FUNC == ENABLE)
    BC_Reset();
    DBG_IND("\r\n");
    return 0;
#else
    DBG_ERR("not support\r\n");
    return -1;
#endif
}

static INT32 param_bcfgget(INT32 argc, char* argv[])
{
#if (MOVIE_BC_FUNC == ENABLE)
    IMG_BUF      InputImg;
    IVSImageBuf  imgbuf = {0};
    UINT32       i;

    BC_GetForeground(&InputImg);
    imgbuf.uiWidth = InputImg.Width;
    imgbuf.uiHeight = InputImg.Height;
    imgbuf.ePxlFmt = InputImg.PxlFmt;

    for (i=0;i<MAX_PLANE_NUM;i++)
    {
        if (InputImg.PxlAddr[i])
            imgbuf.uiPxlAddr[i] = NvtMem_GetPhyAddr(InputImg.PxlAddr[i]);
    }
    memcpy(imgbuf.uiLineOffset, InputImg.LineOffset, sizeof(UINT32)*MAX_PLANE_NUM);
    NvtUctrl_SetConfigData((void *)&imgbuf, sizeof(imgbuf));
    DBG_IND("w=%d,h=%d Fmt=%d,Y=0x%x,U=0x%x,V=0x%x\r\n",imgbuf.uiWidth,imgbuf.uiHeight,imgbuf.ePxlFmt,imgbuf.uiPxlAddr[0],imgbuf.uiPxlAddr[1],imgbuf.uiPxlAddr[2]);
    return 0;
#else
    DBG_ERR("not support\r\n");
    return -1;
#endif
}

static INT32 param_bcbgget(INT32 argc, char* argv[])
{
#if (MOVIE_BC_FUNC == ENABLE)
    IMG_BUF      InputImg;
    IVSImageBuf  imgbuf={0};
    UINT32       i;

    BC_GetBackground(&InputImg);
    imgbuf.uiWidth = InputImg.Width;
    imgbuf.uiHeight = InputImg.Height;
    imgbuf.ePxlFmt = InputImg.PxlFmt;
    for (i=0;i<MAX_PLANE_NUM;i++)
    {
        if (InputImg.PxlAddr[i])
            imgbuf.uiPxlAddr[i] = NvtMem_GetPhyAddr(InputImg.PxlAddr[i]);
    }
    memcpy(imgbuf.uiLineOffset, InputImg.LineOffset, sizeof(UINT32)*MAX_PLANE_NUM);
    NvtUctrl_SetConfigData((void *)&imgbuf, sizeof(imgbuf));
    DBG_IND("w=%d,h=%d Fmt=%d,Y=0x%x,U=0x%x,V=0x%x\r\n",imgbuf.uiWidth,imgbuf.uiHeight,imgbuf.ePxlFmt,imgbuf.uiPxlAddr[0],imgbuf.uiPxlAddr[1],imgbuf.uiPxlAddr[2]);
    return 0;
#else
    DBG_ERR("not support\r\n");
    return -1;
#endif
}

STATIC_ASSERT(sizeof(URECT) == sizeof(IVSObjData));

static INT32 param_bcrslt(INT32 argc, char* argv[])
{
#if (MOVIE_BC_FUNC == ENABLE)
    URECT             TargetCoord={0};
    static IVSBCData  ivsBCData;

    if(argc!=4)
    {
        DBG_ERR("argc!=4 (%d)\r\n",argc);
        return -1;
    }

    ivsBCData.bUpdated = BC_GetRsltFlag();

    TargetCoord.x = atoi(argv[0]);
    TargetCoord.y = atoi(argv[1]);
    TargetCoord.w = atoi(argv[2]);
    TargetCoord.h = atoi(argv[3]);
    ivsBCData.uiObjectNum = BC_GetTarget((URECT*)&ivsBCData.sObjectData, &TargetCoord);
	BC_ResetRsltFlag();
    NvtUctrl_SetConfigData((void *)&ivsBCData, sizeof(ivsBCData));
    #if 0
    DBG_IND("uiObjectNum = %d\r\n",ivsBCData.uiObjectNum);
    {
        UINT32      i;
        IVSObjData  *pObjData;
        for (i=0;i<ivsBCData.uiObjectNum;i++)
        {
            pObjData = &ivsBCData.sObjectData[i];
            DBG_IND("bc[%d] = x=%d y=%d w=%d h=%d\r\n",i,pObjData->uiCoordinateX,pObjData->uiCoordinateY,pObjData->uiWidth,pObjData->uiHeight);
        }
    }
    #endif
    return 0;
#else
    DBG_ERR("not support\r\n");
    return -1;
#endif
}

static INT32 param_bcblkrslt(INT32 argc, char* argv[])
{
#if (MOVIE_BC_FUNC == ENABLE)
    URECT                TargetCoord={0};
    static IVSBCBLKData  ivsBCBlkData;

    if(argc!=4)
    {
        DBG_ERR("argc!=4 (%d)\r\n",argc);
        return -1;
    }

    ivsBCBlkData.bUpdated = BC_GetBlkRsltFlag();

    TargetCoord.x = atoi(argv[0]);
    TargetCoord.y = atoi(argv[1]);
    TargetCoord.w = atoi(argv[2]);
    TargetCoord.h = atoi(argv[3]);
    ivsBCBlkData.uiObjectNum = BC_GetBlkTarget((URECT*)&ivsBCBlkData.sObjectData, &TargetCoord);
	BC_ResetBlkRsltFlag();
    NvtUctrl_SetConfigData((void *)&ivsBCBlkData, sizeof(ivsBCBlkData));
    #if 0
    DBG_IND("uiObjectNum = %d\r\n",ivsBCBlkData.uiObjectNum);
    {
        UINT32      i;
        IVSObjData  *pObjData;
        for (i=0;i<ivsBCBlkData.uiObjectNum;i++)
        {
            pObjData = &ivsBCBlkData.sObjectData[i];
            DBG_IND("bc[%d] = x=%d y=%d w=%d h=%d\r\n",i,pObjData->uiCoordinateX,pObjData->uiCoordinateY,pObjData->uiWidth,pObjData->uiHeight);
        }
    }
    #endif
    return 0;
#else
    DBG_ERR("not support\r\n");
    return -1;
#endif
}
//#NT#2016/08/26#Lincy Lin -end
static INT32 param_begin(char* str, INT32 len)
{
    //DBG_IND("get: %s\r\n",str);
    memset(pMovieInfo, 0 , sizeof(UCtrlMovieInfoType));
    return 0;
}

#if (AUDIO_PLAY_FUNCTION == ENABLE)
extern AUDIO_CH Movie_AudioChTable[5];
#endif

static INT32 param_end(UTOKEN_END_INFO* p_info)
{
#if (UCTRL_APP_MOVIE_FEATURE_SETGET == ENABLE)
    char retStr[64];
    UINT32 channel_id = 0;
    BOOL   bIsSizeChange = FALSE;
    BOOL   bIsCropChange = FALSE;
    //int    iCurrMode = System_GetState(SYS_STATE_CURRMODE);
    BOOL   bIsRestartMovie = FALSE;
    BOOL   bIsRestartAudio = FALSE;


    if (p_info->err_code < 0)
    {
        snprintf(retStr,sizeof(retStr), "ERR: parm %s", p_info->failed_name);
        NvtUctrl_SetRetString(retStr);
        DBG_ERR("%s\r\n",retStr);
        return p_info->err_code;
    }
    #if 0
    if(iCurrMode != PRIMARY_MODE_MOVIE)
    {
        snprintf(retStr,sizeof(retStr), "ERR: CurrMode is not Movie");
        NvtUctrl_SetRetString(retStr);
        DBG_ERR("%s\r\n",retStr);
        return -1;
    }
    #endif

    // set parameters
    if (pMovieInfo->recsize.isSet)
    {
        UI_SetData(FL_MOVIE_SIZE_MENU, pMovieInfo->recsize.index);
        //UctrlMain_SendEvent(NVTEVT_EXE_MOVIESIZE,            1,  pMovieInfo->recsize.index);
        //UctrlMain_SendEvent(NVTEVT_EXE_IMAGE_RATIO,          1,  GetMovieSizeRatio(UI_GetData(FL_MOVIE_SIZE)));
    }

    // set parameters
    if (pMovieInfo->liveview_channel_id.isSet)
    {
        channel_id = pMovieInfo->liveview_channel_id.index;
    }
    if (pMovieInfo->liveview_vid_enc_fmt.isSet)
    {
        #if MEDIA_VASTREAM
        if (GetMovieVidEncFmt_2p(channel_id) != pMovieInfo->liveview_vid_enc_fmt.index)
        {
            //MovRec_UpdateParam(MOVREC_RECPARAM_VID_ACTCODEC_ID, pMovieInfo->liveview_vid_enc_fmt.index, channel_id, 0);
            bIsSizeChange = TRUE;
            bIsRestartMovie = TRUE;
        }
        #endif
        SetMovieVidEncFmt_2p(channel_id, pMovieInfo->liveview_vid_enc_fmt.index);
    }
    if (pMovieInfo->liveview_vid_size.isSet)
    {
        if (GetMovieSizeIdx_2p(channel_id) != pMovieInfo->liveview_vid_size.index)
        {
            SetMovieSizeIdx_2p(channel_id, pMovieInfo->liveview_vid_size.index);
            bIsSizeChange = TRUE;
            bIsRestartMovie = TRUE;
        }
    }

    if (pMovieInfo->liveview_vid_framerate.isSet)
    {
        #if MEDIA_VASTREAM
        if (GetMovieFrameRate_2p(channel_id,0) != pMovieInfo->liveview_vid_framerate.index)
        {
            //MovRec_UpdateParam(MOVREC_RECPARAM_FRAMERATE, pMovieInfo->liveview_vid_framerate.index, channel_id, 0);
            bIsRestartMovie = TRUE;
        }
        #endif
        SetMovieLiveViewFrameRate(channel_id, 0, pMovieInfo->liveview_vid_framerate.index);
        #if (NEW_PATH == DISABLE)
        // FPS of P2 is not a factor of P1! Set it to be the same!
        if ( (GetMovieFrameRate_2p(0, 0) % GetMovieFrameRate_2p(1, 0)) !=0 )
        {
            SetMovieLiveViewFrameRate(0, 0, pMovieInfo->liveview_vid_framerate.index);
            SetMovieLiveViewFrameRate(1, 0, pMovieInfo->liveview_vid_framerate.index);
        }
        #endif
    }

    if (pMovieInfo->liveview_vid_bitrate.isSet)
    {
        #if MEDIA_VASTREAM
        if (GetMovieTargetBitrate_2p(channel_id,0) != pMovieInfo->liveview_vid_bitrate.index)
        {
            //MovRec_UpdateParam(MOVREC_RECPARAM_TARGETRATE, pMovieInfo->liveview_vid_bitrate.index, channel_id, 0);
            bIsRestartMovie = TRUE;
        }
        #endif
        SetMovieTargetBitrate(channel_id, 0, pMovieInfo->liveview_vid_bitrate.index);
    }

    if (pMovieInfo->liveview_vid_advanced_bitrate.isSet)
    {
        if (pMovieInfo->liveview_vid_advanced_bitrate.eBitrateMode == CODEC_VENC_BIT_RATE_CTRL_CBR)
        {
            SM_H264_CBR_INFO sH264CBRInfo={0};
			UINT32 uiOptValue;

            sH264CBRInfo.uiCbrEn = ENABLE;
            sH264CBRInfo.fStatTime = pMovieInfo->liveview_vid_advanced_bitrate.fStatisticsTime;
            sH264CBRInfo.uiBitRate = pMovieInfo->liveview_vid_advanced_bitrate.uiTargetBitrate * 1024;
            sH264CBRInfo.uiQpDelta = pMovieInfo->liveview_vid_advanced_bitrate.uiQPDelta;
            sH264CBRInfo.uiQpFrmRate = pMovieInfo->liveview_vid_advanced_bitrate.uiQPFrameRate;
            sH264CBRInfo.iMaxQp = (INT32)(pMovieInfo->liveview_vid_advanced_bitrate.uiMaxQP);
			uiOptValue = 1; // Set to quality first mode
			switch (pMovieInfo->liveview_vid_advanced_bitrate.uiIPWeight)
			{
				case 0:
					uiOptValue |= (0x75 << 4);
					break;
				case 1:
					uiOptValue |= (0x53 << 4);
					break;
				case 2:
					uiOptValue |= (0x31 << 4);
					break;
				case 3:
					uiOptValue |= (0x19 << 4);
					break;
				case 4:
					uiOptValue |= (0x9B << 4);
					break;
				case 5:
					uiOptValue |= (0xBD << 4);
					break;
				case 6:
				default:
					uiOptValue |= (0xDF << 4);
					break;
			}

			switch (pMovieInfo->liveview_vid_advanced_bitrate.uiBitrateTolerance)
			{
				case 0:
					uiOptValue |= (0x28 << 24);
					break;
				case 1:
					uiOptValue |= (0x4C << 24);
					break;
				case 2:
					uiOptValue |= (0x70 << 24);
					break;
				case 3:
					uiOptValue |= (0x94 << 24);
					break;
				case 4:
					uiOptValue |= (0xB8 << 24);
					break;
				case 5:
				default:
					uiOptValue |= (0xDC << 24);
					break;
			}

			sH264CBRInfo.uiOpt1 = uiOptValue;

            DBG_DUMP("CBR: %f %u %u %u %d %u %u\r\n", sH264CBRInfo.fStatTime,
                                                      sH264CBRInfo.uiBitRate,
                                                      sH264CBRInfo.uiQpDelta,
                                                      sH264CBRInfo.uiQpFrmRate,
                                                      sH264CBRInfo.iMaxQp,
                                                      sH264CBRInfo.uiIPWeight,
                                                      sH264CBRInfo.uiOpt1);
            SetMovieBRC_2p(channel_id, MOVIE_BRC_TYPE_CBR, (VOID *)&sH264CBRInfo);
        }
        else
        {
            SM_H264_VBR_INFO sH264VBRInfo={0};

            sH264VBRInfo.uiVbrEn = ENABLE;
            sH264VBRInfo.iMaxQp = (INT32)(pMovieInfo->liveview_vid_advanced_bitrate.uiMaxQP);
            sH264VBRInfo.iMinQp = (INT32)(pMovieInfo->liveview_vid_advanced_bitrate.uiMinQP);
            sH264VBRInfo.uiMaxBitrate = pMovieInfo->liveview_vid_advanced_bitrate.uiMaxBitrate * 1024;
            sH264VBRInfo.uiMBRP = 0;
            sH264VBRInfo.uiQpGopDelta = pMovieInfo->liveview_vid_advanced_bitrate.uiQPGOPDelta;
			switch (pMovieInfo->liveview_vid_advanced_bitrate.uiBitrateTolerance)
			{
				case 0:
					sH264VBRInfo.uiOpt1 = (0x28 << 24);
					break;
				case 1:
					sH264VBRInfo.uiOpt1 = (0x4C << 24);
					break;
				case 2:
					sH264VBRInfo.uiOpt1 = (0x70 << 24);
					break;
				case 3:
					sH264VBRInfo.uiOpt1 = (0x94 << 24);
					break;
				case 4:
					sH264VBRInfo.uiOpt1 = (0xB8 << 24);
					break;
				case 5:
				default:
					sH264VBRInfo.uiOpt1 = (0xDC << 24);
					break;
			}
            DBG_DUMP("VBR: %d %d %u %u %u %u\r\n", sH264VBRInfo.iMaxQp,
                                                   sH264VBRInfo.iMinQp,
                                                   sH264VBRInfo.uiMaxBitrate,
                                                   sH264VBRInfo.uiMBRP,
                                                   sH264VBRInfo.uiQpGopDelta,
                                                   sH264VBRInfo.uiOpt1);
            SetMovieBRC_2p(channel_id, MOVIE_BRC_TYPE_VBR, (VOID *)&sH264VBRInfo);
        }
     }

    if (pMovieInfo->liveview_vid_gop.isSet)
    {
        #if MEDIA_VASTREAM
        if (GetMovieGOP_2p(channel_id) != pMovieInfo->liveview_vid_gop.index)
        {
            //MovRec_UpdateParam(MOVREC_RECPARAM_H264GOPNUM, pMovieInfo->liveview_vid_gop.index, channel_id, 0);
            bIsRestartMovie = TRUE;
        }
        #endif
        SetMovieGOP_2p(channel_id, pMovieInfo->liveview_vid_gop.index);
    }

    if (pMovieInfo->liveview_vid_svc_t.isSet)
    {
        #if MEDIA_VASTREAM
        if (GetMovieSVCT_2p(channel_id) != pMovieInfo->liveview_vid_svc_t.index)
        {
            bIsRestartMovie = TRUE;
        }
        #endif
        SetMovieSVCT_2p(channel_id, pMovieInfo->liveview_vid_svc_t.index);
    }

    if (pMovieInfo->liveview_crop.isSet)
    {
        MOVIE_ENCODE_CROP EncodeCrop;
        UINT32 CurrentWidth = GetMovieSizeWidth_2p(channel_id, 0);
        UINT32 CurrentHeight = GetMovieSizeHeight_2p(channel_id, 0);

        bIsCropChange = TRUE;
        EncodeCrop.bCropEnable = pMovieInfo->liveview_crop.bCropEnable;
        //#NT#2016/03/30#David Tsai -begin
        //#NT#Add cropping window position check, avoid cropping area overflow
        if (CROP_SOURCE_W < pMovieInfo->liveview_crop.uiCoordinateX + CurrentWidth)
            EncodeCrop.uiCoordinateX = CROP_SOURCE_W - CurrentWidth - 1;
        else
            EncodeCrop.uiCoordinateX = pMovieInfo->liveview_crop.uiCoordinateX;

        if (CROP_SOURCE_H < pMovieInfo->liveview_crop.uiCoordinateY + CurrentHeight)
            EncodeCrop.uiCoordinateY = CROP_SOURCE_H - CurrentHeight - 1;
        else
            EncodeCrop.uiCoordinateY = pMovieInfo->liveview_crop.uiCoordinateY;
        //#NT#2016/03/30#David Tsai -end
        SetMovieCrop_2p(channel_id, &EncodeCrop);
        bIsRestartMovie = TRUE;
    }

    if (pMovieInfo->liveview_aud_enc_fmt.isSet)
    {
        switch (pMovieInfo->liveview_aud_enc_fmt.index)
        {
            case 0:
                Movie_SetAudCodec(MOVAUDENC_ULAW);
                bIsRestartAudio = TRUE;
                break;
            case 1:
                Movie_SetAudCodec(MOVAUDENC_AAC);
                bIsRestartAudio = TRUE;
                break;
            case 2:
                Movie_SetAudCodec(MOVAUDENC_ALAW);
                bIsRestartAudio = TRUE;
                break;
            case 3:
                Movie_SetAudCodec(MOVAUDENC_PPCM);
                bIsRestartAudio = TRUE;
                break;
        }
    }

    if (pMovieInfo->liveview_aud_samplerate.isSet)
    {
        Movie_SetAudSampleRate(pMovieInfo->liveview_aud_samplerate.index * 1000);
        bIsRestartAudio = TRUE;
    }

    if (pMovieInfo->liveview_aud_channel.isSet)
    {
        switch (pMovieInfo->liveview_aud_channel.index)
        {
            case 0:
                Movie_SetAudChannel(MOVREC_AUDTYPE_LEFT);
                bIsRestartAudio = TRUE;
                break;
            case 1:
                Movie_SetAudChannel(MOVREC_AUDTYPE_RIGHT);
                bIsRestartAudio = TRUE;
                break;
            case 2:
                Movie_SetAudChannel(MOVREC_AUDTYPE_STEREO);
                bIsRestartAudio = TRUE;
                break;
            case 3:
                Movie_SetAudChannel(MOVREC_AUDTYPE_MONO);
                bIsRestartAudio = TRUE;
                break;
        }
    }

    if (pMovieInfo->wdr.isSet)
    {
        UI_SetData(FL_MOVIE_WDR_MENU, pMovieInfo->wdr.index);
        //UctrlMain_SendEvent(NVTEVT_EXE_MOVIE_WDR,            1,  pMovieInfo->wdr.index);
    }
    if (pMovieInfo->hdr.isSet)
    {
        UI_SetData(FL_MOVIE_HDR_MENU, pMovieInfo->hdr.index);
        //UctrlMain_SendEvent(NVTEVT_EXE_SHDR,                 1,  pMovieInfo->hdr.index);
    }
        if (pMovieInfo->rsc.isSet)
    {
        UI_SetData(FL_MovieRSCIndex_MENU, pMovieInfo->rsc.index);
        //UctrlMain_SendEvent(NVTEVT_EXE_MOVIE_RSC,            1,  pMovieInfo->rsc.index);
    }

    if (pMovieInfo->cycrec.isSet)
        UctrlMain_SendEvent(NVTEVT_EXE_CYCLIC_REC,           1,  pMovieInfo->cycrec.index);
    if (pMovieInfo->ev.isSet)
        UctrlMain_SendEvent(NVTEVT_EXE_MOVIE_EV,             1,  pMovieInfo->ev.index);
    if (pMovieInfo->motion.isSet)
        UctrlMain_SendEvent(NVTEVT_EXE_MOTION_DET,           1,  pMovieInfo->motion.index);
    if (pMovieInfo->audio.isSet )
        UctrlMain_SendEvent(NVTEVT_EXE_MOVIE_AUDIO,          1,  pMovieInfo->audio.index);
    if (pMovieInfo->dstamp.isSet)
        UctrlMain_SendEvent(NVTEVT_EXE_MOVIE_DATE_IMPRINT,   1,  pMovieInfo->dstamp.index);
    if (pMovieInfo->gsensor.isSet)
        UctrlMain_SendEvent(NVTEVT_EXE_GSENSOR,              1,  pMovieInfo->gsensor.index);
    if (pMovieInfo->ircut.isSet)
        UctrlMain_SendEvent(NVTEVT_EXE_MOVIE_IR_CUT,         1,  pMovieInfo->ircut.index);
    if (pMovieInfo->rotate.isSet)
        UctrlMain_SendEvent(NVTEVT_EXE_MOVIE_SENSOR_ROTATE,  1,  pMovieInfo->rotate.index);
    if (pMovieInfo->murgpro.isSet)
        UctrlMain_SendEvent(NVTEVT_EXE_MOVIE_PROTECT_MANUAL, 1,  pMovieInfo->murgpro.index);
    if (pMovieInfo->aurgpro.isSet)
        UctrlMain_SendEvent(NVTEVT_EXE_MOVIE_PROTECT_AUTO,   1,  pMovieInfo->aurgpro.index);
    if (pMovieInfo->dualcam.isSet)
        UctrlMain_SendEvent(NVTEVT_EXE_DUALCAM,     1,  pMovieInfo->dualcam.index);
    //#NT#2016/09/20#KCHong -begin
    //#NT#Should set FL_XXX_MENU instead of FL_XXX and the flag will sync when CheckReOpen
    if (pMovieInfo->timelap.isSet)
        SysSetFlag(FL_MOVIE_TIMELAPSE_REC_MENU, pMovieInfo->timelap.index);
    //#NT#2016/09/20#KCHong -end
    if (pMovieInfo->pim.isSet)
        SysSetFlag(FL_MOVIE_PIM, pMovieInfo->pim.index);

    //#NT#2016/09/08#HM Tseng -begin
    //#NT#Support audio channel
    if (pMovieInfo->aout_channel.isSet)
        bIsRestartAudio = TRUE;
    //#NT#2016/09/08#HM Tseng -end

#if (NEW_PATH == ENABLE)

    if (bIsSizeChange || bIsCropChange)
    {
        //#NT#2016/04/14#Charlie Chang -begin
        //#NT#  using pMovieInfo->vratio.isSet to set ratio not here
        //USIZE             ImageRatioSize={0};
        //#NT#2016/04/14#Charlie Chang -end
        UINT32            uiWidth = GetMovieSizeWidth_2p(channel_id,0);
        UINT32            uiHeight = GetMovieSizeHeight_2p(channel_id,0);
        MOVIE_ENCODE_CROP *pEncodeCrop;
        URECT             CropWin;
        //#NT#2016/04/14#Charlie Chang -begin
        //#NT#  using pMovieInfo->vratio.isSet to set ratio not here
        //ImageRatioSize = IMAGERATIO_SIZE[GetMovieSizeRatio_2p(channel_id)];
        //#NT#2016/04/14#Charlie Chang -end
        pEncodeCrop = GetMovieCrop_2p(channel_id);
        if (pEncodeCrop && pEncodeCrop->bCropEnable)
        {
            DBG_IND("bCropEnable = %d\r\n",pEncodeCrop->bCropEnable);
            CropWin.x = pEncodeCrop->uiCoordinateX;
            CropWin.y = pEncodeCrop->uiCoordinateY;
            CropWin.w = uiWidth;
            CropWin.h = uiHeight;
            ImageApp_IPCam_CfgImgSize(channel_id, ALIGN_CEIL_32(CROP_SOURCE_W), CROP_SOURCE_H);
            ImageApp_IPCam_Config(IPCAM_CFG_CROP,(UINT32)&CropWin);
            ImageApp_IPCam_CfgWindow(channel_id);

        }
        else
        {
            ImageApp_IPCam_CfgImgSize(channel_id, ALIGN_CEIL_32(uiWidth), uiHeight);

            CropWin.x = 0;
            CropWin.y = 0;
            CropWin.w = 0;
            CropWin.h = 0;
            ImageApp_IPCam_Config(IPCAM_CFG_CROP,(UINT32)&CropWin);
            ImageApp_IPCam_CfgWindow(channel_id);
            //#NT#2016/04/14#Charlie Chang -begin
            //#NT#  using pMovieInfo->vratio.isSet to set ratio not here
            //ImageApp_IPCam_CfgImgRatio(channel_id, ImageRatioSize.w, ImageRatioSize.h);
            //#NT#2016/04/14#Charlie Chang -end
        }
    }
    //#NT#2016/04/14#Charlie Chang -begin
    //#NT#  using pMovieInfo->vratio.isSet to set ratio
    if(pMovieInfo->vratio.isSet)
    {
        USIZE    ImageRatioSize={0};
        ImageRatioSize = IMAGERATIO_SIZE[pMovieInfo->vratio.index];
        ImageApp_IPCam_CfgImgRatio(channel_id, ImageRatioSize.w, ImageRatioSize.h);
        bIsRestartMovie = 1;
        ImageApp_IPCam_Config(IPCAM_CFG_RATIO,pMovieInfo->vratio.index);
    }
    //#NT#2016/04/14#Charlie Chang -end

	//#NT#2017/01/25#Charlie Chang -begin
    //#NT# support multi-sensor
    if(pMovieInfo->liveview_source_id.isSet){
		#if(IPCAM_FUNC == ENABLE)
			IPCAM_IPL *pIPL_map=NULL;
			pIPL_map = ImageApp_IPCam_Get_IPL_Mapping();
			pIPL_map[channel_id]= pMovieInfo->liveview_source_id.index;
			ImageApp_IPCam_Set_IPL_Mapping( pIPL_map[0], pIPL_map[1], pIPL_map[2]);
			bMovieModeRestart = TRUE;
		#endif
	}

	//#NT#2017/01/25#Charlie Chang -end
    //DBG_IND("bIsSizeChange = %d, bIsCropChange = %d, bIsRestartMovie = %d\r\n",bIsSizeChange,bIsCropChange,bIsRestartMovie);
    if (bIsRestartMovie)
    {

        //UctrlMain_SendEvent(NVTEVT_EXE_VIDEO_PAUSE_RESUME,     1,  channel_id);

        UctrlMain_Lock();
        //Ux_SendEvent(NVTEVT_EXE_VIDEO_PAUSE_RESUME,     1,  channel_id);
        ImageApp_IPCam_SetPath(channel_id, IPCAM_STATE_CHGPARM);
        ImageStream_UpdateAll();
        UctrlMain_UnLock();
        #if 0
        DBG_IND("Restart begin\r\n");
        MovRec_PathN_Start_Stop(0, 0);
        MovRec_PathN_Start_Stop(0, 1);
        DBG_IND("Restart end\r\n");
        #endif
    }
    #if (!MEDIA_4_0)
    if (bIsRestartAudio)
    {
        UINT32 g_uiAudSampleRate = 32000;
        UINT32 g_uiAudCodec = MOVAUDENC_AAC;
        UINT32 g_uiAudChannel = MOVREC_AUDTYPE_RIGHT;

        #if 0
        ImageApp_IPCam_SetPath(0, IPCAM_STATE_CHGPARM);
        ImageApp_IPCam_SetPath(1, IPCAM_STATE_CHGPARM);
        ImageApp_IPCam_SetPath(2, IPCAM_STATE_CHGPARM);
        ImageStream_UpdateAll();
        #endif
        g_uiAudCodec = Movie_GetAudCodec();
        g_uiAudSampleRate = Movie_GetAudSampleRate();
        g_uiAudChannel = Movie_GetAudChannel();

//#NT#2016/08/29#HM Tseng -begin
//#NT#For audio play testing mode
#if (AUDIO_PLAY_FUNCTION == ENABLE && AUDIO_PLAY_TEST == DISABLE)
        System_AudioOutStop();//stop output
#endif
//#NT#2016/08/29#HM Tseng -end
        VSMediaRecAudClose();
        Delay_DelayMs(40);
        MovRec_UpdateParam(MOVREC_RECPARAM_AUD_CODEC, g_uiAudCodec, 0, 0);
        MovRec_UpdateParam(MOVREC_RECPARAM_AUD_SAMPLERATE, g_uiAudSampleRate, 0, 0);
//#NT#2016/08/29#HM Tseng -begin
//#NT#2016/09/30#David Tsai -begin
//#NT# Switch audio source
		switch (g_uiAudChannel)
		{
			case MOVREC_AUDTYPE_MONO:
				MovRec_ChangeParameter(MOVREC_RECPARAM_AUD_SRC, AUDIO_CH_MONO, 0, 0);
				break;
			case MOVREC_AUDTYPE_RIGHT:
				MovRec_ChangeParameter(MOVREC_RECPARAM_AUD_SRC, AUDIO_CH_RIGHT, 0, 0);
				break;
			case MOVREC_AUDTYPE_STEREO:
				MovRec_ChangeParameter(MOVREC_RECPARAM_AUD_SRC, AUDIO_CH_STEREO, 0, 0);
				break;
			case MOVREC_AUDTYPE_LEFT:
			default:
				MovRec_ChangeParameter(MOVREC_RECPARAM_AUD_SRC, AUDIO_CH_LEFT, 0, 0);
				break;
		}
//#NT#2016/09/30#David Tsai -end
//#NT#For audio play testing mode
#if (AUDIO_PLAY_FUNCTION == ENABLE && AUDIO_PLAY_TEST == DISABLE)
        //#NT#2016/12/22#HM Tseng -begin
        //#NT# Support AEC function
        #if (AUDIO_AEC_FUNC == ENABLE)
        if (g_uiAudChannel == MOVREC_AUDTYPE_STEREO) {
            NvtAec_SetConfig(NVTAEC_CONFIG_ID_RECORD_CH_NO, 2);
        } else {
            NvtAec_SetConfig(NVTAEC_CONFIG_ID_RECORD_CH_NO, 1);
        }
        #endif
        //#NT#2016/12/22#HM Tseng -end
        //#NT#2016/09/08#HM Tseng -begin
        //#NT#Support audio channel
        UINT32 aout_channel = 0;
        if (pMovieInfo->aout_channel.isSet)
            aout_channel = pMovieInfo->aout_channel.index;
        else
            aout_channel = System_GetAudioChannel();
        System_SetAudioParam(0, g_uiAudSampleRate, aout_channel, 16);
        //#NT#2016/09/08#HM Tseng -end
#endif
#if (AUDIO_PLAY_FUNCTION == ENABLE && AUDIO_PLAY_TEST == DISABLE)
        System_AudioOutStart();//start output
#endif
//#NT#2016/08/29#HM Tseng -end

        //#NT#2016/04/07#Charlie Chang -begin
        //#NT# pause resume ImageStream when audio config changed

        UctrlMain_Lock();
        int i=0;
        for (i=0; i < IPCAM_STATE_MAX_ID ; i++){
            ImageApp_IPCam_SetPath(i, IPCAM_STATE_CHGPARM);
        }
        ImageStream_UpdateAll();
        UctrlMain_UnLock();
        //#NT#2016/04/07#Charlie Chang -end

        VSWA_Record_OpenAll();
        CHKPNT;
    }
    #endif

    /*
    if (pMovieInfo->start.isSet)
    {
        ImageApp_IPCam_SetPath(channel_id, IPCAM_STATE_ENABLE);
    }
    else if (pMovieInfo->stop.isSet)
    {
        ImageApp_IPCam_SetPath(channel_id, IPCAM_STATE_DISABLE);
    }
    else
    */
    {

        //ImageApp_IPCam_SetPath(channel_id, IPCAM_STATE_CHGPARM);
    }
    //ImageStream_UpdateAll();

#else
    if (FlowMovie_CheckReOpenItem())
    {
        // change mode for re-configuration
        //UctrlMain_SendEvent(NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_MOVIE);
        bIsRestartMovie = TRUE;
    }
    else if(pMovieInfo->liveview_vid_framerate.isSet || pMovieInfo->liveview_vid_bitrate.isSet || \
            pMovieInfo->liveview_aud_enc_fmt.isSet || pMovieInfo->liveview_aud_samplerate.isSet)
    {
        // change mode for re-configuration
        //UctrlMain_SendEvent(NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_MOVIE);
        bIsRestartMovie = TRUE;
    }
    if (bIsRestartMovie)
        UctrlMain_SendEvent(NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_MOVIE);
#endif
    // record
    if (pMovieInfo->start.isSet)
    {
        if (!bUctrlRecStart)
        {
            bUctrlRecStart = TRUE;
            // start record
            Ux_PostEvent(NVTEVT_EXE_MOVIE_REC_START, 0);

            /*
            #if (_KEY_METHOD_ == _KEY_METHOD_4KEY_)
            Ux_PostEvent(NVTEVT_KEY_SHUTTER2 , 1, NVTEVT_KEY_PRESS);
            #else
            Ux_PostEvent(NVTEVT_KEY_SELECT, 1, NVTEVT_KEY_PRESS);
            #endif
            */
        }
    }


    // stop
    if (pMovieInfo->stop.isSet)
    {
        if (bUctrlRecStart)
        {
            bUctrlRecStart = FALSE;
            Ux_PostEvent(NVTEVT_EXE_MOVIE_REC_STOP, 0);

            /*
            #if (_KEY_METHOD_ == _KEY_METHOD_4KEY_)
            Ux_PostEvent(NVTEVT_KEY_SHUTTER2 , 1, NVTEVT_KEY_PRESS);
            #else
            Ux_PostEvent(NVTEVT_KEY_SELECT, 1, NVTEVT_KEY_PRESS);
            #endif
            */
        }
    }

    // snapshot
    if (pMovieInfo->snapshot.isSet)
    {
        #if (MEDIA_VASTREAM==ENABLE)
        //UINT32 i;

        snapShotPathID = pMovieInfo->snapshot.index;
        SnapShot_Start(snapShotPathID);
        //#NT#2016/08/31#Lincy Lin -begin
        //#NT#Fix snapshot sometimes fail because of image width*height to large
        #if 0
        for (i=0;i<100;i++)
        {
            if (IsSnapShotFinish)
                break;
            Delay_DelayMs(30);
        }
        SnapShot_Stop(snapShotPathID);
        #endif
        //#NT#2016/08/31#Lincy Lin -end
        snprintf(retStr, sizeof(retStr), "%lu %lu", NvtMem_GetPhyAddr(snapShotData.Addr), snapShotData.Size);
        DBG_IND("reStr= %s\r\n",retStr);
        NvtUctrl_SetRetString(retStr);
        #else
        UINT32 uiAddr=0x0, uiSize=0x0;//, i;

        if (bUctrlRecStart)
        {
            ImageApp_IPCam_Snapshot(channel_id);

            MovRec_WaitJpgEnc(channel_id);
            MovRec_RawEncGetJpgData(&uiAddr, &uiSize, 0);
        }

        snprintf(retStr, sizeof(retStr), "%lu %lu", NvtMem_GetPhyAddr(uiAddr), uiSize);
        NvtUctrl_SetRetString(retStr);
        DBG_IND("Snapshot addr=0x%x, size=%d\r\n",uiAddr,uiSize);
        #endif
        #if 0
        {
            CHAR         tempfilename[30];
            FST_FILE     filehdl = NULL;

            snprintf(tempfilename,sizeof(tempfilename), "A:\\snap.JPG");
            filehdl = FileSys_OpenFile(tempfilename,FST_CREATE_ALWAYS|FST_OPEN_WRITE);
            if (filehdl)
            {
                FileSys_WriteFile(filehdl, (UINT8 *)uiAddr, &uiSize, 0, NULL);
                FileSys_CloseFile(filehdl);
            }
        }
        #endif

    }
	#if(IPCAM_FUNC == ENABLE)
	if(bMovieModeRestart == TRUE){
		bMovieModeRestart = FALSE;
		UctrlMain_SendEvent(NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_MOVIE);
	}
	#endif
#endif
    return 0;
}


static INT32 convert_motion_level(MD_LEVEL level){


    if(level==MD_LEVEL_N5){
        return 0;
    }
    else if(level == MD_LEVEL_N4){
        return 1;
    }
    else if(level == MD_LEVEL_N3){
        return 2;
    }
    else if(level == MD_LEVEL_N2){
        return 3;
    }
    else if(level == MD_LEVEL_N1){
        return 4;
    }
    else if(level == MD_LEVEL_NORMAL){
        return 5;
    }
    else if(level == MD_LEVEL_P1){
        return 6;
    }
    else if(level == MD_LEVEL_P2){
        return 7;
    }
    else if(level == MD_LEVEL_P3){
        return 8;
    }
    else if(level == MD_LEVEL_P4){
        return 9;
    }
    else if(level == MD_LEVEL_P5){
        return 10;
    }
    else{
        DBG_ERR("convert error level=%d\r\n",level);
        return 5;
    }

    return 5;

}
//#NT#2016/11/01#Charlie Chang -begin
//#NT# support motion detect start function
static INT32 param_motion_start(INT32 argc, char* argv[]){
#if IPCAM_FUNC
    if(argc >= 2){
        //UINT32 MD_ID = atoi(argv[0]);
        UINT32 enable = atoi(argv[1]);


		if(enable == 0){
            Movie_MDLock(FALSE);
			DBG_DUMP("stop notify motion event!\r\n");
		}
		else{
            Movie_MDLock(TRUE);
			DBG_DUMP("start notify motion event!\r\n");
		}

	}
#endif
    return 0;
}
//#NT#2016/11/01#Charlie Chang -end
static INT32 param_motion_get(INT32 argc, char* argv[])
{
    MD_INFO Info;
    MD_GetInfo(&Info);
    INT32 level = convert_motion_level(Info.Level);
    INT8 Wnd[NVT_MD_WND_Y][NVT_MD_WND_X];
    char response_buf[1024]={0};
    UINT32 buf_len=0;
    INT32 i=0, j =0;
    INT32 Wnd_value[NVT_MD_WND_Y];



    memcpy(Wnd, Info.WeightWin, sizeof(UINT8)*NVT_MD_WND_X*NVT_MD_WND_Y);
    DBG_DUMP("motion windows!!!\r\n");
    for(i = 0 ; i< NVT_MD_WND_Y; i++){
        INT32 tmp_value=0;
        for(j=0; j < NVT_MD_WND_X; j++){

            if(Wnd[i][j]==1){
                tmp_value = tmp_value + (1<<j);
            }
            DBG_DUMP("%d ",Wnd[i][j]);
        }
        DBG_DUMP("\r\n");
        Wnd_value[i]=tmp_value;

    }
    if(level == 10){
        buf_len = snprintf(response_buf, sizeof(response_buf)-1,"level:High wnd:");
    }
    else if(level == 0){
        buf_len = snprintf(response_buf, sizeof(response_buf)-1,"level:Low wnd:");
    }
    else{
        buf_len = snprintf(response_buf, sizeof(response_buf)-1,"level:%d wnd:",level);
    }
    for(i = 0; i < NVT_MD_WND_Y; i++){
        if(i==(NVT_MD_WND_Y-1)){
            buf_len += snprintf(response_buf+buf_len, sizeof(response_buf)-1,"%d", Wnd_value[i]);
        }
        else{
            buf_len += snprintf(response_buf+buf_len, sizeof(response_buf)-1,"%d,", Wnd_value[i]);
        }
    }

    NvtUctrl_SetRetString(response_buf);
    return 0;
}


static INT32 param_motion_set(INT32 argc, char* argv[])
{
    INT32 i=0, j=0;
    UINT8 md_wnd[NVT_MD_WND_Y][NVT_MD_WND_X]=
    {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    };
	#if(IPCAM_FUNC ==ENABLE)
    	BOOL init_flag = FALSE;
		UINT32 count =0;
	    do{
	    	init_flag= ImageApp_IPCam_GetConfig(IPCAM_CFG_INIT_FLAG);
			if(init_flag == FALSE){
            	Delay_DelayMs(100);
				DBG_DUMP("motion detect process not ready, please wait\r\n");
				count++;
			}
			if(count >10){
				DBG_ERR("motion detect process init fail\r\n");
				return -1;
			}
	    }while(init_flag==FALSE);

    #endif
     //////////////////////////////////////////////////////////////////
     ////first parameter : ID  second : windows config     third: level
     //////////////////////////////////////////////////////////////////
    if(argc!=3)
    {
        DBG_ERR("argc!=3 (%d)\r\n",argc);
        return -1;
    }
    INT32 md_id = atoi(argv[0]);
    if(md_id < 0){
        DBG_ERR("md_id error md_id=%d\r\n",md_id);
    }
    MD_LEVEL md_level;
    if(!strcmp(argv[2],"High")){
        md_level = MD_LEVEL_P5;
    }
    else if(!strcmp(argv[2],"9")){
        md_level = MD_LEVEL_P4;
    }
    else if(!strcmp(argv[2],"8")){
        md_level = MD_LEVEL_P3;
    }
    else if(!strcmp(argv[2],"7")){
        md_level = MD_LEVEL_P2;
    }
    else if(!strcmp(argv[2],"6")){
        md_level = MD_LEVEL_P1;
    }
    else if(!strcmp(argv[2],"5")){
        md_level = MD_LEVEL_NORMAL;
    }
    else if(!strcmp(argv[2],"4")){
        md_level = MD_LEVEL_N1;
    }
    else if(!strcmp(argv[2],"3")){
        md_level = MD_LEVEL_N2;
    }
    else if(!strcmp(argv[2],"2")){
        md_level = MD_LEVEL_N3;
    }
    else if(!strcmp(argv[2],"1")){
        md_level = MD_LEVEL_N4;
    }
    else if(!strcmp(argv[2],"Low")){
        md_level = MD_LEVEL_N5;
    }
    else{
        DBG_ERR("level error  argv[2]=%s\n",argv[2]);
        md_level = MD_LEVEL_NORMAL;
    }

    MD_SetLevel(md_id, md_level);

    char *token = NULL;
    INT32 wnd_index=0;
    INT32 wnd_index2=0;
    for(i=0; i< NVT_MD_WND_Y ; i++){
        if(i==0){
            token = strtok(argv[1], ",");
        }
        else{
            token = strtok(NULL, ",");
        }

        wnd_index = atoi(token);
        for(j=0 ; j<NVT_MD_WND_X ; j++){
            wnd_index2 = (1<<j);

            if( (wnd_index2& wnd_index)==wnd_index2){
                md_wnd[i][j]=1;
            }
        }
    }
    MD_SetDetWin(md_id, &md_wnd[0][0]);
    NvtUctrl_SetRetString("OK");
    return 0;
}

/////////////////////
static INT32 param_unknown(char* name)
{
    DBG_ERR("unknown param: %s\r\n",name);
    return 0;
}

UTOKEN_PARAM tbl_movie_param[] = {
    {"recsize" , param_recsize}, // [resolution index]
    #if UCTRL_APP_MOVIE_FEATURE_SETGET == ENABLE
    {"lvchannelset"   , param_channelid_set},    // [channel set]
    {"lvencfmtset"    , param_liveviewencfmt_set}, // [video encode format index set]
    {"lvencfmtget"    , param_liveviewencfmt_get}, // [video encode format  index get]
    {"lvsizeset"      , param_liveviewsize_set}, // [resolution index set]
    {"lvsizeget"      , param_liveviewsize_get}, // [resolution index get]
    {"lvframerateset" , param_liveviewframerate_set}, // [frame rate number set]
    {"lvframerateget" , param_liveviewframerate_get}, // [frame rate number get]
    {"lvbitrateset"   , param_liveviewbitrate_set}, // [bit rate number set]
    {"lvbitrateget"   , param_liveviewbitrate_get}, // [bit rate number get]
    {"lvgopset"       , param_liveviewgop_set},     // [gop number set]
    {"lvcropset"      , param_liveviewcrop_set},     // [gop number set]
    {"lvaudenctypeset"    , param_lvaudenctype_set},  // [Live view audio codec set]
    {"lvaudenctypeget"    , param_lvaudenctype_get},  // [Live view audio codec get]
    {"lvaudsamprateset"   , param_lvaudsamprate_set}, // [Live view audio sampling rate set]
    {"lvaudsamprateget"   , param_lvaudsamprate_get}, // [Live view audio sampling rate get]
    {"lvratioset"    , param_liveviewratio_set},  // [video ratio set]
    {"vencGet"  , param_venc_get},  // get video encoder setting
    {"vencSet"  , param_venc_set},  // set video encoder setting
    {"vencROIGet"  , param_vencROI_get},  // get video encoder ROI setting
    {"vencROISet"  , param_vencROI_set},  // set video encoder ROI setting
    {"vencStat" , param_vstat_get}, // get video encoder status
    {"vencIreq" , param_venc_ireq}, // I-frame request
    {"aencGet"  , param_aenc_get},  // get audio encoder setting
    {"aencSet"  , param_aenc_set},  // set audio encoder setting
    {"voutGet"  , param_vout_get},  // get video output setting
    {"voutSet"  , param_vout_set},  // set video output setting
    {"aoutGet"  , param_aout_get},  // get audio output setting
    {"aoutSet"  , param_aout_set},  // set audio output setting
    {"aoutSwitch" , param_aout_switch}, //  on/off audio output
    {"aoutStatus" , param_aout_status}, //  get audio output status
    {"HDRGet"  , param_hdr_get},  // get HDR setting
    {"HDRSet"  , param_hdr_set},  // set HDR setting
//#NT#2016/05/11#David Tsai -begin
//#NT#Add dummy function to make all project compatible to core 2 linux
#else
    {"lvchannelset"   , param_dummy_func},    // [channel set]
    {"lvencfmtset"    , param_dummy_func}, // [video encode format index set]
    {"lvencfmtget"    , param_dummy_func}, // [video encode format  index get]
    {"lvsizeset"      , param_dummy_func}, // [resolution index set]
    {"lvsizeget"      , param_dummy_func}, // [resolution index get]
    {"lvframerateset" , param_dummy_func}, // [frame rate number set]
    {"lvframerateget" , param_dummy_func}, // [frame rate number get]
    {"lvbitrateset"   , param_dummy_func}, // [bit rate number set]
    {"lvbitrateget"   , param_dummy_func}, // [bit rate number get]
    {"lvgopset"       , param_dummy_func},     // [gop number set]
    {"lvcropset"      , param_dummy_func},     // [gop number set]
    {"lvaudenctypeset"    , param_dummy_func},  // [Live view audio codec set]
    {"lvaudenctypeget"    , param_dummy_func},  // [Live view audio codec get]
    {"lvaudsamprateset"   , param_dummy_func}, // [Live view audio sampling rate set]
    {"lvaudsamprateget"   , param_dummy_func}, // [Live view audio sampling rate get]
    {"lvratioset"    , param_dummy_func},  // [video ratio set]
    {"vencGet"  , param_dummy_func},  // get video encoder setting
    {"vencSet"  , param_dummy_func},  // set video encoder setting
    {"vencStat" , param_dummy_func}, // get video encoder status
    {"vencIreq" , param_dummy_func}, // I-frame request
    {"aencGet"  , param_dummy_func},  // get audio encoder setting
    {"aencSet"  , param_dummy_func},  // set audio encoder setting
    {"voutGet"  , param_dummy_func},  // get video output setting
    {"voutSet"  , param_dummy_func},  // set video output setting
    {"aoutGet"  , param_dummy_func},  // get audio output setting
    {"aoutSet"  , param_dummy_func},  // set audio output setting
    {"aoutSwitch" , param_dummy_func}, //  on/off audio output
    {"aoutStatus" , param_dummy_func}, //  get audio output status
    {"HDRGet"  , param_dummy_func},  // get HDR setting
    {"HDRSet"  , param_dummy_func},  // set HDR setting
//#NT#2016/05/11#David Tsai -end
#endif
    {"cycrec"  , param_cycrec},  // [time index]
    {"wdr"     , param_wdr},     // [on/off index]
    {"hdr"     , param_hdr},     // [on/off index]
    {"ev"      , param_ev},      // [value index]
    {"motion"  , param_motion},  // [on/off index]
    {"audio"   , param_audio},   // [on/off index]
    {"dstamp"  , param_dstamp},  // [on/off index]
    {"rsc"     , param_rsc},     // [on/off index]
    {"gsensor" , param_gsensor}, // [level index]
    {"timelap" , param_timelap}, // [interval index]
    {"ircut"   , param_ircut},   // [on/off index]
    {"rotate"  , param_rotate},  // [on/off index]
    {"murgpro" , param_murgpro}, // [on/off index]
    {"aurgpro" , param_aurgpro}, // [on/off index]
    {"pim"     , param_pim},     // [on/off index]
    {"dualcam" , param_dualcam}, // [display index]
    {"start"   , param_start},
    {"stop"    , param_stop},
    {"snapshot", param_snapshot},
    {"fdstart" , param_fdstart}, // fd start/stop
    {"fdset"   , param_fdset},   // fd setting get
    {"fdget"   , param_fdget},   // fd setting get
    {"fdrslt"  , param_fdrslt},  // get fd detect result
    {"ftgstart" , param_ftgstart}, // fgt start/stop
    {"ftgset"   , param_ftgset},   // ftg setting get
    {"ftgget"   , param_ftgget},   // ftg setting get
    {"ftgrslt"  , param_ftgrslt},  // get ftg detect result
    {"ftgsetroi"  , param_ftgsetroi},  // set ftg roi
	{"ftggetroi", param_ftggetroi}, // get ftg roi
    {"ftgsnapshotnotify", param_ftgsnapshotnotify}, // encode to snapshot
    {"ftgsnapshotget", param_ftgsnapshotget}, // get ftg snapshot
    {"ftgsnapshotremove", param_ftgsnapshotremove}, // remove ftg snapshot
    {"motionset", param_motion_set}, // set motion config
    {"motionget", param_motion_get}, //get motion config
    //#NT#2016/11/01#Charlie Chang -begin
    //#NT# control motion start/stop
    {"motionstart", param_motion_start}, //get motion config
    //#NT#2016/11/01#Charlie Chang -end
    //#NT#2016/05/23#David Tsai -begin
    //#NT# Add tampering detection control for uctrl
    {"tdstart" , param_tdstart}, // td start/stop
    {"tdset"   , param_tdset},   // td setting get
    {"tdget"   , param_tdget},   // td setting get
    {"tdrslt"  , param_tdrslt},  // get td detect result
    //#NT#2016/05/23#David Tsai -end
    //#NT#2016/08/26#Lincy Lin -begin
    //#NT# Add uctrl command for BC/MV/IPL buffer APIs .
    {"dspbufget"  , param_dspbuf_get}, // get dsp temp buffer
    {"imgget"     , param_img_get}, // get current ipl image
    {"mvstart" , param_mvstart}, // mv start
    {"mvset"   , param_mvset},   // mv setting set
    {"mvget"   , param_mvget},   // mv setting get
    {"mvrslt"  , param_mvrslt},  // get mv result
    {"bcstart" , param_bcstart}, // bc start
    {"bcset"   , param_bcset},   // bc setting set
    {"bcget"   , param_bcget},   // bc setting get
    {"bcreset" , param_bcreset},  // reset bc
    {"bcfgget" , param_bcfgget},  // get bc foreground
    {"bcbgget" , param_bcbgget},  // get bc background
    {"bcrslt"  , param_bcrslt},   // get bc result
    {"bcblkrslt"  , param_bcblkrslt},   // get bc block result
    //#NT#2016/08/26#Lincy Lin -end
	{"blendget", param_blendget},
	{"blendset", param_blendset},
    {NULL,NULL}, //last tag, it must be
};

UTOKEN_MODULE module_movie = {
    "umovie",
    tbl_movie_param,
    param_begin,
    param_end,
    param_unknown,
    NULL
};

#if SXCMD_TEST
static BOOL Cmd_umovie_set(CHAR* strCmd)
{
    uToken_Parse(strCmd,&module_movie);
    return TRUE;
}

SXCMD_BEGIN(umovie, "umovie")
SXCMD_ITEM("set %", Cmd_umovie_set, "")
SXCMD_END()
#endif


void UCtrlAppMovie_Open(void)
{
    memset(pMovieInfo, 0 , sizeof(UCtrlMovieInfoType));
#if SXCMD_TEST
    SxCmd_AddTable(umovie);
#endif

}

void UCtrlAppMovie_Close(void)
{
}



