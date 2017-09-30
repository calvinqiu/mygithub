/*
    IPL flow Callback Function

    This file is the API of the flow callback function.

    @file       IPL_FlowCBSample.c
    @ingroup    mISYSAlg
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/
#define MS_CTRLBY_TIMER 1
#include "IPL_IQSIM_EVB_FF_Int.h"
#include "Delay.h"
#include "Lens.h"
#include "ae_alg.h"
#include "LensCtrlTsk.h"
#include "sie_lib.h"
#if MS_CTRLBY_TIMER
#include "timer.h"
#endif
void IPL_FCB_GetCapRawNormal(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo);
void IPL_FCB_GetCapRawSim(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo);
void IPL_FCB_GetCapRawInVid(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo);
void IPL_FCB_GetCapRawEBurst(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo);
//#NT#2016/03/30#Harry Tsai -begin
//#NT# Add side by side capture mode
void IPL_FCB_GetCapRawDual(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo);
//#NT#2016/03/30#Harry Tsai -end

typedef void (*CapFP)(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo);

//static FLASH_INFOR MainFlashInfo = {0};

CapFP CaptureFP[IPLCTRL_CAP_FLOW_MAX] =
{
    NULL,
    IPL_FCB_GetCapRawNormal,
    IPL_FCB_GetCapRawNormal,
    IPL_FCB_GetCapRawNormal,
    IPL_FCB_GetCapRawNormal,
    IPL_FCB_GetCapRawEBurst,
    IPL_FCB_GetCapRawInVid,
    IPL_FCB_GetCapRawNormal,
    IPL_FCB_GetCapRawNormal,
    IPL_FCB_GetCapRawSim,
    IPL_FCB_GetCapRawNormal,
    IPL_FCB_GetCapRawNormal,
//#NT#2016/03/30#Harry Tsai -begin
//#NT# Add side by side capture mode
    IPL_FCB_GetCapRawDual
//#NT#2016/03/30#Harry Tsai -end
};

void IPL_FCB_PrepareCapAE(IPL_PROC_ID Id)
{

}


void IPL_FCB_FlashTrig(FLASH_INFOR *Data)
{
}
/*
static void IPL_FCB_FlashCfg_Main(FLASH_INFOR *Data)
{
}

static void IPL_FCB_MshutterConfig(IPL_PROC_ID Id)
{
}
*/

void IPL_FCB_MshutterClose(UINT32 uiEvent)
{
}

void IPL_FCB_MshutterOpen(IPL_PROC_ID Id)
{
}

void IPL_FCB_PreFlash(IPL_PROC_ID Id)
{
}

BOOL GRS_Mode = 0;
void IPL_FCB_GetCapRawNormal(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo)
{
}

void IPL_FCB_GetCapRawInVid(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo)
{
}

void IPL_FCB_GetCapRawEBurst(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo)
{
}

void IPL_FCB_GetCapRawDual(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo)
{
}

void IPL_FCB_GetCapRaw(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo)
{
    if (CaptureFP[IPL_CtrlGetInfor(Id, IPLCTRL_CAPFLOW)] != NULL)
    {
        if (IPLCTRL_CAP_FLOW_EBURST == IPL_CtrlGetInfor(Id, IPLCTRL_CAPFLOW))
        {
            if (ExtInfo->Param[0] != 2)
            {
                IPL_CBMsgProc(Id, IPL_CBMSG_SHUTTER_SOUND, NULL);
            }
        }
        else
        {
            IPL_CBMsgProc(Id, IPL_CBMSG_SHUTTER_SOUND, NULL);
        }
        CaptureFP[IPL_CtrlGetInfor(Id, IPLCTRL_CAPFLOW)](Id, Info, ExtInfo);
    }
    else
    {
        DBG_ERR("capture flow(%d) FP null\r\n", IPL_CtrlGetInfor(Id, IPLCTRL_CAPFLOW));
    }
}

void IPL_FCB_GetCapRawSim(IPL_PROC_ID Id, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo)
{
    DBG_ERR("IQSIM Cap Raw\r\n");
    UINT32 ch0, ch2;
    IQSIM_APPINFO *AppInfo;

    IQSIM_GetRawAddr(&ch0, &ch2);
    AppInfo = (IQSIM_APPINFO *)IQSIM_GetAppInfo();

    UINT32 size = AppInfo->IQSimPriInfo.RawInfo.Size.w * AppInfo->IQSimPriInfo.RawInfo.Size.h * AppInfo->IQSimPriInfo.RawInfo.BitNum / 8;
    DBG_ERR("Ch0 = %x, %x, size = %d\r\n", Info->psie->CH0.Out.PPB0_Addr, ch0, size);
    memcpy((void*)Info->psie->CH0.Out.PPB0_Addr, (void*)ch0, size);
#if 0
    FST_FILE    pFileHdl;
    pFileHdl = FileSys_OpenFile("A:\\IQSIM\\CH0Dump.RAW",FST_CREATE_ALWAYS|FST_OPEN_WRITE);
    FileSys_WriteFile(pFileHdl, (UINT8*)ch0, &size, 0, NULL);
    FileSys_CloseFile(pFileHdl);
#endif

}

void IPL_FCB_SetCapAWB(IPL_PROC_ID Id, IPC_SIE_Info *Info)
{
}

void IPL_FCB_AlgGDC(IPL_PROC_ID Id, UINT32 SelIdx, UINT32 Value)
{
    IPL_MODE Mode;

    Mode = IPL_CtrlGetInfor(Id, IPLCTRL_CUR_MODE);

    if ((Mode != IPL_MODE_PREVIEW) &&
        (Mode != IPL_MODE_VIDEO) &&
        (Mode != IPL_MODE_VIDEOREC))
    {
        return;
    }

    if (Value == SEL_GDCCOMP_ON)
    {

    }
    else
    {
    }

    DBG_ERR("runtime set DCE interface not ready\r\n");
}

