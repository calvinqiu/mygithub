
#include <stdio.h>
#include <string.h>
#include "AppControl.h"
//Common
#include "SysKer.h"
#include "SysCfg.h"
#include "PrjCfg.h"
#include "AppLib.h"
#include "usb.h"
//#include "PrjCfg_Demo.h"
#include "UIAppCommon.h"
#include "dma.h"

//Lib
#include "UVAC.h"
#include "GxUSB.h"
#include "UIAppUsbCam.h"
#include "Utility.h"
#include "Sensor.h"
#include "ImageApp_UsbMovie.h"
#include "ImageUnit_UsbUVAC.h"
#include "MovieInterface.h"
#include "UIPhotoInfo.h"
#include "UIInfo.h"
#include "ImageUnit_CamDisp.h"
#include "UIMovieMapping.h"
#include "SMediaRecAPI.h"
#include "FileDB.h"
#include "NameRule_FileDB.h"


#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UIApp_UVAC
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////
#define UVAC_VENDCMD_CURCMD                         0x01
#define UVAC_VENDCMD_VERSION                        0x02

#define UVAC_PRJ_SET_LIVEVIEW_SAVEFILE          0  //1V + 1F
#define UVAC_PRJ_SET_LIVEVIEW_1V_ONLY           1  //1V only
#define UVAC_PRJ_SET_LIVEVIEW_2V                2  //2V
//#NT#2017/02/24#Ben Wang -begin
//#NT#UVC support higher frame size in MJPEG format.
#define UVAC_MAX_PAYLOAD_FRAME_SIZE (800*1024)    //800k
//#NT#2017/02/24#Ben Wang -end
#if (DISABLE == UVAC_MODE_2_PATH)
#define UVAC_PRJ_SET_THIS           UVAC_PRJ_SET_LIVEVIEW_1V_ONLY
#else
#define UVAC_PRJ_SET_THIS           UVAC_PRJ_SET_LIVEVIEW_2V
//#define UVAC_PRJ_SET_THIS           UVAC_PRJ_SET_LIVEVIEW_SAVEFILE
//#define UVAC_PRJ_SET_THIS           UVAC_PRJ_SET_LIVEVIEW_1V_ONLY
#endif


#if (UVAC_PRJ_SET_THIS == UVAC_PRJ_SET_LIVEVIEW_SAVEFILE)
//Single Video liveview and movie-recording a file
#define UVAC_2PATH                  DISABLE
#define UVAC_SAVE_FILE              ENABLE
#define UVAC_TEST_FOR_CPU_EXCEPTION_MFK         DISABLE
#elif (UVAC_PRJ_SET_THIS == UVAC_PRJ_SET_LIVEVIEW_1V_ONLY)
//Single Video liveview only
#define UVAC_2PATH                  DISABLE
#define UVAC_SAVE_FILE              DISABLE
#define UVAC_TEST_FOR_CPU_EXCEPTION_MFK         ENABLE  //ENABLE DISABLE
#else
//Two Video liveview
#define UVAC_2PATH                  ENABLE
#define UVAC_SAVE_FILE              DISABLE
#define UVAC_TEST_FOR_CPU_EXCEPTION_MFK         ENABLE  //ENABLE DISABLE
#endif

#define UVAC_WINUSB_INTF            DISABLE   //ENABLE    DISABLE


UVAC_VEND_DEV_DESC gUIUvacDevDesc = {0};
UINT8 gUIUvacVendCmdVer[8]= {'1','.','0','8','.','0','0','7'};

typedef struct _VEND_CMD_
{
    UINT32 cmd;
    UINT8 data[36]; //<= 60
}VEND_CMD, *PVEND_CMD;

typedef struct
{
    UINT32  uiWidth;
    UINT32  uiHeight;
    UINT32  uiVidFrameRate;
    UINT32  uiH264TBR;
    UINT32  uiMJPEGTBR;
} UVC_TARGET_SIZE_PARAM;

static UVC_TARGET_SIZE_PARAM g_UvcVidSizeTable[] =
{                           //H264      //MJPEG
    {1920,  1080,   30,     1800*1024,  3*1800*1024},
    {1280,  720,    30,     800*1024,   4*800*1024},
    { 640,  480,    30,     400*1024,   4*400*1024},
    { 320,  240,    30,     200*1024,   4*200*1024},
};


/**
* 1.NVT_UI_UVAC_RESO_CNT < UVAC_VID_RESO_MAX_CNT
* 2.Discrete fps:
*       The values of fps[] must be in a decreasing order and 0 shall be put to the last one.
*       The element, fpsCnt, must be correct for the values in fps[]
* 3.Must be a subset of g_MovieRecSizeTable[] for MFK-available.
*
*/
static UVAC_VID_RESO gUIUvacVidReso[NVT_UI_UVAC_RESO_CNT] =
{
    {1920,  1080,   1,      30,      0,      0},        //16:9
    {1280,   720,   1,      30,      0,      0},        //16:9
    //{ 848,   480,   1,      30,      0,      0},      //Skype didn't support this resolution.
    { 640,   480,   1,      30,      0,      0},        //4:3
    { 320,   240,   1,      30,      0,      0}         //4:3
};

//NVT_UI_UVAC_AUD_SAMPLERATE_CNT <= UVAC_AUD_SAMPLE_RATE_MAX_CNT
UINT32 gUIUvacAudSampleRate[NVT_UI_UVAC_AUD_SAMPLERATE_CNT] =
{
	NVT_UI_FREQUENCY_32K // it's better to be the same as sound effect!
};

_ALIGNED(4) static UINT16 m_UVACSerialStrDesc3[] =
{
    0x0320,                             // 20: size of String Descriptor = 32 bytes
                                        // 03: String Descriptor type
    '9', '6', '6', '6', '0',            // 96611-00000-001 (default)
    '0', '0', '0', '0', '0',
    '0', '0', '1', '0', '0'
};
_ALIGNED(4) const static UINT8 m_UVACManuStrDesc[] =
{
    USB_VENDER_DESC_STRING_LEN*2+2, // size of String Descriptor = 6 bytes
    0x03,                       // 03: String Descriptor type
    USB_VENDER_DESC_STRING
};

_ALIGNED(4) const static UINT8 m_UVACProdStrDesc[] =
{
    USB_PRODUCT_DESC_STRING_LEN*2+2, // size of String Descriptor = 6 bytes
    0x03,                       // 03: String Descriptor type
    USB_PRODUCT_DESC_STRING
};

//====== Movie Mode Setting ======
static BOOL bSDIsSlow = FALSE;

VIEW_SRC gMovie_src_uvac =
{
    TRUE, //BYPASS DISP-SRV, DIRECT OUTPUT TO DISPLAY
    VIEW_HANDLE_PRIMARY,
    NULL,
};

