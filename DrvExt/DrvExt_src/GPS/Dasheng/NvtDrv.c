#include "Type.h"
#include "GPS.h"
#include "NvtDrvID.h"
#include "uart.h"
#define THIS_DBGLVL         2       //0=OFF, 1=ERROR, 2=TRACE
#define __MODULE__          DashengGPS
#define __DBGLVL__          THIS_DBGLVL
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

#include "hal.h"
#include "transport_layer.h"
#include "patch_reader.h"
#include "crc8bits.h"
#include "NvtStdio.h"
//#include "FileSysTsk.h"
//#include "fs_file_op.h"
#include "pad.h"//RTS & CTS
#include "Delay.h"

#define DELAY_AFTER_NSTANDBY_HIGH               100000
#define AUTOBAUD_2_START_DOWNLOAD_TIMEOUT_MS    100000

// flag define
#define FLG_DASHENG_START                FLGPTN_BIT(0)
#define FLG_DASHENG_STOP                 FLGPTN_BIT(1)
#define FLG_DASHENG_ACT                  FLGPTN_BIT(2)
#define FLG_DASHENG_EXIT                 FLGPTN_BIT(3)
#define FLG_DASHENG_IDLE                 FLGPTN_BIT(4)

#define BRCM_NEWLINE 0xb0
#define BRCM_UART_ENTER 0x01

static BOOL       bIsOpen    = FALSE;
static BOOL       Data_Start = FALSE;
static UINT32     GPSHeartBeat = 0;
static CHAR pcstring[512];
RMCINFO    RMCInfo;

ER GPSRec_Open(UART_BAUDRATE BaudRate)
{
    if (!SEM_ID_DASHENG)
    {
        DBG_ERR("ID not installed\r\n");
        return E_SYS;
    }

    // already opened
    if (bIsOpen)
        return E_OK;
    wai_sem(SEM_ID_DASHENG);

    clr_flg(FLG_ID_GPS, GPS_FLAG_DATAVALID);
    Data_Start = FALSE;
    GPSHeartBeat = 0;
    memset(&RMCInfo, 0, sizeof(RMCInfo));

    reset_hal();
    reset_patch();
    reset_transport();
    reset_crc();

    sta_tsk(GPSRECEIVE_ID, 0);

    bIsOpen = TRUE;
    sig_sem(SEM_ID_DASHENG);

    return E_OK;
}

ER GPSRec_Close(void)
{
    DBG_IND("\r\n");

    if (!bIsOpen)
        return E_OK;
    wai_sem(SEM_ID_DASHENG);

    sta_tsk(GPSRECEIVE_ID, 0);
    //uart_putChar('x');
    uart4_putChar('x');
    Data_Start=FALSE;

    //Patch_Stop();
    // terminate task
    ter_tsk(GPSRECEIVE_ID);
    uart4_close();

    bIsOpen = FALSE;
    sig_sem(SEM_ID_DASHENG);

    return E_OK;
}

