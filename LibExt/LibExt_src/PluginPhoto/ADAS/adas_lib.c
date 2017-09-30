#include    "Type.h"
#include    "GxImage.h"
#include    "adas_int.h"
#include    "adas_lib.h"
#include    "ldws_lib.h"
#include    "fcws_lib.h"
#include    "IPL_Utility.h"
#include    "IPL_Cmd.h"
#include    "HwMem.h"
#include    "FileSysTsk.h"
#include    "ime_lib.h"
#include    "Delay.h"
#include    "IPL_Hal_IME_Info.h"
#include    "IPL_Hal.h"
#include    "IPL_Ctrl_flow_Int.h"
#include    "IPL_Ctrl_Int.h"
#include    "IPL_Hal_Op_Int.h"
#include    "autoVP_alg.h"
#include    "autoVP_Parms.h"
#include    "stdio.h"
#include    "GPS.h"
#include    "SnG_lib.h"
#include    "dis_lib.h"
#include    "dis_alg.h"
#include    "rtc.h"
#include    "af_api.h"
#include    "ise_lib.h"
#include    "dma.h"


#define ADAS_STAMPLOG       1

#define THIS_DBGLVL         2 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          ADAS
#define __DBGLVL__          THIS_DBGLVL
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

#define LDWS_ROI_SIZE_X                 720
#define LDWS_ROI_SIZE_Y                 480
#define ADAS_Y_WIDTH                    1920
#define ADAS_Y_HEIGHT                   1080
#define ADAS_BUF_SIZE                   LDWS_ROI_SIZE_X*LDWS_ROI_SIZE_Y*2  // ADAS working buffer size
#define ADAS_Y_SIZE                     ADAS_Y_WIDTH*ADAS_Y_HEIGHT
#define INPPUT_IMGBUG_SIZE              LDWS_ROI_SIZE_X*LDWS_ROI_SIZE_Y
#define LDWS_WORKINGBUG_SIZE            50*LDWS_ROI_SIZE_X
#define FCWS_EDGEMAP_SIZE               480*480
#define FCWS_HIST_SIZE                  4*256
#define IMERegBase                      0xC0C40000
#define GET_32BitsValue_ADAS(addr)      (*(volatile UINT32*)(addr))
#define ADAS_ABS(a)                     ((a)>=0 ? a:-(a))
#define SPRINTF_BUFFER_SIZE             200

// SnG
#define SpecificVal                     1024


// ADAS global variables
IPL_IME_PATH_INFO       g_ImePath4Info;
static UINT32           g_uiAdasWorkBufAddr, g_uiLdwsWorkBufAddr, g_uiFcwsWorkBufAddr, g_uiFcwsUsedBufAddr;
UINT32                  g_uiAdasSrcFrameAddr, g_uiAdasEdgeAddr, g_uiAdasHistAddr, g_uiAdasFrameWidth, g_uiAdasFrameHeight, g_uiAdasFrameLineOfs;
ADAS_CBMSG_FP           g_AdasCbFP = NULL;
IPL_IME_BUF_ADDR        IPL_IME_CurInfo1;
IPL_IME_INFOR           IPL_IME_CurInfo2;
ADAS_RESULT_INFO        g_AdasRslt;
UINT32                  FLG_ID_ADAS = 0;            //#NT#2016/02/19#KCHong#[0094606]

// ADAS Control variables
BOOL                    g_AdasStop = TRUE;
BOOL                    g_firstSetCropInfo = TRUE;
//BOOL                    g_AdasfirstSetdateInfo = TRUE;
BOOL                    g_firstSetCropStartPos = TRUE;
UINT32                  g_uiAdasRate = 1;
UINT32                  g_uiAdasCount = 0;
BOOL                    g_AdasLock = FALSE;
//static UINT32           uiTimeDistCuv[1000] = {0};

// LDWS global variables
BOOL                    g_LdwsEnable = FALSE;
LDWS_INIT_PARMS         g_LdwsSysParamter;
LDWS_RESULT_INFO        g_LdwsRslt;
UINT32                  g_uiLdwsProcessTime = 0;
static LDWS_DEBUG_INFO  LdwsDebugInfo;
static APPS_FCWS_ALG_INFO FcwsAlgDebugInfo;
static LDWS_SRCDATA_INFO LdwsSrcData;
static FCWS_SRCDATA_INFO FcwsSrcData;
static LDWS_INFOFORFCWS LdInfo2Fcws;

// FCWS global variables
BOOL                    g_FcwsEnable = FALSE;;
FCWS_INIT_PARMS         g_FcwsSysParamter;
IME_STL_INFO            g_StlParam;
//FCWS_DEBUG_INFO         g_FcDebugInfo;
APPS_FCWS_DEBUG_INFO    g_FcDebugInfo;
FCWS_RESULT_INFO        g_FcwsRslt;
UINT32                  g_uiFcwsProcessTime = 0;

// IME
static UINT32           g_setImeIniParmsflag = 1;
IME_GET_STL_INFO        g_HistInfo, g_HorHistInfo, g_VerHistInfo;

// MovieStamp
UINT32                  g_uiFcDist = 0;
UINT32                  g_uiFcKelDist = 0;
UINT32                  g_uiFcKelEn = 0;
UINT32                  g_uiFCW_E = 0;
UINT32                  g_Failure = 0;
UINT32                  g_uideltaS = 0;
UINT32                  g_uiLastDist = 0;
UINT32                  g_uiSpeed = 0;
UINT32                  g_uiDeltaTime = 0;


// Auto-VP
AUTO_VP_RESULT_INFO     g_AutoVPRslt;
AUTO_VP_SRCIMG_INFO     g_AutoVPSrc;
UINT32                  g_AutoVPUpdateRate = 100;
UINT32                  g_AutoVPSub = 20;
UINT32                  g_uiP1FrameWidth = 0, g_uiP1FrameHeight = 0;

// LDWS warning variables
UINT32                   g_WarngEn = FALSE;
UINT32                   g_WarngCnt = 0;
UINT32                   g_LeftWarngEn = FALSE;
UINT32                   g_RightWarngEn = FALSE;
UINT32                   g_LeftWarngCnt = 0, g_RightWarngCnt = 0;
UINT32                   g_WarningLDTimes = 100;
UINT32                   g_WarningVideoCnt = 30;
UINT32                   g_LDwargdebounce = 3000000; // 3s


// FCWS warning variables
UINT32                   g_FCWarngEn = FALSE;
UINT32                   g_FCWarngCnt = 0;
UINT32                   g_WarningFCTimes = 100;
UINT32                   g_FCWarningVideoCnt = 30;
UINT32                   g_FCwargdebounce = 5000000;


// project parameters setting
UINT32                   g_uiFsizeVal = 100;
UINT32                   g_uiLeftSensitivity = 15;
UINT32                   g_uiRightSensitivity = 15;
UINT32                   g_uiFcwsWarningDist = 15;


// Time array
UINT32                   g_uiTimeArray[10] = {0};

// GPS Speed
//FLOAT                    g_CurSpeed = 50;
UINT32                   g_uiSpeedLimit = 0;

// System time
//RTC_TIME                 g_time;
//RTC_DATE                 g_date;
//UINT32                   g_CurTime;


UINT32                   g_FcStartTime = 0, g_FcEndTime = 0, g_FcdeltaTime = 0;
UINT32                   g_ISECopyTotal = 0, g_ADASTime = 0, g_GraphicTime = 0;



#if ADAS_STAMPLOG
extern UINT32 ipe_idx;
extern UINT32 g_saveCnt;
extern UINT32 g_ValidFlag;
extern UINT32 g_stopngoRslt;
extern UINT32 g_ValidBlkNum;
extern UINT32 g_StopTimeCnt;
extern UINT32 g_StopCnt;
extern UINT32 g_VoteNum;
extern UINT32 g_cadNum0;
extern UINT32 g_cadNum1;
extern UINT32 g_cadNum2;
extern UINT32 g_cadNum3;
extern UINT32 g_cadNum4;
extern UINT32 g_StopTimeDuration;
extern UINT32 g_FcdeltaTime;
extern UINT32 g_SnGdeltaTime;
//extern UINT32 g_isoValue;
//extern UINT32 g_Eth;
extern UINT32 g_stopngoRslt;
extern UINT32 g_AutoVPTime;
//MovieStamp
static void ADAS_SetMovieLog(UINT32 Id, char* Buf, UINT32 BufLen)
{
    // Add log here!!!
    //sprintf(Buf, "%02d::%02d %03d %01d %02d %02d %02d %02d %02d %02d %02d %02d %02d %04d %01d %01d", g_uiFcDist, g_saveCnt, ipe_idx, g_ValidFlag, g_ValidBlkNum, g_cadNum0, g_cadNum1, g_cadNum2, g_cadNum3, g_cadNum4, g_StopCnt, g_StopTimeCnt, g_StopTimeDuration/1000, g_isoValue, g_Eth, g_stopngoRslt);
    //sprintf(Buf, "%02d::%02d::%02d %03d %01d %02d %02d %02d %02d %02d %02d %02d %02d %02d %01d", g_uiFcDist, g_uiFcKelDist, g_saveCnt, ipe_idx, g_ValidFlag, g_ValidBlkNum, g_cadNum0, g_cadNum1, g_cadNum2, g_cadNum3, g_cadNum4, g_StopCnt, g_StopTimeCnt, g_StopTimeDuration/1000, g_stopngoRslt);
    //sprintf(Buf, "%02d::%02d::", g_uiFcDist, g_uiFcKelDist);
    //snprintf(Buf,SPRINTF_BUFFER_SIZE, "%02d::%02d", g_uiFcDist, g_uiFcKelDist);
    snprintf(Buf,SPRINTF_BUFFER_SIZE, "%02d::%02d/%d %d %d/%d", g_uiFcDist, g_uiFcKelDist, g_ADASTime/1000, g_ISECopyTotal/1000, g_GraphicTime/1000, g_AutoVPTime/1000);

}
#endif

