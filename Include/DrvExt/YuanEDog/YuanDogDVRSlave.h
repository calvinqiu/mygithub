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

//���ڶ�ȡһ���ֽ�
//���ڲ����������򷵻ط�0��
//���ڴ��������򷵻�0��
//�˺�����YuanIntReciever���ã����˺������ط�0ʱ���˳�YuanIntReciever
//����:		
//	Data		����ָ��
//����ֵ:
//	0 �ɹ��� ��0 ʧ��
typedef int (*cbYuanCloudReadByte)(unsigned char* Data);

//���ڶ�ȡ����
//����:		
//	Data		����ָ��
//	length		���ݴ�С
//����ֵ:
//	0 �ɹ��� ��0 ʧ��
typedef int (*cbYuanCloudReadBytes)(unsigned char* Data, int Lenght);

//����дһ���ֽ�
//����:		
//	Data		����ָ��
//����ֵ:
//	0 �ɹ��� ��0 ʧ��
typedef int (*cbYuanCloudWriteByte)(const unsigned char* Data);

//����д����
//����:		
//	Data		����ָ��
//	length		���ݴ�С
//����ֵ:
//	0 �ɹ��� ��0 ʧ��
typedef int (*cbYuanCloudWriteBytes)(const unsigned char* Data, int Lenght);

//�ƹ�Ӧ��ָ��(YuanSendConfiguration)
//����:		��
//����ֵ:
//	0 �ɹ��� ��0 ʧ��
typedef int (*cbYuanCloudAckCfg)();

//�ƹ�������Ϣ
//����:
//   Version			�汾��
//   LimitedSpeed		�����ٶ�
//   OverSpeed			��������
//   MeasureMode		ģʽ����
//   SenMode			�ж�����
//   Volume				��������
//����ֵ:
//	0 �ɹ��� ��0 ʧ��
typedef int (*cbYuanCloudAckRequestCfg)(unsigned short SlienceSpeed,
										unsigned short LimitedSpeed,unsigned short MeasureMode,
										unsigned char  SenMode,		unsigned char  Volume);
//�ƹ�������Ϣ
//����:
//   Version			�汾��
//   LimitedSpeed		�����ٶ�
//   OverSpeed			��������
//   MeasureMode		ģʽ����
//   SenMode			�ж�����
//   Volume				��������
//   TestMode			����ģʽ
//����ֵ:
//	0 �ɹ��� ��0 ʧ��
typedef int (*cbYuanCloudAckRequestCfgEx)(unsigned short SlienceSpeed,
										unsigned short LimitedSpeed,unsigned short MeasureMode,
										unsigned char  SenMode,		unsigned char  Volume,
										unsigned char VoiceMode, unsigned char TraficEnable, unsigned char TestMode);

//�ƹ�GPS����
//����:
//	Fixed		GPS״���� 'A' ��λ�ɹ��� 'V' δ��λ��
//	UTCDate		DDMMYY(230715): 2015��7��23�գ�
//	UTCTime		HHMMSS(135730): 13��57��30�룻
//	Latitude	����-ddmm.nnll  	��һ���ֽ�dd;  �ڶ����ֽ�mm; �������ֽ�nn; ���ĸ��ֽ�ll;
//	Longitude	����-dddmm.nnll		��һ���ֽ�ddd; �ڶ����ֽ�mm; �������ֽ�nn; ���ĸ��ֽ�ll;
//	Speed				�ٶ�
//	Bearing				ƫ��-����-��ʱ��
//	SatelliteNumber		������
//����ֵ:
//	0 �ɹ��� ��0 ʧ��
typedef int (*cbYuanCloudGPSInfo)(char Fixed, unsigned long UTCDate, unsigned long UTCTime,
								  float Latitude,  float Longitude,
								  float Speed, float Bearing, int SatelliteNumber);
//�ƹ�������Ϣ
//����:
//	SatelliteNumber		����������
//	Inused				�����Ƿ���Ч��
//	PRN					����PRN��
//	Inused				����Inused��
//����ֵ:
//	0 �ɹ��� ��0 ʧ��
typedef int (*cbYuanCloudSatelliteInfo)(int SatelliteNumber, 
										unsigned char* PRN, unsigned char* Inused, unsigned char* SRN);

