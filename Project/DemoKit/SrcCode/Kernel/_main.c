/**
    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.

    @file       _main.c
    @ingroup

    @brief      first C entry function (low-level function to start Kernel)
                first C entry function( low-level function to start Kernel)

    @note       Nothing.
*/

#include <stdio.h>
#include <stdlib.h>
#include "Type.h"
#include "SysCfg.h"
#include "BinInfo.h"
#include <math.h>
#include "PrjCfg.h"

#include "DxSys.h"
#include "GxSystem.h"
#include "SysMain.h"
//#NT#2016/02/18#Lincy Lin -begin
//#NT#Support logfile function
#if (LOGFILE_FUNC==ENABLE)
#include "LogFile.h"
#endif
//#NT#2016/02/18#Lincy Lin -end

#define _MAIN_BREAK         DISABLE

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          os_main
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////
extern void user_exceptionCB(UINT32 * tbl);
extern void user_stackoverflowCB(UINT32 tcb);
EXP_HANDLER user_exp_handler =
{
    user_exceptionCB,
    user_stackoverflowCB
};
#if(_MAIN_BREAK == ENABLE)
void _main_break(void);
#endif
void _fill_draminfo(void);
void _clear_stack(void);

extern BININFO gBinInfo;
TM_BOOT_ARRAY(128);
static UINT32 LdLoadTime = 0;

void OS_Main(void)
{
    TM_BOOT_INIT(128);
    //if(gBinInfo.ld.LdCtrl & LDCF_PARTLOAD_EN)
    //#NT#2016/02/25#Niven Cho -begin
    //#NT#fine tune linux-cardv boot time
    LdLoadTime = gBinInfo.ld.LdLoadTime;
    //LdLoadTime = 59000;
    //#NT#2016/02/25#Niven Cho -end
    //if(LdLoadTime > 270000)
    //    LdLoadTime -= 270000;
    //if(LdLoadTime > 260000)
    //    LdLoadTime -= 260000;
    TM_BOOT_BEGIN("Ldr",0);
    TM_BOOT_END("Ldr",(char*)(&LdLoadTime)); //save "loader loadtime" => TM nand_load
    #if(_MAIN_BREAK == ENABLE)
    _main_break(); //enable this line for trace boot flow
    #endif
    TM_BOOT_BEGIN("os","init");
    //#NT#2016/02/18#Lincy Lin -begin
    //#NT#Support logfile function
    #if (LOGFILE_FUNC==ENABLE)
    LogFile_SetConsole(LOGFILE_CON_UART_AND_STORE);
    #else
    DxSys_Entry();
    #endif
    //#NT#2016/02/18#Lincy Lin -end

    DBG_DUMP("\r\nF/W %s Start ...\r\n",_CHIP_NAME_);
    DBG_IND("(OS_Main begin)\r\n");

    DBG_IND("(reg exp handler)\r\n");

    DxSys_RegExpHandler(&user_exp_handler);

    /////////////////////////////////////////////////////////////////////////////////////
    DBG_IND("(kernel init)\r\n");
    DxSys_Validate(MEM_DRAM_SIZE); //(check DRAM size)
    //Config OS (Config Memory Layout)
    OS_Init(MEM_DRAM_ADDR, MEM_DRAM_SIZE);
    OS_BeginCode();
    {
        MEM_RANGE* pCodeSection = (MEM_RANGE*)(MEM_DRAM_ADDR+CODE_SECTION_OFFSET);
        MEM_RANGE* pZISection = (MEM_RANGE*)(MEM_DRAM_ADDR+ZI_SECTION_OFFSET);
        DBG_IND("(Install code section)\r\n");
        //Config code sections for partial-load
        OS_CONFIG_CODESECTION(pCodeSection);
        DBG_IND("(Install code zi)\r\n");
        //Config ZI section
        OS_CONFIG_ZISECTION(pZISection);
    }
    OS_EndCode();
    OS_BeginStack();
    {
        //Install driver OS resource (Config Task,Flag,Semaphore)
        //  Note: stack is config by Task
        DBG_IND("(Install Drv ID)\r\n");
        Install_Drv();
        //Install OS resource (Config Task,Flag,Semaphore) for user
        //  Note: stack is config by Task
        DBG_IND("(Install User ID)\r\n");
        Install_User();
    }
    OS_EndStack();
    OS_BeginHeap();
    {
        //Install OS resource (Config MemPool) for user
        //  Note: heap is config by MemPool
        DBG_IND("(Install User ID for mempool)\r\n");
        Install_UserMEM();
    }
    OS_EndHeap();
    _fill_draminfo();
    _clear_stack();
    if(__DBGLVL__ > 1)
    {
        OS_DumpMemRange();
        OS_DumpKernelResCnt();
    }
    /////////////////////////////////////////////////////////////////////////////////////
    DBG_IND("(OS_Main end)\r\n");
    TM_BOOT_END("os","init");
}

