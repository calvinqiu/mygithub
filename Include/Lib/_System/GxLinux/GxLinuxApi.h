/**
    Linux Utility

    for start and close Core - Linux

    @file       GxLinuxApi.h
    @ingroup    mGXLINUX

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _GXLINUXAPI_H
#define _GXLINUXAPI_H

#define GXLINUX_PRELOAD_ROOTFS_SIZE 0x40
#define GXLINUX_PRELOAD_UIMAGE_SIZE 0x40
#define GXLINUX_PRELOAD_UBOOT_SIZE  0x580

typedef enum _GXLINUX_UART
{
    GXLINUX_UART_1,
    GXLINUX_UART_2,
    ENUM_DUMMY4WORD(GXLINUX_UART)
}GXLINUX_UART;

typedef struct _GXLINUX_OPEN
{
    UINT32 uiAddr; ///< Linux start of memory region
    UINT32 uiSize; ///< Linux size of memory region
    GXLINUX_UART Uart; ///< the uart which linux uses to output and input
    char* ExtCmdLine; ///< kernel command line, it will append in startup kernel command line. set it NULL to disable it.
    BOOL bIgnoreKernelCheck; ///< if use u-boot to load linux f.w, set it ture to avoid checking linux-kernel vaild.
}GXLINUX_OPEN,*PGXLINUX_OPEN;

typedef struct _GXLINUX_UPD_UBI
{
    UINT32 uiAddr; ///< Linux start of memory region
    UINT32 uiSize; ///< Linux size of memory region
    UINT32 uiAddr_AllInOne; ///< addr of all in one bin
    UINT32 uiSize_AllInOne; ///< size of all in one bin
}GXLINUX_UPD_UBI, *PGXLINUX_UPD_UBI;

extern BOOL GxLinux_Init(void); ///< for install command
extern BOOL GxLinux_Open(GXLINUX_OPEN* pOpen);
extern BOOL GxLinux_Close(void);
extern BOOL GxLinux_Suspend(void);
extern BOOL GxLinux_Resume(void);
extern BOOL GxLinux_UpdUBI(GXLINUX_UPD_UBI* pUBI);

extern BOOL GxLinux_ChkCoreInfo(UINT32 LinuxMemPoolAddr, UINT32 LinuxMemPoolSize);

//for checking RootFs, the context have to load GXLINUX_PRELOAD_ROOTFS_SIZE as least
extern BOOL GxLinux_ChkValidRootFs(UINT32 uiFsAddr);
extern UINT32 GxLinux_GetRootFsBinSize(UINT32 uiFsAddr);
//for checking uImage, the context have to load GXLINUX_PRELOAD_UIMAGE_SIZE as least
extern BOOL GxLinux_ChkValiduImage(UINT32 uiuImageAddr);
extern UINT32 GxLinux_GetuImageBinSize(UINT32 uiuImageAddr);
//for checking uboot, the context have to load GXLINUX_PRELOAD_UBOOT_SIZE as least
extern BOOL GxLinux_ChkValiduBoot(UINT32 uiuBootAddr);
extern UINT32 GxLinux_GetuBootBinSize(UINT32 uiuBootAddr);
extern BOOL GxLinux_ChkCrc32_Linux(UINT32 uiAddr);

#endif