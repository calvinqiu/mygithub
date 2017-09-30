#pragma once

#include <string.h>
#include <stdio.h>
#include "wiced.h"
#include "wiced_bt_stack.h"
#include "wiced_bt_dev.h"
#include "wiced_bt_ble.h"
#include "wiced_bt_gatt.h"

#define XIAOYI_SHORT_LOCAL_NAME "XiaoYi"
#define MAX_HOGP_REPORT         5
#define MAX_HID_DESCRIPTER_LEN  255


typedef struct {
    uint16_t uuid;
    uint16_t shandle;
    uint16_t ehandle;
}handle_range_t;

typedef struct {
    uint8_t report_id;
    uint8_t report_type;
    uint16_t report_handle;
    uint16_t report_ccc_value;
    uint16_t report_ccc_handle;
    uint16_t report_ref_handle;
}hogp_report_t;

typedef struct {
    handle_range_t hid_service;
    uint16_t handle_hidInfo;
    uint16_t handle_controlPoint;
    uint16_t handle_reportMap;
    uint16_t handle_protocolMode;
    uint16_t handle_bootKbInputReport;
    uint16_t handle_bootKboutputReport;
    hogp_report_t hogp_report[MAX_HOGP_REPORT];
    uint16_t  hid_desp_len;
    uint8_t   hid_descriptor[MAX_HID_DESCRIPTER_LEN];
}xiaoyi_rc_cb_t, *xiaoyi_rc_cb_p;

void xiaoyi_rc_init(xiaoyi_rc_cb_p p);