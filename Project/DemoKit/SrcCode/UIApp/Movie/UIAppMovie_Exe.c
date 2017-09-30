////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIAppCommon.h"
#include "UIAppMovie.h"
#include "UIAppPhoto.h"
#include "UICfgDefault.h"
#include "NVTUserCommand.h"
#include "UIMovieMapping.h"
#include "UIPhotoMapping.h"
#include "SysCfg.h" //for memory POOL_ID
#include "UIAppPlay.h"
#include "GxDisplay.h"
#include "Sensor.h"
#include "DxSensor.h"
#include "ImageApp_CamMovie.h"
#include "ImageApp_CamPhoto.h"
#include "ImageUnit_UserDraw.h"
#include "MovieStamp.h"
#include "UIMode.h"
#include "PrjCfg.h"
#include "IQS_Utility.h"
#include "SysMain.h"
#include "GSensor.h"
//#NT#2016/08/03#Charlie Chang -begin
//#NT# support iqtool
#include "IqtoolIpcAPI.h"
//#NT#2016/08/03#Charlie Chang -end
//#NT#2016/06/14#Charlie Chang -begin
//#NT# for Contrast set
#include "iqs_api.h"
//#NT#2016/06/14#Charlie Chang -end
//#NT#2013/05/15#Calvin Chang#Add customer's user data to MOV/MP4 file format -begin
#include "MovieUdtaVendor.h"
//#NT#2013/05/15#Calvin Chang -end
#if (PIP_VIEW_FUNC == ENABLE)
#include "AppDisp_PipView.h"
#endif
#if(IPCAM_FUNC==ENABLE)
#include "WifiAppCmd.h"
#include "RtspNvtApi.h"
#include "WifiAppXML.h"
#endif
#if(WIFI_AP_FUNC==ENABLE)
#include "WifiAppCmd.h"
#include "RtspNvtApi.h"
#include "WifiAppXML.h"
#endif
#include "UIPhotoFuncInfo.h"
#include "rsc_common.h"
#include "ImageUnit_CamDisp.h"
#include "ImageUnit_UserDraw.h"
#include "ImageApp_NetMovie.h"
#include "ImageApp_IPCam.h"
//#NT#2016/05/31#Ben Wang -begin
//#NT#Add UVC multimedia function.
#if(UVC_MULTIMEDIA_FUNC == ENABLE || IPCAM_UVC_FUNC == ENABLE)
#include "ImageApp_UsbMovie.h"
#include "ImageUnit_UsbUVAC.h"
#endif
//#NT#2016/05/31#Ben Wang -end
#include "gpio.h"
#include "UIControlWnd.h"
#include "dma.h"
#include "dis_alg_fw.h"

#if(UCTRL_FUNC==ENABLE)
#include "UCtrlAppMovie.h"
#include "UCtrlMain.h"
#endif

#if (AUDIO_PLAY_FUNCTION == ENABLE)
#include "Audio.h"
#include "WavStudioTsk.h"
//#NT#2016/12/22#HM Tseng -begin
//#NT# Support AEC function
#if(AUDIO_AEC_FUNC==ENABLE)
#include "nvtaec_api.h"
#endif
//#NT#2016/12/22#HM Tseng -end
#endif

//#NT#2016/10/18#Jeah Yen -begin
//#NT#move code to support sensor map
#include "md_api.h"
//#NT#2016/10/18#Jeah Yen -begin

//#NT#2016/03/08#Lincy Lin -begin
//#NT#Support object tracking function
#if MOVIE_OT_FUNC
#include "NvtOt.h"
#endif
//#NT#2016/03/18#Lincy Lin -end

//#NT#2016/05/20#Yang Jin -begin
//#NT#Support Dis dsp function
#if MOVIE_DISDSP_FUNC
#include "dis_alg_dsp.h"
#endif
//#NT#2016/05/20#Yang Jin -end


//#NT#2016/04/22#Nestor Yang -begin
//#NT# Support Driver Drowsiness Detection(DDD)
#if MOVIE_DDD_FUNC
#include "DDD_lib.h"
#endif
//#NT#2016/04/22#Nestor Yang -end
//#NT#2016/05/23#David Tsai -begin
//#NT# Support tarmpering detection and background calculation function
//#NT#2016/10/14#Yuzhe Bian -begin
//#NT# Support trip-wire detection & trip-zone detection function
#if MOVIE_BC_FUNC || MOVIE_TD_FUNC || MOVIE_TWD_FUNC || MOVIE_TZD_FUNC
#include "BC_lib.h"
#endif
//#NT#2016/10/14#Yuzhe Bian -end

#if MOVIE_TD_FUNC
#include "TD_lib.h"
#endif
//#NT#2016/05/23#David Tsai -end

//#NT#2016/10/14#Yuzhe Bian -begin
//#NT# Support trip-wire detection & trip-zone detection function
#if MOVIE_TWD_FUNC
#include "TWD_lib.h"
#endif
#if MOVIE_TZD_FUNC
#include "TZD_lib.h"
#endif
//#NT#2016/10/14#Yuzhe Bian -end

#if MOVIE_TSR_FUNC_
#include "tsr_lib.h"
#include "TSD_DSP_lib.h"
#endif

//#NT#2016/10/17#Bin Xiao -begin
//#NT# Support Face Tracking Grading(FTG)
#if MOVIE_FTG_FUNC
#include "ftg_lib.h"
#include "ftg_fd_lib.h"
#endif
//#NT#2016/10/17#Bin Xiao -end


//#NT#2016/06/28#KCHong -begin
//#NT#DbgLog in GPS section
#if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
#include "mp4log.h"
#endif
//#NT#2016/06/28#KCHong -end

//#NT#2016/09/20#Bob Huang -begin
//#NT#Support HDMI Display with 3DNR Out
#if (_3DNROUT_FUNC == ENABLE)
BOOL gb3DNROut = FALSE;
#endif
//#NT#2016/09/20#Bob Huang -end
#if (_ADAS_DSP_FUNC_ == ENABLE)
#include "ADAS_DSP_Lib.h"
#endif

extern void System_DispSourceByTarget(ISIZE* pImgSize, USIZE* pAspectRatio);
extern UINT32 System_GetEnableDisp(void);
extern UINT32 System_GetEnableSensor(void);
extern void SMediaRec_SetEncPath(UINT32 uiVidEncId, VIDENC_PATH path);

//#NT#2012/07/31#Hideo Lin -begin
//#NT#Test codes
#define MOVIE_YUV_SIZE_MAX      (1920*1080*2)   // maximum YUV raw data size
#define MOVIE_YUV_NUM           8   // 8 source images for fixed YUV mode

//#NT#2012/10/30#Calvin Chang#Generate Audio NR pattern by noises of zoom, focus and iris -begin
#define MOVIE_AUDIO_NR_PATTERN_ENABLE    DISABLE
//#NT#2012/10/30#Calvin Chang -end


#if (FISHEYE_TYPE == ENABLE)

IPL_VR360_DATA g_VR360Setting;
#endif

#if (AUDIO_PLAY_FUNCTION == ENABLE)
AUDIO_CH Movie_AudioChTable[5] =
{
    0,
    AUDIO_CH_LEFT,
    AUDIO_CH_RIGHT,
    AUDIO_CH_STEREO,
    AUDIO_CH_MONO,
};
#endif


#if (MOVIE_TEST_ENABLE == ENABLE)
static MEDIAREC_TESTMODE g_MediaRecTestMode = {0};
extern void MovRec_SetTestMode(MEDIAREC_TESTMODE *pTestMode);
#endif
//#NT#2016/06/08#Lincy Lin -begin
//#NT#Implement generic OSD and video drawing mechanism for ALG function
static void MovieExe_CalcFDDispCord(URECT *dispCord);
static void MovieExe_CalcOSDDispCord(URECT *dispCord);
//#NT#2016/06/08#Lincy Lin -end

#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UiAppMovie
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
//#define __DBGFLT__          "[cap]"
//#define __DBGFLT__          "[cb]"
//#define __DBGFLT__          "[set]"
//#define __DBGFLT__          "[zoom]"
//#define __DBGFLT__          "[af]"
#include "DebugModule.h"

//---------------------UIAPP_PHOTO Global Variables----------

BOOL _g_bFirstMovie = TRUE;
BOOL _g_bFirstMovieRec = FALSE;
BOOL bIsWiFiRecorded = FALSE;

//#NT#2010/05/21#Photon Lin -begin
#define THUMB_FRAME_SIZE 0x32000
//#NT#2010/05/21#Photon Lin -end
#define MINUTE_240 14400  // 4 hours
#define MINUTE_60  3600   //1 // 1 hour
#define MINUTE_29  1740   //29 minutes

#if(IPCAM_FUNC == ENABLE)
//#NT#2016/07/07#David Tsai -begin
//#NT#Two stream mode for IP Cam
#if TWO_STREAM_MODE
#if ((_MODEL_DSC_ == _MODEL_IPCAM4_TL_DEWARP_) || \
     (_MODEL_DSC_ == _MODEL_TCT_IPCAM_EVB_)     || \
     (_MODEL_DSC_ == _MODEL_TCT_IPCAM_TL_))
#define RTSP_MAX_CLIENT         2
#else
#define RTSP_MAX_CLIENT         1
#endif
#else
#define RTSP_MAX_CLIENT         8
#endif
//#NT#2016/07/07#David Tsai -end
#else
#define RTSP_MAX_CLIENT         1
#endif
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
//#NT#2017/02/24#Ben Wang -begin
//#NT#Fix that UVC_MULTIMEDIA_FUNC depends on WIFI_AP_FUNC.
#if((WIFI_AP_FUNC==ENABLE) || (UVC_MULTIMEDIA_FUNC == ENABLE))
//#NT#2017/02/24#Ben Wang -end
static USIZE MOVIE_HTTPLVIEW_SIZE[IMAGERATIO_MAX_CNT]=
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
//static IPL_IME_INFOR            g_ImeInfo = {0};
//static IPL_SET_IMGRATIO_DATA    g_ImgRatio = {0};
static BOOL    bSDIsSlow    = FALSE;
#if (MOVIE_REC_YUVMERGE == ENABLE)
static volatile BOOL   g_bMovieRecYUVMergeEnable = 0;  // Isiah, implement YUV merge mode of recording func.
static volatile UINT32 g_uiMovieRecYUVMergeInterval = 1, g_uiMovieRecYUVMergeCounter = 0;
#endif
//#NT#2016/06/08#Lincy Lin -begin
//#NT#Implement generic OSD and video drawing mechanism for ALG function
URECT  gMovieFdDispCord;
URECT  gMovieOsdDispCord;
//#NT#2016/06/08#Lincy Lin -end
// For UCTRL use.
static UINT32 g_uiAudChannel = MOVREC_AUDTYPE_RIGHT;
static UINT32 g_uiAudSampleRate = 32000;
static UINT32 g_uiAudCodec = MOVAUDENC_AAC;

//#NT#2016/06/28#KCHong -begin
//#NT#DbgLog in GPS section
#if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
// Declare debug log sub-section ID here
// Ex: UINT32 DbgLogTestID = MP4LOG_NULL_ID;
#if (_ADAS_FUNC_ == ENABLE)
UINT32 DbgLogSnGID = MP4LOG_NULL_ID;
UINT32 DbgLogFCLDID = MP4LOG_NULL_ID;
UINT32 DbgLogAutoVPID = MP4LOG_NULL_ID;
#endif

//#NT#2016/11/08#CC Chao -begin
//#NT#Saving log for TSR
#if (_TSR_FUNC_ == ENABLE)
UINT32 DbgLogTsrID = MP4LOG_NULL_ID;
#endif
//#NT#2016/11/08

//#NT#2016/07/20#ML Cui -begin
//#NT#Saving log for DDD
#if (MOVIE_DDD_SAVE_LOG == ENABLE)
UINT32 DbgLogDDDID = MP4LOG_NULL_ID;
#endif
//#NT#2016/7/20

//#NT#2016/08/06#XQ Liu -begin
//#NT#Saving log for TD
#if (MOVIE_TD_SAVE_LOG == ENABLE)
UINT32 DbgLogTDID = MP4LOG_NULL_ID;
#endif
//#NT#2016/08/06#XQ Liu -end

#endif
//#NT#2016/06/28#KCHong -end

//#NT#2016/07/26#Niven Cho -begin
//#NT#IPCAM-DSP
#if (_DSP_TYPE_ == _DSP_FREERTOS_) && (IPCAM_FUNC==ENABLE)
#include "ROI_lib.h"
#include "IPL_Display.h"
_ALIGNED(4) static UINT8 g_ROIBuf[512] = {0};
#endif
//#NT#2016/07/26#Niven Cho -end

//#NT#2016/09/29#KCHong -begin
//#NT#The GPS related variables should not depend on ADAS.
#if 1//(GPS_FUNCTION == ENABLE)
FLOAT g_CurSpeed = 0.0;
BOOL g_GPSLinked = FALSE;
BOOL g_GPSStatus = FALSE;
#endif
//#NT#2016/09/29#KCHong -end

//#NT#2016/06/20#KCHong -begin
//#NT#New ADAS
#if (_ADAS_FUNC_ == ENABLE || (_CPU2_TYPE_==_CPU2_LINUX_ && IPCAM_FUNC==DISABLE))
static BOOL g_ADASFuncSupported = FALSE;
#endif

#if (_ADAS_FUNC_ == ENABLE)
static ADAS_DSP_RESULT_INFO g_AdasRltOSD = {0};
static ADAS_APPS_RESULT_INFO g_AdasRltVideo = {0};
#if (_SNG_FUNC_ == ENABLE)
static ADAS_SNG_ROI_INFO g_SnGROIOSD = {0};
static ADAS_SNG_ROI_INFO g_SnGROIVideo = {0};
#endif
#if 1
ADAS_DSP_RESULT_INFO* MovieExe_GetAdasRltOSD(void)
{
    return &g_AdasRltOSD;
}
#else 
ADAS_APPS_RESULT_INFO* MovieExe_GetAdasRltOSD(void)
{
    return &g_AdasRltOSD;
}
#endif 

ADAS_APPS_RESULT_INFO* MovieExe_GetAdasRltVideo(void)
{
    return &g_AdasRltVideo;
}

ADAS_SNG_ROI_INFO* MovieExe_GetSnGROIOSD(void)
{
#if (_SNG_FUNC_ == ENABLE)
    return &g_SnGROIOSD;
#else
    return NULL;
#endif
}

ADAS_SNG_ROI_INFO* MovieExe_GetSnGROIVideo(void)
{
#if (_SNG_FUNC_ == ENABLE)
    return &g_SnGROIVideo;
#else
    return NULL;
#endif
}

BOOL MovieExe_IsAdasFuncSupported(void)
{
    return g_ADASFuncSupported;
}
#endif
//#NT#2016/06/20#KCHong -end

ER MovieExe_SetGetReadyCB2_Sample(void);//2013/09/23 Meg
//#NT#2016/05/31#Ben Wang -begin
//#NT#Add UVC multimedia function.
#if(UVC_MULTIMEDIA_FUNC == ENABLE || IPCAM_UVC_FUNC == ENABLE)
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
_ALIGNED(4) static UINT16 m_UVACSerialStrDesc3[] =
{
    0x0320,                             // 20: size of String Descriptor = 32 bytes
                                        // 03: String Descriptor type
    '9', '6', '6', '6', '0',            // 96611-00000-001 (default)
    '0', '0', '0', '0', '0',
    '0', '0', '1', '0', '0'
};
static void xUSBMakerInit_UVAC(UVAC_VEND_DEV_DESC *pUVACDevDesc)
{
    pUVACDevDesc->pManuStringDesc = (UVAC_STRING_DESC *)m_UVACManuStrDesc;
    pUVACDevDesc->pProdStringDesc = (UVAC_STRING_DESC *)m_UVACProdStrDesc;
    pUVACDevDesc->pSerialStringDesc = (UVAC_STRING_DESC *)m_UVACSerialStrDesc3;
    pUVACDevDesc->VID = USB_VID;
    pUVACDevDesc->PID = USB_PID_PCCAM;
}
#include "USBCDC.h"
static BOOL CdcPstnReqCB(CDC_COM_ID ComID, UINT8 Code, UINT8 *pData, UINT32 *pDataLen)
{
    BOOL bSupported = TRUE;
    CDCLineCoding LineCoding;
    DBGD(ComID);
    switch (Code)
    {
        case REQ_GET_LINE_CODING:
            DBG_DUMP("Get Line Coding\r\n");
            LineCoding.uiBaudRateBPS = 115200;
            LineCoding.uiCharFormat = 0;//CDC_LINEENCODING_OneStopBit;
            LineCoding.uiParityType = 0;//CDC_PARITY_None;
            LineCoding.uiDataBits = 8;
            *pDataLen = sizeof(LineCoding);
            memcpy(pData, &LineCoding, *pDataLen);
            break;
        case REQ_SET_LINE_CODING:
            DBG_DUMP("Set Line Coding\r\n");
            if(*pDataLen == sizeof(LineCoding))
            {
                memcpy(&LineCoding, pData, *pDataLen);
            }
            else
            {
                bSupported = FALSE;
            }
            break;
        case REQ_SET_CONTROL_LINE_STATE:
            DBG_DUMP("Control Line State = 0x%X\r\n",*(UINT16 *)pData);
            //debug console test
            if(*(UINT16 *)pData == 0x3)//console ready
            {
            }
            break;
        default:
            bSupported = FALSE;
            break;
    }
    return bSupported;
}

#endif
//#NT#2016/05/31#Ben Wang -end
void MovieExe_IPL_SetIInfo(UINT32 index, UINT32 value)
{
    if(System_GetEnableSensor() & SENSOR_1)
    {
        ImageUnit_SetParam(&ISF_ImagePipe1, index, value);
    }
    if(System_GetEnableSensor() & SENSOR_2)
    {
        ImageUnit_SetParam(&ISF_ImagePipe2, index, value);
    }
}
void MovieExe_Cap_SetIInfo(UINT32 index, UINT32 value)
{
    #if (MOVIE_PIM_CAPTURE == ENABLE)
    ImageUnit_SetParam(&ISF_Cap, index, value);
    #endif
}

void MovieExe_RSC_SetSwitch(UINT32 index, UINT32 value)
{
//#NT#2016/10/18#Jeah Yen -begin
//#NT#support sensor map
#if (_IPPLIB_ != _IPL_FAKE_)
    // RSC just support IPL 1
    RSC_SetSwitch(IPL_PATH_1, index, value);
#endif
//#NT#2016/10/18#Jeah Yen -end
}


ISIZE Movie_GetBufferSize(void) //return USER CUSTOM buffer size
{
    return GxVideo_GetDeviceSize(DOUT1);
}

//#NT#2012/07/31#Hideo Lin -begin
//-------------------------------------------------------------------------------------------------
// Set (load) YUV test pattern from JPEG files
#if (MOVIE_TEST_ENABLE == ENABLE)
#define YUV_RAW_PATH_FHD    "A:\\YUV_RAW\\FHD\\CLOCK\\"
#define YUV_RAW_PATH_HD     "A:\\YUV_RAW\\HD\\CLOCK\\"
#define YUV_RAW_PATH_VGA    "A:\\YUV_RAW\\VGA\\CLOCK\\"
#define YUV_RAW_PATH_QVGA   "A:\\YUV_RAW\\QVGA\\CLOCK\\"

static char sDirPath[40];
static char sFileName[40];

static void Movie_SetYUV(UINT32 uiMovieSize)
{
    UINT32      i;
    UINT32      uiPoolAddr, uiRawAddr, uiFileSize, uiHeight;
    UINT32      uiYLineOffset, uiUVLineOffset;
    char        *pDirPath;
    ER          err = E_OK;
    FST_FILE    pFile;

    uiPoolAddr = OS_GetMempoolAddr(POOL_ID_APP);
    uiRawAddr = uiPoolAddr;

    // ----------------------------------- <--- APP buffer starting address
    // | YUV buffer 1 for media recorder |
    // -----------------------------------
    // | YUV buffer 2 for media recorder |
    // -----------------------------------
    // | YUV buffer 3 for media recorder |
    // -----------------------------------
    // | ...                             |
    // ----------------------------------- <--- media recorder starting address
    //
    // Note: Be sure the buffer is enough for storing the necessary YUV patterns!

    //pDirPath = UserMedia_GetRawDirPath();

    switch (uiMovieSize)
    {
    case MOVIE_SIZE_1080P_FULLRES: // 1080p
    case MOVIE_SIZE_1080P: // 1080p
        uiYLineOffset  = 1920;
        uiUVLineOffset = 1920; // UV pack
        uiHeight = 1088;
        //if (*pDirPath == 0) // never set directory path, use default
        {
            sprintf(sDirPath, YUV_RAW_PATH_FHD, sizeof(YUV_RAW_PATH_FHD));
            pDirPath = sDirPath;
        }
        break;

    case MOVIE_SIZE_VGA:
    //case MOVIE_SIZE_WVGA: // no WVGA pattern, use VGA instead
        uiYLineOffset  = 640;
        uiUVLineOffset = 640; // UV pack
        uiHeight = 480;
        //if (*pDirPath == 0) // never set directory path, use default
        {
            sprintf(sDirPath, YUV_RAW_PATH_VGA, sizeof(YUV_RAW_PATH_VGA));
            pDirPath = sDirPath;
        }
        break;

    case MOVIE_SIZE_QVGA:
        uiYLineOffset  = 320;
        uiUVLineOffset = 320; // UV pack
        uiHeight = 240;
        //if (*pDirPath == 0) // never set directory path, use default
        {
            sprintf(sDirPath, YUV_RAW_PATH_QVGA, sizeof(YUV_RAW_PATH_QVGA));
            pDirPath = sDirPath;
        }
        break;

    case MOVIE_SIZE_720P:
    default:
        uiYLineOffset  = 1280;
        uiUVLineOffset = 1280; // UV pack
        uiHeight = 720;
        //if (*pDirPath == 0) // never set directory path, use default
        {
            sprintf(sDirPath, YUV_RAW_PATH_HD, sizeof(YUV_RAW_PATH_HD));
            pDirPath = sDirPath;
        }
        break;
    }

    g_MediaRecTestMode.YUVInfo.uiTotalNum = MOVIE_YUV_NUM;

    for (i = 0; i < g_MediaRecTestMode.YUVInfo.uiTotalNum; i++)
    {
        // 1. Read Y data from file
        sprintf(sFileName, "%sY%02d.RAW", pDirPath, i+1);
        uiFileSize = MOVIE_YUV_SIZE_MAX;
        pFile = FileSys_OpenFile(sFileName, FST_OPEN_READ);
        if (pFile)
        {
            FileSys_ReadFile(pFile, (UINT8 *)uiRawAddr, &uiFileSize, 0, NULL);
        }
        FileSys_CloseFile(pFile);
        DBG_IND("File %s, size %d\r\n", sFileName, uiFileSize);

        if (err != E_OK)
        {
            DBG_ERR("Open %s for reading failed!\r\n", sFileName);
            return;
        }
        else if (uiFileSize == MOVIE_YUV_SIZE_MAX)
        {
            DBG_ERR("Read buffer is not enough!\r\n");
            return;
        }

        // Set Y source address to media recorder
        g_MediaRecTestMode.YUVInfo.YUVSrc[i].uiYAddr = uiRawAddr;
        //uiRawAddr += uiFileSize;
        uiRawAddr += (uiYLineOffset * uiHeight);

        // 2. Read UV data from file
        sprintf(sFileName, "%sUV%02d.RAW", pDirPath, i+1);
        uiFileSize = MOVIE_YUV_SIZE_MAX;
        pFile = FileSys_OpenFile(sFileName, FST_OPEN_READ);
        if (pFile)
        {
            FileSys_ReadFile(pFile, (UINT8 *)uiRawAddr, &uiFileSize, 0, NULL);
        }
        FileSys_CloseFile(pFile);
        DBG_IND("File %s, size %d\r\n", sFileName, uiFileSize);

        if (err != E_OK)
        {
            DBG_ERR("Open %s for reading failed!\r\n", sFileName);
            return;
        }
        else if (uiFileSize == MOVIE_YUV_SIZE_MAX)
        {
            DBG_ERR("Read buffer is not enough!\r\n");
            return;
        }

        // Set UV source address to media recorder
        g_MediaRecTestMode.YUVInfo.YUVSrc[i].uiCbAddr = uiRawAddr;
        g_MediaRecTestMode.YUVInfo.YUVSrc[i].uiCrAddr = uiRawAddr;
        //uiRawAddr += uiFileSize;
        uiRawAddr += (uiUVLineOffset * uiHeight) / 2;

        // Set Y line offset
        g_MediaRecTestMode.YUVInfo.YUVSrc[i].uiYLineOffset  = uiYLineOffset;
        // Set UV line offset
        g_MediaRecTestMode.YUVInfo.YUVSrc[i].uiUVLineOffset = uiUVLineOffset;

        DBG_MSG("g_UserMediaYUVInfo.YUVSrc[%d].uiYAddr        0x%x\r\n", i, g_MediaRecTestMode.YUVInfo.YUVSrc[i].uiYAddr);
        DBG_MSG("g_UserMediaYUVInfo.YUVSrc[%d].uiCbAddr       0x%x\r\n", i, g_MediaRecTestMode.YUVInfo.YUVSrc[i].uiCbAddr);
        DBG_MSG("g_UserMediaYUVInfo.YUVSrc[%d].uiCrAddr       0x%x\r\n", i, g_MediaRecTestMode.YUVInfo.YUVSrc[i].uiCrAddr);
        DBG_MSG("g_UserMediaYUVInfo.YUVSrc[%d].uiYLineOffset  %d\r\n",   i, g_MediaRecTestMode.YUVInfo.YUVSrc[i].uiYLineOffset);
        DBG_MSG("g_UserMediaYUVInfo.YUVSrc[%d].uiUVLineOffset %d\r\n",   i, g_MediaRecTestMode.YUVInfo.YUVSrc[i].uiUVLineOffset);
    }
}
#endif

void Movie_SetSDSlow(BOOL IsSlow)
{
    bSDIsSlow = IsSlow;
}

void Movie_SetQPInitLevel(void)
{
    UINT32  uiQPLevel = H264_QP_LEVEL_5;

    // To do: set H.264 QP initial level according to ISO value

    MovRec_ChangeParameter(MOVREC_RECPARAM_H264QPLEVEL, uiQPLevel, 0, 0);
}

UINT32 Movie_GetAudChannel(void)
{
    return g_uiAudChannel;
}

void Movie_SetAudChannel(UINT32 uiAudChannel)
{
    g_uiAudChannel = uiAudChannel;
}

UINT32 Movie_GetAudSampleRate(void)
{
    return g_uiAudSampleRate;
}

void Movie_SetAudSampleRate(UINT32 uiAudSampleRate)
{
    g_uiAudSampleRate = uiAudSampleRate;
}

UINT32 Movie_GetAudCodec(void)
{
    return g_uiAudCodec;
}

