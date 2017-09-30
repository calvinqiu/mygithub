/**
    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.

    @file       PrjCfg.h
    @ingroup

    @brief

    @note       Nothing.
*/

#ifndef _PRJCFG_H_
#define _PRJCFG_H_

#include "Type.h"
#include "Debug.h"


#define DEBUG_LEVEL_NONE                    0   ///< there is no debug message will show beside using debug_msg
#define DEBUG_LEVEL_ERROR                   1   ///< only debug_err() message will show
#define DEBUG_LEVEL_WARNING                 2   ///< only debug_wrn() and debug_err() message will show
#define DEBUG_LEVEL_INDICATION              3   ///< debug_ind(), debug_wrn() and debug_err() message will show

#define DEBUG_LEVEL                         DEBUG_LEVEL_ERROR   ///< debug level there are DEBUG_LEVEL_NONE/DEBUG_LEVEL_ERROR/DEBUG_LEVEL_WARNING/DEBUG_LEVEL_INDICATION can be select

#if (DEBUG_LEVEL >= DEBUG_LEVEL_ERROR)
#define debug_err(msg)                      do{debug_msg("ERR: ");debug_msg msg ;}while(0)
#else
#define debug_err(msg)
#endif

#if (DEBUG_LEVEL >= DEBUG_LEVEL_WARNING)
#define debug_wrn(msg)                      do{debug_msg("WRN: ");debug_msg msg ;}while(0)
#else
#define debug_wrn(msg)
#endif

#if (DEBUG_LEVEL >= DEBUG_LEVEL_INDICATION)
#define debug_ind(msg)                      do{debug_msg("IND: ");debug_msg msg ;}while(0)
#else
#define debug_ind(msg)
#endif
//#NT#2016/03/03#Ben Wang -begin
//#NT#Add debug message for script test.
extern BOOL g_bDbgTest;
#define DBG_TEST(fmtstr, args...) do{ if(g_bDbgTest) debug_msg("DBGT: " fmtstr, ##args);  }while(0)
//#NT#2016/03/03#Ben Wang -end

#include "PrjCfg_Comm_Head.h"

#if defined(YQCONFIG_USE_FIX_PROJECT_SUPPORT)
#include "PrjCfg_ALL.h"
#else

#if (_FPGA_EMULATION_ == ENABLE)
#include "PrjCfg_FPGA.h"
#elif (_MODEL_DSC_ == _MODEL_EVB_ \
    || _MODEL_DSC_ == _MODEL_DEMO1_)
#include "PrjCfg_Demo.h"
#elif (_MODEL_DSC_ == _MODEL_IPCAM1_EVB_ \
    || _MODEL_DSC_ == _MODEL_IPCAM2_EVB_ \
    || _MODEL_DSC_ == _MODEL_IPCAM1_ \
    || _MODEL_DSC_ == _MODEL_IPCAM2_ \
    || _MODEL_DSC_ == _MODEL_IPCAM3_ \
    || _MODEL_DSC_ == _MODEL_PMX_IPCAM_EVB_)
