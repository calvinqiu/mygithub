/*
 * $ Copyright Broadcom Corporation $
 */

/** @file
 *
 * BLE Proximity Monitor Sample Application
 *
 * Features demonstrated
 *  - WICED BT GATT client APIs
 *
 * On startup this demo:
 *  - Initializes the Bluetooth sub system
 *  - Initiates Bluetooth LE connection to address specified by WICED_BT_CFG_PEER_BDA
 *  - On connection establishment:
 *      - Discover Link loss, Immediate Alert, and TX Power Level services on remote device
 *      - Get attributes handles for Link loss alert level, Immediate Alert alert level, and TX Power level
 *      - Read remote device's TX Power level (if remote supports TX Power Level service)
 *      - Write to remote devices's Link Loss alert level attribute
 *      - If remote device supports TX Power level, then start a thread periodically monitor link's signal strength (rssi).
 *           if rssi drops below threshold, then send an Immediate Alert write request.
 *
 * Application Instructions
 *   Connect a PC terminal to the serial port of the WICED Eval board,
 *   then build and download the application as described in the WICED
 *   Quick Start Guide
 *
 */

#include <string.h>
#include <stdio.h>
#include "SwTimer.h"
#include "wiced.h"
#include "wiced_bt_stack.h"
#include "wiced_bt_dev.h"
#include "wiced_bt_ble.h"
#include "wiced_bt_gatt.h"
#include "wiced_bt_cfg.h"
#include "sdpdefs.h"

#include "xiaoyi_HoGP.h"
#include "hoGP.h"
#include "utron_utils.h"
#include "BT_BrcmInt.h"

//#NT#2015/08/27#Lincy Lin -begin
//#NT#if enable file server
#define RFC_SERVER_ENABLE                   TRUE
//#NT#2015/08/27#Lincy Lin -end

#if (DEMO_APP_TYPE == DEMO_APP_BLE_XIAOYI)
/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/
#define SCAN_FOR_PEER                       TRUE        /* If TRUE, scan for peer instead of connecting to WICED_BT_CFG_PEER_BDA */
#define SCAN_REMOTE_NAME_MAXLEN             32          /* Maximum length stored for remote device names */

/******************************************************
 *                   Enumerations
 ******************************************************/

/* Proximity montitor states */
typedef enum
{
    HOGP_HOST_STATE_IDLE,
    HOGP_HOST_STATE_DISCOVERING_SERVICES,
    HOGP_HOST_STATE_DISCOVERING_CHARACTERS,
    HOGP_HOST_STATE_DISCOVERING_DESCRIPTERS,
    HOGP_HOST_STATE_DISCOVERING_REPORT_DESP_ATTRIBUTE,
    HOGP_HOST_STATE_DISCOVERING_REPORT_REF_ATTRIBUTE,
    HOGP_HOST_STATE_DISCOVERING_REPORT_ATTRIBUTE,
    HOGP_HOST_STATE_ENABLE_REPORT,
    HOGP_HOST_STATE_WORKING
} proximity_monitor_state_t;

/* Enumeration of services application is interested in */


/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *                    Structures
 ******************************************************/
typedef struct
{
    /* Handles for link loss service */
    uint16_t start_handle;              /* Start handle of service */
    uint16_t end_handle;                /* End handle of service */
    uint16_t attribute_value_handle;    /* Either handle to Alert Level or TX Power attribute value, depending on service */
} proximity_service_t, hid_service_t;

typedef void(*wiced_thread_function_t)(void* arg);


/******************************************************
 *               Function Declarations
 ******************************************************/
static wiced_bt_dev_status_t bt_le_proximity_management_cback (wiced_bt_management_evt_t event, wiced_bt_management_evt_data_t *p_event_data);
static void bt_le_proximity_monitor_init(void);
//static void bt_le_rssi_monitor_task (uint32_t arg);
static void bt_le_proximity_start_discovery(void);
static void scan_result_cback(wiced_bt_ble_scan_results_t *p_scan_result, uint8_t *p_adv_data);

void set_BD(void);
void bt_rfcomm_server_init(void);
void bt_le_proximity_reporter_init(void);
wiced_bt_gatt_status_t bt_le_gatt_server_cback(wiced_bt_gatt_evt_t event, wiced_bt_gatt_event_data_t *p_event_data);
wiced_bt_gatt_status_t bt_le_proximity_gatt_read_request(wiced_bt_gatt_read_t *p_read_request);
wiced_bt_gatt_status_t bt_le_proximity_gatt_write_request(wiced_bt_gatt_write_t *p_write_request);
wiced_bt_gatt_status_t bt_le_proximity_gatt_cback(wiced_bt_gatt_evt_t event, wiced_bt_gatt_event_data_t *p_event_data);
static void bt_le_init(void);
static void delete_one_old_bond_info(wiced_bt_device_address_t bda);
void display_bda(unsigned char * bda);

/******************************************************
 *               Variable Definitions
 ******************************************************/
proximity_monitor_state_t   proximity_monitor_state = HOGP_HOST_STATE_IDLE;
uint16_t                    connection_id;
wiced_bool_t                connection_initiated = FALSE;
wiced_bool_t                bonding_required = FALSE;
wiced_bt_device_address_t   peer_address;
wiced_bt_ble_address_type_t peer_address_type;

