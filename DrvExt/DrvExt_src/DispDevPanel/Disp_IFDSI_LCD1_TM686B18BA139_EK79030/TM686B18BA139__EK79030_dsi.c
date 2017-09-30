
#if 1
/*
    Display object for driving DSI device

    @file       TG078UW006A0_dsi.c
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

#define PANEL_WIDTH     480//1280
#define PANEL_HEIGHT    1280//400



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


#if 0
#define DSI_TARGET_CLK             480
#elif 1
#define DSI_TARGET_CLK             320
#else
#if 1
#define DSI_TARGET_CLK             240
#else
#define DSI_TARGET_CLK             120
#endif
#endif


#if(DSI_PACKET_FORMAT == DSI_FORMAT_RGB888)
#if (DSI_TARGET_CLK == 480)
#define _IDE_fDCLK      60.00f //((((DSI_TARGET_CLK / 8) * 2)/3))
#elif(DSI_TARGET_CLK == 320)
#define _IDE_fDCLK      60.00f 
#else
#if (DSI_TARGET_CLK == 240)
#define _IDE_fDCLK      60.00f //80.00f//54.00f//53.00f//60.00f //24.00f// must > ((((DSI_TARGET_CLK / 8) * 2) / 3)) ->2: lane number
#else
#define _IDE_fDCLK      13.33f
#endif
#endif
#else
#error "Format not RGB888"
#endif

/*
MIPI CLK Speed:349Mbps(175MHZ)
H blank porch:160
H front porch:160
H pulse width:24


V blank porch:10
V front porch:12
V pulse width:2
*/
#define HVALIDST    160//160 //0x39  //uiHSyncBackPorch(HPB) -> 53
#define VVALIDST    10  //0x15  //uiVSyncBackPorchOdd/Even
#define HSYNCT      24  //24//0x02
#define VSYNCT      0x02

#define ROTATE_180DEGREE      DISABLE//ENABLE//
/*
    Panel Parameters for TCON HX8369B
*/

const tPANEL_CMD tCmdModeDSI[] =
{
#if 1

    {DSICMD_CMD,0xCD},
    {DSICMD_DATA,0xAA},	

    //{DSICMD_CMD,0x61},
    //{DSICMD_DATA,0x84},
    
    {DSICMD_CMD,0x65},
    {DSICMD_DATA,0x08},	//{DSICMD_DATA,0xAC},//{DSICMD_DATA,0x08},	

    {DSICMD_CMD,0x3A},
    {DSICMD_DATA,0x14},

    {DSICMD_CMD,0x32},
    {DSICMD_DATA,0x00},	//0x00   // BIST  

    {DSICMD_CMD,0x36},
    {DSICMD_DATA,0x02},

    {DSICMD_CMD,0x67},
    {DSICMD_DATA,0x82},

    {DSICMD_CMD,0x69},
    {DSICMD_DATA,0x20},
        
    {DSICMD_CMD,0x6D},
    {DSICMD_DATA,0x01},

    {DSICMD_CMD,0x68},
    {DSICMD_DATA,0x00},

    {DSICMD_CMD,0x53},
    {DSICMD_DATA,0x1F},
    {DSICMD_DATA,0x19},
    {DSICMD_DATA,0x17},
    {DSICMD_DATA,0x09},
    {DSICMD_DATA,0x09},
    {DSICMD_DATA,0x10},
    {DSICMD_DATA,0x14},
    {DSICMD_DATA,0x14},
    {DSICMD_DATA,0x14},
    {DSICMD_DATA,0x09},
    {DSICMD_DATA,0x0A},
    {DSICMD_DATA,0x06},
    {DSICMD_DATA,0x05},
    {DSICMD_DATA,0x08},
    {DSICMD_DATA,0x07},
    {DSICMD_DATA,0x09},
    {DSICMD_DATA,0x06},
    {DSICMD_DATA,0x05},
    {DSICMD_DATA,0x04},

    {DSICMD_CMD,0x54},
    {DSICMD_DATA,0x1F},
    {DSICMD_DATA,0x19},
    {DSICMD_DATA,0x16},
    {DSICMD_DATA,0x17},
    {DSICMD_DATA,0x17},
    {DSICMD_DATA,0x12},
    {DSICMD_DATA,0x11},
    {DSICMD_DATA,0x14},
    {DSICMD_DATA,0x14},
    {DSICMD_DATA,0x16},
    {DSICMD_DATA,0x08},
    {DSICMD_DATA,0x08},
    {DSICMD_DATA,0x07},
    {DSICMD_DATA,0x07},
    {DSICMD_DATA,0x0A},
    {DSICMD_DATA,0x0B},
    {DSICMD_DATA,0x08},
    {DSICMD_DATA,0x06},
    {DSICMD_DATA,0x06},

    {DSICMD_CMD,0x29},
    {DSICMD_DATA,0x10},

    {DSICMD_CMD,0x2A},
    {DSICMD_DATA,0x0C},

    {DSICMD_CMD,0x6C},
    {DSICMD_DATA,0x81},
    
    {DSICMD_CMD,0x55},
    {DSICMD_DATA,0x00},
    {DSICMD_DATA,0x0F},
    {DSICMD_DATA,0x00},
    {DSICMD_DATA,0x0F},
    {DSICMD_DATA,0x00},
    {DSICMD_DATA,0x0F},
    {DSICMD_DATA,0x00},
    {DSICMD_DATA,0x0F},	

    {DSICMD_CMD,0x56},
    {DSICMD_DATA,0x00},
    {DSICMD_DATA,0x0F},
    {DSICMD_DATA,0x00},
    {DSICMD_DATA,0x0F},
    {DSICMD_DATA,0x00},
    {DSICMD_DATA,0x0F},
    {DSICMD_DATA,0x00},
    {DSICMD_DATA,0x0F},
    {DSICMD_DATA,0x00},
    {DSICMD_DATA,0x0F},
    {DSICMD_DATA,0x00},
    {DSICMD_DATA,0x0F},
    {DSICMD_DATA,0x00},
    {DSICMD_DATA,0x0F},
    {DSICMD_DATA,0x00},
    {DSICMD_DATA,0x0F},

    {DSICMD_CMD,0x57},
    {DSICMD_DATA,0x00},
    {DSICMD_DATA,0x00},
    {DSICMD_DATA,0x00},
    {DSICMD_DATA,0x00},

    {DSICMD_CMD,0x30},
    {DSICMD_DATA,0x00},

    {DSICMD_CMD,0x39},
    {DSICMD_DATA,0x11},

    {DSICMD_CMD,0x33},
    {DSICMD_DATA,0x08},

    {DSICMD_CMD,0x35},
    {DSICMD_DATA,0x25},

    {DSICMD_CMD,0x4F},
    {DSICMD_DATA,0x51},

    {DSICMD_CMD,0x4E},
    {DSICMD_DATA,0x45},

    {DSICMD_CMD,0x41},
    {DSICMD_DATA,0x4E},//0x5E

    {DSICMD_CMD,0x73},
    {DSICMD_DATA,0x30},

    {DSICMD_CMD,0x74},
    {DSICMD_DATA,0x10},

    {DSICMD_CMD,0x76},
    {DSICMD_DATA,0x40},

    {DSICMD_CMD,0x77},
    {DSICMD_DATA,0x00},

    {DSICMD_CMD,0x28},
    {DSICMD_DATA,0x31},

    {DSICMD_CMD,0x7c},
    {DSICMD_DATA,0x80},

    {DSICMD_CMD,0x2e},
    {DSICMD_DATA,0x04},//PWM

    {DSICMD_CMD,0x4c},
    {DSICMD_DATA,0x80},//VCSW1

    {DSICMD_CMD,0x47},
    {DSICMD_DATA,0x16},

    {DSICMD_CMD,0x48},
    {DSICMD_DATA,0x6A},

    {DSICMD_CMD,0x50},
    {DSICMD_DATA,0xc0},

    {DSICMD_CMD,0x78},
    {DSICMD_DATA,0x6E},

    {DSICMD_CMD,0x2D},
    {DSICMD_DATA,0x31},

    {DSICMD_CMD,0x63},
    {DSICMD_DATA,0x04},

    {DSICMD_CMD,0x4D},
    {DSICMD_DATA,0x00},
#endif
};


