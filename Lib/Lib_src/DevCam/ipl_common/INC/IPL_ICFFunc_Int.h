/**
    ICFFunc.

    @file       IPL_ICFFunc_Int.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _IPL_PLUG_ICF_INT_H_
#define _IPL_PLUG_ICF_INT_H_

#ifndef _IPL_COM_INT_INC
    #error "illegal include IPL common module internal .H file"
#endif

#include "Type.h"
#include "IPL_Utility.h"
/**
    ICFFUNC event function type
*/
typedef void (*ICFFUNC_EVENT_FP)(UINT32 Id);

/**
    ICFFunc event

    control state:

        ICFFunc_Open:    do nothing
        ICFFunc_Trig:    Process Event -> Process Event -> Process Event....
        ICFFunc_Pause:   ....->Process Event -> Process Event -> Process Pause
        ICFFunc_Resume:  Start Event -> Process Event -> Process Event -> Process Event....
*/
typedef struct _ICFFUNC_EVENT_OBJ
{
    ICFFUNC_EVENT_FP Process;    ///< process event
} ICFFUNC_EVENT_OBJ;

/**
    ICFFunc input object
*/
typedef struct
{
    ICFFUNC_EVENT_OBJ EventTab;   ///< event table
} ICFFUNC_OBJ_INFO;

/**
    ICF Funct Select
*/
typedef enum _ICFFUNC_EN
{
    ICF_FUNC_NONE   = 0,    ///< NONE
    ICF_FUNC_WDR    = 1,    ///< WDR
    ICF_FUNC_SHDR   = 2,    ///< Sensor HDR
    ICF_FUNC_STITCH = 3,    ///< Image Stitch
    ICF_FUNC_MAX    = 4,
    ENUM_DUMMY4WORD(ICFFUNC_EN)
}ICFFUNC_EN;


/**
     open ICFFunc process
     @param[in] Id          IPL id
     @param[in] Obj         process event
     @return Description of data returned.
         - @b ER: E_OK success
*/
extern ER ICFFunc_Open(IPL_PROC_ID Id, ICFFUNC_OBJ_INFO *Obj);

/**
     close ICFFunc process
     @param[in] Id          IPL id

     @return Description of data returned.
         - @b ER: E_OK success
*/
extern ER ICFFunc_Close(IPL_PROC_ID Id);

/**
     resume ICFFunc process(for IPL control)

     @param[in] Id          IPL id
     @return Description of data returned.
         - @b ER: E_OK success
*/
extern ER ICFFunc_Resume(IPL_PROC_ID Id);

/**
     pause ICFFunc process(for IPL control)

     @param[in] Id          IPL id
     @param[in] WaitFinish  wait pause event finish
     @return Description of data returned.
         - @b ER: E_OK success
*/
extern ER ICFFunc_Pause(IPL_PROC_ID Id, BOOL WaitFinish);

/**
     check ICFFunc is paused or not(for IPL control)

     @param[in] Id          IPL id
     @return Description of data returned.
         - @b BOOL: TRUE paused, not
*/
extern BOOL ICFFunc_IsPause(IPL_PROC_ID Id);
/**
     trigger event, for IPL hook

     @param[in] Id          IPL id
*/
extern void ICFFunc_TrigEvent(UINT32 Id);

#endif //_IPL_PLUG_ICF_INT_H_
