/**
    ISR sample.

    @file       IPL_Isr_TVP5150_EVB_FF.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "IPL_TVP5150_EVB_FF_Int.h"
void IPL_IsrSIECB(IPL_PROC_ID Id, UINT32 IntStatus,UINT32 idx)
{
    //customized op
    if (IntStatus & SIE_FLDEND)
    {
        if (idx > 1)    //skip 3A for first frame(garbage)
        {
			//#NT#2016/10/06#Jarkko Chang -begin
            //#NT# skip trigger 2a when ccir sensor
		    if (IPL_CtrlGetInfor(Id, IPLCTRL_CFMODE) != IPL_CF_CCIR)
		    {
	            if((Id == IPL_ID_2) && (IPL_CtrlGetInfor(Id, IPLCTRL_EXT_FUNC_EN) == ICF_FUNC_SHDR))
	            {
	            }
	            else
	            {
	                AE_TrigEvent(IPL_UTI_CONV2_AE_ID(Id));
	                AWB_TrigEvent(IPL_UTI_CONV2_AWB_ID(Id));
	            }
		    }
			//#NT#2016/10/06#Jarkko Chang -end

            if (IPL_CtrlGetInfor(Id, IPLCTRL_EXT_FUNC_EN) == ICF_FUNC_WDR)
                ICFFunc_TrigEvent(Id);

            if((AFTsk_IsCAFLock(IPL_UTI_CONV2_AF_ID(Id))) && (Id == IPL_ID_1))
                AF_CAFProcess(IPL_UTI_CONV2_AF_ID(Id));
            else if(Id == IPL_ID_1)
                AF_TrigEvent(IPL_UTI_CONV2_AF_ID(Id));
        }
    }

    DISALG_PARAM disInfo = {0};
    UINT32 Buf,Size;
    if (IntStatus & SIE_DRAM4_END)
    {
        if (IPL_CtrlGetInfor(Id, IPLCTRL_DIS_EN) == ENABLE)
        {
            //QInfo = IPL_CTRLQuery(Id, ICF_ETHOUT_RDY);

            disInfo.frame_cnt = idx;
            disInfo.InSizeH = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_H);

            disInfo.InLineOfs = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_ETH_SIZE_HLOS);
            disInfo.InSizeV = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_V);

            IPL_BufGet(Id, IPLBUF_SIEETH_1, &Buf, &Size);
            disInfo.InAdd0 = Buf;
            IPL_BufGet(Id, IPLBUF_SIEETH_2, &Buf, &Size);
            disInfo.InAdd1 = Buf;
            IPL_BufGet(Id, IPLBUF_SIEETH_3, &Buf, &Size);
            disInfo.InAdd2 = Buf;

//            DBG_ERR("%d %d %d %d 0x%.8x 0x%.8x\r\n", disInfo.frame_cnt, disInfo.InSizeH,
//                                disInfo.InLineOfs, disInfo.InSizeV, disInfo.InAdd0, disInfo.InAdd1);
            if (dis_getTriggerStatus() == FALSE)
            {
                dis_setDisInfor(&disInfo);
            }
        }
    }

    if (IntStatus & SIE_BP1)
    {
    }
    if (IntStatus & SIE_BP2)
    {
    }
}

void IPL_IsrIMECB(IPL_PROC_ID Id, UINT32 IntStatus,UINT32 idx)
{
    if (IntStatus & IME_FMD)
    {
        UINT32 ProcId = Id;
        IPL_MODE_INFOR Info1 = {0}, Info2={0};
        Info1.Id = IPL_ID_1;
        IPL_GetCmd(IPL_GET_CUR_MODE, (void *)&Info1);
        Info2.Id = IPL_ID_2;
        IPL_GetCmd(IPL_GET_CUR_MODE, (void *)&Info2);
        if ((Info1.Mode != IPL_MODE_OFF && Info1.Mode != IPL_MODE_INIT) &&
            (Info2.Mode != IPL_MODE_OFF && Info2.Mode != IPL_MODE_INIT) )
        {
            if ( Id == IPL_ID_1 )
            {
                ProcId = IPL_ID_2;
            }
            else if ( Id == IPL_ID_2 )
            {
                ProcId = IPL_ID_1;
            }
        }
        IQS_Ctrl_ISR(ProcId);

        IPL_CBMsgProc(Id, IPL_CBMSG_IMEFMD, NULL);
    }
}

void IPL_IsrIPECB(IPL_PROC_ID Id, UINT32 IntStatus,UINT32 idx)
{
    if (IntStatus & IPE_FMD)
    {
    }
}

void IPL_IsrIFECB(IPL_PROC_ID Id, UINT32 IntStatus,UINT32 idx)
{
    if (IntStatus & IFE_FMD)
    {
    }
}


void IPL_IsrDCECB(IPL_PROC_ID Id, UINT32 IntStatus,UINT32 idx)
{
    if (IntStatus & DCE_FMD)
    {
        IQS_TrigEvent(IPL_UTI_CONV2_IQS_ID(Id));
    }
}