#if (UVAC_TEST_FOR_CPU_EXCEPTION_MFK && USE_FILEDB)
static CHAR g_CarDVdef[20]="NOVATEK";//"CarDV";
static FILEDB_HANDLE gMovieFDBHdl = FILEDB_CREATE_ERROR;
static FILEDB_INIT_OBJ gMovieFDBInitObj={
                         "A:\\NOVATEK\\MOVIE\\",  //rootPath
                         NULL,  //defaultfolder
                         NULL,  //filterfolder
                         TRUE,  //bIsRecursive
                         TRUE,  //bIsCyclic
                         TRUE,  //bIsMoveToLastFile
                         TRUE, //bIsSupportLongName, 2013/07/01 Meg
                         FALSE, //bIsDCFFileOnly
                         TRUE,  //bIsFilterOutSameDCFNumFolder
                         {'N','V','T','I','M'}, //OurDCFDirName[5]
                         {'I','M','A','G'}, //OurDCFFileName[4]
                         FALSE, //bIsFilterOutSameDCFNumFile
                         FALSE, //bIsChkHasFile
                         CARDV_PATHLEN,    //u32MaxFilePathLen
                         10000, //u32MaxFileNum
                         FILEDB_FMT_ANY,       //fileFilter
                         0,     //u32MemAddr
                         0,     //u32MemSize
                         NULL   //fpChkAbort
};

static void xUvac_InitFileDB(void)
{
    #define ROOT_PATH_LENGTH    20
    PFILEDB_INIT_OBJ   pFDBInitObj = &gMovieFDBInitObj;
    CHAR              rootPath[ROOT_PATH_LENGTH]="A:\\";

    //strcat(rootPath, g_CarDVdef);
    strncat(rootPath, g_CarDVdef, ROOT_PATH_LENGTH-1);
    NH_FileDB_SetRootFolder(g_CarDVdef);
    //strcat(rootPath, "\\");
    strncat(rootPath, "\\",ROOT_PATH_LENGTH-1);
    //strcat(rootPath, "MOVIE");
    strncat(rootPath, "MOVIE",ROOT_PATH_LENGTH-1);
    //strcat(rootPath, "\\");
    strncat(rootPath, "\\",ROOT_PATH_LENGTH-1);
    rootPath[ROOT_PATH_LENGTH-1]=0;
    pFDBInitObj->rootPath = rootPath;
    gMovieFDBHdl = FileDB_Create(pFDBInitObj);
    FileDB_SortBy(gMovieFDBHdl,FILEDB_SORT_BY_FILEPATH,FALSE);
}
#endif

static void GetUvcTBRAspRatio(UINT32 codec, UINT32 width, UINT32 height, UINT32 fps, UINT32 *pTBR, UINT32 *pAspRatio)
{
    UINT32 tbr = 0x400000;
    UINT32 ratio = VIDENC_DAR_DEFAULT;
    UINT32 i = 0;
    UINT32 ItemNum = sizeof(g_UvcVidSizeTable)/sizeof(UVC_TARGET_SIZE_PARAM);
    DBG_IND("codec=%d,w=%d,%d,fps=%d,pTBR=0x%x,pAspRatio=0x%x\r\n",codec, width, height, fps, pTBR, pAspRatio);
    for (i = 0; i < ItemNum; i++)
    {
        if ((g_UvcVidSizeTable[i].uiWidth == width) && (g_UvcVidSizeTable[i].uiHeight == height)&& (g_UvcVidSizeTable[i].uiVidFrameRate == fps))
        {
            if(codec == MEDIAVIDENC_H264)
            {
                tbr = g_UvcVidSizeTable[i].uiH264TBR;
            }
            else//MJPEG
            {
                tbr = g_UvcVidSizeTable[i].uiMJPEGTBR;
            }
            break;
        }
    }
    if (ItemNum <= i)
    {
        DBG_ERR("No Match Reso(%d, %d, %d) in g_UvcVidSizeTable\r\n", width, height, fps);
    }
    DBG_IND(":w=%d,%d,fps=%d,tbr=%d,ratio=%d\r\n", width, height, fps, tbr, ratio);
    if (pTBR)
    {
        *pTBR = tbr;
    }
    else
    {
        DBG_ERR("NULL Input pTBR\r\n");
    }
    if (pAspRatio)
    {
        *pAspRatio = ratio;
    }
    else
    {
        DBG_ERR("NULL Input pAspRatio\r\n");
    }

}
void xUvac_SetSDSlow(BOOL IsSlow)
{
    bSDIsSlow = IsSlow;
}
void xUvac_MFKRecordCB(UINT32 uiEventID, UINT32 param)
{
    DBG_IND("evt=%d,p=%d\r\n", uiEventID, param);
}
void xUvac_Movie_IPLCB(IPL_CBMSG uiMsgID, void *Data)
{
    DBG_IND(":%d,0x%x;\r\n",(UINT32)uiMsgID, (UINT32)Data);
}
void xUvac_Movie_RecordCB(UINT32 uiEventID, UINT32 param)
{
    switch (uiEventID)
    {
        case MOVREC_EVENT_RESULT_OVERTIME:
            DBG_DUMP("EVENT %d: Overtime!\r\n", uiEventID);
            break;
        case MOVREC_EVENT_RESULT_FULL:
            DBG_ERR("EVENT %d: Full!\r\n", uiEventID);
            break;
        case MOVREC_EVENT_RESULT_HW_ERR:
        case MOVREC_EVENT_RESULT_NOTREADY:
        case MOVREC_EVENT_RESULT_WRITE_ERR:
            DBG_ERR("EVENT %d: HW error!\r\n", uiEventID);
            break;
        case MOVREC_EVENT_THRESHOLD:
            //DBG_IND("EVENT: uiSeconds %d\r\n", (UINT32)param);
            break;
        case MOVREC_EVENT_ENCONE_OK:
            //DBG_IND("ENCODE Ok\r\n");
            break;
        case MOVREC_EVENT_RESULT_NORMAL:
            DBG_IND("NORMAL\r\n");
            break;
        case MOVREC_EVENT_RESULT_SLOW:
        case MOVREC_EVENT_RESULT_CUT_FAIL:
            DBG_ERR("EVENT %d: Slow card!\r\n", uiEventID);
            break;
        case MOVREC_EVENT_AUDBSCB:
            DBG_IND("AUD SCB\r\n");
            break;
        case MOVREC_EVENT_IMEDIRECT_STOPH264:
            SMediaRec_Vtrig_NoKickH264(1);
            DBG_DUMP("^R======Time stop h264!!!!\r\n");
            break;
        default:
            //DBG_DUMP(":%d, 0x%x;\r\n",(UINT32)uiEventID, (UINT32)param);
            break;
    }
}
void xUvac_Movie_CaptureCB(IMG_CAP_CBMSG Msg, void *Data)
{
    DBG_IND(":%d, 0x%x; Do nothing now.\r\n", (UINT32)Msg, (UINT32)Data);
}

