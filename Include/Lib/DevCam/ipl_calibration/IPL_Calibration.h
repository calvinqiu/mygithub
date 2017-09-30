#ifndef _IPL_CALIBRATION_H_
#define _IPL_CALIBRATION_H_
/**
    IPL calibration data structure

    Describe calibration items which are supported.
    including working buffer control mech., calibration environment, calibration algorithm, calibration UI control.

    @file       IPL_Calibration.h
    @ingroup    mILibCal

    Copyright   Novatek Microelectronics Corp. 2013.  All rights reserved.
*/

#include "IPL_Calibration_3A.h"
#include "IPL_Calibration_ALG.h"
#include "IPL_Calibration_BUF.h"
//#NT#2016/07/19#Silvia Wu -begin
//#NT# add calibration UI/TYPE commom API
#include "IPL_Calibration_TYPE.h"
#include "IPL_Calibration_UI.h"
//#NT#2016/07/19#Silvia Wu -end
#include "IPL_Ctrl.h"
/** \addtogroup mILibCal */
//@{

#if 0
    - UI
#endif

/**
    calibration UI event
*/
typedef enum _CalUICB
{
    UICB_NONE       = 0,    ///< -
    UICB_START      = 1,    ///< prepare to initiate calibration item.
    UICB_WAITKEY    = 2,    ///
    UICB_PROCESSING = 3,    ///< -
    UICB_CLEAN      = 4,    ///< -
    UICB_END        = 5,    ///< -
    UICB_DELETE     = 6,    ///< -
    ENUM_DUMMY4WORD(CalUICB)
}CalUICB;

//#NT#2016/07/19#Silvia Wu -begin
//#NT# move calibration UI color information to IPL_Calibration_UI.h
/**
    calibration String Color
*/
/*typedef enum _CalSTRCOLOR
{
    UICOL_NONE      = 0,    ///< -
    UICOL_RED       = 1,    ///< -
    UICOL_GREEN     = 2,    ///< -
    UICOL_BLUE      = 3,    ///< -
    UICOL_WHITE     = 4,    ///< -
    UICOL_BLACK     = 5,    ///< -
    UICOL_GRAY      = 6,    ///< -
    UICOL_YELLOW    = 7,    ///< -
    UICOL_MAX       = 8,    ///< -
    ENUM_DUMMY4WORD(CalSTRCOLOR)
}CalSTRCOLOR;*/
//#NT#2016/07/19#Silvia Wu -end

/**
    function prototype of calibration UI control
    CalUICB: event
    CHAR*:   string
*/
typedef void (*fpUI_cb)(CalUICB,CHAR*,UINT32);

/**
    calibration item. UI structure
    @note plz refer to CAL_UICB()
*/
typedef struct _CAL_UICB_OBJ
{
    fpUI_cb     AWB_UI;         ///< plz refer to CAL_UICB()
    fpUI_cb     ISO_UI;         ///< plz refer to CAL_UICB()
    fpUI_cb     ECS_UI;         ///< plz refer to CAL_UICB()
    fpUI_cb     MSHUT_UI;       ///< plz refer to CAL_UICB()
    fpUI_cb     DP_W_UI;        ///< plz refer to CAL_UICB()
    fpUI_cb     DP_D_UI;        ///< plz refer to CAL_UICB()
    fpUI_cb     LENSCEN_UI;     ///< plz refer to CAL_UICB()
    //fpUI_cb     CAL_STATUS_UI;
}CAL_UICB_OBJ,*pCAL_UICB_OBJ;

