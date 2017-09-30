#if 1
// john modify  for use  8018B driveric  to driver  5inch MIPI lcd

/*
    Display object for driving DSI device

    @file       hx5274a.c
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



#define PANEL_WIDTH     720
#define PANEL_HEIGHT    1280    //864//   800



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

#define DSI_PACKET_FORMAT      DSI_FORMAT_RGB888 //  DSI_FORMAT_RGB888  //  DSI_FORMAT_RGB888



#define DSI_TARGET_CLK       480 //  480    //john modify  20150724


#if(DSI_PACKET_FORMAT == DSI_FORMAT_RGB888)
#define _IDE_fDCLK       80.00f  //    ((((DSI_TARGET_CLK / 8) * 2)/3))
#elif(DSI_PACKET_FORMAT == DSI_FORMAT_RGB565)
#define _IDE_fDCLK      ((((DSI_TARGET_CLK / 8) * 2)/2))
#else
#error "Format not RGB888"
#endif

#define HVALIDST    0x39  //uiHSyncBackPorch(HPB) -> 53  //0x39
#define VVALIDST    0x15 //uiVSyncBackPorchOdd/Even  //0x15
#define HSYNCT      0x02
#define VSYNCT      0x02


/*
    Panel Parameters for TCON HX8369B
*/
//@{
/*Used in DSI*/
const tPANEL_CMD tCmdModeDSI[] =
{
        {DSICMD_CMD,0x11},
        {CMDDELAY_MS,10},

        {DSICMD_CMD,0xB0},                 //
        {DSICMD_DATA,0x00},

        {DSICMD_CMD,0xBA},                 //
        {DSICMD_DATA,0x05},  // 05

        {DSICMD_CMD,0xb8},                 //
        {DSICMD_DATA,0x00},  // Self-protection function enable

           {DSICMD_CMD,0xC1},                 //
             {DSICMD_DATA,0xe4},  //0x14

        {DSICMD_CMD,0xC9},                 //
             {DSICMD_DATA,0x14},  //0x14

        {DSICMD_CMD,0xCF},                 //
        {DSICMD_DATA,0x00},

        {DSICMD_CMD,0xD3},                 //
        {DSICMD_DATA,0x01},

        {DSICMD_CMD,0xBA},                 //
        {DSICMD_DATA,0x05},  // 05

        {DSICMD_CMD,0xCF},                 //
        {DSICMD_DATA,0x00},
        {CMDDELAY_MS,10},

        // 0XBX
        {DSICMD_CMD,0xB0},                 //
        {DSICMD_DATA,0x01},
        {DSICMD_CMD,0xB2},                 //
        {DSICMD_DATA,0xD4},
        {DSICMD_CMD,0xB5},                 //
        {DSICMD_DATA,0xB4},
        {DSICMD_CMD,0xB7},                 //
        {DSICMD_DATA,0x40},
        {DSICMD_CMD,0xB8},                 //
        {DSICMD_DATA,0x52},
        {DSICMD_CMD,0xB9},                 //
        {DSICMD_DATA,0x20},
        {DSICMD_CMD,0xBA},                 //
        {DSICMD_DATA,0x40},
        //0XCX
        {DSICMD_CMD,0xC8},                 //
        {DSICMD_DATA,0x17},
        {DSICMD_CMD,0xC9},                 //
        {DSICMD_DATA,0x15},
        {DSICMD_CMD,0xCA},                 //
        {DSICMD_DATA,0x15},
        {DSICMD_CMD,0xCB},                 //
        {DSICMD_DATA,0x14},
        //0XDX
        {DSICMD_CMD,0xD4},                 //
        {DSICMD_DATA,0x3C},
        {DSICMD_CMD,0xD5},                 //
        {DSICMD_DATA,0x57},
        {DSICMD_CMD,0xD6},                 //
        {DSICMD_DATA,0x3C},
        {DSICMD_CMD,0xD7},                 //
        {DSICMD_DATA,0x57},
        //0XEX
        {DSICMD_CMD,0xE0},                 //
        {DSICMD_DATA,0x14},
        {DSICMD_CMD,0xE1},                 //
        {DSICMD_DATA,0x15},
        {DSICMD_CMD,0xE2},                 //
        {DSICMD_DATA,0x15},
        {DSICMD_CMD,0xE3},                 //
        {DSICMD_DATA,0x14},
        {DSICMD_CMD,0xE8},                 //
        {DSICMD_DATA,0x43},
        {DSICMD_CMD,0xE9},                 //
        {DSICMD_DATA,0x50},
        {DSICMD_CMD,0xEA},                 //
        {DSICMD_DATA,0x43},
        {DSICMD_CMD,0xEB},                 //
        {DSICMD_DATA,0x50},
        {DSICMD_CMD,0xEC},                 //
        {DSICMD_DATA,0x3C},
        {DSICMD_CMD,0xED},                 //
        {DSICMD_DATA,0x57},
        {DSICMD_CMD,0xEE},                 //
        {DSICMD_DATA,0x3C},
        {DSICMD_CMD,0xEF},                 //
        {DSICMD_DATA,0x57},
        {CMDDELAY_MS,10},
//  john add for  adjust  HUE, BRIGHT
#if 1
        {DSICMD_CMD,0xB0},                 //
        {DSICMD_DATA,0x07},
        {DSICMD_CMD,0xB1},                 //
        {DSICMD_DATA,0x03}, //D7~D0 : D7 X,D6 X  D5~D0  SKIN_TONE_EN,reg_ce_all_en,reg_hue_en,reg_sat_en,reg_bc_en,reg_sharp_en

    //  B2~B6 :  Sharpness  setting
        {DSICMD_CMD,0xB2},             // Sharpness delta v threshold 2
        {DSICMD_DATA,0x10},          //
        {DSICMD_CMD,0xB3},                 // Sharpness delta v threshold 3
        {DSICMD_DATA,0x6d},                //
        {DSICMD_CMD,0xB4},                 //Sharpness delta v threshold 4
        {DSICMD_DATA,0x10},              //
        {DSICMD_CMD,0xB5},  //Sharpness gain1
        {DSICMD_DATA,0x12}, //
        {DSICMD_CMD,0xB6},  //Sharpness gain2
        {DSICMD_DATA,0x01}, //


        {DSICMD_CMD,0xB7},  //Set saturation area
        {DSICMD_DATA,0xe0}, //

        {DSICMD_CMD,0xB9},                 // r gain
        {DSICMD_DATA,0x06}, //  6
        {DSICMD_CMD,0xBA},   //Color ry gain
        {DSICMD_DATA,0xd}, // D
        {DSICMD_CMD,0xBB}, //Color y gain
        {DSICMD_DATA,0x14}, //
        {DSICMD_CMD,0xBC}, //Color yg gain
        {DSICMD_DATA,0x14}, //
        {DSICMD_CMD,0xBD}, //Color g gain
        {DSICMD_DATA,0x14}, //
        {DSICMD_CMD,0xBE}, //Color gc gain
        {DSICMD_DATA,0x14}, //
        {DSICMD_CMD,0xBF}, //Color c gain
        {DSICMD_DATA,0x17},

        {DSICMD_CMD,0xC0}, //Color cb gain
        {DSICMD_DATA,0x17}, //
        {DSICMD_CMD,0xC1}, //Color b gain
        {DSICMD_DATA,0x19}, //
        {DSICMD_CMD,0xC2}, //Color bm gain
        {DSICMD_DATA,0x1C}, //
           {DSICMD_CMD,0xC3}, //Color m gain
        {DSICMD_DATA,0x1C}, //

        {DSICMD_CMD,0xC4}, //Color mr gain
        {DSICMD_DATA,0x17}, //

        {DSICMD_CMD,0xC5}, //Brightness function point 0 gain
        {DSICMD_DATA,0x4}, // 0
        {DSICMD_CMD,0xC6}, //Brightness function point 64 gain
        {DSICMD_DATA,0x0f}, // 8
        {DSICMD_CMD,0xC7},//Brightness function point 128 gain
        {DSICMD_DATA,0x06}, // 3
        {DSICMD_CMD,0xC8}, //Brightness function point 192 gain
        {DSICMD_DATA,0x2}, //  1


        {DSICMD_CMD,0xCA}, //Hue - coshr
        {DSICMD_DATA,0x3}, //
           {DSICMD_CMD,0xCB}, //Hue - coshry
        {DSICMD_DATA,0x0}, //
        {DSICMD_CMD,0xCC}, //Hue - coshy
        {DSICMD_DATA,0x1}, //
        {DSICMD_CMD,0xCD}, //Hue - coshyg
        {DSICMD_DATA,0x11}, //
        {DSICMD_CMD,0xCE},  //Hue - coshg
        {DSICMD_DATA,0x03}, //
        {DSICMD_CMD,0xCF}, //Hue - coshgc
        {DSICMD_DATA,0x0}, //
        {DSICMD_CMD,0xD0}, //Hue - coshc
        {DSICMD_DATA,0x19}, //
        {DSICMD_CMD,0xD1}, //Hue - coshcb
        {DSICMD_DATA,0x05}, //
        {DSICMD_CMD,0xD2}, //Hue - coshb
        {DSICMD_DATA,0x02}, //
        {DSICMD_CMD,0xD3}, //Hue - coshbm
        {DSICMD_DATA,0x0}, //
        {DSICMD_CMD,0xD4}, //Hue - coshm
        {DSICMD_DATA,0x02}, //
        {DSICMD_CMD,0xD5}, //Hue - coshmr
        {DSICMD_DATA,0x1}, //

        {DSICMD_CMD,0xD7},   // Hue - sinhr
        {DSICMD_DATA,0x73},  //
        {DSICMD_CMD,0xD8}, //Hue - sinhry
        {DSICMD_DATA,0x4},
        {DSICMD_CMD,0xD9},//Hue - sinhy
        {DSICMD_DATA,0x7},
        {DSICMD_CMD,0xDA},  //Hue - sinhyg
        {DSICMD_DATA,0x5F},
        {DSICMD_CMD,0xDB},  //Hue - sinhg
        {DSICMD_DATA,0x73},
        {DSICMD_CMD,0xDC},  //Hue - sinhgc
        {DSICMD_DATA,0x0},
        {DSICMD_CMD,0xDD}, //Hue - sinhc
        {DSICMD_DATA,0x58},

        {DSICMD_CMD,0xDE},  //Hue - sinhcb
        {DSICMD_DATA,0x12},
        {DSICMD_CMD,0xDF},  //Hue - sinhb
        {DSICMD_DATA,0xB},

        {DSICMD_CMD,0xE0},  //Hue - sinhbm
        {DSICMD_DATA,0x2},
        {DSICMD_CMD,0xE1},  //Hue - sinhm
        {DSICMD_DATA,0xB},
        {DSICMD_CMD,0xE2}, //Hue - sinhmr
        {DSICMD_DATA,0x79},



        {DSICMD_CMD,0xE3},  //The lower bound of Chroma of the skin region (0~255)
        {DSICMD_DATA,0x1E},
        {DSICMD_CMD,0xE4}, //The upper bound of Chroma of the skin region (0~255)
        {DSICMD_DATA,0x96},

        {DSICMD_CMD,0xE5}, //The end axis index of Hue of the skin region.(//0~95)
        {DSICMD_DATA,0xA},
        {DSICMD_CMD,0xE6}, //The start axis index of Hue of the skin region.(0~95)
        {DSICMD_DATA,0x5A},
        {DSICMD_CMD,0xE7},  //The gain degradation speed along with the saturation
        {DSICMD_DATA,0x4F},

        {CMDDELAY_MS,10},
#endif

        {DSICMD_CMD,0xB0},                 //
        {DSICMD_DATA,0x09},
        {DSICMD_CMD,0xC0},                 //
        {DSICMD_DATA,0xA5},
        {DSICMD_CMD,0xC3},                 //
        {DSICMD_DATA,0x30},
        {DSICMD_CMD,0xC7},                 //
        {DSICMD_DATA,0x07},
        {CMDDELAY_MS,10},

        {DSICMD_CMD,0xB0},                 //
        {DSICMD_DATA,0x00},
        {DSICMD_CMD,0xC2},                 //
        {DSICMD_DATA,0x30},

        {DSICMD_CMD,0xB0},                 //
        {DSICMD_DATA,0x08},
        {DSICMD_CMD,0xB1},                 //
        {DSICMD_DATA,0x22},

        {DSICMD_CMD,0x35},                 // TE enable, for test
        {DSICMD_DATA,0x00},

        {DSICMD_CMD,0x29},                  // display on
        {CMDDELAY_MS,120},
};

