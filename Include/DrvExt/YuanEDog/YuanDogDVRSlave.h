/*******************************************************************************/
/** \file YuanDogDVRSlave.h  Basic set of API control by YuanDog moudle.
For more information see \ref Yuanjitong.ltd

* Copyright 2015-2016 Yuanjitong Corporation -- http://www.yuanjitong.com
* This program is the proprietary software of Yuanjitong Corporation and/or
* its licensors, and may only be used, duplicated, modified or distributed
* pursuant to the terms and conditions of a separate, written license
* agreement executed between you and Yuanjitong (an "Authorized License").
* Except as set forth in an Authorized License, Yuanjitong grants no license
* (express or implied), right to use, or waiver of any kind with respect to
* the Software, and Yuanjitong expressly reserves all rights in and to the
* Software and all intellectual property rights therein. IF YOU HAVE NO
* AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY
* WAY, AND SHOULD IMMEDIATELY NOTIFY YUANJITONG AND DISCONTINUE ALL USE OF
* THE SOFTWARE.
*/

#ifndef YUANDOGDVRSLAVE_H_
#define YUANDOGDVRSLAVE_H_

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

/////////////////////////////////////////////////////////////////////////////////////
// UART : 9600(Bandrate),8(Data bits),1(Stop bits),none(Parity),none(flower control)


/////////////////////////////////////////////////////////////////////////////////////
// process
//                         YuanInit(YUANCLOUDCALLBACK)  // Called YuanInit at begin then assign call back functio.
//                            |
//                    |uart innterrupt| 				// Put YuanIntReceiver into Uart interrupt function, which used to analyser YuanCloud protocol
//                            |
//                    |YuanIntReciever| -> ||			// whihc will call YuanReadByte or YuanReadBytes to parase protocol.
//                            |-- 	|YuanCloudEDogInfo(got edog infor from parament)|
//                            |--   |YuanCloudGPSInfo|
//                            |--   |YuanCloudRadarInfo|
//                            |--   |YuanCloudPostion|
//                            |--   |YuanCloudTfafic|
//                            |--   |YuanCloudWeather|
//                            |--   |YuanCloudGSMInfo|
////////////////////////////////////////////////////////////
// callback function

//串口读取一个字节
//串口不存在数据则返回非0；
//串口存在数据则返回0；
//此函数由YuanIntReciever调用，当此函数返回非0时，退出YuanIntReciever
//参数:		
//	Data		数据指针
//返回值:
//	0 成功； 非0 失败
typedef int (*cbYuanCloudReadByte)(unsigned char* Data);

//串口读取数组
//参数:		
//	Data		数据指针
//	length		数据大小
//返回值:
//	0 成功； 非0 失败
typedef int (*cbYuanCloudReadBytes)(unsigned char* Data, int Lenght);

//串口写一个字节
//参数:		
//	Data		数据指针
//返回值:
//	0 成功； 非0 失败
typedef int (*cbYuanCloudWriteByte)(const unsigned char* Data);

//串口写数组
//参数:		
//	Data		数据指针
//	length		数据大小
//返回值:
//	0 成功； 非0 失败
typedef int (*cbYuanCloudWriteBytes)(const unsigned char* Data, int Lenght);

//云狗应答指令(YuanSendConfiguration)
//参数:		无
//返回值:
//	0 成功； 非0 失败
typedef int (*cbYuanCloudAckCfg)();

//云狗配置信息
//参数:
//   Version			版本号
//   LimitedSpeed		静音速度
//   OverSpeed			超速设置
//   MeasureMode		模式设置
//   SenMode			感度设置
//   Volume				音量设置
//返回值:
//	0 成功； 非0 失败
typedef int (*cbYuanCloudAckRequestCfg)(unsigned short SlienceSpeed,
										unsigned short LimitedSpeed,unsigned short MeasureMode,
										unsigned char  SenMode,		unsigned char  Volume);
//云狗配置信息
//参数:
//   Version			版本号
//   LimitedSpeed		静音速度
//   OverSpeed			超速设置
//   MeasureMode		模式设置
//   SenMode			感度设置
//   Volume				音量设置
//   TestMode			测试模式
//返回值:
//	0 成功； 非0 失败
typedef int (*cbYuanCloudAckRequestCfgEx)(unsigned short SlienceSpeed,
										unsigned short LimitedSpeed,unsigned short MeasureMode,
										unsigned char  SenMode,		unsigned char  Volume,
										unsigned char VoiceMode, unsigned char TraficEnable, unsigned char TestMode);

