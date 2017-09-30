#
# $ Copyright Broadcom Corporation $
#

NAME := BT_RFCOMM_CLIENT

GLOBAL_DEFINES     := BT_TRACE_PROTOCOL=FALSE BT_USE_TRACES=FALSE

$(NAME)_SOURCES := 	\
			bt_rfcomm_client.c \
			wiced_bt_cfg.c

$(NAME)_INCLUDES := .

$(NAME)_COMPONENTS := libraries/bluetooth_bte
