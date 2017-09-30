#include "SysKer.h"
#include "MsdcNvtApi.h"
#include "MsdcNvtCallback.h"
#include "MsdcNvtCb_IQSim.h"
#include <string.h>
#include "IPL_Cmd.h"
#include "IPL_AlgInfor.h"
#include "SysCfg.h"
#include "FileSysTsk.h"
#include "ImgCaptureAPI.h"
#include "ImgCapInfor.h"
#include "Utility.h"
#include "DCF.h"
#include "IQSim.h"
#include "IQS_Ctrl.h"
#include "IQUC_Ctrl.h"
#include "ae_task.h"
#include "awb_task.h"
#include "UIAppMovie.h"
#include "ae_api.h"
#include "awb_api.h"
#include "ae_alg.h"
#include "awb_alg.h"
#include "IQS_SettingFlow.h"
#include "DxSensor.h"
#include "IPL_Ctrl.h"

//#include "Dep_Exif.h"
//#include "Dep_Fs.h"
//#include "Dep_DispSrv.h"
//#include "IPL_Display.h"

#define THIS_DBGLVL         2 //0=OFF, 1=ERROR, 2=TRACE
#define __MODULE__          MsdcIQSim
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"




#define IQCMD_SET                   0x9a00ff00
#define IQCMD_SCENELOCK             0x9a00ff01
#define IQCMD_SCENEUNLOCK           0x9a00ff02
#define IQCMD_INITPARAM             0x9a00ff03
#define IQCMD_DUMPIQSETTING         0x9a00ff04
#define IQCMD_DUMPIQSETTING_SIQT    0x9a00ff05

#define IQCMD_SAVEPRVRAW_START 0x9a00f002
#define IQCMD_SAVEPRVRAW_END   0x9a00f003
#define IQCMD_PRVRAWINFO_GET   0x9a00f004
#define IQCMD_RUNPARAM         0x9a00f005
#define IQCMD_READFROMMEMORY   0x9a00f006
#define IQCMD_READFROMSTORAGE  0x9a00f006
#define IQCMD_WRITETOSTORAGE   0x9a00f007
#define IQCMD_GETMCTFOUT       0x9a00f008
#define IQCMD_GETIMEINFO       0x9a00f009

typedef struct
{
    UINT32 id;
    UINT32 enable;
    UINT32 type;
    UINT32 width;
    UINT32 height;
    UINT32 offset;
} IQUC_IME_INFO;

#define AECMD_ENABLE           0x9a00f101
//#define AECMD_SET              0x9a00f102
//#define AECMD_GET              0x9a00f103
//#define AECMD_LUMINANCE        0x9a00f104

#define AWBCMD_ENABLE          0x9a00f201
//#define AWBCMD_SET             0x9a00f202
//#define AWBCMD_GET             0x9a00f203

extern UINT32 uiISOGain[AE_ID_MAX_NUM], uiExpoTime[AE_ID_MAX_NUM], uiIris[AE_ID_MAX_NUM];
extern UINT32 uiRGain[AWB_ID_MAX_NUM], uiGGain[AWB_ID_MAX_NUM], uiBGain[AWB_ID_MAX_NUM];

IQUC_CTRL_ITEM IQItem;
void IQS_GetIQCtrl(void);
void IQS_SetIQCtrl(void);
#define IQSIM_MAX_BIN_SIZE 0x100000
#define IQSIM_MAX_CH0_SIZE 0x1800000
#define IQSIM_MAX_CH2_SIZE 0x1800000


void IQSim_GetPRIFmt(void);
void IQSim_GetBinAddr(void);
void IQSim_GetRawCh0Addr(void);
void IQSim_GetRawCh2Addr(void);
void IQSim_GetJpgAddr(void);
void IQSim_GetJpgOk(void);
void IQSim_GetJpgSize(void);
void IQSim_SetConvertInit(void);
void IQSim_SetConvertProc(void);
void IQSim_SetConvertEnd(void);
void IQSim_GetQVAddr(void);
void IQSim_GetQVSize(void);
void IQSim_GetQVFmt(void);
void IQSim_GetPRIAddr(void);
void IQSim_GetPRISize(void);
void IQSAE_GetExpectLum(void);
void IQSAE_GetHistogramadj(void);
void IQSAE_GetOverExp(void);
void IQSAE_GetBoundary(void);
void IQSAE_GetConverge(void);
void IQSAE_GetManual(void);
void IQSAE_GetStatus(void);
void IQSAE_SetExpectLum(void);
void IQSAE_SetHistogramadj(void);
void IQSAE_SetOverExp(void);
void IQSAE_SetBoundary(void);
void IQSAE_SetConverge(void);
void IQSAE_SetManual(void);
void IQSAWB_GetCTTable(void);
void IQSAWB_GetWTable(void);
void IQSAWB_GetManual(void);
void IQSAWB_GetStatus(void);
void IQSAWB_GetCA(void);
void IQSAWB_GetFlag(void);
void IQSAWB_SetCTTable(void);
void IQSAWB_SetWTable(void);
void IQSAWB_SetManual(void);
void IQSISP_SetECS(void);
void IQSISP_SetOUTLIER(void);
void IQSISP_SetRANGE(void);
void IQSISP_SetOB(void);
void IQSISP_SetGBAL(void);
void IQSISP_SetLDC(void);
void IQSISP_SetEDGE(void);
void IQSISP_SetCCTRL(void);
void IQSISP_SetCOLORNF(void);
void IQSISP_SetTNR(void);
void IQSISP_SetGAMMA(void);
void IQSISP_SetCC(void);
void IQSISP_SetMemMap(void);
void IQSISP_SetSensorId(void);
void IQSISP_SetPathId(void);
void IQSISP_SetSIEPause(void);
void IQSISP_GetECS(void);
void IQSISP_GetOUTLIER(void);
void IQSISP_GetRANGE(void);
void IQSISP_GetOB(void);
void IQSISP_GetGBAL(void);
void IQSISP_GetLDC(void);
void IQSISP_GetEDGE(void);
void IQSISP_GetCCTRL(void);
void IQSISP_GetCOLORNF(void);
void IQSISP_GetTNR(void);
void IQSISP_GetGAMMA(void);
void IQSISP_GetCC(void);
void IQSISP_GetImeInfo(void);
void IQSISP_GetImeRdyInfo(void);
void IQSISP_GetVer(void);
void IQSISP_GetRawInfo(void);
void IQSISP_GetAWDR(void);
void IQSISP_GetTNRCFG(void);
void IQSISP_SetAWDR(void);
void IQSISP_SetTNRCFG(void);

