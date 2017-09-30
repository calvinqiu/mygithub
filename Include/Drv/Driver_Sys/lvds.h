/**
    LVDS/HiSPi Controller global header

    LVDS/HiSPi controller is basically the high speed sensor interface controller.
    In NT96660, two lvds engines are implemented (Dual LVDS sensors capable).
    The user can reference this section for the detail description of the each driver API usage
    and also the parameter descriptions and its limitations.
    The overall combinational usage flow is introduced in the application note document,
    and the user must reference to the application note for the driver usage flow.

    @file       lvds.h
    @ingroup    mIDrvIO_LVDS
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.
*/
#ifndef _LVDS_H
#define _LVDS_H

#include "Type.h"

/**
    @addtogroup mIDrvIO_LVDS
*/
//@{

/**
    LVDS Clock Lane Number

    This definition is used in lvds_setConfig(LVDS_CONFIG_ID_CLANE_NUMBER)
    to specify the clock lane number is used for the LVDS controller.
    The user should notice that if two clock lanes are used,
    the input pad pins HSI_D0~4 are mapped to HSI_CK0P/N and HSI_D5~9 are mapped to HSI_CK1P/N.
    If dual LVDS interface are used, the lvds controller which uses second clock lane(HSI_CK1P/N) must still
    set Clock Lane number to LVDS_CLKLANE_2.
*/
typedef enum
{
    LVDS_CLKLANE_1,             ///< LVDS/HiSPi uses single clock lane.
                                ///< This means enabling the HSI_CK0P/N.
    LVDS_CLKLANE_2,             ///< LVDS/HiSPi uses two clock lanes.
                                ///< If two clock lanes are used, the data lane number must be even numbered.
                                ///< This means enabling the HSI_CK0P/N and HSI_CK1P/N. If dual LVDS interface are used,
                                ///< the lvds controller which uses second clock lane(HSI_CK1P/N) must still
                                ///< set Clock Lane number to LVDS_CLKLANE_2 to enable second clock lane.

    ENUM_DUMMY4WORD(LVDS_CLKLANE_NO)
}LVDS_CLKLANE_NO;

/**
    LVDS Data Lane Number

    This definition is used in lvds_setConfig(LVDS_CONFIG_ID_DLANE_NUMBER)
    to specify the data lane number are used for the LVDS controller.
    If two clock lanes are used, the data lane number must be even number.
    If two lvds controllers are used, maximum data lane number is 4 lanes.
*/
typedef enum
{
    LVDS_DATLANE_1,             ///< LVDS/HiSPi use  1 data lane.
    LVDS_DATLANE_2,             ///< LVDS/HiSPi use  2 data lanes.
    LVDS_DATLANE_3,             ///< LVDS/HiSPi use  3 data lanes.
    LVDS_DATLANE_4,             ///< LVDS/HiSPi use  4 data lanes.
    LVDS_DATLANE_6,             ///< LVDS/HiSPi use  6 data lanes.
    LVDS_DATLANE_8,             ///< LVDS/HiSPi use  8 data lanes.
    LVDS_DATLANE_10,            ///< LVDS/HiSPi use 10 data lanes.

    ENUM_DUMMY4WORD(LVDS_DATLANE_NO)
}LVDS_DATLANE_NO;

