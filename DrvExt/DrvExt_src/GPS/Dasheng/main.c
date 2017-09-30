#include "./hal/hal.h"
#include "./transport/transport_layer.h"
#include "./patch_reader/patch_reader.h"
#include "./NvtDrvID.h"

#define DELAY_AFTER_NSTANDBY_HIGH               100000
#define AUTOBAUD_2_START_DOWNLOAD_TIMEOUT_MS    100000


#include "uart.h"

#define THIS_DBGLVL         2       //0=OFF, 1=ERROR, 2=TRACE
#define __MODULE__          SampleDrv
#define __DBGLVL__          THIS_DBGLVL
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
//#define uart4_putString   DBG_ERR

//int main() 
int dasheng_main(void)
{    
    TurnAsicOn(0);    
    TurnAsicOn(1);

    hal_usleep(DELAY_AFTER_NSTANDBY_HIGH);
    
    //if (!OpenUART())
    //{
    //    uart4_putString("Open UART failed.\n");
    //    return -1;
    //}

    // Send auto baud command
    DBG_ERR("Send auto baud command....");
    //uart_putString("Send auto baud command UART111111");
    SendAutoBaudCommand();
    DBG_ERR("[Success]\r\n");
    hal_usleep(AUTOBAUD_2_START_DOWNLOAD_TIMEOUT_MS);
    
    // Start remote sync
    DBG_ERR("Start remote sync....");
    if (!StartRemoteSync())
    {
        DBG_ERR("Failed!\n");
        
        return -1;
    }
    DBG_ERR("[Success]\r\n");
    
    DBG_ERR("Request version....");
    if (!RequestVersion())
    {
        DBG_ERR("Failed!\n");
       
        return -1;
    }
    DBG_ERR("[Success]\r\n");

    return 0;
}
