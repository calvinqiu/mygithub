#include "DxCfg.h"
#include "IOCfg.h"

#include "DxDisplay.h"
#include "DxCommon.h"
#include "display.h"


void DrvIDEEffectControl(PDISPCTRL_PARAM pCtrl, PDISP_OBJ pObj,BOOL ide_en);

UINT8 HuiTbl[] = {
/*
  128,109, 90, 73, 56, 41, 27, 15,  6,  0,
  0,  0,  0,  1,  8, 16, 26, 38, 51, 65,
  80, 95,111,128  };
  128, 128, 128, 128, 128, 128, 128, 128,
  128, 128, 128, 128, 128, 128, 128, 128,
  128, 128, 128, 128, 128, 128, 128, 128};
    128,125,122,121,121,123,128,135,144,155,
    166,178,189,199,207,212,215,213,207,195,
    181,164,146,128};*/
    128,125,122,121,121,123,128,135,
141,148,157,167,178,188,196,205,
209,208,200,188,174,158,146,128};

INT8 SatTbl[] = {
  /*131, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0 };  */
    189,190,190,190,190,190,190,190,190,190,
    190,190,190,190,190,190,190,190,190,190,
    190,190,190,190};

INT8 IntTbl[] = {
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0 };

_ALIGNED(4) static const UINT8 Tbl_DDS[8]=
{
     32,32,32,32,32,32,32,32
	 //0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
};

void DrvIDEEffectControl(PDISPCTRL_PARAM pCtrl, PDISP_OBJ pObj,BOOL ide_en)
{

        pCtrl->SEL.SET_CC_DDS.pDdsTab = (UINT8*)Tbl_DDS;
        pObj->dispCtrl(DISPCTRL_SET_CC_DDS,pCtrl);

        //ICST0
        memset(pCtrl,0,sizeof(DISPCTRL_PARAM));
        pCtrl->SEL.SET_ICST0_EN.bEn = ide_en;
        pObj->dispCtrl(DISPCTRL_SET_ICST0_EN,pCtrl);

        //CST
        memset(pCtrl,0,sizeof(DISPCTRL_PARAM));
        pCtrl->SEL.SET_CST_EN.bEn = ide_en;
        pObj->dispCtrl(DISPCTRL_SET_CST_EN,pCtrl);

        //CC
        memset(pCtrl,0,sizeof(DISPCTRL_PARAM));
        pCtrl->SEL.SET_CC_EN.bEn = ide_en;
        pObj->dispCtrl(DISPCTRL_SET_CC_EN,pCtrl);

        //CCA
        memset(pCtrl,0,sizeof(DISPCTRL_PARAM));
        pCtrl->SEL.SET_CCA_EN.bEn = ide_en;
        pObj->dispCtrl(DISPCTRL_SET_CCA_EN,pCtrl);

        memset(pCtrl,0,sizeof(DISPCTRL_PARAM));
        pCtrl->SEL.SET_CCA_HUE_EN.bEn = ide_en;
        pObj->dispCtrl(DISPCTRL_SET_CCA_HUE_EN,pCtrl);

        memset(pCtrl,0,sizeof(DISPCTRL_PARAM));
        pCtrl->SEL.SET_CCA_YCON_EN.bEn = ide_en;
        pObj->dispCtrl(DISPCTRL_SET_CCA_YCON_EN,pCtrl);

        memset(pCtrl,0,sizeof(DISPCTRL_PARAM));
        pCtrl->SEL.SET_CCA_CCON_EN.bEn = ide_en;
        pObj->dispCtrl(DISPCTRL_SET_CCA_CCON_EN,pCtrl);

        //GAMMA
        memset(pCtrl,0,sizeof(DISPCTRL_PARAM));
        pCtrl->SEL.SET_GAMMA_EN.bEn = ide_en;
        pObj->dispCtrl(DISPCTRL_SET_GAMMA_EN,pCtrl);

        memset(pCtrl,0,sizeof(DISPCTRL_PARAM));
        pCtrl->SEL.SET_CC_EN.bEn = ide_en;
        pObj->dispCtrl(DISPCTRL_SET_CC_EN ,pCtrl);

        // update HUE table
        memset(pCtrl,0,sizeof(DISPCTRL_PARAM));
        pCtrl->SEL.SET_CC_HUE.pHueTab = (UINT8 *)HuiTbl;
        pObj->dispCtrl(DISPCTRL_SET_CC_HUE ,pCtrl);

        // update Saturation table
        memset(pCtrl,0,sizeof(DISPCTRL_PARAM));
        pCtrl->SEL.SET_CC_SAT.pSatTab = (INT8 *)SatTbl;
        pObj->dispCtrl(DISPCTRL_SET_CC_SAT,pCtrl);

        // update INT table
        memset(pCtrl,0,sizeof(DISPCTRL_PARAM));
        pCtrl->SEL.SET_CC_INT.pIntTab = (INT8 *)IntTbl;
        pObj->dispCtrl(DISPCTRL_SET_CC_INT,pCtrl);


        // update contrast offset
        memset(pCtrl,0,sizeof(DISPCTRL_PARAM));
        pCtrl->SEL.SET_CCA_YCON.uiycon=0x80;
        pObj->dispCtrl(DISPCTRL_SET_CCA_YCON,pCtrl);

        memset(pCtrl,0,sizeof(DISPCTRL_PARAM));
        pCtrl->SEL.SET_CCA_CCON.uiccon=0x80;
        pObj->dispCtrl(DISPCTRL_SET_CCA_CCON,pCtrl);

        // update saturation offset
        memset(pCtrl,0,sizeof(DISPCTRL_PARAM));
        pCtrl->SEL.SET_CC_SAT_OFS.isatofs=0x5A;//0x80;
        pObj->dispCtrl(DISPCTRL_SET_CC_SAT_OFS,pCtrl);

        // update brightness offset
        memset(pCtrl,0,sizeof(DISPCTRL_PARAM));
        pCtrl->SEL.SET_CC_INT_OFS.iintofs=0;//0x80;
        pObj->dispCtrl(DISPCTRL_SET_CC_INT_OFS,pCtrl);


        pObj->load(TRUE);
    }


