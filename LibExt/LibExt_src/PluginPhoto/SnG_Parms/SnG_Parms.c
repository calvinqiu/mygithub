#include    "Type.h"
#include    "IPL_Utility.h"
#include    "IPL_Cmd.h"
#include    "HwMem.h"
#include    "FileSysTsk.h"
#include    "stdio.h"
#include    "GPS.h"
#include    "SnG_lib.h"
#include    "dis_lib.h"
#include    "dis_alg.h"
#include    "SnG_Parms.h"
#include    "adas_lib.h"
#include    "SnG_Parms_int.h"
#include    "rtc.h"


#define THIS_DBGLVL         2 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SnG_Parms
#define __DBGLVL__          THIS_DBGLVL
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

#if SNG_DEBUG
#define SNG_BUF_SIZE                    25*1024*1024
#else
#define SNG_BUF_SIZE                    1024
#endif
#define ADAS_ABS(a)                     ((a)>=0 ? a:-(a))

// SnG
#define SpecificVal                     1024


// System time
RTC_TIME                 g_time;
RTC_DATE                 g_date;
UINT32                   g_CurTime;

// ADAS Parms
ADAS_RESULT_INFO         g_AdasRslt_SnG;


// SnG variables
UINT32                   g_uiSnGParmsRate;
static UINT32            g_uiSnGParmsCount = 0;
UINT32                   g_uiSnGWorkBufAddr, g_DisMvOutAddr, g_DisMvOutAddrCur;
UINT32                   g_FrmCount = 0, g_FrmCnt2SnG = 0;
STOPNGO_IMGINFO          g_ImgMvInfo;
STOPNGO_RSLTINFO         g_StopNGoRsltStatus;
static UINT32            g_SaveDisMvFlg = FALSE;
UINT32                   g_saveCnt = 1;
UINT32                   g_ValidFlag = 0;
UINT32                   g_stopngoRslt = 0;
UINT32                   g_StopCnt = 0;
UINT32                   g_StopTimeCnt = 0;
UINT32                   g_ValidBlkNum = 0;
UINT32                   g_VoteNum = 0;
UINT32                   g_StopTimeDuration = 0;
UINT32                   g_cadNum0, g_cadNum1, g_cadNum2, g_cadNum3, g_cadNum4;
UINT32                   ipe_idx;
BOOL                     g_firstSetdateInfo = TRUE;

// DIS parameters
UINT32                   g_DisMvOutAddrCur;
static MOTION_INFOR      g_DisMv[MVNUMMAX] = {0};
static MV_INFO_SIZE      g_DisMvInfo;
//static UINT32            g_DisCorrectBlkV = 23;
DISALG_PARAM             g_DisInputInfo;

// MovieStamp
UINT32                   g_SnGStartTime = 0, g_SnGEndTime = 0, g_SnGdeltaTime = 0;


extern ADAS_CBMSG_FP     g_AdasCbFP;


UINT32 SnGParms_CalcBuffSize(void)
{
    UINT32 BuffSize;

    BuffSize = SNG_BUF_SIZE;//third party required

    return BuffSize;
}

INT32 SnGParms_Init(MEM_RANGE *buf, MEM_RANGE *cachebuf, UINT32 ProcessRate)
{
    if (!buf)
    {
        DBG_ERR("buf is NULL\r\n");
        return SNGPARMS_STA_ERROR;
    }
    if (!buf->Addr)
    {
        DBG_ERR("bufAddr is NULL\r\n");
        return SNGPARMS_STA_ERROR;
    }
    if (buf->Size < SnGParms_CalcBuffSize())
    {
        DBG_ERR("bufSize %d < need %d\r\n",buf->Size,SnGParms_CalcBuffSize());
        return SNGPARMS_STA_ERROR;
    }

    g_uiSnGWorkBufAddr = buf->Addr; // non-cache buffer
    g_DisMvOutAddr = g_uiSnGWorkBufAddr;
    g_DisMvOutAddrCur = g_DisMvOutAddr;


    g_uiSnGParmsRate = ProcessRate;

    StopNGo_Init();

    memset(&g_AdasRslt_SnG.SnGInfo, 0, sizeof(STOPNGO_RSLTINFO));


    return SNGPARMS_STA_OK;
}

