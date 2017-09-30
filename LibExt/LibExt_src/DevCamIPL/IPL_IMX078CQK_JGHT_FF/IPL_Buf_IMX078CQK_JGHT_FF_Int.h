#ifndef IPL_BUF_IMX078CQK_JGHT_FF_INT_H
#define IPL_BUF_IMX078CQK_JGHT_FF_INT_H

/**
    IPLBUF_MODE
    Mode selection for IPP-Buffer-Allocation
    Add ones if necessary
*/
typedef enum {
    IPLBUF_MODE_FLOW_A = 0,     ///< for Preview/Live-view mode
    IPLBUF_MODE_FLOW_C,         ///< for FW Trig mode
    IPLBUF_MODE_FLOW_D,         ///< for dewarp mode
    IPLBUF_MODE_CAP,            ///< for capture mode(only working buffer)
    IPLBUF_MODE_MAX_CNT,
    ENUM_DUMMY4WORD(IPLBUF_MODE)
} IPLBUF_MODE;


/**
    IPLBUF_PRV_IDX for IPLBUF_MODE_PRV
    Manage all the buffers for Image-Pipeline
    Add ones if necessary
*/
typedef enum
{
    IPLBUF_SIERAW_H_1 = 0,  ///< for SIE output raw image header
    IPLBUF_SIERAW_H_2,      ///< for SIE output raw image header
    IPLBUF_SIERAW_H_3,      ///< for SIE output raw image header
    IPLBUF_SIERAW_H_4,      ///< for SIE output raw image header
    IPLBUF_SIERAW_H_5,      ///< for SIE output raw image header
    IPLBUF_SIERAW_H_6,      ///< for SIE output raw image header
    IPLBUF_SIERAW_H_7,      ///< for SIE output raw image header
    IPLBUF_SIERAW_H_8,      ///< for SIE output raw image header
    IPLBUF_SIERAW_H_9,      ///< for SIE output raw image header
    IPLBUF_SIERAW_H_10,      ///< for SIE output raw image header
    IPLBUF_SIERAW_H_MAX,    ///< for SIE output raw image header

    IPLBUF_SIERAW_CH0_1,        ///< for SIE output raw image
    IPLBUF_SIERAW_CH0_2,        ///< for SIE output raw image
    IPLBUF_SIERAW_CH0_3,        ///< for SIE output raw image
    IPLBUF_SIERAW_CH0_4,        ///< for SIE output raw image
    IPLBUF_SIERAW_CH0_5,        ///< for SIE output raw image
    IPLBUF_SIERAW_CH0_6,        ///< for SIE output raw image
    IPLBUF_SIERAW_CH0_7,        ///< for SIE output raw image
    IPLBUF_SIERAW_CH0_8,        ///< for SIE output raw image
    IPLBUF_SIERAW_CH0_9,        ///< for SIE output raw image
    IPLBUF_SIERAW_CH0_10,        ///< for SIE output raw image
    IPLBUF_SIERAW_CH0_MAX,

    IPLBUF_SIEY_CH1_1,        ///< for SIE output Y image
    IPLBUF_SIEY_CH1_2,        ///< for SIE output Y image
    IPLBUF_SIEY_CH1_3,        ///< for SIE output Y image
    IPLBUF_SIEY_CH1_4,        ///< for SIE output Y image
    IPLBUF_SIEY_CH1_5,        ///< for SIE output Y image
    IPLBUF_SIEY_CH1_6,        ///< for SIE output Y image
    IPLBUF_SIEY_CH1_7,        ///< for SIE output Y image
    IPLBUF_SIEY_CH1_8,        ///< for SIE output Y image
    IPLBUF_SIEY_CH1_9,        ///< for SIE output Y image
    IPLBUF_SIEY_CH1_10,        ///< for SIE output Y image
    IPLBUF_SIEY_CH1_MAX,

    IPLBUF_SIEUV_CH2_1,        ///< for SIE output UV image
    IPLBUF_SIEUV_CH2_2,        ///< for SIE output UV image
    IPLBUF_SIEUV_CH2_3,        ///< for SIE output UV image
    IPLBUF_SIEUV_CH2_4,        ///< for SIE output UV image
    IPLBUF_SIEUV_CH2_5,        ///< for SIE output UV image
    IPLBUF_SIEUV_CH2_6,        ///< for SIE output UV image
    IPLBUF_SIEUV_CH2_7,        ///< for SIE output UV image
    IPLBUF_SIEUV_CH2_8,        ///< for SIE output UV image
    IPLBUF_SIEUV_CH2_9,        ///< for SIE output UV image
    IPLBUF_SIEUV_CH2_10,        ///< for SIE output UV image
    IPLBUF_SIEUV_CH2_MAX,

    IPLBUF_SIERAW_CH3_1,        ///< for SIE output sub image
    IPLBUF_SIERAW_CH3_2,        ///< for SIE output sub image
    IPLBUF_SIERAW_CH3_MAX,

    IPLBUF_SIEVA_1,         ///< for SIE output VA
    IPLBUF_SIEVA_2,         ///< for SIE output VA
    IPLBUF_SIEVA_MAX,

    IPLBUF_SIEETH_1,        ///< for SIE output ETH
    IPLBUF_SIEETH_2,        ///< for SIE output ETH
    IPLBUF_SIEETH_3,        ///< for SIE output ETH
    IPLBUF_SIEETH_MAX,

    IPLBUF_IPEOUT_1,         ///< for IPE output
    IPLBUF_IPEOUT_2,         ///< for IPE output
    IPLBUF_IPEOUT_MAX,

    IPLBUF_IFE2LCAOUT_1,   ///< for IFE2 D2D
    IPLBUF_IFE2LCAOUT_2,   ///< for IFE2 D2D
    IPLBUF_IFE2LCAOUT_MAX,

    IPLBUF_IMEP1_1,         ///< for IME YCC image output pingpong 1
    IPLBUF_IMEP1_2,         ///< for IME YCC image output pingpong 2
    IPLBUF_IMEP1_3,         ///< for IME YCC image output pingpong 3
    IPLBUF_IMEP1_4,         ///< for IME YCC image output pingpong 4
    IPLBUF_IMEP1_5,         ///< for IME YCC image output pingpong 5
    IPLBUF_IMEP1_6,         ///< for IME YCC image output pingpong 6
    IPLBUF_IMEP1_7,         ///< for IME YCC image output pingpong 7
    IPLBUF_IMEP1_8,         ///< for IME YCC image output pingpong 8
    IPLBUF_IMEP1_9,         ///< for IME YCC image output pingpong 9
    IPLBUF_IMEP1_10,        ///< for IME YCC image output pingpong 10
    IPLBUF_IMEP1_MAX,
    IPLBUF_IMEP1_SPRT,

    IPLBUF_IMEP2_1,         ///< for IME YCC image output pingpong 1
    IPLBUF_IMEP2_2,         ///< for IME YCC image output pingpong 2
    IPLBUF_IMEP2_3,         ///< for IME YCC image output pingpong 3
    IPLBUF_IMEP2_4,         ///< for IME YCC image output pingpong 4
    IPLBUF_IMEP2_5,         ///< for IME YCC image output pingpong 5
    IPLBUF_IMEP2_6,         ///< for IME YCC image output pingpong 6
    IPLBUF_IMEP2_7,         ///< for IME YCC image output pingpong 7
    IPLBUF_IMEP2_8,         ///< for IME YCC image output pingpong 8
    IPLBUF_IMEP2_9,         ///< for IME YCC image output pingpong 9
    IPLBUF_IMEP2_10,        ///< for IME YCC image output pingpong 10
    IPLBUF_IMEP2_MAX,
    IPLBUF_IMEP2_SPRT,

    IPLBUF_IMEP3_1,         ///< for IME YCC image output pingpong 1
    IPLBUF_IMEP3_2,         ///< for IME YCC image output pingpong 2
    IPLBUF_IMEP3_3,         ///< for IME YCC image output pingpong 3
    IPLBUF_IMEP3_4,         ///< for IME YCC image output pingpong 4
    IPLBUF_IMEP3_5,         ///< for IME YCC image output pingpong 5
    IPLBUF_IMEP3_6,         ///< for IME YCC image output pingpong 6
    IPLBUF_IMEP3_7,         ///< for IME YCC image output pingpong 7
    IPLBUF_IMEP3_8,         ///< for IME YCC image output pingpong 8
    IPLBUF_IMEP3_9,         ///< for IME YCC image output pingpong 9
    IPLBUF_IMEP3_10,        ///< for IME YCC image output pingpong 10
    IPLBUF_IMEP3_MAX,
    IPLBUF_IMEP3_SPRT,

    IPLBUF_IMEP4_1,         ///< for IME YCC image output pingpong 1
    IPLBUF_IMEP4_2,         ///< for IME YCC image output pingpong 2
    IPLBUF_IMEP4_3,         ///< for IME YCC image output pingpong 3
    IPLBUF_IMEP4_4,         ///< for IME YCC image output pingpong 4
    IPLBUF_IMEP4_5,         ///< for IME YCC image output pingpong 5
    IPLBUF_IMEP4_6,         ///< for IME YCC image output pingpong 6
    IPLBUF_IMEP4_7,         ///< for IME YCC image output pingpong 7
    IPLBUF_IMEP4_8,         ///< for IME YCC image output pingpong 8
    IPLBUF_IMEP4_9,         ///< for IME YCC image output pingpong 9
    IPLBUF_IMEP4_10,        ///< for IME YCC image output pingpong 10
    IPLBUF_IMEP4_MAX,

    IQUCBUF_MCTFOUT_1,
    IQUCBUF_MCTFOUT_2,
    IQUCBUF_MCTFOUT_MAX,

    IPLBUF_PRV_MAX_CNT,
    ENUM_DUMMY4WORD(IPLBUF_PRV_IDX)
} IPLBUF_PRV_IDX;