UINT32 ADAS_CalcBuffSize(void)
{
    UINT32 BuffSize;

    //DBG_IND("\r\n ADAS:: ADAS_CalcBuffSize \r\n" );
    BuffSize = ADAS_BUF_SIZE;//third party required

    return BuffSize;
}
void ADAS_setLdwsEanble(BOOL key)
{
    g_LdwsEnable = key;
    //DBG_DUMP("LDWS enable status: %d\r\n",g_LdwsEnable);
}
void ADAS_setFcwsEanble(BOOL key)
{
    g_FcwsEnable = key;
    //DBG_DUMP("FCWS enable status: %d\r\n",g_FcwsEnable);
}

void ADAS_Stop(void)
{
    FLGPTN uiFlag;      //#NT#2016/02/19#KCHong#[0094606]

    //DBG_DUMP("^GIn ADAS_Stop(), g_AdasStop = %d!!\r\n",g_AdasStop);
    if (g_AdasStop == FALSE)
    {
        wai_flg(&uiFlag, FLG_ID_ADAS, FLGADAS_IPPDONE, TWF_ORW);        //#NT#2016/02/19#KCHong#[0094606]
        g_AdasStop = TRUE;
        ADAS_Path4Off();
        g_firstSetCropInfo = TRUE;
        //DBG_DUMP("g_AdasStop = %d\r\n",g_AdasStop);

        ADAS_setLDWSInit();

        ADAS_setFCWSInit();

        memset(&g_AdasRslt, 0, sizeof(ADAS_RESULT_INFO));
    }else{
        DBG_ERR("please execute ADAS_Start() first...\r\n");
        g_AdasStop = TRUE;
    }

}
void ADAS_Start(void)
{
#if ADAS_STAMPLOG
    ADAS_SetMovieLogFP(ADAS_SetMovieLog);
#endif

    if (g_LdwsEnable && (g_AdasStop == TRUE) )
    {
        g_AdasStop = FALSE;
        ADAS_Path4Off();
    }
}
BOOL ADAS_GetStatus(void)
{
    return !g_AdasStop;
}
void ADAS_SetFsizeValue(UINT32 Val)
{
    g_uiFsizeVal = Val;
}
UINT32 ADAS_GetFsizeValue(void)
{
    return g_uiFsizeVal;
}
void ADAS_SetLeftSensitivity(UINT32 Val)
{
    g_uiLeftSensitivity = Val;
}
UINT32 ADAS_GetLeftSensitivity(void)
{
    return g_uiLeftSensitivity;
}
void ADAS_SetRightSensitivity(UINT32 Val)
{
    g_uiRightSensitivity = Val;
}
UINT32 ADAS_GetRightSensitivity(void)
{
    return g_uiRightSensitivity;
}
void ADAS_SetFcwsWarningDist(UINT32 Val)
{
    g_uiFcwsWarningDist = Val;
}
UINT32 ADAS_GetFcwsWarningDist(void)
{
    return g_uiFcwsWarningDist;
}

void ADAS_SetCurSpeed(FLOAT Val)
{
    //g_CurSpeed = Val;
}
INT32 ADAS_Init(MEM_RANGE *buf, MEM_RANGE *cachebuf, UINT32 ProcessRate)
{
    //CHKPNT;
    //DBG_DUMP("ADAS_Init()\r\n");
    if (!buf)
    {
        DBG_ERR("buf is NULL\r\n");
        return ADAS_STA_ERROR;
    }
    if (!buf->Addr)
    {
        DBG_ERR("bufAddr is NULL\r\n");
        return ADAS_STA_ERROR;
    }
    if (buf->Size < ADAS_CalcBuffSize())
    {
        DBG_ERR("bufSize %d < need %d\r\n",buf->Size,ADAS_CalcBuffSize());
        return ADAS_STA_ERROR;
    }

    OS_CONFIG_FLAG(FLG_ID_ADAS);            //#NT#2016/02/19#KCHong#[0094606]

    //LDWS initial
    g_uiAdasWorkBufAddr = buf->Addr; // non-cache buffer
    g_uiLdwsWorkBufAddr = g_uiAdasWorkBufAddr + INPPUT_IMGBUG_SIZE;
    g_uiFcwsWorkBufAddr = g_uiLdwsWorkBufAddr + LDWS_WORKINGBUG_SIZE;


    //g_uiSnGWorkBufAddr = g_uiAdasWorkBufAddr + LDWS_ROI_SIZE_X*LDWS_ROI_SIZE_Y*2;
    //g_DisMvOutAddr = g_uiSnGWorkBufAddr;
    //g_DisMvOutAddrCur = g_DisMvOutAddr;


    g_uiAdasRate = ProcessRate;

    g_uiSpeedLimit = 50;




    memset(&g_ImePath4Info, 0, sizeof(IPL_IME_PATH_INFO));


    ADAS_setLDWSInit();

    ADAS_setFCWSInit();

    //StopNGo_Init();

    memset(&g_AdasRslt, 0, sizeof(ADAS_RESULT_INFO));


    return ADAS_STA_OK;
}

INT32 ADAS_setLDWSInit(void)
{
    // Set NVT LDWS parameters

    // user enter
    g_LdwsSysParamter.uiHs                   = 111;
    g_LdwsSysParamter.iInitRow               = 57;
    g_LdwsSysParamter.iLsensitivity          = g_uiLeftSensitivity;
    g_LdwsSysParamter.iRsensitivity          = g_uiRightSensitivity;
    g_LdwsSysParamter.iFsizeDq2              = g_uiFsizeVal;//145;
    g_LdwsSysParamter.uiCamera2CarDistanceCm = 160;





    LDWS_setInitParms(&g_LdwsSysParamter);


    // set initial value for ldws result structure
    g_LdwsRslt.DepartureDir                  = LDWS_DEPARTURE_NONE; // no departure
    g_LdwsRslt.Failure                       = LDWS_FAILURE_FALSE;
    g_LdwsRslt.uiLx1                          = 0;
    g_LdwsRslt.uiLx2                          = 0;
    g_LdwsRslt.uiLy1                          = 0;
    g_LdwsRslt.uiLy2                          = 0;
    g_LdwsRslt.uiRx1                          = 0;
    g_LdwsRslt.uiRx2                          = 0;
    g_LdwsRslt.uiRy1                          = 0;
    g_LdwsRslt.uiRy2                          = 0;
    #if 0
    g_LdwsRslt.AutoVpParms.uiVPYNew          = 240;// new VP
    g_LdwsRslt.AutoVpParms.uiVPYDef          = 240;// default VP
    g_LdwsRslt.AutoVpParms.uiEnerUpdateMode  = FALSE;
    #else
    g_LdwsRslt.AutoVpParms.AutoVPUpdate = FALSE;
    g_LdwsRslt.AutoVpParms.uiVPYRslt = 240;
    #endif

    g_LdwsRslt.uiVPYNew = 480 - (LDWS_ROI_SIZE_Y/2); // new VP
    g_LdwsRslt.uiVPYDef = 480 - (LDWS_ROI_SIZE_Y/2); // default VP
    g_LdwsRslt.uiVPXDef = 360;

    g_LdwsRslt.AutoVpParms.AutoVPRslt.x = 960;
    g_LdwsRslt.AutoVpParms.AutoVPRslt.y = 540;


    return ADAS_STA_OK;
}

INT32 ADAS_setFCWSInit(void)
{
    FCWS_setInitParms(&g_LdwsSysParamter);
    memset(&g_FcwsRslt, 0, sizeof(FCWS_RESULT_INFO));

    return ADAS_STA_OK;
}