//0x01�����������:E_DOG_239.jpg			0x02���̶���������:E_DOG_240.jpg				
//0x03������������:E_DOG_241.jpg			0x04���߼����ϲ�������:E_DOG_242.jpg	
//0x05���������·��:E_DOG_243.jpg			0x06���Ҳศ��Ϊ��������:E_DOG_244.jpg
//0x07���Ҳศ���г�������:E_DOG_245.jpg	0x0A: ����������:E_DOG_243.jpg
//0x0B: ��������յ�:E_DOG_243.jpg			0x11�����´���Ƹ��������:E_DOG_246.jpg
//0x12�����Ӽ��:E_DOG_247.jpg				0x13��Խ�߱任����:E_DOG_248.jpg
//0x14�����:E_DOG_249.jpg					0x15����ʱͣ����ֹ·��:E_DOG_250.jpg
//0x16��ѧУ·��:E_DOG_251.jpg				0x17���շ�վ:E_DOG_252.jpg
//0x18�������:E_DOG_253.jpg				0x19����ֹ��ת��:E_DOG_254.jpg
//0x1A����ֹ��ת��:E_DOG_255.jpg			0x1B����ֹ��ͷ:E_DOG_256.jpg					
//0x1C������·��:E_DOG_257.jpg				0x1D��������ר�õ�	:E_DOG_258.jpg	
//0x1E: ����·��:E_DOG_259.jpg				0x1F: ��ʯ·��:E_DOG_260.jpg
//0x20: ����վ:E_DOG_261.jpg				0x21: ������ϵͳ������:E_DOG_262.jpg			
//0x22: ����·��:E_DOG_263.jpg				0x23: �̳�·��:E_DOG_264.jpg					
//0x24: Υ�����·��:E_DOG_265.jpg			0x25: Υ��Ʋ�·��:E_DOG_266.jpg
//0x26: ��վ:E_DOG_267.jpg				0x27: ��·����:E_DOG_268.jpg					
//0x28: ��Ϣվ:E_DOG_269.jpg				0x29: ��ת��·��:E_DOG_270.jpg					
//0x2A: ���ٳ���:E_DOG_271.jpg				0x2B: ��ѩ·��:E_DOG_272.jpg
//0x2C: �¹ʶ෢·��:E_DOG_273.jpg			0x2D: ������·��:E_DOG_274.jpg				
//0xFF: ȡ���澯
//�ƹ����ӹ�����
//����:
//	Type		���ӹ���������
//	Latitude	����-ddmm.nnll  	��һ���ֽ�dd;  �ڶ����ֽ�mm; �������ֽ�nn; ���ĸ��ֽ�ll;
//	Longitude	����-dddmm.nnll		��һ���ֽ�ddd; �ڶ����ֽ�mm; �������ֽ�nn; ���ĸ��ֽ�ll;
//	Speed				�ٶ�
//	Bearing				ƫ��-����-��ʱ��
//	Distance			���������
//	LimitedSpeed		���������Ƴ���
//����ֵ:
//	0 �ɹ��� ��0 ʧ��
typedef int (*cbYuanCloudEDogInfo)(unsigned char Type, float Latitude,  float Longitude,
								 float Speed, float Bearing, 
								 float Distance, unsigned char LimitedSpeed);

//         0x00-���״��豸
//         0x80-���䣬			E_DOG_047.jpg
//         0x58-KAƵ��			E_DOG_048.jpg
//         0x50-KƵ��			E_DOG_050.jpg
//         0x48-KUƵ��			E_DOG_049.jpg
//         0x40-XƵ��			E_DOG_046.jpg
//         0xFF-������
//�ƹ��״�����
//����:		
//	FrequencySeg		�״�Ƶ�Σ�
//����ֵ:
//	0 �ɹ��� ��0 ʧ��
typedef int (*cbYuanCloudRadarInfo)(unsigned char FrequencySeg);