const tPANEL_CMD tCmdStandbyDSI[] =
{

     {DSICMD_CMD,     0x01},      // reset in
  //   {DSICMD_CMD,     0x28},     // Display OFF
     {CMDDELAY_MS,    120},
 //    {DSICMD_CMD,     0x10},      // Sleep in
 //    {CMDDELAY_MS,    120},

};

const tLCD_PARAM tModeDSI[] =
{
    /***********       MI Serial Format 1      *************/
    {
        // tPANEL_PARAM
        {
            /* Old prototype */
            PINMUX_DSI_4_LANE_VDO_SYNC_EVENT_RGB888,  // PINMUX_DSI_4_LANE_VDO_SYNC_EVENT_RGB888
            _IDE_fDCLK,                     //!< fDCLK
            (192+PANEL_WIDTH),             //!< uiHSyncTotalPeriod(HTOTAL)  // 180 // 64  // 192 [ok]
            PANEL_WIDTH,                    //!< uiHSyncActivePeriod(HACT)
            96,                           //!< uiHSyncBackPorch(HBP)  //80  // 32 //96 [ok]
            32+ PANEL_HEIGHT,            //!< uiVSyncTotalPeriod(VTOTAL)
            PANEL_HEIGHT,                   //!< uiVSyncActivePeriod
            0x10,//VVALIDST,                       //!< uiVSyncBackPorchOdd
            0x10,//VVALIDST,                       //!< uiVSyncBackPorchEven
            PANEL_HEIGHT,                   //!< uiBufferWidth
            PANEL_WIDTH,                    //!< uiBufferHeight
            PANEL_HEIGHT,                    //!< uiWindowWidth
            PANEL_WIDTH,                   //!< uiWindowHeight
            FALSE,                          //!< bYCbCrFormat
            /* New added parameters */
           2, //HSYNCT,                         //!< uiHSyncSYNCwidth
           2  // HSYNCT                          //!< uiVSyncSYNCwidth
        },

        // tIDE_PARAM
        {
            /* Old prototype */
         PINMUX_LCD_SEL_MIPI ,//  PINMUX_LCD_SEL_GPIO,//PINMUX_LCD_SEL_GPIO,            //!< pinmux_select_lcd;
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


const tPANEL_CMD tCmdWeakUpbyDSI[] =
{
//   {CMDDELAY_MS,    120},
    {DSICMD_CMD,     0x11},     // Sleep out
    {CMDDELAY_MS,    120},
    {DSICMD_CMD,     0x29},      // Display ON
    {CMDDELAY_MS,    50},

};



const tLCD_PARAM tModeDSIWeakup[] =
{
    /***********       MI Serial Format 1      *************/
    {
        // tPANEL_PARAM
        {
            /* Old prototype */
            PINMUX_DSI_4_LANE_VDO_SYNC_EVENT_RGB888,  // PINMUX_DSI_4_LANE_VDO_SYNC_EVENT_RGB888
            _IDE_fDCLK,                     //!< fDCLK
            (192+PANEL_WIDTH),             //!< uiHSyncTotalPeriod(HTOTAL)  // 180 // 64  // 192 [ok]
            PANEL_WIDTH,                    //!< uiHSyncActivePeriod(HACT)
            96,                           //!< uiHSyncBackPorch(HBP)  //80  // 32 //96 [ok]
            32+ PANEL_HEIGHT,            //!< uiVSyncTotalPeriod(VTOTAL)
            PANEL_HEIGHT,                   //!< uiVSyncActivePeriod
            0x10,//VVALIDST,                       //!< uiVSyncBackPorchOdd
            0x10,//VVALIDST,                       //!< uiVSyncBackPorchEven
            PANEL_HEIGHT,                   //!< uiBufferWidth
            PANEL_WIDTH,                    //!< uiBufferHeight
            PANEL_HEIGHT,                    //!< uiWindowWidth
            PANEL_WIDTH,                   //!< uiWindowHeight
            FALSE,                          //!< bYCbCrFormat
            /* New added parameters */
           2, //HSYNCT,                         //!< uiHSyncSYNCwidth
           2  // HSYNCT                          //!< uiVSyncSYNCwidth
        },

        // tIDE_PARAM
        {
            /* Old prototype */
         PINMUX_LCD_SEL_MIPI ,//  PINMUX_LCD_SEL_GPIO,//PINMUX_LCD_SEL_GPIO,            //!< pinmux_select_lcd;
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

        (tPANEL_CMD*)tCmdWeakUpbyDSI,                  //!< pCmdQueue
        sizeof(tCmdWeakUpbyDSI)/sizeof(tPANEL_CMD),    //!< nCmd
    }
};




const tLCD_ROT *tRotDSI = NULL;

//@}

const tPANEL_CMD tCmdSleepDSI[] =
{
    {DSICMD_CMD,     0x28},     // Display OFF
    {DSICMD_CMD,     0x10},      // Sleep in
//      {DSICMD_CMD,0x36},                 // TE enable, for test
//      {DSICMD_DATA,0x00},

};




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

    dsi_setConfig(DSI_CONFIG_ID_THS_PREPARE, 4);//4         //  4
    dsi_setConfig(DSI_CONFIG_ID_THS_ZERO, 6); // 6
    dsi_setConfig(DSI_CONFIG_ID_THS_TRAIL, 7); // 7
    dsi_setConfig(DSI_CONFIG_ID_THS_EXIT, 6);  // 6

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
    dsi_setConfig(DSI_CONFIG_ID_DATALANE_NO, DSI_DATA_LANE_3);  //DSI_DATA_LANE_0
//#endif
    //dsi_setConfig(DSI_CONFIG_ID_TE_BTA_INTERVAL, 0x1F);

    dsi_setConfig(DSI_CONFIG_ID_CLK_LP_CTRL, 0x1);
    //dsi_setConfig(DSI_CONFIG_ID_SYNC_DLY_CNT, 0xF);
//  FOR  4lane  use
    dsi_setConfig(DSI_CONFIG_ID_EOT_PKT_EN, 1);
}
#endif