const tPANEL_CMD tCmdStandbyDSI[] =
{
    {DSICMD_CMD,     0x10}      // Sleep in
};

const tLCD_PARAM tModeDSI[] =
{
    /***********       MI Serial Format 1      *************/
    {
        // tPANEL_PARAM
        {
            /* Old prototype */
            PINMUX_DSI_4_LANE_VDO_SYNC_EVENT_RGB888,
            _IDE_fDCLK,                     //!< fDCLK
            #if 1 //(DSI_TARGET_CLK == 240)
            824,//704,//824,//746,                            //!< uiHSyncTotalPeriod(HTOTAL)
            #else
            744,                            //!< uiHSyncTotalPeriod(HTOTAL)
            #endif
            PANEL_WIDTH,                    //!< uiHSyncActivePeriod(HACT)
            HVALIDST,//0xA0,                           //!< uiHSyncBackPorch(HBP)
            1304,                           //!< uiVSyncTotalPeriod(VTOTAL)
            PANEL_HEIGHT,                   //!< uiVSyncActivePeriod
            VVALIDST,                       //!< uiVSyncBackPorchOdd
            VVALIDST,                       //!< uiVSyncBackPorchEven
            #if 1
            PANEL_HEIGHT,                   //!< uiBufferWidth
            PANEL_WIDTH,                    //!< uiBufferHeight
            #else
            PANEL_WIDTH,                    //!< uiBufferWidth
            PANEL_HEIGHT,                   //!< uiBufferHeight
            #endif
            PANEL_WIDTH,                    //!< uiWindowWidth
            PANEL_HEIGHT,                   //!< uiWindowHeight
            FALSE,                          //!< bYCbCrFormat
            /* New added parameters */
            HSYNCT,                         //!< uiHSyncSYNCwidth
            VSYNCT                          //!< uiVSyncSYNCwidth
        },

        // tIDE_PARAM
        {
            /* Old prototype */
            PINMUX_LCD_SEL_GPIO,            //!< pinmux_select_lcd;
            ICST_CCIR601,                   //!< icst;
            {FALSE, FALSE},                  //!< dithering[2];
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
    //dsi_setConfig(DSI_CONFIG_ID_FREQ, 266 * 1000000);
    //dsi_setConfig(DSI_CONFIG_ID_FREQ, 276 * 1000000);
   // dsi_setConfig(DSI_CONFIG_ID_FREQ, 320 * 1000000);
    //dsi_setConfig(DSI_CONFIG_ID_FREQ, 328 * 1000000);
    
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
#elif(DSI_TARGET_CLK == 240 ||DSI_TARGET_CLK == 320)  // To adjust...
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
#elif(DSI_TARGET_CLK == 54)
    dsi_setConfig(DSI_CONFIG_ID_TLPX, 1);
    dsi_setConfig(DSI_CONFIG_ID_BTA_TA_GO, 4);

    dsi_setConfig(DSI_CONFIG_ID_THS_PREPARE, 1);
    dsi_setConfig(DSI_CONFIG_ID_THS_ZERO, 0);
    dsi_setConfig(DSI_CONFIG_ID_THS_TRAIL, 1);
    dsi_setConfig(DSI_CONFIG_ID_THS_EXIT, 4);

    dsi_setConfig(DSI_CONFIG_ID_TCLK_PREPARE, 0);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_ZERO, 2);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_POST, 0);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_PRE, 1);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_TRAIL, 1);