//云狗GPS数据
//参数:
//	Fixed		GPS状况： 'A' 定位成功； 'V' 未定位；
//	UTCDate		DDMMYY(230715): 2015年7月23日；
//	UTCTime		HHMMSS(135730): 13点57分30秒；
//	Latitude	经度-ddmm.nnll  	第一个字节dd;  第二个字节mm; 第三个字节nn; 第四个字节ll;
//	Longitude	经度-dddmm.nnll		第一个字节ddd; 第二个字节mm; 第三个字节nn; 第四个字节ll;
//	Speed				速度
//	Bearing				偏角-正北-逆时钟
//	SatelliteNumber		卫星数
//返回值:
//	0 成功； 非0 失败
typedef int (*cbYuanCloudGPSInfo)(char Fixed, unsigned long UTCDate, unsigned long UTCTime,
								  float Latitude,  float Longitude,
								  float Speed, float Bearing, int SatelliteNumber);
//云狗卫星信息
//参数:
//	SatelliteNumber		卫星数量；
//	Inused				卫星是否有效；
//	PRN					卫星PRN；
//	Inused				卫星Inused；
//返回值:
//	0 成功； 非0 失败
typedef int (*cbYuanCloudSatelliteInfo)(int SatelliteNumber, 
										unsigned char* PRN, unsigned char* Inused, unsigned char* SRN);

//0x01：闯红灯照相:E_DOG_239.jpg			0x02：固定测速照相:E_DOG_240.jpg				
//0x03：流动测速区:E_DOG_241.jpg			0x04：高架桥上测速照相:E_DOG_242.jpg	
//0x05：区间测速路段:E_DOG_243.jpg			0x06：右侧辅道为流动测速:E_DOG_244.jpg
//0x07：右侧辅道有超速照相:E_DOG_245.jpg	0x0A: 区间测速起点:E_DOG_243.jpg
//0x0B: 区间测速终点:E_DOG_243.jpg			0x11：桥下闯红灯辅道闯红灯:E_DOG_246.jpg
//0x12：电子监控:E_DOG_247.jpg				0x13：越线变换车道:E_DOG_248.jpg
//0x14：隧道:E_DOG_249.jpg					0x15：临时停车禁止路段:E_DOG_250.jpg
//0x16：学校路段:E_DOG_251.jpg				0x17：收费站:E_DOG_252.jpg
//0x18：单向道:E_DOG_253.jpg				0x19：禁止左转弯:E_DOG_254.jpg
//0x1A：禁止右转弯:E_DOG_255.jpg			0x1B：禁止调头:E_DOG_256.jpg					
//0x1C：禁行路段:E_DOG_257.jpg				0x1D：公交车专用道	:E_DOG_258.jpg	
//0x1E: 多雾路段:E_DOG_259.jpg				0x1F: 落石路段:E_DOG_260.jpg
//0x20: 加油站:E_DOG_261.jpg				0x21: 交流道系统交流道:E_DOG_262.jpg			
//0x22: 机场路段:E_DOG_263.jpg				0x23: 商场路段:E_DOG_264.jpg					
//0x24: 违规稽查路段:E_DOG_265.jpg			0x25: 违规酒侧路段:E_DOG_266.jpg
//0x26: 火车站:E_DOG_267.jpg				0x27: 铁路道口:E_DOG_268.jpg					
//0x28: 休息站:E_DOG_269.jpg				0x29: 急转弯路段:E_DOG_270.jpg					
//0x2A: 高速出口:E_DOG_271.jpg				0x2B: 冰雪路段:E_DOG_272.jpg
//0x2C: 事故多发路段:E_DOG_273.jpg			0x2D: 急下坡路段:E_DOG_274.jpg				
//0xFF: 取消告警
//云狗电子狗数据
//参数:
//	Type		电子狗数据类型
//	Latitude	经度-ddmm.nnll  	第一个字节dd;  第二个字节mm; 第三个字节nn; 第四个字节ll;
//	Longitude	经度-dddmm.nnll		第一个字节ddd; 第二个字节mm; 第三个字节nn; 第四个字节ll;
//	Speed				速度
//	Bearing				偏角-正北-逆时钟
//	Distance			报警点距离
//	LimitedSpeed		报警点限制车速
//返回值:
//	0 成功； 非0 失败
typedef int (*cbYuanCloudEDogInfo)(unsigned char Type, float Latitude,  float Longitude,
								 float Speed, float Bearing, 
								 float Distance, unsigned char LimitedSpeed);