/**
    LVDS Pixel Depth

    This definition is used in lvds_setConfig(LVDS_CONFIG_ID_PIXEL_DEPTH)
    to specify the pixel depth of the sensor output.
    Please refer to the driver appilication note for the detail introduction.
    If the used pixel depth is greater than 12 bits, we can enable the controller's
    rounding function to round the bit13 to bit12 by lvds_setConfig(LVDS_CONFIG_ID_ROUND_ENABLE, ENABLE).
    This is because the NT96660's image pipeline design is 12bits per pixel, and the rounding function can introdue
    the bit13 information to bit12 to provide the better information than purely 12bits information.
*/
typedef enum
{
    LVDS_PIXDEPTH_8BIT,         ///< Pixel depth is  8 bits per pixel.
    LVDS_PIXDEPTH_10BIT,        ///< Pixel depth is 10 bits per pixel.
    LVDS_PIXDEPTH_12BIT,        ///< Pixel depth is 12 bits per pixel.
    LVDS_PIXDEPTH_14BIT,        ///< Pixel depth is 14 bits per pixel.
    LVDS_PIXDEPTH_16BIT,        ///< Pixel depth is 16 bits per pixel.

    LVDS_PIXDEPTH_CROP2LANE,    //  Reserved for novatek internal usage only.
    ENUM_DUMMY4WORD(LVDS_PIXDEPTH)
}LVDS_PIXDEPTH;

/**
    LVDS Receive Bit Order per pixel

    This definition is used in lvds_setConfig(LVDS_CONFIG_ID_PIXEL_INORDER)
    to specify the pixel input MSb/LSb first order from the sensor output streaming.
*/
typedef enum
{
    LVDS_DATAIN_BIT_ORDER_LSB,  ///< Data Input Bit order is Least Significant Bit First.
    LVDS_DATAIN_BIT_ORDER_MSB,  ///< Data Input Bit order is Most Significant Bit First.

    ENUM_DUMMY4WORD(LVDS_DATAIN_BIT_ORDER)
}LVDS_DATAIN_BIT_ORDER;

/**
    LVDS Vsync Pulse Generation Method

    This definition is used in lvds_setConfig(LVDS_CONFIG_ID_VD_GEN_METHOD)
    to specify the controller Vsync pulse generation method.
    This is an important function setting for sensor image synchronization and would be different across different sensor vendors.
    Please refer to the application note for this function introduction.
*/
typedef enum
{
    LVDS_VSYNC_GEN_MATCH_VD,        ///< LVDS/HiSPi Vsync Pulse Generate is compare the VD Sync code matched.
                                    ///< This option is suited for the PANASONIC/Aptina LVDS sensor.
    LVDS_VSYNC_GEN_VD2HD,           ///< LVDS/HiSPi Vsync Pulse Generate is detecting SYNC_VD to SYNC_HD transition.
                                    ///< This option is suited for the SONY/SHARP/Samsung LVDS sensor.
    LVDS_VSYNC_GEN_HD2VD,           ///< LVDS/HiSPi Vsync Pulse Generate is detecting SYNC_HD to SYNC_VD transition.
                                    ///< This option is reserved for future usage.
    LVDS_VSYNC_GEN_MATCH_VD_BY_FSET,///< LVDS/HiSPi Vsync Pulse Generate is depending on the FSET bit changed in SYNC Code.
                                    ///< After FSET bit changed, the first detected VD_SYNC Code would generate Vsync to SIE1.
                                    ///< The first detected VD2_SYNC Code would generate Vsync to SIE2.
                                    ///< This mode is used in the HDR mode for SONY Sensor.

    ENUM_DUMMY4WORD(LVDS_VSYNC_GEN)
}LVDS_VSYNC_GEN;

