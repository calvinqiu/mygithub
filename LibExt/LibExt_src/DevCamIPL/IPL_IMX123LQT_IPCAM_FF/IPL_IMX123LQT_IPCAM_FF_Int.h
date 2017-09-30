#ifndef _IPL_IMX123LQT_IPCAM_FF_INT_H_
#define _IPL_IMX123LQT_IPCAM_FF_INT_H_

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
#include "IPL_Cmd_IMX123LQT_IPCAM_FF_Int.h"
#include "IPL_Mode_IMX123LQT_IPCAM_FF_Int.h"
#include "IPL_Buf_IMX123LQT_IPCAM_FF_Int.h"
#include "IPL_Ctrl_IMX123LQT_IPCAM_FF_Int.h"
#include "IPL_CtrlNormal_IMX123LQT_IPCAM_FF_Int.h"
#include "IPL_CtrlFW_IMX123LQT_IPCAM_FF_Int.h"
#include "IPL_CtrlDeWarp_IMX123LQT_IPCAM_FF_Int.h"
#include "IPL_dzoomTab_IMX123LQT_IPCAM_FF_Int.h"
#include "IPL_FlowCB_IMX123LQT_IPCAM_FF_Int.h"
#include "IPL_Isr_IMX123LQT_IPCAM_FF_Int.h"
#include "IQS_SettingFlow_IMX123LQT_IPCAM_FF_Int.h"
#include "IQS_SettingFunc_IMX123LQT_IPCAM_FF_Int.h"
#include "IQS_SettingTable_IMX123LQT_IPCAM_FF_Int.h"
#include "IQS_ImageEffectTable_IMX123LQT_IPCAM_FF_Int.h"
#include "IPL_ExtFunc_IMX123LQT_IPCAM_FF_Int.h"
#include "IQS_TuningIQFlow_IMX123LQT_IPCAM_FF_int.h"
#include "IQS_TuningIQTable_IMX123LQT_IPCAM_FF_Int.h"

//#NT#2017/02/22#Jarkko Chang -begin
//#NT#increase 2 stripe buffer ratio to 80%
#define RAW_ENC_BUF_RATIO_2STRIPE 	80    //%, 2 stripe ratio: 80
//#NT#2017/02/22#Jarkko Chang -end
#define RAW_ENC_BUF_RATIO 70    //%, minimum value: 66
#define IQS_DUMP_ENGREG   0
#define IQUC_ENABLE       0
#define IPL_TIMEOUT_MS    500  //ms

#define IQS_RATIOTABLE_ENABLE 0

//#NT#2016/09/13#Silvia Wu -begin
//#NT# add support lens center calibration
//calibration
#define CAL_ECS DISABLE
#define CAL_DP  DISABLE
#define CAL_LENSCEN  DISABLE
//#NT#2016/09/13#Silvia Wu -end

//#NT#2016/12/22#Wendy Liao -begin
//#NT#Support IPcam function
extern BOOL IQS_IPCAM_ENABLE;
//#NT#2016/12/22#Wendy Liao -end

#endif //_IPL_SAMPLE_INT_H_
