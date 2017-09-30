/**
    IPH module

    this module provide consistent interface connecting to SIE/IFE/DCE/IPE/IFE2/IME/RDE engine driver

    @file       IPL_Hal.h
    @ingroup    mIIPH
    @note       none

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _IPL_HAL_OP_INT_H_
#define _IPL_HAL_OP_INT_H_

#ifndef _IPL_COM_INT_INC
    #error "illegal include IPL common module internal .H file"
#endif


#include "IPL_Hal.h"
#include "IPL_Utility.h"
/** \addtogroup mIIPH */
//@{

/**
    query operation supported by each engine
*/
typedef union
{
    IPH_SIE_QUE SieQ;
    IPH_IFE_QUE IfeQ;
    IPH_DCE_QUE DceQ;
    IPH_IPE_QUE IpeQ;
    IPH_IFE2_QUE Ife2Q;
    IPH_IME_QUE ImeQ;
    IPH_RDE_QUE RdeQ;
    IPH_ISE_QUE ISE;
    IPH_DRE_QUE DreQ;
    IPH_RHE_QUE RheQ;
    UINT32 data;
}IPH_QUE;

/**
    engine inner table info
*/
typedef union
{
    SIE_READ sieRead;
    IFE_READ ifeRead;
    DCE_READ dceRead;
    IPE_READ ipeRead;
    IFE2_READ ife2Read;
    IME_READ imeRead;
    RDE_READ rdeRead;
    DRE_READ dreRead;
    RHE_READ rheRead;
    UINT32 data;
}IPL_HAL_READ;


_INLINE UINT32 IPL_ID_TO_SIEx(IPL_PROC_ID id)
{
    UINT32 HALEngMap[IPL_ID_MAX_NUM] = {IPL_HAL_SIE1, IPL_HAL_SIE2, IPL_HAL_SIE3, IPL_HAL_SIE4};
    return HALEngMap[id];
}

typedef enum
{
    IPH_ENG_UNKNOWN = 0,
    IPH_ENG_OPEN,
    IPH_ENG_CLOSE,
    IPH_ENG_START,
    IPH_ENG_STOP,
    ENUM_DUMMY4WORD(IPH_ENG_STATUS)
} IPH_ENG_STATUS;

extern IPH_ENG_STATUS IPH_GetStatus(IPL_HAL_ENG eng);

/**
    -operation group
*/
//{

