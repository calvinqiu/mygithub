#include <stdio.h>
#include <string.h>
#include "kernel.h"
#include "SysCfg.h"
#include "Utility.h"
#include "HwClock.h"
#include "GxImage.h"
#include "MovieStamp.h"
#include "DateStampFont10x16.h"
#include "DateStampFont12x20.h"
#include "DateStampFont18x30.h"
#include "DateStampFont20x44.h"
#include "DateStampFont26x44.h"
#include "IQS_Utility.h"
#include "timer.h"
#include "SwTimer.h"
#include "MovieInterface.h"
#include "MovieStampAPI.h"
#include "adas_Apps.h"     //#NT#2016/04/13#Correct the include file#KCHong
#include "fcws_lib.h"
#include "PrjCfg.h"
#include "UIInfo.h"
#include "UIPhotoFuncInfo.h"


#if defined (MULTILINGUAL_FUNCTION) && (MULTILINGUAL_FUNCTION == ENABLE)
#include "UniFont.h"
#endif

#include "dma.h"
#include "grph.h"
#include "FTypeIpcAPI.h"
#include "MovieStampID.h"
//#NT#2016/03/02#Lincy Lin -begin
//#NT#Support object tracking function
#if MOVIE_OT_FUNC
#include "display.h"
#include "UIFlow.h"
#include "NvtOt.h"
#endif
//#NT#2016/03/02#Lincy Lin -end
//#NT#2016/05/23#David Tsai -begin
//#NT# Support tampering detection function

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

//#NT#2016/06/02#Nestor Yang -begin
//#NT# Support Driver Drowsiness Detection(DDD)
#if (MOVIE_DDD_FUNC && MOVIE_DDD_DRAW_VIDEO)
#include "DDD_lib.h"
#endif
//#NT#2016/06/02#Nestor Yang -end
//#NT#2016/06/08#Lincy Lin -begin
//#NT#Implement generic OSD and video drawing mechanism for ALG function
#if MOVIE_FD_FUNC_
#include "fd_lib.h"
#endif
//#NT#2016/06/08#Lincy Lin -end

//#NT#2016/03/24#KCHong -begin
//#NT#Log ADAS/IPP info in GPS section
#include "UIAppMovie.h"
//#NT#2016/03/24#KCHong -end
#if (IPCAM_FUNC)
#include <HwMem.h>
#include "UIControl.h"
#include "UIControlWnd.h"
#endif
//#NT#2016/06/28#KCHong -begin
//#NT#DbgLog in GPS section
#if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
#include "mp4log.h"
#endif
//#NT#2016/06/28#KCHong -end

#if (MOVIE_TSR_FUNC_)
#include "tsr_lib.h"
#include "TSD_DSP_lib.h"
#endif

//#NT#2016/10/17#Bin Xiao -begin
//#NT# Support Face Tracking Grading(FTG)
#if MOVIE_FTG_FUNC
#include "ftg_lib.h"
#endif
//#NT#2016/10/17#Bin Xiao -end
#define __MODULE__              MovieStamp
#define __DBGLVL__              1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__              "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

#define min(a, b) ((a) > (b) ? (b) : (a))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define swap(x,y) {int t; t=x; x=y; y=t; }

#define CHECK_STAMP_TIME        DISABLE
#define CHECK_ENC_INFO          DISABLE
//#define WATERLOGO_FUNCTION      DISABLE // TEMPORARY!     //#NT#2015/11/04#Move this definition to PrjCfg_xxx.h#KCHong

#define COLOR_IDX_2BIT          0   // 2-bit color index
#define COLOR_IDX_4BIT          1   // 4-bit color index
#define COLOR_IDX_MODE          COLOR_IDX_2BIT

#define COLOR_BG_Y              0x00
#define COLOR_BG_U              0x80
#define COLOR_BG_V              0x80
#define COLOR_FR_Y              0x00
#define COLOR_FR_U              0x80
#define COLOR_FR_V              0x80
#define COLOR_FG_Y              RGB_GET_Y(250, 160, 10)
#define COLOR_FG_U              RGB_GET_U(250, 160, 10)
#define COLOR_FG_V              RGB_GET_V(250, 160, 10)
#define COLOR_CK_Y              0x01
#define COLOR_CK_U              0x01
#define COLOR_CK_V              0x01
#define COLOR_ID_BG             0
#define COLOR_ID_FR             1
#define COLOR_ID_FG             2
#define COLORKEY_BG_YUV         0x00000000  // stamp BG data for color key operation
#define COLORKEY_YUV            0x00010101  // color key for transparent background

#define STAMP_WIDTH_MAX         1920        // maximum stamp width (for stamp buffer line-offset setting)
#define STAMP_WIDTH_TOLERANCE   8           // total font width error tolerance
#define STAMP_LOGO_GAP          8           // date stamp and water logo position gap
#define VIDEO_IN_MAX            3           // max 2 video paths
#define MOVIE_STAMP_BUF_0       0
#define MOVIE_STAMP_BUF_1       1
#define MOVIE_STAMP_CHK_TIME    50          // 50ms check once
#define MOVIE_STAMP_MAX_LEN     256
#if defined (MULTILINGUAL_FUNCTION) && (MULTILINGUAL_FUNCTION == ENABLE)
#define LINGUAL_MAX               32
#define LINGUAL_STAMP_BUF_0       0
#define LINGUAL_STAMP_BUF_1       1
#define LINGUAL_STAMP_BUF_TATAL   2
#define LINGUAL_STAMP_BUF_OFF     0xFFFFFFFF
#endif

//variable declare
static STAMP_POS    g_MovieStampPos[VIDEO_IN_MAX] =
{
    {0, 0}, // stamp position for primary image 1
    {0, 0},  // stamp position for primary image 2
    {0, 0}
};
char         g_cMovieStampStr[VIDEO_IN_MAX][MOVIE_STAMP_MAX_LEN];
static UINT32       g_uiMovieStampSetup[VIDEO_IN_MAX] = {STAMP_OFF, STAMP_OFF, STAMP_OFF};
static STAMP_INFO   g_MovieStampInfo[VIDEO_IN_MAX];
static struct tm    g_CurDateTime;
static UINT32       g_uiMovieStampActBuf = MOVIE_STAMP_BUF_0;   // movie stamp active buffer
//#NT#2016/08/19#Hideo Lin -begin
//#NT#To avoid different stamp overlap for same source image
static UINT32       g_uiMovieSrcYAddr[VIDEO_IN_MAX] = {0};
//#NT#2016/08/19#Hideo Lin -end
#if (IPCAM_FUNC && !MOVIE_AE_LOG)
static UINT32       g_uiMovieStampYAddr[2] = {0}; // movie stamp Y address (2 means double buffer)
static UINT32       g_uiMovieStampSize = 0; // buffer size for movie stamp
#else
static UINT32       g_uiMovieStampYAddr[VIDEO_IN_MAX][2] = {0}; // movie stamp Y address (2 means double buffer)
static UINT32       g_uiMovieStampSize[VIDEO_IN_MAX] = {0}; // buffer size for movie stamp
static UINT32       g_uiMovieStampWidth[VIDEO_IN_MAX] = {0};    // movie stamp data width
static UINT32       g_uiWaterLogoYAddr[VIDEO_IN_MAX] = {0}, g_uiWaterLogoUVAddr[VIDEO_IN_MAX] = {0};
static UINT32       g_uiLingualStampYAddr[VIDEO_IN_MAX] = {0};
static UINT32       g_uiLingualScaledStampYAddr[VIDEO_IN_MAX][2] = {0};      // (2 means double buffer)
#endif
//static UINT32       g_uiMovieImageWidth[VIDEO_IN_MAX] = {0};    // movie image width
static UINT32       g_uiMovieStampFormat[VIDEO_IN_MAX] = {STAMP_FORMAT_YUV, STAMP_FORMAT_YUV, STAMP_FORMAT_YUV};  // movie stamp format (YUV or color index)
//static UINT32       g_uiMovieStampHeightMax = 44; // maximum height of date stamp font (gDateStampFont26x44)
static BOOL         g_bStampColorSetup[VIDEO_IN_MAX] = {FALSE, FALSE, FALSE};
static BOOL         g_bMovieStampSwTimerOpen = FALSE;
static SWTIMER_ID   g_MovieStampSwTimerID;
static WATERLOGO_BUFFER g_MovieWaterInfo[VIDEO_IN_MAX] = {0};
static BOOL         g_bWaterLogoEnable[VIDEO_IN_MAX] = {0};
#if defined (MULTILINGUAL_FUNCTION) && (MULTILINGUAL_FUNCTION == ENABLE)
static UINT32       g_uiLingualStampActBuf = LINGUAL_STAMP_BUF_OFF;          // Set lignual stamp active buffer to disable
static STAMP_POS    g_LingualStampPos[VIDEO_IN_MAX] = {0};
static UINT32       g_uiLingualStampWidth[VIDEO_IN_MAX] = {0};
static UINT32       g_uiLingualScaledStampWidth[VIDEO_IN_MAX][2] = {0};
static UINT32       g_uiLingualStampHeight = 0;
static UINT32       g_uiLingualScaledStampHeight = 0;
static UINT32       g_uiLingualStampScalingFactor[VIDEO_IN_MAX] = {44, 44, 44};

// sample unicode string
UINT16 unicode[] = {
    0x806f, 0x8a60, 0x79d1, 0x6280, 0x80a1, 0x4efd, 0x6709, 0x9650, 0x516c, 0x53f8
  };

LINGUAL_INFO    LingualInfo[LINGUAL_MAX];
#endif //

//#NT#2016/04/22#Nestor Yang -begin
//#NT# Support Driver Drowsiness Detection(DDD)
#if (MOVIE_DDD_FUNC && MOVIE_DDD_DRAW_VIDEO)
UINT32 g_DDD_DowsyState;
//#NT#2016/06/02#Nestor Yang -begin
//#NT# Support Driver Drowsiness Detection(DDD)
void Movie_DrawDDD(PIMG_BUF pDstImg);
//#NT#2016/06/02#Nestor Yang -end
#endif
//#NT#2016/04/22#Nestor Yang -end

#if 0 //_ADAS_FUNC_     //#NT#Remove unused variables#2016/04/13#KCHong
extern UINT32 g_uiFcDist;
extern UINT32 g_uiFCW_E;
extern UINT32 g_Failure;
extern FCWS_DEBUG_INFO g_FcDebugInfo;
#endif

#if (IPCAM_FUNC)
UINT32 use_ftype_img[OSD_MAX_CHANNEL] = { 0 };
UINT32 ftype_img_num                        = 0;
UINT8  *ftype_img_src                       = NULL;
UINT8  *ftype_img_dst                       = NULL;
#endif

#if defined(YQCONFIG_ANDROID_SYSTEM_SUPPORT)
extern FLOAT g_CurSpeed;
#endif
void MovieStamp_InitImeStamp(UINT32 uiVidEncId);
//#NT#2016/06/08#Lincy Lin -begin
//#NT#Implement generic OSD and video drawing mechanism for ALG function
void MovieStamp_DrawLineUV(INT32 x1, INT32 y1, INT32 x2, INT32 y2, UINT32 DrawColor, PIMG_BUF pDstImg);
void MovieStamp_DrawPointUV(INT32 x, INT32 y, UINT32 DrawColor, PIMG_BUF pDstImg);
void MovieStamp_DrawRectUV(IRECT *pRect, UINT32 DrawColor, PIMG_BUF pDstImg);
void MovieStamp_DrawALG(UINT32 uiVidEncId,PIMG_BUF pDstImg);
//#NT#2016/06/08#Lincy Lin -end


//#NT#2016/12/07#Charlie Chang -begin
//#NT# for FTG draw frame count
UINT32 g_uiFtgDrawDelayCount = 5;  //how many frame update ftg infomation
UINT32 g_uiFtgRlstType = 0;
//#NT#2016/12/07#Charlie Chang -end
BOOL g_bPipEnable = FALSE;

//#NT#2017/02/16#Charlie Chang -begin
//#NT# support blending
static MOVIESTAMP_BLENDING_CFG g_MovieStampBlendCfg = {0};

//#NT#2017/02/16#Charlie Chang -end
//-------------------------------------------------------------------------------------------------
static void MovieStamp_SwTimerHdl(UINT32 uiEvent)
{
    struct tm   CurDateTime;

    CurDateTime = HwClock_GetTime(TIME_ID_CURRENT);

    // check time varying
#if MOVIE_AE_LOG || MOVIE_ADAS_LOG
    if (1)
#else
     if((g_CurDateTime.tm_sec  != CurDateTime.tm_sec)  ||
        (g_CurDateTime.tm_min  != CurDateTime.tm_min)  ||
        (g_CurDateTime.tm_hour != CurDateTime.tm_hour) ||
        (g_CurDateTime.tm_mday != CurDateTime.tm_mday) ||
        (g_CurDateTime.tm_mon  != CurDateTime.tm_mon)  ||
        (g_CurDateTime.tm_year != CurDateTime.tm_year))
#endif
    {
        g_CurDateTime = CurDateTime;

        // time varied, update stamp database
        MovieStampTsk_TrigUpdate();
    }
}

static void MovieStamp_SwTimerOpen(void)
{
    if (g_bMovieStampSwTimerOpen == FALSE)
    {
        if (SwTimer_Open(&g_MovieStampSwTimerID, MovieStamp_SwTimerHdl) != E_OK)
        {
            DBG_ERR("Sw timer open failed!\r\n");
            return;
        }

        SwTimer_Cfg(g_MovieStampSwTimerID, MOVIE_STAMP_CHK_TIME, SWTIMER_MODE_FREE_RUN);
        SwTimer_Start(g_MovieStampSwTimerID);
        g_bMovieStampSwTimerOpen = TRUE;
    }
}

static void MovieStamp_SwTimerClose(void)
{
    if (g_bMovieStampSwTimerOpen)
    {
        SwTimer_Stop(g_MovieStampSwTimerID);
        SwTimer_Close(g_MovieStampSwTimerID);
        g_bMovieStampSwTimerOpen = FALSE;
    }
}

void Acquire_FType_Buf(void)
{
    wai_sem(FTYPEIPC_SEM_ID);
}

void Release_FType_Buf(void)
{
    sig_sem(FTYPEIPC_SEM_ID);
}

#if (IPCAM_FUNC && !MOVIE_AE_LOG)
static void FType_UpdateData(void);
INT32 FType_CB(unsigned int image_number)
{
    unsigned int i;
    NVTFTYPE_IPC_IMAGE *user_data;

    //#NT#2016/04/22#YongChang Qui -begin
    //#NT#use osd double buffer to avoid broken osd
    Acquire_FType_Buf();
    //#NT#2016/04/22#YongChang Qui -end

    ftype_img_num = 0;
    for(i = 0 ; i < OSD_MAX_CHANNEL ; ++i)
        use_ftype_img[i] = 0;

    user_data = (NVTFTYPE_IPC_IMAGE *)ftype_img_src;
    for(i = 0 ; i < image_number ; ++i){

        if(!user_data[i].on)
            continue;

        if(user_data[i].channel < 0 && user_data[i].channel >= OSD_MAX_CHANNEL){
            DBG_ERR("invalid image[%d] channel[%d]\r\n", i, user_data[i].channel);
            goto out;
        }

        if(user_data[i].width != ALIGN_CEIL_4(user_data[i].width)){
            DBG_ERR("image[%d] width(%d) is not multiple of 4\r\n", i, user_data[i].width);
            goto out;
        }
    }

    ftype_img_num = image_number;
    for(i = 0 ; i < OSD_MAX_CHANNEL ; ++i)
        use_ftype_img[i] = 1;

    hwmem_open();
    hwmem_memcpy((UINT32)ftype_img_dst, (UINT32)ftype_img_src, POOL_SIZE_FTYPEIPC / 2);
    hwmem_close();

    FType_UpdateData();

out:
    Release_FType_Buf();
    //#NT#2016/06/02#YongChang Qui -end
    //#NT#2016/04/22#YongChang Qui -end

    extern VControl FlowIPCamCtrl;
    UxCtrl_SetDirty(&FlowIPCamCtrl,TRUE);
    Ux_Redraw();

    return 0;
}
#endif

void MovieStamp_Enable(void)
{
    g_CurDateTime = HwClock_GetTime(TIME_ID_CURRENT);

    //#NT#2016/08/19#Hideo Lin -begin
    //#NT#To avoid different stamp overlap for same source image
    UINT32 i;
    for (i = 0; i < VIDEO_IN_MAX; i++)
        g_uiMovieSrcYAddr[i] = 0;
    //#NT#2016/08/19#Hideo Lin -end

#if (IPCAM_FUNC && !MOVIE_AE_LOG)
    UINT32 sharedMemAddr = 0;
    static int freetype_init = 0;
    if(!freetype_init){

        freetype_init = 1;

        sharedMemAddr = OS_GetMempoolAddr(POOL_ID_FTYPEIPC);
        if (dma_isCacheAddr(sharedMemAddr))
        {
            DBG_ERR("sharedMemAddr 0x%x should be non-cache Address\r\n",(int)sharedMemAddr);
            return ;
        }
        //#NT#2016/04/22#YongChang Qui -begin
        //#NT#use osd double buffer to avoid broken osd
        ftype_img_src = (UINT8*)sharedMemAddr;
        ftype_img_dst = (UINT8*)(sharedMemAddr + ((POOL_SIZE_FTYPEIPC * 2) / 10));
        FtypeIpc_Open(ftype_img_src, (POOL_SIZE_FTYPEIPC * 2) / 10, FType_CB);
        MovieStamp_SetBuffer(sharedMemAddr + ((POOL_SIZE_FTYPEIPC * 4) / 10), (POOL_SIZE_FTYPEIPC * 6) / 10);
        //#NT#2016/04/22#YongChang Qui -end
    }
#else
    MovieStamp_UpdateData();

    // register movie stamp update callback
    MovieStampTsk_RegUpdateCB(MovieStamp_UpdateData);
#endif
    // open movie stamp task to wait for stamp update flag
    MovieStampTsk_Open();

    // use SW timer to check current time
    MovieStamp_SwTimerOpen();
}

void MovieStamp_Disable(void)
{
    // close SW timer
    MovieStamp_SwTimerClose();

    // close movie stamp task
    MovieStampTsk_Close();
}

