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

#define PANEL_WIDTH     400
#define PANEL_HEIGHT    1280

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
#if 1
#define DSI_TARGET_CLK             240
#else
#define DSI_TARGET_CLK             120
#endif
#endif


#if(DSI_PACKET_FORMAT == DSI_FORMAT_RGB888)
#if (DSI_TARGET_CLK == 480)
#define _IDE_fDCLK      60.00f//((((DSI_TARGET_CLK / 8) * 2)/3))
#else
#if (DSI_TARGET_CLK == 240)
#define _IDE_fDCLK      60.00f //24.00f// must > ((((DSI_TARGET_CLK / 8) * 2) / 3)) ->2: lane number
#else
#define _IDE_fDCLK      13.33f
#endif
#endif
#else
#error "Format not RGB888"
#endif

#define HVALIDST    160 //0x39  //uiHSyncBackPorch(HPB) -> 53
#define VVALIDST    10  //0x15  //uiVSyncBackPorchOdd/Even
#define HSYNCT      24  //0x02
#define VSYNCT      0x02

#define ROTATE_180DEGREE      DISABLE//ENABLE//
/*
    Panel Parameters for TCON HX8369B
*/
//@{
/*Used in DSI*/
/*
MIPI command:
regw(0x10);
regw(0xCD,0xAA);
regw(0x30,0x00);
regw(0x39,0x11);
regw(0x32,0x00);
regw(0x33,0x38);
regw(0x35,0x24);
regw(0x4F,0x35);//26
regw(0x4E,0x27);//13
regw(0x41,0x32);//VCOM36
regw(0x55,0x00,0x0F,0x00,0x0F,0x00,0x0F,0x00,0x0F);
regW(0x56,0x00,0x0F,0x00,0x0F,0x00,0x0F,0x00,0x0F,0x00,0x0F,0x00,0x0F,0x00,0x0F,0x00,0x0F);
regw(0x65,0x08);
regw(0x3A,0x08);
regw(0x36,0x49);
regw(0x67,0x82);
regw(0x69,0x20);
regw(0x6C,0x80);
regw(0x6D,0x01);
regw(0x53,0x1F,0x19,0x15,0x11,0x11,0x11,0x12,0x14,0x15,0x11,0x0D,0x0B,0x0B,0x0D,0x0C,0x0C,0x08,0x04,0x00);
regw(0x54,0x1F,0x19,0x15,0x11,0x11,0x11,0x13,0x15,0x16,0x11,0x0D,0x0C,0x0C,0x0E,0x0C,0x0C,0x08,0x04,0x00);
regw(0x6B,0x00);
regw(0x58,0x00);
regw(0x73,0xF0);
regw(0x76,0x40);
regw(0x77,0x04);//00
regw(0x74,0x17);
regw(0x5E,0x03);
regw(0x68,0x10);
regw(0x6A,0x00);
regw(0x28,0x31);
regw(0x29,0x21);
regw(0x63,0x04);//08
regw(0x27,0x00);
regw(0x7c,0x80);
regw(0x2e,0x05);//PWM
regw(0x4c,0x80);//VCSW1
regw(0x50,0xc0);
regw(0x78,0x6E);
regw(0x2D,0x31);
regw(0x49,0x00);//
regw(0x4D,0x00);
regw(0x11);
*/

