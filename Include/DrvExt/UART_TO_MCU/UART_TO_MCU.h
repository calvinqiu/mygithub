/**
    @file       GPSParser.h
    @ingroup

    @brief      Sample module brief
                Sample module description

    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#ifndef _UART_TO_MCU_H_
#define _UART_TO_MCU_H_

#include "Type.h"
#include "UART.h"
#include "Gpio.h"
#include "Pad.h"

#define RECEIVE_FROM_UART2        1
#define RECEIVE_FROM_UART3        2
#define RECEIVE_FROM_UART4        3

#define _UART_TO_MCU_RECEIVE_FROM_     RECEIVE_FROM_UART2


#define NMEA_SENTENCE_SIZE       180

#define UART_TO_MCU_FLAG_DATAVALID      0x00000001
#define UART_TO_MCU_FLAG_LOG            0x00000002

//MCU  CMD  #PW#2015-10-22#lacy add

#define UPDATE_FILE  	"A:\\MCU_PWU01.bin"
#define MCU_START_CHAR1				0xAA	
#define MCU_START_CHAR2				0x55
#define MCU_FRAME_LEN				0x03
#define MCU_FRAME_VERSION_CHAR		0x02
#define MCU_STOP_CHAR				0xCC
#define MCU_CMD						0x1F
#define MCM_VER_CMD					0xAF	
#define MCM_RESET_CMD				0xAC
//ACK, NACK, SYNCHR bytes
#define ACK             0x79				/* 	hex Value send for acknowledge */
#define NACK            0x1F				/*  hex Value send for non-acknowledge */
#define IDENT           0x7F				/*  hex Value rfequired for master identification */
#define MCU_ACK						0x2A
#define MCU_NCK						0x2B

//commands numbers
#define  GT_COM           0x00   /* Get command */
#define  RM_COM           0x11   /* Read Memory command */
#define  GO_COM           0x21   /* Go command */
#define  WM_COM           0x31   /* Write Memory command */
#define  EM_COM           0x43   /* Erase Memory command */

//MCU  CMD  #PW#2015-10-22#lacy end

typedef enum
{
    MCU_STATUS_IDLE = 0,
    MCU_STATUS_GETVERSION,
    MCU_STATUS_UPDATE
}MCU_STATUS;

extern void UART_TO_MCU_InstallID(void) _SECTION(".kercfg_text");
extern ER UART_TO_MCURec_Open(UART_BAUDRATE BaudRate);
extern ER UART_TO_MCURec_Close(void);
extern BOOL ACK_Flg (void);
extern BOOL Is_MCU_UpData_OK (void);
extern UINT32 Get_MCU_Version (void);

extern MCU_STATUS McuUartGetStatus();
extern void McuUartSetStatus(MCU_STATUS status);

extern UINT32 _SECTION(".kercfg_data") UART_TO_MCUCEIVE_ID; // UART_TO_MCU task
extern UINT32 _SECTION(".kercfg_data") FLG_ID_UART_TO_MCU;// UART_TO_MCU flag

//lacy add 2016-1-7 for updata MCU

#define GPIO_DVR_SYSTEM_OK				  P_GPIO_0	
#define PAD_GPIO_DVR_SYSTEM_OK			  PAD_PIN_PGPIO0
#define GPIO_MCU_RESET_IO				  P_GPIO_11	
#define PAD_GPIO_MCU_RESET_IO			  PAD_PIN_PGPIO11
#define GPIO_MCU_MCU_UART_EN			  P_GPIO_3
#define PAD_GPIO_MCU_UART_EN			  PAD_PIN_PGPIO3 
#define GPIO_MCU_UPDATA_LED				  P_GPIO_X	
#define PAD_GPIO_MCU_UPDATA_LED			  PAD_PIN_PGPIOX

#define GPIO_RED_LED                P_GPIO_33  //FPGA
#define PAD_RED_LED                 PAD_PIN_PGPIO33
#define GPIO_GREEN_LED              P_GPIO_34  //FPGA
#define PAD_GREEN_LED               PAD_PIN_PGPIO34
#define GPIO_BLUE_LED               C_GPIO_9 //FPGA
#define PAD_BLUE_LED                PAD_PIN_CGPIO9

extern void MCU_Updata_IO_Set (void);
extern void MCU_Updata_IO_Clear (void);
extern void MCU_Reset_IO_Set (void);
extern void MCU_Reset_IO_Clear (void);
extern void MCU_Updata_LED_Set (void);
extern void MCU_Updata_LED_Clear (void);
extern void MCU_Updata_OK_LED (void);
extern void MCU_Updata_Fall_LED (void);
extern void MCU_UART_Enabled (void);
extern void MCU_UART_Disable (void);


//lacy add end 2016-1-7 for updata MCU


#endif