/**
     Open engine

     Open engine and hook interrupt handler call-back function
     @note : operation rule as follow.

\n         -------        -------        -----------
\n        | close |  ->  | open  |  ->  | chg/load  | <--
\n         -------        -------        -----------     |
\n            ^                             |            |
\n            |                             v            |
\n         -------                       -------         |
\n        | stop  |     <----------     | start |  ------
\n         -------                       -------
\n            ^                             |
\n            |           -------           |
\n             --------  | wait  |   <------
\n                        -------
\n
     @param[in]  eng       open indicated engine
     @param[in]  fp_ptr    ISR callback func.
     @param[in]  clk       engine working clk
     @return Description of data returned.
         - @b E_OK:     engine open
         - @b E_SYS:    engine is being opened.
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern ER IPH_ENG_Open(IPL_HAL_ENG eng,DRV_ISR fp_ptr,IPH_CLK clk, UINT32 ClkSrc);


_INLINE ER SIEx_ENG_Open(IPL_PROC_ID id, DRV_ISR fp_ptr, IPH_CLK clk,UINT32 PclkSel, UINT32 ClkSrc)
{
    PclkSel = PclkSel<<16;
    return IPH_ENG_Open((IPL_ID_TO_SIEx(id)|PclkSel), fp_ptr, clk, ClkSrc);
}

_INLINE ER IPE_ENG_Open(DRV_ISR fp_ptr, IPH_CLK clk, UINT32 ClkSrc)
{
    return IPH_ENG_Open(IPL_HAL_IPE, fp_ptr, clk, ClkSrc);
}

_INLINE ER DCE_ENG_Open(DRV_ISR fp_ptr, IPH_CLK clk, UINT32 ClkSrc)
{
    return IPH_ENG_Open(IPL_HAL_DCE, fp_ptr, clk, ClkSrc);
}

_INLINE ER IFE_ENG_Open(DRV_ISR fp_ptr,IPH_CLK clk, UINT32 ClkSrc)
{
    return IPH_ENG_Open(IPL_HAL_IFE, fp_ptr, clk, ClkSrc);
}

_INLINE ER IFE2_ENG_Open(DRV_ISR fp_ptr,IPH_CLK clk, UINT32 ClkSrc)
{
    return IPH_ENG_Open(IPL_HAL_IFE2, fp_ptr, clk, ClkSrc);
}

_INLINE ER IME_ENG_Open(DRV_ISR fp_ptr,IPH_CLK clk, UINT32 ClkSrc)
{
    return IPH_ENG_Open(IPL_HAL_IME, fp_ptr, clk, ClkSrc);
}

_INLINE ER RDE_ENG_Open(DRV_ISR fp_ptr,IPH_CLK clk, UINT32 ClkSrc)
{
    return IPH_ENG_Open(IPL_HAL_RDE, fp_ptr, clk, ClkSrc);
}

_INLINE ER ISE_ENG_Open(DRV_ISR fp_ptr,IPH_CLK clk, UINT32 ClkSrc)
{
    return IPH_ENG_Open(IPL_HAL_ISE, fp_ptr, clk, ClkSrc);
}

_INLINE ER DRE_ENG_Open(DRV_ISR fp_ptr,IPH_CLK clk,  UINT32 ClkSrc)
{
    return IPH_ENG_Open(IPL_HAL_DRE, fp_ptr, clk, ClkSrc);
}

_INLINE ER RHE_ENG_Open(DRV_ISR fp_ptr,IPH_CLK clk, UINT32 ClkSrc)
{
    return IPH_ENG_Open(IPL_HAL_RHE, fp_ptr, clk, ClkSrc);
}
#if 0
#endif
/**
     close engine

     @note : plz reference the note of api: IPH_ENG_Open().

     @param[in]  eng       close indicated engine
     @return Description of data returned.
         - @b E_OK:   engine close
         - @b E_SYS:  operation error.
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern ER IPH_ENG_Close(IPL_HAL_ENG eng);

_INLINE ER SIEx_ENG_Close(IPL_PROC_ID id)
{
    return IPH_ENG_Close(IPL_ID_TO_SIEx(id));
}

_INLINE ER IPE_ENG_Close(void)
{
    return IPH_ENG_Close(IPL_HAL_IPE);
}

_INLINE ER DCE_ENG_Close(void)
{
    return IPH_ENG_Close(IPL_HAL_DCE);
}

_INLINE ER IFE_ENG_Close(void)
{
    return IPH_ENG_Close(IPL_HAL_IFE);
}

_INLINE ER IFE2_ENG_Close(void)
{
    return IPH_ENG_Close(IPL_HAL_IFE2);
}

_INLINE ER IME_ENG_Close(void)
{
    return IPH_ENG_Close(IPL_HAL_IME);
}

_INLINE ER RDE_ENG_Close(void)
{
    return IPH_ENG_Close(IPL_HAL_RDE);
}

_INLINE ER ISE_ENG_Close(void)
{
    return IPH_ENG_Close(IPL_HAL_ISE);
}

_INLINE ER DRE_ENG_Close(void)
{
    return IPH_ENG_Close(IPL_HAL_DRE);
}

_INLINE ER RHE_ENG_Close(void)
{
    return IPH_ENG_Close(IPL_HAL_RHE);
}


#if 0
#endif
/**
     engine start to run

     @note : plz reference the note of api: IPH_ENG_Open().

     @param[in]  eng       start indicated engine
     @return Description of data returned.
         - @b E_OK:   engine start
         - @b E_SYS:  operation error.
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern ER IPH_ENG_Start(IPL_HAL_ENG eng);

_INLINE ER SIEx_ENG_Start(IPL_PROC_ID id)
{
    return IPH_ENG_Start(IPL_ID_TO_SIEx(id));
}

_INLINE ER IPE_ENG_Start(void)
{
    return IPH_ENG_Start(IPL_HAL_IPE);
}

_INLINE ER DCE_ENG_Start(void)
{
    return IPH_ENG_Start(IPL_HAL_DCE);
}

_INLINE ER IFE_ENG_Start(void)
{
    return IPH_ENG_Start(IPL_HAL_IFE);
}

_INLINE ER IFE2_ENG_Start(void)
{
    return IPH_ENG_Start(IPL_HAL_IFE2);
}

_INLINE ER IME_ENG_Start(void)
{
    return IPH_ENG_Start(IPL_HAL_IME);
}

_INLINE ER RDE_ENG_Start(void)
{
    return IPH_ENG_Start(IPL_HAL_RDE);
}

_INLINE ER ISE_ENG_Start(void)
{
    return IPH_ENG_Start(IPL_HAL_ISE);
}

_INLINE ER DRE_ENG_Start(void)
{
    return IPH_ENG_Start(IPL_HAL_DRE);
}

_INLINE ER RHE_ENG_Start(void)
{
    return IPH_ENG_Start(IPL_HAL_RHE);
}
#if 0
#endif
/**
    functional design same as IPH_ENG_Start(), but for ISR only
*/
extern ER IPH_ENG_Start_ISR(IPL_HAL_ENG eng);

_INLINE ER SIEx_ENG_Start_ISR(IPL_PROC_ID id)
{
    return IPH_ENG_Start_ISR(IPL_ID_TO_SIEx(id));
}

_INLINE ER IPE_ENG_Start_ISR(void)
{
    return IPH_ENG_Start_ISR(IPL_HAL_IPE);
}

_INLINE ER DCE_ENG_Start_ISR(void)
{
    return IPH_ENG_Start_ISR(IPL_HAL_DCE);
}

_INLINE ER IFE_ENG_Start_ISR(void)
{
    return IPH_ENG_Start_ISR(IPL_HAL_IFE);
}

_INLINE ER IFE2_ENG_Start_ISR(void)
{
    return IPH_ENG_Start_ISR(IPL_HAL_IFE2);
}

_INLINE ER IME_ENG_Start_ISR(void)
{
    return IPH_ENG_Start_ISR(IPL_HAL_IME);
}

_INLINE ER RDE_ENG_Start_ISR(void)
{
    return IPH_ENG_Start_ISR(IPL_HAL_RDE);
}

_INLINE ER ISE_ENG_Start_ISR(void)
{
    return IPH_ENG_Start_ISR(IPL_HAL_ISE);
}

_INLINE ER DRE_ENG_Start_ISR(void)
{
    return IPH_ENG_Start_ISR(IPL_HAL_DRE);
}

_INLINE ER RHE_ENG_Start_ISR(void)
{
    return IPH_ENG_Start_ISR(IPL_HAL_RHE);
}

#if 0
#endif

