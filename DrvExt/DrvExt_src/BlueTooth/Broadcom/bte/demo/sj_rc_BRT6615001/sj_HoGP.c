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

#include "sj_HoGP.h"
#include "hoGP.h"
#include "utron_utils.h"
#include "BT_BrcmInt.h"

/******************************************************
 *                    Constants
 ******************************************************/
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
    HOGP_HOST_STATE_WORKING
} proximity_monitor_state_t;

/* Enumeration of services application is interested in */

/******************************************************
 *               Function Declarations
 ******************************************************/
static wiced_bt_dev_status_t bt_le_proximity_management_cback (wiced_bt_management_evt_t event, wiced_bt_management_evt_data_t *p_event_data);
static void bt_le_proximity_monitor_init(void);
static void bt_le_proximity_start_discovery(void);
static void scan_result_cback(wiced_bt_ble_scan_results_t *p_scan_result, uint8_t *p_adv_data);

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

sj_rc_cb_t sj_rc_cb;
int service_num = 0;
int service_index = 0;

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
    sj_rc_init(&sj_rc_cb);
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

    default:
        WPRINT_BT_APP_INFO(("BT unhandled management event: 0x%02x.\r\n", event));
        break;
    }

    return (status);
}

/* Handle GATT discovery result */
static void bt_le_proximity_handle_discovery_result(wiced_bt_gatt_event_data_t *p_event_data)
{
    uint16_t service_uuid;
	
	WPRINT_BT_APP_INFO(("bt_le_proximity_handle_discovery_result(%d)\n", proximity_monitor_state));

    /* Handle service discovery result */
    if (proximity_monitor_state == HOGP_HOST_STATE_DISCOVERING_SERVICES)
    {
        if (GATT_DISCOVERY_RESULT_SERVICE_UUID_LEN(p_event_data) == LEN_UUID_16)
        {
            /* We are interested in Link loss, Immediate alert, and TX Power services */
			service_uuid = GATT_DISCOVERY_RESULT_SERVICE_UUID16(p_event_data);
            WPRINT_BT_APP_INFO(("Discovered service (0x%04x) handle range: [%04x - %04x]\n", service_uuid, GATT_DISCOVERY_RESULT_SERVICE_START_HANDLE(p_event_data), GATT_DISCOVERY_RESULT_SERVICE_END_HANDLE(p_event_data)));
			
			if(service_num < MAX_SERVICE_NUM){
				sj_rc_cb.hid_service[service_num].shandle = GATT_DISCOVERY_RESULT_SERVICE_START_HANDLE(p_event_data);
				sj_rc_cb.hid_service[service_num].ehandle = GATT_DISCOVERY_RESULT_SERVICE_END_HANDLE(p_event_data);
			}
			else
				WPRINT_BT_APP_INFO(("!!!Too many services(%d)!!!\n", service_num));
			
			service_num++;
        }
    }
    else if (proximity_monitor_state == HOGP_HOST_STATE_DISCOVERING_DESCRIPTERS)
    {
		WPRINT_BT_APP_INFO(("Characteristic(0x%x)\r\n", GATT_DISCOVERY_RESULT_CHARACTERISTIC_DESCRIPTOR_UUID16(p_event_data)));

        /* If discovered Immediate Alert service's Alert Level attribute, then store its handle */
        if (GATT_DISCOVERY_RESULT_CHARACTERISTIC_DESCRIPTOR_UUID_LEN(p_event_data) == LEN_UUID_16)
        {
		WPRINT_BT_APP_INFO(("Descriptor UUID is 0x%04x\r\n", GATT_DISCOVERY_RESULT_CHARACTERISTIC_DESCRIPTOR_UUID16(p_event_data)));
        }

    }

}

