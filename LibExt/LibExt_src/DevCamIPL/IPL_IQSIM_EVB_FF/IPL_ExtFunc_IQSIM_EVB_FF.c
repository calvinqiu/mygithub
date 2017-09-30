/**
    ISR sample.

    @file       IPL_ExtFuncSample.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "IPL_IQSIM_EVB_FF_Int.h"

void ExtFunc_Init(UINT32 Id, ENG_SUBFUNC *Info, IPL_HAL_GROUP* group)
{

}

void ExtFunc_Process(UINT32 Id)
{

}

SR_WDR_PARAM ExtFunc_GetWDRResult(UINT32 Id)
{
    SR_WDR_PARAM WdrParams = {0};
    return WdrParams;
}

void ExtFunc_SetWDRLumaLevel(UINT32 Id, UINT32 Level)
{

}

void ExtFunc_SetWDRContrastLevel(UINT32 Id, UINT32 ContrastLevel)
{
    SR_WDR_CHG_PARAMS ChgParams = {0};

    ChgParams.uiChgSel = SR_WDR_CHG_CONTRAST_LEVEL;
    ChgParams.uiContrastLevel = ContrastLevel;
    SceneRender_WDR_ChgParams(Id, &ChgParams);
}