/**
     engine start to run for speically case

     @note : plz reference the note of api: IPH_ENG_Open().

     @param[in]  eng       start indicated engine
     @return Description of data returned.
         - @b E_OK:   engine start
         - @b E_SYS:  operation error.
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern ER IPH_ENG_Start2(IPL_HAL_ENG eng);

_INLINE ER SIEx_ENG_Start2(IPL_PROC_ID id)
{
    return IPH_ENG_Start2(IPL_ID_TO_SIEx(id));
}

#if 0
#endif
/**
     stop/pause engine

     @note : plz reference the note of api: IPH_ENG_Open().

     @param[in]  eng       stop indicated engine
     @return Description of data returned.
         - @b E_OK:   engine stop
         - @b E_SYS:  operation error.
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern ER IPH_ENG_Stop(IPL_HAL_ENG eng);

_INLINE ER SIEx_ENG_Stop(IPL_PROC_ID id)
{
    return IPH_ENG_Stop(IPL_ID_TO_SIEx(id));
}

_INLINE ER IPE_ENG_Stop(void)
{
    return IPH_ENG_Stop(IPL_HAL_IPE);
}

_INLINE ER DCE_ENG_Stop(void)
{
    return IPH_ENG_Stop(IPL_HAL_DCE);
}

_INLINE ER IFE_ENG_Stop(void)
{
    return IPH_ENG_Stop(IPL_HAL_IFE);
}

_INLINE ER IFE2_ENG_Stop(void)
{
    return IPH_ENG_Stop(IPL_HAL_IFE2);
}

_INLINE ER IME_ENG_Stop(void)
{
    return IPH_ENG_Stop(IPL_HAL_IME);
}

_INLINE ER RDE_ENG_Stop(void)
{
    return IPH_ENG_Stop(IPL_HAL_RDE);
}

_INLINE ER ISE_ENG_Stop(void)
{
    return IPH_ENG_Stop(IPL_HAL_ISE);
}

_INLINE ER DRE_ENG_Stop(void)
{
    return IPH_ENG_Stop(IPL_HAL_DRE);
}

_INLINE ER RHE_ENG_Stop(void)
{
    return IPH_ENG_Stop(IPL_HAL_RHE);
}

#if 0
#endif
/**
    functional design same as IPH_ENG_Stop(), but for ISR only
*/
extern ER IPH_ENG_Stop_ISR(IPL_HAL_ENG eng);

_INLINE ER SIEx_ENG_Stop_ISR(IPL_PROC_ID id)
{
    return IPH_ENG_Stop_ISR(IPL_ID_TO_SIEx(id));
}

_INLINE ER IPE_ENG_Stop_ISR(void)
{
    return IPH_ENG_Stop_ISR(IPL_HAL_IPE);
}

_INLINE ER DCE_ENG_Stop_ISR(void)
{
    return IPH_ENG_Stop_ISR(IPL_HAL_DCE);
}

_INLINE ER IFE_ENG_Stop_ISR(void)
{
    return IPH_ENG_Stop_ISR(IPL_HAL_IFE);
}

_INLINE ER IFE2_ENG_Stop_ISR(void)
{
    return IPH_ENG_Stop_ISR(IPL_HAL_IFE2);
}

_INLINE ER IME_ENG_Stop_ISR(void)
{
    return IPH_ENG_Stop_ISR(IPL_HAL_IME);
}

_INLINE ER RDE_ENG_Stop_ISR(void)
{
    return IPH_ENG_Stop_ISR(IPL_HAL_RDE);
}

_INLINE ER ISE_ENG_Stop_ISR(void)
{
    return IPH_ENG_Stop_ISR(IPL_HAL_ISE);
}

_INLINE ER DRE_ENG_Stop_ISR(void)
{
    return IPH_ENG_Stop_ISR(IPL_HAL_DRE);
}

_INLINE ER RHE_ENG_Stop_ISR(void)
{
    return IPH_ENG_Stop_ISR(IPL_HAL_RHE);
}
#if 0
#endif
/**
     wait indicated signal

     wait indicated signal and then clear flag.

     @note : plz reference the note of api: IPH_ENG_Open().

     @param[in]  eng       indicated engine
     @param[in]  Signal    indicated signal
     @param[in]  bflgclr   clear/non-clear signal before wait
     @return Description of data returned.
         - @b E_OK:   done.
         - @b E_SYS:  operation error.
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern ER IPH_ENG_WaitSignal(IPL_HAL_ENG eng,IPL_ISR_STATUS Signal,BOOL bflgclr);

//note: SIEx_ENG_WaitSignal timeout flow not support multi-task condition
_INLINE ER SIEx_ENG_WaitSignal(IPL_PROC_ID id, IPL_ISR_STATUS Signal, BOOL bflgclr)
{
    return IPH_ENG_WaitSignal(IPL_ID_TO_SIEx(id), Signal, bflgclr);
}

_INLINE ER IPE_ENG_WaitSignal(IPL_ISR_STATUS Signal, BOOL bflgclr)
{
    return IPH_ENG_WaitSignal(IPL_HAL_IPE, Signal, bflgclr);
}

_INLINE ER DCE_ENG_WaitSignal(IPL_ISR_STATUS Signal, BOOL bflgclr)
{
    return IPH_ENG_WaitSignal(IPL_HAL_DCE, Signal, bflgclr);
}

_INLINE ER IFE_ENG_WaitSignal(IPL_ISR_STATUS Signal, BOOL bflgclr)
{
    return IPH_ENG_WaitSignal(IPL_HAL_IFE, Signal, bflgclr);
}

_INLINE ER IFE2_ENG_WaitSignal(IPL_ISR_STATUS Signal, BOOL bflgclr)
{
    return IPH_ENG_WaitSignal(IPL_HAL_IFE2, Signal, bflgclr);
}

_INLINE ER IME_ENG_WaitSignal(IPL_ISR_STATUS Signal, BOOL bflgclr)
{
    return IPH_ENG_WaitSignal(IPL_HAL_IME, Signal, bflgclr);
}

_INLINE ER RDE_ENG_WaitSignal(IPL_ISR_STATUS Signal, BOOL bflgclr)
{
    return IPH_ENG_WaitSignal(IPL_HAL_RDE, Signal, bflgclr);
}

_INLINE ER ISE_ENG_WaitSignal(IPL_ISR_STATUS Signal, BOOL bflgclr)
{
    return IPH_ENG_WaitSignal(IPL_HAL_ISE, Signal, bflgclr);
}

_INLINE ER DRE_ENG_WaitSignal(IPL_ISR_STATUS Signal, BOOL bflgclr)
{
    return IPH_ENG_WaitSignal(IPL_HAL_DRE, Signal, bflgclr);
}

_INLINE ER RHE_ENG_WaitSignal(IPL_ISR_STATUS Signal, BOOL bflgclr)
{
    return IPH_ENG_WaitSignal(IPL_HAL_RHE, Signal, bflgclr);
}


#if 0
#endif
/**
     clear indicated signal flag

     @note none

     @param[in]  eng       indicated engine
     @param[in]  Signal    indicated signal
*/
extern ER IPH_ENG_ClrSignal(IPL_HAL_ENG eng,IPL_ISR_STATUS Signal);