void GPSRec_NMEAParser(CHAR *NMEA_str, UINT32 Len)
{
    char *rmc_head=NULL, *rmc_end=NULL, *pos;
    char buffer[512];
    int num;

    DBG_IND("\r\n%s\r\n", NMEA_str);

    rmc_head = strstr(NMEA_str, "$GPRMC,");
    if(!rmc_head)
        return;

    rmc_end = strstr(rmc_head + 1, "\n");
    if(!rmc_end)
        return;

    memcpy(buffer, rmc_head, rmc_end - rmc_head);

    pos = strstr(rmc_head, ",");
    if(!pos || pos >= rmc_end)
        return;

    pos = strstr(pos + 1, ",");
    if(!pos || pos >= rmc_end || (rmc_end - pos) < 2)
        return;
    else if(pos[1] != 'A'){
        clr_flg(FLG_ID_GPS, GPS_FLAG_DATAVALID);
        return;
    }

    pos = strstr(rmc_head, ",");
    if(!pos || pos >= rmc_end || (rmc_end - pos) < 2)
        return;
    else
        num = sscanf(pos, ",%02d%02d%02d,%c,%f,%c,%f,%c,%f,%f,%02d%02d%02d", 
            &RMCInfo.Hour, &RMCInfo.Minute, &RMCInfo.Second, &RMCInfo.Status,
            &RMCInfo.Latitude, &RMCInfo.NSInd, &RMCInfo.Longitude, &RMCInfo.EWInd,
            &RMCInfo.Speed, &RMCInfo.Angle, 
            &RMCInfo.Day, &RMCInfo.Month, &RMCInfo.Year);

    RMCInfo.Year += 1900;

    if(num == 13){
        DBG_IND("^Glatitude(%lf%c) ; longitude(%lf%c)\r\n", 
            RMCInfo.Latitude, RMCInfo.NSInd, RMCInfo.Longitude, RMCInfo.EWInd);
        set_flg(FLG_ID_GPS, GPS_FLAG_DATAVALID);
    }
    else
        DBG_WRN("sscanf() can't find 13 fields. Only %d fields are found\r\n", num);
}

void Brcm_Gps_Main_Tsk(void)
{
    kent_tsk();

    ////charlie test start uart4
    UART_INIT_PARA UartParaBasic;

    uart4_setConfig(UART_CONFIG_ID_ENDDET_CHAR_NO,2);     
    uart4_setConfig(UART_CONFIG_ID_ENDDET_MODE,UART_ENDDET_MODE_AND); 
    uart4_setConfig(UART_CONFIG_ID_ENDDET_CHAR0,BRCM_NEWLINE);        
    uart4_setConfig(UART_CONFIG_ID_ENDDET_CHAR1,BRCM_UART_ENTER); 
     
    if (uart4_open() != E_OK)
    {
        DBG_ERR("\r\nUART4: Open error\r\n");
        ext_tsk();
    }
    //Download patch file faster¡Aso use 1500000
    UartParaBasic.BaudRate = UART_BAUDRATE_1500000 ;// BaudRate 1500000 bps
    UartParaBasic.Length = UART_LEN_L8_S1; // Character 8bits ; Stop 1 bit
    UartParaBasic.Parity = UART_PARITY_NONE; // No Parity Bit
    UartParaBasic.FlowCtrlMode = UART_FC_MODE_HW_ON; // No Flow Control function
    UartParaBasic.DataTransMode = UART_DATA_TRANS_PIO;// Use PIO Transfer Mode
    uart4_initHW(&UartParaBasic);

    Delay_DelayMs(2000);
    if(dasheng_main())//start dasheng function
        ext_tsk();

#if 0
    ////YongChang test start uart
    if (uart_open() != E_OK)
    {
        DBG_ERR("\r\nUART1: Open error\r\n");

    }

    UartParaBasic.BaudRate = UART_BAUDRATE_115200 ;// BaudRate 115200 bps
    UartParaBasic.Length = UART_LEN_L8_S1; // Character 8bits ; Stop 1 bit
    UartParaBasic.Parity = UART_PARITY_NONE; // No Parity Bit
    UartParaBasic.FlowCtrlMode = UART_FC_MODE_HW_ON; // No Flow Control function
    UartParaBasic.DataTransMode = UART_DATA_TRANS_PIO;// Use PIO Transfer Mode
    uart_initHW(&UartParaBasic);

#endif

    DBG_IND("Initialize patch file....");

    if(!Patch_Initialize())
    {
        DBG_ERR("Failed!\n");
    
        return;
    }
    DBG_ERR("[Success]\r\n");
    
    DBG_ERR("Check patch file....");
    if(!CheckPatch())
    {
        DBG_ERR("Failed!\n");
       
        return;
    }
    DBG_ERR("[Success]\r\n");
    
    DBG_ERR("Start to download patch file............\r\n");
    
    if (!InitiatePatchDownload())
    {
        DBG_ERR("Download failed!\n");
    }
    else
    {
        DBG_ERR("[Success]\r\n");

        uart4_close();

        uart4_open();//Read NMEA only needs 115200
        UartParaBasic.BaudRate = UART_BAUDRATE_115200 ;// BaudRate 115200 bps
        UartParaBasic.Length = UART_LEN_L8_S1; // Character 8bits ; Stop 1 bit
        UartParaBasic.Parity = UART_PARITY_NONE; // No Parity Bit
        UartParaBasic.FlowCtrlMode = UART_FC_MODE_NONE; // No Flow Control function
        UartParaBasic.DataTransMode = UART_DATA_TRANS_PIO;// Use PIO Transfer Mode
        uart4_initHW(&UartParaBasic);
    }

    DBG_ERR("wait 5s before starting gps\r\n");
    Delay_DelayMs(2000);
    DBG_ERR("starting gps\r\n");
    //uart_putChar('p');
    uart4_putChar('p');
    Data_Start=TRUE;
    DBG_ERR("gps started\r\n");

    while(1){
        if(Data_Start==TRUE){

            UINT32  uiLen;
            uiLen = sizeof(pcstring);
            memset(pcstring,0,512);
            uart4_read((UINT8 *)pcstring, (UINT32)uiLen);

            #if 0
            er = uart4_getString(pcstring,&uiLen);
            uart_putString(pcstring);
            for(i=0;i<uiLen;i++){
                DBG_DUMP("%02x",pcstring[i]);
            }
            #else
            GPSHeartBeat++;
            GPSRec_NMEAParser(pcstring, uiLen);
            #endif
        }
        else{
            Delay_DelayMs(10000);
            DBG_ERR("Waiting!\r\n");
        }
    }

    #if 0
    clr_flg(FLG_ID_DASHENG, 0xFFFFFFFF);
    while(1)
    {

        wai_flg(&uiFlag, FLG_ID_DASHENG, FLG_DASHENG_STOP|FLG_DASHENG_ACT, TWF_ORW|TWF_CLR);
        DBG_IND("uiFlag=0x%x\r\n",uiFlag);

        if (uiFlag & FLG_DASHENG_STOP)
        {
            break;
        }
        if (uiFlag & FLG_DASHENG_ACT)
        {
            // do something
            wai_sem(SEM_ID_DASHENG);
            DBG_IND("do some action\r\n");
            sig_sem(SEM_ID_DASHENG);
        }
    }
    set_flg(FLG_ID_DASHENG, FLG_DASHENG_EXIT);
    #endif
    DBG_FUNC_END("\r\n");

    ext_tsk();
}

