/*
    Display object for driving DSI device

    @file       HX8379C_dsi.c
    @ingroup
    @note       This panel MUST select IDE clock to PLL1 ( 480 ). Once change to \n
                another frequence, the _IDE_fDCLK should be re calculated

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "debug.h"
#include "display.h"
#include "dispdev_ifDsi.h"
#include "dispdev_panlcomn.h"

// Default debug level
#ifndef __DBGLVL__
    #define __DBGLVL__  2       // Output all message by default. __DBGLVL__ will be set to 1 via make parameter when release code.
#endif

// Default debug filter
#ifndef __DBGFLT__
    #define __DBGFLT__  "*"     // Display everything when debug level is 2
#endif

#include "DebugModule.h"


#define PANEL_WIDTH     480//480
#define PANEL_HEIGHT    800//864//848



#define HX8379C_IND_MSG(...)       debug_msg(__VA_ARGS__)

#define HX8379C_ERR_MSG(...)       debug_msg(__VA_ARGS__)

#define HX8379C_WRN_MSG(...)       debug_msg(__VA_ARGS__)

#define HX8379C_TE_OFF              0
#define HX8379C_TE_ON               1

#define HX8379C_TE_PACKET           0
#define HX8379C_TE_PIN              1

/*
    RGB888 = 1 pixel = 3bytes packet
    If DSI src = 240MHz, internal clock = 30MHz, data rate = 30MHz x 1bytes = 30MB / sec per lane
    2 lane = 60MB = 20Mpixel ' IDE need > 20MHz
*/


#define DSI_FORMAT_RGB565          0    //N/A in HX8379C
#define DSI_FORMAT_RGB666P         1    //N/A in HX8379C
#define DSI_FORMAT_RGB666L         2    //N/A in HX8379C
#define DSI_FORMAT_RGB888          3    //IDE use 480 & DSI use 480

#define DSI_PACKET_FORMAT          DSI_FORMAT_RGB888



#define DSI_TARGET_CLK            480


#if(DSI_PACKET_FORMAT == DSI_FORMAT_RGB888)
#define _IDE_fDCLK      ((((DSI_TARGET_CLK / 8) * 2)/3))
#else
#error "Format not RGB888"
#endif

#define HVALIDST    0x39  //uiHSyncBackPorch(HPB) -> 53
#define VVALIDST    0x06//0x15  //uiVSyncBackPorchOdd/Even

#define HSYNCT      0x02
#define VSYNCT      0x02

/*
    Panel Parameters for TCON HX8379C
*/
//@{
/*Used in DSI*/