/* Handle GATT discovery complete */
static void bt_le_proximity_handle_discovery_complete(void)
{
    wiced_bt_gatt_discovery_param_t discovery_parameters;
	
	WPRINT_BT_APP_INFO(("bt_le_proximity_handle_discovery_complete(%d)\n", proximity_monitor_state));

    /* If service discovery complete, then start discovery HID attribute */
    if (proximity_monitor_state == HOGP_HOST_STATE_DISCOVERING_SERVICES)
    {
        /* Discovery of services complete. */
        {
			proximity_monitor_state = HOGP_HOST_STATE_DISCOVERING_DESCRIPTERS;
            discovery_parameters.s_handle = sj_rc_cb.hid_service[0].shandle;
            discovery_parameters.e_handle = sj_rc_cb.hid_service[0].ehandle;
			WPRINT_BT_APP_INFO(("Exploring service(%d)\n", 0));
            wiced_bt_gatt_send_discover(connection_id, GATT_DISCOVER_CHARACTERISTIC_DESCRIPTORS, &discovery_parameters);
        }
    }
    else if (proximity_monitor_state == HOGP_HOST_STATE_DISCOVERING_DESCRIPTERS)
    {
		if(service_index < (service_num - 1)){
			service_index++;
			WPRINT_BT_APP_INFO(("Exploring service(%d)\n", service_index));
            discovery_parameters.s_handle = sj_rc_cb.hid_service[service_index].shandle;
            discovery_parameters.e_handle = sj_rc_cb.hid_service[service_index].ehandle;
            wiced_bt_gatt_send_discover(connection_id, GATT_DISCOVER_CHARACTERISTIC_DESCRIPTORS, &discovery_parameters);
		}
		else{
			proximity_monitor_state = HOGP_HOST_STATE_WORKING;
		}
	}
}

/* GATT Read/Write operation completed */
static void bt_le_proximity_handle_operation_complete(wiced_bt_gatt_event_data_t *p_event_data)
{
	WPRINT_BT_APP_INFO(("bt_le_proximity_handle_operation_complete(%d,%d,%d)\n", 
	p_event_data->operation_complete.conn_id, p_event_data->operation_complete.op, p_event_data->operation_complete.status));
    //else if (proximity_monitor_state == HOGP_HOST_STATE_WORKING)
    {
        if ((p_event_data->operation_complete.response_data.att_value.p_data) && (p_event_data->operation_complete.op == GATTC_OPTYPE_NOTIFICATION))
        {
            uint8_t j = 0;

            WPRINT_BT_APP_INFO(("REPORT(%d)", p_event_data->operation_complete.response_data.att_value.handle));
            for (j = 0; j < p_event_data->operation_complete.response_data.att_value.len; j++)
            {
                 WPRINT_BT_APP_INFO((" %02x ", p_event_data->operation_complete.response_data.att_value.p_data[j]));
            }
            WPRINT_BT_APP_INFO(("\n"));
        }
    }
}

static void bt_le_proximity_start_discovery(void)
{
    wiced_bt_gatt_discovery_param_t discovery_parameters;
	WPRINT_BT_APP_INFO(("bt_le_proximity_start_discovery(%d)\n", proximity_monitor_state));

    if (proximity_monitor_state == HOGP_HOST_STATE_IDLE)
    {
        /* Start discovery for all services available on remote device */
        proximity_monitor_state = HOGP_HOST_STATE_DISCOVERING_SERVICES;
        memset(&discovery_parameters, 0, sizeof(wiced_bt_gatt_discovery_param_t));
        discovery_parameters.s_handle = 1;
        discovery_parameters.e_handle = 0xFFFF;
        discovery_parameters.uuid.len = LEN_UUID_16;
        discovery_parameters.uuid.uu.uuid16 = sj_rc_cb.hid_service[0].uuid;
        //wiced_bt_gatt_send_discover(connection_id, GATT_DISCOVER_SERVICES_ALL, &discovery_parameters);
    }
}

