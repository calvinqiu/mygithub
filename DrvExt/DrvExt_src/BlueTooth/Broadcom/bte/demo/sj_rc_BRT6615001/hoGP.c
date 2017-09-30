#include "wiced_bt_cfg.h"

#if (DEMO_APP_TYPE == DEMO_APP_BLE_SJ_BRT6615001)
#include "hoGP.h"

void sj_rc_init(sj_rc_cb_p p)
{
    memset(p, 0, sizeof(sj_rc_cb_t));
    //YongChang//p->hid_service.uuid = 0x1812;
	p->hid_service[0].uuid = 0xFFE0;
	//YongChang//p->hid_service.uuid = 0x1804;
}

#endif