/**
    LVDS Interrupt identification

    This identification can be used in the LVDS ISR callback to identify which interrupt status is issued. And also the
    API lvds_waitInterrupt() to wait the interrupt is triggered.
    The user must make sure that the driver is opened and set enabled before using lvds_waitInterrupt().
*/
typedef enum
{
    LVDS_INTERRUPT_VD           = 0x00000001,       ///< The interrupt is triggered when Vsync event from sensor is got.
    LVDS_INTERRUPT_HD           = 0x00000002,       ///< The interrupt is triggered when Hsync event from sensor is got.
    LVDS_INTERRUPT_FRMEND       = 0x00000004,       ///< The interrupt is triggered when the final line of the current frame is got from sensor.
    LVDS_INTERRUPT_PIXCNT_ER    = 0x00000008,       ///< The interrupt is triggered when the pixel count setting is too large.
    LVDS_INTERRUPT_FIFO_ER      = 0x00000010,       ///< The interrupt is triggered when the controller internal fifo state error.
    LVDS_INTERRUPT_FIFO1_OV     = 0x00000020,       ///< The interrupt is triggered when FIFO1 is OverRun.
    LVDS_INTERRUPT_FIFO2_OV     = 0x00000040,       ///< The interrupt is triggered when FIFO2 is OverRun.

    LVDS_INTERRUPT_VD2          = 0x00000100,       ///< The interrupt is triggered when Vsync 2 event from sensor is got.
    LVDS_INTERRUPT_HD2          = 0x00000200,       ///< The interrupt is triggered when Hsync 2 event from sensor is got.
    LVDS_INTERRUPT_FRMEND2      = 0x00000400,       ///< The interrupt is triggered when the final line of the current frame 2 is got from sensor.
    LVDS_INTERRUPT_PIXCNT_ER2   = 0x00000800,       ///< The interrupt is triggered when the pixel count setting is too large.

    LVDS_INTERRUPT_ALL          = 0x00000F7F,
    ENUM_DUMMY4WORD(LVDS_INTERRUPT)
}LVDS_INTERRUPT;

/**
    LVDS Data Lane identification

    This definition is used in lvds_setChConfig() to specify which of the Data Lane channel is selected
    to assign new configurations.
*/
typedef enum
{
    LVDS_DATLANE_ID_0,                          ///< Select Data Lane 0.
    LVDS_DATLANE_ID_1,                          ///< Select Data Lane 1.
    LVDS_DATLANE_ID_2,                          ///< Select Data Lane 2.
    LVDS_DATLANE_ID_3,                          ///< Select Data Lane 3.
    LVDS_DATLANE_ID_4,                          ///< Select Data Lane 4.
    LVDS_DATLANE_ID_5,                          ///< Select Data Lane 5.
    LVDS_DATLANE_ID_6,                          ///< Select Data Lane 6.
    LVDS_DATLANE_ID_7,                          ///< Select Data Lane 7.
    LVDS_DATLANE_ID_8,                          ///< Select Data Lane 8.
    LVDS_DATLANE_ID_9,                          ///< Select Data Lane 9.

    LVDS_DATLANE_ID_MAX,
    ENUM_DUMMY4WORD(LVDS_DATLANE_ID)
}LVDS_DATLANE_ID;

/**
    LVDS Data Lane configuration selection

    This definition is used in lvds_setChConfig() to specify which of the Data Lane channel is selected
    to assign new configurations.
*/
typedef enum
{
    LVDS_CONFIG_CTRLWORD_HD,                    ///< Set new control word HD configuration to specified data lane.
    LVDS_CONFIG_CTRLWORD_VD,                    ///< Set new control word VD configuration to specified data lane.
    LVDS_CONFIG_CTRLWORD_VD2,                   ///< Set new control word VD2 configuration to specified data lane.

    LVDS_CONFIG_CTRLWORD_HD_MASK,               ///< Set new control word HD mask configuration to specified data lane.
    LVDS_CONFIG_CTRLWORD_VD_MASK,               ///< Set new control word VD mask configuration to specified data lane.

    ENUM_DUMMY4WORD(LVDS_CH_CONFIG_ID)
}LVDS_CH_CONFIG_ID;