//===========================================================================

extern BININFO gBinInfo;

#define DRAMINFO_TOTAL_START    (gBinInfo.dram.DramInfo[0])
#define DRAMINFO_TOTAL_SIZE     (gBinInfo.dram.DramInfo[1])
#define DRAMINFO_CODE_START     (gBinInfo.dram.DramInfo[2])
#define DRAMINFO_CODE_SIZE      (gBinInfo.dram.DramInfo[3])
#define DRAMINFO_STACK_START    (gBinInfo.dram.DramInfo[4])
#define DRAMINFO_STACK_SIZE     (gBinInfo.dram.DramInfo[5])
#define DRAMINFO_HEAP_START     (gBinInfo.dram.DramInfo[6])
#define DRAMINFO_HEAP_SIZE      (gBinInfo.dram.DramInfo[7])
#define DRAMINFO_RESV_START     (gBinInfo.dram.DramInfo[8])
#define DRAMINFO_RESV_SIZE      (gBinInfo.dram.DramInfo[9])

void _fill_draminfo(void)
{
    //Fill memory range to BinInfo

    //Total start
    DRAMINFO_TOTAL_START = OS_GetMemAddr(MEM_TOTAL);
    //Total end
    DRAMINFO_TOTAL_SIZE = OS_GetMemSize(MEM_TOTAL);
    //Code start
    DRAMINFO_CODE_START = OS_GetMemAddr(MEM_CODE);
    //Code size
    DRAMINFO_CODE_SIZE = OS_GetMemSize(MEM_CODE);
    //Stack start
    DRAMINFO_STACK_START = OS_GetMemAddr(MEM_STACK);
    //Stack size
    DRAMINFO_STACK_SIZE = OS_GetMemSize(MEM_STACK);
    //Heap start
    DRAMINFO_HEAP_START = OS_GetMemAddr(MEM_HEAP);
    //Heap size
    DRAMINFO_HEAP_SIZE = OS_GetMemSize(MEM_HEAP);
    //Resv start
    DRAMINFO_RESV_START = OS_GetMemAddr(MEM_RESV);
    //Resv size
    DRAMINFO_RESV_SIZE = OS_GetMemSize(MEM_RESV);
}

void _clear_stack(void)
{
    memset((void *)OS_GetMemAddr(MEM_STACK), 0x00, OS_GetMemSize(MEM_STACK));
}

#if(_MAIN_BREAK == ENABLE)
void _main_break(void)
{
    static int a=0;
    while(!a)
    {
        a=a+a;
    }
    debug_msg("^N(go)\r\n");
}
#endif

//===========================================================================
//Config assert handler

#ifdef __ECOS
#include <cyg/infra/cyg_type.h>        // base types
#include <cyg/infra/cyg_trac.h>        // tracing macros
#include <cyg/infra/cyg_ass.h>         // assertion macros
void __assert_func(const char *fileName, int line, const char *funcName, const char *condition) __attribute__ ((__noreturn__));
externC void
cyg_assert_fail( const char *psz_func, const char *psz_file,
                 cyg_uint32 linenum, const char *psz_msg ) __THROW
{
    __assert_func(psz_file, linenum, psz_func, psz_msg); //call to LibC assert func
}
#endif
void __assert_func(const char *fileName, int line, const char *funcName, const char *condition)
{
    debug_msg("^RAssert:'%s' is failed! (%s:%u)\r\n", condition, fileName, line);
#ifdef __ECOS
    debug_msg("(halt)\r\n");while(1);
#else
    abort();
#endif
}

