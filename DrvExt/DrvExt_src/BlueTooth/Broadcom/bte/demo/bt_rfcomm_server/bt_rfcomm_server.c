/*
 * $ Copyright Broadcom Corporation $
 */

/** @file
 *
 * RFCOMM Server Sample Application
 *
 * Features demonstrated
 *  - WICED BT RFCOMM APIs
 *
 * On startup this demo:
 *  - Initializes the Bluetooth sub system
 *  - Initialize RFCOMM server (using PSM=1), and wait for a connection
 *  - On receiving data over RFCOMM, echo back to remote device
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
#include "wiced_bt_sdp.h"
#include "wiced_bt_rfcomm.h"
#include "wiced_bt_cfg.h"

#include "bt_rfcomm_server.h"
#ifndef WIN32
#include "brcmhal.h"
#else
#include <stdarg.h>
#endif

#if DEMO_APP_TYPE == DEMO_APP_RFCOMM_SVR
/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/
/* Mask of RFCOMM events handled by app callback */
#define BT_RFCOMM_SERVER_EVT_MASK   ((wiced_bt_rfcomm_port_event_t)(PORT_EV_FC | PORT_EV_FCS | PORT_EV_RXCHAR | \
                                            PORT_EV_TXEMPTY | PORT_EV_CTS | PORT_EV_DSR | \
                                            PORT_EV_RING | PORT_EV_CTSS | PORT_EV_DSRS))
//#ifndef WIN32
void app_debug_trace(const char * fmt_str, ...);
//#else
//#define app_debug_trace  debug_trace
//#endif
/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *                    Structures
 ******************************************************/

/******************************************************
 *               Function Declarations
 ******************************************************/
static void bt_rfcomm_server_init(void);
static wiced_bt_dev_status_t bt_rfcomm_server_management_cback (wiced_bt_management_evt_t event, wiced_bt_management_evt_data_t *p_event_data);
static void bt_rfcomm_server_log_data (uint8_t *p_data, uint16_t len);

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
    char mfi_eir[240] = {0,
        4, 0x08, 'a', 'b', 'c',
        17, 0x06, 0x00, 0x00, 0x00, 0x00, 0xDE, 0xCA, 0xFA, 0xDE, 0xDE, 0xCA, 0xDE, 0xAF, 0xDE, 0xCA, 0xCA, 0xFF};

    wiced_bt_dev_write_eir(mfi_eir, sizeof(mfi_eir));
    /* Initialize the RFCOMM server */
    bt_rfcomm_server_init();

}

static void set_BD()
{
    unsigned char bd[] = { 0x01, 0x01, 0x01, 0x1A, 0x43, 0x43 };
    BTM_VendorSpecificCommand(0xFC01, 6, bd, set_address_cb);
}
/******************************************************
 *               Variable Definitions
 ******************************************************/
#define RFCOMM_SERVER_CONNECTION_COUNT 4
uint16_t bt_rfcomm_server_handle[RFCOMM_SERVER_CONNECTION_COUNT];

/******************************************************
 *               Function Definitions
 ******************************************************/
void application_start( )
{
    /* Initialize WICED platform */
    //wiced_init( );

    /* Initialize Bluetooth controller and host stack */
    wiced_bt_stack_init(bt_rfcomm_server_management_cback, &wiced_bt_cfg_settings, wiced_bt_cfg_buf_pools);
}

