/*
    Public header file for RDE module.

    This file is the header file that define the API and data type for TGE
    module.

    @file       tge_lib.h
    @ingroup    mIDrvIPPTGE
    @note

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _TGE_LIB_H
#define _TGE_LIB_H

#include "Type.h"

/**
    @addtogroup mIIPPTGE
*/
//@{

/**
    @name Define TGE function.
*/
//@{
//@}



/**
    @name Define TGE interrput.
*/
//@{
#define TGE_INT_VD                  0x00000001 ///< Enable interrupt
#define TGE_INT_BP1                 0x00000002 ///< Enable interrupt
#define TGE_INT_BP2                 0x00000004 ///< Enable interrupt
#define TGE_INT_FLSH_TRG            0x00000008 ///< Enable interrupt
#define TGE_INT_MSHA_CLOSE_TRG      0x00000010 ///< Enable interrupt
#define TGE_INT_MSHA_OPEN_TRG       0x00000020 ///< Enable interrupt
#define TGE_INT_MSHB_CLOSE_TRG      0x00000040 ///< Enable interrupt
#define TGE_INT_MSHB_OPEN_TRG       0x00000080 ///< Enable interrupt
#define TGE_INT_FLSH_END            0x00000100 ///< Enable interrupt
#define TGE_INT_MSHA_CLOSE_END      0x00000200 ///< Enable interrupt
#define TGE_INT_MSHA_OPEN_END       0x00000400 ///< Enable interrupt
#define TGE_INT_MSHB_CLOSE_END      0x00000800 ///< Enable interrupt
#define TGE_INT_MSHB_OPEN_END       0x00001000 ///< Enable interrupt
#define TGE_INT_ALL                 0xffffffff ///< Enable interrupt
//@}



/**
TGE Enumeration

@name   TGE_Enumeration
*/
//@{

typedef enum
{
    MODE_MASTER         = 0,   ///<
    MODE_SLAVE_TO_PAD      ,   ///<
    MODE_SLAVE_TO_CSI      ,   ///<
    ENUM_DUMMY4WORD(TGE_MODE_SEL)
} TGE_MODE_SEL;


typedef enum
{
    TGE_PHASE_RISING  = 0,  ///< rising edge latch/trigger
    TGE_PHASE_FALLING ,     ///< falling edge latch/trigger
    ENUM_DUMMY4WORD(TGE_DATA_PHASE_SEL)
} TGE_DATA_PHASE_SEL;

typedef enum
{
    WAVEFORM_PULSE  = 0,    ///<
    WAVEFORM_LEVEL ,        ///<
    ENUM_DUMMY4WORD(TGE_MSH_WAVEFORM_SEL)
} TGE_MSH_WAVEFORM_SEL;

typedef enum
{
    TRG_IMD          = 0,   ///<
    TRG_WAIT_VD         ,   ///<
    TRG_WAIT_EXT_TRG    ,   ///<
    ENUM_DUMMY4WORD(TGE_CTRL_PIN_TRG_SEL)
} TGE_CTRL_PIN_TRG_SEL;



typedef enum
{
    TGE_CHG_VDHD            =  0,  ///< change VD/HD                , please refer to "TGE_VDHD_INFO"
    TGE_CHG_TIMING_PAUSE        ,  ///< pause timing(VD/HD)         , please refer to "TGE_TIMING_PAUSE_INFO"
    TGE_CHG_BP                  ,  ///< CHANGE BP                   , please refer to "TGE_BREAKPOINT_INFO"

    TGE_CHG_FLSH_TRIG           ,  ///< trigger Flash                       , please refer to "TGE_CTRL_PIN_TRG_SEL"
    TGE_CHG_MSHA_CLOSE_TRIG     ,  ///< trigger M-shtr-A close pin          , please refer to "TGE_CTRL_PIN_TRG_SEL"
    TGE_CHG_MSHA_OPEN_TRIG      ,  ///< trigger M-shtr-A open  pin          , please refer to "TGE_CTRL_PIN_TRG_SEL"
    TGE_CHG_MSHB_CLOSE_TRIG     ,  ///< trigger M-shtr-B close pin          , please refer to "TGE_CTRL_PIN_TRG_SEL"
    TGE_CHG_MSHB_OPEN_TRIG      ,  ///< trigger M-shtr-B open  pin          , please refer to "TGE_CTRL_PIN_TRG_SEL"

    TGE_CHG_FLSH_CTRL           ,  ///< change  Flash setting               , please refer to "TGE_CTRL_PIN_INFO"
    TGE_CHG_MSHA_CLOSE_CTRL     ,  ///< change  M-shtr-A close pin setting  , please refer to "TGE_CTRL_PIN_INFO"
    TGE_CHG_MSHA_OPEN_CTRL      ,  ///< change  M-shtr-A open  pin setting  , please refer to "TGE_CTRL_PIN_INFO"
    TGE_CHG_MSHB_CLOSE_CTRL     ,  ///< change  M-shtr-B close pin setting  , please refer to "TGE_CTRL_PIN_INFO"
    TGE_CHG_MSHB_OPEN_CTRL      ,  ///< change  M-shtr-B open  pin setting  , please refer to "TGE_CTRL_PIN_INFO"
    TGE_CHG_IDX_NUM             ,  ///<

    ENUM_DUMMY4WORD(TGE_CHANGE_FUN_PARAM_SEL)
} TGE_CHANGE_FUN_PARAM_SEL;

