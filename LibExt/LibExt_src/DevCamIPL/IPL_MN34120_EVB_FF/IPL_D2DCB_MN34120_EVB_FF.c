/*
    IPL D2D Callback Function

    This file is the API of the D2D callback function.

    @file       IPL_D2DCB_MN34120_EVB_FF.c
    @ingroup    mISYSAlg
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#include "IPL_MN34120_EVB_FF_Int.h"
#include "rsc_common.h"

//note: do not delete
//avoid compiler optimize "weak function"
//WEAK_DUMMY_CALL(D2D, CTRL2)
_2DLUT_PARAM GET_Dce2DLut = {0};
static UINT32 IFE2_PPROC_D2D_CB(IPL_PROC_ID id, IFE2_SUBFUNC* subF, IFE2_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{

    UINT32 Cnt = 1;
    return Cnt;
}

static UINT32 IFE2_CAP_D2D_CB(IPL_PROC_ID id, IFE2_SUBFUNC* subF, IFE2_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    UINT32 Cnt = 5;
    static UINT8 ife2LoopCnt=0;
    IPL_HAL_GROUP group;
    ENG_SUBFUNC Info = {0};
    Info.pIfe2_sub = subF;
    group.ife2Update = *Update;

    DBG_IND("EventID = 0x%.8x\r\n", eventID);
    switch(eventID&(~IPL_FLOW_RESERVE))
    {
        case (IPL_FLOW_IFE2D2D_LCA):
            Cnt = IQS_GetIFE2LcaTime(id, IQCB_D2D_IFE2_PRI);
            if ( ife2LoopCnt <= 0 )
                ife2LoopCnt = Cnt;
            ife2LoopCnt--;
            IQCB_SettingFlowFunc(id, IQCB_D2D_IFE2_PRI, &Info, &group);
            break;
        default:
            DBG_ERR("event fail\r\n");
            break;
    }

    *Update = group.ife2Update;

#if IQS_DUMP_ENGREG
    *fpRegD = (fpRegDump)IQS_DumpEngRegister;
#endif
    return Cnt;
}

UINT32 IFE2_D2D_CB(IPL_PROC_ID id, IFE2_SUBFUNC* subF, IFE2_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    UINT32 Cnt = 1;
    DBG_IND("EventID = 0x%.8x\r\n", eventID);

    if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_CAPD2D_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_CAPD2D_MAX + 1)))
    {
        Cnt = IFE2_CAP_D2D_CB(id, subF, Update, eventID,fpRegD);
    }
    else if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_PPROC_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_PPROC_MAX + 1)))
    {
        Cnt = IFE2_PPROC_D2D_CB(id, subF, Update, eventID,fpRegD);
    }
    else
    {
        DBG_ERR("event fail\r\n");
    }
    return Cnt;
}

#if 0
#endif

static UINT32 DCE_PPROC_D2D_CB(IPL_PROC_ID id, USIZE *pRef_1x,Coordinate *pRef_1xCen,DCE_SUBFUNC* subF, DCE_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    return 1;
}

static UINT32 DCE_CAP_D2D_CB(IPL_PROC_ID id, USIZE *pRef_1x,Coordinate *pRef_1xCen,DCE_SUBFUNC* subF, DCE_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    return 1;
}

UINT32 DCE_D2D_CB(IPL_PROC_ID id, USIZE *pRef_1x,Coordinate *pRef_1xCen,DCE_SUBFUNC* subF, DCE_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    UINT32 Cnt = 1;
    DBG_IND("EventID = 0x%.8x\r\n", eventID);

    if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_CAPD2D_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_CAPD2D_MAX + 1)))
    {
        Cnt = DCE_CAP_D2D_CB(id, pRef_1x, pRef_1xCen, subF, Update, eventID,fpRegD);
    }
    else if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_PPROC_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_PPROC_MAX + 1)))
    {
        Cnt = DCE_PPROC_D2D_CB(id, pRef_1x, pRef_1xCen, subF, Update, eventID,fpRegD);
    }
    else
    {
        DBG_ERR("event fail\r\n");
    }
    return Cnt;
}

#if 0
#endif

static UINT32 IPE_PPROC_D2D_CB(IPL_PROC_ID id, IPE_SUBFUNC* subF, IPE_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    return 1;
}

static UINT32 IPE_CAP_D2D_CB(IPL_PROC_ID id, IPE_SUBFUNC* subF, IPE_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    return 1;
}

UINT32 IPE_D2D_CB(IPL_PROC_ID id, IPE_SUBFUNC* subF, IPE_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    UINT32 Cnt = 1;
    DBG_IND("EventID = 0x%.8x\r\n", eventID);

    if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_CAPD2D_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_CAPD2D_MAX + 1)))
    {
        Cnt = IPE_CAP_D2D_CB(id, subF, Update, eventID,fpRegD);
    }
    else if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_PPROC_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_PPROC_MAX + 1)))
    {
        Cnt = IPE_PPROC_D2D_CB(id, subF, Update, eventID,fpRegD);
    }
    else
    {
        DBG_ERR("event fail\r\n");
    }
    return Cnt;
}

#if 0
#endif

static UINT32 IFE_PPROC_D2D_CB(IPL_PROC_ID id, IFE_SUBFUNC* subF, IFE_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    return 1;
}

static UINT32 IFE_CAP_D2D_CB(IPL_PROC_ID id, IFE_SUBFUNC* subF, IFE_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    UINT32 Cnt = 5;
    static UINT32 d2dCnt = 5;
    if(d2dCnt<=1)
        d2dCnt = Cnt;
    d2dCnt--;
    DBG_IND("EventID = 0x%.8x\r\n", eventID);
    return Cnt;
}

UINT32 IFE_D2D_CB(IPL_PROC_ID id, IFE_SUBFUNC* subF, IFE_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    UINT32 Cnt = 1;
    DBG_IND("EventID = 0x%.8x\r\n", eventID);

    if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_CAPD2D_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_CAPD2D_MAX + 1)))
    {
        Cnt = IFE_CAP_D2D_CB(id, subF, Update, eventID,fpRegD);
    }
    else if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_PPROC_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_PPROC_MAX + 1)))
    {
        Cnt = IFE_PPROC_D2D_CB(id, subF, Update, eventID,fpRegD);
    }
    else
    {
        DBG_ERR("event fail\r\n");
    }
    return Cnt;
}

#if 0
#endif

static UINT32 IME_PPROC_D2D_CB(IPL_PROC_ID id, IME_SUBFUNC* subF, IME_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    return 1;
}

static UINT32 IME_CAP_D2D_CB(IPL_PROC_ID id, IME_SUBFUNC* subF, IME_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    UINT32 Cnt = 1;

    //IPL_FLOW_IMED2D_QV: Enable LCA
    IPL_HAL_GROUP group;
    ENG_SUBFUNC Info = {0};
    Info.pIme_sub = subF;
    group.imeUpdate = *Update;

    if ((eventID & (~IPL_FLOW_RESERVE))== IPL_FLOW_IMED2D_SCAL)
    {
        group.imeUpdate = IME_SUBFEN;
        subF->imeFuncEn = 0;
    }

    *Update = group.imeUpdate;
#if IQS_DUMP_ENGREG
    *fpRegD = (fpRegDump)IQS_DumpEngRegister;
#endif
    return Cnt;
}

UINT32 IME_D2D_CB(IPL_PROC_ID id, IME_SUBFUNC* subF, IME_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    UINT32 Cnt = 1;
    DBG_IND("EventID = 0x%.8x\r\n", eventID);

    if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_CAPD2D_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_CAPD2D_MAX + 1)))
    {
        Cnt = IME_CAP_D2D_CB(id, subF, Update, eventID,fpRegD);
    }
    else if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_PPROC_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_PPROC_MAX + 1)))
    {
        Cnt = IME_PPROC_D2D_CB(id, subF, Update, eventID,fpRegD);
    }
    else
    {
        DBG_ERR("event fail\r\n");
    }
    return Cnt;
}
#if 0
#endif

static UINT32 RDE_PPROC_D2D_CB(IPL_PROC_ID id, RDE_SUBFUNC *subF, RDE_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    return 1;
}

static UINT32 RDE_CAP_D2D_CB(IPL_PROC_ID id, RDE_SUBFUNC *subF, RDE_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    DBG_IND("EventID = 0x%.8x\r\n", eventID);
    switch(eventID)
    {
        case IPL_FLOW_RAWDEC:
            *Update |= RDE_INIT;
            subF->RDEFuncEn  = RDE_DeGammaEn;
            subF->GammaTblAddr = (UINT32)&RawDecLut[0];
            break;

        default:
            DBG_ERR("event fail\r\n");
            break;
    }
    return 1;
}

UINT32 RDE_D2D_CB(IPL_PROC_ID id, RDE_SUBFUNC *subF, RDE_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    UINT32 Cnt = 1;
    DBG_IND("EventID = 0x%.8x\r\n", eventID);

    if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_CAPD2D_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_CAPD2D_MAX + 1)))
    {
        Cnt = RDE_CAP_D2D_CB(id, subF, Update, eventID,fpRegD);
    }
    else if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_PPROC_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_PPROC_MAX + 1)))
    {
        Cnt = RDE_PPROC_D2D_CB(id, subF, Update, eventID,fpRegD);
    }
    else
    {
        DBG_ERR("event fail\r\n");
    }
    return Cnt;
}
#if 0
#endif
static UINT32 DRE_PPROC_D2D_CB(IPL_PROC_ID id, DRE_SUBFUNC *subF, DRE_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    return 1;
}

static UINT32 DRE_CAP_D2D_CB(IPL_PROC_ID id, DRE_SUBFUNC *subF, DRE_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    DBG_IND("EventID = 0x%.8x\r\n", eventID);

    IPL_HAL_GROUP group;
    ENG_SUBFUNC Info = {0};
    Info.pDre_sub = subF;
    group.dreUpdate = *Update;

    IQCB_SettingFlowFunc(id, IQCB_D2D_DRE_PRI, &Info, &group);

    *Update = group.dreUpdate;

#if IQS_DUMP_ENGREG
    UINT32 Addr, Size;
    IQS_GetEngDumpBuffer(IPL_HAL_DRE, &Addr, &Size);
    memset((char*)Addr, 0, Size);
    *Update |= DRE_UPDATE_DBG;
    subF->DumpEn = TRUE;
    subF->DumpBufAdr = Addr;
    *fpRegD = (fpRegDump)IQS_DumpEngRegister;
#endif
    return 1;
}

UINT32 DRE_D2D_CB(IPL_PROC_ID id, DRE_SUBFUNC *subF, DRE_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    UINT32 Cnt = 1;
    DBG_IND("EventID = 0x%.8x\r\n", eventID);

    if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_CAPD2D_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_CAPD2D_MAX + 1)))
    {
        Cnt = DRE_CAP_D2D_CB(id, subF, Update, eventID,fpRegD);
    }
    else if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_PPROC_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_PPROC_MAX + 1)))
    {
        Cnt = DRE_PPROC_D2D_CB(id, subF, Update, eventID,fpRegD);
    }
    else
    {
        DBG_ERR("event fail\r\n");
    }
    return Cnt;
}

#if 0
#endif
UINT32 SIE_D2D_CB(IPL_PROC_ID id, SIE_SUBFUNC *subF, SIE_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    UINT32 Cnt = 1;
    return Cnt;
}

#if 0
#endif
static UINT32 IFEIME_PPROC_D2D_IFECB(IPL_PROC_ID id, IFE_SUBFUNC* subF, IFE_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    return 1;
}


static UINT32 IFEIME_CAP_D2D_IFECB(IPL_PROC_ID id, IFE_SUBFUNC* subF, IFE_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    DBG_IND("EventID = 0x%.8x\r\n", eventID);

    IPL_HAL_GROUP group;
    ENG_SUBFUNC Info = {0};
    Info.pIfe_sub = subF;
    group.ifeUpdate = *Update;

    switch((eventID&(~IPL_FLOW_RESERVE)))
    {
        case (IPL_FLOW_RAW2YCC_PRI):
            IQCB_SettingFlowFunc(id, IQCB_PREIME_D2D_IFE_PRI, &Info, &group);
            break;
        case (IPL_FLOW_RAW2YCC_LCA):
            IQCB_SettingFlowFunc(id, IQCB_PREIME_D2D_IFE_SUB, &Info, &group);
            break;

        default:
            DBG_ERR("event fail (%x)\r\n", eventID);
            break;
    }

    *Update = group.ifeUpdate;


#if IQS_DUMP_ENGREG
    *fpRegD = (fpRegDump)IQS_DumpEngRegister;
#endif
    return 1;
}


UINT32 IFEIME_D2D_IFECB(IPL_PROC_ID id, IFE_SUBFUNC* subF, IFE_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    UINT32 Cnt = 1;
    DBG_IND("EventID = 0x%.8x\r\n", eventID);

    if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_CAPD2D_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_CAPD2D_MAX + 1)))
    {
        Cnt = IFEIME_CAP_D2D_IFECB(id, subF, Update, eventID,fpRegD);
    }
    else if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_PPROC_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_PPROC_MAX + 1)))
    {
        Cnt = IFEIME_PPROC_D2D_IFECB(id, subF, Update, eventID,fpRegD);
    }
    else
    {
        DBG_ERR("event fail\r\n");
    }
    return Cnt;
}

#if 0
#endif

static UINT32 IFEIME_PPROC_D2D_DCECB(IPL_PROC_ID id, USIZE *pRef_1x,Coordinate *pRef_1xCen,DCE_SUBFUNC* subF, DCE_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    return 1;
}


static UINT32 IFEIME_CAP_D2D_DCECB(IPL_PROC_ID id, USIZE *pRef_1x,Coordinate *pRef_1xCen,DCE_SUBFUNC* subF, DCE_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    IQS_REFIMG_INFO RefImgInfo = {0};
    IPL_HAL_GROUP group;
    ENG_SUBFUNC Info = {0};
    Info.pDce_sub = subF;
    group.dceUpdate = *Update;

    DBG_IND("EventID = 0x%.8x\r\n", eventID);
    switch((eventID&(~IPL_FLOW_RESERVE)))
    {
        case (IPL_FLOW_RAW2YCC_PRI):
            IQCB_SettingFlowFunc(id, IQCB_PREIME_D2D_DCE_PRI, &Info, &group);
            break;
        case (IPL_FLOW_RAW2YCC_LCA):
            IQCB_SettingFlowFunc(id, IQCB_PREIME_D2D_DCE_SUB, &Info, &group);
            break;
        default:
            DBG_ERR("event fail (%x)\r\n", eventID);
            break;
    }

    if(RSC_OnOffInfor(id)==SEL_RSC_ON)
    {
        UINT32 GET_Dce2DLut;

        GET_Dce2DLut = RSCCTRL_GETADR(id);
        Info.pDce_sub->_2DLutParam = (_2DLUT_PARAM *)GET_Dce2DLut;
        group.dceUpdate |= DCE_2DLUT;

    }
    else
    {
        Info.pDce_sub->_2DLutParam = &GET_Dce2DLut;
        Info.pDce_sub->_2DLutParam->_2DLutAddr = 0;
        Info.pDce_sub->_2DLutParam->_2DLutNum = 0;
        Info.pDce_sub->_2DLutParam->_2DLutScaleFact.HScaleFact = 0;
        Info.pDce_sub->_2DLutParam->_2DLutScaleFact.VScaleFact = 0;
        Info.pDce_sub->_2DLutParam->_2DLutXYOfs.XOfsFrac = 0;
        Info.pDce_sub->_2DLutParam->_2DLutXYOfs.XOfsInt = 0;
        Info.pDce_sub->_2DLutParam->_2DLutXYOfs.YOfsFrac = 0;
        Info.pDce_sub->_2DLutParam->_2DLutXYOfs.YOfsInt=0;

        group.dceUpdate |= DCE_2DLUT;
    }

    *Update = group.dceUpdate;

    IQS_GetRefImageInfo(id, IPL_AlgGetUIInfo(id, IPL_SEL_CAPSENMODE), &RefImgInfo);
    pRef_1x->w = RefImgInfo.width;
    pRef_1x->h = RefImgInfo.height;

    pRef_1xCen->x = pRef_1x->w/2;
    pRef_1xCen->y = pRef_1x->h/2;
    //DBG_ERR("DCE Ref Center=(%d,%d)\r\n",pRef_1xCen->x,pRef_1xCen->y);

    return 1;
}


UINT32 IFEIME_D2D_DCECB(IPL_PROC_ID id, USIZE *pRef_1x,Coordinate *pRef_1xCen,DCE_SUBFUNC* subF, DCE_UPDATE *Update, UINT32 eventID,fpRegDump *fpRegD)
{
    UINT32 Cnt = 1;
    DBG_IND("EventID = 0x%.8x\r\n", eventID);

    if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_CAPD2D_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_CAPD2D_MAX + 1)))
    {
        Cnt = IFEIME_CAP_D2D_DCECB(id, pRef_1x, pRef_1xCen, subF, Update, eventID,fpRegD);
    }
    else if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_PPROC_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_PPROC_MAX + 1)))
    {
        Cnt = IFEIME_PPROC_D2D_DCECB(id, pRef_1x, pRef_1xCen, subF, Update, eventID,fpRegD);
    }
    else
    {
        DBG_ERR("event fail\r\n");
    }
    return Cnt;
}
#if 0
#endif

static UINT32 IFEIME_PPROC_D2D_IPECB(IPL_PROC_ID id, IPE_SUBFUNC* subF,IPE_UPDATE *Update,UINT32 eventID,fpRegDump *fpRegD)
{
    return 1;
}

static UINT32 IFEIME_CAP_D2D_IPECB(IPL_PROC_ID id, IPE_SUBFUNC* subF,IPE_UPDATE *Update,UINT32 eventID,fpRegDump *fpRegD)
{
    DBG_IND("EventID = 0x%.8x\r\n", eventID);
    IPL_HAL_GROUP group;
    ENG_SUBFUNC Info = {0};
    Info.pIpe_sub = subF;
    group.ipeUpdate = *Update;
    switch((eventID&(~IPL_FLOW_RESERVE)))
    {
        case (IPL_FLOW_RAW2YCC_PRI):
            IQCB_SettingFlowFunc(id, IQCB_PREIME_D2D_IPE_PRI, &Info, &group);
            break;
        case (IPL_FLOW_RAW2YCC_LCA):
            IQCB_SettingFlowFunc(id, IQCB_PREIME_D2D_IPE_SUB, &Info, &group);
            break;

        default:
            DBG_ERR("event fail (%x)\r\n", eventID);
            break;
    }

    *Update = group.ipeUpdate;
    return 1;
}

UINT32 IFEIME_D2D_IPECB(IPL_PROC_ID id, IPE_SUBFUNC* subF,IPE_UPDATE *Update,UINT32 eventID,fpRegDump *fpRegD)
{
    UINT32 Cnt = 1;
    DBG_IND("EventID = 0x%.8x\r\n", eventID);

    if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_CAPD2D_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_CAPD2D_MAX + 1)))
    {
        Cnt = IFEIME_CAP_D2D_IPECB(id, subF, Update, eventID,fpRegD);
    }
    else if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_PPROC_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_PPROC_MAX + 1)))
    {
        Cnt = IFEIME_PPROC_D2D_IPECB(id, subF, Update, eventID,fpRegD);
    }
    else
    {
        DBG_ERR("event fail\r\n");
    }
    return Cnt;
}
#if 0
#endif

static UINT32 IFEIME_PPROC_D2D_IMECB(IPL_PROC_ID id, IME_SUBFUNC* subF,IME_UPDATE *Update,UINT32 eventID,fpRegDump *fpRegD)
{
    return 1;
}

static UINT32 IFEIME_CAP_D2D_IMECB(IPL_PROC_ID id, IME_SUBFUNC* subF,IME_UPDATE *Update,UINT32 eventID,fpRegDump *fpRegD)
{
    DBG_IND("EventID = 0x%.8x\r\n", eventID);

    IPL_HAL_GROUP group;
    ENG_SUBFUNC Info = {0};
    Info.pIme_sub = subF;
    group.imeUpdate = *Update;
    switch((eventID&(~IPL_FLOW_RESERVE)))
    {
        case (IPL_FLOW_RAW2YCC_PRI):
            IQCB_SettingFlowFunc(id, IQCB_PREIME_D2D_IME_PRI, &Info, &group);
            break;
        case (IPL_FLOW_RAW2YCC_LCA):
            break;
        default:
            DBG_ERR("event fail (%x)\r\n", eventID);
            break;
    }
    *Update = group.imeUpdate;
    return 1;
}

UINT32 IFEIME_D2D_IMECB(IPL_PROC_ID id, IME_SUBFUNC* subF,IME_UPDATE *Update,UINT32 eventID,fpRegDump *fpRegD)
{
    UINT32 Cnt = 1;
    DBG_IND("EventID = 0x%.8x\r\n", eventID);

    if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_CAPD2D_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_CAPD2D_MAX + 1)))
    {
        Cnt = IFEIME_CAP_D2D_IMECB(id, subF, Update, eventID,fpRegD);
    }
    else if (((eventID & (~IPL_FLOW_RESERVE)) >= IPL_FLOW_PPROC_MIN) && ((eventID & (~IPL_FLOW_RESERVE)) < (IPL_FLOW_PPROC_MAX + 1)))
    {
        Cnt = IFEIME_PPROC_D2D_IMECB(id, subF, Update, eventID,fpRegD);
    }
    else
    {
        DBG_ERR("event fail\r\n");
    }
    return Cnt;
}
#if 0
#endif
void _STRIPE_CB(IPL_PROC_ID id, USIZE *Ref_1x,Coordinate *Ref_1xCen,
                USIZE *SieInSize, USIZE *SieOutSize, UINT32 *SenHRatio,UINT32 *SenVRatio,
                DCE_SUBFUNC* subF, IPL_HAL_GROUP *Update, UINT32 eventID)
{
    ENG_SUBFUNC Info = {0};
    Info.pDce_sub = subF;
    IPL_HAL_GROUP group;
    group = *Update ;
    IQS_REFIMG_INFO RefImgInfo = {0};
    IQS_FLOW_MSG IQSevent = IQCB_PREIME_D2D_DCE_PRI;

    if (eventID == IPL_FLOW_CAL_STRIPE)
    {
        IPL_DzoomGetSIECropSize(id, 0, TRUE, &(SieInSize->w), &(SieInSize->h));
        IPL_DzoomGetSIEOutSize(id, IPL_DzoomGetIndex(id), &(SieOutSize->w), &(SieOutSize->h));
        IQS_GetRefImageInfo(id, IPL_AlgGetUIInfo(id, IPL_SEL_CAPSENMODE), &RefImgInfo);
        Ref_1x->w = RefImgInfo.width;
        Ref_1x->h = RefImgInfo.height;
        Ref_1xCen->x = Ref_1x->w/2;
        Ref_1xCen->y = Ref_1x->h/2;
        *SenHRatio = RefImgInfo.hratio;
        *SenVRatio = RefImgInfo.vratio;

        // coverity[var_deref_model]
        IQCB_SettingFlowFunc(id, IQSevent, &Info, &group);
    }
    else
    {
        DBG_ERR("event fail\r\n");
    }

    if(RSC_OnOffInfor(id)==SEL_RSC_ON)
    {

        UINT32 GET_Dce2DLut;

        GET_Dce2DLut = RSCCTRL_GETADR(id);

        Info.pDce_sub->_2DLutParam = (_2DLUT_PARAM *)GET_Dce2DLut;
        group.dceUpdate |= DCE_2DLUT;

    }
    else
    {
        Info.pDce_sub->_2DLutParam = &GET_Dce2DLut;
        Info.pDce_sub->_2DLutParam->_2DLutAddr = 0;
        Info.pDce_sub->_2DLutParam->_2DLutNum = 0;
        Info.pDce_sub->_2DLutParam->_2DLutScaleFact.HScaleFact = 0;
        Info.pDce_sub->_2DLutParam->_2DLutScaleFact.VScaleFact = 0;
        Info.pDce_sub->_2DLutParam->_2DLutXYOfs.XOfsFrac = 0;
        Info.pDce_sub->_2DLutParam->_2DLutXYOfs.XOfsInt = 0;
        Info.pDce_sub->_2DLutParam->_2DLutXYOfs.YOfsFrac = 0;
        Info.pDce_sub->_2DLutParam->_2DLutXYOfs.YOfsInt=0;

        group.dceUpdate |= DCE_2DLUT;
    }

    *Update = group ;
}