const tPANEL_CMD tCmdModeDSI[] =
{
	{DSICMD_CMD,0xB0},//B0
	{DSICMD_DATA,0x03},

	{DSICMD_CMD,0x11},//11
	{CMDDELAY_MS,200},
	{DSICMD_CMD,0xB0},//B0
	{DSICMD_DATA,0x04},
	
	//{CMDDELAY_MS,200},
	{DSICMD_CMD,0xB3},//B3
	{DSICMD_DATA,0x02},
	{DSICMD_DATA,0x00},

	{DSICMD_CMD,0xB6},//B6
	{DSICMD_DATA,0x52},
	{DSICMD_DATA,0x83},

	{DSICMD_CMD,0xC1},//C1
	{DSICMD_DATA,0x40},//可以改方向0x41
	{DSICMD_DATA,0x31},
	{DSICMD_DATA,0x00},
	{DSICMD_DATA,0x21},
	{DSICMD_DATA,0x21},
	{DSICMD_DATA,0x32},
	{DSICMD_DATA,0x12},
	{DSICMD_DATA,0x28},
	{DSICMD_DATA,0x46},
	{DSICMD_DATA,0x1E},
	{DSICMD_DATA,0xA5},
	{DSICMD_DATA,0x0F},
	{DSICMD_DATA,0x58},
	{DSICMD_DATA,0x21},
	{DSICMD_DATA,0x01},

	{DSICMD_CMD,0xC2},//C2
	{DSICMD_DATA,0x00},
	{DSICMD_DATA,0x06},
	{DSICMD_DATA,0x06},
	{DSICMD_DATA,0x01},
	{DSICMD_DATA,0x03},
	{DSICMD_DATA,0x00},
#if 0
	{DSICMD_CMD,0xC8},//C8
	{DSICMD_DATA,0x02},
	{DSICMD_DATA,0x12},
	{DSICMD_DATA,0x1A},
	{DSICMD_DATA,0x26},
	{DSICMD_DATA,0x35},
	{DSICMD_DATA,0x50},
	{DSICMD_DATA,0x33},
	{DSICMD_DATA,0x20},
	{DSICMD_DATA,0x16},
	{DSICMD_DATA,0x11},
	{DSICMD_DATA,0x08},
	{DSICMD_DATA,0x05},
	{DSICMD_DATA,0x02},
	{DSICMD_DATA,0x12},
	{DSICMD_DATA,0x1A},
	{DSICMD_DATA,0x26},
	{DSICMD_DATA,0x35},
	{DSICMD_DATA,0x50},
	{DSICMD_DATA,0x33},
	{DSICMD_DATA,0x20},
	{DSICMD_DATA,0x16},
	{DSICMD_DATA,0x11},
	{DSICMD_DATA,0x08},
	{DSICMD_DATA,0x05},
	
	{DSICMD_CMD,0xC9},//C9
	{DSICMD_DATA,0x02},
	{DSICMD_DATA,0x12},
	{DSICMD_DATA,0x1A},
	{DSICMD_DATA,0x26},
	{DSICMD_DATA,0x35},
	{DSICMD_DATA,0x50},
	{DSICMD_DATA,0x33},
	{DSICMD_DATA,0x20},
	{DSICMD_DATA,0x16},
	{DSICMD_DATA,0x11},
	{DSICMD_DATA,0x08},
	{DSICMD_DATA,0x05},
	{DSICMD_DATA,0x02},
	{DSICMD_DATA,0x12},
	{DSICMD_DATA,0x1A},
	{DSICMD_DATA,0x26},
	{DSICMD_DATA,0x35},
	{DSICMD_DATA,0x50},
	{DSICMD_DATA,0x33},
	{DSICMD_DATA,0x20},
	{DSICMD_DATA,0x16},
	{DSICMD_DATA,0x11},
	{DSICMD_DATA,0x08},
	{DSICMD_DATA,0x05},

	{DSICMD_CMD,0xCA},//CA
	{DSICMD_DATA,0x02},
	{DSICMD_DATA,0x12},
	{DSICMD_DATA,0x1A},
	{DSICMD_DATA,0x26},
	{DSICMD_DATA,0x35},
	{DSICMD_DATA,0x50},
	{DSICMD_DATA,0x33},
	{DSICMD_DATA,0x20},
	{DSICMD_DATA,0x16},
	{DSICMD_DATA,0x11},
	{DSICMD_DATA,0x08},
	{DSICMD_DATA,0x05},
	{DSICMD_DATA,0x02},
	{DSICMD_DATA,0x12},
	{DSICMD_DATA,0x1A},
	{DSICMD_DATA,0x26},
	{DSICMD_DATA,0x35},
	{DSICMD_DATA,0x50},
	{DSICMD_DATA,0x33},
	{DSICMD_DATA,0x20},
	{DSICMD_DATA,0x16},
	{DSICMD_DATA,0x11},
	{DSICMD_DATA,0x08},
	{DSICMD_DATA,0x05},
#endif

	{DSICMD_CMD,0xC8},//C8
	{DSICMD_DATA,0x02},
	{DSICMD_DATA,0x14},
	{DSICMD_DATA,0x1C},
	{DSICMD_DATA,0x26},
	{DSICMD_DATA,0x35},
	{DSICMD_DATA,0x53},
	{DSICMD_DATA,0x2F},
	{DSICMD_DATA,0x1A},
	{DSICMD_DATA,0x10},
	{DSICMD_DATA,0x0C},
	{DSICMD_DATA,0x08},
	{DSICMD_DATA,0x04},
	{DSICMD_DATA,0x02},
	{DSICMD_DATA,0x14},
	{DSICMD_DATA,0x1C},
	{DSICMD_DATA,0x26},
	{DSICMD_DATA,0x35},
	{DSICMD_DATA,0x53},
	{DSICMD_DATA,0x2F},
	{DSICMD_DATA,0x1A},
	{DSICMD_DATA,0x10},
	{DSICMD_DATA,0x0C},
	{DSICMD_DATA,0x08},
	{DSICMD_DATA,0x04},

	{DSICMD_CMD,0xC9},//C9
	{DSICMD_DATA,0x02},
	{DSICMD_DATA,0x14},
	{DSICMD_DATA,0x1C},
	{DSICMD_DATA,0x26},
	{DSICMD_DATA,0x35},
	{DSICMD_DATA,0x53},
	{DSICMD_DATA,0x2F},
	{DSICMD_DATA,0x1A},
	{DSICMD_DATA,0x10},
	{DSICMD_DATA,0x0C},
	{DSICMD_DATA,0x08},
	{DSICMD_DATA,0x04},
	{DSICMD_DATA,0x02},
	{DSICMD_DATA,0x14},
	{DSICMD_DATA,0x1C},
	{DSICMD_DATA,0x26},
	{DSICMD_DATA,0x35},
	{DSICMD_DATA,0x53},
	{DSICMD_DATA,0x2F},
	{DSICMD_DATA,0x1A},
	{DSICMD_DATA,0x10},
	{DSICMD_DATA,0x0C},
	{DSICMD_DATA,0x08},
	{DSICMD_DATA,0x04},

	{DSICMD_CMD,0xCA},//CA
	{DSICMD_DATA,0x02},
	{DSICMD_DATA,0x14},
	{DSICMD_DATA,0x1C},
	{DSICMD_DATA,0x26},
	{DSICMD_DATA,0x35},
	{DSICMD_DATA,0x53},
	{DSICMD_DATA,0x2F},
	{DSICMD_DATA,0x1A},
	{DSICMD_DATA,0x10},
	{DSICMD_DATA,0x0C},
	{DSICMD_DATA,0x08},
	{DSICMD_DATA,0x04},
	{DSICMD_DATA,0x02},
	{DSICMD_DATA,0x14},
	{DSICMD_DATA,0x1C},
	{DSICMD_DATA,0x26},
	{DSICMD_DATA,0x35},
	{DSICMD_DATA,0x53},
	{DSICMD_DATA,0x2F},
	{DSICMD_DATA,0x1A},
	{DSICMD_DATA,0x10},
	{DSICMD_DATA,0x0C},
	{DSICMD_DATA,0x08},
	{DSICMD_DATA,0x04},

	{DSICMD_CMD,0xD0},//D0
	{DSICMD_DATA,0x29},
	{DSICMD_DATA,0x03},
	{DSICMD_DATA,0xCC},
	{DSICMD_DATA,0xA6},
	{DSICMD_DATA,0x00},// 000C :Switching regulator method    0000 :Charge pump method
	{DSICMD_DATA,0x53},
	{DSICMD_DATA,0x20},
	{DSICMD_DATA,0x10},
	{DSICMD_DATA,0x01},
	{DSICMD_DATA,0x00},
	{DSICMD_DATA,0x01},
	{DSICMD_DATA,0x01},
	{DSICMD_DATA,0x00},
	{DSICMD_DATA,0x03},
	{DSICMD_DATA,0x01},
	{DSICMD_DATA,0x00},

	{DSICMD_CMD,0xD1},//D1
	{DSICMD_DATA,0x18},
	{DSICMD_DATA,0x0C},
	{DSICMD_DATA,0x23},
	{DSICMD_DATA,0x03},
	{DSICMD_DATA,0x75},
	{DSICMD_DATA,0x02},
	{DSICMD_DATA,0x50},

	{DSICMD_CMD,0xD3},//D3
	{DSICMD_DATA,0x33},   

	{DSICMD_CMD,0xD5},//D5
	{DSICMD_DATA,0x2A},
	{DSICMD_DATA,0x2A},

	{DSICMD_CMD,0xDE},//DE
	{DSICMD_DATA,0x01},
	{DSICMD_DATA,0x59},

	{DSICMD_CMD,0xFA},//FA
	{DSICMD_DATA,0x03},

	{DSICMD_CMD,0xD6},//D6
	{DSICMD_DATA,0x28},
	{CMDDELAY_MS,200},

	{DSICMD_CMD,0xB0},//B0
	{DSICMD_DATA,0x03},

	{DSICMD_CMD,0x2A},//2
	{DSICMD_DATA,0x00},
	{DSICMD_DATA,0x00},
	{DSICMD_DATA,0x01},
	{DSICMD_DATA,0xDF},

	{DSICMD_CMD,0x2B},//2
	{DSICMD_DATA,0x00},
	{DSICMD_DATA,0x00},
	{DSICMD_DATA,0x03},
	{DSICMD_DATA,0x1F},

	{DSICMD_CMD,0x36},//36
	{DSICMD_DATA,0x48},//可以改方向0x08

	{DSICMD_CMD,0x3A},//3
	{DSICMD_DATA,0x77},  

	//{DSICMD_CMD,0x35},//3
	//{DSICMD_DATA,0x00},  

	{DSICMD_CMD,0x11},//29
	{CMDDELAY_MS,200},

	{DSICMD_CMD,0x29},//29
	{CMDDELAY_MS,200},
};