/**
    LVDS controller general configuration Selection

    This is used in lvds_setConfig() to specify which of the function is selected to assign new configuration.
*/
typedef enum
{
    LVDS_CONFIG_ID_DLANE_NUMBER,    ///< Set new configuration to Data Lane Number.
                                    ///< Please use "LVDS_DATLANE_NO" as input parameter.
    LVDS_CONFIG_ID_CLANE_NUMBER,    ///< Set new configuration to Clock Lane Number.
                                    ///< Please use "LVDS_CLKLANE_NO" as input parameter.
    LVDS_CONFIG_ID_PIXEL_DEPTH,     ///< Set new configuration to Pixel Depth.
                                    ///< Please use "LVDS_PIXDEPTH" as input parameter.
    LVDS_CONFIG_ID_PIXEL_INORDER,   ///< Set new configuration to input pixel order.
                                    ///< Please use "LVDS_DATAIN_BIT_ORDER" as input parameter.
    LVDS_CONFIG_ID_ROUND_ENABLE,    ///< Set rounding function enable/disable. This is valid only when pixel depth larger than 12bits.
                                    ///< Please use ENABLE / DISABLE as input parameter.
    LVDS_CONFIG_ID_VD_GEN_METHOD,   ///< Set new configuration to Vsync generation method.
                                    ///< Please use "LVDS_VSYNC_GEN" as input parameter.
    LVDS_CONFIG_ID_VD_GEN_WITH_HD,  ///< Set Vsync generation together with Hsync function enable/disable.
                                    ///< Please use ENABLE / DISABLE as input parameter.

    LVDS_CONFIG_ID_VALID_WIDTH,     ///< Set new configuration to image valid width.
                                    ///< Valid value range is from 1 to 32767. This value includes the blanking pixels behind the HD SYNC code.
    LVDS_CONFIG_ID_VALID_HEIGHT,    ///< Set new configuration to image valid height for SIE1 path.
                                    ///< Valid value range is from 1 to 16383. This value includes the blanking lines behind the VD SYNC code.
    LVDS_CONFIG_ID_VALID_HEIGHT2,   ///< Set new configuration to image valid height for SIE2 path.
                                    ///< Valid value range is from 1 to 16383. This value includes the blanking lines behind the VD SYNC code.
    LVDS_CONFIG_ID_FSET_BIT,        ///< Set FrameSet bit position for SONY HDR Type Sensor.
    LVDS_CONFIG_ID_CHID_BIT0,       ///< Set Virtual Channel ID Bit[0] position. Valid value is 0 ~ 15.
    LVDS_CONFIG_ID_CHID_BIT1,       ///< Set Virtual Channel ID Bit[1] position. Valid value is 0 ~ 15.
    LVDS_CONFIG_ID_CHID,            ///< Set virtual Channel ID 1 value. Valid value is 0 ~ 3.
    LVDS_CONFIG_ID_CHID2,           ///< Set virtual Channel ID 2 value. Valid value is 0 ~ 3.
    LVDS_CONFIG_ID_IMGDUPLICATE_EN, ///< Set Enable/Disable SIE1 Image Duplicate to SIE2.
    LVDS_CONFIG_ID_DISABLE_SOURCE,  ///< This selects the LVDS DISABLE is depending on the Frame_End_1 or Frame_End_2 event.
                                    ///< TRUE is Frame_End_2 and FALSE is Frame_End_1. The Frame_End_2 should only be used
                                    ///< at Sensor HDR mode. The default value is FLASE (Frame_End_1).

    LVDS_CONFIG_ID_DP_ENABLE,       ///< Set Drop Pixel function enable/disable.
                                    ///< Please use ENABLE / DISABLE as input parameter.
    LVDS_CONFIG_ID_DP_PIXEL_LEN,    ///< Set Drop Pixel function horizontal pixel period.
                                    ///< Valid value range is from 1 to 64. Uint in pixels.
    LVDS_CONFIG_ID_DP_PIXEL_SEL0,   ///< Set Drop Pixel function horizontal pixel invalid select 0.
                                    ///< The invalid pixel bit mask for pixel period from first-pexel to pixel-32.
                                    ///< The bit-wise 1 means the specified order pixel period would be dropped. Otherwise, it would be reserved.
    LVDS_CONFIG_ID_DP_PIXEL_SEL1,   ///< Set Drop Pixel function horizontal pixel invalid select 1. This is valid only if LVDS_CONFIG_ID_DP_PIXEL_LEN larger than 32.
                                    ///< The invalid pixel bit mask for pixel period from pexel-33 to pixel-64.
                                    ///< The bit-wise 1 means the specified order pixel period would be dropped. Otherwise, it would be reserved.
    LVDS_CONFIG_ID_DP_LINE_LEN,     ///< Set Drop Pixel function vertical height period.
                                    ///< Valid value range is from 1 to 64. Uint in lines.
    LVDS_CONFIG_ID_DP_LINE_SEL0,    ///< Set Drop Pixel function vertical height invalid select 0.
                                    ///< The invalid line bit mask for vertical height period from first-line to line-32.
                                    ///< The bit-wise 1 means the specified order line period would be dropped. Otherwise, it would be reserved.
    LVDS_CONFIG_ID_DP_LINE_SEL1,    ///< Set Drop Pixel function vertical height invalid select 1. This is valid only if LVDS_CONFIG_ID_DP_LINE_LEN larger than 32.
                                    ///< The invalid line bit mask for vertical height period from line-33 to line-64.
                                    ///< The bit-wise 1 means the specified order line period would be dropped. Otherwise, it would be reserved.

    LVDS_CONFIG_ID_OUTORDER_0,      ///< Set the first pixel output from which data lane. Please use "LVDS_PIXEL_ORDER" as input parameter.
                                    ///< The pixel can be re-ordered by this selection. The mapping selection is mapped to the input pad pin location.
    LVDS_CONFIG_ID_OUTORDER_1,      ///< Set the second pixel output from which data lane. Please use "LVDS_PIXEL_ORDER" as input parameter.
                                    ///< The pixel can be re-ordered by this selection. The mapping selection is mapped to the input pad pin location.
    LVDS_CONFIG_ID_OUTORDER_2,      ///< Set the third pixel output from which data lane. Please use "LVDS_PIXEL_ORDER" as input parameter.
                                    ///< The pixel can be re-ordered by this selection. The mapping selection is mapped to the input pad pin location.
    LVDS_CONFIG_ID_OUTORDER_3,      ///< Set the forth pixel output from which data lane. Please use "LVDS_PIXEL_ORDER" as input parameter.
                                    ///< The pixel can be re-ordered by this selection. The mapping selection is mapped to the input pad pin location.
    LVDS_CONFIG_ID_OUTORDER_4,      ///< Set the fifth pixel output from which data lane. Please use "LVDS_PIXEL_ORDER" as input parameter.
                                    ///< The pixel can be re-ordered by this selection. The mapping selection is mapped to the input pad pin location.
    LVDS_CONFIG_ID_OUTORDER_5,      ///< Set the sixth pixel output from which data lane. Please use "LVDS_PIXEL_ORDER" as input parameter.
                                    ///< The pixel can be re-ordered by this selection. The mapping selection is mapped to the input pad pin location.
    LVDS_CONFIG_ID_OUTORDER_6,      ///< Set the seventh pixel output from which data lane. Please use "LVDS_PIXEL_ORDER" as input parameter.
                                    ///< The pixel can be re-ordered by this selection. The mapping selection is mapped to the input pad pin location.
    LVDS_CONFIG_ID_OUTORDER_7,      ///< Set the eighth pixel output from which data lane. Please use "LVDS_PIXEL_ORDER" as input parameter.
                                    ///< The pixel can be re-ordered by this selection. The mapping selection is mapped to the input pad pin location.
    LVDS_CONFIG_ID_OUTORDER_8,      ///< Set the ninth pixel output from which data lane. Please use "LVDS_PIXEL_ORDER" as input parameter.
                                    ///< The pixel can be re-ordered by this selection. The mapping selection is mapped to the input pad pin location.
    LVDS_CONFIG_ID_OUTORDER_9,      ///< Set the tenth pixel output from which data lane. Please use "LVDS_PIXEL_ORDER" as input parameter.
                                    ///< The pixel can be re-ordered by this selection. The mapping selection is mapped to the input pad pin location.

    LVDS_CONFIG_ID_VALID_LANE,      ///< Set input valid data lane location. Please use "LVDS_IN_VALID" as input parameter.
                                    ///< After the Data Lane number is decided by "LVDS_CONFIG_ID_DLANE_NUMBER",
                                    ///< This setting must be used to notify the controller the valid data lanes location from which of the input pad pins.
    LVDS_CONFIG_ID_FIFO_AUTO_RST,   ///< Enable/Disable the FIFO Auto Reset function at each frame end. Default is disabled.
    LVDS_CONFIG_ID_PATGEN_EN,       ///< Enable/Disable the LVDS Embedded Pattern Generator. Default is disabled.
    LVDS_CONFIG_ID_PATGEN_SEL,      ///< Pattern Generator Configuration value. bits[2:0] is PATGEN_MODE. bits[31:16] is PATGEN_VAL.

    /*
        Analog Block Control
    */
    LVDS_CONFIG_ID_A_DLY_ENABLE,    ///< Set LVDS Analog Clock/Data Lanes Delay function enable/disable.
    LVDS_CONFIG_ID_A_DLY_CLK0,      ///< Clock Lane 0 analog Delay Unit select. Valid value range from 0x0 to 0x7.
    LVDS_CONFIG_ID_A_DLY_CLK1,      ///< Clock Lane 1 analog Delay Unit select. Valid value range from 0x0 to 0x7.
    LVDS_CONFIG_ID_A_DLY_DAT0,      ///< Data Lane 0 analog Delay Unit select.  Valid value range from 0x0 to 0x7.
    LVDS_CONFIG_ID_A_DLY_DAT1,      ///< Data Lane 1 analog Delay Unit select.  Valid value range from 0x0 to 0x7.
    LVDS_CONFIG_ID_A_DLY_DAT2,      ///< Data Lane 2 analog Delay Unit select.  Valid value range from 0x0 to 0x7.
    LVDS_CONFIG_ID_A_DLY_DAT3,      ///< Data Lane 3 analog Delay Unit select.  Valid value range from 0x0 to 0x7.
    LVDS_CONFIG_ID_A_DLY_DAT4,      ///< Data Lane 4 analog Delay Unit select.  Valid value range from 0x0 to 0x7.
    LVDS_CONFIG_ID_A_DLY_DAT5,      ///< Data Lane 5 analog Delay Unit select.  Valid value range from 0x0 to 0x7.
    LVDS_CONFIG_ID_A_DLY_DAT6,      ///< Data Lane 6 analog Delay Unit select.  Valid value range from 0x0 to 0x7.
    LVDS_CONFIG_ID_A_DLY_DAT7,      ///< Data Lane 7 analog Delay Unit select.  Valid value range from 0x0 to 0x7.
    LVDS_CONFIG_ID_A_DLY_DAT8,      ///< Data Lane 8 analog Delay Unit select.  Valid value range from 0x0 to 0x7.
    LVDS_CONFIG_ID_A_DLY_DAT9,      ///< Data Lane 9 analog Delay Unit select.  Valid value range from 0x0 to 0x7.
    LVDS_CONFIG_ID_A_IADJ,          ///< Analog Block Current Source adjustment. Set 0x0 is 20uA (Default value). Set 0x1 is 40uA.
    LVDS_CONFIG_ID_A_SYNC_RST_EN,   ///< Analog Block clock lanes synchronous reset control. Default is reset enabled.


    LVDS_CONFIG_ID_CROPALIGN,       //   Reserved for novatek internal usage only.
    LVDS_CONFIG_ID_CROP_BITORDER,   //   Reserved for novatek internal usage only.
    ENUM_DUMMY4WORD(LVDS_CONFIG_ID)
}LVDS_CONFIG_ID;