void Movie_SetAudCodec(UINT32 uiAudCodec)
{
    g_uiAudCodec = uiAudCodec;
}
//#NT#2016/06/15#Charlie Chang -begin
//#NT# reset movie mapping table
#if(IOT_P2P_FUNC == ENABLE)
static void Movie_P2P_ResetMappingTable(void){

    SetMovieTargetBitrate(0, MOVIE_SIZE_CLONE_1920x1080P60_848x480P30, (1024*1024)/8);
    SetMovieTargetFrameRate(0, MOVIE_SIZE_CLONE_1920x1080P60_848x480P30, 30);
    //SetMovieTargetBitrate(0, MOVIE_SIZE_CLONE_1920x1080P30_848x480P30, (128*1024)/8);
    //SetMovieTargetFrameRate(0, MOVIE_SIZE_CLONE_1920x1080P30_848x480P30, 20);
    SetMovieTargetBitrate(0, MOVIE_SIZE_CLONE_1280x720P30_1280x720P30, (1024*1024)/8);
    SetMovieTargetFrameRate(0, MOVIE_SIZE_CLONE_1280x720P30_1280x720P30, 30);
    SetMovieTargetBitrate(0, MOVIE_SIZE_CLONE_848x480P30_848x480P30, (1024*1024)/8);
    SetMovieTargetFrameRate(0, MOVIE_SIZE_CLONE_848x480P30_848x480P30, 30);
    SetMovieTargetBitrate(0, MOVIE_SIZE_CLONE_640x480P30_640x480P30, (1024*1024)/8);
    SetMovieTargetFrameRate(0, MOVIE_SIZE_CLONE_640x480P30_640x480P30, 30);
    SetMovieTargetBitrate(0, MOVIE_SIZE_CLONE_320x240P30_320x240P30, (1024*1024)/8);
    SetMovieTargetFrameRate(0, MOVIE_SIZE_CLONE_320x240P30_320x240P30, 30);
}
#endif
//#NT#2016/06/15#Charlie Chang -end
void Movie_SetRecParam(void)
{
    UINT32  uiRecFormat;
    UINT32  uiFileType, uiWidth, uiHeight, uiMovieSize, uiQuality;
    UINT32  uiVidCodec, uiVidFrameRate, uiTargetBitrate, uiH264GopType, ui3DNRLevel, uiVidRotate, uiDispAspectRatio;
    UINT32  uiAudCodec, uiAudSampleRate, uiAudChannelNum, uiAudChannelType;
    BOOL    bAudFilterEn;
#if (MOVIE_MULTI_RECORD_FUNC)
    UINT32  uiVidCodec2;
    UINT32  uiFileType2=0, uiWidth2=0, uiHeight2=0, uiTargetBitrate2=0, uiDispAspectRatio2=0, uiVidFrameRate2=0;
#endif
    #if (MOVIE_DDD_FUNC)
    UINT32 nSensor= ImageUnit_GetParam(&ISF_ImagePipe, IMAGEPIPE_PARAM_SENSORCOUNT); //return 0/1/2/4
    #endif
    uiMovieSize         = UI_GetData(FL_MOVIE_SIZE);            // movie size
    uiQuality           = UI_GetData(FL_MOVIE_QUALITY);         // movie quality

//#NT#2016/06/15#Charlie Chang -begin
//#NT# wifi p2p need reset mapping table
    #if(IOT_P2P_FUNC == ENABLE)
        Movie_P2P_ResetMappingTable();
    #endif
//#NT#2016/06/15#Charlie Chang -end
    if (SysGetFlag(FL_MOVIE_TIMELAPSE_REC)==MOVIE_TIMELAPSEREC_OFF)
    {
        if (SysGetFlag(FL_MovieAudioRec)==MOVIE_AUD_REC_OFF)
        {
            uiRecFormat     = MEDIAREC_VID_ONLY;            // recording type
        }
        else
        {
            uiRecFormat     = MEDIAREC_AUD_VID_BOTH;            // recording type
        }
    }
    else
    {
        uiRecFormat     = MEDIAREC_TIMELAPSE;               // recording type


        //#NT#2016/12/27#Calvin Chang#[0115114] ¡Vbegin
        // Disable Emergency Recording
        DBG_DUMP("Disable Emergency Recording on TIMELAPSE mode!\r\n");
        UI_SetData(FL_MOVIE_URGENT_PROTECT_AUTO, MOVIE_URGENT_PROTECT_AUTO_OFF);
        UI_SetData(FL_MOVIE_URGENT_PROTECT_MANUAL, MOVIE_URGENT_PROTECT_MANUAL_OFF);
        MovRec_SetEmergencyRecording(0, 0);
        MovRec_SetEmergencyRecording(1, 0);
        //#NT#2016/12/27#Calvin Chang#[0115114] ¡Vend
    }

#if (MOVIE_REC_YUVMERGE == ENABLE)
    // Isiah, implement YUV merge mode of recording func.
    if (FlowMovie_RecGetYUVMergeMode())
    {
        uiRecFormat         = MEDIAREC_MERGEYUV;
    }
#endif
    //#NT#2016/08/03#Niven Cho -begin
    //#NT#CarDV-Linux use TS file
#if (_CPU2_TYPE_ == _CPU2_LINUX_ && IPCAM_FUNC!=ENABLE)
    uiFileType          = MEDIA_FILEFORMAT_TS;                          // file type
#else
    uiFileType          = MEDIA_FILEFORMAT_MP4;                         // file type
#endif
    //#NT#2016/08/03#Niven Cho -end

#if (MOVIE_MULTI_RECORD_FUNC)
    uiWidth             = GetMovieSizeWidth_2p(0, uiMovieSize);         // image width
    uiHeight            = GetMovieSizeHeight_2p(0, uiMovieSize);        // image height
    uiVidFrameRate      = GetMovieFrameRate_2p(0, uiMovieSize);         // video frame rate
    uiTargetBitrate     = GetMovieTargetBitrate_2p(0, uiMovieSize);     // enable rate control and set target rate
    uiDispAspectRatio   = GetMovieDispAspectRatio_2p(0, uiMovieSize);   // display aspect ratio
#else
    uiWidth             = GetMovieSizeWidth(uiMovieSize);               // image width
    uiHeight            = GetMovieSizeHeight(uiMovieSize);              // image height
    uiVidFrameRate      = GetMovieFrameRate(uiMovieSize);               // video frame rate
    uiTargetBitrate     = GetMovieTargetBitrate(uiMovieSize, uiQuality);// enable rate control and set target rate
    uiDispAspectRatio   = GetMovieDispAspectRatio(uiMovieSize);         // display aspect ratio
#endif
    uiH264GopType       = MEDIAREC_H264GOP_IPONLY;              // H.264 GOP type (IP only)

    if (UI_GetData(FL_MovieMCTFIndex) == MOVIE_MCTF_OFF)
    {
        ui3DNRLevel     = H264_3DNR_DISABLE;                    // H.264 3DNR disable
    }
    else
    {
        //ui3DNRLevel     = H264_3DNR_NORMAL;                     // H.264 3DNR enable (normal)
        //ui3DNRLevel     = H264_3DNR_WEAK;                       // H.264 3DNR enable (weak)
        ui3DNRLevel     = H264_3DNR_STRONGEST;                  // H.264 3DNR enable (strongest)
    }

#if (_FPGA_EMULATION_ == ENABLE)
    uiAudSampleRate     = 8000;                                // audio sampling rate
#else
    uiAudSampleRate     = Movie_GetAudSampleRate();            // audio sampling rate
#endif
    uiAudChannelType    = Movie_GetAudChannel();               // audio channel type
//#NT#2016/09/22#Ben Wang -begin
//#NT#Always use PCM for UVAC
#if(UVC_MULTIMEDIA_FUNC == ENABLE || IPCAM_UVC_FUNC == ENABLE)
	uiAudChannelNum     = 2;                                    // audio channel number
#else
    uiAudChannelNum     = 1;                                    // audio channel number
#endif
//#NT#2016/09/22#Ben Wang -end
    bAudFilterEn        = FALSE;                                // enable/disable audio filter

    #if 1
    //#NT#2016/06/03#Charlie Chang -begin
    //#NT# define p2p audio/video config
    #if(IOT_P2P_FUNC == ENABLE)
    uiAudCodec  = MOVAUDENC_PPCM;
    uiAudSampleRate = 8000;
    uiVidCodec  = MEDIAVIDENC_H264;
    uiFileType = MEDIA_FILEFORMAT_MOV;
    #else
    uiAudCodec  = MOVAUDENC_AAC;
    uiVidCodec  = MEDIAVIDENC_H264;
    #endif
    //#NT#2016/06/03#Charlie Chang -end
    #else
    switch (uiFileType)
    {
    case MEDIA_FILEFORMAT_AVI: // AVI format, audio should be PCM, video can be H.264 or MJPEG
        uiAudCodec  = MOVAUDENC_PCM;
        uiVidCodec  = MEDIAVIDENC_H264;
        break;

    case MEDIA_FILEFORMAT_MP4: // MP4 format, audio should be AAC, video should be H.264
        uiAudCodec  = MOVAUDENC_AAC;
        uiVidCodec  = MEDIAVIDENC_H264;
        break;

    case MEDIA_FILEFORMAT_MOV: // MOV format, audio can be PCM or AAC, video can be H.264 or MJPEG
    default:
        uiAudCodec  = MOVAUDENC_PPCM;
        uiVidCodec  = MEDIAVIDENC_H264;
        break;
    }
    #endif

#if(IPCAM_FUNC==ENABLE)
    IQS_SetIPcamFunction(1);
    uiAudCodec = Movie_GetAudCodec();
    uiVidCodec = GetMovieVidEncFmt_2p(VIDENC_ID_1);
#endif

    //#NT#2013/04/17#Calvin Chang#Support Rotation information in Mov/Mp4 File format -begin
    uiVidRotate = MOVREC_MOV_ROTATE_0;
    //#NT#2013/04/17#Calvin Chang -end

    //---------------------------------------------------------------------------------------------
    // set media recording control parameters
    //---------------------------------------------------------------------------------------------
    // disable golf shot recording
    MovRec_ChangeParameter(MOVREC_RECPARAM_GOLFSHOT_ON, FALSE, 0, 0);
    // disable flash recording
    MovRec_ChangeParameter(MOVREC_RECPARAM_FLASH_ON, FALSE, 0, 0);
    //#NT#2016/05/19#Hideo Lin -begin
    //#NT#Always enable power off protection to avoid FAT error as abnormal power off!
    MovRec_ChangeParameter(MOVREC_RECPARAM_PWROFFPT, TRUE, 0, 0);
    //#NT#2016/05/19#Hideo Lin -end
    // disable time lapse recording => not necessary now, since it can be set by using MOVREC_RECPARAM_RECFORMAT
    //MovRec_ChangeParameter(MOVREC_RECPARAM_TIMELAPSE, FALSE, 0, 0);
    // set recording format (MOVREC_VID_ONLY, MEDIAREC_AUD_VID_BOTH, MEDIAREC_TIMELAPSE, ...)
    MovRec_ChangeParameter(MOVREC_RECPARAM_RECFORMAT, uiRecFormat, 0, 0);
    // set customized user data on/off
    //ImageUnit_SetParam(&ISF_Media, MOVREC_RECPARAM_EN_CUSTOMUDATA, FALSE);

    //#NT#2016/10/17#Adam Su -begin
    //#NT#Do not allocate file when cpu2 is linux
#if (_CPU2_TYPE_ == _CPU2_LINUX_ && IPCAM_FUNC!=ENABLE)
    MovRec_ChangeParameter(MOVREC_RECPARAM_NOALLOCFILE, 1, 0, 0);
#endif
    //#NT#2016/10/17#Adam Su -end

    // set recording end type (MEDIAREC_ENDTYPE_NORMAL, MEDIAREC_ENDTYPE_CUTOVERLAP, MEDIAREC_ENDTYPE_CUT_TILLCARDFULL)
#if (MOVIE_AUTOREC_CYCLICTIME == ENABLE)
    if (UI_GetData(FL_MOVIE_CYCLIC_REC) != MOVIE_CYCLICREC_OFF)
    {
        MovRec_ChangeParameter(MOVREC_RECPARAM_ENDTYPE, MOVREC_ENDTYPE_CUTOVERLAP, 0, 0);
    }
    else
#endif
    {
        #if 0
        MovRec_ChangeParameter(MOVREC_RECPARAM_ENDTYPE, MOVREC_ENDTYPE_NORMAL, 0, 0);
        #else
        MovRec_ChangeParameter(MOVREC_RECPARAM_ENDTYPE, SMEDIAREC_ENDTYPE_CUT_TILLCARDFULL, 0, 0);
        MovRec_ChangeParameter(MOVREC_RECPARAM_CUTSEC, 900, 0, 0); // 15 min
        #endif
    }

    // enable/disable time lapse recording
    if (uiRecFormat == MEDIAREC_TIMELAPSE)
    {
        MovRec_ChangeParameter(MOVREC_RECPARAM_TIMELAPSE, TRUE, 0, 0);
        MovRec_ChangeParameter(MOVREC_RECPARAM_TIMELAPSE_TIME, Get_MovieTimeLapseValue(SysGetFlag(FL_MOVIE_TIMELAPSE_REC)), 0, 0);
        MovRec_ChangeParameter(MOVREC_RECPARAM_ENDTYPE, MOVREC_ENDTYPE_NORMAL, 0, 0); // should be normal end for time lapse recording
    }
    else
    {
        MovRec_ChangeParameter(MOVREC_RECPARAM_TIMELAPSE, FALSE, 0, 0);
    }
    #if (YOUKU_SDK == ENABLE && CUT_MOVIE_FUNC == ENABLE)
    #if(SAVE_DUAL_STREAM_FUNC ==  ENABLE)
    "CUT_MOVIE_FUNC and SAVE_DUAL_STREAM_FUNC can NOT be ENABLE at the same time!"
    #endif
    MovRec_SetMaxStopFlashSec(15); // 15 seconds
    MovRec_SetEmergencyRecording(1,1);
    #endif
    //---------------------------------------------------------------------------------------------
    // set file type parameters
    //---------------------------------------------------------------------------------------------
    //#NT#2013/04/17#Calvin Chang#Support Rotation information in Mov/Mp4 File format -begin
    // set rotation information
    MovRec_ChangeParameter(MOVREC_RECPARAM_MOV_ROTATE, uiVidRotate, 0, 0);
    //#NT#2013/04/17#Calvin Chang -end

    //---------------------------------------------------------------------------------------------
    // set video code parameters
    //---------------------------------------------------------------------------------------------
    // (uiVidCodec == MEDIAREC_ENC_H264)
    {
        // set H.264 GOP type (MEDIAREC_H264GOP_IPONLY, MEDIAREC_H264GOP_IPB)
        MovRec_ChangeParameter(MOVREC_RECPARAM_H264GOPTYPE, uiH264GopType, 0, 0);
        // set H.264 3DNR level (H264_3DNR_DISABLE, H264_3DNR_WEAKEST, H264_3DNR_WEAK, ...)
        MovRec_ChangeParameter(MOVREC_RECPARAM_H2643DNRLEVEL, ui3DNRLevel, 0, 0);
        //#NT#2016/09/20#Bob Huang -begin
        //#NT#Support HDMI Display with 3DNR Out
        #if (_3DNROUT_FUNC == ENABLE)
        if(gb3DNROut)
        {
            MovRec_ChangeParameter(MOVREC_RECPARAM_H264_3DNROUT, 1, 0, 0);
        }
        else
        {
            MovRec_ChangeParameter(MOVREC_RECPARAM_H264_3DNROUT, 0, 0, 0);
        }
        #endif
        //#NT#2016/09/20#Bob Huang -end
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
        #if(UCTRL_FUNC==ENABLE)
        if(uiFileType == MEDIA_FILEFORMAT_MP4 && uiAudCodec != MOVAUDENC_AAC)
        {
            uiFileType = MEDIA_FILEFORMAT_MOV;
            DBG_ERR("MP4 doesn't support Audio Format(%d), force changing to MOV file\r\n",uiAudCodec);
        }
        #endif
        #if 0
        else if(uiFileType == MEDIA_FILEFORMAT_MOV && uiAudCodec == MOVAUDENC_AAC)
        {
            uiFileType = MEDIA_FILEFORMAT_MP4;
            DBG_ERR("MOV dosen't support Audio Format(%d), force changing to MP4 file\r\n",uiAudCodec);
        }
        #endif
        MovRec_ChangeParameter(MOVREC_RECPARAM_FILETYPE, uiFileType, 0, 0);
        // set video codec (MEDIAREC_ENC_H264, MEDIAREC_ENC_JPG)
        MovRec_ChangeParameter(MOVREC_RECPARAM_VIDEOCODEC, uiVidCodec, 0, 0);
		#if (IPCAM_FUNC)
			//#NT#2017/02/09#Charlie Chang -begin
        	//#NT# support SBS
			SEL_SBS SBS_flag_p1 = IPL_AlgGetUIInfo(0, IPL_SEL_SBS);
			if(SBS_flag_p1 == SEL_SBS_LR){

				if(uiWidth >= 1440 ){
					uiWidth = 2880;
				}
				else{
					uiWidth= uiWidth*2;
				}
			}
			else if(SBS_flag_p1 == SEL_SBS_UD){
				uiHeight= uiHeight*2 ;

			}

            MovRec_ChangeParameter(MOVREC_RECPARAM_WIDTH, uiWidth, 0, 0);
           	MovRec_ChangeParameter(MOVREC_RECPARAM_HEIGHT, uiHeight, 0, 0);

			//#NT#2017/02/09#Charlie Chang -end
		#else
		    MovRec_ChangeParameter(MOVREC_RECPARAM_WIDTH, uiWidth, 0, 0);
           	MovRec_ChangeParameter(MOVREC_RECPARAM_HEIGHT, uiHeight, 0, 0);
		#endif
        // set target data rate (bytes per second)
        MovRec_ChangeParameter(MOVREC_RECPARAM_TARGETRATE, uiTargetBitrate, 0, 0);
        // set target aspect ratio
        MovRec_ChangeParameter(MOVREC_RECPARAM_DAR, uiDispAspectRatio, 0, 0);
#if (IPCAM_FUNC)
        // enable svc
        MovRec_ChangeParameter(MOVREC_RECPARAM_SVC, VIDENC_SVC_GOP15, VIDENC_ID_1, 0);
        // get gop
        MovRec_SetH264GopNumByPath(GetMovieGOP_2p(VIDENC_ID_1), VIDENC_ID_1);
        // set IPL max width, height
        {

            //#NT#2017/02/10#Charlie Chang -begin
            //#NT# support SBS
            USIZE MaxSize1;
			USIZE size_p1;
            if(SBS_flag_p1 == SEL_SBS_LR){
				MaxSize1.h = uiHeight;
				MaxSize1.w = uiWidth;
				size_p1.h=uiHeight;
				size_p1.w=uiWidth;
				ImageUnit_SetParam(&ISF_Media, MEDIA_PARAM_SBS2_TYPE, ISF_MEDIA_SEL_SBS_LR);
				/////P1 and P3 should be the same, because P1 and P3 need side by side
				ImageUnit_SetParam(&ISF_Media, MEDIA_PARAM_SBS2_P1 , (UINT32)&size_p1);
				ImageUnit_SetParam(&ISF_Media, MEDIA_PARAM_SBS2_P3 , (UINT32)&size_p1);

            }
			else if(SBS_flag_p1 == SEL_SBS_UD){
				MaxSize1.h = MOVIE_PATH1_MAX_H*2;
				MaxSize1.w = MOVIE_PATH1_MAX_W;
				size_p1.h=uiHeight;
				size_p1.w=uiWidth;
				ImageUnit_SetParam(&ISF_Media, MEDIA_PARAM_SBS2_TYPE, ISF_MEDIA_SEL_SBS_UD);
				/////P1 and P3 should be the same, because P1 and P3 need side by side
				ImageUnit_SetParam(&ISF_Media, MEDIA_PARAM_SBS2_P1 , (UINT32)&size_p1);
				ImageUnit_SetParam(&ISF_Media, MEDIA_PARAM_SBS2_P3 , (UINT32)&size_p1);

			}
			else{
				MaxSize1.h = MOVIE_PATH1_MAX_H;
				MaxSize1.w = MOVIE_PATH1_MAX_W;

				ImageUnit_SetParam(&ISF_Media, MEDIA_PARAM_SBS2_TYPE, ISF_MEDIA_SEL_SBS_OFF);

			}

			//#NT#2017/02/10#Charlie Chang -end
			ImageUnit_SetParam(&ISF_Media, MEDIA_PARAM_MAXSIZE_P1, (UINT32)&MaxSize1);
        }
        //MovRec_ChangeParameter(MOVREC_RECPARAM_SVC, GetMovieGOP_2p(VIDENC_ID_1), VIDENC_ID_1, 0);
#endif
    }

#if (MOVIE_MULTI_RECORD_FUNC)
    if(GetMovieRecType_2p(uiMovieSize)==MOVIE_REC_TYPE_DUAL || GetMovieRecType_2p(uiMovieSize)==MOVIE_REC_TYPE_CLONE)
    {
        //File1's type must equal to File2's. For if file1's format is MP4+AAC, file2's is MOV, this case is not support in Media Framework.
        uiFileType2         = uiFileType;
        #if (IPCAM_FUNC)
        uiVidCodec2         = GetMovieVidEncFmt_2p(VIDENC_ID_2);
        #else
        uiVidCodec2         = MEDIAVIDENC_H264;
        #endif
        uiWidth2            = GetMovieSizeWidth_2p(1, uiMovieSize);         // image width
        uiHeight2           = GetMovieSizeHeight_2p(1, uiMovieSize);        // image height
        uiTargetBitrate2    = GetMovieTargetBitrate_2p(1, uiMovieSize);     // enable rate control and set target rate
        uiDispAspectRatio2  = GetMovieDispAspectRatio_2p(1, uiMovieSize);   // display aspect ratio
        uiVidFrameRate2     = GetMovieFrameRate_2p(1, uiMovieSize);
        {
            //#NT#2016/03/04#Lincy Lin -begin
            //#NT#Porting media 4.0 flow
            #if (MEDIA_4_0)
            MOVREC_ADD_INFO      minfo={0};

            minfo.width = uiWidth2;
            minfo.height = uiHeight2;
            minfo.codectype[0] = MEDIAVIDENC_H264;
            minfo.codectype[1] = MEDIAVIDENC_MJPG;
            minfo.targetbitrate = uiTargetBitrate2;
            minfo.vidframerate = uiVidFrameRate2;
            minfo.writeCard = 0;
            minfo.pureStreaming = 1;
            minfo.maxtbr = MOVIE_PATH2_MAX_TBR;
            minfo.maxwidth = MOVIE_PATH2_MAX_W;
            minfo.maxheight = MOVIE_PATH2_MAX_H;
            MovRec_AddPureStreaming(&minfo);
            DBGD(minfo.vsid);
            #endif
            //#NT#2016/03/04#Lincy Lin -end
        }
        //select parameter for input path 1
        {
            // set file type (MEDIAREC_AVI, MEDIAREC_MOV, MEDIAREC_MP4)
            MovRec_ChangeParameter(MOVREC_RECPARAM_FILETYPE_2, uiFileType2, 0, 0);
            // set video codec (MEDIAREC_ENC_H264, MEDIAREC_ENC_JPG)
            MovRec_ChangeParameter(MOVREC_RECPARAM_VIDEOCODEC, uiVidCodec2, 1, 0);
			#if (IPCAM_FUNC)
				//#NT#2017/02/13#Charlie Chang -begin
            	//#NT# support SBS
				SEL_SBS SBS_flag_p2 = IPL_AlgGetUIInfo(0, IPL_SEL_SBS);
			    USIZE MaxSize2;
				USIZE size_p2;
				if(SBS_flag_p2 == SEL_SBS_LR){
					MaxSize2.h = MOVIE_PATH2_MAX_H;
					MaxSize2.w = (MOVIE_PATH2_MAX_W*2 > 2880)? 2880 : MOVIE_PATH2_MAX_W*2;

					if(uiWidth2 >=1440){
						uiWidth2 = 2880;
					}
					else{
						uiWidth2 = uiWidth2*2;
					}

				}
				else if(SBS_flag_p2 == SEL_SBS_UD){
					MaxSize2.h = MOVIE_PATH2_MAX_H*2;
					MaxSize2.w = MOVIE_PATH2_MAX_W;
					uiHeight2 = uiHeight2*2;
				}
				else{
					MaxSize2.h = MOVIE_PATH2_MAX_H;
					MaxSize2.w = MOVIE_PATH2_MAX_W;
				}
				size_p2.h=uiHeight2;
				size_p2.w=uiWidth2;
				/////P2 and P4 should be the same, because P2 and P4 need side by side
				ImageUnit_SetParam(&ISF_Media, MEDIA_PARAM_SBS2_P2 , (UINT32)&size_p2);
				ImageUnit_SetParam(&ISF_Media, MEDIA_PARAM_SBS2_P4 , (UINT32)&size_p2);
				MovRec_ChangeParameter(MOVREC_RECPARAM_WIDTH, size_p2.w, 1, 0);
            	MovRec_ChangeParameter(MOVREC_RECPARAM_HEIGHT, size_p2.h, 1, 0);
				 // set IPL max width, height
				ImageUnit_SetParam(&ISF_Media, MEDIA_PARAM_MAXSIZE_P2, (UINT32)&MaxSize2);
				//#NT#2017/02/13#Charlie Chang -end
			#else
            	// set image width
            	MovRec_ChangeParameter(MOVREC_RECPARAM_WIDTH, uiWidth2, 1, 0);
            	// set image height
            	MovRec_ChangeParameter(MOVREC_RECPARAM_HEIGHT, uiHeight2, 1, 0);
			#endif

            // set target data rate (bytes per second)
            MovRec_ChangeParameter(MOVREC_RECPARAM_TARGETRATE, uiTargetBitrate2, 1, 0);
            // set target aspect ratio
            MovRec_ChangeParameter(MOVREC_RECPARAM_DAR, uiDispAspectRatio2, 1, 0);
            // set video frame rate
            MovRec_ChangeParameter(MOVREC_RECPARAM_FRAMERATE, uiVidFrameRate2, 1, 0);
            //#NT#2016/03/21#Niven Cho -begin
            //#NT#Enable SVC to drop SVC low priority frame whiling cpu2 is too busy.
            if(GetMovieRecType_2p(uiMovieSize)==MOVIE_REC_TYPE_CLONE)
            {
                MovRec_ChangeParameter(MOVREC_RECPARAM_SVC, VIDENC_SVC_GOP15, VIDENC_ID_2, 0);
            }
            //#NT#2016/03/21#Niven Cho -end
#if (IPCAM_FUNC)
            // enable svc
            MovRec_ChangeParameter(MOVREC_RECPARAM_SVC, VIDENC_SVC_GOP15, VIDENC_ID_2, 0);
            // set gop
            //MovRec_ChangeParameter(MOVREC_RECPARAM_H264GOPNUM, GetMovieGOP_2p(VIDENC_ID_2), VIDENC_ID_2, 0);
            MovRec_SetH264GopNumByPath(GetMovieGOP_2p(VIDENC_ID_2), VIDENC_ID_2);

#endif
        }
    }
#else

#endif

#if (IPCAM_FUNC && MEDIA_VASTREAM && (TWO_STREAM_MODE!=ENABLE))
    {
        UINT32  uipath=VIDENC_ID_3;
        UINT32  uiVidCodec3;
        UINT32  uiFileType3=0, uiWidth3=0, uiHeight3=0, uiTargetBitrate3=0, uiDispAspectRatio3=0, uiVidFrameRate3=0;

        //File1's type must equal to File2's. For if file1's format is MP4+AAC, file2's is MOV, this case is not support in Media Framework.
        uiFileType3         = uiFileType;
        uiVidCodec3         = GetMovieVidEncFmt_2p(uipath);
        uiWidth3            = GetMovieSizeWidth_2p(uipath, uiMovieSize);         // image width
        uiHeight3           = GetMovieSizeHeight_2p(uipath, uiMovieSize);        // image height
        uiTargetBitrate3    = GetMovieTargetBitrate_2p(uipath, uiMovieSize);     // enable rate control and set target rate
        uiDispAspectRatio3  = GetMovieDispAspectRatio_2p(uipath, uiMovieSize);   // display aspect ratio
        uiVidFrameRate3     = GetMovieFrameRate_2p(uipath, uiMovieSize);


        //#NT#2016/03/04#Lincy Lin -begin
        //#NT#Porting media 4.0 flow
        #if (MEDIA_4_0)
        MOVREC_ADD_INFO      minfo={0};

        minfo.width = uiWidth3;
        minfo.height = uiHeight3;
        minfo.codectype[0] = MEDIAVIDENC_H264;
        minfo.codectype[1] = MEDIAVIDENC_MJPG;
        minfo.targetbitrate = uiTargetBitrate3;
        minfo.vidframerate = uiVidFrameRate3;
        minfo.writeCard = 0;
        minfo.pureStreaming = 1;
        minfo.maxtbr = MOVIE_PATH3_MAX_TBR;
        minfo.maxwidth = MOVIE_PATH3_MAX_W;
        minfo.maxheight = MOVIE_PATH3_MAX_H;
        MovRec_AddPureStreaming(&minfo);
        DBGD(minfo.vsid);
        #endif
        //#NT#2016/03/04#Lincy Lin -end

        //select parameter for input path 1
        // set file type (MEDIAREC_AVI, MEDIAREC_MOV, MEDIAREC_MP4)
        //MovRec_ChangeParameter(MOVREC_RECPARAM_FILETYPE_2, uiFileType2, 0, 0);
        // set video codec (MEDIAREC_ENC_H264, MEDIAREC_ENC_JPG)
        MovRec_ChangeParameter(MOVREC_RECPARAM_VIDEOCODEC, uiVidCodec3, uipath, 0);
        #if 0
        // set image width
        MovRec_ChangeParameter(MOVREC_RECPARAM_WIDTH, uiWidth3, uipath, 0);
        // set image height
        MovRec_ChangeParameter(MOVREC_RECPARAM_HEIGHT, uiHeight3, uipath, 0);
        #else
        {
          	// URECT CropWin;
			USIZE MaxSize3;
			MaxSize3.h = CROP_SOURCE_H;
			MaxSize3.w = CROP_SOURCE_W;
            ImageUnit_SetParam(&ISF_Media, MEDIA_PARAM_MAXSIZE_P3, (UINT32)&MaxSize3);
            MovRec_ChangeParameter(MOVREC_RECPARAM_WIDTH, uiWidth3, uipath, 0);
            MovRec_ChangeParameter(MOVREC_RECPARAM_HEIGHT, uiHeight3, uipath, 0);
            //#NT#2016/05/31#Charlie Chang -begin
            //#NT# refine code for crop issue when change tv out
            ImageApp_IPCam_CfgWindow(IPCAM_PATH_ID_3);
           // CropWin.x = 0;
          //  CropWin.y = 0;
          //  CropWin.w = uiWidth3;
          //  CropWin.h = uiHeight3;
           // ImageUnit_SetParam(&ISF_Media, MEDIA_PARAM_CROP_P3, (UINT32)&CropWin); //enable window
           //#NT#2016/05/31#Charlie Chang -end
        }
        #endif


        // set target data rate (bytes per second)
        MovRec_ChangeParameter(MOVREC_RECPARAM_TARGETRATE, uiTargetBitrate3, uipath, 0);
        // set target aspect ratio
        MovRec_ChangeParameter(MOVREC_RECPARAM_DAR, uiDispAspectRatio3, uipath, 0);
        // set video frame rate
        MovRec_ChangeParameter(MOVREC_RECPARAM_FRAMERATE, uiVidFrameRate3, uipath, 0);
        // enable svc
        MovRec_ChangeParameter(MOVREC_RECPARAM_SVC, VIDENC_SVC_GOP15, uipath, 0);
        // set gop
        MovRec_SetH264GopNumByPath(GetMovieGOP_2p(uipath), uipath);

        //#NT#2016/03/04#Lincy Lin -begin
        //#NT#Porting media 4.0 flow
        #if (!MEDIA_4_0)
        // set maximum width , height,  TBR
        uipath = VIDENC_ID_1;
        //#NT#2017/02/09#Charlie Chang -begin
        //#NT# support SBS
		SEL_SBS SBS_flag_max = IPL_AlgGetUIInfo(0, IPL_SEL_SBS);
		if(SBS_flag_max == SEL_SBS_OFF){
			MovRec_ChangeParameter(MOVREC_RECPARAM_MAX_WIDTH,  MOVIE_PATH1_MAX_W  ,uipath, 0);
			MovRec_ChangeParameter(MOVREC_RECPARAM_MAX_HEIGHT, MOVIE_PATH1_MAX_H  ,uipath, 0);

		}
		else if(SBS_flag_max == SEL_SBS_LR){

			MovRec_ChangeParameter(MOVREC_RECPARAM_MAX_WIDTH,  ((MOVIE_PATH1_MAX_W*2) > 2880)? 2880:(MOVIE_PATH1_MAX_W*2)  ,uipath, 0);
			MovRec_ChangeParameter(MOVREC_RECPARAM_MAX_HEIGHT, MOVIE_PATH1_MAX_H  ,uipath, 0);

		}
		else if(SBS_flag_max == SEL_SBS_UD){
			MovRec_ChangeParameter(MOVREC_RECPARAM_MAX_WIDTH,  MOVIE_PATH1_MAX_W  ,uipath, 0);
			MovRec_ChangeParameter(MOVREC_RECPARAM_MAX_HEIGHT, MOVIE_PATH1_MAX_H*2  ,uipath, 0);
		}
		else{
			DBG_ERR("get SBS_flag_max =%d error!\r\n",SBS_flag_max);
		}
		//#NT#2017/02/09#Charlie Chang -end
        MovRec_ChangeParameter(MOVREC_RECPARAM_MAX_TBR,    MOVIE_PATH1_MAX_TBR  ,uipath, 0);
        uipath = VIDENC_ID_2;
        //#NT#2017/02/09#Charlie Chang -begin
        //#NT# support SBS
		SEL_SBS SBS_flag_max2 = IPL_AlgGetUIInfo(0, IPL_SEL_SBS);
		if(SBS_flag_max2 == SEL_SBS_OFF){
			MovRec_ChangeParameter(MOVREC_RECPARAM_MAX_WIDTH,  MOVIE_PATH2_MAX_W  ,uipath, 0);
			MovRec_ChangeParameter(MOVREC_RECPARAM_MAX_HEIGHT, MOVIE_PATH2_MAX_H  ,uipath, 0);

		}
		else if(SBS_flag_max2 == SEL_SBS_LR){

			MovRec_ChangeParameter(MOVREC_RECPARAM_MAX_WIDTH,  ((MOVIE_PATH2_MAX_W*2) > 2880)? 2880:(MOVIE_PATH2_MAX_W*2)  ,uipath, 0);
			MovRec_ChangeParameter(MOVREC_RECPARAM_MAX_HEIGHT, MOVIE_PATH2_MAX_H  ,uipath, 0);

		}
		else if(SBS_flag_max2 == SEL_SBS_UD){
			MovRec_ChangeParameter(MOVREC_RECPARAM_MAX_WIDTH,  MOVIE_PATH2_MAX_W  ,uipath, 0);
			MovRec_ChangeParameter(MOVREC_RECPARAM_MAX_HEIGHT, MOVIE_PATH2_MAX_H*2  ,uipath, 0);
		}
		else{
			DBG_ERR("get SBS_flag_max2 =%d error!\r\n",SBS_flag_max2);
		}
		//#NT#2017/02/09#Charlie Chang -end
        MovRec_ChangeParameter(MOVREC_RECPARAM_MAX_TBR,    MOVIE_PATH2_MAX_TBR  ,uipath, 0);

        uipath = VIDENC_ID_3;
        MovRec_ChangeParameter(MOVREC_RECPARAM_MAX_WIDTH,  MOVIE_PATH3_MAX_W  ,uipath, 0);
        MovRec_ChangeParameter(MOVREC_RECPARAM_MAX_HEIGHT, MOVIE_PATH3_MAX_H  ,uipath, 0);
        MovRec_ChangeParameter(MOVREC_RECPARAM_MAX_TBR,    MOVIE_PATH3_MAX_TBR  ,uipath, 0);
        #endif
        //#NT#2016/03/04#Lincy Lin -end

        //#NT#2016/03/11#Lincy Lin -begin
        //#NT#Adjust media used memory in IPCAM
        //#NT#2016/07/07#David Tsai -begin
        //#NT#Two stream mode for IP Cam
        #if TWO_STREAM_MODE
        MovRec_SetvidBs_RevSec(3);
        MovRec_SetvidBs_NumInBs(2);
        #else
        MovRec_SetvidBs_RevSec(3);
        MovRec_SetvidBs_NumInBs(3);
        #endif
        //#NT#2016/07/07#David Tsai -end
        //#NT#2016/03/11#Lincy Lin -end
    }

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

//#NT#2016/08/29#HM Tseng -begin
//#NT#For audio play testing mode
#if (AUDIO_PLAY_FUNCTION == ENABLE && AUDIO_PLAY_TEST == DISABLE)
    System_SetAudioParam(0, uiAudSampleRate, Movie_AudioChTable[uiAudChannelType], 16);
#endif
//#NT#2016/08/29#HM Tseng -end

    Movie_SetQPInitLevel();
    Movie_SetUserData();

    //#NT#2016/06/30#Isiah Chang -begin
    // Enable 32MB alignment recording.
    //MovRec_Enable_FileSizeAlign(0x2000000);
    //#NT#2016/06/30#Isiah Chang -begin

    #if (MOVIE_TEST_ENABLE == ENABLE)
    {// test codes for fixed YUV patterns mode (no IPL)!!!
        if (g_MediaRecTestMode.bEnable && g_MediaRecTestMode.uiVidSrcType == MOVREC_SRC_FIXED_YUV)
        {
            // set YUV information if using fixed YUV as video source
            Movie_SetYUV(uiMovieSize);
            MovRec_SetTestMode(&g_MediaRecTestMode);

            // register callback
            MovRec_SetIPLChangeCB(MovieExe_FakeIPLChangeCB);
        }
    }
    #endif

//#NT#2016/06/20#KCHong -begin
//#NT#New ADAS, use g_ADASFuncSupported instead of ImageUnit_SetParam(&ISF_ADAS, ADAS_PARAM_ENABLE, TRUE/FALSE)
//#NT#2016/06/20#KCHong -end
#if 1//(_MODEL_DSC_ == _MODEL_PWA6_ || _MODEL_DSC_ == _MODEL_FHD6120_|| _MODEL_DSC_ == _MODEL_D6_|| _MODEL_DSC_ == _MODEL_PTL_668_)
    SMediaRec_SetEncPath(0, VIDENC_PATH_D2D); //Default is set D2D
    SMediaRec_SetEncPath(1, VIDENC_PATH_D2D); //Default is set D2D
#if (_ADAS_FUNC_ == ENABLE)
    g_ADASFuncSupported = FALSE;
    switch (UI_GetData(FL_MOVIE_SIZE_MENU))
    {
    case MOVIE_SIZE_FRONT_2880x2160P24:
    case MOVIE_SIZE_FRONT_2560x1440P30:
    case MOVIE_SIZE_FRONT_2304x1296P30:
        ImageUnit_SetParam(&ISF_ImagePipe, IMAGEPIPE_PARAM_AUTOIPLOFF_EN, 1);
        break;

    case MOVIE_SIZE_FRONT_1920x1080P60:
    case MOVIE_SIZE_FRONT_1280x720P120:
    case MOVIE_SIZE_FRONT_1280x720P60:
    case MOVIE_SIZE_FRONT_640x480P240:
        ImageUnit_SetParam(&ISF_ImagePipe, IMAGEPIPE_PARAM_AUTOIPLOFF_EN, 1);
        break;

    case MOVIE_SIZE_DUAL_1920x1080P30_1920x1080P30:
        ImageUnit_SetParam(&ISF_ImagePipe, IMAGEPIPE_PARAM_AUTOIPLOFF_EN, 0);
        break;

    default:
            ImageUnit_SetParam(&ISF_ImagePipe, IMAGEPIPE_PARAM_AUTOIPLOFF_EN, 0);
        break;
    }

    /*----- ADAS enable or not for movie size -----*/
    if ((UI_GetData(FL_MOVIE_FCW) == MOVIE_FCW_ON) || (UI_GetData(FL_MOVIE_LDWS) == MOVIE_LDWS_ON))
    {
        debug_msg("enter switch..\r\n");
        switch (UI_GetData(FL_MOVIE_SIZE_MENU))
        {
            case MOVIE_SIZE_DUAL_1920x1080P30_1920x1080P30:
            case MOVIE_SIZE_FRONT_1920x1080P30:
                g_ADASFuncSupported = TRUE;
                debug_msg("set g_ADASFuncSupported = TRUE..\r\n");
                break;
            default:
                g_ADASFuncSupported = FALSE;
                break;
        }
    }
#endif
#else
    SMediaRec_SetEncPath(0, VIDENC_PATH_D2D); //Default is set D2D
    SMediaRec_SetEncPath(1, VIDENC_PATH_D2D); //Default is set D2D
#if (_ADAS_FUNC_ == ENABLE || (_CPU2_TYPE_==_CPU2_LINUX_ && IPCAM_FUNC==DISABLE))
    g_ADASFuncSupported = FALSE;
    switch (UI_GetData(FL_MOVIE_SIZE_MENU))
    {
        case MOVIE_SIZE_FRONT_2880x2160P24:                                             // ADAS is NOT supported in these modes.
        case MOVIE_SIZE_FRONT_2560x1440P30:
        case MOVIE_SIZE_FRONT_2304x1296P30:
        case MOVIE_SIZE_FRONT_1920x1080P60:
        case MOVIE_SIZE_FRONT_1280x720P120:
        case MOVIE_SIZE_FRONT_1280x720P60:
        case MOVIE_SIZE_FRONT_640x480P240:
            g_ADASFuncSupported = FALSE;
            //#NT#2016/08/18#Isiah Chang -begin
            // Turn off the 2nd IPL bandwidth save function if necessary.
            ImageUnit_SetParam(&ISF_ImagePipe, IMAGEPIPE_PARAM_AUTOIPLOFF_EN, 1);
            //#NT#2016/08/18#Isiah Chang -end
        #if defined(YQCONFIG_ANDROID_SYSTEM_SUPPORT)
        if (UI_GetData(FL_MOVIE_ADAS_CAL) == MOVIE_ADAS_CAL_ON)
        #else
            if ((UI_GetData(FL_MOVIE_FCW) == MOVIE_FCW_ON) || (UI_GetData(FL_MOVIE_LDWS) == MOVIE_LDWS_ON))
        #endif
            {
                DBG_WRN("ADAS is not supported in this mode!\r\n");
            }
            break;

        case MOVIE_SIZE_FRONT_1920x1080P30:                                             // ADAS is supported in these modes.
        case MOVIE_SIZE_DUAL_1920x1080P30_1920x1080P30:
            #if (_ADAS_FUNC_ == ENABLE)
        #if defined(YQCONFIG_ANDROID_SYSTEM_SUPPORT)
        if (UI_GetData(FL_MOVIE_ADAS_CAL) == MOVIE_ADAS_CAL_ON)
        #else
            if ((UI_GetData(FL_MOVIE_FCW) == MOVIE_FCW_ON) || (UI_GetData(FL_MOVIE_LDWS) == MOVIE_LDWS_ON))
        #endif
            {
                g_ADASFuncSupported = TRUE;
            }
            else
            {
                g_ADASFuncSupported = FALSE;
            }
            #endif
            //#NT#2016/08/18#Isiah Chang -begin
            // Turn off the 2nd IPL bandwidth save function if necessary.
            ImageUnit_SetParam(&ISF_ImagePipe, IMAGEPIPE_PARAM_AUTOIPLOFF_EN, 0);
            //#NT#2016/08/18#Isiah Chang -end
            break;

        default:
            g_ADASFuncSupported = FALSE;
            //#NT#2016/08/18#Isiah Chang -begin
            // Turn off the 2nd IPL bandwidth save function if necessary.
            ImageUnit_SetParam(&ISF_ImagePipe, IMAGEPIPE_PARAM_AUTOIPLOFF_EN, 0);
            //#NT#2016/08/18#Isiah Chang -end
            break;
    }

    if(SysGetFlag(FL_MOVIE_TIMELAPSE_REC) != MOVIE_TIMELAPSEREC_OFF)                    // ADAS is NOT supported in timelapse mode.
    {
        g_ADASFuncSupported = FALSE;
        #if defined(YQCONFIG_ANDROID_SYSTEM_SUPPORT)
        if (UI_GetData(FL_MOVIE_ADAS_CAL) == MOVIE_ADAS_CAL_ON)
        #else
        if ((UI_GetData(FL_MOVIE_FCW) == MOVIE_FCW_ON) || (UI_GetData(FL_MOVIE_LDWS) == MOVIE_LDWS_ON))
        #endif
        {
            DBG_WRN("ADAS is not supported in timelapse mode!\r\n");
        }
    }
#endif  // #if (_ADAS_FUNC_ == ENABLE)

#endif

    // ADAS configurations should be set for all MODELs.
    UINT32 CamMovieSFuncCfg = CAMMOVIE_SFUNC_NONE;
    UINT32 NetMovieSFuncCfg = NETMOVIE_SFUNC_NONE;
//#NT#2017/01/23#Ben Wang -begin
//#NT#Support ADAS in UVC_MULTIMEDIA_FUNC, referring to UsbMovieSFuncCfg.
	#if(UVC_MULTIMEDIA_FUNC == ENABLE)
    UINT32 UsbMovieSFuncCfg = NVT_USBMOVIE_SFUNC_NONE;
    #endif
    #if (_ADAS_FUNC_)
    // Use ImageUnit_ALG
    if (g_ADASFuncSupported == TRUE)
    {
        // Always reset IPL1 Path4 crop setting before recording.
        ISF_IMG_INFO IsfImgSetting = {0};
        IsfImgSetting.ImgFmt = GX_IMAGE_PIXEL_FMT_Y_ONLY;
        IsfImgSetting.ImgSize.w = 1920;
        IsfImgSetting.ImgSize.h = 1080;
        IsfImgSetting.Window.w = 720;
        IsfImgSetting.Window.h = 480;
        IsfImgSetting.Window.x  = 600;
        IsfImgSetting.Window.y  = 300;
        ImageUnit_SetParam(&ISF_ALG, ALG_PARAM_SET_IN2, (UINT32)&IsfImgSetting);

        CamMovieSFuncCfg |= (CAMMOVIE_SFUNC_IPL1_PATH3 | CAMMOVIE_SFUNC_IPL1_PATH4);
        NetMovieSFuncCfg |= (NETMOVIE_SFUNC_IPL1_PATH3 | NETMOVIE_SFUNC_IPL1_PATH4);
        #if(UVC_MULTIMEDIA_FUNC == ENABLE)
        UsbMovieSFuncCfg |= (NVT_USBMOVIE_SFUNC_IPL1_PATH3 | NVT_USBMOVIE_SFUNC_IPL1_PATH4);
        #endif
    }
    #endif

    //#NT#2016/05/20#Yang Jin -begin
    //#NT#Support Dis dsp function
    #if (MOVIE_OT_FUNC||MOVIE_DISDSP_FUNC)
    // Use ImageUnit_ALG
    CamMovieSFuncCfg |= CAMMOVIE_SFUNC_IPL1_PATH3;
    NetMovieSFuncCfg |= NETMOVIE_SFUNC_IPL1_PATH3;
    #if(UVC_MULTIMEDIA_FUNC == ENABLE)
    UsbMovieSFuncCfg |= NVT_USBMOVIE_SFUNC_IPL1_PATH3;
    #endif
    #endif
    //#NT#2016/05/20#Yang Jin -end

    //#NT#2016/07/06#CC Chao -begin
    #if (MOVIE_TSR_FUNC_)
    // Use ImageUnit_ALG
    CamMovieSFuncCfg |= CAMMOVIE_SFUNC_IPL1_PATH3;
    NetMovieSFuncCfg |= NETMOVIE_SFUNC_IPL1_PATH3;
    #if(UVC_MULTIMEDIA_FUNC == ENABLE)
    UsbMovieSFuncCfg |= NVT_USBMOVIE_SFUNC_IPL1_PATH3;
    #endif
    #endif
    //#NT#2016/07/06#CC Chao -end

    //#NT#2016/10/17#Bin Xiao -begin
    //#NT# Support Face Tracking Grading(FTG)
    #if (MOVIE_FTG_FUNC)
    // Use ImageUnit_ALG
    CamMovieSFuncCfg |= CAMMOVIE_SFUNC_IPL1_PATH3;
    NetMovieSFuncCfg |= NETMOVIE_SFUNC_IPL1_PATH3;
    #if(UVC_MULTIMEDIA_FUNC == ENABLE)
    UsbMovieSFuncCfg |= NVT_USBMOVIE_SFUNC_IPL1_PATH3;
    #endif
    #endif
    //#NT#2016/10/17#Bin Xiao -end


    #if (MOVIE_DDD_FD_FUNC)
    CamMovieSFuncCfg |= CAMMOVIE_SFUNC_IPL2_PATH4;
    NetMovieSFuncCfg |= NETMOVIE_SFUNC_IPL2_PATH4;
    #if(UVC_MULTIMEDIA_FUNC == ENABLE)
    UsbMovieSFuncCfg |= NVT_USBMOVIE_SFUNC_IPL2_PATH4;
    #endif
    #endif


    #if (MOVIE_DDD_FUNC)
    // Use ImageUnit_ALG
    if(nSensor==1)
    {
        CamMovieSFuncCfg |= CAMMOVIE_SFUNC_IPL1_PATH4 | CAMMOVIE_SFUNC_IPL1_PATH4_PVEN;
        NetMovieSFuncCfg |= NETMOVIE_SFUNC_IPL1_PATH4 | NETMOVIE_SFUNC_IPL1_PATH4_PVEN;
        #if(UVC_MULTIMEDIA_FUNC == ENABLE)
        UsbMovieSFuncCfg |= NVT_USBMOVIE_SFUNC_IPL1_PATH4 | NVT_USBMOVIE_SFUNC_IPL1_PATH4_PVEN;
        #endif
    }
    else
    {
        CamMovieSFuncCfg |= CAMMOVIE_SFUNC_IPL2_PATH4 | CAMMOVIE_SFUNC_IPL2_PATH4_PVEN;
        NetMovieSFuncCfg |= NETMOVIE_SFUNC_IPL2_PATH4 | NETMOVIE_SFUNC_IPL2_PATH4_PVEN;
        #if(UVC_MULTIMEDIA_FUNC == ENABLE)
        UsbMovieSFuncCfg |= NVT_USBMOVIE_SFUNC_IPL2_PATH4 | NVT_USBMOVIE_SFUNC_IPL2_PATH4_PVEN;
        #endif
    }
    #endif
    ImageApp_CamMovie_Config(MOVIE_CFG_SFUNC_EN, CamMovieSFuncCfg);
    ImageApp_NetMovie_Config(NETMOVIE_CFG_SFUNC_EN, NetMovieSFuncCfg);
    #if(UVC_MULTIMEDIA_FUNC == ENABLE)
    ImageApp_UsbMovie_Config(NVT_USBMOVIE_CFG_SFUNC_EN, UsbMovieSFuncCfg);
    #endif
//#NT#2017/01/23#Ben Wang -end
    //---------------------------------------------------------------------------------------------
    // setup movie date stamp if necessary
    //---------------------------------------------------------------------------------------------
    if (UI_GetData(FL_MOVIE_DATEIMPRINT) == MOVIE_DATEIMPRINT_ON)
    {
        //#NT#2016/07/22#YongChang Qui -begin
        //#NT#MovieStamp uses unified memory as IPC & MovieStamp buffer for IPCam with AE_LOG off
        STAMP_COLOR StampColorBg = {RGB_GET_Y( 16,  16,  16), RGB_GET_U( 16,  16,  16), RGB_GET_V( 16,  16,  16)}; // date stamp background color
        STAMP_COLOR StampColorFr = {RGB_GET_Y( 16,  16,  16), RGB_GET_U( 16,  16,  16), RGB_GET_V( 16,  16,  16)}; // date stamp frame color
        STAMP_COLOR StampColorFg = {RGB_GET_Y(224, 224, 192), RGB_GET_U(224, 224, 192), RGB_GET_V(224, 224, 192)}; // date stamp foreground color

        #if !(IPCAM_FUNC && !MOVIE_AE_LOG)
        UINT32      uiStampAddr;
        // use POOL_ID_DATEIMPRINT as movie data stamp buffer
        uiStampAddr = OS_GetMempoolAddr(POOL_ID_DATEIMPRINT);
        MovieStamp_SetBuffer(0, uiStampAddr, POOL_SIZE_DATEIMPRINT/2);
        //MovieStamp_SetDataAddr(0, uiStampAddr);
        #endif
        //#NT#2016/07/22#YongChang Qui -end

        MovieStamp_SetColor(0, &StampColorBg, &StampColorFr, &StampColorFg);

        //MovieStamp_Setup(
        //    0,
        //    STAMP_ON |
        //    STAMP_AUTO |
        //    STAMP_DATE_TIME |
        //    STAMP_BOTTOM_RIGHT |
        //    STAMP_POS_NORMAL |
        //    STAMP_BG_TRANSPARENT |
        //    STAMP_YY_MM_DD,
        //    uiWidth,
        //    uiHeight);

        MovieStamp_Setup(
            0,
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

#if (MOVIE_MULTI_RECORD_FUNC)
        if(GetMovieRecType_2p(uiMovieSize)==MOVIE_REC_TYPE_DUAL || GetMovieRecType_2p(uiMovieSize)==MOVIE_REC_TYPE_CLONE)
        {
            //#NT#2016/07/22#YongChang Qui -begin
            //#NT#MovieStamp uses unified memory as IPC & MovieStamp buffer for IPCam with AE_LOG off
            #if !(IPCAM_FUNC && !MOVIE_AE_LOG)
            MovieStamp_SetBuffer(1, uiStampAddr + POOL_SIZE_DATEIMPRINT/2, POOL_SIZE_DATEIMPRINT/4);
            #endif
            //#NT#2016/07/22#YongChang Qui -end

            MovieStamp_SetColor(1, &StampColorBg, &StampColorFr, &StampColorFg);

            #if 0 // Media don't output stream with datestamp to HTTP.
            //#NT#2016/03/16#Lincy Lin -begin
            //#NT#Support HTTP liveview datastamp function
            //#NT#2016/05/06#Jeah Yen -begin
            //#NT#Support IOTP2P
            if((ImageApp_NetMovie_GetConfig(NETMOVIE_CFG_MODE) == NETMOVIE_TYPE_HTTP)
            || (ImageApp_NetMovie_GetConfig(NETMOVIE_CFG_MODE) == NETMOVIE_TYPE_RTSP_MJPG)
            || (ImageApp_NetMovie_GetConfig(NETMOVIE_CFG_MODE) == NETMOVIE_TYPE_IOTP2P_MJPG))
            //#NT#2016/05/06#Jeah Yen -end
            {
                USIZE   HttpLviewSize;
                UINT32  ImageRatioIdx;

                ImageRatioIdx = GetMovieSizeRatio(UI_GetData(FL_MOVIE_SIZE));
                HttpLviewSize = MOVIE_HTTPLVIEW_SIZE[ImageRatioIdx];
                uiWidth2 = HttpLviewSize.w;
                uiHeight2 = HttpLviewSize.h;
            }
            //#NT#2016/03/16#Lincy Lin -end
            #endif

            MovieStamp_Setup(
                1,
                STAMP_ON |
                STAMP_AUTO |
                STAMP_DATE_TIME |
                STAMP_BOTTOM_RIGHT |
                STAMP_POS_NORMAL |
                STAMP_BG_TRANSPARENT |
                STAMP_YY_MM_DD |
                STAMP_IMG_420UV,
                uiWidth2,
                uiHeight2,
#if defined (WATERLOGO_FUNCTION) && (WATERLOGO_FUNCTION == ENABLE)
                (WATERLOGO_BUFFER *)&g_WaterLogo);
#else
                NULL);
#endif
        }
#endif

#if (IPCAM_FUNC && MEDIA_VASTREAM)
        {
            UINT32 uipath,uiWidth3,uiHeight3;
            uipath              = 2;
            uiWidth3            = GetMovieSizeWidth_2p(uipath, 0);         // image width
            uiHeight3           = GetMovieSizeHeight_2p(uipath, 0);        // image height
            //#NT#2016/07/22#YongChang Qui -begin
            //#NT#MovieStamp uses unified memory as IPC & MovieStamp buffer for IPCam with AE_LOG off
            #if !(IPCAM_FUNC && !MOVIE_AE_LOG)
            MovieStamp_SetBuffer(uipath, uiStampAddr + POOL_SIZE_DATEIMPRINT*3/4, POOL_SIZE_DATEIMPRINT/4);
            #endif
            //#NT#2016/07/22#YongChang Qui -end

            MovieStamp_SetColor(uipath, &StampColorBg, &StampColorFr, &StampColorFg);

            MovieStamp_Setup(
                uipath,
                STAMP_ON |
                STAMP_AUTO |
                STAMP_DATE_TIME |
                STAMP_BOTTOM_RIGHT |
                STAMP_POS_NORMAL |
                STAMP_BG_TRANSPARENT |
                STAMP_YY_MM_DD |
                STAMP_IMG_420UV,
                uiWidth3,
                uiHeight3,
#if defined (WATERLOGO_FUNCTION) && (WATERLOGO_FUNCTION == ENABLE)
                (WATERLOGO_BUFFER *)&g_WaterLogo);
#else
                NULL);
#endif
        }
#endif

        // enable movie stamp
        MovieStamp_Enable();
    }
    else
    {
        MovieStamp_Setup(
            0,
            STAMP_OFF,
            uiWidth,
            uiHeight,
            NULL);

        //MovieStamp_Setup(
        //    0,
        //    STAMP_OFF,
        //    uiWidth,
        //    uiHeight);

#if (MOVIE_MULTI_RECORD_FUNC)

        if(GetMovieRecType_2p(uiMovieSize)==MOVIE_REC_TYPE_DUAL || GetMovieRecType_2p(uiMovieSize)==MOVIE_REC_TYPE_CLONE)
        {
            MovieStamp_Setup(
                1,
                STAMP_OFF,
                uiWidth2,
                uiHeight2,
                NULL);
        }
#endif

#if (IPCAM_FUNC && MEDIA_VASTREAM)
    MovieStamp_Setup(
                2,
                STAMP_OFF,
                0,
                0,
                NULL);
#endif
    }
}

