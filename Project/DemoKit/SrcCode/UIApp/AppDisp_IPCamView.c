#include "AppDisp_IPCamView.h"
#include "GxImage.h"
#include "Color.h"
#include <stdio.h>
#include "SxCmd.h"
#include "UIInfo.h"

#define THIS_DBGLVL         1       //0=OFF, 1=ERROR, 2=TRACE
#define __MODULE__          IPCamV
#define __DBGLVL__          THIS_DBGLVL
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
//#define __DBGFLT__          "[buf]"
#include "DebugModule.h"

static INT32 m_iCleanCnt = 1; //indicate the times of cleaning screen, set 1 as default, for the first time clean screen

#if (TV_FUNC == ENABLE)
extern DX_HANDLE gDevTVObj;
#endif
#if (HDMI_FUNC == ENABLE)
extern DX_HANDLE gDevHDMIObj;
#endif

void IPCamView_SetImg(ISIZE* pImgSize, USIZE* pAspectRatio)
{
}

INT32 IPCamView_OnDraw(DISPSRV_DRAW* pDraw) //PIP = Picture In Picture
{
    ER er;
    UINT32 i,n=(pDraw->bDualSrc==TRUE)?2:1;
    UINT32 uiLockIdx[2];
    IMG_BUF* pSrc=NULL;
    IMG_BUF* pSrcImg[2]={0};
    IMG_BUF* pDst=pDraw->pImg[0];

    #if 0
    {
        #if (TV_FUNC == ENABLE)
        if(GxVideo_GetDevice(DOUT1) != (UINT32)gDevTVObj) 
        {
            return E_OK; //if not insert TV yet, ignore this OnDraw
        }
        #endif
        #if (HDMI_FUNC == ENABLE)
        if(GxVideo_GetDevice(DOUT1) != (UINT32)gDevHDMIObj)
        {
            return E_OK; //if not insert HDMI yet, ignore this OnDraw
        }
        #endif
    }
    #endif
    
    for(i=0;i<n;i++)
    {
        if(pDraw->uiMaskSrc&(1<<i))
        {
            if((er=pDraw->fpLock[i](&uiLockIdx[i],DISPSRV_LOCK_NEWEST))!=E_OK)
            {
                DBG_ERR("failed to lock %d\r\n",i);
                return er;
            }
            else
            {
                 pSrcImg[i] = &pDraw->pDS[i]->pSrc[uiLockIdx[i]];
            }
        }
    }

    //--------------------Customer Draw Begin-----------------------------------
    //Clean Screen
    if(m_iCleanCnt>0)
    {
        GxImg_FillData(pDst, NULL, COLOR_YUV_BLACK);
        m_iCleanCnt--;
    }

    pSrc = pSrcImg[0];
    if(pSrc!=NULL)
    {
        GxImg_ScaleData(pSrc,NULL,pDst,NULL);
    }

    for(i=0;i<n;i++)
    {
        if(pDraw->uiMaskSrc&(1<<i))
        {
            if((er=pDraw->fpUnlock[i](uiLockIdx[i]))!=E_OK)
            {
                DBG_ERR("failed to unlock %d\r\n",i);
                return er;
            }
        }
    }
    //--------------------Customer Draw End-------------------------------------
    pDraw->fpFlip(TRUE);

    return E_OK;
}
