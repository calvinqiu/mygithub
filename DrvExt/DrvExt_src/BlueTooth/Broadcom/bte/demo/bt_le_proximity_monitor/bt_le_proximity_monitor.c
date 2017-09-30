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
#include "wiced.h"
#include "wiced_bt_stack.h"
#include "wiced_bt_dev.h"
#include "wiced_bt_ble.h"
#include "wiced_bt_gatt.h"
#include "wiced_bt_cfg.h"
#include "sdpdefs.h"

#include "bt_le_proximity_monitor.h"
#if (DEMO_APP_TYPE == DEMO_APP_BLE_MONITOR)

/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/
#define MONITOR_TASK_PRIORITY               6           /* Task priority for signal strength monitoring thread  */
#define MONITOR_TASK_STACKSIZE              0x1000      /* Stack size for signal strength monitoring thread     */
#define PROXIMITY_ALERT_LEVEL_MAX           0xFF
#define LINKLOSS_SIGNAL_STRENGTH_THRESHOLD  65          /* Threshold at which to send an Link Loss alert to remote device */
#define RSSI_MONITORING_INTERVAL_MS         2000        /* RSSI monitoring interval in ms */
#define SCAN_FOR_PEER                       TRUE        /* If TRUE, scan for peer instead of connecting to WICED_BT_CFG_PEER_BDA */
#define SCAN_REMOTE_NAME_MAXLEN             32          /* Maximum length stored for remote device names */

/******************************************************
 *                   Enumerations
 ******************************************************/

/* Proximity montitor states */
typedef enum
{
    PROXIMITY_MONITOR_STATE_IDLE,
    PROXIMITY_MONITOR_STATE_DISCOVERING_SERVICES,
    PROXIMITY_MONITOR_STATE_DISCOVERING_LINKLOSS_ALERT_LEVEL_ATTRIBUTE,
    PROXIMITY_MONITOR_STATE_DISCOVERING_IMMEDIATE_ALERT_LEVEL_ATTRIBUTE,
    PROXIMITY_MONITOR_STATE_DISCOVERING_TX_POWER_LEVEL_ATTRIBUTE,
    PROXIMITY_MONITOR_STATE_READING_TX_POWER_LEVEL,
    PROXIMITY_MONITOR_STATE_WRITE_LINKLOSS_ALERT_LEVEL,
    PROXIMITY_MONITOR_STATE_MONITORING
} proximity_monitor_state_t;

/* Enumeration of services application is interested in */
enum
{
    SERVICE_LINKLOSS,
    SERVICE_IMMEDIATE_ALERT,
    SERVICE_TX_POWER,

    SERVICE_MAX
};

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
} proximity_service_t;

/******************************************************
 *               Function Declarations
 ******************************************************/
static wiced_bt_dev_status_t bt_le_proximity_management_cback (wiced_bt_management_evt_t event, wiced_bt_management_evt_data_t *p_event_data);
static void bt_le_proximity_monitor_init(void);
//static void bt_le_rssi_monitor_task (void* arg);
static void bt_le_proximity_start_discovery(void);

/******************************************************
 *               Variable Definitions
 ******************************************************/
proximity_monitor_state_t   proximity_monitor_state = PROXIMITY_MONITOR_STATE_IDLE;
uint16_t                    connection_id;
int8_t                      remote_tx_power_level;
uint8_t                     current_proximity_alert_level;
proximity_service_t         proximity_service[SERVICE_MAX];
wiced_thread_t              monitor_task_id;
const uint8_t               desired_peer_device[] = "WICED Proximity";      /* Name of desired peer device to connect to */
wiced_bool_t                connection_initiated = FALSE;
wiced_bool_t                bonding_required = FALSE;
wiced_bt_device_address_t   peer_address;
wiced_bt_ble_address_type_t peer_address_type;

#ifdef MPU6515
#include <RTOS.h>

typedef void(*wiced_thread_function_t)(void* arg);
static void bt_ble_proximity_rssi_cback(wiced_bt_dev_rssi_result_t *p_rssi_result);

//#define SET_BD_INCLUDED

#ifdef SET_BD_INCLUDED
#include "wiced_bt_dev.h"