//-------------------------------------------------------------------------------------------------
void MovieStamp_Setup(UINT32 uiVidEncId, UINT32 uiFlag, UINT32 uiImageWidth, UINT32 uiImageHeight, WATERLOGO_BUFFER *pWaterLogoBuf)
{
    PSTAMP_INFO     pStampInfo;
    ICON_DB const   *pDB;
    UINT32          uiIconID;
    UINT32          uiStrOffset;

    // get video encoding path and set stamp format firstly
	//#NT#2017/04/19#KCHong -begin
	//#NT#Fixed timestamp format error when http live view
	if (uiVidEncId < 2) {
		g_uiMovieStampFormat[uiVidEncId] = (MovRec_GetEncPath(uiVidEncId) == VIDENC_PATH_D2D) ? STAMP_FORMAT_YUV : STAMP_FORMAT_IDX;
	}  else {
		g_uiMovieStampFormat[uiVidEncId] = STAMP_FORMAT_YUV;
	}
	//#NT#2017/04/19#KCHong -end

    g_uiMovieStampSetup[uiVidEncId] = uiFlag;

    if ((uiFlag & STAMP_SWITCH_MASK) == STAMP_OFF)
    {
        return;
    }

    g_bWaterLogoEnable[uiVidEncId] = pWaterLogoBuf ? TRUE : FALSE;

    #if defined (WATERLOGO_FUNCTION) && (WATERLOGO_FUNCTION == ENABLE)
    {
        if (g_bWaterLogoEnable[uiVidEncId])
        {
            // setup water logo
            memcpy((void *)&g_MovieWaterInfo[uiVidEncId], (const void *)pWaterLogoBuf, sizeof(WATERLOGO_BUFFER));
            g_MovieWaterInfo[uiVidEncId].uiWaterLogoYAddr = g_uiWaterLogoYAddr[uiVidEncId];
            g_MovieWaterInfo[uiVidEncId].uiWaterLogoUVAddr = g_uiWaterLogoUVAddr[uiVidEncId];
        }
    }
    #endif

    pStampInfo = &g_MovieStampInfo[uiVidEncId];
    pStampInfo->pi8Str = &g_cMovieStampStr[uiVidEncId][0];

    // set date stamp font data base
    switch (uiImageWidth)
    {
    case 2560:  // 2560x1440
    case 2304:  // 2304x1296
    case 1920:  // 1920x1080
    case 1536:  // 1536x1536

#if MOVIE_AE_LOG || MOVIE_ADAS_LOG
        pStampInfo->pDataBase = &gDateStampFont12x20;
#else
        pStampInfo->pDataBase = &gDateStampFont26x44;
#endif
        break;

    case 2880:  // 2880x2160 (DAR 16:9)
    case 1728:  // 1728x1296 (DAR 16:9)
    case 1440:  // 1440x1080 (DAR 16:9)
        pStampInfo->pDataBase = &gDateStampFont20x44;
        g_MovieWaterInfo[uiVidEncId].uiWidth = (g_MovieWaterInfo[uiVidEncId].uiWidth * 3) / 4;
        break;

    case 1280:  // 1280x720
        pStampInfo->pDataBase = &gDateStampFont18x30;
        g_MovieWaterInfo[uiVidEncId].uiWidth = (g_MovieWaterInfo[uiVidEncId].uiWidth * 30) / 44;
        g_MovieWaterInfo[uiVidEncId].uiHeight = (g_MovieWaterInfo[uiVidEncId].uiHeight * 30) / 44;
        break;

    case 320:   // QVGA
        pStampInfo->pDataBase = &gDateStampFont10x16;
        g_MovieWaterInfo[uiVidEncId].uiWidth = (g_MovieWaterInfo[uiVidEncId].uiWidth * 16) / 44;
        g_MovieWaterInfo[uiVidEncId].uiHeight = (g_MovieWaterInfo[uiVidEncId].uiHeight * 16) / 44;
        break;

    default:    // VGA & others
        pStampInfo->pDataBase = &gDateStampFont12x20;
        g_MovieWaterInfo[uiVidEncId].uiWidth = (g_MovieWaterInfo[uiVidEncId].uiWidth * 20) / 44;
        g_MovieWaterInfo[uiVidEncId].uiHeight = (g_MovieWaterInfo[uiVidEncId].uiHeight * 20) / 44;
        break;
    }

    #if defined (WATERLOGO_FUNCTION) && (WATERLOGO_FUNCTION == ENABLE)
    {
        if (g_bWaterLogoEnable[uiVidEncId])
        {
            // do water logo scaling
            UINT32      uiLineOffset[2] = {0};
            UINT32      uiPxlAddr[2] = {0};
            UINT32      uiScaleYAddr, uiScaleUVAddr;
            IMG_BUF     SrcImg, DstImg;
            IRECT       SrcRegion = {0};
            IRECT       DstRegion = {0};
            BOOL        bScale = TRUE;

            g_MovieWaterInfo[uiVidEncId].uiWidth = ALIGN_CEIL_4(g_MovieWaterInfo[uiVidEncId].uiWidth);
            g_MovieWaterInfo[uiVidEncId].uiHeight = ALIGN_CEIL_4(g_MovieWaterInfo[uiVidEncId].uiHeight);

            if (g_uiMovieStampFormat[uiVidEncId] == STAMP_FORMAT_YUV)
            {
                // D2D: scale logo data to stamp buffer
                uiScaleYAddr = g_MovieWaterInfo[uiVidEncId].uiWaterLogoYAddr;
                uiScaleUVAddr = g_MovieWaterInfo[uiVidEncId].uiWaterLogoUVAddr;
            }
            else
            {
                // IME direct: use temp buffer to do scaling, then transfer YUV422 UV-packed to YUV422 YUYV-packed
                // use date time stamp buffer for logo scaling since it's not in use at this moment
                uiScaleYAddr = g_uiMovieStampYAddr[uiVidEncId][0];
                uiScaleUVAddr = uiScaleYAddr + g_MovieWaterInfo[uiVidEncId].uiWaterLogoYLineOffset * g_MovieWaterInfo[uiVidEncId].uiHeight;
            }

            if (bScale)
            {
                // Init source buffer
                uiLineOffset[0] = pWaterLogoBuf->uiWaterLogoYLineOffset;
                uiLineOffset[1] = pWaterLogoBuf->uiWaterLogoYLineOffset;
                uiPxlAddr[0]    = pWaterLogoBuf->uiWaterLogoYAddr;
                uiPxlAddr[1]    = pWaterLogoBuf->uiWaterLogoUVAddr;

                GxImg_InitBufEx(&SrcImg,
                                pWaterLogoBuf->uiWidth,
                                pWaterLogoBuf->uiHeight,
                                GX_IMAGE_PIXEL_FMT_YUV422_PACKED,
                                uiLineOffset,
                                uiPxlAddr);

                // Init destination buffer
                uiLineOffset[0] = g_MovieWaterInfo[uiVidEncId].uiWaterLogoYLineOffset;
                uiLineOffset[1] = g_MovieWaterInfo[uiVidEncId].uiWaterLogoYLineOffset;
                uiPxlAddr[0]    = uiScaleYAddr;
                uiPxlAddr[1]    = uiScaleUVAddr;

                GxImg_InitBufEx(&DstImg,
                                g_MovieWaterInfo[uiVidEncId].uiWidth,
                                g_MovieWaterInfo[uiVidEncId].uiHeight,
                                GX_IMAGE_PIXEL_FMT_YUV422_PACKED,
                                uiLineOffset,
                                uiPxlAddr);

                // Set scale region
                SrcRegion.x = 0;
                SrcRegion.y = 0;
                SrcRegion.w = pWaterLogoBuf->uiWidth;
                SrcRegion.h = pWaterLogoBuf->uiHeight;
                //DBG_DUMP("^YLogo Src: x %d, y %d, w %d, h %d\r\n", SrcRegion.x, SrcRegion.y, SrcRegion.w, SrcRegion.h);

                DstRegion.x = 0;
                DstRegion.y = 0;
                DstRegion.w = g_MovieWaterInfo[uiVidEncId].uiWidth;
                DstRegion.h = g_MovieWaterInfo[uiVidEncId].uiHeight;
                //DBG_DUMP("^YLogo Dst: x %d, y %d, w %d, h %d\r\n", DstRegion.x, DstRegion.y, DstRegion.w, DstRegion.h);

                // Scale image by ISE
                GxImg_ScaleData(&SrcImg, &SrcRegion, &DstImg, &DstRegion);
            }

            if (g_uiMovieStampFormat[uiVidEncId] == STAMP_FORMAT_IDX)
            {
                // IME direct: YUV422 UV-packed to YUV422 YUYV-packed
                GRPH_REQUEST request = {0};
                GRPH_IMG imgA = {0};
                GRPH_IMG imgB = {0};
                GRPH_IMG imgC = {0};

                // setup image A/B/C
                imgA.imgID = GRPH_IMG_ID_A;
                imgA.uiAddress = uiScaleYAddr;
                imgA.uiLineoffset = g_MovieWaterInfo[uiVidEncId].uiWaterLogoYLineOffset;
                imgA.uiWidth = g_MovieWaterInfo[uiVidEncId].uiWidth;
                imgA.uiHeight = g_MovieWaterInfo[uiVidEncId].uiHeight;
                imgA.pNext = &imgB;
                imgB.imgID = GRPH_IMG_ID_B;
                imgB.uiAddress = uiScaleUVAddr;
                imgB.uiLineoffset = g_MovieWaterInfo[uiVidEncId].uiWaterLogoYLineOffset;
                imgB.pNext = &imgC;
                imgC.imgID = GRPH_IMG_ID_C;
                imgC.uiAddress = g_MovieWaterInfo[uiVidEncId].uiWaterLogoYAddr;
                imgC.uiLineoffset = g_MovieWaterInfo[uiVidEncId].uiWaterLogoYLineOffset*2;
                imgC.pNext = NULL;

                request.command = GRPH_CMD_PACKING;
                request.format = GRPH_FORMAT_8BITS;
                request.pImageDescript = &imgA;

                graph_open(GRPH_ID_1);
                graph_request(GRPH_ID_1, &request);
                graph_close(GRPH_ID_1);
            }
        }
    }
    #endif

    // set stamp string (for calculating stamp position)
    switch (uiFlag & STAMP_DATE_TIME_MASK)
    {
    case STAMP_DATE_TIME_AMPM:
        //sprintf(pStampInfo->pi8Str, "0000/00/00 00:00:00 AM");
        snprintf(pStampInfo->pi8Str, MOVIE_STAMP_MAX_LEN, "0000/00/00 00:00:00 AM");
        break;

    case STAMP_DATE: // date only is not suitable for movie stamp (it's suitable for still image stamp)
    case STAMP_TIME:
        //sprintf(pStampInfo->pi8Str, "00:00:00");
        snprintf(pStampInfo->pi8Str, MOVIE_STAMP_MAX_LEN, "00:00:00");
        break;

    case STAMP_DATE_TIME:
    default:
        //sprintf(pStampInfo->pi8Str, "0000/00/00 00:00:00");
        snprintf(pStampInfo->pi8Str, MOVIE_STAMP_MAX_LEN, "0000/00/00 00:00:00");
        break;
    }

    // set font width and height (use the width of 1st font, so the total width may have some error)
    pDB = pStampInfo->pDataBase;
    uiStrOffset = pDB->uiDrawStrOffset;
    uiIconID = pStampInfo->pi8Str[0] - uiStrOffset; // 1st font
    pStampInfo->ui32FontWidth  = pDB->pIconHeader[uiIconID].uiWidth;
    pStampInfo->ui32FontHeight = pDB->pIconHeader[uiIconID].uiHeight;
    pStampInfo->ui32DstHeight  = pStampInfo->ui32FontHeight; // no scaling

    // Set date stamp position
    if ((uiFlag & STAMP_OPERATION_MASK) == STAMP_AUTO)
    {
        UINT32  uiStampWidth = (pStampInfo->ui32DstHeight * pStampInfo->ui32FontWidth) / pStampInfo->ui32FontHeight;

        switch (uiFlag & STAMP_POSITION_MASK)
        {
        case STAMP_TOP_LEFT:
            if ((uiFlag & STAMP_POS_END_MASK) == STAMP_POS_END)
            {
                g_MovieStampPos[uiVidEncId].uiX = 0;
                g_MovieStampPos[uiVidEncId].uiY = 0;
            }
            else
            {
                #if 0
                g_MovieStampPos[uiVidEncId].uiX = uiStampWidth * 2; // 2 fonts width gap
                g_MovieStampPos[uiVidEncId].uiY = pStampInfo->ui32DstHeight; // 1 font height gap
                #else
                g_MovieStampPos[uiVidEncId].uiX = uiStampWidth; // 1 font width gap
                g_MovieStampPos[uiVidEncId].uiY = pStampInfo->ui32DstHeight / 2; // 1/2 font height gap
                #endif
            }
            #if defined (MULTILINGUAL_FUNCTION) && (MULTILINGUAL_FUNCTION == ENABLE)
            {
                g_LingualStampPos[uiVidEncId].uiX = uiStampWidth * (strlen(pStampInfo->pi8Str)+16);
                g_LingualStampPos[uiVidEncId].uiY = pStampInfo->ui32DstHeight / 2;
            }
            #endif
            #if defined (WATERLOGO_FUNCTION) && (WATERLOGO_FUNCTION == ENABLE)
            if (g_bWaterLogoEnable[uiVidEncId])
            {
                if (g_MovieWaterInfo[uiVidEncId].uiXPos == WATERLOGO_AUTO_POS && g_MovieWaterInfo[uiVidEncId].uiYPos == WATERLOGO_AUTO_POS)
                {
                    g_MovieWaterInfo[uiVidEncId].uiXPos = g_MovieStampPos[uiVidEncId].uiX;
                    g_MovieWaterInfo[uiVidEncId].uiYPos = g_MovieStampPos[uiVidEncId].uiY;
                    g_MovieStampPos[uiVidEncId].uiX += (g_MovieWaterInfo[uiVidEncId].uiWidth + STAMP_LOGO_GAP);
                }
            }
            #endif
            break;

        case STAMP_TOP_RIGHT:
            if ((uiFlag & STAMP_POS_END_MASK) == STAMP_POS_END)
            {
                g_MovieStampPos[uiVidEncId].uiX = uiImageWidth - uiStampWidth * strlen(pStampInfo->pi8Str) - STAMP_WIDTH_TOLERANCE;
                g_MovieStampPos[uiVidEncId].uiY = 0;
            }
            else
            {
                #if 0
                g_MovieStampPos[uiVidEncId].uiX = uiImageWidth - uiStampWidth * (strlen(pStampInfo->pi8Str) + 2); // 2 fonts width gap
                g_MovieStampPos[uiVidEncId].uiY = pStampInfo->ui32DstHeight; // 1 font height gap
                #else
                g_MovieStampPos[uiVidEncId].uiX = uiImageWidth - uiStampWidth * (strlen(pStampInfo->pi8Str) + 1); // 1 font width gap
                g_MovieStampPos[uiVidEncId].uiY = pStampInfo->ui32DstHeight / 2; // 1/2 font height gap
                #endif
            }
            #if defined (MULTILINGUAL_FUNCTION) && (MULTILINGUAL_FUNCTION == ENABLE)
            {
                g_LingualStampPos[uiVidEncId].uiX = uiStampWidth; // 1 font width gap
                g_LingualStampPos[uiVidEncId].uiY = pStampInfo->ui32DstHeight / 2;
            }
            #endif
            #if defined (WATERLOGO_FUNCTION) && (WATERLOGO_FUNCTION == ENABLE)
            if (g_bWaterLogoEnable[uiVidEncId])
            {
                if (g_MovieWaterInfo[uiVidEncId].uiXPos == WATERLOGO_AUTO_POS && g_MovieWaterInfo[uiVidEncId].uiYPos == WATERLOGO_AUTO_POS)
                {
                    g_MovieWaterInfo[uiVidEncId].uiXPos = g_MovieStampPos[uiVidEncId].uiX - g_MovieWaterInfo[uiVidEncId].uiWidth - STAMP_LOGO_GAP;
                    g_MovieWaterInfo[uiVidEncId].uiYPos = g_MovieStampPos[uiVidEncId].uiY;
                }
            }
            #endif
            break;

        case STAMP_BOTTOM_LEFT:
            if ((uiFlag & STAMP_POS_END_MASK) == STAMP_POS_END)
            {
                g_MovieStampPos[uiVidEncId].uiX = 0;
                g_MovieStampPos[uiVidEncId].uiY = uiImageHeight - pStampInfo->ui32DstHeight;
            }
            else
            {
                #if 0
                g_MovieStampPos[uiVidEncId].uiX = uiStampWidth * 2; // 2 fonts width gap
                g_MovieStampPos[uiVidEncId].uiY = uiImageHeight - pStampInfo->ui32DstHeight * 2; // 1 font height gap
                #else
                g_MovieStampPos[uiVidEncId].uiX = uiStampWidth; // 1 font width gap
                g_MovieStampPos[uiVidEncId].uiY = uiImageHeight - (pStampInfo->ui32DstHeight * 3) / 2; // 1/2 font height gap
                #endif
            }
            #if defined (MULTILINGUAL_FUNCTION) && (MULTILINGUAL_FUNCTION == ENABLE)
            {
                g_LingualStampPos[uiVidEncId].uiX = uiStampWidth * (strlen(pStampInfo->pi8Str)+16); // 1 font width gap
                g_LingualStampPos[uiVidEncId].uiY = uiImageHeight - (pStampInfo->ui32DstHeight * 3) / 2;
            }
            #endif
            #if defined (WATERLOGO_FUNCTION) && (WATERLOGO_FUNCTION == ENABLE)
            if (g_bWaterLogoEnable[uiVidEncId])
            {
                if (g_MovieWaterInfo[uiVidEncId].uiXPos == WATERLOGO_AUTO_POS && g_MovieWaterInfo[uiVidEncId].uiYPos == WATERLOGO_AUTO_POS)
                {
                    g_MovieWaterInfo[uiVidEncId].uiXPos = g_MovieStampPos[uiVidEncId].uiX;
                    g_MovieWaterInfo[uiVidEncId].uiYPos = g_MovieStampPos[uiVidEncId].uiY + pStampInfo->ui32DstHeight - g_MovieWaterInfo[uiVidEncId].uiHeight;
                    g_MovieStampPos[uiVidEncId].uiX += (g_MovieWaterInfo[uiVidEncId].uiWidth + STAMP_LOGO_GAP);
                }
            }
            #endif
            break;

        case STAMP_BOTTOM_RIGHT:
        default:
            if ((uiFlag & STAMP_POS_END_MASK) == STAMP_POS_END)
            {
                g_MovieStampPos[uiVidEncId].uiX = uiImageWidth - uiStampWidth * strlen(pStampInfo->pi8Str) - STAMP_WIDTH_TOLERANCE;
                g_MovieStampPos[uiVidEncId].uiY = uiImageHeight - pStampInfo->ui32DstHeight;
            }
            else
            {
                #if 0
                g_MovieStampPos[uiVidEncId].uiX = uiImageWidth - uiStampWidth * (strlen(pStampInfo->pi8Str) + 2); // 2 fonts width gap
                g_MovieStampPos[uiVidEncId].uiY = uiImageHeight - pStampInfo->ui32DstHeight * 2; // 1 font height gap
                #else
                g_MovieStampPos[uiVidEncId].uiX = uiImageWidth - uiStampWidth * (strlen(pStampInfo->pi8Str) + 1); // 1 font width gap
                g_MovieStampPos[uiVidEncId].uiY = uiImageHeight - (pStampInfo->ui32DstHeight * 3) / 2; // 1/2 font height gap
                #endif
            }
            #if defined (MULTILINGUAL_FUNCTION) && (MULTILINGUAL_FUNCTION == ENABLE)
            {
                g_LingualStampPos[uiVidEncId].uiX = uiStampWidth; // 1 font width gap
                g_LingualStampPos[uiVidEncId].uiY = uiImageHeight - (pStampInfo->ui32DstHeight * 3) / 2;
            }
            #endif
            #if defined (WATERLOGO_FUNCTION) && (WATERLOGO_FUNCTION == ENABLE)
            if (g_bWaterLogoEnable[uiVidEncId])
            {
                if (g_MovieWaterInfo[uiVidEncId].uiXPos == WATERLOGO_AUTO_POS && g_MovieWaterInfo[uiVidEncId].uiYPos == WATERLOGO_AUTO_POS)
                {
                    g_MovieWaterInfo[uiVidEncId].uiXPos = g_MovieStampPos[uiVidEncId].uiX - g_MovieWaterInfo[uiVidEncId].uiWidth - STAMP_LOGO_GAP;
                    g_MovieWaterInfo[uiVidEncId].uiYPos = g_MovieStampPos[uiVidEncId].uiY + pStampInfo->ui32DstHeight - g_MovieWaterInfo[uiVidEncId].uiHeight;
                }
            }
            #endif
            break;
        }
    }

    #if (MOVIE_DDD_FUNC && MOVIE_DDD_DRAW_VIDEO)
    //this is for DDD debug
    g_MovieStampPos[uiVidEncId].uiX -= (pStampInfo->ui32DstHeight * pStampInfo->ui32FontWidth) / pStampInfo->ui32FontHeight;;
    #endif

    g_MovieStampPos[uiVidEncId].uiX = ALIGN_FLOOR_4(g_MovieStampPos[uiVidEncId].uiX);
    g_MovieStampPos[uiVidEncId].uiY = ALIGN_FLOOR_4(g_MovieStampPos[uiVidEncId].uiY);

    #if defined (MULTILINGUAL_FUNCTION) && (MULTILINGUAL_FUNCTION == ENABLE)
    g_LingualStampPos[uiVidEncId].uiX = ALIGN_FLOOR_4(g_LingualStampPos[uiVidEncId].uiX);
    g_LingualStampPos[uiVidEncId].uiY = ALIGN_FLOOR_4(g_LingualStampPos[uiVidEncId].uiY);
    #endif

    #if defined (WATERLOGO_FUNCTION) && (WATERLOGO_FUNCTION == ENABLE)
    if (g_bWaterLogoEnable[uiVidEncId])
    {
        g_MovieWaterInfo[uiVidEncId].uiXPos = ALIGN_FLOOR_4(g_MovieWaterInfo[uiVidEncId].uiXPos);
        g_MovieWaterInfo[uiVidEncId].uiYPos = ALIGN_FLOOR_4(g_MovieWaterInfo[uiVidEncId].uiYPos);
    }
    #endif

    // set default stamp color if necessary
    if (g_bStampColorSetup[uiVidEncId] == FALSE)
    {
        // Stamp background color
        pStampInfo->Color[COLOR_ID_BG].ucY = COLOR_BG_Y;
        pStampInfo->Color[COLOR_ID_BG].ucU = COLOR_BG_U;
        pStampInfo->Color[COLOR_ID_BG].ucV = COLOR_BG_V;

        // Stamp frame color
        pStampInfo->Color[COLOR_ID_FR].ucY = COLOR_FR_Y;
        pStampInfo->Color[COLOR_ID_FR].ucU = COLOR_FR_U;
        pStampInfo->Color[COLOR_ID_FR].ucV = COLOR_FR_V;

        // Stamp foreground color (text body)
        pStampInfo->Color[COLOR_ID_FG].ucY = COLOR_FG_Y;
        pStampInfo->Color[COLOR_ID_FG].ucU = COLOR_FG_U;
        pStampInfo->Color[COLOR_ID_FG].ucV = COLOR_FG_V;
    }

#if !(IPCAM_FUNC && !MOVIE_AE_LOG)
    // init IME stamp data
    if (g_uiMovieStampFormat[uiVidEncId] == STAMP_FORMAT_IDX)
    {
        MovieStamp_InitImeStamp(uiVidEncId);
    }
#endif

    // set image width
    //g_uiMovieImageWidth[uiVidEncId] = uiImageWidth;

    // set active image buffer
    g_uiMovieStampActBuf = MOVIE_STAMP_BUF_0;

#if defined (MULTILINGUAL_FUNCTION) && (MULTILINGUAL_FUNCTION == ENABLE)
    g_uiLingualStampActBuf = LINGUAL_STAMP_BUF_OFF;
#endif

    // Reset reference time
    g_CurDateTime.tm_sec = 61;
}

