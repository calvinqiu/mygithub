/**
    ADAS detection library.

    @file       adas_lib.h
    @ingroup    mILibADAS

    Copyright   Novatek Microelectronics Corp. 2015.  All rights reserved.
*/
#ifndef _ADAS_LIB_H
#define _ADAS_LIB_H

/**
    @addtogroup mILibADAS
*/

#include "Type.h"
#include "ldws_lib.h"
#include "fcws_lib.h"
#include "ime_lib.h"
#include "IPL_Cmd.h"
#include "SnG_lib.h"
#include "mIType.h"
/**
     @name ADASreturn status
*/
//@{
#define ADAS_STA_OK                  (0)                   ///<  the status is ok, for normal case
#define ADAS_STA_ERROR               (-1)                  ///<  some error occurred
//@}

#define _AUTOVP_ENABLE_              ENABLE           ///< Auto-VP is enable


/**
     LDWS detection callback message.

*/
//@{
typedef enum _ADAS_CBMSG
{
    ADAS_CBMSG_WARNNING = 0,       ///< user will receive this notification when the lane departure warning occur
    ADAS_CBMSG_SNG = 1,
    ADAS_CBMSG_MAX_CNT,            ///< total callback message numbers
    ENUM_DUMMY4WORD(ADAS_CBMSG)
} ADAS_CBMSG;
//@}




/**
   SrcImgInfo & LdwsRsltInfo structure:
                 uiLdSrcWidth
 -------------------------------------------------
 - - (uiLdRoiSx, uiLdRoiSy)                      -
 -   -                                           -
 -     ------------uiLdRoiWidth-----------       -
 -     -                                 -       -
 -     -   (iLx1,iLy1)*    * (iRx1,iRy1) -       -
 -     -                          uiLdRoiHeight  - uiLdSrcHeight
 -     -                                 -       -
 -     -(iLx2,iLy2)*           * (iRx2,iRy2)-    -
 -     ----------------------------------        -
 -                                               -
 -                                               -
 -------------------------------------------------

   SrcImgInfo & FcwsRsltInfo structure:

                 uiLdSrcWidth
 -------------------------------------------------
 - - (uiLdRoiSx, uiLdRoiSy)                      -
 -   -                                           -
 -     ------------uiLdRoiWidth-----------       -
 -     -                                 -       -
 -     -   (iLx1,iLy1)*    * (iRx1,iRy1) -       -
 -     -                          uiLdRoiHeight  - uiLdSrcHeight
 -     -                                 -       -
 -     -(iLx2,iLy2)*           * (iRx2,iRy2)-    -
 -     ----------------------------------        -
 -                                               -
 -                                               -
 -------------------------------------------------


*/
//@{
typedef struct _ADAS_RESULT_INFO
{
    LDWS_SRCIMG_INFO    SrcImgInfo;  ///<  source image information
    LDWS_RESULT_INFO    LdwsRsltInfo;  ///<  LDWS result
    FCWS_RESULT_INFO    FcwsRsltInfo;  ///<  FCWS result
    FCWS_DEBUG_INFO     FcwsDebugInfo; ///<  FCWS debug information
    STOPNGO_RSLTINFO    SnGInfo;
} ADAS_RESULT_INFO;
//@}


/**
    LDWS callback funcion proto type
*/
typedef void (*ADAS_CBMSG_FP)(ADAS_CBMSG uiMsgID, void *Data);


/**
     Calculate LDWS detection required buffer size.

     @return the required buffer size.
*/
extern UINT32 ADAS_CalcBuffSize(void);
/**
     Init ADAS Detect engine.
*/
extern INT32 ADAS_Init(MEM_RANGE *buf, MEM_RANGE *cachebuf, UINT32 ProcessRate);

/**
     Uninit ADAS engine.
*/
extern INT32 ADAS_UnInit(void);

/**
    Set LDWS initial parmeters
*/
INT32 ADAS_setLDWSInit(void);