typedef struct
{
    UINT16  opcode;
    UINT16  param_len;
    UINT8   *p_param_buf;
} tBTM_VSC_CMPL;

typedef void (tBTM_VSC_CMPL_CB)(tBTM_VSC_CMPL *p1);

int BTM_VendorSpecificCommand(UINT16 opcode, UINT8 param_len,
    UINT8 *p_param_buf, tBTM_VSC_CMPL_CB *p_cb);


void set_address_cb(tBTM_VSC_CMPL *p1)
{
    /* Initialize the RFCOMM server */
    bt_le_proximity_monitor_init(); 
}

static void set_BD()
{
    const char bd[] = { 0x01, 0x01, 0x01, 0x1A, 0x43, 0x43 };
    BTM_VendorSpecificCommand(0xFC01, 6, bd, set_address_cb);
}
#endif

static void bt_le_rssi_monitor_task(void * arg)
{
    /* Periodically read receive signal strength every 2 seconds */
    /* (thread will be terminated when GATT connection is terminated) */
    while (1)
    {
        //wiced_bt_dev_read_rssi((uint8_t *)WICED_BT_CFG_PEER_BDA, (wiced_bt_dev_cmpl_cback_t *)bt_ble_proximity_rssi_cback);
        OS_Delay(RSSI_MONITORING_INTERVAL_MS);
    }
}
#endif

#ifdef MPU6515
wiced_result_t wiced_rtos_create_thread(wiced_thread_t* thread, uint8_t priority, const char* name, wiced_thread_function_t function, uint32_t stack_size, void* arg)
{
    static char stack[4096];
    wiced_thread_t read_thread_id;
    OS_CREATETASK_EX(&read_thread_id, (const char *)"read tx power", function, 140, stack, arg);
    return WICED_SUCCESS;
}
#endif
/******************************************************
 *               Function Definitions
 ******************************************************/
void application_start( )
{
    /* Initialize WICED platform */
    //wiced_init( );

    /* Initialize Bluetooth controller and host stack */
    wiced_bt_stack_init(bt_le_proximity_management_cback, &wiced_bt_cfg_settings, wiced_bt_cfg_buf_pools);
}

/* Bluetooth management event handler */
static wiced_bt_dev_status_t bt_le_proximity_management_cback (wiced_bt_management_evt_t event, wiced_bt_management_evt_data_t *p_event_data)
{
    wiced_bt_dev_status_t status = WICED_BT_SUCCESS;

    switch (event)
    {
    case BTM_ENABLED_EVT:
        /* Bluetooth controller and host stack enabled */
        WPRINT_BT_APP_INFO(("BT Enable status: 0x%02x.\r\n", p_event_data->enabled.status));
#ifndef SET_BD_INCLUDED
        /* Enable proximity reporter */
        bt_le_proximity_monitor_init();
#else
        set_BD();
#endif
        break;

    case BTM_BLE_SCAN_STATE_CHANGED_EVT:
        break;

    case BTM_SECURITY_REQUEST_EVT:
        WPRINT_BT_APP_INFO(("Security reqeust\n"));
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

    default:
        WPRINT_BT_APP_INFO(("BT unhandled management event: 0x%02x.\n", event));
        break;
    }

    return (status);
}

/* Set link loss alert level on remote device */
static void bt_le_set_link_loss_alert_level(void)
{
    wiced_bt_gatt_value_t write_value;

    proximity_monitor_state = PROXIMITY_MONITOR_STATE_WRITE_LINKLOSS_ALERT_LEVEL;

    /* Send GATT write request for Link Loss Alert Level attribute value */
    write_value.handle      = proximity_service[SERVICE_LINKLOSS].attribute_value_handle;
    write_value.len         = 1;            /* Link loss alert level value length */
    write_value.value[0]    = 2;            /* Set alert level to 2 */
    write_value.auth_req    = GATT_AUTH_REQ_NONE;
    wiced_bt_gatt_send_write (connection_id, GATT_WRITE, &write_value);
}

/* Read the remote proximity reporter's power level */
static void bt_le_read_remote_tx_power_level(void)
{
    wiced_bt_gatt_read_param_t read_param;

    proximity_monitor_state         = PROXIMITY_MONITOR_STATE_READING_TX_POWER_LEVEL;

    /* Send GATT read request for TX Power Level attribute value */
    read_param.by_handle.handle     = proximity_service[SERVICE_TX_POWER].attribute_value_handle;
    read_param.by_handle.auth_req   = GATT_AUTH_REQ_NONE;
    wiced_bt_gatt_send_read(connection_id, GATT_READ_BY_HANDLE, &read_param);
}

