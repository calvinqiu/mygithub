////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "GxImage.h"
#include "ImageUnit_Photo.h"
#include "UIInfo.h"
//#NT#2016/06/28#KCHong -begin
//#NT#DbgLog in GPS section
#if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
#include "mp4log.h" 
#endif
//#NT#2016/06/28#KCHong -end
#if _FD_FUNC_
#include "fd_lib.h"
#include "UIPhotoMapping.h"
#include "ImageUnit_ImagePipe.h"
#endif
#if MOVIE_FD_FUNC_
#include "fd_lib.h"
#include "UIMovieMapping.h"
#include "UIPhotoMapping.h"
#include "UIInfo.h"
#include "ImageUnit_ImagePipe.h"
#endif
#if _TSR_FUNC_
#include "tsr_lib.h"
#include "TSD_DSP_lib.h"
#include "UIAppMovie.h"
#include "UIMovieMapping.h"
#include "UIPhotoMapping.h"
#include "UIInfo.h"
#include "ImageUnit_ImagePipe.h"
#define TSR_TSD_MODEL_NAME "tsd_model.bin"
#define TSR_OCR_MODEL_NAME "ocr_model.bin"
#endif
//#NT#2017/02/20#CCChao -begin
//#NT#tsr model in pstore
#if TSR_MODEL_IN_PSTORE
#include "PStore.h"
#define PS_TSR_MODEL1_DATA            "TSRMDL1_DAT"
#define PS_TSR_MODEL1_SIZE            "TSRMDL1_SIZ"
#define PS_TSR_MODEL2_DATA            "TSRMDL2_DAT"
#define PS_TSR_MODEL2_SIZE            "TSRMDL2_SIZ"
#endif
//#NT#2017/02/20#CCChao -end
#if _SD_FUNC_
#include "sd_lib.h"
#endif
#if _ADAS_FUNC_
//#NT#2016/06/16#KCHong -begin
//#NT#New ADAS
#include "adas_Apps.h"
#include "SnG_Parms.h"
#include "UIAppMovie.h"
#include "ImageUnit_ImagePipe.h"

extern URECT gMovieOsdDispCord;
//#NT#2016/06/16#KCHong -end
//
#if(_ADAS_DSP_FUNC_==ENABLE)
#include "ADAS_DSP_Lib.h"
#endif
//
#endif

//#NT#2016/09/29#KCHong -begin
//#NT#The GPS related variables should not depend on ADAS.
#if 1//(GPS_FUNCTION == ENABLE)
extern FLOAT g_CurSpeed;
extern BOOL g_GPSLinked;
extern BOOL g_GPSStatus;
#endif
//#NT#2016/09/29#KCHong -end
#if IPCAM_FUNC
#include "UCtrlAppEvent.h"
#include "NvtUctrlAPI.h"
#include "md_api.h"
#endif
// #NT#2015/09/17#JS Liu - begin
#if _CVHOST_EX_
#include "CvHost_lib.h"
#include "UIAppMovie_CvHostEx.h"

static BOOL g_CvHostExDone = FALSE;
#endif
// #NT#2015/09/17#JS Liu - end
//#NT#2016/05/31#Omega Yu -begin
#if _NVTCV_EX_
#include "NvtCv_lib.h"
#include "UIAppMovie_NvtCvEx.h"

static BOOL g_NvtCvExDone = FALSE;
#endif
//#NT#2016/05/31#Omega Yu -end
//#NT#2016/03/02#Lincy Lin -begin
//#NT#Support object tracking function
#if MOVIE_OT_FUNC
#include "NvtOt.h"
#include "ImageUnit_ImagePipe.h"
#include "UIMovieMapping.h"
#include "UIAppMovie.h"
#endif
//#NT#2016/03/02#Lincy Lin -end

//#NT#2016/05/20#Yang Jin -begin
//#NT#Support Dis dsp function
#if MOVIE_DISDSP_FUNC
#include "dis_alg_dsp.h"
#include "ImageUnit_ImagePipe.h"
#endif
//#NT#2016/05/20#Yang Jin -end

//#NT#2016/04/22#Nestor Yang -begin
//#NT# Support Driver Drowsiness Detection(DDD)
#if MOVIE_DDD_FUNC
#include "DDD_lib.h"
#include "UIAppMovie.h"
//#NT#2016/05/11#Isiah Chang -begin
//#NT# Support Driver Drowsiness Detection(DDD)
#include "crypto.h"
#define DDD_MODEL_NAME "ciphertext.bin"
#define DDD_MAX_DB_FILE_SIZE    (12 * 1024 * 1024)
//#NT#2016/05/11#Isiah Chang -end
#endif
//#NT#2016/04/22#Nestor Yang -end

//#NT#2016/05/23#David Tsai -begin
//#NT# Add tampering detection and background calculation function
//#NT#2016/10/14#Yuzhe Bian -begin
//#NT# Support trip-wire detection & trip-zone detection function
#if MOVIE_BC_FUNC || MOVIE_TD_FUNC || MOVIE_TWD_FUNC || MOVIE_TZD_FUNC || MOVIE_SCD_FUNC
#include "BC_lib.h"
#include "UIMovieMapping.h"
#include "UIAppMovie.h"
#include "ImageUnit_ImagePipe.h"
#endif

#if MOVIE_TD_FUNC
#include "TD_lib.h"
#endif

#if MOVIE_TWD_FUNC
#include "TWD_lib.h"
#endif
#if MOVIE_TZD_FUNC
#include "TZD_lib.h"
#endif
//#NT#2016/10/14#Yuzhe Bian -end
//#NT#2016/05/23#David Tsai -end

//#NT#2016/10/26#Omega Yu -begin
//#NT# Support scene change detection function
#if MOVIE_SCD_FUNC
#include "SCD_lib.h"
#endif
//#NT#2016/10/26#Omega Yu -end

//#NT#2016/10/17#Bin Xiao -begin
//#NT# Support Face Tracking Grading(FTG)
#if MOVIE_FTG_FUNC
#include "ftg_lib.h"
#include "UIMovieMapping.h"
#include "UIAppMovie.h"
#include "ImageUnit_ImagePipe.h"
#include "UIPhotoMapping.h"
#include "ftg_fd_lib.h"
#endif
//#NT#2016/10/17#Bin Xiao -end


//#NT#2016/10/18#Jeah Yen -begin
//#NT#support sensor map
#include "DxSensor.h"
//#NT#2016/10/18#Jeah Yen -end
#if DDD_MODEL_IN_PSTORE
#include "PStore.h"
#define PS_DDD_MODEL1_DATA            "DDDMDL1_DAT"
#define PS_DDD_MODEL1_SIZE             "DDDMDL1_SIZ"
#endif

#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UIPhotoFuncInfo
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
//#define __DBGFLT__          "[cap]"
//#define __DBGFLT__          "[cb]"
//#define __DBGFLT__          "[set]"
//#define __DBGFLT__          "[zoom]"
//#define __DBGFLT__          "[af]"
#include "DebugModule.h"

//#NT#2016/05/23#David Tsai -begin
//#NT# Add tampering detection and background calculation function
//#NT#2016/10/14#Yuzhe Bian -begin
//#NT# Support trip-wire detection & trip-zone detection function
#if MOVIE_BC_FUNC || MOVIE_TD_FUNC || MOVIE_TWD_FUNC || MOVIE_TZD_FUNC
static BOOL gBCLock=TRUE;
static UINT32 gBCCounter=0, gBCProcessRate=4;// 3;
#if MOVIE_BC_FUNC || MOVIE_TWD_FUNC || MOVIE_TZD_FUNC
static UINT32 gBCUpdateCounter=0;
#endif
#endif
//#NT#2016/10/14#Yuzhe Bian -end
//#NT#2016/05/23#David Tsai -end
#if MOVIE_FTG_FUNC
static BOOL gFTGLock=TRUE;
#endif
extern UINT32 System_GetEnableSensor(void);


//#NT#2016/11/01#Charlie Chang -begin
//#NT# add motion start flag
#if IPCAM_FUNC
static BOOL gMDStartFlag = FALSE;
#endif
//#NT#2016/11/01#Charlie Chang -end

PHOTO_FUNC_INFO PhotoFuncInfo_dummy =
{
    {
    PHOTO_ID_1,             ///< function hook process Id.
    //#NT#2016/10/18#Jeah Yen -begin
    //#NT#support sensor map
    IPL_PATH_1,               ///< IPL Id.
    //#NT#2016/10/18#Jeah Yen -end
    PHOTO_ISR_SIE_VD,       ///< isr tigger event
    NULL,                   ///< init fp, only execute at first process
    NULL,                   ///< process fp
    NULL,                   ///< process end fp
    NULL,                   ///< get working buffer fp
    NULL,                   ///< get working cache buffer fp
    },
    NULL  ///< point next Function Obj, last Function must be set NULL
};

UINT32 Photo_getDualCamSetting(void)
{
    UINT32           myDualCam = DUALCAM_FRONT;

    #if (SENSOR_CAPS_COUNT == 1)
    if(System_GetEnableSensor() == (SENSOR_1) )
        myDualCam = DUALCAM_FRONT;
    else
        myDualCam = DUALCAM_BEHIND;
    #endif
    #if (SENSOR_CAPS_COUNT == 2)
    if(System_GetEnableSensor() == (SENSOR_1) || System_GetEnableSensor() == (SENSOR_2))
    {
        if(System_GetEnableSensor() == (SENSOR_1) )
            myDualCam = DUALCAM_FRONT;
        else
            myDualCam = DUALCAM_BEHIND;
    }
    else
    {
        myDualCam = UI_GetData(FL_DUAL_CAM);
    }
    #endif
    return myDualCam;
}

#if (MOVIE_TSR_FUNC_ && _TSR_FUNC_)
void Movie_TSRInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    TSR_Init(buf, cachebuf, 1);
}

void Movie_TSRProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    IMG_BUF     InputImg={0};
    ISF_PORT*   pSrc = ImageUnit_Out(&ISF_ImagePipe1, ISF_OUT1);
    UINT32      ImageRatioIdx = GetMovieSizeRatio(UI_GetData(FL_MOVIE_SIZE));

    //DBG_WRN("...\r\n");
    //return;

    if( (!pSrc) || (pSrc->Flag & ISF_PORT_FLAG_PAUSE) )
    { return; }

    ImageUnit_SetParam(&ISF_ImagePipe1, IMAGEPIPE_PARAM_PULLBUF_OUT1, (UINT32)&InputImg);

    //GxImg_DumpBuf("A:\\test.raw",&InputImg, GX_IMAGE_DUMP_ALL);

    if(InputImg.PxlAddr[0]==0)
    { return; }

    TSR_Process(&InputImg, Get_FDImageRatioValue(ImageRatioIdx), buf->Addr, cachebuf->Addr);
}

PHOTO_FUNC_INFO PhotoFuncInfo_mvtsr =
{
    {
    PHOTO_ID_2,             ///< function hook process Id.
    //#NT#2016/10/18#Jeah Yen -begin
    //#NT#support sensor map
    IPL_PATH_1,               ///< IPL Id.
    //#NT#2016/10/18#Jeah Yen -end
    PHOTO_ISR_IME_FRM_END,  ///< isr tigger event
    Movie_TSRInit,          ///< init tsr, only execute at first process
    Movie_TSRProcess,       ///< process tsr
    NULL,                   ///< process end tsr
    TSR_CalcBuffSize,       ///< get working buffer tsr
    TSR_CalcCacheBuffSize,  ///< get working cache buffer tsr
    },
    NULL  ///< point next Function Obj, last Function must be set NULL
};

BOOL Movie_TSRDSPReadModel(CHAR *filePath, UINT32 mdlAddr, UINT32 mdlSize)
{
    FST_FILE filehdl;
    filehdl = FileSys_OpenFile(filePath, FST_OPEN_READ);
    if(!filehdl)
    {
        DBG_ERR("Error: can not open file %s\r\n", filePath);
        return FALSE;
    }

    FileSys_ReadFile(filehdl, (UINT8*)mdlAddr, &mdlSize, 0, NULL);
    FileSys_CloseFile( filehdl );
    return TRUE;
}

//#NT#2017/02/20#CCChao -begin
//#NT#Add for save TSR MODEL in pstore
#if TSR_MODEL_IN_PSTORE
UINT32 Movie_TSRCheckPSFormat(void)
{
    #define SETTING_SIZE 30
    #define FORMAT_CHK_STRING  "format="

    UINT32      uiReadSize = SETTING_SIZE;
    UINT32      uiUpdateChk;
    CHAR        cDataBuf[SETTING_SIZE]={0}, *pCh;
    FST_FILE    pFile;

    pFile = FileSys_OpenFile("A:\\tsr_psformat", FST_OPEN_READ);
    if (pFile)
    {
        FileSys_ReadFile(pFile, (UINT8 *)cDataBuf, &uiReadSize, 0, NULL);
        FileSys_CloseFile(pFile);
        //DBG_DUMP("Read size %d\r\n", uiReadSize);
        if (uiReadSize)
        {
            pCh = strstr(cDataBuf, FORMAT_CHK_STRING);
            sscanf(pCh + sizeof(FORMAT_CHK_STRING) - 1, "%d", &uiUpdateChk);
            DBG_DUMP("Movie_TSRCheckPSFormat FormatChk = %d, size %d\r\n", uiUpdateChk, sizeof(FORMAT_CHK_STRING));
            return uiUpdateChk;
        }
    }
    else
    {
        //DBG_ERR("Open tsr_psformat check file error!\r\n");
    }
    return 0;

}
UINT32 Movie_TSRCheckDBUpdate(void)
{
    #define SETTING_SIZE 30
    #define UPDATE_CHK_STRING  "update="

    UINT32      uiReadSize = SETTING_SIZE;
    UINT32      uiUpdateChk;
    CHAR        cDataBuf[SETTING_SIZE]={0}, *pCh;
    FST_FILE    pFile;

    pFile = FileSys_OpenFile("A:\\tsr_update", FST_OPEN_READ);
    if (pFile)
    {
        FileSys_ReadFile(pFile, (UINT8 *)cDataBuf, &uiReadSize, 0, NULL);
        FileSys_CloseFile(pFile);
        //DBG_DUMP("Read size %d\r\n", uiReadSize);
        if (uiReadSize)
        {
            pCh = strstr(cDataBuf, UPDATE_CHK_STRING);
            sscanf(pCh + sizeof(UPDATE_CHK_STRING) - 1, "%d", &uiUpdateChk);
            DBG_DUMP("Movie_TSRCheckDBUpdate UpdateChk = %d, size %d\r\n", uiUpdateChk, sizeof(UPDATE_CHK_STRING));
            return uiUpdateChk;
        }
    }
    else
    {
        //DBG_ERR("Open tsr_update check file error!\r\n");
    }
    return 0;

}
#endif

UINT32 Movie_TSRGetDBFileSizeFrCard_TSD(void)
{
    FST_FILE filehdl = NULL;
    UINT32 ui32FileSize = 0;

    CHAR* pBinaryFileName = "B:\\TSR_MODEL\\"TSR_TSD_MODEL_NAME;
    filehdl = FileSys_OpenFile(pBinaryFileName, FST_OPEN_READ);
    if(!filehdl)
    {
        CHAR* pBinaryFileName2 = "A:\\TSR_MODEL\\"TSR_TSD_MODEL_NAME;
        DBG_DUMP("TSR load tsd model from SD-card!\r\n");
        filehdl = FileSys_OpenFile(pBinaryFileName2, FST_OPEN_READ);
    }

    if(filehdl)
    {
        // Get file size info
        FileSys_SeekFile(filehdl, 0, FST_SEEK_END);
        ui32FileSize = (UINT32)FileSys_TellFile(filehdl);
        //DBG_DUMP("ui32FileSize=0x%x\r\n",ui32FileSize);
        FileSys_SeekFile(filehdl, 0, FST_SEEK_SET);

        FileSys_CloseFile(filehdl);
    }
    else
    {
        DBG_ERR("TSR_Init Failed: load tsd model failed!\r\n");
        return 0;
    }
    return ui32FileSize;
}

