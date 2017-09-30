
#if 0

#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/termios.h>
#include <unistd.h>

#endif

#include "../inc/types.h"

#define UART_NAME    "/dev/ttyUSB1"

void TurnAsicOn(int bOn);
void SendAutoBaudCommand(void);
int  OpenUART(void);
void CloseUART(void);
int  SendData(GlIntU8 *pucData, GlIntU32 usSize);
int  ReadData(GlIntU8 *pucData, GlIntU32 usSize);
void hal_usleep(GlIntU32 usInterval);
int  OpenPatchFile(void);
void ClosePatchFile(void);
int  ReadPatchFile(GlIntU8 *ucbuffer, GlIntU32 iLen);
bool IsPatchFileOpened(void);
void reset_hal(void);