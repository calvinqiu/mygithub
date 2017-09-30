#ifndef _RSC_COMMON_H_
#define _RSC_COMMON_H_
/*
    IPL flow Callback Function

    This file is the API of the flow callback function.

    @file       RSC_Sample_Int.h
    @ingroup    mISYSAlg
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.
*/

#include "IPL_Utility.h"
#include "IPL_Cmd.h"
#include "rsc_taskctrl_int.h"
#include "Sensor.h"
#include "IPL_Hal_DCE_Info.h"
#include "IPL_Hal.h"

//#NT#2016/06/03#liwei cheng -begin
//#NT#add Dimension control and TYPE control
#define RSC_DIM_CTRL     1       //seperate=0,together=1
#define RSC_TYPE_CTRL    0       //fully control=0,partial control=1
//#NT#2016/06/03#liwei cheng -end
/**
    Rolling Shutter Correction On/Off
*/
typedef enum
{
    SEL_RSC_OFF = 0,            ///<-
    SEL_RSC_ON,                 ///<-
    SEL_RSC_MAX,
    ENUM_DUMMY4WORD(SEL_RSC_STATUS)
}SEL_RSC_STATUS;
/**
    Rolling Shutter Correction Crop enable/disable
*/
typedef enum
{
    SEL_RSC_CROPDISABLE = 0,            ///<-
    SEL_RSC_CROPENABLE,                 ///<-
    SEL_RSC_CROPMAX,
    ENUM_DUMMY4WORD(SEL_RSC_CROP)
}SEL_RSC_CROP;
/**
    Rolling Shutter Correction UI enable/disable
*/
typedef enum
{
    SEL_RSC_PRV = 0,            ///<-
    SEL_RSC_CAP,                 ///<-
    SEL_RSC_PRV_CAP,
    SEL_RSC_NONE,
    SEL_RSC_UIMAX,
    ENUM_DUMMY4WORD(SEL_RSC_UISTATUS)
}SEL_RSC_UISTATUS;
typedef enum
{
    SEL_RSC_IDLE = 0,
    SEL_RSC_CHANGEMODE,
    SEL_RSC_RUNTIME,
    ENUM_DUMMY4WORD(SEL_RSC_MODECHG)
}SEL_RSC_MODECHG;
/**
     Alg Information CB
*/
/**
     IPL raw header module close

     @param[in] gyrodata

     @return Description of data returned.

*/
extern void RSC_GetGyroData(RSCCTRL_GYRO_TBL* gyrodata );
/**
     Get EIS Crop Ratio

     @param[in] Id       ipl process id

     @return Description of data returned.
         - @b UINT32     DIS Crop Ratio
*/
extern UINT32 RSC_GetEISRatio(IPL_PROC_ID Id, IPL_MODE NextMode, SENSOR_MODE NextSensoeMode);
/**
     Get RSC On OFF information

     @param[in] Id       ipl process id

     @return Description of data returned.
         - @b SEL_RSC_STATUS    RSC STATUS(ON/OFF)
*/
extern SEL_RSC_STATUS RSC_GetStatus(IPL_PROC_ID Id);
/**
     Set RSC On or Off information

     @param[in] Id       ipl process id
     @param[in] Value    RSC On(Off) Status

     @return Description of data returned.
         - @b none
*/
extern void RSC_SetStatus(IPL_PROC_ID Id, SEL_RSC_STATUS Value);
/**
     Open RSC

     @param[in] Id       ipl process id
     @param[in] Value    RSC On(Off) Status

     @return Description of data returned.
         - @b none
*/
extern void RSC_Process(IPL_PROC_ID Id, UINT32 Value);
extern IPL_MODE RSC_GetCurMode(IPL_PROC_ID Id);
extern void RSC_SetSwitch(IPL_PROC_ID Id, SEL_RSC_MODECHG Value ,SEL_RSC_STATUS Value2);
extern SEL_RSC_UISTATUS   RSC_GetUIStatus(IPL_PROC_ID Id);
extern SEL_RSC_STATUS RSC_OnOffInfor(IPL_PROC_ID Id);
extern void RSC_GetNull2D(IPL_PROC_ID Id,UINT32 SieOutH,UINT32 SieOutV,UINT32 sizeH,UINT32 sizeV,DCE_SUBFUNC* pDce_sub,IPL_HAL_GROUP* group);
extern ER RSC_SetDistorTable(RSC_LENS_DISTORT_INFO* address);
#endif //RSC_COMMON_H
