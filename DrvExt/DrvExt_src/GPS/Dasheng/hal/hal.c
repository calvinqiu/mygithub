#include "hal.h"

#define PATCH_FILE  "/home/android/SensorHub.patch"
#include "uart.h"
int UART_fd = -1;
int PATCH_FILE_fd = -1;
bool PatchFileOpened = false;

#include "stdio.h"
//#include "FileSysTsk.h"
//#include "fs_file_op.h"
#include "Delay.h"

//extern FST_FILE  filehdl;

#define THIS_DBGLVL         2       //0=OFF, 1=ERROR, 2=TRACE
#define __MODULE__          SampleDrv
#define __DBGLVL__          THIS_DBGLVL
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

void TurnAsicOn(int bOn)
{ 
    // Do something here to toggle nSTANDBY pin.
}

int OpenUART(void)
{
    int rc = 0;
#if 0
    struct termios termios;
    
    UART_fd = open(UART_NAME, O_RDWR | O_NOCTTY);
    
    if (UART_fd > 0)
    {
        tcflush(UART_fd, TCIOFLUSH);
        tcgetattr(UART_fd, &termios);
        cfmakeraw(&termios);
        termios.c_cflag &= ~(CSIZE | PARENB | PARODD | CSTOPB | CRTSCTS);
        termios.c_cflag |= CS8 | CRTSCTS| CREAD| CLOCAL;
        tcsetattr(UART_fd, TCSANOW, &termios);
        tcsetattr(UART_fd, TCSANOW, &termios);
        cfsetospeed(&termios, B921600);
        
        rc = 1;
    }    
#endif
    return rc;
}

void CloseUART(void)
{
    //close(UART_fd);
}

void SendAutoBaudCommand(void)
{
    unsigned char aucAutobaud[] = {0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80};

#if 0
    ssize_t written = write(UART_fd, aucAutobaud, sizeof(aucAutobaud));

    if (written <= 0)
    {
        printf("Send auto baud command failed!\n");
    }
#else
    uart4_write(aucAutobaud, 16);

#endif
}

int SendData(GlIntU8 *pucData, GlIntU32 usSize)
{
    int rc = 0;
#if 0
    if (UART_fd > 0)
    {
        rc = write(UART_fd, pucData, usSize);
    }
#endif
    //uart4_putString(pucData);
    rc =1;
    uart4_write(pucData, usSize);
    return rc;
}

int ReadData(GlIntU8 *pucData, GlIntU32 usSize)
{
#if 0
    int i;
    CHAR  data;
    GlIntU8 uartData[50];
    int counter=0;
    int j=0;
    UINT32 pcBufferLen;
    ER er;
#endif
    UINT32 rc = 0;
    CHAR pcstring[100];
    UINT32  uiLen;
    uiLen = sizeof(pcstring); 
#if 0
    if (UART_fd > 0)
    {
        rc = read(UART_fd, pucData, usSize);
    }
#endif

    rc = uart4_read((UINT8 *)pcstring, (UINT32)uiLen);

    memcpy(pucData,pcstring,rc);

    return rc;
}

void hal_usleep(GlIntU32 usInterval)
{
    Delay_DelayUs(usInterval);
}

int OpenPatchFile(void)
{
#if 0
    PATCH_FILE_fd = open(PATCH_FILE, O_RDONLY);
    
    if (PATCH_FILE_fd > 0)
    {
        PatchFileOpened = true;
    }
    
    return PATCH_FILE_fd;
#else
    PatchFileOpened = true;
    
    return 1;
#endif
}

void ClosePatchFile(void)
{
#if 0
    close(PATCH_FILE_fd);
    PatchFileOpened = false;
    PATCH_FILE_fd = -1;
#endif

    return;
}

int ReadPatchFile(GlIntU8 *ucbuffer, GlIntU32 iLen)
{
    int len = 0;
#if 0
    if (PATCH_FILE_fd > 0)
    {
        len = read(PATCH_FILE_fd, ucbuffer, iLen);
    }
    CHAR        cDataBuf[1024]={0};
    UINT32      uiReadSize = 20;

    UINT8* Buffer;
    UINT32 Length;
#endif

    len=1;
    return len;
}

bool IsPatchFileOpened(void)
{
    return PatchFileOpened;
}

void reset_hal(void)
{
    UART_fd = -1;
    PATCH_FILE_fd = -1;
    PatchFileOpened = false;
}
