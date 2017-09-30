/**
    IQ user contrl flow.

    IQUC flow.

    @file       IQUC_sample.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include <stdio.h>

#include "Type.h"
#include "IPL_AlgInfor.h"
#include "ae_api.h"
#include "ae_task.h"
#include "awb_api.h"
#include "awb_task.h"
#include "iqs_api.h"
#include "IPL_Ctrl.h"
#include "IQS_Utility.h"
#include "IQS_Ctrl.h"
#include "IQUC_api.h"
#include "IQUC_Ctrl.h"
#include "IQUC_Sample_int.h"

#define __MODULE__ IQUC
#define __DBGLVL__ 1        //ERROR mode, show err, wrn only
#define __DBGFLT__ "*"      //*=All
#include "DebugModule.h"
#include "FileSysTsk.h"

void IQUCLOG_OPEN1(char *pFilename);
void IQUCLOG1(char *format, va_list args);
void IQUCLOG_CLOSE1(void);
void IQUCLOG_OPEN2(char *pFilename);
void IQUCLOG2(char *format, va_list args);
void IQUCLOG_CLOSE2(void);

extern void IQUCLOG_OPEN(char *pFilename);
extern void IQUCLOG(char *format, ...);
extern void IQUCLOG_CLOSE(void);

extern UINT32 uiRGain[AWB_ID_MAX_NUM], uiGGain[AWB_ID_MAX_NUM], uiBGain[AWB_ID_MAX_NUM];
extern UINT32 uiISOGain[AE_ID_MAX_NUM], uiExpoTime[AE_ID_MAX_NUM]; //, uiIris[AE_ID_MAX_NUM];
extern void IQUC_SetTuningRatio(UINT32 Id, UINT32* IQUC_IQRatio, UINT32* IQUC_CACValue);

IQUC_IF fpIQUC = {0, 0, 0, 0};

UINT32 IQUC_CtrlIndex[IQUC_ID_MAX_NUM] = {0};
IQUC_CTRL_ITEM IQUC_CtrlItem[IQUC_ID_MAX_NUM][IQUC_CTRLITEM_MAX_NUM] = {0};
INT32 IQUC_TuningIQRatio[ADJ_IQRatio_MAX_CNT] = {0};
INT32 IQUC_TuningCACValue[ADJ_IQRatio_CAC_MAX_CNT] = {0};

// Lens
UINT32 IQUC_VIG_R[IQUC_ID_MAX_NUM][17];
UINT32 IQUC_VIG_G[IQUC_ID_MAX_NUM][17];
UINT32 IQUC_VIG_B[IQUC_ID_MAX_NUM][17];
DCE_Distortion  IQUC_DCE_Distortion[IQUC_ID_MAX_NUM];       ///< distortion configuration
UINT32 IQUC_DCE_GLUT[IQUC_ID_MAX_NUM][65];
INT32 IQUC_DCE_RLUT[IQUC_ID_MAX_NUM][65];
INT32 IQUC_DCE_BLUT[IQUC_ID_MAX_NUM][65];
// IFE
UINT32 IQUC_RangeA_Thr[IQUC_ID_MAX_NUM][6];
UINT32 IQUC_RangeB_Thr[IQUC_ID_MAX_NUM][6];
UINT32 IQUC_RangeA_Lut[IQUC_ID_MAX_NUM][17];
UINT32 IQUC_RangeB_Lut[IQUC_ID_MAX_NUM][17];
IFE_FILT IQUC_Filter[IQUC_ID_MAX_NUM];
IFE_GBAL IQUC_Gbal[IQUC_ID_MAX_NUM];
UINT32 IQUC_Outl_Bri[IQUC_ID_MAX_NUM][5];
UINT32 IQUC_Outl_Dark[IQUC_ID_MAX_NUM][5];
// IPE
IPE_EdgeW IQUC_EWParam[IQUC_ID_MAX_NUM];
IPE_DirScorCalTh IQUC_DirScoreTh[IQUC_ID_MAX_NUM];
IPE_EDGEENH IQUC_EdgeEnh[IQUC_ID_MAX_NUM];
UINT8 IQUC_EDtab[IQUC_ID_MAX_NUM][16];
UINT8 IQUC_EStab[IQUC_ID_MAX_NUM][16];
EdgeMap IQUC_EDMap[IQUC_ID_MAX_NUM];
IPE_CCON IQUC_YCCCon[IQUC_ID_MAX_NUM];
UINT8 IQUC_FStab[IQUC_ID_MAX_NUM][16];
IPE_CCTRL IQUC_CCtrl[IQUC_ID_MAX_NUM];
IPE_RGBLPF IQUC_RGBLpf[IQUC_ID_MAX_NUM];
// IFE2
IFE2_REFCENT_Set IQUC_RefCenter_Y[IQUC_ID_MAX_NUM];
IFE2_REFCENT_Set IQUC_RefCenter_UV[IQUC_ID_MAX_NUM];
IFE2_Filter_Param IQUC_IFE2_Filter[IQUC_ID_MAX_NUM];
// IME
IME_CHRA_ADAP IQUC_ChromaAdapt[IQUC_ID_MAX_NUM];
IME_CHRA_CADAP IQUC_ChromaAdapt_Color[IQUC_ID_MAX_NUM];

#define IQUCLOG_LUT(name, size, format, linesize) \
    { \
        IQUCLOG("\r\n{\r\n{"); \
        int ilut = 0;\
        for ( ilut=0; ilut<size; ilut++ ) {\
            IQUCLOG(format, name[ilut]); \
            if ( ilut!=size-1 ) IQUCLOG(", "); \
            if ( linesize!=0 && (ilut % linesize)==linesize-1 ) IQUCLOG("\r\n");\
        }\
        IQUCLOG("},");\
        if ( linesize==0 || (ilut % linesize)!=0  )  IQUCLOG("\r\n");\
        IQUCLOG("}\r\n"); \
    }

char g_IQUCLogName[256] = {0};
char g_IQUCLogBuf[3072] = {0};
FST_FILE g_IQUCLogFileHdl;

void IQUCLOG_OPEN1(char *pFilename)
{
    if(!pFilename)
        return;
    strncpy(g_IQUCLogName, pFilename, sizeof(g_IQUCLogName)-1);
    g_IQUCLogFileHdl = FileSys_OpenFile(g_IQUCLogName,FST_OPEN_ALWAYS|FST_OPEN_WRITE);
    FileSys_SeekFile(g_IQUCLogFileHdl, 0, FST_SEEK_END);
    memset(g_IQUCLogBuf,0,sizeof(char)*3072);
}

void IQUCLOG1(char *format, va_list args)
{
    int ret;
    char message[1024];

    ret = vsnprintf(message, sizeof(message), format, args);
    snprintf(g_IQUCLogBuf, sizeof(g_IQUCLogBuf), "%s%s", g_IQUCLogBuf,message);

    UINT32 WriteSize;
    WriteSize = strlen(g_IQUCLogBuf);
    if ( WriteSize >= 2048 )
    {
        FileSys_WriteFile(g_IQUCLogFileHdl, (UINT8*)g_IQUCLogBuf, &WriteSize, 0, NULL);
        memset(g_IQUCLogBuf,0,sizeof(g_IQUCLogBuf));
    }
}

void IQUCLOG_CLOSE1(void)
{
    UINT32 WriteSize;
    WriteSize = strlen(g_IQUCLogBuf);
    if ( WriteSize > 0 )
        FileSys_WriteFile(g_IQUCLogFileHdl, (UINT8*)g_IQUCLogBuf, &WriteSize, 0, NULL);

    FileSys_CloseFile(g_IQUCLogFileHdl);
}


void IQUCLOG_OPEN2(char *pFilename)
{
    return;
}

void IQUCLOG2(char *format, va_list args)
{
    int ret;
    char message[1024];
    ret = vsnprintf(message, sizeof(message), format, args);
    DBG_DUMP("%s",message);
}

void IQUCLOG_CLOSE2(void)
{
    return ;
}


void IQUCLOG_OPEN(char *pFilename)
{
    IQUCLOG_OPEN1(pFilename);
    IQUCLOG_OPEN2(pFilename);
    return;
}

void IQUCLOG(char *format, ...)
{

    va_list args;
    va_start (args, format);
    //#NT#2016/02/19#Kristin Huang -begin
    //#NT# Fix warnings
    if(args)
    {
        IQUCLOG1(format, args);
        IQUCLOG2(format, args);
    }
    //#NT#2016/02/19#Kristin Huang -end
    va_end (args);
}

void IQUCLOG_CLOSE(void)
{
    IQUCLOG_CLOSE1();
    IQUCLOG_CLOSE2();
    return ;
}

UINT32 IQUC_InitCtrlItem(UINT32 Id, IQUC_IF **pfpIQUC)
{
    //UINT32 i;
    if ( ADJ_ITEM_MAX_CNT >= IQUC_ITEM_POOL_MAX_CNT )
    {
        DBG_ERR("ITEM_POOL size < ADJ_ITEM size\r\n");
        return E_SYS;
    }

    memset(IQUC_CtrlItem[Id], 0, sizeof(IQUC_CTRL_ITEM)*IQUC_CTRLITEM_MAX_NUM);
    IQUC_CtrlIndex[Id] = 0;
    //for ( i=0; i<IQUC_CTRLITEM_MAX_NUM; i++ )
    //{
    //    IQUC_CtrlItem[Id][i].ADJ_ITEM[ADJ_ITEM_MAX_CNT] = IQUC_ITEM_END_TAG;
    //}
    fpIQUC.GetCtrlItem = IQUC_GetCtrlItem;
    fpIQUC.SetCtrlIndex = IQUC_SetCtrlIndex;
    fpIQUC.GetCtrlIndex = IQUC_GetCtrlIndex;
    fpIQUC.SetCtrlItem = IQUC_SetCtrlItem;
    fpIQUC.Set2ACtrlItem = IQUC_Set2ACtrlItem;
    fpIQUC.SettingFlow = IQUC_SettingFlow;
    fpIQUC.DumpInfo = IQTUC_DumpRatioTable;
    *pfpIQUC = &fpIQUC;

    return E_OK;
}

UINT32 IQUC_GetCtrlIndex(UINT32 Id)
{
    return IQUC_CtrlIndex[Id];
}

UINT32 IQUC_SetCtrlIndex(UINT32 Id, UINT32 IQIndex)
{
    IQUC_CtrlIndex[Id] = IQIndex;
    return E_OK;
}

IQUC_CTRL_ITEM* IQUC_GetCtrlItem(UINT32 Id, UINT32 Index)
{
    UINT32 RatioItem;
    UINT32 ISO, ExpTime, Iris;
    UINT32 RGain, GGain, BGain;
    AWB_GetColorGain(Id, &RGain, &GGain, &BGain);
    AE_GetPrvAEArg(Id, &ISO, &ExpTime, &Iris);
    IQUC_CtrlItem[Id][Index].IQIndex = IQUC_CtrlIndex[Id];

    IQUC_GetTuningRatio(Id, (UINT32*)IQUC_TuningIQRatio, (UINT32*)IQUC_TuningCACValue);

    for( RatioItem=0; RatioItem<ADJ_IQRatio_MAX_CNT; RatioItem++ )
        IQUC_CtrlItem[Id][Index].ADJ_RATIO[RatioItem] = IQUC_TuningIQRatio[RatioItem];

    for( RatioItem=0; RatioItem<ADJ_IQRatio_CAC_MAX_CNT; RatioItem++ )
        IQUC_CtrlItem[Id][Index].ADJ_CAC_VALUE[RatioItem] = IQUC_TuningCACValue[RatioItem];

    IQUC_CtrlItem[Id][Index].ADJ_ITEM[ADJ_GAIN] = ISO;
    IQUC_CtrlItem[Id][Index].ADJ_ITEM[ADJ_EXPOSURE] = ExpTime;
    IQUC_CtrlItem[Id][Index].ADJ_ITEM[ADJ_APERTURE] = Iris;

    IQUC_CtrlItem[Id][Index].ADJ_ITEM[ADJ_AWB_RGAIN] = RGain;
    IQUC_CtrlItem[Id][Index].ADJ_ITEM[ADJ_AWB_GGAIN] = GGain;
    IQUC_CtrlItem[Id][Index].ADJ_ITEM[ADJ_AWB_BGAIN] = BGain;

    IQUC_CtrlItem[Id][Index].ADJ_ITEM[ADJ_IMAGEFLIP] = IPL_AlgGetUIInfo(Id, IPL_SEL_IMAGEFLIP);
    return &IQUC_CtrlItem[Id][Index];
}

UINT32 IQUC_Set2ACtrlItem(UINT32 Id, UINT32 Index, IQUC_CTRL_ITEM *Item)
{
    uiRGain[Id] = Item->ADJ_ITEM[ADJ_AWB_RGAIN];
    uiGGain[Id] = Item->ADJ_ITEM[ADJ_AWB_GGAIN];
    uiBGain[Id] = Item->ADJ_ITEM[ADJ_AWB_BGAIN];

    uiExpoTime[Id] = Item->ADJ_ITEM[ADJ_EXPOSURE];
    uiISOGain[Id]  = Item->ADJ_ITEM[ADJ_GAIN];
    return E_OK;
}

UINT32 IQUC_SetCtrlItem(UINT32 Id, UINT32 Index, IQUC_CTRL_ITEM *Item)
{
    //if ( (UINT32)Item->ADJ_ITEM[ADJ_ITEM_MAX_CNT] != IQUC_ITEM_END_TAG )
    //{
    //    DBG_ERR("wrong IQUC structure, ignore \r\n");
    //    return E_NOSPT;
    //}
    if ( Item->IQIndex != Index || Item->Id != Id )
    {
        DBG_ERR("wrong IQUC mapping, ignore \r\n");
        return E_ID;
    }
    DBG_DUMP("User layer IQ adjustment %d, %d\r\n", Id, Index);
    IQUC_CtrlItem[Id][Index] = *Item;
    IPL_AlgSetUIInfo(Id, IPL_SEL_IMAGEFLIP, IQUC_CtrlItem[Id][Index].ADJ_ITEM[ADJ_IMAGEFLIP]);

    uiRGain[Id] = IQUC_CtrlItem[Id][Index].ADJ_ITEM[ADJ_AWB_RGAIN];
    uiGGain[Id] = IQUC_CtrlItem[Id][Index].ADJ_ITEM[ADJ_AWB_GGAIN];
    uiBGain[Id] = IQUC_CtrlItem[Id][Index].ADJ_ITEM[ADJ_AWB_BGAIN];
    AWB_SetColorGain(Id, uiRGain[Id], uiGGain[Id], uiBGain[Id]);

    uiExpoTime[Id] = IQUC_CtrlItem[Id][Index].ADJ_ITEM[ADJ_EXPOSURE];
    uiISOGain[Id]  = IQUC_CtrlItem[Id][Index].ADJ_ITEM[ADJ_GAIN];
    //uiIris[Id]     = IQUC_CtrlItem[Id][Index].ADJ_ITEM[ADJ_APERTURE];

    UINT32 RatioItem;

    for( RatioItem=0; RatioItem<ADJ_IQRatio_MAX_CNT; RatioItem++ )
    {
        IQUC_TuningIQRatio[RatioItem] = IQUC_CtrlItem[Id][Index].ADJ_RATIO[RatioItem];
    }

    for( RatioItem=0; RatioItem<ADJ_IQRatio_CAC_MAX_CNT; RatioItem++ )
    {
        IQUC_TuningCACValue[RatioItem] = IQUC_CtrlItem[Id][Index].ADJ_CAC_VALUE[RatioItem];
    }

    IQUC_SetTuningRatio(Id, (UINT32*)IQUC_TuningIQRatio, (UINT32*)IQUC_TuningCACValue);

    IQS_SettingFlowFunc(Id, IQS_MANUAL_TRIG);
    return E_OK;
}

UINT32 IQUC_DumpUserControlTable(UINT32 Id)
{
    //IQUC_CTRL_ITEM* pIQUserAdj = IQUC_GetCtrlItem(Id, IQUC_CtrlIndex[Id]);
    char filename[64];
    snprintf(filename, sizeof(filename), "A:\\IQUCTab%02d%02d.txt", Id,IQUC_CtrlIndex[Id]);
    IQUCLOG_OPEN(filename);
    IQUCLOG("=======================================================\r\n");
    IQUCLOG("{\r\n");
    IQUCLOG("0, //Cmd\r\n");
    IQUCLOG("{0,0,0,0,0,0,0,0}, //CmdData[8]\r\n");
    IQUCLOG("%d, //Id\r\n", Id);
    IQUCLOG("%d, //CtrlIndex\r\n", IQUC_CtrlIndex[Id]);
    IQUCLOG("0,  //Status\r\n");
    IQUCLOG("{\r\n");

// Lens
    IQUCLOG("    %d,  //ADJ_VIG_R       \r\n", IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_VIG_R       ]);
    IQUCLOG("    %d,  //ADJ_VIG_G       \r\n", IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_VIG_G       ]);
    IQUCLOG("    %d,  //ADJ_VIG_B       \r\n", IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_VIG_B       ]);
    IQUCLOG("    %d,  //ADJ_DISTORTION  \r\n", IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_DISTORTION  ]);
    IQUCLOG("    %d,  //ADJ_CAC_R       \r\n", IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_CAC_R       ]);
    IQUCLOG("    %d,  //ADJ_CAC_B       \r\n", IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_CAC_B       ]);
// IFE
    IQUCLOG("    %d,  //ADJ_RANGEA_THR  \r\n", IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_RANGEA_THR  ]);
    IQUCLOG("    %d,  //ADJ_RANGEB_THR  \r\n", IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_RANGEB_THR  ]);
    IQUCLOG("    %d,  //ADJ_RANGEA_LUT  \r\n", IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_RANGEA_LUT  ]);
    IQUCLOG("    %d,  //ADJ_RANGEB_LUT  \r\n", IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_RANGEB_LUT  ]);
    IQUCLOG("    %d,  //ADJ_CLAMP_TH    \r\n", IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_CLAMP_TH    ]);
    IQUCLOG("    %d,  //ADJ_CLAMP_MUL   \r\n", IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_CLAMP_MUL   ]);
    IQUCLOG("    %d,  //ADJ_CLAMP_DLT   \r\n", IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_CLAMP_DLT   ]);
    IQUCLOG("    %d,  //ADJ_BILAT_W     \r\n", IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_BILAT_W     ]);
    IQUCLOG("    %d,  //ADJ_RTH_W       \r\n", IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_RTH_W       ]);
    IQUCLOG("    %d,  //ADJ_GBAL_DTAB   \r\n", IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_GBAL_DTAB   ]);
    IQUCLOG("    %d,  //ADJ_GBALTHR     \r\n", IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_GBALTHR     ]);
    IQUCLOG("    %d,  //ADJ_OUTL_BRI    \r\n", IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_OUTL_BRI    ]);
    IQUCLOG("    %d,  //ADJ_OUTL_DARK   \r\n", IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_OUTL_DARK   ]);
// IPE
    IQUCLOG("    %d,  //ADJ_EWDIR0      \r\n", IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_EWDIR0      ]);
    IQUCLOG("    %d,  //ADJ_EWDIR1      \r\n", IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_EWDIR1      ]);
    IQUCLOG("    %d,  //ADJ_EWDIR2      \r\n", IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_EWDIR2      ]);
    IQUCLOG("    %d,  //ADJ_DYNEMAP     \r\n", IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_DYNEMAP     ]);
    IQUCLOG("    %d,  //ADJ_SCORETH0    \r\n", IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_SCORETH0    ]);
    IQUCLOG("    %d,  //ADJ_SCORETH1    \r\n", IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_SCORETH1    ]);
    IQUCLOG("    %d,  //ADJ_SCORETH2    \r\n", IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_SCORETH2    ]);
    IQUCLOG("    %d,  //ADJ_SCORETH3    \r\n", IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_SCORETH3    ]);
    IQUCLOG("    %d,  //ADJ_ENHP        \r\n", IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_ENHP        ]);
    IQUCLOG("    %d,  //ADJ_ENHN        \r\n", IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_ENHN        ]);
    IQUCLOG("    %d,  //ADJ_EDTAB_DARK  \r\n", IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_EDTAB_DARK  ]);
    IQUCLOG("    %d,  //ADJ_EDTAB_LIGHT \r\n", IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_EDTAB_LIGHT ]);
    IQUCLOG("    %d,  //ADJ_ESTAB_DARK  \r\n", IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_ESTAB_DARK  ]);
    IQUCLOG("    %d,  //ADJ_ESTAB_LIGHT \r\n", IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_ESTAB_LIGHT ]);
    IQUCLOG("    %d,  //ADJ_EDMAP_ETHRL \r\n", IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_EDMAP_ETHRL ]);
    IQUCLOG("    %d,  //ADJ_EDMAP_ETABL \r\n", IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_EDMAP_ETABL ]);
    IQUCLOG("    %d,  //ADJ_CONTRAST    \r\n", IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_CONTRAST    ]);
    IQUCLOG("    %d,  //ADJ_FSTAB_DARK  \r\n", IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_FSTAB_DARK  ]);
    IQUCLOG("    %d,  //ADJ_FSTAB_LIGHT \r\n", IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_FSTAB_LIGHT ]);
    IQUCLOG("    %d,  //ADJ_SATURATION  \r\n", IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_SATURATION  ]);
    IQUCLOG("    %d,  //ADJ_VDETDIV     \r\n", IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_VDETDIV     ]);
    IQUCLOG("    %d,  //ADJ_RGBLPF_LPFW \r\n", IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_RGBLPF_LPFW ]);
// IFE2
    IQUCLOG("    %d,  //ADJ_IFE2_Y_CWT   \r\n", IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_IFE2_Y_CWT   ]);
    IQUCLOG("    %d,  //ADJ_IFE2_UV_CWT  \r\n", IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_IFE2_UV_CWT  ]);
    IQUCLOG("    %d,  //ADJ_IFE2_ED_HV_TH\r\n", IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_IFE2_ED_HV_TH]);
    IQUCLOG("    %d,  //ADJ_IFE2_ED_PN_TH\r\n", IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_IFE2_ED_PN_TH]);
// IME
    IQUCLOG("    %d,  //ADJ_CHRA_OUT_WT  \r\n", IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_CHRA_OUT_WT  ]);
    IQUCLOG("    %d,  //ADJ_CHRA_CA_TH   \r\n", IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_CHRA_CA_TH   ]);
// MCTF
    IQUCLOG("    %d,  //ADJ_TNR          \r\n", IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_TNR          ]);

    //IQUCLOG("    %d,  //ADJ_MFR1        \r\n", pIQUserAdj->ADJ_ITEM[ADJ_MFR1        ]);
    //IQUCLOG("    %d,  //ADJ_MFR2        \r\n", pIQUserAdj->ADJ_ITEM[ADJ_MFR2        ]);
    //IQUCLOG("    %d,  //ADJ_MFR3        \r\n", pIQUserAdj->ADJ_ITEM[ADJ_MFR3        ]);
    //IQUCLOG("    %d,  //ADJ_MFR_TH      \r\n", pIQUserAdj->ADJ_ITEM[ADJ_MFR_TH      ]);

    IQUCLOG("}\r\n");
    IQUCLOG("}\r\n");
    IQUCLOG_CLOSE();

    return E_OK;
}

UINT32 IQUC_DumpSettingTable(UINT32 Id)
{
    char filename[64];
    snprintf(filename, sizeof(filename), "A:\\IQUCFull%02d%02d.txt", Id,IQUC_CtrlIndex[Id]);
    IQUCLOG_OPEN(filename);

    IQUCLOG("=======================================================\r\n");
    IQUCLOG("Sensor = %d, CtrlIndex = %d \r\n", Id, IQUC_CtrlIndex[Id]);

    IQUCLOG("\r\nSEL_IMAGEFLIP = %d", IPL_AlgGetUIInfo(Id, IPL_SEL_IMAGEFLIP));
    IQUCLOG("\r\nRGain = %d, GGain = %d, BGain = %d", uiRGain[Id] , uiGGain[Id] , uiBGain[Id] );
    IQUCLOG("\r\nExpoTime = %d, ISO = %d\r\n", uiExpoTime[Id], uiISOGain[Id]);

// DCE
    IQUCLOG("\r\n//////////////////\r\n// DCE\r\n//////////////////\r\n\r\n");

    IQUCLOG("IQS_DCE_GLUT[65] = "); IQUCLOG_LUT(IQUC_DCE_GLUT[Id], 65, "%d", 10);
    IQUCLOG("IQS_DCE_ColorAbbration = \r\n{\r\n{%d, {%d, %d, %d}},\r\n}\r\n", IQUC_DCE_Distortion[Id].ColorAber.Cacsel, IQUC_DCE_Distortion[Id].ColorAber.CACGain.Fact_R, IQUC_DCE_Distortion[Id].ColorAber.CACGain.Fact_G, IQUC_DCE_Distortion[Id].ColorAber.CACGain.Fact_B);

// IFE
    IQUCLOG("\r\n//////////////////\r\n// IFE\r\n//////////////////\r\n\r\n");

    IQUCLOG("IQS_VIG_R[17] = "); IQUCLOG_LUT(IQUC_VIG_R[Id],  17, "%d", 0);

    IQUCLOG("IQS_RangeA_Thr[6] = "); IQUCLOG_LUT(IQUC_RangeA_Thr[Id],  6, "%d", 0);
    IQUCLOG("IQS_RangeA_Lut[17] = "); IQUCLOG_LUT(IQUC_RangeA_Lut[Id], 17, "%d", 0);
    IQUCLOG("IQS_RangeB_Thr[6] = "); IQUCLOG_LUT(IQUC_RangeB_Thr[Id],  6, "%d", 0);
    IQUCLOG("IQS_RangeB_Lut[17] = "); IQUCLOG_LUT(IQUC_RangeB_Lut[Id], 17, "%d", 0);

    IQUCLOG("IQS_Filter =  \r\n{\r\n{%d, &IQS_SpatialFilter[II], &IQS_RangeFilter[II], {%d, %d, %d}, %d, %d, %d},\r\n}\r\n",
            IQUC_Filter[Id].IFEFiltMode,
            IQUC_Filter[Id].Clamp.Th, IQUC_Filter[Id].Clamp.Mul, IQUC_Filter[Id].Clamp.Dlt,
            IQUC_Filter[Id].Bilat_w, IQUC_Filter[Id].Rth_W, IQUC_Filter[Id].Bin);

    IQUCLOG("IQS_Gbal_Dtab[16] =  \r\n{\r\n{%d,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},\r\n}\r\n", IQUC_Gbal[Id].pGbalDtab[0]);
    IQUCLOG("IQS_Gbal = \r\n{\r\n{%d, IQS_Gbal_Stab[II], IQS_Gbal_Dtab[II]},\r\n}\r\n", IQUC_Gbal[Id].GbalThr);

    IQUCLOG("IQS_Outl_Bri[5] = "); IQUCLOG_LUT(IQUC_Outl_Bri[Id], 5, "%d", 0);
    IQUCLOG("IQS_Outl_Dark[5] = "); IQUCLOG_LUT(IQUC_Outl_Dark[Id], 5, "%d", 0);

// IPE
    IQUCLOG("\r\n//////////////////\r\n// IPE\r\n//////////////////\r\n\r\n");

    IQUCLOG("IQS_EWParam =  \r\n{\r\n{%d, %d, %d, %d},\r\n}\r\n", IQUC_EWParam[Id].uiEWDir0, IQUC_EWParam[Id].uiEWDir1, IQUC_EWParam[Id].uiEWDir2, IQUC_EWParam[Id].DynEMEn);
    IQUCLOG("IQS_DirScoreTh =  \r\n{\r\n{%d, %d, %d, %d},\r\n}\r\n", IQUC_DirScoreTh[Id].uiScoreTh0, IQUC_DirScoreTh[Id].uiScoreTh1, IQUC_DirScoreTh[Id].uiScoreTh2, IQUC_DirScoreTh[Id].uiScoreTh3);
    IQUCLOG("IQS_EdgeEnh =  \r\n{\r\n{%d, %d, %d, %d},\r\n}\r\n", IQUC_EdgeEnh[Id].EnhP,  IQUC_EdgeEnh[Id].EnhN,  IQUC_EdgeEnh[Id].InvP,  IQUC_EdgeEnh[Id].InvN);

    IQUCLOG("IQS_EDTab[16] = "); IQUCLOG_LUT(IQUC_EDtab[Id],  16, "%d", 0);
    IQUCLOG("IQS_EStab[16] = "); IQUCLOG_LUT(IQUC_EStab[Id],  16, "%d", 0);

    IQUCLOG("IQS_EDMap = \r\n{\r\n{%d, %d, %d, %d, %d, (UINT32)&IQS_EDTab[II]},\r\n}\r\n", IQUC_EDMap[Id].EM_InSel, IQUC_EDMap[Id].EthrL, IQUC_EDMap[Id].EthrH, IQUC_EDMap[Id].EtabL, IQUC_EDMap[Id].EtabH);

    IQUCLOG("\r\nContrast   = %3d;             // Range: 0~255\r\n", IQUC_YCCCon[Id].YCon);
    IQUCLOG("Saturation = %3d;             // Range: -128~127\r\n", IQUC_CCtrl[Id].SatOfs);
    IQUCLOG("IQS_CCtrl.uiVdetDiv = %3d;    // Range: 0~255\r\n\r\n", IQUC_CCtrl[Id].uiVdetDiv);

    IQUCLOG("IQS_FStab[16] = "); IQUCLOG_LUT(IQUC_FStab[Id],  16, "%d", 0);

    IQUCLOG("IQS_RGBLpf = \r\n{\r\n{{%d, %d, %d, %d, %d}, {%d, %d, %d, %d, %d}, {%d, %d, %d, %d, %d}},\r\n}\r\n",
            IQUC_RGBLpf[Id].R.lpfw, IQUC_RGBLpf[Id].R.sonlyw, IQUC_RGBLpf[Id].R.rangeth0, IQUC_RGBLpf[Id].R.rangeth1, IQUC_RGBLpf[Id].R.lpfsize,
            IQUC_RGBLpf[Id].G.lpfw, IQUC_RGBLpf[Id].G.sonlyw, IQUC_RGBLpf[Id].G.rangeth0, IQUC_RGBLpf[Id].G.rangeth1, IQUC_RGBLpf[Id].G.lpfsize,
            IQUC_RGBLpf[Id].B.lpfw, IQUC_RGBLpf[Id].B.sonlyw, IQUC_RGBLpf[Id].B.rangeth0, IQUC_RGBLpf[Id].B.rangeth1, IQUC_RGBLpf[Id].B.lpfsize);

// IFE2
    IQUCLOG("\r\n//////////////////\r\n// IFE2\r\n//////////////////\r\n\r\n");

    IQUCLOG("IQS_RefCenter_Y =  \r\n{\r\n{(UINT32)IQS_RefCenter_YTh[II], (UINT32)IQS_RefCenter_YWt[II], %d, %d, %d},\r\n}\r\n",
            IQUC_RefCenter_Y[Id].uiCntWt, IQUC_RefCenter_Y[Id].uiOutlDth, IQUC_RefCenter_Y[Id].uiOutlTh);
    IQUCLOG("IQS_RefCenter_UV =  \r\n{\r\n{(UINT32)IQS_RefCenter_UVTh[II], (UINT32)IQS_RefCenter_UVWt[II], %d, %d, %d},\r\n}\r\n",
            IQUC_RefCenter_UV[Id].uiCntWt, IQUC_RefCenter_UV[Id].uiOutlDth, IQUC_RefCenter_UV[Id].uiOutlTh);

    IQUCLOG("IQS_IFE2Filter =  \r\n{\r\n{xx, {%d, {%d, %d}, %d, {(UINT32)IQS_IFE2Filter_YTh[II], (UINT32)IQS_IFE2Filter_YWt[II]},  {(UINT32)IQS_IFE2Filter_UTh[II], (UINT32)IQS_IFE2Filter_UWt[II]}, {(UINT32)IQS_IFE2Filter_VTh[II], (UINT32)IQS_IFE2Filter_VWt[II]}}},\r\n}\r\n",
            IQUC_IFE2_Filter[Id].FiltSize, IQUC_IFE2_Filter[Id].FiltEdth.uiEdthHV, IQUC_IFE2_Filter[Id].FiltEdth.uiEdthPN, IQUC_IFE2_Filter[Id].EdgeKernelSize);

// IME
    IQUCLOG("\r\n//////////////////\r\n// IME\r\n//////////////////\r\n\r\n");

    IQUCLOG("IQS_ChromaAdapt = \r\n{\r\n{{%d, %d, %d}, {%d, %d, %d, %d, %d}, {%d, %d, %d, %d, %d}, %d},\r\n}\r\n",
            IQUC_ChromaAdapt[Id].LcaRefWt.LcaRefY, IQUC_ChromaAdapt[Id].LcaRefWt.LcaRefUV, IQUC_ChromaAdapt[Id].LcaRefWt.LcaOutUV,
            IQUC_ChromaAdapt[Id].LcaYinfo.LcaY, IQUC_ChromaAdapt[Id].LcaYinfo.LcaYTh, IQUC_ChromaAdapt[Id].LcaYinfo.LcaYWtPrc, IQUC_ChromaAdapt[Id].LcaYinfo.LcaYWtS, IQUC_ChromaAdapt[Id].LcaYinfo.LcaYWtE,
            IQUC_ChromaAdapt[Id].LcaUVinfo.LcaUV, IQUC_ChromaAdapt[Id].LcaUVinfo.LcaUVTh, IQUC_ChromaAdapt[Id].LcaUVinfo.LcaUVWtPrc, IQUC_ChromaAdapt[Id].LcaUVinfo.LcaUVWtS, IQUC_ChromaAdapt[Id].LcaUVinfo.LcaUVWtE,
            IQUC_ChromaAdapt[Id].SubRatio);

    IQUCLOG("IQS_ChromaAdapt_Color = \r\n{\r\n{%d, {%d, %d}, {%d, %d, %d, %d, %d}},\r\n}\r\n",
            IQUC_ChromaAdapt_Color[Id].LcaCLREn, IQUC_ChromaAdapt_Color[Id].LcaRefWt.LcaCaCentU, IQUC_ChromaAdapt_Color[Id].LcaRefWt.LcaCaCentU,
            IQUC_ChromaAdapt_Color[Id].LcaCLRinfo.LcaCLRRng, IQUC_ChromaAdapt_Color[Id].LcaCLRinfo.LcaCLRTh, IQUC_ChromaAdapt_Color[Id].LcaCLRinfo.LcaCLRWtPrc,
            IQUC_ChromaAdapt_Color[Id].LcaCLRinfo.LcaCLRWtS, IQUC_ChromaAdapt_Color[Id].LcaCLRinfo.LcaCLRWtE);

    IQUCLOG_CLOSE();
    return E_OK;
}

UINT32 IQTUC_DumpRatioTable(UINT32 Id, CHAR *params, CHAR *file_name)
{
    ///////////////////////////////////////////////////
    //*
    //* Sample for New File
    //*
    ///////////////////////////////////////////////////
    /*FST_FILE filehdl = NULL;
    char tmp_buf[1024]={0};
    UINT32 uiBufSize = sizeof(tmp_buf);
    CHAR FileName[20] = {"IQ_TEST.txt"};
    snprintf(tmp_buf, uiBufSize, "A:\\%s", FileName);
    filehdl = FileSys_OpenFile(tmp_buf, FST_CREATE_ALWAYS|FST_OPEN_WRITE);
    if (filehdl)
    {
        snprintf(tmp_buf, uiBufSize, "this is a IQ sample file");
        FileSys_WriteFile(filehdl, (UINT8 *)tmp_buf, &uiBufSize, 0, NULL);
        FileSys_CloseFile(filehdl);
        snprintf(file_name, 20, "%s", FileName);
    }*/

    CHAR FileName[30] = {"IQS_TuningIQTable.txt"};

    extern IQTUC_DUMP_FP IQS_fpIQTUCDump;
    if ( IQS_fpIQTUCDump != NULL )
    {
        (IQS_fpIQTUCDump)(Id);
    }else
        DBG_DUMP("====DUMP ERROR====\r\n");

    snprintf(file_name, 30, "%s", FileName);

    return 0;
}

