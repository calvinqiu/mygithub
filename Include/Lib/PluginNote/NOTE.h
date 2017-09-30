/**
    NOTE.

    The common function for NOTE control.

    @file       NOTEe.h
    @ingroup    Lib\PluginNote
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2017.  All rights reserved.
*/
#ifndef _NOTE_H_
#define _NOTE_H_
#ifdef WIN32
#define ENUM_DUMMY4WORD(a)
#endif

#include "ae_task.h"
#include "ae_alg.h"

#include "awb_task.h"
#include "awb_alg.h"

extern void Note_Ae_Init(UINT32 Id, void *AE_Info_Ptr, void *AE_ParamInfo_Ptr, void *AE_SetParamInfo_Ptr, UINT32 *AE_Table_Ptr);
extern void Note_Ae_Process(UINT32 Id);
extern void AE_SetPLINEInfo(UINT32 Id, AE_EXT_SET_INFO SetInfo);
extern void AE_GetPrvAEArg(UINT32 Id, UINT32 *ISO, UINT32 *ExpT, UINT32 *Iris);

extern void NOTE_AWB_Init(UINT32 Id, AWB_PARAM_Pr AWB_Table_Ptr);
extern void Note_AWB_Process(UINT32 Id);

extern UINT32 IQS_GetInvGammaValue(UINT32 Id, UINT32 Data10Bit);
extern void Note_iqstInit(UINT32 id);
#endif //_AWB_ALG_H_