/**
     Calibration item UI control

     control calibration item UI, (like wait key, show string ... etc.)

     @return Description of data returned.
         - @b AWB_UI:   Item: AWB UI control
         - @b ISO_UI:   Item: ISO UI control
         - @b ECS_UI:   Item: ECS UI control
         - @b MSHUT_UI: Item: MSHUT UI control
         - @b DP_W_UI:  Item: DP(bight pixel) UI control
         - @b DP_D_UI:  Item: DP(dark pixel) UI control
         - @b LensCenter_UI:  Item: Lens Center calibration UI control
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern pCAL_UICB_OBJ CAL_UICB(void);

#if 0
    - ITEM
#endif


typedef struct _CAL_STATUS_CHK
{
    UINT32 ISO_STATUS;
    UINT32 AWB_STATUS;
    UINT32 DP_STATUS;
    UINT32 ECS_STATUS;
    UINT32 AECHK_STATUS;
}CAL_STATUS_CHK;

typedef struct _CalIQInfo
{
    CAL_STATUS_CHK Cal_Status;
    UINT32 Cal_AWB_Rg;
    UINT32 Cal_AWB_Bg;
    UINT32 Cal_ISO_Base;
    UINT32 Cal_AE_ChkRst;    //Check Capture Luminance at LV 15
}CalIQInfo;

/**
    calibration Debug
*/
// calibration debug index
typedef enum _CalDebugIdx
{
    //#NT#2016/03/24#Silvia Wu -begin
    //#NT# add debug message
    CALDEBUG_NONE              = 0,    ///< - no debug info
    CALDEBUG_INFO              = 1,    ///< - save debug information in SD card
    CALDEBUG_INFO_ECS          = 2,    ///< - save ECS debug information in SD card
    CALDEBUG_INFO_PARAM        = 3,    ///< - save parameter in SD card
    CALDEBUG_INFO_USER_A       = 4,    ///< - save debug information in SD card which define by user
    CALDEBUG_INFO_USER_B       = 5,    ///< - save debug information in SD card which define by user
    CALDEBUG_INFO_USER_C       = 6,    ///< - save debug information in SD card which define by user
    CALDEBUG_RAW               = 7,    ///< - save raw in SD card
    CALDEBUG_RESULT_DATA       = 8,    ///< - save calibration result data in SD card
    CALDEBUG_OSD_WAITKEY       = 9,    ///< - wait anykey and show OSD information to debug
    CALDEBUG_PRV_START         = 10,   ///< - show preview at the begging of the calibration station in every station
    CALDEBUG_PRV_END           = 11,   ///< - show preview at the end of the calibration station in every station
    CALDEBUG_USER_A            = 12,   ///< - define by user
    CALDEBUG_USER_B            = 13,   ///< - define by user
    CALDEBUG_USER_C            = 14,   ///< - define by user
    CALDEBUG_MAX_NUM,
    ENUM_DUMMY4WORD(CalDebugIdx)
    //#NT#2016/03/24#Silvia Wu -end
}CalDebugIdx;

//#NT#2016/07/19#Silvia Wu -begin
//#NT# add calibration debug init state & debug cfg item
// calibration debug item initial state
typedef enum _CALINITSTATE
{
    INITSTATE_None        = 0,
    INITSTATE_Normal      = 1,    ///< CAL_MODE_Normal
    INITSTATE_Web_Option  = 2,    ///< web user option
    INITSTATE_Web_Fixed   = 3,    ///< web fixed initial value
    ENUM_DUMMY4WORD(CALINITSTATE)
}CALINITSTATE;

// calibration debug item
typedef struct
{
    CalDebugIdx Index;
    CHAR    *pItemName;
    INT32   ItemValue;
    CALINITSTATE InitState;
}CalDbgCfgItem;
//#NT#2016/07/19#Silvia Wu -end

typedef enum _CAL_DEBUG_STATUS
{
    _CAL_DEBUG_NONE       = 0x0,          ///< no result
    _CAL_DEBUG_OK         = 0xfedba950,   ///< result is pass
    _CAL_DEBUG_OverFlow   = 0x51,         ///< result is over spec
    _CAL_DEBUG_UnderFlow  = 0x52,         ///< result is under spec
    _CAL_DEBUG_DP_D_OK    = 0x61,         ///< DP Dark is finish and PASS
    _CAL_DEBUG_DP_B_OK    = 0x62,         ///< DP Bright is finish and PASS
    _CAL_DEBUG_AE_TIMEOUT = 0x70,         ///< AE timeout
    ENUM_DUMMY4WORD(CAL_DEBUG_STATUS)
}CAL_DEBUG_STATUS;

typedef struct
{
    BOOL    AeTimeout;   //calibration Ae status
    UINT32  Lum;
    UINT32  ISO;
    UINT32  ISOBase;
    UINT32  ExpT;
    UINT32  Ratio;
}CAL_DEBUG_AE;

#define Max_SenMode_Condition 20

typedef struct
{
    CAL_DEBUG_STATUS      CalStatusRst[Max_SenMode_Condition];   //calibration status results
    CAL_DEBUG_AE    CalAeRstDark[Max_SenMode_Condition];   //calibration ae converge results (DPDark)
    CAL_DEBUG_AE    CalAeRstBright[Max_SenMode_Condition]; //calibration ae converge results (DPBright)
    UINT32          DCnt[Max_SenMode_Condition];           //DP(Dark)
    UINT32          BCnt[Max_SenMode_Condition];           //DP(Bright)
    UINT32          CalDebugInfor[Max_SenMode_Condition];
}CAL_DEBUG_INFO_DP;


