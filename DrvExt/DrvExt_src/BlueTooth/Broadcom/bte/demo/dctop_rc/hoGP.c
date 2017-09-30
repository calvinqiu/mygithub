/*
 * $ Copyright Broadcom Corporation $
 */

#include <string.h>
#include <stdio.h>
#include "wiced.h"
#include "wiced_bt_stack.h"
#include "wiced_bt_dev.h"
#include "wiced_bt_ble.h"
#include "wiced_bt_gatt.h"
#include "wiced_bt_cfg.h"
#include "sdpdefs.h"

#include "hoGP.h"
#include "hoGP.h"
#include "utron_utils.h"
#include "BT_BrcmInt.h"

/******************************************************
 *               Function Declarations
 ******************************************************/
static wiced_bt_dev_status_t bt_le_proximity_management_cback (wiced_bt_management_evt_t event, wiced_bt_management_evt_data_t *p_event_data);
wiced_bt_gatt_status_t bt_le_gatt_server_cback(wiced_bt_gatt_evt_t event, wiced_bt_gatt_event_data_t *p_event_data);
void bt_le_proximity_reporter_init(void);
wiced_bt_gatt_status_t bt_le_proximity_gatt_read_request(wiced_bt_gatt_read_t *p_read_request);
wiced_bt_gatt_status_t bt_le_proximity_gatt_write_request(wiced_bt_gatt_write_t *p_write_request);
wiced_bt_gatt_status_t bt_le_proximity_gatt_cback(wiced_bt_gatt_evt_t event, wiced_bt_gatt_event_data_t *p_event_data);
static void bt_le_init(void);

/******************************************************
 *               Variable Definitions
 ******************************************************/
UINT8 bt_bda[6] = { 0x43, 0x43, 0x80, 0x00, 0x00, 0x11};

static UINT8 *loadBdaFromNv(void)
{
    return bt_bda;
}

/******************************************************
 *               Function Definitions
 ******************************************************/
void application_start(void)
{
    /* Initialize WICED platform */
    /* Initialize Bluetooth controller and host stack */
    wiced_bt_stack_init(bt_le_proximity_management_cback, &wiced_bt_cfg_settings, wiced_bt_cfg_buf_pools);
}

void application_stop(void)
{
}

/* Bluetooth management event handler */
static wiced_bt_dev_status_t bt_le_proximity_management_cback (wiced_bt_management_evt_t event, wiced_bt_management_evt_data_t *p_event_data)
{
    wiced_bt_dev_status_t status = WICED_BT_SUCCESS;
    WPRINT_BT_APP_INFO(("bt_le_proximity_management_cback(%d)\n", event));

    switch (event)
    {
    case BTM_ENABLED_EVT:
        /* Bluetooth controller and host stack enabled */
        WPRINT_BT_APP_INFO(("BT Enable status: 0x%02x.\n", p_event_data->enabled.status));
        if (p_event_data->enabled.status == WICED_BT_SUCCESS)
        {
            wiced_bt_dev_write_local_addr(loadBdaFromNv(), 0);
            bt_le_init();
        }
        break;

    case BTM_BLE_SCAN_STATE_CHANGED_EVT:
        WPRINT_BT_APP_INFO(("ble_scan_state_changed is %d\n", p_event_data->ble_scan_state_changed));
        break;

    case BTM_SECURITY_REQUEST_EVT:
        wiced_bt_ble_security_grant(p_event_data->security_request.bd_addr, WICED_BT_SUCCESS);
        break;

    case BTM_PAIRING_IO_CAPABILITIES_BLE_REQUEST_EVT:
        p_event_data->pairing_io_capabilities_ble_request.local_io_cap = BTM_IO_CAPABILIES_NONE;/* No IO capabilities on this platform */
        p_event_data->pairing_io_capabilities_ble_request.auth_req = BTM_LE_AUTH_REQ_BOND;/* Bonding required */
        break;

    case BTM_PAIRING_COMPLETE_EVT:
        WPRINT_BT_APP_INFO(("Pairing complete %i.\n", p_event_data->pairing_complete.pairing_complete_info.ble.status));
        break;

    case BTM_USER_CONFIRMATION_REQUEST_EVT:
        /* User confirmation request for pairing (sample app always accepts) */
        wiced_bt_dev_confirm_req_reply(WICED_BT_SUCCESS, p_event_data->user_confirmation_request.bd_addr);
        break;

    case BTM_LOCAL_IDENTITY_KEYS_REQUEST_EVT:
        /* Request to restore local identity keys from NVRAM (requested during Bluetooth start up) */
        /* (sample app does not store keys to NVRAM. New local identity keys will be generated).   */
        status = WICED_BT_NO_RESOURCES;
        break;

    case BTM_LOCAL_IDENTITY_KEYS_UPDATE_EVT:
        /* Request to store newly generated local identity keys to NVRAM */
        /* (sample app does not store keys to NVRAM) */
        break;

    case BTM_BLE_ADVERT_STATE_CHANGED_EVT:
        /* adv state change callback */
        WPRINT_BT_APP_INFO(("---->>> New ADV state: %d\r\n", p_event_data->ble_advert_state_changed));
        break;

    default:
        WPRINT_BT_APP_INFO(("BT unhandled management event: 0x%02x.\r\n", event));
        break;
    }

    return (status);
}

