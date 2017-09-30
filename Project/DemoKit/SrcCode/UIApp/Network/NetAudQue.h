#ifndef _WSP_NETAUDPLAY_H_
#define _WSP_NETAUDPLAY_H_

#include "SysKer.h"
#include "WavStudioTsk.h"

extern void NetAudQue_InstallID(void) _SECTION(".kercfg_text");
extern UINT32 _SECTION(".kercfg_data") NAUDQUE_SEM_ID; ///< sem id

extern void NetAudQue_Start(UINT32 DevID, PWAVSTUD_AUD_INFO pCodec, UINT32 Addr, UINT32 Size);
extern void NetAudQue_Stop(UINT32 DevID);
extern BOOL NetAudQue_Push(UINT32 DevID, UINT32 Addr, UINT32 SampleCount);
extern BOOL NetAudQue_PullCB(PAUDIO_BUF_QUEUE pAudBufQue, UINT32 id);

#endif //_WSP_NETAUDPLAY_H_