void MovieStamp_SetPos(UINT32 uiVidEncId, UINT32 x, UINT32 y)
{
    g_MovieStampPos[uiVidEncId].uiX = x;
    g_MovieStampPos[uiVidEncId].uiY = y;
}

void MovieStamp_SetColor(UINT32 uiVidEncId, PSTAMP_COLOR pStampColorBg, PSTAMP_COLOR pStampColorFr, PSTAMP_COLOR pStampColorFg)
{
    g_bStampColorSetup[uiVidEncId] = TRUE;

    // Stamp background color
    g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_BG].ucY = pStampColorBg->ucY;
    g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_BG].ucU = pStampColorBg->ucU;
    g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_BG].ucV = pStampColorBg->ucV;

    // Stamp frame color
    g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_FR].ucY = pStampColorFr->ucY;
    g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_FR].ucU = pStampColorFr->ucU;
    g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_FR].ucV = pStampColorFr->ucV;

    // Stamp foreground color (text body)
    g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_FG].ucY = pStampColorFg->ucY;
    g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_FG].ucU = pStampColorFg->ucU;
    g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_FG].ucV = pStampColorFg->ucV;
}

#if 0
UINT32 MovieStamp_GetMaxFontHeight(void)
{
    return g_uiMovieStampHeightMax;
}
#endif

#if defined (MULTILINGUAL_FUNCTION) && (MULTILINGUAL_FUNCTION == ENABLE)
static UINT32 MovieStamp_GetLingualDataWidth(LINGUAL_INFO *pingualInfo,UINT32 uiStrLen)
{
    UINT32  i;
    UINT32  uiDataWidth;

    uiDataWidth = 0;
    for (i = 0; i < uiStrLen; i++)
    {
        uiDataWidth += pingualInfo[i].glyph.GLYPHINFO.wBoxX;
        uiDataWidth += ((pingualInfo[i].glyph.GLYPHINFO.wBoxX) % 2);        // if the font width is odd, append one pixel fot make it even.
    }
    return ALIGN_CEIL_4(uiDataWidth);
}

static BOOL MovieStamp_DrawMultiLingualIcon420UV(PSTAMP_INFO pStampInfo,LINGUAL_INFO LingualInfo,UINT32 uiFBY, UINT32 uiFBCb, UINT32 uiFBCr, UINT32 uiLineOffset)
{
    UINT8   *uiIconData;
    UINT8   *pChar, *pCharU, *pCharV;
    UINT32  uiIconWidth;
    UINT32  uiIconHeight;
    UINT32  uiPixelCount;
    UINT32  i, j;
    UINT32  uiIndex;

    uiIconWidth = LingualInfo.glyph.GLYPHINFO.wBoxX;
    uiIconHeight = LingualInfo.glyph.GLYPHINFO.wBoxY;
    uiIconData = &LingualInfo.GlyphData[0];

    uiPixelCount = 0;

    for(i = 0; i < uiIconHeight; i++)
    {
        pChar = (UINT8 *)uiFBY;
        pCharU = (UINT8 *)uiFBCb;
        pCharV = (UINT8 *)uiFBCb+1;

        for(j = 0; j < uiIconWidth; j ++)
        {
            uiIndex = (*uiIconData >> (6 - (uiPixelCount % 4 * 2))) & 0x03;

            if (uiIndex)
            {
                if (uiIndex == 1) // frame color
                {
                    *pChar = pStampInfo->Color[COLOR_ID_FR].ucY;
                }
                else // foreground color
                {
                    *pChar = pStampInfo->Color[COLOR_ID_FG].ucY;
                }

                if (((j&1)==0)&&((i&1)==0))
                {
                    if (uiIndex == 1) // frame color
                    {
                        *pCharU = pStampInfo->Color[COLOR_ID_FR].ucU;
                        *pCharV = pStampInfo->Color[COLOR_ID_FR].ucV;
                    }
                    else // foreground color
                    {
                        *pCharU = pStampInfo->Color[COLOR_ID_FG].ucU;
                        *pCharV = pStampInfo->Color[COLOR_ID_FG].ucV;
                    }
                }
            }
            pChar++;
            if ((j&1)==0)
            {
                pCharU+=2;
                pCharV+=2;
            }
            uiPixelCount ++;
            if (uiPixelCount % 4 == 0)
                uiIconData ++;

        }
        uiFBY += uiLineOffset;
        if  ((i&1) == 0)
        {
            uiFBCb += uiLineOffset;
            uiFBCr += uiLineOffset;
        }
    }
    return TRUE;

}

static BOOL MovieStamp_DrawMultiLingualIcon422UV(PSTAMP_INFO pStampInfo,LINGUAL_INFO LingualInfo,UINT32 uiFBY, UINT32 uiFBCb, UINT32 uiFBCr, UINT32 uiLineOffset)
{
    UINT8   *uiIconData;
    UINT8   *pChar, *pCharU, *pCharV;
    UINT32  uiIconWidth;
    UINT32  uiIconHeight;
    UINT32  uiPixelCount;
    UINT32  i, j;
    UINT32  uiIndex;

    uiIconWidth = LingualInfo.glyph.GLYPHINFO.wBoxX;
    uiIconHeight = LingualInfo.glyph.GLYPHINFO.wBoxY;
    uiIconData = &LingualInfo.GlyphData[0];

    uiPixelCount = 0;

    for(i = 0; i < uiIconHeight; i++)
    {
        pChar = (UINT8 *)uiFBY;
        pCharU = (UINT8 *)uiFBCb;
        pCharV = (UINT8 *)uiFBCb+1;

        for(j = 0; j < uiIconWidth; j ++)
        {
            uiIndex = (*uiIconData >> (6 - (uiPixelCount % 4 * 2))) & 0x03;

            if (uiIndex)
            {
                if (uiIndex == 1) // frame color
                {
                    *pChar = pStampInfo->Color[COLOR_ID_FR].ucY;
                }
                else // foreground color
                {
                    *pChar = pStampInfo->Color[COLOR_ID_FG].ucY;
                }

                if ((j&1)==0)
                {
                    if (uiIndex == 1) // frame color
                    {
                        *pCharU = pStampInfo->Color[COLOR_ID_FR].ucU;
                        *pCharV = pStampInfo->Color[COLOR_ID_FR].ucV;
                    }
                    else // foreground color
                    {
                        *pCharU = pStampInfo->Color[COLOR_ID_FG].ucU;
                        *pCharV = pStampInfo->Color[COLOR_ID_FG].ucV;
                    }
                }
            }
            pChar++;
            if ((j&1)==0)
            {
                pCharU+=2;
                pCharV+=2;
            }
            uiPixelCount ++;
            if (uiPixelCount % 4 == 0)
                uiIconData ++;

        }
        uiFBY += uiLineOffset;
//        if  ((i&1) == 0)
        {
            uiFBCb += uiLineOffset;
            uiFBCr += uiLineOffset;
        }
    }
    return TRUE;

}

static UINT32 MovieStamp_DrawMultiLingualString(PSTAMP_INFO pStampInfo, PSTAMP_BUFFER pStampBuffer,UINT16 *pUniCodeArr,UINT32 datalen)
{
 UINT32  i, j;
 UINT32  uiFBAddrY, uiFBAddrCb, uiFBAddrCr;
 UINT32  uiDataWidth;

    uiFBAddrY  = pStampBuffer->uiYAddr;
    uiFBAddrCb = pStampBuffer->uiUAddr;
    uiFBAddrCr = pStampBuffer->uiVAddr;

    uiDataWidth = 0;
    for (i=0 ; i<datalen ; i++)
    {
        j = LingualInfo[i].glyph.GLYPHINFO.wBoxX;
        uiDataWidth += j;

        if (j%2!=0)
           j++;

        if (pStampBuffer->uiFormat == GX_IMAGE_PIXEL_FMT_YUV420_PACKED)
        {
            MovieStamp_DrawMultiLingualIcon420UV(pStampInfo,LingualInfo[i], uiFBAddrY, uiFBAddrCb, uiFBAddrCr, pStampBuffer->uiYLineOffset);
        }
        else
        {
            MovieStamp_DrawMultiLingualIcon422UV(pStampInfo,LingualInfo[i], uiFBAddrY, uiFBAddrCb, uiFBAddrCr, pStampBuffer->uiYLineOffset);
        }

        uiFBAddrY  += (j);
        uiFBAddrCb += (j);
        uiFBAddrCr += (j);
    }
    return uiDataWidth;
}
#endif

#if (!IPCAM_FUNC || MOVIE_AE_LOG)
static BOOL MovieStamp_DrawIcon420UV(PSTAMP_INFO pStampInfo, UINT16 uiIconID, UINT32 uiFBY, UINT32 uiFBCb, UINT32 uiFBCr, UINT32 uiLineOffset)
{
    ICON_HEADER pIconHeader;
    ICON_DB const *pDB;
    UINT8   const *uiIconData;
    UINT8   *pChar, *pCharU, *pCharV;
    UINT32  uiIconWidth;
    UINT32  uiIconHeight;
    UINT32  uiPixelCount;
    UINT32  i, j;
    UINT32  uiIndex;

    pDB = pStampInfo->pDataBase;
    pIconHeader = pDB->pIconHeader[uiIconID];
    uiIconWidth = pIconHeader.uiWidth;
    uiIconHeight = pIconHeader.uiHeight;
    uiIconData = (pDB->pIconData) + (pIconHeader.uiOffset);

    uiPixelCount = 0;

    for(i = 0; i < uiIconHeight; i++)
    {
        pChar = (UINT8 *)uiFBY;
        pCharU = (UINT8 *)uiFBCb;
        pCharV = (UINT8 *)uiFBCb+1;

        for(j = 0; j < uiIconWidth; j ++)
        {
            uiIndex = (*uiIconData >> (6 - (uiPixelCount % 4 * 2))) & 0x03;

            if (uiIndex)
            {
                if (uiIndex == 1) // frame color
                {
                    *pChar = pStampInfo->Color[COLOR_ID_FR].ucY;
                }
                else // foreground color
                {
                    *pChar = pStampInfo->Color[COLOR_ID_FG].ucY;
                }

                if (((j&1)==0)&&((i&1)==0))
                {
                    if (uiIndex == 1) // frame color
                    {
                        *pCharU = pStampInfo->Color[COLOR_ID_FR].ucU;
                        *pCharV = pStampInfo->Color[COLOR_ID_FR].ucV;
                    }
                    else // foreground color
                    {
                        *pCharU = pStampInfo->Color[COLOR_ID_FG].ucU;
                        *pCharV = pStampInfo->Color[COLOR_ID_FG].ucV;
                    }
                }
            }
            pChar++;
            if ((j&1)==0)
            {
                pCharU+=2;
                pCharV+=2;
            }
            uiPixelCount ++;
            if (uiPixelCount % 4 == 0)
                uiIconData ++;

        }
        uiFBY += uiLineOffset;
        if  ((i&1) == 0)
        {
            uiFBCb += uiLineOffset;
            uiFBCr += uiLineOffset;
        }
    }
    return TRUE;
}

static BOOL MovieStamp_DrawIcon422UV(PSTAMP_INFO pStampInfo, UINT16 uiIconID, UINT32 uiFBY, UINT32 uiFBCb, UINT32 uiFBCr, UINT32 uiLineOffset)
{
    ICON_HEADER pIconHeader;
    ICON_DB const *pDB;
    UINT8   const *uiIconData;
    UINT8   *pChar, *pCharU, *pCharV;
    UINT32  uiIconWidth;
    UINT32  uiIconHeight;
    UINT32  uiPixelCount;
    UINT32  i, j;
    UINT32  uiIndex;

    pDB = pStampInfo->pDataBase;
    pIconHeader = pDB->pIconHeader[uiIconID];
    uiIconWidth = pIconHeader.uiWidth;
    uiIconHeight = pIconHeader.uiHeight;
    uiIconData = (pDB->pIconData) + (pIconHeader.uiOffset);

    uiPixelCount = 0;

    for(i = 0; i < uiIconHeight; i++)
    {
        pChar = (UINT8 *)uiFBY;
        pCharU = (UINT8 *)uiFBCb;
        pCharV = (UINT8 *)uiFBCb+1;

        for(j = 0; j < uiIconWidth; j ++)
        {
            uiIndex = (*uiIconData >> (6 - (uiPixelCount % 4 * 2))) & 0x03;

            if (uiIndex)
            {
                if (uiIndex == 1) // frame color
                {
                    *pChar = pStampInfo->Color[COLOR_ID_FR].ucY;
                }
                else // foreground color
                {
                    *pChar = pStampInfo->Color[COLOR_ID_FG].ucY;
                }

                //if (((j&1)==0)&&((i&1)==0))
                if ((j&1)==0)
                {
                    if (uiIndex == 1) // frame color
                    {
                        *pCharU = pStampInfo->Color[COLOR_ID_FR].ucU;
                        *pCharV = pStampInfo->Color[COLOR_ID_FR].ucV;
                    }
                    else // foreground color
                    {
                        *pCharU = pStampInfo->Color[COLOR_ID_FG].ucU;
                        *pCharV = pStampInfo->Color[COLOR_ID_FG].ucV;
                    }
                }
            }
            pChar++;
            if ((j&1)==0)
            {
                pCharU+=2;
                pCharV+=2;
            }
            uiPixelCount ++;
            if (uiPixelCount % 4 == 0)
                uiIconData ++;

        }
        uiFBY += uiLineOffset;
        //if  ((i&1) == 0)
        {
            uiFBCb += uiLineOffset;
            uiFBCr += uiLineOffset;
        }
    }
    return TRUE;
}
#endif

#if (COLOR_IDX_MODE == COLOR_IDX_4BIT)
static BOOL MovieStamp_DrawIcon4bitIdx(PSTAMP_INFO pStampInfo, UINT16 uiIconID, UINT32 uiFBY, UINT32 uiLineOffset, UINT32 uiPixelShift)
{
    ICON_HEADER pIconHeader;
    ICON_DB const *pDB;
    UINT8   const *uiIconData;
    UINT8   *pChar;
    UINT8   u4ColorIdx;
    UINT32  uiIconWidth;
    UINT32  uiIconHeight;
    UINT32  uiPixelCount;
    UINT32  i, j;
    UINT32  uiIndex;

    pDB = pStampInfo->pDataBase;
    pIconHeader = pDB->pIconHeader[uiIconID];
    uiIconWidth = pIconHeader.uiWidth;
    uiIconHeight = pIconHeader.uiHeight;
    uiIconData = (pDB->pIconData) + (pIconHeader.uiOffset);

    uiPixelCount = 0;
    DBG_MSG("uiIconWidth %d, uiIconHeight %d, uiFBY 0x%08x, uiLineOffset %d\r\n", uiIconWidth, uiIconHeight, uiFBY, uiLineOffset);

    for(i = 0; i < uiIconHeight; i++)
    {
        pChar = (UINT8 *)uiFBY;

        for(j = 0; j < uiIconWidth; j ++)
        {
            uiIndex = (*uiIconData >> (6 - (uiPixelCount % 4 * 2))) & 0x03;

            switch (uiIndex)
            {
            case COLOR_ID_BG: // background color
                u4ColorIdx = 0;
                break;

            case COLOR_ID_FR: // frame color
                u4ColorIdx = 1;
                break;

            default: // foreground color
                u4ColorIdx = 2;
                break;
            }

            switch ((j + uiPixelShift) % 2)
            {
            case 1:
                *pChar = (*pChar & 0x0f) | (u4ColorIdx << 4);
                pChar++;
                break;

            default:
                *pChar = (*pChar & 0xf0) | u4ColorIdx;
                break;
            }

            uiPixelCount ++;
            if (uiPixelCount % 4 == 0)
                uiIconData ++;

        }
        uiFBY += uiLineOffset;
    }
    return TRUE;
}

#else // COLOR_IDX_2BIT

#if (!IPCAM_FUNC || MOVIE_AE_LOG)
static BOOL MovieStamp_DrawIcon2bitIdx(PSTAMP_INFO pStampInfo, UINT16 uiIconID, UINT32 uiFBY, UINT32 uiLineOffset, UINT32 uiPixelShift)
{
    ICON_HEADER pIconHeader;
    ICON_DB const *pDB;
    UINT8   const *uiIconData;
    UINT8   *pChar;
    UINT8   u4ColorIdx;
    UINT32  uiIconWidth;
    UINT32  uiIconHeight;
    UINT32  uiPixelCount;
    UINT32  i, j;
    UINT32  uiIndex;

    pDB = pStampInfo->pDataBase;
    pIconHeader = pDB->pIconHeader[uiIconID];
    uiIconWidth = pIconHeader.uiWidth;
    uiIconHeight = pIconHeader.uiHeight;
    uiIconData = (pDB->pIconData) + (pIconHeader.uiOffset);

    uiPixelCount = 0;
    DBG_MSG("uiIconWidth %d, uiIconHeight %d, uiFBY 0x%08x, uiLineOffset %d\r\n", uiIconWidth, uiIconHeight, uiFBY, uiLineOffset);

    for(i = 0; i < uiIconHeight; i++)
    {
        pChar = (UINT8 *)uiFBY;

        for(j = 0; j < uiIconWidth; j ++)
        {
            uiIndex = (*uiIconData >> (6 - (uiPixelCount % 4 * 2))) & 0x03;

            switch (uiIndex)
            {
            case COLOR_ID_BG: // background color
                u4ColorIdx = 0;
                break;

            case COLOR_ID_FR: // frame color
                u4ColorIdx = 1;
                break;

            default: // foreground color
                u4ColorIdx = 2;
                break;
            }

            switch ((j + uiPixelShift) % 4)
            {
            case 1:
                *pChar = (*pChar & 0xf3) | (u4ColorIdx << 2);
                break;

            case 2:
                *pChar = (*pChar & 0xcf) | (u4ColorIdx << 4);
                break;

            case 3:
                *pChar = (*pChar & 0x3f) | (u4ColorIdx << 6);
                pChar++;
                break;

            default:
                *pChar = (*pChar & 0xfc) | u4ColorIdx;
                break;
            }

            uiPixelCount ++;
            if (uiPixelCount % 4 == 0)
                uiIconData ++;

        }
        uiFBY += uiLineOffset;
    }
    return TRUE;
}
#endif
#endif

#if (!IPCAM_FUNC || MOVIE_AE_LOG)
static UINT32 MovieStamp_GetStampDataWidth(PSTAMP_INFO pStampInfo)
{
    ICON_DB const *pDB;
    UINT32  i;
    UINT32  uiStrLen;
    UINT32  uiDataWidth;
    UINT32  uiIconID;
    UINT32  uiStrOffset;

    uiStrLen = strlen(pStampInfo->pi8Str);

    pDB = pStampInfo->pDataBase;
    uiStrOffset = pDB->uiDrawStrOffset;

    uiDataWidth = 0;
    for (i = 0; i < uiStrLen; i++)
    {
        //get icon database
        uiIconID = pStampInfo->pi8Str[i] - uiStrOffset;
        uiDataWidth += pDB->pIconHeader[uiIconID].uiWidth;
    }

    return uiDataWidth;
}

static UINT32 MovieStamp_DrawString(UINT32 uiVidEncId, PSTAMP_INFO pStampInfo, PSTAMP_BUFFER pStampBuffer)
{
    ICON_DB const *pDB;
    UINT32  i, j;
    UINT32  uiStrLen;
    UINT32  uiDataWidth;
    UINT32  uiIconID;
    UINT32  uiStrOffset;
    UINT32  uiFBAddrY, uiFBAddrCb, uiFBAddrCr;
    UINT32  uiStampAddr, uiPixelShift;

    uiStrLen = strlen(pStampInfo->pi8Str);

    pDB = pStampInfo->pDataBase;
    uiStrOffset = pDB->uiDrawStrOffset;

    uiFBAddrY  = pStampBuffer->uiYAddr;
    uiFBAddrCb = pStampBuffer->uiUAddr;
    uiFBAddrCr = pStampBuffer->uiVAddr;

    uiDataWidth = 0;
    for (i = 0; i < uiStrLen; i++)
    {
        //get icon database
        uiIconID = pStampInfo->pi8Str[i] - uiStrOffset;
        j = pDB->pIconHeader[uiIconID].uiWidth;

        //if (j % 2)
        //{
        //    j++;
        //}

        if (g_uiMovieStampFormat[uiVidEncId] == STAMP_FORMAT_IDX) // color index format (for direct path only)
        {
            #if (COLOR_IDX_MODE == COLOR_IDX_4BIT)
                uiStampAddr = uiFBAddrY + uiDataWidth / 2;
                uiPixelShift = uiDataWidth % 2;
                MovieStamp_DrawIcon4bitIdx(pStampInfo, uiIconID, uiStampAddr, pStampBuffer->uiYLineOffset, uiPixelShift);
            #else // COLOR_IDX_2BIT
                uiStampAddr = uiFBAddrY + uiDataWidth / 4;
                uiPixelShift = uiDataWidth % 4;
                MovieStamp_DrawIcon2bitIdx(pStampInfo, uiIconID, uiStampAddr, pStampBuffer->uiYLineOffset, uiPixelShift);
            #endif
        }
        else // YUV format
        {
            if (pStampBuffer->uiFormat == GX_IMAGE_PIXEL_FMT_YUV420_PACKED)
                MovieStamp_DrawIcon420UV(pStampInfo, uiIconID, uiFBAddrY, uiFBAddrCb, uiFBAddrCr, pStampBuffer->uiYLineOffset);
            else
                MovieStamp_DrawIcon422UV(pStampInfo, uiIconID, uiFBAddrY, uiFBAddrCb, uiFBAddrCr, pStampBuffer->uiYLineOffset);

            uiFBAddrY  += (j);
            uiFBAddrCb += (j);
            uiFBAddrCr += (j);
        }

        uiDataWidth += j;
    }

    return uiDataWidth;
}
#endif