/* GATT Read/Write operation completed */
static void bt_le_proximity_handle_operation_complete(wiced_bt_gatt_event_data_t *p_event_data)
{
    WPRINT_BT_APP_INFO(("bt_le_proximity_handle_operation_complete(%d,%d,%d)\n", 
    p_event_data->operation_complete.conn_id, p_event_data->operation_complete.op, p_event_data->operation_complete.status));
}

/* GATT event handler */
wiced_bt_gatt_status_t bt_le_proximity_gatt_cback(wiced_bt_gatt_evt_t event, wiced_bt_gatt_event_data_t *p_event_data)
{
    wiced_bt_gatt_status_t  status = WICED_BT_GATT_SUCCESS;
    uint8_t                 *bda;

    WPRINT_BT_APP_INFO(("bt_le_proximity_gatt_cback(%d)\n", event));

    switch (event)
    {
    case GATT_CONNECTION_STATUS_EVT:
        /* GATT connection status change */
        bda = p_event_data->connection_status.bd_addr;
        WPRINT_BT_APP_INFO(("GATT connection to [%02X:%02X:%02X:%02X:%02X:%02X] %s.\n",  bda[0], bda[1], bda[2], bda[3], bda[4], bda[5],
            (p_event_data->connection_status.connected ? "established" : "released/failed")));
        WPRINT_BT_APP_INFO(("role is %d\n", p_event_data->connection_status.link_role));

        if (p_event_data->connection_status.link_role == 1)
        {
            if (p_event_data->connection_status.connected)
                WPRINT_BT_APP_INFO(("connection_id is %d\r\n", p_event_data->connection_status.conn_id));
            bt_le_gatt_server_cback(event, p_event_data);
        }
        else
            WPRINT_BT_APP_INFO(("bt is configured to peripheral only\r\n"));
        break;

    case GATT_OPERATION_CPLT_EVT:
        /* Read/write operation complete */
        bt_le_proximity_handle_operation_complete(p_event_data);
        break;

    case GATT_ATTRIBUTE_REQUEST_EVT:
        /* GATT attribute read/write request */
        if (p_event_data->attribute_request.request_type == GATTS_REQ_TYPE_WRITE)
        {
            status = bt_le_proximity_gatt_write_request(&p_event_data->attribute_request.data.write_req);
        }
        else if (p_event_data->attribute_request.request_type == GATTS_REQ_TYPE_READ)
        {
            status = bt_le_proximity_gatt_read_request(&p_event_data->attribute_request.data.read_req);

        }
        break;

    default:
        WPRINT_BT_APP_INFO(("!!!Unhandled Event(%d)!!!\n", event));
        break;
    }

    return (status);
}

static void bt_le_init(void)
{
    wiced_bt_gatt_register(&bt_le_proximity_gatt_cback);
    /* enable proximity reporter*/
    bt_le_proximity_reporter_init();
}