/**
    LVDS Pixel Output Order Select

    This is used in lvds_setConfig(LVDS_CONFIG_ID_OUTORDER_X) to specify
    which of the data lane's pixel is selected to output in higher priority.
*/
typedef enum
{
    LVDS_PIXEL_ORDER_DL0,           ///< Data Lane 0 is selected.
    LVDS_PIXEL_ORDER_DL1,           ///< Data Lane 1 is selected.
    LVDS_PIXEL_ORDER_DL2,           ///< Data Lane 2 is selected.
    LVDS_PIXEL_ORDER_DL3,           ///< Data Lane 3 is selected.
    LVDS_PIXEL_ORDER_DL4,           ///< Data Lane 4 is selected.
    LVDS_PIXEL_ORDER_DL5,           ///< Data Lane 5 is selected.
    LVDS_PIXEL_ORDER_DL6,           ///< Data Lane 6 is selected.
    LVDS_PIXEL_ORDER_DL7,           ///< Data Lane 7 is selected.
    LVDS_PIXEL_ORDER_DL8,           ///< Data Lane 8 is selected.
    LVDS_PIXEL_ORDER_DL9,           ///< Data Lane 9 is selected.

    ENUM_DUMMY4WORD(LVDS_PIXEL_ORDER)
}LVDS_PIXEL_ORDER;