void MovieExe_CalcDispSize(ISIZE *pDispSize, ISIZE *pDevSize, ISIZE *pDevRatio, ISIZE *pImgRatio)
{
    if (((pDevRatio->w * 100) / pDevRatio->h) > ((pImgRatio->w * 100) / pImgRatio->h)) // device aspect ratio > image ratio
    {
        pDispSize->h = pDevSize->h;
        pDispSize->w = ALIGN_ROUND_16((pDevSize->w * pImgRatio->w * pDevRatio->h) / (pImgRatio->h * pDevRatio->w));
    }
    else
    {
        pDispSize->w = pDevSize->w;
        pDispSize->h = ALIGN_ROUND_4((pDevSize->h * pImgRatio->h * pDevRatio->w) / (pImgRatio->w * pDevRatio->h));
    }
}



extern PHOTO_FUNC_INFO PhotoFuncInfo_dummy;
extern PHOTO_FUNC_INFO PhotoFuncInfo_dis;
extern UINT32 System_GetConnectSensor(void);

//#NT#2016/02/24#Hideo Lin -begin
//#NT#Check movie size setting by sensor connection
#if (MOVIE_MULTI_RECORD_FUNC)
extern UINT32 System_GetEnableSensor(void);
void MovieExe_CheckMovieSizeBySensorConnect(void)
{
    static BOOL bFirstTime = TRUE;
    static UINT32 uiPrevSensor = 0; // previous active sensor
    UINT32 uiSensor; // current active sensor
    UINT32 uiMovieSize;

    uiMovieSize = SysGetFlag(FL_MOVIE_SIZE);
    uiSensor = System_GetEnableSensor();

    if (bFirstTime)
    {
        bFirstTime = FALSE;

        if (uiSensor != (SENSOR_1|SENSOR_2))
        {
            if (GetMovieRecType_2p(uiMovieSize) == MOVIE_REC_TYPE_DUAL)
            {
                SysSetFlag(FL_MOVIE_SIZE, DEFAULT_MOVIE_SIZE_SINGLE);
                SysSetFlag(FL_MOVIE_SIZE_MENU, DEFAULT_MOVIE_SIZE_SINGLE);
            }
            //#NT#2016/06/17#Brain Yen -begin
            //#NT#for special aspect panel and Pipview LR
            #if (PIP_VIEW_LR == ENABLE)
                SysSetFlag(FL_DUAL_CAM, DUALCAM_LR_FRONT_FULL);
                PipView_SetStyle(UI_GetData(FL_DUAL_CAM));
            #endif
            //#NT#2016/06/17#Brain Yen -end
        }
        else if (uiSensor == (SENSOR_1|SENSOR_2))
        {
            if (SysGetFlag(FL_MOVIE_SIZE) <= MOVIE_SIZE_FRONT_1920x1080P60)
            {
                // insert sensor, set movie to dual record
                SysSetFlag(FL_MOVIE_SIZE, DEFAULT_MOVIE_SIZE_DUAL);
                SysSetFlag(FL_MOVIE_SIZE_MENU, DEFAULT_MOVIE_SIZE_DUAL);
                //#NT#2016/06/17#Brain Yen -begin
                //#NT#for special aspect panel and Pipview LR
                #if (PIP_VIEW_LR == ENABLE)
					#if (_MODEL_DSC_ == _MODEL_MIO_MT39_)
					SysSetFlag(FL_DUAL_CAM, DUALCAM_LR_FULL);
					#else
                    SysSetFlag(FL_DUAL_CAM, DUALCAM_LR_16_9);
					#endif
                    PipView_SetStyle(UI_GetData(FL_DUAL_CAM));
                #else
                #if (PIP_VIEW_FUNC == ENABLE)
                SysSetFlag(FL_DUAL_CAM_MENU, DUALCAM_BOTH);
                SysSetFlag(FL_DUAL_CAM, DUALCAM_BOTH);
                PipView_SetStyle(UI_GetData(FL_DUAL_CAM));
                #endif
                #endif
                //#NT#2016/06/17#Brain Yen -end
            }
        }
    }
    else
    {
        if (uiPrevSensor != uiSensor)
        {
            if (uiPrevSensor == (SENSOR_1|SENSOR_2))
            {
                // remove sensor, set movie to single record if necessary
                if (GetMovieRecType_2p(uiMovieSize) == MOVIE_REC_TYPE_DUAL)
                {
                    SysSetFlag(FL_MOVIE_SIZE, DEFAULT_MOVIE_SIZE_SINGLE);
                    SysSetFlag(FL_MOVIE_SIZE_MENU, DEFAULT_MOVIE_SIZE_SINGLE);
                }
                //#NT#2016/06/17#Brain Yen -begin
                //#NT#for special aspect panel and Pipview LR
                #if (PIP_VIEW_LR == ENABLE)
                    SysSetFlag(FL_DUAL_CAM, DUALCAM_LR_FRONT_FULL);
                    PipView_SetStyle(UI_GetData(FL_DUAL_CAM));
                #endif
                //#NT#2016/06/17#Brain Yen -end
            }
            else if (uiSensor == (SENSOR_1|SENSOR_2))
            {
                // insert sensor, set movie to dual record
                SysSetFlag(FL_MOVIE_SIZE, DEFAULT_MOVIE_SIZE_DUAL);
                SysSetFlag(FL_MOVIE_SIZE_MENU, DEFAULT_MOVIE_SIZE_DUAL);
                //#NT#2016/06/17#Brain Yen -begin
                //#NT#for special aspect panel and Pipview LR
                #if (PIP_VIEW_LR == ENABLE)
					#if (_MODEL_DSC_ == _MODEL_MIO_MT39_)
					SysSetFlag(FL_DUAL_CAM, DUALCAM_LR_FULL);
					#else
                    SysSetFlag(FL_DUAL_CAM, DUALCAM_LR_16_9);
					#endif
                    PipView_SetStyle(UI_GetData(FL_DUAL_CAM));
                #else
                #if (PIP_VIEW_FUNC == ENABLE)
                SysSetFlag(FL_DUAL_CAM_MENU, DUALCAM_BOTH);
                SysSetFlag(FL_DUAL_CAM, DUALCAM_BOTH);

                PipView_SetStyle(UI_GetData(FL_DUAL_CAM));
                #endif
                #endif
                //#NT#2016/06/17#Brain Yen -end
            }
        }
    }
    uiPrevSensor = uiSensor;
}
#endif
//#NT#2016/02/24#Hideo Lin -end

#if(IPCAM_FUNC==ENABLE)
void RtspIFramReqCb(UINT32 pathId)
{

    //Ux_PostEvent(NVTEVT_EXE_VIDEO_PAUSE_RESUME, 1, pathId);
    #if UCTRL_FUNC
    //DBG_DUMP("IB\r\n");
    UctrlMain_Lock();
    //#NT#2017/02/22#David Tsai -begin
    //#NT#Use MovRec_ResetI to handle I-frame request(not reset BRC)
	MovRec_ResetI(pathId);
    //MovRec_PathN_Start_Stop(pathId, 0);
    //MovRec_PathN_Start_Stop(pathId, 1);
    //#NT#2017/02/22#David Tsai -end
    UctrlMain_UnLock();
    //Ux_SendEvent(NVTEVT_EXE_VIDEO_PAUSE_RESUME, 1, pathId);
    //UctrlMain_SendEvent(NVTEVT_EXE_VIDEO_PAUSE_RESUME,     1,  pathId);
    //DBG_DUMP("IE\r\n");
    #endif
}
#endif

/**
  Initialize application for AVI mode

  Initialize application for AVI mode.

  @param void
  @return void
*/


INT32 MovieExe_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    MEM_RANGE Pool;
#if(WIFI_AP_FUNC==ENABLE)
#if (XML_USE_APP_BUFFER==ENABLE)
    MEM_RANGE xmlPool;
#endif
#endif
//#NT#2016/03/07#KCHong -begin
//#NT#Low power timelapse function
#if (TIMELAPSE_LPR_FUNCTION == ENABLE)
    MEM_RANGE TLLPRPool;
#endif
//#NT#2016/03/07#KCHong -end
//#NT#2016/06/28#KCHong -begin
//#NT#DbgLog in GPS section
#if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
    UINT32 LogInfoBufSize = 0;
    MEM_RANGE LogInfoPool;
#endif
//#NT#2016/06/28#KCHong -end
    USIZE     ImageRatioSize;
    UINT32    useFileDB = 0;
    UINT32    ImageRatioIdx;
    UINT32    uiFrameRate;
    UINT32    uiPhotoCacheSize = 0;
    //#NT#2016/06/14#KCHong -begin
    //#NT#ImageUnit_ALG
    UINT32 CamMovieSFuncCfg = CAMMOVIE_SFUNC_NONE;
    UINT32 NetMovieSFuncCfg = NETMOVIE_SFUNC_NONE;
	//#NT#2017/01/23#Ben Wang -begin
	//#NT#Support ADAS in UVC_MULTIMEDIA_FUNC.
	#if(UVC_MULTIMEDIA_FUNC == ENABLE)
    UINT32 UsbMovieSFuncCfg = NVT_USBMOVIE_SFUNC_NONE;
    #endif
    //#NT#2017/01/23#Ben Wang -end
    ISF_IMG_INFO IsfImgSetting = {0};
    //#NT#2016/06/14#KCHong -end

    PHOTO_FUNC_INFO* phead = NULL;
    //#NT#2016/06/03#Brain Yen -begin
    //#NT#for special aspect panel and pipview
    #if (PIP_VIEW_LR == ENABLE || _LCDTYPE_ == _LCDTYPE_TG078UW006A0_DSI_ || _LCDTYPE_ == _LCDTYPE_M935UX01M_SN75LVDS83B_||_LCDTYPE_ == _LCDTYPE_XJ040WV1402C_R61408_DSI_ )
    USIZE DeviceRatioSize= GxVideo_GetDeviceAspect(DOUT1);
    #endif
    //#NT#2016/06/03#Brain Yen -end
    UINT32 nSensor= ImageUnit_GetParam(&ISF_ImagePipe, IMAGEPIPE_PARAM_SENSORCOUNT); //return 0/1/2/4

    //call default
    Ux_DefaultEvent(pCtrl,NVTEVT_EXE_OPEN,paramNum,paramArray);

//#NT#2016/02/24#Hideo Lin -begin
//#NT#Check movie size setting by sensor connection
#if (MOVIE_MULTI_RECORD_FUNC)
    MovieExe_CheckMovieSizeBySensorConnect();
#endif
//#NT#2016/02/24#Hideo Lin -end

//#NT#2016/06/28#KCHong -begin
//#NT#DbgLog in GPS section
#if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
    mp4log_Open();
    // Add debug log sub-section here
    // Ex: mp4log_AddSection("TEST", 1, &DbgLogTestID);
#if (_ADAS_FUNC_ == ENABLE)
    mp4log_AddSection("SNG ", 3, &DbgLogSnGID);
    mp4log_AddSection("FCLD",10, &DbgLogFCLDID);
    mp4log_AddSection("AUVP", 1, &DbgLogAutoVPID);
#endif

//#NT#2016/11/08#CC Chao -begin
//#NT#Saving log for TSR
#if (_TSR_FUNC_ == ENABLE)
    mp4log_AddSection("TSRL", 1, &DbgLogTsrID);
#endif
//#NT#2016/11/08

    //#NT#2016/07/20#ML Cui -begin
    //#NT#Saving log for DDD
#if (MOVIE_DDD_SAVE_LOG == ENABLE)
    mp4log_AddSection("DDD ", 20, &DbgLogDDDID);
#endif
    //#NT#2016/07/20#ML Cui -end

    //#NT#2016/08/06#XQ Liu -begin
    //#NT#Saving log for TD
#if (MOVIE_TD_SAVE_LOG == ENABLE)
    mp4log_AddSection("TDXX", 8, &DbgLogTDID);
#endif
    //#NT#2016/08/06#XQ Liu - end

#endif
//#NT#2016/06/28#KCHong -end

//#NT#2016/07/26#Niven Cho -begin
//#NT#IPCAM-DSP
#if (_DSP_TYPE_ == _DSP_FREERTOS_) && (IPCAM_FUNC==ENABLE)
    MEM_RANGE WrkBuff;
    WrkBuff.Addr = (UINT32)&g_ROIBuf[0];
    WrkBuff.Size = sizeof(g_ROIBuf);
    ROI_Init(&WrkBuff);
    IPL_Display_SetPostProcessEn(TRUE);
    IPL_Display_InitPstPrcCB(ROI_ProcessCb);
    //#NT#2016/08/30#David Tsai -begin
    //#NT#Disable ROI as default setting
    ROI_SetParam(ROI_PARAM_ENABLE, FALSE);
    ROI_SetParam(ROI_PARAM_ENABLE2, FALSE);
    ROI_SetParam(ROI_PARAM_ENABLE3, FALSE);
    //#NT#2016/08/30#David Tsai -end
#if 0 //set it as 1 to test ROI
    ROI_SetParam(ROI_PARAM_LEVEL, 10);
    ROI_SetParam(ROI_PARAM_ROI_X, 480);
    ROI_SetParam(ROI_PARAM_ROI_Y, 270);
    ROI_SetParam(ROI_PARAM_ROI_W, 960);
    ROI_SetParam(ROI_PARAM_ROI_H, 540);
    ROI_SetParam(ROI_PARAM_DRAW_ROI, 1);
    ROI_SetParam(ROI_PARAM_LEVEL2, 10);
    ROI_SetParam(ROI_PARAM_ROI_X2, 16);
    ROI_SetParam(ROI_PARAM_ROI_Y2, 16);
    ROI_SetParam(ROI_PARAM_ROI_W2, 320);
    ROI_SetParam(ROI_PARAM_ROI_H2, 240);
    ROI_SetParam(ROI_PARAM_DRAW_ROI2, 1);
    ROI_Start();
#endif
#endif
//#NT#2016/07/26#Niven Cho -end

#if(FORCE_UNPLUG_RTSP == ENABLE)
    ImageApp_IPCam_Config(IPCAM_CFG_PLUG_RTSP,0);
#endif

//#NT#2017/03/06#Charlie Chang -begin
//#NT# support SBS
	#if(IPCAM_FUNC==ENABLE)
		UI_SetData(FL_MOVIE_SBS_MODE, ImageApp_IPCam_GetConfig(IPCAM_CFG_SBS_FLAG));
	#endif
//#NT#2017/03/06#Charlie Chang -end

    //#NT#2016/08/03#Charlie Chang -begin
    //#NT# support iqtool
    #if(IQTOOL_FUNC == ENABLE)
    IqtoolIpc_Open(OS_GetMempoolAddr(POOL_ID_IQTOOLIPC), POOL_SIZE_IQTOOL_IPC);
    #endif
    //#NT#2016/08/03#Charlie Chang -end

	//#NT#2017/01/05#Charlie Chang -begin
	//#NT# support face tracking grading landmark save to pstore
	#if (FTG_SAVE_LANDMARK_TO_PSTORE == ENABLE)
		#if(MOVIE_FTG_FUNC == ENABLE)
			ftg_enablepstore(TRUE);
			MEM_RANGE ftg_buf;
			ftg_buf.Addr = (UINT32)OS_GetMempoolAddr(POOL_ID_APP);
			ftg_buf.Size = ftg_max_landmark_model_len;
			ftg_savelandmarktopstore(&ftg_buf);
		#endif
	#endif

	//#NT#2017/01/05#Charlie Chang -end
#if(IPCAM_FUNC==ENABLE)
	#if(IPCAM_UVC_FUNC == ENABLE)
    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_UVC)
    {
        Pool = AppMem_Alloc("ImageStream", APPMEM_ALL);
        ImageApp_UsbMovie_Config(NVT_USBMOVIE_CFG_POOL, (UINT32)&Pool);
    }
    else
	#endif
    {
        Pool = AppMem_Alloc("ImageStream", APPMEM_ALL);
        ImageApp_IPCam_Config(IPCAM_CFG_POOL, (UINT32)&Pool);
        ImageApp_IPCam_Config(IPCAM_CFG_IPL_MAXBUF, 5);
        ImageUnit_SetParam(&ISF_NetRTSP, NETRTSP_PARAM_MAX_CLIENT, RTSP_MAX_CLIENT);
		#if(RTSP_AUDIO_ENABLE == ENABLE)
        ImageUnit_SetParam(&ISF_NetRTSP, NETRTSP_PARAM_AUDIO, 1);
		#else
		ImageUnit_SetParam(&ISF_NetRTSP, NETRTSP_PARAM_AUDIO, 0);
		#endif
        ImageUnit_SetParam(&ISF_NetRTSP, NETRTSP_PARAM_MEDIA_SOURCE, RTSPNVT_MEDIA_SOURCE_BY_URL);
        ImageUnit_SetParam(&ISF_NetRTSP, NETRTSP_PARAM_IFRAME_REQ_CB, (UINT32)RtspIFramReqCb);
        //#NT#2016/06/13#Niven Cho -begin
        //set VI WMM category, refer to http://sourcedaddy.com/windows-7/selecting-dscp-values.html
        ImageUnit_SetParam(&ISF_NetRTSP, NETRTSP_PARAM_TOS,(0x20)<<2);
        //#NT#2016/06/13#Niven Cho -end
        ImageUnit_SetParam(&ISF_Media, MEDIA_PARAM_RAWENC_NOFILE, (UINT32)0);
    }
//#NT#2017/02/24#Ben Wang -begin
//#NT#Fix that UVC_MULTIMEDIA_FUNC depends on WIFI_AP_FUNC.
#elif((WIFI_AP_FUNC==ENABLE) || (UVC_MULTIMEDIA_FUNC == ENABLE))
//#NT#2017/02/24#Ben Wang -end
    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
    {
        #if (XML_USE_APP_BUFFER==ENABLE)
        xmlPool = AppMem_Alloc("XML", POOL_SIZE_TEMP);
        XML_SetTempMem(xmlPool.Addr,xmlPool.Size);
        #endif
        //#NT#2016/03/07#KCHong -begin
        //#NT#Low power timelapse function
        #if (TIMELAPSE_LPR_FUNCTION == ENABLE)
        TLLPRPool = AppMem_Alloc("TLLPR", TL_BUFFER_SIZE);
        MovieTLLPR_SetMem(TLLPRPool.Addr,TLLPRPool.Size);
        #endif
        //#NT#2016/03/07#KCHong -end
        //#NT#2016/06/28#KCHong -begin
        //#NT#DbgLog in GPS section
        #if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
        if ((LogInfoBufSize = mp4log_GetTotalLogSize()) != 0)
        {
            LogInfoPool = AppMem_Alloc("LOGINFO", (2 * LogInfoBufSize * 1024 - 16));
            mp4log_SetMem(LogInfoPool.Addr,LogInfoPool.Size);
        }
        #endif
        //#NT#2016/06/28#KCHong -end
        Pool = AppMem_Alloc("ImageStream", APPMEM_ALL);
        ImageApp_NetMovie_Config(NETMOVIE_CFG_POOL, (UINT32)&Pool);
    }
//#NT#2016/05/31#Ben Wang -begin
//#NT#Add UVC multimedia function.
#if(UVC_MULTIMEDIA_FUNC == ENABLE)
    else if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_UVC)
    {
        #if (XML_USE_APP_BUFFER==ENABLE)
        xmlPool = AppMem_Alloc("XML", POOL_SIZE_TEMP);
        XML_SetTempMem(xmlPool.Addr,xmlPool.Size);
        #endif
        //#NT#2016/03/07#KCHong -begin
        //#NT#Low power timelapse function
        #if (TIMELAPSE_LPR_FUNCTION == ENABLE)
        TLLPRPool = AppMem_Alloc("TLLPR", TL_BUFFER_SIZE);
        MovieTLLPR_SetMem(TLLPRPool.Addr,TLLPRPool.Size);
        #endif
        //#NT#2016/03/07#KCHong -end
        //#NT#2016/06/28#KCHong -begin
        //#NT#DbgLog in GPS section
        #if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
        if ((LogInfoBufSize = mp4log_GetTotalLogSize()) != 0)
        {
            LogInfoPool = AppMem_Alloc("LOGINFO", (2 * LogInfoBufSize * 1024 - 16));
            mp4log_SetMem(LogInfoPool.Addr,LogInfoPool.Size);
        }
        #endif
        //#NT#2016/06/28#KCHong -end
        Pool = AppMem_Alloc("ImageStream", APPMEM_ALL);
        ImageApp_UsbMovie_Config(NVT_USBMOVIE_CFG_POOL, (UINT32)&Pool);
    }
#endif
//#NT#2016/05/31#Ben Wang -end
    else
    {
        //#NT#2016/03/07#KCHong -begin
        //#NT#Low power timelapse function
        #if (TIMELAPSE_LPR_FUNCTION == ENABLE)
        TLLPRPool = AppMem_Alloc("TLLPR", TL_BUFFER_SIZE);
        MovieTLLPR_SetMem(TLLPRPool.Addr,TLLPRPool.Size);
        #endif
        //#NT#2016/03/07#KCHong -end
        //#NT#2016/06/28#KCHong -begin
        //#NT#DbgLog in GPS section
        #if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
        if ((LogInfoBufSize = mp4log_GetTotalLogSize()) != 0)
        {
            LogInfoPool = AppMem_Alloc("LOGINFO", (2 * LogInfoBufSize * 1024 - 16));
            mp4log_SetMem(LogInfoPool.Addr,LogInfoPool.Size);
        }
        #endif
        //#NT#2016/06/28#KCHong -end
        Pool = AppMem_Alloc("ImageStream", APPMEM_ALL);
        ImageApp_CamMovie_Config(MOVIE_CFG_POOL, (UINT32)&Pool);
    }
#else
    {
        //#NT#2016/03/07#KCHong -begin
        //#NT#Low power timelapse function
        #if (TIMELAPSE_LPR_FUNCTION == ENABLE)
        TLLPRPool = AppMem_Alloc("TLLPR", TL_BUFFER_SIZE);
        MovieTLLPR_SetMem(TLLPRPool.Addr,TLLPRPool.Size);
        #endif
        //#NT#2016/03/07#KCHong -end
        //#NT#2016/06/28#KCHong -begin
        //#NT#DbgLog in GPS section
        #if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
        if ((LogInfoBufSize = mp4log_GetTotalLogSize()) != 0)
        {
            LogInfoPool = AppMem_Alloc("LOGINFO", (2 * LogInfoBufSize * 1024 - 16));
            mp4log_SetMem(LogInfoPool.Addr,LogInfoPool.Size);
        }
        #endif
        //#NT#2016/06/28#KCHong -end
        Pool = AppMem_Alloc("ImageStream", APPMEM_ALL);
        ImageApp_CamMovie_Config(MOVIE_CFG_POOL, (UINT32)&Pool);
    }
#endif


#if (MOVIE_PIM_CAPTURE == ENABLE)
    ImageApp_CamMovie_Config(MOVIE_CFG_PIM_CAPTURE, TRUE);
    ImageApp_CamMovie_Config(MOVIE_CFG_CAPTURE_UNIT, (UINT32)&ISF_Cap);
    ImageUnit_SetParam(&ISF_Cap, CAP_PARAM_BUFSIZE, 88*1024*1024); // 88MB for picture in movie function (can capture 2 14M images)
    ImageApp_CamMovie_Config(MOVIE_CFG_CAPTURE_UNIT, (UINT32)&ISF_Cap);
#else
    ImageApp_CamMovie_Config(MOVIE_CFG_PIM_CAPTURE, FALSE);
#endif
#if (MOVIE_PIM_SNAPSHOT == ENABLE)
    ImageApp_CamMovie_Config(MOVIE_CFG_PIM_SNAPSHOT, TRUE);
    ImageApp_NetMovie_Config(NETMOVIE_CFG_PIM_SNAPSHOT, TRUE);
    ImageApp_IPCam_Config(IPCAM_CFG_PIM_SNAPSHOT, TRUE);
//#NT#2016/05/31#Ben Wang -begin
//#NT#Add UVC multimedia function.
#if(UVC_MULTIMEDIA_FUNC == ENABLE)
    ImageApp_UsbMovie_Config(NVT_USBMOVIE_CFG_PIM_SNAPSHOT, TRUE);
#endif
//#NT#2016/05/31#Ben Wang -end
#else
    ImageApp_CamMovie_Config(MOVIE_CFG_PIM_SNAPSHOT, FALSE);
    ImageApp_NetMovie_Config(NETMOVIE_CFG_PIM_SNAPSHOT, FALSE);
    ImageApp_IPCam_Config(IPCAM_CFG_PIM_SNAPSHOT, FALSE);
//#NT#2016/05/31#Ben Wang -begin
//#NT#Add UVC multimedia function.
#if(UVC_MULTIMEDIA_FUNC == ENABLE)
    ImageApp_UsbMovie_Config(NVT_USBMOVIE_CFG_PIM_SNAPSHOT, FALSE);
#endif
//#NT#2016/05/31#Ben Wang -end
#endif

    // Change movie size before IPL change mode.
    Ux_SendEvent(0, NVTEVT_EXE_MOVIESIZE, 1, UI_GetData(FL_MOVIE_SIZE));

    //
    // Setup IPL information.
    //
    phead = &PhotoFuncInfo_dummy;
    phead->pNext = NULL; //mark end

//#NT#2015/04/26#Pin Su -begin
    #if _ADAS_FUNC_
    //#NT#2016/03/25#KCHong -begin
    //#NT#New ADAS
    // --------------------------------------------------------------------------------------------------
    //    *** ADAS parameter setting ***
    //    User can set ADAS parameters (FSize/LD sensitivity/FC warning distance/ LD debounce time/FC debounce time)
    //    If not set, ADAS lib will use default settings.
    //    Important notice:
    //        ADAS_SetLdFcFsizeValueByPixel(), ADAS_SetLdwsLeftSensitivity(), ADAS_SetLdwsRightSensitivity() MUST be set before ADAS_Init()
    //        Only ADAS_SetFcwsUIWarningDist() / ADAS_SetFcwsDebounceTime / ADAS_SetLdwsDebounceTime can be changed during ADAS detection.
    //        Ex. Different car speed has different FC warning distance.
    //
    //        FSize          : Please refer to APNote to measure FSize
    //        LD Sensitivity : 0~50, default is 15. Bigger value means more sentive.
    //        FC distance    : 1~40, default is 15. The unit is meter.
    //   --------------------------------------------------------------------------------------------------
    // Set fsize value. This value CANNOT be changed on-the-fly.
    ADAS_SetLdFcFsizeValueByPixel(188);

    // Set fcws warning distance (unit: m). This value can be changed on-the-fly.
    ADAS_SetFcwsUIWarningDist(20);//15
    // Set fcws debounce time (unit: ms). This value can be changed on-the-fly.
    ADAS_SetFcwsDebounceTime(10*1000);

    // Set ldws left side sensitivity. This value CANNOT be changed on-the-fly.
    ADAS_SetLdwsLeftSensitivity(20);//15
    // Set ldws right side sensitivity. This value CANNOT be changed on-the-fly.
    ADAS_SetLdwsRightSensitivity(20);//15
    // Set ldws debounce time (Unit: ms). This value can be changed on-the-fly.
    ADAS_SetLdwsDebounceTime(5*1000);//10*1000
	// Set fcws output format
	ADAS_SetFcwsCarRsltFmt(ADAS_FCWS_RSLT_LANE_WIDTH);
    //#NT#2016/10/18#Jeah Yen -begin
    //#NT#support sensor map
    // Set sng IPL source. This value CANNOT be changed on-the-fly.
	ADAS_selHighPrioAppsDataSrc(IPL_PATH_1);
    // Set fc/ld IPL source. This value CANNOT be changed on-the-fly.
	ADAS_selMidPrioAppsDataSrc(IPL_PATH_1);
    // Set autoVP IPL source. This value CANNOT be changed on-the-fly.
	ADAS_selLowPrioAppsDataSrc(IPL_PATH_1);
    //#NT#2016/10/18#Jeah Yen -end

    {
        PHOTO_FUNC_INFO* pfunc = &PhotoFuncInfo_adas;
        PHOTO_FUNC_INFO* pcurr = 0;
        pcurr = phead; while(pcurr->pNext) pcurr = pcurr->pNext; //find last func
        pcurr->pNext = pfunc; //append this func
        pfunc->pNext = NULL; //mark end
    }

    //ADAS_RegisterCB(UIMovie_ADASCB);
    {
        PHOTO_FUNC_INFO* pfunc = &PhotoFuncInfo_SnG;
        PHOTO_FUNC_INFO* pcurr = 0;
        pcurr = phead; while(pcurr->pNext) pcurr = pcurr->pNext; //find last func
        pcurr->pNext = pfunc; //append this func
        pfunc->pNext = NULL; //mark end
    }

#if (_AUTOVP_FUNC_ == ENABLE)
    {
        PHOTO_FUNC_INFO* pfunc = &PhotoFuncInfo_autoVP;
        PHOTO_FUNC_INFO* pcurr = 0;
        pcurr = phead; while(pcurr->pNext) pcurr = pcurr->pNext; //find last func
        pcurr->pNext = pfunc; //append this func
        pfunc->pNext = NULL; //mark end
    }