#if (IPCAM_FUNC && !MOVIE_AE_LOG)
void MovieStamp_SetBuffer(UINT32 uiAddr, UINT32 uiSize)
{
    g_uiMovieStampYAddr[0] = uiAddr;
    g_uiMovieStampYAddr[1] = (uiAddr + (uiSize / 2));
    g_uiMovieStampSize     = (uiSize / 2);
}
#else
void MovieStamp_SetBuffer(UINT32 uiVidEncId, UINT32 uiAddr, UINT32 uiSize)
{
    //#NT#2015/11/04#KCHong -begin
    // The required buffer size now is based on the definition switch.
    // For current project setting (if waterlogo and multilingual function is enabled.
    // Total buffer size                = 1MB (for each VidEncId)
    // Waterlog buffer size             =  64K (256*128*2)
    // Multilingual buffer size         = 120K (1920*32*2) ==> Assume font height should less than 32, otherwise need to re-adjust the buffer size.
    // Multilingual scaling buffer size = 330K (1920*44*2*2) ==> Ping-Pong buffer. Assume scaled font height should less than 44, otherwise need to re-adjust the buffer size.
    // Movie stamp buffer               = 510K (1920*68*2*2) ==> Use rest memories. Ping-pong buffer.

    /*
    g_uiWaterLogoYAddr[uiVidEncId]     = uiAddr;
    g_uiWaterLogoUVAddr[uiVidEncId]    = uiAddr + WATERLOGO_BUF_SIZE / 2; // YUV422, UV-packed
    g_uiMovieStampYAddr[uiVidEncId][0] = uiAddr + WATERLOGO_BUF_SIZE;
    g_uiMovieStampYAddr[uiVidEncId][1] = uiAddr + (uiSize - WATERLOGO_BUF_SIZE) / 2;
    */
    g_uiWaterLogoYAddr[uiVidEncId]     = uiAddr;
    g_uiWaterLogoUVAddr[uiVidEncId]    = g_uiWaterLogoYAddr[uiVidEncId] + WATERLOGO_BUF_SIZE / 2; // YUV422, UV-packed

    g_uiLingualStampYAddr[uiVidEncId] = uiAddr + WATERLOGO_BUF_SIZE;
    g_uiLingualScaledStampYAddr[uiVidEncId][0] = g_uiLingualStampYAddr[uiVidEncId] + MULTILINGUAL_BUF_SIZE;
    g_uiLingualScaledStampYAddr[uiVidEncId][1] = g_uiLingualScaledStampYAddr[uiVidEncId][0] + MULTILINGUAL_SCALLED_BUF_SIZE;

    g_uiMovieStampYAddr[uiVidEncId][0] = g_uiLingualScaledStampYAddr[uiVidEncId][1] + MULTILINGUAL_SCALLED_BUF_SIZE;
    //#NT#2016/04/22#YongChang Qui -begin
    //#NT#IPCam osd try to avoid buffer overrun
    g_uiMovieStampYAddr[uiVidEncId][1] = g_uiMovieStampYAddr[uiVidEncId][0] + (uiAddr + uiSize - g_uiMovieStampYAddr[uiVidEncId][0]) / 2;
    g_uiMovieStampSize[uiVidEncId] = (g_uiMovieStampYAddr[uiVidEncId][1] - g_uiMovieStampYAddr[uiVidEncId][0]);
    //#NT#2016/04/22#YongChang Qui -end
    //#NT#2015/11/04#KCHong -end
}
#endif

#if (CHECK_ENC_INFO == ENABLE)
extern void MediaRec_GetEncInfo(UINT32 uiVidEncId, UINT32 puiParam[7]);
#endif

#if defined (MULTILINGUAL_FUNCTION) && (MULTILINGUAL_FUNCTION == ENABLE)
void MultiLingualString_UpdateData(void)
{
    GX_IMAGE_PIXEL_FMT GxImgFormat;
    IMG_BUF         SrcImg;
    UINT32          uiPreWidth, uiWidth;
    UINT32          uiBgData;
    UINT32          uiLineOffset[2] = {0};
    UINT32          uiPxlAddr[2] = {0};
    UINT32          i;
    UINT32          uiUnicodeLen = 0;
    STAMP_BUFFER    LingualStampBuffer = {0};
    STAMP_BUFFER    LingualScaleStampBuffer = {0};
    IMG_BUF         DstImg;
    IRECT           SrcRegion = {0};
    IRECT           DstRegion = {0};

    for (i = 0; i < VIDEO_IN_MAX; i++)
    {
        if ((g_uiMovieStampSetup[i] & STAMP_SWITCH_MASK) == STAMP_ON)
        {
            switch (g_uiMovieStampSetup[i] & STAMP_IMG_FORMAT_MASK)
            {
            case STAMP_IMG_420UV:
                GxImgFormat = GX_IMAGE_PIXEL_FMT_YUV420_PACKED;
                break;

            case STAMP_IMG_422UV:
                GxImgFormat = GX_IMAGE_PIXEL_FMT_YUV422_PACKED;
                break;

            default:
                DBG_ERR("Only support 420/422 UV pack!\r\n");
                return;
            }

            for (uiUnicodeLen=0;uiUnicodeLen<(sizeof(unicode)/sizeof(UINT16));uiUnicodeLen++)
            {
                UniFont_GetGlyphFromUnicode(unicode[uiUnicodeLen],&LingualInfo[uiUnicodeLen]);
                if (LingualInfo[uiUnicodeLen].glyph.GLYPHINFO.wBoxY > g_uiLingualStampHeight)
                    g_uiLingualStampHeight = LingualInfo[uiUnicodeLen].glyph.GLYPHINFO.wBoxY;
            }

            LingualStampBuffer.uiYLineOffset   = STAMP_WIDTH_MAX;
            LingualStampBuffer.uiUVLineOffset  = STAMP_WIDTH_MAX;
            LingualStampBuffer.uiYAddr         = g_uiLingualStampYAddr[i];
            LingualStampBuffer.uiUAddr         = LingualStampBuffer.uiYAddr + LingualStampBuffer.uiYLineOffset * g_uiLingualStampHeight;
            LingualStampBuffer.uiVAddr         = LingualStampBuffer.uiUAddr; // for 420 UV pack
            LingualStampBuffer.uiFormat        = GxImgFormat;

            uiPreWidth = g_uiLingualStampWidth[i]; // previous stamp data width
            g_uiLingualStampWidth[i] = MovieStamp_GetLingualDataWidth(&LingualInfo[0], uiUnicodeLen);
            g_uiLingualStampWidth[i] = ALIGN_CEIL_4(g_uiLingualStampWidth[i]); // limitation of index color stamp mode
            uiWidth = (uiPreWidth > g_uiLingualStampWidth[i]) ? uiPreWidth : g_uiLingualStampWidth[i];

            // Set background data, share same uiBgData setting with Movie Stamp
            if (g_uiMovieStampFormat[i] == STAMP_FORMAT_IDX) // color index format (for direct path only)
            {
                #if (COLOR_IDX_MODE == COLOR_IDX_4BIT)
                uiWidth >>= 1;
                #else
                uiWidth >>= 2;
                #endif
                uiBgData = COLOR_ID_BG;
            }
            else // STAMP_FORMAT_YUV
            {
                if ((g_uiMovieStampSetup[i] & STAMP_BG_FORMAT_MASK) == STAMP_BG_TRANSPARENT)
                {
                    //uiBgData = COLORKEY_BG_YUV; // for LE op
                    uiBgData = COLORKEY_YUV; // for EQ op
                }
                else
                {
                    uiBgData =  ((UINT32)g_MovieStampInfo[i].Color[COLOR_ID_BG].ucV << 16) |
                                ((UINT32)g_MovieStampInfo[i].Color[COLOR_ID_BG].ucU << 8)  |
                                g_MovieStampInfo[i].Color[COLOR_ID_BG].ucY;
                }
            }

            // Clear source (stamp) buffer (in fact, index color mode doesn't need to copy Y, U, V data,
            // it only needs to copy "color index" data; but however, use GxImg function temporarily.
            uiLineOffset[0] = LingualStampBuffer.uiYLineOffset;
            uiLineOffset[1] = LingualStampBuffer.uiUVLineOffset;
            uiPxlAddr[0]    = LingualStampBuffer.uiYAddr;
            uiPxlAddr[1]    = LingualStampBuffer.uiUAddr;

            GxImg_InitBufEx(&SrcImg,
                            uiWidth,
                            g_uiLingualStampHeight,
                            GxImgFormat,
                            uiLineOffset,
                            uiPxlAddr);
            GxImg_FillData(&SrcImg, REGION_MATCH_IMG, uiBgData);

            MovieStamp_DrawMultiLingualString(&g_MovieStampInfo[i], &LingualStampBuffer,&unicode[0],uiUnicodeLen);

            g_uiLingualScaledStampHeight = (g_uiLingualStampHeight * g_uiLingualStampScalingFactor[i]) / g_uiLingualStampHeight;
            g_uiLingualScaledStampWidth[i][(g_uiLingualStampActBuf + 1) % LINGUAL_STAMP_BUF_TATAL] = (g_uiLingualStampWidth[i] * g_uiLingualStampScalingFactor[i]) / g_uiLingualStampHeight;

            // Init source (stamp) buffer
            uiLineOffset[0] = LingualStampBuffer.uiYLineOffset;
            uiLineOffset[1] = LingualStampBuffer.uiUVLineOffset;
            uiPxlAddr[0]    = LingualStampBuffer.uiYAddr;
            uiPxlAddr[1]    = LingualStampBuffer.uiUAddr;

            GxImg_InitBufEx(&SrcImg,
                            g_uiLingualStampWidth[i],
                            g_uiLingualStampHeight,
                            GxImgFormat,
                            uiLineOffset,
                            uiPxlAddr);

            LingualScaleStampBuffer.uiYLineOffset   = STAMP_WIDTH_MAX;
            LingualScaleStampBuffer.uiUVLineOffset  = STAMP_WIDTH_MAX;
            LingualScaleStampBuffer.uiYAddr         = g_uiLingualScaledStampYAddr[i][(g_uiLingualStampActBuf + 1) % LINGUAL_STAMP_BUF_TATAL];
            LingualScaleStampBuffer.uiUAddr         = LingualScaleStampBuffer.uiYAddr + LingualScaleStampBuffer.uiYLineOffset * g_uiLingualScaledStampHeight;
            LingualScaleStampBuffer.uiVAddr         = LingualScaleStampBuffer.uiUAddr; // for 420 UV pack
            LingualScaleStampBuffer.uiFormat        = GxImgFormat;

            uiLineOffset[0] = LingualScaleStampBuffer.uiYLineOffset;
            uiLineOffset[1] = LingualScaleStampBuffer.uiUVLineOffset;
            uiPxlAddr[0]    = LingualScaleStampBuffer.uiYAddr;
            uiPxlAddr[1]    = LingualScaleStampBuffer.uiUAddr;

            GxImg_InitBufEx(&DstImg,
                            g_uiLingualScaledStampWidth[i][(g_uiLingualStampActBuf + 1) % LINGUAL_STAMP_BUF_TATAL],
                            g_uiLingualScaledStampHeight,
                            GxImgFormat,
                            uiLineOffset,
                            uiPxlAddr);

            // Set scale region
            SrcRegion.x = 0;
            SrcRegion.y = 0;
            SrcRegion.w = g_uiLingualStampWidth[i];
            SrcRegion.h = g_uiLingualStampHeight;

            DstRegion.x = 0;
            DstRegion.y = 0;
            DstRegion.w = g_uiLingualScaledStampWidth[i][(g_uiLingualStampActBuf + 1) % LINGUAL_STAMP_BUF_TATAL];
            DstRegion.h = g_uiLingualScaledStampHeight;

            // Scale image by ISE
            GxImg_SetParm(GXIMG_PARM_SCALE_METHOD,GXIMG_SCALE_NEAREST);
            GxImg_ScaleData(&SrcImg, &SrcRegion, &DstImg, &DstRegion);
            GxImg_SetParm(GXIMG_PARM_SCALE_METHOD,GXIMG_SCALE_AUTO);
        }
    }
    if (g_uiLingualStampScalingFactor[0])
    g_uiLingualStampActBuf = (g_uiLingualStampActBuf + 1) % LINGUAL_STAMP_BUF_TATAL;
}
#endif
//#NT#2016/03/31#YongChang_Qui -begin
//#NT#Keep movie ae log on osd if switched on
#if (IPCAM_FUNC && !MOVIE_AE_LOG)
//#NT#2016/03/31#YongChang_Qui -end
extern int FType_DrawHDMI(NVTFTYPE_IPC_IMAGE *image_config, UINT32 ftype_img_num, UINT32 *use_ftype_img, UINT32 *srcBuf, UINT32 bufLen);
static UINT8 YUVDTbl[3][256] = { 0 };
static int FType_DrawString(UINT32 image_idx, UINT32 *srcBuf)
{
    GX_IMAGE_PIXEL_FMT GxImgFormat;
    UINT32             uiWidth;
    UINT32             uiBgData;
    UINT32             uiFBAddrY, uiFBAddrCb, uiFBAddrCr;
    NVTFTYPE_IPC_IMAGE *image_config;
    IMG_BUF            SrcImg, DstImg;
    UINT32             uiLineOffset[2] = {0};
    UINT32             uiPxlAddr[2] = {0};
    UINT32             lookupTblAdr[3] = {(UINT32)&YUVDTbl[0], (UINT32)&YUVDTbl[1], (UINT32)&YUVDTbl[2]};
    UINT32             alpha_src_buf, alpha_dst_buf;

    if(image_idx >= ftype_img_num){
        DBG_ERR("image_idx(%d) >= ftype_img_num(%d)\r\n", image_idx, ftype_img_num);
        return -1;
    }

    image_config = &(((NVTFTYPE_IPC_IMAGE *)ftype_img_dst)[image_idx]);

    if(!image_config->on || !use_ftype_img[image_config->channel]){
        return -1;
    }

    if(image_config->channel < 0 || image_config->channel >= VIDEO_IN_MAX){
        return -1;
    }

    if ((g_uiMovieStampSetup[image_config->channel] & STAMP_SWITCH_MASK) == STAMP_ON)
    {
        switch (g_uiMovieStampSetup[image_config->channel] & STAMP_IMG_FORMAT_MASK)
        {
        case STAMP_IMG_420UV:
            GxImgFormat = GX_IMAGE_PIXEL_FMT_YUV420_PACKED;
            break;

        case STAMP_IMG_422UV:
            GxImgFormat = GX_IMAGE_PIXEL_FMT_YUV422_PACKED;
            break;

        default:
            DBG_ERR("Only support 420/422 UV pack!\r\n");
            return -1;
        }
    }
    else
        return -1;

    if(!image_config->width || !image_config->height){
        return -1;
    }

    uiFBAddrY  = *srcBuf;
    uiFBAddrCb = uiFBAddrY + (image_config->width * image_config->height);
    uiFBAddrCr = uiFBAddrCb;
    uiWidth = image_config->width;
    alpha_dst_buf = uiFBAddrCb + (image_config->width * image_config->height);
    *srcBuf += (3 * image_config->width * image_config->height);

    if (g_uiMovieStampFormat[image_config->channel] == STAMP_FORMAT_IDX) // color index format (for direct path only)
    {
        #if (COLOR_IDX_MODE == COLOR_IDX_4BIT)
        uiWidth >>= 1;
        #else
        uiWidth >>= 2;
        #endif
        uiBgData = COLOR_ID_BG;
    }
    else // STAMP_FORMAT_YUV
    {
        if ((g_uiMovieStampSetup[image_config->channel] & STAMP_BG_FORMAT_MASK) == STAMP_BG_TRANSPARENT)
        {
            //uiBgData = COLORKEY_BG_YUV; // for LE op
            uiBgData = COLORKEY_YUV; // for EQ op
        }
        else
        {
            uiBgData =  ((UINT32)g_MovieStampInfo[image_config->channel].Color[COLOR_ID_BG].ucV << 16) |
                ((UINT32)g_MovieStampInfo[image_config->channel].Color[COLOR_ID_BG].ucU << 8)  |
                g_MovieStampInfo[image_config->channel].Color[COLOR_ID_BG].ucY;
        }
    }

    // Clear source (stamp) buffer (in fact, index color mode doesn't need to copy Y, U, V data,
    // it only needs to copy "color index" data; but however, use GxImg function temporarily.
    uiLineOffset[0] = image_config->width;
    uiLineOffset[1] = image_config->width;
    uiPxlAddr[0]    = uiFBAddrY;
    uiPxlAddr[1]    = uiFBAddrCb;

    memset(&SrcImg, 0, sizeof(SrcImg));
    GxImg_InitBufEx(&SrcImg, uiWidth, image_config->height, GxImgFormat, uiLineOffset, uiPxlAddr);
    GxImg_FillData(&SrcImg, REGION_MATCH_IMG, uiBgData);

    YUVDTbl[0][0]   = uiBgData;
    YUVDTbl[0][240] = RGB_GET_Y((image_config->edge_color & 0x0FF0000) >> 16, (image_config->edge_color & 0x0FF00) >> 8, image_config->edge_color & 0x0FF);
    YUVDTbl[0][255] = RGB_GET_Y((image_config->font_color & 0x0FF0000) >> 16, (image_config->font_color & 0x0FF00) >> 8, image_config->font_color & 0x0FF);
    YUVDTbl[1][0]   = uiBgData;
    YUVDTbl[1][240] = RGB_GET_U((image_config->edge_color & 0x0FF0000) >> 16, (image_config->edge_color & 0x0FF00) >> 8, image_config->edge_color & 0x0FF);
    YUVDTbl[1][255] = RGB_GET_U((image_config->font_color & 0x0FF0000) >> 16, (image_config->font_color & 0x0FF00) >> 8, image_config->font_color & 0x0FF);
    YUVDTbl[2][0]   = uiBgData;
    YUVDTbl[2][240] = RGB_GET_V((image_config->edge_color & 0x0FF0000) >> 16, (image_config->edge_color & 0x0FF00) >> 8, image_config->edge_color & 0x0FF);
    YUVDTbl[2][255] = RGB_GET_V((image_config->font_color & 0x0FF0000) >> 16, (image_config->font_color & 0x0FF00) >> 8, image_config->font_color & 0x0FF);

    uiLineOffset[0] = image_config->width;
    uiPxlAddr[0]    = ((UINT32)image_config->bmp_ptr + (UINT32)ftype_img_dst);
    memset(&SrcImg, 0, sizeof(SrcImg));
    GxImg_InitBufEx(&SrcImg, image_config->width, image_config->height, GX_IMAGE_PIXEL_FMT_Y_ONLY, uiLineOffset, uiPxlAddr);

    uiLineOffset[0] = image_config->width;
    uiLineOffset[1] = image_config->width;
    uiPxlAddr[0]    = uiFBAddrY;
    uiPxlAddr[1]    = uiFBAddrCb;
    memset(&DstImg, 0, sizeof(DstImg));
    GxImg_InitBufEx(&DstImg, image_config->width, image_config->height, GX_IMAGE_PIXEL_FMT_YUV420_PACKED, uiLineOffset, uiPxlAddr);

    GxImg_GrayToYUV(&SrcImg, &DstImg, lookupTblAdr);

    hwmem_open();
    alpha_src_buf = ((UINT32)image_config->alpha_ptr + (UINT32)ftype_img_dst);
    hwmem_memcpy(alpha_dst_buf, alpha_src_buf, image_config->width * image_config->height);
    hwmem_close();

    image_config->bmp_ptr   = uiFBAddrY;
    image_config->alpha_ptr = alpha_dst_buf;

    return 0;
}

