/**
    Motion Detection Algorithm.

    depend on CA status to detect the object motion

    @file       md_alg.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include "Type.h"
#include "md_sample_int.h"
#include "md_api.h"
#include "md_alg.h"
#include "IPL_AlgInfor.h"
#include "IPL_Cmd.h"
//#NT#2016/02/26#Harry Tsai -begin
//#NEW, calculate Lum from IME output for CCIR-in
#include "IPL_Display.h"
#include "Sensor.h"
//#NT#2016/02/26#Harry Tsai -end


#define __MODULE__ MDALG
#define __DBGLVL__ 1        //ERROR mode, show err, wrn only
#define __DBGFLT__ "*"      //*=All
#include "DebugModule.h"

UINT32 MD_LevelMap[MD_LEVEL_MAX] = {50,60,70,80,90,100,110,120,130,140,150};

static UINT16 MDAccLum1[MD_LAWIN_TOTAL];
static UINT16 MDAccLum2[MD_LAWIN_TOTAL];
static UINT16 MDAccHisto[MD_LAWIN_TOTAL];
MDALG_INFO MdInfo[MD_PROC_CNT] = {0};

UINT32 MD_BlkDiff_Thr[MD_PROC_CNT] = {MD_BLKDIFF_THR, MD_BLKDIFF_THR};
UINT32 MD_BlkCnt_Thr[MD_PROC_CNT] = {MD_BLKCNT_THR, MD_BLKCNT_THR};
UINT32 MD_DiffSum_Thr[MD_PROC_CNT] = {MD_DIFFSUM_THR, MD_DIFFSUM_THR};
UINT32 gMDRatio[MD_PROC_CNT] = {0};
BOOL MD_Det_Win_Init[MD_PROC_CNT] = {FALSE, FALSE};
UINT32 gMD_Level = MD_LEVEL_NORMAL;

UINT8 MD_DetWin[MD_DETWIN_Y][MD_DETWIN_X] = {0};

//#NT#2016/10/05#Harry Tsai -begin
//#NEW, Add for ROI's reference
UINT32 MD_BlkDiff_Thr_Ref[MD_PROC_CNT] = {MD_BLKDIFF_THR, MD_BLKDIFF_THR};
//#NT#2016/10/05#Harry Tsai -end


void MD_SetDetWin(UINT32 Id, UINT8 *win)
{
    if (MD_Det_Win_Init[Id] == FALSE)
    {
        memset(MD_DetWin, 1, sizeof(UINT8)*MD_DETWIN_Y*MD_DETWIN_X);
        MD_Det_Win_Init[Id] = TRUE;
    }

    memcpy(MD_DetWin, win, sizeof(UINT8)*MD_DETWIN_Y*MD_DETWIN_X);

    //#NT#2016/09/20#Charlie Chang -begin
    // call MD_Process for correct first frame detect error
    MD_Process(Id);
    //#NT#2016/09/20#Charlie Chang -end
}

void MD_SetLevel(UINT32 Id, MD_LEVEL level)
{
    UINT32 MDLevel;

    if (level >= MD_LEVEL_MAX)
    {
        DBG_ERR("Id: %d, Level %d Overflow, force set to MD_LEVEL_NORMAL", Id, level);
        level = MD_LEVEL_NORMAL;
    }

    gMD_Level = level;
    MDLevel = MD_LevelMap[level];

    MD_BlkDiff_Thr[Id] = MD_BLKDIFF_THR * 100 / MDLevel;
    MD_BlkCnt_Thr[Id] = MD_BLKCNT_THR * 100 / MDLevel;
    MD_DiffSum_Thr[Id] = MD_DIFFSUM_THR * 100 / MDLevel;
}

static void MD_getLA(UINT32 Id, UINT16 *Lum1, UINT16 *Lum2, UINT16 *Histo, UINT32 *WinXNum, UINT32 *WinYNum)
{
    IPL_LA_INFOR LAInfo;
    IPL_LA_SET_INFOR LASetInfo = {0};
    UINT16 i, j;
    UINT32 Ratio_X, Ratio_Y;
    UINT32 WinCnt = 0;

    if (MD_Det_Win_Init[Id] == FALSE)
    {
        memset(MD_DetWin, 1, sizeof(UINT8)*MD_DETWIN_Y*MD_DETWIN_X);
        MD_Det_Win_Init[Id] = TRUE;
    }

    LASetInfo.Id = Id;
    IPL_GetCmd(IPL_GET_LA_INFOR, (void *)&LASetInfo);

    if (LASetInfo.WinXNum >= MD_DETWIN_X)
    {
        Ratio_X = LASetInfo.WinXNum/MD_DETWIN_X;
        Ratio_Y = LASetInfo.WinYNum/MD_DETWIN_Y;
    }
    else
    {
        if(LASetInfo.WinXNum != 0)
            Ratio_X = MD_DETWIN_X/LASetInfo.WinXNum;
        else
        {
            DBG_ERR("LASetInfo.WinXNum is zero!!\r\n");
            Ratio_X = 1;
        }

        if(LASetInfo.WinYNum != 0)
            Ratio_Y = MD_DETWIN_Y/LASetInfo.WinYNum;
        else
        {
            DBG_ERR("LASetInfo.WinYNum is zero!!\r\n");
            Ratio_Y = 1;
        }
            
    }

    LAInfo.Id = Id;
    LAInfo.Lum1 = Lum1;
    LAInfo.Lum2 = Lum2;
    LAInfo.Histo = Histo;
    IPL_GetCmd(IPL_GET_CUR_LA_INFOR, (void *)&LAInfo);

    for (j = 0; j < LASetInfo.WinYNum; j ++)
    {
        for (i = 0; i < LASetInfo.WinXNum; i ++)
        {
            if (MD_DetWin[j/Ratio_Y][i/Ratio_X] > 1)
                MD_DetWin[j/Ratio_Y][i/Ratio_X] = 1;

            Lum1[j * LASetInfo.WinXNum + i] = ((Lum1[j * LASetInfo.WinXNum + i] >> (12-MD_ACC_BIT)) * MD_DetWin[j/Ratio_Y][i/Ratio_X]);
            Lum2[j * LASetInfo.WinXNum + i] = ((Lum2[j * LASetInfo.WinXNum + i] >> (10-MD_ACC_BIT)) * MD_DetWin[j/Ratio_Y][i/Ratio_X]);
            WinCnt += MD_DetWin[j/Ratio_Y][i/Ratio_X];
        }
    }

    if((LASetInfo.WinXNum != 0) && (LASetInfo.WinYNum != 0))
        gMDRatio[Id] = ((WinCnt * MD_NORMAL_BASE) / (LASetInfo.WinXNum * LASetInfo.WinYNum));
    else
    {
        DBG_ERR("LASetInfo.WinXNum = %d, LASetInfo.WinYNum =%d!!\r\n");
        gMDRatio[Id] = MD_NORMAL_BASE;
    }
    
    *WinXNum = LASetInfo.WinXNum;
    *WinYNum = LASetInfo.WinYNum;
}

//#NT#2016/02/26#Harry Tsai -begin
//#NEW, calculate Lum from IME output for CCIR-in
static UINT16 BlockAverageY(UINT32 YAddr,UINT32 x_start,UINT32 y_start,UINT32 width,UINT32 height,UINT32 offset)
{
    UINT16    i,j;
    UINT32    Yall=0,YAvg=0,Ypixel=0,cnt=0,tmp=0;

    for(j=0;j<height;j+=4)
    {
        for(i=0;i<width;i+=16)
        {
            tmp = *(volatile UINT32 *)(YAddr+(offset*(y_start+j))+4*((x_start+i)/4));
            Ypixel=(tmp    )&(0xff);
            Yall+=Ypixel;
            Ypixel=(tmp>>8 )&(0xff);
            Yall+=Ypixel;
            Ypixel=(tmp>>16)&(0xff);
            Yall+=Ypixel;
            Ypixel=(tmp>>24)&(0xff);
            Yall+=Ypixel;
            cnt+=4;
        }
    }

    if(cnt != 0)
        YAvg = Yall/cnt;
    else
        YAvg = 0;
    
    return (UINT16)(YAvg);
}

static void MD_getImeY(UINT32 Id, UINT16 *Lum1, UINT16 *Lum2, UINT16 *Histo, UINT32 *WinXNum, UINT32 *WinYNum)
{
    UINT16 i,j,Lum,Idx;
    UINT32 h_size, v_size, offset;
    IPL_YCC_IMG_INFO FB_para;

    //get IME ready buffer address
    FB_para = IPL_GetIMEReadyBufferAddr(1,Id);

    h_size = FB_para.Ch[0].Width;
    v_size = FB_para.Ch[0].Height;
    offset = FB_para.Ch[0].LineOfs;

    for(j=0;j<MD_LAWIN_Y;j++)
    {
        for(i=0;i<MD_LAWIN_X;i++)
        {
            Idx = j*MD_DETWIN_X+i;
            Lum = BlockAverageY(FB_para.PixelAddr[0],((i*h_size)/MD_LAWIN_X),((j*v_size)/MD_LAWIN_Y),(h_size/MD_LAWIN_X),(v_size/MD_LAWIN_Y),offset);
            Lum1[Idx] = Lum;
            Lum2[Idx] = Lum;
        }
    }

    gMDRatio[Id] = (MD_NORMAL_BASE);
    
    *WinXNum = MD_LAWIN_X;
    *WinYNum = MD_LAWIN_Y;
}
//#NT#2016/02/26#Harry Tsai -end


BOOL MD_Process(UINT32 Id)
{
//#NT#2016/05/25#Harry_Tsai -begin
    BOOL rslt;// = TRUE;
//#NT#2016/05/25#Harry_Tsai -end
    UINT32 WinXNum, WinYNum;
    //#NT#2016/02/26#Harry Tsai -begin
    //#NEW, calculate Lum from IME output for CCIR-in
    ER rt = E_OK;
    SENSOR_INFO Info = {0};

    rt = Sensor_GetStatus(IPL_UTI_CONV2_SEN_ID(Id), IPL_AlgGetUIInfo(Id, IPL_SEL_PRVSENMODE), &Info);
    if (rt != E_OK)
        return FALSE;

    //#NT#2016/03/01#Harry Tsai -begin
    //#FIXED, Coverity CID 50899
    if(Info.Mode == NULL)
        return FALSE;
    //#NT#2016/03/01#Harry Tsai -end

    if (Info.Mode->ModeType != SENSOR_MODE_CCIR && Info.Mode->ModeType != SENSOR_MODE_CCIR_INTERLACE)
        MD_getLA(Id, MDAccLum1, MDAccLum2, MDAccHisto, &WinXNum, &WinYNum);
    else
        MD_getImeY(Id, MDAccLum1, MDAccLum2, MDAccHisto, &WinXNum, &WinYNum);
    //#NT#2016/02/26#Harry Tsai -end

    MdInfo[Id].CAWinX = WinXNum;
    MdInfo[Id].CAWinY = WinYNum;
    MdInfo[Id].AccTab[MDALG_CH_R] = MDAccLum1;
    MdInfo[Id].AccTab[MDALG_CH_G] = MDAccLum1;
    MdInfo[Id].AccTab[MDALG_CH_B] = MDAccLum1;
    MdInfo[Id].BlockDiffThres = MD_BlkDiff_Thr[Id];
    MdInfo[Id].BlockCntThres = (MD_BlkCnt_Thr[Id] * gMDRatio[Id]) / MD_NORMAL_BASE;
    MdInfo[Id].TotalDiffThres = (MD_DiffSum_Thr[Id] * gMDRatio[Id]) / MD_NORMAL_BASE;
    //#NT#2016/10/05#Harry Tsai -begin
    //#NEW, Add for ROI's reference
    MdInfo[Id].BlockDiffThres_Ref = MD_BlkDiff_Thr_Ref[Id];
    //#NT#2016/10/05#Harry Tsai -end

    rslt = MDAlg_Start(Id, (UINT32)NULL, (UINT32)NULL, MdInfo[Id]);

    if (rslt == TRUE )
        DBG_IND("^GObject Motion\r\n");

    return rslt;
}

void MD_GetInfo(MD_INFO *Info)
{
    Info->Level = gMD_Level;
    Info->WeightWin = &MD_DetWin[0][0];
}