#endif


    #if 0 // 2 lanes.
//#if (HX8369B_CFG_LANE == HX8369B_CFG_1_LANE)
//    dsi_setConfig(DSI_CONFIG_ID_DATALANE_NO, DSI_DATA_LANE_0);
//#else
    dsi_setConfig(DSI_CONFIG_ID_DATALANE_NO, DSI_DATA_LANE_1);
//#endif
    //dsi_setConfig(DSI_CONFIG_ID_TE_BTA_INTERVAL, 0x1F);

    dsi_setConfig(DSI_CONFIG_ID_CLK_LP_CTRL, 0x1);
    //dsi_setConfig(DSI_CONFIG_ID_SYNC_DLY_CNT, 0xF);

    dsi_setConfig(DSI_CONFIG_ID_EOT_PKT_EN, 0);
    #else // 4 lanes.
    dsi_setConfig(DSI_CONFIG_ID_DATALANE_NO, DSI_DATA_LANE_3);
        #if 0
        dsi_setConfig(DSI_CONFIG_ID_TE_BTA_INTERVAL, 0x1F);
        #endif
    //dsi_setConfig(DSI_CONFIG_ID_CLK_PHASE_OFS, 0x3);
    //dsi_setConfig(DSI_CONFIG_ID_PHASE_DELAY_ENABLE_OFS, 0x1);

    dsi_setConfig(DSI_CONFIG_ID_CLK_LP_CTRL, 0x1);
        #if 0
        dsi_setConfig(DSI_CONFIG_ID_SYNC_DLY_CNT, 0xF);
        #endif

    dsi_setConfig(DSI_CONFIG_ID_EOT_PKT_EN, 0);  // Need to set TRUE??
    #endif
}

#else