/* Bluetooth management event handler */
static wiced_bt_dev_status_t bt_rfcomm_server_management_cback (wiced_bt_management_evt_t event, wiced_bt_management_evt_data_t *p_event_data)
{
    wiced_bt_dev_status_t status = WICED_BT_SUCCESS;
    wiced_bt_device_address_t bda;
    char pin[4] = { '0', '0', '0', '0' };

    switch (event)
    {
    case BTM_ENABLED_EVT:
        /* Bluetooth controller and host stack enabled */
        app_debug_trace("BT Enable status: 0x%02x.\n", p_event_data->enabled.status);

        if (p_event_data->enabled.status == WICED_BT_SUCCESS)
        {
            wiced_bt_dev_read_local_addr(bda);
            app_debug_trace("Local Bluetooth Address: [%02X:%02X:%02X:%02X:%02X:%02X]\n", bda[0], bda[1], bda[2], bda[3], bda[4], bda[5]);

            set_BD();
        }
        break;

    /* Bluetooth Security Events */
    case BTM_PAIRED_DEVICE_LINK_KEYS_REQUEST_EVT:
        /* Request for stored link keys for remote device (if any) */
        /* (sample app does not store link keys to NVRAM) */
        status = WICED_BT_UNKNOWN_ADDR;
        break;

    case BTM_PAIRED_DEVICE_LINK_KEYS_UPDATE_EVT:
        /* Request store newly generated pairing link keys to NVRAM */
        /* (sample app does not store link keys to NVRAM) */
        app_debug_trace("link key update\n");
        p_event_data->paired_device_link_keys_update.key_data;
        break;

    case BTM_PAIRING_IO_CAPABILITIES_BR_EDR_REQUEST_EVT:
        /* Request for local IO capabilities (sample app does not have i/o capabilities) */
        p_event_data->pairing_io_capabilities_br_edr_request.local_io_cap = BTM_IO_CAPABILIES_NONE;
        break;

    case BTM_USER_CONFIRMATION_REQUEST_EVT:
        /* User confirmation request for pairing (sample app always accepts) */
        wiced_bt_dev_confirm_req_reply (WICED_BT_SUCCESS, p_event_data->user_confirmation_request.bd_addr);
        break;

    case BTM_PAIRING_COMPLETE_EVT:
        /* Pairing complete */
        app_debug_trace("BTM_PAIRING_COMPLETE_EVT\n");
        break;

    case BTM_LOCAL_IDENTITY_KEYS_REQUEST_EVT:
        /* Request to restore local identity keys from NVRAM (requested during Bluetooth start up) */
        /* (sample app does not store keys to NVRAM. New local identity keys will be generated).   */
        app_debug_trace("BTM_LOCAL_IDENTITY_KEYS_REQUEST_EVT\n");
        status = WICED_BT_NO_RESOURCES;
        break;

    case BTM_LOCAL_IDENTITY_KEYS_UPDATE_EVT:
        /* Request to store newly generated local identity keys to NVRAM */
        /* (sample app does not store keys to NVRAM) */
        app_debug_trace("BTM_LOCAL_IDENTITY_KEYS_UPDATE_EVT\n");
        break;

    case BTM_PIN_REQUEST_EVT:
        //wiced_bt_dev_pin_code_reply(p_event_data->pin_request.bd_addr, WICED_BT_SUCCESS, 4, (uint8_t *)pin);
        break;

    default:
        WPRINT_BT_APP_INFO(("BT management callback: unhandled event (%i)\n", event));
        break;
    }

    return (status);
}
/* RFCOMM connection management callback callback */
void bt_rfcomm_server_conn_cback(wiced_bt_rfcomm_result_t code, uint16_t port_handle)
{
    if (code == WICED_BT_RFCOMM_SUCCESS)
    {
        /* RFCOMM connection established. Send test string to remote device */
        WPRINT_BT_APP_INFO(("RFCOMM connection established.\n"));
    }
    else if (code == WICED_BT_RFCOMM_CLOSED)
    {
        WPRINT_BT_APP_INFO(("RFCOMM connection closed.\n"));
    }
    else
    {
        WPRINT_BT_APP_INFO(("%s unhandled code=0x%x\n", __FUNCTION__, code));
    }
}

/* RFCOMM port event callback */
void bt_rfcomm_server_evt_cback(wiced_bt_rfcomm_port_event_t event, uint16_t port_handle)
{
    WPRINT_BT_APP_INFO(("bt_rfcomm_server_evt_cback event mask=0x%04X\n", (int)event));
}

//#define TEST_RFCOMM_RX

