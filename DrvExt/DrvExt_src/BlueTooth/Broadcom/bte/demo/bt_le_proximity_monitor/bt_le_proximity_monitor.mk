#
# $ Copyright Broadcom Corporation $
#

NAME := BT_LE_PROXIMITY_MONITOR

GLOBAL_DEFINES     := BT_TRACE_PROTOCOL=TRUE BT_USE_TRACES=TRUE

$(NAME)_SOURCES := 	\
			bt_le_proximity_monitor.c \
			wiced_bt_cfg.c

$(NAME)_INCLUDES := .

$(NAME)_COMPONENTS := libraries/bluetooth_bte