UINT32 Movie_TSRGetDBFileSize_TSD(void)
{
    UINT32 ui32FileSize = 0;
#if TSR_MODEL_IN_PSTORE
    PPSTORE_SECTION_HANDLE  pSection;
    UINT32  result;
    if ((pSection = PStore_OpenSection(PS_TSR_MODEL1_SIZE, PS_RDWR | PS_CREATE)) != E_PS_SECHDLER)
    {
        result = PStore_ReadSection((UINT8 *)&ui32FileSize, 0, sizeof(UINT32), pSection);
        DBG_IND("result=%d, ui32FileSize=0x%x\r\n",result,ui32FileSize);
        if (ui32FileSize==0)
        {
            DBG_ERR("PStore Save default TSD Model Size\r\n");
            ui32FileSize=Movie_TSRGetDBFileSizeFrCard_TSD();
            DBG_ERR("ui32FileSize=%d\r\n",ui32FileSize);
            PStore_WriteSection((UINT8 *)&ui32FileSize,0,sizeof(UINT32),pSection);
            PStore_CloseSection(pSection);
        }
        else
        {
            DBG_DUMP("PStore Load TSD Model Size OK\r\n");
            PStore_CloseSection(pSection);
        }
    }
    else
    {
        DBG_ERR("PStore Load TSD Model Size Fail!\r\n");
        return 0;
    }
#else
    ui32FileSize=Movie_TSRGetDBFileSizeFrCard_TSD();
#endif
    return ui32FileSize;
}

void Movie_TSRGetDBFile_TSD(UINT8* pBinaryMap, UINT32* uiBuflen)
{
    //load Landmark Model
    FST_FILE filehdl = NULL;

    CHAR* pBinaryFileName = "B:\\TSR_MODEL\\"TSR_TSD_MODEL_NAME;
    filehdl = FileSys_OpenFile(pBinaryFileName, FST_OPEN_READ);
    if(!filehdl)
    {
        CHAR* pBinaryFileName2 = "A:\\TSR_MODEL\\"TSR_TSD_MODEL_NAME;
        DBG_DUMP("TSR load tsd model from SD-card!\r\n");
        filehdl = FileSys_OpenFile(pBinaryFileName2, FST_OPEN_READ);
    }

    if(filehdl)
    {
        FileSys_ReadFile(filehdl, (UINT8*)pBinaryMap,
                     uiBuflen, 0, NULL);
        FileSys_CloseFile(filehdl);
    }
    else
    {
        DBG_ERR("TSR_Init Failed: load tsd model failed!\r\n");
        return;
    }
}
//#NT#2017/02/20#CCChao -end

//#NT#2017/04/17#PengXu -begin
//#NT#Add for save OCR MODEL in pstore
UINT32 Movie_TSRGetDBFileSizeFrCard_OCR(void)
{
    FST_FILE filehdl = NULL;
    UINT32 ui32FileSize = 0;

    CHAR* pBinaryFileName = "B:\\TSR_MODEL\\"TSR_OCR_MODEL_NAME;
    filehdl = FileSys_OpenFile(pBinaryFileName, FST_OPEN_READ);
    if(!filehdl)
    {
        CHAR* pBinaryFileName2 = "A:\\TSR_MODEL\\"TSR_OCR_MODEL_NAME;
        DBG_DUMP("TSR load ocr model from SD-card!\r\n");
        filehdl = FileSys_OpenFile(pBinaryFileName2, FST_OPEN_READ);
    }

    if(filehdl)
    {
        // Get file size info
        FileSys_SeekFile(filehdl, 0, FST_SEEK_END);
        ui32FileSize = (UINT32)FileSys_TellFile(filehdl);
        //DBG_DUMP("ui32FileSize=0x%x\r\n",ui32FileSize);
        FileSys_SeekFile(filehdl, 0, FST_SEEK_SET);

        FileSys_CloseFile(filehdl);
    }
    else
    {
        DBG_ERR("TSR_Init Failed: load ocr model failed!\r\n");
        return 0;
    }
    return ui32FileSize;
}

UINT32 Movie_TSRGetDBFileSize_OCR(void)
{
    UINT32 ui32FileSize = 0;
#if TSR_MODEL_IN_PSTORE
    PPSTORE_SECTION_HANDLE  pSection;
    UINT32  result;
    if ((pSection = PStore_OpenSection(PS_TSR_MODEL2_SIZE, PS_RDWR | PS_CREATE)) != E_PS_SECHDLER)
    {
        result = PStore_ReadSection((UINT8 *)&ui32FileSize, 0, sizeof(UINT32), pSection);
        DBG_IND("result=%d, ui32FileSize=0x%x\r\n",result,ui32FileSize);
        if (ui32FileSize==0)
        {
            DBG_ERR("PStore Save default OCR Model Size\r\n");
            ui32FileSize=Movie_TSRGetDBFileSizeFrCard_OCR();
            DBG_ERR("ui32FileSize=%d\r\n",ui32FileSize);
            PStore_WriteSection((UINT8 *)&ui32FileSize,0,sizeof(UINT32),pSection);
            PStore_CloseSection(pSection);
        }
        else
        {
            DBG_DUMP("PStore Load OCR Model Size OK\r\n");
            PStore_CloseSection(pSection);
        }
    }
    else
    {
        DBG_ERR("PStore Load OCR Model Size Fail!\r\n");
        return 0;
    }
#else
    ui32FileSize=Movie_TSRGetDBFileSizeFrCard_OCR();
#endif
    return ui32FileSize;
}

void Movie_TSRGetDBFile_OCR(UINT8* pBinaryMap, UINT32* uiBuflen)
{
    //load Landmark Model
    FST_FILE filehdl = NULL;

    CHAR* pBinaryFileName = "B:\\TSR_MODEL\\"TSR_OCR_MODEL_NAME;
    filehdl = FileSys_OpenFile(pBinaryFileName, FST_OPEN_READ);
    if(!filehdl)
    {
        CHAR* pBinaryFileName2 = "A:\\TSR_MODEL\\"TSR_OCR_MODEL_NAME;
        DBG_DUMP("TSR load ocr model from SD-card!\r\n");
        filehdl = FileSys_OpenFile(pBinaryFileName2, FST_OPEN_READ);
    }

    if(filehdl)
    {
        FileSys_ReadFile(filehdl, (UINT8*)pBinaryMap,
                     uiBuflen, 0, NULL);
        FileSys_CloseFile(filehdl);
    }
    else
    {
        DBG_ERR("TSR_Init Failed: load ocr model failed!\r\n");
        return;
    }
}
//#NT#2017/04/19#Peng Xu -end

void Movie_TSRDSPInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
//#NT#2017/02/21#CCChao -begin
//#NT# TSR will load model from an addr that generate by system.

    UINT32 tsdDspBufAddr = buf->Addr;
	UINT32 tsdModelAddr = tsdDspBufAddr + NvtCv_TSDDSP_CalcBuffSize();
	UINT32 ocrModelAddr = tsdModelAddr + NvtCv_TSDDSP_TSDModelSize();
	UINT32 uiDBFileSize_tsd = Movie_TSRGetDBFileSize_TSD();
	UINT32 uiDBFileSize_ocr = Movie_TSRGetDBFileSize_OCR();
	UINT32 uiBuflen_tsd = ALIGN_CEIL_16(uiDBFileSize_tsd);
	UINT32 uiBuflen_ocr = ALIGN_CEIL_16(uiDBFileSize_ocr);

    #if 1
#if (TSR_MODEL_IN_PSTORE)
    if(Movie_TSRCheckPSFormat()==1)
    {
        DBG_ERR("PStore format!\r\n");
        System_PS_Format();
    }
#endif
#if (TSR_MODEL_IN_PSTORE)
    UINT32 uiUpdateDBChk=Movie_TSRCheckDBUpdate();
#endif
    if(uiDBFileSize_tsd > NvtCv_TSDDSP_TSDModelSize())
    {
        DBG_ERR("TSD DB file size is too large!\r\n");
        return;
    }
    if(uiDBFileSize_ocr > NvtCv_TSDDSP_OCRModelSize())
    {
        DBG_ERR("OCR DB file size is too large!\r\n");
        return;
    }

    tsdModelAddr = ALIGN_CEIL_4(tsdModelAddr);      //Input address must algin words
    ocrModelAddr = ALIGN_CEIL_4(ocrModelAddr);      //Input address must algin words
    if(buf->Size < uiBuflen_tsd + uiBuflen_ocr + 4)
    {
        DBG_ERR("memory buf is (%d) not enough!\r\n", buf->Size);
    }

#if (TSR_MODEL_IN_PSTORE==DISABLE)
    //init Model
    {//load Tsr Model
        FST_FILE filehdl = NULL;

        CHAR* pBinaryFileName_tsd = "B:\\TSR_MODEL\\"TSR_TSD_MODEL_NAME;
        filehdl = FileSys_OpenFile(pBinaryFileName_tsd, FST_OPEN_READ);
        if(!filehdl)
        {
            CHAR* pBinaryFileName2_tsd = "A:\\TSR_MODEL\\"TSR_TSD_MODEL_NAME;
            DBG_DUMP("TSR load tsd model from SD-card!\r\n");
            filehdl = FileSys_OpenFile(pBinaryFileName2_tsd, FST_OPEN_READ);
        }

        if(filehdl)
        {
            FileSys_ReadFile(filehdl, (UINT8*)tsdModelAddr, &uiBuflen_tsd, 0, NULL);
            FileSys_CloseFile(filehdl);
        }
        else
        {
            DBG_ERR("TSR_Init Failed: load tsd model failed!\r\n");
            return;
        }

        CHAR* pBinaryFileName_ocr = "B:\\TSR_MODEL\\"TSR_OCR_MODEL_NAME;
        filehdl = FileSys_OpenFile(pBinaryFileName_ocr, FST_OPEN_READ);
        if(!filehdl)
        {
            CHAR* pBinaryFileName2_ocr = "A:\\TSR_MODEL\\"TSR_OCR_MODEL_NAME;
            DBG_DUMP("TSR load ocr model from SD-card!\r\n");
            filehdl = FileSys_OpenFile(pBinaryFileName2_ocr, FST_OPEN_READ);
        }

        if(filehdl)
        {
            FileSys_ReadFile(filehdl, (UINT8*)ocrModelAddr, &uiBuflen_ocr, 0, NULL);
            FileSys_CloseFile(filehdl);
        }
        else
        {
            DBG_ERR("TSR_Init Failed: load ocr model failed!\r\n");
            return;
        }
    }
#else
    PPSTORE_SECTION_HANDLE  pSection;
    UINT32  result;

    uiBuflen_tsd=ALIGN_CEIL_16(uiDBFileSize_tsd);
	uiBuflen_ocr=ALIGN_CEIL_16(uiDBFileSize_ocr);
    if ((pSection = PStore_OpenSection(PS_TSR_MODEL1_DATA, PS_RDWR | PS_CREATE)) != E_PS_SECHDLER)
    {
        result = PStore_ReadSection((UINT8 *)tsdModelAddr, 0, uiBuflen_tsd, pSection);
        if (result != E_PS_OK || uiUpdateDBChk)
        {
            DBG_ERR("Save TSD Model Data in PStore\r\n");
            //load Landmark Model
            Movie_TSRGetDBFile_TSD((UINT8 *)tsdModelAddr, &uiBuflen_tsd);
            PStore_WriteSection((UINT8 *)tsdModelAddr,0,uiBuflen_tsd,pSection);
            PStore_CloseSection(pSection);
        }
        else
        {
            DBG_DUMP("PStore Load TSD Model Data OK\r\n");
            PStore_CloseSection(pSection);
        }
    }
    else
    {
        DBG_ERR("3.TSR_Init Failed: load tsd model failed!\r\n");
        return;
    }

	if ((pSection = PStore_OpenSection(PS_TSR_MODEL2_DATA, PS_RDWR | PS_CREATE)) != E_PS_SECHDLER)
    {
        result = PStore_ReadSection((UINT8 *)ocrModelAddr, 0, uiBuflen_ocr, pSection);
        if (result != E_PS_OK || uiUpdateDBChk)
        {
            DBG_ERR("Save OCR Model Data in PStore\r\n");
            //load Landmark Model
            Movie_TSRGetDBFile_OCR((UINT8 *)ocrModelAddr, &uiBuflen_ocr);
            PStore_WriteSection((UINT8 *)ocrModelAddr,0,uiBuflen_ocr,pSection);
            PStore_CloseSection(pSection);
        }
        else
        {
            DBG_DUMP("PStore Load OCR Model Data OK\r\n");
            PStore_CloseSection(pSection);
        }
    }
    else
    {
        DBG_ERR("3.TSR_Init Failed: load ocr model failed!\r\n");
        return;
    }
#endif
    #else
    Movie_TSRDSPReadModel("A:\\"TSR_TSD_MODEL_NAME, tsdModelAddr, NvtCv_TSDDSP_TSDModelSize());
	Movie_TSRDSPReadModel("A:\\"TSR_OCR_MODEL_NAME, ocrModelAddr, NvtCv_TSDDSP_OCRModelSize());
    #endif

    NvtCv_TSDDSP_Init(tsdDspBufAddr, cachebuf->Addr, tsdModelAddr, ocrModelAddr);
//#NT#2017/02/21#CCChao -end
}

void Movie_TSRDSPProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    IMG_BUF     InputImg={0};
    ISF_PORT*   pSrc = ImageUnit_Out(&ISF_ImagePipe1, ISF_OUT1);

    if( (!pSrc) || (pSrc->Flag & ISF_PORT_FLAG_PAUSE) )
    { return; }

    ImageUnit_SetParam(&ISF_ImagePipe1, IMAGEPIPE_PARAM_PULLBUF_OUT1, (UINT32)&InputImg);

    if(InputImg.PxlAddr[0]==0)
    { return; }

    NvtCv_TSDDSP_Process();
}

void Photo_TSRDSPProcessEnd(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    IMG_BUF     InputImg={0};
    ISF_PORT*   pSrc = ImageUnit_Out(&ISF_ImagePipe1, ISF_OUT1);
    UINT32 logSize=0, logBuf=0;

    if( (!pSrc) || (pSrc->Flag & ISF_PORT_FLAG_PAUSE) )
    { return; }

    ImageUnit_SetParam(&ISF_ImagePipe1, IMAGEPIPE_PARAM_PULLBUF_OUT1, (UINT32)&InputImg);

    if(InputImg.PxlAddr[0]==0)
    { return; }

    Ux_PostEvent(NVTEVT_EXE_MOVIE_ALGEND,1, NVTEVT_EXE_MOVIE_ALGEND);

    logSize = TSR_GetLog(&logBuf);
    if(logSize!=0)
    { mp4log_AddLog(DbgLogTsrID, (char*)logBuf, logSize); }
}

UINT32 Movie_TSRDSP_CalcBufWithModel(VOID)
{
    UINT32 uiDBFileSize_tsd = Movie_TSRGetDBFileSize_TSD();
	UINT32 uiDBFileSize_ocr = Movie_TSRGetDBFileSize_OCR();
    if(uiDBFileSize_tsd==0)
        uiDBFileSize_tsd=Movie_TSRGetDBFileSizeFrCard_TSD();
    if(uiDBFileSize_ocr==0)
        uiDBFileSize_ocr=Movie_TSRGetDBFileSizeFrCard_OCR();

    DBG_DUMP("^Ytsd filesize:%d\r\n", uiDBFileSize_tsd);
	DBG_DUMP("^Yocr filesize:%d\r\n", uiDBFileSize_ocr);
    if(uiDBFileSize_tsd > NvtCv_TSDDSP_TSDModelSize())
    {
        DBG_ERR("TSD DB file size is too large!\r\n");
        return 0;
    }
    if(uiDBFileSize_ocr > NvtCv_TSDDSP_OCRModelSize())
    {
        DBG_ERR("OCR DB file size is too large!\r\n");
        return 0;
    }

    return TSR_CalcBuffSize() + (uiDBFileSize_tsd + uiDBFileSize_ocr + 4);
}

PHOTO_FUNC_INFO PhotoFuncInfo_mvtsrdsp =
{
    {
    PHOTO_ID_DSPH,                  ///< function hook process Id.
    //#NT#2016/10/18#Jeah Yen -begin
    //#NT#support sensor map
    IPL_PATH_1,                     ///< IPL Id.
    //#NT#2016/10/18#Jeah Yen -end
    PHOTO_ISR_IME_FRM_END,          ///< isr tigger event
    Movie_TSRDSPInit,               ///< init tsr, only execute at first process
    Movie_TSRDSPProcess,            ///< process tsr
    Photo_TSRDSPProcessEnd,         ///< process end tsr
    Movie_TSRDSP_CalcBufWithModel,  ///< get working buffer tsr
    NvtCv_TSDDSP_CalcCacheBuffSize, ///< get working cache buffer tsr
    },
    NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif


#if (PHOTO_MODE==ENABLE)
#if _FD_FUNC_

void Photo_FDInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    //#if (_FPGA_EMULATION_ == ENABLE)
    FD_Config(FD_CFG_SCALE_ENG, FD_SCALE_ENG2);
    //#endif
    // Init FD buffer, set max face number to 10, and process rate as 3
    FD_Init(buf, cachebuf, MAX_FDNUM, 15, 3);
}