//check IPLBUF_PRV_IDX range
STATIC_ASSERT((IPLBUF_PRV_MAX_CNT <= IPL_BUF_ELEMENT_MAX));

typedef enum
{
    IPLBUF_DRE_DBGBUF,

    IPLBUF_CAP_MAX_CNT,
    ENUM_DUMMY4WORD(IPLBUF_CAP_IDX)
} IPLBUF_CAP_IDX;

STATIC_ASSERT((IPLBUF_CAP_MAX_CNT <= IPL_BUF_ELEMENT_MAX));

/**
     IPLBUF Assigner result

     Detailed description.
     @note (Optional)
*/
typedef struct _IPPBUF_RST
{
    ER     er_code;         ///< Description
    UINT32 BufSize;         ///< required buffer size
}IPLBUF_RST,*PIPLBUF_RST;

typedef struct
{
    UINT32 BufNum;
    UINT32 Width;
    UINT32 Height;
    UINT32 LineOfs;
    UINT32 Bit;
} IPLBUF_RAWINFO;

typedef struct
{
    UINT32 BufNum;
    UINT32 Width;
    UINT32 Height;
    UINT32 LineOfs;
    IPL_YUV_IMG_TYPE Fmt;
} IPLBUF_YCCINFO;

typedef struct
{
    UINT32 BufNum;
    UINT32 Width;
    UINT32 Height;
    UINT32 LineOfs;
    UINT32 HistBufOfs;
    UINT32 EdgeMapBufOfs;
} IPLBUF_IMEP4INFO;