const tPANEL_CMD tCmdModeDSI[] =
{
    {DSICMD_CMD,0xB0},{DSICMD_DATA,0x5A},

    {DSICMD_CMD,0xB1},{DSICMD_DATA,0x00},
    {DSICMD_CMD,0x89},{DSICMD_DATA,0x01},
    {DSICMD_CMD,0x91},{DSICMD_DATA,0x07},
    {DSICMD_CMD,0x92},{DSICMD_DATA,0xF9},
    {DSICMD_CMD,0xB1},{DSICMD_DATA,0x03},
    {DSICMD_CMD,0x2C},{DSICMD_DATA,0x28},

    {DSICMD_CMD,0x00},{DSICMD_DATA,0xB7},
    {DSICMD_CMD,0x01},{DSICMD_DATA,0x1B},
    {DSICMD_CMD,0x02},{DSICMD_DATA,0x00},
    {DSICMD_CMD,0x03},{DSICMD_DATA,0x00},
    {DSICMD_CMD,0x04},{DSICMD_DATA,0x00},
    {DSICMD_CMD,0x05},{DSICMD_DATA,0x00},
    {DSICMD_CMD,0x06},{DSICMD_DATA,0x00},
    {DSICMD_CMD,0x07},{DSICMD_DATA,0x00},
    {DSICMD_CMD,0x08},{DSICMD_DATA,0x00},
    {DSICMD_CMD,0x09},{DSICMD_DATA,0x00},
    {DSICMD_CMD,0x0A},{DSICMD_DATA,0x01},
    {DSICMD_CMD,0x0B},{DSICMD_DATA,0x01},
    {DSICMD_CMD,0x0C},{DSICMD_DATA,0x20},
    {DSICMD_CMD,0x0D},{DSICMD_DATA,0x00},
    {DSICMD_CMD,0x0E},{DSICMD_DATA,0x24},
    {DSICMD_CMD,0x0F},{DSICMD_DATA,0x1C},
    {DSICMD_CMD,0x10},{DSICMD_DATA,0xC9},
    {DSICMD_CMD,0x11},{DSICMD_DATA,0x60},
    {DSICMD_CMD,0x12},{DSICMD_DATA,0x70},
    {DSICMD_CMD,0x13},{DSICMD_DATA,0x01},
    //{DSICMD_CMD,0x14},{DSICMD_DATA,0xE7},
    {DSICMD_CMD,0x14},{DSICMD_DATA,0xE5},
    {DSICMD_CMD,0x15},{DSICMD_DATA,0xFF},
    {DSICMD_CMD,0x16},{DSICMD_DATA,0x3D},
    {DSICMD_CMD,0x17},{DSICMD_DATA,0x0E},
    {DSICMD_CMD,0x18},{DSICMD_DATA,0x01},
    {DSICMD_CMD,0x19},{DSICMD_DATA,0x00},
    {DSICMD_CMD,0x1A},{DSICMD_DATA,0x00},
    {DSICMD_CMD,0x1B},{DSICMD_DATA,0xFC},
    {DSICMD_CMD,0x1C},{DSICMD_DATA,0x0B},
    {DSICMD_CMD,0x1D},{DSICMD_DATA,0xA0},
    {DSICMD_CMD,0x1E},{DSICMD_DATA,0x03},
    {DSICMD_CMD,0x1F},{DSICMD_DATA,0x04},
    {DSICMD_CMD,0x20},{DSICMD_DATA,0x0C},
    {DSICMD_CMD,0x21},{DSICMD_DATA,0x00},
    {DSICMD_CMD,0x22},{DSICMD_DATA,0x04},
    {DSICMD_CMD,0x23},{DSICMD_DATA,0x81},
    {DSICMD_CMD,0x24},{DSICMD_DATA,0x1F},
    {DSICMD_CMD,0x25},{DSICMD_DATA,0x10},
    {DSICMD_CMD,0x26},{DSICMD_DATA,0x9B},
    {DSICMD_CMD,0x2D},{DSICMD_DATA,0x01},
    {DSICMD_CMD,0x2E},{DSICMD_DATA,0x84},
    {DSICMD_CMD,0x2F},{DSICMD_DATA,0x00},
    {DSICMD_CMD,0x30},{DSICMD_DATA,0x02},
    {DSICMD_CMD,0x31},{DSICMD_DATA,0x08},
    {DSICMD_CMD,0x32},{DSICMD_DATA,0x01},
    {DSICMD_CMD,0x33},{DSICMD_DATA,0x1C},
    {DSICMD_CMD,0x34},{DSICMD_DATA,0x40},
    {DSICMD_CMD,0x35},{DSICMD_DATA,0xFF},
    {DSICMD_CMD,0x36},{DSICMD_DATA,0xFF},
    {DSICMD_CMD,0x37},{DSICMD_DATA,0xFF},
    {DSICMD_CMD,0x38},{DSICMD_DATA,0xFF},
    {DSICMD_CMD,0x39},{DSICMD_DATA,0xFF},
    {DSICMD_CMD,0x3A},{DSICMD_DATA,0x05},
    {DSICMD_CMD,0x3B},{DSICMD_DATA,0x00},
    {DSICMD_CMD,0x3C},{DSICMD_DATA,0x00},
    {DSICMD_CMD,0x3D},{DSICMD_DATA,0x00},
    {DSICMD_CMD,0x3E},{DSICMD_DATA,0xCF},
    {DSICMD_CMD,0x3F},{DSICMD_DATA,0x84},
    {DSICMD_CMD,0x40},{DSICMD_DATA,0x28},
    {DSICMD_CMD,0x41},{DSICMD_DATA,0xFC},
    {DSICMD_CMD,0x42},{DSICMD_DATA,0x01},
    {DSICMD_CMD,0x43},{DSICMD_DATA,0x40},
    {DSICMD_CMD,0x44},{DSICMD_DATA,0x05},
    {DSICMD_CMD,0x45},{DSICMD_DATA,0xE8},
    {DSICMD_CMD,0x46},{DSICMD_DATA,0x16},
    {DSICMD_CMD,0x47},{DSICMD_DATA,0x00},
    {DSICMD_CMD,0x48},{DSICMD_DATA,0x00},
    {DSICMD_CMD,0x49},{DSICMD_DATA,0x88},
    {DSICMD_CMD,0x4A},{DSICMD_DATA,0x08},
    {DSICMD_CMD,0x4B},{DSICMD_DATA,0x05},
    {DSICMD_CMD,0x4C},{DSICMD_DATA,0x03},
    {DSICMD_CMD,0x4D},{DSICMD_DATA,0xD0},
    {DSICMD_CMD,0x4E},{DSICMD_DATA,0x13},
    {DSICMD_CMD,0x4F},{DSICMD_DATA,0xFF},
    {DSICMD_CMD,0x50},{DSICMD_DATA,0x0A},
    {DSICMD_CMD,0x51},{DSICMD_DATA,0x53},
    {DSICMD_CMD,0x52},{DSICMD_DATA,0x26},
    {DSICMD_CMD,0x53},{DSICMD_DATA,0x22},
    {DSICMD_CMD,0x54},{DSICMD_DATA,0x09},
    {DSICMD_CMD,0x55},{DSICMD_DATA,0x22},
    {DSICMD_CMD,0x56},{DSICMD_DATA,0x00},
    {DSICMD_CMD,0x57},{DSICMD_DATA,0x1C},
    {DSICMD_CMD,0x58},{DSICMD_DATA,0x03},
    {DSICMD_CMD,0x59},{DSICMD_DATA,0x3F},
    {DSICMD_CMD,0x5A},{DSICMD_DATA,0x28},
    {DSICMD_CMD,0x5B},{DSICMD_DATA,0x01},
    {DSICMD_CMD,0x5C},{DSICMD_DATA,0xCC},
    {DSICMD_CMD,0x5D},{DSICMD_DATA,0x21},
    {DSICMD_CMD,0x5E},{DSICMD_DATA,0x84},
    {DSICMD_CMD,0x5F},{DSICMD_DATA,0x10},
    {DSICMD_CMD,0x60},{DSICMD_DATA,0x42},
    {DSICMD_CMD,0x61},{DSICMD_DATA,0x40},
    {DSICMD_CMD,0x62},{DSICMD_DATA,0x06},
    {DSICMD_CMD,0x63},{DSICMD_DATA,0x3A},
    {DSICMD_CMD,0x64},{DSICMD_DATA,0xA6},
    {DSICMD_CMD,0x65},{DSICMD_DATA,0x04},
    {DSICMD_CMD,0x66},{DSICMD_DATA,0x09},
    {DSICMD_CMD,0x67},{DSICMD_DATA,0x21},
    {DSICMD_CMD,0x68},{DSICMD_DATA,0x84},
    {DSICMD_CMD,0x69},{DSICMD_DATA,0x10},
    {DSICMD_CMD,0x6A},{DSICMD_DATA,0x42},
    {DSICMD_CMD,0x6B},{DSICMD_DATA,0x08},
    {DSICMD_CMD,0x6C},{DSICMD_DATA,0x21},
    {DSICMD_CMD,0x6D},{DSICMD_DATA,0x84},
    {DSICMD_CMD,0x6E},{DSICMD_DATA,0x74},
    {DSICMD_CMD,0x6F},{DSICMD_DATA,0xE2},
    {DSICMD_CMD,0x70},{DSICMD_DATA,0x6B},
    {DSICMD_CMD,0x71},{DSICMD_DATA,0x6B},
    {DSICMD_CMD,0x72},{DSICMD_DATA,0x94},
    {DSICMD_CMD,0x73},{DSICMD_DATA,0x10},
    {DSICMD_CMD,0x74},{DSICMD_DATA,0x42},
    {DSICMD_CMD,0x75},{DSICMD_DATA,0x08},
    {DSICMD_CMD,0x76},{DSICMD_DATA,0x00},
    {DSICMD_CMD,0x77},{DSICMD_DATA,0x00},
    {DSICMD_CMD,0x78},{DSICMD_DATA,0x0F},
    {DSICMD_CMD,0x79},{DSICMD_DATA,0xE0},
    {DSICMD_CMD,0x7A},{DSICMD_DATA,0x01},
    {DSICMD_CMD,0x7B},{DSICMD_DATA,0xFF},
    {DSICMD_CMD,0x7C},{DSICMD_DATA,0xFF},
    {DSICMD_CMD,0x7D},{DSICMD_DATA,0x0F},
    {DSICMD_CMD,0x7E},{DSICMD_DATA,0x41},
    {DSICMD_CMD,0x7F},{DSICMD_DATA,0xFE},


    {DSICMD_CMD,0xB1},{DSICMD_DATA,0x02},

    {DSICMD_CMD,0x00},{DSICMD_DATA,0xFF},
    {DSICMD_CMD,0x01},{DSICMD_DATA,0x05},
    {DSICMD_CMD,0x02},{DSICMD_DATA,0xC8},
    {DSICMD_CMD,0x03},{DSICMD_DATA,0x00},
    {DSICMD_CMD,0x04},{DSICMD_DATA,0x14},
    {DSICMD_CMD,0x05},{DSICMD_DATA,0x4B},
    {DSICMD_CMD,0x06},{DSICMD_DATA,0x64},
    {DSICMD_CMD,0x07},{DSICMD_DATA,0x0A},
    {DSICMD_CMD,0x08},{DSICMD_DATA,0xC0},
    {DSICMD_CMD,0x09},{DSICMD_DATA,0x00},
    {DSICMD_CMD,0x0A},{DSICMD_DATA,0x00},
    {DSICMD_CMD,0x0B},{DSICMD_DATA,0x10},
    {DSICMD_CMD,0x0C},{DSICMD_DATA,0xE6},
    {DSICMD_CMD,0x0D},{DSICMD_DATA,0x0D},
    {DSICMD_CMD,0x0F},{DSICMD_DATA,0x00},
    {DSICMD_CMD,0x10},{DSICMD_DATA,0x79},
    {DSICMD_CMD,0x11},{DSICMD_DATA,0x7C},
    {DSICMD_CMD,0x12},{DSICMD_DATA,0xFF},
    {DSICMD_CMD,0x13},{DSICMD_DATA,0x9E},
    {DSICMD_CMD,0x14},{DSICMD_DATA,0x87},
    {DSICMD_CMD,0x15},{DSICMD_DATA,0xAA},
    {DSICMD_CMD,0x16},{DSICMD_DATA,0xA4},
    {DSICMD_CMD,0x17},{DSICMD_DATA,0x59},
    {DSICMD_CMD,0x18},{DSICMD_DATA,0x65},
    {DSICMD_CMD,0x19},{DSICMD_DATA,0xA5},
    {DSICMD_CMD,0x1A},{DSICMD_DATA,0x6D},
    {DSICMD_CMD,0x1B},{DSICMD_DATA,0x0E},
    {DSICMD_CMD,0x1C},{DSICMD_DATA,0xFF},
    {DSICMD_CMD,0x1D},{DSICMD_DATA,0xFF},
    {DSICMD_CMD,0x1E},{DSICMD_DATA,0xFF},
    {DSICMD_CMD,0x1F},{DSICMD_DATA,0xFF},
    {DSICMD_CMD,0x20},{DSICMD_DATA,0xFF},
    {DSICMD_CMD,0x21},{DSICMD_DATA,0xFF},
    {DSICMD_CMD,0x22},{DSICMD_DATA,0xFF},
    {DSICMD_CMD,0x23},{DSICMD_DATA,0xFF},
    {DSICMD_CMD,0x24},{DSICMD_DATA,0xFF},
    {DSICMD_CMD,0x25},{DSICMD_DATA,0xFF},
    {DSICMD_CMD,0x26},{DSICMD_DATA,0xFF},
    {DSICMD_CMD,0x27},{DSICMD_DATA,0x1F},
    {DSICMD_CMD,0x28},{DSICMD_DATA,0xFF},
    {DSICMD_CMD,0x29},{DSICMD_DATA,0xFF},
    {DSICMD_CMD,0x2A},{DSICMD_DATA,0xFF},
    {DSICMD_CMD,0x2B},{DSICMD_DATA,0xFF},
    {DSICMD_CMD,0x2C},{DSICMD_DATA,0xFF},
    {DSICMD_CMD,0x2D},{DSICMD_DATA,0x07},
    {DSICMD_CMD,0x33},{DSICMD_DATA,0x3F},
    {DSICMD_CMD,0x35},{DSICMD_DATA,0x7F},
    {DSICMD_CMD,0x36},{DSICMD_DATA,0x3F},
    {DSICMD_CMD,0x38},{DSICMD_DATA,0xFF},
    {DSICMD_CMD,0x3A},{DSICMD_DATA,0x80},
    {DSICMD_CMD,0x3B},{DSICMD_DATA,0x01},
    {DSICMD_CMD,0x3C},{DSICMD_DATA,0x80},
    {DSICMD_CMD,0x3D},{DSICMD_DATA,0x2C},
    {DSICMD_CMD,0x3E},{DSICMD_DATA,0x00},
    {DSICMD_CMD,0x3F},{DSICMD_DATA,0x90},
    {DSICMD_CMD,0x40},{DSICMD_DATA,0x05},
    {DSICMD_CMD,0x41},{DSICMD_DATA,0x00},
    {DSICMD_CMD,0x42},{DSICMD_DATA,0xB2},
    {DSICMD_CMD,0x43},{DSICMD_DATA,0x00},
    {DSICMD_CMD,0x44},{DSICMD_DATA,0x40},
    {DSICMD_CMD,0x45},{DSICMD_DATA,0x06},
    {DSICMD_CMD,0x46},{DSICMD_DATA,0x00},
    {DSICMD_CMD,0x47},{DSICMD_DATA,0x00},
    {DSICMD_CMD,0x48},{DSICMD_DATA,0x9B},
    {DSICMD_CMD,0x49},{DSICMD_DATA,0xD2},
    {DSICMD_CMD,0x4A},{DSICMD_DATA,0x21},
    {DSICMD_CMD,0x4B},{DSICMD_DATA,0x43},
    {DSICMD_CMD,0x4C},{DSICMD_DATA,0x16},
    {DSICMD_CMD,0x4D},{DSICMD_DATA,0xC0},
    {DSICMD_CMD,0x4E},{DSICMD_DATA,0x0F},
    {DSICMD_CMD,0x4F},{DSICMD_DATA,0xF1},
    {DSICMD_CMD,0x50},{DSICMD_DATA,0x78},
    {DSICMD_CMD,0x51},{DSICMD_DATA,0x7A},
    {DSICMD_CMD,0x52},{DSICMD_DATA,0x34},
    {DSICMD_CMD,0x53},{DSICMD_DATA,0x99},
    {DSICMD_CMD,0x54},{DSICMD_DATA,0xA2},
    {DSICMD_CMD,0x55},{DSICMD_DATA,0x02},
    {DSICMD_CMD,0x56},{DSICMD_DATA,0x14},
    {DSICMD_CMD,0x57},{DSICMD_DATA,0xB8},
    {DSICMD_CMD,0x58},{DSICMD_DATA,0xDC},
    {DSICMD_CMD,0x59},{DSICMD_DATA,0xD4},
    {DSICMD_CMD,0x5A},{DSICMD_DATA,0xEF},
    {DSICMD_CMD,0x5B},{DSICMD_DATA,0xF7},
    {DSICMD_CMD,0x5C},{DSICMD_DATA,0xFB},
    {DSICMD_CMD,0x5D},{DSICMD_DATA,0xFD},
    {DSICMD_CMD,0x5E},{DSICMD_DATA,0x7E},
    {DSICMD_CMD,0x5F},{DSICMD_DATA,0xBF},
    {DSICMD_CMD,0x60},{DSICMD_DATA,0xEF},
    {DSICMD_CMD,0x61},{DSICMD_DATA,0xE6},
    {DSICMD_CMD,0x62},{DSICMD_DATA,0x76},
    {DSICMD_CMD,0x63},{DSICMD_DATA,0x73},
    {DSICMD_CMD,0x64},{DSICMD_DATA,0xBB},
    {DSICMD_CMD,0x65},{DSICMD_DATA,0xDD},
    {DSICMD_CMD,0x66},{DSICMD_DATA,0x6E},
    {DSICMD_CMD,0x67},{DSICMD_DATA,0x37},
    {DSICMD_CMD,0x68},{DSICMD_DATA,0x8C},
    {DSICMD_CMD,0x69},{DSICMD_DATA,0x08},
    {DSICMD_CMD,0x6A},{DSICMD_DATA,0x31},
    {DSICMD_CMD,0x6B},{DSICMD_DATA,0xB8},
    {DSICMD_CMD,0x6C},{DSICMD_DATA,0xB8},
    {DSICMD_CMD,0x6D},{DSICMD_DATA,0xB8},
    {DSICMD_CMD,0x6E},{DSICMD_DATA,0xB8},
    {DSICMD_CMD,0x6F},{DSICMD_DATA,0xB8},
    {DSICMD_CMD,0x70},{DSICMD_DATA,0x5C},
    {DSICMD_CMD,0x71},{DSICMD_DATA,0x2E},
    {DSICMD_CMD,0x72},{DSICMD_DATA,0x17},
    {DSICMD_CMD,0x73},{DSICMD_DATA,0x00},
    {DSICMD_CMD,0x74},{DSICMD_DATA,0x00},
    {DSICMD_CMD,0x75},{DSICMD_DATA,0x00},
    {DSICMD_CMD,0x76},{DSICMD_DATA,0x00},
    {DSICMD_CMD,0x77},{DSICMD_DATA,0x00},
    {DSICMD_CMD,0x78},{DSICMD_DATA,0x00},
    {DSICMD_CMD,0x79},{DSICMD_DATA,0x00},
    {DSICMD_CMD,0x7A},{DSICMD_DATA,0xDC},
    {DSICMD_CMD,0x7B},{DSICMD_DATA,0xDC},
    {DSICMD_CMD,0x7C},{DSICMD_DATA,0xDC},
    {DSICMD_CMD,0x7D},{DSICMD_DATA,0xDC},
    {DSICMD_CMD,0x7E},{DSICMD_DATA,0xDC},
    {DSICMD_CMD,0x7F},{DSICMD_DATA,0x6E},
    {DSICMD_CMD,0x0B},{DSICMD_DATA,0x00},

    {DSICMD_CMD,0xB1},{DSICMD_DATA,0x03},
    {DSICMD_CMD,0x2C},{DSICMD_DATA,0x2C},

    {DSICMD_CMD,0xB1},{DSICMD_DATA,0x00},
    {DSICMD_CMD,0x89},{DSICMD_DATA,0x03},
    {DSICMD_CMD,0x11},
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
            746,                            //!< uiHSyncTotalPeriod(HTOTAL)
            PANEL_WIDTH,                    //!< uiHSyncActivePeriod(HACT)
            0xA0,                           //!< uiHSyncBackPorch(HBP)
            1304,                           //!< uiVSyncTotalPeriod(VTOTAL)
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
#elif(DSI_TARGET_CLK == 240)  // To adjust...
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


    #if 1 // 2 lanes.
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

