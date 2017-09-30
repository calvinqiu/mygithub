#include "GxImage.h"
#include "UIInfo.h"
#include "SysCfg.h"
#include "AppDisp_SbsView.h"
#include "ImageUnit_UserDraw.h"

#define THIS_DBGLVL         1       //0=OFF, 1=ERROR, 2=TRACE
#define __MODULE__          Sbs
#define __DBGLVL__          THIS_DBGLVL
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
//#define __DBGFLT__          "[buf]"
#include "DebugModule.h"

void SbsView_SetImg(ISIZE* pImgSize, USIZE* pAspectRatio)
{
    {
        ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_WIDTH, 0x00010001); // 1x
        ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_HEIGHT, 0x00010001); // 1x
        ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_WIDTH2, 0x00010001); // 1x
        ImageUnit_SetParam(&ISF_UserDraw, USERDRAW_PARAM_SCALE_HEIGHT2, 0x00010001); // 1x
    }
}

INT32 SbsView_OnDraw(DISPSRV_DRAW* pDraw)
{
    ER er;
    UINT32 uiLockIdx;
    IMG_BUF* pSrc=NULL;
    IMG_BUF* pDst = pDraw->pImg[0];

    if((er=pDraw->fpLock[DISPSRV_SRC_IDX_PRIMARY](&uiLockIdx,DISPSRV_LOCK_NEWEST))!=E_OK)
    {//locked fail indicate skip this draw
        return er;
    }

    pSrc = &pDraw->pDS[DISPSRV_SRC_IDX_PRIMARY]->pSrc[uiLockIdx];

    GxImg_CopyData(pSrc,NULL,pDst,NULL);
    pDraw->fpUnlock[DISPSRV_SRC_IDX_PRIMARY](uiLockIdx);
    pDraw->fpFlip(TRUE);

    if(pDraw->bDualHandle)
    {
        DBG_ERR("Not Support.\r\n");
    }

    return E_OK;
}


