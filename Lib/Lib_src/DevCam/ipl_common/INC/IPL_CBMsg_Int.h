#ifndef _IPL_CBMSG_INT_H_
#define _IPL_CBMSG_INT_H_

#ifndef _IPL_COM_INT_INC
    #error "illegal include IPL common module internal .H file"
#endif

/**
    IPL_CBMsg_Int.h


    @file       IPL_CBMsg_Int.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include "Type.h"
#include "IPL_CBMsg.h"

/**
     initial callback message function

     @param[in] Id IPL process Id
     @return [ER]
         - @b E_OK: SUCCESS
         - @b E_SYS: FAIL
*/
extern void IPL_CBMsgInit(IPL_PROC_ID Id, IPL_CBMSG_FP fp);

/**
     un-initial callback message function

     @param[in] Id IPL process Id
     @return [ER]
         - @b E_OK: SUCCESS
         - @b E_SYS: FAIL
*/
extern void IPL_CBMsgUnInit(IPL_PROC_ID Id);

/**
     send CB message

     @param[in] Id IPL process Id
     @param[in] IPL_CBMSG ipl callback message
     @param[in] void* data pointer
*/
extern void IPL_CBMsgProc(IPL_PROC_ID Id, IPL_CBMSG Msg, void *Data);

#endif //_IPL_CBMSG_INT_H_
