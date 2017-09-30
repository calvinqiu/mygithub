#ifndef _UCTRLAPP_CUSTOM_H
#define _UCTRLAPP_CUSTOM_H
#include "Type.h"

#define UCTRL_CUSTOM_MIN_BUFFER_SIZE	0x180000

typedef enum
{
	UCTRL_CUSTOM_BUFFER_ADDR, 	 	///< working buffer address
	UCTRL_CUSTOM_BUFFER_SIZE, 	 	///< working buffer size
    UCTRL_CUSTOM_SNAPSHOT_DONE,  	///< inform UCtrlAppCustom the result of snapshop
    UCTRL_CUSTOM_REC_FINISH,  		///< inform UCtrlAppCustom the recording is stopped
    ENUM_DUMMY4WORD(UCTRL_CUSTOM_CONFIG_ID)
} UCTRL_CUSTOM_CONFIG_ID;

extern void UCtrlAppCustom_InstallID(void) _SECTION(".kercfg_text");
extern void UCtrlAppCustom_InstallCmd(void);
extern void UCtrlAppCustom_SetData(UCTRL_CUSTOM_CONFIG_ID Config, UINT32 Value);
#endif //_UCTRLAPP_CUSTOM_H