typedef enum
{
    CLKSRC_PXCLK = 0, ///< TGE clock source as
    CLKSRC_MCLK     , ///< TGE clock source as
} TGE_CLKSRC_SEL;

typedef enum
{
    TGE_WAIT_VD         =  0,  ///<
    TGE_WAIT_BP1            ,  ///<
    ENUM_DUMMY4WORD(TGE_WAIT_EVENT_SEL)
} TGE_WAIT_EVENT_SEL;



typedef struct
{
    TGE_MODE_SEL        ModeSel;    ///<
    TGE_DATA_PHASE_SEL  VdPhase;    ///<
    TGE_DATA_PHASE_SEL  HdPhase;    ///<
    BOOL                bVdInv;     ///<
    BOOL                bHdInv;     ///<
    BOOL                bFlshExtTrgInv;     ///<
    BOOL                bMshExtTrgInv;     ///<
    BOOL                bFlshCtrlInv;     ///<
    BOOL                bMshACtrlInv;     ///<
    BOOL                bMshBCtrlInv;     ///<
    BOOL                bMshASwap;     ///<
    BOOL                bMshBSwap;     ///<
    TGE_MSH_WAVEFORM_SEL    MshAWvFrm;
    TGE_MSH_WAVEFORM_SEL    MshBWvFrm;
} TGE_BASIC_SET_INFO;

typedef struct
{
    UINT32      uiVdPeriod;
    UINT32      uiVdAssert;
    UINT32      uiVdFrontBlnk;
    UINT32      uiHdPeriod;
    UINT32      uiHdAssert;
    UINT32      uiHdCnt;
} TGE_VDHD_INFO;


typedef struct
{
    UINT32 uiBp1;             ///< break point 1
    UINT32 uiBp2;             ///< break point 2
} TGE_BREAKPOINT_INFO;



typedef struct
{
    UINT8       uiClkCnt;
    BOOL        bVD;
    BOOL        bHD;
    BOOL        bFlshExtTrg;
    BOOL        bMshExtTrg;
    BOOL        bFlshCtrl;
    BOOL        bMshCtrlA0;
    BOOL        bMshCtrlA1;
    BOOL        bMshCtrlB0;
    BOOL        bMshCtrlB1;
} TGE_ENGINE_STATUS_INFO;


typedef struct
{
    UINT32      uiDelay;
    UINT32      uiAssert;
    //TGE_CTRL_PIN_TRG_SEL TrigSel;
} TGE_CTRL_PIN_INFO;


typedef struct
{
    BOOL    bVdPause;
    BOOL    bHdPause;
} TGE_TIMING_PAUSE_INFO;



/**
    TGE Entire Configuration.

    Structure of entire TGE configuration.
*/
//@{
typedef struct
{
    // No run-time change
    UINT32  uiIntrpEn;
    TGE_BASIC_SET_INFO          BasicSetInfo;
    TGE_VDHD_INFO               VdHdInfo;
    TGE_BREAKPOINT_INFO         BreakPointInfo;

} TGE_MODE_PARAM;
//@}
//@}
typedef struct _TGE_OPENOBJ{
    void (*pfTgeIsrCb)(UINT32 uiIntpStatus); ///< isr callback function
    TGE_CLKSRC_SEL TgeClkSel;   ///< TGE clock source selection
} TGE_OPENOBJ;

/**
TGE API

@name   TGE_API
*/
//@{

// tge_lib.c
extern void tge_isr(void);
extern ER tge_open(TGE_OPENOBJ *pObjCB);
extern BOOL tge_isOpened(void);
extern ER tge_close(void);
extern ER tge_pause(void);
extern ER tge_start(void);
extern ER tge_setMode(TGE_MODE_PARAM *pTgeParam);
extern ER tge_chgParam(void *pParam, TGE_CHANGE_FUN_PARAM_SEL FunSel);
extern ER tge_waitEvent(TGE_WAIT_EVENT_SEL WaitEvent, BOOL bClrFlag);
extern UINT32 tge_getClockRate(void);
// tge_int.c
#endif
//@}

//@}