const tPANEL_CMD tCmdStandbyDSI[] =
{
    {DSICMD_CMD,     0x28},     // Display OFF
    {DSICMD_CMD,     0x10}      // Sleep in
};


const tLCD_PARAM tModeDSI[] =
{
    /***********       MI Serial Format 1      *************/
    {
        // tPANEL_PARAM
        {
            /* Old prototype */
            PINMUX_DSI_2_LANE_CMD_MODE_RGB888,
            _IDE_fDCLK,                             //!< fDCLK
            (0xF0+PANEL_WIDTH),             //!< uiHSyncTotalPeriod(HTOTAL)
            PANEL_WIDTH,                    //!< uiHSyncActivePeriod(HACT)
            0x06,                           //!< uiHSyncBackPorch(HBP)
            0x52 + PANEL_HEIGHT,            //!< uiVSyncTotalPeriod(VTOTAL)
            PANEL_HEIGHT,                           //!< uiVSyncActivePeriod
            VVALIDST,                       //!< uiVSyncBackPorchOdd
            VVALIDST,                       //!< uiVSyncBackPorchEven
            PANEL_HEIGHT,                   //!< uiBufferWidth
            PANEL_WIDTH,                    //!< uiBufferHeight
            PANEL_WIDTH,                    //!< uiWindowWidth
            PANEL_HEIGHT,                   //!< uiWindowHeight
            FALSE,                           //!< bYCbCrFormat
            /* New added parameters */
            HSYNCT,                         //!< uiHSyncSYNCwidth
            HSYNCT                          //!< uiVSyncSYNCwidth
        },

        // tIDE_PARAM
        {
            /* Old prototype */
            PINMUX_LCD_SEL_GPIO,            //!< pinmux_select_lcd;
            ICST_CCIR601,                   //!< icst;
            {TRUE, FALSE},                  //!< dithering[2];
            DISPLAY_DEVICE_MIPIDSI,         //!< **DONT-CARE**
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
            {0x00,0x00},                //!< **DONT-CARE**

            /* New added parameters */
            FALSE,                          //!< YCex
            FALSE,                          //!< HLPF
            {FALSE,FALSE,FALSE},            //!< subpixOdd[3]
            {FALSE,FALSE,FALSE},            //!< subpixEven[3]
            {IDE_DITHER_5BITS,IDE_DITHER_6BITS,IDE_DITHER_5BITS},//!< DitherBits[3]
            FALSE                           //!< clk1/2
        },

        (tPANEL_CMD*)tCmdModeDSI,                  //!< pCmdQueue
        sizeof(tCmdModeDSI)/sizeof(tPANEL_CMD),    //!< nCmd
    }
};

