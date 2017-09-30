/*
    Display object object Golden Sample for driving Memory interface panel

    @file       ILI9341.c
    @ingroup
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "Debug.h"
#include "display.h"
#include "dispdev_IFmi.h"
#include "dispdev_panlcomn.h"

 
/*
    Panel Parameters for TCON ILI9341
*/
//@{
/*Used in MI format*/
const tPANEL_CMD tCmdModeMiFmt3[] =
{

        {MICMD_CMD, 0x11},
        {CMDDELAY_MS, 120},

        {MICMD_CMD, 0x36},
        {MICMD_DATA,0xA0},//A0

        {MICMD_CMD, 0x3a},
        {MICMD_DATA,0x66},

     {MICMD_CMD, 0xb2},
        {MICMD_DATA,0x08},
        {MICMD_DATA,0x08},
        {MICMD_DATA,0x00},
        {MICMD_DATA,0x22},
        {MICMD_DATA,0x22},

        {MICMD_CMD, 0xb7},
        {MICMD_DATA,0x35},
       /*
        {MICMD_CMD, 0x2b},
        {MICMD_DATA,0x00},
        {MICMD_DATA,0x00},
        {MICMD_DATA,0x01},
        {MICMD_DATA,0x3f},

        {MICMD_CMD, 0x2a},
        {MICMD_DATA,0x00},
        {MICMD_DATA,0x00},
        {MICMD_DATA,0x00},
        {MICMD_DATA,0xef},
        */
        {MICMD_CMD, 0xbb},
        {MICMD_DATA,0x27},

        {MICMD_CMD, 0xc0},
        {MICMD_DATA,0x2c},

        {MICMD_CMD, 0xc2},
        {MICMD_DATA,0x01},

        {MICMD_CMD, 0xc3},
        {MICMD_DATA,0x0b},

        {MICMD_CMD, 0xc4},
        {MICMD_DATA,0x20},

        {MICMD_CMD, 0xc6},
        {MICMD_DATA,0x0f},

        {MICMD_CMD, 0xd0},
        {MICMD_DATA,0xa4},
        {MICMD_DATA,0xa1},

        {MICMD_CMD, 0xe0},
        {MICMD_DATA,0xd0},
        {MICMD_DATA,0x00},
        {MICMD_DATA,0x02},
        {MICMD_DATA,0x07},
        {MICMD_DATA,0x07},
        {MICMD_DATA,0x19},
        {MICMD_DATA,0x2e},
        {MICMD_DATA,0x54},
        {MICMD_DATA,0x41},
        {MICMD_DATA,0x2d},
        {MICMD_DATA,0x17},
        {MICMD_DATA,0x18},
        {MICMD_DATA,0x14},
        {MICMD_DATA,0x18},

        {MICMD_CMD, 0xe1},
        {MICMD_DATA,0xd0},
        {MICMD_DATA,0x00},
        {MICMD_DATA,0x02},
        {MICMD_DATA,0x07},
        {MICMD_DATA,0x04},
        {MICMD_DATA,0x24},
        {MICMD_DATA,0x2c},
        {MICMD_DATA,0x44},
        {MICMD_DATA,0x42},
        {MICMD_DATA,0x1c},
        {MICMD_DATA,0x1a},
        {MICMD_DATA,0x17},
        {MICMD_DATA,0x15},
        {MICMD_DATA,0x18},

     {MICMD_CMD, 0x29}, //Display ON
        {MICMD_CMD, 0x2C}, //Display ON
//************* Start Initial Sequence **********//
};


const tPANEL_CMD tCmdStandbyMI[] =
{

//  {MICMD_CMD,0x28}, //Display off
  {CMDDELAY_US, 1},
//  {MICMD_CMD,0x10}, //enter sleep mode
//  {MICMD_CMD,0x2c}, //memory write
};