/**
    LVDS Input Valid Lane Location Select

    This is used in lvds_setConfig(LVDS_CONFIG_ID_VALID_LANE) to notify the controller
    which of the data lanes is valid from input pins.
*/
typedef enum
{
    LVDS_IN_VALID_D0         = 0x0001,  ///< Select HSI_D0.
    LVDS_IN_VALID_D1         = 0x0002,  ///< Select HSI_D1.
    LVDS_IN_VALID_D2         = 0x0004,  ///< Select HSI_D2.
    LVDS_IN_VALID_D3         = 0x0008,  ///< Select HSI_D3.
    LVDS_IN_VALID_D4         = 0x0010,  ///< Select HSI_D4.
    LVDS_IN_VALID_D5         = 0x0020,  ///< Select HSI_D5.
    LVDS_IN_VALID_D6         = 0x0040,  ///< Select HSI_D6.
    LVDS_IN_VALID_D7         = 0x0080,  ///< Select HSI_D7.
    LVDS_IN_VALID_D8         = 0x0100,  ///< Select HSI_D8.
    LVDS_IN_VALID_D9         = 0x0200,  ///< Select HSI_D9.

    LVDS_IN_VALID_SEQ1LANE   = 0x001,   ///< Select HSI_D0.
    LVDS_IN_VALID_SEQ2LANE   = 0x003,   ///< Select HSI_D0/HSI_D1.
    LVDS_IN_VALID_SEQ3LANE   = 0x007,   ///< Select HSI_D0/HSI_D1/HSI_D2.
    LVDS_IN_VALID_SEQ4LANE   = 0x00F,   ///< Select HSI_D0/HSI_D1/HSI_D2/HSI_D3.
    LVDS_IN_VALID_SEQ6LANE   = 0x03F,   ///< Select HSI_D0/HSI_D1/HSI_D2/HSI_D3/HSI_D4/HSI_D5.
    LVDS_IN_VALID_SEQ8LANE   = 0x0FF,   ///< Select HSI_D0/HSI_D1/HSI_D2/HSI_D3/HSI_D4/HSI_D5/HSI_D6/HSI_D7.
    LVDS_IN_VALID_SEQ10LANE  = 0x3FF,   ///< Select HSI_D0/HSI_D1/HSI_D2/HSI_D3/HSI_D4/HSI_D5/HSI_D6/HSI_D7/HSI_D8/HSI_D9.

    ENUM_DUMMY4WORD(LVDS_IN_VALID)
}LVDS_IN_VALID;