#ifdef TEST_RFCOMM_RX
void test_speed(uint16_t len)
{
    static uint32_t length = 0;
    static wiced_time_t time_ms = 0;
    static wiced_time_t tmp_ms = 0;

    //wiced_time_get_time(&tmp_ms);
    tmp_ms = OS_GetTime32();
    length += (len & 0x0ffff);

    if(time_ms == 0)
    {
        time_ms = tmp_ms;
        return;
    }

    if((tmp_ms - time_ms) >= 10000)
    {
        app_debug_trace("rx/tx speed %d, length %d, time(%d)\r\n", length / 10, length, (tmp_ms - time_ms));
        time_ms = tmp_ms;
        length = 0;
    }
}
#endif

/* RFCOMM Data RX callback */
int bt_rfcomm_server_data_cback(uint16_t port_handle, void *p_data, uint16_t len)
{

#ifdef TEST_RFCOMM_RX

    //WPRINT_BT_APP_INFO(("."));
    test_speed(len);
#else
    uint16_t len_sent;
    WPRINT_BT_APP_INFO(("RFCOMM RX (len=%i)\n", len));
    bt_rfcomm_server_log_data (p_data, len);

    /* Echo back to client */
    WPRINT_BT_APP_INFO(("RFCOMM TX (len=%i)\n", len));
    bt_rfcomm_server_log_data (p_data, len);

    wiced_bt_rfcomm_write_data(port_handle, (char *)p_data, len, &len_sent);
#endif
    return 0;
}

/* Log data */
static void bt_rfcomm_server_log_data (uint8_t *p_data, uint16_t len)
{
    uint16_t i, j, offset = 0;

    /* Display incoming data */
    while (len > 0)
    {
        WPRINT_BT_APP_INFO(("   %04X: ", offset));

        j = 16;
        if (len < 16)
            j = len;

        for (i=0; i<j; i++)
        {
            WPRINT_BT_APP_INFO(("%02X ", ((uint8_t *)p_data)[offset+i]));
        }

        WPRINT_BT_APP_INFO(("\n"));
        offset += j;
        len -= j;
    }
}

/* Initialize the RFCOMM server */
static void bt_rfcomm_server_init(void)
{
    wiced_bt_device_address_t bd_addr;
    int i = 0;
    /* Initialize SDP server database for rfcble_app */
    wiced_bt_sdp_db_init((UINT8 *)wiced_bt_sdp_db, wiced_bt_sdp_db_size);

    for (i = 0; i < RFCOMM_SERVER_CONNECTION_COUNT; i++)
    {
        /* Create RFCOMM server connection */
        wiced_bt_rfcomm_create_connection(UUID_SERVCLASS_SERIAL_PORT,
            BT_RFCOMM_SERVER_APP_SCN,
            TRUE,
            BT_RFCOMM_SERVER_APP_MTU,
            bd_addr,
            &bt_rfcomm_server_handle[i],
            bt_rfcomm_server_conn_cback);

        /* Set data callback RFCOMM */
        wiced_bt_rfcomm_set_data_callback(bt_rfcomm_server_handle[i], bt_rfcomm_server_data_cback);

        /* Set event callback RFCOMM, and events to be notified of */
        wiced_bt_rfcomm_set_event_mask(bt_rfcomm_server_handle[i], BT_RFCOMM_SERVER_EVT_MASK);
        wiced_bt_rfcomm_set_event_callback(bt_rfcomm_server_handle[i], bt_rfcomm_server_evt_cback);
    }


    /* Enable connectability (use default connectability window/interval) */
    wiced_bt_dev_set_connectability (BTM_CONNECTABLE, 0, 0);

    /* Enable discoverability (use default discoverability window/interval) */
    wiced_bt_dev_set_discoverability (BTM_GENERAL_DISCOVERABLE, 0, 0);

    WPRINT_BT_APP_INFO(("Waiting for RFCOMM connection (scn=%i)...\r\n", BT_RFCOMM_SERVER_APP_SCN));

}

void app_debug_trace(const char * fmt_str, ...)
{
    char buffer[512];
    va_list ap;

    va_start(ap, fmt_str);
    vsprintf(buffer, fmt_str, ap);
    va_end(ap);

#ifndef WIN32
#ifdef USE_UART0_TRACE
    BrcmHalLog(BRCM_HALLOG_INFO, buffer);
#else
    i2c_debug_trace(time);
#endif
#else
    void log2udp( char * msg);
    log2udp(buffer);
#endif
}

#endif