xiaoyi_rc_cb_t xiaoyi_rc_cb;

typedef struct
{
    UINT16  opcode;
    UINT16  param_len;
    UINT8   *p_param_buf;
} tBTM_VSC_CMPL;

typedef void (tBTM_VSC_CMPL_CB)(tBTM_VSC_CMPL *p1);

int BTM_VendorSpecificCommand(UINT16 opcode, UINT8 param_len,
    UINT8 *p_param_buf, tBTM_VSC_CMPL_CB *p_cb);

#if 0 //SPP_ENABLE

void set_address_cb(tBTM_VSC_CMPL *p1)
{
    /* Initialize the RFCOMM server */
    bt_rfcomm_server_init();

    /* Enable proximity monitor */
    bt_le_proximity_monitor_init();

    /* enable proximity reporter*/
    //bt_le_proximity_reporter_init();
}

void set_BD(void)
{
    unsigned char bd[] = { 0x01, 0x01, 0x01, 0x8A, 0x43, 0x43 };
    BTM_VendorSpecificCommand(0xFC01, 6, bd, set_address_cb);
}
#endif

UINT8 bt_bda[6] = { 0x43, 0x43, 0x80, 0x00, 0x00, 0x11};

static UINT8 *loadBdaFromNv(void)
{
    return bt_bda;
}
// this will let 43438 to set uart GPIO to low
void set_uart_low(void)
{
    UINT8 param[] = { 0x04, 0x10, 0x00, 0xFF, 0x01, 0x00, 0x00, 0x00 };
    BTM_VendorSpecificCommand(0xFC4C, 8, param, 0);
}

// remember RC device, in the next boot, system need to know
// if it is RC device which is paired before.
#define MAX_RC_COUNT 5
typedef struct{
    uint32_t count;
    wiced_bt_device_address_t rclist[MAX_RC_COUNT];
}rc_store_t;
static rc_store_t app_rc_store;
/*
 * load rc address from NV
*/
static void load_stored_rc_info(void)
{
    memset(&app_rc_store, 0, sizeof(app_rc_store));
    app_rc_store.count = 1;
    app_rc_store.rclist[0][0] = 0x04;
    app_rc_store.rclist[0][1] = 0xe6;
    app_rc_store.rclist[0][2] = 0x76;
    app_rc_store.rclist[0][3] = 0xab;
    app_rc_store.rclist[0][4] = 0x75;
    app_rc_store.rclist[0][5] = 0xa6;
}

static int find_rc_record(wiced_bt_device_address_t bda)
{
    uint32_t i = 0;
    for (i = 0; i < app_rc_store.count; i++)
    {
        if (!memcmp(bda, app_rc_store.rclist[i], 6))
        {
            // found one recored
            return 1;
        }
    }

    // not a rc device
    return 0;
}

/*
* load rc address to NV
*/
static void save_rc_bda(wiced_bt_device_address_t bda)
{
    if (!find_rc_record(bda))
    {
        memcpy(app_rc_store.rclist[app_rc_store.count], bda, 6);
        app_rc_store.count++;
    }
}