void xUvac_MFKRegCB(void)
{
    ImageUnit_SetParam(&ISF_ImagePipe1, IMAGEPIPE_PARAM_IPL_CB, (UINT32)xUvac_Movie_IPLCB);
    ImageUnit_SetParam(&ISF_Media, MEDIA_PARAM_REC_CB, (UINT32)xUvac_Movie_RecordCB);
    ImageUnit_SetParam(&ISF_Cap, CAP_PARAM_CAP_CB, (UINT32)xUvac_Movie_CaptureCB);
    ImageUnit_SetParam(&ISF_Cap, CAP_PARAM_DETS2_CB, (UINT32)NULL);
}
__inline void xUvac_SetMFKRecParam(PNVT_USBMOVIE_VID_RESO pVidReso1, PNVT_USBMOVIE_VID_RESO pVidReso2)
{
    UINT32  uiRecFormat;
    UINT32  uiFileType, uiWidth, uiHeight;
    UINT32  uiVidCodec, uiVidFrameRate, uiTargetBitrate=0, uiH264GopType, ui3DNRLevel, uiVidRotate, uiDispAspectRatio=0;
    UINT32  uiAudCodec, uiAudSampleRate, uiAudChannelNum, uiAudChannelType;
    BOOL    bAudFilterEn;
#if MOVIE_MULTI_RECORD_FUNC
    UINT32  uiFileType2, uiWidth2, uiHeight2, uiTargetBitrate2=0, uiDispAspectRatio2=0, uiVidCodec2, uiVidFrameRate2;
#endif

    uiRecFormat         = MEDIAREC_AUD_VID_BOTH;                // recording type
    uiFileType          = MEDIA_FILEFORMAT_MOV;                         //MEDIAREC_MP4

    DBG_IND("@@Codec1=%d, Codec2=%d@@\r\n", pVidReso1->codec, pVidReso2->codec);
#if 0
    switch (uiFileType)
    {
    case MEDIA_FILEFORMAT_AVI: // AVI format, audio should be PCM, video can be H.264 or MJPEG
        uiAudCodec  = MOVAUDENC_PCM;
        uiVidCodec  = MEDIAVIDENC_H264;    //MEDIAREC_ENC_JPG
        break;

    case MEDIA_FILEFORMAT_MP4: // MP4 format, audio should be AAC, video should be H.264
        uiAudCodec  = MOVAUDENC_AAC;
        uiVidCodec  = MEDIAVIDENC_H264;
        break;

    case MEDIA_FILEFORMAT_MOV: // MOV format, audio can be PCM or AAC, video can be H.264 or MJPEG
    default:
        uiAudCodec  = MOVAUDENC_PPCM; //MEDIAREC_ENC_PCM
        uiVidCodec  = MEDIAVIDENC_H264;
        break;
    }
#else
    uiAudCodec  = MOVAUDENC_PPCM; //MEDIAREC_ENC_PCM
    uiVidCodec  = MEDIAVIDENC_H264;
#endif
    if (pVidReso1->codec == UVAC_VIDEO_FORMAT_H264)
    {
        uiVidCodec  = MEDIAVIDENC_H264;
    }
    else
    {
        uiVidCodec  = MEDIAVIDENC_MJPG;
    }

    DBG_IND("uiFileType=%d, uiAudCodec=%d\r\n", uiFileType, uiAudCodec);
    uiWidth             = pVidReso1->width;         // image width
    uiHeight            = pVidReso1->height;        // image height
    uiVidFrameRate      = pVidReso1->fps;         // video frame rate
    GetUvcTBRAspRatio(uiVidCodec, uiWidth, uiHeight, uiVidFrameRate, &uiTargetBitrate, &uiDispAspectRatio);
    uiH264GopType       = MEDIAREC_H264GOP_IPONLY;              // H.264 GOP type (IP only)
    //uiH264GopType       = MEDIAREC_H264GOP_IPB;               // H.264 GOP type (IPB)
    DBG_DUMP("CB-V1 W=%d, %d, fps=%d, TBR=%d, Ratio=%d, codec=%d, RecFmt=%d, FileType=%d, Gop=%d\r\n", \
        uiWidth,uiHeight,uiVidFrameRate,uiTargetBitrate,uiDispAspectRatio,uiVidCodec,uiRecFormat,uiFileType,uiH264GopType);

    ui3DNRLevel         = H264_3DNR_DISABLE;                    // H.264 3DNR disable
    uiAudSampleRate     = gUIUvacAudSampleRate[0];              // audio sampling rate
    uiAudChannelType    = MOVREC_AUDTYPE_STEREO;               // audio channel type
    uiAudChannelNum     = 2;                                    // audio channel number

    bAudFilterEn        = FALSE;                                // enable/disable audio filter
    //#Support Rotation information in Mov/Mp4 File format -begin
    uiVidRotate         = MOVREC_MOV_ROTATE_0;


    //---------------------------------------------------------------------------------------------
    // set media recording control parameters
    //---------------------------------------------------------------------------------------------
    // disable golf shot recording
    MovRec_ChangeParameter(MOVREC_RECPARAM_GOLFSHOT_ON, FALSE, 0, 0);
    // disable flash recording
    MovRec_ChangeParameter(MOVREC_RECPARAM_FLASH_ON, FALSE, 0, 0);
    // enable/disable power off protection
    if (bSDIsSlow == TRUE)
        MovRec_ChangeParameter(MOVREC_RECPARAM_PWROFFPT, FALSE, 0, 0);
    else
        MovRec_ChangeParameter(MOVREC_RECPARAM_PWROFFPT, TRUE, 0, 0);
    // disable time lapse recording => not necessary now, since it can be set by using MOVREC_RECPARAM_RECFORMAT
    //MovRec_ChangeParameter(MOVREC_RECPARAM_TIMELAPSE, FALSE, 0, 0);
    // set recording format (MOVREC_VID_ONLY, MEDIAREC_AUD_VID_BOTH, MEDIAREC_TIMELAPSE, ...)
    MovRec_ChangeParameter(MOVREC_RECPARAM_RECFORMAT, uiRecFormat, 0, 0);

    // set recording end type (MEDIAREC_ENDTYPE_NORMAL, MEDIAREC_ENDTYPE_CUTOVERLAP, MEDIAREC_ENDTYPE_CUT_TILLCARDFULL)
    #if (MOVIE_AUTOREC_CYCLICTIME == ENABLE)
    if (UI_GetData(FL_MOVIE_CYCLIC_REC) != MOVIE_CYCLICREC_OFF)
    {
        MovRec_ChangeParameter(MOVREC_RECPARAM_ENDTYPE, MOVREC_ENDTYPE_CUTOVERLAP, 0, 0);
    }
    else
    #endif
    {
        MovRec_ChangeParameter(MOVREC_RECPARAM_ENDTYPE, MOVREC_ENDTYPE_NORMAL, 0, 0);
    }
#if 0
    // enable/disable time lapse recording
    if (uiRecFormat == MEDIAREC_TIMELAPSE)
    {
        MovRec_ChangeParameter(MOVREC_RECPARAM_TIMELAPSE, TRUE, 0, 0);
        MovRec_ChangeParameter(MOVREC_RECPARAM_TIMELAPSE_TIME, 500, 0, 0); // test: 2 fps
        MovRec_ChangeParameter(MOVREC_RECPARAM_ENDTYPE, MOVREC_ENDTYPE_NORMAL, 0, 0); // should be normal end for time lapse recording
    }
    else
#endif
    {
        MovRec_ChangeParameter(MOVREC_RECPARAM_TIMELAPSE, FALSE, 0, 0);
    }

    //---------------------------------------------------------------------------------------------
    // set file type parameters
    //---------------------------------------------------------------------------------------------
    // set rotation information
    //#Support Rotation information in Mov/Mp4 File format -begin
    MovRec_ChangeParameter(MOVREC_RECPARAM_MOV_ROTATE, uiVidRotate, 0, 0);

    //---------------------------------------------------------------------------------------------
    // set video code parameters
    //---------------------------------------------------------------------------------------------
    // (uiVidCodec == MEDIAREC_ENC_H264)
    {
        // set H.264 GOP type (MEDIAREC_H264GOP_IPONLY, MEDIAREC_H264GOP_IPB)
        MovRec_ChangeParameter(MOVREC_RECPARAM_H264GOPTYPE, uiH264GopType, 0, 0);
        // set H.264 3DNR level (H264_3DNR_DISABLE, H264_3DNR_WEAKEST, H264_3DNR_WEAK, ...)
        MovRec_ChangeParameter(MOVREC_RECPARAM_H2643DNRLEVEL, ui3DNRLevel, 0, 0);
    }
    // (uiVidCodec == MEDIAREC_ENC_JPG)
    {
        MovRec_ChangeParameter(MOVREC_RECPARAM_JPG_YUVFORMAT, MOVREC_JPEG_FORMAT_420, 0, 0);
    }

    //---------------------------------------------------------------------------------------------
    // set video parameters (common for 1 and 2)
    //---------------------------------------------------------------------------------------------
    // set video frame rate
    MovRec_ChangeParameter(MOVREC_RECPARAM_FRAMERATE, uiVidFrameRate, 0, 0);

    //---------------------------------------------------------------------------------------------
    // set video parameters
    //---------------------------------------------------------------------------------------------
    //select parameter for input path 0 (default)
    {
        // set file type (MEDIAREC_AVI, MEDIAREC_MOV, MEDIAREC_MP4)
        MovRec_ChangeParameter(MOVREC_RECPARAM_FILETYPE, uiFileType, 0, 0);
        // set video codec (MEDIAREC_ENC_H264, MEDIAREC_ENC_JPG)
        MovRec_ChangeParameter(MOVREC_RECPARAM_VIDEOCODEC, uiVidCodec, 0, 0);
        // set image width
        MovRec_ChangeParameter(MOVREC_RECPARAM_WIDTH, uiWidth, 0, 0);
        // set image height
        MovRec_ChangeParameter(MOVREC_RECPARAM_HEIGHT, uiHeight, 0, 0);
        // set target data rate (bytes per second)
        MovRec_ChangeParameter(MOVREC_RECPARAM_TARGETRATE, uiTargetBitrate, 0, 0);
        // set target aspect ratio
        MovRec_ChangeParameter(MOVREC_RECPARAM_DAR, uiDispAspectRatio, 0, 0);
    }

#if MOVIE_MULTI_RECORD_FUNC
    uiFileType2 = MEDIA_FILEFORMAT_MOV; //MEDIAREC_MP4

    if (pVidReso2->codec == UVAC_VIDEO_FORMAT_H264)
    {
        uiVidCodec2  = MEDIAVIDENC_H264;
    }
    else
    {
        uiVidCodec2  = MEDIAVIDENC_MJPG;
    }
    uiWidth2             = pVidReso2->width;         // image width
    uiHeight2            = pVidReso2->height;        // image height
    uiVidFrameRate2      = pVidReso2->fps;         // video frame rate
    GetUvcTBRAspRatio(uiVidCodec2, uiWidth2, uiHeight2, uiVidFrameRate2, &uiTargetBitrate2, &uiDispAspectRatio2);
    if (uiVidFrameRate2 != uiVidFrameRate)
    {
        DBG_ERR("V1-fps=%d NOT Equal V2-fps=%d !!!\r\n", uiVidFrameRate, uiVidFrameRate2);
        DBG_ERR("V1-fps=%d NOT Equal V2-fps=%d !!!\r\n", uiVidFrameRate, uiVidFrameRate2);
    }
    DBG_DUMP("CB-V2  W=%d, %d, fps=%d, TBR=%d, Ratio=%d, codec=%d, FileType=%d\r\n", \
        uiWidth2,uiHeight2,uiVidFrameRate2,uiTargetBitrate2,uiDispAspectRatio2,uiVidCodec2,uiFileType2);

    // set file type (MEDIAREC_AVI, MEDIAREC_MOV, MEDIAREC_MP4)
    MovRec_ChangeParameter(MOVREC_RECPARAM_FILETYPE_2, uiFileType2, 0, 0);
    // set video codec (MEDIAREC_ENC_H264, MEDIAREC_ENC_JPG)
    MovRec_ChangeParameter(MOVREC_RECPARAM_VIDEOCODEC, uiVidCodec2, 1, 0);
    // set image width
    MovRec_ChangeParameter(MOVREC_RECPARAM_WIDTH, uiWidth2, 1, 0);
    // set image height
    MovRec_ChangeParameter(MOVREC_RECPARAM_HEIGHT, uiHeight2, 1, 0);
    // set target data rate (bytes per second)
    MovRec_ChangeParameter(MOVREC_RECPARAM_TARGETRATE, uiTargetBitrate2, 1, 0);
    // set target aspect ratio
    MovRec_ChangeParameter(MOVREC_RECPARAM_DAR, uiDispAspectRatio2, 1, 0);
#endif

    //---------------------------------------------------------------------------------------------
    // set audio parameters
    //---------------------------------------------------------------------------------------------
    // set audio sampling rate (AUDIO_SR_8000, AUDIO_SR_11025, AUDIO_SR_12000, ...)
    MovRec_ChangeParameter(MOVREC_RECPARAM_AUD_SAMPLERATE, uiAudSampleRate, 0, 0);
    // set audio source (AUDIO_CH_RIGHT, AUDIO_CH_LEFT, AUDIO_CH_STEREO)
    MovRec_ChangeParameter(MOVREC_RECPARAM_AUD_SRC, uiAudChannelType, 0, 0);
    // set audio channel number (1 or 2; if audio channel is mono but channel number is 2, the audio data will be duplicated)
    MovRec_ChangeParameter(MOVREC_RECPARAM_AUD_CHS, uiAudChannelNum, 0, 0);
    // set audio codec (MEDIAREC_ENC_PCM, MEDIAREC_ENC_AAC)
    MovRec_ChangeParameter(MOVREC_RECPARAM_AUD_CODEC, uiAudCodec, 0, 0);
    // set audio filter on/off
    MovRec_ChangeParameter(MOVREC_RECPARAM_EN_AUDFILTER, bAudFilterEn, 0, 0);

	//force to turn off EMR
    MovRec_SetEmergencyRecording(0, 0);
	MovRec_SetEmergencyRecording(1, 0);


}
void xUvac_ConfigMovieSizeCB(PNVT_USBMOVIE_VID_RESO pVidReso1, PNVT_USBMOVIE_VID_RESO pVidReso2)
{
    UINT32 ratioW = NVT_USBMOVIE_CROPRATIO_W_16;
    UINT32 ratioH = NVT_USBMOVIE_CROPRATIO_H_9;
    UINT32 nSensor = ImageUnit_GetParam(&ISF_ImagePipe, IMAGEPIPE_PARAM_SENSORCOUNT); //return 0/1/2/4
    DBG_IND(":pVidReso1=0x%x,pVidReso2=0x%x,senCnt=%d\r\n", pVidReso1, pVidReso2,nSensor);
    if (nSensor == 2)
    {
        ImageUnit_SetParam(&ISF_ImagePipe1,IPL_SEL_VIDEO_SIZE, SEL_VIDEOSIZE_1920x1080);
        ImageUnit_SetParam(&ISF_ImagePipe1,IPL_SEL_VIDEO_FPS, SEL_VIDEOFPS_60);
        ImageUnit_SetParam(&ISF_ImagePipe2,IPL_SEL_VIDEO_SIZE, SEL_VIDEOSIZE_1920x1080);
        ImageUnit_SetParam(&ISF_ImagePipe2,IPL_SEL_VIDEO_FPS, SEL_VIDEOFPS_60);
        ratioW = NVT_USBMOVIE_CROPRATIO_W_16;
        ratioH = NVT_USBMOVIE_CROPRATIO_H_9;
        DBG_IND("Cofig CfgAspect for ISF_ImagePipe1\r\n");
        ImageUnit_Begin(&ISF_ImagePipe1, 0);
        ImageUnit_CfgAspect(ISF_IN1, ratioW , ratioH);
        ImageUnit_End();
        DBG_IND("Cofig CfgAspect for ISF_ImagePipe2\r\n");
        ImageUnit_Begin(&ISF_ImagePipe2, 0);
        ImageUnit_CfgAspect(ISF_IN1, NVT_USBMOVIE_CROPRATIO_W_16, NVT_USBMOVIE_CROPRATIO_H_9);
        ImageUnit_End();
    }
    else if (nSensor < 2)
    {
        ISF_UNIT*  pUnit;
        //#NT#2016/12/12#Ben Wang -begin
        //#NT#Fix multi sensor connect mapping when only sensor2 enable.
        //UINT32 uSensorMask = ImageUnit_GetParam(&ISF_ImagePipe, IMAGEPIPE_PARAM_SENSORMASK);
        pUnit = &ISF_ImagePipe1;
		//#NT#2016/12/12#Ben Wang -end
        if ((1280 == pVidReso1->width) && (720 == pVidReso1->height))
        {
            ImageUnit_SetParam(pUnit,IPL_SEL_VIDEO_SIZE, SEL_VIDEOSIZE_1280x720);
            ImageUnit_SetParam(pUnit,IPL_SEL_VIDEO_FPS, SEL_VIDEOFPS_30);
            ratioW = NVT_USBMOVIE_CROPRATIO_W_16;
            ratioH = NVT_USBMOVIE_CROPRATIO_H_9;
        }
        else if ((1920 == pVidReso1->width) && (1080 == pVidReso1->height) && (60 == pVidReso1->fps))
        {
            ImageUnit_SetParam(pUnit,IPL_SEL_VIDEO_SIZE, SEL_VIDEOSIZE_1920x1080);
            ImageUnit_SetParam(pUnit,IPL_SEL_VIDEO_FPS, SEL_VIDEOFPS_60);
            ratioW = NVT_USBMOVIE_CROPRATIO_W_16;
            ratioH = NVT_USBMOVIE_CROPRATIO_H_9;
        }
        else
        {
            ImageUnit_SetParam(pUnit,IPL_SEL_VIDEO_SIZE, SEL_VIDEOSIZE_1920x1080);
            ImageUnit_SetParam(pUnit,IPL_SEL_VIDEO_FPS, SEL_VIDEOFPS_30);
            if (((640 == pVidReso1->width) && (480 == pVidReso1->height)) || \
                ((320 == pVidReso1->width) && (240 == pVidReso1->height)) )
            {
                ratioW = NVT_USBMOVIE_CROPRATIO_W_4;
                ratioH = NVT_USBMOVIE_CROPRATIO_H_3;
            }
            else if (((848 == pVidReso1->width) && (480 == pVidReso1->height)) || ((1920 == pVidReso1->width) && (1080 == pVidReso1->height)) )
            {
                ratioW = NVT_USBMOVIE_CROPRATIO_W_16;
                ratioH = NVT_USBMOVIE_CROPRATIO_H_9;
            }
            else
            {
                DBG_WRN("Need to add new: %d, %d, %d, %d\r\n", pVidReso1->width, pVidReso1->height, pVidReso1->fps, pVidReso1->codec);
            }
        }
        ImageUnit_Begin(pUnit, 0);
        ImageUnit_CfgAspect(ISF_IN1, ratioW , ratioH);
        ImageUnit_End();
    }
    else
    {
        DBG_ERR("Not support number of sensor=%d\r\n", nSensor);
    }
    xUvac_SetMFKRecParam(pVidReso1, pVidReso2);
}
void xUvac_InitSetMFK(void)
{
    MEM_RANGE Pool;
    UINT32 useFileDB = 0;
    NVT_USBMOVIE_VID_RESO vidReso1, vidReso2;

    ImageUnit_SetParam(&ISF_Cap, CAP_PARAM_BUFSIZE, 0); // use full buffer
    xUvac_MFKRegCB();

    gMovie_src_uvac.bDirectMode = TRUE;///???
    ImageUnit_SetParam(&ISF_CamDisp, CAMDISP_PARAM_VIEWSRC, (UINT32)&gMovie_src_uvac);

    vidReso1.width = vidReso2.width = gUIUvacVidReso[NVT_UI_UVAC_RESO_IDX_DEF].width;
    vidReso1.height = vidReso2.height = gUIUvacVidReso[NVT_UI_UVAC_RESO_IDX_DEF].height;
    vidReso1.fps = vidReso2.fps = gUIUvacVidReso[NVT_UI_UVAC_RESO_IDX_DEF].fps[0];
    vidReso1.codec = vidReso2.codec =  UVAC_VIDEO_FORMAT_H264;
    #if  (_SENSORLIB2_ != _SENSORLIB2_OFF_)
    DBG_ERR("Dual Sensor\r\n");
    if (2 == ImageUnit_GetParam(&ISF_ImagePipe, IMAGEPIPE_PARAM_SENSORCOUNT))
    {
        vidReso2.width = gUIUvacVidReso[NVT_UI_UVAC_RESO_IDX_DUALSNESOR_DEF].width;
        vidReso2.height = gUIUvacVidReso[NVT_UI_UVAC_RESO_IDX_DUALSNESOR_DEF].height;
        vidReso2.fps = gUIUvacVidReso[NVT_UI_UVAC_RESO_IDX_DUALSNESOR_DEF].fps[0];
        vidReso2.codec =  UVAC_VIDEO_FORMAT_H264;
    }
    #endif
    xUvac_SetMFKRecParam(&vidReso1, &vidReso2);

    #if USE_FILEDB
    UI_SetData(FL_IsUseFileDB, 1);
    #else
    UI_SetData(FL_IsUseFileDB, 0);
    #endif
    useFileDB = UI_GetData(FL_IsUseFileDB);
    if (useFileDB)
    {
        DBG_DUMP("^MSet useFileDB for MFK\r\n");
        Pool.Addr = dma_getCacheAddr(OS_GetMempoolAddr(POOL_ID_FILEDB));
        Pool.Size = POOL_SIZE_FILEDB;
        ImageUnit_SetParam(&ISF_CamFile, CAMFILE_PARAM_USE_FILEDB, (UINT32)&Pool);
        // config FDB root
        ImageApp_CamFile_SetFolder(FILEDB_CARDV_ROOT);

        #if (UVAC_TEST_FOR_CPU_EXCEPTION_MFK && USE_FILEDB)
        gMovieFDBInitObj.u32MemAddr = Pool.Addr;
        gMovieFDBInitObj.u32MemSize = Pool.Size;
        #endif
    }
    else
    {
        DBG_DUMP("^MSet useFileDB NULL\r\n");
        ImageUnit_SetParam(&ISF_CamFile, CAMFILE_PARAM_USE_FILEDB, 0);
    }
}
//======= end movie mode related setting  ======