#endif
//#NT#2016/03/25#KCHong -end
//#NT#2015/04/26#Pin Su -end
    #endif

    //#NT#2016/07/06#CC Chao -begin
    #if MOVIE_TSR_FUNC_
    {
        UINT32      myDualCam;
        PHOTO_FUNC_INFO* pfunc = &PhotoFuncInfo_mvtsr;
        PHOTO_FUNC_INFO* pcurr = 0;
        pcurr = phead; while(pcurr->pNext) pcurr = pcurr->pNext; //find last func
        pcurr->pNext = pfunc; //append this func
        pfunc->pNext = NULL; //mark end

        myDualCam = Photo_getDualCamSetting();
        //#NT#2016/10/18#Jeah Yen -begin
        //#NT#support sensor map
        if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH)
            PhotoFuncInfo_mvtsr.Info.IPLId = IPL_PATH_1;
        else
            PhotoFuncInfo_mvtsr.Info.IPLId = IPL_PATH_2;
        //#NT#2016/10/18#Jeah Yen -end

        DBG_IND("myDualCam = %d, TSR IPLID = %d\r\n",myDualCam,PhotoFuncInfo_mvtsr.Info.IPLId);

        TSR_Lock(FALSE);
    }
    #endif
    //#NT#2016/07/06#CC Chao -end

    //#NT#2016/07/15#XQ Liu -begin
    #if MOVIE_TSR_FUNC_
    {
        UINT32      myDualCam;
        PHOTO_FUNC_INFO* pfunc = &PhotoFuncInfo_mvtsrdsp;
        PHOTO_FUNC_INFO* pcurr = 0;
        pcurr = phead; while(pcurr->pNext) pcurr = pcurr->pNext; //find last func
        pcurr->pNext = pfunc; //append this func
        pfunc->pNext = NULL; //mark end

        myDualCam = Photo_getDualCamSetting();
        //#NT#2016/10/18#Jeah Yen -begin
        //#NT#support sensor map
        if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH)
            PhotoFuncInfo_mvtsrdsp.Info.IPLId = IPL_PATH_1;
        else
            PhotoFuncInfo_mvtsrdsp.Info.IPLId = IPL_PATH_2;
        //#NT#2016/10/18#Jeah Yen -begin

        DBG_IND("myDualCam = %d, TSR IPLID = %d\r\n",myDualCam,PhotoFuncInfo_mvtsrdsp.Info.IPLId);
    }
    #endif
    //#NT#2016/07/15#XQ Liu -end

    #if ((_ADAS_FUNC_ == ENABLE)&&(_ADAS_DSP_FUNC_ ==ENABLE))
    {
        PHOTO_FUNC_INFO* pfunc = &PhotoFuncInfo_mvldfcdsp;
        PHOTO_FUNC_INFO* pcurr = 0;
        pcurr = phead; while(pcurr->pNext) pcurr = pcurr->pNext; //find last func
        pcurr->pNext = pfunc; //append this func
        pfunc->pNext = NULL; //mark end
    }
    #endif
	

    // #NT#2015/09/17#JS Liu - begin
    #if _CVHOST_EX_
    {
        PHOTO_FUNC_INFO* pfunc = &MovieFuncInfo_CvHostEx;
        PHOTO_FUNC_INFO* pcurr = 0;
        pcurr = phead; while(pcurr->pNext) pcurr = pcurr->pNext; //find last func
        pcurr->pNext = pfunc; //append this func
        pfunc->pNext = NULL; //mark end
    }
    #endif
    // #NT#2015/09/17#JS Liu - end


    //#NT#2016/05/31#Omega Yu -begin
    #if _NVTCV_EX_
    {
        PHOTO_FUNC_INFO* pfunc = &MovieFuncInfo_NvtCvEx;
        PHOTO_FUNC_INFO* pcurr = 0;
        pcurr = phead; while(pcurr->pNext) pcurr = pcurr->pNext; //find last func
        pcurr->pNext = pfunc; //append this func
        pfunc->pNext = NULL; //mark end
    }
    #endif
    //#NT#2016/05/31#Omega Yu -end


    #if (MOVIE_TEST_ENABLE != ENABLE)
    {// DIS function
        PHOTO_FUNC_INFO* pfunc = &PhotoFuncInfo_dis;
        PHOTO_FUNC_INFO* pcurr = 0;
        pcurr = phead; while(pcurr->pNext) pcurr = pcurr->pNext; //find last func
        pcurr->pNext = pfunc; //append this func
        pfunc->pNext = NULL; //mark end
    }
    #endif
    #if (IPCAM_FUNC ==ENABLE)
    {
        PHOTO_FUNC_INFO* pfunc = &PhotoFuncInfo_ipcam_motion;
        PHOTO_FUNC_INFO* pcurr = 0;
        pcurr = phead; while(pcurr->pNext) pcurr = pcurr->pNext; //find last func
        pcurr->pNext = pfunc; //append this func
        pfunc->pNext = NULL; //mark end
    }
    #endif
    #if MOVIE_FD_FUNC_
    {
        UINT32      myDualCam;
        PHOTO_FUNC_INFO* pfunc = &PhotoFuncInfo_mvfd;
        PHOTO_FUNC_INFO* pcurr = 0;
        pcurr = phead; while(pcurr->pNext) pcurr = pcurr->pNext; //find last func
        pcurr->pNext = pfunc; //append this func
        pfunc->pNext = NULL; //mark end

        myDualCam = Photo_getDualCamSetting();
        //#NT#2016/10/18#Jeah Yen -begin
        //#NT#support sensor map
        if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH)
            PhotoFuncInfo_mvfd.Info.IPLId = IPL_PATH_1;
        else
            PhotoFuncInfo_mvfd.Info.IPLId = IPL_PATH_2;
        //#NT#2016/10/18#Jeah Yen -end

        DBG_IND("myDualCam = %d, FD IPLID = %d\r\n",myDualCam,PhotoFuncInfo_mvfd.Info.IPLId);
    }
    //#NT#2016/06/08#Lincy Lin -begin
    //#NT#Implement generic OSD and video drawing mechanism for ALG function
    //MovieExe_GetFDDispCord(&gMovieFdDispCord);
    //#NT#2016/06/08#Lincy Lin -end
    // unlock FD
    //#NT#2016/04/22#Nestor Yang -begin
    //#NT# Support Driver Drowsiness Detection(DDD)
    //#NT#2016/05/20#David Tsai -begin
    //#NT# IP cam will unlock FD by CPU_CORE2
    #if(IPCAM_FUNC!=ENABLE)
    FD_Lock(FALSE);
    #endif
    //#NT#2016/05/20#David Tsai -end
    //#NT#2016/04/22#Nestor Yang -end
    #endif
    //#NT#2016/03/02#Lincy Lin -begin
    //#NT#Support object tracking function
    #if MOVIE_OT_FUNC
    {
        UINT32      myDualCam;
        PHOTO_FUNC_INFO* pfunc = &PhotoFuncInfo_mvot;
        PHOTO_FUNC_INFO* pcurr = 0;
        pcurr = phead; while(pcurr->pNext) pcurr = pcurr->pNext; //find last func
        pcurr->pNext = pfunc; //append this func
        pfunc->pNext = NULL; // mark end

        myDualCam = Photo_getDualCamSetting();
        //#NT#2016/10/18#Jeah Yen -begin
        //#NT#support sensor map
        if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH)
            PhotoFuncInfo_mvot.Info.IPLId = IPL_PATH_1;
        else
            PhotoFuncInfo_mvot.Info.IPLId = IPL_PATH_2;
        //#NT#2016/10/18#Jeah Yen -end
    }
    //#NT#2016/06/08#Lincy Lin -begin
    //#NT#Implement generic OSD and video drawing mechanism for ALG function
    //MovieExe_GetFDDispCord(&gMovieFdDispCord);
    //#NT#2016/06/08#Lincy Lin -end
    #endif
    //#NT#2016/03/02#Lincy Lin -end

    //#NT#2016/05/20#Yang Jin -begin
    //#NT#Support Dis dsp function

    #if MOVIE_DISDSP_FUNC
    {
        UINT32      myDualCam;
        PHOTO_FUNC_INFO* pfunc = &PhotoFuncInfo_mvdisdsp;
        PHOTO_FUNC_INFO* pcurr = 0;
        pcurr = phead; while(pcurr->pNext) pcurr = pcurr->pNext; //find last func
        pcurr->pNext = pfunc; //append this func
        pfunc->pNext = NULL; // mark end

        myDualCam = Photo_getDualCamSetting();
        //#NT#2016/10/18#Jeah Yen -begin
        //#NT#support sensor map
        if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH)
            PhotoFuncInfo_mvdisdsp.Info.IPLId = IPL_PATH_1;
        else
            PhotoFuncInfo_mvdisdsp.Info.IPLId = IPL_PATH_2;
        //#NT#2016/10/18#Jeah Yen -end
    }
    #endif

    //#NT#2016/05/20#Yang Jin -end

	//#NT#2016/11/15#ML Cui -begin
	//#NT# Support DDD's Face Detection(DDDFD)
    #if MOVIE_DDD_FD_FUNC
    {
        UINT32      myDualCam;

        PHOTO_FUNC_INFO* pfunc = &PhotoFuncInfo_mvdddfd;
        PHOTO_FUNC_INFO* pcurr = 0;
        pcurr = phead; while(pcurr->pNext) pcurr = pcurr->pNext; //find last func
        pcurr->pNext = pfunc; //append this func
        pfunc->pNext = NULL; //mark end

        myDualCam = Photo_getDualCamSetting();
        if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH)
            PhotoFuncInfo_mvdddfd.Info.IPLId = IPL_PATH_1;
        else
            PhotoFuncInfo_mvdddfd.Info.IPLId = IPL_PATH_2;

    }
    #endif
	//#NT#2016/11/15#ML Cui -end

    //#NT#2016/04/22#Nestor Yang -begin
    //#NT# Support Driver Drowsiness Detection(DDD)
    #if MOVIE_DDD_FUNC
    {
        UINT32      myDualCam;

        PHOTO_FUNC_INFO* pfunc = &PhotoFuncInfo_mvddd;
        PHOTO_FUNC_INFO* pcurr = 0;
        pcurr = phead; while(pcurr->pNext) pcurr = pcurr->pNext; //find last func
        pcurr->pNext = pfunc; //append this func
        pfunc->pNext = NULL; //mark end

        myDualCam = Photo_getDualCamSetting();
        //#NT#2016/10/18#Jeah Yen -begin
        //#NT#support sensor map
        if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH)
            PhotoFuncInfo_mvddd.Info.IPLId = IPL_PATH_1;
        else
            PhotoFuncInfo_mvddd.Info.IPLId = IPL_PATH_2;
        //#NT#2016/10/18#Jeah Yen -end
    }
    //#NT#2016/06/08#Lincy Lin -begin
    //#NT#Implement generic OSD and video drawing mechanism for ALG function
    //MovieExe_GetFDDispCord(&gMovieFdDispCord);
    //#NT#2016/06/08#Lincy Lin -end

    DDD_Lock(FALSE);
    #endif
    //#NT#2016/04/22#Nestor Yang -end
    //#NT#2016/05/23#David Tsai -begin
    //#NT# Support tarmpering detection and background calculation function
    //#NT#2016/10/14#Yuzhe Bian -begin
    //#NT# Support trip-wire detection & trip-zone detection function
    #if MOVIE_BC_FUNC || MOVIE_TD_FUNC || MOVIE_TWD_FUNC || MOVIE_TZD_FUNC
    {
        UINT32      myDualCam;
        PHOTO_FUNC_INFO* pfunc = &PhotoFuncInfo_mvbc;
        PHOTO_FUNC_INFO* pcurr = 0;
        pcurr = phead; while(pcurr->pNext) pcurr = pcurr->pNext; //find last func
        pcurr->pNext = pfunc; //append this func
        pfunc->pNext = NULL; // mark end

        myDualCam = Photo_getDualCamSetting();
        //#NT#2016/10/18#Jeah Yen -begin
        //#NT#support sensor map
        if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH)
            PhotoFuncInfo_mvbc.Info.IPLId = IPL_PATH_1;
        else
            PhotoFuncInfo_mvbc.Info.IPLId = IPL_PATH_2;
        //#NT#2016/10/18#Jeah Yen -end
    }
    //#NT#2016/10/14#Yuzhe Bian -end
    //#NT#2016/06/08#Lincy Lin -begin
    //#NT#Implement generic OSD and video drawing mechanism for ALG function
    //MovieExe_GetFDDispCord(&gMovieFdDispCord);
    //#NT#2016/06/08#Lincy Lin -end
    #if(IPCAM_FUNC!=ENABLE)
    Movie_BCLock(FALSE);
    #endif
    #endif

    #if MOVIE_TD_FUNC
    {
        UINT32      myDualCam;
        PHOTO_FUNC_INFO* pfunc = &PhotoFuncInfo_mvtd;
        PHOTO_FUNC_INFO* pcurr = 0;
        pcurr = phead; while(pcurr->pNext) pcurr = pcurr->pNext; //find last func
        pcurr->pNext = pfunc; //append this func
        pfunc->pNext = NULL; // mark end

        myDualCam = Photo_getDualCamSetting();
        //#NT#2016/10/18#Jeah Yen -begin
        //#NT#support sensor map
        if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH)
            PhotoFuncInfo_mvtd.Info.IPLId = IPL_PATH_1;
        else
            PhotoFuncInfo_mvtd.Info.IPLId = IPL_PATH_2;
        //#NT#2016/10/18#Jeah Yen -end
    }
    //#NT#2016/06/08#Lincy Lin -begin
    //#NT#Implement generic OSD and video drawing mechanism for ALG function
    //MovieExe_GetFDDispCord(&gMovieFdDispCord);
    //#NT#2016/06/08#Lincy Lin -end
    #endif
    //#NT#2016/05/23#David Tsai -end

    //#NT#2016/10/14#Yuzhe Bian -begin
    //#NT# Support trip-wire detection & trip-zone detection function
    #if MOVIE_TWD_FUNC
    {
        UINT32      myDualCam;
        PHOTO_FUNC_INFO* pfunc = &PhotoFuncInfo_mvtwd;
        PHOTO_FUNC_INFO* pcurr = 0;
        pcurr = phead; while(pcurr->pNext) pcurr = pcurr->pNext; //find last func
        pcurr->pNext = pfunc; //append this func
        pfunc->pNext = NULL; // mark end

        myDualCam = Photo_getDualCamSetting();
        //#NT#2016/10/18#Jeah Yen -begin
        //#NT#support sensor map
        if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH)
            PhotoFuncInfo_mvtwd.Info.IPLId = IPL_PATH_1;
        else
            PhotoFuncInfo_mvtwd.Info.IPLId = IPL_PATH_2;
        //#NT#2016/10/18#Jeah Yen -end
    }
    #endif
    #if MOVIE_TZD_FUNC
    {
        UINT32      myDualCam;
        PHOTO_FUNC_INFO* pfunc = &PhotoFuncInfo_mvtzd;
        PHOTO_FUNC_INFO* pcurr = 0;
        pcurr = phead; while(pcurr->pNext) pcurr = pcurr->pNext; //find last func
        pcurr->pNext = pfunc; //append this func
        pfunc->pNext = NULL; // mark end

        myDualCam = Photo_getDualCamSetting();
        //#NT#2016/10/18#Jeah Yen -begin
        //#NT#support sensor map
        if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH)
            PhotoFuncInfo_mvtzd.Info.IPLId = IPL_PATH_1;
        else
            PhotoFuncInfo_mvtzd.Info.IPLId = IPL_PATH_2;
        //#NT#2016/10/18#Jeah Yen -end
    }
    #endif
    //#NT#2016/10/14#Yuzhe Bian -end

    //#NT#2016/10/27#Omega Yu -begin
    //#NT# Support scene change detection function
    #if MOVIE_SCD_FUNC
    {
        UINT32      myDualCam;
        PHOTO_FUNC_INFO* pfunc = &PhotoFuncInfo_mvscd;
        PHOTO_FUNC_INFO* pcurr = 0;
        pcurr = phead; while(pcurr->pNext) pcurr = pcurr->pNext; //find last func
        pcurr->pNext = pfunc; //append this func
        pfunc->pNext = NULL; // mark end

        myDualCam = Photo_getDualCamSetting();
        if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH)
            PhotoFuncInfo_mvscd.Info.IPLId = IPL_PATH_1;
        else
            PhotoFuncInfo_mvscd.Info.IPLId = IPL_PATH_2;
    }
    #endif
    //#NT#2016/10/27#Omega Yu -end

    //#NT#2016/10/17#Bin Xiao -begin
    //#NT# Support Face Tracking Grading(FTG)
    #if MOVIE_FTG_FUNC
    {
        UINT32      myDualCam;
        PHOTO_FUNC_INFO* pfunc = &PhotoFuncInfo_mvftg;
        PHOTO_FUNC_INFO* pcurr = 0;
        pcurr = phead; while(pcurr->pNext) pcurr = pcurr->pNext; //find last func
        pcurr->pNext = pfunc; //append this func
        pfunc->pNext = NULL; //mark end

        myDualCam = Photo_getDualCamSetting();
        //#NT#2016/10/18#Jeah Yen -begin
        //#NT#support sensor map
        if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH)
            PhotoFuncInfo_mvftg.Info.IPLId = IPL_PATH_1;
        else
            PhotoFuncInfo_mvftg.Info.IPLId = IPL_PATH_2;
        //#NT#2016/10/18#Jeah Yen -begin

        DBG_IND("myDualCam = %d, FTG IPLID = %d\r\n",myDualCam,PhotoFuncInfo_mvftg.Info.IPLId);
    }
	#if(IPCAM_FUNC!=ENABLE)
	Movie_FTGLock(FALSE);
    #endif
    #endif
    //#NT#2016/10/17#Bin Xiao -end

	//#NT#2016/10/17#Bin Xiao -begin
    //#NT# Support FD used by Face Tracking Grading(FTG)
    #if MOVIE_FTG_FUNC
    {
        UINT32      myDualCam;
        PHOTO_FUNC_INFO* pfunc = &PhotoFuncInfo_mvftgfd;
        PHOTO_FUNC_INFO* pcurr = 0;
        pcurr = phead; while(pcurr->pNext) pcurr = pcurr->pNext; //find last func
        pcurr->pNext = pfunc; //append this func
        pfunc->pNext = NULL; //mark end

        myDualCam = Photo_getDualCamSetting();
        //#NT#2016/10/18#Jeah Yen -begin
        //#NT#support sensor map
        if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH)
            PhotoFuncInfo_mvftgfd.Info.IPLId = IPL_PATH_1;
        else
            PhotoFuncInfo_mvftgfd.Info.IPLId = IPL_PATH_2;
        //#NT#2016/10/18#Jeah Yen -end

        DBG_IND("myDualCam = %d, FTGFD IPLID = %d\r\n",myDualCam,PhotoFuncInfo_mvftgfd.Info.IPLId);
    }
	/*
	#if(IPCAM_FUNC!=ENABLE)
	Movie_FTGLock(FALSE);
	#endif
	*/
    #endif
    //#NT#2016/10/17#Bin Xiao -end


    ImageUnit_SetParam(&ISF_ImagePipe, PHOTO_PARAM_FUNCINFO, (UINT32)phead);

    uiPhotoCacheSize = 0;
    #if MOVIE_FD_FUNC_
    uiPhotoCacheSize += 2000*1024;
    #endif
    //#NT#2016/05/23#David Tsai -begin
    //#NT# Support tarmpering detection and background calculation function
    //#NT#2016/10/14#Yuzhe Bian -begin
    //#NT# Support trip-wire detection & trip-zone detection function
    #if MOVIE_BC_FUNC || MOVIE_TD_FUNC || MOVIE_TWD_FUNC || MOVIE_TZD_FUNC
    uiPhotoCacheSize += 4 * 1024 * 1024;
    #endif
    #if  MOVIE_TWD_FUNC
    uiPhotoCacheSize += 3 * 1024;
    #endif
    #if  MOVIE_TZD_FUNC
    uiPhotoCacheSize += 3 * 1024;
    #endif
    //#NT#2016/10/14#Yuzhe Bian -end

    //#NT#2016/05/23#David Tsai -end
    #if _ADAS_FUNC_
    uiPhotoCacheSize += 4*1024*1024;
    #endif
	//#NT#2016/10/17#Bin Xiao -begin
    //#NT# Support FD used by Face Tracking Grading(FTG)
    #if MOVIE_FTG_FUNC
    uiPhotoCacheSize += 2000*1024;
    #endif
    //#NT#2016/10/17#Bin Xiao -end

    //#NT#2016/11/15#ML Cui -begin
    //#NT# Add DDD cache setting
    #if (MOVIE_DDD_FUNC)
    uiPhotoCacheSize += DDD_CalcCacheBuffSize();
    #endif
    //#NT#2016/11/15#ML Cui -end

    ImageUnit_SetParam(&ISF_ImagePipe, PHOTO_PARAM_CACHESIZE, uiPhotoCacheSize);

    //set photo function
    {
        IPL_POST_PROC_FUNC_EN Func1_En = IPL_FUNC_NONE;
        IPL_POST_PROC_FUNC_EN Func2_En = IPL_FUNC_NONE;
		//#NT#2016/12/23#Charlie Chang -begin
		//#NT# support dewarp function
		#if(IPCAM_FUNC == ENABLE)
			#if (FISHEYE_TYPE == ENABLE)
				BOOL dewarp_enable = ImageApp_IPCam_GetConfig(IPCAM_CFG_DEWARP_FLAG);
				if(dewarp_enable == TRUE){
					memset(&g_VR360Setting, 0, sizeof(IPL_VR360_DATA));
					g_VR360Setting.ori[0] = 1;
					g_VR360Setting.ori[1] = 0;
					g_VR360Setting.IME_P1_Start_Offset = 95;
					g_VR360Setting.IME_P2_Start_Offset = 100;
					g_VR360Setting.IME_P3_Start_Offset = 100;
					g_VR360Setting.ImgHeightAdj = 1024;
					Func1_En |= IPL_FUNC_VR360;
					ImageUnit_SetParam(&ISF_ImagePipe, PHOTO_PARAM_VR360_1, (UINT32)&g_VR360Setting);
				}
			#endif
		#endif
		//#NT#2016/12/23#Charlie Chang -end
        // NOTE: WDR & SHDR is only one can active at the same time
        #if WDR_FUNC
        if (UI_GetData(FL_MOVIE_WDR) == MOVIE_WDR_ON || UI_GetData(FL_MOVIE_HDR) == MOVIE_HDR_OFF)
        {
            Func1_En |= IPL_FUNC_WDR;
            Func2_En |= IPL_FUNC_WDR;
        }
        MovieExe_IPL_SetIInfo(IPL_SEL_WDR, UI_GetData(FL_MOVIE_WDR));
        #endif
        #if SHDR_FUNC
        if (UI_GetData(FL_MOVIE_HDR) == MOVIE_HDR_ON && UI_GetData(FL_MOVIE_WDR) == MOVIE_WDR_OFF)
        {
            Func1_En |= IPL_FUNC_SHDR;
            Func2_En |= IPL_FUNC_SHDR;
            MovieExe_IPL_SetIInfo(IPL_SEL_SHDR, SEL_SHDR_ON);
        }
        else
        {
            UI_SetData(FL_MOVIE_HDR, MOVIE_HDR_OFF);
            MovieExe_IPL_SetIInfo(IPL_SEL_SHDR, SEL_SHDR_OFF);
        }
        #endif

        //#NT#2016/06/14#KCHong -begin
        //#NT#ImageUnit_ALG, Config image setting here
        // IPL1 path3
        memset(&IsfImgSetting, 0, sizeof(ISF_IMG_INFO));
        IsfImgSetting.ImgFmt = GX_IMAGE_PIXEL_FMT_YUV420_PACKED;
        IsfImgSetting.ImgSize.w = 640;
        IsfImgSetting.ImgSize.h = 480;
        ImageUnit_SetParam(&ISF_ALG, ALG_PARAM_SET_IN1, (UINT32)&IsfImgSetting);

        // IPL1 path4
        memset(&IsfImgSetting, 0, sizeof(ISF_IMG_INFO));
        IsfImgSetting.ImgFmt = GX_IMAGE_PIXEL_FMT_Y_ONLY;
        IsfImgSetting.ImgSize.w = 1920;
        IsfImgSetting.ImgSize.h = 1080;
        #if (_MODEL_DSC_ == _MODEL_EVB_ && MOVIE_DDD_FUNC == ENABLE)
        IsfImgSetting.Window.w = 0;
        IsfImgSetting.Window.h = 0;
        IsfImgSetting.Window.x  = 0;
        IsfImgSetting.Window.y  = 0;
        #else
        IsfImgSetting.Window.w = 720;
        IsfImgSetting.Window.h = 480;
        IsfImgSetting.Window.x  = 600;
        IsfImgSetting.Window.y  = 300;
        #endif
        ImageUnit_SetParam(&ISF_ALG, ALG_PARAM_SET_IN2, (UINT32)&IsfImgSetting);

        // IPL2 path3
        memset(&IsfImgSetting, 0, sizeof(ISF_IMG_INFO));
        IsfImgSetting.ImgFmt = GX_IMAGE_PIXEL_FMT_YUV420_PACKED;
        IsfImgSetting.ImgSize.w = 640;
        IsfImgSetting.ImgSize.h = 480;
        ImageUnit_SetParam(&ISF_ALG, ALG_PARAM_SET_IN3, (UINT32)&IsfImgSetting);

        // IPL2 path4
        memset(&IsfImgSetting, 0, sizeof(ISF_IMG_INFO));
        IsfImgSetting.ImgFmt = GX_IMAGE_PIXEL_FMT_Y_ONLY;
        //#NT#2016/07/19#ML Cui-begin
        //#NT# DDD Changed input from IME2 Path3 to path4
        #if (MOVIE_DDD_FUNC)
        IsfImgSetting.ImgSize.w = 1280;
        IsfImgSetting.ImgSize.h = 720;
        IsfImgSetting.Window.w = 0;
        IsfImgSetting.Window.h = 0;
        IsfImgSetting.Window.x  = 0;
        IsfImgSetting.Window.y  = 0;
        #else
        IsfImgSetting.ImgSize.w = 1920;
        IsfImgSetting.ImgSize.h = 1080;
        IsfImgSetting.Window.w = 720;
        IsfImgSetting.Window.h = 480;
        IsfImgSetting.Window.x  = 600;
        IsfImgSetting.Window.y  = 300;
        #endif
        //#NT#2016/07/19#ML Cui-End
        if(nSensor==1)
            ImageUnit_SetParam(&ISF_ALG, ALG_PARAM_SET_IN2, (UINT32)&IsfImgSetting);
        else
            ImageUnit_SetParam(&ISF_ALG, ALG_PARAM_SET_IN4, (UINT32)&IsfImgSetting);

        #if (_ADAS_FUNC_)
        Func1_En |= IPL_FUNC_ADAS;
		//#NT#2017/01/23#Ben Wang -begin
		//#NT#Support ADAS in UVC_MULTIMEDIA_FUNC, referring to UsbMovieSFuncCfg.
        if (g_ADASFuncSupported== TRUE)
        {
            // Use ImageUnit_ALG
            CamMovieSFuncCfg |= (CAMMOVIE_SFUNC_IPL1_PATH3 | CAMMOVIE_SFUNC_IPL1_PATH4 | CAMMOVIE_SFUNC_IPL1_PATH4_PVEN);
            NetMovieSFuncCfg |= (NETMOVIE_SFUNC_IPL1_PATH3 | NETMOVIE_SFUNC_IPL1_PATH4);
            #if(UVC_MULTIMEDIA_FUNC == ENABLE)
            UsbMovieSFuncCfg |= (NVT_USBMOVIE_SFUNC_IPL1_PATH3 | NVT_USBMOVIE_SFUNC_IPL1_PATH4);
            #endif
        }
        #endif


        //#NT#2016/05/20#Yang Jin -begin
        //#NT#Support Dis dsp function
        #if (MOVIE_OT_FUNC||MOVIE_DISDSP_FUNC)
        // Use ImageUnit_ALG
        CamMovieSFuncCfg |= CAMMOVIE_SFUNC_IPL1_PATH3;
        NetMovieSFuncCfg |= NETMOVIE_SFUNC_IPL1_PATH3;
        #if(UVC_MULTIMEDIA_FUNC == ENABLE)
        UsbMovieSFuncCfg |= NVT_USBMOVIE_SFUNC_IPL1_PATH3;
        #endif
        #endif
        //#NT#2016/05/20#Yang Jin -end


        // Use ImageUnit_ALG
        //#NT#2016/07/19#ML Cui -Begin
        //#NT# DDD Changed input from IME2 Path3 to path4
        #if (MOVIE_DDD_FD_FUNC)
        CamMovieSFuncCfg |= CAMMOVIE_SFUNC_IPL2_PATH4 | CAMMOVIE_SFUNC_IPL2_PATH4_PVEN;
        NetMovieSFuncCfg |= NETMOVIE_SFUNC_IPL2_PATH4 | NETMOVIE_SFUNC_IPL2_PATH4_PVEN;
        #if(UVC_MULTIMEDIA_FUNC == ENABLE)
        UsbMovieSFuncCfg |= NVT_USBMOVIE_SFUNC_IPL2_PATH4 | NVT_USBMOVIE_SFUNC_IPL2_PATH4_PVEN;
        #endif
        #endif
        //#NT#2016/07/19#ML Cui -End


        // Use ImageUnit_ALG
        //#NT#2016/07/19#ML Cui -Begin
        //#NT# DDD Changed input from IME2 Path3 to path4
        #if (MOVIE_DDD_FUNC)
        if(nSensor==1)
        {
            CamMovieSFuncCfg |= CAMMOVIE_SFUNC_IPL1_PATH4 | CAMMOVIE_SFUNC_IPL1_PATH4_PVEN;
            NetMovieSFuncCfg |= NETMOVIE_SFUNC_IPL1_PATH4 | NETMOVIE_SFUNC_IPL1_PATH4_PVEN;
            #if(UVC_MULTIMEDIA_FUNC == ENABLE)
            UsbMovieSFuncCfg |= NVT_USBMOVIE_SFUNC_IPL1_PATH4 | NVT_USBMOVIE_SFUNC_IPL1_PATH4_PVEN;
            #endif
        }

        else
        {
            CamMovieSFuncCfg |= CAMMOVIE_SFUNC_IPL2_PATH4 | CAMMOVIE_SFUNC_IPL2_PATH4_PVEN;
            NetMovieSFuncCfg |= NETMOVIE_SFUNC_IPL2_PATH4 | NETMOVIE_SFUNC_IPL2_PATH4_PVEN;
            #if(UVC_MULTIMEDIA_FUNC == ENABLE)
            UsbMovieSFuncCfg |= NVT_USBMOVIE_SFUNC_IPL2_PATH4 | NVT_USBMOVIE_SFUNC_IPL2_PATH4_PVEN;
            #endif
        }
        #endif
        //#NT#2016/07/19#ML Cui -End

        //#NT#2016/07/06#CC Chao -begin
        //#NT#Support Dis dsp function
        #if (MOVIE_TSR_FUNC_)
        // Use ImageUnit_ALG
        CamMovieSFuncCfg |= CAMMOVIE_SFUNC_IPL1_PATH3;
        NetMovieSFuncCfg |= NETMOVIE_SFUNC_IPL1_PATH3;
        #if(UVC_MULTIMEDIA_FUNC == ENABLE)
        UsbMovieSFuncCfg |= NVT_USBMOVIE_SFUNC_IPL1_PATH3;
        #endif
        #endif
        //#NT#2016/07/06#CC Chao -end

        //#NT#2016/10/17#Bin Xiao -begin
        //#NT# Support Face Tracking Grading(FTG)
        #if (MOVIE_FTG_FUNC)
        // Use ImageUnit_ALG
        CamMovieSFuncCfg |= CAMMOVIE_SFUNC_IPL1_PATH3;
        NetMovieSFuncCfg |= NETMOVIE_SFUNC_IPL1_PATH3;
        #if(UVC_MULTIMEDIA_FUNC == ENABLE)
        UsbMovieSFuncCfg |= NVT_USBMOVIE_SFUNC_IPL1_PATH3;
        #endif
        #endif
        //#NT#2016/10/17#Bin Xiao -end


        ImageApp_CamMovie_Config(MOVIE_CFG_SFUNC_EN, CamMovieSFuncCfg);
        ImageApp_NetMovie_Config(NETMOVIE_CFG_SFUNC_EN, NetMovieSFuncCfg);
        #if(UVC_MULTIMEDIA_FUNC == ENABLE)
        ImageApp_UsbMovie_Config(NVT_USBMOVIE_CFG_SFUNC_EN, UsbMovieSFuncCfg);
        #endif
		//#NT#2017/01/23#Ben Wang -end
        ImageUnit_SetParam(&ISF_ImagePipe, PHOTO_PARAM_FUNC1, Func1_En);
        ImageUnit_SetParam(&ISF_ImagePipe, PHOTO_PARAM_FUNC2, Func2_En);
    }

    //#NT#2016/09/20#Bob Huang -begin
    //#NT#Support HDMI Display with 3DNR Out
    #if (_3DNROUT_FUNC == ENABLE)
    if(gb3DNROut)
    {
        ImageApp_CamMovie_Config(MOVIE_CFG_3DNROUT, TRUE);
    }
    else
    {
        ImageApp_CamMovie_Config(MOVIE_CFG_3DNROUT, FALSE);
    }
    #endif
    //#NT#2016/09/20#Bob Huang -end

    Movie_RegCB();

    #if (MOVIE_TEST_ENABLE == ENABLE) // test mode: using fixed YUV patterns
    {
        g_MediaRecTestMode.bEnable = TRUE;
        g_MediaRecTestMode.uiVidSrcType = MOVREC_SRC_FIXED_YUV;
        g_MediaRecTestMode.uiAudSrcType = MOVREC_SRC_NORMAL;

        // Set YUV information if using fixed YUV as video source (MEDIAREC_SRC_FIXED_YUV)
        if (g_MediaRecTestMode.uiVidSrcType == MOVREC_SRC_FIXED_YUV)
        {
            //Movie_SetYUV(UI_GetData(FL_MOVIE_SIZE));
        }

        MovRec_SetTestMode(&g_MediaRecTestMode);
    }
    #endif

    ImageRatioIdx = GetMovieSizeRatio(UI_GetData(FL_MOVIE_SIZE));
    ImageRatioSize = IMAGERATIO_SIZE[ImageRatioIdx];

//#NT#2016/02/22#Jeah Yen -begin
//#NT#Support IPL direction function
    if(System_GetEnableSensor() & SENSOR_1)
    {
        ImageUnit_Begin(&ISF_ImagePipe1, 0);
            ImageUnit_CfgAspect(ISF_IN1, ImageRatioSize.w, ImageRatioSize.h);


            #if(IPCAM_FUNC==ENABLE)
            //#NT#2016/04/27#Charlie Chang -begin
            //#NT# ip cam support mirror and flip
               int rotate_type=0;
               rotate_type = IPL_AlgGetUIInfo(0, IPL_SEL_IMAGEFLIP);
               if(rotate_type == SEL_IMAGEFLIP_DSP_H_V){
                   ImageUnit_CfgDirect(ISF_IN1, FLIP_H_V);
               }
               else if(rotate_type == SEL_IMAGEFLIP_DSP_H){
                   ImageUnit_CfgDirect(ISF_IN1, FLIP_H);
               }
               else if(rotate_type == SEL_IMAGEFLIP_DSP_V){
                   ImageUnit_CfgDirect(ISF_IN1, FLIP_V);
               }
               else{
                   ImageUnit_CfgDirect(ISF_IN1, FLIP_NONE);
               }
            //#NT#2016/04/27#Charlie Chang -end
            #else
            //#NT#2016/03/02#Niven Cho -begin
            //#NT#Use chip filp instead of sensor flip
            ImageUnit_CfgDirect(ISF_IN1, (UI_GetData(FL_MOVIE_SENSOR_ROTATE) == SEN_ROTATE_ON)? FLIP_V : FLIP_NONE);
            //#NT#2016/03/02#Niven Cho -end
            #endif

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
            //#NT#2016/06/03#Brain Yen -begin
            //#NT#for special aspect panel and pipview
            #if (PIP_VIEW_LR == ENABLE || _LCDTYPE_ == _LCDTYPE_TG078UW006A0_DSI_ || _LCDTYPE_ == _LCDTYPE_M935UX01M_SN75LVDS83B_||_LCDTYPE_ == _LCDTYPE_XJ040WV1402C_R61408_DSI_)
            ImageUnit_CfgAspect(ISF_IN1, DeviceRatioSize.w, DeviceRatioSize.h);
            #elif (SBS_VIEW_FUNC == ENABLE)
            //#NT#2017/03/03#Jeah Yen -begin
            //#NT# support sbs mode
            ImageUnit_CfgAspect(ISF_IN1, ImageRatioSize.w, ImageRatioSize.h);
            if(UI_GetData(FL_MOVIE_SBS_MODE) == MOVIE_SBS_LR)
            {
                ImageUnit_CfgAspect(ISF_IN1, ImageRatioSize.w << 1, ImageRatioSize.h);
            }
            else
            if(UI_GetData(FL_MOVIE_SBS_MODE) == MOVIE_SBS_UD)
            {
                ImageUnit_CfgAspect(ISF_IN1, ImageRatioSize.w, ImageRatioSize.h << 1);
            }
            //#NT#2017/03/03#Jeah Yen -end
            #else
            ImageUnit_CfgAspect(ISF_IN1, ImageRatioSize.w, ImageRatioSize.h);
            #endif
            //#NT#2016/06/03#Brain Yen -end
            ImageUnit_CfgWindow(ISF_IN1, 0, 0, 0, 0);  //window range = full device range
        ImageUnit_End();
    }
    if (System_GetEnableDisp() & DISPLAY_2)
    {
        ImageUnit_Begin(&ISF_CamDisp, 0);
            ImageUnit_CfgImgSize(ISF_IN2, 0, 0); //buffer size = full device size
            #if (SBS_VIEW_FUNC == ENABLE)
            //#NT#2017/03/03#Jeah Yen -begin
            //#NT# support sbs mode
            ImageUnit_CfgAspect(ISF_IN1, ImageRatioSize.w, ImageRatioSize.h);
            if(UI_GetData(FL_MOVIE_SBS_MODE) == MOVIE_SBS_LR)
            {
                ImageUnit_CfgAspect(ISF_IN1, ImageRatioSize.w << 1, ImageRatioSize.h);
            }
            else
            if(UI_GetData(FL_MOVIE_SBS_MODE) == MOVIE_SBS_UD)
            {
                ImageUnit_CfgAspect(ISF_IN1, ImageRatioSize.w, ImageRatioSize.h << 1);
            }
            //#NT#2017/03/03#Jeah Yen -end
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

    #if(IPCAM_FUNC==ENABLE)
    {
        //ISF_UserDraw ISF_IN1 is connect to ISF_ImagePipe1,
        // default max size is come from it's ISF_OUT1 or ISF_OUT2
        // user can set MAX_WIDTH, MAX_HEIGHT to change this value
        //(Here for IPCAM, it should be assign as max size of 3rd streaming, NOT and display)

        ISIZE  maxSize = {MOVIE_PATH2_MAX_W, MOVIE_PATH2_MAX_H};
        UINT32 ImageRatioIdx2;
        USIZE  ImageRatioSize2;

        ImageRatioIdx2 = GetMovieSizeRatio_2p(VIDENC_ID_2);
        ImageRatioSize2 = IMAGERATIO_SIZE[ImageRatioIdx2];

        ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_MAX_WIDTH, maxSize.w);
        ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_MAX_HEIGHT, maxSize.h);

        //(Here for IPCAM, it should be assign as "current" size of 2rd streaming)
        ImageUnit_Begin(&ISF_UserDraw, 0);
            ImageUnit_CfgImgSize(ISF_IN1, GetMovieSizeWidth_2p(VIDENC_ID_2,0), GetMovieSizeHeight_2p(VIDENC_ID_2,0));
            ImageUnit_CfgAspect(ISF_IN1, ImageRatioSize2.w, ImageRatioSize2.h);
            ImageUnit_CfgWindow(ISF_IN1, 0, 0, 0, 0);  //window range = full device range
        ImageUnit_End();
    }
    #endif

    //#NT#2016/09/21#Brain Yen -begin
    //#NT#for special aspect panel and pipview
    #if (PIP_VIEW_LR == ENABLE)
    {
        //enlarge UserDraw and IPL maxsize
        USIZE DeviceRatioSize= GxVideo_GetDeviceAspect(DOUT1); //16:5
        ISIZE devSize = GxVideo_GetDeviceSize(DOUT1); //1280x400
        //#NT#2016/12/22#Brain Yen -begin
        //#NT#Modify for WIFI record fail bug
        if(UI_GetData(FL_DUAL_CAM)>=DUALCAM_LR_16_9 && UI_GetData(FL_DUAL_CAM)<=DUALCAM_LR_BEHIND_FULL )
        {
            //for photo<->movie mode change bug
            #if (DUALCAM_PIP_BEHIND_FLIP == ENABLE)
            if(UI_GetData(FL_DUAL_CAM)==DUALCAM_LR_BEHIND_FULL)
                PipView_SetStyle(UI_GetData(FL_DUAL_CAM));
            #endif
            //#NT#2017/01/25#Brain Yen -begin
            //#NT#Modify for saving memory
            //#NT#2017/04/17#Brain Yen -begin
            //#NT#Fine fune only for 2nd path  autopower off
            //check ImageUnit_SetParam(&ISF_ImagePipe, IMAGEPIPE_PARAM_AUTOIPLOFF_EN, 1);
            UINT8 bISF_AutoIPLOff_En = FALSE;
            switch (UI_GetData(FL_MOVIE_SIZE_MENU))
            {
                case MOVIE_SIZE_FRONT_2880x2160P24:                                             // ADAS is NOT supported in these modes.
                case MOVIE_SIZE_FRONT_2560x1440P30:
                case MOVIE_SIZE_FRONT_2304x1296P30:
                case MOVIE_SIZE_FRONT_1920x1080P60:
                case MOVIE_SIZE_FRONT_1280x720P120:
                case MOVIE_SIZE_FRONT_1280x720P60:
                case MOVIE_SIZE_FRONT_640x480P240:
                    bISF_AutoIPLOff_En=TRUE;
                    break;
                default:
                    break;
            }

            if(bISF_AutoIPLOff_En==TRUE)
            {
                //for front sensor
                ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_MAX_WIDTH, devSize.w); // 1280
                ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_MAX_HEIGHT, devSize.h*ImageRatioSize.h*DeviceRatioSize.w/(DeviceRatioSize.h*ImageRatioSize.w)); // 400*9/5
                //for rear sensor
                ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_MAX_WIDTH2, 80); // sync ISF_UserDraw_Common_SyncSize
                ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_MAX_HEIGHT2, 60);
            }
            else
            {
                //for front sensor
                ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_MAX_WIDTH, devSize.w); // 1280
                ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_MAX_HEIGHT, devSize.h*ImageRatioSize.h*DeviceRatioSize.w/(DeviceRatioSize.h*ImageRatioSize.w)); // 400*9/5
                //for rear sensor
                ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_MAX_WIDTH2, devSize.w); // 1280
                ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_MAX_HEIGHT2, devSize.h*ImageRatioSize.h*DeviceRatioSize.w/(DeviceRatioSize.h*ImageRatioSize.w)); // 400*9/5
            }
            //#NT#2017/04/17#Brain Yen -end
            //#NT#2017/01/25#Brain Yen -end
        }
        else
        {
            ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_MAX_WIDTH, devSize.w); // 1280
            ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_MAX_HEIGHT, devSize.h); // 400*9/5
            //for rear sensor
            ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_MAX_WIDTH2, devSize.w); // 1280
            ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_MAX_HEIGHT2, devSize.h); // 400*9/5
        }
        //#NT#2016/12/22#Brain Yen -end
    }
    #endif
    //#NT#2016/09/21#Brain Yen -end

    #if (PIP_VIEW_FASTSWITCH == DISABLE) // Disable the second path of IPL if the front path only was chosen.
    ImageApp_CamMovie_Config(MOVIE_CFG_FRONTVIEW_IPL2_ENABLE, FALSE);
    #endif