_INLINE ER SIEx_ENG_ClrSignal(IPL_PROC_ID id, IPL_ISR_STATUS Signal)
{
    return IPH_ENG_ClrSignal(IPL_ID_TO_SIEx(id), Signal);
}

_INLINE ER IPE_ENG_ClrSignal(IPL_ISR_STATUS Signal)
{
    return IPH_ENG_ClrSignal(IPL_HAL_IPE, Signal);
}

_INLINE ER DCE_ENG_ClrSignal(IPL_ISR_STATUS Signal)
{
    return IPH_ENG_ClrSignal(IPL_HAL_DCE, Signal);
}

_INLINE ER IFE_ENG_ClrSignal(IPL_ISR_STATUS Signal)
{
    return IPH_ENG_ClrSignal(IPL_HAL_IFE, Signal);
}

_INLINE ER IFE2_ENG_ClrSignal(IPL_ISR_STATUS Signal)
{
    return IPH_ENG_ClrSignal(IPL_HAL_IFE2, Signal);
}

_INLINE ER IME_ENG_ClrSignal(IPL_ISR_STATUS Signal)
{
    return IPH_ENG_ClrSignal(IPL_HAL_IME, Signal);
}

_INLINE ER RDE_ENG_ClrSignal(IPL_ISR_STATUS Signal)
{
    return IPH_ENG_ClrSignal(IPL_HAL_RDE, Signal);
}

_INLINE ER ISE_ENG_ClrSignal(IPL_ISR_STATUS Signal, BOOL bflgclr)
{
    return IPH_ENG_ClrSignal(IPL_HAL_ISE, Signal);
}

_INLINE ER DRE_ENG_ClrSignal(IPL_ISR_STATUS Signal, BOOL bflgclr)
{
    return IPH_ENG_ClrSignal(IPL_HAL_DRE, Signal);
}

_INLINE ER RHE_ENG_ClrSignal(IPL_ISR_STATUS Signal, BOOL bflgclr)
{
    return IPH_ENG_ClrSignal(IPL_HAL_RHE, Signal);
}
#if 0
#endif
/**
     chg IPP_HAL inner parameter set.

     @note:this chg do not configure engine reg. directly.

     @param[in]  id        pipeline index
     @param[in]  idx       indicated param. set
     @param[in]  eng       indicated engine
     @param[in]  Idx       chg indicated param. set
     @param[in]  group     update bit
     @param[in]  pEng      update data
     @return Description of data returned.
         - @b E_OK:   done.
         - @b E_SYS:  operation error.
     Example: (Optional)
     @code
     {
        SIE sie;
        ...
        IPH_ENG_ChgParam(IPL_ID_1,IPL_HAL_SIE,grp,(void*)&sie);
     }
     @endcode
*/
extern ER IPH_ENG_ChgParam(IPL_PROC_ID id,IPL_HAL_PARASET Idx,IPL_HAL_ENG eng,IPL_HAL_GROUP group,void* pEng);

_INLINE ER SIEx_ENG_ChgParam(IPL_PROC_ID id, IPL_HAL_PARASET Idx, IPL_HAL_GROUP group, void* pSIEx_mgr)
{
    return IPH_ENG_ChgParam(id, Idx, IPL_ID_TO_SIEx(id), group, pSIEx_mgr);
}

_INLINE ER IPE_ENG_ChgParam(IPL_PROC_ID id,IPL_HAL_PARASET Idx,IPL_HAL_GROUP group,IPE* pIpe_mgr)
{
    return IPH_ENG_ChgParam(id,Idx,IPL_HAL_IPE,group,(void*)pIpe_mgr);
}

_INLINE ER DCE_ENG_ChgParam(IPL_PROC_ID id,IPL_HAL_PARASET Idx,IPL_HAL_GROUP group,DCE* pDce_mgr)
{
    return IPH_ENG_ChgParam(id,Idx,IPL_HAL_DCE,group,(void*)pDce_mgr);
}

_INLINE ER IFE_ENG_ChgParam(IPL_PROC_ID id,IPL_HAL_PARASET Idx,IPL_HAL_GROUP group,IFE* pIfe_mgr)
{
    return IPH_ENG_ChgParam(id,Idx,IPL_HAL_IFE,group,(void*)pIfe_mgr);
}

_INLINE ER IFE2_ENG_ChgParam(IPL_PROC_ID id,IPL_HAL_PARASET Idx,IPL_HAL_GROUP group,IFE2* pIfe2_mgr)
{
    return IPH_ENG_ChgParam(id,Idx,IPL_HAL_IFE2,group,(void*)pIfe2_mgr);
}