/* Read the remote proximity reporter's power level */
static void bt_le_start_monitoring_signal_strength(void)
{
    proximity_monitor_state = PROXIMITY_MONITOR_STATE_MONITORING;

    wiced_rtos_create_thread( &monitor_task_id, MONITOR_TASK_PRIORITY, "RSSI_MONITOR", &bt_le_rssi_monitor_task, MONITOR_TASK_STACKSIZE, NULL );
}


/* Handle GATT discovery result */
static void bt_le_proximity_handle_discovery_result(wiced_bt_gatt_event_data_t *p_event_data)
{
    uint16_t service_uuid;

    /* Handle service discovery result */
    if (proximity_monitor_state == PROXIMITY_MONITOR_STATE_DISCOVERING_SERVICES)
    {
        if (GATT_DISCOVERY_RESULT_SERVICE_UUID_LEN(p_event_data) == LEN_UUID_16)
        {
            /* We are interested in Link loss, Immediate alert, and TX Power services */
            service_uuid = GATT_DISCOVERY_RESULT_SERVICE_UUID16(p_event_data);

            WPRINT_BT_APP_INFO(("Discovered service (0x%04x) handle range: [%04x - %04x]\r\n", service_uuid, GATT_DISCOVERY_RESULT_SERVICE_START_HANDLE(p_event_data), GATT_DISCOVERY_RESULT_SERVICE_END_HANDLE(p_event_data)));

            /* If desired service was discovered, store the service's start and end attibute handles */
            if (service_uuid == UUID_SERVCLASS_LINKLOSS)
            {
                WPRINT_BT_APP_INFO(("Discovered LinkLoss service\r\n"));
                proximity_service[SERVICE_LINKLOSS].start_handle        = GATT_DISCOVERY_RESULT_SERVICE_START_HANDLE(p_event_data);
                proximity_service[SERVICE_LINKLOSS].end_handle          = GATT_DISCOVERY_RESULT_SERVICE_END_HANDLE(p_event_data);
            }
            else if (service_uuid == UUID_SERVCLASS_IMMEDIATE_ALERT)
            {
                WPRINT_BT_APP_INFO(("Discovered Immediate alert service\r\n"));
                proximity_service[SERVICE_IMMEDIATE_ALERT].start_handle = GATT_DISCOVERY_RESULT_SERVICE_START_HANDLE(p_event_data);
                proximity_service[SERVICE_IMMEDIATE_ALERT].end_handle   = GATT_DISCOVERY_RESULT_SERVICE_END_HANDLE(p_event_data);
            }
            else if (service_uuid == UUID_SERVCLASS_TX_POWER)
            {
                WPRINT_BT_APP_INFO(("Discovered TX Power service\n"));
                proximity_service[SERVICE_TX_POWER].start_handle        = GATT_DISCOVERY_RESULT_SERVICE_START_HANDLE(p_event_data);
                proximity_service[SERVICE_TX_POWER].end_handle          = GATT_DISCOVERY_RESULT_SERVICE_END_HANDLE(p_event_data);
            }
        }
    }
    /* Handle Link Loss Alert Level attribute discovery result */
    else if (proximity_monitor_state == PROXIMITY_MONITOR_STATE_DISCOVERING_LINKLOSS_ALERT_LEVEL_ATTRIBUTE)
    {
        /* If discovered Link Loss service's Alert Level attribute, then store its handle */
        if ((GATT_DISCOVERY_RESULT_CHARACTERISTIC_DESCRIPTOR_UUID_LEN(p_event_data) == LEN_UUID_16) &&
            (GATT_DISCOVERY_RESULT_CHARACTERISTIC_DESCRIPTOR_UUID16(p_event_data) == GATT_UUID_ALERT_LEVEL))
        {
            WPRINT_BT_APP_INFO(("Discovered link loss alert level value attribute (0x%04x)\r\n", GATT_DISCOVERY_RESULT_CHARACTERISTIC_DESCRIPTOR_VALUE_HANDLE(p_event_data)));
            proximity_service[SERVICE_LINKLOSS].attribute_value_handle  = GATT_DISCOVERY_RESULT_CHARACTERISTIC_DESCRIPTOR_VALUE_HANDLE(p_event_data);
        }
    }
    /* Handle Immediate Alert Level attribute discovery result */
    else if (proximity_monitor_state == PROXIMITY_MONITOR_STATE_DISCOVERING_IMMEDIATE_ALERT_LEVEL_ATTRIBUTE)
    {
        /* If discovered Immediate Alert service's Alert Level attribute, then store its handle */
        if ((GATT_DISCOVERY_RESULT_CHARACTERISTIC_DESCRIPTOR_UUID_LEN(p_event_data) == LEN_UUID_16) &&
            (GATT_DISCOVERY_RESULT_CHARACTERISTIC_DESCRIPTOR_UUID16(p_event_data) == GATT_UUID_ALERT_LEVEL))
        {
            WPRINT_BT_APP_INFO(("Discovered immediate alert level value attribute (0x%04x)\r\n", GATT_DISCOVERY_RESULT_CHARACTERISTIC_DESCRIPTOR_VALUE_HANDLE(p_event_data)));
            proximity_service[SERVICE_IMMEDIATE_ALERT].attribute_value_handle   = GATT_DISCOVERY_RESULT_CHARACTERISTIC_DESCRIPTOR_VALUE_HANDLE(p_event_data);
        }
    }
    /* Handle TX Power Level attribute discovery result */
    else if (proximity_monitor_state == PROXIMITY_MONITOR_STATE_DISCOVERING_TX_POWER_LEVEL_ATTRIBUTE)
    {
        /* If discovered TX Power Level service's TX Power attribute, then store its handle */
        if ((GATT_DISCOVERY_RESULT_CHARACTERISTIC_DESCRIPTOR_UUID_LEN(p_event_data) == LEN_UUID_16) &&
            (GATT_DISCOVERY_RESULT_CHARACTERISTIC_DESCRIPTOR_UUID16(p_event_data) == GATT_UUID_TX_POWER_LEVEL))
        {
            WPRINT_BT_APP_INFO(("Discovered tx power level value attribute (0x%04x)\r\n", GATT_DISCOVERY_RESULT_CHARACTERISTIC_DESCRIPTOR_VALUE_HANDLE(p_event_data)));
            proximity_service[SERVICE_TX_POWER].attribute_value_handle      = GATT_DISCOVERY_RESULT_CHARACTERISTIC_DESCRIPTOR_VALUE_HANDLE(p_event_data);
        }
    }
}