//�ƹ�λ����Ϣ
//����:		
//	Provice		ʡ���У��磨�㶫���㶫ʡ���Ϻ����Ϻ��У���
//	City		�У������磨���ݣ������У��ֶ����ֶ�������
//	Country		�أ��У������磨�����������أ���ɽ����ɽ�У���������������
//	Road		·���ֵ���С�����磨�º�·������ֵ��������ӣ�
//����ֵ:
//	0 �ɹ��� ��0 ʧ��
typedef int (*cbYuanCloudPostion)(char *Provice, char *City, char *Country, char *Road);

//�ƹ�·����Ϣ
//����:		
//	Trafic		·����Ϣ��
//����ֵ:
//	0 �ɹ��� ��0 ʧ��
typedef int (*cbYuanCloudTfafic)(char *Trafic);

//�ƹ�������Ϣ
//����:		
//	Weather		�������
	// 0	"��",						//���죺E_DOG_320.jpg�����ϣ�E_DOG_318.jpg
	// 1	"����",						//���죺E_DOG_321.jpg�����ϣ�E_DOG_319.jpg
	// 2	"��",						//E_DOG_322.jpg
	// 3	"����",						//E_DOG_323.jpg
	// 4	"������",					//E_DOG_324.jpg
	// 5	"��������б���",			//E_DOG_325.jpg
	// 6	"���ѩ",					//E_DOG_326.jpg
	// 7	"С��",						//E_DOG_327.jpg
	// 8	"����",						//E_DOG_328.jpg
	// 9	"����",						//E_DOG_329.jpg
	//10	"����",						//E_DOG_330.jpg
	//11	"����",					//E_DOG_331.jpg
	//12	"�ش���",					//E_DOG_332.jpg
	//13	"��ѩ",						//E_DOG_333.jpg
	//14	"Сѩ",						//E_DOG_334.jpg
	//15	"��ѩ",						//E_DOG_335.jpg
	//16	"��ѩ",						//E_DOG_336.jpg
	//17	"��ѩ",						//E_DOG_337.jpg
	//18	"��",						//E_DOG_338.jpg
	//19	"����",						//E_DOG_339.jpg
	//20	"ɳ����",					//E_DOG_340.jpg
	//21	"С������",					//E_DOG_341.jpg
	//22	"�е�����",					//E_DOG_342.jpg
	//23	"�󵽱���",					//E_DOG_343.jpg
	//24	"���굽����",				//E_DOG_344.jpg
	//25	"���굽�ش���",			//E_DOG_345.jpg
	//26	"С����ѩ",					//E_DOG_346.jpg
	//27	"�е���ѩ",					//E_DOG_347.jpg
	//28	"�󵽱�ѩ",					//E_DOG_348.jpg
	//29	"����",						//E_DOG_349.jpg
	//30	"��ɳ",						//E_DOG_350.jpg
	//31	"ǿɳ����",					//E_DOG_351.jpg
	//32	"��",						//E_DOG_352.jpg
//	MinTemp		����¶�
//	MaxTemp		����¶�
//	CurTemp		��ǰ�¶�
//  AQI			��������ָ��
//  PM25		PM2.5
//����ֵ:
//	0 �ɹ��� ��0 ʧ��
typedef int (*cbYuanCloudWeather)(int Weather, int MinTemp, int MaxTemp, int CurTemp, int AQI, int PM25);

//�ƹ�GSM��Ϣ
//����:		
//	Signal		GSM �ź�ǿ�ȣ�1~5: �ź�ǿ�ȣ� 99��0xFF��SIM ��δ����; 0:���źţ�
//	Cloud		Cloud ���ӳɹ���0 δ���ӵ��������� 1�����ӵ���������
//����ֵ:
//	0 �ɹ��� ��0 ʧ��
typedef int (*cbYuanCloudGSMInfo)(int Signal, int Cloud);

//�ƹ��豸��Ϣ
//����:		
//	HWVer
//	MTKVer	
//	EDogVer
//	ServVer	
//	IMEI
//	ICCID	
//	IMSI	
//����ֵ:
//	0 �ɹ��� ��0 ʧ��
typedef int (*cbYuanCloudDeviceInfo)(unsigned short HWVer,
								  unsigned short MTKVer,
								  unsigned short EDogVer,
								  unsigned short ServVer,
								  unsigned char	*IMEI,
								  unsigned char *ICCID,
								  unsigned char *IMSI);