//         0x00-无雷达设备
//         0x80-镭射，			E_DOG_047.jpg
//         0x58-KA频，			E_DOG_048.jpg
//         0x50-K频，			E_DOG_050.jpg
//         0x48-KU频，			E_DOG_049.jpg
//         0x40-X频；			E_DOG_046.jpg
//         0xFF-无数据
//云狗雷达数据
//参数:		
//	FrequencySeg		雷达频段；
//返回值:
//	0 成功； 非0 失败
typedef int (*cbYuanCloudRadarInfo)(unsigned char FrequencySeg);

//云狗位置信息
//参数:		
//	Provice		省，市，如（广东，广东省，上海，上海市）；
//	City		市，区，如（广州，广州市，浦东，浦东区）；
//	Country		县，市，区，如（祁阳，祁阳县，昆山，昆山市，宝安，宝安区）
//	Road		路，街道，小区，如（新湖路，西乡街道，华美居）
//返回值:
//	0 成功； 非0 失败
typedef int (*cbYuanCloudPostion)(char *Provice, char *City, char *Country, char *Road);

//云狗路况信息
//参数:		
//	Trafic		路况信息；
//返回值:
//	0 成功； 非0 失败
typedef int (*cbYuanCloudTfafic)(char *Trafic);

//云狗天气信息
//参数:		
//	Weather		天气序号
	// 0	"晴",						//白天：E_DOG_320.jpg；晚上：E_DOG_318.jpg
	// 1	"多云",						//白天：E_DOG_321.jpg；晚上：E_DOG_319.jpg
	// 2	"阴",						//E_DOG_322.jpg
	// 3	"阵雨",						//E_DOG_323.jpg
	// 4	"雷阵雨",					//E_DOG_324.jpg
	// 5	"雷阵雨伴有冰雹",			//E_DOG_325.jpg
	// 6	"雨夹雪",					//E_DOG_326.jpg
	// 7	"小雨",						//E_DOG_327.jpg
	// 8	"中雨",						//E_DOG_328.jpg
	// 9	"大雨",						//E_DOG_329.jpg
	//10	"暴雨",						//E_DOG_330.jpg
	//11	"大暴雨",					//E_DOG_331.jpg
	//12	"特大暴雨",					//E_DOG_332.jpg
	//13	"阵雪",						//E_DOG_333.jpg
	//14	"小雪",						//E_DOG_334.jpg
	//15	"中雪",						//E_DOG_335.jpg
	//16	"大雪",						//E_DOG_336.jpg
	//17	"暴雪",						//E_DOG_337.jpg
	//18	"雾",						//E_DOG_338.jpg
	//19	"冻雨",						//E_DOG_339.jpg
	//20	"沙尘暴",					//E_DOG_340.jpg
	//21	"小到中雨",					//E_DOG_341.jpg
	//22	"中到大雨",					//E_DOG_342.jpg
	//23	"大到暴雨",					//E_DOG_343.jpg
	//24	"暴雨到大暴雨",				//E_DOG_344.jpg
	//25	"大暴雨到特大暴雨",			//E_DOG_345.jpg
	//26	"小到中雪",					//E_DOG_346.jpg
	//27	"中到大雪",					//E_DOG_347.jpg
	//28	"大到暴雪",					//E_DOG_348.jpg
	//29	"浮尘",						//E_DOG_349.jpg
	//30	"扬沙",						//E_DOG_350.jpg
	//31	"强沙尘暴",					//E_DOG_351.jpg
	//32	"霾",						//E_DOG_352.jpg
