#include "SysKer.h"
#include "BT_BrcmID.h"
#include "BT_API.h"
#include "utron_utils.h"


UINT32 SEM_ID_BT = 0;
UINT32 FLAG_ID_BT = 0;
void BT_InstallID(void)
{
    OS_CONFIG_SEMPHORE(SEM_ID_BT, 0, 1, 1);
    OS_CONFIG_FLAG(FLAG_ID_BT);
    bte_create_tasks();
}