void Photo_FDProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    #if 0
    IMG_BUF          InputImg;
    IPL_IME_BUF_ADDR CurInfo;
    UINT32           srcW,srcH,PxlFmt;
    UINT32           PxlAddr[3];
    UINT32           LineOff[3];
    UINT32           ImageRatioIdx;

    ImageRatioIdx = GetPhotoSizeRatio(UI_GetData(FL_PHOTO_SIZE));
    //#NT#2016/10/18#Jeah Yen -begin
    //#NT#support sensor map
    if (localInfo->DualCam > IPL_PATH_2)
        CurInfo.Id = IPL_PATH_1;
    else
        CurInfo.Id = localInfo->DualCam;
    //#NT#2016/10/18#Jeah Yen -end
    IPL_GetCmd(IPL_GET_IME_CUR_BUF_ADDR, (void *)&CurInfo);

    if (CurInfo.ImeP2.type == IPL_IMG_Y_PACK_UV422)
    {
        PxlFmt = GX_IMAGE_PIXEL_FMT_YUV422_PACKED;
    }
    else
    {
        PxlFmt = GX_IMAGE_PIXEL_FMT_YUV420_PACKED;
    }
    srcW = CurInfo.ImeP2.Ch[0].Width;
    srcH = CurInfo.ImeP2.Ch[0].Height;
    LineOff[0] = CurInfo.ImeP2.Ch[0].LineOfs;
    LineOff[1] = CurInfo.ImeP2.Ch[1].LineOfs;
    LineOff[2] = CurInfo.ImeP2.Ch[2].LineOfs;
    PxlAddr[0] = CurInfo.ImeP2.PixelAddr[0];
    PxlAddr[1] = CurInfo.ImeP2.PixelAddr[1];
    PxlAddr[2] = CurInfo.ImeP2.PixelAddr[2];
    GxImg_InitBufEx(&InputImg, srcW, srcH, PxlFmt, LineOff, PxlAddr);
    FD_Process(&InputImg, Get_FDImageRatioValue(ImageRatioIdx));
    #else
    IMG_BUF          InputImg;
    UINT32           ImageRatioIdx, myDualCam;
    ISF_UNIT*        pUnit;

    ImageRatioIdx = GetPhotoSizeRatio(UI_GetData(FL_PHOTO_SIZE));

    myDualCam = Photo_getDualCamSetting();

    //if (localInfo->DualCam == DUALCAM_FRONT || localInfo->DualCam == DUALCAM_BOTH)
    if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH)
        pUnit = &ISF_ImagePipe1;
    else
        pUnit = &ISF_ImagePipe2;
    ImageUnit_SetParam(pUnit,IMAGEPIPE_PARAM_PULLBUF_OUT2, (UINT32)&InputImg);
    //GxImg_DumpBuf("A:\\test.raw",&InputImg,GX_IMAGE_DUMP_ALL);
    FD_Process(&InputImg, Get_FDImageRatioValue(ImageRatioIdx));
    #endif
}


PHOTO_FUNC_INFO PhotoFuncInfo_fd =
{
    {
    PHOTO_ID_2,             ///< function hook process Id.
    //#NT#2016/10/18#Jeah Yen -begin
    //#NT#support sensor map
    IPL_PATH_1,               ///< IPL Id.
    //#NT#2016/10/18#Jeah Yen -end
    PHOTO_ISR_IME_FRM_END,  ///< isr tigger event
    Photo_FDInit,           ///< init fp, only execute at first process
    Photo_FDProcess,        ///< process fp
    NULL,                   ///< process end fp
    FD_CalcBuffSize,        ///< get working buffer fp
    FD_CalcCacheBuffSize,   ///< get working cache buffer fp
    },
    NULL  ///< point next Function Obj, last Function must be set NULL
};


#endif
#endif

#if MOVIE_FD_FUNC_
void Movie_FDInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    //#if (_FPGA_EMULATION_ == ENABLE)
    FD_Config(FD_CFG_SCALE_ENG, FD_SCALE_ENG2);
    //#endif
    // Init FD buffer, set max face number to 10, and process rate as 3
    FD_Init(buf, cachebuf, MAX_FDNUM, 15, 3);
}

void Movie_FDProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    #if 0
    IMG_BUF          InputImg;
    IPL_IME_BUF_ADDR CurInfo;
    UINT32           srcW,srcH,PxlFmt;
    UINT32           PxlAddr[3];
    UINT32           LineOff[3];
    UINT32           ImageRatioIdx;

    //DBG_DUMP("Movie_FDProcess\r\n");
    ImageRatioIdx = GetMovieSizeRatio(UI_GetData(FL_MOVIE_SIZE));
    //#NT#2016/10/18#Jeah Yen -begin
    //#NT#support sensor map
    //if (UI_GetData(FL_DUAL_CAM) == DUALCAM_BOTH)
        CurInfo.Id = IPL_PATH_1;
    //else
    //    CurInfo.Id = UI_GetData(FL_DUAL_CAM);
    //#NT#2016/10/18#Jeah Yen -end

    //DBG_DUMP("CurInfo.Id = %d\r\n",CurInfo.Id);
    IPL_GetCmd(IPL_GET_IME_CUR_BUF_ADDR, (void *)&CurInfo);

    if (CurInfo.ImeP2.type == IPL_IMG_Y_PACK_UV422)
    {
        PxlFmt = GX_IMAGE_PIXEL_FMT_YUV422_PACKED;
    }
    else
    {
        PxlFmt = GX_IMAGE_PIXEL_FMT_YUV420_PACKED;
    }
    srcW = CurInfo.ImeP2.Ch[0].Width;
    srcH = CurInfo.ImeP2.Ch[0].Height;
    LineOff[0] = CurInfo.ImeP2.Ch[0].LineOfs;
    LineOff[1] = CurInfo.ImeP2.Ch[1].LineOfs;
    LineOff[2] = CurInfo.ImeP2.Ch[2].LineOfs;
    PxlAddr[0] = CurInfo.ImeP2.PixelAddr[0];
    PxlAddr[1] = CurInfo.ImeP2.PixelAddr[1];
    PxlAddr[2] = CurInfo.ImeP2.PixelAddr[2];
    GxImg_InitBufEx(&InputImg, srcW, srcH, PxlFmt, LineOff, PxlAddr);
    FD_Process(&InputImg, Get_FDImageRatioValue(ImageRatioIdx));
    #else
    IMG_BUF          InputImg;
    UINT32           ImageRatioIdx, myDualCam;
    ISF_UNIT*        pUnit;

    #if (SENSOR_CAPS_COUNT == 1)
    if(System_GetEnableSensor() == (SENSOR_1) )
        myDualCam = DUALCAM_FRONT;
    else
        myDualCam = DUALCAM_BEHIND;
    #endif
    #if (SENSOR_CAPS_COUNT == 2)
    if(System_GetEnableSensor() == (SENSOR_1) || System_GetEnableSensor() == (SENSOR_2))
    {
        if(System_GetEnableSensor() == (SENSOR_1) )
            myDualCam = DUALCAM_FRONT;
        else
            myDualCam = DUALCAM_BEHIND;
    }
    else
    {
        myDualCam = UI_GetData(FL_DUAL_CAM);
    }
    #endif

    ImageRatioIdx = GetMovieSizeRatio(UI_GetData(FL_MOVIE_SIZE));
    if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH)
        pUnit = &ISF_ImagePipe1;
    else
        pUnit = &ISF_ImagePipe2;

    //#NT#2016/12/15#Charlie Chang -begin
    //#NT# make sure the Img port is existed.
    ISF_PORT *pSrc = ImageUnit_Out(pUnit, ISF_OUT2);
	if(pSrc != NULL && !(pSrc->Flag & ISF_PORT_FLAG_PAUSE)){
        ImageUnit_SetParam(pUnit,IMAGEPIPE_PARAM_PULLBUF_OUT2, (UINT32)&InputImg);
	}
	else{
		pSrc = ImageUnit_Out(pUnit, ISF_OUT1);
		if(pSrc != NULL && !(pSrc->Flag & ISF_PORT_FLAG_PAUSE) ){
        	ImageUnit_SetParam(pUnit,IMAGEPIPE_PARAM_PULLBUF_OUT1, (UINT32)&InputImg);
		}
		else{
			pSrc = ImageUnit_Out(pUnit, ISF_OUT3);
			if(pSrc != NULL && !(pSrc->Flag & ISF_PORT_FLAG_PAUSE) ){
				ImageUnit_SetParam(pUnit,IMAGEPIPE_PARAM_PULLBUF_OUT3, (UINT32)&InputImg);
			}
			else{
				if(pUnit == &ISF_ImagePipe1){
					pUnit = &ISF_ImagePipe2;
					ImageUnit_SetParam(pUnit,IMAGEPIPE_PARAM_PULLBUF_OUT1, (UINT32)&InputImg);
				}
				else{
					pUnit = &ISF_ImagePipe1;
					ImageUnit_SetParam(pUnit,IMAGEPIPE_PARAM_PULLBUF_OUT1, (UINT32)&InputImg);
				}
			}
		}
	}
	//#NT#2016/12/15#Charlie Chang -end
    //GxImg_DumpBuf("A:\\test.raw",&InputImg,GX_IMAGE_DUMP_ALL);
    FD_Process(&InputImg, Get_FDImageRatioValue(ImageRatioIdx));
    #endif
}
PHOTO_FUNC_INFO PhotoFuncInfo_mvfd =
{
    {
    PHOTO_ID_2,             ///< function hook process Id.
    //#NT#2016/10/18#Jeah Yen -begin
    //#NT#support sensor map
    IPL_PATH_1,               ///< IPL Id.
    //#NT#2016/10/18#Jeah Yen -end
    PHOTO_ISR_IME_FRM_END,  ///< isr tigger event
    Movie_FDInit,           ///< init fp, only execute at first process
    Movie_FDProcess,        ///< process fp
    NULL,                   ///< process end fp
    FD_CalcBuffSize,        ///< get working buffer fp
    FD_CalcCacheBuffSize,   ///< get working cache buffer fp
    },
    NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif

#if _SD_FUNC_
void Photo_SDInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    //SD_UnInit();
    // Init SD buffer , and set process rate as 3
    SD_Init(buf,3);
}



void Photo_SDProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    SD_Process();
}

PHOTO_FUNC_INFO PhotoFuncInfo_sd =
{
    {
    PHOTO_ID_2,             ///< function hook process Id.
    //#NT#2016/10/18#Jeah Yen -begin
    //#NT#support sensor map
    IPL_PATH_1,               ///< IPL Id.
    //#NT#2016/10/18#Jeah Yen -end
    PHOTO_ISR_IME_FRM_END,  ///< isr tigger event
    Photo_SDInit,           ///< init fp, only execute at first process
    Photo_SDProcess,        ///< process fp
    NULL,                   ///< process end fp
    SD_CalcBuffSize,        ///< get working buffer fp
    NULL,                   ///< get working cache buffer fp
    },
    NULL  ///< point next Function Obj, last Function must be set NULL
};

#endif

#if IPCAM_FUNC


void Movie_MDLock(BOOL enable){

    gMDStartFlag = enable;
}

struct IPCAM_MOTION_INFO{

    BOOL set_no_trigger;


};

void EVENT_motion_detection_init(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    struct IPCAM_MOTION_INFO *motion_info = (struct IPCAM_MOTION_INFO *)buf;
    motion_info->set_no_trigger=0;

}
INT32 EVENT_motion_size(void)
{
    return sizeof(struct IPCAM_MOTION_INFO);

}
void EVENT_motion_detection(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    INT32 id=0;
    BOOL trigger=FALSE;
    double now_time = 0;
    double time_out=1000000;
    static INT32 start_index = -1;
    static double old_time = 0;

    struct IPCAM_MOTION_INFO *motion_info = (struct IPCAM_MOTION_INFO *)buf;

    if(old_time == 0){
        old_time = clock();
    }
    //wait boot device finish
    if(start_index ==-1){
        now_time = clock();
        if(now_time - old_time <= 10000000){ ///10secs
            return;
        }

        start_index ++;
        DBG_DUMP("Motion detection start!!\r\n");
    }
	//#NT#2016/11/01#Charlie Chang -begin
    //#NT# start motion detection if flag enable
    if(gMDStartFlag == FALSE){
        if(motion_info->set_no_trigger ==1){
            DBG_DUMP("motion stop trigger\r\n");
            NvtUctrl_Notify(MOTION1_NOTRIGGER);
            motion_info->set_no_trigger = 0;
		}
        return;
	}
	//#NT#2016/11/01#Charlie Chang -end
    trigger = MD_Process(id);
    now_time = clock();
    if(trigger == TRUE){

        if(motion_info->set_no_trigger == 1){
            DBG_IND("motion still trigger\r\n");
		}
        else
        {

            DBG_DUMP("motion trigger\r\n");
            NvtUctrl_Notify(MOTION1_TRIGGER);
            motion_info->set_no_trigger =1;

        }
        old_time = now_time;

    }
    else{
        if(((now_time - old_time) >time_out) && (motion_info->set_no_trigger==1))
        {

            DBG_DUMP("motion stop trigger\r\n");
            NvtUctrl_Notify(MOTION1_NOTRIGGER);
            motion_info->set_no_trigger = 0;

        }
    }


    return;
}

PHOTO_FUNC_INFO PhotoFuncInfo_ipcam_motion =
{
    {
    PHOTO_ID_2,             ///< function hook process Id.
    //#NT#2016/10/18#Jeah Yen -begin
    //#NT#support sensor map
    IPL_PATH_1,               ///< IPL Id.
    //#NT#2016/10/18#Jeah Yen -end
    PHOTO_ISR_IME_FRM_END,  ///< isr tigger event
    EVENT_motion_detection_init,   /// < init fp, only execute at first process
    EVENT_motion_detection,        ///< process fp
    NULL,                   ///< process end fp
    NULL,                   ///< get working buffer fp
    NULL,                   ///< get working cache buffer fp
    },
    NULL                    ///< point next Function Obj, last Function must be set NULL
};


#endif


// #NT#2015/09/17#JS Liu - begin
#if _CVHOST_EX_
void Movie_CvHostExProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    if(g_CvHostExDone!=TRUE)
    {
        debug_msg("\n\r************************\n");
        debug_msg("\r*  Run CvHost Example  *\n");
        debug_msg("\r************************\n");
        #if _CVHOST_EX_ENDSP_
        SxCmd_DoCommand("dsp open");    // ASSUME DSP is opened already!!
        #endif

        UIAppMovie_CvHostEx_Opening(buf);
        UIAppMovie_CvHostEx_Sobel3x3(buf);
        UIAppMovie_CvHostEx_Gaussian3X3(buf);

        g_CvHostExDone = TRUE;

        #if _CVHOST_EX_ENDSP_
        SxCmd_DoCommand("dsp close");
        #endif
        debug_msg("\r** CvHost Example End **\n");
    }
}

UINT32 Movie_CvHostExCalcBufSize(void)
{
    UINT32 ImgRowsMax = 1080;
    UINT32 ImgColsMax = 1920;
    UINT32 ImgSizeMax = ImgRowsMax * ImgColsMax;
    UINT32 CvHostBufSize = CvHost_GetWorkMemSize();

    return ImgSizeMax*3 + CvHostBufSize;    // input gray, output gray and CvHost working buffer
}

PHOTO_FUNC_INFO MovieFuncInfo_CvHostEx =
{
    {
    PHOTO_ID_2,                     ///< function hook process Id.
    //#NT#2016/10/18#Jeah Yen -begin
    //#NT#support sensor map
    IPL_PATH_1,                       ///< IPL Id.
    //#NT#2016/10/18#Jeah Yen -end
    PHOTO_ISR_IME_FRM_END,          ///< isr tigger event
    NULL,
    Movie_CvHostExProcess,
    NULL,
    Movie_CvHostExCalcBufSize,
    NULL,
    },
    NULL
};
#endif
// #NT#2015/09/17#JS Liu - end


//#NT#2016/05/31#Omega Yu -begin
#if _NVTCV_EX_
void Movie_NvtCvExProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    if(g_NvtCvExDone!=TRUE)
    {
        debug_msg("\n\r************************\n");
        debug_msg("\r*  Run NvtCv Example   *\n");
        debug_msg("\r************************\n");
        #if _NVTCV_EX_ENDSP_
        SxCmd_DoCommand("dsp open");    // ASSUME DSP is opened already!!
        #endif

        UIAppMovie_NvtCvEx_test(buf);

        g_NvtCvExDone = TRUE;

        #if _NVTCV_EX_ENDSP_
        SxCmd_DoCommand("dsp close");
        #endif
        debug_msg("\r** NvtCv Example End **\n");
    }
}

