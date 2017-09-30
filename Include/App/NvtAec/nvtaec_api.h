#ifndef _NVTAECAPI_H
#define _NVTAECAPI_H

#include "Type.h"
#include "Audio.h"

extern void NvtAec_InstallID(void) _SECTION(".kercfg_text");
extern BOOL NvtAec_ChkModInstalled(void);

typedef enum
{
    NVTAEC_CONFIG_ID_LEAK_ESTIMTAE_EN,      ///< Enable/Disable the AEC leak estimate.
    NVTAEC_CONFIG_ID_LEAK_ESTIMTAE_VAL,     ///< Initial Condition of the leak estimate. Value range 25 ~ 99.
    NVTAEC_CONFIG_ID_NOISE_CANCEL_LVL,      ///< Defualt is -20dB. Suggest value range -3 ~ -40. Unit in dB.
    NVTAEC_CONFIG_ID_ECHO_CANCEL_LVL,       ///< Defualt is -50dB. Suggest value range -30 ~ -60. Unit in dB.
    NVTAEC_CONFIG_ID_RECORD_CH_NO,          ///< Record channel number.
    NVTAEC_CONFIG_ID_PLAYBACK_CH_NO,        ///< Playback channel number.
    NVTAEC_CONFIG_ID_SAMPLERATE,            ///< Audio sample rate.
    NVTAEC_CONFIG_ID_FILTER_LEN,            ///< Set AEC internal Filter Length. Set to 0 is using default value. Default is 0.
    NVTAEC_CONFIG_ID_FRAME_SIZE,            ///< Set AEC internal Frame Size. Set to 0 is using default value. Default is 0.

    ENUM_DUMMY4WORD(NVTAEC_CONFIG_ID)
} NVTAEC_CONFIG_ID;

/*
    Get play start time.

    Get play start time.

    @param[in] p1    no use.
    @param[in] p2    no use.
    @param[in] p3    no use.

    @return
     - @b E_OK:     Get successfully.
*/
extern ER NvtAec_GetPlayStart(UINT32 p1, UINT32 p2, UINT32 p3);

/*
    Add buffer to be played to queue.

    Add buffer to be played to queue.

    @param[in] p1    play buffer address.
    @param[in] p2    play buffer size.

    @return
     - @b E_OK:     success.
     - @b E_SYS:    buffer is too large.
*/
extern ER NvtAec_AddBufToQue(UINT32 p1, UINT32 p2);

/*
    Open AEC library.

    Open AEC library.

    @param[in] uiCh    Audio channel number.
    @param[in] uiSR    Audio sample rate.

    @return
     - @b E_OK:     Initial successfully.
     - @b E_OACV:   Open AEC failed.
     - @b E_SYS:    Initial AEC failed.
*/
extern ER NvtAec_Open(INT32 iSampleRate, INT32 iMicCH, INT32 iSpkCH);

/*
    Close AEC library.

    Close AEC library.

    @return
     - @b E_OK:     Close successfully.
     - @b E_SYS:    AEC is not opened.
*/
extern ER NvtAec_Close(void);

/*
    Reset play queue.

    Reset play queue when play task is stopped.

    @return
     - @b E_OK:     Reset successfully.
*/
extern ER NvtAec_ResetPlayQue(void);

/*
    Set NvtAec configuration.

    Set NvtAec configuration.

    @param[in]  NvtAecSel       Configuration type.
    @param[in]  iNvtAecConfig   Configuration value. (According to the confiuration type)
*/
extern void NvtAec_SetConfig(NVTAEC_CONFIG_ID NvtAecSel, INT32 iNvtAecConfig);

/*
    Enable NvtAec processing.

    Enable NvtAec processing.

    @param[in]  bEn       Eable configuration. (TRUE: enable, FALSE: disable)
*/
extern void NvtAec_Enable(BOOL bEn);

/*
    Calculate recorded sample count.

    Calculate recorded sample count.

    @param[in] p1    no use.
    @param[in] p2    no use.
    @param[in] p3    no use.

    @return
     - @b E_OK:     success.
*/
extern ER NvtAec_RecStart(UINT32 p1, UINT32 p2, UINT32 p3);

/*
    Proceed AEC.

    Proceed AEC.

    @param[in] p1    record buffer address.
    @param[in] p2    record buffer size.
    @param[in] p3    record sample rate.

    @return
     - @b E_OK:     success.
*/
extern ER NvtAec_Proc(UINT32 p1, UINT32 p2, UINT32 p3);

#endif
