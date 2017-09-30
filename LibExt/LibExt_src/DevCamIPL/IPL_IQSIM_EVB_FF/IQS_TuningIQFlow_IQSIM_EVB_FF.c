#ifndef _IQS_TUNINGIQFLOW_IQSIM_EVB_FF_C_
#define _IQS_TUNINGIQFLOW_IQSIM_EVB_FF_C_

#include "IPL_IQSIM_EVB_FF_Int.h"

///////////////////
// IPCAM set IQ info
///////////////////
void IQS_SetIQInfo(UINT32 Id, IQS_EXT_SET_IQ_INFO IQSExtSetIQInfo)
{

}

void IQS_GetIQInfo(UINT32 Id, IQS_EXT_SET_IQ_INFO* IQSExtSetIQInfo)
{

}

void IQUC_GetTuningRatio(UINT32 Id, UINT32* IQUC_TuningIQRatio, UINT32* IQUC_TuningCACValue)
{

}

void IQUC_SetTuningRatio(UINT32 Id, UINT32* IQUC_TuningIQRatio, UINT32* IQUC_TuningCACValue)
{

}

void IQS_CalRatio(UINT32 SenId_Value, UINT32 IQIdxL, UINT32 IQIdxH)
{

}

void IQS_CalValue(UINT32 SenId_Value, UINT32 IQIdxL, UINT32 IQIdxH)
{

}

///////////////////
// IQ param interpolation for ISO
///////////////////
void IQS_Intpl_ISO(UINT32 Id, IQS_FLOW_MSG MsgID)
{

}

const char* ADJIQ_ITEM_Strings[] =
{
    "ADJIQ_OB",
    "ADJIQ_OUTL_BRI",
    "ADJIQ_OUTL_DARK",
    "ADJIQ_NR",
    "ADJIQ_NR_FP",
    "ADJIQ_NR_SPATIAL",
    "ADJIQ_GBAL",
    "ADJIQ_EDGE_WHITE",
    "ADJIQ_EDGE_BLACK",
    "ADJIQ_EDGE_THICK",
    "ADJIQ_EDGE_START",
    "ADJIQ_EDGE_STEP",
    "ADJIQ_SAT",
    "ADJIQ_CONT",
    "ADJIQ_LCA"
};

extern void IQUCLOG_OPEN(char *pFilename);
extern void IQUCLOG(char *format, ...);
extern void IQUCLOG_CLOSE(void);

UINT32 IQUC_DumpRatioTable(UINT32 Id)
{
    return E_SYS;
}
#endif
