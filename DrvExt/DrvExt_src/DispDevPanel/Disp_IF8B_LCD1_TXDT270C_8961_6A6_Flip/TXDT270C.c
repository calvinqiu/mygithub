/*
    Display object object panel configuration parameters for TXDT270CDR_2V7

    @file       TXDT270CDR_2V7.c
    @ingroup
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include "Debug.h"
#include "display.h"
#include "dispdev_IF8bits.h"
#include "dispdev_panlcomn.h"

#define LCD_T30P66	0

/*
    Panel Parameters for TXDT270CDR_2V7
*/
//@{
const tPANEL_CMD tCmdStandby[] =
{
    {0x2B, 0x00}
};

//00 04  01 af  0d 35
//00 03  01 f1  0d 3c
const tPANEL_CMD tCmdModeRgbd320[] =
{
#if (LCD_T30P66 == 0)

	{0x05, 0x5F},  //set power mode (standby)
	{CMDDELAY_MS, 80},
	{0x05, 0x1F},
	{CMDDELAY_MS, 80},
	{0x05, 0x5F},
	{CMDDELAY_MS, 80},
	{0x2B,0x01},
	{0x0B,0x80},
	{0x00,0x0D},

	//{0x01,0xAC},
	{0x01,0xAC},


	{0x04,0x0B},


	{0x03,0x39},//brightness
	{0x0D,0x3c},//contrast

	{0x0E,0x45},
	{0x0F,0x40},

	{0x10,0x45},
	{0x11,0x40},

	{0x2f,0x69},
	{0x16,0x04},
	{0x95,0x80}

#else//T30P66-03

	{0x05, 0x5F},  //set power mode (standby)
	{CMDDELAY_MS, 5},
	{0x05, 0x1F},
	{CMDDELAY_MS, 10},
	{0x05, 0x5c},
	{CMDDELAY_MS, 50},

	{0x0B,0x80},//83 DE mode, VOMDC is controlied by R1

	//{0x00,0x07},//VCOMAC	0a
	{0x00,0x06},//VCOMAC	0a
	{0x01,0xBF},//VCOMDC	90

	{0x03,0x40},//brightness setting
	{0x0D,0x40},//contrast setting
	{0x04,0x0b},//8-bits RGB interface
	//{0x04,0x1b};//8-bit Dummy RGB 320 x 240

//	{0x09,0x08},

	{0x2F,0x69},
	{0x95,0x80},
	{0x97,0x00},
	{0x9a,0x02},
	{0xa6,0x68},
	{0x20,0xd0},

	{0x16,0x00},
	{0x30,0x19},
	{0x31,0x0D},
	{0x32,0x01},
	{0x33,0x02},
	{0x34,0x04},
	{0x35,0x06},
	{0x36,0x09},
	{0x37,0x0B},
	{0x38,0x0C},
	{0x39,0x0B},
	{0x3A,0x0A},
	{0x3B,0x02},

	{0x2B,0x01}//exit standby
#endif
};

