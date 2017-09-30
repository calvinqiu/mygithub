#ifndef _IPL_CALIBRATION_UI_H_
#define _IPL_CALIBRATION_UI_H_
/**
    IPL Calibration UI setting

    UI setting only for calibration.

    @file       IPL_Calibration_UI.h
    @ingroup    mILibCal

    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.
*/

/**
    calibration String Color
*/
typedef enum _CalSTRCOLOR
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
}CalSTRCOLOR;

/**
    calibration UI RGB define
*/
typedef struct
{
    UINT32 R;   ///< R (range:0~255)
    UINT32 G;   ///< G (range:0~255)
    UINT32 B;   ///< B (range:0~255)
}CAL_UI_RGB;

/**
    calibration UI color define
*/
typedef struct
{
    UINT32 idx;  ///< Project color idx define (CLRID_IDX_XX)
    CAL_UI_RGB RGBColor;   ///< RGB color
}CAL_UI_COLOR;


/**
    calibration UI color mapping
*/
typedef struct
{
    CalSTRCOLOR CalColor;   ///< calibration color define
    CAL_UI_RGB RGBColor;   ///< RGB color
}CalColorMapRGB;

/**
    calibration UI initial setting object
*/
typedef void (*CAL_UI_SHOW_FP)(CHAR *pStr, PURECT pRect, UINT32 StrColor, UINT32 BgColor);
typedef void (*CAL_UI_WAITKEY_FP)(UINT32 KeyMask);
typedef struct
{
    USIZE  ScreenSize;     ///< Screen Size
    USIZE  StringSize;     ///< String Size
    UINT32 MaxShowLine;        ///< MaxShowLine
    //ColorMap* ColorMapTbl; ///< Color mapping table
    CAL_UI_SHOW_FP SHOW_CB_FP;       ///< calibration UI show line callback function
    CAL_UI_WAITKEY_FP WAITKEY_CB_FP;       ///< calibration UI waitkey callback function
}CAL_UI_INIT_OBJ;

/**
    calibration UI operation structure
    @note plz refer to Cal_UIEvent()
*/
typedef struct _CAL_UI_OBJ
{
    BOOL (*fpInit)(CAL_UI_INIT_OBJ InitObj); ///< Init calibration UI object
    ER   (*fpShowLine)(CHAR *pstr, IPOINT StringPos, UINT32 ColorInfo); ///< showline for calibration UI
    ER   (*fpDeleteLine) (IPOINT pStringPos, UINT32 ColorInfo); ///< delete line for calibration UI
    ER   (*fpCleanScreen)(UINT32 ColorInfo); ///< clean screen for calibration UI
    ER   (*fpWaitkey) (CHAR *pstr, IPOINT pStringPos, UINT32 ColorInfo, UINT32 keyMask); ///< waitkey for calibration UI
}CAL_UI_OBJ,*pCAL_UI_OBJ;

/**
     calibration UI operation

     @return Description of data returned.
         - @b fpInit: Init calibration UI object
         - @b fpShowLine: showline for calibration UI
         - @b fpDeleteLine: delete line for calibration UI
         - @b fpCleanScreen: clean screen for calibration UI
         - @b fpWaitkey: waitkey for calibration UI
*/
extern pCAL_UI_OBJ Cal_UIEvent(void);
//@}
#endif
