#ifndef YUANDOGHALMQTT_H__
#define YUANDOGHALMQTT_H__

#include "YuanDogUtils.h"

#ifdef __cplusplus
extern "C" {
#endif

/////////////////////////////////////////////////////////////////////
//
#define YUAN_CLOUD_CFG	"cfg"
#define YUAN_CLOUD_NMEA "nmea"

typedef void (*YuanHalMqttOnConnect)(int connect, uint32 state);		//connect: 1:connecte; 0:disconnect;

typedef void (*YuanHalMqttOnReceiveMessage)(const char *from, 			//from: source ID (default:000001)
										  const char *to, 				//to:	target ID (default:000000,or group ID,or device ID (IMEI))
										  const char *command,			//command: receieved command
										  void *msg, 					//msg: message of command
										  unsigned int msg_len);	 	//msg_len: lenght of message

typedef void (*YUANHAL_MQTT_THREAD)(void* lpParam);

/////////////////////////////////////////////////////////////////////
// global function

//Start-up MQTT
int YuanHalMqttStart(const char *id,							//id: (IMEI)
					 const char *host,							//host: www.cloudvechild.com
					 int port,									//port: 1883
					 const char *server_id,						//server_id: 000001
					 int heart_beat,							//heart_beat: 30
					 YuanHalMqttOnConnect on_connect,			//on_connect: callback function of connect
					 YuanHalMqttOnReceiveMessage on_message	//on_message: callback function of message
				  );

//Subscript MQTT				  
int YuanHalMqttSubscribeMessage(const char *command);	//command: command of subscript to set-up command received by server

//Send MQTT			  
int YuanMqttSendMessage(const char *from, 				//from: source ID (default:000001)
						const char *to,					//to:	target ID (default:000000,or group ID,or device ID (IMEI))
						const char *command, 			//command: receieved command
						void *msg, 						//msg: message of command
						unsigned int msg_len,			//msg_len: lenght of message
						int qos, 						//qos: quality 
						int retain);					//retain: 

//Stop MQTT						
int YuanHalMqttStop(void);

#ifdef __cplusplus
}
#endif

#endif