UINT32 IQUC_SettingFlow(UINT32 Id, UINT32 EngID, ENG_SUBFUNC *pEngSubF)
{
    UINT32 i, base1, base2;
    //IQUC_CTRL_ITEM* pIQUserAdj = IQUC_GetCtrlItem(Id, IQUC_CtrlIndex[Id]);
    DCE_SUBFUNC*  pDce_sub  = pEngSubF->pDce_sub;
    IFE_SUBFUNC*  pIfe_sub  = pEngSubF->pIfe_sub;
    IPE_SUBFUNC*  pIpe_sub  = pEngSubF->pIpe_sub;
    IFE2_SUBFUNC* pIfe2_sub = pEngSubF->pIfe2_sub;
    IME_SUBFUNC*  pIme_sub  = pEngSubF->pIme_sub;

    switch((UINT32)EngID)
    {
        case IQS_ENG_DCE:
            memcpy(IQUC_DCE_GLUT[Id], pDce_sub->pDistortion->GeoLUT.pGGeoLut, sizeof(UINT32)*65);
            for ( i=0; i<65; i++ )
                IQUC_DCE_GLUT[Id][i] = IQUC_CLAMP(IQUC_DCE_GLUT[Id][i]+IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_DISTORTION]*i, 0, 65535);
            IQUC_DCE_Distortion[Id] = *pDce_sub->pDistortion;
            IQUC_DCE_Distortion[Id].GeoLUT.pGGeoLut = (UINT32*)IQUC_DCE_GLUT[Id];
            IQUC_DCE_Distortion[Id].ColorAber.CACGain.Fact_R = IQUC_DCE_Distortion[Id].ColorAber.CACGain.Fact_R + IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_CAC_R];
            IQUC_DCE_Distortion[Id].ColorAber.CACGain.Fact_B = IQUC_DCE_Distortion[Id].ColorAber.CACGain.Fact_B + IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_CAC_B];
            //IQUC_DCE_Distortion[Id].Fov.Gain = IQUC_DCE_Distortion[Id].Fov.Gain + pIQUserAdj->ADJ_ITEM[ADJ_FOV];
            pDce_sub->pDistortion = &IQUC_DCE_Distortion[Id];
            break;

        case IQS_ENG_IFE:
            // IQS_Filter
            memcpy(IQUC_RangeA_Thr[Id], pIfe_sub->pFilter->Range->A->pThres, sizeof(UINT32)*6);
            memcpy(IQUC_RangeB_Thr[Id], pIfe_sub->pFilter->Range->B->pThres, sizeof(UINT32)*6);
            memcpy(IQUC_RangeA_Lut[Id], pIfe_sub->pFilter->Range->A->pThLut, sizeof(UINT32)*17);
            memcpy(IQUC_RangeB_Lut[Id], pIfe_sub->pFilter->Range->B->pThLut, sizeof(UINT32)*17);
            for (i=1; i<6; i++)
            {
                IQUC_RangeA_Thr[Id][i] = IQUC_CLAMP((IQUC_RangeA_Thr[Id][i]+IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_RANGEA_THR]), 0, 1023);
                IQUC_RangeB_Thr[Id][i] = IQUC_CLAMP((IQUC_RangeB_Thr[Id][i]+IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_RANGEB_THR]), 0, 1023);
            }
            for (i=1; i<17; i++)
            {
                IQUC_RangeA_Lut[Id][i] = IQUC_CLAMP((IQUC_RangeA_Lut[Id][i]+IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_RANGEA_LUT]), 0, 1023);
                IQUC_RangeB_Lut[Id][i] = IQUC_CLAMP((IQUC_RangeB_Lut[Id][i]+IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_RANGEB_LUT]), 0, 1023);
            }
            IQUC_Filter[Id] = *pIfe_sub->pFilter;
            IQUC_Filter[Id].Range->A->pThres = IQUC_RangeA_Thr[Id];
            IQUC_Filter[Id].Range->B->pThres = IQUC_RangeB_Thr[Id];
            IQUC_Filter[Id].Range->A->pThLut = IQUC_RangeA_Lut[Id];
            IQUC_Filter[Id].Range->B->pThLut = IQUC_RangeB_Lut[Id];
            IQUC_Filter[Id].Clamp.Th  = IQUC_CLAMP((IQUC_Filter[Id].Clamp.Th +IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_CLAMP_TH]) , 0, 4095);
            IQUC_Filter[Id].Clamp.Mul = IQUC_CLAMP((IQUC_Filter[Id].Clamp.Mul+IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_CLAMP_MUL]), 0, 255);
            IQUC_Filter[Id].Clamp.Dlt = IQUC_CLAMP((IQUC_Filter[Id].Clamp.Dlt+IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_CLAMP_DLT]), 0, 4095);
            IQUC_Filter[Id].Bilat_w   = IQUC_CLAMP((IQUC_Filter[Id].Bilat_w  +IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_BILAT_W])  , 0, 15);
            IQUC_Filter[Id].Rth_W     = IQUC_CLAMP((IQUC_Filter[Id].Rth_W    +IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_RTH_W])    , 0, 15);
            pIfe_sub->pFilter = &IQUC_Filter[Id];

            // IQS_Vig
            if (pIfe_sub->pVig != NULL)
            {
                memcpy(IQUC_VIG_R[Id], (UINT32*)pIfe_sub->pVig->R_LUT_TblAddr, sizeof(UINT32)*17);
                memcpy(IQUC_VIG_G[Id], (UINT32*)pIfe_sub->pVig->G_LUT_TblAddr, sizeof(UINT32)*17);
                memcpy(IQUC_VIG_B[Id], (UINT32*)pIfe_sub->pVig->B_LUT_TblAddr, sizeof(UINT32)*17);
                for (i=0; i<17; i++)
                {
                    IQUC_VIG_R[Id][i] = IQUC_CLAMP((IQUC_VIG_R[Id][i]+IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_VIG_R]*i), 0, 1023);
                    IQUC_VIG_G[Id][i] = IQUC_CLAMP((IQUC_VIG_G[Id][i]+IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_VIG_G]*i), 0, 1023);
                    IQUC_VIG_B[Id][i] = IQUC_CLAMP((IQUC_VIG_B[Id][i]+IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_VIG_B]*i), 0, 1023);
                }
                pIfe_sub->pVig->R_LUT_TblAddr = (UINT32)IQUC_VIG_R[Id];
                pIfe_sub->pVig->G_LUT_TblAddr = (UINT32)IQUC_VIG_G[Id];
                pIfe_sub->pVig->B_LUT_TblAddr = (UINT32)IQUC_VIG_B[Id];
            }

            // IQS_Gbal
            IQUC_Gbal[Id] = *pIfe_sub->pGbal;
            IQUC_Gbal[Id].pGbalDtab[0] = IQUC_CLAMP(IQUC_Gbal[Id].pGbalDtab[0]+IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_GBAL_DTAB], 0, 15);
            IQUC_Gbal[Id].GbalThr = IQUC_CLAMP(IQUC_Gbal[Id].GbalThr+IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_GBALTHR], 0, 1023);
            pIfe_sub->pGbal = &IQUC_Gbal[Id];

            // IQS_Outl
            memcpy(IQUC_Outl_Bri[Id],  pIfe_sub->pOutl->pBrightThres, sizeof(UINT32)*5);
            memcpy(IQUC_Outl_Dark[Id], pIfe_sub->pOutl->pDarkThres,   sizeof(UINT32)*5);
            base1 = IQUC_Outl_Bri[Id][0];
            base2 = IQUC_Outl_Dark[Id][0];
            if (base1 == 0) base1 = 1;
            if (base2 == 0) base2 = 1;
            for (i=0; i<5; i++)
            {
                IQUC_Outl_Bri[Id][i]  = IQUC_CLAMP((IQUC_Outl_Bri[Id][i] *(base1+IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_OUTL_BRI]))/base1,  0, 4095);
                IQUC_Outl_Dark[Id][i] = IQUC_CLAMP((IQUC_Outl_Dark[Id][i]*(base2+IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_OUTL_DARK]))/base2, 0, 4095);
            }
            pIfe_sub->pOutl->pBrightThres = IQUC_Outl_Bri[Id];
            pIfe_sub->pOutl->pDarkThres = IQUC_Outl_Dark[Id];
            break;

        case IQS_ENG_IPE:
            // IQS_EdgeEnh
            IQUC_EdgeEnh[Id] = *pIpe_sub->pedgeEnh;
            IQUC_EdgeEnh[Id].EnhP = IQUC_CLAMP((INT32)(IQUC_EdgeEnh[Id].EnhP+IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_ENHP]), 0, 1023);
            IQUC_EdgeEnh[Id].EnhN = IQUC_CLAMP((INT32)(IQUC_EdgeEnh[Id].EnhN+IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_ENHN]), 0, 1023);
            pIpe_sub->pedgeEnh = &IQUC_EdgeEnh[Id];

            // IQS_EDMap & IQS_ESMap
            memcpy(IQUC_EDtab[Id], (void *)pIpe_sub->pedgeMap->edMap.TAB_TblAddr, sizeof(UINT8)*16);
            memcpy(IQUC_EStab[Id], (void *)pIpe_sub->pedgeMap->esMap.TAB_TblAddr, sizeof(UINT8)*16);
            for (i=0; i<4; i++)
            {
                IQUC_EDtab[Id][i] = IQUC_CLAMP((IQUC_EDtab[Id][i]+(IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_EDTAB_DARK]*(4-i))/4), 0, 255);
                IQUC_EStab[Id][i] = IQUC_CLAMP((IQUC_EStab[Id][i]+(IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_ESTAB_DARK]*(4-i))/4), 0, 255);
                IQUC_EDtab[Id][i+12] = IQUC_CLAMP((IQUC_EDtab[Id][i+12]+(IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_EDTAB_LIGHT]*(i+1))/4), 0, 255);
                IQUC_EStab[Id][i+12] = IQUC_CLAMP((IQUC_EStab[Id][i+12]+(IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_ESTAB_LIGHT]*(i+1))/4), 0, 255);
            }
            IQUC_EDMap[Id] = pIpe_sub->pedgeMap->edMap;
            IQUC_EDMap[Id].EthrL = IQUC_CLAMP(IQUC_EDMap[Id].EthrL+IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_EDMAP_ETHRL], 0, 1023);
            IQUC_EDMap[Id].EtabL = IQUC_CLAMP(IQUC_EDMap[Id].EtabL+IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_EDMAP_ETABL], 0, 7);
            pIpe_sub->pedgeMap->edMap.TAB_TblAddr = (UINT32)IQUC_EDtab[Id];
            pIpe_sub->pedgeMap->esMap.TAB_TblAddr = (UINT32)IQUC_EStab[Id];
            pIpe_sub->pedgeMap->edMap = IQUC_EDMap[Id];

            // IQS_EWParam
            IQUC_EWParam[Id] = *pIpe_sub->pedgeExt->pEWParam;
            IQUC_EWParam[Id].uiEWDir0 = IQUC_CLAMP(IQUC_EWParam[Id].uiEWDir0+IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_EWDIR0], 0, 31);
            IQUC_EWParam[Id].uiEWDir1 = IQUC_CLAMP(IQUC_EWParam[Id].uiEWDir1+IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_EWDIR1], 0, 31);
            IQUC_EWParam[Id].uiEWDir2 = IQUC_CLAMP(IQUC_EWParam[Id].uiEWDir2+IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_EWDIR2], 0, 31);
            IQUC_EWParam[Id].DynEMEn  = IQUC_CLAMP(IQUC_EWParam[Id].DynEMEn+IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_DYNEMAP], 0, 1);
            pIpe_sub->pedgeExt->pEWParam = &IQUC_EWParam[Id];

            // IQS_DirScoreTh
            IQUC_DirScoreTh[Id] = *pIpe_sub->pedgeExt->ScrCalThTab;
            IQUC_DirScoreTh[Id].uiScoreTh0 = IQUC_CLAMP(IQUC_DirScoreTh[Id].uiScoreTh0+IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_SCORETH0], 0, 255);
            IQUC_DirScoreTh[Id].uiScoreTh1 = IQUC_CLAMP(IQUC_DirScoreTh[Id].uiScoreTh1+IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_SCORETH1], 0, 255);
            IQUC_DirScoreTh[Id].uiScoreTh2 = IQUC_CLAMP(IQUC_DirScoreTh[Id].uiScoreTh2+IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_SCORETH2], 0, 255);
            IQUC_DirScoreTh[Id].uiScoreTh3 = IQUC_CLAMP(IQUC_DirScoreTh[Id].uiScoreTh3+IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_SCORETH3], 0, 255);
            pIpe_sub->pedgeExt->ScrCalThTab = &IQUC_DirScoreTh[Id];

            // Saturation
            IQUC_CCtrl[Id] = *pIpe_sub->pCctrl;
            IQUC_CCtrl[Id].SatOfs = IQUC_CLAMP(IQUC_CCtrl[Id].SatOfs+IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_SATURATION], -128, 127);
            IQUC_CCtrl[Id].uiVdetDiv = IQUC_CLAMP(IQUC_CCtrl[Id].uiVdetDiv+IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_VDETDIV], 0, 127);
            pIpe_sub->pCctrl = &IQUC_CCtrl[Id];

            // IQS_FStab
            memcpy(IQUC_FStab[Id], (void *)pIpe_sub->pCC->Fstab_TblAddr, sizeof(UINT8)*16);
            for (i=0; i<4; i++)
            {
                IQUC_FStab[Id][i] = IQUC_CLAMP((IQUC_FStab[Id][i]+(IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_FSTAB_DARK]*(4-i))/4), 0, 255);
                IQUC_FStab[Id][i+12] = IQUC_CLAMP((IQUC_FStab[Id][i+12]+(IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_FSTAB_LIGHT]*(i+1))/4), 0, 255);
            }
            pIpe_sub->pCC->Fstab_TblAddr = (UINT32)IQUC_FStab[Id];

            // Contrast
            IQUC_YCCCon[Id] = *pIpe_sub->pYCCcon;
            IQUC_YCCCon[Id].YCon  = IQUC_CLAMP((IQUC_YCCCon[Id].YCon+IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_CONTRAST]),  0, 255);
            IQUC_YCCCon[Id].UVCon = IQUC_CLAMP((IQUC_YCCCon[Id].UVCon+IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_CONTRAST]), 0, 255);
            pIpe_sub->pYCCcon = &IQUC_YCCCon[Id];

            // IQS_RGBLpf
            IQUC_RGBLpf[Id] = *pIpe_sub->prgblpf;
            IQUC_RGBLpf[Id].R.lpfw = IQUC_CLAMP((IQUC_RGBLpf[Id].R.lpfw+IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_RGBLPF_LPFW]), 0, 15);
            IQUC_RGBLpf[Id].G.lpfw = IQUC_CLAMP((IQUC_RGBLpf[Id].G.lpfw+IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_RGBLPF_LPFW]), 0, 15);
            IQUC_RGBLpf[Id].B.lpfw = IQUC_CLAMP((IQUC_RGBLpf[Id].B.lpfw+IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_RGBLPF_LPFW]), 0, 15);
            pIpe_sub->prgblpf = &IQUC_RGBLpf[Id];
            break;

        case IQS_ENG_IFE2:
            // IQS_RefCenter_Y & IQS_RefCenter_UV
            IQUC_RefCenter_Y[Id] = pIfe2_sub->pRefcent->RefCentY;
            IQUC_RefCenter_UV[Id] = pIfe2_sub->pRefcent->RefCentUV;
            IQUC_RefCenter_Y[Id].uiCntWt = IQUC_CLAMP(IQUC_RefCenter_Y[Id].uiCntWt+IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_IFE2_Y_CWT], 0, 31);
            IQUC_RefCenter_UV[Id].uiCntWt = IQUC_CLAMP(IQUC_RefCenter_UV[Id].uiCntWt+IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_IFE2_UV_CWT], 0, 31);
            pIfe2_sub->pRefcent->RefCentY = IQUC_RefCenter_Y[Id];
            pIfe2_sub->pRefcent->RefCentUV = IQUC_RefCenter_UV[Id];

            // IQS_IFE2Filter
            IQUC_IFE2_Filter[Id] = *pIfe2_sub->pFilter;
            IQUC_IFE2_Filter[Id].FiltEdth.uiEdthHV = IQUC_CLAMP(IQUC_IFE2_Filter[Id].FiltEdth.uiEdthHV+IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_IFE2_ED_HV_TH], 0, 255);
            IQUC_IFE2_Filter[Id].FiltEdth.uiEdthPN = IQUC_CLAMP(IQUC_IFE2_Filter[Id].FiltEdth.uiEdthPN+IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_IFE2_ED_PN_TH], 0, 255);
            pIfe2_sub->pFilter = &IQUC_IFE2_Filter[Id];
            break;

        case IQS_ENG_IME:
            // IQS_ChromaAdapt
            IQUC_ChromaAdapt[Id] = *pIme_sub->ChromaAdap.pChromaAdaption;
            IQUC_ChromaAdapt[Id].LcaRefWt.LcaOutUV = IQUC_CLAMP(IQUC_ChromaAdapt[Id].LcaRefWt.LcaOutUV+IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_CHRA_OUT_WT], 0, 31);
            pIme_sub->ChromaAdap.pChromaAdaption = &IQUC_ChromaAdapt[Id];

            // IQS_ChromaAdapt_Color
            IQUC_ChromaAdapt_Color[Id] = *pIme_sub->ChromaAdap.pChromaColorAdaption;
            IQUC_ChromaAdapt_Color[Id].LcaCLRinfo.LcaCLRTh = IQUC_CLAMP(IQUC_ChromaAdapt_Color[Id].LcaCLRinfo.LcaCLRTh+IQUC_CtrlItem[Id][IQUC_CtrlIndex[Id]].ADJ_ITEM[ADJ_CHRA_CA_TH], 0, 15);
            pIme_sub->ChromaAdap.pChromaColorAdaption = &IQUC_ChromaAdapt_Color[Id];
            break;

        default:
            break;
    }
    return E_OK;
}