/* Handle GATT discovery complete */
static void bt_le_proximity_handle_discovery_complete(void)
{
    wiced_bt_gatt_discovery_param_t discovery_parameters;

    /* If service discovery complete, then start discovery for Link Loss Alert Level attribute */
    if (proximity_monitor_state == PROXIMITY_MONITOR_STATE_DISCOVERING_SERVICES)
    {
        /* Discovery of services complete. */

        /* Get Link Loss characteristic handles, if Link Loss service handle was discovered */
        if (proximity_service[SERVICE_LINKLOSS].start_handle)
        {
            proximity_monitor_state         = PROXIMITY_MONITOR_STATE_DISCOVERING_LINKLOSS_ALERT_LEVEL_ATTRIBUTE;
            discovery_parameters.s_handle   = proximity_service[SERVICE_LINKLOSS].start_handle;
            discovery_parameters.e_handle   = proximity_service[SERVICE_LINKLOSS].end_handle;
            wiced_bt_gatt_send_discover (connection_id, GATT_DISCOVER_CHARACTERISTIC_DESCRIPTORS, &discovery_parameters);
        }
        else
        {
            WPRINT_BT_APP_INFO(("Proximity Link Loss service not found.\n"));
            wiced_bt_gatt_disconnect(connection_id);
        }
    }
    /* If discovery for Link Loss Alert Level attribute complete, then start discovery for Immediate Alert Level attribute */
    else if (proximity_monitor_state == PROXIMITY_MONITOR_STATE_DISCOVERING_LINKLOSS_ALERT_LEVEL_ATTRIBUTE)
    {
        /* Get Immediate Alert characteristic handles, if Immediate Alert service handle was discovered */
        if (proximity_service[SERVICE_IMMEDIATE_ALERT].start_handle)
        {
            proximity_monitor_state         = PROXIMITY_MONITOR_STATE_DISCOVERING_IMMEDIATE_ALERT_LEVEL_ATTRIBUTE;
            discovery_parameters.s_handle   = proximity_service[SERVICE_IMMEDIATE_ALERT].start_handle;
            discovery_parameters.e_handle   = proximity_service[SERVICE_IMMEDIATE_ALERT].end_handle;
            wiced_bt_gatt_send_discover (connection_id, GATT_DISCOVER_CHARACTERISTIC_DESCRIPTORS, &discovery_parameters);
        }
        else
        {
            /* Remote proximity does not support the optional immediate alert service. */

            /* Set remote's link loss alert level */
            bt_le_set_link_loss_alert_level();
        }
    }
    /* If discovery for Immediate Alert Level attribute complete, then start discovery for TX Power Level attribute */
    else if (proximity_monitor_state == PROXIMITY_MONITOR_STATE_DISCOVERING_IMMEDIATE_ALERT_LEVEL_ATTRIBUTE)
    {
        /* Get TX Power Level characteristic handles, if TX Power Level service handle was discovered */
        if (proximity_service[SERVICE_TX_POWER].start_handle)
        {
            proximity_monitor_state         = PROXIMITY_MONITOR_STATE_DISCOVERING_TX_POWER_LEVEL_ATTRIBUTE;
            discovery_parameters.s_handle   = proximity_service[SERVICE_TX_POWER].start_handle;
            discovery_parameters.e_handle   = proximity_service[SERVICE_TX_POWER].end_handle;
            wiced_bt_gatt_send_discover (connection_id, GATT_DISCOVER_CHARACTERISTIC_DESCRIPTORS, &discovery_parameters);
        }
        else
        {
            /* Remote proximity does not support the optional TX Power Level reporting. */

            /* Set remote's link loss alert level */
            bt_le_set_link_loss_alert_level();
        }
    }
    /* If discovery for TX Power Level attribute complete (final attribute we are interested in), then read remote's TX Power Level value attribute */
    else if (proximity_monitor_state == PROXIMITY_MONITOR_STATE_DISCOVERING_TX_POWER_LEVEL_ATTRIBUTE)
    {
        /* Read remote's TX Power Level */
        bt_le_read_remote_tx_power_level();
    }
}

