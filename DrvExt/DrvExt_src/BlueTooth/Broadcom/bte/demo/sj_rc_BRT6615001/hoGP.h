#pragma once

#include <string.h>
#include <stdio.h>
#include "wiced.h"
#include "wiced_bt_stack.h"
#include "wiced_bt_dev.h"
#include "wiced_bt_ble.h"
#include "wiced_bt_gatt.h"

#define SJ_LONG_LOCAL_NAME "SHIJUN"
#define MAX_HOGP_REPORT         5
#define MAX_HID_DESCRIPTER_LEN  255
#define MAX_SERVICE_NUM 20


typedef struct {
    uint16_t uuid;
    uint16_t shandle;
    uint16_t ehandle;
}handle_range_t;

typedef struct {
    uint8_t report_id;
    uint8_t report_type;
    uint16_t report_ccc_value;
    uint16_t report_ccc_handle;
}hogp_report_t;

typedef struct {
    handle_range_t hid_service[MAX_SERVICE_NUM];
    uint16_t handle_reportMap;
    hogp_report_t hogp_report[MAX_HOGP_REPORT];
    uint16_t  hid_desp_len;
    uint8_t   hid_descriptor[MAX_HID_DESCRIPTER_LEN];
}sj_rc_cb_t, *sj_rc_cb_p;

void sj_rc_init(sj_rc_cb_p p);