#if (MOVIE_MULTI_RECORD_FUNC)
    if(GetMovieRecType_2p(UI_GetData(FL_MOVIE_SIZE))==MOVIE_REC_TYPE_DUAL
        || GetMovieRecType_2p(UI_GetData(FL_MOVIE_SIZE))==MOVIE_REC_TYPE_CLONE)
    {
        ImageApp_CamMovie_Config(MOVIE_CFG_RECORDCOUNT, 2); // dual Recording
    }
    else
    {
        ImageApp_CamMovie_Config(MOVIE_CFG_RECORDCOUNT, 1); // single Recording
    }
#else
    ImageApp_CamMovie_Config(MOVIE_CFG_RECORDCOUNT, 1); // single Recording
#endif

#if(1)//_PM_DEMO_)
SMediaRec_SetEncPath(0, VIDENC_PATH_D2D);
SMediaRec_SetEncPath(1, VIDENC_PATH_D2D);
#endif


#if (MOVIE_DIS == ENABLE)
    // DIS can only be enabled as RSC off
    if (UI_GetData(FL_MovieDisIndex) == MOVIE_DIS_ON &&
        UI_GetData(FL_MovieRSCIndex) == MOVIE_RSC_OFF)
    {
        MovieExe_IPL_SetIInfo(IPL_SEL_DISCOMP, SEL_DISCOMP_ON);
    }
    else
    {
        UI_SetData(FL_MovieDisIndex, MOVIE_DIS_OFF);
        MovieExe_IPL_SetIInfo(IPL_SEL_DISCOMP, SEL_DISCOMP_OFF);
    }
#endif

    #if (_ADAS_FUNC_ || MOVIE_FTG_FUNC)
    //#NT#2016/03/25#KCHong -begin
    //#NT#New ADAS
    dis_setDisViewRatio(1000);
    MovieExe_IPL_SetIInfo(IPL_SEL_DISCOMP, SEL_DISCOMP_ON);
    //#NT#2016/03/25#KCHong -end
    #endif

    uiFrameRate = GetMovieFrameRate(UI_GetData(FL_MOVIE_SIZE));

#if (MOVIE_RSC == ENABLE)
    if((UI_GetData(FL_MovieRSCIndex) == MOVIE_RSC_ON) &&
       (uiFrameRate  <= 120))
        MovieExe_RSC_SetSwitch(SEL_RSC_CHANGEMODE, SEL_RSC_ON);
    else
        MovieExe_RSC_SetSwitch(SEL_RSC_CHANGEMODE, SEL_RSC_OFF);
#endif

    if (uiFrameRate >= 240)
    {
        ImageApp_CamMovie_Config(MOVIE_CFG_IPL_MAXBUF, TRUE);
    }
    else
    {
        #if (_MODEL_DSC_ == _MODEL_TWIN_360_)
        ImageApp_CamMovie_Config(MOVIE_CFG_IPL_SETBUFCNT, 7); // 7 IPL display buffers for TWIN-360
        ImageApp_NetMovie_Config(NETMOVIE_CFG_IPL_SETBUFCNT, 7); // 7 IPL display buffers for TWIN-360
        #else
        ImageApp_CamMovie_Config(MOVIE_CFG_IPL_MAXBUF, FALSE);
        #endif
    }

    //#NT#2017/03/03#Jeah Yen -begin
    //#NT# support sbs mode
	#if 0 //(_MODEL_DSC_ == _MODEL_TWIN_360_)
	IPL_AlgSetUIInfo(0, IPL_SEL_SBS, SEL_SBS_LR);
	IPL_AlgSetUIInfo(1, IPL_SEL_SBS, SEL_SBS_LR);
	#endif
    //#NT#2017/03/03#Jeah Yen -end

#if (IPCAM_FUNC)
    MovRec_SetH264MaxQp(0,H264_MAX_QP_LEVEL_2);
    MovRec_SetH264MaxQp(1,H264_MAX_QP_LEVEL_2);
    MovRec_SetH264GopNum(60);
#endif

    //#NT#2013/02/27#Lincy Lin -begin
    //#NT#For capture in movie write file naming

    #if USE_FILEDB
    UI_SetData(FL_IsUseFileDB, 1);
    #else
    UI_SetData(FL_IsUseFileDB, 0);
    #endif
    useFileDB = UI_GetData(FL_IsUseFileDB);

    if(UI_GetData(FL_CardStatus)==CARD_INSERTED)
    {
        ImageUnit_SetParam(&ISF_CamFile, CAMFILE_PARAM_MOVIE_NAMERULE_EMPTY, FALSE);

        if (useFileDB)
        {
            FlowMovie_FileDBCreate();
        }
        else
        {
            ImageUnit_SetParam(&ISF_CamFile, CAMFILE_PARAM_USE_FILEDB, 0);
        }
    }
    else
    {
        ImageUnit_SetParam(&ISF_CamFile, CAMFILE_PARAM_MOVIE_NAMERULE_EMPTY, TRUE);
    }

    //#NT#2016/08/10#YongChang Qui -begin
    //#NT#Revert mistakenly disabled functions
    ImageUnit_SetParam(&ISF_Media, MEDIA_PARAM_DRAW_CB, (UINT32)MovieStamp_CopyData);// register movie stamp callback function
    #if !(IPCAM_FUNC && !MOVIE_AE_LOG)
    ImageUnit_SetParam(&ISF_Media, MEDIA_PARAM_DRAW_DIRECT_CB, (UINT32)MovieStamp_SetImeStamp);// register movie stamp callback function on Record Direct Path
    ImageUnit_SetParam(&ISF_Media, MEDIA_PARAM_AUDIOFILTER_CB, (UINT32)UIMovie_AudFilterProc);// register audio filtering callback function
    #endif
    ImageUnit_SetParam(&ISF_Media, MEDIA_PARAM_IMM_CB, (UINT32)UIMovie_ImmProc); // register instant events processing callback function
    ImageUnit_SetParam(&ISF_Media, MEDIA_PARAM_RAWENC_CB, (UINT32)UIMovie_RawEncCB);// register raw encode callback function
    //#NT#2016/08/10#YongChang Qui -end

    ImageUnit_SetParam(&ISF_Media, MEDIA_PARAM_MOVIEPRV_CHGMODE, (UINT32)TRUE);

#if (MEDIA_VASTREAM==ENABLE)
    //#NT#2016/08/11#Niven Cho -begin
    //#NT#IPCAM-NO-AUDIO
    #if(AUDIO_PLAY_FUNCTION)
    ImageUnit_SetParam(&ISF_Media, MEDIA_PARAM_VASTRM_MODE_EN, (UINT32)TRUE);
    //#NT#2016/12/22#HM Tseng -begin
    //#NT# Support AEC function
    #if(AUDIO_AEC_FUNC==ENABLE)
    MovRec_RegisterCB_Start(NvtAec_RecStart);
    MovRec_RegisterCB_Get(NvtAec_Proc);
    #endif
    //#NT#2016/12/22#HM Tseng -end
    #else
    ImageUnit_SetParam(&ISF_Media, MEDIA_PARAM_VASTRM_WOA_MODE_EN, (UINT32)TRUE);
    #endif
    //#NT#2016/08/11#Niven Cho -end
    ImageUnit_SetParam(&ISF_Media, MEDIA_PARAM_VASTRM_UPDATE_CB, (UINT32)UIVAStrm_UpdParamsCB);  // regiter callback function for VA stream update parameters.
#endif
//#NT#2016/03/04#Lincy Lin -begin
//#NT#Porting media 4.0 flow
#if (MEDIA_4_0==ENABLE)
    ImageUnit_SetParam(&ISF_Media, MEDIA_PARAM_MEDIA_4_0_MODE_EN, (UINT32)TRUE);
#endif
//#NT#2016/03/04#Lincy Lin -end

#if (SBS_VIEW_FUNC==ENABLE)
    //#NT#2017/03/03#Jeah Yen -begin
    //#NT# support sbs mode
    if(UI_GetData(FL_MOVIE_SBS_MODE) == MOVIE_SBS_OFF)
    {
        ImageUnit_SetParam(&ISF_Media, MEDIA_PARAM_DUAL_SENSOR_SBS, (UINT32)FALSE);
        ImageUnit_SetParam(&ISF_CamDisp, CAMDISP_PARAM_SBSVIEW_EN, (UINT32)FALSE);
        ImageUnit_SetParam(&ISF_ImagePipe, IMAGEPIPE_PARAM_SBSVIEW_EN, (UINT32)FALSE);
        ImageUnit_SetParam(&ISF_ImagePipe, IMAGEPIPE_PARAM_SBSVIEW_MODE, SEL_SBS_OFF);
        MovieExe_IPL_SetIInfo(IPL_SEL_SBS, SEL_SBS_OFF);
    }
    else
    {
        ImageUnit_SetParam(&ISF_Media, MEDIA_PARAM_DUAL_SENSOR_SBS, (UINT32)TRUE);
        ImageUnit_SetParam(&ISF_CamDisp, CAMDISP_PARAM_SBSVIEW_EN, (UINT32)TRUE);
        ImageUnit_SetParam(&ISF_ImagePipe, IMAGEPIPE_PARAM_SBSVIEW_EN, (UINT32)TRUE);
        if(UI_GetData(FL_MOVIE_SBS_MODE) == MOVIE_SBS_LR)
        {
            ImageUnit_SetParam(&ISF_ImagePipe, IMAGEPIPE_PARAM_SBSVIEW_MODE, SEL_SBS_LR);
            MovieExe_IPL_SetIInfo(IPL_SEL_SBS, SEL_SBS_LR);
        }
        if(UI_GetData(FL_MOVIE_SBS_MODE) == MOVIE_SBS_UD)
        {
            ImageUnit_SetParam(&ISF_ImagePipe, IMAGEPIPE_PARAM_SBSVIEW_MODE, SEL_SBS_UD);
            MovieExe_IPL_SetIInfo(IPL_SEL_SBS, SEL_SBS_UD);
        }
    }
    //#NT#2017/03/03#Jeah Yen -end
#endif

    if(System_GetState(SYS_STATE_POWERON) == SYSTEM_POWERON_NORMAL)
    {
#if ((POWERON_FAST_WIFI != ENABLE) && (POWERON_FAST_RECORD == ENABLE))
        ImageUnit_SetParam(&ISF_Media, MEDIA_PARAM_MOVIEPRV_CHGMODE, (UINT32)FALSE);
#endif
    }

#if(IPCAM_FUNC==ENABLE)
	#if(IPCAM_UVC_FUNC == ENABLE)
    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_UVC)
    {
        static UVAC_VID_RESO gUIUvacVidReso[] =
        {
            { 848,   480,   1,      30,      0,      0}
        };
        //#define NVT_UI_FREQUENCY_48K                    0x00BB80   //48k
		#define NVT_UI_FREQUENCY_32K					32000 // it's better to be the same as sound effect!
        static UINT32 gUIUvacAudSampleRate[] =
        {
            NVT_UI_FREQUENCY_32K
        };

        UVAC_VID_RESO_ARY uvacVidResoAry = {0};
        UVAC_AUD_SAMPLERATE_ARY uvacAudSampleRateAry = {0};
        UVAC_VEND_DEV_DESC gUIUvacDevDesc = {0};
        UINT32 uiUvcSizePath;
        ImageApp_UsbMovie_Config(NVT_USBMOVIE_CFG_REC_COUNT, 1); // single Recording
        uiUvcSizePath = 0;

        //default use UVAC H264
        ImageApp_UsbMovie_Config(NVT_USBMOVIE_CFG_MEDIA_FMT,NVT_USBMOVIE_MEDIA_H264);

        gUIUvacVidReso[0].width = GetMovieSizeWidth_2p(uiUvcSizePath, UI_GetData(FL_MOVIE_SIZE));
        gUIUvacVidReso[0].height = GetMovieSizeHeight_2p(uiUvcSizePath, UI_GetData(FL_MOVIE_SIZE));
        gUIUvacVidReso[0].fps[0] = GetMovieFrameRate_2p(uiUvcSizePath, UI_GetData(FL_MOVIE_SIZE));
        ImageUnit_SetParam(&ISF_UsbUVAC, IMGUNIT_UVAC_CFG_VID_FMT_TYPE, IMGUNIT_UVAC_VIDEO_FORMAT_H264_ONLY);


        DBG_DUMP("^BUVAC MULTIMEDIA movie index[%d]=%dx%dP%d_%dx%dP%d\r\n",  UI_GetData(FL_MOVIE_SIZE),
                                                                GetMovieSizeWidth_2p(0, UI_GetData(FL_MOVIE_SIZE)),
                                                                GetMovieSizeHeight_2p(0, UI_GetData(FL_MOVIE_SIZE)),
                                                                GetMovieFrameRate_2p(0, UI_GetData(FL_MOVIE_SIZE)),
                                                                GetMovieSizeWidth_2p(1, UI_GetData(FL_MOVIE_SIZE)),
                                                                GetMovieSizeHeight_2p(1, UI_GetData(FL_MOVIE_SIZE)),
                                                                GetMovieFrameRate_2p(1, UI_GetData(FL_MOVIE_SIZE)));

		ImageApp_UsbMovie_Config(NVT_USBMOVIE_CFG_OPEN_REC_IMMEDIATELY,FALSE);
        uvacVidResoAry.aryCnt = 1;//MOVIE_SIZE_ID_MAX;
        uvacVidResoAry.pVidResAry = &gUIUvacVidReso[0];
        ImageUnit_SetParam(&ISF_UsbUVAC, IMGUNIT_UVAC_CFG_VID_RESO_ARY, (UINT32)&uvacVidResoAry);
		//#NT#2016/09/22#Ben Wang -begin
		//#NT#Always use PCM for UVAC
        Movie_SetAudChannel(MOVREC_AUDTYPE_STEREO);
        Movie_SetAudSampleRate(gUIUvacAudSampleRate[0]);
        ImageUnit_SetParam(&ISF_Media, MEDIA_PARAM_ASTRM_PCM_OUTPUT, TRUE);
        //#NT#2016/09/22#Ben Wang -end
        uvacAudSampleRateAry.aryCnt = 1;
        uvacAudSampleRateAry.pAudSampleRateAry = &gUIUvacAudSampleRate[0];
        ImageUnit_SetParam(&ISF_UsbUVAC, IMGUNIT_UVAC_CFG_AUD_SAMPLERATE_ARY, (UINT32)&uvacAudSampleRateAry);

        xUSBMakerInit_UVAC(&gUIUvacDevDesc);
        ImageUnit_SetParam(&ISF_UsbUVAC, IMGUNIT_UVAC_CFG_UVAC_VEND_DEV_DESC, (UINT32)&gUIUvacDevDesc);

        Movie_SetRecParam();

        ImageUnit_SetParam(&ISF_UsbUVAC, IMGUNIT_UVAC_CFG_CDC_ENABLE, FALSE);
        ImageUnit_SetParam(&ISF_UsbUVAC, IMGUNIT_UVAC_CFG_CDC_PSTN_REQUEST_CB, (UINT32)CdcPstnReqCB);
        ImageApp_UsbMovie_Config(NVT_USBMOVIE_CFG_MODE,NVT_USBMOVIE_MODE_MULTIMEDIA);
        ImageApp_UsbMovie_Open();
    }
    else
#endif
    {
        if(System_GetState(SYS_STATE_POWERON) == SYSTEM_POWERON_SAFE)
            MovieStamp_Disable(); // disable movie stamp until record start
        if(_g_bFirstMovie){TM_BOOT_BEGIN("sensor","ipl_open");}

        //UI_SetData(FL_MOVIE_SIZE_MENU, MOVIE_SIZE_FRONT_1280x720P30);
        //UI_SetData(FL_WIFI_MOVIE_APP_PREVIEW_SIZE, WIFI_MOVIE_APP_PREVIEW_SIZE_WVGA);
        Movie_SetRecParam();
        {
            #if MEDIA_VASTREAM
            //#NT#2016/07/07#David Tsai -begin
            //#NT#Two stream mode for IP Cam
            #if TWO_STREAM_MODE
            BOOL pathen[3]={TRUE,TRUE,FALSE};
            #else
            BOOL pathen[3]={TRUE,TRUE,TRUE};
            #endif
            //#NT#2016/07/07#David Tsai -end
            #else
            BOOL pathen[3]={TRUE,TRUE,FALSE};
            #endif
            ImageApp_IPCam_Open(pathen);

        }
        //ImageApp_IPCam_RecStart(IPCAM_CHANNEL_ID_1);
        //ImageApp_IPCam_RecStart(IPCAM_CHANNEL_ID_2);

//#NT#2016/08/29#HM Tseng -begin
//#NT#For audio play testing mode
#if (AUDIO_PLAY_FUNCTION == ENABLE && AUDIO_PLAY_TEST == DISABLE)
        System_AudioOutStart();//start output
#endif
//#NT#2016/08/29#HM Tseng -end

    }
//#NT#2017/02/24#Ben Wang -begin
//#NT#Fix that UVC_MULTIMEDIA_FUNC depends on WIFI_AP_FUNC.
#elif((WIFI_AP_FUNC==ENABLE) || (UVC_MULTIMEDIA_FUNC == ENABLE))
//#NT#2017/02/24#Ben Wang -end
    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
    {
        ImageApp_NetMovie_Config(NETMOVIE_CFG_REC_PARAM_CB,(UINT32)Movie_SetRecParam);
        if(GetMovieRecType_2p(UI_GetData(FL_MOVIE_SIZE))==MOVIE_REC_TYPE_FRONT)
        {
            ImageApp_NetMovie_Config(NETMOVIE_CFG_REC_COUNT, 1); // single Recording
        }
        else
        {
            ImageApp_NetMovie_Config(NETMOVIE_CFG_REC_COUNT, 2); // dual Recording
            //#if (YOUKU_SDK == ENABLE && SAVE_DUAL_STREAM_FUNC == ENABLE)
            #if (SAVE_DUAL_STREAM_FUNC == ENABLE || SMALL_CLONE_MOVIE == ENABLE)
            ImageApp_NetMovie_Config(NETMOVIE_CFG_WRITE_COUNT, 2); // dual file
            #endif
        }

        {
            USIZE HttpLviewSize;
            HttpLviewSize = MOVIE_HTTPLVIEW_SIZE[ImageRatioIdx];
            ImageUnit_Begin(&ISF_NetHTTP, 0);
                ImageUnit_CfgAspect(ISF_IN1, ImageRatioSize.w, ImageRatioSize.h);
                ImageUnit_CfgImgSize(ISF_IN1, HttpLviewSize.w, HttpLviewSize.h);
                ImageUnit_CfgAspect(ISF_IN2, ImageRatioSize.w, ImageRatioSize.h);
                ImageUnit_CfgImgSize(ISF_IN2, HttpLviewSize.w, HttpLviewSize.h);
            ImageUnit_End();
            PhotoExe_CfgNetHttp(HttpLviewSize);
        }

        #if(MOVIE_LIVEVIEW==HTTP_LIVEVIEW)
        ImageApp_NetMovie_Config(NETMOVIE_CFG_MODE,NETMOVIE_TYPE_HTTP);
        #elif (MOVIE_LIVEVIEW==RTSP_LIVEVIEW)
        {
            if(System_GetEnableSensor() == (SENSOR_1|SENSOR_2))
            {
                #if (SBS_VIEW_FUNC==ENABLE)
                ImageApp_NetMovie_Config(NETMOVIE_CFG_MODE,NETMOVIE_TYPE_RTSP);
                #else
                ImageApp_NetMovie_Config(NETMOVIE_CFG_MODE,NETMOVIE_TYPE_HTTP);
                #endif
                ImageApp_NetMovie_Config(NETMOVIE_CFG_WRITE_COUNT, 2); // dual file

            }
            else
            {
                ImageApp_NetMovie_Config(NETMOVIE_CFG_MODE,NETMOVIE_TYPE_RTSP);
                ImageApp_NetMovie_Config(NETMOVIE_CFG_WRITE_COUNT, 1); // single file
                //#NT#2016/08/10#Brain Yen -begin
                //#NT#add for WIFI double movie record
                #if (SAVE_DUAL_STREAM_FUNC == ENABLE || SMALL_CLONE_MOVIE == ENABLE)
                ImageApp_NetMovie_Config(NETMOVIE_CFG_WRITE_COUNT, 2); // double file
                #endif
                //#NT#2016/08/10#Brain Yen -end
            }
        }
        //#NT#2016/05/06#Jeah Yen -begin
        //#NT#Support IOTP2P
        #elif (MOVIE_LIVEVIEW==IOTP2P_LIVEVIEW)
        {
            if(System_GetEnableSensor() == (SENSOR_1|SENSOR_2))
                ImageApp_NetMovie_Config(NETMOVIE_CFG_MODE,NETMOVIE_TYPE_IOTP2P_MJPG);
            else
                ImageApp_NetMovie_Config(NETMOVIE_CFG_MODE,NETMOVIE_TYPE_IOTP2P);
        }
        //#NT#2016/05/06#Jeah Yen -end
        #endif
        ImageUnit_SetParam(&ISF_NetRTSP, NETRTSP_PARAM_MAX_CLIENT, RTSP_MAX_CLIENT);
        //#NT#2016/06/13#Niven Cho -begin
        //set VI WMM category, refer to http://sourcedaddy.com/windows-7/selecting-dscp-values.html
        ImageUnit_SetParam(&ISF_NetRTSP, NETRTSP_PARAM_TOS, 0xC0); //wifi-team suggest it as 0xC0
        //#NT#2016/06/13#Niven Cho -end
#if (POWERON_FAST_RECORD == ENABLE)
        if(!bIsWiFiRecorded)
        {
            UI_SetData(FL_WIFI_AUTO_RECORDING, WIFI_AUTO_RECORDING_ON);
        }
#endif
        if(!bIsWiFiRecorded && UI_GetData(FL_WIFI_AUTO_RECORDING) == WIFI_AUTO_RECORDING_ON)
        {

            UINT32 MaxTime = MovieExe_GetFreeRecSec();
            if(((MaxTime<=3) && (SysGetFlag(FL_MOVIE_CYCLIC_REC) == MOVIE_CYCLICREC_OFF)) || (UI_GetData(FL_CardStatus)!=CARD_INSERTED))
            {
                UI_SetData(FL_WIFI_MOVIE_FMT,WIFI_RTSP_LIVEVIEW);
                //#NT#2016/07/11#Charlie Chang -begin
                //#NT# save rec status in FL_MOVIE_REC flag
                UI_SetData(FL_MOVIE_REC,0);
                //#NT#2016/07/11#Charlie Chang -end
                ImageApp_NetMovie_Config(NETMOVIE_CFG_OPEN_REC_IMMEDIATELY,FALSE);
                DBG_ERR("recTime<3 sec\r\n");
            }
            else
            {
                UI_SetData(FL_WIFI_MOVIE_FMT,WIFI_RTSP_REC);
                //#NT#2016/07/11#Charlie Chang -begin
                //#NT# save rec status in FL_MOVIE_REC flag
                UI_SetData(FL_MOVIE_REC,1);
                //#NT#2016/07/11#Charlie Chang -end
                ImageApp_NetMovie_Config(NETMOVIE_CFG_OPEN_REC_IMMEDIATELY,TRUE);
            }
            //#NT#2016/07/15#Isiah chang -begin
            //#NT#Check FL_WIFI_AUTO_RECORDING option only one time after boot.
            //bAutoRecordEn = TRUE;
            //#NT#2016/07/15#Isiah chang -end
        }
        else
        {
            UI_SetData(FL_WIFI_MOVIE_FMT,WIFI_RTSP_LIVEVIEW);
            //#NT#2016/07/11#Charlie Chang -begin
            //#NT# save rec status in FL_MOVIE_REC flag
            UI_SetData(FL_MOVIE_REC,0);
            //#NT#2016/07/11#Charlie Chang -end
            ImageApp_NetMovie_Config(NETMOVIE_CFG_OPEN_REC_IMMEDIATELY,FALSE);
        }

        //#NT#2016/07/15#Isiah chang -begin
        //#NT#Check FL_WIFI_AUTO_RECORDING option only one time after boot.
        bIsWiFiRecorded = TRUE;
        //#NT#2016/07/15#Isiah chang -end

        Movie_SetRecParam();
        if(_g_bFirstMovie){TM_BOOT_BEGIN("sensor","ipl_open");}
#if (POWERON_FAST_RECORD == ENABLE)
        if(_g_bFirstMovie)
            _g_bFirstMovieRec = TRUE;
        if(_g_bFirstMovieRec){TM_BOOT_BEGIN("flow","rec");}
#endif
        //#NT#2016/05/24#Charlie Chang -begin
        //#NT# using stream1 to send data
#if (MOVIE_NET_USE_STREAM_1== ENABLE)
        ImageApp_NetMovie_Config(NETMOVIE_CFG_USE_STEAM_1,TRUE);
        ImageUnit_SetParam(&ISF_NetRTSP, NETRTSP_PARAM_MEDIA_SOURCE, RTSPNVT_MEDIA_SOURCE_PATH1_FIRST);
#endif
        //#NT#2016/05/23#Charlie Chang -end
        ImageApp_NetMovie_Open();
debug_msg("\r\n_ADAS_FUNC_...1111\r\n");

//#NT#2016/07/19#KCHong#[0105559] -begin
//#NT#Start ADAS function if NETMOVIE_CFG_OPEN_REC_IMMEDIATELY is set
#if _ADAS_FUNC_
    debug_msg("\r\g_ADASFuncSupported = %d\r\n", g_ADASFuncSupported);
    if ((ImageApp_NetMovie_GetConfig(NETMOVIE_CFG_OPEN_REC_IMMEDIATELY) == TRUE) && (g_ADASFuncSupported == TRUE))
    {
        memset(&g_AdasRltOSD, 0, sizeof(ADAS_APPS_RESULT_INFO));
        memset(&g_AdasRltVideo, 0, sizeof(ADAS_APPS_RESULT_INFO));
		g_AdasRltOSD.LdwsDspRsltInfo.Failure = LDWS_FAILURE_TRUE;//g_AdasRltOSD.LdwsRsltInfo.Failure = LDWS_FAILURE_TRUE;
        
        g_AdasRltVideo.LdwsRsltInfo.Failure = LDWS_FAILURE_TRUE;
        #if (_SNG_FUNC_ == ENABLE)
        memset(&g_SnGROIOSD, 0, sizeof(ADAS_SNG_ROI_INFO));
        memset(&g_SnGROIVideo, 0, sizeof(ADAS_SNG_ROI_INFO));
        #endif
        ADAS_AppsStart();
        #if defined(YQCONFIG_DEBUG_MESSAGE_SUPPORT)
        debug_msg("^RADAS_AppsStart--\r\n");
        #endif
    }
#endif
//#NT#2016/07/19#KCHong#[0105559] -end

        //WifiCmd_Done(WIFIFLAG_MODE_DONE,E_OK);
        Ux_PostEvent(NVTEVT_WIFI_EXE_MODE_DONE,1,E_OK);
    }
//#NT#2016/05/31#Ben Wang -begin
//#NT#Add UVC multimedia function.
#if(UVC_MULTIMEDIA_FUNC == ENABLE)
    else if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_UVC)
    {
    	//#NT#2017/02/24#Ben Wang -begin
        //#NT#Don't record file by default in SYS_SUBMODE_UVC.
        //static BOOL bAutoRecordEn = FALSE;
        //#NT#2017/02/24#Ben Wang -end
        static UVAC_VID_RESO gUIUvacVidReso[] =
        {
            { 848,   480,   1,      30,      0,      0}
        };
        //#define NVT_UI_FREQUENCY_48K                    0x00BB80   //48k
		#define NVT_UI_FREQUENCY_32K					32000 // it's better to be the same as sound effect!
        static UINT32 gUIUvacAudSampleRate[] =
        {
            NVT_UI_FREQUENCY_32K
        };

        UVAC_VID_RESO_ARY uvacVidResoAry = {0};
        UVAC_AUD_SAMPLERATE_ARY uvacAudSampleRateAry = {0};
        UVAC_VEND_DEV_DESC gUIUvacDevDesc = {0};
        UINT32 uiUvcSizePath;
        //ImageApp_UsbMovie_Config(NETMOVIE_CFG_REC_PARAM_CB,(UINT32)Movie_SetRecParam);
        if(GetMovieRecType_2p(UI_GetData(FL_MOVIE_SIZE))==MOVIE_REC_TYPE_FRONT)
        {
            ImageApp_UsbMovie_Config(NVT_USBMOVIE_CFG_REC_COUNT, 1); // single Recording
            uiUvcSizePath = 0;
        }
        else
        {
            ImageApp_UsbMovie_Config(NVT_USBMOVIE_CFG_REC_COUNT, 2); // dual Recording
            uiUvcSizePath = 1;
        }

        if(System_GetEnableSensor() == (SENSOR_1|SENSOR_2))
        {
            #if (SBS_VIEW_FUNC==ENABLE)
            ImageApp_UsbMovie_Config(NVT_USBMOVIE_CFG_MEDIA_FMT,NVT_USBMOVIE_MEDIA_H264);
            //use the same stream size with file for UVC stream
            uiUvcSizePath = 0;
            ImageApp_UsbMovie_Config(NVT_USBMOVIE_CFG_REC_COUNT, 1); // single Recording
            #else
            ImageApp_UsbMovie_Config(NVT_USBMOVIE_CFG_WRITE_COUNT,2);
            #if 1//dual UVAC
            ImageApp_UsbMovie_Config(NVT_USBMOVIE_CFG_CHANNEL, UVAC_CHANNEL_2V2A);
            ImageApp_UsbMovie_Config(NVT_USBMOVIE_CFG_MEDIA_FMT,NVT_USBMOVIE_MEDIA_H264);
            #else//single UVAC with PIP view
            ImageApp_UsbMovie_Config(NVT_USBMOVIE_CFG_MEDIA_FMT,NVT_USBMOVIE_MEDIA_MJPEG);
            #endif
            #endif
        }
        else
        {
            //default use UVAC H264
            ImageApp_UsbMovie_Config(NVT_USBMOVIE_CFG_MEDIA_FMT,NVT_USBMOVIE_MEDIA_H264);
	        //ImageApp_UsbMovie_Config(NVT_USBMOVIE_CFG_MEDIA_FMT,NVT_USBMOVIE_MEDIA_MJPEG);
        }
        if(NVT_USBMOVIE_MEDIA_MJPEG == ImageApp_UsbMovie_GetConfig(NVT_USBMOVIE_CFG_MEDIA_FMT))
        {
            USIZE HttpLviewSize;
            HttpLviewSize = MOVIE_HTTPLVIEW_SIZE[ImageRatioIdx];
            ImageUnit_SetParam(&ISF_NetHTTP, NETHTTP_PARAM_MAX_IMAGE_WIDTH1, HttpLviewSize.w);
		    ImageUnit_SetParam(&ISF_NetHTTP, NETHTTP_PARAM_MAX_IMAGE_HEIGHT1, HttpLviewSize.h);
            ImageUnit_Begin(&ISF_NetHTTP, 0);
                ImageUnit_CfgAspect(ISF_IN1, ImageRatioSize.w, ImageRatioSize.h);
                ImageUnit_CfgImgSize(ISF_IN1, HttpLviewSize.w, HttpLviewSize.h);
                ImageUnit_CfgAspect(ISF_IN2, ImageRatioSize.w, ImageRatioSize.h);
                ImageUnit_CfgImgSize(ISF_IN2, HttpLviewSize.w, HttpLviewSize.h);
            ImageUnit_End();
            PhotoExe_CfgNetHttp(HttpLviewSize);
            gUIUvacVidReso[0].width = HttpLviewSize.w;
            gUIUvacVidReso[0].height = HttpLviewSize.h;
            gUIUvacVidReso[0].fps[0] = 30;//ISF_NetHTTP only support fps=30 ?
            ImageUnit_SetParam(&ISF_UsbUVAC, IMGUNIT_UVAC_CFG_VID_FMT_TYPE, IMGUNIT_UVAC_VIDEO_FORMAT_MJPEG_ONLY);
        }
        else//NVT_USBMOVIE_MEDIA_H264
        {
            gUIUvacVidReso[0].width = GetMovieSizeWidth_2p(uiUvcSizePath, UI_GetData(FL_MOVIE_SIZE));
            gUIUvacVidReso[0].height = GetMovieSizeHeight_2p(uiUvcSizePath, UI_GetData(FL_MOVIE_SIZE));
            gUIUvacVidReso[0].fps[0] = GetMovieFrameRate_2p(uiUvcSizePath, UI_GetData(FL_MOVIE_SIZE));
            ImageUnit_SetParam(&ISF_UsbUVAC, IMGUNIT_UVAC_CFG_VID_FMT_TYPE, IMGUNIT_UVAC_VIDEO_FORMAT_H264_ONLY);
        }


        DBG_DUMP("^BUVAC MULTIMEDIA movie index[%d]=%dx%dP%d_%dx%dP%d\r\n",  UI_GetData(FL_MOVIE_SIZE),
                                                                GetMovieSizeWidth_2p(0, UI_GetData(FL_MOVIE_SIZE)),
                                                                GetMovieSizeHeight_2p(0, UI_GetData(FL_MOVIE_SIZE)),
                                                                GetMovieFrameRate_2p(0, UI_GetData(FL_MOVIE_SIZE)),
                                                                GetMovieSizeWidth_2p(1, UI_GetData(FL_MOVIE_SIZE)),
                                                                GetMovieSizeHeight_2p(1, UI_GetData(FL_MOVIE_SIZE)),
                                                                GetMovieFrameRate_2p(1, UI_GetData(FL_MOVIE_SIZE)));
		//#NT#2017/02/24#Ben Wang -begin
        //#NT#Don't record file by default in SYS_SUBMODE_UVC.
		#if 0
        if(!bAutoRecordEn && UI_GetData(FL_WIFI_AUTO_RECORDING) == WIFI_AUTO_RECORDING_ON)
        {

            UINT32 MaxTime = MovieExe_GetFreeRecSec();
            if((MaxTime<=3) && (SysGetFlag(FL_MOVIE_CYCLIC_REC) == MOVIE_CYCLICREC_OFF))
            {
                UI_SetData(FL_WIFI_MOVIE_FMT,WIFI_RTSP_LIVEVIEW);
                ImageApp_UsbMovie_Config(NVT_USBMOVIE_CFG_OPEN_REC_IMMEDIATELY,FALSE);
                DBG_ERR("recTime<3 sec\r\n");
            }
            else
            {
                UI_SetData(FL_WIFI_MOVIE_FMT,WIFI_RTSP_REC);
                ImageApp_UsbMovie_Config(NVT_USBMOVIE_CFG_OPEN_REC_IMMEDIATELY,TRUE);
            }
            bAutoRecordEn = TRUE;
        }
        else
        #endif
        //#NT#2017/02/24#Ben Wang -end
        {
            UI_SetData(FL_WIFI_MOVIE_FMT,WIFI_RTSP_LIVEVIEW);
            ImageApp_UsbMovie_Config(NVT_USBMOVIE_CFG_OPEN_REC_IMMEDIATELY,FALSE);
        }

        uvacVidResoAry.aryCnt = 1;//MOVIE_SIZE_ID_MAX;
        uvacVidResoAry.pVidResAry = &gUIUvacVidReso[0];
        ImageUnit_SetParam(&ISF_UsbUVAC, IMGUNIT_UVAC_CFG_VID_RESO_ARY, (UINT32)&uvacVidResoAry);
		//#NT#2016/09/22#Ben Wang -begin
		//#NT#Always use PCM for UVAC
        Movie_SetAudChannel(MOVREC_AUDTYPE_STEREO);
        Movie_SetAudSampleRate(gUIUvacAudSampleRate[0]);
        ImageUnit_SetParam(&ISF_Media, MEDIA_PARAM_ASTRM_PCM_OUTPUT, TRUE);
        //#NT#2016/09/22#Ben Wang -end
        uvacAudSampleRateAry.aryCnt = 1;
        uvacAudSampleRateAry.pAudSampleRateAry = &gUIUvacAudSampleRate[0];
        ImageUnit_SetParam(&ISF_UsbUVAC, IMGUNIT_UVAC_CFG_AUD_SAMPLERATE_ARY, (UINT32)&uvacAudSampleRateAry);

        xUSBMakerInit_UVAC(&gUIUvacDevDesc);
        ImageUnit_SetParam(&ISF_UsbUVAC, IMGUNIT_UVAC_CFG_UVAC_VEND_DEV_DESC, (UINT32)&gUIUvacDevDesc);

        Movie_SetRecParam();
        if(_g_bFirstMovie){TM_BOOT_BEGIN("sensor","ipl_open");}
#if (POWERON_FAST_RECORD == ENABLE)
        if(_g_bFirstMovie)
            _g_bFirstMovieRec = TRUE;
        if(_g_bFirstMovieRec){TM_BOOT_BEGIN("flow","rec");}
#endif
        ImageUnit_SetParam(&ISF_UsbUVAC, IMGUNIT_UVAC_CFG_CDC_ENABLE, TRUE);
        ImageUnit_SetParam(&ISF_UsbUVAC, IMGUNIT_UVAC_CFG_CDC_PSTN_REQUEST_CB, (UINT32)CdcPstnReqCB);
        ImageApp_UsbMovie_Config(NVT_USBMOVIE_CFG_MODE,NVT_USBMOVIE_MODE_MULTIMEDIA);
#if 0
        if(System_GetState(SYS_STATE_PREVSUBMODE)==SYS_SUBMODE_UVC)
        {//just restore media, IPL
            ImageApp_UsbMovie_RecStop();
        }
        else
#endif
        {
            ImageApp_UsbMovie_Open();
        }
        //Ux_PostEvent(NVTEVT_WIFI_EXE_MODE_DONE,1,E_OK);
        //#NT#2017/01/23#Ben Wang -begin
		//#NT#Support ADAS in UVC_MULTIMEDIA_FUNC.
		debug_msg("\r\n_ADAS_FUNC_...2222\r\n");
		#if _ADAS_FUNC_
         debug_msg("\r\g_ADASFuncSupported = %d\r\n", g_ADASFuncSupported);
		if ((ImageApp_UsbMovie_GetConfig(NVT_USBMOVIE_CFG_OPEN_REC_IMMEDIATELY) == TRUE) && (g_ADASFuncSupported == TRUE))
	    {
	        memset(&g_AdasRltOSD, 0, sizeof(ADAS_APPS_RESULT_INFO));
	        memset(&g_AdasRltVideo, 0, sizeof(ADAS_APPS_RESULT_INFO));
	        g_AdasRltOSD.LdwsRsltInfo.Failure = LDWS_FAILURE_TRUE;
	        g_AdasRltVideo.LdwsRsltInfo.Failure = LDWS_FAILURE_TRUE;
	        #if (_SNG_FUNC_ == ENABLE)
	        memset(&g_SnGROIOSD, 0, sizeof(ADAS_SNG_ROI_INFO));
	        memset(&g_SnGROIVideo, 0, sizeof(ADAS_SNG_ROI_INFO));
	        #endif
	        ADAS_AppsStart();
            debug_msg("\r\nstart adas...2222\r\n");
	    }
		#endif
		//#NT#2017/01/23#Ben Wang -end
    }
