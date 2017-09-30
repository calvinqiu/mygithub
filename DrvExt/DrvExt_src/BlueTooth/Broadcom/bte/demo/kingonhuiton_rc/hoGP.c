#include "wiced_bt_cfg.h"

#if (DEMO_APP_TYPE == DEMO_APP_BLE_KINGONHUITON)
#include "hoGP.h"

void kingonhuiton_rc_init(kingonhuiton_rc_cb_p p)
{
    memset(p, 0, sizeof(kingonhuiton_rc_cb_t));
    p->hid_service.uuid = 0xFFF0;
}

#endif