//	MinTemp		最低温度
//	MaxTemp		最高温度
//	CurTemp		当前温度
//  AQI			空气质量指数
//  PM25		PM2.5
//返回值:
//	0 成功； 非0 失败
typedef int (*cbYuanCloudWeather)(int Weather, int MinTemp, int MaxTemp, int CurTemp, int AQI, int PM25);

//云狗GSM信息
//参数:		
//	Signal		GSM 信号强度（1~5: 信号强度； 99，0xFF：SIM 卡未插入; 0:无信号）
//	Cloud		Cloud 连接成功（0 未连接到服务器； 1：连接到服务器）
//返回值:
//	0 成功； 非0 失败
typedef int (*cbYuanCloudGSMInfo)(int Signal, int Cloud);

//云狗设备信息
//参数:		
//	HWVer
//	MTKVer	
//	EDogVer
//	ServVer	
//	IMEI
//	ICCID	
//	IMSI	
//返回值:
//	0 成功； 非0 失败
typedef int (*cbYuanCloudDeviceInfo)(unsigned short HWVer,
								  unsigned short MTKVer,
								  unsigned short EDogVer,
								  unsigned short ServVer,
								  unsigned char	*IMEI,
								  unsigned char *ICCID,
								  unsigned char *IMSI);

//云狗请求照片
//参数:		
//	FrameNum	Frame 号码
//返回值:
//	0 成功； 非0 失败
typedef int (*cbYuanCloudRequestPhoto)(unsigned short FrameNum);

//平台打印函数
//参数:		
//	format		
//返回值:
//	0 成功； 非0 失败
typedef int (*cbSysPrintLog)(const char *format,...);
////////////////////////////////////////////////////////////
// typedef struct
typedef struct YuanCloudCallBack
{
	cbYuanCloudReadByte				read_byte;			//串口读取一个字节
	cbYuanCloudReadBytes			read_bytes;			//串口读数组
	cbYuanCloudWriteByte			write_byte;			//串口写一个字节
	cbYuanCloudWriteBytes			write_bytes;		//串口写数组
	cbYuanCloudAckCfg 				ack_cfg;			//云狗应答指令-ACK EDOG接收到配置信息
	cbYuanCloudAckRequestCfg		ack_request_cfg;	//云狗应答指令-DVR接收到配置信息
	cbYuanCloudAckRequestCfgEx		ack_request_cfgex;	//云狗应答指令-DVR接收到扩展配置信息
	cbYuanCloudEDogInfo 			edog_info;			//云狗配置信息
	cbYuanCloudGPSInfo 				gps_info;			//云狗GPS数据
	cbYuanCloudSatelliteInfo 		satellite_info;		//云狗卫星信息
	cbYuanCloudRadarInfo			radar_info;			//云狗雷达数据
	cbYuanCloudPostion 				cloud_pos;			//云狗位置信息
	cbYuanCloudTfafic				cloud_trafic;		//云狗路况信息
	cbYuanCloudWeather 				cloud_weather;		//云狗天气信息
	cbYuanCloudGSMInfo 				cloud_gsminfo;		//云狗GSM信号
	cbYuanCloudDeviceInfo			cloud_deiveinfo;	//云狗设备信息
	cbYuanCloudRequestPhoto			cloud_requestphoto; //云狗请求照片
	cbSysPrintLog					print_log;			//平台打印函数
}YUANCLOUDCALLBACK;

////////////////////////////////////////////////////////////
// global function
//DVR初始化云狗
//参数:
//	YUANCLOUDCALLBACK		云狗回调函数
//返回值:
//	0 成功； 非0 失败
extern int YuanDogDvrInit(YUANCLOUDCALLBACK *cb_YanCloud);

//DVR发送配置信息指令给云狗
//参数:
//   LimitedSpeed		静音速度(0，20, 30, 40， 50， 60)
//   OverSpeed			超速设置(0, 40 , 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180)
//   MeasureMode		模式设置(1，2，3)(闯红灯关闭模式，照相系统模式，安全驾驶模式)
//   SenMode			感度设置(0, 1, 2)(雷达关闭，智能模式，全频模式)
//   Volume				音量设置(0, 1, 2, 3)(音量关闭，音量小，音量一般,音量大)
//     默认设置：
//       静音速度：60KM                  超速设置：无
//       感度设置：全频模式              音量设置：中
//       模式设置：安全驾驶模式
//返回值:
//	0 成功； 非0 失败
extern int YuanDvrD2ESendConfiguration(
							unsigned short SlienceSpeed,unsigned char  SenMode,  
							unsigned short OverSpeed,	unsigned short MeasureMode,
							unsigned char  Volume);