#endif
//#NT#2016/05/31#Ben Wang -end
    else
    {
        Movie_SetRecParam();
        if(System_GetState(SYS_STATE_POWERON) == SYSTEM_POWERON_SAFE)
            MovieStamp_Disable(); // disable movie stamp until record start
        if(_g_bFirstMovie){TM_BOOT_BEGIN("sensor","ipl_open");}
        ImageApp_CamMovie_Open();
    }
#else
    {
        Movie_SetRecParam();
        if(System_GetState(SYS_STATE_POWERON) == SYSTEM_POWERON_SAFE)
            MovieStamp_Disable(); // disable movie stamp until record start
        if(_g_bFirstMovie){TM_BOOT_BEGIN("sensor","ipl_open");}
        ImageApp_CamMovie_Open();
    }
#endif

#if UCTRL_FUNC
    UCtrlAppMovie_Open();
#endif


//#NT#2016/03/18#Niven Cho -begin
//#NT#improve the using of gprof
#if (_GPROF_TOOL_ == _GPROF_TOOL_ON_)
    //take the place of SxCmd Temp Buffer after ImageApp_Open, becasue it has changed in Open()
    SxCmd_RegTempMemFunc(System_GetGprofBuffer);
#endif
//#NT#2016/03/18#Niven Cho -end

    return NVTEVT_CONSUME;
}

INT32 MovieExe_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    #if MOVIE_TSR_FUNC_
    // unlock TSR
    TSR_Lock(TRUE);
    TSR_UnInit();
    #endif
    #if MOVIE_FD_FUNC_
    // unlock FD
    FD_Lock(TRUE);
    FD_UnInit();
    #endif
    //#NT#2016/05/04#ML Cui -begin
    //#NT#Add DDD UnInit into MovieExe_OnClose
    #if MOVIE_DDD_FUNC
    DDD_Lock(TRUE);
    DDD_UnInit();
    #endif
    //#NT#2016/05/04#ML Cui -end

    // disable movie stamp
    MovieStamp_Setup(
            0,
            STAMP_OFF,
            0,
            0,
            NULL);

#if (MOVIE_MULTI_RECORD_FUNC)
    if(GetMovieRecType_2p(UI_GetData(FL_MOVIE_SIZE))==MOVIE_REC_TYPE_DUAL
        || GetMovieRecType_2p(UI_GetData(FL_MOVIE_SIZE))==MOVIE_REC_TYPE_CLONE)
    {
        MovieStamp_Setup(
                1,
                STAMP_OFF,
                0,
                0,
                NULL);
    }
#endif

    MovieStamp_Disable();
//#NT#2016/06/28#KCHong -begin
//#NT#DbgLog in GPS section
#if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
    mp4log_Close();
#endif
//#NT#2016/06/28#KCHong -end
#if(IPCAM_FUNC==ENABLE)
	#if (IPCAM_UVC_FUNC == ENABLE)
	if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_UVC)
    {
        ImageApp_UsbMovie_Close();
    }
	else
	#endif
    {
//#NT#2016/08/29#HM Tseng -begin
//#NT#For audio play testing mode
#if (AUDIO_PLAY_FUNCTION == ENABLE && AUDIO_PLAY_TEST == DISABLE)
        System_AudioOutStop();//stop output
#endif
//#NT#2016/08/29#HM Tseng -end
        ImageApp_IPCam_Close();
    }
//#NT#2017/02/24#Ben Wang -begin
//#NT#Fix that UVC_MULTIMEDIA_FUNC depends on WIFI_AP_FUNC.
#elif((WIFI_AP_FUNC==ENABLE) || (UVC_MULTIMEDIA_FUNC == ENABLE))
//#NT#2017/02/24#Ben Wang -end
    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
    {
        //Ux_SendEvent(&UIMovieRecObjCtrl, NVTEVT_STOP_RTSP, 0);
        //ImageApp_CamMovie_RegRTSPSetBSCB(NULL);
        //ImageUnit_SetParam(&ISF_Media, MEDIA_PARAM_BITSTREAM_CB, 0);  //obsoleted
        //RtspNvt_Close();
        ImageApp_NetMovie_Close();
    }
//#NT#2016/05/31#Ben Wang -begin
//#NT#Add UVC multimedia function.
#if(UVC_MULTIMEDIA_FUNC == ENABLE)
    else if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_UVC)
    {
#if 0
        if(System_GetState(SYS_STATE_NEXTSUBMODE)==SYS_SUBMODE_UVC)
        {//just close media, IPL but UVC
            ImageApp_UsbMovie_LviewStop();
        }
        else
#endif
        {
            ImageApp_UsbMovie_Close();
        }
    }
#endif
//#NT#2016/05/31#Ben Wang -end
    else
    {
        ImageApp_CamMovie_Close();
    }
#else
    {
        ImageApp_CamMovie_Close();
    }
#endif

//#NT#2016/07/26#Niven Cho -begin
//#NT#IPCAM-DSP
#if (_DSP_TYPE_ == _DSP_FREERTOS_) && (IPCAM_FUNC==ENABLE)
    ROI_Stop();
#endif
//#NT#2016/07/26#Niven Cho -end

    //call default
    Ux_DefaultEvent(pCtrl,NVTEVT_EXE_CLOSE,paramNum,paramArray);
    return NVTEVT_CONSUME;
}

//-------------------------------------------------------------------------------------------------
INT32 MovieExe_OnMovieSize(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;
    UINT32 uiQuality;
    UINT32 uiFrameRate;
    UINT32 bChangeMode=FALSE;

    if(paramNum > 0)
        uiSelect = paramArray[0];

    if(paramNum > 1)
        bChangeMode = paramArray[1];

    DBG_IND("uiSelect %d bChangeMode %d \r\n",uiSelect,bChangeMode);

    UI_SetData(FL_MOVIE_SIZE, uiSelect);

    //for demo,set preview size the same as record size
    #if 0  // Remove useless code.
    UI_SetData(FL_WIFI_MOVIE_APP_PREVIEW_SIZE, uiSelect);
    DBG_IND("set wifi preview size %d\r\n",uiSelect);
    #endif
    uiQuality = UI_GetData(FL_MOVIE_QUALITY);
#if (MOVIE_MULTI_RECORD_FUNC)
    uiFrameRate = GetMovieFrameRate_2p(0, uiSelect);
#else
    uiFrameRate = GetMovieFrameRate(uiSelect);
#endif

#if (MOVIE_RSC == ENABLE)
    // enable RSC if necessary (only support frame rate smaller than 60fps)
    //rsc tmp
    if ((UI_GetData(FL_MovieRSCIndex) == MOVIE_RSC_ON) &&
        (uiFrameRate <= 120))
    {
        MovieExe_RSC_SetSwitch(SEL_RSC_RUNTIME, SEL_RSC_ON);
    }
    else
    {
        MovieExe_RSC_SetSwitch(SEL_RSC_RUNTIME, SEL_RSC_OFF);
    }
#endif

    // set IPL parameters
    //MOVIE_SIZE_TAG
    switch (uiSelect)
    {
        case MOVIE_SIZE_FRONT_2880x2160P24:
        {
            MovieExe_IPL_SetIInfo(IPL_SEL_VIDEO_SIZE, SEL_VIDEOSIZE_2880x2160);
            MovieExe_IPL_SetIInfo(IPL_SEL_VIDEO_FPS, SEL_VIDEOFPS_24);
            break;
        }
        case MOVIE_SIZE_FRONT_2560x1440P30:
        {
            MovieExe_IPL_SetIInfo(IPL_SEL_VIDEO_SIZE, SEL_VIDEOSIZE_2560x1440);
            MovieExe_IPL_SetIInfo(IPL_SEL_VIDEO_FPS, SEL_VIDEOFPS_30);
            break;
        }
        #if 0
        case MOVIE_SIZE_FRONT_2304x1296P30:
        {
            DBG_ERR("IPL is not implemented MOVIE_SIZE_2304x1296P30 yet!!\r\n");
            break;
            MovieExe_IPL_SetIInfo(IPL_SEL_VIDEO_SIZE, SEL_VIDEOSIZE_1920x1080);
            MovieExe_IPL_SetIInfo(IPL_SEL_VIDEO_FPS, SEL_VIDEOFPS_30);
            break;
        }

        case MOVIE_SIZE_FRONT_1920x1080P96:
        {
            DBG_ERR("IPL is not implemented MOVIE_SIZE_1080P96 yet!!\r\n");
            break;
            MovieExe_IPL_SetIInfo(IPL_SEL_VIDEO_SIZE, SEL_VIDEOSIZE_1920x1080);
            MovieExe_IPL_SetIInfo(IPL_SEL_VIDEO_FPS, SEL_VIDEOFPS_96);
            break;
        }
        #endif

        case MOVIE_SIZE_FRONT_1920x1080P60:
        {
            MovieExe_IPL_SetIInfo(IPL_SEL_VIDEO_SIZE, SEL_VIDEOSIZE_1920x1080);
            MovieExe_IPL_SetIInfo(IPL_SEL_VIDEO_FPS, SEL_VIDEOFPS_60);
            break;
        }

        case MOVIE_SIZE_FRONT_1920x1080P30:
        case MOVIE_SIZE_DUAL_1920x1080P30_1920x1080P30:
        case MOVIE_SIZE_CLONE_1920x1080P30_1280x720P30:
        case MOVIE_SIZE_CLONE_1920x1080P30_1920x1080P30:
        {
            MovieExe_IPL_SetIInfo(IPL_SEL_VIDEO_SIZE, SEL_VIDEOSIZE_1920x1080);
            MovieExe_IPL_SetIInfo(IPL_SEL_VIDEO_FPS, SEL_VIDEOFPS_30);
            break;
        }

        case MOVIE_SIZE_FRONT_1280x720P120:
        {
            MovieExe_IPL_SetIInfo(IPL_SEL_VIDEO_SIZE, SEL_VIDEOSIZE_1280x720);
            MovieExe_IPL_SetIInfo(IPL_SEL_VIDEO_FPS, SEL_VIDEOFPS_120);
            break;
        }

        case MOVIE_SIZE_FRONT_1280x720P60:
        {
            MovieExe_IPL_SetIInfo(IPL_SEL_VIDEO_SIZE, SEL_VIDEOSIZE_1280x720);
            MovieExe_IPL_SetIInfo(IPL_SEL_VIDEO_FPS, SEL_VIDEOFPS_60);
            break;
        }

        case MOVIE_SIZE_FRONT_1280x720P30:
        {
            MovieExe_IPL_SetIInfo(IPL_SEL_VIDEO_SIZE, SEL_VIDEOSIZE_1280x720);
            MovieExe_IPL_SetIInfo(IPL_SEL_VIDEO_FPS, SEL_VIDEOFPS_30);
            break;
        }

        case MOVIE_SIZE_CLONE_1920x1080P60_640x360P30:
        {
            MovieExe_IPL_SetIInfo(IPL_SEL_VIDEO_SIZE, SEL_VIDEOSIZE_1920x1080);
            MovieExe_IPL_SetIInfo(IPL_SEL_VIDEO_FPS, SEL_VIDEOFPS_60);
            break;
        }

        case MOVIE_SIZE_FRONT_640x480P240: // use IME scale
        {
            MovieExe_IPL_SetIInfo(IPL_SEL_VIDEO_SIZE, SEL_VIDEOSIZE_640x480);
            MovieExe_IPL_SetIInfo(IPL_SEL_VIDEO_FPS, SEL_VIDEOFPS_240);
            break;
        }

        default: //Others that smaller than 720P, use ime scale
        {
            MovieExe_IPL_SetIInfo(IPL_SEL_VIDEO_SIZE, SEL_VIDEOSIZE_1920x1080);
            MovieExe_IPL_SetIInfo(IPL_SEL_VIDEO_FPS, SEL_VIDEOFPS_30);
            break;
        }
    }

    /*
    if (uiSelect == MOVIE_SIZE_1080P_FULLRES) // for full resolution input 1080p30
    {
        MovieExe_IPL_SetIInfo(IPL_SEL_VIDEO_FPS,SEL_VIDEOFPS_FULLRES);
    }
    else
    {
        switch (uiFrameRate)
        {
        case 60: // 60fps
            MovieExe_IPL_SetIInfo(IPL_SEL_VIDEO_FPS,SEL_VIDEOFPS_60);
            break;

        case 120: // 120fps
            MovieExe_IPL_SetIInfo(IPL_SEL_VIDEO_FPS,SEL_VIDEOFPS_120);
            break;

        default: // 30fps
            MovieExe_IPL_SetIInfo(IPL_SEL_VIDEO_FPS,SEL_VIDEOFPS_30);
            break;
        }
    }
    */

    // Change Image Ratio
    Ux_SendEvent(0, NVTEVT_EXE_IMAGE_RATIO, 1, GetMovieSizeRatio(UI_GetData(FL_MOVIE_SIZE)));

    if (bChangeMode)
        Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, System_GetState(SYS_STATE_CURRMODE));
    else
        DBG_WRN("should change mode\r\n");

    return NVTEVT_CONSUME;
}

static USIZE MovieExe_RatioSizeConvert(USIZE* devSize, USIZE* devRatio, USIZE* Imgratio)
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
//#NT#2016/06/08#Lincy Lin -begin
//#NT#Implement generic OSD and video drawing mechanism for ALG function
static void MovieExe_CalcFDDispCord(URECT *dispCord)
{
    UINT32 ImageRatioIdx = 0;
    USIZE  ImageRatioSize={0};
    URECT  fdDispCoord;
    ISIZE  dev1size;
    USIZE  dev1Ratio;
    USIZE  finalSize={0};

    ImageRatioIdx = GetMovieSizeRatio(UI_GetData(FL_MOVIE_SIZE));
    ImageRatioSize = IMAGERATIO_SIZE[ImageRatioIdx];

    //1.get current device size (current mode)
    dev1size = GxVideo_GetDeviceSize(DOUT1);
    //2.get current device aspect Ratio
    dev1Ratio = GxVideo_GetDeviceAspect(DOUT1);
    finalSize = MovieExe_RatioSizeConvert((USIZE *)&dev1size,&dev1Ratio,&ImageRatioSize);
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

static void MovieExe_CalcOSDDispCord(URECT *dispCord)
{
    UINT32 ImageRatioIdx = 0;
    USIZE  ImageRatioSize={0};
    URECT  OsdDispCoord;
    ISIZE  dev1size;
    USIZE  dev1Ratio;
    USIZE  finalSize={0};

    ImageRatioIdx = GetMovieSizeRatio(UI_GetData(FL_MOVIE_SIZE));
    ImageRatioSize = IMAGERATIO_SIZE[ImageRatioIdx];

    //1.get current device size (current mode)
    dev1size.w = OSD_W;
    dev1size.h = OSD_H;
    //2.get current device aspect Ratio
    //#NT#2016/06/16#Lincy Lin -begin
    //#NT#Fix osd display ratio mismatch video bug
    //dev1Ratio.w = OSD_W;
    //dev1Ratio.h = OSD_H;
    dev1Ratio = GxVideo_GetDeviceAspect(DOUT1);
    //#NT#2016/06/16#Lincy Lin -end

    finalSize = MovieExe_RatioSizeConvert((USIZE *)&dev1size,&dev1Ratio,&ImageRatioSize);
    OsdDispCoord.w = finalSize.w;
    OsdDispCoord.h = finalSize.h;

    if (OsdDispCoord.w == (UINT32)dev1size.w)
    {
        OsdDispCoord.x = 0;
        OsdDispCoord.y = (dev1size.h - finalSize.h)>>1;
    }
    else
    {
        OsdDispCoord.y = 0;
        OsdDispCoord.x = (dev1size.w - finalSize.w)>>1;

    }
    *dispCord = OsdDispCoord;
}
//#NT#2016/06/08#Lincy Lin -end

//-------------------------------------------------------------------------------------------------
INT32 MovieExe_OnImageRatio(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32  ImageRatioIdx = 0;
    USIZE   ImageRatioSize = {0};
    //#NT#2016/06/03#Brain Yen -begin
    //#NT#for special aspect panel and pipview
    #if (PIP_VIEW_LR == ENABLE || _LCDTYPE_ == _LCDTYPE_TG078UW006A0_DSI_ || _LCDTYPE_ == _LCDTYPE_M935UX01M_SN75LVDS83B_||_LCDTYPE_ == _LCDTYPE_XJ040WV1402C_R61408_DSI_)
    USIZE DeviceRatioSize= GxVideo_GetDeviceAspect(DOUT1);
    #endif
    //#NT#2016/06/03#Brain Yen -end

    DBG_IND("\r\n");

    if (paramNum > 0)
        ImageRatioIdx = paramArray[0]; // parameter 0: image size
    DBG_MSG("ImageRatioIdx =%d\r\n", ImageRatioIdx);

    //check if imagestream is already open?
    if(ImageStream_IsOpen() != ISF_OK)
    {
        // ignore this change
        return NVTEVT_CONSUME;
    }

    if (ImageRatioIdx >=IMAGERATIO_MAX_CNT)
    {
        DBG_ERR("ImageRatioIdx =%d\r\n", ImageRatioIdx);
        return NVTEVT_CONSUME;
    }

    ImageRatioSize = IMAGERATIO_SIZE[ImageRatioIdx];
    DBG_MSG("ImageRatioSize=%d:%d\r\n", ImageRatioSize.w, ImageRatioSize.h);
    //#NT#2016/06/08#Lincy Lin -begin
    //#NT#Implement generic OSD and video drawing mechanism for ALG function
    MovieExe_CalcFDDispCord(&gMovieFdDispCord);
    MovieExe_CalcOSDDispCord(&gMovieOsdDispCord);
    //#NT#2016/06/08#Lincy Lin -end

    //1. Set Display image ratio
    if (System_GetEnableDisp() & DISPLAY_1)
    {
        ImageUnit_Begin(&ISF_CamDisp, 0);
            ImageUnit_CfgImgSize(ISF_IN1, 0, 0); //buffer size = full device size
            //#NT#2016/06/03#Brain Yen -begin
            //#NT#for special aspect panel and pipview
            #if (PIP_VIEW_LR == ENABLE || _LCDTYPE_ == _LCDTYPE_TG078UW006A0_DSI_ || _LCDTYPE_ == _LCDTYPE_M935UX01M_SN75LVDS83B_||_LCDTYPE_ == _LCDTYPE_XJ040WV1402C_R61408_DSI_)
            ImageUnit_CfgAspect(ISF_IN1, DeviceRatioSize.w, DeviceRatioSize.h);
			#elif (SBS_VIEW_FUNC == ENABLE)
            //#NT#2017/03/03#Jeah Yen -begin
            //#NT# support sbs mode
            ImageUnit_CfgAspect(ISF_IN1, ImageRatioSize.w, ImageRatioSize.h);
            if(UI_GetData(FL_MOVIE_SBS_MODE) == MOVIE_SBS_LR)
            {
                ImageUnit_CfgAspect(ISF_IN1, ImageRatioSize.w << 1, ImageRatioSize.h);
            }
            else
            if(UI_GetData(FL_MOVIE_SBS_MODE) == MOVIE_SBS_UD)
            {
                ImageUnit_CfgAspect(ISF_IN1, ImageRatioSize.w, ImageRatioSize.h << 1);
            }
            //#NT#2017/03/03#Jeah Yen -end
            #else
            ImageUnit_CfgAspect(ISF_IN1, ImageRatioSize.w, ImageRatioSize.h);
            #endif
            //#NT#2016/06/03#Brain Yen -end
            ImageUnit_CfgWindow(ISF_IN1, 0, 0, 0, 0);  //window range = full device range
        ImageUnit_End();
    }
    if (System_GetEnableDisp() & DISPLAY_2)
    {
        ImageUnit_Begin(&ISF_CamDisp, 0);
            ImageUnit_CfgImgSize(ISF_IN2, 0, 0); //buffer size = full device size
            #if (SBS_VIEW_FUNC == ENABLE)
            //#NT#2017/03/03#Jeah Yen -begin
            //#NT# support sbs mode
            ImageUnit_CfgAspect(ISF_IN2, ImageRatioSize.w, ImageRatioSize.h);
            if(UI_GetData(FL_MOVIE_SBS_MODE) == MOVIE_SBS_LR)
            {
                ImageUnit_CfgAspect(ISF_IN2, ImageRatioSize.w << 1, ImageRatioSize.h);
            }
            else
            if(UI_GetData(FL_MOVIE_SBS_MODE) == MOVIE_SBS_UD)
            {
                ImageUnit_CfgAspect(ISF_IN2, ImageRatioSize.w, ImageRatioSize.h << 1);
            }
            //#NT#2017/03/03#Jeah Yen -end
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

    //2. Set Preview image ratio, and direction
//#NT#2016/02/22#Jeah Yen -begin
//#NT#Support IPL direction function
    if(System_GetEnableSensor() & SENSOR_1)
    {
        ImageUnit_Begin(&ISF_ImagePipe1, 0);
            ImageUnit_CfgAspect(ISF_IN1, ImageRatioSize.w, ImageRatioSize.h);

            #if(IPCAM_FUNC==ENABLE)
            //#NT#2016/04/27#Charlie Chang -begin
            //#NT# ip cam support mirror and flip
               int rotate_type=0;
               rotate_type = IPL_AlgGetUIInfo(0, IPL_SEL_IMAGEFLIP);
               if(rotate_type == SEL_IMAGEFLIP_DSP_H_V){
                   ImageUnit_CfgDirect(ISF_IN1, FLIP_H_V);
               }
               else if(rotate_type == SEL_IMAGEFLIP_DSP_H){
                   ImageUnit_CfgDirect(ISF_IN1, FLIP_H);
               }
               else if(rotate_type == SEL_IMAGEFLIP_DSP_V){
                   ImageUnit_CfgDirect(ISF_IN1, FLIP_V);
               }
               else{
                   ImageUnit_CfgDirect(ISF_IN1, FLIP_NONE);
               }
            //#NT#2016/04/27#Charlie Chang -end
            #else
            //#NT#2016/03/02#Niven Cho -begin
            //#NT#Use chip filp instead of sensor flip
            ImageUnit_CfgDirect(ISF_IN1, (UI_GetData(FL_MOVIE_SENSOR_ROTATE) == SEN_ROTATE_ON)? FLIP_V : FLIP_NONE);
            //#NT#2016/03/02#Niven Cho -end
            #endif

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

    return NVTEVT_CONSUME;
}

//-------------------------------------------------------------------------------------------------
INT32 MovieExe_OnMovieQuality(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiQuality = 0;
    UINT32 uiSize;

    if(paramNum > 0)
        uiQuality = paramArray[0];

    UI_SetData(FL_MOVIE_QUALITY, uiQuality);
    uiSize = UI_GetData(FL_MOVIE_SIZE);

    MovRec_ChangeParameter(MOVREC_RECPARAM_TARGETRATE, GetMovieTargetBitrate(uiSize, uiQuality), 0, 0);

    return NVTEVT_CONSUME;
}

//-------------------------------------------------------------------------------------------------
INT32 MovieExe_OnWB(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if (paramNum)
        uiSelect = paramArray[0];

    UI_SetData(FL_WB, uiSelect);
    MovieExe_IPL_SetIInfo(IPL_SEL_WBMODE, Get_WBValue(uiSelect));

    return NVTEVT_CONSUME;
}

//-------------------------------------------------------------------------------------------------
INT32 MovieExe_OnColor(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if (paramNum)
        uiSelect = paramArray[0];

    UI_SetData(FL_MOVIE_COLOR, uiSelect);
    MovieExe_IPL_SetIInfo(IPL_SEL_IMAGEEFFECT, Get_ColorValue(uiSelect));

    return NVTEVT_CONSUME;
}

//-------------------------------------------------------------------------------------------------
INT32 MovieExe_OnEV(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if (paramNum)
        uiSelect = paramArray[0];

    UI_SetData(FL_EV, uiSelect);
    //photo and movie use the same EV value,should syncronize photo EV setting
    Photo_SetUserIndex(PHOTO_USR_EV,uiSelect);
    MovieExe_IPL_SetIInfo(IPL_SEL_AEEV, Get_EVValue(uiSelect));

    return NVTEVT_CONSUME;
}
//-------------------------------------------------------------------------------------------------
//#NT#2016/06/03#Charlie Chang -begin
//#NT# add contrast ,audio in, flip mirror, quality set
INT32 MovieExe_OnContrast(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{

    if(paramNum != 1){
        DBG_ERR("paramNum =%d error!!!\r\n",paramNum);
        return NVTEVT_CONSUME;
    }
    UINT32 uiSelect = 0;
    uiSelect = paramArray[0];
    if( uiSelect >= MOVIE_CONTRAST_ID_MAX){
        DBG_ERR("Contrast uiSelect =%d error\r\n",uiSelect);
        return NVTEVT_CONSUME;
    }
    IPL_AlgSetUIInfo(0, IPL_SEL_CONTRAST, uiSelect);

    uiSelect = IPL_AlgGetUIInfo(0, IPL_SEL_CONTRAST);
    DBG_DUMP("Contrast:%d\r\n",uiSelect);
    UI_SetData(FL_MOVIE_CONTRAST, uiSelect);

    return NVTEVT_CONSUME;
}
INT32 MovieExe_OnAudioIn(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{

    DBG_ERR("MovieExe_OnAudioin not implement need to do!!!!!!!!!!\r\n");

    return NVTEVT_CONSUME;
}
INT32 MovieExe_OnAudioInSR(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{

    DBG_ERR("MovieExe_OnAudioInSR not implement need to do!!!!!!!!!!\r\n");

    return NVTEVT_CONSUME;
}
INT32 MovieExe_OnFipMirror(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{

    if(paramNum < 1){
        DBG_ERR("paramNum =%d error!!!\r\n",paramNum);
        return NVTEVT_CONSUME;
    }
    UINT32 uiSelect = 0;
    uiSelect = paramArray[0];
    UINT32 ipl_id = 0;

    if( uiSelect >= MOVIE_FLIP_MIRROR_ID_MAX){
        DBG_ERR("flip value=%d error\r\n",uiSelect);
        return NVTEVT_CONSUME;
    }
    UI_SetData(FL_MOVIE_CONTRAST, uiSelect);
    if(uiSelect == MOVIE_FLIP_MIRROR_NONE){
        uiSelect= SEL_IMAGEFLIP_DSP_OFF;
    }
    IPL_AlgSetUIInfo(ipl_id, IPL_SEL_IMAGEFLIP, uiSelect);

    ImageUnit_Begin(&ISF_ImagePipe1, 0);

    int rotate_type=0;
    rotate_type = IPL_AlgGetUIInfo(ipl_id, IPL_SEL_IMAGEFLIP);
    if(rotate_type == SEL_IMAGEFLIP_DSP_H_V){
        DBG_DUMP("movie flip_H_V\r\n");
        ImageUnit_CfgDirect(ISF_IN1, FLIP_H_V);
    }
    else if(rotate_type == SEL_IMAGEFLIP_DSP_H){
        DBG_DUMP("movie flip_H\r\n");
        ImageUnit_CfgDirect(ISF_IN1, FLIP_H);
    }
    else if(rotate_type == SEL_IMAGEFLIP_DSP_V){
        DBG_DUMP("movie flip_V\r\n");
        ImageUnit_CfgDirect(ISF_IN1, FLIP_V);
    }
    else{
        ImageUnit_CfgDirect(ISF_IN1, FLIP_NONE);
        DBG_DUMP("movie flip_NONE\r\n");
    }
    ImageUnit_End();

    return NVTEVT_CONSUME;
}
INT32 MovieExe_OnQualitySet(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{

    if(paramNum < 1){
        DBG_ERR("paramNum =%d error!!!\r\n",paramNum);
        return NVTEVT_CONSUME;
    }
    UINT32 uiSelect = 0;
    uiSelect = paramArray[0];

    switch(uiSelect){

        case MOVIE_QUALITY_SET_MAX:
            UI_SetData(FL_MOVIE_SIZE, MOVIE_SIZE_FRONT_1920x1080P60);
            break;
        case MOVIE_QUALITY_SET_HIGH:
            UI_SetData(FL_MOVIE_SIZE, MOVIE_SIZE_FRONT_1280x720P30);
            break;
        case MOVIE_QUALITY_SET_MIDDLE:
            UI_SetData(FL_MOVIE_SIZE, MOVIE_SIZE_FRONT_848x480P30);
            break;
        case MOVIE_QUALITY_SET_LOW:
            UI_SetData(FL_MOVIE_SIZE, MOVIE_SIZE_FRONT_640x480P30);
            break;
        case MOVIE_QUALITY_SET_MIN:
            UI_SetData(FL_MOVIE_SIZE, MOVIE_SIZE_FRONT_320x240P30);
            break;
        default:
            DBG_ERR("uiSelect =%d error!! \r\n",uiSelect);
            return NVTEVT_CONSUME;
    }
    //reopen movie mode
    Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_MOVIE);
    return NVTEVT_CONSUME;
}
//#NT#2016/06/03#Charlie Chang -end

//-------------------------------------------------------------------------------------------------
//#NT#2017/03/03#Jeah Yen -begin
//#NT# support sbs mode
INT32 MovieExe_OnSBSMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    if(paramNum < 1){
        DBG_ERR("paramNum =%d error!!!\r\n",paramNum);
        return NVTEVT_CONSUME;
    }
    UINT32 uiSelect = 0;
    uiSelect = paramArray[0];

    UI_SetData(FL_MOVIE_SBS_MODE, uiSelect);

    return NVTEVT_CONSUME;
}
//#NT#2017/03/03#Jeah Yen -end

//-------------------------------------------------------------------------------------------------
INT32 MovieExe_OnMovieAudio(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if (paramNum)
        uiSelect = paramArray[0];

    UI_SetData(FL_MOVIE_AUDIO, uiSelect);

    if (uiSelect == MOVIE_AUDIO_OFF)
    {
        ImageUnit_SetParam(&ISF_Media, MEDIA_PARAM_AUDIOVOL, RECMOVIE_AUD_OFF); //set mute!
        DBG_DUMP("===Movie Rec Set Vol = %d\r\n", RECMOVIE_AUD_OFF);
    }
    else
    {
        UINT32 current_vol = UI_GetData(FL_MovieAudioRecIndex);
        ImageUnit_SetParam(&ISF_Media, MEDIA_PARAM_AUDIOVOL, current_vol); //restore to current vol
        DBG_DUMP("===Movie Rec Set Vol = %d\r\n", current_vol);
    }

    return NVTEVT_CONSUME;
}

INT32 MovieExe_OnMovieAudioRecVolume(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    UINT32 current_vol = UI_GetData(FL_MovieAudioRecIndex);
    if(paramNum > 0)
        uhSelect = paramArray[0];
    DBG_IND("+-idx=%d\r\n",uhSelect);
    if (current_vol != uhSelect)
    {
        UI_SetData(FL_MovieAudioRecIndex,uhSelect);
        ImageUnit_SetParam(&ISF_Media, MEDIA_PARAM_AUDIOVOL, uhSelect); //set current vol
        DBG_DUMP("===Movie Rec Set Vol = %d\r\n", uhSelect);
    }
    return NVTEVT_CONSUME;
}

INT32 MovieExe_OnMovieAudioRec(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    UINT32 current_audio_stream = UI_GetData(FL_MovieAudioRec);
    if(paramNum > 0)
        uhSelect = paramArray[0];
    DBG_IND("+-idx=%d\r\n",uhSelect);
    if (current_audio_stream != uhSelect)
    {
        UI_SetData(FL_MovieAudioRec,uhSelect);
    }
    return NVTEVT_CONSUME;
}

INT32 MovieExe_OnDigitalZoom(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];
    DBG_IND("+-MovieExe_OnDigitalZoom:idx=%d\r\n",uhSelect);
    UI_SetData(FL_Dzoom, uhSelect);
    if (uhSelect == DZOOM_OFF)
    {
        UI_SetData(FL_DzoomIndex, DZOOM_10X);
    }
    return NVTEVT_CONSUME;
}

INT32 MovieExe_OnDis(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if (paramNum)
    {
        uiSelect = paramArray[0];
    }

    DBG_IND("idx=%d\r\n", uiSelect);

    UI_SetData(FL_MovieDisIndex, uiSelect);

    //if (UI_GetData(FL_ModeIndex) == DSC_MODE_MOVIE)
    {
#if (MOVIE_DIS == ENABLE)
        if (uiSelect == MOVIE_DIS_ON)
        {
            // disable RSC
            UI_SetData(FL_MovieRSCIndex, MOVIE_RSC_OFF);
#if (MOVIE_RSC == ENABLE)
            //rsc tmp
            MovieExe_RSC_SetSwitch(SEL_RSC_RUNTIME, SEL_RSC_OFF);
#endif
            // enable DIS
            MovieExe_IPL_SetIInfo(IPL_SEL_DISCOMP, SEL_DISCOMP_ON);
        }
        else
        {
            MovieExe_IPL_SetIInfo(IPL_SEL_DISCOMP, SEL_DISCOMP_OFF);
        }
#endif
    }
    return NVTEVT_CONSUME;
}

INT32 MovieExe_OnMCTF(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if (paramNum)
        uiSelect = paramArray[0];

    UI_SetData(FL_MovieMCTFIndex, uiSelect);

    return NVTEVT_CONSUME;
}

INT32 MovieExe_OnEdge(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if (paramNum)
        uiSelect = paramArray[0];
    UI_SetData(FL_EDGE, uiSelect);

    if (uiSelect == MOVIE_EDGE_ON)
    {
        MovieExe_IPL_SetIInfo(IPL_SEL_IPPEDGE, SEL_IPPEDGE_ON);
    }
    else
    {
        MovieExe_IPL_SetIInfo(IPL_SEL_IPPEDGE, SEL_IPPEDGE_OFF);
    }
    return NVTEVT_CONSUME;
}

INT32 MovieExe_OnNR(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if (paramNum)
        uiSelect = paramArray[0];

    UI_SetData(FL_NR, uiSelect);

    if (uiSelect == MOVIE_NR_ON)
    {
        MovieExe_IPL_SetIInfo(IPL_SEL_IPPNR, SEL_IPPNR_ON);
    }
    else
    {
        MovieExe_IPL_SetIInfo(IPL_SEL_IPPNR, SEL_IPPNR_OFF);
    }
    return NVTEVT_CONSUME;
}

INT32 MovieExe_OnRSC(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;
    UINT32 uiFrameRate;

    if (paramNum)
        uiSelect = paramArray[0];

    UI_SetData(FL_MovieRSCIndex, uiSelect);

#if (MOVIE_MULTI_RECORD_FUNC)
    uiFrameRate = GetMovieFrameRate_2p(0, UI_GetData(FL_MOVIE_SIZE));
#else
    uiFrameRate = GetMovieFrameRate(UI_GetData(FL_MOVIE_SIZE));
#endif

#if (MOVIE_RSC == ENABLE)
    if (uiSelect == MOVIE_RSC_ON)
    {
        // disable DIS
        UI_SetData(FL_MovieDisIndex, MOVIE_DIS_OFF);
#if (MOVIE_DIS == ENABLE)
        MovieExe_IPL_SetIInfo(IPL_SEL_DISCOMP, SEL_DISCOMP_OFF);
#endif
        // enable RSC if necessary (only support frame rate smaller than 60fps)
        //rsc tmp
        if ((UI_GetData(FL_MovieRSCIndex) == MOVIE_RSC_ON) &&
            (uiFrameRate <= 120))
        {
            MovieExe_RSC_SetSwitch(SEL_RSC_RUNTIME, SEL_RSC_ON);

            if (UI_GetData(FL_MOVIE_HDR) == MOVIE_HDR_ON)
            {
                Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_SHDR, 1, SHDR_OFF);
            }
        }
        else
        {
            MovieExe_RSC_SetSwitch(SEL_RSC_RUNTIME, SEL_RSC_OFF);
        }
    }
    else
    {
        MovieExe_RSC_SetSwitch(SEL_RSC_RUNTIME, SEL_RSC_OFF);
    }
#endif

    return NVTEVT_CONSUME;
}

INT32 MovieExe_OnWDR(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if (paramNum)
        uiSelect = paramArray[0];

    #if defined(YQCONFIG_DEBUG_MESSAGE_SUPPORT)
    debug_msg("n\r MovieExe_OnWDR = %d \n\r",uiSelect);
    #endif
    if (uiSelect == MOVIE_WDR_ON)
    {
        UI_SetData(FL_MOVIE_WDR, MOVIE_WDR_ON);
        MovieExe_IPL_SetIInfo(IPL_SEL_WDR, SEL_WDR_ON);

        if (UI_GetData(FL_MOVIE_HDR) == MOVIE_HDR_ON)
            Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_SHDR, 1, MOVIE_HDR_OFF);
    }
    else
    {
        UI_SetData(FL_MOVIE_WDR, MOVIE_WDR_OFF);
        MovieExe_IPL_SetIInfo(IPL_SEL_WDR, SEL_WDR_OFF);
    }

    return NVTEVT_CONSUME;
}

INT32 MovieExe_OnSHDR(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    UINT32 bChangeMode=FALSE;

    DBG_IND("%d \r\n",paramArray[0]);
    if(paramNum>0)
        uhSelect= paramArray[0];

    if(paramNum > 1)
        bChangeMode = paramArray[1];

    UI_SetData(FL_MOVIE_HDR, uhSelect);

    #if SHDR_FUNC
    if (UI_GetData(FL_MOVIE_HDR) == MOVIE_HDR_ON)
    {
        MovieExe_IPL_SetIInfo(IPL_SEL_SHDR, SEL_SHDR_ON);
    }
    else
    {
        MovieExe_IPL_SetIInfo(IPL_SEL_SHDR, SEL_SHDR_OFF);
    }

    if (UI_GetData(FL_MOVIE_HDR) == MOVIE_HDR_ON)// && UI_GetData(FL_MOVIE_WDR) == MOVIE_WDR_ON)
    {
        UI_SetData(FL_MOVIE_WDR, MOVIE_WDR_OFF);
        UI_SetData(FL_MovieRSCIndex, MOVIE_RSC_OFF);
    }

    // need to off to preview again
    if (bChangeMode)
        Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, System_GetState(SYS_STATE_CURRMODE));
    else
        DBG_WRN("should change mode\r\n");
    #endif

    return NVTEVT_CONSUME;
}

INT32 MovieExe_OnSensorRotate(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if (paramNum)
        uiSelect = paramArray[0];
    //#NT#2016/05/25#Jeah Yen -begin
    //#NT#apply rotate to sensor 2
    //#NT#2016/03/02#Niven Cho -begin
    //#NT#Use chip filp instead of sensor flip
    if (uiSelect == SEN_ROTATE_ON)
    {
        UI_SetData(FL_MOVIE_SENSOR_ROTATE, SEN_ROTATE_ON);
    }
    else
    {
        UI_SetData(FL_MOVIE_SENSOR_ROTATE, SEN_ROTATE_OFF);
    }
    if(System_GetEnableSensor() & SENSOR_1)
    {
        ImageUnit_Begin(&ISF_ImagePipe1, 0);

           #if(IPCAM_FUNC==ENABLE)
           //#NT#2016/04/27#Charlie Chang -begin
           //#NT# ip cam support mirror and flip
               int rotate_type=0;
               rotate_type = IPL_AlgGetUIInfo(0, IPL_SEL_IMAGEFLIP);
        if(rotate_type == SEL_IMAGEFLIP_DSP_H_V)
        {
                   ImageUnit_CfgDirect(ISF_IN1, FLIP_H_V);
               }
        else if(rotate_type == SEL_IMAGEFLIP_DSP_H)
        {
                   ImageUnit_CfgDirect(ISF_IN1, FLIP_H);
               }
        else if(rotate_type == SEL_IMAGEFLIP_DSP_V)
        {
                   ImageUnit_CfgDirect(ISF_IN1, FLIP_V);
               }
        else
        {
                   ImageUnit_CfgDirect(ISF_IN1, FLIP_NONE);
               }
            //#NT#2016/04/27#Charlie Chang -end
            #else
            //#NT#2016/03/02#Niven Cho -begin
            //#NT#Use chip filp instead of sensor flip
            ImageUnit_CfgDirect(ISF_IN1, (UI_GetData(FL_MOVIE_SENSOR_ROTATE) == SEN_ROTATE_ON)? FLIP_V : FLIP_NONE);
            //#NT#2016/03/02#Niven Cho -end
            #endif

        ImageUnit_End();
    }
    //#NT#2016/03/02#Niven Cho -end
    if(System_GetEnableSensor() & SENSOR_2)
    {
        ImageUnit_Begin(&ISF_ImagePipe2, 0);
            ImageUnit_CfgDirect(ISF_IN1, 0);
        ImageUnit_End();
    }
    ImageStream_UpdateSize();
    //#NT#2016/05/25#Jeah Yen -end
    return NVTEVT_CONSUME;
}

INT32 MovieExe_OnIRCut(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
//#NT#2016/02/22#Niven Cho -begin
//#NT#add IR_CUT_FUNC to enable IR CUT function.
#if (IR_CUT_FUNC == ENABLE)
    UINT32 uiSelect = 0;

    if (paramNum)
        uiSelect = paramArray[0];

    DBG_IND("%d \r\n",uiSelect);
    if (uiSelect == MOVIE_IR_CUT_ON)
    {
        UI_SetData(FL_MOVIE_IR_CUT, MOVIE_IR_CUT_ON);
        MovieExe_IPL_SetIInfo(IPL_SEL_IRCUT, SEL_IRCUT_DAY);
        UIMovie_IRCutCtrl(ON);
    }
    #if( _UI_STYLE_ == _UI_STYLE_IPCAM_)
    else if (uiSelect == MOVIE_IR_CUT_AUTO)
    {
        UI_SetData(FL_MOVIE_IR_CUT, MOVIE_IR_CUT_AUTO);
        MovieExe_IPL_SetIInfo(IPL_SEL_IRCUT,  SEL_IRCUT_AUTO);
    }
    #endif
    else
    {
        UI_SetData(FL_MOVIE_IR_CUT, MOVIE_IR_CUT_OFF);
        MovieExe_IPL_SetIInfo(IPL_SEL_IRCUT,  SEL_IRCUT_NIGHT);
        UIMovie_IRCutCtrl(OFF);
    }
#endif
//#NT#2016/02/22#Niven Cho -end
    return NVTEVT_CONSUME;
}

INT32 MovieExe_OnGdc(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];

    DBG_IND("%d \r\n",uhSelect);
#if(MOVIE_GDC_FUNC ==ENABLE)
    if (uhSelect == TRUE)
    {
        if (UI_GetData(FL_MovieRSCIndex) == MOVIE_RSC_ON)
        {
            MovieExe_RSC_SetSwitch(SEL_RSC_RUNTIME, SEL_RSC_OFF);
            DBG_WRN("GDC is enable, so RSC is forced off\r\n");
        }

        MovieExe_IPL_SetIInfo(IPL_SEL_GDCCOMP,SEL_GDCCOMP_ON);
    }
    else
    {
        MovieExe_IPL_SetIInfo(IPL_SEL_GDCCOMP,SEL_GDCCOMP_OFF);
    }
#endif
    return NVTEVT_CONSUME;
}

INT32 MovieExe_OnSmear(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];
    DBG_IND("+-idx=%d\r\n",uhSelect);

    UI_SetData(FL_MovieSmearIndex,uhSelect);
#if(MOVIE_SMEAR_R_FUNC ==ENABLE)
    AppPhoto_SetData(&CustomPhotoObjCtrl, _SmearModeEn,
        (uhSelect==MOVIE_SMEAR_ON)?_SmearMode_on:_SmearMode_off);
#endif
    return NVTEVT_CONSUME;
}

