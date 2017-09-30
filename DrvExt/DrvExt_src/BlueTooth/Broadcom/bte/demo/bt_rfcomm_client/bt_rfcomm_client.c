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
 *  - Initiates RFCOMM connection to address specified by WICED_BT_CFG_PEER_BDA (using PSM = 1)
 *  - On connection establishment, send "Hello World" to remote device
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
#include "wiced_bt_rfcomm.h"
#include "wiced_bt_sdp.h"
#include "wiced_bt_cfg.h"

#include "bt_rfcomm_client.h"

/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/
/* Mask of RFCOMM events handled by app callback */
#define BT_RFCOMM_CLIENT_EVT_MASK   ((wiced_bt_rfcomm_port_event_t)(PORT_EV_FC | PORT_EV_FCS | PORT_EV_RXCHAR | \
                                            PORT_EV_TXEMPTY | PORT_EV_CTS | PORT_EV_DSR | \
                                            PORT_EV_RING | PORT_EV_CTSS | PORT_EV_DSRS))

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
static void bt_rfcomm_client_init(void);
static wiced_bt_dev_status_t bt_rfcomm_client_management_cback (wiced_bt_management_evt_t event, wiced_bt_management_evt_data_t *p_event_data);
static void bt_rfcomm_client_log_data (uint8_t *p_data, uint16_t len);

/******************************************************
 *               Variable Definitions
 ******************************************************/
uint16_t bt_rfcomm_client_handle;

/******************************************************
 *               Function Definitions
 ******************************************************/
void application_start( void )
{
    /* Initialize WICED platform */
    //wiced_init( );

    /* Initialize Bluetooth controller and host stack */
    wiced_bt_stack_init(bt_rfcomm_client_management_cback, &wiced_bt_cfg_settings, wiced_bt_cfg_buf_pools);
}

/* Bluetooth management event handler */
static wiced_bt_dev_status_t bt_rfcomm_client_management_cback (wiced_bt_management_evt_t event, wiced_bt_management_evt_data_t *p_event_data)
{
    wiced_bt_dev_status_t status = WICED_BT_SUCCESS;

    switch (event)
    {
    case BTM_ENABLED_EVT:
        /* Bluetooth controller and host stack enabled */
        WPRINT_BT_APP_INFO(("BT Enable status: 0x%02x.\n", p_event_data->enabled.status));

        /* Initialize the RFCOMM server */
        bt_rfcomm_client_init();
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

    default:
        WPRINT_BT_APP_INFO(("BT management callback: unhandled event (%i)\n", event));
        break;
    }

    return (status);
}

/* RFCOMM connection management callback callback */
void bt_rfcomm_conn_cback(wiced_bt_rfcomm_result_t code, uint16_t port_handle)
{
    char outbuf[] = "Hello World";
    UINT16 len;

    if (code == WICED_BT_RFCOMM_SUCCESS)
    {
        /* RFCOMM connection established. Send test string to remote device */
        WPRINT_BT_APP_INFO(("RFCOMM connection established.\n"));

        WPRINT_BT_APP_INFO(("RFCOMM TX (len=%i)\n", sizeof(outbuf)));
        bt_rfcomm_client_log_data ((uint8_t *)outbuf, sizeof(outbuf));

        wiced_bt_rfcomm_write_data(port_handle, outbuf, sizeof(outbuf), &len);
    }
    else if (code == WICED_BT_RFCOMM_PEER_CONNECTION_FAILED)
    {
        WPRINT_BT_APP_INFO(("RFCOMM connection failed.\n"));
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
void bt_rfcomm_client_event_cback(wiced_bt_rfcomm_port_event_t event, uint16_t port_handle)
{
    WPRINT_BT_APP_INFO(("bt_rfcomm_client_event_cback event mask=0x%04X\n", (int)event));
}

/* RFCOMM Data RX callback */
int bt_rfcomm_client_data_cback(uint16_t port_handle, void *p_data, uint16_t len)
{
    WPRINT_BT_APP_INFO(("RFCOMM RX (len=%i)\n", len));
    bt_rfcomm_client_log_data (p_data, len);
    return 0;
}

/* Initiate RFCOMM connection */
static void bt_rfcomm_client_open(wiced_bt_device_address_t bd_addr, uint8_t scn)
{
    WPRINT_BT_APP_INFO(("Opening RFCOMM connection to [%02X:%02X:%02X:%02X:%02X:%02X], scn=%i...\n",
            bd_addr[0], bd_addr[1], bd_addr[2], bd_addr[3], bd_addr[4], bd_addr[5], scn));

    /* Create RFCOMM server connection */
    wiced_bt_rfcomm_create_connection(UUID_SERVCLASS_SERIAL_PORT,
                scn,
                FALSE,
                BT_RFCOMM_CLIENT_APP_MTU,
                bd_addr,
                &bt_rfcomm_client_handle,
                bt_rfcomm_conn_cback);

    /* Set data callback RFCOMM */
    wiced_bt_rfcomm_set_data_callback(bt_rfcomm_client_handle, bt_rfcomm_client_data_cback);

    /* Set event callback RFCOMM, and events to be notified of */
    wiced_bt_rfcomm_set_event_mask(bt_rfcomm_client_handle, BT_RFCOMM_CLIENT_EVT_MASK);
    wiced_bt_rfcomm_set_event_callback(bt_rfcomm_client_handle, bt_rfcomm_client_event_cback);

}

/* Log data */
static void bt_rfcomm_client_log_data (uint8_t *p_data, uint16_t len)
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
static void bt_rfcomm_client_init(void)
{
    bt_rfcomm_client_open ((uint8_t *)WICED_BT_CFG_PEER_BDA, 1);
}