/* GATT event handler */
wiced_bt_gatt_status_t bt_le_proximity_gatt_cback(wiced_bt_gatt_evt_t event, wiced_bt_gatt_event_data_t *p_event_data)
{
    wiced_bt_gatt_status_t  status = WICED_BT_GATT_SUCCESS;
    uint8_t                 *bda;
    wiced_result_t          result;

	WPRINT_BT_APP_INFO(("bt_le_proximity_gatt_cback(%d)\n", event));

    switch (event)
    {
    case GATT_CONNECTION_STATUS_EVT:
        /* GATT connection status change */
        bda = p_event_data->connection_status.bd_addr;
        WPRINT_BT_APP_INFO(("GATT connection to [%02X:%02X:%02X:%02X:%02X:%02X] %s.\n",  bda[0], bda[1], bda[2], bda[3], bda[4], bda[5],
            (p_event_data->connection_status.connected ? "established" : "released/failed")));
	WPRINT_BT_APP_INFO(("role is %d\n", p_event_data->connection_status.link_role));

        {
            memcpy(peer_address, bda, BD_ADDR_LEN);

            //#NT#2015/08/26#Lincy Lin -end
            if (p_event_data->connection_status.connected)
            {
                /* Connection established. */
                connection_id = p_event_data->connection_status.conn_id;
				WPRINT_BT_APP_INFO(("connection_id is %d\r\n", connection_id));

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
            }
            else
            {
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

    default:
		WPRINT_BT_APP_INFO(("!!!Unhandled Event(%d)!!!\n", event));
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

    if ((p_adv_devname = wiced_bt_ble_check_advertising_data(p_adv_data, BTM_BLE_ADVERT_TYPE_NAME_COMPLETE, &adv_devname_len)) != NULL)
    {
        if (adv_devname_len > SCAN_REMOTE_NAME_MAXLEN)
            adv_devname_len = SCAN_REMOTE_NAME_MAXLEN;

        memcpy(devname, p_adv_devname, adv_devname_len);
        devname[adv_devname_len] = '\0';
        WPRINT_BT_APP_INFO((" %s\n", devname));

        /* If connection not initiated yet, and this device has the name we are looking for, then initiate teh connection */
        if ((!connection_initiated) && (strncmp((char* )p_adv_devname, SJ_LONG_LOCAL_NAME, strlen(SJ_LONG_LOCAL_NAME)) == 0))
        {
            /* Stop scanning */
            wiced_bt_ble_scan(BTM_BLE_SCAN_TYPE_NONE, FALSE, NULL);

            WPRINT_BT_APP_INFO(("Opening GATT connection to [%02X:%02X:%02X:%02X:%02X:%02X] (addr type=%s)...\n",
                    p_scan_result->remote_bd_addr[0], p_scan_result->remote_bd_addr[1], p_scan_result->remote_bd_addr[2],
                    p_scan_result->remote_bd_addr[3], p_scan_result->remote_bd_addr[4], p_scan_result->remote_bd_addr[5],
                    (p_scan_result->ble_addr_type == BLE_ADDR_PUBLIC) ? "Public":"Random"));

            bonding_required = TRUE;        /* This is a new device; initiate bonding after connection comes up, to resolve bd address in subsequent connections */
            memcpy(peer_address, (uint8_t *)p_scan_result->remote_bd_addr, BD_ADDR_LEN);
            peer_address_type = p_scan_result->ble_addr_type;
            delete_one_old_bond_info(peer_address);
            wiced_bt_gatt_le_connect (peer_address, peer_address_type, BLE_CONN_MODE_HIGH_DUTY, TRUE);
            connection_initiated = TRUE;
        }
    }

}

static void bt_le_init(void)
{
	service_num = 0;
	service_index = 0;
    wiced_bt_gatt_register(&bt_le_proximity_gatt_cback);
    /* Enable proximity monitor */
    bt_le_proximity_monitor_init();
}

/* Initialize Proximity Reporter */
static void bt_le_proximity_monitor_init(void)
{
            WPRINT_BT_APP_INFO(("Scanning for %s...\n", SJ_LONG_LOCAL_NAME));
            wiced_bt_ble_scan(BTM_BLE_SCAN_TYPE_HIGH_DUTY, FALSE, scan_result_cback);
}