void SetTestAppInfo(void);

//Step 2: Create your function mapping table for 'Get' Command
static void (*MSDCIQSimGet[])(void)=
{
        IQSim_GetBinAddr,     // 0 - get input bin address
        IQSim_GetRawCh0Addr,  // 1 - get input raw address
        IQSim_GetRawCh2Addr,  // 2 - get input raw address
        IQSim_GetJpgAddr,     // 3 - get output JPG address
        IQSim_GetJpgOk,       // 4 - get output JPG status
        IQSim_GetJpgSize,     // 5 - get output JPG size
        IQSim_GetQVAddr,      // 6 - get output Quickview address
        IQSim_GetQVSize,      // 7 - get output Quickview size
        IQSim_GetQVFmt,       // 8 - get output Quickview size
        IQSim_GetPRIAddr,     // 9 - get output Primary Image address
        IQSim_GetPRISize,     // 10- get output Primary Image size
        IQSim_GetPRIFmt,      // 11- get output Primary Image size
        IQS_GetIQCtrl,      // 12
        IQSAE_GetExpectLum, // 13
        IQSAE_GetHistogramadj,
        IQSAE_GetOverExp,
        IQSAE_GetBoundary,
        IQSAE_GetConverge,
        IQSAE_GetManual,
        IQSAE_GetStatus,
        IQSAWB_GetCTTable,  // 20
        IQSAWB_GetWTable,
        IQSAWB_GetManual,
        IQSAWB_GetStatus,
        IQSISP_GetECS,      // 24
        IQSISP_GetOUTLIER,
        IQSISP_GetRANGE,
        IQSISP_GetOB,
        IQSISP_GetGBAL,
        IQSISP_GetLDC,
        IQSISP_GetEDGE,
        IQSISP_GetCCTRL,
        IQSISP_GetCOLORNF,
        IQSISP_GetTNR,
        IQSISP_GetGAMMA,
        IQSISP_GetCC,
		IQSISP_GetImeInfo, // 36
		IQSISP_GetImeRdyInfo,
		IQSISP_GetVer,
		IQSAWB_GetCA,		//39
		IQSAWB_GetFlag,		//40
		IQSISP_GetRawInfo,
		IQSISP_GetAWDR,
		IQSISP_GetTNRCFG,
};

//Step 3: Create your function mapping table for 'Set' Command
static void (*MSDCIQSimSet[])(void)=
{
        IQSim_SetConvertInit,
        IQSim_SetConvertProc,
        IQSim_SetConvertEnd,
        IQS_SetIQCtrl,
        IQSAE_SetExpectLum,     //4
        IQSAE_SetHistogramadj,
        IQSAE_SetOverExp,
        IQSAE_SetBoundary,
        IQSAE_SetConverge,
        IQSAE_SetManual,
        IQSAWB_SetCTTable,      //10
        IQSAWB_SetWTable,
        IQSAWB_SetManual,
        IQSISP_SetECS,          //13
        IQSISP_SetOUTLIER,
        IQSISP_SetRANGE,
        IQSISP_SetOB,
        IQSISP_SetGBAL,
        IQSISP_SetLDC,
        IQSISP_SetEDGE,
        IQSISP_SetCCTRL,
        IQSISP_SetCOLORNF,
        IQSISP_SetTNR,
        IQSISP_SetGAMMA,
        IQSISP_SetCC,			//24
		IQSISP_SetMemMap,
		IQSISP_SetSensorId,
		IQSISP_SetPathId,
		IQSISP_SetSIEPause,
		IQSISP_SetAWDR,
		IQSISP_SetTNRCFG,
};


BOOL MsdcNvtRegSi_IQSim(void)
{
    return MsdcNvt_AddCallback_Si(MSDCIQSimGet,sizeof(MSDCIQSimGet)/sizeof(MSDCIQSimGet[0]),MSDCIQSimSet,sizeof(MSDCIQSimSet)/sizeof(MSDCIQSimSet[0]));
}

UINT32 IplId = 0;
UINT32 PathId = 0;

void IQSISP_GetRawInfo(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

	if (uiLength < 40)
	{
		DBG_ERR("size mismatch");
		return;
	}
	ipl_tuning_getrawinfo(IplId, (UINT32)pData, uiLength);
}


void IQSISP_GetVer(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

	if (uiLength < 4)
	{
		DBG_ERR("size mismatch");
		return;
	}
	pData[0] = 0x36363658;
}

void IQSAE_GetExpectLum(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

    AETuning_Get_Cmd(IplId, AE_GET_EXPECTLUM, (UINT32)pData, uiLength);
}
void IQSAE_GetHistogramadj(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

    AETuning_Get_Cmd(IplId, AE_GET_HISTOGRAMADJ, (UINT32)pData, uiLength);

}

void IQSAE_GetOverExp(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

    AETuning_Get_Cmd(IplId, AE_GET_OVEREXP, (UINT32)pData, uiLength);
}

void IQSAE_GetBoundary(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

    AETuning_Get_Cmd(IplId, AE_GET_BOUNDARY, (UINT32)pData, uiLength);
}

void IQSAE_GetConverge(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

    AETuning_Get_Cmd(IplId, AE_GET_CONVERGE, (UINT32)pData, uiLength);

}

