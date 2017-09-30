/*
    Display object for driving DSI device

    @file       HX8369B_dsi.c
    @ingroup
    @note       This panel MUST select IDE clock to PLL1 ( 480 ). Once change to \n
                another frequence, the _IDE_fDCLK should be re calculated

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "display.h"
#include "dispdev_IFdsi.h"
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

#define REVERSE_180_DEGREE ENABLE

#define PANEL_WIDTH     480
#define PANEL_HEIGHT    864



#define HX8369B_IND_MSG(...)       debug_msg(__VA_ARGS__)

#define HX8369B_ERR_MSG(...)       debug_msg(__VA_ARGS__)

#define HX8369B_WRN_MSG(...)       debug_msg(__VA_ARGS__)

#define HX8369B_TE_OFF              0
#define HX8369B_TE_ON               1

#define HX8369B_TE_PACKET           0
#define HX8369B_TE_PIN              1

/*
    RGB888 = 1 pixel = 3bytes packet
    If DSI src = 240MHz, internal clock = 30MHz, data rate = 30MHz x 1bytes = 30MB / sec per lane
    2 lane = 60MB = 20Mpixel ' IDE need > 20MHz
*/


#define DSI_FORMAT_RGB565          0    //N/A in HX8369B
#define DSI_FORMAT_RGB666P         1    //N/A in HX8369B
#define DSI_FORMAT_RGB666L         2    //N/A in HX8369B
#define DSI_FORMAT_RGB888          3    //IDE use 480 & DSI use 480

#define DSI_PACKET_FORMAT          DSI_FORMAT_RGB888


#if 1
#define DSI_TARGET_CLK             480
#else
#define DSI_TARGET_CLK             240
#endif


#if(DSI_PACKET_FORMAT == DSI_FORMAT_RGB888)
#if (DSI_TARGET_CLK == 480)
#define _IDE_fDCLK      60.00f //((((DSI_TARGET_CLK / 8) * 2)/3))
#else
#define _IDE_fDCLK      24.00f// must > ((((DSI_TARGET_CLK / 8) * 2) / 3)) ->2: lane number
#endif
#else
#error "Format not RGB888"
#endif

#define HVALIDST    0x39  //uiHSyncBackPorch(HPB) -> 53
#define VVALIDST    0x0A  //uiVSyncBackPorchOdd/Even
#define HSYNCT      0x02
#define VSYNCT      0x02