//DVR发送扩展配置信息指令给云狗
//参数:
//   LimitedSpeed		静音速度(0，20, 30, 40， 50， 60)
//   OverSpeed			超速设置(0, 40 , 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180)
//   MeasureMode		模式设置(x00~0x3F)(红绿灯照相，固定測速照相,流动測速照相, 電子監控, 警示信息, 生活信息)
//   SenMode			感度设置(0, 1, 2)(雷达关闭，全频模式，智能模式)
//   Volume				音量设置(0, 1, 2, 3)(音量关闭，音量小，音量一般,音量大)
//	 VoiceMode			语音播报(0, 1)(简洁模式，标准模式)
//	 TraficEnable		路况播报(0, 1)(路况播报关闭，路况播报开启)
//	 TestMode			测试模式(0, 1)(退出GPS测试模式，进入GPS测试模式)
//     默认设置：
//       静音速度：60KM                  超速设置：无
//       感度设置：全频模式              音量设置：中
//       模式设置：全选（安全驾驶模式）
//		 路况播报：一般模式				 语音播报：打开
//		 GPS测速模式：退出
//返回值:
//	0 成功； 非0 失败
extern int YuanDvrD2ESendConfigurationEx(
							unsigned short SlienceSpeed,unsigned char  SenMode,  
							unsigned short OverSpeed,	unsigned short MeasureMode,
							unsigned char  Volume, unsigned char VoiceMode, unsigned char TraficEnable, unsigned char TestMode);
//DVR发送请求配置信息指令给云狗
//参数:		无
//返回值:
//	0 成功； 非0 失败
extern int YuanDvrD2ESendRequestConfiguration(void);

//DVR发送请求配置信息指令给云狗
//参数:		无
//返回值:
//	0 成功； 非0 失败
extern int YuanDvrD2ESendRequestConfigurationEx(void);

//DVR发送请求云狗设备信息指令给云狗
//参数:		无
//返回值:
//	0 成功； 非0 失败
extern int YuanDvrD2ESendReqeustDevieInfo(void);

//DVR发送请求云狗信息指令给云狗
//参数:		无
//返回值:
//	0 成功； 非0 失败
extern int YuanDvrD2ESendReqeustCloud(void);

//DVR发送DVR状态
//参数:		
//   Status			0：off；
//					1：on； 
//					2：sleep
//					3：拍摄模式；
//返回值:
//	0 成功； 非0 失败
extern int YuanDvrD2ESendDvrStatus(unsigned char Status);

//DVR发送DVR状态
//参数:		
//   PriEvent			主事件
//							0	汽车制动
//							1	汽车启动
//							2	汽车停止（未制动）
//							3	汽车急刹
//							4	汽车侧滑
//							5	汽车旋转
//							6	汽车碰撞（停止时）
//							0xFF	BURST 模式
//	 SubEvent			次事件 
//返回值:
//	0 成功； 非0 失败
extern int YuanDvrD2ESendSensor(unsigned char PriEvent, unsigned char SubEvent);

//DVR发送DVR状态
//参数:		
//   TotalFrame			总帧数
//   Length				数据长度
//返回值:
//	0 成功； 非0 失败
extern int YuanDvrD2ETrigerSendPhoto(unsigned short TotalFrame, unsigned long  FileSize);

//DVR发送DVR状态
//参数:		
//   FrameNum			桢号
//   TotalFrame			总帧数
//   Length				数据长度
//   pData				数据
//返回值:
//	0 成功； 非0 失败
extern int YuanDvrD2ESendPhoto(unsigned short  FrameNum, 
								unsigned short TotalFrame,
								unsigned long  FileSize,	
								unsigned short Length,
								unsigned char  *pData);

//UART 中断接收函数
//参数:		无
//返回值:
//	status flag
extern int YuanSlaveIntReciever(void);

#ifdef __cplusplus
}
#endif

//YUANDOGDVRSLAVE_H_
#endif