/* GATT Read/Write operation completed */
static void bt_le_proximity_handle_operation_complete(wiced_bt_gatt_event_data_t *p_event_data)
{
    /* Read operation completed for TX Power Level */
    if (proximity_monitor_state == PROXIMITY_MONITOR_STATE_READING_TX_POWER_LEVEL)
    {
        /* Received remote's TX Power Level. Store it */
        remote_tx_power_level = *(uint8_t *)p_event_data->operation_complete.response_data.att_value.p_data;

        WPRINT_BT_APP_INFO(("Remote TX Power Level: %i\r\n", remote_tx_power_level));

        /* Set remote's link loss alert level */
        bt_le_set_link_loss_alert_level();
    }
    /* Write operation completed for Link Loss Alert Level */
    else if (proximity_monitor_state == PROXIMITY_MONITOR_STATE_WRITE_LINKLOSS_ALERT_LEVEL)
    {
        /* Begin monitoring rssi */
        bt_le_start_monitoring_signal_strength();
    }
}

static void bt_le_proximity_start_discovery(void)
{
    wiced_bt_gatt_discovery_param_t discovery_parameters;

    /* Reset stored handles */
    memset(&proximity_service, 0, sizeof(proximity_service));
    current_proximity_alert_level   = PROXIMITY_ALERT_LEVEL_MAX;

    /* Start discovery for all services available on remote device */
    proximity_monitor_state = PROXIMITY_MONITOR_STATE_DISCOVERING_SERVICES;
    memset(&discovery_parameters, 0, sizeof(wiced_bt_gatt_discovery_param_t));
    discovery_parameters.s_handle   = 1;
    discovery_parameters.e_handle   = 0xFFFF;
    discovery_parameters.uuid.len   = LEN_UUID_16;
    wiced_bt_gatt_send_discover (connection_id, GATT_DISCOVER_SERVICES_ALL, &discovery_parameters);
}
/* GATT event handler */
static wiced_bt_gatt_status_t bt_le_proximity_gatt_cback(wiced_bt_gatt_evt_t event, wiced_bt_gatt_event_data_t *p_event_data)
{
    wiced_bt_gatt_status_t          status = WICED_BT_GATT_SUCCESS;
    uint8_t                         *bda;
    wiced_result_t          result;

    switch (event)
    {
    case GATT_CONNECTION_STATUS_EVT:
        /* GATT connection status change */
        bda = p_event_data->connection_status.bd_addr;
        WPRINT_BT_APP_INFO(("GATT connection to [%02X:%02X:%02X:%02X:%02X:%02X] %s.\n",  bda[0], bda[1], bda[2], bda[3], bda[4], bda[5],
            (p_event_data->connection_status.connected ? "established" : "released/failed")));

        memcpy(peer_address, bda, BD_ADDR_LEN);

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
                bt_le_proximity_start_discovery();
            }
        }
        else
        {
            /* Connection dropped */

            /* If signal strength monitoring was enabled, then terminate the monitoring thread */
            if (proximity_monitor_state == PROXIMITY_MONITOR_STATE_MONITORING)
            {
                wiced_rtos_delete_thread (&monitor_task_id);
            }
            proximity_monitor_state = PROXIMITY_MONITOR_STATE_IDLE;
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
        break;
    }

    return (status);
}