const tLCD_ROT *tRotDSI = NULL;

//@}

tLCD_ROT* dispdev_getLcdRotateDSICmd(UINT32 *ModeNumber)
{
    if(tRotDSI != NULL)
    {
        *ModeNumber = sizeof(tRotDSI)/sizeof(tLCD_ROT);
    }
    else
    {
        *ModeNumber = 0;
    }
    return (tLCD_ROT*)tRotDSI;
}

tLCD_PARAM* dispdev_getConfigModeDSI(UINT32 *ModeNumber)
{
    *ModeNumber = sizeof(tModeDSI)/sizeof(tLCD_PARAM);
    return (tLCD_PARAM*)tModeDSI;
}

tPANEL_CMD* dispdev_getStandbyCmdDSI(UINT32 *CmdNumber)
{
    *CmdNumber = sizeof(tCmdStandbyDSI)/sizeof(tPANEL_CMD);
    return (tPANEL_CMD*)tCmdStandbyDSI;
}


void dispdev_setDSIConfig(tDSI_CONFIG *pDsiConfig)
{
    dsi_setConfig(DSI_CONFIG_ID_FREQ, DSI_TARGET_CLK * 1000000);
#if(DSI_TARGET_CLK == 160)
    dsi_setConfig(DSI_CONFIG_ID_TLPX, 1);
    dsi_setConfig(DSI_CONFIG_ID_BTA_TA_GO, 7);

    dsi_setConfig(DSI_CONFIG_ID_THS_PREPARE, 1);
    dsi_setConfig(DSI_CONFIG_ID_THS_ZERO, 4);
    dsi_setConfig(DSI_CONFIG_ID_THS_TRAIL, 2);
    dsi_setConfig(DSI_CONFIG_ID_THS_EXIT, 3);

    dsi_setConfig(DSI_CONFIG_ID_TCLK_PREPARE, 1);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_ZERO, 7);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_POST, 8);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_PRE, 1);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_TRAIL, 1);
