#ifndef YUANDOGHALFILE_H__
#define YUANDOGHALFILE_H__

#include "YuanDogUtils.h"

#ifdef __cplusplus
extern "C"{
#endif 

/////////////////////////////////////////////////////////////////////
// global function

/////////////////////////////////////////////////////////////////////
// config file function	
//打开配置文件
//参数:				无
//返回值:
//	1: 成功；	0: 失败；
int	YuanHalConfigFileOpen(void);

//关闭配置文件
//参数:				无
//返回值:
//	1: 成功；	0: 失败；
int	YuanHalConfigFileClose(void);

//从配置文件中读取一行文本
//参数:
//	pBuf			字符串数组指针
//	Count			字符串数组长度
//返回值:
//	返回字符串指针；
char*	YuanHalConfigFileGets(char* pBuf,uint Count);

/////////////////////////////////////////////////////////////////////
// A-GPS file function	

//销毁A-GPS文件内容，文件保留
//参数:				无
//返回值:			无
void	YuanHalaGpsDestory(void);

//增加A-GPS数据
//参数:
//	pBuf			数据指针
//	nBytes			数据长度
//返回值:
//	1: 成功；	0: 失败；
int	YuanHalaGpsFileSet(void* pBuf, uint nBytes);

//取A-GPS数据库内容
//参数:
//	pBuf			数据指针
//	nBytes			数据长度
//返回值:
//	1: 成功；	0: 失败；
int	YuanHalaGpsFileGet(uint nPosition, void* pBuf, uint nBytes);
/////////////////////////////////////////////////////////////////////
// e-dog file(db/index) function	
//初始化电子狗数据库文件
//参数:				无
//返回值:			
//	1: 成功；	0: 失败；；
int	YuanHalDogDBInit(void);

//销毁电子狗数据库文件内容，文件保留
//参数:				无
//返回值:			无
void	YuanHalDogDBDestory(void);

//获取电子狗数据库长度
//参数:				无
//返回值:			
//	返回电子狗数据库长度
uint	YuanHalDogDBLength(void);

//封包电子狗数据库头
//参数:				无
//返回值:			
//	返回电子狗数据库长度
uint	YuanHalPackageDogDBHead(void);

//写电子狗数据库内容
//参数:
//	nPosition		数据位置，偏移量；
//	pBuf			数据指针
//	nBytes			数据长度
//返回值:
//	1: 成功；	0: 失败；
int	YuanHalDogDBSet(uint nPosition, uint8* pBuf, uint nBytes);

//取电子狗数据库内容
//参数:
//	nPosition		数据位置，偏移量；
//	pBuf			数据指针
//	nBytes			数据长度
//返回值:
//	1: 成功；	0: 失败；
int	YuanHalDogDBGet(uint nPosition, void* pBuf, uint nBytes);

//初始化电子狗数据库索引文件
//参数:				无
//返回值:			
//	1: 成功；	0: 失败；
int	YuanHalDogIndexInit(void);

//销毁电子狗数据库索引文件内容，文件保留
//参数:				无
//返回值:			无
void	YuanHalDogIndexDestory(void);

//获取电子狗数据库索引文件长度
//参数:				无
//返回值:			
//	返回电子狗数据库索引文件长度
uint	YuanHalDogIndexLength(void);

//写电子狗数据库索引文件内容
//参数:
//	nPosition		数据位置，偏移量；
//	pBuf			数据指针
//	nBytes			数据长度
//返回值:
//	1: 成功；	0: 失败；
int	YuanHalDogIndexSet(uint nPosition, uint8* pBuf, uint nBytes);

//取电子狗数据库索引文件内容
//参数:
//	nPosition		数据位置，偏移量；
//	pBuf			数据指针
//	nBytes			数据长度
//返回值:
//	1: 成功；	0: 失败；
int		YuanHalDogIndexGet(uint nPosition, void* pBuf, uint nBytes);

/////////////////////////////////////////////////////////////////////
// NVRM file function	
//写NVRM内容
//参数:
//	nPosition		数据位置，偏移量；
//	pBuf			数据指针
//	nBytes			数据长度
//返回值:
//	1: 成功；	0: 失败；
int		YuanHalNVRMSet(uint nPosition, uint8* pBuf, uint nBytes);

//读NVRM内容
//参数:
//	nPosition		数据位置，偏移量；
//	pBuf			数据指针
//	nBytes			数据长度
//返回值:
//	1: 成功；	0: 失败；
int		YuanHalNVRMGet(uint nPosition, uint8* pBuf, uint nBytes);

//获取NVRM文件长度
//参数:				无
//返回值:			
//	返回电子狗数据库索引文件长度
uint	YuanHalNVRMLength(void);

/////////////////////////////////////////////////////////////////////
// YBT file function	
//打开配置文件
//参数:				无
//返回值:
//	1: 成功；	0: 失败；
int		YuanYBTDBFileOpen(const char* FileName);

//从YBT文件中读取一行文本
//参数:
//	pBuf			字符串数组指针
//	Count			字符串数组长度
//返回值:
//	返回字符串指针；
char*	YuanYBTDBFileGets(char* pBuf,uint Count);

//关闭YBT文件
//参数:				无
//返回值:
//	1: 成功；	0: 失败；
int	YuanYBTDBFileClose(void);


//读取电子狗升级数据包
//参数:				
//	start				电子狗数据索引
//	buffer				电子狗数据指针
//	buffer_size			电子狗数据长度（256条数据）
//返回值
//  读取到的字节数
unsigned int YuanHalDogDBLoadEx(unsigned int start, void *buffer, unsigned int buffer_len);

//下载电子狗数据升级包
//参数:
//	ver				电子狗数据版本号
//	url				升级电子狗数据链接地址
//	onCompleted		下载结束回调函数
//	onError			下载失败回调函数
//	data			下载结束回调函数参数
//返回值:
//	0: 成功；	-1: 失败；
int YuanHalDogDBUpdate(int ver, const char *url, void (*onCompleted)(int ver, void *), void (*onError)(int ver, void *), void *data);

//开始上传文件到服务器
//参数：
// url				上传地址
// imei				设备IMEI
// filename			文件名,传递NULL时，默认扩展名为jpg, 传递其它值时使用该值作为保存文件的文件名的最后字符，
//					比如filename="photo.png",则最终上传后的文件名为 "xxxxxxphoto.png",其中xxxxxx为服务器生成的类容
// size				文件长度
// onUploadStatus	上传执行状态，code为状态码，见下，msg为错误信息，id为记录编号，url为接收的路径
// handle			回调函数的参数
//返回值:
//	指向上传对象的一个指针，在YuanHalDogUploadPut中的第一个参数

enum {
	YUANDOG_UPLOAD_OK			=0,		//成功
	YUANDOG_UPLOAD_COMPLETED	=1,		//上传结束
	YUANDOG_UPLOAD_BUFFERED		=2,		//缓冲区可写
	YUANDOG_UPLOAD_ERR			=-1,	//错误
	YUANDOG_UPLOAD_PENDING		=-2		//缓冲区满
};

void* YuanHalDogUploadStart(const char *url,
						  const char *imei,
						  const char *filename,
						  int size,
						  void (*onUploadStatus)(int code, const char *msg, int id, const char *url, void *handle),
						  void *handle);
//发送上传的数据
//参数：
//	handle：上传对象的指针，由YuanHalDogUploadStart返回得到
//	buf：需要上传的数据指针
//	size：需要上传的字节数
//返回值：
//	YUANDOG_UPLOAD_ERR
//	YUANDOG_UPLOAD_PENDING
//	>0 实际缓冲的字节数
int YuanHalDogUploadPut(void *handle, void *buf, int size);
#ifdef __cplusplus
}
#endif

#endif