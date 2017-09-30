/**
    Buf sample.

    @file       IPL_BufSample.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include "IPL_IMX117CQT_EVB_FF_Int.h"

CHAR* IPL_PrvBufNameTab[IPLBUF_PRV_MAX_CNT] =
{
    "SIE_RAW_H_1",
    "SIE_RAW_H_2",
    "SIE_RAW_H_3",
    "SIE_RAW_H_4",
    "SIE_RAW_H_5",
    "SIE_RAW_H_6",
    "SIE_RAW_H_7",
    "SIE_RAW_H_8",
    "SIE_RAW_H_9",
    "SIE_RAW_H_10",
    "",
    "SIE_RAW_CH0_1",
    "SIE_RAW_CH0_2",
    "SIE_RAW_CH0_3",
    "SIE_RAW_CH0_4",
    "SIE_RAW_CH0_5",
    "SIE_RAW_CH0_6",
    "SIE_RAW_CH0_7",
    "SIE_RAW_CH0_8",
    "SIE_RAW_CH0_9",
    "SIE_RAW_CH0_10",
    "",
    "SIE_Y_CH1_1",
    "SIE_Y_CH1_2",
    "SIE_Y_CH1_3",
    "SIE_Y_CH1_4",
    "SIE_Y_CH1_5",
    "SIE_Y_CH1_6",
    "SIE_Y_CH1_7",
    "SIE_Y_CH1_8",
    "SIE_Y_CH1_9",
    "SIE_Y_CH1_10",
    "",
    "SIE_UV_CH2_1",
    "SIE_UV_CH2_2",
    "SIE_UV_CH2_3",
    "SIE_UV_CH2_4",
    "SIE_UV_CH2_5",
    "SIE_UV_CH2_6",
    "SIE_UV_CH2_7",
    "SIE_UV_CH2_8",
    "SIE_UV_CH2_9",
    "SIE_UV_CH2_10",
    "",
    "SIE_RAW_CH3_1",
    "SIE_RAW_CH3_2",
    "",
    "SIE_VA_1",
    "SIE_VA_2",
    "",
    "SIE_ETH_1",
    "SIE_ETH_2",
    "SIE_ETH_3",
    "",
    "IPE_OUT_1",
    "IPE_OUT_2",
    "",
    "IPEIFE2_LCA_1",
    "IPEIFE2_LCA_2",
    "",
    "IMEP1_1",
    "IMEP1_2",
    "IMEP1_3",
    "IMEP1_4",
    "IMEP1_5",
    "IMEP1_6",
    "IMEP1_7",
    "IMEP1_8",
    "IMEP1_9",
    "IMEP1_10",
    "",
    "IMEP1_SPRT",

    "IMEP2_1",
    "IMEP2_2",
    "IMEP2_3",
    "IMEP2_4",
    "IMEP2_5",
    "IMEP2_6",
    "IMEP2_7",
    "IMEP2_8",
    "IMEP2_9",
    "IMEP2_10",
    "",
    "IMEP2_SPRT",

    "IMEP3_1",
    "IMEP3_2",
    "IMEP3_3",
    "IMEP3_4",
    "IMEP3_5",
    "IMEP3_6",
    "IMEP3_7",
    "IMEP3_8",
    "IMEP3_9",
    "IMEP3_10",
    "",
    "IMEP3_SPRT",

    "IMEP4_1",
    "IMEP4_2",
    "IMEP4_3",
    "IMEP4_4",
    "IMEP4_5",
    "IMEP4_6",
    "IMEP4_7",
    "IMEP4_8",
    "IMEP4_9",
    "IMEP4_10",
    "",
    "MCTFOUT_1",
    "MCTFOUT_2",
    "",
};


CHAR* IPL_CapBufNameTab[IPLBUF_CAP_MAX_CNT] =
{
    "DRE_DBGBUF"
};

#define IPPBUF_WORDALIGN(x) IPL_ALIGN_ROUNDUP(x, 4)
#define IPPBUF_4WORDALIGN(x) IPL_ALIGN_ROUNDUP(x, 16)

extern IPLBUF_RST IPL_BufAssignFlowA(IPLBUF_ENG2DRAMINFO *Data);
extern IPLBUF_RST IPL_BufAssignFlowC(IPLBUF_ENG2DRAMINFO *Data);
extern IPLBUF_RST IPL_BufAssignFlowD(IPLBUF_ENG2DRAMINFO *Data);
extern IPLBUF_RST IPL_BufAssignFlowCap(IPLBUF_ENG2DRAMINFO *Data);


FPIPPBUFASSIGNER IPL_BufAssignObj[IPLBUF_MODE_MAX_CNT] =
{
    IPL_BufAssignFlowA,
    IPL_BufAssignFlowC,
    IPL_BufAssignFlowD,
    IPL_BufAssignFlowCap,
};

IPLBUF_RST IPL_BufAssign(IPLBUF_MODE ModeIdx, IPLBUF_ENG2DRAMINFO *Data)
{
    IPLBUF_RST rtRst;

    rtRst.er_code = E_SYS;
    rtRst.BufSize = 0;
    if (ModeIdx >= IPLBUF_MODE_MAX_CNT)
    {
        DBG_ERR("ModeIdx = %d overflow\r\n", ModeIdx);
        return rtRst;

    }

    if (IPL_BufAssignObj[ModeIdx] != NULL)
    {
        rtRst = IPL_BufAssignObj[ModeIdx](Data);
    }
    return rtRst;
}

IPLBUF_RST IPL_BufAssignFlowA(IPLBUF_ENG2DRAMINFO *Data)
{
    IPLBUF_RST rtRst;
    UINT32 BufStAddr;
    UINT32 BufSize;
    UINT32 Size, HeadSize = 0, Addr, i;
    UINT32 Y_Size, UV_Size;
    UINT32 IPL2_StartAddr, SprtLSize = 0, SprtRSize = 0;
    UINT32 SeamH, SprtL, SprtR;

    IPL_BufGetResInfo(Data->Id, &BufStAddr, &BufSize);
    Addr = BufStAddr;

    if (Data->SIECh0En == ENABLE)
    {
        if (((Data->SIEOutCh0.BufNum > 0) && (Data->SIEOutCh0.BufNum <= (IPLBUF_SIERAW_CH0_MAX - IPLBUF_SIERAW_CH0_1))) &&
            ((IPLBUF_SIERAW_CH0_MAX - IPLBUF_SIERAW_CH0_1) == (IPLBUF_SIERAW_H_MAX - IPLBUF_SIERAW_H_1))
            )
        {
            Size = IPL_UTI_RAW_BUF_SIZE(Data->SIEOutCh0.Width, Data->SIEOutCh0.LineOfs, Data->SIEOutCh0.Height);
            Size = IPPBUF_WORDALIGN(Size);
            HeadSize = IPL_HeaderRawGetBufSize();
            for (i = 0; i < Data->SIEOutCh0.BufNum; i ++)
            {
                IPL_BufSet(Data->Id, (IPLBUF_SIERAW_H_1 + i), Addr, HeadSize, IPL_PrvBufNameTab[(IPLBUF_SIERAW_H_1 + i)]);
                Addr += HeadSize;
                IPL_BufSet(Data->Id, (IPLBUF_SIERAW_CH0_1 + i), Addr, Size, IPL_PrvBufNameTab[(IPLBUF_SIERAW_CH0_1 + i)]);
                Addr += Size;
            }
        }
        else
        {
            DBG_ERR("Id: %d, SIE Ch0 out Buffer number error (%d)\r\n", Data->Id, Data->SIEOutCh0.BufNum);
        }
    }

    if (Data->SIECh1En == ENABLE && Data->SIECh2En == ENABLE)
    {
        if ((Data->SIEOutCh1.BufNum == Data->SIEOutCh2.BufNum) && (Data->SIEOutCh1.BufNum > 0) && (Data->SIEOutCh1.BufNum <= (IPLBUF_SIEY_CH1_MAX - IPLBUF_SIEY_CH1_1)))
        {
			//#NT#2016/09/13#Jarkko Chang -begin
		    //#NT# add support ccir/ccir_interlace + ise scale with multi output
            //#NT#2016/09/02#Jarkko Chang -begin
            //#NT# add support ccir interlace with multi output
            //#NT#2016/08/03#Jarkko Chang -begin
            //#NT# modify control flow for support ccir scale
            //#NT#2016/03/15#Jarkko Chang -begin
            //#NT# add support CCIR_DEINTERLACE2 flow
            if (IPL_CtrlGetInfor(Data->Id, IPLCTRL_CFMODEEXT) == IPL_CFEXT_DEINTERLACE
                || IPL_CtrlGetInfor(Data->Id, IPLCTRL_CFMODEEXT) == IPL_CFEXT_DEINTERLACE2
                || IPL_CtrlGetInfor(Data->Id, IPLCTRL_CFMODEEXT) == IPL_CFEXT_DEINTERLACE3
                || IPL_CtrlGetInfor(Data->Id, IPLCTRL_CFMODEEXT) == IPL_CFEXT_IMESCALE
                || IPL_CtrlGetInfor(Data->Id, IPLCTRL_CFMODEEXT) == IPL_CFEXT_ISESCALE
                || IPL_CtrlGetInfor(Data->Id, IPLCTRL_CFMODEEXT) == IPL_CFEXT_ISESCALE_MULPATH
                || IPL_CtrlGetInfor(Data->Id, IPLCTRL_CFMODEEXT) == IPL_CFEXT_DEINTERLACE_ISESCALE_MULPATH)
                HeadSize = IPL_HeaderRawGetBufSize();
            //#NT#2016/03/15#Jarkko Chang -end
            //#NT#2016/08/03#Jarkko Chang -end
            //#NT#2016/09/02#Jarkko Chang -end
            //#NT#2016/09/13#Jarkko Chang -end
            Y_Size = IPL_UTI_RAW_BUF_SIZE(Data->SIEOutCh1.Width, Data->SIEOutCh1.LineOfs, Data->SIEOutCh1.Height);
            Y_Size = IPPBUF_WORDALIGN(Y_Size);

            UV_Size = IPL_UTI_RAW_BUF_SIZE(Data->SIEOutCh2.Width, Data->SIEOutCh2.LineOfs, Data->SIEOutCh2.Height);
            UV_Size = IPPBUF_WORDALIGN(UV_Size);
            for (i = 0; i < Data->SIEOutCh1.BufNum; i ++)
            {
				//#NT#2016/09/13#Jarkko Chang -begin
			    //#NT# add support ccir/ccir_interlace + ise scale with multi output
                //#NT#2016/09/02#Jarkko Chang -begin
                //#NT# add support ccir interlace with multi output
                //#NT#2016/08/03#Jarkko Chang -begin
                //#NT# modify control flow for support ccir scale
                //#NT#2016/03/15#Jarkko Chang -begin
                //#NT# add support CCIR_DEINTERLACE2 flow
                if (IPL_CtrlGetInfor(Data->Id, IPLCTRL_CFMODEEXT) == IPL_CFEXT_DEINTERLACE
                    || IPL_CtrlGetInfor(Data->Id, IPLCTRL_CFMODEEXT) == IPL_CFEXT_DEINTERLACE2
                    || IPL_CtrlGetInfor(Data->Id, IPLCTRL_CFMODEEXT) == IPL_CFEXT_DEINTERLACE3
                    || IPL_CtrlGetInfor(Data->Id, IPLCTRL_CFMODEEXT) == IPL_CFEXT_IMESCALE
                    || IPL_CtrlGetInfor(Data->Id, IPLCTRL_CFMODEEXT) == IPL_CFEXT_ISESCALE
    	            || IPL_CtrlGetInfor(Data->Id, IPLCTRL_CFMODEEXT) == IPL_CFEXT_ISESCALE_MULPATH
	                || IPL_CtrlGetInfor(Data->Id, IPLCTRL_CFMODEEXT) == IPL_CFEXT_DEINTERLACE_ISESCALE_MULPATH)
                {
                    IPL_BufSet(Data->Id, (IPLBUF_SIERAW_H_1 + i), Addr, HeadSize, IPL_PrvBufNameTab[(IPLBUF_SIERAW_H_1 + i)]);
                    Addr += HeadSize;
                }
                //#NT#2016/03/15#Jarkko Chang -end
                //#NT#2016/08/03#Jarkko Chang -end
                //#NT#2016/09/02#Jarkko Chang -end
				//#NT#2016/09/13#Jarkko Chang -end
				IPL_BufSet(Data->Id, (IPLBUF_SIEY_CH1_1 + i), Addr, Y_Size, IPL_PrvBufNameTab[(IPLBUF_SIEY_CH1_1 + i)]);
                Addr += Y_Size;

                IPL_BufSet(Data->Id, (IPLBUF_SIEUV_CH2_1 + i), Addr, UV_Size, IPL_PrvBufNameTab[(IPLBUF_SIEUV_CH2_1 + i)]);
                Addr += UV_Size;
            }

			//#NT#2016/09/13#Jarkko Chang -begin
	        //#NT# add smart mirror flow (DCE+IME), need one working buffer, size = SIE_Y_OUT + SIE_UV_OUT
			if (IPL_CtrlGetInfor(Data->Id, IPLCTRL_CFMODEEXT) == IPL_CFEXT_SMART_MIRROR)
            {
                Size = Y_Size + UV_Size;
				//#NT#2016/09/14#Jarkko Chang -begin
                //#NT# fixed buffer index error (IME_P1 -> SIE_CH0)
                IPL_BufSet(Data->Id, (IPLBUF_SIERAW_CH0_1), Addr, Size, IPL_PrvBufNameTab[IPLBUF_SIERAW_CH0_1]);
				//#NT#2016/09/14#Jarkko Chang -end
				Addr += Size;
            }
			//#NT#2016/09/13#Jarkko Chang -end
        }
        else
        {
            DBG_ERR("Id: %d, SIE Ch1/Ch2 out Buffer number error (CH1: %d, CH2: %d)\r\n", Data->Id, Data->SIEOutCh1.BufNum, Data->SIEOutCh2.BufNum);
        }
    }
    else if (Data->SIECh1En == ENABLE)
    {
        if ((Data->SIEOutCh1.BufNum > 0) && (Data->SIEOutCh1.BufNum <= (IPLBUF_SIEY_CH1_MAX - IPLBUF_SIEY_CH1_1)))
        {
            Size = IPL_UTI_RAW_BUF_SIZE(Data->SIEOutCh1.Width, Data->SIEOutCh1.LineOfs, Data->SIEOutCh1.Height);
            Size = IPPBUF_WORDALIGN(Size);

            for (i = 0; i < Data->SIEOutCh1.BufNum; i ++)
            {
                IPL_BufSet(Data->Id, (IPLBUF_SIEY_CH1_1 + i), Addr, Size, IPL_PrvBufNameTab[(IPLBUF_SIEY_CH1_1 + i)]);
                Addr += Size;
            }
        }
        else
        {
            DBG_ERR("Id: %d, SIE Ch1 out Buffer number error (%d)\r\n", Data->Id, Data->SIEOutCh2.BufNum);
        }
    }
    else if (Data->SIECh2En == ENABLE)
    {
        if ((Data->SIEOutCh2.BufNum > 0) && (Data->SIEOutCh2.BufNum <= (IPLBUF_SIEUV_CH2_MAX - IPLBUF_SIEUV_CH2_1)))
        {
            Size = IPL_UTI_RAW_BUF_SIZE(Data->SIEOutCh2.Width, Data->SIEOutCh2.LineOfs, Data->SIEOutCh2.Height);
            Size = IPPBUF_WORDALIGN(Size);
            for (i = 0; i < Data->SIEOutCh2.BufNum; i ++)
            {
                IPL_BufSet(Data->Id, (IPLBUF_SIEUV_CH2_1 + i), Addr, Size, IPL_PrvBufNameTab[(IPLBUF_SIEUV_CH2_1 + i)]);
                Addr += Size;
            }
        }
        else
        {
            DBG_ERR("Id: %d, SIE Ch2 out Buffer number error (%d)\r\n", Data->Id, Data->SIEOutCh3.BufNum);
        }
    }

    if (Data->SIECAEn == ENABLE)
    {
        if ((Data->SIECABufNum > 0) && (Data->SIECABufNum <= (IPLBUF_SIEY_CH1_MAX - IPLBUF_SIEY_CH1_1)))
        {
            Size = IPPBUF_WORDALIGN(Data->SIECABufSize);
            for (i = 0; i < Data->SIECABufNum; i ++)
            {
                IPL_BufSet(Data->Id, (IPLBUF_SIEY_CH1_1 + i), Addr, Size, IPL_PrvBufNameTab[(IPLBUF_SIEY_CH1_1 + i)]);
                Addr += Size;
            }
        }
        else
        {
            DBG_ERR("Id: %d, SIE CA out Buffer number error (%d)\r\n", Data->Id, Data->SIECABufNum);
        }
    }

    if (Data->SIELAEn == ENABLE)
    {
        if ((Data->SIELABufNum > 0) && (Data->SIELABufNum <= (IPLBUF_SIEUV_CH2_MAX - IPLBUF_SIEUV_CH2_1)))
        {
            Size = IPPBUF_WORDALIGN(Data->SIELABufSize);
            //#NT#2016/02/26#Jarkko Chang -begin
            //#NT# LA always assign one more buffer for flip+SHDR working buffer.
            for (i = 0; i < Data->SIELABufNum + 1; i ++)
            {
                IPL_BufSet(Data->Id, (IPLBUF_SIEUV_CH2_1 + i), Addr, Size, IPL_PrvBufNameTab[(IPLBUF_SIEUV_CH2_1 + i)]);
                Addr += Size;
            }
            //#NT#2016/02/26#Jarkko Chang -end
        }
        else
        {
            DBG_ERR("Id: %d, SIE LA out Buffer number error (%d)\r\n", Data->Id, Data->SIELABufNum);
        }
    }


    if (Data->SIECh3En == ENABLE)
    {
        if ((Data->SIEOutCh3.BufNum > 0) && (Data->SIEOutCh3.BufNum <= (IPLBUF_SIERAW_CH3_MAX - IPLBUF_SIERAW_CH3_1)))
        {
            Size = IPL_UTI_RAW_BUF_SIZE(Data->SIEOutCh3.Width, Data->SIEOutCh3.LineOfs, Data->SIEOutCh3.Height);
            Size = IPPBUF_WORDALIGN(Size);
            for (i = 0; i < Data->SIEOutCh3.BufNum; i ++)
            {
                IPL_BufSet(Data->Id, (IPLBUF_SIERAW_CH3_1 + i), Addr, Size, IPL_PrvBufNameTab[(IPLBUF_SIERAW_CH3_1 + i)]);
                Addr += Size;
            }
        }
        else
        {
            DBG_ERR("Id: %d, SIE Ch3 out Buffer number error (%d)\r\n", Data->Id, Data->SIEOutCh3.BufNum);
        }
    }
    else if (Data->SIEVAEn == ENABLE)
    {
        if ((Data->SIEVABufNum > 0) && (Data->SIEVABufNum <= (IPLBUF_SIEVA_MAX - IPLBUF_SIEVA_1)))
        {
            Size = IPPBUF_WORDALIGN(Data->SIEVABufSize);
            for (i = 0; i < Data->SIEVABufNum; i ++)
            {
                IPL_BufSet(Data->Id, (IPLBUF_SIEVA_1 + i), Addr, Size, IPL_PrvBufNameTab[(IPLBUF_SIEVA_1 + i)]);
                Addr += Size;
            }
        }
        else
        {
            DBG_ERR("Id: %d, SIE VA out Buffer number error (%d)\r\n", Data->Id, Data->SIEVABufNum);
        }
    }

    if (Data->SIEEthEn == ENABLE)
    {
        Size = IPPBUF_WORDALIGN(Data->SIEEthSize);
        IPL_BufSet(Data->Id, IPLBUF_SIEETH_1, Addr, Size, IPL_PrvBufNameTab[IPLBUF_SIEETH_1]);
        Addr += Size;
        IPL_BufSet(Data->Id, IPLBUF_SIEETH_2, Addr, Size, IPL_PrvBufNameTab[IPLBUF_SIEETH_2]);
        Addr += Size;
        IPL_BufSet(Data->Id, IPLBUF_SIEETH_3, Addr, Size, IPL_PrvBufNameTab[IPLBUF_SIEETH_3]);
        Addr += Size;
    }

    if (Data->IPEOutEn == ENABLE)
    {
        if ((Data->IPEOut.BufNum > 0) && (Data->IPEOut.BufNum <= (IPLBUF_IPEOUT_MAX - IPLBUF_IPEOUT_1)))
        {
            Size = IPL_YUV_SIZE(Data->IPEOut.Fmt, Data->IPEOut.Width, Data->IPEOut.Height);
            for (i = 0 ; i < Data->IPEOut.BufNum ; i ++)
            {
                IPL_BufSet(Data->Id, (IPLBUF_IPEOUT_1 + i), Addr, Size, IPL_PrvBufNameTab[(IPLBUF_IPEOUT_1 + i)]);
                Addr += Size;
            }
        }
        else
        {
            DBG_ERR("Id: %d, IFE2 LCA out Buffer number error (%d)\r\n", Data->Id, Data->IPEOut.BufNum);
        }
    }

    if (Data->IFE2LCAEn == ENABLE)
    {
        if ((Data->IFE2LCABufNum > 0) && (Data->IFE2LCABufNum <= (IPLBUF_IFE2LCAOUT_MAX - IPLBUF_IFE2LCAOUT_1)))
        {
            Size = IPL_YUV_SIZE(Data->IPEOut.Fmt, Data->IPEOut.Width, Data->IPEOut.Height);
            for (i = 0; i < Data->IFE2LCABufNum; i ++)
            {
                IPL_BufSet(Data->Id, (IPLBUF_IFE2LCAOUT_1 + i), Addr, Size, IPL_PrvBufNameTab[(IPLBUF_IFE2LCAOUT_1 + i)]);
                Addr += Size;
            }
        }
        else
        {
            DBG_ERR("Id: %d, IFE2 LCA out Buffer number error (%d)\r\n", Data->Id, Data->IFE2LCABufNum);
        }
    }

    if (Data->IMEPath1En == ENABLE)
    {
        if ((Data->IMEPath1.BufNum > 0) && (Data->IMEPath1.BufNum <= (IPLBUF_IMEP1_MAX - IPLBUF_IMEP1_1)))
        {
            if (IPL_CtrlGetInfor(Data->Id, IPLCTRL_EXT_FUNC_EN) == ICF_FUNC_STITCH)
            {

                SeamH = IPL_ALIGN_ROUNDUP(Stitch_GetStitchAreaWidthInPixel(Data->IMEPath1.Width),4);
                SprtL = IPL_ALIGN_ROUNDUP(Stitch_GetLeftImgCutPointInPixel(Data->IMEPath1.Width),4);
                SprtR = IPL_ALIGN_ROUNDUP(Stitch_GetRightImgCutPointInPixel(Data->IMEPath1.Width),4);
                //#NT#2016/07/20#Jarkko Chang -begin
                //#NT# ime output buffer need 4 word align
                SprtLSize = IPL_YUV_SIZE(Data->IMEPath1.Fmt, SprtL, Data->IMEPath1.Height);
                SprtLSize = IPPBUF_4WORDALIGN(SprtLSize);
                SprtRSize = IPL_YUV_SIZE(Data->IMEPath1.Fmt, SprtR, Data->IMEPath1.Height);
                SprtRSize = IPPBUF_4WORDALIGN(SprtRSize);

                Size = IPL_YUV_SIZE(Data->IMEPath1.Fmt, Data->IMEPath1.LineOfs, Data->IMEPath1.Height);
                Size *= 2;
                Size += IPL_YUV_SIZE(Data->IMEPath1.Fmt, SeamH, Data->IMEPath1.Height);
                Size = IPPBUF_4WORDALIGN(Size);
                //#NT#2016/07/20#Jarkko Chang -end
                if (Data->Id == IPL_ID_1)
                {
                    IPL_BufSet(Data->Id, IPLBUF_IMEP1_SPRT, Addr, SprtLSize, IPL_PrvBufNameTab[IPLBUF_IMEP1_SPRT]);
                    Addr += (SprtLSize + SprtRSize);

                    for (i = 0; i < Data->IMEPath1.BufNum; i ++)
                    {
                        IPL_BufSet(Data->Id, (IPLBUF_IMEP1_1 + i), Addr, Size, IPL_PrvBufNameTab[(IPLBUF_IMEP1_1 + i)]);
                        Addr += Size;
                    }
                }
                else if (Data->Id == IPL_ID_2)
                {
                    IPL_BufGet(IPL_ID_1, (IPLBUF_IMEP1_SPRT), &IPL2_StartAddr, &Y_Size);
                    IPL2_StartAddr += SprtLSize;
                    IPL_BufSet(Data->Id, (IPLBUF_IMEP1_SPRT|IPL_BUF_CHK_BOUNDARY_DISABLE), IPL2_StartAddr, SprtRSize, IPL_PrvBufNameTab[IPLBUF_IMEP1_SPRT]);

                    for (i = 0; i < Data->IMEPath1.BufNum; i ++)
                    {
                        IPL_BufGet(IPL_ID_1, (IPLBUF_IMEP1_1 + i), &IPL2_StartAddr, &Size);
                        IPL2_StartAddr += (Data->IMEPath1.Width + SeamH - SprtL);
                        IPL_BufSet(Data->Id, ((IPLBUF_IMEP1_1 + i) | IPL_BUF_CHK_BOUNDARY_DISABLE), IPL2_StartAddr, Size, IPL_PrvBufNameTab[(IPLBUF_IMEP1_1 + i)]);
                    }
                }
            }
            else
            {
                Size = IPL_YUV_SIZE(Data->IMEPath1.Fmt, Data->IMEPath1.LineOfs, Data->IMEPath1.Height);
                //#NT#2016/07/20#Jarkko Chang -begin
                //#NT# ime output buffer need 4 word align
                Size = IPPBUF_4WORDALIGN(Size);
                //#NT#2016/07/20#Jarkko Chang -end
                for (i = 0; i < Data->IMEPath1.BufNum; i ++)
                {
					//#NT#2017/02/06#Jarkko Chang -begin
                    //#NT# add support side by side left/right, up/down case
					if(IPL_CtrlGetInfor(Data->Id, IPLCTRL_SBS_TYPE) == SEL_SBS_LR || IPL_CtrlGetInfor(Data->Id, IPLCTRL_SBS_TYPE) == SEL_SBS_UD)
					{
	                    if (Data->Id == IPL_ID_1)
	                    {
	                        IPL_BufSet(Data->Id, (IPLBUF_IMEP1_1 + i), Addr, Size*2, IPL_PrvBufNameTab[(IPLBUF_IMEP1_1 + i)]);
	                        Addr += (Size*2);
	                    }
	                    else if (Data->Id == IPL_ID_2)
	                    {
	                        UINT32 tmpBufAddr, tmpBufSize;
	                        IPL_BufGet(IPL_ID_1, (IPLBUF_IMEP1_1 + i), &tmpBufAddr, &tmpBufSize);
							if(IPL_CtrlGetInfor(Data->Id, IPLCTRL_SBS_TYPE) == SEL_SBS_LR)
							{
		                        IPL_BufSet(Data->Id, (IPL_BUF_CHK_BOUNDARY_DISABLE|(IPLBUF_IMEP1_1 + i)), (tmpBufAddr + Data->IMEPath1.Width), tmpBufSize - Data->IMEPath1.Width, IPL_PrvBufNameTab[(IPLBUF_IMEP1_1 + i)]);
							}
							else if(IPL_CtrlGetInfor(Data->Id, IPLCTRL_SBS_TYPE) == SEL_SBS_UD)
							{
		                        IPL_BufSet(Data->Id, (IPL_BUF_CHK_BOUNDARY_DISABLE|(IPLBUF_IMEP1_1 + i)), (tmpBufAddr + (Data->IMEPath1.LineOfs*Data->IMEPath1.Height)), Size - (Data->IMEPath1.LineOfs*Data->IMEPath1.Height), IPL_PrvBufNameTab[(IPLBUF_IMEP1_1 + i)]);
							}
	                    }
	                    else
	                    {
	                        DBG_ERR("not support\r\n");
	                    }
					}
					else
					{
	                    IPL_BufSet(Data->Id, (IPLBUF_IMEP1_1 + i), Addr, Size, IPL_PrvBufNameTab[(IPLBUF_IMEP1_1 + i)]);

						if (IPL_CtrlGetInfor(Data->Id, IPLCTRL_VR360_EN) == ENABLE){
		                    memset((UINT32 *)Addr, 0x80, Size);
	    	                memset((UINT32 *)Addr, 0x0, Data->IMEPath1.LineOfs*Data->IMEPath1.Height);
						}
	                    Addr += Size;
					}
	                //#NT#2017/02/06#Jarkko Chang -end
                }

				//#NT#2016/09/13#Jarkko Chang -begin
		        //#NT# add support ccir/ccir_interlace + ise scale with multi output
                //#NT#2016/08/03#Jarkko Chang -begin
                //#NT# modify control flow for support ccir scale
                //#NT#2016/03/15#Jarkko Chang -begin
                //#NT# add support CCIR_DEINTERLACE2 flow, ISE Bug, need assign one more buffer
                if (IPL_CtrlGetInfor(Data->Id, IPLCTRL_CFMODEEXT) == IPL_CFEXT_DEINTERLACE2
					|| IPL_CtrlGetInfor(Data->Id, IPLCTRL_CFMODEEXT) == IPL_CFEXT_ISESCALE
					|| IPL_CtrlGetInfor(Data->Id, IPLCTRL_CFMODEEXT) == IPL_CFEXT_ISESCALE_MULPATH
					|| IPL_CtrlGetInfor(Data->Id, IPLCTRL_CFMODEEXT) == IPL_CFEXT_DEINTERLACE_ISESCALE_MULPATH)
                {
                    IPL_BufSet(Data->Id, (IPLBUF_IMEP1_1 + i), Addr, Size, IPL_PrvBufNameTab[(IPLBUF_IMEP1_1 + i)]);
                    Addr += Size;
                }
                //#NT#2016/07/20#Jarkko Chang -end
                //#NT#2016/03/15#Jarkko Chang -end
                //#NT#2016/08/03#Jarkko Chang -end
                //#NT#2016/07/20#Jarkko Chang -begin
                //#NT# vr360 need another 1 working buffer
				//#NT#2016/09/13#Jarkko Chang -end
				else if (IPL_CtrlGetInfor(Data->Id, IPLCTRL_VR360_EN) == ENABLE)
                {
                    Size /= 2;
                    IPL_BufSet(Data->Id, (IPLBUF_IMEP1_1 + i), Addr, Size, IPL_PrvBufNameTab[(IPLBUF_IMEP1_1 + i)]);
                    Addr += Size;
                }
            }
        }
        else
        {
            DBG_ERR("Id: %d, IME P1 out Buffer number error (%d)\r\n", Data->Id, Data->IMEPath1.BufNum);
        }
    }

    if (Data->IMEPath2En == ENABLE)
    {
        if ((Data->IMEPath2.BufNum > 0) && (Data->IMEPath2.BufNum <= (IPLBUF_IMEP2_MAX - IPLBUF_IMEP2_1)))
        {
            if (IPL_CtrlGetInfor(Data->Id, IPLCTRL_EXT_FUNC_EN) == ICF_FUNC_STITCH)
            {
                SeamH = IPL_ALIGN_ROUNDUP(Stitch_GetStitchAreaWidthInPixel(Data->IMEPath2.Width),4);
                SprtL = IPL_ALIGN_ROUNDUP(Stitch_GetLeftImgCutPointInPixel(Data->IMEPath2.Width),4);
                SprtR = IPL_ALIGN_ROUNDUP(Stitch_GetRightImgCutPointInPixel(Data->IMEPath2.Width),4);
                //#NT#2016/07/20#Jarkko Chang -begin
                //#NT# ime output buffer need 4 word align
                SprtLSize = IPL_YUV_SIZE(Data->IMEPath2.Fmt, SprtL, Data->IMEPath2.Height);
                SprtLSize = IPPBUF_4WORDALIGN(SprtLSize);
                SprtRSize = IPL_YUV_SIZE(Data->IMEPath2.Fmt, SprtR, Data->IMEPath2.Height);
                SprtRSize = IPPBUF_4WORDALIGN(SprtRSize);

                Size = IPL_YUV_SIZE(Data->IMEPath2.Fmt, Data->IMEPath2.LineOfs, Data->IMEPath2.Height);
                Size *= 2;
                Size += IPL_YUV_SIZE(Data->IMEPath2.Fmt, SeamH, Data->IMEPath2.Height);
                Size = IPPBUF_4WORDALIGN(Size);
                //#NT#2016/07/20#Jarkko Chang -end
                if (Data->Id == IPL_ID_1)
                {
                    IPL_BufSet(Data->Id, IPLBUF_IMEP2_SPRT, Addr, SprtLSize, IPL_PrvBufNameTab[IPLBUF_IMEP2_SPRT]);
                    Addr += (SprtLSize + SprtRSize);

                    for (i = 0; i < Data->IMEPath2.BufNum; i ++)
                    {
                        IPL_BufSet(Data->Id, (IPLBUF_IMEP2_1 + i), Addr, Size, IPL_PrvBufNameTab[(IPLBUF_IMEP2_1 + i)]);
                        Addr += Size;
                    }
                }
                else if (Data->Id == IPL_ID_2)
                {
                    IPL_BufGet(IPL_ID_1, (IPLBUF_IMEP2_SPRT), &IPL2_StartAddr, &Y_Size);
                    IPL2_StartAddr += SprtLSize;
                    IPL_BufSet(Data->Id, (IPLBUF_IMEP2_SPRT|IPL_BUF_CHK_BOUNDARY_DISABLE), IPL2_StartAddr, SprtRSize, IPL_PrvBufNameTab[IPLBUF_IMEP2_SPRT]);

                    for (i = 0; i < Data->IMEPath2.BufNum; i ++)
                    {
                        IPL_BufGet(IPL_ID_1, (IPLBUF_IMEP2_1 + i), &IPL2_StartAddr, &Size);
                        IPL2_StartAddr += (Data->IMEPath2.Width + SeamH - SprtL);
                        IPL_BufSet(Data->Id, ((IPLBUF_IMEP2_1 + i) | IPL_BUF_CHK_BOUNDARY_DISABLE), IPL2_StartAddr, Size, IPL_PrvBufNameTab[(IPLBUF_IMEP2_1 + i)]);
                    }
                }
            }
            else
            {
                Size = IPL_YUV_SIZE(Data->IMEPath2.Fmt, Data->IMEPath2.LineOfs, Data->IMEPath2.Height);
                //#NT#2016/07/20#Jarkko Chang -begin
                //#NT# ime output buffer need 4 word align
                Size = IPPBUF_4WORDALIGN(Size);
                //#NT#2016/07/20#Jarkko Chang -end
                for (i = 0; i < Data->IMEPath2.BufNum; i ++)
                {
					//#NT#2017/02/06#Jarkko Chang -begin
                    //#NT# add support side by side left/right, up/down case
					if(IPL_CtrlGetInfor(Data->Id, IPLCTRL_SBS_TYPE) == SEL_SBS_LR || IPL_CtrlGetInfor(Data->Id, IPLCTRL_SBS_TYPE) == SEL_SBS_UD)
					{
	                    if (Data->Id == IPL_ID_1)
	                    {
	                        IPL_BufSet(Data->Id, (IPLBUF_IMEP2_1 + i), Addr, Size*2, IPL_PrvBufNameTab[(IPLBUF_IMEP2_1 + i)]);
	                        Addr += (Size*2);
	                    }
	                    else if (Data->Id == IPL_ID_2)
	                    {
	                        UINT32 tmpBufAddr, tmpBufSize;
	                        IPL_BufGet(IPL_ID_1, (IPLBUF_IMEP2_1 + i), &tmpBufAddr, &tmpBufSize);
							if(IPL_CtrlGetInfor(Data->Id, IPLCTRL_SBS_TYPE) == SEL_SBS_LR)
							{
		                        IPL_BufSet(Data->Id, (IPL_BUF_CHK_BOUNDARY_DISABLE|(IPLBUF_IMEP2_1 + i)), (tmpBufAddr + Data->IMEPath2.Width), tmpBufSize - Data->IMEPath2.Width, IPL_PrvBufNameTab[(IPLBUF_IMEP2_1 + i)]);
							}
							else if(IPL_CtrlGetInfor(Data->Id, IPLCTRL_SBS_TYPE) == SEL_SBS_UD)
							{
		                        IPL_BufSet(Data->Id, (IPL_BUF_CHK_BOUNDARY_DISABLE|(IPLBUF_IMEP2_1 + i)), (tmpBufAddr + (Data->IMEPath2.LineOfs*Data->IMEPath2.Height)), Size - (Data->IMEPath2.LineOfs*Data->IMEPath2.Height), IPL_PrvBufNameTab[(IPLBUF_IMEP2_1 + i)]);
							}
	                    }
	                    else
	                    {
	                        DBG_ERR("not support\r\n");
	                    }
					}
					else
					{
	                    IPL_BufSet(Data->Id, (IPLBUF_IMEP2_1 + i), Addr, Size, IPL_PrvBufNameTab[(IPLBUF_IMEP2_1 + i)]);
						if (IPL_CtrlGetInfor(Data->Id, IPLCTRL_VR360_EN) == ENABLE){
		                    memset((UINT32 *)Addr, 0x80, Size);
	    	                memset((UINT32 *)Addr, 0x0, Data->IMEPath2.LineOfs*Data->IMEPath2.Height);
						}
	                    Addr += Size;
					}
					//#NT#2017/02/06#Jarkko Chang -end
                }
                //#NT#2016/07/20#Jarkko Chang -begin
                //#NT# vr360 need another 1 working buffer
                if (IPL_CtrlGetInfor(Data->Id, IPLCTRL_VR360_EN) == ENABLE)
                {
                    Size /= 2;
                    IPL_BufSet(Data->Id, (IPLBUF_IMEP2_1 + i), Addr, Size, IPL_PrvBufNameTab[(IPLBUF_IMEP2_1 + i)]);
                    Addr += Size;
                }
                //#NT#2016/07/20#Jarkko Chang -end

            }
        }
        else
        {
            DBG_ERR("Id: %d, IME P2 out Buffer number error (%d)\r\n", Data->Id, Data->IMEPath2.BufNum);
        }
    }

    if (Data->IMEPath3En == ENABLE)
    {
        if ((Data->IMEPath3.BufNum > 0) && (Data->IMEPath3.BufNum <= (IPLBUF_IMEP3_MAX - IPLBUF_IMEP3_1)))
        {
            if (IPL_CtrlGetInfor(Data->Id, IPLCTRL_EXT_FUNC_EN) == ICF_FUNC_STITCH)
            {
                SeamH = IPL_ALIGN_ROUNDUP(Stitch_GetStitchAreaWidthInPixel(Data->IMEPath3.Width),4);
                SprtL = IPL_ALIGN_ROUNDUP(Stitch_GetLeftImgCutPointInPixel(Data->IMEPath3.Width),4);
                SprtR = IPL_ALIGN_ROUNDUP(Stitch_GetRightImgCutPointInPixel(Data->IMEPath3.Width),4);

                SprtLSize = IPL_YUV_SIZE(Data->IMEPath3.Fmt, SprtL, Data->IMEPath3.Height);
                //#NT#2016/07/20#Jarkko Chang -begin
                //#NT# ime output buffer need 4 word align
                SprtLSize = IPPBUF_4WORDALIGN(SprtLSize);
                SprtRSize = IPL_YUV_SIZE(Data->IMEPath3.Fmt, SprtR, Data->IMEPath3.Height);
                SprtRSize = IPPBUF_4WORDALIGN(SprtRSize);

                Size = IPL_YUV_SIZE(Data->IMEPath3.Fmt, Data->IMEPath3.LineOfs, Data->IMEPath3.Height);
                Size *= 2;
                Size += IPL_YUV_SIZE(Data->IMEPath3.Fmt, SeamH, Data->IMEPath3.Height);
                Size = IPPBUF_4WORDALIGN(Size);
                //#NT#2016/07/20#Jarkko Chang -end
                if (Data->Id == IPL_ID_1)
                {
                    IPL_BufSet(Data->Id, IPLBUF_IMEP3_SPRT, Addr, SprtLSize, IPL_PrvBufNameTab[IPLBUF_IMEP3_SPRT]);
                    Addr += (SprtLSize + SprtRSize);

                    for (i = 0; i < Data->IMEPath3.BufNum; i ++)
                    {
                        IPL_BufSet(Data->Id, (IPLBUF_IMEP3_1 + i), Addr, Size, IPL_PrvBufNameTab[(IPLBUF_IMEP3_1 + i)]);
                        Addr += Size;
                    }
                }
                else if (Data->Id == IPL_ID_2)
                {
                    IPL_BufGet(IPL_ID_1, (IPLBUF_IMEP3_SPRT), &IPL2_StartAddr, &Y_Size);
                    IPL2_StartAddr += SprtLSize;
                    IPL_BufSet(Data->Id, (IPLBUF_IMEP3_SPRT|IPL_BUF_CHK_BOUNDARY_DISABLE), IPL2_StartAddr, SprtRSize, IPL_PrvBufNameTab[IPLBUF_IMEP3_SPRT]);

                    for (i = 0; i < Data->IMEPath3.BufNum; i ++)
                    {
                        IPL_BufGet(IPL_ID_1, (IPLBUF_IMEP3_1 + i), &IPL2_StartAddr, &Size);
                        IPL2_StartAddr += (Data->IMEPath3.Width + SeamH - SprtL);
                        IPL_BufSet(Data->Id, ((IPLBUF_IMEP3_1 + i) | IPL_BUF_CHK_BOUNDARY_DISABLE), IPL2_StartAddr, Size, IPL_PrvBufNameTab[(IPLBUF_IMEP3_1 + i)]);
                    }
                }
            }
            else
            {
                Size = IPL_YUV_SIZE(Data->IMEPath3.Fmt, Data->IMEPath3.LineOfs, Data->IMEPath3.Height);
                //#NT#2016/07/20#Jarkko Chang -begin
                //#NT# ime output buffer need 4 word align
                Size = IPPBUF_4WORDALIGN(Size);
                //#NT#2016/07/20#Jarkko Chang -end
                for (i = 0; i < Data->IMEPath3.BufNum; i ++)
                {
					//#NT#2017/02/06#Jarkko Chang -begin
                    //#NT# add support side by side left/right, up/down case
					if(IPL_CtrlGetInfor(Data->Id, IPLCTRL_SBS_TYPE) == SEL_SBS_LR || IPL_CtrlGetInfor(Data->Id, IPLCTRL_SBS_TYPE) == SEL_SBS_UD)
					{
	                    if (Data->Id == IPL_ID_1)
	                    {
	                        IPL_BufSet(Data->Id, (IPLBUF_IMEP3_1 + i), Addr, Size*2, IPL_PrvBufNameTab[(IPLBUF_IMEP3_1 + i)]);
	                        Addr += (Size*2);
	                    }
	                    else if (Data->Id == IPL_ID_2)
	                    {
	                        UINT32 tmpBufAddr, tmpBufSize;
	                        IPL_BufGet(IPL_ID_1, (IPLBUF_IMEP3_1 + i), &tmpBufAddr, &tmpBufSize);
							if(IPL_CtrlGetInfor(Data->Id, IPLCTRL_SBS_TYPE) == SEL_SBS_LR)
							{
		                        IPL_BufSet(Data->Id, (IPL_BUF_CHK_BOUNDARY_DISABLE|(IPLBUF_IMEP3_1 + i)), (tmpBufAddr + Data->IMEPath3.Width), tmpBufSize - Data->IMEPath3.Width, IPL_PrvBufNameTab[(IPLBUF_IMEP3_1 + i)]);
							}
							else if(IPL_CtrlGetInfor(Data->Id, IPLCTRL_SBS_TYPE) == SEL_SBS_UD)
							{
		                        IPL_BufSet(Data->Id, (IPL_BUF_CHK_BOUNDARY_DISABLE|(IPLBUF_IMEP3_1 + i)), (tmpBufAddr + (Data->IMEPath3.LineOfs*Data->IMEPath3.Height)), Size - (Data->IMEPath3.LineOfs*Data->IMEPath3.Height), IPL_PrvBufNameTab[(IPLBUF_IMEP3_1 + i)]);
							}
	                    }
	                    else
	                    {
	                        DBG_ERR("not support\r\n");
	                    }
					}
					else
					{
	                    IPL_BufSet(Data->Id, (IPLBUF_IMEP3_1 + i), Addr, Size, IPL_PrvBufNameTab[(IPLBUF_IMEP3_1 + i)]);
						if (IPL_CtrlGetInfor(Data->Id, IPLCTRL_VR360_EN) == ENABLE){
		                    memset((UINT32 *)Addr, 0x80, Size);
	    	                memset((UINT32 *)Addr, 0x0, Data->IMEPath3.LineOfs*Data->IMEPath3.Height);
						}
	                    Addr += Size;
					}
					//#NT#2017/02/06#Jarkko Chang -end
                }
                //#NT#2016/07/20#Jarkko Chang -begin
                //#NT# vr360 need another 1 working buffer
                if (IPL_CtrlGetInfor(Data->Id, IPLCTRL_VR360_EN) == ENABLE)
                {
                    Size /= 2;
                    IPL_BufSet(Data->Id, (IPLBUF_IMEP3_1 + i), Addr, Size, IPL_PrvBufNameTab[(IPLBUF_IMEP3_1 + i)]);
                    Addr += Size;
                }
                //#NT#2016/07/20#Jarkko Chang -end
            }
        }
        else
        {
            DBG_ERR("Id: %d, IME P3 out Buffer number error (%d)\r\n", Data->Id, Data->IMEPath3.BufNum);
        }
    }

    if (Data->IMEPath4En == ENABLE)
    {
        if ((Data->IMEPath4.BufNum > 0) && (Data->IMEPath4.BufNum <= (IPLBUF_IMEP4_MAX - IPLBUF_IMEP4_1)))
        {
            // arrange Y, Histo, EdgeMap buffer
            Size = IPL_YUV_SIZE(IPL_IMG_Y, Data->IMEPath4.LineOfs, Data->IMEPath4.Height);
            Size = IPPBUF_WORDALIGN(Size);
            IPL_CtrlSetInfor(Data->Id, IPLCTRL_IME_P4_EDGEMAP_BUFOFS, Size);
            if (IPL_CtrlGetInfor(Data->Id, IPLCTRL_IME_P4_EXTOUT_EN) == ENABLE)
            {
                Size +=  ADAS_getFcwsEdgeMapSz();
                Size = IPPBUF_WORDALIGN(Size);
                IPL_CtrlSetInfor(Data->Id, IPLCTRL_IME_P4_HISTO_BUFOFS, Size);
                Size +=  ADAS_getHistSz();
            }
            Size = IPPBUF_WORDALIGN(Size);

            for (i = 0; i < Data->IMEPath4.BufNum; i ++)
            {
                IPL_BufSet(Data->Id, (IPLBUF_IMEP4_1 + i), Addr, Size, IPL_PrvBufNameTab[(IPLBUF_IMEP4_1 + i)]);
                Addr += Size;
            }
        }
        else
        {
            DBG_ERR("Id: %d, IME P4 out Buffer number error (%d)\r\n", Data->Id, Data->IMEPath4.BufNum);
        }
    }

#if IQUC_ENABLE
    if (Data->IMEPath1En == ENABLE)
    {
        DBG_IND("IQUCBUF_MCTFOUT %d %d\r\n", Data->IMEPath1.LineOfs, Data->IMEPath1.Height);
        Size = IPL_YUV_SIZE(Data->IMEPath1.Fmt, Data->IMEPath1.LineOfs, Data->IMEPath1.Height);
        Size = IPPBUF_WORDALIGN(Size) * 2;
        IPL_BufSet(Data->Id, IQUCBUF_MCTFOUT_1, Addr, Size, IPL_PrvBufNameTab[IQUCBUF_MCTFOUT_1]);
        Addr += Size;
    }
#endif
    rtRst.er_code = E_OK;
    rtRst.BufSize = (Addr - BufStAddr);
    if (rtRst.BufSize > BufSize)
    {
        rtRst.er_code = E_NOMEM;
        DBG_ERR("Id: %d, Buf. not enough 0x%x<0x%x\r\n", Data->Id, BufSize, rtRst.BufSize);
    }
    return rtRst;
}

IPLBUF_RST IPL_BufAssignFlowC(IPLBUF_ENG2DRAMINFO *Data)
{
    return IPL_BufAssignFlowA(Data);
}

IPLBUF_RST IPL_BufAssignFlowD(IPLBUF_ENG2DRAMINFO *Data)
{
    return IPL_BufAssignFlowA(Data);
}

IPLBUF_RST IPL_BufAssignFlowCap(IPLBUF_ENG2DRAMINFO *Data)
{
    IPLBUF_RST rtRst;
    UINT32 BufStAddr=0, BufSize=0, Addr=0, Size=0;

    IPL_BufGetResInfo(Data->Id, &BufStAddr, &BufSize);
    Addr = BufStAddr;
    Size = 0;
#if IQS_DUMP_ENGREG
    Size = IQS_DRE_REG_SIZE;
    Size = IPPBUF_WORDALIGN(Size);
    IPL_BufSet(Data->Id, (IPLBUF_DRE_DBGBUF), Addr, Size, IPL_CapBufNameTab[(IPLBUF_DRE_DBGBUF)]);
    Addr += Size;
#endif

    rtRst.er_code = E_OK;
    rtRst.BufSize = (Addr - BufStAddr);
    if (rtRst.BufSize > BufSize)
    {
        rtRst.er_code = E_NOMEM;
        DBG_ERR("Id: %d, Buf. not enough 0x%x<0x%x\r\n", Data->Id, BufSize, rtRst.BufSize);
    }
    return rtRst;
}