/*
    Display object for driving DSI device

    @file       NT35521.c
    @ingroup
    @note       This panel MUST select IDE clock to PLL1 ( 480 ). Once change to \n
                another frequence, the _IDE_fDCLK should be re calculated

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "display.h"
#include "dispdev_IFdsi.h"
#include "dispdev_panlcomn.h"

#define PANEL_WIDTH     720
#define PANEL_HEIGHT    1280

#define NT35521_IND_MSG(...)       debug_msg(__VA_ARGS__)

#define NT35521_ERR_MSG(...)       debug_msg(__VA_ARGS__)

#define NT35521_WRN_MSG(...)       debug_msg(__VA_ARGS__)
#define NT35521_TE_OFF              0
#define NT35521_TE_ON               1

#define NT35521_TE_PACKET           0
#define NT35521_TE_PIN              1


#define DSI_FORMAT_RGB565          0
#define DSI_FORMAT_RGB666P         1
#define DSI_FORMAT_RGB666L         2
#define DSI_FORMAT_RGB888          3

#define DSI_OP_MODE_CMD_MODE       1     //NT35521 no CMD mode
#define DSI_OP_MODE_VDO_MODE       0
#define DSI_PACKET_FORMAT          DSI_FORMAT_RGB888

//#define DSI_TARGET_CLK             480
#define DSI_TARGET_CLK             240
//#define DSI_TARGET_CLK             160
//#define DSI_TARGET_CLK             120
//#define DSI_TARGET_CLK             54
#define DSI_OP_MODE                DSI_OP_MODE_VDO_MODE

#if (DSI_PACKET_FORMAT == DSI_FORMAT_RGB666P) && (DSI_OP_MODE == DSI_OP_MODE_CMD_MODE)
#error "Command mode not support RGB666P"
#endif
#if(DSI_PACKET_FORMAT == DSI_FORMAT_RGB888) || (DSI_PACKET_FORMAT == DSI_FORMAT_RGB666L)
#define _IDE_fDCLK      ((((DSI_TARGET_CLK / 8) * 2)/3)*3)
#elif(DSI_PACKET_FORMAT == DSI_FORMAT_RGB666P)
#if (DSI_TARGET_CLK == 240)
#define _IDE_fDCLK      26.66f
#else
//#define _IDE_fDCLK      ((((((DSI_TARGET_CLK / 8) * 2))*9)/4))
#if(DSI_TARGET_CLK == 120)
#define _IDE_fDCLK      13.33f
#else

#endif
#endif
#elif(DSI_PACKET_FORMAT == DSI_FORMAT_RGB565)
#define _IDE_fDCLK      ((((DSI_TARGET_CLK / 8) * 2) / 2))
#endif


/*
    Panel Parameters for TCON NT35521
*/
//@{
/*Used in DSI*/
const tPANEL_CMD tCmdModeDSI[] =
{
    {DSICMD_CMD,     0xFF},
    {DSICMD_DATA,    0xAA},
    {DSICMD_DATA,    0x55},
    {DSICMD_DATA,    0xA5},
    {DSICMD_DATA,    0x80},

    {DSICMD_CMD,     0x6F},
    {DSICMD_DATA,    0x11},
    {DSICMD_DATA,    0x00},

    {DSICMD_CMD,     0xF7},
    {DSICMD_DATA,    0x20},
    {DSICMD_DATA,    0x00},

    {DSICMD_CMD,     0x6F},
    {DSICMD_DATA,    0x06},

    {DSICMD_CMD,     0xF7},
    {DSICMD_DATA,    0x0A},

    {DSICMD_CMD,     0x6F},
    {DSICMD_DATA,    0x19},

    {DSICMD_CMD,     0xF7},
    {DSICMD_DATA,    0x12},

    {DSICMD_CMD,     0x6F},
    {DSICMD_DATA,    0x08},

    {DSICMD_CMD,     0xFA},
    {DSICMD_DATA,    0x40},

    {DSICMD_CMD,     0x6F},
    {DSICMD_DATA,    0x11},

    {DSICMD_CMD,     0xF3},
    {DSICMD_DATA,    0x01},

    {DSICMD_CMD,     0xF0},
    {DSICMD_DATA,    0x55},
    {DSICMD_DATA,    0xAA},
    {DSICMD_DATA,    0x52},
    {DSICMD_DATA,    0x08},
    {DSICMD_DATA,    0x00},

    //LPX begin
    {DSICMD_CMD,     0xB2},
    {DSICMD_DATA,    0x65},
    {DSICMD_DATA,    0x07}, //org 0x02
    //LPX end

    {DSICMD_CMD,     0xC8},
    {DSICMD_DATA,    0x80},

    {DSICMD_CMD,     0xB1},
    {DSICMD_DATA,    0x68},
    {DSICMD_DATA,    0x21},

    {DSICMD_CMD,     0xB6},
    {DSICMD_DATA,    0x0F},

    {DSICMD_CMD,     0x6F},
    {DSICMD_DATA,    0x02},

    {DSICMD_CMD,     0xB8},
    {DSICMD_DATA,    0x08},

    {DSICMD_CMD,     0xBB},
    {DSICMD_DATA,    0x74},
    {DSICMD_DATA,    0x44},

    {DSICMD_CMD,     0xBC},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x00},

    {DSICMD_CMD,     0xC7},
    {DSICMD_DATA,    0x01},

    {DSICMD_CMD,     0xBD},
    {DSICMD_DATA,    0x02},
    {DSICMD_DATA,    0xB0},
    {DSICMD_DATA,    0x10},
    {DSICMD_DATA,    0x10},
    {DSICMD_DATA,    0x00},

    {DSICMD_CMD,     0xF0},
    {DSICMD_DATA,    0x55},
    {DSICMD_DATA,    0xAA},
    {DSICMD_DATA,    0x52},
    {DSICMD_DATA,    0x08},
    {DSICMD_DATA,    0x01},

    {DSICMD_CMD,     0xB0},
    {DSICMD_DATA,    0x05},
    {DSICMD_DATA,    0x05},

    {DSICMD_CMD,     0xB1},
    {DSICMD_DATA,    0x05},
    {DSICMD_DATA,    0x05},

    {DSICMD_CMD,     0xBC},
    {DSICMD_DATA,    0x88},
    {DSICMD_DATA,    0x14},

    {DSICMD_CMD,     0xBD},
    {DSICMD_DATA,    0x88},
    {DSICMD_DATA,    0x14},

    {DSICMD_CMD,     0xCA},
    {DSICMD_DATA,    0x00},

    {DSICMD_CMD,     0xC0},
    {DSICMD_DATA,    0x04},

    {DSICMD_CMD,     0xBE},
    {DSICMD_DATA,    0x29},

    {DSICMD_CMD,     0xB3},
    {DSICMD_DATA,    0x29},
    {DSICMD_DATA,    0x29},

    {DSICMD_CMD,     0xB4},
    {DSICMD_DATA,    0x14},
    {DSICMD_DATA,    0x14},

    {DSICMD_CMD,     0xB9},
    {DSICMD_DATA,    0x36},
    {DSICMD_DATA,    0x36},

    {DSICMD_CMD,     0xBA},
    {DSICMD_DATA,    0x24},
    {DSICMD_DATA,    0x24},

    {DSICMD_CMD,     0xF0},
    {DSICMD_DATA,    0x55},
    {DSICMD_DATA,    0xAA},
    {DSICMD_DATA,    0x52},
    {DSICMD_DATA,    0x08},
    {DSICMD_DATA,    0x02},

    {DSICMD_CMD,     0xEE},
    {DSICMD_DATA,    0x01},

    {DSICMD_CMD,     0xB0},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x09},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x1B},

    {DSICMD_CMD,     0x6F},
    {DSICMD_DATA,    0x06},

    {DSICMD_CMD,     0xB0},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x2E},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x3F},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x5B},

    {DSICMD_CMD,     0x6F},
    {DSICMD_DATA,    0x0C},

    {DSICMD_CMD,     0xB0},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x75},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x9F},

    {DSICMD_CMD,     0xB1},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0xC4},
    {DSICMD_DATA,    0x01},
    {DSICMD_DATA,    0x02},
    {DSICMD_DATA,    0x01},
    {DSICMD_DATA,    0x30},

    {DSICMD_CMD,     0x6F},
    {DSICMD_DATA,    0x06},

    {DSICMD_CMD,     0xB1},
    {DSICMD_DATA,    0x01},
    {DSICMD_DATA,    0x7D},
    {DSICMD_DATA,    0x01},
    {DSICMD_DATA,    0xBD},
    {DSICMD_DATA,    0x01},
    {DSICMD_DATA,    0xBF},

    {DSICMD_CMD,     0x6F},
    {DSICMD_DATA,    0x0C},

    {DSICMD_CMD,     0xB1},
    {DSICMD_DATA,    0x01},
    {DSICMD_DATA,    0xFF},
    {DSICMD_DATA,    0x02},
    {DSICMD_DATA,    0x3F},

    {DSICMD_CMD,     0xB2},
    {DSICMD_DATA,    0x02},
    {DSICMD_DATA,    0x6F},
    {DSICMD_DATA,    0x02},
    {DSICMD_DATA,    0xAB},
    {DSICMD_DATA,    0x02},
    {DSICMD_DATA,    0xD0},

    {DSICMD_CMD,     0x6F},
    {DSICMD_DATA,    0x06},

    {DSICMD_CMD,     0xB2},
    {DSICMD_DATA,    0x03},
    {DSICMD_DATA,    0x07},
    {DSICMD_DATA,    0x03},
    {DSICMD_DATA,    0x2A},
    {DSICMD_DATA,    0x03},
    {DSICMD_DATA,    0x58},

    {DSICMD_CMD,     0x6F},
    {DSICMD_DATA,    0x0C},

    {DSICMD_CMD,     0xB2},
    {DSICMD_DATA,    0x03},
    {DSICMD_DATA,    0x75},
    {DSICMD_DATA,    0x03},
    {DSICMD_DATA,    0x9B},

    {DSICMD_CMD,     0xB3},
    {DSICMD_DATA,    0x03},
    {DSICMD_DATA,    0xE7},
    {DSICMD_DATA,    0x03},
    {DSICMD_DATA,    0xFF},

    {DSICMD_CMD,     0xF0},
    {DSICMD_DATA,    0x55},
    {DSICMD_DATA,    0xAA},
    {DSICMD_DATA,    0x52},
    {DSICMD_DATA,    0x08},
    {DSICMD_DATA,    0x06},

    {DSICMD_CMD,     0xB0},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x10},

    {DSICMD_CMD,     0xB1},
    {DSICMD_DATA,    0x12},
    {DSICMD_DATA,    0x14},

    {DSICMD_CMD,     0xB2},
    {DSICMD_DATA,    0x16},
    {DSICMD_DATA,    0x18},

    {DSICMD_CMD,     0xB3},
    {DSICMD_DATA,    0x1A},
    {DSICMD_DATA,    0x29},

    {DSICMD_CMD,     0xB4},
    {DSICMD_DATA,    0x2A},
    {DSICMD_DATA,    0x08},

    {DSICMD_CMD,     0xB5},
    {DSICMD_DATA,    0x31},
    {DSICMD_DATA,    0x31},

    {DSICMD_CMD,     0xB6},
    {DSICMD_DATA,    0x31},
    {DSICMD_DATA,    0x31},

    {DSICMD_CMD,     0xB7},
    {DSICMD_DATA,    0x31},
    {DSICMD_DATA,    0x31},

    {DSICMD_CMD,     0xB8},
    {DSICMD_DATA,    0x31},
    {DSICMD_DATA,    0x0A},

    {DSICMD_CMD,     0xB9},
    {DSICMD_DATA,    0x31},
    {DSICMD_DATA,    0x31},

    {DSICMD_CMD,     0xBA},
    {DSICMD_DATA,    0x31},
    {DSICMD_DATA,    0x31},

    {DSICMD_CMD,     0xBB},
    {DSICMD_DATA,    0x0B},
    {DSICMD_DATA,    0x31},

    {DSICMD_CMD,     0xBC},
    {DSICMD_DATA,    0x31},
    {DSICMD_DATA,    0x31},

    {DSICMD_CMD,     0xBD},
    {DSICMD_DATA,    0x31},
    {DSICMD_DATA,    0x31},

    {DSICMD_CMD,     0xBE},
    {DSICMD_DATA,    0x31},
    {DSICMD_DATA,    0x31},

    {DSICMD_CMD,     0xBF},
    {DSICMD_DATA,    0x09},
    {DSICMD_DATA,    0x2A},

    {DSICMD_CMD,     0xC0},
    {DSICMD_DATA,    0x29},
    {DSICMD_DATA,    0x1B},

    {DSICMD_CMD,     0xC1},
    {DSICMD_DATA,    0x19},
    {DSICMD_DATA,    0x17},

    {DSICMD_CMD,     0xC2},
    {DSICMD_DATA,    0x15},
    {DSICMD_DATA,    0x13},

    {DSICMD_CMD,     0xC3},
    {DSICMD_DATA,    0x11},
    {DSICMD_DATA,    0x01},

    {DSICMD_CMD,     0xE5},
    {DSICMD_DATA,    0x31},
    {DSICMD_DATA,    0x31},

    {DSICMD_CMD,     0xC4},
    {DSICMD_DATA,    0x09},
    {DSICMD_DATA,    0x1B},

    {DSICMD_CMD,     0xC5},
    {DSICMD_DATA,    0x19},
    {DSICMD_DATA,    0x17},

    {DSICMD_CMD,     0xC6},
    {DSICMD_DATA,    0x15},
    {DSICMD_DATA,    0x13},

    {DSICMD_CMD,     0xC7},
    {DSICMD_DATA,    0x11},
    {DSICMD_DATA,    0x29},

    {DSICMD_CMD,     0xC8},
    {DSICMD_DATA,    0x2A},
    {DSICMD_DATA,    0x01},

    {DSICMD_CMD,     0xC9},
    {DSICMD_DATA,    0x31},
    {DSICMD_DATA,    0x31},

    {DSICMD_CMD,     0xCA},
    {DSICMD_DATA,    0x31},
    {DSICMD_DATA,    0x31},

    {DSICMD_CMD,     0xCB},
    {DSICMD_DATA,    0x31},
    {DSICMD_DATA,    0x31},

    {DSICMD_CMD,     0xCC},
    {DSICMD_DATA,    0x31},
    {DSICMD_DATA,    0x0B},

    {DSICMD_CMD,     0xCD},
    {DSICMD_DATA,    0x31},
    {DSICMD_DATA,    0x31},

    {DSICMD_CMD,     0xCE},
    {DSICMD_DATA,    0x31},
    {DSICMD_DATA,    0x31},

    {DSICMD_CMD,     0xCF},
    {DSICMD_DATA,    0x0A},
    {DSICMD_DATA,    0x31},

    {DSICMD_CMD,     0xD0},
    {DSICMD_DATA,    0x31},
    {DSICMD_DATA,    0x31},

    {DSICMD_CMD,     0xD1},
    {DSICMD_DATA,    0x31},
    {DSICMD_DATA,    0x31},

    {DSICMD_CMD,     0xD2},
    {DSICMD_DATA,    0x31},
    {DSICMD_DATA,    0x31},

    {DSICMD_CMD,     0xD3},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x2A},

    {DSICMD_CMD,     0xD4},
    {DSICMD_DATA,    0x29},
    {DSICMD_DATA,    0x10},

    {DSICMD_CMD,     0xD5},
    {DSICMD_DATA,    0x12},
    {DSICMD_DATA,    0x14},

    {DSICMD_CMD,     0xD6},
    {DSICMD_DATA,    0x16},
    {DSICMD_DATA,    0x18},

    {DSICMD_CMD,     0xD7},
    {DSICMD_DATA,    0x1A},
    {DSICMD_DATA,    0x08},

    {DSICMD_CMD,     0xE6},
    {DSICMD_DATA,    0x31},
    {DSICMD_DATA,    0x31},

    {DSICMD_CMD,     0xD8},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x54},
    {DSICMD_DATA,    0x00},

    {DSICMD_CMD,     0xD9},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x15},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x00},

    {DSICMD_CMD,     0xE7},
    {DSICMD_DATA,    0x00},

    {DSICMD_CMD,     0xF0},
    {DSICMD_DATA,    0x55},
    {DSICMD_DATA,    0xAA},
    {DSICMD_DATA,    0x52},
    {DSICMD_DATA,    0x08},
    {DSICMD_DATA,    0x03},

    {DSICMD_CMD,     0xB0},
    {DSICMD_DATA,    0x20},
    {DSICMD_DATA,    0x00},

    {DSICMD_CMD,     0xB1},
    {DSICMD_DATA,    0x20},
    {DSICMD_DATA,    0x00},

    {DSICMD_CMD,     0xB2},
    {DSICMD_DATA,    0x05},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x00},

    {DSICMD_CMD,     0xB6},
    {DSICMD_DATA,    0x05},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x00},

    {DSICMD_CMD,     0xB7},
    {DSICMD_DATA,    0x05},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x00},

    {DSICMD_CMD,     0xBA},
    {DSICMD_DATA,    0x57},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x00},

    {DSICMD_CMD,     0xBB},
    {DSICMD_DATA,    0x57},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x00},

    {DSICMD_CMD,     0xC0},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x00},

    {DSICMD_CMD,     0xC1},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x00},

    {DSICMD_CMD,     0xC4},
    {DSICMD_DATA,    0x60},

    {DSICMD_CMD,     0xC5},
    {DSICMD_DATA,    0x40},

    {DSICMD_CMD,     0xF0},
    {DSICMD_DATA,    0x55},
    {DSICMD_DATA,    0xAA},
    {DSICMD_DATA,    0x52},
    {DSICMD_DATA,    0x08},
    {DSICMD_DATA,    0x05},

    {DSICMD_CMD,     0xBD},
    {DSICMD_DATA,    0x03},
    {DSICMD_DATA,    0x01},
    {DSICMD_DATA,    0x03},
    {DSICMD_DATA,    0x03},
    {DSICMD_DATA,    0x03},

    {DSICMD_CMD,     0xB0},
    {DSICMD_DATA,    0x17},
    {DSICMD_DATA,    0x06},

    {DSICMD_CMD,     0xB1},
    {DSICMD_DATA,    0x17},
    {DSICMD_DATA,    0x06},

    {DSICMD_CMD,     0xB2},
    {DSICMD_DATA,    0x17},
    {DSICMD_DATA,    0x06},

    {DSICMD_CMD,     0xB3},
    {DSICMD_DATA,    0x17},
    {DSICMD_DATA,    0x06},

    {DSICMD_CMD,     0xB4},
    {DSICMD_DATA,    0x17},
    {DSICMD_DATA,    0x06},

    {DSICMD_CMD,     0xB5},
    {DSICMD_DATA,    0x17},
    {DSICMD_DATA,    0x06},

    {DSICMD_CMD,     0xB8},
    {DSICMD_DATA,    0x00},

    {DSICMD_CMD,     0xB9},
    {DSICMD_DATA,    0x00},

    {DSICMD_CMD,     0xBA},
    {DSICMD_DATA,    0x00},

    {DSICMD_CMD,     0xBB},
    {DSICMD_DATA,    0x02},

    {DSICMD_CMD,     0xBC},
    {DSICMD_DATA,    0x00},

    {DSICMD_CMD,     0xC0},
    {DSICMD_DATA,    0x07},

    {DSICMD_CMD,     0xC4},
    {DSICMD_DATA,    0x81},

    {DSICMD_CMD,     0xC5},
    {DSICMD_DATA,    0xA3},

    {DSICMD_CMD,     0xC8},
    {DSICMD_DATA,    0x05},
    {DSICMD_DATA,    0x30},

    {DSICMD_CMD,     0xC9},
    {DSICMD_DATA,    0x01},
    {DSICMD_DATA,    0x31},

    {DSICMD_CMD,     0xCC},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x3C},

    {DSICMD_CMD,     0xCD},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x3C},

    {DSICMD_CMD,     0xD1},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x05},
    {DSICMD_DATA,    0x09},
    {DSICMD_DATA,    0x07},
    {DSICMD_DATA,    0x10},

    {DSICMD_CMD,     0xD2},
    {DSICMD_DATA,    0x00},
    {DSICMD_DATA,    0x05},
    {DSICMD_DATA,    0x0E},
    {DSICMD_DATA,    0x07},
    {DSICMD_DATA,    0x10},

    {DSICMD_CMD,     0xE5},
    {DSICMD_DATA,    0x06},

    {DSICMD_CMD,     0xE6},
    {DSICMD_DATA,    0x06},

    {DSICMD_CMD,     0xE7},
    {DSICMD_DATA,    0x06},

    {DSICMD_CMD,     0xE8},
    {DSICMD_DATA,    0x06},

    {DSICMD_CMD,     0xE9},
    {DSICMD_DATA,    0x06},

    {DSICMD_CMD,     0xEA},
    {DSICMD_DATA,    0x06},

    {DSICMD_CMD,     0xED},
    {DSICMD_DATA,    0x30},

    {DSICMD_CMD,     0x6F},
    {DSICMD_DATA,    0x11},

    {DSICMD_CMD,     0xF3},
    {DSICMD_DATA,    0x01},