//0:OK
//1...:TBD
static UINT32 xUvacVendReqCB(PUVAC_VENDOR_PARAM pParam)
{
    if (pParam)
    {
        DBG_IND("bHostToDevice       = 0x%x\r\n", pParam->bHostToDevice);
        DBG_IND("uiReguest       = 0x%x\r\n",     pParam->uiReguest    );
        DBG_IND("uiValue       = 0x%x\r\n",       pParam->uiValue      );
        DBG_IND("uiIndex       = 0x%x\r\n",       pParam->uiIndex      );
        DBG_IND("uiDataAddr       = 0x%x\r\n",    pParam->uiDataAddr   );
        DBG_IND("uiDataSize       = 0x%x\r\n",    pParam->uiDataSize   );
    }
    else
    {
        DBG_ERR(" Input parameter NULL\r\n");
    }
    return 0;
}
//0:OK
//1...:TBD
static UINT32 xUvacVendReqIQCB(PUVAC_VENDOR_PARAM pParam)
{
    if (pParam)
    {
        DBG_IND("bHostToDevice       = 0x%x\r\n", pParam->bHostToDevice);
        DBG_IND("uiReguest       = 0x%x\r\n",     pParam->uiReguest    );
        DBG_IND("uiValue       = 0x%x\r\n",       pParam->uiValue      );
        DBG_IND("uiIndex       = 0x%x\r\n",       pParam->uiIndex      );
        DBG_IND("uiDataAddr       = 0x%x\r\n",    pParam->uiDataAddr   );
        DBG_IND("uiDataSize       = 0x%x\r\n",    pParam->uiDataSize   );
    }
    else
    {
        DBG_ERR(" Input parameter NULL\r\n");
    }
    return 0;
}
static void xUSBMakerInit_UVAC(UVAC_VEND_DEV_DESC *pUVACDevDesc)
{
    pUVACDevDesc->pManuStringDesc = (UVAC_STRING_DESC *)m_UVACManuStrDesc;
    pUVACDevDesc->pProdStringDesc = (UVAC_STRING_DESC *)m_UVACProdStrDesc;

    pUVACDevDesc->pSerialStringDesc = (UVAC_STRING_DESC *)m_UVACSerialStrDesc3;

    pUVACDevDesc->VID = USB_VID;
    pUVACDevDesc->PID = USB_PID_PCCAM;

    pUVACDevDesc->fpVendReqCB = xUvacVendReqCB;
    pUVACDevDesc->fpIQVendReqCB = xUvacVendReqIQCB;

}