//===========================================================================
//Config float math exception handler

//for math lib linking
int __errno = 0;
//for math lib linking
int matherr(struct exception *e)
{
    debug_msg("^R*** Math error: type=%d name=\"%s\" arg1=%f arg2=%f retval=%f err=%d\r\n",
        e->type, e->name, e->arg1, e->arg2, e->retval, e->err);
    return 0;
}

//===========================================================================
//Config cpu exception handler

void user_exceptionCB(UINT32 * tbl)
{
    if(tbl[EXP_ISR_FLAG] == TRUE)
    {
        debug_msg("^R*** CPU Exception in ISR! cause=0x%08x, addr=0x%08x\r\n",
            tbl[EXP_CAUSE], tbl[EXP_PC]);
    }
    else
    {
        debug_msg("^R*** CPU Exception in Task[%s]! cause=0x%08x, addr=0x%08x\r\n",
            OS_GetTaskName(), tbl[EXP_CAUSE], tbl[EXP_PC]);
    }
#if (_EMULATION_ == ENABLE)
    while(1){};
#endif
/*

/////////////////////// Reason of CPU Exception CAUSE /////////////////////////

0 0x00 [Int] Interrupt
- possible reasons:

1 0x01 [Mod] TLB modification exception
- possible reasons:

2 0x02 [TLBL] TLB exception (load or instruction fetch)
- possible reasons: c code "read from invalid address"
- possible reasons: c code "run on invalid address"

3 0x03 [TLBS] TLB exception (store)
- possible reasons: c code "write to invalid address"

4 0x04 [AdEL] Address error exception (load or instruction fetch)
- possible reasons: c code "read from unalign address"
- possible reasons: c code "run on unalign address"

5 0x05 [AdES] Address error exception (store)
- possible reasons: c code "write to unalign address"

6 0x06 [IBE] Bus error exception (instruction fetch)
- possible reasons: c code "run on cache address, but cache is not initial correctly"

7 0x07 [DBE] Bus error exception (data reference: load or store)
- possible reasons: c code "read from cache address, but cache is not initial correctly"
- possible reasons: c code "write to cache address, but cache is not initial correctly"

8 0x08 [Sys] Syscall exception
- possible reasons: asm code "syscall" instruction

9 0x09 [Bp] Breakpoint exception.
- possible reasons: asm code "break" instruction

10 0x0a [RI] Reserved instruction exception
- possible reasons: c code "call lib math error"

11 0x0b [CpU] Coprocessor Unusable exception
- possible reasons:

12 0x0c [Ov] Arithmetic Overflow exception
- possible reasons: asm code "signed integer is overflow after add operation"

13 0x0d [Tr] Trap exception
- possible reasons: c code "integer divided by zero"
- possible reasons: asm code "trap" instructions

14 0x0e - Reserved
- possible reasons:

15 0x0f [FPE] Floating point exception
- possible reasons:

16 0x10 [IS1] Coprocessor 2 implementation specific exception
17 0x11 [CEU] CorExtend Unusable
18 0x12 [C2E] Precise Coprocessor 2 exception
19 0x13 - Reserved
20 0x14 - Reserved
21 0x15 - Reserved
22 0x16 - Reserved

23 0x17 [WATCH] Reference to WatchHi/WatchLo address
=> cause: user code is access memory that is protected by CPU watch

24 0x18 [MCheck] Machine checkcore
25 0x19 - Reserved
26 0x1a [DSPDis] DSP ASE State Disabled exception
27 0x1b - Reserved
28 0x1c - Reserved
29 0x1d - Reserved
30 0x1e [CacheErr] Cache error.
31 0x1f - Reserved
*/

/*

1.integer divided by zero --> exception = 0x09 or 0x0d

  enable/disable
    -mcheck-zero-division [default]
    -mno-check-zero-division

  its exception can be configured by
    -mdivide-traps [default]
    -mdivide-breaks

2.float divided by zero --> NO exception for current Soft-FP

*/
}