//BIST EN
/*
    {DSICMD_CMD,     0xF0},
    {DSICMD_DATA,    0x55},
    {DSICMD_DATA,    0xAA},
    {DSICMD_DATA,    0x52},
    {DSICMD_DATA,    0x08},
    {DSICMD_DATA,    0x00},

    {DSICMD_CMD,     0xEF},
    {DSICMD_DATA,    0x07},
    {DSICMD_DATA,    0xFF},

    {DSICMD_CMD,     0xEE},
    {DSICMD_DATA,    0x87},
    {DSICMD_DATA,    0x78},
    {DSICMD_DATA,    0x02},
    {DSICMD_DATA,    0x40},
*/
//BIST EN end

    // Sleep out
    {DSICMD_CMD,     0x11},
    {CMDDELAY_MS,    10},

    // Display on
    {DSICMD_CMD,     0x29},
    {CMDDELAY_MS,    10},
};


const tPANEL_CMD tCmdStandbyDSI[] =
{
    {DSICMD_CMD,     0x28},         // Display OFF
    {CMDDELAY_MS,    10},
    //{DSICMD_CMD,     0x10},      // Sleep in
    //{CMDDELAY_MS,    10},
};

const tLCD_PARAM tModeDSI[] =
{
    /***********       MI Serial Format 1      *************/
    {
        // tPANEL_PARAM
        {
            /* Old prototype */
            //PINMUX_DSI_4_LANE_CMD_MODE_RGB565,    //!< LCDMode
            //PINMUX_DSI_4_LANE_VDO_SYNC_EVENT_RGB565,//!< LCDMode
            //PINMUX_DSI_4_LANE_CMD_MODE_RGB565,
            //PINMUX_DSI_4_LANE_CMD_MODE_RGB666L,
#if (DSI_OP_MODE == DSI_OP_MODE_CMD_MODE)
#if(DSI_PACKET_FORMAT == DSI_FORMAT_RGB888)
            PINMUX_DSI_4_LANE_CMD_MODE_RGB888,
#elif(DSI_PACKET_FORMAT == DSI_FORMAT_RGB666L)
            PINMUX_DSI_4_LANE_CMD_MODE_RGB666L,
#elif(DSI_PACKET_FORMAT == DSI_FORMAT_RGB565)
            PINMUX_DSI_4_LANE_CMD_MODE_RGB565,
#endif
#else
 #if(DSI_PACKET_FORMAT == DSI_FORMAT_RGB888)
            PINMUX_DSI_4_LANE_VDO_SYNC_PULSE_RGB888,
            //PINMUX_DSI_4_LANE_VDO_SYNC_EVENT_RGB888,
#elif(DSI_PACKET_FORMAT == DSI_FORMAT_RGB666L)
            PINMUX_DSI_4_LANE_VDO_SYNC_PULSE_RGB666L,
#elif(DSI_PACKET_FORMAT == DSI_FORMAT_RGB666P)
            PINMUX_DSI_4_LANE_VDO_SYNC_PULSE_RGB666P,
#elif(DSI_PACKET_FORMAT == DSI_FORMAT_RGB565)
            ///PINMUX_DSI_4_LANE_VDO_SYNC_PULSE_RGB565,
            PINMUX_DSI_4_LANE_VDO_SYNC_EVENT_RGB565,
#endif
#endif
            //PINMUX_DSI_4_LANE_VDO_SYNC_PULSE_RGB888,
            //PINMUX_DSI_4_LANE_VDO_SYNC_EVENT_RGB888, //OK
            //PINMUX_DSI_4_LANE_VDO_SYNC_PULSE_RGB565,
            //PINMUX_DSI_4_LANE_VDO_SYNC_EVENT_RGB565, //OK
            //PINMUX_DSI_4_LANE_VDO_SYNC_EVENT_RGB666L,
            //PINMUX_DSI_4_LANE_VDO_SYNC_PULSE_RGB666L,
            //PINMUX_DSI_4_LANE_VDO_SYNC_EVENT_RGB666P,
            //PINMUX_DSI_4_LANE_VDO_SYNC_PULSE_RGB666P,
            _IDE_fDCLK,                             //!< fDCLK
            (42+PANEL_WIDTH),                     //!< uiHSyncTotalPeriod
            PANEL_WIDTH,                            //!< uiHSyncActivePeriod
            21,                                   //!< uiHSyncBackPorch
            0x20+PANEL_HEIGHT,                      //!< uiVSyncTotalPeriod
            PANEL_HEIGHT,                           //!< uiVSyncActivePeriod
            0x10,                                   //!< uiVSyncBackPorchOdd
            0x10,                                   //!< uiVSyncBackPorchEven
            PANEL_WIDTH,                            //!< uiBufferWidth
            PANEL_HEIGHT,                           //!< uiBufferHeight
            PANEL_WIDTH,                            //!< uiWindowWidth
            PANEL_HEIGHT,                           //!< uiWindowHeight
            FALSE,                                  //!< bYCbCrFormat

            /* New added parameters */
            0x01,                                   //!< uiHSyncSYNCwidth
            0x01                                    //!< uiVSyncSYNCwidth
        },

        // tIDE_PARAM
        {
            /* Old prototype */
            PINMUX_LCD_SEL_GPIO,            //!< pinmux_select_lcd;
            ICST_CCIR601,                   //!< icst;
            {FALSE, FALSE},                  //!< dithering[2];
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
#if 0
    // DSI input source clock = 480
    // Target can be 480 / 240 / 160 / 120
    FLOAT   DsiTargetClk = DSI_TARGET_CLK;
    UINT32  Div;


    Div = (UINT32) (pDsiConfig->fDsiSrcClk/DsiTargetClk);

    if(Div == 0)
    {
        NT35510_WRN_MSG("Div = 0 force ++\r\n");
        Div++;
    }
    pll_setClockRate(PLL_CLKSEL_DSI_CLKDIV, PLL_DSI_CLKDIV(Div-1));
#else
    dsi_setConfig(DSI_CONFIG_ID_FREQ, DSI_TARGET_CLK * 1000000);
#endif
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
/*
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
*/
    dsi_setConfig(DSI_CONFIG_ID_TLPX, 3);
    dsi_setConfig(DSI_CONFIG_ID_BTA_TA_GO, 4);
    dsi_setConfig(DSI_CONFIG_ID_THS_PREPARE, 4);//4
    dsi_setConfig(DSI_CONFIG_ID_THS_ZERO, 6); /*8*/
    dsi_setConfig(DSI_CONFIG_ID_THS_TRAIL, 7);
    dsi_setConfig(DSI_CONFIG_ID_THS_EXIT, 6);

    dsi_setConfig(DSI_CONFIG_ID_TCLK_PREPARE, 3);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_ZERO, 16);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_POST, 16);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_PRE, 2); /*7*/
    dsi_setConfig(DSI_CONFIG_ID_TCLK_TRAIL, 3);
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
#elif(DSI_TARGET_CLK == 54)
    dsi_setConfig(DSI_CONFIG_ID_TLPX, 1);
    dsi_setConfig(DSI_CONFIG_ID_BTA_TA_GO, 4);

    dsi_setConfig(DSI_CONFIG_ID_THS_PREPARE, 1);
    dsi_setConfig(DSI_CONFIG_ID_THS_ZERO, 0);
    dsi_setConfig(DSI_CONFIG_ID_THS_TRAIL, 1);
    dsi_setConfig(DSI_CONFIG_ID_THS_EXIT, 4);

    dsi_setConfig(DSI_CONFIG_ID_TCLK_PREPARE, 0);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_ZERO, 2);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_POST, 0);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_PRE, 1);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_TRAIL, 1);