_INLINE ER IME_ENG_ChgParam(IPL_PROC_ID id,IPL_HAL_PARASET Idx,IPL_HAL_GROUP group,IME* pIme_mgr)
{
    return IPH_ENG_ChgParam(id,Idx,IPL_HAL_IME,group,(void*)pIme_mgr);
}

_INLINE ER RDE_ENG_ChgParam(IPL_PROC_ID id,IPL_HAL_PARASET Idx,IPL_HAL_GROUP group,RDE* pRde_mgr)
{
    return IPH_ENG_ChgParam(id,Idx,IPL_HAL_RDE,group,(void*)pRde_mgr);
}

_INLINE ER ISE_ENG_ChgParam(IPL_PROC_ID id,IPL_HAL_PARASET Idx,IPL_HAL_GROUP group,ISE* pIse_mgr)
{
    return IPH_ENG_ChgParam(id, Idx, IPL_HAL_ISE, group, (void*)pIse_mgr);
}

_INLINE ER DRE_ENG_ChgParam(IPL_PROC_ID id,IPL_HAL_PARASET Idx,IPL_HAL_GROUP group,DRE* pDre_mgr)
{
    return IPH_ENG_ChgParam(id, Idx, IPL_HAL_DRE, group, (void*)pDre_mgr);
}

_INLINE ER RHE_ENG_ChgParam(IPL_PROC_ID id,IPL_HAL_PARASET Idx,IPL_HAL_GROUP group,RHE* pRhe_mgr)
{
    return IPH_ENG_ChgParam(id, Idx, IPL_HAL_RHE, group, (void*)pRhe_mgr);
}

#if 0
#endif
extern ER IPH_ENG_ChgParam_ISR(IPL_PROC_ID id,IPL_HAL_PARASET Idx,IPL_HAL_ENG eng,IPL_HAL_GROUP group,void* pEng);

_INLINE ER SIEx_ENG_ChgParam_ISR(IPL_PROC_ID id, IPL_HAL_PARASET Idx, IPL_HAL_GROUP group, void* pSIEx_mgr)
{
    return IPH_ENG_ChgParam_ISR(id, Idx, IPL_ID_TO_SIEx(id), group, pSIEx_mgr);
}

_INLINE ER IPE_ENG_ChgParam_ISR(IPL_PROC_ID id,IPL_HAL_PARASET Idx,IPL_HAL_GROUP group,IPE* pIpe_mgr)
{
    return IPH_ENG_ChgParam_ISR(id,Idx,IPL_HAL_IPE,group,(void*)pIpe_mgr);
}

_INLINE ER DCE_ENG_ChgParam_ISR(IPL_PROC_ID id,IPL_HAL_PARASET Idx,IPL_HAL_GROUP group,DCE* pDce_mgr)
{
    return IPH_ENG_ChgParam_ISR(id,Idx,IPL_HAL_DCE,group,(void*)pDce_mgr);
}

_INLINE ER IFE_ENG_ChgParam_ISR(IPL_PROC_ID id,IPL_HAL_PARASET Idx,IPL_HAL_GROUP group,IFE* pIfe_mgr)
{
    return IPH_ENG_ChgParam_ISR(id,Idx,IPL_HAL_IFE,group,(void*)pIfe_mgr);
}

_INLINE ER IFE2_ENG_ChgParam_ISR(IPL_PROC_ID id,IPL_HAL_PARASET Idx,IPL_HAL_GROUP group,IFE2* pIfe2_mgr)
{
    return IPH_ENG_ChgParam_ISR(id,Idx,IPL_HAL_IFE2,group,(void*)pIfe2_mgr);
}

_INLINE ER IME_ENG_ChgParam_ISR(IPL_PROC_ID id,IPL_HAL_PARASET Idx,IPL_HAL_GROUP group,IME* pIme_mgr)
{
    return IPH_ENG_ChgParam_ISR(id,Idx,IPL_HAL_IME,group,(void*)pIme_mgr);
}

_INLINE ER RDE_ENG_ChgParam_ISR(IPL_PROC_ID id,IPL_HAL_PARASET Idx,IPL_HAL_GROUP group,RDE* pRde_mgr)
{
    return IPH_ENG_ChgParam_ISR(id,Idx,IPL_HAL_RDE,group,(void*)pRde_mgr);
}

_INLINE ER ISE_ENG_ChgParam_ISR(IPL_PROC_ID id,IPL_HAL_PARASET Idx,IPL_HAL_GROUP group,ISE* pIse_mgr)
{
    return IPH_ENG_ChgParam_ISR(id,Idx, IPL_HAL_ISE, group, (void*)pIse_mgr);
}

_INLINE ER DRE_ENG_ChgParam_ISR(IPL_PROC_ID id,IPL_HAL_PARASET Idx,IPL_HAL_GROUP group,DRE* pDre_mgr)
{
    return IPH_ENG_ChgParam_ISR(id, Idx, IPL_HAL_DRE, group, (void*)pDre_mgr);
}

_INLINE ER RHE_ENG_ChgParam_ISR(IPL_PROC_ID id,IPL_HAL_PARASET Idx,IPL_HAL_GROUP group,RHE* pRhe_mgr)
{
    return IPH_ENG_ChgParam_ISR(id, Idx, IPL_HAL_RHE, group, (void*)pRhe_mgr);
}

