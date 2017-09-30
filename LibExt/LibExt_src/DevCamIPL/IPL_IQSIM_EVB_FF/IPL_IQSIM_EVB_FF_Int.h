#ifndef _IPL_IQSIM_INT_H_
#define _IPL_IQSIM_INT_H_

#include "IQSim.h"
#include "HwMem.h"

//other
#include "Type.h"
#include "DxSensor.h"
#include "ae_api.h"
#include "ae_task.h"
#include "awb_api.h"
#include "awb_task.h"
#include "af_api.h"
#include "af_task.h"
#include "IQS_Task.h"
#include "IPL_Cmd.h"
#include "IPL_Utility.h"
#include "Flashlight_api.h"
#include "dis_alg.h"
#include "awb_alg.h"
#include "ae_alg.h"
#include "SHDRCtrl.h"
#include "IQUC_Ctrl.h"
#include "adas_lib.h"
#include "rsc_common.h"
#include "Stitch_lib.h"

//common
#include "IPL_Hal_Op_Int.h"
#include "IPL_Ctrl_Int.h"
#include "IPL_Ctrl_flow_Int.h"
#include "IPL_Buffer_Int.h"
#include "IPL_CBMsg_Int.h"
#include "IPL_Display.h"
#include "IPL_dzoom_Int.h"
#include "IPL_CtrlInfor_Int.h"
#include "IPL_AlgInfor_Int.h"
#include "IPL_Task_int.h"
#include "IPL_Cmd_int.h"
#include "IPL_Debug_Int.h"
#include "IQS_SettingFlow.h"
#include "IQS_SettingFlow_Id_Int.h"
#include "IQS_Utility.h"
#include "IQS_Ctrl.h"
#include "IPL_HeaderInfor_Raw.h"
#include "IPL_ICFFunc_Int.h"
#include "SceneRenderLib.h"
#include "iqs_api.h"

//sample
#include "IPL_Cmd_IQSIM_EVB_FF_Int.h"
#include "IPL_Mode_IQSIM_EVB_FF_Int.h"
#include "IPL_Buf_IQSIM_EVB_FF_Int.h"
#include "IPL_Ctrl_IQSIM_EVB_FF_Int.h"
#include "IPL_CtrlNormal_IQSIM_EVB_FF_Int.h"
#include "IPL_CtrlFW_IQSIM_EVB_FF_Int.h"
#include "IPL_CtrlDeWarp_IQSIM_EVB_FF_Int.h"
#include "IPL_dzoomTab_IQSIM_EVB_FF_Int.h"
#include "IPL_FlowCB_IQSIM_EVB_FF_Int.h"
#include "IPL_Isr_IQSIM_EVB_FF_Int.h"
#include "IQS_SettingFlow_IQSIM_EVB_FF_Int.h"
#include "IQS_SettingFunc_IQSIM_EVB_FF_Int.h"
#include "IQS_SettingTable_IQSIM_EVB_FF_Int.h"
#include "IQS_ImageEffectTable_IQSIM_EVB_FF_Int.h"
#include "IPL_ExtFunc_IQSIM_EVB_FF_Int.h"
#include "IQS_TuningIQFlow_IQSIM_EVB_FF_int.h"
#include "IQS_TuningIQTable_IQSIM_EVB_FF_Int.h"

#define RAW_ENC_BUF_RATIO 100    //%, minimum value: 66

#define IQS_DUMP_ENGREG 1
#define IPL_TIMEOUT_MS  500  //ms

//calibration
#define CAL_ECS DISABLE
#define CAL_DP  DISABLE

#endif //_IPL_SAMPLE_INT_H_
