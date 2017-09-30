#include "PrjCfg.h"
#if (AUDIO_PLAY_FUNCTION == ENABLE)

#include "NetAudQue.h"

UINT32 NAUDQUE_SEM_ID = 0;

void NetAudQue_InstallID(void)
{
    OS_CONFIG_SEMPHORE(NAUDQUE_SEM_ID,0,1,1);
}

#endif