#define ROTATE_180DEGREE      DISABLE//ENABLE//
/*
    Panel Parameters for TCON HX8369B
*/
//@{
/*Used in DSI*/
const tPANEL_CMD tCmdModeDSI[] =
{

#if 1

    {DSICMD_CMD,0xBF}, // Password
    {DSICMD_DATA,0x91}, //
    {DSICMD_DATA,0x61}, //
    {DSICMD_DATA,0xF2}, //

    {DSICMD_CMD,0xB3}, //VCOM
    {DSICMD_DATA,0x00}, //
    {DSICMD_DATA,0x69}, //

    {DSICMD_CMD,0xB4}, //VCOM_R
    {DSICMD_DATA,0x00}, //
    {DSICMD_DATA,0x69}, //

    {DSICMD_CMD,0xB8}, //VGMP, VGSP, VGMN, VGSN
    {DSICMD_DATA,0x00}, //
    {DSICMD_DATA,0x9F}, //VGMP[7:0] //
    {DSICMD_DATA,0x01}, //VGSP[7:0] //
    {DSICMD_DATA,0x00}, //
    {DSICMD_DATA,0x9F}, //VGMN[7:0] //
    {DSICMD_DATA,0x01}, //VGSN[7:0] //

    {DSICMD_CMD,0xBA}, //GIP output voltage level.
    {DSICMD_DATA,0x34}, //VGH_REG[6:0]
    {DSICMD_DATA,0x23}, //VGL_REG[5:0]
    {DSICMD_DATA,0x00}, //

    {DSICMD_CMD,0xC3}, //SET RGB CYC
    {DSICMD_DATA,0x04}, //Column

    {DSICMD_CMD,0xC4}, //SET TCON
    {DSICMD_DATA,0x30}, //
    {DSICMD_DATA,0x6A}, //854 LINE


    {DSICMD_CMD,0xC7}, //POWER CTRL
    {DSICMD_DATA,0x00}, //DCDCM[3:0]
    {DSICMD_DATA,0x01}, //AVDD_RT[1:0]
    {DSICMD_DATA,0x32}, //
    {DSICMD_DATA,0x05}, //
    {DSICMD_DATA,0x65}, //
    {DSICMD_DATA,0x2C}, //
    {DSICMD_DATA,0x13}, //
    {DSICMD_DATA,0xA5}, //
    {DSICMD_DATA,0xA5}, //

    #if(REVERSE_180_DEGREE == ENABLE)
    {DSICMD_CMD,0x36},  //MADCTL
    {DSICMD_DATA,0xC0},
    #endif

    //Gamma 2.2

    {DSICMD_CMD,0xC8}, //Gamma
    {DSICMD_DATA,0x60}, //
    {DSICMD_DATA,0x4E}, //
    {DSICMD_DATA,0x3F}, //
    {DSICMD_DATA,0x34}, //
    {DSICMD_DATA,0x33}, //
    {DSICMD_DATA,0x25}, //
    {DSICMD_DATA,0x2B}, //
    {DSICMD_DATA,0x16}, //
    {DSICMD_DATA,0x30}, //
    {DSICMD_DATA,0x2F}, //
    {DSICMD_DATA,0x30}, //
    {DSICMD_DATA,0x4E}, //
    {DSICMD_DATA,0x3C}, //
    {DSICMD_DATA,0x44}, //
    {DSICMD_DATA,0x36}, //
    {DSICMD_DATA,0x34}, //
    {DSICMD_DATA,0x2A}, //
    {DSICMD_DATA,0x23}, //
    {DSICMD_DATA,0x1F}, //
    {DSICMD_DATA,0x60}, //
    {DSICMD_DATA,0x4E}, //
    {DSICMD_DATA,0x3F}, //
    {DSICMD_DATA,0x34}, //
    {DSICMD_DATA,0x33}, //
    {DSICMD_DATA,0x25}, //
    {DSICMD_DATA,0x2B}, //
    {DSICMD_DATA,0x16}, //
    {DSICMD_DATA,0x30}, //
    {DSICMD_DATA,0x2F}, //
    {DSICMD_DATA,0x30}, //
    {DSICMD_DATA,0x4E}, //
    {DSICMD_DATA,0x3C}, //
    {DSICMD_DATA,0x44}, //
    {DSICMD_DATA,0x36}, //
    {DSICMD_DATA,0x34}, //
    {DSICMD_DATA,0x2A}, //
    {DSICMD_DATA,0x23}, //
    {DSICMD_DATA,0x1F}, //

    {DSICMD_CMD,0xD4}, //CGOUTx_L GS=0
    {DSICMD_DATA,0x1F}, //
    {DSICMD_DATA,0x1E}, //
    {DSICMD_DATA,0x05}, //
    {DSICMD_DATA,0x07}, //
    {DSICMD_DATA,0x01}, //
    {DSICMD_DATA,0x1F}, //
    {DSICMD_DATA,0x1F}, //
    {DSICMD_DATA,0x1F}, //
    {DSICMD_DATA,0x1F}, //
    {DSICMD_DATA,0x1F}, //
    {DSICMD_DATA,0x1F}, //
    {DSICMD_DATA,0x1F}, //
    {DSICMD_DATA,0x1F}, //
    {DSICMD_DATA,0x1F}, //
    {DSICMD_DATA,0x1F}, //
    {DSICMD_DATA,0x1F}, //

    {DSICMD_CMD,0xD5}, //CGOUTx_R GS=0
    {DSICMD_DATA,0x1F}, //
    {DSICMD_DATA,0x1E}, //
    {DSICMD_DATA,0x04}, //
    {DSICMD_DATA,0x06}, //
    {DSICMD_DATA,0x00}, //
    {DSICMD_DATA,0x1F}, //
    {DSICMD_DATA,0x1F}, //
    {DSICMD_DATA,0x1F}, //
    {DSICMD_DATA,0x1F}, //
    {DSICMD_DATA,0x1F}, //
    {DSICMD_DATA,0x1F}, //
    {DSICMD_DATA,0x1F}, //
    {DSICMD_DATA,0x1F}, //
    {DSICMD_DATA,0x1F}, //
    {DSICMD_DATA,0x1F}, //
    {DSICMD_DATA,0x1F}, //

    {DSICMD_CMD,0xD6}, //CGOUTx_L GS=1
    {DSICMD_DATA,0x1F}, //
    {DSICMD_DATA,0x1F}, //
    {DSICMD_DATA,0x06}, //
    {DSICMD_DATA,0x04}, //
    {DSICMD_DATA,0x00}, //
    {DSICMD_DATA,0x1E}, //
    {DSICMD_DATA,0x1F}, //
    {DSICMD_DATA,0x1F}, //
    {DSICMD_DATA,0x1F}, //
    {DSICMD_DATA,0x1F}, //
    {DSICMD_DATA,0x1F}, //
    {DSICMD_DATA,0x1F}, //
    {DSICMD_DATA,0x1F}, //
    {DSICMD_DATA,0x1F}, //
    {DSICMD_DATA,0x1F}, //
    {DSICMD_DATA,0x1F}, //

    {DSICMD_CMD,0xD7}, //CGOUTx_R GS=1
    {DSICMD_DATA,0x1F}, //
    {DSICMD_DATA,0x1F}, //
    {DSICMD_DATA,0x07}, //
    {DSICMD_DATA,0x05}, //
    {DSICMD_DATA,0x01}, //
    {DSICMD_DATA,0x1E}, //
    {DSICMD_DATA,0x1F}, //
    {DSICMD_DATA,0x1F}, //
    {DSICMD_DATA,0x1F}, //
    {DSICMD_DATA,0x1F}, //
    {DSICMD_DATA,0x1F}, //
    {DSICMD_DATA,0x1F}, //
    {DSICMD_DATA,0x1F}, //
    {DSICMD_DATA,0x1F}, //
    {DSICMD_DATA,0x1F}, //
    {DSICMD_DATA,0x1F}, //

    {DSICMD_CMD,0xD8}, //SETGIP1
    {DSICMD_DATA,0x20}, //
    {DSICMD_DATA,0x00}, //
    {DSICMD_DATA,0x00}, //
    {DSICMD_DATA,0x10}, //
    {DSICMD_DATA,0x03}, //
    {DSICMD_DATA,0x10}, //
    {DSICMD_DATA,0x01}, //
    {DSICMD_DATA,0x02}, //
    {DSICMD_DATA,0x00}, //
    {DSICMD_DATA,0x01}, //
    {DSICMD_DATA,0x02}, //
    {DSICMD_DATA,0x03}, //
    {DSICMD_DATA,0x7b}, //
    {DSICMD_DATA,0x00}, //
    {DSICMD_DATA,0x00}, //
    {DSICMD_DATA,0x31}, //
    {DSICMD_DATA,0x04}, //
    {DSICMD_DATA,0x06}, //
    {DSICMD_DATA,0x7b}, //
    {DSICMD_DATA,0x08}, //

    {DSICMD_CMD,0xD9}, // SETGIP2
    {DSICMD_DATA,0x00}, //
    {DSICMD_DATA,0x00}, //
    {DSICMD_DATA,0x00}, //
    {DSICMD_DATA,0x88}, //
    {DSICMD_DATA,0x00}, //
    {DSICMD_DATA,0x00}, //
    {DSICMD_DATA,0x06}, //
    {DSICMD_DATA,0x7b}, //
    {DSICMD_DATA,0x00}, //
    {DSICMD_DATA,0x00}, //
    {DSICMD_DATA,0x00}, //
    {DSICMD_DATA,0x3B}, //
    {DSICMD_DATA,0x2F}, //
    {DSICMD_DATA,0x1F}, //
    {DSICMD_DATA,0x00}, //
    {DSICMD_DATA,0x00}, //
    {DSICMD_DATA,0x00}, //
    {DSICMD_DATA,0x03}, //
    {DSICMD_DATA,0x7b}, //

    {DSICMD_CMD,0x11}, // SLPOUT
    {CMDDELAY_MS,   120},

    {DSICMD_CMD,0x29}, // DSPON
    {CMDDELAY_MS,   120},

#else
    {DSICMD_CMD,    0xFF},
    {DSICMD_DATA,   0xFF},
    {DSICMD_DATA,   0x98},
    {DSICMD_DATA,   0x06},

    {DSICMD_CMD,    0xBA},
    {DSICMD_DATA,   0x60},

    {DSICMD_CMD,    0xBC},
    {DSICMD_DATA,   0x01},
    {DSICMD_DATA,   0x0E},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x01},
    {DSICMD_DATA,   0x01},
    {DSICMD_DATA,   0x0B},
    {DSICMD_DATA,   0x0F},
    {DSICMD_DATA,   0x30},
    {DSICMD_DATA,   0x10},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x01},
    {DSICMD_DATA,   0x01},
    {DSICMD_DATA,   0x03},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x40},
    {DSICMD_DATA,   0x42},
    {DSICMD_DATA,   0x01},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x40},

    {DSICMD_CMD,    0xBD},
    {DSICMD_DATA,   0x01},
    {DSICMD_DATA,   0x23},
    {DSICMD_DATA,   0x45},
    {DSICMD_DATA,   0x67},
    {DSICMD_DATA,   0x01},
    {DSICMD_DATA,   0x23},
    {DSICMD_DATA,   0x45},
    {DSICMD_DATA,   0x67},

    {DSICMD_CMD,    0xBE},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x21},
    {DSICMD_DATA,   0xAB},
    {DSICMD_DATA,   0x60},
    {DSICMD_DATA,   0x22},
    {DSICMD_DATA,   0x22},
    {DSICMD_DATA,   0x22},
    {DSICMD_DATA,   0x22},
    {DSICMD_DATA,   0x22},

    {DSICMD_CMD,    0xC7},
    {DSICMD_DATA,   0x47},      //VCOM
    {DSICMD_CMD,    0xC0},
    {DSICMD_DATA,   0x2B},
    {DSICMD_DATA,   0x0B},
    {DSICMD_DATA,   0x0C},  //03 , 0B ,0C   //POWER

    {DSICMD_CMD,    0xFC},
    {DSICMD_DATA,   0x08},  //LVGL

    {DSICMD_CMD,    0xDF},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x20},  //Engineering

    {DSICMD_CMD,    0xF3},
    {DSICMD_DATA,   0x74},  //DVDD

    {DSICMD_CMD,    0xB4},
    {DSICMD_DATA,   0x02},
    {DSICMD_DATA,   0x02},
    {DSICMD_DATA,   0x02},  //Display Inversion

    {DSICMD_CMD,    0xF7},
    {DSICMD_DATA,   0x81},      //Panel Resolution Selection Set

    {DSICMD_CMD,    0xB1},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x13},
    {DSICMD_DATA,   0x13},      //Frame Rate Control

    {DSICMD_CMD,    0xF2},
    {DSICMD_DATA,   0x80},
    {DSICMD_DATA,   0x04},
    {DSICMD_DATA,   0x40},
    {DSICMD_DATA,   0x28},  //Panel Timing Control

    {DSICMD_CMD,    0xC1},
    {DSICMD_DATA,   0x17},
    {DSICMD_DATA,   0x7A},
    {DSICMD_DATA,   0x7A},
    {DSICMD_DATA,   0x20},          ////////////POWER

    {DSICMD_CMD,    0xE0},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x14},
    {DSICMD_DATA,   0x1B},
    {DSICMD_DATA,   0x0D},
    {DSICMD_DATA,   0x0E},
    {DSICMD_DATA,   0x0C},
    {DSICMD_DATA,   0x07},
    {DSICMD_DATA,   0x05},
    {DSICMD_DATA,   0x05},
    {DSICMD_DATA,   0x0A},
    {DSICMD_DATA,   0x0F},
    {DSICMD_DATA,   0x0C},
    {DSICMD_DATA,   0x0C},
    {DSICMD_DATA,   0x18},
    {DSICMD_DATA,   0x14},
    {DSICMD_DATA,   0x00},  //Positive Gamma

    {DSICMD_CMD,    0xE1},
    {DSICMD_DATA,   0x00},
    {DSICMD_DATA,   0x14},
    {DSICMD_DATA,   0x1B},
    {DSICMD_DATA,   0x0D},
    {DSICMD_DATA,   0x0E},
    {DSICMD_DATA,   0x0C},
    {DSICMD_DATA,   0x07},
    {DSICMD_DATA,   0x05},
    {DSICMD_DATA,   0x05},
    {DSICMD_DATA,   0x0A},
    {DSICMD_DATA,   0x0F},
    {DSICMD_DATA,   0x0C},
    {DSICMD_DATA,   0x0C},
    {DSICMD_DATA,   0x18},
    {DSICMD_DATA,   0x14},
    {DSICMD_DATA,   0x00},  //Negative Gamma

    {DSICMD_CMD,    0x35},
    {DSICMD_DATA,   0x00},  //TE

    {DSICMD_CMD,    0x11},
    {CMDDELAY_MS,    120},
    {DSICMD_CMD,    0x29},
    {CMDDELAY_MS,    10},
