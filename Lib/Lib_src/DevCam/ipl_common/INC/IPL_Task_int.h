/**
    IPL task.

    @file       IPL_Task_int.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _IPL_TASK_INT_H_
#define _IPL_TASK_INT_H_

#ifndef _IPL_COM_INT_INC
    #error "illegal include IPL common module internal .H file"
#endif

#include "IPL_Cmd.h"
#include "IPL_AlgInfor_Int.h"

#define FLGPTN_IPL_IDLE             FLGPTN_BIT(0)
#define FLGPTN_IPL_SET_PROC         FLGPTN_BIT(1)
#define FLGPTN_IPL_GET_PROC         FLGPTN_BIT(2)
#define FLGPTN_IPL_END              FLGPTN_BIT(3)
#define FLGPTN_IPL_END_DONE         FLGPTN_BIT(4)
#define FLGPTN_IPL_RES              FLGPTN_BIT(5)

#define FLGPTN_IPL_DZ_END_0         FLGPTN_BIT(6)
#define FLGPTN_IPL_DZ_END_1         FLGPTN_BIT(7)
#define FLGPTN_IPL_DZ_END_2         FLGPTN_BIT(8)
#define FLGPTN_IPL_DZ_END_3         FLGPTN_BIT(9)


extern void IPL_Tsk(void);
extern void IPL_InstallCmd(void);
extern UINT32 _SECTION(".kercfg_data") IPL_TaskID;
extern UINT32 _SECTION(".kercfg_data") IPL_FlgID;
#endif //_IPL_TASK_INT_H_
