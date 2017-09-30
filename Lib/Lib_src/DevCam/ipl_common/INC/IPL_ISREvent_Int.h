#ifndef _IPL_ISREVENT_INT_H_
#define _IPL_ISREVENT_INT_H_

#ifndef _IPL_COM_INT_INC
    #error "illegal include IPL common module internal .H file"
#endif

/**
    IPL_ISREvent_Int.h


    @file       IPL_ISREvent_Int.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include "IPL_Utility.h"
#include "IPL_ISREvent.h"
#include "IPL_Cmd.h"

typedef struct
{
    IPL_ISR_EVENT_FP FP[IPL_ISR_MAX_CNT];
} IPL_ISR_EVENT_TAB;

typedef struct
{
//    UINT32 ProcNum;
    IPL_ISR_EVENT_TAB EventTab;     ///<for IPL_lib inner modules
    IPL_ISR_EVENT_TAB CB_FP;        ///<for modules those are not in IPL_lib
    IPL_ISR_RAWHEADER_CB RAWHeader_CB; ///<for raw hearder callback with ext_lib
    IPL_H264_CB H264_CB;                ///< for h264 callback when trigger start
} IPL_ISR_OBJ_INFO;

extern IPL_ISR_EVENT_FP EventTab[IPL_ID_MAX_NUM][IPL_ISR_MAX_CNT];      ///<for IPL_lib inner modules
extern IPL_ISR_EVENT_FP EventCBTab[IPL_ID_MAX_NUM][IPL_ISR_MAX_CNT];    ///<for modules those are not in IPL_lib
extern IPL_ISR_RAWHEADER_CB EventRawHCBTab[IPL_ID_MAX_NUM];             ///<raw header
extern IPL_H264_CB IPLH264CBTab[IPL_ID_MAX_NUM];

extern void IPL_ISRInit(IPL_PROC_ID id,IPL_ISR_OBJ_INFO *Obj);
/**
    for IPL_lib inner modules
*/
#define IPL_H264CBProc(CBId, stamp, Id)     \
{                                           \
    if (IPLH264CBTab[CBId] != NULL)         \
    {                                       \
        IPLH264CBTab[CBId](stamp, Id, 0);   \
    }                                       \
}

#define IPL_RAWHCBProc(Id, frameCnt)        \
{                                           \
    if (EventRawHCBTab[Id] != NULL)         \
    {                                       \
        EventRawHCBTab[Id](Id, frameCnt);   \
    }                                       \
}

#define IPL_ISRProc(Id,IPLModule,IntStatus,idx) \
{                                               \
    if (EventTab[Id][IPLModule] != NULL)        \
    {                                           \
        EventTab[Id][IPLModule](Id,IntStatus,idx); \
    }                                           \
}

/**
    for modules those are not in IPL_lib
*/
#define IPL_ISRCBProc(Id,ISR,IntStatus,idx)     \
{                                               \
    if (EventCBTab[Id][ISR] != NULL)            \
    {                                           \
        EventCBTab[Id][ISR](Id,IntStatus,idx);     \
    }                                           \
}

extern void IPL_ISRUnInit(IPL_PROC_ID id, IPL_ISR isr);
#endif //_IPL_ISREVENT_INT_H_