#endif
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
            PINMUX_DSI_2_LANE_VDO_SYNC_EVENT_RGB888,
            _IDE_fDCLK,                     //!< fDCLK
            #if 1 //(DSI_TARGET_CLK == 240)
            (0xF0+PANEL_WIDTH+2),             //!< uiHSyncTotalPeriod(HTOTAL)
            #else
            (0xC0+PANEL_WIDTH),             //!< uiHSyncTotalPeriod(HTOTAL)
            #endif
            PANEL_WIDTH,                    //!< uiHSyncActivePeriod(HACT)
            0x06,                           //!< uiHSyncBackPorch(HBP)
            0x52 + PANEL_HEIGHT, //0x52 + PANEL_HEIGHT,            //!< uiVSyncTotalPeriod(VTOTAL)
            PANEL_HEIGHT,                   //!< uiVSyncActivePeriod
            VVALIDST,                       //!< uiVSyncBackPorchOdd
            VVALIDST,                       //!< uiVSyncBackPorchEven
            PANEL_HEIGHT,                   //!< uiBufferWidth
            PANEL_WIDTH,                    //!< uiBufferHeight
            PANEL_WIDTH,                    //!< uiWindowWidth
            PANEL_HEIGHT,                   //!< uiWindowHeight
            FALSE,                          //!< bYCbCrFormat
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
            {0x00,0x00},                    //!< **DONT-CARE**

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
    #if 0
    if(tRotDSI != NULL)
    {
        *ModeNumber = sizeof(tRotDSI)/sizeof(tLCD_ROT);
    }
    else
	#endif
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