UINT32 Movie_NvtCvExCalcBufSize(void)
{
    UINT32 ImgRowsMax = 1080;
    UINT32 ImgColsMax = 1920;
    UINT32 ImgSizeMax = ImgRowsMax * ImgColsMax;
    UINT32 NvtCvBufSize = NvtCv_GetWrkMemSize();

    return ImgSizeMax*3 + NvtCvBufSize;    // input gray, output gray and NvtCv working buffer
}

PHOTO_FUNC_INFO MovieFuncInfo_NvtCvEx =
{
    {
    PHOTO_ID_2,                     ///< function hook process Id.
    //#NT#2016/10/18#Jeah Yen -begin
    //#NT#support sensor map
    IPL_PATH_1,                       ///< IPL Id.
    //#NT#2016/10/18#Jeah Yen -end
    PHOTO_ISR_IME_FRM_END,          ///< isr tigger event
    NULL,
    Movie_NvtCvExProcess,
    NULL,
    Movie_NvtCvExCalcBufSize,
    NULL,
    },
    NULL
};
#endif // _NVTCV_EX_
//#NT#2016/05/31#Omega Yu -end


#if _ADAS_FUNC_
void Photo_ADASInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    // Init LDWS buffer , and set process rate as 1
    //#NT#2016/03/25#KCHong -begin
    //#NT#New ADAS
    //ADAS_Init(buf,cachebuf,1);
    ADAS_MidPrioAppsInit(buf,cachebuf,1,0);
    //#NT#2016/03/25#KCHong -end
    //DBG_ERR("\r\n LDWS:: Photo_LDWSInit \r\n" );
}

void Photo_ADASProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    //#NT#2016/03/25#KCHong -begin
    //#NT#New ADAS
    //ADAS_Process();
    //g_CurSpeed = 90;
    ADAS_MidPrioAppsProcess();
#if 1//(GPS_FUNCTION == ENABLE)
	AV_DumpRltToADASDspBuf((UINT32)g_CurSpeed);
#endif

    //#NT#2016/03/25#KCHong -end
    //DBG_ERR("\r\n LDWS:: Photo_LDWSProcess \r\n" );
}

//#NT#2016/06/15#KCHong -begin
//#NT#ImageUnit_ALG
#if 0
void Photo_ADASProcessEnd(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    URECT ADASCropWin = {0};
    URECT DispCord = {0,0,OSD_W,OSD_H};
    ADAS_APPS_RESULT_INFO *pAdasRlt = MovieExe_GetAdasRltOSD();

    if (ADAS_GetAppsStatus() == FALSE)
    {
        return;
    }

    // Get crop window setting
    if (ADAS_GetNewCropWindow(&ADASCropWin) == TRUE)
    {
        Ux_PostEvent(NVTEVT_CB_ADAS_SETCROPWIN, 2, ((ADASCropWin.x << 16) | ADASCropWin.y), ((ADASCropWin.w << 16) | ADASCropWin.h));
    }

    // Get FC/LD Result
    DispCord = gMovieOsdDispCord;
    if (ADAS_GetLdFcRslt(pAdasRlt, &DispCord) == ADAS_STA_OK)
    {
//#NT#2016/09/29#KCHong -begin
//#NT#The GPS related variables should not depend on ADAS.
#if (GPS_FUNCTION == ENABLE)
        if (((g_GPSLinked == FALSE) || (g_GPSStatus == FALSE) || (g_CurSpeed > 50)) && (ADAS_GetStopDectRslt() == STOPDECT_STOP_FALSE))
#else
        if (ADAS_GetStopDectRslt() == STOPDECT_STOP_FALSE)
#endif
//#NT#2016/09/29#KCHong -end
        {
            if ( (pAdasRlt->LdwsRsltInfo.Failure == LDWS_FAILURE_FALSE) &&
                ((pAdasRlt->LdwsRsltInfo.DepartureDirVoice == LDWS_DEPARTURE_LEFT) || (pAdasRlt->LdwsRsltInfo.DepartureDirVoice == LDWS_DEPARTURE_RIGHT)))
            {
                Ux_PostEvent(NVTEVT_CB_ADAS_SHOWALARM , 1, ADAS_ALARM_LD);
                debug_msg("Send 1 ADAS_ALARM_LD\r\n");
            }
            else if ((pAdasRlt->LdwsRsltInfo.Failure == LDWS_FAILURE_FALSE) && (pAdasRlt->FcwsRsltInfo.FCSoundWarning == TRUE))
            {
                Ux_PostEvent(NVTEVT_CB_ADAS_SHOWALARM , 1, ADAS_ALARM_FC);
                debug_msg("Send 1 ADAS_ALARM_FC\r\n");
            }
        }
    }
#if (_DRAW_LDWSINFO_ON_VIDEO_ == ENABLE)
    IMG_BUF InputImg={0};
    ImageUnit_SetParam(&ISF_ImagePipe1,IMAGEPIPE_PARAM_PULLBUF_OUT1, (UINT32)&InputImg);
    DispCord.x = 0;
    DispCord.y = 0;
    DispCord.w = InputImg.Width;
    DispCord.h = (InputImg.Height == 1088)?1080:(InputImg.Height);
    pAdasRlt = MovieExe_GetAdasRltVideo();
    if (ADAS_GetLdFcRslt(pAdasRlt, &DispCord) == ADAS_STA_OK)
    {
    }
#endif

    // Get Log Data
    //#NT#2016/06/28#KCHong -begin
    //#NT#DbgLog in GPS section
    #if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
    UINT32 LogSize = 0;
    UINT32 LogBuf = 0;
    LogSize = ADAS_GetLdFcLog(&LogBuf);
    if(LogSize)
    {
        mp4log_AddLog(DbgLogFCLDID, (char*)LogBuf, LogSize);
    }
    #endif
    //#NT#2016/06/28#KCHong -end

    #if (_DRAW_LDFCINFO_ON_OSD_ == ENABLE)
    if (1)
    #else
    if (ADAS_IsAdasDebug())
    #endif
    {
        Ux_PostEvent(NVTEVT_EXE_MOVIE_ALGEND,1, NVTEVT_EXE_MOVIE_ALGEND);
    }
}
#else
void Photo_ADASProcessEnd(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
}
#endif
//#NT#2016/06/15#KCHong -end

PHOTO_FUNC_INFO PhotoFuncInfo_adas =
{
    {
    PHOTO_ID_2,             ///< function hook process Id.
    //#NT#2016/10/18#Jeah Yen -begin
    //#NT#support sensor map
    IPL_PATH_1,               ///< IPL Id.
    //#NT#2016/10/18#Jeah Yen -end
    PHOTO_ISR_IME_FRM_END,  ///< isr tigger event
    Photo_ADASInit,           ///< init fp, only execute at first process
    Photo_ADASProcess,        ///< process fp
    Photo_ADASProcessEnd,   ///< process end fp         //#NT#2016/06/15#KCHong
    //#NT#2016/03/25#KCHong -begin
    //#NT#New ADAS
    //ADAS_CalcBuffSize,        ///< get working buffer fp
    ADAS_CalcMidPrioAppsBuffSize,        ///< get working buffer fp
    //#NT#2016/03/25#KCHong -end
    NULL,   ///< get working cache buffer fp
    },
    NULL  ///< point next Function Obj, last Function must be set NULL
};

void Photo_SnGInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    // Init LDWS buffer , and set process rate as 1
    //#NT#2016/03/25#KCHong -begin
    //#NT#New ADAS
    //SnGParms_Init(buf,cachebuf,1);
    ADAS_HighPrioAppsInit(buf,cachebuf,1, 66); // 66ms
    //#NT#2016/03/25#KCHong -end
    //DBG_ERR("\r\n LDWS:: Photo_LDWSInit \r\n" );
}

void Photo_SnGProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    //#NT#2016/03/25#KCHong -begin
    //#NT#New ADAS
    //SnGParms_Process();
    ADAS_HighPrioAppsProcess();
    //#NT#2016/03/25#KCHong -end
    //DBG_ERR("\r\n LDWS:: Photo_LDWSProcess \r\n" );
}

//#NT#2016/06/16#KCHong -begin
//#NT#ImageUnit_ALG
void Photo_SnGProcessEnd(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    URECT DispCord = {0,0,OSD_W,OSD_H};
    ADAS_APPS_RESULT_INFO *pAdasRlt = MovieExe_GetAdasRltOSD();

    if (ADAS_GetAppsStatus() == FALSE)
    {
        return;
    }

    // Get SnG Result
    DispCord = gMovieOsdDispCord;
    if (ADAS_GetSnGRslt(pAdasRlt, &DispCord) == ADAS_STA_OK)
    {
//#NT#2016/09/29#KCHong -begin
//#NT#The GPS related variables should not depend on ADAS.
#if (GPS_FUNCTION == ENABLE)
        if ((g_GPSLinked == FALSE) || (g_GPSStatus == FALSE) || (g_CurSpeed <= 50))
#endif
//#NT#2016/09/29#KCHong -end
        {
            if ( pAdasRlt->SnGInfo.uiGoStatus == 1 )
            {
                Ux_PostEvent(NVTEVT_CB_ADAS_SHOWALARM , 1, ADAS_ALARM_GO);
            }
            if (pAdasRlt->SnGInfo.uiStopStatus == 3 )
            {
                Ux_PostEvent(NVTEVT_CB_ADAS_SHOWALARM , 1, ADAS_ALARM_STOP);
            }
        }
    }

    #if ((_SNG_FUNC_ == ENABLE) && (_DRAW_SNG_ROI_ == ENABLE))
    ADAS_SNG_ROI_INFO *pSnGROI = NULL;

    pSnGROI = MovieExe_GetSnGROIOSD();
    ADAS_GetSnGRoi(pSnGROI, &DispCord);

    #if (_DRAW_LDWSINFO_ON_VIDEO_ == ENABLE)
    IMG_BUF InputImg={0};
    ImageUnit_SetParam(&ISF_ImagePipe1,IMAGEPIPE_PARAM_PULLBUF_OUT1, (UINT32)&InputImg);
    DispCord.x = 0;
    DispCord.y = 0;
    DispCord.w = InputImg.Width;
    DispCord.h = (InputImg.Height == 1088)?1080:(InputImg.Height);

    pAdasRlt = MovieExe_GetAdasRltVideo();
    ADAS_GetSnGRslt(pAdasRlt, &DispCord);

    pSnGROI = MovieExe_GetSnGROIVideo();
    ADAS_GetSnGRoi(pSnGROI, &DispCord);
    #endif
    #endif

    // Get Log Data
    //#NT#2016/06/28#KCHong -begin
    //#NT#DbgLog in GPS section
    #if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
    UINT32 LogSize = 0;
    UINT32 LogBuf = 0;
    LogSize = ADAS_GetSnGLog(&LogBuf);
    if(LogSize)
    {
        mp4log_AddLog(DbgLogSnGID, (char*)LogBuf, LogSize);
    }
    #endif
    //#NT#2016/06/28#KCHong -end

}
//#NT#2016/06/16#KCHong -end

PHOTO_FUNC_INFO PhotoFuncInfo_SnG =
{
    {
    PHOTO_ID_1,             ///< function hook process Id.
    //#NT#2016/10/18#Jeah Yen -begin
    //#NT#support sensor map
    IPL_PATH_1,               ///< IPL Id.
    //#NT#2016/10/18#Jeah Yen -end
    PHOTO_ISR_IME_FRM_END,  ///< isr tigger event
    Photo_SnGInit,           ///< init fp, only execute at first process
    Photo_SnGProcess,        ///< process fp
    Photo_SnGProcessEnd,     ///< process end fp
    //#NT#2016/03/25#KCHong -begin
    //#NT#New ADAS
    //SnGParms_CalcBuffSize,        ///< get working buffer fp
    ADAS_CalcHighPrioAppsBuffSize,        ///< get working buffer fp
    //#NT#2016/03/25#KCHong -end
    NULL,   ///< get working cache buffer fp
    },
    NULL  ///< point next Function Obj, last Function must be set NULL
};

void Photo_AutoVPInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    //#NT#2016/03/25#KCHong -begin
    //#NT#New ADAS
    //AutoVPParms_Init(buf, cachebuf, 1, FALSE);
    ADAS_LowPrioAppsInit(buf, cachebuf, 1, 1*1000);// 1s
    //#NT#2016/03/25#KCHong -end
}

void Photo_AutoVPProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    //DBG_DUMP("^YIn Photo_AutoVPProcess...\r\n");
    //#NT#2016/03/25#KCHong -begin
    //#NT#New ADAS
    //AutoVPParms_Process();
    ADAS_LowPrioAppsProcess();
    //#NT#2016/03/25#KCHong -end
}

void Photo_AutoVPProcessEnd(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    if (ADAS_GetAppsStatus() == FALSE)
    {
        return;
    }

    // Get Log Data
    //#NT#2016/06/28#KCHong -begin
    //#NT#DbgLog in GPS section
    #if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
    UINT32 LogSize = 0;
    UINT32 LogBuf = 0;
    LogSize = ADAS_GetAutoVPLog(&LogBuf);
    if(LogSize)
    {
        mp4log_AddLog(DbgLogAutoVPID, (char*)LogBuf, LogSize);
    }
    #endif
    //#NT#2016/06/28#KCHong -end
}

PHOTO_FUNC_INFO PhotoFuncInfo_autoVP =
{
    {
    PHOTO_ID_3,                     ///< function hook process Id.
    //#NT#2016/10/18#Jeah Yen -begin
    //#NT#support sensor map
    IPL_PATH_1,                       ///< IPL Id.
    //#NT#2016/10/18#Jeah Yen -end
    PHOTO_ISR_IME_FRM_END,          ///< isr tigger event
    Photo_AutoVPInit,               ///< init fp, only execute at first process
    Photo_AutoVPProcess,            ///< process fp
    Photo_AutoVPProcessEnd,         ///< process end fp
    //#NT#2016/03/25#KCHong -begin
    //#NT#New ADAS
    //AutoVPParms_CalcBuffSize,       ///< get working buffer fp
    //AutoVPParms_CalcCacheBuffSize,  ///< get working cache buffer fp
    ADAS_CalcLowPrioAppsBuffSize,       ///< get working buffer fp
    ADAS_CalcLowPrioAppsCacheBuffSize,  ///< get working cache buffer fp
    //#NT#2016/03/25#KCHong -end
    },
    NULL  ///< point next Function Obj, last Function must be set NULL
};

#if _ADAS_DSP_FUNC_

void Movie_ADASDSPInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{

	AV_ADASDSP_Init(buf);

}

void Movie_ADASDSPProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{

	AV_ADASDSP_Process();


}

void Photo_ADASDSPProcessEnd(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	//CHKPNT;
    URECT ADASCropWin = {0};
    URECT DispCord = {0,0,OSD_W,OSD_H};
    ADAS_DSP_RESULT_INFO *pAdasRlt = MovieExe_GetAdasDspRltOSD();
    //debug_msg("Photo_ADASDSPProcessEnd....\r\n");
    if (ADAS_GetAppsStatus() == FALSE)
    {
        //debug_msg("ADAS_GetAppsStatus() = FALSE\r\n");
        return;
    }

    // Get crop window setting
    if (ADAS_GetNewCropWindow(&ADASCropWin) == TRUE)
    {
        Ux_PostEvent(NVTEVT_CB_ADAS_SETCROPWIN, 2, ((ADASCropWin.x << 16) | ADASCropWin.y), ((ADASCropWin.w << 16) | ADASCropWin.h));
    }

    // Get FC/LD Result
     DispCord = gMovieOsdDispCord;

    if(ADASDsp_GetLdFcRslt(pAdasRlt, &DispCord) == ADAS_STA_OK)
    {
		
        if (1)
        {   
            //debug_msg("\r\n----LdwsDspRsltInfo.Failure--- =%d\r\n",(pAdasRlt->LdwsDspRsltInfo.Failure==LDWS_FAILURE_FALSE)?1:0);
            //debug_msg("\r\n----LdwsDspRsltInfo.DepartureDirVoice--- =%d\r\n",pAdasRlt->LdwsDspRsltInfo.DepartureDirVoice);
            if ( (pAdasRlt->LdwsDspRsltInfo.Failure == LDWS_FAILURE_FALSE) &&
                ((pAdasRlt->LdwsDspRsltInfo.DepartureDirVoice == LDWS_DEPARTURE_LEFT) || (pAdasRlt->LdwsDspRsltInfo.DepartureDirVoice == LDWS_DEPARTURE_RIGHT)))
            {

                 	Ux_PostEvent(NVTEVT_CB_ADAS_SHOWALARM , 1, ADAS_ALARM_LD);
                    debug_msg("\r\nwarning:ADAS_ALARM_LD\r\n");
        	
            }
			else if((pAdasRlt->FcwsDspRsltInfo.Failure== FCWS_FAILURE_FALSE) && (pAdasRlt->FcwsDspRsltInfo.FCSoundWarning == TRUE))
	        {
	
				Ux_PostEvent(NVTEVT_CB_ADAS_SHOWALARM , 1, ADAS_ALARM_FC);
                debug_msg("\r\nwarning:ADAS_ALARM_FC\r\n");

	        }
        }

	}

    #if (_DRAW_LDFCINFO_ON_OSD_ == ENABLE)
    if (1)
    #else
    if (ADAS_IsAdasDebug())
    #endif
    {
        Ux_PostEvent(NVTEVT_EXE_MOVIE_ALGEND,1, NVTEVT_EXE_MOVIE_ALGEND);
    }
}




