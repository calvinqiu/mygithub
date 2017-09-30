#ifndef IPL_EXT_FUNC_H_
#define IPL_EXT_FUNC_H_
/**
    IPL_ExtFuncSample_Int.h


    @file       IPL_ExtFuncSample_Int.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

extern void ExtFunc_Init(UINT32 Id, ENG_SUBFUNC *Info,IPL_HAL_GROUP* group);
extern void ExtFunc_Process(UINT32 Id);
SR_WDR_PARAM ExtFunc_GetWDRResult(UINT32 Id);
extern void ExtFunc_SetWDRContrastLevel(UINT32 Id, UINT32 ContrastLevel);
extern void ExtFunc_SetWDRLumaLevel(UINT32 Id, UINT32 Level);
#endif //IPL_EXT_FUNC_MN34110_H_
