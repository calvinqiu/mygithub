#include    "Type.h"
#include    "GxImage.h"
#include    "ise_lib.h"
#include    "autoVP_Parms_int.h"
#include    "autoVP_Parms.h"
#include    "autoVP_alg.h"
#include    "IPL_Utility.h"
#include    "IPL_Cmd.h"
#include    "HwMem.h"
#include    "FileSysTsk.h"
#include    "time.h"

#define THIS_DBGLVL         2 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          Auto_VP_PARMS
#define __DBGLVL__          THIS_DBGLVL
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

#define THIRDPARTY_A_AUTO_VP_BUF_SIZE (1920*1088*1 + 640*360)               // working buffer size
#define THIRDPARTY_A_AUTO_VP_CACHE_BUF_SIZE (4*1024*1024)                   // cache buffer size
//#define UPDATE_VP        TRUE

// System
static UINT32                   g_uiAutoVPSrcFrameAddr  = 0;
static UINT32                   g_uiAutoVPBufAddr       = 0;
static UINT32                   g_uiAutoVPCacheBufAddr  = 0;
static UINT32                   g_uiAutoVPRate          = 1;
static UINT32                   g_uiAutoVPCount         = 0;
static BOOL                     g_AutoVPLock            = FALSE;
UINT32                          g_AutoVPTime            = 0;

//AutoVP
static AUTO_VP_RESULT_INFO g_AutoVPRstInfo;
static AUTO_VP_SRCIMG_INFO g_AutoVPSrcImg;
static AUTO_VP_DEBUG_INFO  g_AutoVPDbgInfo;

extern UINT32 g_LdwsEnable;
//extern FLOAT g_CurSpeed;
extern BOOL g_AdasStop;

ER autoVPCopyIMEImg(UINT32 oriImgAddr, UINT32 outImgAddr, UINT32 oriWidth, UINT32 oriHeight, UINT32 oriLineoffset, UINT32 outWidth, UINT32 outHeight, UINT32 outLineoffset);

UINT32 AutoVPParms_CalcBuffSize(void)
{
    UINT32 BuffSize;

    //DBG_IND("AutoVP:: AutoVP_CalcBuffSize \r\n" );
    BuffSize = THIRDPARTY_A_AUTO_VP_BUF_SIZE;

    return BuffSize;
}

UINT32 AutoVPParms_CalcCacheBuffSize(void)
{
    UINT32 BuffSize;

    //DBG_IND("AutoVP:: AutoVP_CalcCacheBuffSize \r\n" );
    BuffSize = THIRDPARTY_A_AUTO_VP_CACHE_BUF_SIZE;

    return BuffSize;
}

INT32 AutoVPParms_Init(MEM_RANGE *buf, MEM_RANGE *cacheBuf, UINT32 ProcessRate, BOOL DebugFlag)
{
    //IMG_BUF   in_img;

    if (!buf)
    {
        DBG_ERR("buf is NULL\r\n");
        return AUTO_VP_PARMS_STA_ERROR;
    }
    if (!buf->Addr)
    {
        DBG_ERR("bufAddr is NULL\r\n");
        return AUTO_VP_PARMS_STA_ERROR;
    }
    if (buf->Size < AutoVPParms_CalcBuffSize())
    {
        DBG_ERR("bufSize %d < need %d\r\n",buf->Size,AutoVPParms_CalcBuffSize());
        return AUTO_VP_PARMS_STA_ERROR;
    }

    if (!cacheBuf)
    {
        DBG_ERR("cacheBuf is NULL\r\n");
        return AUTO_VP_PARMS_STA_ERROR;
    }
    if (!cacheBuf->Addr)
    {
        DBG_ERR("cacheBufAddr is NULL\r\n");
        return AUTO_VP_PARMS_STA_ERROR;
    }
    if (cacheBuf->Size < AutoVPParms_CalcCacheBuffSize())
    {
        DBG_ERR("cacheBufSize %d < need %d\r\n",cacheBuf->Size,AutoVPParms_CalcCacheBuffSize());
        return AUTO_VP_PARMS_STA_ERROR;
    }


    //AutoVP initial
    g_uiAutoVPSrcFrameAddr = buf->Addr;
    g_uiAutoVPBufAddr = buf->Addr;
    g_uiAutoVPCacheBufAddr = cacheBuf->Addr;

    g_uiAutoVPRate = ProcessRate;

    //AutoVP set parameter
    AutoVP_Init();

    // set initial value for autoVP result structure
    memset(&g_AutoVPRstInfo, 0, sizeof(AUTO_VP_RESULT_INFO));
    //g_AutoVPRstInfo.Failure        = AUTO_VP_FAILURE_TRUE;

    return AUTO_VP_PARMS_STA_OK;
}

INT32 AutoVPParms_UnInit(void)
{
    AutoVP_Uninit();
    return AUTO_VP_PARMS_STA_OK;
}