PHOTO_FUNC_INFO PhotoFuncInfo_mvldfcdsp =
{
    {
    PHOTO_ID_DSPH,                  ///< function hook process Id.
    IPL_PATH_1,                     ///< IPL Id.
    PHOTO_ISR_IME_FRM_END,          ///< adas dsp tigger event
    Movie_ADASDSPInit,               ///< init adas dsp, only execute at first process
    Movie_ADASDSPProcess,            ///< process adas dsp
    Photo_ADASDSPProcessEnd,         ///< process end adas dsp
    AV_ADASDSP_CalcBuffSize,  ///< get working buffer adas dsp
    NULL, ///< 
    },
    NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif

#endif

//#NT#2016/03/25#KCHong -begin
//#NT#New ADAS
#if MOVIE_DIS || _ADAS_FUNC_ || MOVIE_FTG_FUNC || MOVIE_SCD_FUNC
//#NT#2016/03/25#KCHong -end
#include "af_api.h"
#include "IPL_HeaderInfor_Raw.h"
#include "dis_alg.h"
#include "IPL_Cmd.h"


void Photo_DIS_Init(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    dis_initialize(buf,cachebuf);

    //#NT#2016/04/25#KCHong -begin
    //#NT#Disable FW/ROI dis to decrease CPU usage.
    #if (_ADAS_FUNC_ == ENABLE)
    DISALG_CFGINFO info;
    info.iCfgVal = FALSE;
    dis_SetConfig(DISALG_CFG_GLOBALMOTEN, &info);
    dis_SetConfig(DISALG_CFG_ROIMOTEN, &info);
    //#NT#2016/04/25#KCHong -end
    #endif

    AF_VA_Setup(0);
}

//#NT#2016/05/25#Harry_Tsai -begin
void Photo_DIS(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    IPL_SIE_CUR_FRAME_INFO Info = {0};
#if (_ADAS_FUNC_ != ENABLE)
    DISALG_VECTOR DIS_Offset = {0};
    HEADER_RAW_DATA RawHeaderInfo = {0};
#endif

    //DBG_DUMP("^Gbefore set VA!!!!!!!!!!!!\r\n");
    //#NT#2016/10/18#Jeah Yen -begin
    //#NT#support sensor map
    Info.Id = IPL_PATH_1;
    IPL_GetCmd(IPL_GET_SIE_CUR_FRAME_INFOR, &Info);

    if(Info.FrameCnt < 2)
        return;

#if (_ADAS_FUNC_ != ENABLE)
    //#NT#2016/11/15#Lincy Lin -begin
    //#NT#Work around for FTG with DIS will have some error.
    #if (MOVIE_FTG_FUNC == ENABLE)
	IPL_HeaderRawClrChkStatus (IPL_PATH_1, (HEADER_RAW_STS_IS_OFS));
	#else
    if(IPL_AlgGetUIInfo(IPL_PATH_1, IPL_SEL_DISCOMP) == SEL_DISCOMP_ON)
        IPL_HeaderRawSetChkStatus(IPL_PATH_1, (HEADER_RAW_STS_IS_OFS));
    else
        IPL_HeaderRawClrChkStatus (IPL_PATH_1, (HEADER_RAW_STS_IS_OFS));
	#endif
	//#NT#2016/11/15#Lincy Lin -end

    if (IPL_HeaderRawGetCurInfo_Dly1VD(IPL_PATH_1, &RawHeaderInfo) != E_OK)
    {
        DBG_ERR("DIS: Get header error!!!\r\n");
    }
#endif

    if(IPL_AlgGetUIInfo(IPL_PATH_1, IPL_SEL_DISCOMP) == SEL_DISCOMP_ON)
    {
        dis_process(buf,cachebuf);
#if (_ADAS_FUNC_ != ENABLE)
        dis_getFrameCorrVecPxl(&DIS_Offset,RawHeaderInfo.frame_cnt);
        if(DIS_Offset.vector.iX %2)
            DIS_Offset.vector.iX -= 1;
        if(DIS_Offset.vector.iY%2)
            DIS_Offset.vector.iY -= 1;
#endif
    }
#if (_ADAS_FUNC_ != ENABLE)
    else
    {
        DIS_Offset.vector.iX = 0;
        DIS_Offset.vector.iY = 0;
    }
#endif

#if (_ADAS_FUNC_ != ENABLE)
    switch(IPL_AlgGetUIInfo(IPL_PATH_1, IPL_SEL_IMAGEFLIP))
    {
        case SEL_IMAGEFLIP_DSP_H:
            DIS_Offset.vector.iX = 0-DIS_Offset.vector.iX;
            break;
        case SEL_IMAGEFLIP_DSP_V:
            DIS_Offset.vector.iY = 0-DIS_Offset.vector.iY;
            break;
        case SEL_IMAGEFLIP_DSP_H_V:
            DIS_Offset.vector.iX = 0-DIS_Offset.vector.iX;
            DIS_Offset.vector.iY = 0-DIS_Offset.vector.iY;
            break;
        default:
            break;
    }
    //#NT#2016/08/26#Lincy Lin -begin
    //#NT# Add uctrl command for BC/MV/IPL buffer APIs .
    if(IPL_AlgGetUIInfo(IPL_PATH_1, IPL_SEL_DISCOMP) == SEL_DISCOMP_ON && UI_GetData(FL_MovieDisIndex) == MOVIE_DIS_ON )
    //#NT#2016/08/26#Lincy Lin -end
        IPL_HeaderRawSetInfo_Dly1VD(IPL_PATH_1, IPL_HEADER_RAW_IS_OFS, (void *)&DIS_Offset.vector);
#endif


//#NT#2016/10/17#Bin Xiao -begin
//#NT# Save DIS motion vecter result to FTG buffers.
#if (MOVIE_FTG_FUNC == ENABLE)
    #if 0
    static UINT32 ftg_num1 = 0;
    static UINT32 ftg_time1 = 0;
	static UINT32 ftg_max1 = 0;
    UINT32 t1 = Perf_GetCurrent();
	ftg_savedisrslt();
    UINT32 t2 = Perf_GetCurrent();

	if(ftg_num1 < 1500 && ftg_num1 >= 1000) {
		if(ftg_max1 < t2-t1) {
			ftg_max1 = t2-t1;
		}
		ftg_time1 += t2-t1;
		ftg_num1++;
	} else if(ftg_num1 >= 1000) {
	    DBG_ERR("500 ftg_savedisrslt uses %d us...max %d us\n\r", ftg_time1, ftg_max1);
	} else {
	    ftg_num1++;
	}
	#else
	if (!gFTGLock) {

		ftg_savedisrslt();

	}
	#endif

#endif
//#NT#2016/10/17#Bin Xiao -end



    //IPL_HeaderRawSetInfo(IPL_PATH_1, IPL_HEADER_RAW_IS_OFS, (void *)&DIS_Offset.vector);
    //DBG_ERR("cnt=%d,(%d,%d)%d %d\r\n",Info.FrameCnt,DIS_Offset.vector.iX,DIS_Offset.vector.iY,DIS_Offset.frame_cnt,RawHeaderInfo.frame_cnt);
    //#NT#2016/10/18#Jeah Yen -end
}
//#NT#2016/05/25#Harry_Tsai -end

//
PHOTO_FUNC_INFO PhotoFuncInfo_dis =
{
    {
    PHOTO_ID_1,             ///< function hook process Id.
    //#NT#2016/10/18#Jeah Yen -begin
    //#NT#support sensor map
    IPL_PATH_1,               ///< IPL Id.
    //#NT#2016/10/18#Jeah Yen -end
//#NT#2016/05/04#Harry_Tsai -begin
//#NT# Change to VD
    PHOTO_ISR_SIE_VD,       ///< isr tigger event
//#NT#2016/05/04#Harry_Tsai -end
    Photo_DIS_Init,                   ///< init fp, only execute at first process
    Photo_DIS,           ///< process fp
    NULL,                   ///< process end fp
    dis_GetPrvMaxBuffer,        ///< get working buffer fp
    NULL,                   ///< get working cache buffer fp
    },
    NULL  ///< point next Function Obj, last Function must be set NULL
};
#else
PHOTO_FUNC_INFO PhotoFuncInfo_dis = {0};
#endif

//#NT#2016/03/02#Lincy Lin -begin
//#NT#Support object tracking function
#if MOVIE_OT_FUNC
void Movie_OTInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    NvtOt_Init(buf);
}

void Movie_OTProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{

    IMG_BUF          InputImg={0};
    ISF_PORT* pSrc = ImageUnit_Out(&ISF_ImagePipe1, ISF_OUT3);
    UINT32           ImageRatioIdx;
    OT_UIMGSIZE      InputImgSize;

    ImageRatioIdx = GetMovieSizeRatio(UI_GetData(FL_MOVIE_SIZE));

    if(pSrc && !(pSrc->Flag & ISF_PORT_FLAG_PAUSE))
    {
        ImageUnit_SetParam(&ISF_ImagePipe1,IMAGEPIPE_PARAM_PULLBUF_OUT3, (UINT32)&InputImg);

        InputImgSize.width = InputImg.Width;
        InputImgSize.height = InputImg.Height;
        InputImgSize.lineoffset = InputImg.LineOffset[0];
        InputImgSize.addr = InputImg.PxlAddr[0];
        InputImgSize.imgRatio = ImageRatioIdx;


        if(OT_SetImgSize( &InputImgSize)==FALSE)  //set should be done for every frame.
            return;

        //#NT#2016/03/22#M B Wang -begin
        //#NT#set ALG version and relevant property according to macro definition
        //#NT# you can choose either below codes or sxcmd to config property, but not both.
//	        #if OT_SINGLE_SCALE
//	        OT_SetConfig(OT_CFG_ALG_VERSION, 0);//0: single-scale, 1: multi-scale, 2: similar-object. default is multiScale.
//	        OT_SetConfig(OT_CFG_FEATURE_TYPE,3); //0: OT_CELL4_BIN9, 1: OT_CELL8_BIN4.
//	        #elif OT_MULTI_SCALE
//	        OT_SetConfig(OT_CFG_ALG_VERSION, 1);
//	        OT_SetConfig(OT_CFG_FEATURE_TYPE,0);
//	        #endif
        //#NT#2016/03/22#M B Wang -end



       NvtCv_ot_process();


        //#NT#2016/06/08#Lincy Lin -begin
        //#NT#Implement generic OSD and video drawing mechanism for ALG function
        Ux_PostEvent(NVTEVT_EXE_MOVIE_ALGEND,1, NVTEVT_EXE_MOVIE_ALGEND);
        //#NT#2016/06/08#Lincy Lin -end
    }
}
PHOTO_FUNC_INFO PhotoFuncInfo_mvot =
{
    {
    PHOTO_ID_DSPH,          ///< function hook process Id.
    //#NT#2016/10/18#Jeah Yen -begin
    //#NT#support sensor map
    IPL_PATH_1,               ///< IPL Id.
    //#NT#2016/10/18#Jeah Yen -end
    PHOTO_ISR_IME_FRM_END,  ///< isr tigger event
    Movie_OTInit,           ///< init fp, only execute at first process
    Movie_OTProcess,        ///< process fp
    NULL,                   ///< process end fp
    OT_CalcBuffSize,        ///< get working buffer fp
    NULL,//OT_CalcCacheBuffSize,   ///< get working cache buffer fp
    },
    NULL  ///< point next Function Obj, last Function must be set NULL
};

#endif
//#NT#2016/03/02#Lincy Lin -end

//#NT#2016/05/20#Yang Jin -begin
//#NT#Support Dis dsp function
#if MOVIE_DISDSP_FUNC
void Movie_DISDSPInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    DisDsp_Init(buf);
}

void Movie_DISDSPProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{

    IMG_BUF   InputImg={0};
    ISF_PORT* pSrc = ImageUnit_Out(&ISF_ImagePipe1, ISF_OUT3);
    DIS_DSP_ROIGMV_OUT roiOut;
    UIMGSIZE InputImgSize = {0};

    if(pSrc && !(pSrc->Flag & ISF_PORT_FLAG_PAUSE))
    {
        ImageUnit_SetParam(&ISF_ImagePipe1,IMAGEPIPE_PARAM_PULLBUF_OUT3, (UINT32)&InputImg);

        InputImgSize.width = InputImg.Width;
        InputImgSize.height = InputImg.Height;
        InputImgSize.lineoffset = InputImg.LineOffset[0];
        InputImgSize.addr = InputImg.PxlAddr[0];

        DisDsp_ImgSize_SetInputInfo(&InputImgSize);
        NvtCv_disdsp_process( );

        roiOut = DisDsp_Roi_GetResult();
        DBG_ERR("x = %d\t y = %d\t valid = %d\n\r", roiOut.vector.iX, roiOut.vector.iY, roiOut.bValidVec);
    }
}
PHOTO_FUNC_INFO PhotoFuncInfo_mvdisdsp=
{
    {
    PHOTO_ID_DSPH,          ///< function hook process Id.
    //#NT#2016/10/18#Jeah Yen -begin
    //#NT#support sensor map
    IPL_PATH_1,               ///< IPL Id.
    //#NT#2016/10/18#Jeah Yen -end
    PHOTO_ISR_IME_FRM_END,  ///< isr tigger event
    Movie_DISDSPInit,       ///< init fp, only execute at first process
    Movie_DISDSPProcess,    ///< process fp
    NULL,                   ///< process end fp
    DisDsp_CalcBuffSize,    ///< get working buffer fp
    NULL,                   ///< get working cache buffer fp
    },
    NULL  ///< point next Function Obj, last Function must be set NULL
};

#endif
//#NT#2016/05/20#Yang Jin -end



//#NT#2016/11/16#ML Cui -begin
//#NT# The FD for Driver Drowsiness Detection(DDDFD)
#if MOVIE_DDD_FD_FUNC
#include "DDD_fd.h"

void Movie_DDDFdInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    //GET_EXTERN_STU(FD_MODEL_NAME).pfInit(buf, cachebuf, 3);
    debug_msg("DDDFD Init\r\n");
    FD_FUNC(DDDFd)->pfInit(buf, cachebuf);
    FD_FUNC(DDDFd)->pfSetParam(FD_PARAM_USE_HISTORY, TRUE);
    FD_FUNC(DDDFd)->pfSetParam(FD_PARAM_MAX_RSLT, 2);
    FD_FUNC(DDDFd)->pfSetParam(FD_PARAM_ADJ_CONTRAST, FALSE);
}