#if 0
#endif
/**
     Load engine reg.

     configure IPL_Hal inner parameter set into engine reg with specified group

     @note : plz reference the note of api: IPH_ENG_Open().

     @param[in]  id        pipeline index
     @param[in]  eng       indicated engine
     @param[in]  Idx       indicated param. set
     @param[in]  group     update bit
     @return Description of data returned.
         - @b E_OK:   done.
         - @b E_SYS:  operation error.
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern ER IPH_ENG_Load(IPL_PROC_ID id,IPL_HAL_PARASET Idx,IPL_HAL_ENG eng,IPL_HAL_GROUP group);

_INLINE ER SIEx_ENG_Load(IPL_PROC_ID id, IPL_HAL_PARASET Idx, IPL_HAL_GROUP group)
{
    return IPH_ENG_Load(id, Idx, IPL_ID_TO_SIEx(id), group);
}

_INLINE ER IPE_ENG_Load(IPL_PROC_ID id, IPL_HAL_PARASET Idx, IPL_HAL_GROUP group)
{
    return IPH_ENG_Load(id, Idx, IPL_HAL_IPE, group);
}

_INLINE ER DCE_ENG_Load(IPL_PROC_ID id, IPL_HAL_PARASET Idx, IPL_HAL_GROUP group)
{
    return IPH_ENG_Load(id, Idx, IPL_HAL_DCE, group);
}

_INLINE ER IFE_ENG_Load(IPL_PROC_ID id, IPL_HAL_PARASET Idx, IPL_HAL_GROUP group)
{
    return IPH_ENG_Load(id, Idx, IPL_HAL_IFE, group);
}

_INLINE ER IFE2_ENG_Load(IPL_PROC_ID id, IPL_HAL_PARASET Idx, IPL_HAL_GROUP group)
{
    return IPH_ENG_Load(id, Idx, IPL_HAL_IFE2, group);
}

_INLINE ER IME_ENG_Load(IPL_PROC_ID id, IPL_HAL_PARASET Idx, IPL_HAL_GROUP group)
{
    return IPH_ENG_Load(id, Idx, IPL_HAL_IME, group);
}

_INLINE ER RDE_ENG_Load(IPL_PROC_ID id, IPL_HAL_PARASET Idx, IPL_HAL_GROUP group)
{
    return IPH_ENG_Load(id, Idx, IPL_HAL_RDE, group);
}

_INLINE ER ISE_ENG_Load(IPL_PROC_ID id, IPL_HAL_PARASET Idx, IPL_HAL_GROUP group)
{
    return IPH_ENG_Load(id, Idx, IPL_HAL_ISE, group);
}

_INLINE ER DRE_ENG_Load(IPL_PROC_ID id, IPL_HAL_PARASET Idx, IPL_HAL_GROUP group)
{
    return IPH_ENG_Load(id, Idx, IPL_HAL_DRE, group);
}

_INLINE ER RHE_ENG_Load(IPL_PROC_ID id, IPL_HAL_PARASET Idx, IPL_HAL_GROUP group)
{
    return IPH_ENG_Load(id, Idx, IPL_HAL_RHE, group);
}

#if 0
#endif
extern ER IPH_ENG_Load_ISR(IPL_PROC_ID id,IPL_HAL_PARASET Idx,IPL_HAL_ENG eng,IPL_HAL_GROUP group);

_INLINE ER SIEx_ENG_Load_ISR(IPL_PROC_ID id, IPL_HAL_PARASET Idx, IPL_HAL_GROUP group)
{
    return IPH_ENG_Load_ISR(id, Idx, IPL_ID_TO_SIEx(id), group);
}

_INLINE ER IPE_ENG_Load_ISR(IPL_PROC_ID id, IPL_HAL_PARASET Idx, IPL_HAL_GROUP group)
{
    return IPH_ENG_Load_ISR(id, Idx, IPL_HAL_IPE, group);
}

_INLINE ER DCE_ENG_Load_ISR(IPL_PROC_ID id, IPL_HAL_PARASET Idx, IPL_HAL_GROUP group)
{
    return IPH_ENG_Load_ISR(id, Idx, IPL_HAL_DCE, group);
}

_INLINE ER IFE_ENG_Load_ISR(IPL_PROC_ID id, IPL_HAL_PARASET Idx, IPL_HAL_GROUP group)
{
    return IPH_ENG_Load_ISR(id, Idx, IPL_HAL_IFE, group);
}

_INLINE ER IFE2_ENG_Load_ISR(IPL_PROC_ID id, IPL_HAL_PARASET Idx, IPL_HAL_GROUP group)
{
    return IPH_ENG_Load_ISR(id, Idx, IPL_HAL_IFE2, group);
}

_INLINE ER IME_ENG_Load_ISR(IPL_PROC_ID id, IPL_HAL_PARASET Idx, IPL_HAL_GROUP group)
{
    return IPH_ENG_Load_ISR(id, Idx, IPL_HAL_IME, group);
}

_INLINE ER RDE_ENG_Load_ISR(IPL_PROC_ID id, IPL_HAL_PARASET Idx, IPL_HAL_GROUP group)
{
    return IPH_ENG_Load_ISR(id, Idx, IPL_HAL_RDE, group);
}

_INLINE ER ISE_ENG_Load_ISR(IPL_PROC_ID id, IPL_HAL_PARASET Idx, IPL_HAL_GROUP group)
{
    return IPH_ENG_Load_ISR(id, Idx, IPL_HAL_ISE, group);
}

_INLINE ER DRE_ENG_Load_ISR(IPL_PROC_ID id, IPL_HAL_PARASET Idx, IPL_HAL_GROUP group)
{
    return IPH_ENG_Load_ISR(id, Idx, IPL_HAL_DRE, group);
}

_INLINE ER RHE_ENG_Load_ISR(IPL_PROC_ID id, IPL_HAL_PARASET Idx, IPL_HAL_GROUP group)
{
    return IPH_ENG_Load_ISR(id, Idx, IPL_HAL_RHE, group);
}

//}
#if 0
#endif

/**
     read IPL_Hal inner table

     @note :none

     @param[in]  id        pipeline index
     @param[in]  eng       indicated engine
     @param[in]  Idx       indicated param. set
     @param[in]  readItem  item to be read
     @param[out] Output    output inner table content,data type plz reference "IPL_HAL_READ" description
     @return Description of data returned.
         - @b E_OK:   done.
         - @b E_SYS:  operation error.
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern ER IPH_ENG_Read(IPL_PROC_ID id,IPL_HAL_ENG eng,IPL_HAL_PARASET Idx,IPL_HAL_READ readItem,void* Output);

_INLINE ER SIEx_ENG_Read(IPL_PROC_ID id, IPL_HAL_PARASET Idx, IPL_HAL_READ readItem, void* Output)
{
    return IPH_ENG_Read(id, IPL_ID_TO_SIEx(id), Idx, readItem, Output);
}

_INLINE ER IPE_ENG_Read(IPL_PROC_ID id, IPL_HAL_PARASET Idx, IPL_HAL_READ readItem, void* Output)
{
    return IPH_ENG_Read(id, IPL_HAL_IPE, Idx, readItem, Output);
}

_INLINE ER DCE_ENG_Read(IPL_PROC_ID id, IPL_HAL_PARASET Idx, IPL_HAL_READ readItem, void* Output)
{
    return IPH_ENG_Read(id, IPL_HAL_DCE, Idx, readItem, Output);
}

_INLINE ER IFE_ENG_Read(IPL_PROC_ID id, IPL_HAL_PARASET Idx, IPL_HAL_READ readItem, void* Output)
{
    return IPH_ENG_Read(id, IPL_HAL_IFE, Idx, readItem, Output);
}

_INLINE ER IFE2_ENG_Read(IPL_PROC_ID id, IPL_HAL_PARASET Idx, IPL_HAL_READ readItem, void* Output)
{
    return IPH_ENG_Read(id, IPL_HAL_IFE2, Idx, readItem, Output);
}

_INLINE ER IME_ENG_Read(IPL_PROC_ID id, IPL_HAL_PARASET Idx, IPL_HAL_READ readItem, void* Output)
{
    return IPH_ENG_Read(id, IPL_HAL_IME, Idx, readItem, Output);
}

_INLINE ER RDE_ENG_Read(IPL_PROC_ID id, IPL_HAL_PARASET Idx, IPL_HAL_READ readItem, void* Output)
{
    return IPH_ENG_Read(id, IPL_HAL_RDE, Idx, readItem, Output);
}

_INLINE ER ISE_ENG_Read(IPL_PROC_ID id, IPL_HAL_PARASET Idx, IPL_HAL_READ readItem, void* Output)
{
    return IPH_ENG_Read(id, IPL_HAL_ISE, Idx, readItem, Output);
}

_INLINE ER DRE_ENG_Read(IPL_PROC_ID id, IPL_HAL_PARASET Idx, IPL_HAL_READ readItem, void* Output)
{
    return IPH_ENG_Read(id, IPL_HAL_DRE, Idx, readItem, Output);
}

_INLINE ER RHE_ENG_Read(IPL_PROC_ID id, IPL_HAL_PARASET Idx, IPL_HAL_READ readItem, void* Output)
{
    return IPH_ENG_Read(id, IPL_HAL_RHE, Idx, readItem, Output);
}
#if 0
#endif
/**
    functional design same as IPH_ENG_Read(), but for ISR only
*/
extern ER IPH_ENG_Read_ISR(IPL_PROC_ID id,IPL_HAL_ENG eng,IPL_HAL_PARASET Idx,IPL_HAL_READ readItem,void* Output);

