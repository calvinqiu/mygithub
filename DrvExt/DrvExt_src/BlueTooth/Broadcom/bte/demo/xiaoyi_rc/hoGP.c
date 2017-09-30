#include "wiced_bt_cfg.h"

#if (DEMO_APP_TYPE == DEMO_APP_BLE_XIAOYI)
#include "hoGP.h"

void xiaoyi_rc_init(xiaoyi_rc_cb_p p)
{
    memset(p, 0, sizeof(xiaoyi_rc_cb_t));
    p->hid_service.uuid = 0x1812;
}

#endif