void Movie_DDDFdProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    IMG_BUF InputImg = {0};
    UINT32 myDualCam;
    ISF_PORT* pSrc;
    ISF_UNIT* pUnit;

    static UINT32 index = 0;

    if(index++ % 3 != 0)
        return;

    #if (SENSOR_CAPS_COUNT == 1)
    if(System_GetEnableSensor() == SENSOR_1)
        myDualCam = DUALCAM_FRONT;
    else
        myDualCam = DUALCAM_BEHIND;
    #elif (SENSOR_CAPS_COUNT == 2)
    if(System_GetEnableSensor() == SENSOR_1 || System_GetEnableSensor() == SENSOR_2)
    {//only enable one sensor
        if(System_GetEnableSensor() == SENSOR_1)
            myDualCam = DUALCAM_FRONT;
        else
            myDualCam = DUALCAM_BEHIND;
    }
    else
    {//enable dual sensors
        myDualCam = DUALCAM_BEHIND;
    }
    #else
    #error "Undefined case"
    #endif

    if (myDualCam == DUALCAM_FRONT)
        pUnit = &ISF_ImagePipe1;
    else
        pUnit = &ISF_ImagePipe2;

    #if (_MODEL_DSC_ == _MODEL_PWA6_668_)
    pSrc = ImageUnit_Out(pUnit, ISF_OUT2);
    #else
    pSrc = ImageUnit_Out(pUnit, ISF_OUT4);




    #endif
    if(pSrc && !(pSrc->Flag & ISF_PORT_FLAG_PAUSE))
    {
        #if (_MODEL_DSC_ == _MODEL_PWA6_668_)
        ImageUnit_SetParam(pUnit, IMAGEPIPE_PARAM_PULLBUF_OUT2, (UINT32)&InputImg);
        #else
        ImageUnit_SetParam(pUnit,IMAGEPIPE_PARAM_PULLBUF_OUT4, (UINT32)&InputImg);
        #endif

        FD_FUNC(DDDFd)->pfProcess(&InputImg);

    }


}

static UINT32 Movie_DDDFdGetBuf(VOID)
{
    return FD_FUNC(DDDFd)->pfCalcBuffSize();
}

static UINT32 Movie_DDDFdGetCache(VOID)
{
    return FD_FUNC(DDDFd)->pfCalcCacheBuffSize();
}

static VOID Movie_DDDFdEnd(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    Ux_PostEvent(NVTEVT_EXE_MOVIE_ALGEND,1, NVTEVT_EXE_MOVIE_ALGEND);
}

PHOTO_FUNC_INFO PhotoFuncInfo_mvdddfd =
{
    {
    PHOTO_ID_2,                 ///< function hook process Id.
    IPL_PATH_1,                   ///< IPL Id.
    PHOTO_ISR_IME_FRM_END,      ///< isr tigger event
    Movie_DDDFdInit,           ///< init tsr, only execute at first process
    Movie_DDDFdProcess,        ///< process tsr
    Movie_DDDFdEnd,            ///< process end tsr
    Movie_DDDFdGetBuf,       ///< get working buffer tsr
    Movie_DDDFdGetCache,  ///< get working cache buffer tsr
    },
    NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif
//#NT#2016/11/16#ML Cui -end


//#NT#2016/04/22#Nestor Yang -begin
//#NT# Support Driver Drowsiness Detection(DDD)
#if MOVIE_DDD_FUNC
#define DDD_USING_FD_OR_NOT DISABLE
#if DDD_USING_FD_OR_NOT
#include "NvtFd.h"
#include "fde_lib.h"
static UINT32 g_pFdHandleBuf = 0;
#endif
#if !(MOVIE_FD_FUNC_ || MOVIE_DDD_FD_FUNC)
#error "DDD need enable MOVIE_FD_FUNC_ or MOVIE_DDD_FD_FUNC"
#endif

#include "DDD_fd.h"

static INT32 Movie_DDDGetFdRslt(const IMG_BUF* pImgBuf, RECT* pFdRect)
{
#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif
    INT32 iFaceNum;
#if DDD_USING_FD_OR_NOT
    if(pFdRect->width != 0 && g_pFdHandleBuf)
    {
        FACE face = {0};

        face.uiStx = pFdRect->x;
        face.uiSty = pFdRect->y;
        face.uiWidth = pFdRect->width;
        face.uiHeight = pFdRect->height;
        face.uiTrgIdx = FDE_R000_P000;

        UINT32 t1 = Perf_GetCurrent();
        iFaceNum = NvtFD_DetectFaceOrNot(pImgBuf, &face, 1, g_pFdHandleBuf);

        if(iFaceNum)
        {
//            debug_msg("Front time : %d\r\n", Perf_GetCurrent() - t1);
            return iFaceNum;
        }
        else
        {
            INT32 iShake = DDD_GetPoseAngleShake();
            //The FD's Left and Right is opposite to DDD's
            if(iShake > 200)
                face.uiTrgIdx = FDE_R000_PR90;
            else if(iShake < -200)
                face.uiTrgIdx = FDE_R000_PL90;
            iFaceNum = NvtFD_DetectFaceOrNot(pImgBuf, &face, 1, g_pFdHandleBuf);
            if(iFaceNum)
            {

                debug_msg("Profile %d time : %d\r\n", face.uiTrgIdx, Perf_GetCurrent() - t1);
                return iFaceNum;
            }

        }
        debug_msg("recall FD wast time : %d \r\n");
    }

#endif

#if MOVIE_DDD_FD_FUNC
    FACE faceRslt;
    iFaceNum = FD_FUNC(DDDFd)->pfGetFDResult(&faceRslt, 1, FD_SORT_BY_SIZE);

    if(iFaceNum > 0)
    {
        pFdRect->x = (INT32)faceRslt.uiStx;
        pFdRect->y = (INT32)faceRslt.uiSty;
        pFdRect->width = (INT32)faceRslt.uiWidth;
        pFdRect->height = (INT32)faceRslt.uiHeight;
    }
#else
    iFaceNum = FD_GetRsltFaceNum();
    if(iFaceNum >= 0)
    {
        static FD_FACE faceRslt[FD_NUM_MAX] = {0};
#if 1
        URECT DispCord;
        DispCord.x = 0;
        DispCord.y = 0;
        DispCord.w = pImgBuf->Width;
        DispCord.h = pImgBuf->Height;
        FD_GetRsltFace(faceRslt, &DispCord);
        pFdRect->height = pFdRect->width = MAX(faceRslt[0].w, faceRslt[0].h);
        pFdRect->x = faceRslt[0].x + faceRslt[0].w / 2 - pFdRect->width / 2;
        pFdRect->y = faceRslt[0].y + faceRslt[0].h / 2 - pFdRect->height / 2;
#else

        FD_GetRsltFace(faceRslt, 0);
        pFdRect->x = faceRslt[0].x * pImgBuf->Width / 640;
        pFdRect->y = faceRslt[0].y * pImgBuf->Height / 360;
        pFdRect->width = MAX(faceRslt[0].w * pImgBuf->Width / 640,
                             faceRslt[0].h * pImgBuf->Height / 360);
        pFdRect->height = pFdRect->width;
#endif
    }
#endif



    return iFaceNum;
}
//#NT#2016/11/10#Brain Yen -begin
//#NT#Add for save DDD MODEL in pstore
#if DDD_MODEL_IN_PSTORE
UINT32 Movie_DDDCheckPSFormat(void)
{
    #define SETTING_SIZE 30
    #define FORMAT_CHK_STRING  "format="

    UINT32      uiReadSize = SETTING_SIZE;
    UINT32      uiUpdateChk;
    CHAR        cDataBuf[SETTING_SIZE]={0}, *pCh;
    FST_FILE    pFile;

    pFile = FileSys_OpenFile("A:\\ddd_psformat", FST_OPEN_READ);
    if (pFile)
    {
        FileSys_ReadFile(pFile, (UINT8 *)cDataBuf, &uiReadSize, 0, NULL);
        FileSys_CloseFile(pFile);
        //DBG_DUMP("Read size %d\r\n", uiReadSize);
        if (uiReadSize)
        {
            pCh = strstr(cDataBuf, FORMAT_CHK_STRING);
            sscanf(pCh + sizeof(FORMAT_CHK_STRING) - 1, "%d", &uiUpdateChk);
            DBG_DUMP("Movie_DDDCheckPSFormat FormatChk = %d, size %d\r\n", uiUpdateChk, sizeof(FORMAT_CHK_STRING));
            return uiUpdateChk;
        }
    }
    else
    {
        //DBG_ERR("Open ddd_psformat check file error!\r\n");
    }
    return 0;

}
UINT32 Movie_DDDCheckDBUpdate(void)
{
    #define SETTING_SIZE 30
    #define UPDATE_CHK_STRING  "update="

    UINT32      uiReadSize = SETTING_SIZE;
    UINT32      uiUpdateChk;
    CHAR        cDataBuf[SETTING_SIZE]={0}, *pCh;
    FST_FILE    pFile;

    pFile = FileSys_OpenFile("A:\\ddd_update", FST_OPEN_READ);
    if (pFile)
    {
        FileSys_ReadFile(pFile, (UINT8 *)cDataBuf, &uiReadSize, 0, NULL);
        FileSys_CloseFile(pFile);
        //DBG_DUMP("Read size %d\r\n", uiReadSize);
        if (uiReadSize)
        {
            pCh = strstr(cDataBuf, UPDATE_CHK_STRING);
            sscanf(pCh + sizeof(UPDATE_CHK_STRING) - 1, "%d", &uiUpdateChk);
            DBG_DUMP("Movie_DDDCheckDBUpdate UpdateChk = %d, size %d\r\n", uiUpdateChk, sizeof(UPDATE_CHK_STRING));
            return uiUpdateChk;
        }
    }
    else
    {
        //DBG_ERR("Open ddd_update check file error!\r\n");
    }
    return 0;

}
#endif
//#NT#2016/11/10#Brain Yen -end
UINT32 Movie_DDDGetDBFileSizeFrCard(void)
{
    FST_FILE filehdl = NULL;
    UINT32 ui32FileSize = 0;

    CHAR* pBinaryFileName = "B:\\DDD_MODEL\\"DDD_MODEL_NAME;
    filehdl = FileSys_OpenFile(pBinaryFileName, FST_OPEN_READ);
    if(!filehdl)
    {
        CHAR* pBinaryFileName2 = "A:\\DDD_MODEL\\"DDD_MODEL_NAME;
        DBG_DUMP("DDD load model from SD-card!\r\n");
        filehdl = FileSys_OpenFile(pBinaryFileName2, FST_OPEN_READ);
    }

    if(filehdl)
    {
        // Get file size info
        FileSys_SeekFile(filehdl, 0, FST_SEEK_END);
        ui32FileSize = (UINT32)FileSys_TellFile(filehdl);
        //DBG_DUMP("ui32FileSize=0x%x\r\n",ui32FileSize);
        FileSys_SeekFile(filehdl, 0, FST_SEEK_SET);

        FileSys_CloseFile(filehdl);
    }
    else
    {
        DBG_ERR("DDD_Init Failed: load model failed!\r\n");
        return 0;
    }
    return ui32FileSize;
}
//#NT#2016/05/11#Isiah Chang -begin
//#NT# Decrypt DDD database file and then feed it into DDD_Init().
UINT32 Movie_DDDGetDBFileSize(void)
{
    UINT32 ui32FileSize = 0;
#if DDD_MODEL_IN_PSTORE
    PPSTORE_SECTION_HANDLE  pSection;
    UINT32  result;
    if ((pSection = PStore_OpenSection(PS_DDD_MODEL1_SIZE, PS_RDWR | PS_CREATE)) != E_PS_SECHDLER)
    {
        result = PStore_ReadSection((UINT8 *)&ui32FileSize, 0, sizeof(UINT32), pSection);
        DBG_IND("result=%d, ui32FileSize=0x%x\r\n",result,ui32FileSize);
        if (ui32FileSize==0)
        {
            DBG_ERR("PStore Save default DDD Model Size\r\n");
            ui32FileSize=Movie_DDDGetDBFileSizeFrCard();
            DBG_ERR("ui32FileSize=%d\r\n",ui32FileSize);
            PStore_WriteSection((UINT8 *)&ui32FileSize,0,sizeof(UINT32),pSection);
            PStore_CloseSection(pSection);
        }
        else
        {
            DBG_DUMP("PStore Load DDD Model Size OK\r\n");
            PStore_CloseSection(pSection);
        }
    }
    else
    {
        DBG_ERR("PStore Load DDD Model Size Fail!\r\n");
        return 0;
    }
#else
    ui32FileSize=Movie_DDDGetDBFileSizeFrCard();
#endif
    return ui32FileSize;
}
//#NT#2016/05/11#Isiah Chang -end
void Movie_DDDGetDBFile(UINT8* pBinaryMap, UINT32* uiBuflen)
{
    //load Landmark Model
    FST_FILE filehdl = NULL;

    CHAR* pBinaryFileName = "B:\\DDD_MODEL\\"DDD_MODEL_NAME;
    filehdl = FileSys_OpenFile(pBinaryFileName, FST_OPEN_READ);
    if(!filehdl)
    {
        CHAR* pBinaryFileName2 = "A:\\DDD_MODEL\\"DDD_MODEL_NAME;
        DBG_DUMP("DDD load model from SD-card!\r\n");
        filehdl = FileSys_OpenFile(pBinaryFileName2, FST_OPEN_READ);
    }

    if(filehdl)
    {
        FileSys_ReadFile(filehdl, (UINT8*)pBinaryMap,
                     uiBuflen, 0, NULL);
        FileSys_CloseFile(filehdl);
    }
    else
    {
        DBG_ERR("DDD_Init Failed: load model failed!\r\n");
        return;
    }
}
void Movie_DDDInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
//#NT#2016/05/11#Isiah Chang -begin
//#NT# Decrypt DDD database file and then feed it into DDD_Init().
//#NT#2016/05/04#ML Cui -begin
//#NT# DDD will load model from an addr that generate by system.

    #if 1
#if (DDD_MODEL_IN_PSTORE)
    if(Movie_DDDCheckPSFormat()==1)
    {
        DBG_ERR("PStore format!\r\n");
        System_PS_Format();
    }
#endif
    ER bRetval;
    UINT32 pBinaryMap = buf->Addr;
    UINT32 uiDBFileSize = Movie_DDDGetDBFileSize();
    UINT32 uiBuflen = ALIGN_CEIL_16(uiDBFileSize);
#if (DDD_MODEL_IN_PSTORE)
    UINT32 uiUpdateDBChk=Movie_DDDCheckDBUpdate();
#endif
    if(uiDBFileSize > DDD_MAX_DB_FILE_SIZE)
    {
        DBG_ERR("DDD DB file size is too large!\r\n");
        return;
    }

    pBinaryMap = ALIGN_CEIL_4(pBinaryMap);//Input address must algin words
    if(buf->Size < uiBuflen + 4)
    {
        DBG_ERR("memory buf is (%d) not enough!\r\n", buf->Size);
    }
//#NT#2016/11/10#Brain Yen -begin
//#NT#Add for save DDD MODEL in pstore
#if (DDD_MODEL_IN_PSTORE==DISABLE)
    //init Model
    {//load Landmark Model
        FST_FILE filehdl = NULL;

        CHAR* pBinaryFileName = "B:\\DDD_MODEL\\"DDD_MODEL_NAME;
        filehdl = FileSys_OpenFile(pBinaryFileName, FST_OPEN_READ);
        if(!filehdl)
        {
            CHAR* pBinaryFileName2 = "A:\\DDD_MODEL\\"DDD_MODEL_NAME;
            DBG_DUMP("DDD load model from SD-card!\r\n");
            filehdl = FileSys_OpenFile(pBinaryFileName2, FST_OPEN_READ);
        }

        if(filehdl)
        {
            FileSys_ReadFile(filehdl, (UINT8*)pBinaryMap,
                         &uiBuflen, 0, NULL);
            FileSys_CloseFile(filehdl);
        }
        else
        {
            DBG_ERR("DDD_Init Failed: load model failed!\r\n");
            return;
        }
    }
#else
    PPSTORE_SECTION_HANDLE  pSection;
    UINT32  result;

    uiBuflen=ALIGN_CEIL_16(uiDBFileSize);
    if ((pSection = PStore_OpenSection(PS_DDD_MODEL1_DATA, PS_RDWR | PS_CREATE)) != E_PS_SECHDLER)
    {
        result = PStore_ReadSection((UINT8 *)pBinaryMap, 0, uiBuflen, pSection);
        if (result != E_PS_OK || uiUpdateDBChk)
        {
            DBG_ERR("Save DDD Model Data in PStore\r\n");
            //load Landmark Model
            Movie_DDDGetDBFile((UINT8 *)pBinaryMap, &uiBuflen);
            PStore_WriteSection((UINT8 *)pBinaryMap,0,uiBuflen,pSection);
            PStore_CloseSection(pSection);
        }
        else
        {
            DBG_DUMP("PStore Load DDD Model Data OK\r\n");
            PStore_CloseSection(pSection);
        }
    }
    else
    {
        DBG_ERR("3.DDD_Init Failed: load model failed!\r\n");
        return;
    }
#endif
//#NT#2016/11/10#Brain Yen -end
    // Decrypt DDD database.
    buf->Size -= uiBuflen + 4;
    buf->Addr += uiBuflen + 4;
    crypto_open();
    bRetval = crypto_decryption(pBinaryMap, pBinaryMap, uiBuflen);
    crypto_close();
    #if DDD_USING_FD_OR_NOT
    g_pFdHandleBuf = buf->Addr;
    buf->Size -= NvtFD_GetBuffSize();
    buf->Addr += NvtFD_GetBuffSize();
    #endif

    if(bRetval != E_OK)
    {
        DBG_ERR("DDD decrypt failed!\r\n");
        return;
    }
    #else
    UINT32 pBinaryMap = DDD_GetBinaryMap(&tmpBuf);//this func will be replaced.
    #endif

    if(DDD_Init(buf, cachebuf, pBinaryMap,
                 Movie_DDDGetFdRslt) == DDD_STA_OK)
    {
        DBG_DUMP("DDD init success\r\n");
        if(DDD_LoadParam("A:\\DDD_config.txt"))
            DBG_DUMP("DDD load param from SD card\r\n");
        else
            DBG_DUMP("DDD using default param\r\n");

        if(DDD_LoadUserParam("A:\\ddd_user_config.txt"))
            DBG_DUMP("DDD load user param from SD card\r\n");
        else
            DBG_DUMP("DDD using default user param\r\n");
    }
    else
        DBG_ERR("DDD init failed!\r\n");
//#NT#2016/05/04#ML Cui -end
//#NT#2016/05/11#Isiah Chang -end
}