_INLINE ER SIEx_ENG_Read_ISR(IPL_PROC_ID id, IPL_HAL_PARASET Idx, IPL_HAL_READ readItem, void* Output)
{
    return IPH_ENG_Read_ISR(id, IPL_ID_TO_SIEx(id), Idx, readItem, Output);
}

_INLINE ER IPE_ENG_Read_ISR(IPL_PROC_ID id, IPL_HAL_PARASET Idx, IPL_HAL_READ readItem, void* Output)
{
    return IPH_ENG_Read_ISR(id, IPL_HAL_IPE, Idx, readItem, Output);
}

_INLINE ER DCE_ENG_Read_ISR(IPL_PROC_ID id, IPL_HAL_PARASET Idx, IPL_HAL_READ readItem, void* Output)
{
    return IPH_ENG_Read_ISR(id, IPL_HAL_DCE, Idx, readItem, Output);
}

_INLINE ER IFE_ENG_Read_ISR(IPL_PROC_ID id, IPL_HAL_PARASET Idx, IPL_HAL_READ readItem, void* Output)
{
    return IPH_ENG_Read_ISR(id, IPL_HAL_IFE, Idx, readItem, Output);
}

_INLINE ER IFE2_ENG_Read_ISR(IPL_PROC_ID id, IPL_HAL_PARASET Idx, IPL_HAL_READ readItem, void* Output)
{
    return IPH_ENG_Read_ISR(id, IPL_HAL_IFE2, Idx, readItem, Output);
}

_INLINE ER IME_ENG_Read_ISR(IPL_PROC_ID id, IPL_HAL_PARASET Idx, IPL_HAL_READ readItem, void* Output)
{
    return IPH_ENG_Read_ISR(id, IPL_HAL_IME, Idx, readItem, Output);
}

_INLINE ER RDE_ENG_Read_ISR(IPL_PROC_ID id, IPL_HAL_PARASET Idx, IPL_HAL_READ readItem, void* Output)
{
    return IPH_ENG_Read_ISR(id, IPL_HAL_RDE, Idx, readItem, Output);
}

_INLINE ER ISE_ENG_Read_ISR(IPL_PROC_ID id, IPL_HAL_PARASET Idx, IPL_HAL_READ readItem, void* Output)
{
    return IPH_ENG_Read_ISR(id, IPL_HAL_ISE, Idx, readItem, Output);
}