const tLCD_PARAM tModeMI[] =
{
    /***********       MI Serial Format 1      *************/
    {
        // tPANEL_PARAM
        {
            /* Old prototype */
            PINMUX_LCDMODE_MI_FMT3,         //!< LCDMode
            6.80f,                          //!< fDCLK
            (0x20+240),                     //!< uiHSyncTotalPeriod
            240,                            //!< uiHSyncActivePeriod
            0x10,                           //!< uiHSyncBackPorch
            0x8+320,                        //!< uiVSyncTotalPeriod
            320,                            //!< uiVSyncActivePeriod
            0x02,                           //!< uiVSyncBackPorchOdd
            0x02,                           //!< uiVSyncBackPorchEven
            320,                            //!< uiBufferWidth
            240,                            //!< uiBufferHeight
            320,                            //!< uiWindowWidth
            240,                            //!< uiWindowHeight
            FALSE,                          //!< bYCbCrFormat

            /* New added parameters */
            0x01,                           //!< uiHSyncSYNCwidth
            0x01                            //!< uiVSyncSYNCwidth
        },

        // tIDE_PARAM
        {
            /* Old prototype */
            PINMUX_LCD_SEL_PARALLE_MI_8BITS|PINMUX_LCD_SEL_TE_ENABLE,  //!< pinmux_select_lcd;
            ICST_CCIR601,                   //!< icst;
            {FALSE, FALSE},                   //!< dithering[2];
            DISPLAY_DEVICE_MI,              //!< **DONT-CARE**
            IDE_PDIR_RGB,                   //!< pdir;
            IDE_LCD_R,                      //!< odd;
            IDE_LCD_G,                      //!< even;
            TRUE,                           //!< hsinv;
            TRUE,                           //!< vsinv;
            FALSE,                          //!< hvldinv;
            FALSE,                          //!< vvldinv;
            TRUE,                           //!< clkinv;
            FALSE,                          //!< fieldinv;
            FALSE,                          //!< **DONT-CARE**
            FALSE,                          //!< interlace;
            FALSE,                          //!< **DONT-CARE**
            0x40,                           //!< ctrst;
            0x00,                           //!< brt;
            0x40,                           //!< cmults;
            FALSE,                          //!< cex;
            FALSE,                          //!< **DONT-CARE**
            TRUE,                           //!< **DONT-CARE**
            TRUE,                           //!< tv_powerdown;
            {0x00,0x00},                    //!< **DONT-CARE**

            /* New added parameters */
            FALSE,                          //!< YCex
            FALSE,                          //!< HLPF
            {FALSE,FALSE,FALSE},            //!< subpixOdd[3]
            {FALSE,FALSE,FALSE},            //!< subpixEven[3]
            {IDE_DITHER_6BITS,IDE_DITHER_6BITS,IDE_DITHER_6BITS}//!< DitherBits[3]
        },

        (tPANEL_CMD*)tCmdModeMiFmt3,                  //!< pCmdQueue
        sizeof(tCmdModeMiFmt3)/sizeof(tPANEL_CMD),    //!< nCmd
    }
};

const tPANEL_CMD tCmdRotate_None[] =
{
  {MICMD_CMD,0x36}, // Memory Access Control
  {MICMD_DATA,0x08},//48

     //{MICMD_CMD,0xB6},
     //{MICMD_DATA,0x0a},
     //{MICMD_DATA,0x82},//A2

 // {MICMD_CMD,0xB6},
 // {MICMD_DATA,0x0a},
 // {MICMD_DATA,0x82},//A2

//  {MICMD_CMD,0x0B}, // Read Display MADCTL
//  {MICMD_DATA,0x00},
  {MICMD_CMD,0x2c}, //memory write

};
const tPANEL_CMD tCmdRotate_180[] =
{


  {MICMD_CMD,0x36}, // Memory Access Control
  {MICMD_DATA,0x08},  //88

//  {MICMD_CMD,0x0B}, // Read Display MADCTL
//  {MICMD_DATA,0x14},
   //{MICMD_CMD,0xB6},
 // {MICMD_DATA,0x0a},
 //  {MICMD_DATA,0xA2},//A2  gs=1

  {MICMD_CMD,0x2c}, //memory write
};
const tLCD_ROT tRotMI[] =
{
    {DISPDEV_LCD_ROTATE_NONE,(tPANEL_CMD*)tCmdRotate_None,3},
    {DISPDEV_LCD_ROTATE_180,(tPANEL_CMD*)tCmdRotate_180,3}
};

//@}

tLCD_ROT* dispdev_getLcdRotateMiCmd(UINT32 *ModeNumber)
{
    *ModeNumber = sizeof(tRotMI)/sizeof(tLCD_ROT);    
    return (tLCD_ROT*)tRotMI;
}

tLCD_PARAM* dispdev_getConfigModeMI(UINT32 *ModeNumber)
{
    *ModeNumber = sizeof(tModeMI)/sizeof(tLCD_PARAM);
    return (tLCD_PARAM*)tModeMI;
}

tPANEL_CMD* dispdev_getStandbyCmdMI(UINT32 *CmdNumber)
{
    *CmdNumber = sizeof(tCmdStandbyMI)/sizeof(tPANEL_CMD);
    return (tPANEL_CMD*)tCmdStandbyMI;
}

void dispdev_writeToLcdMi(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 value)
{
    if(addr & MICMD_DATA)
        mi_setCmd1(WrDATA, (value&0xff));
    else
        mi_setCmd1(WrCMD, (value&0xff));

    mi_setCmdCnt(1);
    mi_waitCmdDone();
}


void dispdev_readFromLcdMi(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 *pValue)
{
    *pValue = 0;
  //  debug_err(("No MI Read reg implement!\r\n"));
}

void      dispdev_setMiConfig(tMI_CONFIG *pMiConfig)
{
    FLOAT   MiTargetClk = 40.0; // Max is 60MHz

    mi_setCtrl(PARALLEL_I80, MI_AUTOFMT3, pMiConfig->DataFmt, FALSE, FALSE, FALSE, FALSE);
    //mi_setSerialCsCtrl(TRUE);
    //mi_setPolCtrl(FALSE, FALSE, FALSE, FALSE, SCLMOD0);
    //mi_setSerialAutoRsBitValue(TRUE);
    #if 0
    Div = (UINT32) (pMiConfig->fMiSrcClk/MiTargetClk);
    pll_setClockRate(PLL_CLKSEL_MI_CLKDIV, PLL_MI_CLKDIV(Div-1));
    #else
    mi_setConfig(MI_CONFIG_ID_FREQ, MiTargetClk*1000000);
    #endif
}

void dispdev_setMiAutoCmd(void)
{
    return;
}
