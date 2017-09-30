#
# $ Copyright Broadcom Corporation $
#

NAME := BT_LE_PROXIMITY_REPORTER

GLOBAL_DEFINES     := BT_TRACE_PROTOCOL=FALSE BT_USE_TRACES=FALSE

$(NAME)_SOURCES := 	\
			bt_le_proximity_reporter.c \
            wiced_bt_gatt_db.c \
			wiced_bt_cfg.c

$(NAME)_INCLUDES := .

$(NAME)_COMPONENTS := libraries/bluetooth_bte