INT32 SnGParms_Process(void)
{
    g_uiSnGParmsCount++;

    if((g_uiSnGParmsCount%g_uiSnGParmsRate)==0)
    {
        SnGParms_Detection();//The main detection function
    }

    return SNGPARMS_STA_OK;
}


void SnGParms_Detection(void)//The main LDWS Detection code
{
    UINT32 i;
    UINT32 IniFrm = 1;
    MDS_DIM mdsDim;
    UINT32 MvValidChk;
    UINT32 Time;
    //UINT32 OutLength;
    //FST_FILE filehdl;
    //char filename[50];

    //DBG_DUMP("SnG CurrentTime = %d\r\n",Perf_GetCurrent());

    g_SnGStartTime = Perf_GetCurrent();

    g_FrmCount++;

    g_FrmCnt2SnG++;

    if (g_firstSetdateInfo)
    {
        g_time = rtc_getTime();
        g_date = rtc_getDate();
        g_CurTime = Perf_GetCurrent();

        g_firstSetdateInfo = FALSE;
    }


    g_DisMvInfo.motVec = g_DisMv;
    dis_getReadyMotionVec(&g_DisMvInfo);
    dis_getInputInfo(&g_DisInputInfo);

    //DBG_DUMP("^Yg_FrmCount = %d, g_FrmCnt2SnG = %d, g_SaveDisMvFlg = %d sie_frame_cnt = %d\r\n",g_FrmCount, g_FrmCnt2SnG, g_SaveDisMvFlg, g_DisInputInfo.frame_cnt);


    //DBG_DUMP("DIS source width = %d, height = %d\r\n",g_DisInputInfo.InSizeH,g_DisInputInfo.InSizeV);
    //DBG_DUMP("numH = %d, numV = %d\r\n",g_DisMvInfo.uiNumH, g_DisMvInfo.uiNumV);
#if 0
    for(y=12; y<g_DisMvInfo.uiNumV; y++)
    {
        for(x=0; x<g_DisMvInfo.uiNumH-15; x++)
        {
            DBG_DUMP("%d ",g_DisMv[y*g_DisMvInfo.uiNumH+x].uiCnt);
        }
        DBG_DUMP("\r\n");
    }
#endif


#if 1
    if (g_FrmCount >= IniFrm) // copy IPE edge map
    {
        mdsDim = dis_getMDSDim();

        //DBG_DUMP("uiMdsNum = %d, uiBlkNumH = %d, uiBlkNumV = %d\r\n",mdsDim.uiMdsNum, mdsDim.uiBlkNumH, mdsDim.uiBlkNumV);
#if 0
        if (mdsDim.uiBlkNumV != g_DisCorrectBlkV)
        {
            //DBG_DUMP("^Gmismatch!!\r\n");
            SnGParms_dumpMvInfo2dram2(mdsDim);
        }
        else
        {
            SnGParms_dumpMvInfo2dram(mdsDim);
        }
#endif

        #if 0
        if ((g_FrmCnt2SnG%20) == 0)
        {
            if ( (g_DisInputInfo.InSizeH > 0) && (g_DisInputInfo.InSizeV > 0) )
            {
                //DBG_DUMP("ipe_addr = 0x%08x, SizeH = %d, SizeV = %d\tr\n",g_DisInputInfo.InAdd0, g_DisInputInfo.InSizeH, g_DisInputInfo.InSizeV);
                // dump IPE edge map
                OutLength = g_DisInputInfo.InSizeH*g_DisInputInfo.InSizeV/4;
                //sprintf(filename ,"A:\\IPE_EDG_20150910\\EipeEdge_frm%d.bin",g_FrmCnt2SnG);
                sprintf(filename ,"A:\\SNG_DebugInfo_%d_%d_%d\\EipeEdge_frm%d.bin",g_date, g_time, g_CurTime,g_FrmCnt2SnG);
                filehdl = FileSys_OpenFile( filename, FST_CREATE_ALWAYS|FST_OPEN_WRITE );
                FileSys_WriteFile( filehdl, (UINT8*)g_DisInputInfo.InAdd0, &OutLength, 0, NULL );
                FileSys_CloseFile( filehdl );
            }
        }
        #endif


        // run stop&go algorithm
        if (1)//(mdsDim.uiBlkNumV == g_DisCorrectBlkV)
        {
            #if 0
            if (mdsDim.uiBlkNumV != g_DisCorrectBlkV)
            {
                DBG_DUMP("DIS motion block V number %d != 23\r\n",mdsDim.uiBlkNumV);
            }
            #endif
            g_ImgMvInfo.uiEdgImgHeight = g_DisInputInfo.InSizeV;
            g_ImgMvInfo.uiEdgImgWidth= g_DisInputInfo.InSizeH;
            g_ImgMvInfo.uiMvBlkNumH = mdsDim.uiBlkNumH * mdsDim.uiMdsNum;
            g_ImgMvInfo.uiMvBlkNumV = mdsDim.uiBlkNumV;



            MvValidChk = 0;
            for(i = 0;i<g_ImgMvInfo.uiMvBlkNumV*g_ImgMvInfo.uiMvBlkNumH;i++)
            {
                if (g_DisMv[i].uiSad != 0)
                {
                    MvValidChk = 1;
                    break;
                }
            }

            if (MvValidChk == 1) // motion is valid
            {
                //DBG_DUMP("MvValidChk = %d\r\n ",MvValidChk);
                Perf_Mark();
                StopNGo_Detection(g_DisMv, g_ImgMvInfo, 30, 40, 3, 3, 15, g_FrmCnt2SnG,10*1000);
                Time = Perf_GetDuration();

                g_StopNGoRsltStatus = StopNGo_getStopNGoStatus();
#if 0
                if (g_StopNGoRsltStatus.uiMvValid == STOPNGO_MV_VALID)
                    g_ValidFlag = 1;
                else
                    g_ValidFlag = 0;
#endif

                if (g_StopNGoRsltStatus.uiMovingStatus== STOPNGO_MOVING)
                    g_stopngoRslt = 0; //moving
                if (g_StopNGoRsltStatus.uiStopStatus == STOPNGO_STOP)
                    g_stopngoRslt = 1; //stop
                if (g_StopNGoRsltStatus.uiGoStatus== STOPNGO_GO)
                    g_stopngoRslt = 2; //go
#if 0
                g_ValidBlkNum = g_StopNGoRsltStatus.uiValidBlkNum;
                g_StopCnt = g_StopNGoRsltStatus.uiStopCnt;
                g_StopTimeCnt = g_StopNGoRsltStatus.uiStopTimeCnt;
                g_VoteNum = g_StopNGoRsltStatus.uiVoteNum;
                g_cadNum0 = g_StopNGoRsltStatus.uicadNum0;
                g_cadNum1 = g_StopNGoRsltStatus.uicadNum1;
                g_cadNum2 = g_StopNGoRsltStatus.uicadNum2;
                g_cadNum3 = g_StopNGoRsltStatus.uicadNum3;
                g_cadNum4 = g_StopNGoRsltStatus.uicadNum4;
                g_StopTimeDuration = g_StopNGoRsltStatus.uiTimeDuration;
#endif

                memcpy(&g_AdasRslt_SnG.SnGInfo, &g_StopNGoRsltStatus, sizeof(STOPNGO_RSLTINFO));


                if(g_AdasCbFP)
                {
                    g_AdasCbFP(ADAS_CBMSG_SNG, (void *)&g_AdasRslt_SnG);
                }


            }else{
                //DBG_DUMP("^GMvValidChk = 0\r\n");
            }

        }else{
            //DBG_DUMP("^Gskip stop&go algorithm!!\r\n");
        }
    }
#endif

#if 1
    if (g_SaveDisMvFlg)
    {
        //SnGParms_dumpMvInfo2file();
        g_SaveDisMvFlg = FALSE;
        g_FrmCount = 0;
        g_DisMvOutAddrCur = g_DisMvOutAddr;
        g_saveCnt++;
    }
#endif

    ipe_idx = g_FrmCnt2SnG;

    g_SnGEndTime = Perf_GetCurrent();
    g_SnGdeltaTime = (g_SnGEndTime > g_SnGStartTime)?(g_SnGEndTime - g_SnGStartTime):(0xFFFFFFFF-g_SnGEndTime + g_SnGStartTime + 1);


}
#if 0
void SnGParms_dumpMvInfo2dram2(MDS_DIM mdsDim)
{
    UINT32 ix,iy;
    UINT32 mov_wid = mdsDim.uiBlkNumH*mdsDim.uiMdsNum;
    UINT32 uiIdx = 0;
    UINT32 DisMvBufAddrEnd;


    DisMvBufAddrEnd = g_DisMvOutAddrCur;

    // IPE idx
    for(iy=0; iy<1; iy++)
    {
        DisMvBufAddrEnd += sprintf((char *)DisMvBufAddrEnd, "\t");
        for(ix=0; ix<mov_wid; ix++)
        {
            uiIdx = iy*mov_wid + ix;
            DisMvBufAddrEnd += sprintf((char *)DisMvBufAddrEnd, "%d\t", g_DisInputInfo.frame_cnt);
        }
        DisMvBufAddrEnd += sprintf((char *)DisMvBufAddrEnd, "\r\n");
    }
    // mv_x
    for(iy=0; iy<g_DisCorrectBlkV; iy++)
    {
        DisMvBufAddrEnd += sprintf((char *)DisMvBufAddrEnd, "\t");
        for(ix=0; ix<mov_wid; ix++)
        {
            uiIdx = iy*mov_wid + ix;
            DisMvBufAddrEnd += sprintf((char *)DisMvBufAddrEnd, "%d\t", SpecificVal);
        }
        DisMvBufAddrEnd += sprintf((char *)DisMvBufAddrEnd, "\r\n");
    }
    // mv_y
    for(iy=0; iy<g_DisCorrectBlkV; iy++)
    {
        DisMvBufAddrEnd += sprintf((char *)DisMvBufAddrEnd, "\t");
        for(ix=0; ix<mov_wid; ix++)
        {
            uiIdx = iy*mov_wid + ix;
            DisMvBufAddrEnd += sprintf((char *)DisMvBufAddrEnd, "%d\t", SpecificVal);
        }
        DisMvBufAddrEnd += sprintf((char *)DisMvBufAddrEnd, "\r\n");
    }
    // cnt
    for(iy=0; iy<g_DisCorrectBlkV; iy++)
    {
        DisMvBufAddrEnd += sprintf((char *)DisMvBufAddrEnd, "\t");
        for(ix=0; ix<mov_wid; ix++)
        {
            uiIdx = iy*mov_wid + ix;
            DisMvBufAddrEnd += sprintf((char *)DisMvBufAddrEnd, "%d\t", SpecificVal);
        }
        DisMvBufAddrEnd += sprintf((char *)DisMvBufAddrEnd, "\r\n");
    }
    // sad
    for(iy=0; iy<g_DisCorrectBlkV; iy++)
    {
        DisMvBufAddrEnd += sprintf((char *)DisMvBufAddrEnd, "\t");
        for(ix=0; ix<mov_wid; ix++)
        {
            uiIdx = iy*mov_wid + ix;
            DisMvBufAddrEnd += sprintf((char *)DisMvBufAddrEnd, "%d\t", SpecificVal);
        }
        DisMvBufAddrEnd += sprintf((char *)DisMvBufAddrEnd, "\r\n");
    }
    // bValid
    for(iy=0; iy<g_DisCorrectBlkV; iy++)
    {
        DisMvBufAddrEnd += sprintf((char *)DisMvBufAddrEnd, "\t");
        for(ix=0; ix<mov_wid; ix++)
        {
            uiIdx = iy*mov_wid + ix;
            DisMvBufAddrEnd += sprintf((char *)DisMvBufAddrEnd, "%d\t", SpecificVal);
        }
        DisMvBufAddrEnd += sprintf((char *)DisMvBufAddrEnd, "\r\n");
    }

    g_DisMvOutAddrCur = DisMvBufAddrEnd;

}
void SnGParms_dumpMvInfo2dram(MDS_DIM mdsDim)
{
    UINT32 ix,iy;
    UINT32 mov_wid = mdsDim.uiBlkNumH*mdsDim.uiMdsNum;
    UINT32 uiIdx = 0;
    UINT32 DisMvBufAddrEnd;


    DisMvBufAddrEnd = g_DisMvOutAddrCur;

    // IPE idx
    for(iy=0; iy<1; iy++)
    {
        DisMvBufAddrEnd += sprintf((char *)DisMvBufAddrEnd, "\t");
        for(ix=0; ix<mov_wid; ix++)
        {
            uiIdx = iy*mov_wid + ix;
            DisMvBufAddrEnd += sprintf((char *)DisMvBufAddrEnd, "%d\t", g_DisInputInfo.frame_cnt);
        }
        DisMvBufAddrEnd += sprintf((char *)DisMvBufAddrEnd, "\r\n");
    }
    // mv_x
    for(iy=0; iy<mdsDim.uiBlkNumV; iy++)
    {
        DisMvBufAddrEnd += sprintf((char *)DisMvBufAddrEnd, "\t");
        for(ix=0; ix<mov_wid; ix++)
        {
            uiIdx = iy*mov_wid + ix;
            DisMvBufAddrEnd += sprintf((char *)DisMvBufAddrEnd, "%d\t", g_DisMv[uiIdx].iX);
        }
        DisMvBufAddrEnd += sprintf((char *)DisMvBufAddrEnd, "\r\n");
    }
    // mv_y
    for(iy=0; iy<mdsDim.uiBlkNumV; iy++)
    {
        DisMvBufAddrEnd += sprintf((char *)DisMvBufAddrEnd, "\t");
        for(ix=0; ix<mov_wid; ix++)
        {
            uiIdx = iy*mov_wid + ix;
            DisMvBufAddrEnd += sprintf((char *)DisMvBufAddrEnd, "%d\t", g_DisMv[uiIdx].iY);
        }
        DisMvBufAddrEnd += sprintf((char *)DisMvBufAddrEnd, "\r\n");
    }
    // cnt
    for(iy=0; iy<mdsDim.uiBlkNumV; iy++)
    {
        DisMvBufAddrEnd += sprintf((char *)DisMvBufAddrEnd, "\t");
        for(ix=0; ix<mov_wid; ix++)
        {
            uiIdx = iy*mov_wid + ix;
            DisMvBufAddrEnd += sprintf((char *)DisMvBufAddrEnd, "%d\t", g_DisMv[uiIdx].uiCnt);
        }
        DisMvBufAddrEnd += sprintf((char *)DisMvBufAddrEnd, "\r\n");
    }
    // sad
    for(iy=0; iy<mdsDim.uiBlkNumV; iy++)
    {
        DisMvBufAddrEnd += sprintf((char *)DisMvBufAddrEnd, "\t");
        for(ix=0; ix<mov_wid; ix++)
        {
            uiIdx = iy*mov_wid + ix;
            DisMvBufAddrEnd += sprintf((char *)DisMvBufAddrEnd, "%d\t", g_DisMv[uiIdx].uiSad);
        }
        DisMvBufAddrEnd += sprintf((char *)DisMvBufAddrEnd, "\r\n");
    }
    // bValid
    for(iy=0; iy<mdsDim.uiBlkNumV; iy++)
    {
        DisMvBufAddrEnd += sprintf((char *)DisMvBufAddrEnd, "\t");
        for(ix=0; ix<mov_wid; ix++)
        {
            uiIdx = iy*mov_wid + ix;
            DisMvBufAddrEnd += sprintf((char *)DisMvBufAddrEnd, "%d\t", g_DisMv[uiIdx].bValid);
        }
        DisMvBufAddrEnd += sprintf((char *)DisMvBufAddrEnd, "\r\n");
    }

    g_DisMvOutAddrCur = DisMvBufAddrEnd;

}
void SnGParms_dumpMvInfo2file(void)
{
    FST_FILE filehdl;
    char filename[50];
    UINT32 OutLength;
    static UINT32 saveCnt = 0;


    //DBG_DUMP("SnG g_uiAdasWorkBufAddr = 0x%08x, Dump StarAddr = 0x%08x, EndAddr = 0x%08x\r\n",g_uiAdasWorkBufAddr, g_DisMvOutAddr,g_DisMvOutAddrCur);

    OutLength = g_DisMvOutAddrCur - g_DisMvOutAddr;
    //sprintf(filename ,"A:\\dis_mvS%d.bin",g_saveCnt);
    sprintf(filename ,"A:\\SNG_DebugInfo_%d_%d_%d\\dis_mvS%d.bin",g_date, g_time, g_CurTime, g_saveCnt);
    filehdl = FileSys_OpenFile( filename, FST_CREATE_ALWAYS|FST_OPEN_WRITE );
    FileSys_WriteFile( filehdl, (UINT8*)g_DisMvOutAddr, &OutLength, 0, NULL );
    FileSys_CloseFile( filehdl );

    //DBG_DUMP("Dump size = %d\r\n",OutLength);

    //DBG_DUMP("^GDump file done!!\r\n");

    saveCnt++;
}
#endif
void SetSaveDisMvFlg(UINT32 flag)
{
    g_SaveDisMvFlg = flag;
}

