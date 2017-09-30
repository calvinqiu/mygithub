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

#define PANEL_WIDTH     480
#define PANEL_HEIGHT    800

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



#define DSI_TARGET_CLK             320

#if(DSI_PACKET_FORMAT == DSI_FORMAT_RGB888)
#define _IDE_fDCLK      32.00f// must > ((((DSI_TARGET_CLK / 8) * 2) / 3)) ->2: lane number
#else
#error "Format not RGB888"
#endif

#define HVALIDST    80//0x39  //uiHSyncBackPorch(HPB) -> 53
#define VVALIDST    20//0x15  //uiVSyncBackPorchOdd/Even
#define HSYNCT      20//0x02
#define VSYNCT      10///0x02

#define ROTATE_180DEGREE  DISABLE//ENABLE //


/*
    Panel Parameters for TCON HX8369B
*/
//@{
/*Used in DSI*/
const tPANEL_CMD tCmdModeDSI[] =
{
	{CMDDELAY_MS,300},

	{DSICMD_CMD,0x11},

	{CMDDELAY_MS,300},

	//---------------------------------------Bank0 Setting-------------------------------------------------//
	//------------------------------------Display Control setting----------------------------------------------//
	{DSICMD_CMD, 0xFF},
	{DSICMD_DATA,0x77},
	{DSICMD_DATA,0x01},
	{DSICMD_DATA,0x00},
	{DSICMD_DATA,0x00},
	{DSICMD_DATA,0x10},

	{DSICMD_CMD, 0xC0},
	{DSICMD_DATA,0x63},
	{DSICMD_DATA,0x00},

	{DSICMD_CMD, 0xC1},
	{DSICMD_DATA,0x16},
	{DSICMD_DATA,0x02},

	{DSICMD_CMD, 0xC2},
	{DSICMD_DATA,0x37},
	{DSICMD_DATA,0x08},

	{DSICMD_CMD, 0xCC},
	{DSICMD_DATA,0x10},
	//-------------------------------------Gamma Cluster Setting-------------------------------------------//
	{DSICMD_CMD, 0xB0},
	{DSICMD_DATA,0x00},
	{DSICMD_DATA,0x16},
	{DSICMD_DATA,0x1E},
	{DSICMD_DATA,0x0E},
	{DSICMD_DATA,0x11},
	{DSICMD_DATA,0x06},
	{DSICMD_DATA,0x10},
	{DSICMD_DATA,0x09},
	{DSICMD_DATA,0x09},
	{DSICMD_DATA,0x2B},
	{DSICMD_DATA,0x06},
	{DSICMD_DATA,0x13},
	{DSICMD_DATA,0x10},
	{DSICMD_DATA,0x2A},
	{DSICMD_DATA,0x31},
	{DSICMD_DATA,0x1F},
	{DSICMD_CMD,0xB1},
	{DSICMD_DATA,0x00},
	{DSICMD_DATA,0x16},
	{DSICMD_DATA,0x1E},
	{DSICMD_DATA,0x0D},
	{DSICMD_DATA,0x11},
	{DSICMD_DATA,0x06},
	{DSICMD_DATA,0x10},
	{DSICMD_DATA,0x07},
	{DSICMD_DATA,0x08},
	{DSICMD_DATA,0x2C},
	{DSICMD_DATA,0x07},
	{DSICMD_DATA,0x15},
	{DSICMD_DATA,0x14},
	{DSICMD_DATA,0x2B},
	{DSICMD_DATA,0x30},
	{DSICMD_DATA,0x1F},
	//---------------------------------------End Gamma Setting----------------------------------------------//
	//------------------------------------End Display Control setting----------------------------------------//
	//-----------------------------------------Bank0 Setting End---------------------------------------------//
	//-------------------------------------------Bank1 Setting---------------------------------------------------//

	//-------------------------------- Power Control Registers Initial --------------------------------------//
	{DSICMD_CMD, 0xFF},
	{DSICMD_DATA,0x77},
	{DSICMD_DATA,0x01},
	{DSICMD_DATA,0x00},
	{DSICMD_DATA,0x00},
	{DSICMD_DATA,0x11},
	{DSICMD_CMD,0xB0},
	{DSICMD_DATA,0x3D},
	//-------------------------------------------Vcom Setting---------------------------------------------------//
	{DSICMD_CMD, 0xB1},
	{DSICMD_DATA,0x79},
	//-----------------------------------------End Vcom Setting-----------------------------------------------//

	{DSICMD_CMD, 0xD2},
	{DSICMD_DATA,0x05},

	{DSICMD_CMD, 0xB2},
	{DSICMD_DATA,0x07},
	{DSICMD_CMD,0xB3},
	{DSICMD_DATA,0x80},
	{DSICMD_CMD,0xB5},
	{DSICMD_DATA,0x47},
	{DSICMD_CMD,0xB7},
	{DSICMD_DATA,0x85},
	{DSICMD_CMD,0xB8},
	{DSICMD_DATA,0x20},
	{DSICMD_CMD,0xB9},
	{DSICMD_DATA,0x10},
	{DSICMD_CMD,0xC1},
	{DSICMD_DATA,0x78},

	{DSICMD_CMD,0xC2},
	{DSICMD_DATA,0x78},

	{DSICMD_CMD,0xD0},
	{DSICMD_DATA,0x88},
	{CMDDELAY_MS,100},
	//---------------------------------------------GIP Setting----------------------------------------------------//
	{DSICMD_CMD,0xE0},
	{DSICMD_DATA,0x00},
	{DSICMD_DATA,0x00},
	{DSICMD_DATA,0x02},
	{DSICMD_CMD,0xE1},
	{DSICMD_DATA,0x08},
	{DSICMD_DATA,0x00},
	{DSICMD_DATA,0x0A},
	{DSICMD_DATA,0x00},
	{DSICMD_DATA,0x09},
	{DSICMD_DATA,0x00},
	{DSICMD_DATA,0x0B},
	{DSICMD_DATA,0x00},
	{DSICMD_DATA,0x00},
	{DSICMD_DATA,0x44},
	{DSICMD_DATA,0x44},
	{DSICMD_CMD,0xE2},
	{DSICMD_DATA,0x00},
	{DSICMD_DATA,0x00},
	{DSICMD_DATA,0x00},
	{DSICMD_DATA,0x00},
	{DSICMD_DATA,0x00},
	{DSICMD_DATA,0x00},
	{DSICMD_DATA,0x00},
	{DSICMD_DATA,0x00},
	{DSICMD_DATA,0x00},
	{DSICMD_DATA,0x00},
	{DSICMD_DATA,0x00},
	{DSICMD_DATA,0x00},
	{DSICMD_DATA,0x00},
	{DSICMD_CMD,0xE3},
	{DSICMD_DATA,0x00},
	{DSICMD_DATA,0x00},
	{DSICMD_DATA,0x33},
	{DSICMD_DATA,0x33},
	{DSICMD_CMD,0xE4},
	{DSICMD_DATA,0x22},
	{DSICMD_DATA,0x22},
	{DSICMD_CMD,0xE5},
	{DSICMD_DATA,0x11},
	{DSICMD_DATA,0x40},
	{DSICMD_DATA,0xA0},
	{DSICMD_DATA,0xA0},
	{DSICMD_DATA,0x0D},
	{DSICMD_DATA,0x40},
	{DSICMD_DATA,0xA0},
	{DSICMD_DATA,0xA0},
	{DSICMD_DATA,0x13},
	{DSICMD_DATA,0x40},
	{DSICMD_DATA,0xA0},
	{DSICMD_DATA,0xA0},
	{DSICMD_DATA,0x0F},
	{DSICMD_DATA,0x40},
	{DSICMD_DATA,0xA0},
	{DSICMD_DATA,0xA0},
	{DSICMD_CMD,0xE6},
	{DSICMD_DATA,0x00},
	{DSICMD_DATA,0x00},
	{DSICMD_DATA,0x33},
	{DSICMD_DATA,0x33},
	{DSICMD_CMD,0xE7},
	{DSICMD_DATA,0x22},
	{DSICMD_DATA,0x22},
	{DSICMD_CMD,0xE8},
	{DSICMD_DATA,0x10},
	{DSICMD_DATA,0x40},
	{DSICMD_DATA,0xA0},
	{DSICMD_DATA,0xA0},
	{DSICMD_DATA,0x0C},
	{DSICMD_DATA,0x40},
	{DSICMD_DATA,0xA0},
	{DSICMD_DATA,0xA0},
	{DSICMD_DATA,0x12},
	{DSICMD_DATA,0x40},
	{DSICMD_DATA,0xA0},
	{DSICMD_DATA,0xA0},
	{DSICMD_DATA,0x0E},
	{DSICMD_DATA,0x40},
	{DSICMD_DATA,0xA0},
	{DSICMD_DATA,0xA0},
	{DSICMD_CMD,0xEB},
	{DSICMD_DATA,0x02},
	{DSICMD_DATA,0x00},
	{DSICMD_DATA,0xE4},
	{DSICMD_DATA,0xE4},
	{DSICMD_DATA,0x11},
	{DSICMD_DATA,0x00},
	{DSICMD_DATA,0x00},
	{DSICMD_CMD,0xEC},
	{DSICMD_DATA,0x00},
	{DSICMD_DATA,0x00},
	{DSICMD_CMD,0xED},
	{DSICMD_DATA,0xAB},
	{DSICMD_DATA,0xFF},
	{DSICMD_DATA,0x75},
	{DSICMD_DATA,0x64},
	{DSICMD_DATA,0x01},
	{DSICMD_DATA,0xFF},
	{DSICMD_DATA,0xFF},
	{DSICMD_DATA,0xFF},
	{DSICMD_DATA,0xFF},
	{DSICMD_DATA,0xFF},
	{DSICMD_DATA,0xFF},
	{DSICMD_DATA,0x10},
	{DSICMD_DATA,0x46},
	{DSICMD_DATA,0x57},
	{DSICMD_DATA,0xFF},
	{DSICMD_DATA,0xBA},
	//--------------------------------------------End GIP Setting-----------------------------------------------//
	//------------------------------ Power Control Registers Initial End-----------------------------------//
	//------------------------------------------Bank1 Setting----------------------------------------------------//
	{DSICMD_CMD,0xFF},
	{DSICMD_DATA,0x77},
	{DSICMD_DATA,0x01},
	{DSICMD_DATA,0x00},
	{DSICMD_DATA,0x00},
	{DSICMD_DATA,0x00},
	{DSICMD_CMD,0x29},
       
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
            (0xC0+PANEL_WIDTH),             //!< uiHSyncTotalPeriod(HTOTAL)
            PANEL_WIDTH,                    //!< uiHSyncActivePeriod(HACT)
            80,//0x60,                //!< uiHSyncBackPorch(HBP)
            0x52 + PANEL_HEIGHT,            //!< uiVSyncTotalPeriod(VTOTAL)
            PANEL_HEIGHT,                   //!< uiVSyncActivePeriod
            20,                       //!< uiVSyncBackPorchOdd
            20,                       //!< uiVSyncBackPorchEven
            PANEL_HEIGHT,                   //!< uiBufferWidth
            PANEL_WIDTH,                    //!< uiBufferHeight
            PANEL_WIDTH,                    //!< uiWindowWidth
            PANEL_HEIGHT,                   //!< uiWindowHeight
            FALSE,                           //!< bYCbCrFormat
            /* New added parameters */
            20,                         //!< uiHSyncSYNCwidth
            10                          //!< uiVSyncSYNCwidth
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

/*#if(DSI_TARGET_CLK == 480)
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
*/
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
/*#endif*/

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