//===========================================================================
//Config stack-overflow exception handler

#ifdef __ECOS
// eCos kernel just generate assert fail for these kinds of stack corrupt:
// 1."Stack base corrupt"
// 2."Stack top corrupt"
// 3."Gap between stack limit and base corrupt"
// EX:
// Thread "t9":: Stack base corrupt - i: 0
// 80CA7EE0: 54 37 D0 80 20 C0 09 80  EA 5E C4 82 EB 5F C5 83  |T7.. ....^..._..|
// 80CA7EF0: EC 58 C2 84 ED 59 C3 85  4C 37 D0 80 00 00 95 80  |.X...Y..L7......|
// ASSERT FAIL: <C2>thread.inl[120]void Cyg_HardwareThread::check_stack() Stack base corrupt
// ASSERT FAIL: <C2>[120]void Cyg_HardwareThread::check_stack() Stack base corrupt
#endif
void user_stackoverflowCB(UINT32 tcb)
{
#ifndef __ECOS
    volatile struct tcb* pTcb = (volatile struct tcb*)tcb;
    debug_msg("^R*** StackOverflow task[%d][%s] stk start[0x%08x] size[0x%08x]\r\n",
        pTcb->p_iden, tasknametable[pTcb->p_iden], pTcb->p_istk, pTcb->p_isiz);
#endif

    //TODO:
    //1.dump current stack
    //2.extract frame address
    //3.lookup frame address to API name
}

//===========================================================================




//===========================================================================
#include "FwSrvApi.h"

extern UINT32 UserSection_Load[10];
extern UINT32 UserSection_Order_full[];
extern void UserSection_LoadCb(const UINT32 Idx);

//Config code sections for partial-load
//To assign content of each section,
//please edit the code info file: \Project\DemoKit\Kernel\CodeInfo.s
//please edit the linker script file: \Project\DemoKit\[Model].lds

//This array is sort by section id
UINT32 UserSection_Load[10] = {1,0,0,0,0,0,0,0,0,0};

//This array is sort by loading order
#if (_FW_PARTIAL_COMPRESS_==_FW_PARTIAL_COMPRESS_ON_)
UINT32 UserSection_Order_full[] =
    {CODE_SECTION_02,FWSRV_PL_BURST_END_TAG};
#else
UINT32 UserSection_Order_full[] =
    {CODE_SECTION_02,CODE_SECTION_03,CODE_SECTION_04,CODE_SECTION_05,CODE_SECTION_06,
     CODE_SECTION_07,CODE_SECTION_08,CODE_SECTION_09,CODE_SECTION_10,FWSRV_PL_BURST_END_TAG};
#endif

int order = 1;
void UserSection_LoadCb(const UINT32 Idx)
{
    DBG_MSG("^G   Section-%.2ld: Range[0x%08X~0x%08X] Size=0x%08X (LOAD) - %d\r\n",
        Idx+1, OS_GetMemAddr(Idx), OS_GetMemAddr(Idx)+OS_GetMemSize(Idx), OS_GetMemSize(Idx), Perf_GetCurrent());

    UserSection_Load[Idx] = 1; //mark loaded
    order++;
//#NT#2016/03/30#Jeah Yen -begin
//#NT#isolate fast boot code
#if (POWERON_FAST_BOOT == ENABLE)
    if(Idx == 0) //Section-01
    {
        INIT_SETFLAG(FLGINIT_LOADCODE1);
    }
    if(Idx == 1) //Section-02
    {
        INIT_SETFLAG(FLGINIT_LOADCODE2);
    }
    if(Idx == 9) //Section-10
    {
        INIT_SETFLAG(FLGINIT_LOADCODE3);
    }
#endif
//#NT#2016/03/30#Jeah Yen -end
}