const tLCD_PARAM tMode[] =
{
    /***********       RGB Dummy-320 MODE         *************/
    {
        // tPANEL_PARAM
        {
            /* Old prototype */
            PINMUX_LCDMODE_RGBD320,         //!< LCDMode

            #if (LCD_T30P66 == 0)
			27.05f, 						//!< fDCLK
			#else
            24.54f,                         //!< fDCLK
			#endif

            1560,                           //!< uiHSyncTotalPeriod
            1280,                           //!< uiHSyncActivePeriod
            241,                            //!< uiHSyncBackPorch
            262,                            //!< uiVSyncTotalPeriod
            240,                            //!< uiVSyncActivePeriod
            21,                             //!< uiVSyncBackPorchOdd
            21,                             //!< uiVSyncBackPorchEven
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
            PINMUX_LCD_SEL_SERIAL_RGB_8BITS,//!< pinmux_select_lcd;
            ICST_CCIR601,                   //!< icst;
            {FALSE,FALSE},                  //!< dithering[2];
            DISPLAY_DEVICE_AU,              //!< **DONT-CARE**
            IDE_PDIR_RGB,                   //!< pdir;
            IDE_LCD_R,                      //!< odd;
            IDE_LCD_B,                      //!< even;
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
            0x58,                           //!< cmults;
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

        (tPANEL_CMD*)tCmdModeRgbd320,                  //!< pCmdQueue
        sizeof(tCmdModeRgbd320)/sizeof(tPANEL_CMD),    //!< nCmd
    }

};
const tPANEL_CMD tCmdRotate_None[] =
{
    {0x04,0x18}
};
const tPANEL_CMD tCmdRotate_180[] =
{
    {0x04,0x1b}
};
const tLCD_ROT tRotate[] =
{
    {DISPDEV_LCD_ROTATE_NONE,(tPANEL_CMD*)tCmdRotate_None,1},
    {DISPDEV_LCD_ROTATE_180,(tPANEL_CMD*)tCmdRotate_180,1}
};
//@}

tLCD_ROT* dispdev_getLcdRotateCmd(UINT32 *ModeNumber)
{
#if 0
    if(tRotate != NULL)
    {
        *ModeNumber = sizeof(tRotate)/sizeof(tLCD_ROT);
    }
    else
    {
        *ModeNumber = 0;
    }
#else
        *ModeNumber = sizeof(tRotate)/sizeof(tLCD_ROT);
#endif    
    return (tLCD_ROT*)tRotate;
}

tLCD_PARAM* dispdev_getConfigMode(UINT32 *ModeNumber)
{
    *ModeNumber = sizeof(tMode)/sizeof(tLCD_PARAM);
    return (tLCD_PARAM*)tMode;
}

tPANEL_CMD* dispdev_getStandbyCmd(UINT32 *CmdNumber)
{
    *CmdNumber = sizeof(tCmdStandby)/sizeof(tPANEL_CMD);
    return (tPANEL_CMD*)tCmdStandby;
}

#if 1
void dispdev_writeToLcdSif(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 value)
{
    UINT32                  uiSIFData;
    DISPDEV_IOCTRL_PARAM    DevIOCtrl;

    uiSIFData = ((((UINT32)addr << 8)|((UINT32)value)) << 16);

    pDispDevControl(DISPDEV_IOCTRL_GET_REG_IF, &DevIOCtrl);
    sif_send(DevIOCtrl.SEL.GET_REG_IF.uiSifCh, uiSIFData, 0, 0);
}


void dispdev_writeToLcdGpio(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 value)
{
    UINT32                  uiSIFData,j;
    UINT32                  SifClk,SifSen,SifData;
    DISPDEV_IOCTRL_PARAM    DevIOCtrl;

    pDispDevControl(DISPDEV_IOCTRL_GET_REG_IF, &DevIOCtrl);
    SifClk  = DevIOCtrl.SEL.GET_REG_IF.uiGpioClk;
    SifSen  = DevIOCtrl.SEL.GET_REG_IF.uiGpioSen;
    SifData = DevIOCtrl.SEL.GET_REG_IF.uiGpioData;

    gpio_setDir(SifSen, GPIO_DIR_OUTPUT);
    gpio_setDir(SifClk, GPIO_DIR_OUTPUT);
    gpio_setDir(SifData, GPIO_DIR_OUTPUT);

    gpio_setPin(SifSen);
    gpio_setPin(SifData);
    gpio_setPin(SifClk);

    uiSIFData = (UINT32)addr << 8 | (UINT32)value;
    //debug_err(("Has not implement dispdev_readFromLcdGpio%x %x  %x\r\n",uiSIFData,addr,value));
    Delay_DelayUs(500);

    gpio_clearPin(SifSen);

    for(j = 16; j > 0; j--)
    {
        if(((uiSIFData >> (j - 1)) & 0x01))
            gpio_setPin(SifData);
        else
            gpio_clearPin(SifData);

        Delay_DelayUs(100);
        gpio_clearPin(SifClk);
        Delay_DelayUs(200);
        gpio_setPin(SifClk);
        Delay_DelayUs(100);
    }

    Delay_DelayUs(500);
    gpio_setPin(SifSen);

}



void dispdev_readFromLcdGpio(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32* p_value)
{
    debug_msg("Has not implement dispdev_readFromLcdGpio \r\n");
}
#endif