/**
    Set FCWS initial parmeters
*/
INT32 ADAS_setFCWSInit(void);

/**
     Process the LDWS detection.
*/
extern INT32 ADAS_Process(void);

/**
    ADAS callback
*/
extern void   ADAS_RegisterCB(ADAS_CBMSG_FP CB);

/**
    Get FCWS histogram size
*/
extern UINT32 ADAS_getHistSz(void);

/**
    Get Y size
*/
extern USIZE ADAS_getYSz(void);

/**
    Get FCWS edge map size
*/
extern UINT32 ADAS_getFcwsEdgeMapSz(void);


/**
    LDWS enable
*/
extern void ADAS_setLdwsEanble(BOOL key);

/**
    FCWS enable
*/
extern void ADAS_setFcwsEanble(BOOL key);

/**
    ADAS Disable
*/
extern void ADAS_Stop(void);
/**
    ADAS Enable
*/
extern void ADAS_Start(void);
/*
    Get ADAS Status
*/
extern BOOL ADAS_GetStatus(void);

/**
    Set Crop information to IME
*/
extern void ADAS_setCropInfo2Ime(UINT32 CropSx, UINT32 CropSy);
/**
    Get histogram information from IME
*/
extern void ADAS_getHistInfo(IME_GET_STL_INFO *AdasInfo);
/**
    Set FCWS parameters to IME
*/
extern void ADAS_setImeParms(void);
/**
    Wait until accessing IPP finish
*/
extern void ADAS_waitIPPAccessEnd(void);
/**
    Close IME path 4
*/
extern void ADAS_Path4Off(void);
/**
    Get IME path 4 information
*/
extern IPL_IME_PATH_INFO ADAS_getPath4Info(void);
/**
    Update new VP for LDWS detection
*/
extern void ADAS_updateVP(void);
/**
    Set Fsize Value
*/
extern void ADAS_SetFsizeValue(UINT32 Val);
/**
    Get Fsize Value
*/
extern UINT32 ADAS_GetFsizeValue(void);
/**
    Set Left Sensitivity
*/

extern void ADAS_SetLeftSensitivity(UINT32 Val);
/**
    Get Left Sensitivity
*/
extern UINT32 ADAS_GetLeftSensitivity(void);
/**
    Set Right Sensitivity
*/
extern void ADAS_SetRightSensitivity(UINT32 Val);
/**
    Get Right Sensitivity
*/
extern UINT32 ADAS_GetRightSensitivity(void);
/**
    Set FCWS warning dist
*/
extern void ADAS_SetFcwsWarningDist(UINT32 Val);
/**
    Get FCWS warning dist
*/
extern UINT32 ADAS_GetFcwsWarningDist(void);
/**
    Set Current Speed
*/
extern void ADAS_SetCurSpeed(FLOAT Val);

/**
    SnG detection function
*/
extern void SnGParms_Detection(void);

/**
    Set save flag
*/
extern void SetSaveDisMvFlg(UINT32 flag);

extern ER ADAS_CopyIMEImg(UINT32 oriImgAddr, UINT32 outImgAddr, UINT32 oriWidth, UINT32 oriHeight, UINT32 oriLineoffset, UINT32 outWidth, UINT32 outHeight, UINT32 outLineoffset);

typedef void (*ADAS_MOVIELOG_FP)(UINT32 Id, char* Buf, UINT32 BufLen);
void ADAS_SetMovieLogFP(ADAS_MOVIELOG_FP fp);
void ADAS_GetLogString(UINT32 Id, char* Buf, UINT32 BufLen);

//#NT#2016/02/19#KCHong#[0094606] -begin
#define FLGADAS_IPPDONE         FLGPTN_BIT(0)       //0x00000001
extern UINT32 _SECTION(".kercfg_data") FLG_ID_ADAS;
//#NT#2016/02/19#KCHong#[0094606] -end

#endif

