#ifndef SIMSERIALPORT_H_
#define SIMSERIALPORT_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*cbsimSerialPortRead)(const unsigned char* pBuf, int nBytes);

extern int simSerialPortHostInit(cbsimSerialPortRead simSerialPortHostRead);
extern int simSerialPortSlaveInit(cbsimSerialPortRead simSerialPortSlaveRead);

extern int simSerialPortHostWrites(const unsigned char* pBuf, int nBytes);
extern int simSerialPortSlaveWrites(const unsigned char* pBuf, int nBytes); 

#ifdef __cplusplus
}
#endif

#endif
