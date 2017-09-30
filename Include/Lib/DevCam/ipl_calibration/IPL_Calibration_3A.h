#ifndef _IPL_CALIBRATION_3A_H_
#define _IPL_CALIBRATION_3A_H_
/**
    ipl calibration 3A

    this calibration 3A is for getting proper cal. condition.

    @file       IPL_Calibration_3A.h
    @ingroup    mILibCal

    Copyright   Novatek Microelectronics Corp. 2013.  All rights reserved.
*/

#include "Type.h"
#include "ErrorNo.h"
#include "Sensor.h"
#include "IPL_Utility.h"
#include "IPL_Hal_IPE_Info.h"

/** \addtogroup mILibCal */
//@{

/**
    calibration auto exposure fix option
*/
typedef enum _CAL_AE_TYPE
{
    AE_ADJ_GAIN = 0x00, ///< adjust iso
    AE_ADJ_TIME,        ///< adjust exposure time
    AE_ADJ_ISOBASE,     ///< adjust ISO Base
    AE_FIX_ALL,         ///< Fix iso/base and exposure time
    ENUM_DUMMY4WORD(CAL_AE_TYPE)
} CAL_AE_TYPE;

/**
    calibration current auto exposure status
*/
typedef enum _CAL_AE_STATUS
{
    CAL_AE_NONE    = 0, ///< none
    CAL_AE_RUNNING = 1, ///< converging lumi.
    CAL_AE_TIMEOUT,     ///< lumi. can't be converged to target lumi.
    CAL_AE_STABLE,      ///< lumi. is converged to target lumi.
    ENUM_DUMMY4WORD(CAL_AE_STATUS)
} CAL_AE_STATUS;

/*
    calibration current awb status
*/
typedef enum _CAL_AWB_STATUS
{
    CAL_AWB_NONE    = 0, ///< none
    CAL_AWB_OK,      ///< awb average OK
    ENUM_DUMMY4WORD(CAL_AWB_STATUS)
} CAL_AWB_STATUS;

/**
    Calibration AE setting
*/
typedef struct _CAL_AE_INFO
{
    BOOL            bPreGamma;           ///< TRUE if source is before gamma proc.
    CAL_AE_TYPE     uiAEType;            ///< converge type
    UINT32          uiTarget;            ///< taget luminance
    UINT32 uiExpT[SEN_MFRAME_MAX_NUM];   ///< exposure time
    UINT32 uiRatio[SEN_MFRAME_MAX_NUM];  ///< CompensateRatio
    UINT32 uiISO[SEN_MFRAME_MAX_NUM];    ///< ISO
    UINT32 uiISOBase;                    ///< ISO Base
    UINT32 uiAdjStep;                    ///< AE adjust step
    UINT32 uiTimeout;                    ///< timeout count
} CAL_AE_INFO;

/**
    calibration ae result
*/
typedef struct _CAL_AE_RST
{
    CAL_AE_STATUS status;       ///< rst status
    UINT32 uiExpT[SEN_MFRAME_MAX_NUM];   ///< current exposure time
    UINT32 uiRatio[SEN_MFRAME_MAX_NUM];  ///< current CompensateRatio
    UINT32 uiISO[SEN_MFRAME_MAX_NUM];    ///< current ISO
    UINT32 uiISOBase;           ///< current ISO Base
    UINT32 convergeLum;           ///< final converge lum in AE
}CAL_AE_RST;

/*
    calibration awb result
*/
typedef struct _CAL_AWB_RST
{
    CAL_AWB_STATUS status;       ///< rst status
    UINT32 Rgain;                ///< current R gain
    UINT32 Bgain;                ///< current B gain
}CAL_AWB_RST;

/**
    calibration 3A operation structure
    @note plz refer to Cal_3AEvent
*/
typedef struct _CAL_3A_OBJ
{
    CAL_AE_RST  (*fpAEStable)(IPL_RAW_IMG_INFO* pRaw,CAL_AE_INFO aeInfo,IPL_PROC_ID Id);   ///< plz refer to Cal_3AEvent
    CAL_AWB_RST (*fpAWBStable)(IPL_RAW_IMG_INFO* pRaw,CCRange BitSel);
}CAL_3A_OBJ,*pCAL_3A_OBJ;

/**
     calibration 3A operation


     @return Description of data returned.
         - @b fpAEStable:   search defect pixel on bayer image.
\n                      input para:
\n                          pRaw:   image information
\n                          aeInfo: converging condition
\n                          SenId:  sensor Idx
\n                      return:
\n                          the result of convergence
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern pCAL_3A_OBJ Cal_3AEvent(void);
//@}
#endif
