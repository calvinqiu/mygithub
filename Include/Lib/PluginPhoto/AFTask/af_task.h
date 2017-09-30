/**
    Public header file for AF

    This file is the header file that define the API and data type for AF.

    @file       af_task.h
    @ingroup    mILibAFAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.
*/
#ifndef _AF_TASK_H_
#define _AF_TASK_H_

#include "Type.h"
#include "ErrorNo.h"

/** \addtogroup mIAF */
//@{

/**
    af process id
*/
typedef enum
{
    AF_ID_1 = 0,                ///< af id 1
    //AF_ID_2                   ///< af id 2
    AF_ID_MAX_NUM,
    ENUM_DUMMY4WORD(AF_PROC_ID)
} AF_PROC_ID;

#define MAX_AF_PROC_NUM      AF_ID_MAX_NUM

/**
    af status
*/
typedef enum
{
    AF_TASK_STATUS_CLOSE = 0,
    AF_TASK_STATUS_OPEN  = 1,
    AF_TASK_STATUS_START = 2,
    AF_TASK_STATUS_PROC  = 3,
    AF_TASK_STATUS_PAUSE = 4,
    ENUM_DUMMY4WORD(AF_TASK_STATUS)
} AF_TASK_STATUS;

/**
    af event function type
*/
typedef void (*AF_EVENT_FP)(UINT32 id, UINT32 CurrentStatus);

/**
    caf process function type
*/
typedef void (*AF_PROC_CAF)(UINT32 id);

/**
    Register CAF function
    @param CAF function
*/
void AFTsk_RegisterCAF(AF_PROC_CAF pAFTskCAF);

/**
    caf trigger function.
*/
void AF_CAFProcess(AF_PROC_ID Id);

void AFTsk_SetCafRate(AF_PROC_ID Id, UINT8 rate);

/**
     Lock caf process(for IPL control)

     @param[in] lock          lock or unlock caf process
     @return void.
*/
void AFTsk_LockCAF(AF_PROC_ID Id, UINT8 lock);

/**
     Report if caf process is lock or not(for IPL control)

     @param[in] void
     @return
        - @b TRUE:    caf is lock.
        - @b FALSE:   caf is unlock.
*/
BOOL AFTsk_IsCAFLock(AF_PROC_ID Id);

/**
    af event

    control state:

        AF_Open:    do nothing
        AF_Start:   Start Event -> Process Event -> Process Event -> Process Event....
        AF_Pause:   ....->Process Event -> Process Event -> Process Pause
        AF_Resume:  Start Event -> Process Event -> Process Event -> Process Event....
*/
typedef struct
{
    AF_EVENT_FP Start;
    AF_EVENT_FP Process;
    AF_EVENT_FP Pause;
} AF_EVENT_OBJ;

/**
    af input object
*/
typedef struct
{
    #if 1 //af_tmp
    AF_EVENT_OBJ EventTab;     ///< event table
    #else
    UINT32 ProcNum;
    AF_EVENT_OBJ EventTab[AF_ID_MAX_NUM];
    #endif
} AF_OBJ_INFO;

/**
     check af task status.

     @param[in] Id          af process id (AF_PROC_ID)
     @return Description of data returned.
         - @b AF_TASK_STATUS: AF_TASK_STATUS_CLOSE,
                              AF_TASK_STATUS_OPEN,
                              AF_TASK_STATUS_START,
                              AF_TASK_STATUS_PROC,
                              AF_TASK_STATUS_PAUSE
*/
AF_TASK_STATUS AF_GetCtrlObjStatus(AF_PROC_ID Id);

/**
    install AF control id
*/
void AF_InstallID(void) _SECTION(".kercfg_text");

/**
     Wait until AF task is pause.

     @param[in] Id          af process id (AF_PROC_ID)
     @return Description of data returned.
         - @b FLGPTN: FLGPTN_START_DONE | FLGPTN_PAUSE_DONE
*/
FLGPTN AF_WaitIdle(AF_PROC_ID Id);

/**
     Af lock/unlock for user suspend af process

     @param[in] Id          af process id (AF_PROC_ID)
     @param[in] flag        lock flag: enable, unlock flag: enable
     @param[in] WaitFinish  wait suspend flow finish
*/
void AF_Lock(AF_PROC_ID Id, BOOL flag, BOOL WaitFinish);

/**
     check af is lock or not

     @param[in] Id          af process id (AF_PROC_ID)
     @return Description of data returned.
         - @b BOOL: true lock, false unlock
*/
BOOL AF_IsLock(AF_PROC_ID Id);

/**
     trigger event, for IPL hook

     @param[in] Id          af process id (AF_PROC_ID)
*/
void AF_TrigEvent(AF_PROC_ID Id);

/**
     open af process

     @param[in] Id          af process id
     @param[in] Obj         af process event
     @return Description of data returned.
         - @b ER: E_OK success
*/
#if 0 //af_tmp
ER AF_Open(AF_OBJ_INFO *Obj);
#else
ER AF_Open(AF_PROC_ID Id, AF_OBJ_INFO *Obj);
#endif

/**
     close af process
     @param[in] Id          af process id

     @return Description of data returned.
         - @b ER: E_OK success
*/
#if 0 //af_tmp
ER AF_Close(void);
#else
ER AF_Close(AF_PROC_ID Id);
#endif

/**
     start af process

     @param[in] Id          af process id (AF_PROC_ID)
     @param[in] WaitFinish  wait start event finish
     @return Description of data returned.
         - @b ER: E_OK success
*/
ER AF_Start(AF_PROC_ID Id, BOOL WaitFinish);

/**
     resume af process(for IPL control)

     @param[in] Id          af process id (AF_PROC_ID)
     @return Description of data returned.
         - @b ER: E_OK success
*/
ER AF_Resume(AF_PROC_ID Id);

/**
     pause af process(for IPL control)

     @param[in] Id          af process id (AF_PROC_ID)
     @param[in] WaitFinish  wait pause event finish
     @return Description of data returned.
         - @b ER: E_OK success
*/
ER AF_Pause(AF_PROC_ID Id, BOOL WaitFinish);

//@}
#endif //_AF_TASK_H_