INT32 AutoVPParms_Process(void)
{
    INT32 Status;

    g_uiAutoVPCount++;

    if ( (!g_AutoVPLock /*&& (g_LdwsEnable) && (!g_AdasStop)*/ /*&& (g_CurSpeed >50)*/)
        && (g_uiAutoVPCount%g_uiAutoVPRate)==0 )
    {
        if (AutoVP_GetInitFlag() == OFF)
        {
           return AUTO_VP_PARMS_STA_ERROR;
        }
        else
        {
           Status = AutoVPParms_Detection();    //The main detection function
           return Status;
        }
    }
    return AUTO_VP_PARMS_STA_OK;
}

#if 0
static bool VP_dumpDramToFile(CHAR* cImgFilePath, UINT32 uiImgStartAddr, UINT32 uiDataSize)
{
    UINT32 Pos = 0;
    INT32 FileOpenStatus = 0, ret = 0;
    FST_FILE filehdl = NULL;

    DBG_IND("%s\r\n", cImgFilePath);
    DBG_IND("uiDataSize = %d\r\n", uiDataSize);
    DBG_IND("data dumping from %x\r\n", uiImgStartAddr);
    //DBG_IND("%x bytes are dumped to %s ....\r\n", uiDataSize, cImgFilePath);
   filehdl = FileSys_OpenFile(cImgFilePath, FST_CREATE_ALWAYS|FST_OPEN_WRITE);
    DBG_DUMP("FileSys_OpenFile\r\n");
    if(!filehdl)
    {
        DBG_ERR("dumping result from DRAM - fail ....\r\n");
        return false;
    }
    FileOpenStatus = FileSys_SeekFile(filehdl, Pos, FST_SEEK_SET);
    ret = FileSys_WriteFile(filehdl, (UINT8*)uiImgStartAddr, &uiDataSize, 0, NULL);
    FileSys_CloseFile(filehdl);
    DBG_DUMP("FileSys_CloseFile\r\n");
    return true;
}
#endif
#if 0
static bool VP_dumpDramToFileEx1Ch( CHAR* cImgFilePath, UINT32 pData, INT32 w, INT32 h )
{
    VP_dumpDramToFile(cImgFilePath, pData, w*h);

    return TRUE;
}
#endif