void IQSAE_GetManual(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

    AETuning_Get_Cmd(IplId, AE_GET_MANUAL, (UINT32)pData, uiLength);

}

void IQSAE_GetStatus(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

    AETuning_Get_Cmd(IplId, AE_GET_STATUS, (UINT32)pData, uiLength);
}

void IQSAE_SetExpectLum(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

    AETuning_Set_Cmd(IplId, AE_SET_EXPECTLUM, (UINT32)pData, uiLength);

}
void IQSAE_SetHistogramadj(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

    AETuning_Set_Cmd(IplId, AE_SET_HISTOGRAMADJ, (UINT32)pData, uiLength);
}

void IQSAE_SetOverExp(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

    AETuning_Set_Cmd(IplId, AE_SET_OVEREXP, (UINT32)pData, uiLength);
}

void IQSAE_SetBoundary(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

    AETuning_Set_Cmd(IplId, AE_SET_BOUNDARY, (UINT32)pData, uiLength);
}

void IQSAE_SetConverge(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

    AETuning_Set_Cmd(IplId, AE_SET_CONVERGE, (UINT32)pData, uiLength);
}

void IQSAE_SetManual(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

    AETuning_Set_Cmd(IplId, AE_SET_MANUAL, (UINT32)pData, uiLength);
}

void IQSAWB_GetCTTable(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

    AWBTuning_Get_Cmd(IplId, AWB_TUNING_GET_CTTABLE, (UINT32)pData, uiLength);
}

void IQSAWB_GetWTable(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

    AWBTuning_Get_Cmd(IplId, AWB_TUNING_GET_WTABLE, (UINT32)pData, uiLength);
}

void IQSAWB_GetManual(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

    AWBTuning_Get_Cmd(IplId, AWB_TUNING_GET_MANUAL, (UINT32)pData, uiLength);
}

void IQSAWB_GetStatus(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

    AWBTuning_Get_Cmd(IplId, AWB_TUNING_GET_STATUS, (UINT32)pData, uiLength);
}

void IQSAWB_GetCA(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

    AWBTuning_Get_Cmd(IplId, AWB_TUNING_GET_CA, (UINT32)pData, uiLength);
}

void IQSAWB_GetFlag(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

    AWBTuning_Get_Cmd(IplId, AWB_TUNING_GET_FLAG, (UINT32)pData, uiLength);
}

void IQSAWB_SetCTTable(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

    AWBTuning_Set_Cmd(IplId, AWB_TUNING_SET_CTTABLE, (UINT32)pData, uiLength);
}

void IQSAWB_SetWTable(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

    AWBTuning_Set_Cmd(IplId, AWB_TUNING_SET_WTABLE, (UINT32)pData, uiLength);
}

void IQSAWB_SetManual(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

    AWBTuning_Set_Cmd(IplId, AWB_TUNING_SET_MANUAL, (UINT32)pData, uiLength);
}

void IQSISP_SetECS(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

    ISP_IQ_set_param(ISP_ECS, (UINT32)pData, uiLength);
}

void IQSISP_SetOUTLIER(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

    ISP_IQ_set_param(ISP_OUTLIER, (UINT32)pData, uiLength);
}

void IQSISP_SetRANGE(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

    ISP_IQ_set_param(ISP_RANGE, (UINT32)pData, uiLength);
}
void IQSISP_SetOB(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

    ISP_IQ_set_param(ISP_OB, (UINT32)pData, uiLength);
}

void IQSISP_SetGBAL(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

    ISP_IQ_set_param(ISP_GBAL, (UINT32)pData, uiLength);
}

void IQSISP_SetLDC(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

    ISP_IQ_set_param(ISP_LDC, (UINT32)pData, uiLength);
}

void IQSISP_SetEDGE(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

    ISP_IQ_set_param(ISP_EDGE, (UINT32)pData, uiLength);
}

void IQSISP_SetCCTRL(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

    ISP_IQ_set_param(ISP_CCTRL, (UINT32)pData, uiLength);
}

void IQSISP_SetCOLORNF(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

    ISP_IQ_set_param(ISP_COLORNF, (UINT32)pData, uiLength);
}

void IQSISP_SetTNR(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

    ISP_IQ_set_param(ISP_TNR, (UINT32)pData, uiLength);
}

void IQSISP_SetGAMMA(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

    ISP_IQ_set_param(ISP_GAMMA, (UINT32)pData, uiLength);
}

void IQSISP_SetCC(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

    ISP_IQ_set_param(ISP_CC, (UINT32)pData, uiLength);
}

void IQSISP_SetSensorId(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

	if (uiLength < 4)
	{
		DBG_ERR("set sensor id fail\r\n");
		return;
	}
	IplId = pData[0];
}

void IQSISP_SetPathId(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

	if (uiLength < 4)
	{
		DBG_ERR("set path id fail\r\n");
		return;
	}
	PathId = pData[0];
}

void IQSISP_SetSIEPause(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

	if (uiLength < 4)
	{
		DBG_ERR("set sie pause fail\r\n");
		return;
	}

	ipl_tuning_setsiemode(IplId, pData[0]);
}

void IQSISP_SetAWDR(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

    ISP_IQ_set_param(ISP_AWDR, (UINT32)pData, uiLength);
}

void IQSISP_SetTNRCFG(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

    ISP_IQ_set_param(ISP_TNR_CONFIG, (UINT32)pData, uiLength);
}

UINT16 IsoIdx = 0xffff;

UINT32 IQSISP_VirtualMemMap(UINT32 MemOffset, UINT32 MemSize)
{
	static UINT32 addr = 0x0;
	if (MemOffset == 0)
	{
		IPC_WaitImeFMD2(IplId, FALSE, 100);

		IPL_IME_BUF_ADDR ImeInfo;
		ImeInfo.Id = IplId;
		IPL_GetCmd(IPL_GET_IME_CUR_BUF_ADDR, (void *)&ImeInfo);

		if (PathId == 0) addr = ImeInfo.ImeP1.PixelAddr[0];
		if (PathId == 1) addr = ImeInfo.ImeP2.PixelAddr[0];
		if (PathId == 2) addr = ImeInfo.ImeP3.PixelAddr[0];
		if (PathId == 3) addr = ImeInfo.ImeP4.PixelAddr[0];
	}
	return (addr + MemOffset);
}

