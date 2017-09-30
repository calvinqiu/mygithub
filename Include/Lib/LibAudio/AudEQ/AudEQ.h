/**
    Audio Equalizer library Driver header file.

    This file is the driver of Audio Equalizer library.
    This library provides fixed 5-band equalizer. The 5 frequency bands are at 120/500/1500/5000/10000 Hz.
    And the user can adjust the gain range from -12 to +12 dB for each frequency band (1dB per step).
    The input audio stream can Mono or Stereo and the sampling rate can be 32/44.1/48 KHz.

    @file       AudEQ.h
    @ingroup    mIAudEQ
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2015.  All rights reserved.
*/
#ifndef _AUDEQ_H
#define _AUDEQ_H

#include "Type.h"

/**
    @addtogroup mIAudEQ
*/
//@{


/**
    Audio Equalizer Channel Number

    This is used in AudEQ_SetConfig(AUDEQ_CONFIG_ID_CHANNEL_NO) to specify the audio channel number.
*/
typedef enum
{
    AUDEQ_CH_MONO   = 1,    ///< MONO   Channel.
    AUDEQ_CH_STEREO = 2,    ///< STEREO Channel.

    ENUM_DUMMY4WORD(AUDEQ_CH)
}AUDEQ_CH;


/**
    Audio Equalizer Sampling Rate

    This is the supporting sampling rate list for the Equalizer library and is used in AudEQ_SetConfig(AUDEQ_CONFIG_ID_SAMPLE_RATE)
    to assign the audio bit stream sampling rate.
*/
typedef enum
{
    AUDEQ_SR_32000  = 32000,    ///< Audio Bit Stream sampling rate is 32 KHz.
    AUDEQ_SR_44100  = 44100,    ///< Audio Bit Stream sampling rate is 44.1 KHz.
    AUDEQ_SR_48000  = 48000,    ///< Audio Bit Stream sampling rate is 48 KHz.

    ENUM_DUMMY4WORD(AUDEQ_SR)
}AUDEQ_SR;


/**
    Audio Equalizer Configuration Select ID

    This is used in AudEQ_SetConfig() as the select ID to assign new configurations
    for Audio Equalizer library.
*/
typedef enum
{
    AUDEQ_CONFIG_ID_CHANNEL_NO,     ///< Configure Channel No. Please use AUDEQ_CH_MONO or AUDEQ_CH_STEREO as iEqConfig.
    AUDEQ_CONFIG_ID_SAMPLE_RATE,    ///< Configure SampleRate. Please use AUDEQ_SR_32000/44100/48000 as iEqConfig.

    AUDEQ_CONFIG_ID_BAND120Hz,      ///< Configure Gain value for band-120Hz.  Allow range is from -12 ~ +12 dB.
    AUDEQ_CONFIG_ID_BAND500Hz,      ///< Configure Gain value for band-500Hz.  Allow range is from -12 ~ +12 dB.
    AUDEQ_CONFIG_ID_BAND1500Hz,     ///< Configure Gain value for band-1500Hz. Allow range is from -12 ~ +12 dB.
    AUDEQ_CONFIG_ID_BAND5000Hz,     ///< Configure Gain value for band-5000Hz. Allow range is from -12 ~ +12 dB.
    AUDEQ_CONFIG_ID_BAND10000Hz,    ///< Configure Gain value for band-10000Hz.Allow range is from -12 ~ +12 dB.

    ENUM_DUMMY4WORD(AUDEQ_CONFIG_ID)
}AUDEQ_CONFIG_ID;





/**
    Structure for audio Equalizer input BitStream information

    Structure for audio Equalizer input BitStream information, such as BS length and BS buffer address.
    This is used in AudEQ_ApplyEqualizer().
*/
typedef struct
{
    UINT32      uiBsAddrIn;     ///< The Audio  input BitStream buffer Address.
    UINT32      uiBsAddrOut;    ///< The Audio output BitStream buffer Address.

    UINT32      uiBsLength;     ///< The Audio input BitStream Length in Byte count.

}AUDEQ_BITSTREAM,*PAUDEQ_BITSTREAM;



//
//  Export APIs
//
extern BOOL     AudEQ_Open(void);
extern BOOL     AudEQ_IsOpened(void);
extern BOOL     AudEQ_Close(void);
extern BOOL     AudEQ_SetConfig(AUDEQ_CONFIG_ID EqSel, INT32 iEqConfig);
extern BOOL     AudEQ_ApplyEqualizer(PAUDEQ_BITSTREAM  pAudBitStream);
extern BOOL     AudEQ_Init(void);
//@}

#endif