//�ƹ�������Ƭ
//����:		
//	FrameNum	Frame ����
//����ֵ:
//	0 �ɹ��� ��0 ʧ��
typedef int (*cbYuanCloudRequestPhoto)(unsigned short FrameNum);

//ƽ̨��ӡ����
//����:		
//	format		
//����ֵ:
//	0 �ɹ��� ��0 ʧ��
typedef int (*cbSysPrintLog)(const char *format,...);
////////////////////////////////////////////////////////////
// typedef struct
typedef struct YuanCloudCallBack
{
	cbYuanCloudReadByte				read_byte;			//���ڶ�ȡһ���ֽ�
	cbYuanCloudReadBytes			read_bytes;			//���ڶ�����
	cbYuanCloudWriteByte			write_byte;			//����дһ���ֽ�
	cbYuanCloudWriteBytes			write_bytes;		//����д����
	cbYuanCloudAckCfg 				ack_cfg;			//�ƹ�Ӧ��ָ��-ACK EDOG���յ�������Ϣ
	cbYuanCloudAckRequestCfg		ack_request_cfg;	//�ƹ�Ӧ��ָ��-DVR���յ�������Ϣ
	cbYuanCloudAckRequestCfgEx		ack_request_cfgex;	//�ƹ�Ӧ��ָ��-DVR���յ���չ������Ϣ
	cbYuanCloudEDogInfo 			edog_info;			//�ƹ�������Ϣ
	cbYuanCloudGPSInfo 				gps_info;			//�ƹ�GPS����
	cbYuanCloudSatelliteInfo 		satellite_info;		//�ƹ�������Ϣ
	cbYuanCloudRadarInfo			radar_info;			//�ƹ��״�����
	cbYuanCloudPostion 				cloud_pos;			//�ƹ�λ����Ϣ
	cbYuanCloudTfafic				cloud_trafic;		//�ƹ�·����Ϣ
	cbYuanCloudWeather 				cloud_weather;		//�ƹ�������Ϣ
	cbYuanCloudGSMInfo 				cloud_gsminfo;		//�ƹ�GSM�ź�
	cbYuanCloudDeviceInfo			cloud_deiveinfo;	//�ƹ��豸��Ϣ
	cbYuanCloudRequestPhoto			cloud_requestphoto; //�ƹ�������Ƭ
	cbSysPrintLog					print_log;			//ƽ̨��ӡ����
}YUANCLOUDCALLBACK;

////////////////////////////////////////////////////////////
// global function
//DVR��ʼ���ƹ�
//����:
//	YUANCLOUDCALLBACK		�ƹ��ص�����
//����ֵ:
//	0 �ɹ��� ��0 ʧ��
extern int YuanDogDvrInit(YUANCLOUDCALLBACK *cb_YanCloud);

//DVR����������Ϣָ����ƹ�
//����:
//   LimitedSpeed		�����ٶ�(0��20, 30, 40�� 50�� 60)
//   OverSpeed			��������(0, 40 , 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180)
//   MeasureMode		ģʽ����(1��2��3)(����ƹر�ģʽ������ϵͳģʽ����ȫ��ʻģʽ)
//   SenMode			�ж�����(0, 1, 2)(�״�رգ�����ģʽ��ȫƵģʽ)
//   Volume				��������(0, 1, 2, 3)(�����رգ�����С������һ��,������)
//     Ĭ�����ã�
//       �����ٶȣ�60KM                  �������ã���
//       �ж����ã�ȫƵģʽ              �������ã���
//       ģʽ���ã���ȫ��ʻģʽ
//����ֵ:
//	0 �ɹ��� ��0 ʧ��
extern int YuanDvrD2ESendConfiguration(
							unsigned short SlienceSpeed,unsigned char  SenMode,  
							unsigned short OverSpeed,	unsigned short MeasureMode,
							unsigned char  Volume);