void IQSISP_SetMemMap(void)
{
	#define REG_TAG(ch0, ch1, ch2, ch3) ((UINT32)(UINT8)(ch0) | ((UINT32)(UINT8)(ch1) << 8) | ((UINT32)(UINT8)(ch2) << 16) | ((UINT32)(UINT8)(ch3) << 24 ))

    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
	if (pData[0] == REG_TAG('V', 'M', 'A', 'P'))
	{
		MsdcNvt_RegHandler(MSDCNVT_HANDLER_TYPE_VIRTUAL_MEM_MAP, IQSISP_VirtualMemMap);
}
	else
	{
		MsdcNvt_RegHandler(MSDCNVT_HANDLER_TYPE_VIRTUAL_MEM_MAP, NULL);
	}

	#undef REG_TAG
}

void IQSISP_GetAWDR(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();
	sISP_AWDR* sPtr = (sISP_AWDR *)pData;
	sPtr->Id = IplId;
	sPtr->ISO_index = IsoIdx;
    ISP_IQ_get_param(ISP_AWDR, (UINT32)pData, uiLength);
}

void IQSISP_GetTNRCFG(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

	sISP_TNR_CONFIG* sPtr = (sISP_TNR_CONFIG *)pData;
	sPtr->Id = IplId;
	sPtr->ISO_index = IsoIdx;
    ISP_IQ_get_param(ISP_TNR_CONFIG, (UINT32)pData, uiLength);
}

void IQSISP_GetImeRdyInfo(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

	if (uiLength < (6 * 4))
	{
		DBG_ERR("size mismatch\r\n");
		return;
	}
	IPL_IME_BUF_ADDR ImeInfo;
	ImeInfo.Id = IplId;
	IPL_GetCmd(IPL_GET_IME_CUR_BUF_ADDR, (void *)&ImeInfo);

	pData[0] = ImeInfo.ImeP1.PixelAddr[0];
	pData[1] = ImeInfo.ImeP1.PixelAddr[1];
	pData[2] = ImeInfo.ImeP2.PixelAddr[0];
	pData[3] = ImeInfo.ImeP2.PixelAddr[1];
	pData[4] = ImeInfo.ImeP3.PixelAddr[0];
	pData[5] = ImeInfo.ImeP3.PixelAddr[1];
}

void IQSISP_GetImeInfo(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();

	if (uiLength < (27 * 4))
	{
		DBG_ERR("size mismatch\r\n");
		return;
	}

	IPL_IME_INFOR ImeInfo = {0};
	ImeInfo.Id = IplId;
	if (IPL_GetCmd(IPL_GET_IME_INFOR, (void *)&ImeInfo) != E_OK)
	{
		DBG_ERR("ipl get ime command fail\r\n");
		return;
	}

	pData[0] = ImeInfo.P1Enable;
	pData[1] = ImeInfo.ImeP1[0].Ch[0].Width;
	pData[2] = ImeInfo.ImeP1[0].Ch[0].Height;
	pData[3] = ImeInfo.ImeP1[0].Ch[0].LineOfs;
	pData[4] = ImeInfo.ImeP1[0].type;
	pData[5] = ImeInfo.IME_P1_Crop.Start.x;
	pData[6] = ImeInfo.IME_P1_Crop.Start.y;
	pData[7] = ImeInfo.IME_P1_Crop.SizeH;
	pData[8] = ImeInfo.IME_P1_Crop.SizeV;

	pData[9] = ImeInfo.P2Enable;
	pData[10] = ImeInfo.ImeP2[0].Ch[0].Width;
	pData[11] = ImeInfo.ImeP2[0].Ch[0].Height;
	pData[12] = ImeInfo.ImeP2[0].Ch[0].LineOfs;
	pData[13] = ImeInfo.ImeP2[0].type;
	pData[14] = ImeInfo.IME_P2_Crop.Start.x;
	pData[15] = ImeInfo.IME_P2_Crop.Start.y;
	pData[16] = ImeInfo.IME_P2_Crop.SizeH;
	pData[17] = ImeInfo.IME_P2_Crop.SizeV;

	pData[18] = ImeInfo.P3Enable;
	pData[19] = ImeInfo.ImeP3[0].Ch[0].Width;
	pData[20] = ImeInfo.ImeP3[0].Ch[0].Height;
	pData[21] = ImeInfo.ImeP3[0].Ch[0].LineOfs;
	pData[22] = ImeInfo.ImeP3[0].type;
	pData[23] = ImeInfo.IME_P3_Crop.Start.x;
	pData[24] = ImeInfo.IME_P3_Crop.Start.y;
	pData[25] = ImeInfo.IME_P3_Crop.SizeH;
	pData[26] = ImeInfo.IME_P3_Crop.SizeV;
}

void IQSISP_GetECS(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();
	sISP_ECS* sPtr = (sISP_ECS *)pData;
	sPtr->Id = IplId;
    ISP_IQ_get_param(ISP_ECS, (UINT32)pData, uiLength);
}

void IQSISP_GetOUTLIER(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();
	sISP_OUTLIER* sPtr = (sISP_OUTLIER *)pData;
	sPtr->Id = IplId;
	sPtr->ISO_index = IsoIdx;
    ISP_IQ_get_param(ISP_OUTLIER, (UINT32)pData, uiLength);
}