//
//  Prepare to obselete
//

/*
    LVDS Receive Bit Order per pixel for the pixel depth as LVDS_PIXDEPTH_CROP2LANE

    Reserved for novatek internal usage only.
*/
typedef enum
{
    LVDS_CROP2LANE_BIT_ORDER_MSB,  // Data Input Bit order is Most Significant Bit First.
    LVDS_CROP2LANE_BIT_ORDER_LSB,  // Data Input Bit order is Least Significant Bit First.

    ENUM_DUMMY4WORD(LVDS_CROP2LANE_BIT_ORDER)
}LVDS_CROP2LANE_BIT_ORDER;

/*
    LVDS Crop Align Selection for pixel depth as LVDS_PIXDEPTH_CROP2LANE

    Reserved for novatek internal usage only.
*/
typedef enum
{
    LVDS_CROP2LANE_BIT15_4,         // Crop two lanes 16bits' Bit[15:4].
    LVDS_CROP2LANE_BIT14_3,         // Crop two lanes 16bits' Bit[14:3].
    LVDS_CROP2LANE_BIT13_2,         // Crop two lanes 16bits' Bit[13:2].
    LVDS_CROP2LANE_BIT12_1,         // Crop two lanes 16bits' Bit[12:1].
    LVDS_CROP2LANE_BIT11_0,         // Crop two lanes 16bits' Bit[11:0].
    LVDS_CROP2LANE_BIT9_0,          // Crop two lanes 16bits' Bit[9:0]. (With 2 dummy bit 0 at the LSB)
    LVDS_CROP2LANE_BIT7_0,          // Crop two lanes 16bits' Bit[7:0]. (With 4 dummy bit 0 at the LSB)

    ENUM_DUMMY4WORD(LVDS_CROP2LANE_ALIGN)
}LVDS_CROP2LANE_ALIGN;




