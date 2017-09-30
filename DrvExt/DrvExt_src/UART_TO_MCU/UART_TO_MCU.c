/*
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       GPS.c
    @ingroup

    @brief      Excute command via UART interface.
                Excute command and output message via UART interface.

    @note       Nothing.

    @version    V1.00.000
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Debug.h"
#include "SysKer.h"
#include "kernel.h"
#include "uart.h"
#include "UART_TO_MCU.h"
#include "../../../Project/DemoKit/SrcCode/NVTUserCommand.h"
#include "../../../Include/Lib/DevStrg/FileSys/FileSysTsk.h"

//@{
#define THIS_DBGLVL         1       //0=OFF, 1=ERROR, 2=TRACE
#define __MODULE__          UART_TO_MCU
#define __DBGLVL__          THIS_DBGLVL
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
//#define __DBGFLT__          "[buf]"
#include "DebugModule.h"

#define _UART_TO_MCU_DEBUG_SYMBOL           0
#if _UART_TO_MCU_DEBUG_SYMBOL
#define UART_TO_MTK_debug(arg)              DBG_MSG arg
#else
#define UART_TO_MTK_debug(arg)
#endif

#define _UART_TO_MCU_NMEA_DEBUG_SYMBOL      0
#if _UART_TO_MCU_NMEA_DEBUG_SYMBOL
#define NMEA_debug(arg)             DBG_MSG arg
#else
#define NMEA_debug(arg)
#endif


//Declare functions
//void UART_TO_MTKRec_NMEAParser(CHAR *NMEA_str, UINT32 Len, NMEATYPE NMEAtype);

#define PRI_UART_TO_MCURECEIVE            10
#define STKSIZE_UART_TO_MCURECEIVE        6144//64KB stack size

UINT32  UART_TO_MCURECEIVE_ID = 0;
UINT32  FLG_ID_UART_TO_MCU = 0;

UINT32 MCUAddressStart = 0x00009000;//默认MCU升级初始地址
#define ONE_FRAME_BYTE_COUNTER   128
BOOL Uart_ACK_Flg = FALSE;
BOOL Is_MCU_UpData_OK_Flg = FALSE;


extern char gMCUVersion[2];

extern BOOL Is_validMCUVersion (void);
extern void UPdataMCUVersion(void);

//extern UIMenuStoreInfo  currentInfo;


extern  UINT8 McuFWBuf[];
extern UINT32 MCUuiFileSize;

BOOL UserUpgradeMCUFWTransmitOneFrame(UINT8*,UINT32);
BOOL UserUpgradeMCUFWOVEREEPROMDATA(UINT32 AddressStart,UINT8 data);

extern void UART_TO_MCURecTsk(void);



void UART_TO_MCU_InstallID(void)
{
    OS_CONFIG_TASK(UART_TO_MCURECEIVE_ID, PRI_UART_TO_MCURECEIVE, STKSIZE_UART_TO_MCURECEIVE, UART_TO_MCURecTsk);
    OS_CONFIG_FLAG(FLG_ID_UART_TO_MCU);
}

static BOOL bUART_TO_MCURecOpened = FALSE;
ER UART_TO_MCURec_Open(UART_BAUDRATE BaudRate)
{
    if (bUART_TO_MCURecOpened)
        return E_SYS;

    #if (_UART_TO_MCU_RECEIVE_FROM_ == RECEIVE_FROM_UART2)
    if (uart2_open() == E_OK)
    {
        DBG_DUMP("\r\nUART2: Open success\r\n");
        uart2_init(BaudRate, UART_LEN_L8_S1, UART_PARITY_NONE);
    }
    else
    {
        DBG_ERR("UART2: open fail!\r\n");
        return E_SYS;
    }
    #elif (_UART_TO_MCU_RECEIVE_FROM_ == RECEIVE_FROM_UART3)
    if (uart3_open() == E_OK)
    {
        DBG_DUMP("\r\nUART3: Open success\r\n");
        uart3_init(BaudRate, UART_LEN_L8_S1, UART_PARITY_NONE);
        debug_msg("uart3_init(BaudRate, UART_LEN_L8_S1, UART_PARITY_NONE)\r\n");
    }
    else
    {
        DBG_ERR("UART3: open fail!\r\n");
        return E_SYS;
    }
    #elif (_UART_TO_MCU_RECEIVE_FROM_ == RECEIVE_FROM_UART4)
    if (uart4_open() == E_OK)
    {
        DBG_DUMP("\r\nUART4: Open success\r\n");
        uart4_init(BaudRate, UART_LEN_L8_S1, UART_PARITY_NONE);
        debug_msg("uart4_init(BaudRate, UART_LEN_L8_S1, UART_PARITY_NONE)\r\n");
    }
    else
    {
        DBG_ERR("UART3: open fail!\r\n");
        return E_SYS;
    }
    #endif

    DBG_DUMP("Start UART_TO_MCU receive task\r\n");
    sta_tsk(UART_TO_MCURECEIVE_ID,0);

    loc_cpu();
    bUART_TO_MCURecOpened = TRUE;
    unl_cpu();

    return E_OK;
}

ER UART_TO_MCURec_Close(void)
{

    if (!bUART_TO_MCURecOpened)
        return E_SYS;

    #if (_UART_TO_MCU_RECEIVE_FROM_ == RECEIVE_FROM_UART2)
    if (uart2_close() == E_OK)
        DBG_DUMP("UART2: close success\r\n");
    else
        DBG_ERR("UART2: close fail!\r\n");
    #elif (_UART_TO_MCU_RECEIVE_FROM_ == RECEIVE_FROM_UART3)
    if (uart3_close() == E_OK)
        DBG_DUMP("UART3: close success\r\n");
    else
        DBG_ERR("UART3: close fail!\r\n");
    #elif (_UART_TO_MCU_RECEIVE_FROM_ == RECEIVE_FROM_UART4)
    if (uart4_close() == E_OK)
        DBG_DUMP("UART4: close success\r\n");
    else
        DBG_ERR("UART4: close fail!\r\n");
    #endif

    DBG_DUMP("Terminate MCU receive task\r\n");

    ter_tsk(UART_TO_MCURECEIVE_ID);

    loc_cpu();
    bUART_TO_MCURecOpened = FALSE;
    unl_cpu();

    return E_OK;
}

void UART_TO_MCURecTsk(void)
{
    UINT32  uiLen,i;
    CHAR    RecSentence[NMEA_SENTENCE_SIZE];
    static UINT32  ByteFrameCounter;
    static CHAR dataTemp;
    static UINT32 Step = 0;


    kent_tsk();
    clr_flg(FLG_ID_UART_TO_MCU, UART_TO_MCU_FLAG_DATAVALID);

//------------Jacky lan init IO pin------------
    gpio_setDir(GPIO_DVR_SYSTEM_OK, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_DVR_SYSTEM_OK);
    gpio_setDir(GPIO_MCU_RESET_IO, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_MCU_RESET_IO);
    gpio_setDir(GPIO_MCU_MCU_UART_EN, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_MCU_MCU_UART_EN);

    gpio_setDir(GPIO_RED_LED, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_RED_LED);
    gpio_setDir(GPIO_GREEN_LED, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_GREEN_LED);
    gpio_setDir(GPIO_BLUE_LED, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_BLUE_LED);
//--------------------------------------
    uart2_putChar(0xAA);    
    debug_msg("Get_MCU_Version CMD = 0xAA\r\n");
    while(1)
    {
        dataTemp=0;	
        uart2_getChar(&dataTemp);
        debug_msg("Get_MCU_RX_CMD = %X\r\n",dataTemp);
        if((dataTemp == MCU_ACK)||(dataTemp == 0xBB))
        {
            Get_MCU_Version();
            UPdataMCUVersion();	     
        }
        else if(dataTemp == ACK)
        {
            Step = 0;
            //uart2_getChar(&dataTemp);
            if(Step == 0)
            {
                if(dataTemp == ACK)
                {
                    debug_msg("ACK = %X\r\n",dataTemp);
                    Step = 1;
                }
                else
                {
                    Ux_PostEvent(NVTEVT_MCU_UPDATA_ERROR, 0,0);
                    debug_msg("MCU Uptada FW Fall !!!");
                    continue;
                }
            }
            else
            {
                Step = 0;
            }

            if(Step == 1)
            {
                uart2_putChar(0x00);
                uart2_putChar(0xff);
                debug_msg("uart4_putChar(0x00)\r\n");
                debug_msg("uart4_putChar(0xff)\r\n");
                uart2_getChar(&dataTemp);
                if(dataTemp == ACK)
                {
                    debug_msg("Go Cmd ACK = 0x%X\r\n",dataTemp);
                    for(i = 0; i < 7; i++)
                    {
                        uart2_getChar(RecSentence+i);
                        debug_msg("MCU Cmd = 0x%X\r\n",RecSentence[i]);
                    }
                    if(0x05==RecSentence[0])
                    {
                        if(0x10==RecSentence[1])
                        {
                            MCUAddressStart=0x00009000;
                        }
                        else
                        {
                            MCUAddressStart=(UINT32)(RecSentence[1]<<8);
                        }
                    }
                    else
                    {
                    }
                    Step = 2;
                }
                else
                {
                    Ux_PostEvent(NVTEVT_MCU_UPDATA_ERROR, 0,0);
                    debug_msg("MCU Uptada FW Fall !!!");
                    continue;
                }
            }
            if(Step == 2)
            {
                uart2_getChar(&dataTemp);
                if(dataTemp == ACK)
                {
                    debug_msg("Go Cmd ACK end = 0x%X\r\n",dataTemp);
                    ByteFrameCounter = (MCUuiFileSize % ONE_FRAME_BYTE_COUNTER == 0)?(MCUuiFileSize / ONE_FRAME_BYTE_COUNTER):((MCUuiFileSize / ONE_FRAME_BYTE_COUNTER) + 1);
                    debug_msg("MCUuiFileSize = %d\r\n",MCUuiFileSize);
                    debug_msg("ByteFrameCounter = %d\r\n",ByteFrameCounter);
                    debug_msg("MCU FW Updata Start... \r\n",ByteFrameCounter);
                    for(i = 0; i < ByteFrameCounter; i++)
                    {
                        UserUpgradeMCUFWTransmitOneFrame(McuFWBuf,i);
                    }
                    if(MCUAddressStart!=0x00009000)
                    {
                        UserUpgradeMCUFWOVEREEPROMDATA(0x4040,0x10); //该参数和MCU有关，详情产看MCU程序
                    }
                    debug_msg("^G" "========>> MCU WF UpData is Done!========>> \r\n");
                    Ux_PostEvent(NVTEVT_MCU_UPDATA_FINISH, 0,0);
                    Step = 0;
                }
                else
                {
                    Ux_PostEvent(NVTEVT_MCU_UPDATA_ERROR, 0,0);
                    debug_msg("MCU Uptada FW Fall !!!");
                    continue;
                }
            }
        }
        else
        {

        }
    }
}


BOOL UserUpgradeMCUFWTransmitOneFrame(UINT8 *SourceDat,UINT32 ByteFrameCounter)
{
    static UINT32 FrameCounter;
    // UINT32 AddressStart = 0x00009000;
    UINT32 AddressStart = MCUAddressStart;

    CHAR ByteCounter = ONE_FRAME_BYTE_COUNTER;
    UINT32 ByteChecksum = 0;
    UINT32 AddressChecksum = 0;
    UINT32 Index,i;
    static CHAR dataTemp;
    static UINT32 MCU_LED_Stat = 0;

    UINT8 Address4,Address3,Address2,Address1;
    AddressStart = AddressStart + (ByteFrameCounter * 0x80);
    Address4 = (UINT8)((AddressStart & 0xff000000) >> 24);
    Address3 = (UINT8)((AddressStart & 0x00ff0000) >> 16);
    Address2 = (UINT8)((AddressStart & 0x0000ff00) >> 8);
    Address1 = (UINT8)((AddressStart & 0x000000ff) >> 0);

    debug_msg("Address4 = %X Address3 = %X Address2 = %X Address1 = %X \r\n",Address4,Address3,Address2,Address1);

    uart2_putChar(0x31);
    uart2_putChar(0xce);

    debug_msg("uart4_putChar(0x31)\r\n");
    debug_msg("uart4_putChar(0xce)\r\n");
    uart2_getChar(&dataTemp);
    if(dataTemp == ACK)
    {
        debug_msg("WM Cmd ACK = 0x%X\r\n",dataTemp);

        uart2_putChar(Address4);
        uart2_putChar(Address3);
        uart2_putChar(Address2);
        uart2_putChar(Address1);

        AddressChecksum = Address4 ^ Address3 ^ Address2 ^ Address1;

        debug_msg("AddressChecksum = 0x%X\r\n",AddressChecksum);
        uart2_putChar(AddressChecksum);
        uart2_getChar(&dataTemp);
        if(dataTemp == ACK)
        {
            debug_msg("WM Cmd AddressChecksum ACK is OK = 0x%X\r\n",dataTemp);
            uart2_putChar(ByteCounter - 1);
            debug_msg("ByteCounter = %d\r\n",(ByteCounter - 1));
            Index = ByteFrameCounter * ONE_FRAME_BYTE_COUNTER;
            ByteChecksum = (ByteCounter - 1);
            for(i = 0; i < ByteCounter; i++)
            {
                if((MCUuiFileSize - 1) < (Index+i))
                {
                    McuFWBuf[Index+i] = 0x00;
                }
                uart2_putChar(McuFWBuf[Index+i]);
                //debug_msg("MCU FW Data = 0x%02X--%d\r\n",SourceDat[Index+i],(Index+i));

                ByteChecksum ^= SourceDat[Index+i];
            }
            uart2_putChar(ByteChecksum);
            debug_msg("ByteChecksum = %d\r\n",ByteChecksum);
            uart2_getChar(&dataTemp);
            if(dataTemp == ACK)
            {
                debug_msg("WM Cmd one ByteFrame ACK = 0x%X\r\n",dataTemp);

                if(MCU_LED_Stat == FALSE)
                {
                    MCU_Updata_LED_Set();
                    MCU_LED_Stat = TRUE;
                }
                else if(MCU_LED_Stat == TRUE)
                {
                    MCU_Updata_LED_Clear();
                    MCU_LED_Stat = FALSE;
                }
            }
        }
    }
    return 1;
}

BOOL UserUpgradeMCUFWOVEREEPROMDATA(UINT32 AddressStart,UINT8 Data)
{
    UINT32 AddressChecksum = 0;
    static CHAR dataTemp;
    UINT8 Address4,Address3,Address2,Address1;
    Address4 = (UINT8)((AddressStart & 0xff000000) >> 24);
    Address3 = (UINT8)((AddressStart & 0x00ff0000) >> 16);
    Address2 = (UINT8)((AddressStart & 0x0000ff00) >> 8);
    Address1 = (UINT8)((AddressStart & 0x000000ff) >> 0);
    debug_msg("Address4 = %X Address3 = %X Address2 = %X Address1 = %X \r\n",Address4,Address3,Address2,Address1);
    uart2_putChar(0x21);
    uart2_putChar(0xde);
    debug_msg("uart2_putChar(0x21)\r\n");
    debug_msg("uart2_putChar(0xde)\r\n");
    uart2_getChar(&dataTemp);
    if(dataTemp == ACK)
    {
        debug_msg("GO Cmd ACK = 0x%X\r\n",dataTemp);
        uart2_putChar(Address4);
        uart2_putChar(Address3);
        uart2_putChar(Address2);
        uart2_putChar(Address1);
        AddressChecksum = Address4 ^ Address3 ^ Address2 ^ Address1;
        uart2_putChar(AddressChecksum);
        debug_msg("AddressChecksum = 0x%X\r\n",AddressChecksum);
        dataTemp=0;
        uart2_getChar(&dataTemp);
        if(dataTemp == ACK)
        {
            debug_msg("WM Cmd AddressChecksum ACK is OK = 0x%X\r\n",dataTemp);
            uart2_putChar(Data);
            debug_msg("EEPROMdata=0x%X\r\n",Data);
            dataTemp=0;
            uart2_getChar(&dataTemp);
            if(dataTemp == ACK)
            {
                debug_msg("GO Cmd OVER ACK = 0x%X\r\n",dataTemp);
            }
        }
    }
    return 1;
}
UINT32 Get_MCU_Version (void)
{
    char MSBdata,LSMdata;
    UINT16 gMCUVersionNum;
    MCU_UART_Enabled();
    Delay_DelayMs(20);
    uart2_getChar(&MSBdata);
    uart2_getChar(&LSMdata);
    gMCUVersionNum = ((MSBdata << 8) | LSMdata);
    gMCUVersion[0] =  MSBdata;
    gMCUVersion[1] =  LSMdata;
    debug_msg("^RGet_MCU_Version = %04X\r\n",gMCUVersionNum);
}

BOOL ACK_Flg (void)
{
    return Uart_ACK_Flg;
}

BOOL Is_MCU_UpData_OK (void)
{
    return Is_MCU_UpData_OK_Flg;
}

//lacy add 2016-1-7 for updata MCU
void MCU_Updata_IO_Set (void)
{
    gpio_setPin(GPIO_DVR_SYSTEM_OK);
}

void MCU_Updata_IO_Clear (void)
{
    gpio_clearPin(GPIO_DVR_SYSTEM_OK);
}

void MCU_Reset_IO_Set (void)
{
    gpio_setPin(GPIO_MCU_RESET_IO);
}

void MCU_Reset_IO_Clear (void)
{
    gpio_clearPin(GPIO_MCU_RESET_IO);
}

void MCU_Updata_LED_Set (void)
{
    gpio_clearPin(GPIO_BLUE_LED);
    //gpio_clearPin(GPIO_RED_LED);
    //gpio_clearPin(GPIO_GREEN_LED);
}

void MCU_Updata_LED_Clear (void)
{
    gpio_setPin(GPIO_BLUE_LED);
    gpio_setPin(GPIO_GREEN_LED);
    gpio_setPin(GPIO_RED_LED);
}

void MCU_Updata_OK_LED (void)
{
    gpio_setPin(GPIO_BLUE_LED);
    gpio_clearPin(GPIO_GREEN_LED);
    gpio_setPin(GPIO_RED_LED);
}

void MCU_Updata_Fall_LED (void)
{
    gpio_setPin(GPIO_BLUE_LED);
    gpio_setPin(GPIO_GREEN_LED);
    gpio_clearPin(GPIO_RED_LED);
}

void MCU_UART_Enabled (void)
{
    gpio_setPin(GPIO_MCU_MCU_UART_EN);
}

void MCU_UART_Disable (void)
{
    gpio_clearPin(GPIO_MCU_MCU_UART_EN);
}

//lacy add end 2016-1-7 for updata MCU

MCU_STATUS McuUartGetStatus()
{
    //return gMcuStatus;
}

void McuUartSetStatus(MCU_STATUS status)
{
    //gMcuStatus=status;
}