/* Received Signal Strength measurement handler */
static void bt_ble_proximity_rssi_cback(wiced_bt_dev_rssi_result_t *p_rssi_result)
{
    wiced_bt_gatt_value_t write_value;
    uint8_t new_proximity_alert_level;
    int8_t adjusted_signal_strength = (remote_tx_power_level - p_rssi_result->rssi);

    /* If signal strength falls below threshold, send an Immediate Alert write request */
    if (adjusted_signal_strength > LINKLOSS_SIGNAL_STRENGTH_THRESHOLD)
    {
        new_proximity_alert_level = 2;
    }
    else
    {
        new_proximity_alert_level = 0;
    }

    /* Send the alert (unless rssi is below threshold and current alert level is already 0) */
    if ((new_proximity_alert_level != 0) || (current_proximity_alert_level != 0))
    {
        WPRINT_BT_APP_INFO(("Sending immediate alert (adjusted rssi:%i, alert level: %i)\n", adjusted_signal_strength, new_proximity_alert_level));
        write_value.handle      = proximity_service[SERVICE_IMMEDIATE_ALERT].attribute_value_handle; /* Handle of Immediate Alert service's alert level handle */
        write_value.len         = 1;                                /* Length of immediate alert level value */
        write_value.value[0]    = new_proximity_alert_level;   /* Set alert level to 2 */
        write_value.auth_req    = GATT_AUTH_REQ_NONE;          /* No authentication requested */
        wiced_bt_gatt_send_write (connection_id, GATT_WRITE_NO_RSP, &write_value);
    }
    current_proximity_alert_level = new_proximity_alert_level;

}
#if 0// !defined(WIN32) && !defined(MPU6515)
/* Receive signal strength monitoring thread (started when connected to a device that supports TX Power Level service) */
static void bt_le_rssi_monitor_task (uint32_t arg)
{
    /* Periodically read receive signal strength every 2 seconds */
    /* (thread will be terminated when GATT connection is terminated) */
    while (1)
    {
        wiced_bt_dev_read_rssi((uint8_t *)peer_address, BT_TRANSPORT_LE, (wiced_bt_dev_cmpl_cback_t *)bt_ble_proximity_rssi_cback);
        wiced_rtos_delay_milliseconds (RSSI_MONITORING_INTERVAL_MS);
    }
}
#endif
#if (SCAN_FOR_PEER == TRUE)
/* Callback function for LE scan results */
static void scan_result_cback (wiced_bt_ble_scan_results_t *p_scan_result, uint8_t *p_adv_data)
{
    UINT8 *p_adv_devname;
    UINT8 adv_devname_len;
    UINT8 devname[SCAN_REMOTE_NAME_MAXLEN+1];

    if (!p_scan_result)
    {
        WPRINT_BT_APP_INFO(("LE scan completed.\r\n"));
        return;
    }

    WPRINT_BT_APP_INFO(("Inquiry result: [%02X:%02X:%02X:%02X:%02X:%02X]",
        p_scan_result->remote_bd_addr[0], p_scan_result->remote_bd_addr[1], p_scan_result->remote_bd_addr[2],
        p_scan_result->remote_bd_addr[3], p_scan_result->remote_bd_addr[4], p_scan_result->remote_bd_addr[5]));

    if ((p_adv_devname = wiced_bt_ble_check_advertising_data(p_adv_data, BTM_BLE_ADVERT_TYPE_NAME_COMPLETE, &adv_devname_len)) != NULL)
    {
        if (adv_devname_len > SCAN_REMOTE_NAME_MAXLEN)
            adv_devname_len = SCAN_REMOTE_NAME_MAXLEN;

        memcpy(devname, p_adv_devname, adv_devname_len);
        devname[adv_devname_len] = '\0';
        WPRINT_BT_APP_INFO((" %s\n", devname));

        /* If connection not initiated yet, and this device has the name we are looking for, then initiate teh connection */
        //if ((!connection_initiated) && (memcmp(p_adv_devname, desired_peer_device, adv_devname_len) == 0))
        if ((!connection_initiated) && (memcmp(WICED_BT_CFG_PEER_BDA, p_scan_result->remote_bd_addr, 6) == 0))
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
            wiced_bt_gatt_le_connect (peer_address, peer_address_type, BLE_CONN_MODE_HIGH_DUTY, TRUE);
            connection_initiated = TRUE;
        }
    }
    else
    {
        WPRINT_BT_APP_INFO(("\n"));
    }

}
#endif /* SCAN_FOR_PEER */
/* Initialize Proximity Reporter */
static void bt_le_proximity_monitor_init(void)
{
    /* Register for gatt event notifications */
    wiced_bt_gatt_register(&bt_le_proximity_gatt_cback);

#if (SCAN_FOR_PEER == TRUE)
    {
        wiced_bt_dev_bonded_device_info_t paired_device_addr[10];
        uint16_t num_devices;

        /* If there is a previously stored device, then connect to it */
        num_devices = 10;
        if ((wiced_bt_dev_get_bonded_devices(paired_device_addr, &num_devices) == WICED_BT_SUCCESS) && (num_devices > 0))
        {
            WPRINT_BT_APP_INFO(("Opening GATT connection to previously paired device [%02X:%02X:%02X:%02X:%02X:%02X]...\r\n",
                  paired_device_addr[0].bd_addr[0], paired_device_addr[0].bd_addr[1], paired_device_addr[0].bd_addr[2],
                  paired_device_addr[0].bd_addr[3], paired_device_addr[0].bd_addr[4], paired_device_addr[0].bd_addr[5]));
            wiced_bt_gatt_le_connect (paired_device_addr[0].bd_addr, paired_device_addr[0].addr_type, BLE_CONN_MODE_HIGH_DUTY, TRUE);
            connection_initiated = TRUE;
        }
        else
        {
            WPRINT_BT_APP_INFO(("Scanning for %s...\r\n", desired_peer_device));
            wiced_bt_ble_scan(BTM_BLE_SCAN_TYPE_HIGH_DUTY, FALSE, scan_result_cback);
        }
    }
#else
    /* Initiate GATT connection to server */
    WPRINT_BT_APP_INFO(("Opening GATT connection to [%02X:%02X:%02X:%02X:%02X:%02X]...\r\n",
            WICED_BT_CFG_PEER_BDA[0], WICED_BT_CFG_PEER_BDA[1], WICED_BT_CFG_PEER_BDA[2],
            WICED_BT_CFG_PEER_BDA[3], WICED_BT_CFG_PEER_BDA[4], WICED_BT_CFG_PEER_BDA[5]));

    wiced_bt_gatt_le_connect ((uint8_t *)WICED_BT_CFG_PEER_BDA, BLE_ADDR_PUBLIC, BLE_CONN_MODE_HIGH_DUTY,TRUE);
    connection_initiated = TRUE;
#endif
}
#endif