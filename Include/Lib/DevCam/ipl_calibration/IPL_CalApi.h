#ifndef _IPL_CALAPI_H_
#define _IPL_CALAPI_H_
#include "Type.h"
#include "ErrorNo.h"
#include "IPL_Ctrl_Par.h"
#include "IPL_CalCtrl.h"
#include "IPL_Calibration_3A.h"
#include "IPL_Calibration_ALG.h"
#include "IPL_Calibration_BUF.h"
//#NT#2016/07/19#Silvia Wu -begin
//#NT# add calibration UI/TYPE commom API
#include "IPL_Calibration_UI.h"
#include "IPL_Calibration_TYPE.h"
//#NT#2016/07/19#Silvia Wu -end

typedef struct
{
    void*   pDrv_Sensor;
    void*   pCal_Sensor;
    fp_ImgRdy   fpRdyImg;//CB at ime FMD
}CTRL_PTR,*pCTRL_PTR;

//define for SetMode callback
typedef void (*fptr)(IPL_PROC_ID Id, pIPLC_INFO pOutInfo,pCTRL_PTR pInfo);

/**
    calibration pipeline mode
*/
typedef enum
{
    CAL_IPL_NONE        = 0,
    CAL_IPL_ALLSTOP     = 1,    ///< pipeline YCC part stop and close
    CAL_IPL_ALLRUN      = 2,    ///< start run but with sie_only
    CAL_IPL_SIESTOP     = 3,    ///< SIE stop
    CAL_IPL_CAP         = 4,    ///< single image
    CAL_IPL_SIERESUME   = 5,    ///< SIE resume
    CAL_IPL_PRV         = 6,    ///< streaming
    ENUM_DUMMY4WORD(CAL_IPL_MODE)
}CAL_IPL_MODE;

/**
    calibration bayer info
*/
typedef struct
{
    IPL_RAW_IMG_INFO BaseInfo;
    Coordinate       ActStart;
    Coordinate       CropStart;
}IPL_RAW_IMG;

//#NT#2016/07/19#Silvia Wu -begin
//#NT# add calibration save any data common API
/**
    calibration save data info
*/
typedef struct
{
    CHAR *FileName;
    UINT32 DataAddr;
    UINT32 DataSize;
    BOOL bDeletFile;
}CAL_SAVEDATA;
//#NT#2016/07/19#Silvia Wu -end

/**
    Calibration object
*/
typedef struct
{
    ER                  (*fpSetMode)(IPL_PROC_ID,CAL_IPL_MODE,pCTRL_PTR,fptr);
    IPL_RAW_IMG         (*fpGetRawInfo)(IPL_PROC_ID);
    ER                  (*fpSaveRAW)(UINT32, UINT32, UINT32);
    pCAL_3A_OBJ         (*fp3AEvent)(void);
    pCAL_BUF_OBJ        (*fpBufEvent)(void);
    pCAL_ALG_OBJ        (*fpAlgEvent)(void);
//#NT#2016/07/19#Silvia Wu -begin
//#NT# add calibration save any data/UI/type common API
    pCAL_UI_OBJ         (*fpUIEvent)(void);
    pCAL_TYPE_OBJ       (*fpTypeEvent)(void);
    ER                  (*fpSaveData)(CAL_SAVEDATA* SaveDataInfo);
//#NT#2016/07/19#Silvia Wu -end
} CalApi_Obj, *pCalApi_Obj;

/**
    calibration application
*/
extern pCalApi_Obj CalApi(void);

#endif
