#ifndef IPL_EXT_FUNC_OV4689_IPCAM_H_
#define IPL_EXT_FUNC_OV4689_IPCAM_H_
/**
    IPL_ExtFuncSample_Int.h


    @file       IPL_ExtFuncSample_Int.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

//#NT#2016/03/23#Jarkko Chang -begin
//#NT# add support different wdr trigger interval
#define WDR_TAB_END 0xffffffff
#define WDR_PROC_CNT         2

//#NT#2016/05/19#Jarkko Chang -begin
//#NT# add support wdr2.5
/**
    wdr Version
*/
typedef enum _WDR_VERSION
{
    WDR_VERSION_2_0,    ///< WDR Version 2.0
    WDR_VERSION_2_5,    ///< WDR Version 2.5
    ENUM_DUMMY4WORD(WDR_VERSION)
} WDR_VERSION;
//#NT#2016/05/19#Jarkko Chang -end

//#NT#2017/03/13#Wendy Liao -begin
//#NT# add WDR auto mode
/**
    wdr auto reference
*/
typedef enum _WDR_AUTO_MODE
{
    WDR_AUTO_EXP,
    WDR_AUTO_LV,
    ENUM_DUMMY4WORD(WDR_AUTO_MODE)
} WDR_AUTO_MODE;
//#NT#2017/03/13#Wendy Liao -end

typedef struct WDR_INTERVAL_PARAM
{
     UINT32 VideoFps;
     UINT32 TrigIntervel;
} WDR_INTERVAL_PARAM;

UINT32 ExtFunc_getTrigInterval(UINT32 Id, UINT32 fps);
//#NT#2016/03/23#Jarkko Chang -end

//#NT#2016/04/08#Jarkko Chang -begin
//#NT# add for wdr library need video fps information
UINT32 ExtFunc_getVideoFps(UINT32 Id, UINT32 fps);
//#NT#2016/04/08#Jarkko Chang -end

extern void ExtFunc_Init(UINT32 Id, ENG_SUBFUNC *Info,IPL_HAL_GROUP* group);
extern void ExtFunc_Process(UINT32 Id);
SR_WDR_PARAM ExtFunc_GetWDRResult(UINT32 Id);
extern void ExtFunc_SetWDRContrastLevel(UINT32 Id, UINT32 ContrastLevel);
extern void ExtFunc_SetWDRLumaLevel(UINT32 Id, UINT32 Level);
//#NT#2016/12/01#Jarkko Chang -begin
//#NT# update wdr/shdr re init issue
extern void ExtFunc_Reset(UINT32 Id);
//#NT#2016/12/01#Jarkko Chang -end
#endif //IPL_EXT_FUNC_AR0238CS_H_