void IQSISP_GetRANGE(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();
	sISP_RANGE* sPtr = (sISP_RANGE *)pData;
	sPtr->Id = IplId;
	sPtr->ISO_index = IsoIdx;
    ISP_IQ_get_param(ISP_RANGE, (UINT32)pData, uiLength);
}
void IQSISP_GetOB(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();
	sISP_OB* sPtr = (sISP_OB *)pData;
	sPtr->Id = IplId;
	sPtr->ISO_index = IsoIdx;
    ISP_IQ_get_param(ISP_OB, (UINT32)pData, uiLength);
}

void IQSISP_GetGBAL(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();
	sISP_GBAL* sPtr = (sISP_GBAL *)pData;
	sPtr->Id = IplId;
	sPtr->ISO_index = IsoIdx;
    ISP_IQ_get_param(ISP_GBAL, (UINT32)pData, uiLength);
}

void IQSISP_GetLDC(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();
	sISP_LDC* sPtr = (sISP_LDC *)pData;
	sPtr->Id = IplId;
	sPtr->ISO_index = IsoIdx;
    ISP_IQ_get_param(ISP_LDC, (UINT32)pData, uiLength);
}

void IQSISP_GetEDGE(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();
	sISP_EDGE* sPtr = (sISP_EDGE *)pData;
	sPtr->Id = IplId;
	sPtr->ISO_index = IsoIdx;
    ISP_IQ_get_param(ISP_EDGE, (UINT32)pData, uiLength);
}

void IQSISP_GetCCTRL(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();
	sISP_CCTRL* sPtr = (sISP_CCTRL *)pData;
	sPtr->Id = IplId;
	sPtr->ISO_index = IsoIdx;
    ISP_IQ_get_param(ISP_CCTRL, (UINT32)pData, uiLength);
}

void IQSISP_GetCOLORNF(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();
	sISP_COLORNF* sPtr = (sISP_COLORNF *)pData;
	sPtr->Id = IplId;
	sPtr->ISO_index = IsoIdx;
    ISP_IQ_get_param(ISP_COLORNF, (UINT32)pData, uiLength);
}

void IQSISP_GetTNR(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();
	sISP_TNR* sPtr = (sISP_TNR *)pData;
	sPtr->Id = IplId;
	sPtr->ISO_index = IsoIdx;
    ISP_IQ_get_param(ISP_TNR, (UINT32)pData, uiLength);
}

void IQSISP_GetGAMMA(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();
	sISP_GAMMA* sPtr = (sISP_GAMMA *)pData;
	sPtr->Id = IplId;
    ISP_IQ_get_param(ISP_GAMMA, (UINT32)pData, uiLength);
}

void IQSISP_GetCC(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();
	sISP_CC* sPtr = (sISP_CC *)pData;
	sPtr->Id = IplId;
    ISP_IQ_get_param(ISP_CC, (UINT32)pData, uiLength);
}

void IQSim_GetBinAddr(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();
    if(uiLength!=sizeof(UINT32))
        return;
    pData[0] = (UINT32)IQSIM_GetAppInfo();
}

void IQSim_GetRawCh0Addr(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();
    if(uiLength!=sizeof(UINT32))
        return;
    UINT32 ch0, ch2;
    IQSIM_GetRawAddr(&ch0, &ch2);
    pData[0] = ch0;
}

void IQSim_GetRawCh2Addr(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();
    if(uiLength!=sizeof(UINT32))
        return;
    UINT32 ch0, ch2;
    IQSIM_GetRawAddr(&ch0, &ch2);
    pData[0] = ch2;
}


void IQSim_GetJpgAddr(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();
    if(uiLength!=sizeof(UINT32))
        return;

    UINT32 Addr, Size;
    ImgCap_GetCurJpgInfo(&Addr, &Size);
    pData[0] = Addr;
    DBG_ERR("IQSim_GetJpgAddr return %0x08x\r\n", pData[0]);
}

void IQSim_GetJpgSize(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();
    if(uiLength!=sizeof(UINT32))
        return;

    UINT32 Addr, Size;
    ImgCap_GetCurJpgInfo(&Addr, &Size);
    pData[0] = Size;
    DBG_ERR("IQSim_GetJpgSize return %d\r\n", pData[0]);
}


void IQSim_GetQVAddr(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();
    if(uiLength!=sizeof(UINT32))
        return;

    UINT32 Addr, Size, Fmt;
    ImgCap_GetCurQVInfo(&Addr, &Size, &Fmt);
    pData[0] = Addr;
    DBG_ERR("IQSim_GetQVAddr return %0x08x\r\n", pData[0]);
}

void IQSim_GetQVSize(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();
    if(uiLength!=sizeof(UINT32))
        return;

    UINT32 Addr, Size, Fmt;
    ImgCap_GetCurQVInfo(&Addr, &Size, &Fmt);
    pData[0] = Size;
    DBG_ERR("IQSim_GetQVSize return %d\r\n", pData[0]);
}

void IQSim_GetQVFmt(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();
    if(uiLength!=sizeof(UINT32))
        return;

    UINT32 Addr, Size, Fmt;
    ImgCap_GetCurQVInfo(&Addr, &Size, &Fmt);
    pData[0] = Fmt;
    DBG_ERR("IQSim_GetQVFmt return %d\r\n", pData[0]);
}


void IQSim_GetPRIAddr(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();
    if(uiLength!=sizeof(UINT32))
        return;

    UINT32 Addr, Size, Fmt;
    ImgCap_GetCurPRIInfo(&Addr, &Size, &Fmt);
    pData[0] = Addr;
    DBG_ERR("IQSim_GetPRIAddr return %0x08x\r\n", pData[0]);
}

void IQSim_GetPRISize(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();
    if(uiLength!=sizeof(UINT32))
        return;

    UINT32 Addr, Size, Fmt;
    ImgCap_GetCurPRIInfo(&Addr, &Size, &Fmt);
    pData[0] = Size;
    DBG_ERR("IQSim_GetPRISize return %d\r\n", pData[0]);
}