static void FType_UpdateData(void)
{
    UINT32          i, total_size = 0;
    UINT32          srcBuf;
    UINT32          channel_ready[OSD_MAX_CHANNEL] = { 1 };
    int             channel;
    NVTFTYPE_IPC_IMAGE *image_config = (NVTFTYPE_IPC_IMAGE *)ftype_img_dst;

    for (i = 0; i < ftype_img_num; i++)
        if(image_config[i].on && image_config[i].channel >= 0 && image_config[i].channel < VIDEO_IN_MAX && use_ftype_img[image_config[i].channel])
            if(image_config[i].bmp_ptr && image_config[i].alpha_ptr)
                total_size += (2 * image_config[i].width * image_config[i].height);

    if(total_size > g_uiMovieStampSize){
        for(i = 0 ; i < OSD_MAX_CHANNEL ; ++i)
            use_ftype_img[i] = 0;
        DBG_ERR("OSD image[%d]'s buffer size(%d) overruns movie stamp size(%d)\r\n", i, total_size, g_uiMovieStampSize);
        return;
    }

    if (g_uiMovieStampActBuf == MOVIE_STAMP_BUF_0)
        srcBuf = g_uiMovieStampYAddr[MOVIE_STAMP_BUF_1];
    else
        srcBuf = g_uiMovieStampYAddr[MOVIE_STAMP_BUF_0];

    for(i = 0 ; i < OSD_MAX_CHANNEL ; ++i)
        channel_ready[i] = 1;

    //Prepare YUV buffer for streaming
    for (i = 0; i < ftype_img_num; i++)
    {
        channel = image_config[i].channel;

        if(!image_config[i].on || !use_ftype_img[channel])
            continue;

        if(channel < 0 || channel >= VIDEO_IN_MAX){
            channel_ready[channel] = 0;
            continue;
        }

        if(!image_config[i].bmp_ptr && !image_config[i].alpha_ptr)
            continue;

        if(!image_config[i].width || !image_config[i].height){
            DBG_ERR("image_config[%d] dim[%d,%d] skipped\r\n", i, image_config[i].width, image_config[i].height);
            channel_ready[channel] = 0;
            continue;
        }

        if(FType_DrawString(i, &srcBuf))
            channel_ready[channel] = 0;
    }

    if(FType_DrawHDMI(image_config, ftype_img_num, use_ftype_img, &srcBuf, g_uiMovieStampSize - (srcBuf - (g_uiMovieStampActBuf == MOVIE_STAMP_BUF_0 ? g_uiMovieStampYAddr[MOVIE_STAMP_BUF_1] : g_uiMovieStampYAddr[MOVIE_STAMP_BUF_0]))))
        channel_ready[OSD_HDMI_CHANNEL] = 0;

    for(i = 0 ; i < OSD_MAX_CHANNEL ; ++i)
        use_ftype_img[i] = (use_ftype_img[i] && channel_ready[i]);

    if (g_uiMovieStampActBuf == MOVIE_STAMP_BUF_0)
        g_uiMovieStampActBuf = MOVIE_STAMP_BUF_1;
    else
        g_uiMovieStampActBuf = MOVIE_STAMP_BUF_0;
}

//#NT#2017/02/06#Charlie Chang -begin
//#NT# support PIP
#define PIP_TO_VEDIO_ID 0
#define PIP_FROM_VEDEO_ID 2
void MovieStamp_PIPCopyData(UINT32 uiVidEncId, UINT32 yAddr, UINT32 cbAddr, UINT32 crAddr, UINT32 lineY, UINT32 imgWidth){


	UINT32  imgHeight =0;
	UINT32  uiLineOffset[2] = {0};
    UINT32  uiPxlAddr[2] = {0};
	IMG_BUF DstImg;
	static IMG_BUF CopyImg={0};
	GX_IMAGE_PIXEL_FMT GxImgFormat = GX_IMAGE_PIXEL_FMT_YUV420_PACKED;


	MovRec_GetInfo(MOVREC_GETINFO_WIDTH_HEIGHT, &imgWidth, &imgHeight, &uiVidEncId);
    uiLineOffset[0] = lineY;
    uiLineOffset[1] = lineY;
    uiPxlAddr[0]    = yAddr;
    uiPxlAddr[1]    = cbAddr;

    GxImg_InitBufEx(&DstImg,
                     imgWidth,
                     imgHeight,
                     GxImgFormat,
                     uiLineOffset,
                     uiPxlAddr);


	if(uiVidEncId==PIP_FROM_VEDEO_ID){
		memcpy(&CopyImg,&DstImg,sizeof(DstImg));
	}
	if(uiVidEncId==PIP_TO_VEDIO_ID){
		if(CopyImg.Height <=0){
			/////maybe Copy not initial , wait get CopyImg
			return;
		}

		if(CopyImg.Height > DstImg.Height || CopyImg.Width > DstImg.Width){
			DBG_ERR("vid(%d) w=%d h=%d  vid(%d) w=%d h=%d, copy size is too big\r\n",
				PIP_FROM_VEDEO_ID, CopyImg.Width, CopyImg.Height,
				PIP_TO_VEDIO_ID, DstImg.Width, DstImg.Height);
			return;
		}

		GxImg_CopyData(&CopyImg, REGION_MATCH_IMG, &DstImg, REGION_MATCH_IMG);
	}

}


#define MAX_BLENDING_WIDTH 1280
void MovieStamp_blendCopyData(UINT32 uiVidEncId, UINT32 yAddr, UINT32 cbAddr, UINT32 crAddr, UINT32 lineY, UINT32 imgWidth){


	UINT32  imgHeight =0;
	UINT32  uiLineOffset[2] = {0};
    UINT32  uiPxlAddr[2] = {0};
	IMG_BUF DstImg;
	static IMG_BUF CopyImg={0};
	GX_IMAGE_PIXEL_FMT GxImgFormat = GX_IMAGE_PIXEL_FMT_YUV420_PACKED;


	MovRec_GetInfo(MOVREC_GETINFO_WIDTH_HEIGHT, &imgWidth, &imgHeight, &uiVidEncId);
    uiLineOffset[0] = lineY;
    uiLineOffset[1] = lineY;
    uiPxlAddr[0]    = yAddr;
    uiPxlAddr[1]    = cbAddr;

    GxImg_InitBufEx(&DstImg,
                     imgWidth,
                     imgHeight,
                     GxImgFormat,
                     uiLineOffset,
                     uiPxlAddr);

	if(uiVidEncId==g_MovieStampBlendCfg.uiSrcVideoID){
		memcpy(&CopyImg,&DstImg,sizeof(DstImg));
	}
	if(uiVidEncId==g_MovieStampBlendCfg.uiDesVideoID){
		if(CopyImg.Height <=0){
			/////maybe Copy not initial , wait get CopyImg
			return;
		}

		if(CopyImg.Height != DstImg.Height || CopyImg.Width != DstImg.Width){
			DBG_ERR("vid(%d) w=%d h=%d  vid(%d) w=%d h=%d, blending size error\r\n",
				PIP_FROM_VEDEO_ID, CopyImg.Width, CopyImg.Height,
				PIP_TO_VEDIO_ID, DstImg.Width, DstImg.Height);
			return;
		}
		if(CopyImg.Width > MAX_BLENDING_WIDTH){
			DBG_ERR("width too hight please set <=%d\r\n",MAX_BLENDING_WIDTH);
			return;
		}
		GxImg_CopyBlendingData(&CopyImg, REGION_MATCH_IMG, &DstImg, REGION_MATCH_IMG, g_MovieStampBlendCfg.uiAlpha);

	}

}