INT32 AutoVPParms_Detection(void)
{
    UINT32 autoVPCurrTime = 0;
    static UINT32 autoVPTotalTime = 0, autoVPLastTime = 0;
    AUTO_VP_SRCIMG_INFO *pAutoVP_Src_info = &g_AutoVPSrcImg;
    UINT32            uiAdasSrcFrameAddr;
    UINT32            uiAutoVPFrameWidth, uiAutoVPFrameHeight, uiAutoVPFrameLineOfs;
    UINT32            uiROIImgX, uiROIImgY, uiROIImgWidth, uiROIImgHeight;
    IPL_IME_BUF_ADDR  IPL_IME_CurInfo1 = {0};

    //DBG_DUMP("^Yin AutoVP\r\n");

#if 1

    autoVPCurrTime = Perf_GetCurrent();
    autoVPTotalTime = (autoVPCurrTime > autoVPLastTime)?(autoVPCurrTime - autoVPLastTime):(0xFFFFFFFF-autoVPCurrTime + autoVPLastTime + 1);
    autoVPLastTime = autoVPCurrTime;
    g_AutoVPTime = autoVPTotalTime;

    //Get source frame data from ime output
    IPL_IME_CurInfo1.Id = IPL_ID_1; // camera 1
    IPL_GetCmd(IPL_GET_IME_CUR_BUF_ADDR, (void *)&IPL_IME_CurInfo1);
    uiAdasSrcFrameAddr = IPL_IME_CurInfo1.ImeP3.PixelAddr[0];       //Image source frame buffer Y address

    uiAutoVPFrameWidth    = IPL_IME_CurInfo1.ImeP3.Ch[0].Width;     //Image source width
    uiAutoVPFrameHeight   = IPL_IME_CurInfo1.ImeP3.Ch[0].Height;    //Image source height
    uiAutoVPFrameLineOfs  = IPL_IME_CurInfo1.ImeP3.Ch[0].LineOfs;   //Image source line offset

    if(!IPL_IME_CurInfo1.P3Enable)
    {
        //DBG_ERR("IME path3 is not enable\r\n");
        return AUTO_VP_PARMS_STA_ERROR;
    }

    if (!uiAdasSrcFrameAddr)
    {
        DBG_DUMP("Err: Buf not ready!!!\r\n");
        return AUTO_VP_PARMS_STA_ERROR;
    }

    pAutoVP_Src_info->uiScaleRsl = 10;
    if (uiAutoVPFrameWidth >= AUTO_VP_SIZE_WIDTH)
    {
        pAutoVP_Src_info->uiScaleUp = (1<<pAutoVP_Src_info->uiScaleRsl);
        pAutoVP_Src_info->uiScaleDown = (uiAutoVPFrameWidth<<pAutoVP_Src_info->uiScaleRsl) / AUTO_VP_SIZE_WIDTH;
    }
    else
    {
        pAutoVP_Src_info->uiScaleDown = (1<<pAutoVP_Src_info->uiScaleRsl);
        pAutoVP_Src_info->uiScaleUp = (AUTO_VP_SIZE_WIDTH<<pAutoVP_Src_info->uiScaleRsl) / uiAutoVPFrameWidth;
    }

    uiROIImgX       = 0;
    uiROIImgY       = (uiAutoVPFrameHeight>>1);
    uiROIImgWidth   = uiAutoVPFrameWidth;
    uiROIImgHeight  = ((uiAutoVPFrameHeight>>1) - (uiAutoVPFrameHeight/10));

    pAutoVP_Src_info->uiVPSrcBufAddr  = g_uiAutoVPSrcFrameAddr;
    pAutoVP_Src_info->uiVPSrcWidth    = uiROIImgWidth * pAutoVP_Src_info->uiScaleUp / pAutoVP_Src_info->uiScaleDown;
    pAutoVP_Src_info->uiVPSrcHeight   = uiROIImgHeight * pAutoVP_Src_info->uiScaleUp / pAutoVP_Src_info->uiScaleDown;
    pAutoVP_Src_info->uiVPSrcLineofs  = pAutoVP_Src_info->uiVPSrcWidth;
    pAutoVP_Src_info->uiFrameWidth    = uiAutoVPFrameWidth;
    pAutoVP_Src_info->uiFrameHeight   = uiAutoVPFrameHeight;
    pAutoVP_Src_info->uiRoiWidth      = uiROIImgWidth;
    pAutoVP_Src_info->uiRoiHeight     = uiROIImgHeight;
    pAutoVP_Src_info->uiRoiSx         = uiROIImgX;
    pAutoVP_Src_info->uiRoiSy         = uiROIImgY;

    if((pAutoVP_Src_info->uiVPSrcWidth<1) || (pAutoVP_Src_info->uiVPSrcWidth>AUTO_VP_SIZE_WIDTH)
        || (pAutoVP_Src_info->uiVPSrcHeight<1) || (pAutoVP_Src_info->uiVPSrcHeight>AUTO_VP_SIZE_HEIGHT))
    {
        DBG_WRN("Check auto VP size: (%d, %d)\r\n", pAutoVP_Src_info->uiVPSrcWidth, pAutoVP_Src_info->uiVPSrcHeight);
    }

    g_AutoVPRstInfo.ISETime = clock();
    autoVPCopyIMEImg(uiAdasSrcFrameAddr + uiROIImgX + uiROIImgY*uiAutoVPFrameLineOfs
                      , pAutoVP_Src_info->uiVPSrcBufAddr
                      , uiROIImgWidth, uiROIImgHeight, uiAutoVPFrameLineOfs
                      , pAutoVP_Src_info->uiVPSrcWidth, pAutoVP_Src_info->uiVPSrcHeight, pAutoVP_Src_info->uiVPSrcLineofs);
    g_AutoVPRstInfo.ISETime = clock() - g_AutoVPRstInfo.ISETime;

    if (pAutoVP_Src_info->uiVPSrcWidth <= 0)
    {
        return AUTO_VP_PARMS_STA_ERROR;
    }

    if ((pAutoVP_Src_info->uiVPSrcWidth != 0) && (pAutoVP_Src_info->uiVPSrcHeight != 0))
    {
        //UINT32 autoVPTime = clock();
        AutoVP_Detection(g_AutoVPSrcImg, g_uiAutoVPBufAddr, g_uiAutoVPCacheBufAddr, &g_AutoVPRstInfo, &g_AutoVPDbgInfo);
        //autoVPTime = clock() - autoVPTime;

        #if 0
        DBG_DUMP("^BISE TIme: %d\r\n", pAutoVPRsltInfo->ISETime);
        DBG_DUMP("^BGraphic TIme: %d\r\n", pAutoVPRsltInfo->GraphicTime);
        DBG_DUMP("^BAuto VP TIme: %d\r\n", autoVPTime);
        #endif
    }

    if (g_AutoVPRstInfo.FailureStatus)
    {
        g_AutoVPRstInfo.HoughCount = 0;
    }
#endif

    return AUTO_VP_PARMS_STA_OK;
}

ER autoVPCopyIMEImg(UINT32 oriImgAddr, UINT32 outImgAddr, UINT32 oriWidth, UINT32 oriHeight, UINT32 oriLineoffset, UINT32 outWidth, UINT32 outHeight, UINT32 outLineoffset)
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

AUTO_VP_RESULT_INFO AutoVPParms_getAutoVPRst(void)
{
    return g_AutoVPRstInfo;
}

AUTO_VP_SRCIMG_INFO AutoVPParms_getAutoVPSrc(void)
{
    return g_AutoVPSrcImg;
}

AUTO_VP_DEBUG_INFO AutoVPParms_getAutoVPDbg(void)
{
    return g_AutoVPDbgInfo;
}