//-------------------------------------------------------------------------------------------------
INT32 MovieExe_OnDateImprint(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if (paramNum)
        uiSelect = paramArray[0];

    UI_SetData(FL_MOVIE_DATEIMPRINT, uiSelect);

    return NVTEVT_CONSUME;
}
//-------------------------------------------------------------------------------------------------
INT32 MovieExe_OnGSENSOR(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if (paramNum)
        uiSelect = paramArray[0];

    UI_SetData(FL_GSENSOR, uiSelect);

    switch (UI_GetData(FL_GSENSOR))
    {
    case GSENSOR_OFF:
        GSensor_SetSensitivity(GSENSOR_SENSITIVITY_OFF);
        break;
    case GSENSOR_LOW:
        GSensor_SetSensitivity(GSENSOR_SENSITIVITY_LOW);
        break;
    case GSENSOR_MED:
        GSensor_SetSensitivity(GSENSOR_SENSITIVITY_MED);
        break;
    case GSENSOR_HIGH:
        GSensor_SetSensitivity(GSENSOR_SENSITIVITY_HIGH);
        break;
    default:
        GSensor_SetSensitivity(GSENSOR_SENSITIVITY_OFF);
        break;
    }

    //debug_err(("MovieExe_OnGSENSOR =%d\r\n ",FL_GSENSOR));
    return NVTEVT_CONSUME;
}

//-------------------------------------------------------------------------------------------------
INT32 MovieExe_OnCyclicRec(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;
    UINT32 uiCyclicRecTime = 0;

    if (paramNum)
        uiSelect = paramArray[0];

    UI_SetData(FL_MOVIE_CYCLIC_REC, uiSelect);

    switch (uiSelect)
    {
    case MOVIE_CYCLICREC_3MIN:
        uiCyclicRecTime = 180;
        break;

    case MOVIE_CYCLICREC_5MIN:
        uiCyclicRecTime = 300;
        break;

    case MOVIE_CYCLICREC_10MIN:
        uiCyclicRecTime = 600;
        break;

    case MOVIE_CYCLICREC_OFF:
        break;

    default:
        uiCyclicRecTime = 300;
        break;
    }

#if (MOVIE_AUTOREC_CYCLICTIME == ENABLE)
    if (uiSelect != MOVIE_CYCLICREC_OFF)
    {
        MovRec_ChangeParameter(MOVREC_RECPARAM_CUTSEC, uiCyclicRecTime, 0, 0);
        MovRec_ChangeParameter(MOVREC_RECPARAM_ENDTYPE, MOVREC_ENDTYPE_CUTOVERLAP, 0, 0);
    }
    else
#endif
    {
        MovRec_ChangeParameter(MOVREC_RECPARAM_CUTSEC, 900, 0, 0); // 15 min
        MovRec_ChangeParameter(MOVREC_RECPARAM_ENDTYPE, MOVREC_ENDTYPE_CUT_TILLCARDFULL, 0, 0);
    }

    return NVTEVT_CONSUME;
}

//-------------------------------------------------------------------------------------------------
INT32 MovieExe_OnMotionDet(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect = 0;

    if (paramNum)
        uiSelect = paramArray[0];

    UI_SetData(FL_MOVIE_MOTION_DET, uiSelect);

    return NVTEVT_CONSUME;
}

//#NT#2012/07/31#Hideo Lin -begin
//-------------------------------------------------------------------------------------------------
#if (MOVIE_TEST_ENABLE == ENABLE)
extern void MovRec_SetIPLChangeCB(MediaRecIPLChangeCB *pIPLChangeCB); // temporary
void MovieExe_FakeIPLChangeCB(UINT32 mode, UINT32 param)
{
    UINT32  APP_RESERVE_SIZE = 1920 * 1080 * 15;
    UINT32  uiPoolAddr;

    uiPoolAddr = OS_GetMempoolAddr(POOL_ID_APP);

    switch (mode)
    {
    case MOVREC_IPLCHG_VIDEO:
        // Re-allocate buffer for Media Recorder
        MovRec_ReAllocBuf2VA(uiPoolAddr + APP_RESERVE_SIZE, POOL_SIZE_APP - APP_RESERVE_SIZE);
        break;
    }
}
#endif


#if (MOVIE_REC_YUVMERGE == ENABLE)
extern void Movie_IPLChangeCB(UINT32 mode, UINT32 param);
#endif

INT32 MovieExe_OnRecStart(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //#NT#2016/07/11#Charlie Chang -begin
    //#NT# save rec status
    UI_SetData(FL_MOVIE_REC, 1);
    //#NT#2016/07/11#Charlie Chang -end
    if(_g_bFirstMovie)
        _g_bFirstMovieRec = TRUE;
    if(_g_bFirstMovieRec){TM_BOOT_BEGIN("flow","rec");}

    if(System_GetState(SYS_STATE_POWERON) == SYSTEM_POWERON_SAFE)
    {
        Movie_SetRecParam(); //update again for changed by UI menu
    }
//#NT#2016/03/18#Lincy Lin -begin
//#NT#Support object tracking function
#if MOVIE_OT_FUNC
    OT_START();
#endif
//#NT#2016/03/18#Lincy Lin -end

//#NT#2016/05/20#Yang Jin -begin
//#NT#Support dis dsp function
#if MOVIE_DISDSP_FUNC
        DisDsp_START();
#endif
//#NT#2016/05/20#Yang Jin -end

    // send command to UIMovieRecObj to start movie recording
    //Ux_SendEvent(&UIMovieRecObjCtrl, NVTEVT_START_REC_MOVIE, 0);
#if(IPCAM_FUNC==ENABLE)
    {
        ImageApp_IPCam_RecStart(IPCAM_PATH_ID_1);
        //ImageApp_IPCam_RecStart(IPCAM_PATH_ID_2);
    }
//#NT#2017/02/24#Ben Wang -begin
//#NT#Fix that UVC_MULTIMEDIA_FUNC depends on WIFI_AP_FUNC.
#elif((WIFI_AP_FUNC==ENABLE) || (UVC_MULTIMEDIA_FUNC == ENABLE))
//#NT#2017/02/24#Ben Wang -end
    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
    {
        ImageApp_NetMovie_RecStart();
    }
//#NT#2016/05/31#Ben Wang -begin
//#NT#Add UVC multimedia function.
#if(UVC_MULTIMEDIA_FUNC == ENABLE)
    else if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_UVC)
    {
        ImageApp_UsbMovie_RecStart();
    }
#endif
//#NT#2016/05/31#Ben Wang -end
    else
    {
        ImageApp_CamMovie_RecStart();
    }
#else
    {
        ImageApp_CamMovie_RecStart();
    }
#endif

//#NT#2016/08/29#HM Tseng -begin
//#NT#For audio play testing mode
#if (AUDIO_PLAY_FUNCTION == ENABLE && AUDIO_PLAY_TEST == DISABLE)
    System_AudioOutStart();//start output
#endif
//#NT#2016/08/29#HM Tseng -end

#if (MOVIE_REC_YUVMERGE == ENABLE)
    // Isiah, implement YUV merge mode of recording func.
    if(FlowMovie_RecGetYUVMergeMode())
    {
        ImageStream_SetActive(ISF_AMD_SLEEP1);
    }
#endif

//#NT#2016/03/25#KCHong -begin
//#NT#New ADAS, ADAS_Start() moves to ImageUnit_ADAS
debug_msg("\r\n_ADAS_FUNC_...3333\r\n");

#if _ADAS_FUNC_
    //#NT#2016/06/16#KCHong - begin
    //#NT#ImageUnit_ALG
     debug_msg("\r\ng_ADASFuncSupported = %d\r\n", g_ADASFuncSupported);
    debug_msg("\r\nMovRec_IsRecording() = %d\r\n", MovRec_IsRecording());

    if (MovRec_IsRecording() && (g_ADASFuncSupported == TRUE))
    {
        memset(&g_AdasRltOSD, 0, sizeof(ADAS_APPS_RESULT_INFO));
        memset(&g_AdasRltVideo, 0, sizeof(ADAS_APPS_RESULT_INFO));
        g_AdasRltOSD.LdwsDspRsltInfo.Failure = LDWS_FAILURE_TRUE;//g_AdasRltOSD.LdwsRsltInfo.Failure = LDWS_FAILURE_TRUE;
        g_AdasRltVideo.LdwsRsltInfo.Failure = LDWS_FAILURE_TRUE;
        #if (_SNG_FUNC_ == ENABLE)
        memset(&g_SnGROIOSD, 0, sizeof(ADAS_SNG_ROI_INFO));
        memset(&g_SnGROIVideo, 0, sizeof(ADAS_SNG_ROI_INFO));
        #endif
        ADAS_AppsStart();
        debug_msg("\r\nstart adas...3333\r\n");
    }
    //#NT#2016/06/16#KCHong# - end
#endif
//#NT#2016/03/25#KCHong -end

    //#NT#2012/10/30#Calvin Chang#Generate Audio NR pattern by noises of zoom, focus and iris -begin
    #if (MOVIE_AUDIO_NR_PATTERN_ENABLE == ENABLE)
    BKG_PostEvent(NVTEVT_BKW_ANR_LENS_ACTION);
    #endif
    //#NT#2012/10/30#Calvin Chang -end

    //#NT#2012/10/23#Philex Lin - begin
    // disable auto power off/USB detect timer
    KeyScan_EnableMisc(FALSE);
    //#NT#2012/10/23#Philex Lin - end
    return NVTEVT_CONSUME;
}

INT32 MovieExe_OnRecStep(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if (MOVIE_REC_YUVMERGE == ENABLE)
    #if 0
    if(FlowMovie_RecGetYUVMergeMode())
    {
        MEDIAREC_READYBUF_INFO readyInfo;
        IPL_IME_BUF_ADDR CurInfo;
        IPL_SLEEP_INFO Info;

        FlowMovie_RecYUVMergeCounterInc();
        if(FlowMovie_RecGetYUVMergeRecCounter() < FlowMovie_RecGetYUVMergeRecInterval())
        {
            return NVTEVT_CONSUME;
        }
        else // reset counter;
        {
            FlowMovie_RecSetYUVMergeRecCounter(0);
        }

        Info.Id = 0;
        IPL_SetCmd(IPL_SET_WAKEUP, (void *)&Info);
        IPL_WaitCmdFinish();
        // Wait until AE/AWB are stable.
        //#NT#2016/10/18#Jeah Yen -begin
        //#NT#support sensor map
        IPC_WaitVD(FALSE,IPL_PATH_1);
        IPC_WaitVD(FALSE,IPL_PATH_1);
        IPC_WaitVD(FALSE,IPL_PATH_1);

        CurInfo.Id = IPL_PATH_1;
        IPL_GetCmd(IPL_GET_IME_CUR_BUF_ADDR, (void *)&CurInfo);
        //#NT#2016/10/18#Jeah Yen -end

        readyInfo.y = CurInfo.ImeP1.PixelAddr[0];
        readyInfo.cb = CurInfo.ImeP1.PixelAddr[1];
        //readyInfo.cr = CurInfo.ImeP1.PixelAddr[2];
        readyInfo.cr = CurInfo.ImeP1.PixelAddr[1]; // UV pack
        readyInfo.y_lot = CurInfo.ImeP1.Ch[0].LineOfs;
        readyInfo.uv_lot = CurInfo.ImeP1.Ch[1].LineOfs;
        readyInfo.uiBufID = CurInfo.ImeP1.PixelAddr[2]; // for IME ready buffer check
        MovRec_GiveYUV(&readyInfo);

        // Wait 40 ms to ensure encode finished.
        Delay_DelayMs(40);

        Info.Id = 0;
        IPL_SetCmd(IPL_SET_SLEEP, (void *)&Info);
        IPL_WaitCmdFinish();
    }
    #endif
#endif
#if (MOVIE_REC_YUVMERGE == ENABLE)
    // Isiah, implement YUV merge mode of recording func.
    if(FlowMovie_RecGetYUVMergeMode())
    {
        ImageApp_CamMovie_RecStart(); //step encode
        ImageStream_SetActive(ISF_AMD_SLEEP1);
    }
#endif

    return NVTEVT_CONSUME;
}

//-------------------------------------------------------------------------------------------------
#if (MOVIE_PIM_CAPTURE == ENABLE)
static UINT32 g_uiDoPIM = FALSE;
#endif

INT32 MovieExe_OnRecStop(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //#NT#2016/07/11#Charlie Chang -begin
    //#NT# save rec status
    UI_SetData(FL_MOVIE_REC, 0);
    //#NT#2016/07/11#Charlie Chang -end
    if(System_GetState(SYS_STATE_POWERON) == SYSTEM_POWERON_NORMAL)
        ImageUnit_SetParam(&ISF_Media, MEDIA_PARAM_MOVIEPRV_CHGMODE, (UINT32)TRUE); // change back to default

//#NT#2016/06/16#KCHong -begin
//#NT#ADAS
#if _ADAS_FUNC_
    if (ADAS_GetAppsStatus())
    {
        ADAS_AppsStop();
    }
#endif
//#NT#2016/06/16#KCHong -end

//#NT#2016/03/18#Lincy Lin -begin
//#NT#Support object tracking function
#if MOVIE_OT_FUNC
    OT_END();
#endif
//#NT#2016/03/18#Lincy Lin -end

//#NT#2016/05/20#Yang Jin -begin
//#NT#Support dis dsp function
#if MOVIE_DISDSP_FUNC
        DisDsp_END();
#endif
//#NT#2016/05/20#Yang Jin -end

//#NT#2016/08/29#HM Tseng -begin
//#NT#For audio play testing mode
#if (AUDIO_PLAY_FUNCTION == ENABLE && AUDIO_PLAY_TEST == DISABLE)
    System_AudioOutStop();//stop output
#endif
//#NT#2016/08/29#HM Tseng -end

    // send command to UIMovieRecObj to stop movie recording
    //Ux_SendEvent(&UIMovieRecObjCtrl, NVTEVT_STOP_REC_MOVIE, 0);
#if(IPCAM_FUNC==ENABLE)
    if (1)
    {
        ImageApp_IPCam_RecStop(IPCAM_PATH_ID_1);
        //ImageApp_IPCam_RecStop(IPCAM_PATH_ID_2);
    }
//#NT#2017/02/24#Ben Wang -begin
//#NT#Fix that UVC_MULTIMEDIA_FUNC depends on WIFI_AP_FUNC.
#elif((WIFI_AP_FUNC==ENABLE) || (UVC_MULTIMEDIA_FUNC == ENABLE))
//#NT#2017/02/24#Ben Wang -end
    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
    {
        ImageApp_NetMovie_RecStop();
    }
//#NT#2016/05/31#Ben Wang -begin
//#NT#Add UVC multimedia function.
#if(UVC_MULTIMEDIA_FUNC == ENABLE)
    else if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_UVC)
    {
        ImageApp_UsbMovie_RecStop();
    }
#endif
//#NT#2016/05/31#Ben Wang -end
    else
    {
        ImageApp_CamMovie_RecStop();
    }
#else
    {
        ImageApp_CamMovie_RecStop();
    }
#endif

    //#NT#2012/10/23#Philex Lin - begin
    // Enable auto power off/USB detect timer
    KeyScan_EnableMisc(TRUE);
    //#NT#2012/10/23#Philex Lin - end

#if (MOVIE_PIM_CAPTURE == ENABLE)
    if (g_uiDoPIM)
    {
        ImageApp_CamMovie_CapStop(); //must call this to start post-capture process, and return to preview
        g_uiDoPIM = FALSE;
    }
#endif

#if (MOVIE_REC_YUVMERGE == ENABLE)
    #if 0
    // Isiah, implement YUV merge mode of recording func.
    if(FlowMovie_RecGetYUVMergeMode())
    {
        IPL_SLEEP_INFO Info;
        Info.Id = 0;
        IPL_SetCmd(IPL_SET_WAKEUP, (void *)&Info);
        IPL_WaitCmdFinish();

        //#NT#2016/10/18#Jeah Yen -begin
        //#NT#support sensor map
        IPC_WaitVD(FALSE, IPL_PATH_1);
        IPC_WaitVD(FALSE, IPL_PATH_1);
        IPC_WaitVD(FALSE, IPL_PATH_1);
        //#NT#2016/10/18#Jeah Yen -end

        Movie_IPLChangeCB(MEDIAREC_IPLCHG_PREVIEW, 0);
        debug_msg("Change IPL to movie preview mode\r\n");
    }
    #endif
#endif

    if(System_GetState(SYS_STATE_CURRSUBMODE)!=SYS_SUBMODE_WIFI)
    {
        // disable movie stamp
        MovieStamp_Disable();
    }

    return NVTEVT_CONSUME;
}

//-------------------------------------------------------------------------------------------------
INT32 MovieExe_OnRecPIM(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if (MOVIE_PIM_CAPTURE == ENABLE)
    // do picture in movie (capture full size still image as movie recording; still image will be processed after recording end)
    DBG_IND("Capture full size image!\r\n");
    g_uiDoPIM = TRUE;
    MovieExe_Cap_SetIInfo(CAP_SEL_DRIVEMODE, SEL_DRIVEMODE_INVID);
    MovieExe_Cap_SetIInfo(CAP_SEL_PICNUM, 1);
    MovieExe_Cap_SetIInfo(CAP_SEL_FILEFMT, SEL_FILEFMT_JPG);
    MovieExe_Cap_SetIInfo(CAP_SEL_JPGFMT, SEL_JPGFMT_422);
    MovieExe_Cap_SetIInfo(CAP_SEL_JPG_IMG_H_SIZE, GetPhotoSizeWidth(0));  // 0 is max still image size
    MovieExe_Cap_SetIInfo(CAP_SEL_JPG_IMG_V_SIZE, GetPhotoSizeHeight(0)); // 0 is max still image size
    MovieExe_Cap_SetIInfo(CAP_SEL_RAW_BUF_NUM, 2); // 2 raw image buffer with 12-bit resolution for 128MB DRAM
    MovieExe_Cap_SetIInfo(CAP_SEL_JPG_BUF_NUM, 1); // 1 JPEG buffer
    ImageApp_CamMovie_CapStart();
#endif

    return NVTEVT_CONSUME;
}

//-------------------------------------------------------------------------------------------------
INT32 MovieExe_OnRecRawEnc(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if 0
    UINT32  uiSrcWidth, uiSrcHeight, uiDAR, uiMovieSize;
    UINT32  uiJpegWidth = 320, uiJpegHeight = 240;
#endif
    // do raw encode (capture still image as movie recording; still image size is equal to or smaller than video size;
    // still image can be processed during movie recording)
    DBG_IND("Capture image!\r\n");

#if 0
    if (paramNum > 1)
    {
        uiJpegWidth  = paramArray[0];
        uiJpegHeight = paramArray[1];
    }

    uiMovieSize = UI_GetData(FL_MOVIE_SIZE);
    uiSrcWidth  = GetMovieSizeWidth(uiMovieSize);
    uiSrcHeight = GetMovieSizeHeight(uiMovieSize);
    uiDAR = GetMovieDispAspectRatio(uiMovieSize);

    if (uiDAR == VIDENC_DAR_16_9)
    {
        uiSrcWidth = (uiSrcHeight * 16) / 9;
    }

    // limit JPEG width
    if (uiJpegWidth > uiSrcWidth)
    {
        uiJpegWidth = uiSrcWidth;
    }
    if (uiJpegWidth < 320)
    {
        uiJpegWidth = 320;
    }

    // limit JPEG height
    if (uiJpegHeight > uiSrcHeight)
    {
        uiJpegHeight = uiSrcHeight;
    }
    if (uiJpegHeight < 240)
    {
        uiJpegHeight = 240;
    }

    RawEnc_ChangeParameter(RAWENC_JPEG_WIDTH,  uiJpegWidth,  0, 0);
    RawEnc_ChangeParameter(RAWENC_JPEG_HEIGHT, uiJpegHeight, 0, 0);
    RawEnc_TriggerEncode();
#endif

#if (MOVIE_PIM_SNAPSHOT == ENABLE)
#if(IPCAM_FUNC==ENABLE)
    {
        ImageApp_IPCam_Snapshot(IPCAM_PATH_ID_1);
    }
//#NT#2017/02/24#Ben Wang -begin
//#NT#Fix that UVC_MULTIMEDIA_FUNC depends on WIFI_AP_FUNC.
#elif((WIFI_AP_FUNC==ENABLE) || (UVC_MULTIMEDIA_FUNC == ENABLE))
//#NT#2017/02/24#Ben Wang -end
    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
    {
        ImageApp_NetMovie_Snapshot();
    }
//#NT#2016/05/31#Ben Wang -begin
//#NT#Add UVC multimedia function.
#if(UVC_MULTIMEDIA_FUNC == ENABLE)
    else if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_UVC)
    {
        ImageApp_UsbMovie_Snapshot();
    }
#endif
//#NT#2016/05/31#Ben Wang -end
    else
    {
        ImageApp_CamMovie_Snapshot(); //the snapshot size will equal to record size
    }
#else
    {
        ImageApp_CamMovie_Snapshot(); //the snapshot size will equal to record size
    }
#endif
#endif

    return NVTEVT_CONSUME;
}
//#NT#2012/07/31#Hideo Lin -end

INT32 MovieExe_OnCallback(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    NVTEVT event;

    event = paramArray[0];

    DBG_IND("[cb]%d\r\n",event);
    switch(event)
    {
        case NVTEVT_ALGMSG_FLASH:
            DBG_IND("[cb]NVTEVT_ALGMSG_FLASH\r\n");
            break;

        case NVTEVT_ALGMSG_QVSTART:
            DBG_IND("[cb]NVTEVT_ALGMSG_QVSTART\r\n");
            break;

        case NVTEVT_ALGMSG_JPGOK:
            DBG_IND("[cb]NVTEVT_ALGMSG_JPGOK\r\n");
            break;

        case NVTEVT_ALGMSG_CAPFSTOK:
            DBG_IND("[cb]NVTEVT_ALGMSG_CAPFSTOK\r\n");
            break;


        case NVTEVT_ALGMSG_CAPTUREEND:
            DBG_IND("[cb]NVTEVT_ALGMSG_CAPTUREEND \r\n");
            break;

        case NVTEVT_ALGMSG_DZOOMSTEP:
            DBG_IND("[cb]NVTEVT_ALGMSG_DZOOMSTEP \r\n");
            break;

        case NVTEVT_ALGMSG_PREVIEW_STABLE:
            DBG_IND("[cb]NVTEVT_ALGMSG_PREVIEW_STABLE \r\n");
            break;

        default:

            break;
    }
    return NVTEVT_PASS;
}

//#NT#2016/06/21#Jeah Yen -begin
//#NT#Support display fast switch
INT32 MovieExe_OnVideoChange(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    #if (PIP_VIEW_FUNC == ENABLE)
    PipView_SetStyle(UI_GetData(FL_DUAL_CAM));
    #endif
    //DBGD(Perf_GetCurrent());
    if(System_GetState(SYS_STATE_CURRMODE)==PRIMARY_MODE_MOVIE)
    {
        Ux_SendEvent(0,NVTEVT_EXE_IMAGE_RATIO,1,GetMovieSizeRatio(UI_GetData(FL_MOVIE_SIZE)));
    }
    //DBGD(Perf_GetCurrent());
    return NVTEVT_PASS; //PASS this event to UIWnd!
}
//#NT#2016/06/21#Jeah Yen -end