//DVR������չ������Ϣָ����ƹ�
//����:
//   LimitedSpeed		�����ٶ�(0��20, 30, 40�� 50�� 60)
//   OverSpeed			��������(0, 40 , 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180)
//   MeasureMode		ģʽ����(x00~0x3F)(���̵����࣬�̶��y������,�����y������, ��ӱO��, ��ʾ��Ϣ, ������Ϣ)
//   SenMode			�ж�����(0, 1, 2)(�״�رգ�ȫƵģʽ������ģʽ)
//   Volume				��������(0, 1, 2, 3)(�����رգ�����С������һ��,������)
//	 VoiceMode			��������(0, 1)(���ģʽ����׼ģʽ)
//	 TraficEnable		·������(0, 1)(·�������رգ�·����������)
//	 TestMode			����ģʽ(0, 1)(�˳�GPS����ģʽ������GPS����ģʽ)
//     Ĭ�����ã�
//       �����ٶȣ�60KM                  �������ã���
//       �ж����ã�ȫƵģʽ              �������ã���
//       ģʽ���ã�ȫѡ����ȫ��ʻģʽ��
//		 ·��������һ��ģʽ				 ������������
//		 GPS����ģʽ���˳�
//����ֵ:
//	0 �ɹ��� ��0 ʧ��
extern int YuanDvrD2ESendConfigurationEx(
							unsigned short SlienceSpeed,unsigned char  SenMode,  
							unsigned short OverSpeed,	unsigned short MeasureMode,
							unsigned char  Volume, unsigned char VoiceMode, unsigned char TraficEnable, unsigned char TestMode);
//DVR��������������Ϣָ����ƹ�
//����:		��
//����ֵ:
//	0 �ɹ��� ��0 ʧ��
extern int YuanDvrD2ESendRequestConfiguration(void);

//DVR��������������Ϣָ����ƹ�
//����:		��
//����ֵ:
//	0 �ɹ��� ��0 ʧ��
extern int YuanDvrD2ESendRequestConfigurationEx(void);

//DVR���������ƹ��豸��Ϣָ����ƹ�
//����:		��
//����ֵ:
//	0 �ɹ��� ��0 ʧ��
extern int YuanDvrD2ESendReqeustDevieInfo(void);

//DVR���������ƹ���Ϣָ����ƹ�
//����:		��
//����ֵ:
//	0 �ɹ��� ��0 ʧ��
extern int YuanDvrD2ESendReqeustCloud(void);

//DVR����DVR״̬
//����:		
//   Status			0��off��
//					1��on�� 
//					2��sleep
//					3������ģʽ��
//����ֵ:
//	0 �ɹ��� ��0 ʧ��
extern int YuanDvrD2ESendDvrStatus(unsigned char Status);

//DVR����DVR״̬
//����:		
//   PriEvent			���¼�
//							0	�����ƶ�
//							1	��������
//							2	����ֹͣ��δ�ƶ���
//							3	������ɲ
//							4	�����໬
//							5	������ת
//							6	������ײ��ֹͣʱ��
//							0xFF	BURST ģʽ
//	 SubEvent			���¼� 
//����ֵ:
//	0 �ɹ��� ��0 ʧ��
extern int YuanDvrD2ESendSensor(unsigned char PriEvent, unsigned char SubEvent);

//DVR����DVR״̬
//����:		
//   TotalFrame			��֡��
//   Length				���ݳ���
//����ֵ:
//	0 �ɹ��� ��0 ʧ��
extern int YuanDvrD2ETrigerSendPhoto(unsigned short TotalFrame, unsigned long  FileSize);

//DVR����DVR״̬
//����:		
//   FrameNum			���
//   TotalFrame			��֡��
//   Length				���ݳ���
//   pData				����
//����ֵ:
//	0 �ɹ��� ��0 ʧ��
extern int YuanDvrD2ESendPhoto(unsigned short  FrameNum, 
								unsigned short TotalFrame,
								unsigned long  FileSize,	
								unsigned short Length,
								unsigned char  *pData);

//UART �жϽ��պ���
//����:		��
//����ֵ:
//	status flag
extern int YuanSlaveIntReciever(void);

#ifdef __cplusplus
}
#endif

//YUANDOGDVRSLAVE_H_
#endif