void Movie_DDDProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    IMG_BUF InputImg = {0};
    UINT32 myDualCam;
    ISF_PORT* pSrc;
    ISF_UNIT* pUnit;
    static BOOL bNeedClean = FALSE;
//CHKPNT;
    #if (SENSOR_CAPS_COUNT == 1)
    if(System_GetEnableSensor() == SENSOR_1)
        myDualCam = DUALCAM_FRONT;
    else
        myDualCam = DUALCAM_BEHIND;
    #elif (SENSOR_CAPS_COUNT == 2)
    if(System_GetEnableSensor() == SENSOR_1 || System_GetEnableSensor() == SENSOR_2)
    {//only enable one sensor
        if(System_GetEnableSensor() == SENSOR_1)
            myDualCam = DUALCAM_FRONT;
        else
            myDualCam = DUALCAM_BEHIND;
    }
    else
    {//enable dual sensors
        myDualCam = DUALCAM_BEHIND;
    }
    #else
    #error "Undefined case"
    #endif

    if (myDualCam == DUALCAM_FRONT)
        pUnit = &ISF_ImagePipe1;
    else
        pUnit = &ISF_ImagePipe2;
//#NT#2016/05/03#ML Cui -begin
//Make DDD support for PWA6_668

    #if (_MODEL_DSC_ == _MODEL_PWA6_668_)
    pSrc = ImageUnit_Out(pUnit, ISF_OUT2);
    #else
    //#NT#2016/07/19#ML Cui -Begin
    //#NT# DDD Changed input from IME2 Path3 to Path4
    pSrc = ImageUnit_Out(pUnit, ISF_OUT4);

    //if(!pSrc)
    //{
      //  DBG_ERR("Get input failed!\r\n");
//        return;
    //}
    //#NT#2016/07/19#ML Cui -End
    #endif

    if(pSrc && !(pSrc->Flag & ISF_PORT_FLAG_PAUSE))
    {
        #if (_MODEL_DSC_ == _MODEL_PWA6_668_)
        ImageUnit_SetParam(pUnit, IMAGEPIPE_PARAM_PULLBUF_OUT2, (UINT32)&InputImg);
        #else
        ImageUnit_SetParam(pUnit, IMAGEPIPE_PARAM_PULLBUF_OUT4, (UINT32)&InputImg);
        #endif
//#NT#2016/05/03#ML Cui -end
        DDD_Process(&InputImg);
        if(!bNeedClean)
            bNeedClean = TRUE;
    }
    else
    {
        //we should clean the DDD result here to prevent OSD keep redrawing the landmark
        if(bNeedClean)
        {
            DDD_ClearRsltMsg();
            bNeedClean = FALSE;
        }
    }
}



//Warning frequency will be more and more frequently then bState is always true
#define DDD_SET_WRN_STATE(bFlag, bState, maxFreq, minFreq, rate) \
    static UINT32 ui##bFlag##Frame = 0;   \
    static UINT32 ui##bFlag##Freq = minFreq;  \
    static BOOL   bFlag = FALSE;    \
    Movie_DDDSetWarnForStatistic(bState, &ui##bFlag##Frame, &ui##bFlag##Freq, \
                &bFlag, maxFreq, minFreq, rate)

static void Movie_DDDSetWarnForStatistic(BOOL bState,
                                         UINT32 *uiWarnFrame, UINT32 *uiWarnFreq, BOOL *bWarn,
                                         UINT32 uiMaxFreq, UINT32 uiMinFreq, FLOAT rate)
{

    //debug_msg("uiWarnFrame:%d\r\n", *uiWarnFrame);
    if(bState)
    {

        (*uiWarnFrame)++;
        if(*uiWarnFrame == 1)
        {
            *bWarn = TRUE;
        }
        else if((*uiWarnFrame) % (*uiWarnFreq) == 0)
        {
            *bWarn = TRUE;
            *uiWarnFreq /= rate;
            if(*uiWarnFreq < uiMinFreq)
                *uiWarnFreq = uiMinFreq;
        }
    }
    else
    {
        *uiWarnFrame = 0;
        *uiWarnFreq = uiMaxFreq;
        *bWarn = FALSE;
    }

}

//#NT#2016/07/20#Brain Yen -begin
//#NT#Add for DDD ararm
void Movie_DDDProcessEnd(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    RECT trackRect;
    DDD_WARN dowsyState;
    UINT32 WarnEvent = 0;
    static UINT32 PreWarnEvent=DDD_UWRN_NORM;
    static UINT16 EventCnt=0;
    if(DDD_GetTrackingRect(&trackRect))
    {

        dowsyState = DDD_GetUserWarning();

        DDD_SET_WRN_STATE(bYawnWarn, (dowsyState & (DDD_UWRN_YAWN | DDD_UWRN_YAWN_DURA)), 500, 50, 2.0);
        DDD_SET_WRN_STATE(bPerWarn,  (dowsyState & (DDD_UWRN_BLINK | DDD_UWRN_EYE)), 150, 30, 1.2);
        DDD_SET_WRN_STATE(bNodeWarn, (dowsyState & (DDD_UWRN_NOD | DDD_UWRN_NOD_DURA)),  500, 50, 2.0);
        DDD_SET_WRN_STATE(bDisWarn, (dowsyState & (DDD_UWRN_DIS | DDD_UWRN_DIS_DURA)),  500, 50, 2.0);


        if(dowsyState)
        {
            if(dowsyState & DDD_UWRN_NOFACE)
            {
                EventCnt=0;
                PreWarnEvent=DDD_UWRN_NOFACE;
            }
            else
            {
                if(bDisWarn)
                {
                    DBG_IND("Serious Warning! Driver distraction!\r\n");
                    WarnEvent=DDD_ALARM_DIS;
                    bPerWarn = FALSE;
                }
                else if((dowsyState & DDD_UWRN_EYE_DURA))
                {
                    DBG_IND("Serious Warning! Driver's eye closed too long!\r\n");
                    WarnEvent=DDD_ALARM_EYE;
                }
                else if(bPerWarn)
                {
                    DBG_IND("Warning! Driver drossy!!\r\n");
                    WarnEvent=DDD_ALARM_PERCLOS;
                    bPerWarn = FALSE;
                }
                else if(bYawnWarn)
                {
                    DBG_IND("Remind~ driver yawn too frequently\r\n");
                    WarnEvent=DDD_ALARM_YAWN;
                    bYawnWarn = FALSE;
                }
                else if(bNodeWarn)
                {
                    DBG_IND("Annotation! Driver is a little tired!\r\n");
                    WarnEvent=DDD_ALARM_NODE;
                    bNodeWarn = FALSE;
                }
                if(PreWarnEvent == WarnEvent)
                    EventCnt++;
                else
                    EventCnt=0;
                if((EventCnt%20)==0)
                {
                   Ux_PostEvent(NVTEVT_CB_DDD_SHOWALARM , 1, WarnEvent);
                }
                PreWarnEvent = WarnEvent;
            }
        }
        else
        {
            EventCnt=0;
            PreWarnEvent=DDD_UWRN_NORM;
        }
    }

    #if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
    #if (MOVIE_DDD_SAVE_LOG == ENABLE)

    {
        UINT32 uiLogSize;
        UINT32 uiLogAddr;
        uiLogSize = DDD_GetLog(&uiLogAddr);
//        debug_msg("uiLogSize: %d\r\n", uiLogSize);
        if(uiLogSize)
        {
            mp4log_AddLog(DbgLogDDDID, (CHAR*)uiLogAddr, uiLogSize);
        }
    }
    #endif
    #endif
}
#undef DDD_SET_WRN_STATE
//#NT#2016/07/20#Brain Yen -end
//#NT#2016/05/11#Isiah Chang -begin
//#NT# Decrypt DDD database file and then feed it into DDD_Init().
UINT32 Movie_DDDCalcBufWithBinaryMap(VOID)
{
    UINT32 uiDBFileSize = Movie_DDDGetDBFileSize();
    if(uiDBFileSize==0)
        uiDBFileSize=Movie_DDDGetDBFileSizeFrCard();
    DBG_DUMP("^Yddd filesize:%d\r\n", uiDBFileSize);
    if(uiDBFileSize > DDD_MAX_DB_FILE_SIZE)
    {
        DBG_ERR("DDD DB file size is too large!\r\n");
        return 0;
    }
    #if DDD_USING_FD_OR_NOT
    return NvtFD_GetBuffSize() + DDD_CalcBuffSize() + uiDBFileSize + 4;
    #else
    return DDD_CalcBuffSize() + uiDBFileSize + 4;
    #endif
}

PHOTO_FUNC_INFO PhotoFuncInfo_mvddd =
{
    {
    PHOTO_ID_DSPH,          ///< function hook process Id.
    //#NT#2016/10/18#Jeah Yen -begin
    //#NT#support sensor map
    IPL_PATH_1,               ///< IPL Id.
    //#NT#2016/10/18#Jeah Yen -end
    PHOTO_ISR_IME_FRM_END,  ///< isr tigger event
    Movie_DDDInit,          ///< init fp, only execute at first process
    Movie_DDDProcess,       ///< process fp
    Movie_DDDProcessEnd,  ///< process end fp
    //NULL,                   ///< process end fp
    Movie_DDDCalcBufWithBinaryMap,///< get working buffer fp
    DDD_CalcCacheBuffSize,  ///< get working cache buffer fp
    },
    NULL                    ///< point next Function Obj, last Function must be set NULL
};
//#NT#2016/05/11#Isiah Chang -end
#endif
//#NT#2016/04/22#Nestor Yang -end

//#NT#2016/05/27#David Tsai -begin
//#NT#Support tampering detection and background capture function
//#NT#2016/10/14#Yuzhe Bian -begin
//#NT# Support trip-wire detection & trip-zone detection function
#if MOVIE_BC_FUNC || MOVIE_TD_FUNC || MOVIE_TWD_FUNC || MOVIE_TZD_FUNC
//#NT#2016/10/14#Yuzhe Bian -end
void Movie_BCLock(BOOL bLock)
{
    gBCLock = bLock;
}

void Movie_BCProcessRateSet(UINT32 ProcessRate)
{
    gBCProcessRate = ProcessRate;
}

UINT32 Movie_BCProcessRateGet(void)
{
    return gBCProcessRate;
}

void Movie_BCInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    BC_Init(buf->Addr, cachebuf->Addr);
}

void Movie_BCProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    IMG_BUF          InputImg;
    UINT32           ImageRatioIdx, myDualCam;
    ISF_UNIT*        pUnit;

    if (gBCLock == TRUE)
        return;

    gBCCounter++;
    if (gBCCounter >= gBCProcessRate)
    {
        gBCCounter = 0;
        gBCUpdateCounter++;
    }
    else
    {
        return;
    }

    #if (SENSOR_CAPS_COUNT == 1)
    if(System_GetEnableSensor() == (SENSOR_1) )
        myDualCam = DUALCAM_FRONT;
    else
        myDualCam = DUALCAM_BEHIND;
    #endif
    #if (SENSOR_CAPS_COUNT == 2)
    if(System_GetEnableSensor() == (SENSOR_1) || System_GetEnableSensor() == (SENSOR_2))
    {
        if(System_GetEnableSensor() == (SENSOR_1) )
            myDualCam = DUALCAM_FRONT;
        else
            myDualCam = DUALCAM_BEHIND;
    }
    else
    {
        myDualCam = UI_GetData(FL_DUAL_CAM);
    }
    #endif

    ImageRatioIdx = GetMovieSizeRatio(UI_GetData(FL_MOVIE_SIZE));
    if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH)
        pUnit = &ISF_ImagePipe1;
    else
        pUnit = &ISF_ImagePipe2;
    ImageUnit_SetParam(pUnit,IMAGEPIPE_PARAM_PULLBUF_OUT2, (UINT32)&InputImg);
    //GxImg_DumpBuf("A:\\test.raw",&InputImg,GX_IMAGE_DUMP_ALL);

    BC_Process(&InputImg);

}

PHOTO_FUNC_INFO PhotoFuncInfo_mvbc =
{
    {
    PHOTO_ID_2,             ///< function hook process Id.
    //#NT#2016/10/18#Jeah Yen -begin
    //#NT#support sensor map
    IPL_PATH_1,               ///< IPL Id.
    //#NT#2016/10/18#Jeah Yen -end
    PHOTO_ISR_IME_FRM_END,  ///< isr tigger event
    Movie_BCInit,           ///< init fp, only execute at first process
    Movie_BCProcess,        ///< process fp
    NULL,                   ///< process end fp
    BC_CalcBuffSize,        ///< get working buffer fp
    BC_CalcCacheBuffSize,   ///< get working cache buffer fp
    },
    NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif


#if MOVIE_TD_FUNC
void Movie_TDInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    TDInit(buf->Addr, cachebuf->Addr);
}

void Movie_TDProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    IMG_BUF          InputImg;
    UINT32           ImageRatioIdx, myDualCam;
    ISF_UNIT*        pUnit;
    static UINT32    TDUpdateCounter=0;
    if (gBCLock == TRUE)
        return;
    if (gBCUpdateCounter != TDUpdateCounter)
        TDUpdateCounter = gBCUpdateCounter;
    else
        return;

    #if (SENSOR_CAPS_COUNT == 1)
    if(System_GetEnableSensor() == (SENSOR_1) )
        myDualCam = DUALCAM_FRONT;
    else
        myDualCam = DUALCAM_BEHIND;
    #endif
    #if (SENSOR_CAPS_COUNT == 2)
    if(System_GetEnableSensor() == (SENSOR_1) || System_GetEnableSensor() == (SENSOR_2))
    {
        if(System_GetEnableSensor() == (SENSOR_1) )
            myDualCam = DUALCAM_FRONT;
        else
            myDualCam = DUALCAM_BEHIND;
    }
    else
    {
        myDualCam = UI_GetData(FL_DUAL_CAM);
    }
    #endif

    ImageRatioIdx = GetMovieSizeRatio(UI_GetData(FL_MOVIE_SIZE));
    if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH)
        pUnit = &ISF_ImagePipe1;
    else
        pUnit = &ISF_ImagePipe2;
    ImageUnit_SetParam(pUnit,IMAGEPIPE_PARAM_PULLBUF_OUT2, (UINT32)&InputImg);
    //GxImg_DumpBuf("A:\\test.raw",&InputImg,GX_IMAGE_DUMP_ALL);

    TDProcess(&InputImg);
    //#NT#2016/06/08#Lincy Lin -begin
    //#NT#Implement generic OSD and video drawing mechanism for ALG function
    Ux_PostEvent(NVTEVT_EXE_MOVIE_ALGEND,1, NVTEVT_EXE_MOVIE_ALGEND);
    //#NT#2016/06/08#Lincy Lin -end

	//#NT#2016/10/27#Charlie Chang -begin
    //#NT#notify to ipc if td status change

    BOOL td_status = FALSE;
	static BOOL old_status = FALSE;
    td_status = TDGetAlarm();
	if(td_status != old_status){
        if(td_status == TRUE){
            NvtUctrl_Notify(TAMPER1_TRIGGER);
		}
		else{
            NvtUctrl_Notify(TAMPER1_NOTRIGGER);
		}
		old_status = td_status;
	}

    //#NT#2016/10/27#Charlie Chang -end
}