INT32 MovieExe_OnDualcam(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;
    if(paramNum>0)
        uhSelect= paramArray[0];

    DBG_IND("%d \r\n",uhSelect);

    #if (SENSOR_CAPS_COUNT == 1)
    DBG_ERR("single sensor, not support this option!\r\n");
    #endif
    #if (SENSOR_CAPS_COUNT == 2)

    UI_SetData(FL_DUAL_CAM, uhSelect);

    // also change the capture id
    //PhotoExe_OnCaptureID(pCtrl, paramNum, paramArray);
    //if (localInfo->DualCam != uhSelect)
    #if (PIP_VIEW_FASTSWITCH==ENABLE)
    {
        Ux_SendEvent(0, NVTEVT_SENSOR_DISPLAY, 1, (SENSOR_1|SENSOR_2)); //for Always trigger PIP View
        //#NT#2015/11/25#Niven Cho#[87393][88541] -begin
        //Here be invoked at startup without in any mode
        if(System_GetState(SYS_STATE_CURRMODE)==PRIMARY_MODE_MOVIE)
        {
            Ux_SendEvent(0,NVTEVT_EXE_IMAGE_RATIO,1,GetMovieSizeRatio(UI_GetData(FL_MOVIE_SIZE)));
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

    #if(WIFI_AP_FUNC==ENABLE)
    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
    {
        // config pip view parameter
        {
            USIZE   HttpLviewSize;
            UINT32  ImageRatioIdx;

            ImageRatioIdx = GetMovieSizeRatio(UI_GetData(FL_MOVIE_SIZE));
            HttpLviewSize = MOVIE_HTTPLVIEW_SIZE[ImageRatioIdx];
            PhotoExe_CfgNetHttpPIP(HttpLviewSize,UI_GetData(FL_DUAL_CAM));

        }
    }
    #endif

    #endif
    return NVTEVT_CONSUME;
}

//#NT#2016/06/08#Lincy Lin -begin
//#NT#Implement generic OSD and video drawing mechanism for ALG function
static void MovieExe_CallBackUpdateInfo(UINT32 callBackEvent)
{
    VControl *pCurrnetWnd;

    Ux_GetFocusedWindow(&pCurrnetWnd);
    Ux_SendEvent(pCurrnetWnd,NVTEVT_UPDATE_INFO, 1, callBackEvent);
}
//#NT#2016/06/08#Lincy Lin -end
INT32 MovieExe_OnFDEnd(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    #if MOVIE_FD_FUNC_
    {
        //Flush FD event before draw
        Ux_FlushEventByRange(NVTEVT_EXE_MOVIE_FDEND,NVTEVT_EXE_MOVIE_FDEND);
        MovieExe_CallBackUpdateInfo(UIAPPPHOTO_CB_FDEND);
    }
    #endif
    return NVTEVT_CONSUME;

}
//#NT#2016/10/17#Bin Xiao -begin
//#NT# Support Face Tracking Grading(FTG)
INT32 MovieExe_OnFTGEnd(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    #if MOVIE_FTG_FUNC
    {
        //Flush FTG event before draw
        Ux_FlushEventByRange(NVTEVT_EXE_MOVIE_FTGEND,NVTEVT_EXE_MOVIE_FTGEND);
        MovieExe_CallBackUpdateInfo(UIAPPPHOTO_CB_FTGEND);
    }
    #endif
    return NVTEVT_CONSUME;
}
//#NT#2016/10/17#Bin Xiao -end




#if 0
INT32 MovieExe_OnTSREnd(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    #if MOVIE_TSR_FUNC_
    {
        //Flush TSR event before draw
        Ux_FlushEventByRange(NVTEVT_EXE_MOVIE_TSREND, NVTEVT_EXE_MOVIE_TSREND);
        MovieExe_CallBackUpdateInfo(UIAPPPHOTO_CB_TSREND);
    }
    #endif
    return NVTEVT_CONSUME;

}
#endif

INT32 MovieExe_OnProtectAuto(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect    = 0;
    UINT32 uiWidth     = GetMovieSizeWidth(UI_GetData(FL_MOVIE_SIZE));

    if (paramNum)
        uiSelect = paramArray[0];

    UI_SetData(FL_MOVIE_URGENT_PROTECT_AUTO, uiSelect);

    if (UI_GetData(FL_MOVIE_URGENT_PROTECT_AUTO) == MOVIE_URGENT_PROTECT_AUTO_ON ||
        UI_GetData(FL_MOVIE_URGENT_PROTECT_MANUAL) == MOVIE_URGENT_PROTECT_MANUAL_ON)
    {
#if (MOVIE_MULTI_RECORD_FUNC)
        if(GetMovieRecType_2p(UI_GetData(FL_MOVIE_SIZE))==MOVIE_REC_TYPE_DUAL
            || GetMovieRecType_2p(UI_GetData(FL_MOVIE_SIZE))==MOVIE_REC_TYPE_CLONE)
        {
            DBG_ERR("dual record, not support this option!\r\n");
            MovRec_SetEmergencyRecording(0, 0);
            return NVTEVT_CONSUME;
        }
#endif
        // protect on
        if (uiWidth <= 1920)
        {
            //#NT#2016/08/09#Niven Cho -begin
            //#NT#LINUX-CARDV, Protect memory is not enough
            #if (_ADAS_FUNC_ == ENABLE || _CPU2_TYPE_==_CPU2_LINUX_)
            MovRec_SetMaxStopFlashSec(10); // 10 seconds
            #else
            MovRec_SetMaxStopFlashSec(30); // 30 seconds
            #endif
            MovRec_SetEmergencyRecording(0, 1);
            //#NT#2016/08/09#Niven Cho -end
        }
        else
        {
            DBG_DUMP("use MovRec_SetCrash() under higher resolution!\r\n");
            MovRec_SetEmergencyRecording(0, 0);  // use MovRec_SetCrash() under higher resolution.
        }

    }
    else
    {
        // protect off
        MovRec_SetEmergencyRecording(0, 0);
    }

    return NVTEVT_CONSUME;
}
INT32 MovieExe_OnProtectManual(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uiSelect    = 0;
    UINT32 uiWidth     = GetMovieSizeWidth(UI_GetData(FL_MOVIE_SIZE));

    if (paramNum)
        uiSelect = paramArray[0];

    UI_SetData(FL_MOVIE_URGENT_PROTECT_MANUAL, uiSelect);

    if (UI_GetData(FL_MOVIE_URGENT_PROTECT_AUTO) == MOVIE_URGENT_PROTECT_AUTO_ON ||
        UI_GetData(FL_MOVIE_URGENT_PROTECT_MANUAL) == MOVIE_URGENT_PROTECT_MANUAL_ON)
    {
#if (MOVIE_MULTI_RECORD_FUNC)
        if(GetMovieRecType_2p(UI_GetData(FL_MOVIE_SIZE))==MOVIE_REC_TYPE_DUAL
            || GetMovieRecType_2p(UI_GetData(FL_MOVIE_SIZE))==MOVIE_REC_TYPE_CLONE)
        {
            DBG_ERR("dual record, not support this option!\r\n");
            MovRec_SetEmergencyRecording(0, 0);
            return NVTEVT_CONSUME;
        }
#endif
        // protect manual on
        if (uiWidth <= 1920)
        {
            //#NT#2016/08/09#Niven Cho -begin
            //#NT#LINUX-CARDV, Protect memory is not enough
            #if (_ADAS_FUNC_ == ENABLE || _CPU2_TYPE_==_CPU2_LINUX_)
            MovRec_SetMaxStopFlashSec(10); // 10 seconds
            #else
            MovRec_SetMaxStopFlashSec(30); // 30 seconds
            #endif

            MovRec_SetEmergencyRecording(0, 1);
            //#NT#2016/08/09#Niven Cho -end
        }
        else
        {
            DBG_DUMP("use MovRec_SetCrash() under higher resolution!\r\n");
            MovRec_SetEmergencyRecording(0, 0);  // use MovRec_SetCrash() under higher resolution.
        }
    }
    else
    {
        // protect manual off
        MovRec_SetEmergencyRecording(0, 0);
    }

    return NVTEVT_CONSUME;
}


INT32 MovieExe_OnLDWS(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if _ADAS_FUNC_
    UINT32 uiSelect = 0;

    if (paramNum)
        uiSelect = paramArray[0];

	//#NT#2016/10/12#KCHong#[0111054] -begin
	//#NT#Don't need to sync flag due to the flag will be synced in CheckReOpenItem()
    #if defined(YQCONFIG_ANDROID_SYSTEM_SUPPORT)
    UI_SetData(FL_MOVIE_LDWS, uiSelect);
    #else
    #endif
	//#NT#2016/10/12#KCHong#[0111054] -end

    if (uiSelect == MOVIE_LDWS_ON)
    {
        //#NT#2016/03/25#KCHong -begin
        //#NT#New ADAS
        #if defined(YQCONFIG_DEBUG_MESSAGE_SUPPORT)
        debug_msg("MovieExe_OnLDWS ON..\r\n");
        #endif
        ADAS_AppsSetLdwsEanble(TRUE);
        ADAS_AppsSetStopDectEanble(TRUE);
        #if (_AUTOVP_FUNC_ == ENABLE)
        ADAS_AppsSetAutoVPEanble(TRUE);
        #endif
        #if (_SNG_FUNC_ == ENABLE)
        ADAS_AppsSetSnGEanble(TRUE);
        #endif
        //#NT#2016/03/25#KCHong -end
    }
    else
    {
        //#NT#2016/03/25#KCHong -begin
        //#NT#New ADAS
        #if defined(YQCONFIG_DEBUG_MESSAGE_SUPPORT)
        debug_msg("MovieExe_OnLDWS OFF..\r\n");
        #endif
        if (SysGetFlag(FL_MOVIE_FCW) == MOVIE_FCW_ON)
        {
            // Do nothing since FC relies on LD
        }
        else
        {
            // Disable all ADAS function
            ADAS_AppsSetFcwsEanble(FALSE);
            ADAS_AppsSetLdwsEanble(FALSE);
            ADAS_AppsSetStopDectEanble(FALSE);
            #if (_AUTOVP_FUNC_ == ENABLE)
            ADAS_AppsSetAutoVPEanble(FALSE);
            #endif
            #if (_SNG_FUNC_ == ENABLE)
            ADAS_AppsSetSnGEanble(FALSE);
            #endif
        }
        //#NT#2016/03/25#KCHong -end
    }
#endif

    return NVTEVT_CONSUME;
}

INT32 MovieExe_OnFCW(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{

#if _ADAS_FUNC_
    UINT32 uiSelect = 0;

    if (paramNum)
        uiSelect = paramArray[0];

	//#NT#2016/10/12#KCHong#[0111054] -begin
	//#NT#Don't need to sync flag due to the flag will be synced in CheckReOpenItem()
    #if defined(YQCONFIG_ANDROID_SYSTEM_SUPPORT)
    UI_SetData(FL_MOVIE_FCW, uiSelect);
    #else
    #endif
	//#NT#2016/10/12#KCHong#[0111054] -end

    if (uiSelect == MOVIE_FCW_ON)
    {
        #if defined(YQCONFIG_DEBUG_MESSAGE_SUPPORT)
        debug_msg("MovieExe_OnFCW ON..\r\n");
        #endif
        //#NT#2016/03/25#KCHong -begin
        //#NT#New ADAS
        ADAS_AppsSetFcwsEanble(TRUE);
        ADAS_AppsSetLdwsEanble(TRUE);
        ADAS_AppsSetStopDectEanble(TRUE);
        #if (_AUTOVP_FUNC_ == ENABLE)
        ADAS_AppsSetAutoVPEanble(TRUE);
        #endif
        #if (_SNG_FUNC_ == ENABLE)
        ADAS_AppsSetSnGEanble(TRUE);
        #endif
        //#NT#2016/03/25#KCHong -end
    }
    else
    {
        //#NT#2016/03/25#KCHong -begin
        //#NT#New ADAS
        #if defined(YQCONFIG_DEBUG_MESSAGE_SUPPORT)
        debug_msg("MovieExe_OnFCW OFF..\r\n");
        #endif
        if (SysGetFlag(FL_MOVIE_LDWS) == MOVIE_LDWS_ON)
        {
            // Only disalbe FC
            ADAS_AppsSetFcwsEanble(FALSE);
        }
        else
        {
            // Disable all ADAS function
            ADAS_AppsSetFcwsEanble(FALSE);
            ADAS_AppsSetLdwsEanble(FALSE);
            ADAS_AppsSetStopDectEanble(FALSE);
            #if (_AUTOVP_FUNC_ == ENABLE)
            ADAS_AppsSetAutoVPEanble(FALSE);
            #endif
            #if (_SNG_FUNC_ == ENABLE)
            ADAS_AppsSetSnGEanble(FALSE);
            #endif
        }
        //#NT#2016/03/25#KCHong -end
    }
#endif

    return NVTEVT_CONSUME;
}

INT32 MovieExe_OnPauseResume(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{

#if IPCAM_FUNC
    UINT32 channel_id = paramArray[0];
    ImageApp_IPCam_SetPath(channel_id, IPCAM_STATE_CHGPARM);
    ImageStream_UpdateAll();
#endif
    return NVTEVT_CONSUME;
}

#if (MOVIE_REC_YUVMERGE == ENABLE)
//#NT#2013/10/29#Isiah Chang -begin
//#Implement YUV merge mode of recording func.
void FlowMovie_RecSetYUVMergeMode(BOOL bEnable)
{
    g_bMovieRecYUVMergeEnable = bEnable;
}

BOOL FlowMovie_RecGetYUVMergeMode(void)
{
    return g_bMovieRecYUVMergeEnable;
}

void FlowMovie_RecSetYUVMergeRecInterval(UINT32 uiSec)
{
    g_uiMovieRecYUVMergeInterval = uiSec;
}

UINT32 FlowMovie_RecGetYUVMergeRecInterval(void)
{
    return g_uiMovieRecYUVMergeInterval;
}

void FlowMovie_RecSetYUVMergeRecCounter(UINT32 uiCount)
{
    g_uiMovieRecYUVMergeCounter = uiCount;
}

UINT32 FlowMovie_RecGetYUVMergeRecCounter(void)
{
    return g_uiMovieRecYUVMergeCounter;
}

void FlowMovie_RecYUVMergeCounterInc(void)
{
    g_uiMovieRecYUVMergeCounter++;
}
//#NT#2013/10/29#Isiah Chang -end
#endif

void FlowMovie_FileDBCreate(void)
{
    #if USE_FILEDB
    MEM_RANGE Pool;

    Pool.Addr = dma_getCacheAddr(OS_GetMempoolAddr(POOL_ID_FILEDB));
    Pool.Size = POOL_SIZE_FILEDB;
    ImageUnit_SetParam(&ISF_CamFile, CAMFILE_PARAM_USE_FILEDB, (UINT32)&Pool);
    // config FDB root
//#NT#2016/08/12#Hideo Lin -begin
//#NT#For small size clone movie
#if (CUSTOM_NAMING_RULE == DISABLE)
    // sample code for FileDB Naming Rule
    // disable custom naming
    ImageUnit_SetParam(&ISF_CamFile, CAMFILE_PARAM_CUSTOM_NAMING, FALSE);

    // set root folder
    ImageApp_CamFile_SetFolder(FILEDB_CARDV_ROOT);

    // set folder name of movie, RO, and photo (sample code)
    #if 0
    ImageApp_CamFile_SetMovieFolder(0, "MOVIE");
    ImageApp_CamFile_SetROFolder(0, "LOCK");
    ImageApp_CamFile_SetEMRFolder(0, "EMERGENCY");
    ImageApp_CamFile_SetPhotoFolder(0, "PHOTO");
    #endif

    //#NT#2016/03/07#KCHong -begin
    //#NT#Low power timelapse function
    #if ((YOUKU_SDK == ENABLE) || (TIMELAPSE_LPR_FUNCTION == ENABLE))
    char RootFolder[NMC_OTHERS_MAX_LEN];
    // enable custom naming
    ImageUnit_SetParam(&ISF_CamFile, CAMFILE_PARAM_CUSTOM_NAMING, TRUE);
    // set root folder
    snprintf(RootFolder,NMC_OTHERS_MAX_LEN,"A:\\%s\\",FILEDB_CARDV_ROOT);
    ImageApp_CamFile_SetRootFolder(RootFolder);
    // set folder name of 1st movie path
    ImageApp_CamFile_SetMovieFolder(0, "MOVIE");
    //ImageApp_CamFile_SetROFolder(0, "RO");
    ImageApp_CamFile_SetPhotoFolder(0, "PHOTO");
    #if (SAVE_DUAL_STREAM_FUNC == ENABLE)
    // set folder name of 2nd movie path
    ImageApp_CamFile_SetMovieFolder(1, WIFI_MOVIE_FOLDER);
    #endif
    #if ((SAVE_DUAL_STREAM_FUNC == ENABLE) || (TIMELAPSE_LPR_FUNCTION == ENABLE))
    // set custom file name
    //manual naming method
    ImageApp_CamFile_SetFileNamingMethod(FILE_NAMING_MANUAL);
    // register callback functions for manual file naming method
    ImageApp_CamFile_RegMovieFileNamingCB(UIMovie_MovieFileNamingCB);
    ImageApp_CamFile_RegPhotoFileNamingCB(UIMovie_PhotoFileNamingCB);
    ImageApp_CamFile_RegEMRMovFileNamingCB(UIMovie_EMRMovFileNamingCB);
    #endif
    #if (CUT_MOVIE_FUNC == ENABLE)
    ImageApp_CamFile_SetROFolder(1, CUT_MOVIE_FOLDER);
    #endif
    #endif//(YOUKU_SDK == ENABLE)
    //#NT#2016/03/07#KCHong -end
#else // sample code for Custom Naming Rule

    // enable custom naming
    ImageUnit_SetParam(&ISF_CamFile, CAMFILE_PARAM_CUSTOM_NAMING, TRUE);

    #if (SMALL_CLONE_MOVIE == DISABLE)
        // set root folder
        #if (FS_MULTI_STRG_FUNC == DISABLE)
        ImageApp_CamFile_SetRootFolder("A:\\CarDV\\");
        #else
        ImageApp_CamFile_SetRootFolderByPath(0, "A:\\CarDV\\");
        ImageApp_CamFile_SetRootFolderByPath(1, "B:\\CarDV\\");
        #endif

        // set folder name of 1st movie path
        ImageApp_CamFile_SetMovieFolder(0, "Movie_Front");
        ImageApp_CamFile_SetROFolder(0, "RO_Front");
        ImageApp_CamFile_SetEMRFolder(0, "EMR_Front");
        ImageApp_CamFile_SetPhotoFolder(0, "Photo_Front");

        // set folder name of 2nd movie path
        ImageApp_CamFile_SetMovieFolder(1, "Movie_Rear");
        ImageApp_CamFile_SetROFolder(1, "RO_Rear");
        ImageApp_CamFile_SetEMRFolder(1, "EMR_Rear");
        ImageApp_CamFile_SetPhotoFolder(1, "Photo_Rear");

        // set custom file name
        #if 0 // manual naming method
        {
            ImageApp_CamFile_SetFileNamingMethod(FILE_NAMING_MANUAL);
            // register callback functions for manual file naming method
            ImageApp_CamFile_RegMovieFileNamingCB(UIMovie_MovieFileNamingCB);
            ImageApp_CamFile_RegPhotoFileNamingCB(UIMovie_PhotoFileNamingCB);
            ImageApp_CamFile_RegEMRMovFileNamingCB(UIMovie_EMRMovFileNamingCB);
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
            ImageApp_CamFile_SetFileNameSerialNum(0, ENABLE);
            ImageApp_CamFile_SetFileNameSerialNum(1, ENABLE);
            ImageApp_CamFile_SetSemiAutoFileNaming(); // activate semi-auto file naming
        }
        #else // auto naming method => need to do nothing
        #endif
    #else // small size clone movie
        // set root folder
        #if (FS_MULTI_STRG_FUNC == DISABLE)
        ImageApp_CamFile_SetRootFolder(DV_ROOT_PATH);
        #else
        ImageApp_CamFile_SetRootFolderByPath(0, DV_ROOT_PATH);
        ImageApp_CamFile_SetRootFolderByPath(1, "B:\\CarDV\\");
        #endif

        // set folder name of 1st movie path
        ImageApp_CamFile_SetMovieFolder(0, MOVIE_FOLDER);
        ImageApp_CamFile_SetROFolder(0, MOVIE_RO_FOLDER);
        ImageApp_CamFile_SetEMRFolder(0, MOVIE_RO_FOLDER);
        ImageApp_CamFile_SetPhotoFolder(0, PHOTO_FOLDER);

        // set folder name of 2nd movie path
        ImageApp_CamFile_SetMovieFolder(1, MOVIE_SHARE_FOLDER);
        ImageApp_CamFile_SetROFolder(1, MOVIE_SHARE_RO_FOLDER);
        ImageApp_CamFile_SetEMRFolder(1, MOVIE_SHARE_RO_FOLDER);
        ImageApp_CamFile_SetPhotoFolder(1, PHOTO_FOLDER);

        // set custom file name
        ImageApp_CamFile_SetFileNamingMethod(FILE_NAMING_MANUAL);
        // register callback functions for manual file naming method
        ImageApp_CamFile_RegMovieFileNamingCB(UIMovie_MovieFileNamingCB);
        //ImageApp_CamFile_RegEMRMovFileNamingCB(UIMovie_EMRMovieFileNamingCB);
        ImageApp_CamFile_RegPhotoFileNamingCB(UIMovie_PhotoFileNamingCB);
    #endif
#endif
//#NT#2016/08/12#Hideo Lin -end
    #endif
}

INT32 SetupExe_OnFILEDB_FILEID_RESET(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);

void FlowMovie_FileDBCreate_Fast(void)
{
    if(System_GetState(SYS_STATE_POWERON) == SYSTEM_POWERON_NORMAL)
    {
        #if USE_FILEDB
        MEM_RANGE Pool;

        Pool.Addr = dma_getCacheAddr(OS_GetMempoolAddr(POOL_ID_FILEDB));
        Pool.Size = POOL_SIZE_FILEDB;
        ImageUnit_SetParam(&ISF_CamFile, CAMFILE_PARAM_USE_FILEDB, (UINT32)&Pool);
        // config FDB root
        ImageApp_CamFile_SetFolder(FILEDB_CARDV_ROOT);
        // create FDB
        ImageUnit_SetParam(&ISF_CamFile, CAMFILE_PARAM_MOVIE_FILEDB_CRATE, 1);
        // reset FDB file id
        if (UI_GetData(FL_IsUseFileDB))
        {
            //Ux_SendEvent(&UISetupObjCtrl,NVTEVT_FILEID_RESET,0);
            SetupExe_OnFILEDB_FILEID_RESET(NULL,0,NULL);
        }
        #endif
    }
}
BOOL FlowMovie_CheckReOpenItem(void)
{
    BOOL bReOpen = FALSE, bMovieSizeMenu = FALSE;

    #if defined(YQCONFIG_DEBUG_MESSAGE_SUPPORT)
    debug_msg("\n\r FlowMovie_CheckReOpenItem \n\r");
    #endif
    if(UI_GetData(FL_MOVIE_SIZE_MENU) != UI_GetData(FL_MOVIE_SIZE))
    {
        // Set resolution on mode open to avoid wrong preview image.
        UI_SetData(FL_MOVIE_SIZE, UI_GetData(FL_MOVIE_SIZE_MENU));
        bReOpen = TRUE;
        bMovieSizeMenu = TRUE;
    }

    if(UI_GetData(FL_MOVIE_HDR_MENU) != UI_GetData(FL_MOVIE_HDR))
    {
        if(UI_GetData(FL_MOVIE_HDR_MENU)==MOVIE_HDR_ON)
        {
            UI_SetData(FL_MOVIE_WDR, MOVIE_WDR_OFF);
            UI_SetData(FL_MOVIE_WDR_MENU, MOVIE_WDR_OFF);
            UI_SetData(FL_MovieRSCIndex, MOVIE_RSC_OFF);
            UI_SetData(FL_MovieRSCIndex_MENU, MOVIE_RSC_OFF);
        }
        UI_SetData(FL_MOVIE_HDR, UI_GetData(FL_MOVIE_HDR_MENU));
        bReOpen = TRUE;
    }

    if(UI_GetData(FL_MOVIE_WDR_MENU) != UI_GetData(FL_MOVIE_WDR))
    {
        #if defined(YQCONFIG_DEBUG_MESSAGE_SUPPORT)
        debug_msg("\n\r FL_MOVIE_WDR_MENU %d\n\r",UI_GetData(FL_MOVIE_WDR_MENU));
        #endif

        if(UI_GetData(FL_MOVIE_WDR_MENU)==MOVIE_WDR_ON)
        {
            UI_SetData(FL_MOVIE_HDR, MOVIE_HDR_OFF);
            UI_SetData(FL_MOVIE_HDR_MENU, MOVIE_HDR_OFF);
        }
        UI_SetData(FL_MOVIE_WDR, UI_GetData(FL_MOVIE_WDR_MENU));
        bReOpen = TRUE;
    }

    if(UI_GetData(FL_MovieRSCIndex_MENU) != UI_GetData(FL_MovieRSCIndex))
    {

        if(UI_GetData(FL_MovieRSCIndex_MENU)==MOVIE_RSC_ON)
        {
            UI_SetData(FL_MOVIE_HDR, MOVIE_HDR_OFF);
            UI_SetData(FL_MOVIE_HDR_MENU, MOVIE_HDR_OFF);
        }
        UI_SetData(FL_MovieRSCIndex, UI_GetData(FL_MovieRSCIndex_MENU));
        bReOpen = TRUE;
    }

#if (_SENSORLIB2_ != _SENSORLIB2_OFF_)
    if (bMovieSizeMenu)
    {
        switch (UI_GetData(FL_MOVIE_SIZE_MENU))
        {
        case MOVIE_SIZE_FRONT_2880x2160P24:
        case MOVIE_SIZE_FRONT_2560x1440P30:
        case MOVIE_SIZE_FRONT_1920x1080P60:
        case MOVIE_SIZE_FRONT_1280x720P120:
        case MOVIE_SIZE_FRONT_1280x720P60:
        case MOVIE_SIZE_FRONT_640x480P240:
            //#NT#2016/06/17#Brain Yen -begin
            //#NT#for special aspect panel and Pipview LR
            #if (PIP_VIEW_LR == ENABLE)
            UI_SetData(FL_DUAL_CAM_MENU, DUALCAM_LR_FRONT_FULL);
            #else
            UI_SetData(FL_DUAL_CAM_MENU, DUALCAM_FRONT);
            #endif
            //#NT#2016/06/17#Brain Yen -end
            break;

        default:
            //#NT#2016/06/17#Brain Yen -begin
            //#NT#for special aspect panel and Pipview LR
            #if (PIP_VIEW_LR == ENABLE)
            if(System_GetEnableSensor() & SENSOR_2)
            {
				#if (_MODEL_DSC_ == _MODEL_MIO_MT39_)
				UI_SetData(FL_DUAL_CAM_MENU, DUALCAM_LR_FULL);
				#else
                UI_SetData(FL_DUAL_CAM_MENU, DUALCAM_LR_16_9);
				#endif
            }
            else
            {
                UI_SetData(FL_DUAL_CAM_MENU, DUALCAM_LR_FRONT_FULL);
            }
            #else
            if(System_GetEnableSensor() & SENSOR_2)
            {
                UI_SetData(FL_DUAL_CAM_MENU, DUALCAM_BOTH);
            }
            else
            {
                UI_SetData(FL_DUAL_CAM_MENU, DUALCAM_FRONT);
            }
            #endif
            //#NT#2016/06/17#Brain Yen -end
            break;
        }


    }
#endif

    if(UI_GetData(FL_DUAL_CAM_MENU) != UI_GetData(FL_DUAL_CAM))
    {
        if (bMovieSizeMenu == FALSE)
        {
            switch (UI_GetData(FL_MOVIE_SIZE_MENU))
            {
            case MOVIE_SIZE_FRONT_2880x2160P24:
            case MOVIE_SIZE_FRONT_2560x1440P30:
            case MOVIE_SIZE_FRONT_1920x1080P60:
            case MOVIE_SIZE_FRONT_1280x720P120:
            case MOVIE_SIZE_FRONT_1280x720P60:
            case MOVIE_SIZE_FRONT_640x480P240:
                break;

            default:
        Ux_PostEvent(NVTEVT_EXE_DUALCAM, 1, UI_GetData(FL_DUAL_CAM_MENU));
                break;
            }
        }
        else
        {
            UI_SetData(FL_DUAL_CAM, UI_GetData(FL_DUAL_CAM_MENU));
            #if (PIP_VIEW_FUNC == ENABLE)
            PipView_SetStyle(UI_GetData(FL_DUAL_CAM));
            #endif
        }

        if (bMovieSizeMenu == FALSE)
            bReOpen = FALSE; // because in NVTEVT_EXE_DUALCAM, NVTEVT_EXE_MOVIE_DUALCAM will invoke ReOpen
    }

#if (WIFI_FINALCAM_APP_STYLE == ENABLE || YOUKU_SDK == ENABLE)
    {
        UINT32 uiCurRecWidth, uiCurRecHeight;
        UINT32 uiTargetWidth, uiTargetHeight;

        MovRec_GetInfo(MOVREC_GETINFO_WIDTH_HEIGHT_P2, &uiCurRecWidth, &uiCurRecHeight,0);

        DBG_DUMP("^YCurW:%d, CurH:%d\r\n", uiCurRecWidth, uiCurRecHeight);

        switch(UI_GetData(FL_WIFI_MOVIE_APP_PREVIEW_SIZE))
        {
            default:
            case WIFI_MOVIE_APP_PREVIEW_SIZE_VGA_16_9:  // 640 x 360
            uiTargetWidth = 640;
            uiTargetHeight = 360;
            break;

            case WIFI_MOVIE_APP_PREVIEW_SIZE_720P:      // 1280 x 720
            uiTargetWidth = 1280;
            uiTargetHeight = 720;
            break;

            case WIFI_MOVIE_APP_PREVIEW_SIZE_WVGA:      // 848 x 480
            uiTargetWidth = 848;
            uiTargetHeight = 480;
            break;

            case WIFI_MOVIE_APP_PREVIEW_SIZE_VGA_4_3:   // 640 x 480
            uiTargetWidth = 640;
            uiTargetHeight = 480;
            break;

            case WIFI_MOVIE_APP_PREVIEW_SIZE_QVGA:      // 320 x 240
            uiTargetWidth = 320;
            uiTargetHeight = 240;
            break;
        }

        if(uiTargetWidth != uiCurRecWidth || uiTargetHeight !=uiCurRecHeight)
            bReOpen = TRUE;
    }
#elif (UCTRL_APP_MOVIE_FEATURE_SETGET == ENABLE)
    // check channel 1
    {
        UINT32 uiCurRecWidth=0, uiCurRecHeight=0;
        UINT32 uiTargetWidth=0, uiTargetHeight=0;
        UINT32 channelId=0;

        MovRec_GetInfo(MOVREC_GETINFO_WIDTH_HEIGHT, &uiCurRecWidth, &uiCurRecHeight,0);

        DBG_DUMP("^YPath 1-> CurW:%d, CurH:%d\r\n", uiCurRecWidth, uiCurRecHeight);

        uiTargetWidth = GetMovieSizeWidth_2p(channelId, 0);
        uiTargetHeight = GetMovieSizeHeight_2p(channelId,0);
        if(uiTargetWidth != uiCurRecWidth || uiTargetHeight !=uiCurRecHeight)
            bReOpen = TRUE;
    }
    // check channel 2
    {
        UINT32 uiCurRecWidth=0, uiCurRecHeight=0;
        UINT32 uiTargetWidth=0, uiTargetHeight=0;
        UINT32 channelId=1;

        MovRec_GetInfo(MOVREC_GETINFO_WIDTH_HEIGHT_P2, &uiCurRecWidth, &uiCurRecHeight,0);

        DBG_DUMP("^YPath 2-> CurW:%d, CurH:%d\r\n", uiCurRecWidth, uiCurRecHeight);

        uiTargetWidth = GetMovieSizeWidth_2p(channelId, 0);
        uiTargetHeight = GetMovieSizeHeight_2p(channelId,0);

        if(uiTargetWidth != uiCurRecWidth || uiTargetHeight !=uiCurRecHeight)
            bReOpen = TRUE;
    }
#endif

//#NT#2016/09/13#KCHong#[0109190][0106941][0105955][0104994][0099778] -begin
//#NT#Check whether FC/LD/Timelapse is changed no matter _ADAS_FUNC_ is enabled or not (Also merge the comments for reading)
#if 1	//(_ADAS_FUNC_ == ENABLE)
	if (UI_GetData(FL_MOVIE_LDWS) != UI_GetData(FL_MOVIE_LDWS_MENU)) {
		UI_SetData(FL_MOVIE_LDWS, UI_GetData(FL_MOVIE_LDWS_MENU));
		bReOpen = TRUE;
	}

	if (UI_GetData(FL_MOVIE_FCW) != UI_GetData(FL_MOVIE_FCW_MENU)) {
		UI_SetData(FL_MOVIE_FCW, UI_GetData(FL_MOVIE_FCW_MENU));
		bReOpen = TRUE;
	}

	if (UI_GetData(FL_MOVIE_TIMELAPSE_REC) != UI_GetData(FL_MOVIE_TIMELAPSE_REC_MENU)) {
		if ((UI_GetData(FL_MOVIE_TIMELAPSE_REC) == MOVIE_TIMELAPSEREC_OFF) &&
			(UI_GetData(FL_MOVIE_LDWS) == MOVIE_LDWS_ON || UI_GetData(FL_MOVIE_FCW) == MOVIE_FCW_ON)) {
			bReOpen = TRUE;
		}
		UI_SetData(FL_MOVIE_TIMELAPSE_REC, UI_GetData(FL_MOVIE_TIMELAPSE_REC_MENU));
	}
#endif
//#NT#2016/09/13#KCHong#[0109190][0106941][0105955][0104994][0099778] -end

    return bReOpen;
}

//#NT#2016/03/02#Lincy Lin -begin
//#NT#Support object tracking function
INT32 MovieExe_OnOTEnd(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    #if MOVIE_OT_FUNC
    {
        //Flush OT event before draw
        Ux_FlushEventByRange(NVTEVT_EXE_MOVIE_OTEND,NVTEVT_EXE_MOVIE_OTEND);
        MovieExe_CallBackUpdateInfo(UIAPPPHOTO_CB_OTEND);
    }
    #endif
    return NVTEVT_CONSUME;

}
//#NT#2016/03/02#Lincy Lin -end

//#NT#2016/06/08#Lincy Lin -begin
//#NT#Implement generic OSD and video drawing mechanism for ALG function

INT32 MovieExe_OnALGEnd(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //Flush ALG event before draw
    Ux_FlushEventByRange(NVTEVT_EXE_MOVIE_ALGEND,NVTEVT_EXE_MOVIE_ALGEND);
    #if defined(YQCONFIG_ANDROID_SYSTEM_SUPPORT)
    static UINT8 uSendCount = 0;
    uSendCount++;
    if(uSendCount== 254)uSendCount = 0;
    if(uSendCount%2 == 0)
    #endif
    MovieExe_CallBackUpdateInfo(UIAPPPHOTO_CB_ALGEND);
    return NVTEVT_CONSUME;
}
//#NT#2016/06/08#Lincy Lin -end

//#NT#2016/05/25#David Tsai -begin
//#NT#Support tampering detection function
INT32 MovieExe_OnTDEnd(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    #if MOVIE_TD_FUNC
    {
        //Flush TD event before draw
        Ux_FlushEventByRange(NVTEVT_EXE_MOVIE_TDEND,NVTEVT_EXE_MOVIE_TDEND);
        MovieExe_CallBackUpdateInfo(UIAPPPHOTO_CB_TDEND);
    }
    #endif
    return NVTEVT_CONSUME;
}
//#NT#2016/05/25#David Tsai -end

//#NT#2016/10/14#Yuzhe Bian -begin
//#NT# Support trip-wire detection & trip-zone detection function
INT32 MovieExe_OnTWDEnd(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    #if MOVIE_TWD_FUNC
    {
        //Flush TWD event before draw
        Ux_FlushEventByRange(NVTEVT_EXE_MOVIE_TWDEND,NVTEVT_EXE_MOVIE_TWDEND);
        MovieExe_CallBackUpdateInfo(UIAPPPHOTO_CB_TWDEND);
    }
    #endif
    return NVTEVT_CONSUME;
}
INT32 MovieExe_OnTZDEnd(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    #if MOVIE_TZD_FUNC
    {
        //Flush TZD event before draw
        Ux_FlushEventByRange(NVTEVT_EXE_MOVIE_TZDEND,NVTEVT_EXE_MOVIE_TZDEND);
        MovieExe_CallBackUpdateInfo(UIAPPPHOTO_CB_TZDEND);
    }
    #endif
    return NVTEVT_CONSUME;
}
//#NT#2016/10/14#Yuzhe Bian -end

//#NT#2016/06/16#KCHong -begin
//#NT#ImageUnit_ALG
INT32 MovieExe_OnADASSetCropWin(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    URECT CropWin;

    CropWin.x = (paramArray[0] & 0xffff0000) >> 16;
    CropWin.y = (paramArray[0] & 0x0000ffff);
    CropWin.w = (paramArray[1] & 0xffff0000) >> 16;
    CropWin.h = (paramArray[1] & 0x0000ffff);

    //DBG_DUMP("UI: Set crop = %d/%d/%d/%d\r\n", CropWin.x, CropWin.y, CropWin.w, CropWin.h);
    ImageUnit_Begin(&ISF_ALG, 0);
        ImageUnit_CfgWindow(ISF_IN2, CropWin.x, CropWin.y, CropWin.w, CropWin.h);
    ImageUnit_End();
    ImageStream_UpdateSize();
    return NVTEVT_CONSUME;
}
//#NT#2016/06/16#KCHong -end

//#NT#2016/10/18#Jeah Yen -begin
//#NT#move code to support sensor map
INT32 MovieExe_OnMotionDetRun(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    BOOL bOK = MD_Process(IPL_PATH_1);
    //#NT#2016/10/26#Jeah Yen -begin
    //#NT#fix mantis 0111898
    if(paramNum >= 1)
        *(UINT32*)(paramArray[0]) = (UINT32)bOK;
    //#NT#2016/10/26#Jeah Yen -end
    return NVTEVT_CONSUME;
}
//#NT#2016/10/18#Jeah Yen -begin

////////////////////////////////////////////////////////////

EVENT_ENTRY CustomMovieObjCmdMap[] =
{
    {NVTEVT_EXE_OPEN,                 MovieExe_OnOpen                     },
    {NVTEVT_EXE_CLOSE,                MovieExe_OnClose                    },
    {NVTEVT_EXE_MOVIESIZE,            MovieExe_OnMovieSize                },
    {NVTEVT_EXE_IMAGE_RATIO,          MovieExe_OnImageRatio               },
    {NVTEVT_EXE_MOVIEQUALITY,         MovieExe_OnMovieQuality             },
    {NVTEVT_EXE_WB,                   MovieExe_OnWB                       },
    {NVTEVT_EXE_MOVIECOLOR,           MovieExe_OnColor                    },
    {NVTEVT_EXE_MOVIE_EV,             MovieExe_OnEV                       },
    {NVTEVT_EXE_MOVIE_AUDIO,          MovieExe_OnMovieAudio               },
    {NVTEVT_EXE_MOVIE_AUDIO_VOL,      MovieExe_OnMovieAudioRecVolume      },
    {NVTEVT_EXE_MOVIE_AUDIO_REC,      MovieExe_OnMovieAudioRec            },
    {NVTEVT_EXE_MOVIEDZOOM,           MovieExe_OnDigitalZoom              },
    {NVTEVT_EXE_MOVIEDIS,             MovieExe_OnDis                      },
    {NVTEVT_EXE_MOVIE_MCTF,           MovieExe_OnMCTF                     },
    {NVTEVT_EXE_MOVIE_EDGE,           MovieExe_OnEdge                     },
    {NVTEVT_EXE_MOVIE_NR,             MovieExe_OnNR                       },
    {NVTEVT_EXE_MOVIE_RSC,            MovieExe_OnRSC                      },
    {NVTEVT_EXE_MOVIE_WDR,            MovieExe_OnWDR                      },
    {NVTEVT_EXE_SHDR,                 MovieExe_OnSHDR                     },
    {NVTEVT_EXE_MOVIE_SENSOR_ROTATE,  MovieExe_OnSensorRotate             },
    {NVTEVT_EXE_MOVIE_IR_CUT,         MovieExe_OnIRCut                    },
    {NVTEVT_EXE_MOVIEGDC,             MovieExe_OnGdc                      },
    {NVTEVT_EXE_MOVIESMEAR,           MovieExe_OnSmear                    },
    {NVTEVT_EXE_MOVIE_DATE_IMPRINT,   MovieExe_OnDateImprint              },
    {NVTEVT_EXE_GSENSOR,              MovieExe_OnGSENSOR                  },
    {NVTEVT_EXE_CYCLIC_REC,           MovieExe_OnCyclicRec                },
    {NVTEVT_EXE_MOTION_DET,           MovieExe_OnMotionDet                },
    {NVTEVT_CALLBACK,                 MovieExe_OnCallback                 },
    //#NT#2012/07/31#Hideo Lin -begin
    //#NT#For custom movie record/stop event
    {NVTEVT_EXE_MOVIE_REC_START,      MovieExe_OnRecStart                 },
    {NVTEVT_EXE_MOVIE_REC_STEP,       MovieExe_OnRecStep                  },
    {NVTEVT_EXE_MOVIE_REC_STOP,       MovieExe_OnRecStop                  },
    {NVTEVT_EXE_MOVIE_REC_PIM,        MovieExe_OnRecPIM                   },
    {NVTEVT_EXE_MOVIE_REC_RAWENC,     MovieExe_OnRecRawEnc                },
    //#NT#2012/07/31#Hideo Lin -end
    //#NT#2016/06/21#Jeah Yen -begin
    //#NT#Support display fast switch
    {NVTEVT_VIDEO_CHANGE,             MovieExe_OnVideoChange              },
    //#NT#2016/06/21#Jeah Yen -end
    {NVTEVT_EXE_DUALCAM,              MovieExe_OnDualcam                  },
    {NVTEVT_EXE_MOVIE_FDEND,          MovieExe_OnFDEnd                    },
    {NVTEVT_EXE_MOVIE_PROTECT_AUTO,   MovieExe_OnProtectAuto              },
    {NVTEVT_EXE_MOVIE_PROTECT_MANUAL, MovieExe_OnProtectManual           },

    {NVTEVT_EXE_MOVIE_LDWS,           MovieExe_OnLDWS                     },
    {NVTEVT_EXE_MOVIE_FCW,            MovieExe_OnFCW                      },
    {NVTEVT_EXE_VIDEO_PAUSE_RESUME,   MovieExe_OnPauseResume              },
    //#NT#2016/03/02#Lincy Lin -begin
    //#NT#Support object tracking function
    {NVTEVT_EXE_MOVIE_OTEND,          MovieExe_OnOTEnd                    },
    //#NT#2016/03/03#Lincy Lin -end
    //#NT#2016/05/25#David Tsai -begin
    //#NT#Support tampering detection function
    {NVTEVT_EXE_MOVIE_TDEND,          MovieExe_OnTDEnd                    },
    //#NT#2016/05/25#David Tsai -end
    //#NT#2016/10/14#Yuzhe Bian -begin
    //#NT# Support trip-wire detection & trip-zone detection function
    {NVTEVT_EXE_MOVIE_TWDEND,         MovieExe_OnTWDEnd                   },
    {NVTEVT_EXE_MOVIE_TZDEND,         MovieExe_OnTZDEnd                   },
    //#NT#2016/10/14#Yuzhe Bian -end
    {NVTEVT_EXE_MOVIE_FTGEND,         MovieExe_OnFTGEnd                   },
    //#NT#2016/03/25#KCHong -begin
    //#NT#New ADAS
    {NVTEVT_CB_ADAS_SETCROPWIN,       MovieExe_OnADASSetCropWin           },
    //#NT#2016/03/25#KCHong -end
    //#NT#2016/06/08#Lincy Lin -begin
    //#NT#Implement generic OSD and video drawing mechanism for ALG function
    {NVTEVT_EXE_MOVIE_ALGEND,         MovieExe_OnALGEnd                    },
    //#NT#2016/06/08#Lincy Lin -end
    //#NT#2016/06/03#Charlie Chang -begin
    //#NT# support contrast, audioIn, flip, quality set
    {NVTEVT_EXE_MOVIE_CONTRAST,       MovieExe_OnContrast                 },
    {NVTEVT_EXE_MOVIE_AUDIOIN ,       MovieExe_OnAudioIn                  },
    {NVTEVT_EXE_MOVIE_AUDIOIN_SR,     MovieExe_OnAudioInSR                },
    {NVTEVT_EXE_MOVIE_FLIP_MIRROR,    MovieExe_OnFipMirror                },
    {NVTEVT_EXE_MOVIE_QUALITY_SET,    MovieExe_OnQualitySet               },
    //#NT#2016/06/03#Charlie Chang -end
    //#NT#2017/03/03#Jeah Yen -begin
    //#NT# support sbs mode
    {NVTEVT_EXE_MOVIE_SBS_MODE,       MovieExe_OnSBSMode               },
    //#NT#2017/03/03#Jeah Yen -end
    //#NT#2016/10/18#Jeah Yen -begin
    //#NT#move code to support sensor map
    {NVTEVT_EXE_MOTION_DET_RUN,       MovieExe_OnMotionDetRun             },
    //#NT#2016/10/18#Jeah Yen -begin
    {NVTEVT_NULL,                     0},  //End of Command Map
};

CREATE_APP(CustomMovieObj,APP_SETUP)