void IPL_FCB_AlgDIS(IPL_PROC_ID Id,UINT32 SelIdx, UINT32 Value)
{
    UINT32 Buf, Size;
    IPL_MODE Mode;
    IPL_DZOOM Info_DZ;

    Mode = IPL_CtrlGetInfor(Id, IPLCTRL_CUR_MODE);

    if ((Mode != IPL_MODE_PREVIEW) &&
        (Mode != IPL_MODE_VIDEO) &&
        (Mode != IPL_MODE_VIDEOREC))
    {
        return;
    }

    if (Value == SEL_DISCOMP_OFF)
    {
        //IQS_SettingFlowFunc(Id, IQS_DISCOMP_OFF);
        IPL_CtrlSetInfor(Id, IPLCTRL_DIS_EN, DISABLE);
    }
    else if (Value == SEL_DISCOMP_ON)
    {
        IPL_BufGet(Id, IPLBUF_SIEETH_1, &Buf, &Size);
        if (Size != 0)
        {
            //IQS_SettingFlowFunc(Id, IQS_DISCOMP_ON);
            IPL_CtrlSetInfor(Id, IPLCTRL_DIS_EN, ENABLE);
        }
        else
        {
            return;
        }
    }

    Info_DZ.Id = Id;
    Info_DZ.Idx = IPL_DzoomGetIndex(Info_DZ.Id);
    IPL_SetCmd(IPL_SET_DZOOM, (void *)&Info_DZ);
    IPL_WaitCmdFinish();
}

void IPL_FCB_AlgIE(IPL_PROC_ID Id,UINT32 SelIdx, UINT32 Value)
{
    IQS_SettingFlowFunc(Id, IQS_IMAGE_EFFECT);
}

void IPL_FCB_ImageFlip(IPL_PROC_ID Id,UINT32 SelIdx, UINT32 Value)
{
//#NT#2016/03/02#Jarkko Chang -begin
//#NT# support both dsp and sensor flip
    if (Value == SEL_IMAGEFLIP_DSP_H_V || Value == SEL_IMAGEFLIP_DSP_H || Value == SEL_IMAGEFLIP_DSP_V || Value == SEL_IMAGEFLIP_DSP_OFF)
    {
        IPL_SET_FLIP_DATA FlipInfo;

        FlipInfo.Id = Id;
        if ( Value == SEL_IMAGEFLIP_DSP_H_V )
        FlipInfo.FlipInfo = FLIP_H_V;
        if ( Value == SEL_IMAGEFLIP_DSP_H )
        FlipInfo.FlipInfo = FLIP_H;
        if ( Value == SEL_IMAGEFLIP_DSP_V )
        FlipInfo.FlipInfo = FLIP_V;
        if ( Value == SEL_IMAGEFLIP_DSP_OFF )
        FlipInfo.FlipInfo = FLIP_NONE;

        IPL_SetCmd(IPL_SET_FLIP, (void *)&FlipInfo);
        IPL_WaitCmdFinish();
    }
    else
    {
        UINT32 option;
        SENSOR_ID SenId;

        SenId = IPL_UTI_CONV2_SEN_ID(Id);
        option = Sensor_GetUserOption(SenId);
        option &= ~(SENSOR_OPTION_MIRROR | SENSOR_OPTION_FLIP);
        if ( Value == SEL_IMAGEFLIP_SEN_H_V )
            option = (SENSOR_OPTION_MIRROR | SENSOR_OPTION_FLIP);
        if ( Value == SEL_IMAGEFLIP_SEN_H )
            option = (SENSOR_OPTION_MIRROR);
        if ( Value == SEL_IMAGEFLIP_SEN_V )
            option = (SENSOR_OPTION_FLIP);
        Sensor_SetUserOption(SenId, option);

    }
//#NT#2016/03/02#Jarkko Chang -end
}

void IPL_FCB_AlgWDR(IPL_PROC_ID Id, UINT32 SelIdx, UINT32 Value)
{
    IPL_MODE Mode;

    Mode = IPL_CtrlGetInfor(Id, IPLCTRL_CUR_MODE);
	//#NT#2017/01/06#Jarkko Chang -begin
    //#NT# modify for wdr_defog
    if (Value != SEL_WDR_OFF)
    {
        ExtFunc_Init(Id, NULL, NULL);
    }
    else
    {
        IQS_SettingFlowFunc(Id, IQS_WDR_END);
    }
	//#NT#2017/01/06#Jarkko Chang -end
}

void IPL_FCB_AlgWDRLevel(IPL_PROC_ID Id, UINT32 SelIdx, UINT32 Value)
{
    ExtFunc_SetWDRLumaLevel(Id, Value);
}

void IPL_RAWHeaderCB(IPL_PROC_ID Id, UINT32 FrmCnt)
{
    AEALG_ARG AeData = {0};
    IPL_HEADER_RAW_EXP_INFO AeHeaderInfo = {0};

    IPL_HeaderRawLock(Id);
    //write header information at here
    if (FrmCnt > 0)
    {
        AE_GetSyncInfo(Id, FrmCnt, &AeData);

        AeHeaderInfo.expt = AeData.ExpoTime;
        AeHeaderInfo.iris = AeData.Iris;
        AeHeaderInfo.iso = AeData.ISOGain;

        IPL_HeaderRawSetInfo_Dly1VD(Id, IPL_HEADER_RAW_EXP, &AeHeaderInfo);
    }

    IPL_HeaderRawUnlock(Id);
}