//#NT#2017/02/06#Charlie Chang -end
void MovieStamp_CopyData(UINT32 uiVidEncId, UINT32 yAddr, UINT32 cbAddr, UINT32 crAddr, UINT32 lineY, UINT32 imgWidth)
{
    GX_IMAGE_PIXEL_FMT GxImgFormat;
    STAMP_BUFFER    StampBuffer;
    IMG_BUF         SrcImg, DstImg;
    UINT32          x, y;
    UINT32          uiYAddrOffset, uiUAddrOffset;
    UINT32          uiStampDataHeight;
    UINT32          uiLineOffset[2] = {0};
    UINT32          uiPxlAddr[2] = {0};
    UINT32          i, total_size, color = 0, is_mask;
    IRECT           MaskRgn;
    NVTFTYPE_IPC_IMAGE *image_config = (NVTFTYPE_IPC_IMAGE *)ftype_img_dst;

	//#NT#2017/02/06#Charlie Chang -begin
    //#NT# support PIP
    if(g_bPipEnable == TRUE){
		MovieStamp_PIPCopyData(uiVidEncId, yAddr, cbAddr, crAddr, lineY, imgWidth);
    }
	//#NT#2017/02/06#Charlie Chang -end

	//#NT#2017/02/16#Charlie Chang -begin
    //#NT# support blending
	if(g_MovieStampBlendCfg.bEnable == TRUE){
		MovieStamp_blendCopyData(uiVidEncId, yAddr, cbAddr, crAddr, lineY, imgWidth);
	}
	//#NT#2017/02/16#Charlie Chang -end
    if (uiVidEncId >= VIDEO_IN_MAX)
    {
        //DBG_ERR("uiVidEncId = %d\r\n",uiVidEncId);
        return;
    }

    if ((g_uiMovieStampSetup[uiVidEncId] & STAMP_SWITCH_MASK) == STAMP_OFF)
    {
        return;
    }

    if (yAddr == 0 || cbAddr == 0)
    {
        return;
    }

    Acquire_FType_Buf();
    //#NT#2016/10/24#Charlie Chang -begin
    //#NT# fixed no using freetype issue
    if(!use_ftype_img[uiVidEncId]){
        #if (MOVIE_FTG_DRAW_VIDEO== ENABLE)
            switch (g_uiMovieStampSetup[uiVidEncId] & STAMP_IMG_FORMAT_MASK)
            {
                case STAMP_IMG_420UV:
                    GxImgFormat = GX_IMAGE_PIXEL_FMT_YUV420_PACKED;
                    break;
                case STAMP_IMG_422UV:
                    GxImgFormat = GX_IMAGE_PIXEL_FMT_YUV422_PACKED;
                    break;
                default:
                    DBG_ERR("Only support 420/422 UV pack!\r\n");
                    goto out;
            }
            goto DO_DRAWALG;

        #else
            goto out;
        #endif
    }
    //#NT#2016/10/24#Charlie Chang -end
    total_size = 0;
    for(i = 0 ; i < ftype_img_num ; ++i)
        if(image_config[i].on && image_config[i].channel == uiVidEncId)
            if(image_config[i].bmp_ptr && image_config[i].alpha_ptr)
                total_size += (2 * image_config[i].width * image_config[i].height);

    if(total_size > g_uiMovieStampSize){
        DBG_ERR("OSD image[%d]'s buffer size(%d) overruns movie stamp size(%d)\r\n", uiVidEncId, total_size, g_uiMovieStampSize);
        goto out;
    }

    switch (g_uiMovieStampSetup[uiVidEncId] & STAMP_IMG_FORMAT_MASK)
    {
    case STAMP_IMG_420UV:
        GxImgFormat = GX_IMAGE_PIXEL_FMT_YUV420_PACKED;
        break;

    case STAMP_IMG_422UV:
        GxImgFormat = GX_IMAGE_PIXEL_FMT_YUV422_PACKED;
        break;

    default:
        DBG_ERR("Only support 420/422 UV pack!\r\n");
        goto out;
    }

    for(i = 0 ; i < ftype_img_num ; ++i){

        if(!image_config[i].on || image_config[i].channel != uiVidEncId)
            continue;

        if(!image_config[i].width || !image_config[i].height)
            continue;

        if(image_config[i].bmp_ptr && image_config[i].alpha_ptr)
            is_mask = 0;
        else
            is_mask = 1;

        x = image_config[i].x;
        y = image_config[i].y;
        if(x > imgWidth || (x + image_config[i].width) > imgWidth)
            continue;

        // Y address offset of destination image to put Y stamp data
        uiYAddrOffset = lineY * y + x;
        // UV address offset of destination image to put UV stamp data
        if (GxImgFormat == GX_IMAGE_PIXEL_FMT_YUV420_PACKED)
        {
            uiUAddrOffset = lineY * y / 2 + x;
        }
        else
        {
            uiUAddrOffset = lineY * y + x;
        }

        if(is_mask){
            MaskRgn.x = x;
            MaskRgn.y = y;
            MaskRgn.w = image_config[i].width;
            MaskRgn.h = image_config[i].height;
            color = RGB_GET_V((image_config[i].font_color & 0x0FF0000) >> 16, (image_config[i].font_color & 0x0FF00) >> 8, image_config[i].font_color & 0x0FF);
            color <<= 8;
            color += RGB_GET_U((image_config[i].font_color & 0x0FF0000) >> 16, (image_config[i].font_color & 0x0FF00) >> 8, image_config[i].font_color & 0x0FF);
            color <<= 8;
            color += RGB_GET_Y((image_config[i].font_color & 0x0FF0000) >> 16, (image_config[i].font_color & 0x0FF00) >> 8, image_config[i].font_color & 0x0FF);
        }
        else{
            uiStampDataHeight           = image_config[i].height;
            StampBuffer.uiYLineOffset   = image_config[i].width;
            StampBuffer.uiUVLineOffset  = image_config[i].width;
            StampBuffer.uiYAddr         = image_config[i].bmp_ptr;
            StampBuffer.uiUAddr         = StampBuffer.uiYAddr + StampBuffer.uiYLineOffset * uiStampDataHeight;
            StampBuffer.uiVAddr         = StampBuffer.uiUAddr; // for 420 UV pack
            StampBuffer.uiFormat        = GxImgFormat;

            // copy data from date stamp draw buffer to image buffer
            // Init source (stamp) buffer
            uiLineOffset[0] = StampBuffer.uiYLineOffset;
            uiLineOffset[1] = StampBuffer.uiUVLineOffset;
            uiPxlAddr[0]    = StampBuffer.uiYAddr;
            uiPxlAddr[1]    = StampBuffer.uiUAddr;

            GxImg_InitBufEx(&SrcImg,
                            image_config[i].width,
                            uiStampDataHeight,
                            GxImgFormat,
                            uiLineOffset,
                            uiPxlAddr);
        }

        // Init destination (image) buffer
        uiLineOffset[0] = lineY;
        uiLineOffset[1] = lineY;
        if(is_mask){
            uiPxlAddr[0]    = yAddr;
            uiPxlAddr[1]    = cbAddr;
        }
        else{
            uiPxlAddr[0]    = yAddr  + uiYAddrOffset;
            uiPxlAddr[1]    = cbAddr + uiUAddrOffset;
        }

        GxImg_InitBufEx(&DstImg,
                        lineY,
                        y + image_config[i].height, // don't care, but should be > 2
                        GxImgFormat,
                        uiLineOffset,
                        uiPxlAddr);

        if ((g_uiMovieStampSetup[uiVidEncId] & STAMP_BG_FORMAT_MASK) == STAMP_BG_TRANSPARENT)
        {
            if(is_mask)
                GxImg_FillDataEx(&DstImg, &MaskRgn, color, GX_IMAGE_FILL_ENG1);
            else
                GxImg_CopyBlendingDataEx(&SrcImg, REGION_MATCH_IMG, &DstImg, REGION_MATCH_IMG, (UINT8*)image_config[i].alpha_ptr);
        }
        else
        {
            GxImg_CopyData(&SrcImg, REGION_MATCH_IMG, &DstImg, REGION_MATCH_IMG);
        }
    }
#if (MOVIE_FTG_DRAW_VIDEO== ENABLE)
DO_DRAWALG:
#endif
    //#NT#2016/06/08#Lincy Lin -begin
    //#NT#Implement generic OSD and video drawing mechanism for ALG function
    {
        UINT32   imgHeight =0;
        // Init destination (image) buffer
        uiLineOffset[0] = lineY;
        uiLineOffset[1] = lineY;
        uiPxlAddr[0]    = yAddr;
        uiPxlAddr[1]    = cbAddr;

		MovRec_GetInfo(MOVREC_GETINFO_WIDTH_HEIGHT, &imgWidth, &imgHeight, &uiVidEncId);

        GxImg_InitBufEx(&DstImg,
                        imgWidth,
                        imgHeight,
                        GxImgFormat,
                        uiLineOffset,
                        uiPxlAddr);
        MovieStamp_DrawALG(uiVidEncId,&DstImg);
    }
    //#NT#2016/06/08#Lincy Lin -end


out:
    Release_FType_Buf();
}
#else
void MovieStamp_UpdateData(void)
{
    GX_IMAGE_PIXEL_FMT GxImgFormat;
    STAMP_BUFFER    StampBuffer;
    struct tm       CurDateTime;
    IMG_BUF         SrcImg = {0};                               /// Fixed for CID 28134
    UINT32          uiPreWidth, uiWidth;
    UINT32          uiStampDataWidth[VIDEO_IN_MAX] = {0};       /// Fixed for CID 28133
    UINT32          uiStampDataHeight;
    UINT32          uiBgData;
    UINT32          uiLineOffset[2] = {0};
    UINT32          uiPxlAddr[2] = {0};
    UINT32          i;
    #if (CHECK_STAMP_TIME == ENABLE)
    UINT32          uiTime, uiTimeCount; // for performance measurement
    #endif

    //CurDateTime = HwClock_GetTime(TIME_ID_CURRENT);
    CurDateTime = g_CurDateTime;
    debug_msg("\r\n************MovieStamp_UpdateData************\r\n");
    for (i = 0; i < VIDEO_IN_MAX; i++)
    {
        if ((g_uiMovieStampSetup[i] & STAMP_SWITCH_MASK) == STAMP_ON)
        {
            switch (g_uiMovieStampSetup[i] & STAMP_IMG_FORMAT_MASK)
            {
            case STAMP_IMG_420UV:
                GxImgFormat = GX_IMAGE_PIXEL_FMT_YUV420_PACKED;
                break;

            case STAMP_IMG_422UV:
                GxImgFormat = GX_IMAGE_PIXEL_FMT_YUV422_PACKED;
                break;

            default:
                DBG_ERR("Only support 420/422 UV pack!\r\n");
                return;
            }

            uiStampDataHeight = g_MovieStampInfo[i].ui32DstHeight;
            StampBuffer.uiYLineOffset   = STAMP_WIDTH_MAX;
            StampBuffer.uiUVLineOffset  = STAMP_WIDTH_MAX;
            if (g_uiMovieStampActBuf == MOVIE_STAMP_BUF_0)
                StampBuffer.uiYAddr     = g_uiMovieStampYAddr[i][MOVIE_STAMP_BUF_1];
            else
                StampBuffer.uiYAddr     = g_uiMovieStampYAddr[i][MOVIE_STAMP_BUF_0];
            StampBuffer.uiUAddr         = StampBuffer.uiYAddr + StampBuffer.uiYLineOffset * uiStampDataHeight;
            StampBuffer.uiVAddr         = StampBuffer.uiUAddr; // for 420 UV pack
            StampBuffer.uiFormat        = GxImgFormat;

            #if (MOVIE_AE_LOG == 0 && MOVIE_ADAS_LOG == 0)
            // Prepare date-time string
            if ((g_uiMovieStampSetup[i] & STAMP_DATE_TIME_MASK) == STAMP_DATE_TIME)
            {
               // debug_msg("\r\n**********g_CurSpeed = %f, %d, %u,%03d, km/h**************\r\n",g_CurSpeed, (UINT32)g_CurSpeed,(UINT32)g_CurSpeed,(UINT32)g_CurSpeed);
                switch (g_uiMovieStampSetup[i] & STAMP_DATE_FORMAT_MASK)
                {
#if defined(YQCONFIG_ANDROID_SYSTEM_SUPPORT)
                case STAMP_DD_MM_YY:debug_msg("1");
                    if((UINT32)g_CurSpeed==0)
                        snprintf(&g_cMovieStampStr[i][0], MOVIE_STAMP_MAX_LEN, "%02d/%02d/%04d %02d:%02d:%02d  ---km/h", CurDateTime.tm_mday, CurDateTime.tm_mon, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
                    else
                        snprintf(&g_cMovieStampStr[i][0], MOVIE_STAMP_MAX_LEN, "%02d/%02d/%04d %02d:%02d:%02d  %03d km/h",CurDateTime.tm_mday, CurDateTime.tm_mon, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec, (UINT32)g_CurSpeed);
                    break;
                case STAMP_MM_DD_YY:debug_msg("2");
                    if((UINT32)g_CurSpeed==0)
                        snprintf(&g_cMovieStampStr[i][0], MOVIE_STAMP_MAX_LEN, "%02d/%02d/%04d %02d:%02d:%02d  ---km/h", CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
                    else
                        snprintf(&g_cMovieStampStr[i][0], MOVIE_STAMP_MAX_LEN, "%02d/%02d/%04d %02d:%02d:%02d  %03d km/h",CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec, (UINT32)g_CurSpeed);
                    break;
                default:debug_msg("3");
                    if((UINT32)g_CurSpeed==0)
                        snprintf(&g_cMovieStampStr[i][0], MOVIE_STAMP_MAX_LEN, "%04d/%02d/%02d %02d:%02d:%02d  ---km/h",CurDateTime.tm_year, CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
                    else
                        snprintf(&g_cMovieStampStr[i][0], MOVIE_STAMP_MAX_LEN, "%04d/%02d/%02d %02d:%02d:%02d  %03d km/h", CurDateTime.tm_year, CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec,(UINT32)g_CurSpeed);
                    break;
                    #else
                case STAMP_DD_MM_YY:debug_msg("4");
                    //sprintf(&g_cMovieStampStr[i][0], "%02d/%02d/%04d %02d:%02d:%02d", CurDateTime.tm_mday, CurDateTime.tm_mon, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
                    snprintf(&g_cMovieStampStr[i][0], MOVIE_STAMP_MAX_LEN, "%02d/%02d/%04d %02d:%02d:%02d", CurDateTime.tm_mday, CurDateTime.tm_mon, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
                    break;
                case STAMP_MM_DD_YY:debug_msg("5");
                    //sprintf(&g_cMovieStampStr[i][0], "%02d/%02d/%04d %02d:%02d:%02d", CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
                    snprintf(&g_cMovieStampStr[i][0], MOVIE_STAMP_MAX_LEN, "%02d/%02d/%04d %02d:%02d:%02d", CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
                    break;
                default:debug_msg("6");
                    //sprintf(&g_cMovieStampStr[i][0], "%04d/%02d/%02d %02d:%02d:%02d", CurDateTime.tm_year, CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
                    snprintf(&g_cMovieStampStr[i][0], MOVIE_STAMP_MAX_LEN, "%04d/%02d/%02d %02d:%02d:%02d", CurDateTime.tm_year, CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
                    break;
#endif
                }
            }
            else if ((g_uiMovieStampSetup[i] & STAMP_DATE_TIME_MASK) == STAMP_DATE_TIME_AMPM)
            {
                if (CurDateTime.tm_hour >= 12)
                {
                    if (CurDateTime.tm_hour > 12)
                        CurDateTime.tm_hour -= 12;

                    switch (g_uiMovieStampSetup[i] & STAMP_DATE_FORMAT_MASK)
                    {
                    case STAMP_DD_MM_YY:
                        //sprintf(&g_cMovieStampStr[i][0], "%02d/%02d/%04d %02d:%02d:%02d PM", CurDateTime.tm_mday, CurDateTime.tm_mon, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
                        snprintf(&g_cMovieStampStr[i][0], MOVIE_STAMP_MAX_LEN, "%02d/%02d/%04d %02d:%02d:%02d PM", CurDateTime.tm_mday, CurDateTime.tm_mon, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
                        break;
                    case STAMP_MM_DD_YY:
                        //sprintf(&g_cMovieStampStr[i][0], "%02d/%02d/%04d %02d:%02d:%02d PM", CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
                        snprintf(&g_cMovieStampStr[i][0], MOVIE_STAMP_MAX_LEN, "%02d/%02d/%04d %02d:%02d:%02d PM", CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
                        break;
                    default:
                        //sprintf(&g_cMovieStampStr[i][0], "%04d/%02d/%02d %02d:%02d:%02d PM", CurDateTime.tm_year, CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
                        snprintf(&g_cMovieStampStr[i][0], MOVIE_STAMP_MAX_LEN, "%04d/%02d/%02d %02d:%02d:%02d PM", CurDateTime.tm_year, CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
                        break;
                    }
                }
                else
                {
                    switch (g_uiMovieStampSetup[i] & STAMP_DATE_FORMAT_MASK)
                    {
                    case STAMP_DD_MM_YY:
                        //sprintf(&g_cMovieStampStr[i][0], "%02d/%02d/%04d %02d:%02d:%02d AM", CurDateTime.tm_mday, CurDateTime.tm_mon, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
                        snprintf(&g_cMovieStampStr[i][0], MOVIE_STAMP_MAX_LEN, "%02d/%02d/%04d %02d:%02d:%02d AM", CurDateTime.tm_mday, CurDateTime.tm_mon, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
                        break;
                    case STAMP_MM_DD_YY:
                        //sprintf(&g_cMovieStampStr[i][0], "%02d/%02d/%04d %02d:%02d:%02d AM", CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
                        snprintf(&g_cMovieStampStr[i][0], MOVIE_STAMP_MAX_LEN, "%02d/%02d/%04d %02d:%02d:%02d AM", CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_year, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
                        break;
                    default:
                        //sprintf(&g_cMovieStampStr[i][0], "%04d/%02d/%02d %02d:%02d:%02d AM", CurDateTime.tm_year, CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
                        snprintf(&g_cMovieStampStr[i][0], MOVIE_STAMP_MAX_LEN, "%04d/%02d/%02d %02d:%02d:%02d AM", CurDateTime.tm_year, CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
                        break;
                    }
                }
            }
            else
            {
                //sprintf(&g_cMovieStampStr[i][0], "%02d:%02d:%02d", CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
                snprintf(&g_cMovieStampStr[i][0], MOVIE_STAMP_MAX_LEN, "%02d:%02d:%02d", CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
            }
            #endif

            #if (MOVIE_DDD_FUNC && MOVIE_DDD_DRAW_VIDEO)
            {
                //this is for DDD debug
                snprintf(&g_cMovieStampStr[i][0], MOVIE_STAMP_MAX_LEN, "%06x %02d/%02d %02d:%02d:%02d", g_DDD_DowsyState, CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
            }
            #endif

            #if MOVIE_AE_LOG
            ////<< spark add AE function
            IQS_GetLogString(i, &g_cMovieStampStr[i][0], 256);
            #endif

            #if MOVIE_ADAS_LOG
            //#NT#2016/03/25#KCHong -begin
            //#NT#New ADAS
            ADAS_GetAppsLogString(i, &g_cMovieStampStr[i][0], 256);
            //#NT#2016/03/25#KCHong -end
            #endif

            #if (CHECK_ENC_INFO == ENABLE)
            {
                UINT32 uiParam[7];

                MediaRec_GetEncInfo(i, uiParam);
                //sprintf(&g_cMovieStampStr[i][0], "%d %d %d %d / %d %d %d",
                //    uiParam[0], uiParam[1], uiParam[2], uiParam[3], uiParam[4], uiParam[5], uiParam[6]);
                //sprintf(&g_cMovieStampStr[i][0], "%d %d %d %d / %d %d %d",
                //    uiParam[3], uiParam[4], uiParam[5], uiParam[6], uiParam[0], uiParam[1], uiParam[2]);
                //sprintf(&g_cMovieStampStr[i][0], "%d", uiParam[0]);
                snprintf(&g_cMovieStampStr[i][0], MOVIE_STAMP_MAX_LEN, "%d", uiParam[0]);
            }
            #endif

            uiPreWidth          = g_uiMovieStampWidth[i]; // previous stamp data width
            uiStampDataWidth[i] = MovieStamp_GetStampDataWidth(&g_MovieStampInfo[i]);
            uiStampDataWidth[i] = ALIGN_CEIL_4(uiStampDataWidth[i]); // limitation of index color stamp mode
            uiWidth             = (uiPreWidth > uiStampDataWidth[i]) ? uiPreWidth : uiStampDataWidth[i];
            //DBG_DUMP("^Ystamp w %d, h %d\r\n", uiWidth, uiStampDataHeight);

            // Set background data
            #if (CHECK_STAMP_TIME == ENABLE)
            uiTime = Perf_GetCurrent();
            #endif

            if (g_uiMovieStampFormat[i] == STAMP_FORMAT_IDX) // color index format (for direct path only)
            {
                #if (COLOR_IDX_MODE == COLOR_IDX_4BIT)
                uiWidth >>= 1;
                #else
                uiWidth >>= 2;
                #endif
                uiBgData = COLOR_ID_BG;
            }
            else // STAMP_FORMAT_YUV
            {
                if ((g_uiMovieStampSetup[i] & STAMP_BG_FORMAT_MASK) == STAMP_BG_TRANSPARENT)
                {
                    //uiBgData = COLORKEY_BG_YUV; // for LE op
                    uiBgData = COLORKEY_YUV; // for EQ op
                }
                else
                {
                    uiBgData =  ((UINT32)g_MovieStampInfo[i].Color[COLOR_ID_BG].ucV << 16) |
                                ((UINT32)g_MovieStampInfo[i].Color[COLOR_ID_BG].ucU << 8)  |
                                g_MovieStampInfo[i].Color[COLOR_ID_BG].ucY;
                }
            }

            // Clear source (stamp) buffer (in fact, index color mode doesn't need to copy Y, U, V data,
            // it only needs to copy "color index" data; but however, use GxImg function temporarily.
            uiLineOffset[0] = StampBuffer.uiYLineOffset;
            uiLineOffset[1] = StampBuffer.uiUVLineOffset;
            uiPxlAddr[0]    = StampBuffer.uiYAddr;
            uiPxlAddr[1]    = StampBuffer.uiUAddr;

            GxImg_InitBufEx(&SrcImg,
                            uiWidth,
                            uiStampDataHeight,
                            GxImgFormat,
                            uiLineOffset,
                            uiPxlAddr);

            GxImg_FillData(&SrcImg, REGION_MATCH_IMG, uiBgData);

            #if (CHECK_STAMP_TIME == ENABLE)
            DBG_DUMP("time1: %d us\r\n", Perf_GetCurrent() - uiTime);
            uiTime = Perf_GetCurrent();
            #endif

            // Draw string by getting data from date stamp font
            MovieStamp_DrawString(i, &g_MovieStampInfo[i], &StampBuffer);

            #if (CHECK_STAMP_TIME == ENABLE)
            DBG_DUMP("time2: %d us\r\n", Perf_GetCurrent() - uiTime);
            #endif
        }
    }

    for (i = 0; i < VIDEO_IN_MAX; i++)
    {
        g_uiMovieStampWidth[i] = uiStampDataWidth[i];
    }

    if (g_uiMovieStampActBuf == MOVIE_STAMP_BUF_0)
        g_uiMovieStampActBuf = MOVIE_STAMP_BUF_1;
    else
        g_uiMovieStampActBuf = MOVIE_STAMP_BUF_0;

#if defined (MULTILINGUAL_FUNCTION) && (MULTILINGUAL_FUNCTION == ENABLE)
    MultiLingualString_UpdateData();        // For MultiLingualString demo
#endif
}

// copy stamp data to image buffer (for D2D path)
void MovieStamp_CopyData(UINT32 uiVidEncId, UINT32 yAddr, UINT32 cbAddr, UINT32 crAddr, UINT32 lineY, UINT32 imgWidth)
{
    GX_IMAGE_PIXEL_FMT GxImgFormat;
    STAMP_BUFFER    StampBuffer;
    IMG_BUF         SrcImg = {0}, DstImg = {0};          /// For CID 27234, 28132
    UINT32          x, y;
    UINT32          uiYAddrOffset, uiUAddrOffset;
    UINT32          uiStampDataHeight;
    UINT32          uiLineOffset[2] = {0};
    UINT32          uiPxlAddr[2] = {0};
    #if defined (MULTILINGUAL_FUNCTION) && (MULTILINGUAL_FUNCTION == ENABLE)
    STAMP_BUFFER    LingualScaledStampBuffer;
    #endif

    #if (CHECK_STAMP_TIME == ENABLE)
    UINT32          uiTime, uiTimeCount; // for performance measurement
    #endif

    if (uiVidEncId >= VIDEO_IN_MAX)
    {
        //DBG_ERR("uiVidEncId = %d\r\n",uiVidEncId);
        return;
    }

    //#NT#2016/06/28#KCHong -begin
    //#NT#DbgLog in GPS section
    #if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
    if (uiVidEncId == 0)
    {
        mp4log_IncFrmCnt(1);
    }
    #endif
    //#NT#2016/06/28#KCHong -end

    if ((g_uiMovieStampSetup[uiVidEncId] & STAMP_SWITCH_MASK) == STAMP_OFF)
    {
        return;
    }

    if (yAddr == 0 || cbAddr == 0)
    {
        return;
    }

    //#NT#2016/08/19#Hideo Lin -begin
    //#NT#To avoid different stamp overlap for same source image
    {
        UINT32 frame_rate = 0, rec_format = 0, video_id = uiVidEncId;
        MovRec_GetInfo(MOVREC_GETINFO_VIDEO_FPS, &frame_rate, &video_id, 0);
        MovRec_GetInfo(MOVREC_GETINFO_RECFORMAT, &rec_format, 0, 0);

        if ((g_uiMovieSrcYAddr[uiVidEncId] == yAddr) &&
            (rec_format != MEDIAREC_TIMELAPSE) &&
            (rec_format != MEDIAREC_MERGEYUV) &&
            (frame_rate >= 15))
        {
            // if Y address of current encode path is the same as previous one (i.e. the same source image),
            // do not apply data stamp otherwise the stamp data will overlap
            return;
        }
        g_uiMovieSrcYAddr[uiVidEncId] = yAddr;
    }
    //#NT#2016/08/19#Hideo Lin -end

    switch (g_uiMovieStampSetup[uiVidEncId] & STAMP_IMG_FORMAT_MASK)
    {
    case STAMP_IMG_420UV:
        GxImgFormat = GX_IMAGE_PIXEL_FMT_YUV420_PACKED;
        break;

    case STAMP_IMG_422UV:
        GxImgFormat = GX_IMAGE_PIXEL_FMT_YUV422_PACKED;
        break;

    default:
        DBG_ERR("Only support 420/422 UV pack!\r\n");
        return;
    }

    x = g_MovieStampPos[uiVidEncId].uiX;
    y = g_MovieStampPos[uiVidEncId].uiY;

    // Y address offset of destination image to put Y stamp data
    uiYAddrOffset = lineY * y + x;
    // UV address offset of destination image to put UV stamp data
    if (GxImgFormat == GX_IMAGE_PIXEL_FMT_YUV420_PACKED)
    {
        uiUAddrOffset = lineY * y / 2 + x;
    }
    else
    {
        uiUAddrOffset = lineY * y + x;
    }

    uiStampDataHeight = g_MovieStampInfo[uiVidEncId].ui32DstHeight;

    StampBuffer.uiYLineOffset   = STAMP_WIDTH_MAX;
    StampBuffer.uiUVLineOffset  = STAMP_WIDTH_MAX;
    StampBuffer.uiYAddr         = g_uiMovieStampYAddr[uiVidEncId][g_uiMovieStampActBuf];
    StampBuffer.uiUAddr         = StampBuffer.uiYAddr + StampBuffer.uiYLineOffset * uiStampDataHeight;
    StampBuffer.uiVAddr         = StampBuffer.uiUAddr; // for 420 UV pack
    StampBuffer.uiFormat        = GxImgFormat;

    #if (CHECK_STAMP_TIME == ENABLE)
    //Perf_Open(); // for performance measurement
    uiTime = Perf_GetCurrent();
    #endif

    // copy data from date stamp draw buffer to image buffer
    // Init source (stamp) buffer
    uiLineOffset[0] = StampBuffer.uiYLineOffset;
    uiLineOffset[1] = StampBuffer.uiUVLineOffset;
    uiPxlAddr[0]    = StampBuffer.uiYAddr;
    uiPxlAddr[1]    = StampBuffer.uiUAddr;

    GxImg_InitBufEx(&SrcImg,
                    g_uiMovieStampWidth[uiVidEncId],
                    uiStampDataHeight,
                    GxImgFormat,
                    uiLineOffset,
                    uiPxlAddr);

    // Init destination (image) buffer
    uiLineOffset[0] = lineY;
    uiLineOffset[1] = lineY;
    uiPxlAddr[0]    = yAddr  + uiYAddrOffset;
    uiPxlAddr[1]    = cbAddr + uiUAddrOffset;

    GxImg_InitBufEx(&DstImg,
                    imgWidth,
                    240, // don't care, but should be > 2
                    GxImgFormat,
                    uiLineOffset,
                    uiPxlAddr);

    if ((g_uiMovieStampSetup[uiVidEncId] & STAMP_BG_FORMAT_MASK) == STAMP_BG_TRANSPARENT)
    {
        GxImg_CopyColorkeyData(&DstImg, REGION_MATCH_IMG, &SrcImg, REGION_MATCH_IMG, COLORKEY_YUV, FALSE);
    }
    else
    {
        GxImg_CopyData(&SrcImg, REGION_MATCH_IMG, &DstImg, REGION_MATCH_IMG);
    }

#if defined (MULTILINGUAL_FUNCTION) && (MULTILINGUAL_FUNCTION == ENABLE)
    uiYAddrOffset = lineY * g_LingualStampPos[uiVidEncId].uiY + g_LingualStampPos[uiVidEncId].uiX;
    if (GxImgFormat == GX_IMAGE_PIXEL_FMT_YUV420_PACKED)
    {
        uiUAddrOffset = lineY * g_LingualStampPos[uiVidEncId].uiY / 2 + g_LingualStampPos[uiVidEncId].uiX;
    }
    else
    {
        uiUAddrOffset = lineY * g_LingualStampPos[uiVidEncId].uiY + g_LingualStampPos[uiVidEncId].uiX;
    }

    // Update scaling factor
    switch(imgWidth)
    {
        case  1920: // 1920x1080
        case  1536: // 1536x1536, 26x44
            g_uiLingualStampScalingFactor[uiVidEncId] = 44;
            break;
        case  1440: // 1440x1080
        case  1728: // 1728x1296, (DAR 16:9)20x44
            g_uiLingualStampScalingFactor[uiVidEncId] = 33;
            break;
        case  1280: // 1280x720, 18x30
            g_uiLingualStampScalingFactor[uiVidEncId] = 30;
            break;
        case  640: //  640x480, 12x20
        case  848: //  848x480, 12x20
            g_uiLingualStampScalingFactor[uiVidEncId] = 20;
            break;
        default: // 320x240, 10x16
            g_uiLingualStampScalingFactor[uiVidEncId] = 1;
            break;
    }

    if (g_uiLingualStampActBuf != LINGUAL_STAMP_BUF_OFF)
    {

        LingualScaledStampBuffer.uiYLineOffset    = STAMP_WIDTH_MAX;
        LingualScaledStampBuffer.uiUVLineOffset   = STAMP_WIDTH_MAX;
        LingualScaledStampBuffer.uiYAddr          = g_uiLingualScaledStampYAddr[uiVidEncId][g_uiLingualStampActBuf];
        LingualScaledStampBuffer.uiUAddr          = LingualScaledStampBuffer.uiYAddr + LingualScaledStampBuffer.uiYLineOffset * g_uiLingualScaledStampHeight;
        LingualScaledStampBuffer.uiVAddr          = LingualScaledStampBuffer.uiUAddr; // for 420 UV pack
        LingualScaledStampBuffer.uiFormat         = GxImgFormat;
        uiLineOffset[0] = LingualScaledStampBuffer.uiYLineOffset;
        uiLineOffset[1] = LingualScaledStampBuffer.uiUVLineOffset;
        uiPxlAddr[0]    = LingualScaledStampBuffer.uiYAddr;
        uiPxlAddr[1]    = LingualScaledStampBuffer.uiUAddr;
        GxImg_InitBufEx(&SrcImg,
                        g_uiLingualScaledStampWidth[uiVidEncId][g_uiLingualStampActBuf],
                        g_uiLingualScaledStampHeight,
                        GxImgFormat,
                        uiLineOffset,
                        uiPxlAddr);

        // Init destination (image) buffer
        uiLineOffset[0] = lineY;
        uiLineOffset[1] = lineY;
        uiPxlAddr[0]    = yAddr  + uiYAddrOffset;
        uiPxlAddr[1]    = cbAddr + uiUAddrOffset;

        GxImg_InitBufEx(&DstImg,
                        imgWidth,
                        240, // don't care, but should be > 2
                        GxImgFormat,
                        uiLineOffset,
                        uiPxlAddr);

        if ((g_uiMovieStampSetup[uiVidEncId] & STAMP_BG_FORMAT_MASK) == STAMP_BG_TRANSPARENT)
        {
            GxImg_CopyColorkeyData(&DstImg, REGION_MATCH_IMG, &SrcImg, REGION_MATCH_IMG, COLORKEY_YUV, FALSE);
        }
        else
        {
            GxImg_CopyData(&SrcImg, REGION_MATCH_IMG, &DstImg, REGION_MATCH_IMG);
        }
    }
#endif
    #if defined (WATERLOGO_FUNCTION) && (WATERLOGO_FUNCTION == ENABLE)
    {
        if (g_bWaterLogoEnable[uiVidEncId])
        {
            UINT32      x2, y2, uiYAddrOffset2, uiUAddrOffset2;
            IMG_BUF     SrcImg2, DstImg2;

            x2 = g_MovieWaterInfo[uiVidEncId].uiXPos;
            y2 = g_MovieWaterInfo[uiVidEncId].uiYPos;

            uiYAddrOffset2 = lineY * y2 + x2;
            if (GxImgFormat == GX_IMAGE_PIXEL_FMT_YUV420_PACKED)
            {
                uiUAddrOffset2 = lineY * y2 / 2 + x2;
            }
            else
            {
                uiUAddrOffset2 = lineY * y2 + x2;
            }

            // Init source (watermark) buffer
            uiLineOffset[0] = g_MovieWaterInfo[uiVidEncId].uiWaterLogoYLineOffset;
            if (GxImgFormat == GX_IMAGE_PIXEL_FMT_YUV420_PACKED)
            {
                uiLineOffset[1] = g_MovieWaterInfo[uiVidEncId].uiWaterLogoYLineOffset * 2; // *2 for YUV422 => YUV420
            }
            else // GX_IMAGE_PIXEL_FMT_YUV422_PACKED
            {
                uiLineOffset[1] = g_MovieWaterInfo[uiVidEncId].uiWaterLogoYLineOffset;
            }
            uiPxlAddr[0]    = g_MovieWaterInfo[uiVidEncId].uiWaterLogoYAddr;
            uiPxlAddr[1]    = g_MovieWaterInfo[uiVidEncId].uiWaterLogoUVAddr;

            GxImg_InitBufEx(&SrcImg2,
                            g_MovieWaterInfo[uiVidEncId].uiWidth,
                            g_MovieWaterInfo[uiVidEncId].uiHeight,
                            GxImgFormat,
                            uiLineOffset,
                            uiPxlAddr);

            // Init destination (image) buffer
            uiLineOffset[0] = lineY;
            uiLineOffset[1] = lineY;
            uiPxlAddr[0]    = yAddr  + uiYAddrOffset2;
            uiPxlAddr[1]    = cbAddr + uiUAddrOffset2;

            GxImg_InitBufEx(&DstImg2,
                            imgWidth,
                            240, // don't care, but should be > 2
                            GxImgFormat,
                            uiLineOffset,
                            uiPxlAddr);

            if (g_MovieWaterInfo[uiVidEncId].uiWaterLogoColorKey == WATERLOGO_FILL_COLOR)
                GxImg_CopyData(&SrcImg2, REGION_MATCH_IMG, &DstImg2, REGION_MATCH_IMG);
            else
                GxImg_CopyColorkeyData(&DstImg2, REGION_MATCH_IMG, &SrcImg2, REGION_MATCH_IMG, g_MovieWaterInfo[uiVidEncId].uiWaterLogoColorKey, FALSE);
        }
    }
    #endif

    //#NT#2016/06/08#Lincy Lin -begin
    //#NT#Implement generic OSD and video drawing mechanism for ALG function
    {
        UINT32   imgHeight;
        // Init destination (image) buffer
        uiLineOffset[0] = lineY;
        uiLineOffset[1] = lineY;
        uiPxlAddr[0]    = yAddr;
        uiPxlAddr[1]    = cbAddr;
        imgHeight       = (cbAddr-yAddr)/lineY;

        GxImg_InitBufEx(&DstImg,
                        imgWidth,
                        imgHeight,
                        GxImgFormat,
                        uiLineOffset,
                        uiPxlAddr);
        MovieStamp_DrawALG(uiVidEncId,&DstImg);
    }
    //#NT#2016/06/08#Lincy Lin -end
    #if (CHECK_STAMP_TIME == ENABLE)
    //DBG_DUMP("time3: %d us\r\n", Perf_GetCurrent() - uiTime);
    {
        static UINT32 k = 0;
        static UINT32 uiTime3 = 0;

        uiTimeCount = Perf_GetCurrent() - uiTime;
        uiTime3 += uiTimeCount;
        k++;
        if (k == 10)
        {
            DBG_DUMP("^Gtime3: %d us\r\n", uiTime3/10);
            uiTime3 = 0;
            k = 0;
        }
    }
    //Perf_Close();
    #endif
}

//-------------------------------------------------------------------------------------------------
// set stamp data to IME (for direct path)

// max. 16 colors for index color
#define STAMP_IDX_COLOR_MAX     16

static UINT32 g_ColorLutY[VIDEO_IN_MAX][STAMP_IDX_COLOR_MAX] = {0};
static UINT32 g_ColorLutU[VIDEO_IN_MAX][STAMP_IDX_COLOR_MAX] = {0};
static UINT32 g_ColorLutV[VIDEO_IN_MAX][STAMP_IDX_COLOR_MAX] = {0};

void MovieStamp_InitImeStamp(UINT32 uiVidEncId)
{
    // Stamp background color
    if ((g_uiMovieStampSetup[uiVidEncId] & STAMP_BG_FORMAT_MASK) == STAMP_BG_TRANSPARENT)
    {
        g_ColorLutY[uiVidEncId][COLOR_ID_BG] = COLOR_CK_Y;
        g_ColorLutU[uiVidEncId][COLOR_ID_BG] = COLOR_CK_U;
        g_ColorLutV[uiVidEncId][COLOR_ID_BG] = COLOR_CK_V;
    }
    else
    {
        g_ColorLutY[uiVidEncId][COLOR_ID_BG] = g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_BG].ucY;
        g_ColorLutU[uiVidEncId][COLOR_ID_BG] = g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_BG].ucU;
        g_ColorLutV[uiVidEncId][COLOR_ID_BG] = g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_BG].ucV;
    }

    // Stamp frame color
    g_ColorLutY[uiVidEncId][COLOR_ID_FR] = g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_FR].ucY;
    g_ColorLutU[uiVidEncId][COLOR_ID_FR] = g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_FR].ucU;
    g_ColorLutV[uiVidEncId][COLOR_ID_FR] = g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_FR].ucV;

    // Stamp foreground color (text body)
    g_ColorLutY[uiVidEncId][COLOR_ID_FG] = g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_FG].ucY;
    g_ColorLutU[uiVidEncId][COLOR_ID_FG] = g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_FG].ucU;
    g_ColorLutV[uiVidEncId][COLOR_ID_FG] = g_MovieStampInfo[uiVidEncId].Color[COLOR_ID_FG].ucV;
}

void MovieStamp_SetImeStamp(UINT32 uiVidEncId, IMESTAMP_INFO *pImeStamp)
{
    // IME set0: support YUYV, for waterlogo
    // IME set1~3: support color index, for general stamp data

    // just enable/disable set1 temporarily
    if ((g_uiMovieStampSetup[uiVidEncId] & STAMP_SWITCH_MASK) == STAMP_ON)
    {
        pImeStamp->DS_Set1.DSEn = TRUE;
        #if (COLOR_IDX_MODE == COLOR_IDX_4BIT)
        pImeStamp->DS_Set1.DSImgInfo.DsMode             = _4B; // use 4-bit color index
        #else
        pImeStamp->DS_Set1.DSImgInfo.DsMode             = _2B; // use 2-bit color index
        #endif
        pImeStamp->DS_Set1.DSImgInfo.DS_SIZE.uiSizeH    = g_uiMovieStampWidth[uiVidEncId];
        pImeStamp->DS_Set1.DSImgInfo.DS_SIZE.uiSizeV    = g_MovieStampInfo[uiVidEncId].ui32DstHeight;
        pImeStamp->DS_Set1.DSImgInfo.DsBlendPos.x       = g_MovieStampPos[uiVidEncId].uiX;
        pImeStamp->DS_Set1.DSImgInfo.DsBlendPos.y       = g_MovieStampPos[uiVidEncId].uiY;
        pImeStamp->DS_Set1.DSImgInfo.uiDSLineoffset     = STAMP_WIDTH_MAX; // use fixed stamp buffer size temporarily
        pImeStamp->DS_Set1.DSImgInfo.uiDSAddress        = g_uiMovieStampYAddr[uiVidEncId][g_uiMovieStampActBuf];
        pImeStamp->DS_Set1.DSIqInfo.uiDsColorKY         = COLOR_CK_Y;
        pImeStamp->DS_Set1.DSIqInfo.uiDsColorKU         = COLOR_CK_U;
        pImeStamp->DS_Set1.DSIqInfo.uiDsColorKV         = COLOR_CK_V;
        pImeStamp->DS_Set1.DSIqInfo.uiBlendWt           = 0x0F; // 4-bit precision
        pImeStamp->ColorLutYAddr                        = (UINT32)&g_ColorLutY[uiVidEncId][0];
        pImeStamp->ColorLutUAddr                        = (UINT32)&g_ColorLutU[uiVidEncId][0];
        pImeStamp->ColorLutVAddr                        = (UINT32)&g_ColorLutV[uiVidEncId][0];

        #if defined (WATERLOGO_FUNCTION) && (WATERLOGO_FUNCTION == ENABLE)
        if (g_bWaterLogoEnable[uiVidEncId])
        {
            pImeStamp->DS_Set0.DSEn = TRUE;
            pImeStamp->DS_Set0.DSImgInfo.DS_SIZE.uiSizeH    = g_MovieWaterInfo[uiVidEncId].uiWidth;
            pImeStamp->DS_Set0.DSImgInfo.DS_SIZE.uiSizeV    = g_MovieWaterInfo[uiVidEncId].uiHeight;
            pImeStamp->DS_Set0.DSImgInfo.DsBlendPos.x       = g_MovieWaterInfo[uiVidEncId].uiXPos;
            pImeStamp->DS_Set0.DSImgInfo.DsBlendPos.y       = g_MovieWaterInfo[uiVidEncId].uiYPos;
            pImeStamp->DS_Set0.DSImgInfo.uiDSLineoffset     = g_MovieWaterInfo[uiVidEncId].uiWaterLogoYLineOffset*2;
            pImeStamp->DS_Set0.DSImgInfo.uiDSAddress        = g_MovieWaterInfo[uiVidEncId].uiWaterLogoYAddr;
            pImeStamp->DS_Set0.DSIqInfo.uiDsColorKY         = g_MovieWaterInfo[uiVidEncId].uiWaterLogoColorKey & 0xFF;
            pImeStamp->DS_Set0.DSIqInfo.uiDsColorKU         = (g_MovieWaterInfo[uiVidEncId].uiWaterLogoColorKey & 0xFF00) >> 8;
            pImeStamp->DS_Set0.DSIqInfo.uiDsColorKV         = (g_MovieWaterInfo[uiVidEncId].uiWaterLogoColorKey & 0xFF0000) >> 16;
            pImeStamp->DS_Set0.DSIqInfo.uiBlendWt           = 0x0F; // 4-bit precision
        }
        #endif
    }
    else
    {
        pImeStamp->DS_Set1.DSEn = FALSE;
    }

    //#NT#2016/06/28#KCHong -begin
    //#NT#DbgLog in GPS section
    #if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
    if (uiVidEncId == 0)
    {
        mp4log_IncFrmCnt(1);
    }
    #endif
    //#NT#2016/06/28#KCHong -end
}
#endif // IPCAM_FUNC && !MOVIE_AE_LOG


//#NT#2016/06/08#Lincy Lin -begin
//#NT#Implement generic OSD and video drawing mechanism for ALG function

void MovieStamp_DrawLineUV(INT32 x1, INT32 y1, INT32 x2, INT32 y2, UINT32 DrawColor, PIMG_BUF pDstImg)
{
    INT32 x, y, x2x1diff, y2y1diff;
    UINT32 LineOff_UV;
    UINT16 CbCr;
    UINT32 UVAddr;

    if( x1 >= (INT32)pDstImg->Width ||
        y1 >= (INT32)pDstImg->Height ||
        x2 >= (INT32)pDstImg->Width ||
        y2 >= (INT32)pDstImg->Height)
    {
        return;
    }

    if(pDstImg->PxlFmt != GX_IMAGE_PIXEL_FMT_YUV420_PACKED)
    {
        return;
    }

    UVAddr = pDstImg->PxlAddr[1];
    LineOff_UV = pDstImg->LineOffset[1];
    CbCr = COLOR_YUVD_GET_U(DrawColor) | (COLOR_YUVD_GET_V(DrawColor) << 8);

    if(y1 == y2)
    {
        x1 &= ~0x00000001; //address U must be even (UV packed)
        x2 &= ~0x00000001; //address U must be even (UV packed)
        y = y1;
        for (x = min(x1, x2); x < max(x1, x2); x += 2)
        {
            *(UINT16 *)(UVAddr + y/2*LineOff_UV + x) = CbCr;
        }
    }
    else if(x1 == x2)
    {
        x = x1 & ~0x00000001;
        for (y = min(y1, y2); y < max(y1, y2); y++)
        {
            *(UINT16 *)(UVAddr + y/2*LineOff_UV + x) = CbCr;
        }
    }
    else
    {
        if(y1 > y2)
        {
            swap(x1, x2);
            swap(y1, y2);
        }

        x2x1diff = x2-x1;
        y2y1diff = y2-y1;

        if(abs(x2x1diff) > abs(y2y1diff))
        {
            x1 &= ~0x00000001; //address U must be even (UV packed)
            x2 &= ~0x00000001; //address U must be even (UV packed)
            for (x = min(x1, x2); x < max(x1, x2); x += 2)
            {
                y = y1 + y2y1diff*(x-x1)/x2x1diff;
                *(UINT16 *)(UVAddr + y/2*LineOff_UV + x) = CbCr;
            }
        }
        else
        {
            for(y = y1; y < y2; y++)
            {
                x = x1 + x2x1diff*(y-y1)/y2y1diff;
                x &= ~0x00000001; //address U must be even (UV packed)
                *(UINT16 *)(UVAddr + y/2*LineOff_UV + x) = CbCr;
            }
        }
    }
}

void MovieStamp_DrawPointUV(INT32 x, INT32 y, UINT32 DrawColor, PIMG_BUF pDstImg)
{
    UINT32 LineOff_UV;
    UINT32 UVAddr;
    UINT16 CbCr;

    if( x >= (INT32)pDstImg->Width ||
        y >= (INT32)pDstImg->Height)
    {
        return;
    }

    if(pDstImg->PxlFmt != GX_IMAGE_PIXEL_FMT_YUV420_PACKED)
    {
        return;
    }

    UVAddr = pDstImg->PxlAddr[1];
    LineOff_UV = pDstImg->LineOffset[1];
    CbCr = COLOR_YUVD_GET_U(DrawColor) | (COLOR_YUVD_GET_V(DrawColor) << 8);

    x &= ~0x00000001; //address U must be even (UV packed)
    *(UINT16 *)(UVAddr + y/2*LineOff_UV + x) = CbCr;
}

void MovieStamp_DrawRectUV(IRECT *pRect, UINT32 DrawColor, PIMG_BUF pDstImg)
{
    MovieStamp_DrawLineUV(  pRect->x, pRect->y, pRect->x + pRect->w, pRect->y, DrawColor, pDstImg);
    MovieStamp_DrawLineUV(  pRect->x, pRect->y, pRect->x, pRect->y + pRect->h, DrawColor, pDstImg);
    MovieStamp_DrawLineUV(  pRect->x + pRect->w, pRect->y, pRect->x + pRect->w, pRect->y + pRect->h, DrawColor, pDstImg);
    MovieStamp_DrawLineUV(  pRect->x, pRect->y + pRect->h, pRect->x + pRect->w, pRect->y + pRect->h, DrawColor, pDstImg);
}

void MovieStamp_DrawRect(PIMG_BUF pDstImg, IRECT*  DrawRect, UINT32 drawColor, UINT32 border)
{
    IRECT  LineRect;

    if (border <2)
        border = 2;

    LineRect.x = DrawRect->x;
    LineRect.y = DrawRect->y;
    LineRect.w = DrawRect->w;
    LineRect.h = border;
    GxImg_FillData(pDstImg, &LineRect, drawColor);

    LineRect.x = DrawRect->x;
    LineRect.y = DrawRect->y + DrawRect->h - border;
    LineRect.w = DrawRect->w;
    LineRect.h = border;
    GxImg_FillData(pDstImg, &LineRect, drawColor);

    LineRect.x = DrawRect->x;
    LineRect.y = DrawRect->y;
    LineRect.w = border;
    LineRect.h = DrawRect->h;
    GxImg_FillData(pDstImg, &LineRect, drawColor);

    LineRect.x = DrawRect->x + DrawRect->w - border;
    LineRect.y = DrawRect->y;
    LineRect.w = border;
    LineRect.h = DrawRect->h;
    GxImg_FillData(pDstImg, &LineRect, drawColor);
}


#if MOVIE_FD_FUNC_
void MovieStamp_DrawFD(PIMG_BUF pDstImg)
{

    #if MOVIE_FD_DRAW_VIDEO

    UINT32            Fdnumber,i;
    static FD_FACE    FdInfo[MAX_FDNUM];
    IRECT             DrawRect;
    UINT32            border = 4; // border can not smaller than 2
    UINT32            drawColor = COLOR_YUV_WHITE;
    URECT             DispCord;

    if (pDstImg->Width < 1280)
    {
        border = 2;
    }
    DispCord.x = 0;
    DispCord.y = 0;
    DispCord.w = pDstImg->Width;
    DispCord.h = pDstImg->Height;
    Fdnumber = FD_GetRsltFace (FdInfo,&DispCord);
    if (Fdnumber > MAX_FDNUM)
    {
        Fdnumber = MAX_FDNUM;
    }
    if (Fdnumber)
    {
        for (i=0;i<Fdnumber;i++)
        {
            DrawRect.x = FdInfo[i].x;
            DrawRect.y = FdInfo[i].y;
            DrawRect.w = FdInfo[i].w;
            DrawRect.h = FdInfo[i].h;
            MovieStamp_DrawRect(pDstImg,&DrawRect,drawColor,border);
        }
    }
    #endif
}
#endif

#if MOVIE_OT_FUNC
void MovieStamp_DrawOT(PIMG_BUF pDstImg)
{
    #if MOVIE_OT_DRAW_VIDEO
    UINT32            OTnumber;
    URECT             OTInfo;
    IRECT             OTRect;
    UINT32            border = 4; // border can not smaller than 2
    UINT32            drawColor = COLOR_YUV_WHITE;
    URECT             DispCord;

    if (pDstImg->Width < 1280)
    {
        border = 2;
    }
    DispCord.x = 0;
    DispCord.y = 0;
    DispCord.w = pDstImg->Width;
    DispCord.h = pDstImg->Height;
    OTnumber = OT_GetTarget(&OTInfo, &DispCord);
    if (OTnumber)
    {
        OTRect.x = OTInfo.x;
        OTRect.y = OTInfo.y;
        OTRect.w = OTInfo.w;
        OTRect.h = OTInfo.h;
        MovieStamp_DrawRect(pDstImg,&OTRect,drawColor,border);
    }
    #endif
}
#endif

#if MOVIE_DDD_FUNC
void MovieStamp_DrawDDD(UINT32 uiVidEncId, PIMG_BUF pDstImg)
{
    #if MOVIE_DDD_DRAW_VIDEO
    //draw variables
    URECT DispCord;
    UINT32 ColorKey;

    //DDD variables
    DDD_SHAPE Shape_Landmark;
    URECT TrackRect;
    UINT32 Dddnumber;
    INT32 iPoseRay[8];
    UINT32 Ret_DDD;

    //other variables
    UINT32 i;
    UINT32 t1, t2;

    //#NT#2016/09/20#ML Cui -begin
    //#NT# DDD need not to draw on front channel.
    //#NT# It was not worked as wish in PTL668 latest HW.
    #if 1 //(_MODEL_DSC_ == _MODEL_PTL_668_)
    if(uiVidEncId == 0)
        return;
    #endif
    //#NT#2016/09/20#ML Cui -end

    t1 = Perf_GetCurrent();
    //-------------------------
    if(pDstImg->PxlFmt != GX_IMAGE_PIXEL_FMT_YUV420_PACKED)
    {
        DBG_ERR("Not YUV420\r\n");
        return;
    }

    DispCord.x = 0;
    DispCord.y = 0;
    DispCord.w = pDstImg->Width;
    DispCord.h = pDstImg->Height;

    //draw landmark
    Dddnumber = DDD_GetRsltLandmark(&Shape_Landmark, &DispCord);
    if(Dddnumber > 0)
    {
        //landmark: head
        Ret_DDD = DDD_GetFacialState(DDD_FACIAL_TYPE_HEAD);
        if(DDD_HEAD_STATE_NODING == Ret_DDD)
            ColorKey = COLOR_YUV_ORANGE;
        else if(DDD_HEAD_STATE_DISTRIBUTE == Ret_DDD)
            ColorKey = COLOR_YUV_RED;
        else
            ColorKey = COLOR_YUV_GREEN;

        for(i = 0; i < 17; i++)
            MovieStamp_DrawPointUV(Shape_Landmark.pt[i*2], Shape_Landmark.pt[i*2+1], ColorKey, pDstImg);

        //landmark: others
        ColorKey = COLOR_YUV_GREEN;

        for(i = 17; i < 36; i++)
            MovieStamp_DrawPointUV(Shape_Landmark.pt[i*2], Shape_Landmark.pt[i*2+1], ColorKey, pDstImg);

        //landmark: left eye
        if(DDD_GetFacialState(DDD_FACIAL_TYPE_LEFT_EYE) == DDD_EYE_CLOSED)
            ColorKey = COLOR_YUV_BLUE;
        else
            ColorKey = COLOR_YUV_GREEN;

        for(i = 36; i < 42; i++)
            MovieStamp_DrawPointUV(Shape_Landmark.pt[i*2], Shape_Landmark.pt[i*2+1], ColorKey, pDstImg);

        //landmark: right eye
        if(DDD_GetFacialState(DDD_FACIAL_TYPE_RIGHT_EYE) == DDD_EYE_CLOSED)
            ColorKey = COLOR_YUV_BLUE;
        else
            ColorKey = COLOR_YUV_GREEN;

        for(i = 42; i < 48; i++)
            MovieStamp_DrawPointUV(Shape_Landmark.pt[i*2], Shape_Landmark.pt[i*2+1], ColorKey, pDstImg);

        //landmark: mouse
        if(DDD_GetFacialState(DDD_FACIAL_TYPE_MOUTH) == DDD_MOUTH_YAWN)
            ColorKey = COLOR_YUV_CYAN;
        else
            ColorKey = COLOR_YUV_GREEN;

        for(i = 48; i < 68; i++)
            MovieStamp_DrawPointUV(Shape_Landmark.pt[i*2], Shape_Landmark.pt[i*2+1], ColorKey, pDstImg);

        //poseray
        if(DDD_GetRsltPoseRay(iPoseRay, 60, &DispCord))
        {
            MovieStamp_DrawLineUV(iPoseRay[0], iPoseRay[1], iPoseRay[2], iPoseRay[3], COLOR_YUV_GREEN, pDstImg);
            MovieStamp_DrawLineUV(iPoseRay[0], iPoseRay[1], iPoseRay[4], iPoseRay[5], COLOR_YUV_BLUE, pDstImg);
            MovieStamp_DrawLineUV(iPoseRay[0], iPoseRay[1], iPoseRay[6], iPoseRay[7], COLOR_YUV_RED, pDstImg);
        }

        //tracking rect
        if(DDD_GetRsltTrackingRect(&TrackRect, &DispCord))
        {
            MovieStamp_DrawRectUV((IRECT *)&TrackRect, COLOR_YUV_PURPLE, pDstImg);
        }
    }

    //-------------------------
    t2 = Perf_GetCurrent();
    DBG_IND("DrawDDD = %d ms\r\n", (t2-t1)/1000);
    #endif
}
#endif


#if _ADAS_FUNC_
void MovieStamp_DrawADAS(UINT32 uiVidEncId,PIMG_BUF pDstImg)
{
    #if _DRAW_LDWSINFO_ON_VIDEO_
    ADAS_APPS_RESULT_INFO *pAdasRlt = MovieExe_GetAdasRltVideo();
    ADAS_SNG_ROI_INFO *pSnGROI = MovieExe_GetSnGROIVideo();

    if ((uiVidEncId != 0) || (ADAS_GetAppsStatus() == FALSE))
    {
        return;
    }

    // draw lane
    MovieStamp_DrawLineUV(pAdasRlt->LdwsRsltInfo.uiLx1, pAdasRlt->LdwsRsltInfo.uiLy1,
                          pAdasRlt->LdwsRsltInfo.uiLx2, pAdasRlt->LdwsRsltInfo.uiLy2,
                         (pAdasRlt->LdwsRsltInfo.DepartureDir == LDWS_DEPARTURE_LEFT)?COLOR_YUV_RED:COLOR_YUV_GREEN, pDstImg);
    MovieStamp_DrawLineUV(pAdasRlt->LdwsRsltInfo.uiRx1, pAdasRlt->LdwsRsltInfo.uiRy1,
                          pAdasRlt->LdwsRsltInfo.uiRx2, pAdasRlt->LdwsRsltInfo.uiRy2,
                         (pAdasRlt->LdwsRsltInfo.DepartureDir == LDWS_DEPARTURE_RIGHT)?COLOR_YUV_RED:COLOR_YUV_GREEN, pDstImg);

    // draw car underline
    if ((pAdasRlt->FcwsRsltInfo.uiLx != pAdasRlt->FcwsRsltInfo.uiRx) || (pAdasRlt->FcwsRsltInfo.uiLy != pAdasRlt->FcwsRsltInfo.uiRy))
    {
        MovieStamp_DrawLineUV(pAdasRlt->FcwsRsltInfo.uiLx, pAdasRlt->FcwsRsltInfo.uiLy,
                              pAdasRlt->FcwsRsltInfo.uiRx, pAdasRlt->FcwsRsltInfo.uiRy,
                             (pAdasRlt->FcwsRsltInfo.FCWarning == TRUE)?COLOR_YUV_RED:COLOR_YUV_GREEN, pDstImg);
    }

    // draw Auto-VP result
    MovieStamp_DrawLineUV((pAdasRlt->LdwsRsltInfo.AutoVpParms.AutoVPRslt.x - 30), pAdasRlt->LdwsRsltInfo.AutoVpParms.AutoVPRslt.y,
                          (pAdasRlt->LdwsRsltInfo.AutoVpParms.AutoVPRslt.x + 30), pAdasRlt->LdwsRsltInfo.AutoVpParms.AutoVPRslt.y,
                         (pAdasRlt->LdwsRsltInfo.Failure == LDWS_FAILURE_FALSE)?COLOR_YUV_BLUE:COLOR_YUV_RED, pDstImg);
    MovieStamp_DrawLineUV(pAdasRlt->LdwsRsltInfo.AutoVpParms.AutoVPRslt.x, (pAdasRlt->LdwsRsltInfo.AutoVpParms.AutoVPRslt.y - 30),
                          pAdasRlt->LdwsRsltInfo.AutoVpParms.AutoVPRslt.x, (pAdasRlt->LdwsRsltInfo.AutoVpParms.AutoVPRslt.y + 30),
                         (pAdasRlt->LdwsRsltInfo.Failure == LDWS_FAILURE_FALSE)?COLOR_YUV_BLUE:COLOR_YUV_RED, pDstImg);

    // draw SnGROI
    if (pSnGROI)
    {
        MovieStamp_DrawLineUV(pSnGROI->uiP1x, pSnGROI->uiP1y, pSnGROI->uiP2x, pSnGROI->uiP2y,
                             (pAdasRlt->SnGInfo.uiMvValid == STOPNGO_MV_VALID)?COLOR_YUV_GREEN:COLOR_YUV_RED, pDstImg);
        MovieStamp_DrawLineUV(pSnGROI->uiP2x, pSnGROI->uiP2y, pSnGROI->uiP3x, pSnGROI->uiP3y,
                             (pAdasRlt->SnGInfo.uiMvValid == STOPNGO_MV_VALID)?COLOR_YUV_GREEN:COLOR_YUV_RED, pDstImg);
        MovieStamp_DrawLineUV(pSnGROI->uiP3x, pSnGROI->uiP3y, pSnGROI->uiP4x, pSnGROI->uiP4y,
                             (pAdasRlt->SnGInfo.uiMvValid == STOPNGO_MV_VALID)?COLOR_YUV_GREEN:COLOR_YUV_RED, pDstImg);
        MovieStamp_DrawLineUV(pSnGROI->uiP4x, pSnGROI->uiP4y, pSnGROI->uiP1x, pSnGROI->uiP1y,
                             (pAdasRlt->SnGInfo.uiMvValid == STOPNGO_MV_VALID)?COLOR_YUV_GREEN:COLOR_YUV_RED, pDstImg);
    }
    #endif
}
#endif

//#NT#2016/05/23#David Tsai -begin
//#NT# Support tampering detection function
#if MOVIE_TD_FUNC
void MovieStamp_DrawTD(PIMG_BUF pDstImg)
{
    #if MOVIE_TD_DRAW_VIDEO
    UINT32            TDnumber, i;
    URECT             TDInfo[TD_MAX_BLK_NUM];
    IRECT  TDRect;
    UINT32 border = 4; // border can not smaller than 2
    UINT32 drawColor = COLOR_YUV_WHITE;
    URECT  DispCord;

    if (pDstImg->Width < 1280)
    {
        border = 2;
    }
    DispCord.x = 0;
    DispCord.y = 0;
    DispCord.w = pDstImg->Width;
    DispCord.h = pDstImg->Height;

    if ((TDnumber = TDGetTarget(TDInfo, &DispCord)) > 0 && TDGetAlarm() == TRUE)
        drawColor = COLOR_YUV_RED;

    for (i = 0; i < TDnumber; i++)
    {
        TDRect.x = TDInfo[i].x;
        TDRect.y = TDInfo[i].y;
        TDRect.w = TDInfo[i].w;
        TDRect.h = TDInfo[i].h;

        MovieStamp_DrawRect(pDstImg,&TDRect,drawColor,border);
    }
    #endif
}
#endif
//#NT#2016/05/23#David Tsai -end

//#NT#2016/10/14#Yuzhe Bian -begin
//#NT# Support trip-wire detection function
#if MOVIE_TWD_FUNC
void MovieStamp_DrawTWD(PIMG_BUF pDstImg)
{
    #if MOVIE_TWD_DRAW_VIDEO
    UINT32            TWDRECTnumber, TWDPOINTnumber, i;
    URECT             TWDInfo[twd_max_objects_number];
    BOOL              TWDState[twd_max_objects_number];
    UPOINT            TWDLINEInfo[twd_max_nodes_number];
    UPOINT TWDStart, TWDEnd;
    IRECT  TWDRect;
    UINT32 border = 4; // border can not smaller than 2
    UINT32 drawColor = COLOR_YUV_RED;
    URECT  DispCord;

    if (pDstImg->Width < 1280)
    {
        border = 2;
    }
    DispCord.x = 0;
    DispCord.y = 0;
    DispCord.w = pDstImg->Width;
    DispCord.h = pDstImg->Height;

    if ((TWDPOINTnumber = twd_getpointsintripwire(TWDLINEInfo, &DispCord)) > 1)
    {
        for (i = 0; i < TWDPOINTnumber - 1; i++)
        {
            TWDStart.x = TWDLINEInfo[i].x;
            TWDStart.y = TWDLINEInfo[i].y;
            TWDEnd.x = TWDLINEInfo[i+1].x;
            TWDEnd.y = TWDLINEInfo[i+1].y;
            MovieStamp_DrawLineUV(  TWDStart.x, TWDStart.y, TWDEnd.x, TWDEnd.y, drawColor, pDstImg);
        }
    }

    if ((TWDRECTnumber = twd_gettargetswithwarn(TWDInfo, TWDState, &DispCord)) > 0)
    {
        for (i = 0; i < TWDRECTnumber; i++)
        {
            TWDRect.x = TWDInfo[i].x;
            TWDRect.y = TWDInfo[i].y;
            TWDRect.w = TWDInfo[i].w;
            TWDRect.h = TWDInfo[i].h;

            if(TWDState[i] == FALSE)
                drawColor = COLOR_YUV_WHITE;
            else
                drawColor = COLOR_YUV_RED;

            MovieStamp_DrawRect(pDstImg,&TWDRect,drawColor,border);
        }
    }
    #endif
}
#endif
//#NT#2016/10/14#Yuzhe Bian -end

//#NT#2016/10/14#Yuzhe Bian -begin
//#NT# Support trip-zone detection function
#if MOVIE_TZD_FUNC
void MovieStamp_DrawTZD(PIMG_BUF pDstImg)
{
    #if MOVIE_TZD_DRAW_VIDEO
    UINT32            TZDRECTnumber, TZDPOINTnumber, i;
    URECT             TZDInfo[tzd_max_objects_number];
    BOOL              TZDState[tzd_max_objects_number];
    UPOINT            TZDLINEInfo[tzd_max_nodes_number];
    UPOINT TZDStart, TZDEnd;
    IRECT  TZDRect;
    UINT32 border = 4; // border can not smaller than 2
    UINT32 drawColor = COLOR_YUV_RED;
    URECT  DispCord;

    if (pDstImg->Width < 1280)
    {
        border = 2;
    }
    DispCord.x = 0;
    DispCord.y = 0;
    DispCord.w = pDstImg->Width;
    DispCord.h = pDstImg->Height;

    if ((TZDPOINTnumber = tzd_getpointsintripzone(TZDLINEInfo, &DispCord)) > 1)
    {
        for (i = 0; i < TZDPOINTnumber - 1; i++)
        {
            TZDStart.x = TZDLINEInfo[i].x;
            TZDStart.y = TZDLINEInfo[i].y;
            TZDEnd.x = TZDLINEInfo[i+1].x;
            TZDEnd.y = TZDLINEInfo[i+1].y;
            MovieStamp_DrawLineUV(  TZDStart.x, TZDStart.y, TZDEnd.x, TZDEnd.y, drawColor, pDstImg);
        }
    }

    if ((TZDRECTnumber = tzd_gettargetswithwarn(TZDInfo, TZDState, &DispCord)) > 0)
    {
        for (i = 0; i < TZDRECTnumber; i++)
        {
            TZDRect.x = TZDInfo[i].x;
            TZDRect.y = TZDInfo[i].y;
            TZDRect.w = TZDInfo[i].w;
            TZDRect.h = TZDInfo[i].h;

            if(TZDState[i] == FALSE)
                drawColor = COLOR_YUV_WHITE;
            else
                drawColor = COLOR_YUV_RED;

            MovieStamp_DrawRect(pDstImg,&TZDRect,drawColor,border);
        }
    }
    #endif
}
#endif
//#NT#2016/10/14#Yuzhe Bian -end

//#NT#2016/06/16#CCChao -begin
//#NT# Support traffic sign recognition function
#if MOVIE_TSR_FUNC_
void MovieStamp_DrawTSR(PIMG_BUF pDstImg)
{
    #if MOVIE_TSR_DRAW_VIDEO
    UINT32              TSRnumber, i;
    static TRAFFIC_SIGN TSRInfo[TSD_FDE_MAX_TS_NUM];
    //UINT32              border = 2; // border can not smaller than 2
    UINT32              drawColor = COLOR_YUV_WHITE;
    URECT               DispCord;

    #if 0
    TSRnumber = TSR_GetRsltTS (TSRInfo,&DispCord);
    drawColor = COLOR_YUV_BLACK;
    for (i=0;i<TSRnumber;i++)
    {
        IRECT DrawRect;
        //if( (TSRInfo[i].uiStx+TSRInfo[i].uiWidth) < (pDstImg->Width>>1) )
        //{ continue; }

        DrawRect.x = TSRInfo[i].uiStx;
        DrawRect.y = TSRInfo[i].uiSty;
        DrawRect.w = TSRInfo[i].uiWidth;
        DrawRect.h = TSRInfo[i].uiHeight;
        MovieStamp_DrawRect(pDstImg,&DrawRect,drawColor,border);
    }
    #else
    TSRnumber = NvtCv_TSDDSP_GetRsltTS(TSRInfo,&DispCord);
    drawColor = COLOR_YUV_WHITE;
    for (i=0;i<TSRnumber;i++)
    {
        #if 0
        IRECT DrawRect;
        DrawRect.x = TSRInfo[i].uiStx;
        DrawRect.y = TSRInfo[i].uiSty;
        DrawRect.w = TSRInfo[i].uiWidth;
        DrawRect.h = TSRInfo[i].uiHeight;
        MovieStamp_DrawRect(pDstImg,&DrawRect,drawColor,border);
        #else
        MovieStamp_DrawLineUV(TSRInfo[i].uiStx, TSRInfo[i].uiSty + TSRInfo[i].uiHeight,
                                TSRInfo[i].uiStx + TSRInfo[i].uiWidth, TSRInfo[i].uiSty + TSRInfo[i].uiHeight,
                                COLOR_YUV_GREEN, pDstImg);
        #endif
    }
    #endif
    #endif
}
#endif
//#NT#2016/06/16#CCChao -end




//#NT#2016/12/07#Charlie Chang -begin
//#NT# get/set moviestatmp config

void MovieStamp_SetConfig(UINT32 type, UINT32 value){


    switch(type){
        case MOVIESTAMP_FTG_DRAW_DELAY_COUNT:
            g_uiFtgDrawDelayCount= value;
            break;
        case MOVIESTAMP_FTG_RLST_TYPE:

            g_uiFtgRlstType = value;
            break;
		case MOVIESTAMP_PIP_ENABLE:
			g_bPipEnable = value;
			break;
		case MOVIESTAMP_BLENDING:

			g_MovieStampBlendCfg.bEnable = ((MOVIESTAMP_BLENDING_CFG *)value)->bEnable;
			g_MovieStampBlendCfg.uiAlpha = ((MOVIESTAMP_BLENDING_CFG *)value)->uiAlpha;
			g_MovieStampBlendCfg.uiSrcVideoID = ((MOVIESTAMP_BLENDING_CFG *)value)->uiSrcVideoID;
			g_MovieStampBlendCfg.uiDesVideoID = ((MOVIESTAMP_BLENDING_CFG *)value)->uiDesVideoID;
			break;
        default:
            DBG_ERR("type(%d) error\r\n",type);
    }
}

UINT32 MovieStamp_GetConfig(UINT32 type){

    switch(type){
        case MOVIESTAMP_FTG_DRAW_DELAY_COUNT:
            return g_uiFtgDrawDelayCount;

        case MOVIESTAMP_FTG_RLST_TYPE:
            return g_uiFtgRlstType;

		case MOVIESTAMP_PIP_ENABLE:
			return g_bPipEnable;
		case MOVIESTAMP_BLENDING:
			return (UINT32)&g_MovieStampBlendCfg;

        default:
            DBG_ERR("type(%d) error\r\n",type);

    }
    return 0;
}


//#NT#2016/12/07#Charlie Chang -end


//#NT#2016/10/17#Bin Xiao -begin
//#NT# Support Face Tracking Grading(FTG)
#if MOVIE_FTG_FUNC
void MovieStamp_DrawFTG(PIMG_BUF pDstImg, UINT32 uiVidEncId)
{
    #if MOVIE_FTG_DRAW_VIDEO

#if 1

    UINT32              i,id;
    static  ftg_face    FTGInfo[3][ftg_num_max];
    static UINT32       FTGnumber[3] ={0};
    IRECT               DrawRect;
    UINT32              border = 4; // border can not smaller than 2
    UINT32              drawColor = COLOR_YUV_WHITE;
    URECT               DispCord;

    static UINT32       draw_count_index[3] = {0};
    if (Movie_FTGGetLockSts())
        return;
	//#NT#2017/01/09#Charlie Chang -begin
    //#NT# only support stream 0 for draw rectangle
	if(uiVidEncId != 0){
		return ;
	}
	//#NT#2017/01/09#Charlie Chang -end
    if(draw_count_index[uiVidEncId] < g_uiFtgDrawDelayCount){
        draw_count_index[uiVidEncId] ++;
    }
    else{
        draw_count_index[uiVidEncId] = 0;
    }

    if (pDstImg->Width < 1280)
    {
        border = 2;
    }
    DispCord.x = 0;
    DispCord.y = 0;
    DispCord.w = pDstImg->Width;
    DispCord.h = pDstImg->Height;

    if(draw_count_index[uiVidEncId] == 0){////update information
		#if 1
		FTGnumber[uiVidEncId] = ftg_getrsltface2(FTGInfo[uiVidEncId], &DispCord, ftg_rlst_only_loc);
		#else
		static UINT32 		id0_width =0;
		static UINT32 		id0_height =0;
		////update stream 0, other stream map stream0
		if(uiVidEncId == 0){
			id0_width = DispCord.w;
			id0_height= DispCord.h;
        	FTGnumber[uiVidEncId] = ftg_getrsltface2(FTGInfo[uiVidEncId], &DispCord, g_uiFtgRlstType);
		}
		else{
			FTGnumber[uiVidEncId] = FTGnumber[0];
			float ratio_w =0;
			float ratio_h =0;
			if(id0_width != 0){
				ratio_w =(float)DispCord.w/(float)id0_width;
				ratio_h = (float)DispCord.h/(float)id0_height;
			}

			for (i=0;i<FTGnumber[uiVidEncId];i++)
    		{
				FTGInfo[uiVidEncId][i].stx = (float)FTGInfo[0][i].stx*ratio_w;
				FTGInfo[uiVidEncId][i].sty = (float)FTGInfo[0][i].sty*ratio_h;
				FTGInfo[uiVidEncId][i].width = (float)FTGInfo[0][i].width*ratio_w;
				FTGInfo[uiVidEncId][i].height = (float)FTGInfo[0][i].height*ratio_h;
				DBG_DUMP("charlie u=%d x=%d y=%d w=%d h=%d\r\n",i,
					FTGInfo[uiVidEncId][i].stx,
					FTGInfo[uiVidEncId][i].sty,
					FTGInfo[uiVidEncId][i].width,
					FTGInfo[uiVidEncId][i].height
					);
			}
		}
		#endif
    }
    for (i=0;i<FTGnumber[uiVidEncId];i++)
    {
        DrawRect.x = FTGInfo[uiVidEncId][i].stx;
        DrawRect.y = FTGInfo[uiVidEncId][i].sty;
        DrawRect.w = FTGInfo[uiVidEncId][i].width;
        DrawRect.h = FTGInfo[uiVidEncId][i].height;
        MovieStamp_DrawRect(pDstImg,&DrawRect,drawColor,border);

        id = FTGInfo[uiVidEncId][i].id;
    }
    #endif
    #endif
}
#endif
//#NT#2016/10/17#Bin Xiao -end
void MovieStamp_DrawALG(UINT32 uiVidEncId,PIMG_BUF pDstImg)
{
    #if (MOVIE_FD_FUNC_== ENABLE) && (MOVIE_DDD_FUNC == DISABLE)
    MovieStamp_DrawFD(pDstImg);
    #endif
    #if (MOVIE_OT_FUNC == ENABLE)
    MovieStamp_DrawOT(pDstImg);
    #endif
    #if (MOVIE_DDD_FUNC == ENABLE)
    MovieStamp_DrawDDD(uiVidEncId, pDstImg);
    #endif
    #if (MOVIE_TD_FUNC == ENABLE)
    MovieStamp_DrawTD(pDstImg);
    #endif
    #if (MOVIE_TWD_FUNC == ENABLE)
    MovieStamp_DrawTWD(pDstImg);
    #endif
    #if (MOVIE_TZD_FUNC == ENABLE)
    MovieStamp_DrawTZD(pDstImg);
    #endif
    #if (_ADAS_FUNC_ == ENABLE)
    MovieStamp_DrawADAS(uiVidEncId,pDstImg);
    #endif
    #if (MOVIE_TSR_FUNC_ == ENABLE)
    //MovieStamp_DrawTSR(pDstImg);
    #endif
    #if (MOVIE_FTG_FUNC == ENABLE)
    MovieStamp_DrawFTG(pDstImg, uiVidEncId);
    #endif
}

//#NT#2016/06/08#Lincy Lin -end