#if (DSI_TARGET_CLK == 480)
    dsi_setConfig(DSI_CONFIG_ID_TLPX, 3);
    dsi_setConfig(DSI_CONFIG_ID_BTA_TA_GO, 0x15);
    dsi_setConfig(DSI_CONFIG_ID_BTA_TA_SURE, 0);
    dsi_setConfig(DSI_CONFIG_ID_BTA_TA_GET, 0x14);

    dsi_setConfig(DSI_CONFIG_ID_THS_PREPARE, 4);
    dsi_setConfig(DSI_CONFIG_ID_THS_ZERO, 6);
    dsi_setConfig(DSI_CONFIG_ID_THS_TRAIL, 7);
    dsi_setConfig(DSI_CONFIG_ID_THS_EXIT, 6);

    dsi_setConfig(DSI_CONFIG_ID_TCLK_PREPARE, 2);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_ZERO, 16);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_POST, 16);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_PRE, 2);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_TRAIL, 3);

    dsi_setConfig(DSI_CONFIG_ID_CLK_PHASE_OFS, 0x4);
    dsi_setConfig(DSI_CONFIG_ID_PHASE_DELAY_ENABLE_OFS, 0x1);
#elif(DSI_TARGET_CLK == 240)
    dsi_setConfig(DSI_CONFIG_ID_TLPX, 3);
    dsi_setConfig(DSI_CONFIG_ID_BTA_TA_GO, 0x15);
    dsi_setConfig(DSI_CONFIG_ID_BTA_TA_SURE, 0);
    dsi_setConfig(DSI_CONFIG_ID_BTA_TA_GET, 0x14);

    dsi_setConfig(DSI_CONFIG_ID_THS_PREPARE, 4);//4
    dsi_setConfig(DSI_CONFIG_ID_THS_ZERO, 6);
    dsi_setConfig(DSI_CONFIG_ID_THS_TRAIL, 7);
    dsi_setConfig(DSI_CONFIG_ID_THS_EXIT, 6);

    dsi_setConfig(DSI_CONFIG_ID_TCLK_PREPARE, 3);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_ZERO, 16);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_POST, 16);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_PRE, 2);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_TRAIL, 3);

    dsi_setConfig(DSI_CONFIG_ID_CLK_PHASE_OFS, 0x7);
    dsi_setConfig(DSI_CONFIG_ID_PHASE_DELAY_ENABLE_OFS, 0x1);
#endif


//#if (HX8369B_CFG_LANE == HX8369B_CFG_1_LANE)
//    dsi_setConfig(DSI_CONFIG_ID_DATALANE_NO, DSI_DATA_LANE_0);
//#else
    dsi_setConfig(DSI_CONFIG_ID_DATALANE_NO, DSI_DATA_LANE_1);
//#endif
    //dsi_setConfig(DSI_CONFIG_ID_TE_BTA_INTERVAL, 0x1F);

    dsi_setConfig(DSI_CONFIG_ID_CLK_LP_CTRL, 0x1);
    //dsi_setConfig(DSI_CONFIG_ID_SYNC_DLY_CNT, 0xF);

    dsi_setConfig(DSI_CONFIG_ID_EOT_PKT_EN, 0);
}