typedef struct
{
    IPL_PROC_ID Id;

    BOOL SIECh0En;
    IPLBUF_RAWINFO SIEOutCh0;

    //ch1
    BOOL SIECh1En;  //output Y when ccir
    IPLBUF_YCCINFO SIEOutCh1;

    BOOL SIECAEn;
    UINT32 SIECABufNum;
    UINT32 SIECABufSize;

    //ch2
    BOOL SIECh2En;    //output UV when ccir
    IPLBUF_YCCINFO SIEOutCh2;

    BOOL SIELAEn;
    UINT32 SIELABufNum;
    UINT32 SIELABufSize;

    //ch3
    BOOL SIECh3En;  //output sub-img when cap
    IPLBUF_RAWINFO SIEOutCh3;

    BOOL SIEVAEn;
    UINT32 SIEVABufNum;
    UINT32 SIEVABufSize;

    BOOL SIEEthEn;  //ch4
    UINT32 SIEEthSize;

    BOOL IPEOutEn;
    IPLBUF_YCCINFO IPEOut;

    BOOL IFE2LCAEn;
    UINT32 IFE2LCABufNum;

    BOOL IMEPath1En;
    IPLBUF_YCCINFO IMEPath1;

    BOOL IMEPath2En;
    IPLBUF_YCCINFO IMEPath2;

    BOOL IMEPath3En;
    IPLBUF_YCCINFO IMEPath3;

    BOOL IMEPath4En;
    IPLBUF_IMEP4INFO IMEPath4;
}IPLBUF_ENG2DRAMINFO;

typedef IPLBUF_RST (*FPIPPBUFASSIGNER)(IPLBUF_ENG2DRAMINFO* Data);


extern IPLBUF_RST IPL_BufAssign(IPLBUF_MODE ModeIdx, IPLBUF_ENG2DRAMINFO *Data);

#endif //IPL_BUF_SAMPLE_H_