//
//  LVDS Exporting APIs
//
extern ER               lvds_open(void);
extern ER               lvds_close(void);
extern BOOL             lvds_isOpened(void);

extern ER               lvds_setEnable(BOOL bEn);
extern BOOL             lvds_getEnable(void);
extern ER               lvds_enableStreaming(void);

extern void             lvds_setConfig(LVDS_CONFIG_ID ConfigID, UINT32 uiCfgValue);
extern UINT32           lvds_getConfig(LVDS_CONFIG_ID ConfigID);

extern void             lvds_setChConfig(LVDS_DATLANE_ID ChID, LVDS_CH_CONFIG_ID ConfigID, UINT32 uiCfgValue);
extern UINT32           lvds_getChConfig(LVDS_DATLANE_ID ChID, LVDS_CH_CONFIG_ID ConfigID);

extern void             lvds_setSyncWord(UINT32 SyncWordLength, UINT32 *pSyncWord);
extern LVDS_INTERRUPT   lvds_waitInterrupt(LVDS_INTERRUPT WaitedFlag);


//
//  LVDS2 Exporting APIs
//
extern ER               lvds2_open(void);
extern ER               lvds2_close(void);
extern BOOL             lvds2_isOpened(void);

extern ER               lvds2_setEnable(BOOL bEn);
extern BOOL             lvds2_getEnable(void);
extern ER               lvds2_enableStreaming(void);

extern void             lvds2_setConfig(LVDS_CONFIG_ID ConfigID, UINT32 uiCfgValue);
extern UINT32           lvds2_getConfig(LVDS_CONFIG_ID ConfigID);

extern void             lvds2_setChConfig(LVDS_DATLANE_ID ChID, LVDS_CH_CONFIG_ID ConfigID, UINT32 uiCfgValue);
extern UINT32           lvds2_getChConfig(LVDS_DATLANE_ID ChID, LVDS_CH_CONFIG_ID ConfigID);

extern void             lvds2_setSyncWord(UINT32 SyncWordLength, UINT32 *pSyncWord);
extern LVDS_INTERRUPT   lvds2_waitInterrupt(LVDS_INTERRUPT WaitedFlag);



//
//  Debug usage only APIs
//
extern void             lvds_dumpDebugInfo(UINT32 uiClockLaneFrequency);
extern void             lvds2_dumpDebugInfo(UINT32 uiClockLaneFrequency);

//@}
#endif