static int connect_to_last_rc(void)
{
    wiced_bt_dev_bonded_device_info_t paired_device_addr[WICED_BT_DCT_MAX_DEVICES];
    uint16_t num_devices;

    /* If there is a previously stored device, then connect to it */
    num_devices = WICED_BT_DCT_MAX_DEVICES;
    int i;
    BOOLEAN found = FALSE;
    if ((wiced_bt_dev_get_bonded_devices(paired_device_addr, &num_devices) == WICED_BT_SUCCESS) && (num_devices > 0))
    {
        for (i = 0; i < num_devices; i++)
        {
            if (find_rc_record(paired_device_addr[i].bd_addr))
            {
                found = TRUE;
                break;
            }
        }
        if (found)
        {
            wiced_bt_gatt_le_connect(paired_device_addr[i].bd_addr, paired_device_addr[i].addr_type, BLE_CONN_MODE_HIGH_DUTY, TRUE);
            connection_initiated = TRUE;
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        return FALSE;
    }
}

/******************************************************
 *               Function Definitions
 ******************************************************/
void application_start(void)
{
    /* Initialize WICED platform */
    //wiced_init( );

    xiaoyi_rc_init(&xiaoyi_rc_cb);
    load_stored_rc_info();
    /* Initialize Bluetooth controller and host stack */
    wiced_bt_stack_init(bt_le_proximity_management_cback, &wiced_bt_cfg_settings, wiced_bt_cfg_buf_pools);
}

void application_stop(void)
{
    wiced_bt_ble_scan(BTM_BLE_SCAN_TYPE_NONE, FALSE, scan_result_cback);
    //SwTimer_DelayMs(50);

}

/* Bluetooth management event handler */
static wiced_bt_dev_status_t bt_le_proximity_management_cback (wiced_bt_management_evt_t event, wiced_bt_management_evt_data_t *p_event_data)
{
    wiced_bt_dev_status_t status = WICED_BT_SUCCESS;

    switch (event)
    {
    case BTM_ENABLED_EVT:
        /* Bluetooth controller and host stack enabled */
        WPRINT_BT_APP_INFO(("BT Enable status: 0x%02x.\n", p_event_data->enabled.status));
        if (p_event_data->enabled.status == WICED_BT_SUCCESS)
        {
            wiced_bt_dev_write_local_addr(loadBdaFromNv(), 0);

            wiced_bt_dev_set_discoverability (BTM_GENERAL_DISCOVERABLE, 0, 0);
#ifdef SPP_ENABLE
            uint8_t mfi_eir[240] = {0, 5, 0x08, 'x', 'B', 'L', 'E',
            17, 0x06, 0x00, 0x00, 0x00, 0x00, 0xDE, 0xCA, 0xFA, 0xDE, 0xDE, 0xCA, 0xDE, 0xAF, 0xDE, 0xCA, 0xCA, 0xFF};
            wiced_bt_dev_write_eir(mfi_eir, sizeof(mfi_eir));

            bt_rfcomm_server_init();
#endif
            bt_le_init();
        }
        break;

    case BTM_BLE_SCAN_STATE_CHANGED_EVT:
        {
            if (p_event_data->ble_scan_state_changed == BTM_BLE_SCAN_TYPE_NONE)
            {
                // set scan off and uart low, ready to power off
                BT_BLE_SetScanOff();
                set_uart_low();
            }
        }
        break;

    case BTM_SECURITY_REQUEST_EVT:
        WPRINT_BT_APP_INFO(("Security reqeust\r\n"));
        wiced_bt_ble_security_grant(p_event_data->security_request.bd_addr, WICED_BT_SUCCESS);
        break;

    case BTM_PAIRING_IO_CAPABILITIES_BLE_REQUEST_EVT:
        WPRINT_BT_APP_INFO(("Pairing IO Capabilities reqeust\n"));
        p_event_data->pairing_io_capabilities_ble_request.local_io_cap = BTM_IO_CAPABILIES_NONE;        /* No IO capabilities on this platform */
        p_event_data->pairing_io_capabilities_ble_request.auth_req = BTM_LE_AUTH_REQ_BOND;              /* Bonding required */
        break;

    case BTM_PAIRING_COMPLETE_EVT:
        WPRINT_BT_APP_INFO(("Pairing complete %i.\n", p_event_data->pairing_complete.pairing_complete_info.ble.status));

        if (p_event_data->pairing_complete.pairing_complete_info.ble.status == WICED_BT_SUCCESS)
        {
            bt_le_proximity_start_discovery ();
        }
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
        if (BTM_BLE_ADVERT_OFF == p_event_data->ble_advert_state_changed)
        {
            //wiced_bt_start_advertisements(BTM_BLE_ADVERT_UNDIRECTED_LOW, 0, NULL);
        }


    default:
        WPRINT_BT_APP_INFO(("BT unhandled management event: 0x%02x.\r\n", event));
        break;
    }

    return (status);
}

static BOOLEAN bt_le_enable_hogp_report(void)
{
    uint8_t buf[16];
    wiced_bt_gatt_value_t *write_value = (wiced_bt_gatt_value_t *)buf;
    uint8_t i = 0;
    BOOLEAN result = FALSE;
    uint16_t hid_ccc_handle = 0;

    for (i = 0; i < MAX_HOGP_REPORT; i++)
    {
        if ((xiaoyi_rc_cb.hogp_report[i].report_ccc_value != 0x01) && (xiaoyi_rc_cb.hogp_report[i].report_ccc_handle != 0))
        {
            result = TRUE;
            hid_ccc_handle = xiaoyi_rc_cb.hogp_report[i].report_ccc_handle;
            xiaoyi_rc_cb.hogp_report[i].report_ccc_value = 0x01;
            break;
        }
    }

    if (result)
    {
        proximity_monitor_state = HOGP_HOST_STATE_ENABLE_REPORT;

        write_value->handle = hid_ccc_handle;
        write_value->len = 2;
        write_value->value[0] = 0x01;
        write_value->value[1] = 0;
        write_value->auth_req = GATT_AUTH_REQ_NONE;
        wiced_bt_gatt_send_write(connection_id, GATT_WRITE, write_value);
    }

    return result;
}


static void bt_le_disable_hogp_report(void)
{
    uint8_t i = 0;
    //BOOLEAN result = FALSE;
    //uint16_t hid_ccc_handle = 0;

    for (i = 0; i < MAX_HOGP_REPORT; i++)
    {
        if ((xiaoyi_rc_cb.hogp_report[i].report_ccc_value != 0) && (xiaoyi_rc_cb.hogp_report[i].report_ccc_handle != 0))
        {
            xiaoyi_rc_cb.hogp_report[i].report_ccc_value = 0;
        }
    }
}

/* Read the remote proximity reporter's power level */
static BOOLEAN bt_le_read_remote_report_reference(void)
{
    wiced_bt_gatt_read_param_t read_param;
    uint8_t i = 0;
    BOOLEAN result = FALSE;
    uint16_t hid_ref_handle = 0;

    for (i = 0; i < MAX_HOGP_REPORT; i++)
    {
        if ((xiaoyi_rc_cb.hogp_report[i].report_type == 0) && (xiaoyi_rc_cb.hogp_report[i].report_ref_handle != 0))
        {
            result = TRUE;
            hid_ref_handle = xiaoyi_rc_cb.hogp_report[i].report_ref_handle;
            break;
        }
    }

    if (result)
    {
        proximity_monitor_state = HOGP_HOST_STATE_DISCOVERING_REPORT_REF_ATTRIBUTE;

        /* Send GATT read request for TX Power Level attribute value */
        read_param.by_handle.handle = hid_ref_handle;
        read_param.by_handle.auth_req = GATT_AUTH_REQ_NONE;
        wiced_bt_gatt_send_read(connection_id, GATT_READ_BY_HANDLE, &read_param);
    }

    return result;
}




/* Handle GATT discovery result */
static void bt_le_proximity_handle_discovery_result(wiced_bt_gatt_event_data_t *p_event_data)
{
    uint16_t service_uuid;

    /* Handle service discovery result */
    if (proximity_monitor_state == HOGP_HOST_STATE_DISCOVERING_SERVICES)
    {
        if (GATT_DISCOVERY_RESULT_SERVICE_UUID_LEN(p_event_data) == LEN_UUID_16)
        {
            /* We are interested in Link loss, Immediate alert, and TX Power services */
            service_uuid = GATT_DISCOVERY_RESULT_SERVICE_UUID16(p_event_data);

            WPRINT_BT_APP_INFO(("Discovered service (0x%04x) handle range: [%04x - %04x]\n", service_uuid, GATT_DISCOVERY_RESULT_SERVICE_START_HANDLE(p_event_data), GATT_DISCOVERY_RESULT_SERVICE_END_HANDLE(p_event_data)));

            /* If desired service was discovered, store the service's start and end attibute handles */
            if (service_uuid == xiaoyi_rc_cb.hid_service.uuid)
            {
                WPRINT_BT_APP_INFO(("HID service found\r\n"));
                xiaoyi_rc_cb.hid_service.shandle = GATT_DISCOVERY_RESULT_SERVICE_START_HANDLE(p_event_data);
                xiaoyi_rc_cb.hid_service.ehandle = GATT_DISCOVERY_RESULT_SERVICE_END_HANDLE(p_event_data);
            }
        }
    }
    /* Handle Immediate Alert Level attribute discovery result */
    else if (proximity_monitor_state == HOGP_HOST_STATE_DISCOVERING_DESCRIPTERS)
    {


        /* If discovered Immediate Alert service's Alert Level attribute, then store its handle */
        if (GATT_DISCOVERY_RESULT_CHARACTERISTIC_DESCRIPTOR_UUID_LEN(p_event_data) == LEN_UUID_16)
        {
            switch (GATT_DISCOVERY_RESULT_CHARACTERISTIC_DESCRIPTOR_UUID16(p_event_data))
            {
            case GATT_UUID_HID_INFORMATION:
                WPRINT_BT_APP_INFO(("GATT_UUID_HID_INFORMATION Handle :(0x%04x)\n", GATT_DISCOVERY_RESULT_CHARACTERISTIC_DESCRIPTOR_VALUE_HANDLE(p_event_data)));
                xiaoyi_rc_cb.handle_hidInfo = GATT_DISCOVERY_RESULT_CHARACTERISTIC_DESCRIPTOR_VALUE_HANDLE(p_event_data);
                break;

            case GATT_UUID_HID_REPORT_MAP:
                WPRINT_BT_APP_INFO(("GATT_UUID_HID_REPORT_MAP Handle :(0x%04x)\n", GATT_DISCOVERY_RESULT_CHARACTERISTIC_DESCRIPTOR_VALUE_HANDLE(p_event_data)));
                xiaoyi_rc_cb.handle_reportMap = GATT_DISCOVERY_RESULT_CHARACTERISTIC_DESCRIPTOR_VALUE_HANDLE(p_event_data);
                break;

            case GATT_UUID_HID_CONTROL_POINT:
                WPRINT_BT_APP_INFO(("GATT_UUID_HID_CONTROL_POINT Handle :(0x%04x)\n", GATT_DISCOVERY_RESULT_CHARACTERISTIC_DESCRIPTOR_VALUE_HANDLE(p_event_data)));
                xiaoyi_rc_cb.handle_controlPoint = GATT_DISCOVERY_RESULT_CHARACTERISTIC_DESCRIPTOR_VALUE_HANDLE(p_event_data);
                break;

            case GATT_UUID_HID_REPORT:
            {
                uint8_t i = 0;

                WPRINT_BT_APP_INFO(("GATT_UUID_HID_REPORT Handle :(0x%04x)\n", GATT_DISCOVERY_RESULT_CHARACTERISTIC_DESCRIPTOR_VALUE_HANDLE(p_event_data)));

                for (i = 0; i < MAX_HOGP_REPORT; i++)
                {
                    if (xiaoyi_rc_cb.hogp_report[i].report_handle == 0)
                    {
                        xiaoyi_rc_cb.hogp_report[i].report_handle = GATT_DISCOVERY_RESULT_CHARACTERISTIC_DESCRIPTOR_VALUE_HANDLE(p_event_data);
                        break;
                    }
                }

            }
            break;

            case GATT_UUID_CHAR_CLIENT_CONFIG:
            {
                uint8_t i = 0;
                uint16_t attr_hdl = GATT_DISCOVERY_RESULT_CHARACTERISTIC_DESCRIPTOR_VALUE_HANDLE(p_event_data);

                WPRINT_BT_APP_INFO(("GATT_UUID_HID_CCC_REPORT Handle :(0x%04x)\n", attr_hdl));

                for (i = 0; i < MAX_HOGP_REPORT; i++)
                {
                    if ((xiaoyi_rc_cb.hogp_report[i].report_handle != 0) && (attr_hdl <= xiaoyi_rc_cb.hogp_report[i].report_handle + 2) && (attr_hdl > xiaoyi_rc_cb.hogp_report[i].report_handle))
                    {
                        xiaoyi_rc_cb.hogp_report[i].report_ccc_handle = attr_hdl;
                    }
                }
            }
            break;

            case GATT_UUID_RPT_REF_DESCR:
            {
                uint8_t i = 0;
                uint16_t attr_hdl = GATT_DISCOVERY_RESULT_CHARACTERISTIC_DESCRIPTOR_VALUE_HANDLE(p_event_data);

                WPRINT_BT_APP_INFO(("GATT_UUID_HID_REF_REPORT Handle :(0x%04x)\n", attr_hdl));

                for (i = 0; i < MAX_HOGP_REPORT; i++)
                {
                    if ((xiaoyi_rc_cb.hogp_report[i].report_handle != 0) && (attr_hdl <= xiaoyi_rc_cb.hogp_report[i].report_handle + 2) && (attr_hdl > xiaoyi_rc_cb.hogp_report[i].report_handle))
                    {
                        xiaoyi_rc_cb.hogp_report[i].report_ref_handle = attr_hdl;
                    }
                }
            }
            break;

            case GATT_UUID_HID_PROTO_MODE:
                WPRINT_BT_APP_INFO(("GATT_UUID_HID_PROTO_MODE Handle :(0x%04x)\n", GATT_DISCOVERY_RESULT_CHARACTERISTIC_DESCRIPTOR_VALUE_HANDLE(p_event_data)));
                xiaoyi_rc_cb.handle_protocolMode = GATT_DISCOVERY_RESULT_CHARACTERISTIC_DESCRIPTOR_VALUE_HANDLE(p_event_data);
                break;

            case GATT_UUID_HID_BT_KB_INPUT:
                WPRINT_BT_APP_INFO(("GATT_UUID_HID_BT_KB_INPUT Handle :(0x%04x)\n", GATT_DISCOVERY_RESULT_CHARACTERISTIC_DESCRIPTOR_VALUE_HANDLE(p_event_data)));
                xiaoyi_rc_cb.handle_bootKbInputReport = GATT_DISCOVERY_RESULT_CHARACTERISTIC_DESCRIPTOR_VALUE_HANDLE(p_event_data);
                break;

            case GATT_UUID_HID_BT_KB_OUTPUT:
                WPRINT_BT_APP_INFO(("GATT_UUID_HID_BT_KB_OUTPUT Handle :(0x%04x)\n", GATT_DISCOVERY_RESULT_CHARACTERISTIC_DESCRIPTOR_VALUE_HANDLE(p_event_data)));
                xiaoyi_rc_cb.handle_bootKboutputReport = GATT_DISCOVERY_RESULT_CHARACTERISTIC_DESCRIPTOR_VALUE_HANDLE(p_event_data);
                break;

            case GATT_UUID_HID_BT_MOUSE_INPUT:
                WPRINT_BT_APP_INFO(("GATT_UUID_HID_BT_MOUSE_INPUT Handle :(0x%04x)\n", GATT_DISCOVERY_RESULT_CHARACTERISTIC_DESCRIPTOR_VALUE_HANDLE(p_event_data)));
                break;

            default:
                break;
            }
        }

    }

}

/* Handle GATT discovery complete */
static void bt_le_proximity_handle_discovery_complete(void)
{
    wiced_bt_gatt_discovery_param_t discovery_parameters;

    /* If service discovery complete, then start discovery HID attribute */
    if (proximity_monitor_state == HOGP_HOST_STATE_DISCOVERING_SERVICES)
    {
        /* Discovery of services complete. */

        /* Get Link Loss characteristic handles, if Link Loss service handle was discovered */
        if (xiaoyi_rc_cb.hid_service.shandle)
        {
            proximity_monitor_state = HOGP_HOST_STATE_DISCOVERING_DESCRIPTERS;
            discovery_parameters.s_handle = xiaoyi_rc_cb.hid_service.shandle;
            discovery_parameters.e_handle = xiaoyi_rc_cb.hid_service.ehandle;
            wiced_bt_gatt_send_discover(connection_id, GATT_DISCOVER_CHARACTERISTIC_DESCRIPTORS, &discovery_parameters);
        }
        else
        {
            WPRINT_BT_APP_INFO(("HOGP service not found.\n"));
            wiced_bt_gatt_disconnect(connection_id);
        }
    }
    else if (proximity_monitor_state == HOGP_HOST_STATE_DISCOVERING_DESCRIPTERS)
    {
        /* get report map */
        if (xiaoyi_rc_cb.hid_service.shandle)
        {
            wiced_bt_gatt_read_param_t read_parameters;
            proximity_monitor_state            = HOGP_HOST_STATE_DISCOVERING_REPORT_DESP_ATTRIBUTE;
            read_parameters.by_handle.auth_req = GATT_AUTH_REQ_NONE;
            read_parameters.by_handle.handle = xiaoyi_rc_cb.handle_reportMap;
            wiced_bt_gatt_send_read(connection_id, GATT_READ_BY_HANDLE, &read_parameters);
            save_rc_bda(peer_address);
        }
        else
        {
            WPRINT_BT_APP_INFO(("HOGP service not found.\n"));
            wiced_bt_gatt_disconnect(connection_id);
        }
    }

}

/* GATT Read/Write operation completed */
static void bt_le_proximity_handle_operation_complete(wiced_bt_gatt_event_data_t *p_event_data)
{
    /* Read operation completed for TX Power Level */
    if (proximity_monitor_state == HOGP_HOST_STATE_DISCOVERING_REPORT_DESP_ATTRIBUTE)
    {
        if (p_event_data->operation_complete.response_data.att_value.p_data)
        {
            memcpy(xiaoyi_rc_cb.hid_descriptor, p_event_data->operation_complete.response_data.att_value.p_data, p_event_data->operation_complete.response_data.att_value.len);
            xiaoyi_rc_cb.hid_desp_len = p_event_data->operation_complete.response_data.att_value.len;
        }


        bt_le_read_remote_report_reference();
    }
    else if (proximity_monitor_state == HOGP_HOST_STATE_DISCOVERING_REPORT_REF_ATTRIBUTE)
    {
        if (p_event_data->operation_complete.response_data.att_value.p_data)
        {
            uint8_t i = 0;

            for (i = 0; i < MAX_HOGP_REPORT; i++)
            {
                if (p_event_data->operation_complete.response_data.att_value.handle == xiaoyi_rc_cb.hogp_report[i].report_ref_handle)
                {
                    xiaoyi_rc_cb.hogp_report[i].report_id = p_event_data->operation_complete.response_data.att_value.p_data[0];
                    xiaoyi_rc_cb.hogp_report[i].report_type = p_event_data->operation_complete.response_data.att_value.p_data[1];
                    break;
                }
            }
        }

        if (!bt_le_read_remote_report_reference())
        {
            bt_le_enable_hogp_report();
        }
    }
    /* Write operation completed for Link Loss Alert Level */
    else if (proximity_monitor_state == HOGP_HOST_STATE_ENABLE_REPORT)
    {
        if (p_event_data->operation_complete.response_data.att_value.p_data)
        {
        }

        if (!bt_le_enable_hogp_report())
        {
            proximity_monitor_state = HOGP_HOST_STATE_WORKING;
        }
    }
    else if (proximity_monitor_state == HOGP_HOST_STATE_WORKING)
    {
        if ((p_event_data->operation_complete.response_data.att_value.p_data) && (p_event_data->operation_complete.op == GATTC_OPTYPE_NOTIFICATION))
        {
            uint8_t i = 0, j = 0;

            for (i = 0; i < MAX_HOGP_REPORT; i++)
            {
                if (xiaoyi_rc_cb.hogp_report[i].report_handle == p_event_data->operation_complete.response_data.handle)
                {
                    WPRINT_BT_APP_INFO(("HOGP HID REPORT# ID:%d, TYPE: %d\n", xiaoyi_rc_cb.hogp_report[i].report_id, xiaoyi_rc_cb.hogp_report[i].report_type));
                    WPRINT_BT_APP_INFO(("REPORT:"));
                    for (j = 0; j < p_event_data->operation_complete.response_data.att_value.len; j++)
                    {
                        WPRINT_BT_APP_INFO((" %02x ", p_event_data->operation_complete.response_data.att_value.p_data[j]));
                    }
                    WPRINT_BT_APP_INFO(("\n"));
                    //#NT#2015/08/21#Lincy Lin -begin
                    //#NT#
                    {
                        BT_RCKEY_CB  *rckeyCB;
                        rckeyCB = BT_GetRCKEY_CB();
                        if (rckeyCB)
                        {
                            rckeyCB(p_event_data->operation_complete.response_data.att_value.p_data,p_event_data->operation_complete.response_data.att_value.len);
                        }
                    }
                    //#NT#2015/08/21#Lincy Lin -end
                    break;
                }
            }
        }
        else if (xiaoyi_rc_cb.handle_reportMap == p_event_data->operation_complete.response_data.handle)
        {
            WPRINT_BT_APP_INFO(("REPORT MAP: hdl [%d] \r\n", xiaoyi_rc_cb.handle_reportMap));
        }
    }
}

static void bt_le_proximity_start_discovery(void)
{
    wiced_bt_gatt_discovery_param_t discovery_parameters;

    if (proximity_monitor_state == HOGP_HOST_STATE_IDLE)
    {
        /* Start discovery for all services available on remote device */
        proximity_monitor_state = HOGP_HOST_STATE_DISCOVERING_SERVICES;
        memset(&discovery_parameters, 0, sizeof(wiced_bt_gatt_discovery_param_t));
        discovery_parameters.s_handle = 1;
        discovery_parameters.e_handle = 0xFFFF;
        discovery_parameters.uuid.len = LEN_UUID_16;
        discovery_parameters.uuid.uu.uuid16 = xiaoyi_rc_cb.hid_service.uuid;
        wiced_bt_gatt_send_discover(connection_id, GATT_DISCOVER_SERVICES_BY_UUID, &discovery_parameters);
    }
}


/* GATT event handler */
wiced_bt_gatt_status_t bt_le_proximity_gatt_cback(wiced_bt_gatt_evt_t event, wiced_bt_gatt_event_data_t *p_event_data)
{
    wiced_bt_gatt_status_t  status = WICED_BT_GATT_SUCCESS;
    uint8_t                 *bda;
    wiced_result_t          result;


    switch (event)
    {
    case GATT_CONNECTION_STATUS_EVT:
        /* GATT connection status change */
        bda = p_event_data->connection_status.bd_addr;
        WPRINT_BT_APP_INFO(("GATT connection to [%02X:%02X:%02X:%02X:%02X:%02X] %s.\n",  bda[0], bda[1], bda[2], bda[3], bda[4], bda[5],
            (p_event_data->connection_status.connected ? "established" : "released/failed")));

        if (p_event_data->connection_status.link_role == 1)
        {
            bt_le_gatt_server_cback(event, p_event_data);
        }
        else
        {
            memcpy(peer_address, bda, BD_ADDR_LEN);

            #if 1
            //#NT#2015/08/26#Lincy Lin -begin
            //#NT#
            {
                BT_EVENT_CB  *eventCB;
                eventCB = BT_GetEvent_CB();

                if (eventCB)
                {
                    if (p_event_data->connection_status.connected)
                        eventCB(BT_CONNECTION_STATUS_CONNECTED);
                    else
                        eventCB(BT_CONNECTION_STATUS_DISCONNECTED);
                }
            }
            #endif
            //#NT#2015/08/26#Lincy Lin -end
            if (p_event_data->connection_status.connected)
            {
                /* Connection established. */
                connection_id = p_event_data->connection_status.conn_id;

                /* If not already bonded, then bond */
                if (bonding_required)
                {
                    WPRINT_BT_APP_INFO(("Bonding started with [%02X:%02X:%02X:%02X:%02X:%02X] (addr type=%s)\n",
                            peer_address[0], peer_address[1], peer_address[2],
                            peer_address[3], peer_address[4], peer_address[5],
                            (peer_address_type == BLE_ADDR_PUBLIC) ? "Public":"Random"));

                    result = wiced_bt_dev_sec_bond (peer_address, peer_address_type, BT_TRANSPORT_LE, 0, NULL);

                    WPRINT_BT_APP_INFO(("wiced_bt_dev_sec_bond result=%i\n", result));
                }
                else
                {
                    if (xiaoyi_rc_cb.handle_hidInfo == 0)
                    {
                        bt_le_proximity_start_discovery();
                    }
                    else
                    {
                        bt_le_enable_hogp_report();
                    }
                }
            }
            else
            {
                bt_le_disable_hogp_report();
                /* Connection dropped */
            wiced_result_t result = wiced_bt_ble_scan(BTM_BLE_SCAN_TYPE_HIGH_DUTY, FALSE, scan_result_cback);
                if (result == WICED_BT_PENDING)
                {
                    WPRINT_BT_APP_INFO(("BLE SCAN started \n"));
                }

                proximity_monitor_state = HOGP_HOST_STATE_IDLE;
            }
            connection_initiated = FALSE;
        }

        break;

    case GATT_DISCOVERY_RESULT_EVT:
        /* Received GATT service discovery result */
        bt_le_proximity_handle_discovery_result(p_event_data);
        break;

    case GATT_DISCOVERY_CPLT_EVT:
        /* Discovery Complete */
        bt_le_proximity_handle_discovery_complete();
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
        break;
    }

    return (status);
}

void display_bda(unsigned char * bda)
{
    WPRINT_BT_APP_INFO(("bda - [%02X:%02X:%02X:%02X:%02X:%02X]",
        bda[0], bda[1], bda[2], bda[3], bda[4], bda[5]));
}

static void delete_one_old_bond_info(wiced_bt_device_address_t bda)
{
    wiced_bt_dev_bonded_device_info_t paired_device_addr[WICED_BT_DCT_MAX_DEVICES];
    uint16_t num_devices;
    int i = 0;
    int found = 0;

    num_devices = WICED_BT_DCT_MAX_DEVICES;
    if ((wiced_bt_dev_get_bonded_devices(paired_device_addr, &num_devices) == WICED_BT_SUCCESS) && (num_devices > 0))
    {
        for (i = 0; i < num_devices; i++)
        {
            display_bda(paired_device_addr[i].bd_addr);
            if (memcmp(bda, paired_device_addr[i].bd_addr, 6) == 0)
            {
                WPRINT_BT_APP_INFO(("scaned device is bonded before\r\n"));
                found = 1;
            }
        }
    }

    if (!found && num_devices == WICED_BT_DCT_MAX_DEVICES)
    {
        WPRINT_BT_APP_INFO(("bonded device info reaches max, delete one \r\n"));
        wiced_bt_dev_delete_bonded_device(paired_device_addr[0].bd_addr);
    }
}

#if (SCAN_FOR_PEER == TRUE)
/* Callback function for LE scan results */
static void scan_result_cback (wiced_bt_ble_scan_results_t *p_scan_result, uint8_t *p_adv_data)
{
    UINT8 *p_adv_devname;
    UINT8 adv_devname_len;
    UINT8 devname[SCAN_REMOTE_NAME_MAXLEN+1];

    if (!p_scan_result)
    {
        WPRINT_BT_APP_INFO(("LE scan completed.\n"));
        return;
    }

    #if 0
    WPRINT_BT_APP_INFO(("Inquiry result: [%02X:%02X:%02X:%02X:%02X:%02X]",
        p_scan_result->remote_bd_addr[0], p_scan_result->remote_bd_addr[1], p_scan_result->remote_bd_addr[2],
        p_scan_result->remote_bd_addr[3], p_scan_result->remote_bd_addr[4], p_scan_result->remote_bd_addr[5]));
    #endif

    if ((p_adv_devname = wiced_bt_ble_check_advertising_data(p_adv_data, BTM_BLE_ADVERT_TYPE_NAME_SHORT, &adv_devname_len)) != NULL)
    {
        if (adv_devname_len > SCAN_REMOTE_NAME_MAXLEN)
            adv_devname_len = SCAN_REMOTE_NAME_MAXLEN;

        memcpy(devname, p_adv_devname, adv_devname_len);
        devname[adv_devname_len] = '\0';
        WPRINT_BT_APP_INFO((" %s\n", devname));

        /* If connection not initiated yet, and this device has the name we are looking for, then initiate teh connection */
        if ((!connection_initiated) && (strcmp((char* )p_adv_devname, XIAOYI_SHORT_LOCAL_NAME) == 0))
        {
            /* Stop scanning */
            wiced_bt_ble_scan(BTM_BLE_SCAN_TYPE_NONE, FALSE, NULL);

            WPRINT_BT_APP_INFO(("Opening GATT connection to [%02X:%02X:%02X:%02X:%02X:%02X] (addr type=%s)...\n",
                    p_scan_result->remote_bd_addr[0], p_scan_result->remote_bd_addr[1], p_scan_result->remote_bd_addr[2],
                    p_scan_result->remote_bd_addr[3], p_scan_result->remote_bd_addr[4], p_scan_result->remote_bd_addr[5],
                    (p_scan_result->ble_addr_type == BLE_ADDR_PUBLIC) ? "Public":"Random"));

            //bonding_required = TRUE;        /* This is a new device; initiate bonding after connection comes up, to resolve bd address in subsequent connections */
            bonding_required = FALSE;        /* This is a new device; initiate bonding after connection comes up, to resolve bd address in subsequent connections */
            memcpy(peer_address, (uint8_t *)p_scan_result->remote_bd_addr, BD_ADDR_LEN);
            peer_address_type = p_scan_result->ble_addr_type;
            delete_one_old_bond_info(peer_address);
            wiced_bt_gatt_le_connect (peer_address, peer_address_type, BLE_CONN_MODE_HIGH_DUTY, TRUE);
            connection_initiated = TRUE;
        }
    }
    #if 0
    else
    {
        WPRINT_BT_APP_INFO(("\n"));
    }
    #endif

}
#endif /* SCAN_FOR_PEER */

static void bt_le_init(void)
{
    wiced_bt_gatt_register(&bt_le_proximity_gatt_cback);
    //wiced_bt_ble_enable_privacy(TRUE);

    /* enable proximity reporter*/
    bt_le_proximity_reporter_init();

    /* Enable proximity monitor */
    bt_le_proximity_monitor_init();
}

/* Initialize Proximity Reporter */
static void bt_le_proximity_monitor_init(void)
{

#if (SCAN_FOR_PEER == TRUE)
    {
        if (!connect_to_last_rc())
        {
            WPRINT_BT_APP_INFO(("Scanning for %s...\n", XIAOYI_SHORT_LOCAL_NAME));
            wiced_bt_ble_scan(BTM_BLE_SCAN_TYPE_HIGH_DUTY, FALSE, scan_result_cback);
        }
    }
#else
    /* Initiate GATT connection to server */
    WPRINT_BT_APP_INFO(("Opening GATT connection to [%02X:%02X:%02X:%02X:%02X:%02X]...\n",
            WICED_BT_CFG_PEER_BDA[0], WICED_BT_CFG_PEER_BDA[1], WICED_BT_CFG_PEER_BDA[2],
            WICED_BT_CFG_PEER_BDA[3], WICED_BT_CFG_PEER_BDA[4], WICED_BT_CFG_PEER_BDA[5]));

    wiced_bt_gatt_le_connect ((uint8_t *)WICED_BT_CFG_PEER_BDA, BLE_ADDR_PUBLIC, BLE_CONN_MODE_HIGH_DUTY,TRUE);
    connection_initiated = TRUE;
#endif
}


#endif