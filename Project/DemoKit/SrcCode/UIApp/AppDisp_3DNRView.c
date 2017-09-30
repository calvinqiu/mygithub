#include "SysKer.h"
#include "DispSrvApi.h"
#include "display.h"
#include "Color.h"
#include "SizeConvert.h"
#include "AppDisp_ScaleView.h"
#include "PrjCfg.h"
#include "GxVideo.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          3DNRVIEW
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

static BOOL gAppDisp3DNRView_StartPlay = FALSE;
static BOOL gbCleanFirst = TRUE;

static ER AppDisp_3DNRView_OnLock(UINT32* puiLocked, UINT32 uiIdx)
{
    *puiLocked = 0;
    return E_OK;
}

static ER AppDisp_3DNRView_OnUnlock(UINT32 uiIdx)
{
    // Do nothing
    return E_OK;
}

static ER AppDisp_3DNRView_OnDraw(DISPSRV_DRAW* pDraw)
{
    ER       er;
    UINT32   uiLockIdx;
    IMG_BUF* pSrc=NULL;
    IMG_BUF* pDst=pDraw->pImg[0];
    IRECT SrcRegion = {0};
    
    if (gAppDisp3DNRView_StartPlay == FALSE)
        return E_OK;     // Media Play has been closed on dual-view mode

    if((er=pDraw->fpLock[DISPSRV_SRC_IDX_PRIMARY](&uiLockIdx,DISPSRV_LOCK_NEWEST))!=E_OK)
    {//locked fail indicate skip this draw
        return er;
    }

    pSrc = &pDraw->pDS[DISPSRV_SRC_IDX_PRIMARY]->pSrc[uiLockIdx];
   
    if(pSrc->PxlAddr[0])
    {  
        SrcRegion.x = 0;
        SrcRegion.y = 0;
        SrcRegion.w = pSrc->Width;
        SrcRegion.h = (pSrc->Height==1088)?1080:pSrc->Height;

        //clean screen
        if(gbCleanFirst)
        {
            gbCleanFirst = FALSE;
            GxImg_FillData(pDst, NULL, COLOR_YUV_BLACK);
        }
        
        GxImg_ScaleData(pSrc, &SrcRegion, pDst, NULL); 
    }

    pDraw->fpFlip(TRUE);
    pDraw->fpUnlock[DISPSRV_SRC_IDX_PRIMARY](uiLockIdx);
    return E_OK;
}

void AppDisp_3DNRView_Start(void)
{
    gAppDisp3DNRView_StartPlay = TRUE;
}

void AppDisp_3DNRView_Stop(void)
{
    gAppDisp3DNRView_StartPlay = FALSE;
}

void AppDisp_3DNRView_Cfg(IMG_BUF* pSrc)
{
    DISPSRV_CMD     Cmd={0};
    DISPSRV_SRC_CFG CfgSrc = {0};
    UINT32          fpDrawCb;

    gbCleanFirst = TRUE;    
    // Configure
    CfgSrc.fpLockCb[DISPSRV_SRC_IDX_PRIMARY] = (DISPSRV_FP_LOCK_CB) AppDisp_3DNRView_OnLock;
    CfgSrc.fpUnlockCb[DISPSRV_SRC_IDX_PRIMARY] = (DISPSRV_FP_UNLOCK_CB) AppDisp_3DNRView_OnUnlock;
    CfgSrc.Desc[DISPSRV_SRC_IDX_PRIMARY].uiNumSrc = 1;
    CfgSrc.Desc[DISPSRV_SRC_IDX_PRIMARY].pSrc = pSrc;
    CfgSrc.Desc[DISPSRV_SRC_IDX_PRIMARY].pSrcRect = NULL;  //No DIS

    //Set the source buffer
    memset(&Cmd,0,sizeof(DISPSRV_CMD));
    Cmd.Prop.bExitCmdFinish = TRUE;
    Cmd.Idx = DISPSRV_CMD_IDX_SET_SRC;
    Cmd.In.uiNumByte = sizeof(CfgSrc);
    Cmd.In.pData = &CfgSrc;
    DispSrv_Cmd(&Cmd);

    // Set the Draw Callback
    fpDrawCb = (UINT32) AppDisp_3DNRView_OnDraw;
    memset(&Cmd,0,sizeof(DISPSRV_CMD));
    Cmd.Prop.bExitCmdFinish = TRUE;
    Cmd.Idx = DISPSRV_CMD_IDX_SET_DRAW_CB;
    Cmd.In.uiNumByte = sizeof(fpDrawCb);
    Cmd.In.pData = &fpDrawCb;
    DispSrv_Cmd(&Cmd);
}

void AppDisp_3DNRView_Trigger(void)
{
    DispSrv_TriggerNext();
}