#elif(DSI_TARGET_CLK == 480)
    dsi_setConfig(DSI_CONFIG_ID_TLPX, 3);
    dsi_setConfig(DSI_CONFIG_ID_BTA_TA_GO, 15);

    dsi_setConfig(DSI_CONFIG_ID_THS_PREPARE, 4);
    dsi_setConfig(DSI_CONFIG_ID_THS_ZERO, 5);
    dsi_setConfig(DSI_CONFIG_ID_THS_TRAIL, 4);
    dsi_setConfig(DSI_CONFIG_ID_THS_EXIT, 7);

    dsi_setConfig(DSI_CONFIG_ID_TCLK_PREPARE, 3);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_ZERO, 14);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_POST, 10);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_PRE, 1);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_TRAIL, 3);

#elif(DSI_TARGET_CLK == 240)
    dsi_setConfig(DSI_CONFIG_ID_TLPX, 1);
    dsi_setConfig(DSI_CONFIG_ID_BTA_TA_GO, 7);

    dsi_setConfig(DSI_CONFIG_ID_THS_PREPARE, 1);
    dsi_setConfig(DSI_CONFIG_ID_THS_ZERO, 4);
    dsi_setConfig(DSI_CONFIG_ID_THS_TRAIL, 3);
    dsi_setConfig(DSI_CONFIG_ID_THS_EXIT, 4);

    dsi_setConfig(DSI_CONFIG_ID_TCLK_PREPARE, 2);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_ZERO, 9);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_POST, 9);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_PRE, 1);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_TRAIL, 2);
#elif(DSI_TARGET_CLK == 120)
    dsi_setConfig(DSI_CONFIG_ID_TLPX, 0);
    dsi_setConfig(DSI_CONFIG_ID_BTA_TA_GO, 3);

    dsi_setConfig(DSI_CONFIG_ID_THS_PREPARE, 1);
    dsi_setConfig(DSI_CONFIG_ID_THS_ZERO, 2);
    dsi_setConfig(DSI_CONFIG_ID_THS_TRAIL, 1);
    dsi_setConfig(DSI_CONFIG_ID_THS_EXIT, 1);

    dsi_setConfig(DSI_CONFIG_ID_TCLK_PREPARE, 0);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_ZERO, 5);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_POST, 8);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_PRE, 1);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_TRAIL, 1);
#endif
//#if (HX8379C_CFG_LANE == HX8379C_CFG_1_LANE)
//    dsi_setConfig(DSI_CONFIG_ID_DATALANE_NO, DSI_DATA_LANE_0);
//#else
    dsi_setConfig(DSI_CONFIG_ID_DATALANE_NO, DSI_DATA_LANE_1);
//#endif
    //dsi_setConfig(DSI_CONFIG_ID_TE_BTA_INTERVAL, 0x1F);
    //dsi_setConfig(DSI_CONFIG_ID_CLK_PHASE_OFS, 0x3);
    //dsi_setConfig(DSI_CONFIG_ID_PHASE_DELAY_ENABLE_OFS, 0x1);

    dsi_setConfig(DSI_CONFIG_ID_CLK_LP_CTRL, 0x1);
    //dsi_setConfig(DSI_CONFIG_ID_SYNC_DLY_CNT, 0xF);

    dsi_setConfig(DSI_CONFIG_ID_EOT_PKT_EN, 0);
}