void IQSim_GetPRIFmt(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();
    if(uiLength!=sizeof(UINT32))
        return;

    UINT32 Addr, Size, Fmt;
    ImgCap_GetCurPRIInfo(&Addr, &Size, &Fmt);
    pData[0] = Fmt;
    DBG_ERR("IQSim_GetPRIFmt return %d\r\n", pData[0]);
}

extern ER ImgCap_RawIsIdle(void);
extern ER ImgProc_IsIdle(void);
extern ER ImgEnc_IsIdle(void);
extern ER ImgProc2_IsIdle(void);

void IQSim_GetJpgOk(void)
{
    UINT32* pData = (UINT32*)MsdcNvt_GetDataBufferAddress();
    UINT32 uiLength = MsdcNvt_GetTransSize();
    if(uiLength!=sizeof(UINT32))
        return;
    if ( ImgCap_RawIsIdle() == E_OK && ImgProc_IsIdle() == E_OK && ImgProc2_IsIdle() == E_OK &&ImgEnc_IsIdle() == E_OK )
        pData[0] = TRUE;
    else
        pData[0] = FALSE;
    DBG_ERR("IQSim_GetJpgOk return %d\r\n", pData[0]);
}

void IQSim_SetConvertInit(void)
{
    //***************************************
    //* Open Sensor in Playback only for IQSIM flow
    //* Add by Wendy Liao 2016/06/17
    //***************************************
    SENSOR_INIT_OBJ SenObj;
    SENSOR_DRVTAB *pSenTab;

    SenObj = DrvSensor_GetObj(IPL_ID_1);
    pSenTab = DrvSensor_GetTab(IPL_ID_1);
    DrvSensor_PowerTurnOn(IPL_ID_1);
    Sensor_Open(IPL_ID_1, &SenObj, pSenTab);
    //***************************************

    UINT32 uiPoolAddr;
    CAP_OBJ_INFO CapObj = {0};
    uiPoolAddr = OS_GetMempoolAddr(POOL_ID_APP);

#if 0
    UINT32 BinAddr = uiPoolAddr;
    UINT32 Ch0Addr = BinAddr+IQSIM_MAX_BIN_SIZE;
    UINT32 Ch2Addr = Ch0Addr+IQSIM_MAX_CH0_SIZE;
    UINT32 uiBufferSize = IQSIM_MAX_BIN_SIZE+IQSIM_MAX_CH0_SIZE+IQSIM_MAX_CH2_SIZE;
#else
    UINT32 BinAddr = uiPoolAddr;
    UINT32 Ch2Addr = uiPoolAddr+POOL_SIZE_APP-IQSIM_MAX_CH2_SIZE;
    UINT32 Ch0Addr = Ch2Addr-IQSIM_MAX_CH0_SIZE;
    UINT32 uiBufferSize = IQSIM_MAX_BIN_SIZE;
#endif

    DBG_ERR("BinAddr = %X\r\n", BinAddr);
    DBG_ERR("Ch0Addr = %X\r\n", Ch0Addr);
    DBG_ERR("Ch2Addr = %X\r\n", Ch2Addr);
    DBG_ERR("uiBufferSize = %X\r\n", uiBufferSize);


    pll_setPLLEn(PLL_ID_5, ENABLE);

    ImgCap_SetUIInfo(CAP_SEL_IPL_ID, IPL_ID_1);

    //enter
    IPL_Open();
   // IPL_Display_OpenTsk();
    CapObj.uiBufAddr = uiPoolAddr + uiBufferSize;
    CapObj.uiBufSize = POOL_SIZE_APP - uiBufferSize;
    CapObj.S2DetFP = NULL;
    CapObj.CBMsgFP = NULL;
    CapObj.WriteFileFP = NULL;
    ImgCap_Open(&CapObj);

    IQSIM_SetAppInfo(BinAddr);
    IQSIM_SetRawAddr(Ch0Addr, Ch2Addr);
}

void IQSim_SetConvertProc(void)
{
    IQSIM_APPINFO *AppInfo;

    IQSIM_ReorgAppInfo();

    AppInfo = (IQSIM_APPINFO *)IQSIM_GetAppInfo();
    ImgCap_SetUIInfo(CAP_SEL_DRIVEMODE, SEL_DRIVEMODE_SIM);
    ImgCap_SetUIInfo(CAP_SEL_PICNUM, 1);
    ImgCap_SetUIInfo(CAP_SEL_RAW_BUF_NUM, 1);
    ImgCap_SetUIInfo(CAP_SEL_JPG_BUF_NUM,1);
    ImgCap_SetUIInfo(CAP_SEL_FILEFMT, SEL_FILEFMT_JPG);
    ImgCap_SetUIInfo(CAP_SEL_JPGFMT, SEL_JPGFMT_422);
    ImgCap_SetUIInfo(CAP_SEL_ENCRATIO, 100);
    ImgCap_SetUIInfo(CAP_SEL_JPG_IMG_H_SIZE, AppInfo->IQSimJpgInfo.Size.w);
    ImgCap_SetUIInfo(CAP_SEL_JPG_IMG_V_SIZE, AppInfo->IQSimJpgInfo.Size.h);

#if 0
    ImgCap_SetUIInfo(CAP_SEL_QV_BUF_H_SIZE, AppInfo->IQSimQVInfo.RawInfo.Size.w);
    ImgCap_SetUIInfo(CAP_SEL_QV_BUF_V_SIZE, AppInfo->IQSimQVInfo.RawInfo.Size.h);
    ImgCap_SetUIInfo(CAP_SEL_QV_IMG_H_SIZE, AppInfo->IQSimQVInfo.RawInfo.Size.w);
    ImgCap_SetUIInfo(CAP_SEL_QV_IMG_V_SIZE, AppInfo->IQSimQVInfo.RawInfo.Size.h);
#endif

//    ImgCap_SetUIInfo(CAP_SEL_QV_BUF_H_SIZE, 640);
//    ImgCap_SetUIInfo(CAP_SEL_QV_BUF_V_SIZE, 480);
    ImgCap_SetUIInfo(CAP_SEL_QV_IMG_H_SIZE, 640);
    ImgCap_SetUIInfo(CAP_SEL_QV_IMG_V_SIZE, 480);
    ImgCap_SetUIInfo(CAP_SEL_REENCTYPE, SEL_REENCTYPE_FIXQ);
    ImgCap_SetUIInfo(CAP_SEL_QUALITY, AppInfo->IQSimJpgInfo.BitRate);
    DBG_ERR("JPG BitRate = %d\r\n", AppInfo->IQSimJpgInfo.BitRate);

    ImgCap_StartCapture();
}