UINT8 xUvacWinUSBCB(UINT32 ControlCode, UINT8 CS, UINT8 *pDataAddr, UINT32 *pDataLen)
{
    DBG_IND("xUvacWinUSBCB ctrl=0x%x, cs=0x%x, pData=0x%x, len=%d\r\n", ControlCode, CS, pDataAddr, *pDataLen);
    return TRUE;
}
UINT8 xUvacEUVendCmdCB_Idx1(UINT32 ControlCode, UINT8 CS, UINT8 *pDataAddr, UINT32 *pDataLen)
{
    static UINT32 vendCmd = NVT_UI_UVAC_VENDCMD_CURCMD;
    UINT8 vendCBRet = TRUE;
    UINT32 cmdLen = sizeof(VEND_CMD);
    PVEND_CMD pVendCmd = (PVEND_CMD)pDataAddr;
    UINT8 *pData = pDataAddr;

    switch(CS)
    {
        case GET_INFO:
            DBG_IND("[VendCmd --GET_INFO] ctrl=0x%x, cs=0x%x, pData=0x%x, len=%d, vendCmd=%d\r\n", ControlCode, CS, pData, *pDataLen, vendCmd);
            *pData = 0x03;
            *pDataLen = 1;
            vendCBRet = TRUE;
            break;
        case GET_LEN:
            DBG_IND("[VendCmd --GET_LEN] ctrl=0x%x, cs=0x%x, pData=0x%x, len=%d, vendCmd=%d\r\n", ControlCode, CS, pData, *pDataLen, vendCmd);
            *pData++ = (cmdLen & 0xFF);
            *pData++ = ((cmdLen >> 8) & 0xFF);
            *pDataLen = 2;
            vendCBRet = TRUE;
            break;
        case GET_RES:
            DBG_IND("[VendCmd --GET_RES] ctrl=0x%x, cs=0x%x, pData=0x%x, len=%d, vendCmd=%d\r\n", ControlCode, CS, pData, *pDataLen, vendCmd);
            *pData = 0x01;
            *pDataLen = 1;
            vendCBRet = TRUE;
            break;
        case GET_MAX:
            DBG_IND("[VendCmd --GET_MAX] ctrl=0x%x, cs=0x%x, pData=0x%x, len=%d, vendCmd=%d\r\n", ControlCode, CS, pData, *pDataLen, vendCmd);
            memset(pData, 0xff, cmdLen);
            *pDataLen = cmdLen;
            vendCBRet = TRUE;
            break;
        case GET_MIN:
            DBG_IND("[VendCmd --GET_MIN] ctrl=0x%x, cs=0x%x, pData=0x%x, len=%d, vendCmd=%d\r\n", ControlCode, CS, pData, *pDataLen, vendCmd);
            break;
        case GET_DEF:
            DBG_IND("[VendCmd --GET_DEF] ctrl=0x%x, cs=0x%x, pData=0x%x, len=%d, vendCmd=%d\r\n", ControlCode, CS, pData, *pDataLen, vendCmd);
            memset(pData, 0, cmdLen);
            *pDataLen = cmdLen;
            vendCBRet = TRUE;
            break;
        case GET_CUR:
            DBG_IND("[VendCmd --GET_CUR] ctrl=0x%x, cs=0x%x, pData=0x%x, len=%d, vendCmd=%d\r\n", ControlCode, CS, pData, *pDataLen, vendCmd);
            if (NVT_UI_UVAC_VENDCMD_CURCMD == vendCmd)
            {
                vendCmd = pVendCmd->data[0];
                *pDataLen = cmdLen;
                vendCBRet = TRUE;
            }
            else if (UVAC_VENDCMD_VERSION == vendCmd)
            {
                memcpy((void *)pData, gUIUvacVendCmdVer, 8);
                *pDataLen = cmdLen;
                vendCBRet = TRUE;
            }
            else
            {
                //TBD: temporarely no needed
                vendCBRet = FALSE;
            }
            break;
        case SET_CUR:
            DBG_IND("[VendCmd --SET_CUR] ctrl=0x%x, cs=0x%x, pData=0x%x, len=%d, vendCmd=%d\r\n", ControlCode, CS, pData, *pDataLen, vendCmd);
            if (NVT_UI_UVAC_VENDCMD_CURCMD == vendCmd)
            {
                vendCmd = pVendCmd->data[0];
                vendCBRet = TRUE;
            }
            else if (UVAC_VENDCMD_VERSION == vendCmd)
            {
                vendCBRet = FALSE;//Version can not be set.
            }
            else
            {
                //TBD: temporarely no needed
                vendCBRet = FALSE;
            }
            break;
        default:
            DBG_IND("[VendCmd --Unknow] ctrl=0x%x, cs=0x%x, pData=0x%x, len=%d, vendCmd=%d\r\n", ControlCode, CS, pData, *pDataLen, vendCmd);
            vendCBRet = FALSE;
            break;
    }
    return vendCBRet;
}
//Dual Sensor Trigger: "sys sensor s1s2" in uart command
INT32 UVACExe_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UVAC_VID_RESO_ARY uvacVidResoAry = {0};
    UVAC_AUD_SAMPLERATE_ARY uvacAudSampleRateAry = {0};
    DBG_IND(" ++\r\n");
    Ux_DefaultEvent(pCtrl,NVTEVT_EXE_OPEN,paramNum,paramArray);//=>System_OnSensorAttach
    if(GxUSB_GetIsUSBPlug())
    {
    	//#NT#2017/02/24#Ben Wang -begin
		//#NT#UVC support higher frame size in MJPEG format.
    	UINT32  uiMaxFrameSize;
    	//#NT#2017/02/24#Ben Wang -end
        MEM_RANGE Pool;
        Pool.Addr = OS_GetMempoolAddr(POOL_ID_APP);
        Pool.Size = POOL_SIZE_APP;
        ImageApp_UsbMovie_Config(NVT_USBMOVIE_CFG_POOL, (UINT32)&Pool);
        ImageApp_UsbMovie_Config(NVT_USBMOVIE_CFG_SET_MOVIE_SIZE_CB, (UINT32)xUvac_ConfigMovieSizeCB);
        ImageUnit_SetParam(&ISF_Cap, CAP_PARAM_BUFSIZE, 0); // use full buffer

        #if UVAC_2PATH
        DBG_IND("UVAC_2PATH Enable, Set Channel:%d\r\n", (UINT32)UVAC_CHANNEL_2V2A);
        ImageApp_UsbMovie_Config(NVT_USBMOVIE_CFG_CHANNEL, UVAC_CHANNEL_2V2A);
        #else
        DBG_IND("UVAC_2PATH Disable, Set Channel:%d\r\n", (UINT32)UVAC_CHANNEL_1V1A);
        ImageApp_UsbMovie_Config(NVT_USBMOVIE_CFG_CHANNEL, UVAC_CHANNEL_1V1A);
        #endif

        #if UVAC_SAVE_FILE
        ImageApp_UsbMovie_Config(NVT_USBMOVIE_CFG_SAVE_MOVIE_FILE, TRUE);
        #else
        ImageApp_UsbMovie_Config(NVT_USBMOVIE_CFG_SAVE_MOVIE_FILE, FALSE);
        #endif
        DBG_IND("Save a movie file:%d\r\n", ImageApp_UsbMovie_GetConfig(NVT_USBMOVIE_CFG_SAVE_MOVIE_FILE));

        ImageApp_UsbMovie_Config(NVT_USBMOVIE_CFG_PIM_CAPTURE, FALSE);
        DBG_IND("Capture:%d\r\n", ImageApp_UsbMovie_GetConfig(NVT_USBMOVIE_CFG_PIM_CAPTURE));

        ImageApp_UsbMovie_Config(NVT_USBMOVIE_CFG_TBR_H264, NVT_USBMOVIE_TBR_H264_LOW);
        DBG_IND("TBR-H264:%d\r\n", ImageApp_UsbMovie_GetConfig(NVT_USBMOVIE_CFG_TBR_H264));

        ImageApp_UsbMovie_Config(NVT_USBMOVIE_CFG_TBR_MJPG, NVT_USBMOVIE_TBR_MJPG_DEFAULT);
        DBG_IND("TBR-MJPG:%d\r\n", ImageApp_UsbMovie_GetConfig(NVT_USBMOVIE_CFG_TBR_MJPG));

        uvacVidResoAry.aryCnt = NVT_UI_UVAC_RESO_CNT;//MOVIE_SIZE_ID_MAX;
        uvacVidResoAry.pVidResAry = &gUIUvacVidReso[0];
        #if NVT_UI_UVAC_RESO_INTERNAL
        DBG_DUMP("Use UVAC internal resolution table\r\n");
        #else
        ImageUnit_SetParam(&ISF_UsbUVAC, IMGUNIT_UVAC_CFG_VID_RESO_ARY, (UINT32)&uvacVidResoAry);
        DBG_IND("Vid Reso:%d\r\n",uvacVidResoAry.aryCnt);
        #endif

        uvacAudSampleRateAry.aryCnt = NVT_UI_UVAC_AUD_SAMPLERATE_CNT;
        uvacAudSampleRateAry.pAudSampleRateAry = &gUIUvacAudSampleRate[0];
        ImageUnit_SetParam(&ISF_UsbUVAC, IMGUNIT_UVAC_CFG_AUD_SAMPLERATE_ARY, (UINT32)&uvacAudSampleRateAry);
        DBG_IND("Aud SampleRate:%d\r\n",uvacAudSampleRateAry.aryCnt);

        xUSBMakerInit_UVAC(&gUIUvacDevDesc);
        ImageUnit_SetParam(&ISF_UsbUVAC, IMGUNIT_UVAC_CFG_UVAC_VEND_DEV_DESC, (UINT32)&gUIUvacDevDesc);
        DBG_IND("VID=0x%x, PID=0x%x\r\n",gUIUvacDevDesc.VID, gUIUvacDevDesc.PID);

        DBG_IND("VendCb:0x%x\r\n",(UINT32)xUvacEUVendCmdCB_Idx1);
        ImageUnit_SetParam(&ISF_UsbUVAC, IMGUNIT_UVAC_CFG_EU_VENDCMDCB_ID01, (UINT32)xUvacEUVendCmdCB_Idx1);
		//#NT#2017/02/24#Ben Wang -begin
		//#NT#UVC support higher frame size in MJPEG format.
		//check max frame size
		if(g_UvcVidSizeTable[NVT_UI_UVAC_RESO_IDX_DEF].uiVidFrameRate)
		{
			uiMaxFrameSize = g_UvcVidSizeTable[NVT_UI_UVAC_RESO_IDX_DEF].uiMJPEGTBR/g_UvcVidSizeTable[NVT_UI_UVAC_RESO_IDX_DEF].uiVidFrameRate;
			if(uiMaxFrameSize > UVAC_MAX_PAYLOAD_FRAME_SIZE)
			{
				//double the size for BRC margin
				ImageUnit_SetParam(&ISF_UsbUVAC, IMGUNIT_UVAC_CFG_MAX_FRAME_SIZE, uiMaxFrameSize*2);
				DBG_DUMP("^BuiMaxFrameSize=%dKB\r\n",uiMaxFrameSize/1024);
			}
		}
		//#NT#2017/02/24#Ben Wang -end

        #if UVAC_WINUSB_INTF
        DBG_IND("Enable WinUSB\r\n");
        ImageUnit_SetParam(&ISF_UsbUVAC, IMGUNIT_UVAC_CFG_WINUSB_ENABLE, TRUE);
        DBG_IND("WinUSBCb:0x%x\r\n",(UINT32)xUvacWinUSBCB);
        ImageUnit_SetParam(&ISF_UsbUVAC, IMGUNIT_UVAC_CFG_WINUSB_CB, (UINT32)xUvacWinUSBCB);
        #else
        DBG_IND("DISABLE WinUSB\r\n");
        ImageUnit_SetParam(&ISF_UsbUVAC, IMGUNIT_UVAC_CFG_WINUSB_ENABLE, FALSE);
        #endif

        if (ImageUnit_GetParam(&ISF_ImagePipe, IMAGEPIPE_PARAM_SENSORCOUNT) < 2)
        {
            ISF_UNIT*  pUnit;
            //#NT#2016/12/12#Ben Wang -begin
	        //#NT#Fix multi sensor connect mapping when only sensor2 enable.
            //UINT32 uSensorMask = ImageUnit_GetParam(&ISF_ImagePipe, IMAGEPIPE_PARAM_SENSORMASK);
            pUnit = &ISF_ImagePipe1;
			//#NT#2016/12/12#Ben Wang -end

            ImageUnit_Begin(pUnit, 0);
            ImageUnit_CfgAspect(ISF_IN1, NVT_USBMOVIE_CROPRATIO_W_16, NVT_USBMOVIE_CROPRATIO_H_9);
            ImageUnit_End();
            ImageUnit_SetParam(pUnit, IPL_SEL_AEEV, SEL_AEEV_00);
        }
        else
        {
            DBG_IND("Cofig CfgAspect for ISF_ImagePipe1\r\n");
            ImageUnit_Begin(&ISF_ImagePipe1, 0);
            ImageUnit_CfgAspect(ISF_IN1, NVT_USBMOVIE_CROPRATIO_W_16, NVT_USBMOVIE_CROPRATIO_H_9);
            ImageUnit_End();
            ImageUnit_SetParam(&ISF_ImagePipe1, IPL_SEL_AEEV, SEL_AEEV_00);
            DBG_IND("Cofig CfgAspect for ISF_ImagePipe2\r\n");
            ImageUnit_Begin(&ISF_ImagePipe2, 0);
            ImageUnit_CfgAspect(ISF_IN1, NVT_USBMOVIE_CROPRATIO_W_16, NVT_USBMOVIE_CROPRATIO_H_9);
            ImageUnit_End();
            ImageUnit_SetParam(&ISF_ImagePipe2, IPL_SEL_AEEV, SEL_AEEV_00);
        }
        DBG_IND("Set Parameter for MFK\r\n");
        xUvac_InitSetMFK();

        DBG_IND("+ImageApp_UsbMovie_Open\r\n");
        ImageApp_UsbMovie_Open();
        DBG_IND("-ImageApp_UsbMovie_Open\r\n");

        #if (UVAC_TEST_FOR_CPU_EXCEPTION_MFK && USE_FILEDB)
        DBG_DUMP("^M======== Work Around Test for CPU exception in MFK for 2 live-view ========\r\n");
        xUvac_InitFileDB();
        #endif

        //DBG_IND("======== Dump ImgUnit Mem ========\r\n");
        //ImageStream_DumpInfo();
    }
    else
    {
        DBG_ERR("USB NOT connected!\r\n");
    }
    return NVTEVT_CONSUME;
}
INT32 UVACExe_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    DBG_IND(" +\r\n");
    ImageApp_UsbMovie_Close();
    Ux_DefaultEvent(pCtrl,NVTEVT_EXE_CLOSE,paramNum,paramArray);
    DBG_IND(" ---\r\n");
    return NVTEVT_CONSUME;
}

////////////////////////////////////////////////////////////
EVENT_ENTRY CustomUSBPCCObjCmdMap[] =
{
    {NVTEVT_EXE_OPEN,               UVACExe_OnOpen},
    {NVTEVT_EXE_CLOSE,              UVACExe_OnClose},
    {NVTEVT_NULL,                   0},  //End of Command Map
};

CREATE_APP(CustomUSBPCCObj, APP_SETUP)