INT32 ADAS_Process(void)
{
    INT32 Status;

    g_uiAdasCount++;

    //DBG_DUMP("In ADAS_Process()\r\n");

    if (!g_AdasLock)
    {
        if((g_uiAdasCount%g_uiAdasRate)==0)
        {
            if (LDWS_checkInitFlag() == OFF)
            {
               return ADAS_STA_ERROR;
            }
            else
            {
               Status = ADAS_Detection();//The main detection function
            }
        }
    }
    return ADAS_STA_OK;
}
void ADAS_setCropInfo2Ime(UINT32 CropSx, UINT32 CropSy)
{
    IPL_IME_DATA    ImeData;

    ImeData.Id = IPL_ID_1;
    ImeData.PathId = IPL_IME_PATH4;
    ImeData.ImeP4.ImgFmt = IPL_IMG_Y;
    ImeData.ImeP4.OutputEn = ENABLE;
    ImeData.ImeP4.ImgSizeH = 1920;
    ImeData.ImeP4.ImgSizeV = 1080;
    ImeData.ImeP4.ImgSizeLOfs = 720; //
    ImeData.ImeP4.CropStartX = CropSx;
    ImeData.ImeP4.CropStartY = CropSy;
    ImeData.ImeP4.CropSizeH = 720;
    ImeData.ImeP4.CropSizeV = 480;

    IPL_SetCmd(IPL_SET_IME, (void *)&ImeData);
    IPL_WaitCmdFinish();

    g_ImePath4Info.ImgFmt = IPL_IMG_Y;
    g_ImePath4Info.OutputEn = ENABLE;
    g_ImePath4Info.ImgSizeH = 1920;
    g_ImePath4Info.ImgSizeV = 1080;
    g_ImePath4Info.ImgSizeLOfs = 720; //
    g_ImePath4Info.CropStartX = CropSx;
    g_ImePath4Info.CropStartY = CropSy;
    g_ImePath4Info.CropSizeH = 720;
    g_ImePath4Info.CropSizeV = 480;
}
IPL_IME_PATH_INFO ADAS_getPath4Info(void)
{
    return g_ImePath4Info;
}
void ADAS_Path4Off(void)
{
    IPL_IME_DATA    ImeData;

    ImeData.Id = IPL_ID_1;
    ImeData.PathId = IPL_IME_PATH4;
    ImeData.ImeP4.ImgFmt = IPL_IMG_Y;
    ImeData.ImeP4.OutputEn = DISABLE;
    ImeData.ImeP4.ImgSizeH = 1920;
    ImeData.ImeP4.ImgSizeV = 1080;
    ImeData.ImeP4.ImgSizeLOfs = 720; //
    ImeData.ImeP4.CropStartX = 600;
    ImeData.ImeP4.CropStartY = 300;
    ImeData.ImeP4.CropSizeH = 720;
    ImeData.ImeP4.CropSizeV = 480;

    IPL_SetCmd(IPL_SET_IME, (void *)&ImeData);
    IPL_WaitCmdFinish();


    g_ImePath4Info.ImgFmt = IPL_IMG_Y;
    g_ImePath4Info.OutputEn = DISABLE;
    g_ImePath4Info.ImgSizeH = 1920;
    g_ImePath4Info.ImgSizeV = 1080;
    g_ImePath4Info.ImgSizeLOfs = 720; //
    g_ImePath4Info.CropStartX = 600;
    g_ImePath4Info.CropStartY = 300;
    g_ImePath4Info.CropSizeH = 720;
    g_ImePath4Info.CropSizeV = 480;
}
void ADAS_setImeParms(void)
{
    IME_SUBFUNC     imeSubf = {0};
    IPL_HAL_GROUP   group = {0};
    IPC_IQparam     Ipc_IQ = {0};
    IME_STATS_INFO  stat_info = {0};

    IME_STATS_EDGE_INFO EdgeMap;          ///< edge map of statistic
    IME_STATS_HIST_INFO Histogram;        ///< edge histogram
    IME_STATS_ROI_INFO  RoiThres;         ///< ROI threshold of edge map



    group.imeUpdate = (IME_DATASTATS_EDGE|IME_DATASTATS_HISTO|IME_DATASTATS_ROI|IME_DATASTATS_CTRL);

    Ipc_IQ.EngSubF.pIme_sub = &imeSubf;
    Ipc_IQ.pGrp = &group;
    Ipc_IQ.Idx = 0;

    imeSubf.pStatsInfo = &stat_info;

    stat_info.StatsEdgeMap = &EdgeMap;
    stat_info.StatsHistogram = &Histogram;
    stat_info.StatsRoiThres = &RoiThres;

    // enable median filter & statistic
    stat_info.StatsEnable = TRUE;
    stat_info.StatsFilterEnable = TRUE;
    stat_info.StatsImgOutBF = FALSE; // ime out after filter
    // set kernel info
    EdgeMap.StatsEdgeKerEn0 = TRUE;
    EdgeMap.StatsEdgeKer0 = STATS_EKER_VA;
    EdgeMap.uiStatsShift0 = 0;
    EdgeMap.StatsEdgeKerEn1 = TRUE;
    EdgeMap.StatsEdgeKer1 = STATS_EKER_HA;
    EdgeMap.uiStatsSft1 = 0;

    // set histogram info
    Histogram.StatsHistPos.x = g_StlParam.StlHist.StlHistPos.uiPosX;
    Histogram.StatsHistPos.y = g_StlParam.StlHist.StlHistPos.uiPosY;
    Histogram.StatsHistSize.sizeH = g_StlParam.StlHist.StlHistSize.uiSizeH;
    Histogram.StatsHistSize.sizeV = g_StlParam.StlHist.StlHistSize.uiSizeV;
    Histogram.StatsHistAccTag.AccTag0 = g_StlParam.StlHist.StlHistAccTag.uiAccTag0;
    Histogram.StatsHistAccTag.AccTag1 = g_StlParam.StlHist.StlHistAccTag.uiAccTag1;


    // set ROI threshold info
    RoiThres.StatsRowPos.uiStatsRow0 = g_StlParam.StlRoi.StlRowPos.uiStlRow0;
    RoiThres.StatsRowPos.uiStatsRow1 = g_StlParam.StlRoi.StlRowPos.uiStlRow1;

    RoiThres.StatsRoi0.RoiTh0 = g_StlParam.StlRoi.StlRoi0.uiRoiTh0;
    RoiThres.StatsRoi0.RoiTh1 = g_StlParam.StlRoi.StlRoi0.uiRoiTh1;
    RoiThres.StatsRoi0.RoiTh2 = g_StlParam.StlRoi.StlRoi0.uiRoiTh2;

    RoiThres.StatsRoi1.RoiTh0 = g_StlParam.StlRoi.StlRoi1.uiRoiTh0;
    RoiThres.StatsRoi1.RoiTh1 = g_StlParam.StlRoi.StlRoi1.uiRoiTh1;
    RoiThres.StatsRoi1.RoiTh2 = g_StlParam.StlRoi.StlRoi1.uiRoiTh2;

    RoiThres.StatsRoi2.RoiTh0 = g_StlParam.StlRoi.StlRoi2.uiRoiTh0;
    RoiThres.StatsRoi2.RoiTh1 = g_StlParam.StlRoi.StlRoi2.uiRoiTh1;
    RoiThres.StatsRoi2.RoiTh2 = g_StlParam.StlRoi.StlRoi2.uiRoiTh2;

    RoiThres.StatsRoi3.RoiTh0 = g_StlParam.StlRoi.StlRoi3.uiRoiTh0;
    RoiThres.StatsRoi3.RoiTh1 = g_StlParam.StlRoi.StlRoi3.uiRoiTh1;
    RoiThres.StatsRoi3.RoiTh2 = g_StlParam.StlRoi.StlRoi3.uiRoiTh2;

    RoiThres.StatsRoi4.RoiTh0 = g_StlParam.StlRoi.StlRoi4.uiRoiTh0;
    RoiThres.StatsRoi4.RoiTh1 = g_StlParam.StlRoi.StlRoi4.uiRoiTh1;
    RoiThres.StatsRoi4.RoiTh2 = g_StlParam.StlRoi.StlRoi4.uiRoiTh2;

    IPL_CTRLRunTimeChg_IQ(0, IPC_IME_IQparam,(void*)&Ipc_IQ); // 0: sensor id
}
void ADAS_getHistInfo(IME_GET_STL_INFO *AdasInfo)
{
    IPH_QUE Que;
    Que.ImeQ = IME_GET_STATISTICS_INFO;

    IME_ENG_ReadReg(Que, (void *)AdasInfo);
}
void ADAS_updateVP(void)
{
    // get Auto-VP result
    g_AutoVPRslt = AutoVPParms_getAutoVPRst();
    g_AutoVPSrc  = AutoVPParms_getAutoVPSrc();

    //DBG_DUMP("^GAuto-VP, %d %d\r\n",g_AutoVPRslt.uiVPNew.x, g_AutoVPRslt.uiVPNew.y);
    //DBG_DUMP("P1 src W = %d, H = %d, %d %d\r\n",g_uiP1FrameWidth,g_uiP1FrameHeight,g_ImePath4Info.ImgSizeH,g_ImePath4Info.ImgSizeV);

    #if _AUTOVP_ENABLE_
    if ((g_uiAdasCount%g_AutoVPUpdateRate) == 0)
    {
        //#NT#2016/01/10#PinSu -begin
        //#NT#Add divide by 0 protection for autoVP

        //DBG_DUMP("check vp: %d %d, w = %d h = %d\r\n",g_AutoVPRslt.uiVPNew.x,g_AutoVPRslt.uiVPNew.y, g_AutoVPSrc.uiFrameWidth, g_AutoVPSrc.uiFrameHeight);
        if ( (g_AutoVPSrc.uiFrameWidth == 0) || (g_AutoVPSrc.uiFrameHeight == 0) )
        {
            g_LdwsRslt.AutoVpParms.AutoVPRslt.x = 0;
            g_LdwsRslt.AutoVpParms.AutoVPRslt.y = 0;
            DBG_DUMP("Adas_updateVP(), AutoVP srcWidth = 0 or srcHeight = 0, please check it!!!\r\n");
        }
        else
        {
            g_LdwsRslt.AutoVpParms.AutoVPRslt.x = g_AutoVPRslt.uiVPNew.x * g_ImePath4Info.ImgSizeH / g_AutoVPSrc.uiFrameWidth;//g_uiP1FrameWidth;
            g_LdwsRslt.AutoVpParms.AutoVPRslt.y = g_AutoVPRslt.uiVPNew.y * g_ImePath4Info.ImgSizeV / g_AutoVPSrc.uiFrameHeight;//g_uiP1FrameHeight;
        }
        //#NT#2016/01/10#PinSu -end

        //g_LdwsRslt.AutoVpParms.AutoVPRslt.x = g_AutoVPRslt.uiVPNew.x * g_ImePath4Info.ImgSizeH / g_uiP1FrameWidth;
        //g_LdwsRslt.AutoVpParms.AutoVPRslt.y = g_AutoVPRslt.uiVPNew.y * g_ImePath4Info.ImgSizeV / g_uiP1FrameHeight;

        //DBG_DUMP("^G1 Auto-VP, c = %d,  %d %d %d %d\r\n",g_uiAdasCount, g_LdwsRslt.AutoVpParms.AutoVPRslt.x, g_LdwsRslt.AutoVpParms.AutoVPRslt.y, g_AutoVPRslt.uiVPNew.x, g_AutoVPRslt.uiVPNew.y);


        if ( (g_LdwsRslt.AutoVpParms.AutoVPRslt.x <= 0) && (g_LdwsRslt.AutoVpParms.AutoVPRslt.y <=0) )
        {
            //DBG_DUMP("in\r\n");
            g_LdwsRslt.AutoVpParms.AutoVPRslt.x = (int)(g_ImePath4Info.ImgSizeH / 2);
            g_LdwsRslt.AutoVpParms.AutoVPRslt.y = (int)(g_ImePath4Info.ImgSizeV / 2);
        }

        //DBG_DUMP("^G1 Auto-VP, c = %d,  %d %d %d %d\r\n",g_uiAdasCount, g_LdwsRslt.AutoVpParms.AutoVPRslt.x, g_LdwsRslt.AutoVpParms.AutoVPRslt.y, g_AutoVPRslt.uiVPNew.x, g_AutoVPRslt.uiVPNew.y);



        if (g_LdwsRslt.AutoVpParms.AutoVPRslt.x < (LDWS_ROI_SIZE_X/2) )
        {
            g_LdwsRslt.AutoVpParms.AutoVPRslt.x = LDWS_ROI_SIZE_X/2;
        }

        //DBG_DUMP("^G2 Auto-VP, c = %d,  %d %d %d %d\r\n",g_uiAdasCount, g_LdwsRslt.AutoVpParms.AutoVPRslt.x, g_LdwsRslt.AutoVpParms.AutoVPRslt.y, g_AutoVPRslt.uiVPNew.x, g_AutoVPRslt.uiVPNew.y);


        if (g_LdwsRslt.AutoVpParms.AutoVPRslt.x > (int)(g_ImePath4Info.ImgSizeH - LDWS_ROI_SIZE_X/2))
        {
            g_LdwsRslt.AutoVpParms.AutoVPRslt.x = (int)(g_ImePath4Info.ImgSizeH - LDWS_ROI_SIZE_X/2);
        }

        //DBG_DUMP("^G3 Auto-VP, c = %d,  %d %d %d %d\r\n",g_uiAdasCount, g_LdwsRslt.AutoVpParms.AutoVPRslt.x, g_LdwsRslt.AutoVpParms.AutoVPRslt.y, g_AutoVPRslt.uiVPNew.x, g_AutoVPRslt.uiVPNew.y);


        if (g_LdwsRslt.AutoVpParms.AutoVPRslt.y < (LDWS_ROI_SIZE_Y/2))
        {
            g_LdwsRslt.AutoVpParms.AutoVPRslt.y = LDWS_ROI_SIZE_Y;
        }

        //DBG_DUMP("^G4 Auto-VP, c = %d,  %d %d %d %d\r\n",g_uiAdasCount, g_LdwsRslt.AutoVpParms.AutoVPRslt.x, g_LdwsRslt.AutoVpParms.AutoVPRslt.y, g_AutoVPRslt.uiVPNew.x, g_AutoVPRslt.uiVPNew.y);


        if (g_LdwsRslt.AutoVpParms.AutoVPRslt.y > (int)(g_ImePath4Info.ImgSizeV - LDWS_ROI_SIZE_Y/2))
        {
            g_LdwsRslt.AutoVpParms.AutoVPRslt.y = (int)(g_ImePath4Info.ImgSizeV - LDWS_ROI_SIZE_Y/2);
        }

        //DBG_DUMP("%d %d\r\n",g_uiP1FrameWidth, g_uiP1FrameHeight);
        //DBG_DUMP("^GUpdate Auto-VP, c = %d,  %d %d %d %d\r\n",g_uiAdasCount, g_LdwsRslt.AutoVpParms.AutoVPRslt.x, g_LdwsRslt.AutoVpParms.AutoVPRslt.y, g_AutoVPRslt.uiVPNew.x, g_AutoVPRslt.uiVPNew.y);

        g_LdwsRslt.AutoVpParms.AutoVPUpdate = TRUE;
        g_LdwsRslt.AutoVpParms.uiVPYRslt = LDWS_ROI_SIZE_Y/2 + g_AutoVPSub;
    }else{
        if ( (g_LdwsRslt.AutoVpParms.AutoVPRslt.x <= 0) && (g_LdwsRslt.AutoVpParms.AutoVPRslt.y <=0) )
        {
            g_LdwsRslt.AutoVpParms.AutoVPRslt.x = (int)(g_ImePath4Info.ImgSizeH / 2);
            g_LdwsRslt.AutoVpParms.AutoVPRslt.y = (int)(g_ImePath4Info.ImgSizeV / 2);
        }
        g_LdwsRslt.AutoVpParms.AutoVPUpdate = FALSE;
        g_LdwsRslt.AutoVpParms.uiVPYRslt = LDWS_ROI_SIZE_Y/2 + g_AutoVPSub;
    }
    #else
    if ( (g_LdwsRslt.AutoVpParms.AutoVPRslt.x <= 0) && (g_LdwsRslt.AutoVpParms.AutoVPRslt.y <=0) )
    {
        g_LdwsRslt.AutoVpParms.AutoVPRslt.x = (int)(g_ImePath4Info.ImgSizeH / 2);
        g_LdwsRslt.AutoVpParms.AutoVPRslt.y = (int)(g_ImePath4Info.ImgSizeV / 2);
    }
    g_LdwsRslt.AutoVpParms.AutoVPUpdate = FALSE;
    g_LdwsRslt.AutoVpParms.uiVPYRslt = LDWS_ROI_SIZE_Y/2;
    #endif

}
ER ADAS_CopyIMEImg(UINT32 oriImgAddr, UINT32 outImgAddr, UINT32 oriWidth, UINT32 oriHeight, UINT32 oriLineoffset, UINT32 outWidth, UINT32 outHeight, UINT32 outLineoffset)
{
    ER erReturn = E_OK;
    ISE_OPENOBJ   IseOpenObj;
    ISE_MODE_PRAM IseEngineParam;

    IseOpenObj.uiIseClockSel = 240;
    IseOpenObj.FP_ISEISR_CB = NULL;
    ise_open(&IseOpenObj);

    IseEngineParam.IOPackFormat = ISE_Y8;
    IseEngineParam.ARGBOutMode = ISE_OUTMODE_A8;
    IseEngineParam.ScaleMethod = ISE_INTEGRATION;
    IseEngineParam.SclFtrMode = ISE_SCLFTR_AUTO;

    IseEngineParam.uiInWidth = oriWidth;
    IseEngineParam.uiInHeight = oriHeight;
    IseEngineParam.uiInLineoffset = ALIGN_CEIL_4(oriLineoffset);
    IseEngineParam.uiInAddr = ALIGN_CEIL_4(oriImgAddr);

    IseEngineParam.uiOutWidth = outWidth;
    IseEngineParam.uiOutHeight = outHeight;
    IseEngineParam.uiOutLineoffset = ALIGN_CEIL_4(outLineoffset);
    IseEngineParam.uiOutAddr = ALIGN_CEIL_4(outImgAddr);

    erReturn = ise_setMode(&IseEngineParam);
    if(erReturn != E_OK)
    {
        return erReturn;
    }

    erReturn = ise_start();
    if(erReturn != E_OK)
    {
        return erReturn;
    }

    erReturn = ise_waitFlagFrameEnd(ISE_FLAG_NO_CLEAR);
    if(erReturn != E_OK)
    {
        return erReturn;
    }

    erReturn = ise_pause();
    if(erReturn != E_OK)
    {
        return erReturn;
    }

    erReturn = ise_close();
    if(erReturn != E_OK)
    {
        return erReturn;
    }

    return E_OK;
}
INT32 ADAS_Detection(void)//The main LDWS Detection code
{
    UINT32 uiIMEImgInBufAddr, uiRoiBufAddr, uiEdgeMapAddr;
    UINT32 uiFcwsEdgeMapAddr;

    //UINT32 LDWS_ROI_SIZE_X = 720, LDWS_ROI_SIZE_Y = 480;
    LDWS_SRCIMG_INFO LdwsSrcInfo;
    UINT32 i;
    static UINT32 ImeHistParmsCnt = 0; // 0: horizontal, 1: vertical

    //UINT32 CenterX, Len;
    static UINT32 base_x = 600, base_y = 300; // FHD base, for LDWS
    static UINT32 CurTime = 0, LastTime = 0,deltaTime = 0;
    //static UINT32 CurTime2 = 0;
    static UINT32 uiTimeIdx = 0;
    UINT32 avgDeltaTime = 0;
    static UINT32 firstSetDist = TRUE, LastDist = 0;
    UINT32 deltaS = 0, deltaSpeed = 0;
    UINT32 avgCnt = 0;
    static UINT32 LDwarngStartTime = 0, FCwarngStartTime = 0;
    UINT32 CurrLDWarngTime = 0, CurrFCWarngTime = 0, delTime = 0;

    //static UINT32 uiTimeDistCuvCnt = 0;
    //char filename[50];
    //FST_FILE filehdl;
    //UINT32 ImgInSize;
    //static UINT32 fileSavCnt = 0;
    //static UINT32 SpeedCnt = 0;
    UINT32 ISECopyE = 0, ISECopyS = 0;
    //static UINT32 icounter = 0;


    CurTime = Perf_GetCurrent();


    //if ( (!g_AdasStop) && (g_CurSpeed >= (FLOAT)g_uiSpeedLimit) )
    if (!g_AdasStop)
    {
        #if 0
        if (g_AdasfirstSetdateInfo)
        {
            g_AdasTesttime = rtc_getTime();
            g_AdasTestdate = rtc_getDate();
            g_AdasTestCurTime = Perf_GetCurrent();

            g_AdasfirstSetdateInfo = FALSE;
        }
        #endif

        deltaTime = (CurTime > LastTime)?(CurTime - LastTime):(0xFFFFFFFF-CurTime + LastTime + 1);
        LastTime = CurTime;

        g_ADASTime = deltaTime;


#if 1 // resource is not enough
        g_uiTimeArray[(uiTimeIdx++)%10] = deltaTime/1000; // ms

        avgCnt = 0;
        for(i = 0;i<10;i++)
        {
            if (g_uiTimeArray[i] > 0)
            {
                avgDeltaTime+= g_uiTimeArray[i];
                avgCnt++;
            }
        }
        avgDeltaTime = avgDeltaTime / avgCnt;

        //DBG_DUMP("%d %d %d %d %d %d %d %d %d %d, %d\r\n",g_uiTimeArray[0],g_uiTimeArray[1],g_uiTimeArray[2],g_uiTimeArray[3],g_uiTimeArray[4],g_uiTimeArray[5],g_uiTimeArray[6],g_uiTimeArray[7],g_uiTimeArray[8],g_uiTimeArray[9],avgDeltaTime);

        if ((uiTimeIdx%10) == 0)
        {
            if ( (avgDeltaTime > 300) && (avgDeltaTime <= 500) )
            {
                //DBG_ERR("Avg delay time > 100ms & < 500ms\r\n");
                DBG_ERR("Avg delay time = %dms\r\n",avgDeltaTime);
            }
            if (avgDeltaTime > 500)
            {
                DBG_ERR("Avg delay time > 500ms, drop ADAS...\r\n");
                //DBG_ERR("Processing Time: %d %d %d\r\n",g_FcStartTime, g_FcEndTime);
                //DBG_ERR("FCWS Delta Time: %d\r\n",g_FcEndTime - g_FcStartTime);
                return ADAS_STA_ERROR;
            }
        }
        //DBG_DUMP("CurTime = %d\r\n",CurTime);
#endif

        if (g_firstSetCropInfo)
        {
            ADAS_setCropInfo2Ime(600, 300);
            Delay_DelayMs(33); // check the setting is work
            g_firstSetCropInfo = FALSE;
        }

        //Get source frame data from ime output
        IPL_IME_CurInfo1.Id = IPL_ID_1; // camera 1
        IPL_GetCmd(IPL_GET_IME_CUR_BUF_ADDR, (void *)&IPL_IME_CurInfo1);
        g_uiAdasSrcFrameAddr = IPL_IME_CurInfo1.ImeP4.PixelAddr[0];//Image source frame buffer Y address
        g_uiAdasEdgeAddr = IPL_IME_CurInfo1.ImeP4.PixelAddr[1];//Image source frame buffer EdgeMap address
        g_uiAdasHistAddr = IPL_IME_CurInfo1.ImeP4.PixelAddr[2];//Image source frame buffer Histogram address
        g_uiAdasFrameWidth   = IPL_IME_CurInfo1.ImeP4.Ch[0].Width;//Image source width

        IPL_IME_CurInfo2.Id = IPL_ID_1; // camera 1
        IPL_GetCmd(IPL_GET_IME_INFOR, (void *)&IPL_IME_CurInfo2);
        g_uiAdasFrameWidth   = IPL_IME_CurInfo2.ImeP4[0].Ch[0].Width;//Image source width
        g_uiAdasFrameHeight  = IPL_IME_CurInfo2.ImeP4[0].Ch[0].Height;//Image source height
        g_uiAdasFrameLineOfs = IPL_IME_CurInfo2.ImeP4[0].Ch[0].LineOfs;//Image source line offset

        //DBG_DUMP("YAddr = 0x%08x, EdgeAddr = 0x%08x, eda = 0x%08x\r\n",g_uiAdasSrcFrameAddr,g_uiAdasEdgeAddr, g_uiAdasSrcFrameAddr+ 1920*1080);
        g_uiP1FrameWidth = IPL_IME_CurInfo1.ImeP1.Ch[0].Width;
        g_uiP1FrameHeight = IPL_IME_CurInfo1.ImeP1.Ch[0].Height;


        if ( (g_firstSetCropStartPos) && (g_uiP1FrameWidth>0) && (g_uiP1FrameHeight>0))
        {
            base_x = g_uiP1FrameWidth/2 - LDWS_ROI_SIZE_X/2;
            base_y = g_uiP1FrameHeight/2 - LDWS_ROI_SIZE_Y/2;
            g_firstSetCropStartPos = FALSE;
            //DBG_DUMP("bx = %d, by = %d, wid = %d, hei = %d\r\n",base_xx,base_yy, g_uiP1FrameWidth, g_uiP1FrameHeight);
        }

        if (g_LdwsEnable)
        {
            #if (_AUTOVP_ENABLE_)
            ADAS_updateVP();
            #endif

            #if (_AUTOVP_ENABLE_)
            LdwsSrcInfo.uiLdwsRoiSx = g_LdwsRslt.AutoVpParms.AutoVPRslt.x - LDWS_ROI_SIZE_X/2;
            LdwsSrcInfo.uiLdwsRoiSy = g_LdwsRslt.AutoVpParms.AutoVPRslt.y - LDWS_ROI_SIZE_Y/2;
            if ((g_uiAdasCount%g_AutoVPUpdateRate) == 0)
            {
                ADAS_setCropInfo2Ime(LdwsSrcInfo.uiLdwsRoiSx, LdwsSrcInfo.uiLdwsRoiSy);
            }else{
            }
            #else
            LdwsSrcInfo.uiLdwsRoiSx = base_x;
            LdwsSrcInfo.uiLdwsRoiSy = base_y;
            #endif
        }


        if (g_FcwsEnable)
        {
            if (g_setImeIniParmsflag)
            {
                if (g_AdasStop)
                {
                    return ADAS_STA_OK;
                }else{
                    clr_flg(FLG_ID_ADAS, FLGADAS_IPPDONE);      //#NT#2016/02/19#KCHong#[0094606]
                }
                if (!g_AdasStop)
                {
                    g_StlParam = FCWS_getImeParms(0, &g_FcDebugInfo); // get initial parms + Horizontal hist parms
                    ADAS_setImeParms();
                    set_flg(FLG_ID_ADAS, FLGADAS_IPPDONE);      //#NT#2016/02/19#KCHong#[0094606]
                }
                g_setImeIniParmsflag = 0;
                Delay_DelayMs(33); // check the setting is work
            }
        }


        if (g_LdwsEnable)
        {
            // copy IME data to ADAS buffer
            uiIMEImgInBufAddr = g_uiAdasWorkBufAddr;
            uiEdgeMapAddr = g_uiAdasEdgeAddr;
            uiFcwsEdgeMapAddr = g_uiFcwsWorkBufAddr;
            g_uiFcwsUsedBufAddr = uiFcwsEdgeMapAddr + FCWS_EDGEMAP_SIZE;


            uiRoiBufAddr = g_uiAdasSrcFrameAddr;


            ISECopyS = Perf_GetCurrent();
            ADAS_CopyIMEImg(uiRoiBufAddr, uiIMEImgInBufAddr, LDWS_ROI_SIZE_X, LDWS_ROI_SIZE_Y, LDWS_ROI_SIZE_X, LDWS_ROI_SIZE_X, LDWS_ROI_SIZE_Y, LDWS_ROI_SIZE_X);
            ADAS_CopyIMEImg(uiEdgeMapAddr, uiFcwsEdgeMapAddr, 480, 480, 480, 480, 480, 480);
            ISECopyE = Perf_GetCurrent();
            g_ISECopyTotal = (ISECopyE > ISECopyS)?(ISECopyE - ISECopyS):(0xFFFFFFFF-ISECopyE + ISECopyS + 1);


            //DBG_DUMP("1 uiIMEImgInBufAddr = 0x%08x\r\n",uiIMEImgInBufAddr);
            uiIMEImgInBufAddr = dma_getCacheAddr(uiIMEImgInBufAddr);
            uiFcwsEdgeMapAddr = dma_getCacheAddr(uiFcwsEdgeMapAddr);
            //DBG_DUMP("2 uiIMEImgInBufAddr = 0x%08x\r\n",uiIMEImgInBufAddr);



            if (g_FcwsEnable)
            {
                if (g_AdasStop)
                {
                    return ADAS_STA_OK;
                }else{
                    clr_flg(FLG_ID_ADAS, FLGADAS_IPPDONE);      //#NT#2016/02/19#KCHong#[0094606]
                }
                // Set histogram information
                if (!g_AdasStop)
                {
                    if ((ImeHistParmsCnt%2) == 0) // horizontal
                    {
                        if (ImeHistParmsCnt == 0)
                        {
                            ADAS_getHistInfo(&g_HistInfo);
                        }else{
                            ADAS_getHistInfo(&g_HorHistInfo);
                            // update horizontal histogram
                            g_HistInfo.uiGetAccTagBin1 = g_HorHistInfo.uiGetAccTagBin1;
                            g_HistInfo.uiGetHistMax1   = g_HorHistInfo.uiGetHistMax1;
                        }
                    }else{
                        ADAS_getHistInfo(&g_VerHistInfo);
                        // update vertical histogram
                        g_HistInfo.uiGetAccTagBin0 = g_VerHistInfo.uiGetAccTagBin0;
                        g_HistInfo.uiGetHistMax0   = g_VerHistInfo.uiGetHistMax0;
                    }
                    ImeHistParmsCnt++;
                    set_flg(FLG_ID_ADAS, FLGADAS_IPPDONE);      //#NT#2016/02/19#KCHong#[0094606]
                }
            }


            // Set image info
            LdwsSrcInfo.uiLdwsSrcBufAddr= uiIMEImgInBufAddr;
            LdwsSrcInfo.uiLdwsSrcHeight= g_uiAdasFrameHeight;
            LdwsSrcInfo.uiLdwsSrcWidth= g_uiAdasFrameWidth;
            LdwsSrcInfo.uiLdwsSrcLineofs = g_uiAdasFrameWidth;
            LdwsSrcInfo.uiLdwsRoiWidth = LDWS_ROI_SIZE_X;
            LdwsSrcInfo.uiLdwsRoiHeight = LDWS_ROI_SIZE_Y;
            LdwsSrcInfo.uiVideoWidth = g_uiP1FrameWidth;
            LdwsSrcInfo.uiVideoHeight = g_uiP1FrameHeight;


            ////////// new struct to fit new LDWS lib //////////
            LdwsSrcData.uiLdwsSrcDataAddr = uiIMEImgInBufAddr;
            LdwsSrcData.uiLdwsSrcDataWidth = g_uiAdasFrameWidth;
            LdwsSrcData.uiLdwsSrcDataHeight = g_uiAdasFrameHeight;
            LdwsSrcData.uiLdwsSrcDataRoiWidth = LDWS_ROI_SIZE_X;
            LdwsSrcData.uiLdwsSrcDataRoiHeight= LDWS_ROI_SIZE_Y;
            LdwsSrcData.uiLdwsWorkBufAddr = g_uiLdwsWorkBufAddr;
            ////////////////////////////////////////////////////


            if (!g_AdasStop)
            {
                LdwsSrcInfo.uiLdwsViewAngle = 50;
                g_uiLdwsProcessTime = LDWS_Detection(&LdwsSrcData, &g_LdwsRslt, &LdwsDebugInfo, 2*1000000);
            }else{
                return ADAS_STA_OK;
            }


            g_GraphicTime = g_uiLdwsProcessTime;


            if (g_LdwsRslt.Failure)
            {
                g_LdwsRslt.uiLx1 = 0;
                g_LdwsRslt.uiLy1 = 0;
                g_LdwsRslt.uiLx2 = 0;
                g_LdwsRslt.uiLy2 = 0;
                g_LdwsRslt.uiRx1 = 0;
                g_LdwsRslt.uiRy1 = 0;
                g_LdwsRslt.uiRx2 = 0;
                g_LdwsRslt.uiRy2 = 0;
            }

            if ( (g_LdwsRslt.uiLx1 > 0) || (g_LdwsRslt.uiLy1 > 0) )
            {
                g_LdwsRslt.uiLx1 = g_LdwsRslt.uiLx1 + LdwsSrcInfo.uiLdwsRoiSx;
                g_LdwsRslt.uiLy1 = g_LdwsRslt.uiLy1 + LdwsSrcInfo.uiLdwsRoiSy;
            }

            if ( (g_LdwsRslt.uiLx2 > 0) || (g_LdwsRslt.uiLy2 > 0) )
            {
                g_LdwsRslt.uiLx2 = g_LdwsRslt.uiLx2 + LdwsSrcInfo.uiLdwsRoiSx;
                g_LdwsRslt.uiLy2 = g_LdwsRslt.uiLy2 + LdwsSrcInfo.uiLdwsRoiSy;
            }

            if ( (g_LdwsRslt.uiRx1 > 0) || (g_LdwsRslt.uiRy1 > 0) )
            {
                g_LdwsRslt.uiRx1 = g_LdwsRslt.uiRx1 + LdwsSrcInfo.uiLdwsRoiSx;
                g_LdwsRslt.uiRy1 = g_LdwsRslt.uiRy1 + LdwsSrcInfo.uiLdwsRoiSy;
            }

            if ( (g_LdwsRslt.uiRx2 > 0) || (g_LdwsRslt.uiRy2 > 0) )
            {
                g_LdwsRslt.uiRx2 = g_LdwsRslt.uiRx2 + LdwsSrcInfo.uiLdwsRoiSx;
                g_LdwsRslt.uiRy2 = g_LdwsRslt.uiRy2 + LdwsSrcInfo.uiLdwsRoiSy;
            }


#if 1
            //def LDWS_WARNING
            // modify warning strategy
            g_LdwsRslt.DepartureDirVideo = g_LdwsRslt.DepartureDir;
            g_LdwsRslt.DepartureDirVoice = g_LdwsRslt.DepartureDir;

            if ((g_LdwsRslt.DepartureDir != LDWS_DEPARTURE_NONE) && (g_WarngEn == FALSE))
            {
                g_WarngEn = TRUE;
                LDwarngStartTime = Perf_GetCurrent();
                //g_LDwarngStartTime = LDwarngStartTime;
            }


            if (g_WarngEn)
            {
                #if 0
                if (g_WarngCnt < g_WarningLDTimes)
                #else
                CurrLDWarngTime = Perf_GetCurrent();
                delTime = (CurrLDWarngTime > LDwarngStartTime)?(CurrLDWarngTime - LDwarngStartTime):(0xFFFFFFFF-CurrLDWarngTime+LDwarngStartTime+1);
                //g_LDwarngDelTime = delTime;
                if (delTime < g_LDwargdebounce)
                #endif
                {
                    g_WarngCnt++;

                    if (g_WarngCnt > 1)
                        g_LdwsRslt.DepartureDirVoice = LDWS_DEPARTURE_NONE;
                    if (g_WarngCnt > g_WarningVideoCnt)
                        g_LdwsRslt.DepartureDirVideo = LDWS_DEPARTURE_NONE;
                }else{
                    g_WarngCnt = 0;
                    g_WarngEn = FALSE;
                    g_LdwsRslt.DepartureDirVoice = LDWS_DEPARTURE_NONE;
                    //LDwarngStartTime = 0;
                    //testCnt = 0;
                }
            }

            //DBG_DUMP("LDwarngStartTime = %d, delTime = %d, LD En = %d, Cnt = %d, LD_Departure = %d, SoundWar = %d, VideoWar = %d\r\n",LDwarngStartTime/1000, delTime/1000, g_WarngEn, g_WarngCnt, g_LdwsRslt.DepartureDir, g_LdwsRslt.DepartureDirVoice, g_LdwsRslt.DepartureDirVideo);
#endif

            //DBG_DUMP("Sx = %d Sy = %d, %d %d %d %d\r\n",LdwsSrcInfo.uiLdwsRoiSx,LdwsSrcInfo.uiLdwsRoiSy,g_LdwsRslt.uiVPXDef,g_LdwsRslt.uiVPYDef,g_LdwsRslt.uiVPXDef,g_LdwsRslt.uiVPYDef);

            memcpy(&g_AdasRslt.LdwsRsltInfo, &g_LdwsRslt, sizeof(LDWS_RESULT_INFO));
            memcpy(&g_AdasRslt.SrcImgInfo, &LdwsSrcInfo, sizeof(LDWS_SRCIMG_INFO));
            g_AdasRslt.LdwsRsltInfo.uiVPXNew = LdwsSrcInfo.uiLdwsRoiSx + LDWS_ROI_SIZE_X/2;
            g_AdasRslt.LdwsRsltInfo.uiVPYNew = g_LdwsRslt.AutoVpParms.uiVPYRslt + LdwsSrcInfo.uiLdwsRoiSy;
            g_AdasRslt.LdwsRsltInfo.uiVPYDef = g_LdwsRslt.uiVPYDef + LdwsSrcInfo.uiLdwsRoiSy;
            g_AdasRslt.LdwsRsltInfo.uiVPXDef = g_LdwsRslt.uiVPXDef + LdwsSrcInfo.uiLdwsRoiSx;



            FcwsSrcData.uiFcwsSrcDataAddr = LdwsSrcData.uiLdwsSrcDataAddr;
            FcwsSrcData.uiFcwsEdgeDataAddr = uiFcwsEdgeMapAddr;
            FcwsSrcData.uiFcwsSrcDataWidth = LdwsSrcData.uiLdwsSrcDataWidth;
            FcwsSrcData.uiFcwsSrcDataHeight = LdwsSrcData.uiLdwsSrcDataHeight;
            FcwsSrcData.uiFcwsSrcDataRoiWidth = LdwsSrcData.uiLdwsSrcDataRoiWidth;
            FcwsSrcData.uiFcwsSrcDataRoiHeight = LdwsSrcData.uiLdwsSrcDataRoiHeight;
            FcwsSrcData.uiFcwsWorkBufAddr = g_uiFcwsUsedBufAddr;
            FcwsSrcData.uiFcwsHistDataAddr = g_uiAdasHistAddr; // un-used


            if (g_FcwsEnable)
            {
                if (!g_AdasStop)
                {
                    LdInfo2Fcws = LDWS_getLdwsInfoForFcws();
                    g_uiFcwsProcessTime = FCWS_Detection(&FcwsSrcData, &LdInfo2Fcws, &g_FcwsRslt, &g_HistInfo,  &FcwsAlgDebugInfo);
                }else{
                    return ADAS_STA_OK;
                }
            }

            //DBG_DUMP("Fc_EN = %d, rslt = %d %d %d %d\r\n",g_FcwsEnable, g_FcwsRslt.uiRx,g_FcwsRslt.uiRy,g_FcwsRslt.uiLx,g_FcwsRslt.uiLy);

            if (g_FcwsEnable)
            {
                g_AdasRslt.FcwsRsltInfo.uiLx = g_FcwsRslt.uiLx + LdwsSrcInfo.uiLdwsRoiSx;
                g_AdasRslt.FcwsRsltInfo.uiLy = g_FcwsRslt.uiLy + LdwsSrcInfo.uiLdwsRoiSy;
                g_AdasRslt.FcwsRsltInfo.uiRx = g_FcwsRslt.uiRx + LdwsSrcInfo.uiLdwsRoiSx;
                g_AdasRslt.FcwsRsltInfo.uiRy = g_FcwsRslt.uiRy + LdwsSrcInfo.uiLdwsRoiSy;
                //DBG_DUMP("rslt = %d %d %d %d\r\n",g_AdasRslt.FcwsRsltInfo.uiRx,g_AdasRslt.FcwsRsltInfo.uiRy,g_AdasRslt.FcwsRsltInfo.uiLx,g_AdasRslt.FcwsRsltInfo.uiLy);

                if (g_LdwsRslt.Failure)
                {
                    g_AdasRslt.FcwsRsltInfo.uiLx = 0;
                    g_AdasRslt.FcwsRsltInfo.uiLy = 0;
                    g_AdasRslt.FcwsRsltInfo.uiRx = 0;
                    g_AdasRslt.FcwsRsltInfo.uiRy = 0;
                    g_FcwsRslt.uiDist = 0;
                    g_FcwsRslt.uiKelDist = 0;
                }

                g_Failure = g_LdwsRslt.Failure;


                //g_AdasRslt.FcwsRsltInfo.uiDist = g_FcwsRslt.uiDist;
                g_AdasRslt.FcwsRsltInfo.uiDist = g_FcwsRslt.uiKelDist;

                if ((g_FcwsRslt.uiKelDist > 0) && (g_FcwsRslt.uiKelDist < g_uiFcwsWarningDist))
                {
                    g_AdasRslt.FcwsRsltInfo.FCWarning = TRUE;
                }else{
                    g_AdasRslt.FcwsRsltInfo.FCWarning = FALSE;
                }

                g_FcDebugInfo.FcwsSearchRangInfo.uiIniRow       = g_FcDebugInfo.FcwsSearchRangInfo.uiIniRow + LdwsSrcInfo.uiLdwsRoiSy;
                g_FcDebugInfo.FcwsSearchRangInfo.uiFinalRow     = g_FcDebugInfo.FcwsSearchRangInfo.uiFinalRow + LdwsSrcInfo.uiLdwsRoiSy;
                g_FcDebugInfo.FcwsSearchRangInfo.uiLeftBoundary = g_FcDebugInfo.FcwsSearchRangInfo.uiLeftBoundary+ LdwsSrcInfo.uiLdwsRoiSx;
                g_FcDebugInfo.FcwsSearchRangInfo.uiRightBoundary = g_FcDebugInfo.FcwsSearchRangInfo.uiRightBoundary+ LdwsSrcInfo.uiLdwsRoiSx;

                //memcpy(&g_AdasRslt.FcwsDebugInfo, &g_FcDebugInfo, sizeof(FCWS_DEBUG_INFO));

                ///////////////////////////////////////////////
                g_AdasRslt.FcwsDebugInfo.uiIniRow = g_FcDebugInfo.FcwsSearchRangInfo.uiIniRow;
                g_AdasRslt.FcwsDebugInfo.uiFinalRow = g_FcDebugInfo.FcwsSearchRangInfo.uiFinalRow;
                g_AdasRslt.FcwsDebugInfo.uiLeftBoundary = g_FcDebugInfo.FcwsSearchRangInfo.uiLeftBoundary;
                g_AdasRslt.FcwsDebugInfo.uiRightBoundary = g_FcDebugInfo.FcwsSearchRangInfo.uiRightBoundary;
                ///////////////////////////////////////////////


                // MovieStamp
                g_uiFcDist = g_FcwsRslt.uiDist;
                g_uiFcKelDist = g_FcwsRslt.uiKelDist;
                g_uiFcKelEn = g_FcwsRslt.uiKelEn;

                if (firstSetDist)
                {
                    LastDist = g_uiFcKelDist;
                    firstSetDist = FALSE;
                }

                //DBG_DUMP("delTime = %d, LastDist = %d, g_uiFcDist = %d\r\n",deltaTime, LastDist, g_uiFcDist);

                if (deltaTime > 0)
                {
                    if (LastDist > g_uiFcKelDist)
                        deltaSpeed = (((LastDist - g_uiFcKelDist)*1000)/(deltaTime/1000));
                    else
                        deltaSpeed = 0;//(((g_uiFcDist - LastDist)*1000)/(deltaTime/1000));
                    if (deltaSpeed > 0)
                    {
                        deltaS = (g_uiFcKelDist*1000) / deltaSpeed;
                    }else{
                        deltaS = 0;
                    }

                }
                //DBG_DUMP("delTime = %d, LastDist = %d, g_uiFcDist = %d, speed = %d, deltaS = %d\r\n",deltaTime, LastDist, g_uiFcDist, deltaSpeed, deltaS);

                //g_uiLastDist = LastDist;
                LastDist = g_uiFcKelDist;

                g_uideltaS = deltaS;
                g_uiSpeed = deltaSpeed;
                g_uiDeltaTime = deltaTime;
                g_uiFCW_E = g_FcwsRslt.uiFcw_e;

                //DBG_DUMP("icounter = %d\r\n",icounter);

#if 0
                //uiTimeDistCuv[uiTimeDistCuvCnt++] = icounter++;
                uiTimeDistCuv[uiTimeDistCuvCnt++] = g_uiFcDist;
                uiTimeDistCuv[uiTimeDistCuvCnt++] = g_uiFcKelDist;
                //DBG_DUMP("g_FcwsRslt.uiDist = %d, g_uiFcDist = %d\r\n",g_FcwsRslt.uiDist, g_uiFcDist);
                //ImgInSize = 2000*4;
                //if (uiTimeDistCuvCnt == 2000)

                ImgInSize = 1000*4;
                if (uiTimeDistCuvCnt == 1000)
                {
                    // dump file
                    sprintf(filename  ,"A:\\FCWS_%d_%d_%d\\TimeDist_f%d.bin",g_AdasTestdate, g_AdasTesttime, g_AdasTestCurTime, fileSavCnt);
                    filehdl = FileSys_OpenFile(filename,FST_OPEN_WRITE|FST_CREATE_ALWAYS);
                    FileSys_WriteFile(filehdl, (UINT8 *)uiTimeDistCuv, &ImgInSize, 0, NULL);
                    FileSys_CloseFile(filehdl);
                    fileSavCnt++;
                    uiTimeDistCuvCnt = 0;
                }
#endif


#if 1
                //def FCWS_WARNING
                // modify warning strategy
                g_AdasRslt.FcwsRsltInfo.FCSoundWarning = g_AdasRslt.FcwsRsltInfo.FCWarning;
                g_AdasRslt.FcwsRsltInfo.FCVideoWarning = g_AdasRslt.FcwsRsltInfo.FCWarning;

                //DBG_DUMP("uiKelDist = %d, warning = %d\r\n", g_FcwsRslt.uiKelDist, g_AdasRslt.FcwsRsltInfo.FCWarning);

                if ((g_AdasRslt.FcwsRsltInfo.FCWarning) && (g_FCWarngEn == FALSE))
                {
                    g_FCWarngEn = TRUE;
                    FCwarngStartTime = Perf_GetCurrent();
                    //g_FCwarngStartTime = FCwarngStartTime;
                }


                if (g_FCWarngEn)
                {
                    #if 0
                    if (g_FCWarngCnt < g_WarningFCTimes)
                    #else
                    CurrFCWarngTime = Perf_GetCurrent();
                    delTime = (CurrFCWarngTime > FCwarngStartTime)?(CurrFCWarngTime - FCwarngStartTime):(0xFFFFFFFF-CurrFCWarngTime + FCwarngStartTime+1);
                    //g_FCwarngDelTime = delTime;
                    if (delTime < g_FCwargdebounce)
                    #endif
                    {
                        g_FCWarngCnt++;
                        //DBG_DUMP("^Gin, FCWarning = %d\r\n",g_AdasRslt.FcwsRsltInfo.FCWarning);

                        if (g_FCWarngCnt > 1)
                            g_AdasRslt.FcwsRsltInfo.FCSoundWarning = FALSE;
                        if (g_FCWarngCnt > g_FCWarningVideoCnt)
                            g_AdasRslt.FcwsRsltInfo.FCVideoWarning = FALSE;
                        else
                            g_AdasRslt.FcwsRsltInfo.FCVideoWarning = TRUE;
                    }else{
                        g_FCWarngCnt = 0;
                        g_FCWarngEn = FALSE;
                        g_AdasRslt.FcwsRsltInfo.FCSoundWarning = FALSE;
                        //FCwarngStartTime = 0;

                    }
                }
                //DBG_DUMP("En = %d, WVCnt = %d, Cnt = %d, SoundWar = %d, VideoWar = %d\r\n",g_FCWarngEn, g_FCWarningVideoCnt, g_FCWarngCnt, g_AdasRslt.FcwsRsltInfo.FCSoundWarning, g_AdasRslt.FcwsRsltInfo.FCVideoWarning);

#endif


            }


            if (g_FcwsEnable)
            {
                if (g_AdasStop)
                {
                    return ADAS_STA_OK;
                }else{
                    clr_flg(FLG_ID_ADAS, FLGADAS_IPPDONE);      //#NT#2016/02/19#KCHong#[0094606]
                }
                if (!g_AdasStop)
                {
                    g_StlParam = FCWS_getImeParms(ImeHistParmsCnt%2, &g_FcDebugInfo);
                    ADAS_setImeParms();
                    set_flg(FLG_ID_ADAS, FLGADAS_IPPDONE);      //#NT#2016/02/19#KCHong#[0094606]
                }else{
                    return ADAS_STA_OK;
                }
            }


            // Auto-VP result
            g_AdasRslt.LdwsRsltInfo.AutoVpParms.AutoVPRslt.x = g_AutoVPRslt.uiVPNew.x;
            g_AdasRslt.LdwsRsltInfo.AutoVpParms.AutoVPRslt.y = g_AutoVPRslt.uiVPNew.y;


            // Auto-VP debug 4 lines
            for(i = 0;i<4;i++)
            {
                if ((g_AutoVPRslt.ChosedLine[i].sP.x < (int)g_uiP1FrameWidth) && (g_AutoVPRslt.ChosedLine[i].sP.x >= 0) && (g_AutoVPRslt.ChosedLine[i].sP.y < (int)g_uiP1FrameHeight) && (g_AutoVPRslt.ChosedLine[i].sP.y >= 0))
                {
                    g_AdasRslt.LdwsRsltInfo.AutoVpParms.AutoVPFourLines[i].sP.x = g_AutoVPRslt.ChosedLine[i].sP.x;
                    g_AdasRslt.LdwsRsltInfo.AutoVpParms.AutoVPFourLines[i].sP.y = g_AutoVPRslt.ChosedLine[i].sP.y;
                }else{

                    g_AdasRslt.LdwsRsltInfo.AutoVpParms.AutoVPFourLines[i].sP.x = 0;
                    g_AdasRslt.LdwsRsltInfo.AutoVpParms.AutoVPFourLines[i].sP.y = 0;
                }

                //DBG_DUMP("%d %d\r\n",g_AdasRslt.LdwsRsltInfo.AutoVpParms.AutoVPFourLines[i].sP.x,g_AdasRslt.LdwsRsltInfo.AutoVpParms.AutoVPFourLines[i].sP.y);


                if ((g_AutoVPRslt.ChosedLine[i].eP.x < (int)g_uiP1FrameWidth) && (g_AutoVPRslt.ChosedLine[i].eP.x >= 0) && (g_AutoVPRslt.ChosedLine[i].eP.y < (int)g_uiP1FrameHeight) && (g_AutoVPRslt.ChosedLine[i].eP.y >= 0))
                {

                    g_AdasRslt.LdwsRsltInfo.AutoVpParms.AutoVPFourLines[i].eP.x = g_AutoVPRslt.ChosedLine[i].eP.x;
                    g_AdasRslt.LdwsRsltInfo.AutoVpParms.AutoVPFourLines[i].eP.y = g_AutoVPRslt.ChosedLine[i].eP.y;
                }else{
                    g_AdasRslt.LdwsRsltInfo.AutoVpParms.AutoVPFourLines[i].eP.x = 0;
                    g_AdasRslt.LdwsRsltInfo.AutoVpParms.AutoVPFourLines[i].eP.y = 0;
                }

                //DBG_DUMP("%d %d\r\n",g_AdasRslt.LdwsRsltInfo.AutoVpParms.AutoVPFourLines[i].eP.x,g_AdasRslt.LdwsRsltInfo.AutoVpParms.AutoVPFourLines[i].eP.y);

                //DBG_DUMP("x = %d, y = %d, x = %d, y = %d\r\n",g_LdwsInfor.AutoVPFourLines[i].sP.x,g_LdwsInfor.AutoVPFourLines[i].sP.y,g_LdwsInfor.AutoVPFourLines[i].eP.x,g_LdwsInfor.AutoVPFourLines[i].eP.y);
            }

#if _AUTOVP_ENABLE_
            //#NT#2016/01/10#PinSu -begin
            //#NT#Add divide by 0 protection for autoVP
            if ( (g_AutoVPSrc.uiFrameWidth == 0) || (g_AutoVPSrc.uiFrameHeight == 0) )
            {
                g_AdasRslt.LdwsRsltInfo.AutoVpParms.AutoVPRslt.x = 100;
                g_AdasRslt.LdwsRsltInfo.AutoVpParms.AutoVPRslt.y = 100;
                DBG_DUMP("Adas_Detection(), AutoVP srcWidth = 0 or srcHeight = 0, please check it!!!\r\n");
            }
            else
            {
                //g_AdasRslt.LdwsRsltInfo.AutoVpParms.AutoVPRslt.x = g_AutoVPRslt.uiVPNew.x*g_uiP1FrameWidth/g_AutoVPSrc.uiVPSrcWidth;
                //g_AdasRslt.LdwsRsltInfo.AutoVpParms.AutoVPRslt.y = g_AutoVPRslt.uiVPNew.y*g_uiP1FrameHeight/g_AutoVPSrc.uiVPSrcHeight;
                g_AdasRslt.LdwsRsltInfo.AutoVpParms.AutoVPRslt.x = g_AutoVPRslt.uiVPNew.x*g_uiAdasFrameWidth/g_AutoVPSrc.uiFrameWidth;
                g_AdasRslt.LdwsRsltInfo.AutoVpParms.AutoVPRslt.y = g_AutoVPRslt.uiVPNew.y*g_uiAdasFrameHeight/g_AutoVPSrc.uiFrameHeight;
            }
            //#NT#2016/01/10#PinSu -end

#else
            g_AdasRslt.LdwsRsltInfo.AutoVpParms.AutoVPRslt.x = 100;
            g_AdasRslt.LdwsRsltInfo.AutoVpParms.AutoVPRslt.y = 100;
#endif


            if(g_AdasCbFP && (g_AdasRslt.SrcImgInfo.uiVideoWidth > 0) && (g_AdasRslt.SrcImgInfo.uiVideoHeight > 0) )
            {
                g_AdasCbFP(ADAS_CBMSG_WARNNING, (void *)&g_AdasRslt);
            }



            // reset FCWS result
            memset(&g_FcwsRslt, 0, sizeof(FCWS_RESULT_INFO));
            // reset LDWS result
            g_LdwsRslt.DepartureDir = LDWS_DEPARTURE_NONE;
            g_LdwsRslt.Failure = LDWS_FAILURE_FALSE;
            g_LdwsRslt.uiLx1 = 0;
            g_LdwsRslt.uiLx2 = 0;
            g_LdwsRslt.uiLy1 = 0;
            g_LdwsRslt.uiLy2 = 0;
            g_LdwsRslt.uiRx1 = 0;
            g_LdwsRslt.uiRx2 = 0;
            g_LdwsRslt.uiRy1 = 0;
            g_LdwsRslt.uiRy2 = 0;


            g_uiAdasFrameWidth = 0;

        }

        return ADAS_STA_OK;
    }

    return ADAS_STA_OK;
}

UINT32 ADAS_getHistSz(void)
{
    return FCWS_HIST_SIZE;
}
USIZE ADAS_getYSz(void)
{
    USIZE Adas_Sz;

    Adas_Sz.h = 1080;
    Adas_Sz.w = 1920;

    return Adas_Sz;
}
UINT32 ADAS_getFcwsEdgeMapSz(void)
{
    return FCWS_EDGEMAP_SIZE;
}
void ADAS_RegisterCB(ADAS_CBMSG_FP CB)
{
    g_AdasCbFP = CB;
}

ADAS_MOVIELOG_FP ADAS_fpMovieLog = NULL;
void ADAS_SetMovieLogFP(ADAS_MOVIELOG_FP fp)
{
    ADAS_fpMovieLog = fp;
}

void ADAS_GetLogString(UINT32 Id, char* Buf, UINT32 BufLen)
{
    if ( ADAS_fpMovieLog != NULL )
    {
        (ADAS_fpMovieLog)(Id, Buf, BufLen);
    }
}