void IQSim_SetConvertEnd(void)
{
    DBG_ERR("IQSim_SetConvertEnd\r\n");

    //exit
#if 0
    {
        FST_FILE    pFileHdl;
        UINT32 JpgAddr, JpgSize;
        ImgCap_GetCurJpgInfo(&JpgAddr, &JpgSize);
        pFileHdl = FileSys_OpenFile("A:\\IQSIM\\IQSIM.JPG",FST_CREATE_ALWAYS|FST_OPEN_WRITE);
        FileSys_WriteFile(pFileHdl, (UINT8*)JpgAddr, &JpgSize, 0, NULL);
        FileSys_CloseFile(pFileHdl);
    }
#endif

    ImgCap_Close();
    IPL_Close();
    //IPL_Display_CloseTsk();
}


extern void SMediaRec_Set3DNROutAddr(UINT32 pathID, UINT32 uiFrameId, UINT32 uiYAddr, UINT32 uiUVAddr);
extern void SMediaRec_Trigger3DNROut(UINT32 pathID);

UINT32 IQUC_Trigger3DNROut(UINT32 Id, UINT32 BufAddr, UINT32 BufSize, UINT32 *OutAddr, UINT32 *OutSize, UINT32 *OutFrameCnt)
{
    UINT32      ui3DNROutYAddr[2], ui3DNROutUVAddr[2], uiSize;
    IPL_IME_BUF_ADDR CurInfo={0};

    CurInfo.Id = Id;
    IPL_GetCmd(IPL_GET_IME_CUR_BUF_ADDR, (void *)&CurInfo);
    uiSize = 0;
    *OutFrameCnt = 2;
    if (CurInfo.P1Enable == ENABLE)
    {
        uiSize = CurInfo.ImeP1.Ch[IPL_IMG_YUV_Y].LineOfs * CurInfo.ImeP1.Ch[IPL_IMG_YUV_Y].Height;
        //DBG_IND("Video Size %d x %d, %d\r\n", CurInfo.ImeP1.Ch[IPL_IMG_YUV_Y].Width , CurInfo.ImeP1.Ch[IPL_IMG_YUV_Y].Height, uiSize);
        // set 3DNR output buffer for 2 frames (if using 2P encode)
        UINT32 Ratio;
        UINT32 Type = CurInfo.ImeP1.type;
        if ((Type == IPL_IMG_YUV444) || (Type == IPL_IMG_Y_PACK_UV444) || (Type == IPL_IMG_PACK_YUV444))
        {
            Ratio = 30;
        }
        else if ((Type == IPL_IMG_YUV422) || (Type == IPL_IMG_Y_PACK_UV422))
        {
            Ratio = 20;
        }
        else if ((Type == IPL_IMG_YUV420) || (Type == IPL_IMG_Y_PACK_UV420))
        {
            Ratio = 15;
        }
        else
        {
            Ratio = 0;
            DBG_ERR("cal yuv size fail (%d)\r\n", Type);
        }
        OutSize[0] = uiSize * Ratio / 10;
        OutSize[1] = OutSize[0];

        OutAddr[0] = BufAddr;
        OutAddr[1] = BufAddr + OutSize[0];

        ui3DNROutYAddr[0]  = OutAddr[0];
        ui3DNROutUVAddr[0] = OutAddr[0] + uiSize;
        ui3DNROutYAddr[1]  = OutAddr[1];
        ui3DNROutUVAddr[1] = OutAddr[1] + uiSize;

        SMediaRec_Set3DNROutAddr(Id/*video ID*/, 0/*1st frame of 2P encode*/, ui3DNROutYAddr[0], ui3DNROutUVAddr[0]);
        SMediaRec_Set3DNROutAddr(Id/*video ID*/, 1/*2nd frame of 2P encode*/, ui3DNROutYAddr[1], ui3DNROutUVAddr[1]);
        SMediaRec_Trigger3DNROut(Id/*video ID*/);
        Delay_DelayMs(30);
    }
    else
    {
        OutSize[0] = BufSize;
        OutSize[1] = BufSize;

        OutAddr[0] = BufAddr;
        OutAddr[1] = BufAddr;
    }
    return uiSize;
}