typedef struct
{
    CAL_DEBUG_STATUS      CalStatusRst[Max_SenMode_Condition];   //calibration status results
    CAL_DEBUG_AE    CalAeRst[Max_SenMode_Condition];             //calibration ae converge results
    UINT32          CalISOBase[Max_SenMode_Condition];           //calibration isobase results
    UINT32          CalDebugInfor[Max_SenMode_Condition];
}CAL_DEBUG_INFO_ISO;


typedef struct
{
    CAL_DEBUG_STATUS      CalStatusRst[Max_SenMode_Condition];   //calibration status results
    CAL_DEBUG_AE    CalAeRst[Max_SenMode_Condition];             //calibration ae converge results
    UINT32          CalDebugInfor[Max_SenMode_Condition];
}CAL_DEBUG_INFO_ECS;


typedef struct
{
    CAL_DEBUG_STATUS      CalStatusRst[Max_SenMode_Condition];   //calibration status results
    CAL_DEBUG_AE    CalAeRst[Max_SenMode_Condition];             //calibration ae converge results
    UINT32          CalRgain[Max_SenMode_Condition];             //calibration Rgain results
    UINT32          CalBgain[Max_SenMode_Condition];             //calibration Bgain results
    UINT32          CalDebugInfor[Max_SenMode_Condition];
}CAL_DEBUG_INFO_AWB;

//#NT#2016/09/13#Silvia Wu -begin
//#NT# add support lens center calibration
typedef struct
{
    CAL_DEBUG_STATUS CalStatusRst[Max_SenMode_Condition];   //calibration status results
    CAL_DEBUG_AE     CalAeRst[Max_SenMode_Condition];       //calibration ae converge results
    UINT32           CalActStrX[Max_SenMode_Condition];      //calibration active start x
    UINT32           CalActStrY[Max_SenMode_Condition];      //calibration active start y
    INT32            CalCenSftX[Max_SenMode_Condition];      //calibration lens center shift x results
    INT32            CalCenSftY[Max_SenMode_Condition];      //calibration lens center shift y results
    UINT32           CalReliability[Max_SenMode_Condition]; //calibration lens center results reliability
    UINT32           CalDebugInfor[Max_SenMode_Condition];
}CAL_DEBUG_INFO_LENSCEN;

typedef struct
{
    CAL_DEBUG_INFO_DP*      CalDebugDP;
    CAL_DEBUG_INFO_ISO*     CalDebugISO;
    CAL_DEBUG_INFO_ECS*     CalDebugECS;
    CAL_DEBUG_INFO_AWB*     CalDebugAWB;
    CAL_DEBUG_INFO_LENSCEN* CalDebugLENSCEN;
}CAL_DEBUG_INFO;
//#NT#2016/09/13#Silvia Wu -end

/**
    function prototype of Calibratoin Item
*/
typedef ER (*fpItem)(IPL_PROC_ID Id, fpUI_cb,UINT32 DmaAddr,UINT32 DmaSize);

/**
    calibration Item structure
*/
typedef struct _CAL_ITEM_OBJ
{
    fpItem  fpISOAdj;           ///< -
    fpItem  fpAWBAdj;           ///< -
    fpItem  fpAWBGSAdj;         ///< -
    fpItem  fpMSHUTAdj;         ///< -
    fpItem  fpECSAdj;           ///< -
    fpItem  fpDP_Adj;           ///< -
    fpItem  fpDP_Adj_Bright;    ///< -
    fpItem  fpDP_Adj_Dark;      ///< -
    fpItem  fpAE_Check;         ///< -
    BOOL    (*fpGetAdjustRst)(IPL_PROC_ID Id, fpUI_cb fpUIcb, CAL_DEBUG_INFO *CalDebugInfor);
    UINT32  (*fpGetDebugLevel) (CalDebugIdx idx);
    //#NT#2016/03/24#Silvia Wu -begin
    //#NT# add debug message
    ER (*fpCAL_SaveMsg)(char* CalSaveMsgName,char* CalSaveMsg, BOOL bDeletFile);
    ER (*fpCAL_SaveData)(char* CalSaveMsgName,UINT32 DataAddr, UINT32 DataSize, BOOL bDeletFile);
    //#NT#2016/03/24#Silvia Wu -end
    fpItem  fpLENSCENAdj;         ///< -
}CAL_ITEM_OBJ,*pCAL_ITEM_OBJ;

extern CalDbgCfgItem* CAL_GetDbgCfgTbl (void);

//@}

#endif
