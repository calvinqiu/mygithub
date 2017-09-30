#ifndef _RSC_TASK_H_
#define _RSC_TASK_H_

#include "stdarg.h"
#include "Type.h"
#include "IPL_Utility.h"

typedef void (*RSC_TSK_FP)(void);

typedef struct
{
    RSC_TSK_FP fpEvent;
    UINT32     EventID;
}RSC_EVENT;

#define FLGPTN_RSC_IDLE      FLGPTN_BIT(0)

#define RSC_BASE    FLGPTN_BIT(0)
#define RSC_OP1     FLGPTN_BIT(1)
#define RSC_OP2     FLGPTN_BIT(2)
#define RSC_OP3     FLGPTN_BIT(3)
#define RSC_OP4     FLGPTN_BIT(4)
#define RSC_OP5     FLGPTN_BIT(5)
#define RSC_OP6     FLGPTN_BIT(6)
#define RSC_OP7     FLGPTN_BIT(7)
#define RSC_OP8     FLGPTN_BIT(8)
#define RSC_OP9     FLGPTN_BIT(9)
#define RSC_OP10    FLGPTN_BIT(10)
#define RSC_OP11    FLGPTN_BIT(11)
#define RSC_OP12    FLGPTN_BIT(12)
#define RSC_OP13    FLGPTN_BIT(13)
#define RSC_OP14    FLGPTN_BIT(14)
#define RSC_OP15    FLGPTN_BIT(15)
#define RSC_OP_MASK (RSC_OP1|RSC_OP2|RSC_OP3|RSC_OP4|RSC_OP5|RSC_OP6|RSC_OP7|RSC_OP8|RSC_OP9|RSC_OP10|RSC_OP11|RSC_OP12|RSC_OP13|RSC_OP14|RSC_OP15)
#define RSC_OP_MAX  15

extern RSC_TSK_FP* RSC_GetCmdEventTbl(void);
extern void RSC_SetCmdEventTbl(RSC_EVENT* pFpTbl,UINT32 TblLen);
extern ER RSC_SetCmd(UINT32 Cmd);
extern BOOL RSC_IsIdle(void);

/**
    install RSC control id
*/
extern void RSC_InstallID(void) _SECTION(".kercfg_text");

/////////////////////////////////////////////////
extern void RSC_Tsk(void);
extern void RSC_OpenTsk(void);
extern void RSC_CloseTsk(void);
extern BOOL RSC_TskOpened(void);
extern void RSC_InstallCmd(void);

#endif