void IQS_GetIQCtrl(void)
{
    IQUC_CTRL_ITEM* pData = (IQUC_CTRL_ITEM*)MsdcNvt_GetDataBufferAddress();
    IQUC_CTRL_ITEM* pItem;
    UINT32 Id = pData->Id;

    //DBG_IND("IQS_GetIQCtrl %d %x\r\n", Id, IQItem.CmdData[0]);

    if ((pItem = IQUCIF_GetCtrlItem(Id, IQUCIF_GetCtrlIndex(Id))) == NULL)
        return;

    if ( IQItem.CmdData[0] == IQCMD_READFROMMEMORY )
    {
        *pData = *pItem;
        //DBG_IND("IQUCIF_GetCtrlIndex = %d %08x %d\r\n", IQUCIF_GetCtrlIndex(Id), pData, pData->ADJ_ITEM[ADJ_EXPOSURE]);
    }
    else if ( IQItem.CmdData[0] == IQCMD_GETMCTFOUT )
    {
        UINT32 BufAddr, BufSize, OutAddr[10], OutSize[10],OutFrameCnt;
        IQUCIF_GetBufferInfo(Id, &BufAddr, &BufSize);
        IQUC_Trigger3DNROut(Id, BufAddr, BufSize, OutAddr, OutSize, &OutFrameCnt);
        pItem->CmdData[0] = OutAddr[0];
        pItem->CmdData[1] = OutSize[0];
        pItem->CmdData[2] = OutAddr[1];
        pItem->CmdData[3] = OutSize[1];
        *pData = *pItem;
        //DBG_IND("GetMCTFOut = 0x%08x(%d) 0x%08x(%d)\r\n", pItem->CmdData[0], pItem->CmdData[1], pItem->CmdData[2], pItem->CmdData[3]);
    }
    else if ( IQItem.CmdData[0] == IQCMD_GETIMEINFO )
    {
        IQUC_IME_INFO Info;
        IPL_IME_BUF_ADDR CurInfo = {0};
        CurInfo.Id = Id;
        Info.id = Id;
        IPL_GetCmd(IPL_GET_IME_CUR_BUF_ADDR, (void *)&CurInfo);
        if ( IQItem.CmdData[1] == 1 )
        {
            Info.enable = CurInfo.P1Enable;
            Info.type   = CurInfo.ImeP1.type;
            Info.width  = CurInfo.ImeP1.Ch[IPL_IMG_YUV_Y].Width;
            Info.height = CurInfo.ImeP1.Ch[IPL_IMG_YUV_Y].Height;
            Info.offset = CurInfo.ImeP1.Ch[IPL_IMG_YUV_Y].LineOfs;
        }
        else if ( IQItem.CmdData[1] == 2 )
        {
            Info.enable = CurInfo.P2Enable;
            Info.type   = CurInfo.ImeP2.type;
            Info.width  = CurInfo.ImeP2.Ch[IPL_IMG_YUV_Y].Width;
            Info.height = CurInfo.ImeP2.Ch[IPL_IMG_YUV_Y].Height;
            Info.offset = CurInfo.ImeP2.Ch[IPL_IMG_YUV_Y].LineOfs;
        }
        else if ( IQItem.CmdData[1] == 3 )
        {
            Info.enable = CurInfo.P3Enable;
            Info.type   = CurInfo.ImeP3.type;
            Info.width  = CurInfo.ImeP3.Ch[IPL_IMG_YUV_Y].Width;
            Info.height = CurInfo.ImeP3.Ch[IPL_IMG_YUV_Y].Height;
            Info.offset = CurInfo.ImeP3.Ch[IPL_IMG_YUV_Y].LineOfs;
        }
        else
        {
            Info.enable = DISABLE;
            Info.type   = 0;
            Info.width  = 0;
            Info.height = 0;
            Info.offset = 0;
            DBG_ERR("wrong IQItem.CmdData[1]!!!\r\n");
        }
        memcpy(pItem->CmdData, &Info, sizeof(IQUC_IME_INFO));
        *pData = *pItem;
        //DBG_IND("Path = %d -->  enable:%d  type:%d   width:%d   height:%d   offset:%d\r\n",IQItem.CmdData[1],  Info.enable, Info.type, Info.width, Info.height, Info.offset );
    }
}

extern UINT32 IQUC_DumpUserControlTable(UINT32 Id);
extern UINT32 IQUC_DumpSettingTable(UINT32 Id);
extern UINT32 IQUC_DumpRatioTable(UINT32 Id);
void IQS_SetIQCtrl(void)
{
    UINT32 Id = IPL_ID_1;
    IQUC_CTRL_ITEM* pData = (IQUC_CTRL_ITEM*)MsdcNvt_GetDataBufferAddress();
    UINT32  uiLength = MsdcNvt_GetTransSize();

    if(!uiLength)
        return ;

    Id = pData->Id;
    //DBG_MSG("IQS_SetIQCtrl %d %X\r\n",Id, pData->Cmd);
    if ( pData->Cmd == IQCMD_SET )
    {
        memcpy(&IQItem,(UINT8*)pData, sizeof(IQUC_CTRL_ITEM));
    }
    else if ( pData->Cmd == IQCMD_SCENELOCK )
    {
        uiRGain[Id] = pData->ADJ_ITEM[ADJ_AWB_RGAIN];
        uiGGain[Id] = pData->ADJ_ITEM[ADJ_AWB_GGAIN];
        uiBGain[Id] = pData->ADJ_ITEM[ADJ_AWB_BGAIN];

        uiExpoTime[Id] = pData->ADJ_ITEM[ADJ_EXPOSURE];
        uiISOGain[Id]  = pData->ADJ_ITEM[ADJ_GAIN];

        DBG_ERR("AE  LOCK %d: %d %d %d\r\n", Id,  uiExpoTime[Id],  uiISOGain[Id],  uiIris[Id]);
        DBG_ERR("AWB LOCK %d: %d %d %d\r\n", Id,  uiRGain[Id],  uiGGain[Id],  uiBGain[Id]);
    }
    else if ( pData->Cmd == IQCMD_SCENEUNLOCK )
    {
        uiRGain[Id] = uiGGain[Id] = uiBGain[Id] = 0;
        uiExpoTime[Id] = uiISOGain[Id] = uiIris[Id] = 0;
        DBG_ERR("%d: %d %d %d\r\n", Id,  uiRGain[Id],  uiGGain[Id],  uiBGain[Id]);
    }

    else if ( pData->Cmd == IQCMD_RUNPARAM )
    {
        IQUCIF_SetCtrlItem(Id, pData->IQIndex, pData);
    }
    else if ( pData->Cmd == IQCMD_DUMPIQSETTING )
    {
        IQUC_DumpUserControlTable(Id);
        IQUC_DumpSettingTable(Id);
    }
    else if ( pData->Cmd == IQCMD_DUMPIQSETTING_SIQT )
    {
        IQUC_DumpRatioTable(Id);
    }
}
