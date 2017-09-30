#ifndef _IPL_CALCTRL_H_
#define _IPL_CALCTRL_H_
#include "Type.h"
#include "ErrorNo.h"
#include "IPL_Ctrl_Par.h"
#include "IPL_Calibration_3A.h"
#include "IPL_Calibration_ALG.h"
#include "IPL_Calibration_BUF.h"
#include "IPL_AlgInfor.h"

/**
     configuration for IPL calibration mode

     this structure only refer to IPL calibration mode!
*/
typedef struct _IPLC_INFO
{
    IPLC_BASEINFO   *pBaseInfo;     ///< -
    IPLC_EXT        *pExtInfo;      ///< -
    fpEngSubFunc    pAdaptSetting;   ///<
    SIE_Dma_Out     CA_PPB;        ///< -
    IPL_PROC_ID     Id;
    SIE_UPDATE      *psieUpdate;    ///< cap only
    SIE             *psie;          ///< cap only
}IPLC_INFO,*pIPLC_INFO;

/**
    calibration ctrl
*/
extern ER IPC_RUN(IPL_PROC_ID Id,pIPLC_INFO pInfo,UINT32 stage,fp_ImgRdy fpRdyImg);
extern ER IPC_PAUSE(IPL_PROC_ID Id,UINT32 stage);

#endif