_INLINE ER DRE_ENG_Read_ISR(IPL_PROC_ID id, IPL_HAL_PARASET Idx, IPL_HAL_READ readItem, void* Output)
{
    return IPH_ENG_Read_ISR(id, IPL_HAL_DRE, Idx, readItem, Output);
}

_INLINE ER RHE_ENG_Read_ISR(IPL_PROC_ID id, IPL_HAL_PARASET Idx, IPL_HAL_READ readItem, void* Output)
{
    return IPH_ENG_Read_ISR(id, IPL_HAL_RHE, Idx, readItem, Output);
}
#if 0
#endif
/**
     read hw register

     @note none

     @param[in]  eng       indicated engine
     @param[in]  Que       which infor need to be queried
     @param[out] Rst       output register rst,data type plz reference "IPH_QUE" description
     @return Description of data returned.
         - @b E_OK:   done.
         - @b E_SYS:  operation error.
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/

extern ER IPH_ENG_ReadReg(IPL_HAL_ENG eng,IPH_QUE Que,void* Output);

_INLINE ER SIEx_ENG_ReadReg(IPL_PROC_ID id, IPH_QUE Que, void* Output)
{
    return IPH_ENG_ReadReg(IPL_ID_TO_SIEx(id), Que, Output);
}

_INLINE ER IPE_ENG_ReadReg(IPH_QUE Que, void* Output)
{
    return IPH_ENG_ReadReg(IPL_HAL_IPE, Que, Output);
}

_INLINE ER DCE_ENG_ReadReg(IPH_QUE Que, void* Output)
{
    return IPH_ENG_ReadReg(IPL_HAL_DCE, Que, Output);
}

_INLINE ER IFE_ENG_ReadReg(IPH_QUE Que, void* Output)
{
    return IPH_ENG_ReadReg(IPL_HAL_IFE, Que, Output);
}

_INLINE ER IFE2_ENG_ReadReg(IPH_QUE Que, void* Output)
{
    return IPH_ENG_ReadReg(IPL_HAL_IFE2, Que, Output);
}

_INLINE ER IME_ENG_ReadReg(IPH_QUE Que, void* Output)
{
    return IPH_ENG_ReadReg(IPL_HAL_IME, Que, Output);
}

_INLINE ER RDE_ENG_ReadReg(IPH_QUE Que, void* Output)
{
    return IPH_ENG_ReadReg(IPL_HAL_RDE, Que, Output);
}

_INLINE ER ISE_ENG_ReadReg(IPH_QUE Que, void* Output)
{
    return IPH_ENG_ReadReg(IPL_HAL_ISE, Que, Output);
}

_INLINE ER DRE_ENG_ReadReg(IPH_QUE Que, void* Output)
{
    return IPH_ENG_ReadReg(IPL_HAL_DRE, Que, Output);
}

_INLINE ER RHE_ENG_ReadReg(IPH_QUE Que, void* Output)
{
    return IPH_ENG_ReadReg(IPL_HAL_RHE, Que, Output);
}

#if 0
#endif
/**
    functional design same as IPH_ENG_ReadReg(), but for ISR only
*/
extern ER IPH_ENG_ReadReg_ISR(IPL_HAL_ENG eng,IPH_QUE Que,void* Output);

_INLINE ER SIEx_ENG_ReadReg_ISR(IPL_PROC_ID id, IPH_QUE Que, void* Output)
{
    return IPH_ENG_ReadReg_ISR(IPL_ID_TO_SIEx(id), Que, Output);
}

_INLINE ER IPE_ENG_ReadReg_ISR(IPH_QUE Que, void* Output)
{
    return IPH_ENG_ReadReg_ISR(IPL_HAL_IPE, Que, Output);
}

_INLINE ER DCE_ENG_ReadReg_ISR(IPH_QUE Que, void* Output)
{
    return IPH_ENG_ReadReg_ISR(IPL_HAL_DCE, Que, Output);
}

_INLINE ER IFE_ENG_ReadReg_ISR(IPH_QUE Que, void* Output)
{
    return IPH_ENG_ReadReg_ISR(IPL_HAL_IFE, Que, Output);
}

_INLINE ER IFE2_ENG_ReadReg_ISR(IPH_QUE Que, void* Output)
{
    return IPH_ENG_ReadReg_ISR(IPL_HAL_IFE2, Que, Output);
}

_INLINE ER IME_ENG_ReadReg_ISR(IPH_QUE Que, void* Output)
{
    return IPH_ENG_ReadReg_ISR(IPL_HAL_IME, Que, Output);
}

_INLINE ER RDE_ENG_ReadReg_ISR(IPH_QUE Que, void* Output)
{
    return IPH_ENG_ReadReg_ISR(IPL_HAL_RDE, Que, Output);
}

_INLINE ER ISE_ENG_ReadReg_ISR(IPH_QUE Que, void* Output)
{
    return IPH_ENG_ReadReg_ISR(IPL_HAL_ISE, Que, Output);
}

_INLINE ER DRE_ENG_ReadReg_ISR(IPH_QUE Que, void* Output)
{
    return IPH_ENG_ReadReg_ISR(IPL_HAL_DRE, Que, Output);
}

_INLINE ER RHE_ENG_ReadReg_ISR(IPH_QUE Que, void* Output)
{
    return IPH_ENG_ReadReg_ISR(IPL_HAL_RHE, Que, Output);
}
#if 0
#endif
extern ER IPH_ENG_Dump(IPL_PROC_ID id,IPL_HAL_PARASET Idx,IPL_HAL_ENG eng,BOOL All);
//@}
#endif