#endif
    dsi_setConfig(DSI_CONFIG_ID_DATALANE_NO, DSI_DATA_LANE_3);
    dsi_setConfig(DSI_CONFIG_ID_TE_BTA_INTERVAL, 0x1F);
    //dsi_setConfig(DSI_CONFIG_ID_CLK_PHASE_OFS, 0x3);
    //dsi_setConfig(DSI_CONFIG_ID_PHASE_DELAY_ENABLE_OFS, 0x1);

    dsi_setConfig(DSI_CONFIG_ID_CLK_LP_CTRL, 0x0);
    dsi_setConfig(DSI_CONFIG_ID_SYNC_DLY_CNT, 0xF);

    dsi_setConfig(DSI_CONFIG_ID_EOT_PKT_EN, TRUE);

    // test lane swap 1230
    //dsi_setConfig(DSI_CONFIG_ID_LANSEL_D0, 1);
    //dsi_setConfig(DSI_CONFIG_ID_LANSEL_D1, 2);
    //dsi_setConfig(DSI_CONFIG_ID_LANSEL_D2, 3);
    //dsi_setConfig(DSI_CONFIG_ID_LANSEL_D3, 0);

    // test lane swap 2301
    //dsi_setConfig(DSI_CONFIG_ID_LANSEL_D0, 2);
    //dsi_setConfig(DSI_CONFIG_ID_LANSEL_D1, 3);
    //dsi_setConfig(DSI_CONFIG_ID_LANSEL_D2, 0);
    //dsi_setConfig(DSI_CONFIG_ID_LANSEL_D3, 1);

    // test lane swap 3012
    //dsi_setConfig(DSI_CONFIG_ID_LANSEL_D0, 3);
    //dsi_setConfig(DSI_CONFIG_ID_LANSEL_D1, 0);
    //dsi_setConfig(DSI_CONFIG_ID_LANSEL_D2, 1);
    //dsi_setConfig(DSI_CONFIG_ID_LANSEL_D3, 2);

}

#endif