ER GPSRec_Reset(void)
{
    return E_OK;
}

BOOL GPSRec_CheckData(void)
{
    DBG_DUMP("GPS: DATA VALID: 0x%X\r\n", kchk_flg(FLG_ID_GPS, GPS_FLAG_DATAVALID));
    if (kchk_flg(FLG_ID_GPS, GPS_FLAG_DATAVALID))
        return TRUE;
    else
        return FALSE;

}

BOOL GPSRec_GetRMCDate(RMCINFO *pRMCInfo)
{
    memcpy(pRMCInfo,&RMCInfo,sizeof(RMCINFO));
    if (RMCInfo.Status == 'A')  //Data valid
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

FLOAT GPSRec_GetGPS_Speed(void)
{
    static FLOAT CurSpeed = 0;

    CurSpeed = RMCInfo.Speed * 1.852;

    return CurSpeed;
}

BOOL GPSRec_GetSpeed(FLOAT *Speed)
{
    static FLOAT CurSpeed = 0;

    CurSpeed = RMCInfo.Speed * 1.852;

    if (RMCInfo.Status == 'A')   //Data valid
    {
        *Speed = RMCInfo.Speed * 1.852;
        return TRUE;
    }
    else
    {
        *Speed = 0.0;
        return FALSE;
    }
}

UINT32 GetGPSHeartBeat(void)
{
    return GPSHeartBeat;
}