//#NT#2016/08/06#XQ Liu -begin
//#NT#Support saving log for TD
void Movie_TDProcessEnd(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
#if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
#if (MOVIE_TD_SAVE_LOG == ENABLE)
    UINT32 logSize = 0;
    UINT32 logBuf = 0;
    logSize = TD_GetLog(&logBuf);
    if(logSize)
    {
        mp4log_AddLog(DbgLogTDID, (char*)logBuf, logSize);
    }
#endif
#endif
}
//#NT#2016/08/06#XQ Liu -end

PHOTO_FUNC_INFO PhotoFuncInfo_mvtd =
{
    {
    PHOTO_ID_2,             ///< function hook process Id.
    //#NT#2016/10/18#Jeah Yen -begin
    //#NT#support sensor map
    IPL_PATH_1,               ///< IPL Id.
    //#NT#2016/10/18#Jeah Yen -end
    PHOTO_ISR_IME_FRM_END,  ///< isr tigger event
    Movie_TDInit,           ///< init fp, only execute at first process
    Movie_TDProcess,        ///< process fp
    Movie_TDProcessEnd,                   ///< process end fp
    TDCalcBuffSize,        ///< get working buffer fp
    TDCalcCacheBuffSize,   ///< get working cache buffer fp
    },
    NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif
//#NT#2016/05/27#David Tsai -end

//#NT#2016/10/14#Yuzhe Bian -begin
//#NT# Support trip-wire detection function
#if MOVIE_TWD_FUNC
void Movie_TWDInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    nvt_twd_init(buf->Addr, cachebuf->Addr);
}
void Movie_TWDProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    IMG_BUF          InputImg;
    UINT32           ImageRatioIdx, myDualCam;
    ISF_UNIT*        pUnit;
    static UINT32    TWDUpdateCounter=0;
    if (gBCLock == TRUE)
        return;
    if (gBCUpdateCounter != TWDUpdateCounter)
        TWDUpdateCounter = gBCUpdateCounter;
    else
        return;
    #if (SENSOR_CAPS_COUNT == 1)
    if(System_GetEnableSensor() == (SENSOR_1) )
        myDualCam = DUALCAM_FRONT;
    else
        myDualCam = DUALCAM_BEHIND;
    #endif
    #if (SENSOR_CAPS_COUNT == 2)
    if(System_GetEnableSensor() == (SENSOR_1) || System_GetEnableSensor() == (SENSOR_2))
    {
        if(System_GetEnableSensor() == (SENSOR_1) )
            myDualCam = DUALCAM_FRONT;
        else
            myDualCam = DUALCAM_BEHIND;
    }
    else
    {
        myDualCam = UI_GetData(FL_DUAL_CAM);
    }
    #endif

    ImageRatioIdx = GetMovieSizeRatio(UI_GetData(FL_MOVIE_SIZE));
    if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH)
        pUnit = &ISF_ImagePipe1;
    else
        pUnit = &ISF_ImagePipe2;

    ImageUnit_SetParam(pUnit,IMAGEPIPE_PARAM_PULLBUF_OUT2, (UINT32)&InputImg);

    nvt_twd_process(&InputImg);

    Ux_PostEvent(NVTEVT_EXE_MOVIE_ALGEND,1, NVTEVT_EXE_MOVIE_ALGEND);

}
PHOTO_FUNC_INFO PhotoFuncInfo_mvtwd =
{
    {
    PHOTO_ID_2,             ///< function hook process Id.
    //#NT#2016/10/18#Jeah Yen -begin
    //#NT#support sensor map
    IPL_PATH_1,               ///< IPL Id.
    //#NT#2016/10/18#Jeah Yen -end
    PHOTO_ISR_IME_FRM_END,  ///< isr tigger event
    Movie_TWDInit,           ///< init fp, only execute at first process
    Movie_TWDProcess,        ///< process fp
    NULL,                   ///< process end fp
    nvt_twd_calcbuffsize,        ///< get working buffer fp
    nvt_twd_calccachebuffsize,   ///< get working cache buffer fp
    },
    NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif
//#NT#2016/10/14#Yuzhe Bian -end

//#NT#2016/10/14#Yuzhe Bian -begin
//#NT# Support trip-zone detection function
#if MOVIE_TZD_FUNC
void Movie_TZDInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    nvt_tzd_init(buf->Addr, cachebuf->Addr);
}
void Movie_TZDProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    IMG_BUF          InputImg;
    UINT32           ImageRatioIdx, myDualCam;
    ISF_UNIT*        pUnit;
    static UINT32    TZDUpdateCounter=0;
    if (gBCLock == TRUE)
        return;
    if (gBCUpdateCounter != TZDUpdateCounter)
        TZDUpdateCounter = gBCUpdateCounter;
    else
        return;
    #if (SENSOR_CAPS_COUNT == 1)
    if(System_GetEnableSensor() == (SENSOR_1) )
        myDualCam = DUALCAM_FRONT;
    else
        myDualCam = DUALCAM_BEHIND;
    #endif
    #if (SENSOR_CAPS_COUNT == 2)
    if(System_GetEnableSensor() == (SENSOR_1) || System_GetEnableSensor() == (SENSOR_2))
    {
        if(System_GetEnableSensor() == (SENSOR_1) )
            myDualCam = DUALCAM_FRONT;
        else
            myDualCam = DUALCAM_BEHIND;
    }
    else
    {
        myDualCam = UI_GetData(FL_DUAL_CAM);
    }
    #endif

    ImageRatioIdx = GetMovieSizeRatio(UI_GetData(FL_MOVIE_SIZE));
    if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH)
        pUnit = &ISF_ImagePipe1;
    else
        pUnit = &ISF_ImagePipe2;

    ImageUnit_SetParam(pUnit,IMAGEPIPE_PARAM_PULLBUF_OUT2, (UINT32)&InputImg);

    nvt_tzd_process(&InputImg);

    Ux_PostEvent(NVTEVT_EXE_MOVIE_ALGEND,1, NVTEVT_EXE_MOVIE_ALGEND);

}
PHOTO_FUNC_INFO PhotoFuncInfo_mvtzd =
{
    {
    PHOTO_ID_2,             ///< function hook process Id.
    //#NT#2016/10/18#Jeah Yen -begin
    //#NT#support sensor map
    IPL_PATH_1,               ///< IPL Id.
    //#NT#2016/10/18#Jeah Yen -end
    PHOTO_ISR_IME_FRM_END,  ///< isr tigger event
    Movie_TZDInit,           ///< init fp, only execute at first process
    Movie_TZDProcess,        ///< process fp
    NULL,                   ///< process end fp
    nvt_tzd_calcbuffsize,        ///< get working buffer fp
    nvt_tzd_calccachebuffsize,   ///< get working cache buffer fp
    },
    NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif
//#NT#2016/10/14#Yuzhe Bian -end

//#NT#2016/10/26#Omega Yu -begin
//#NT# Support scene change detection function
#if MOVIE_SCD_FUNC
void Movie_SCDInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    SCD_Init();
}

void Movie_SCDProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    HEADER_RAW_DATA rawHeaderInfo = {0};
    DISALG_VECTOR disOffset = {0};
    SCD_PROC_PRMS prms = {0};

    if (IPL_HeaderRawGetCurInfo_Dly1VD(IPL_PATH_1, &rawHeaderInfo) != E_OK)
    {
        DBG_ERR("DIS: Get header error!!!\r\n");
    }

    dis_getFrameCorrVecPxl(&disOffset, rawHeaderInfo.frame_cnt);
    //DBG_DUMP("dis: %d %d %d %d\n", disOffset.vector.iX, disOffset.vector.iY, disOffset.frame_cnt, disOffset.score);

    if (1 == disOffset.score)
    {
        prms.iX = disOffset.vector.iX;
        prms.iY = disOffset.vector.iY;
        SCD_Process(&prms);
    }
}

PHOTO_FUNC_INFO PhotoFuncInfo_mvscd =
{
    {
    PHOTO_ID_2,             ///< function hook process Id.
    IPL_PATH_1,             ///< IPL Id.
    PHOTO_ISR_IME_FRM_END,  ///< isr tigger event
    Movie_SCDInit,          ///< init fp, only execute at first process
    Movie_SCDProcess,       ///< process fp
    NULL,                   ///< process end fp
    NULL,                   ///< get working buffer fp
    NULL,                   ///< get working cache buffer fp
    },
    NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif // MOVIE_SCD_FUNC
//#NT#2016/10/26#Omega Yu -end

//#NT#2016/10/17#Bin Xiao -begin
//#NT# Support Face Tracking Grading(FTG)
#if MOVIE_FTG_FUNC
void Movie_FTGLock(BOOL bLock)
{
    gFTGLock = bLock;
	ftg_lock(bLock);
}

BOOL Movie_FTGGetLockSts(void)
{
    return gFTGLock;
}


void Movie_FTGInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	//URECT roi_rect = {300,300,400,300};
	//ftg_setfdroi(TRUE, &roi_rect);

    ftg_init(buf, cachebuf, 5, 15, 3);
}

void Movie_FTGProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    IMG_BUF          InputImg;
    UINT32           ImageRatioIdx, myDualCam;
    ISF_UNIT*        pUnit;
	ISF_PORT*        pSrc;


    if (gFTGLock == TRUE)
        return;

    #if (SENSOR_CAPS_COUNT == 1)
    if(System_GetEnableSensor() == (SENSOR_1) )
        myDualCam = DUALCAM_FRONT;
    else
        myDualCam = DUALCAM_BEHIND;
    #endif
    #if (SENSOR_CAPS_COUNT == 2)
    if(System_GetEnableSensor() == (SENSOR_1) || System_GetEnableSensor() == (SENSOR_2))
    {
        if(System_GetEnableSensor() == (SENSOR_1) )
            myDualCam = DUALCAM_FRONT;
        else
            myDualCam = DUALCAM_BEHIND;
    }
    else
    {
        myDualCam = UI_GetData(FL_DUAL_CAM);
    }
    #endif

    ImageRatioIdx = GetMovieSizeRatio(UI_GetData(FL_MOVIE_SIZE));
    if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH)
        pUnit = &ISF_ImagePipe1;
    else
        pUnit = &ISF_ImagePipe2;

	pSrc = ImageUnit_Out(pUnit, ISF_OUT1);
    if(pSrc && !(pSrc->Flag & ISF_PORT_FLAG_PAUSE))
		ImageUnit_SetParam(pUnit,IMAGEPIPE_PARAM_PULLBUF_OUT1, (UINT32)&InputImg);
	else{

        ImageUnit_SetParam(pUnit,IMAGEPIPE_PARAM_PULLBUF_OUT2, (UINT32)&InputImg);
	}

    #if 0
	static UINT32 ftg_num2 = 0;
    static UINT32 ftg_time2 = 0;
	static UINT32 ftg_max2 = 0;
	UINT32 t1 = Perf_GetCurrent();
    ftg_process(&InputImg, Get_FDImageRatioValue(ImageRatioIdx));
	UINT32 t2 = Perf_GetCurrent();
	if(ftg_num2 < 1500 && ftg_num2 >= 1000) {
		if(ftg_max2 < t2-t1) {
			ftg_max2 = t2-t1;
		}
		ftg_time2 += t2-t1;
		ftg_num2++;
	} else if(ftg_num2 >=1000) {
	    DBG_ERR("500 FTG uses %d us...max %d us\n\r", ftg_time2, ftg_max2);
	} else {
	    ftg_num2++;
	}
	#else
	ftg_process(&InputImg, Get_FDImageRatioValue(ImageRatioIdx));
	#endif

    Ux_PostEvent(NVTEVT_EXE_MOVIE_ALGEND,1, NVTEVT_EXE_MOVIE_ALGEND);
}

PHOTO_FUNC_INFO PhotoFuncInfo_mvftg =
{
    {
    PHOTO_ID_2,              ///< function hook process Id.
    //#NT#2016/10/18#Jeah Yen -begin
    //#NT#support sensor map
    IPL_PATH_1,               ///< IPL Id.
    //#NT#2016/10/18#Jeah Yen -end
    PHOTO_ISR_IME_FRM_END,   ///< isr tigger event
    Movie_FTGInit,           ///< init fp, only execute at first process
    Movie_FTGProcess,        ///< process fp
    NULL,                    ///< process end fp
    ftg_calcbuffsize,        ///< get working buffer fp
    ftg_calccachebuffsize,   ///< get working cache buffer fp
    },
    NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif
//#NT#2016/10/17#Bin Xiao -end
//#NT# Support Face Tracking Grading(FTG)

//#NT#2016/10/17#Bin Xiao -begin
//#NT# Support FD used by Face Tracking Grading(FTG)
#if MOVIE_FTG_FUNC
void Movie_FTGFDInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	ftg_config(ftg_fd_cfg_scale_eng, ftg_fd_scale_eng2);
	ftg_config(ftg_fd_cfg_sensitivity, 1);

    // Init FD buffer, set max face number to 10, and process rate as 3
    ftg_fd_init(buf, cachebuf, MAX_FDNUM, 15, 3);
}

void Movie_FTGFDProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
    IMG_BUF          InputImg;
    UINT32           ImageRatioIdx, myDualCam;
    ISF_UNIT*        pUnit;
	ISF_PORT*        pSrc;

    #if (SENSOR_CAPS_COUNT == 1)
    if(System_GetEnableSensor() == (SENSOR_1) )
        myDualCam = DUALCAM_FRONT;
    else
        myDualCam = DUALCAM_BEHIND;
    #endif
    #if (SENSOR_CAPS_COUNT == 2)
    if(System_GetEnableSensor() == (SENSOR_1) || System_GetEnableSensor() == (SENSOR_2))
    {
        if(System_GetEnableSensor() == (SENSOR_1) )
            myDualCam = DUALCAM_FRONT;
        else
            myDualCam = DUALCAM_BEHIND;
    }
    else
    {
        myDualCam = UI_GetData(FL_DUAL_CAM);
    }
    #endif

    ImageRatioIdx = GetMovieSizeRatio(UI_GetData(FL_MOVIE_SIZE));

    if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH)
        pUnit = &ISF_ImagePipe1;
    else
        pUnit = &ISF_ImagePipe2;

	pSrc = ImageUnit_Out(pUnit, ISF_OUT1);
    if(pSrc && !(pSrc->Flag & ISF_PORT_FLAG_PAUSE))
		ImageUnit_SetParam(pUnit,IMAGEPIPE_PARAM_PULLBUF_OUT1, (UINT32)&InputImg);
	else{

        ImageUnit_SetParam(pUnit,IMAGEPIPE_PARAM_PULLBUF_OUT2, (UINT32)&InputImg);
	}

    #if 0
    static UINT32 ftg_num3 = 0;
    static UINT32 ftg_time3 = 0;
	static UINT32 ftg_max3 = 0;
	UINT32 t1 = Perf_GetCurrent();
    ftg_fd_process(&InputImg, Get_FDImageRatioValue(ImageRatioIdx));
	UINT32 t2 = Perf_GetCurrent();
	if(ftg_num3 < 1500 && ftg_num3 >=1000) {
		if(ftg_max3 < t2-t1) {
			ftg_max3 = t2-t1;
		}
		ftg_time3 += t2-t1;
		ftg_num3++;
	} else if(ftg_num3 >= 1000) {
	    DBG_ERR("500 FTG_FD uses %d us...max %d us\n\r", ftg_time3, ftg_max3);
	} else {
	    ftg_num3++;
	}
	#else
	ftg_fd_process(&InputImg, Get_FDImageRatioValue(ImageRatioIdx));
    #endif

    //Ux_PostEvent(NVTEVT_EXE_MOVIE_ALGEND,1, NVTEVT_EXE_MOVIE_ALGEND);
}

PHOTO_FUNC_INFO PhotoFuncInfo_mvftgfd =
{
    {
    PHOTO_ID_2,              ///< function hook process Id.
    //#NT#2016/10/18#Jeah Yen -begin
    //#NT#support sensor map
    IPL_PATH_1,               ///< IPL Id.
    //#NT#2016/10/18#Jeah Yen -end
    PHOTO_ISR_IME_FRM_END,   ///< isr tigger event
    Movie_FTGFDInit,           ///< init fp, only execute at first process
    Movie_FTGFDProcess,        ///< process fp
    NULL,                    ///< process end fp
    ftg_fd_calcbuffsize,        ///< get working buffer fp
    ftg_fd_calccachebuffsize,   ///< get working cache buffer fp
    },
    NULL  ///< point next Function Obj, last Function must be set NULL
};
#endif
//#NT#2016/10/17#Bin Xiao -end