#include "PrjCfg_IPCam.h"
#elif (_MODEL_DSC_ == _MODEL_IPCAM4_TL_DEWARP_)
#include "PrjCfg_IPCAM4_TL_DEWARP.h"
#elif (_MODEL_DSC_ == _MODEL_YSX_IPCAM1_)
#include "PrjCfg_YSX_IPCAM1.h"
#elif (_MODEL_DSC_ == _MODEL_PPG360_)
#include "PrjCfg_PPG360.h"
#elif (_MODEL_DSC_ == _MODEL_WiFiSDV_)
#include "PrjCfg_WiFiSDV.h"
#elif (_MODEL_DSC_ == _MODEL_CUBIC_DRONE_)
#include "PrjCfg_CUBIC_DRONE.h"
#elif (_MODEL_DSC_ == _MODEL_AIRCAM_)
#include "PrjCfg_AIRCAM.h"
#elif (_MODEL_DSC_ == _MODEL_FIREFLY_)
#include "PrjCfg_FIREFLY.h"
#elif (_MODEL_DSC_ == _MODEL_FIREFLY_665_)
#include "PrjCfg_FIREFLY_665.h"
#elif (_MODEL_DSC_ == _MODEL_FR300_)
#include "PrjCfg_FR300.h"
#elif (_MODEL_DSC_ == _MODEL_CarDV_)
#include "PrjCfg_CarDV.h"
#elif (_MODEL_DSC_ == _MODEL_SP2_)
#include "PrjCfg_SP2.h"
#elif (_MODEL_DSC_ == _MODEL_SP10_)
#include "PrjCfg_SP10.h"
#elif (_MODEL_DSC_ == _MODEL_CarDV_DUAL_TVP5150_)
#include "PrjCfg_CarDV_DUAL_TVP5150.h"
#elif (_MODEL_DSC_ == _MODEL_DUAL_VX1_)
#include "PrjCfg_DUAL_VX1.h"
#elif (_MODEL_DSC_ == _MODEL_Hummingbird_)
#include "PrjCfg_Hummingbird.h"
#elif (_MODEL_DSC_ == _MODEL_C10_)
#include "PrjCfg_C10.h"
#elif (_MODEL_DSC_ == _MODEL_AO_EVB_)
#include "PrjCfg_AO_EVB.h"
#elif (_MODEL_DSC_ == _MODEL_PW663_)
#include "PrjCfg_PW663.h"
#elif (_MODEL_DSC_ == _MODEL_C86B_)
#include "PrjCfg_C86B.h"
#elif (_MODEL_DSC_ == _MODEL_PWA86_)
#include "PrjCfg_PWA86.h"
#elif (_MODEL_DSC_ == _MODEL_PWA6_)
#include "PrjCfg_PWA6.h"
#elif (_MODEL_DSC_ == _MODEL_PWA6_668_)
#include "PrjCfg_PWA6_668.h"
#elif (_MODEL_DSC_ == _MODEL_X2S_)
#include "PrjCfg_X2S.h"
#elif (_MODEL_DSC_ == _MODEL_MIO_C032_)
#include "PrjCfg_MIO_C032.h"
#elif (_MODEL_DSC_ == _MODEL_GS730P_)
#include "PrjCfg_GS730P.h"
#elif (_MODEL_DSC_ == _MODEL_KSKP_)
#include "PrjCfg_KSKP.h"
#elif (_MODEL_DSC_ == _MODEL_TWIN_360_)
#include "PrjCfg_TWIN_360.h"
#elif (_MODEL_DSC_ == _MODEL_XD01_)
#include "PrjCfg_XD01.h"
#elif (_MODEL_DSC_ == _MODEL_TVP5150_)
#include "PrjCfg_TVP5150.h"
#elif (_MODEL_DSC_ == _MODEL_PTL_668_)
#include "PrjCfg_PTL_668.h"
#elif (_MODEL_DSC_ == _MODEL_FHD6120_)
#include "PrjCfg_FHD6120.h"
#elif (_MODEL_DSC_ == _MODEL_PWU01_)
#include "PrjCfg_PWU01.h"
#elif (_MODEL_DSC_ == _MODEL_D6_)
#include "PrjCfg_D6.h"
#elif (_MODEL_DSC_ == _MODEL_CAF050_)
#include "PrjCfg_CAF050.h"
#elif (_MODEL_DSC_ == _MODEL_DEWARP_)
#include "PrjCfg_DEWARP.h"
#elif (_MODEL_DSC_ == _MODEL_TL_IPC320_ \
    || _MODEL_DSC_ == _MODEL_TL_IPC330_ \
    || _MODEL_DSC_ == _MODEL_TL_IPC340_ \
    || _MODEL_DSC_ == _MODEL_TL_IPC350_ \
    || _MODEL_DSC_ == _MODEL_TL_IPC321_)
#include "PrjCfg_TL_IPC320.h"
#elif (_MODEL_DSC_ == _MODEL_SUBEI_TFH9_)
#include "PrjCfg_SUBEI_TFH9.h"
#elif (_MODEL_DSC_ == _MODEL_C10_LITE_)
#include "PrjCfg_C10_Lite.h"
#elif (_MODEL_DSC_ == _MODEL_HY_SMIRROR_)
#include "PrjCfg_HY_SMIRROR.h"
#elif (_MODEL_DSC_ == _MODEL_TL_CD410_)
#include "PrjCfg_TL_CD410.h"
#elif (_MODEL_DSC_ == _MODEL_TL_CD215_)
#include "PrjCfg_TL_CD215.h"
#elif (_MODEL_DSC_ == _MODEL_CarDV_DUAL_AHD_)
#include "PrjCfg_CarDV_DUAL_AHD.h"
#elif (_MODEL_DSC_ == _MODEL_TCT_IPCAM_EVB_ || _MODEL_DSC_ == _MODEL_TCT_IPCAM_TL_ || _MODEL_DSC_ == _MODEL_TCT_IPCAM1_)
#include "PrjCfg_TCT_EVB.h"
#elif (_MODEL_DSC_ == _MODEL_TCT_IPCAM_DUAL_EVB_)
#include "PrjCfg_TCT_DUAL_EVB.h"
#elif (_MODEL_DSC_ == _MODEL_C13_)
#include "PrjCfg_C13.h"
#elif (_MODEL_DSC_ == _MODEL_SWAN_EVB_)
#include "PrjCfg_SWAN.h"
#elif (_MODEL_DSC_ == _MODEL_MIO_IMX326_)
#include "PrjCfg_MIO_IMX326.h"
#elif (_MODEL_DSC_ == _MODEL_MIO_MT39_)
#include "PrjCfg_MIO_MT39.h"
#elif (_MODEL_DSC_ == _MODEL_ADR820D_)
#include "PrjCfg_ADR820.h"
#elif (_MODEL_DSC_ == _MODEL_K40_)
#include "PrjCfg_K40.h"
#elif (_MODEL_DSC_ == _MODEL_U15_)
#include "PrjCfg_U15.h"
#else //Unknown MODEL
#warning Unknown MODEL?
#warning Please assign your "MODEL" in include "ModelConfig.txt"
#error (see above)
#endif

#endif	//YQCONFIG_USE_FIX_PROJECT_SUPPORT

#include "PrjCfg_Comm_Tail.h"

#endif //_PRJCFG_H_
