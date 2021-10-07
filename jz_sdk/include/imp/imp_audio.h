#ifndef __IMP_AUDIO_H__
#define __IMP_AUDIO_H__

#include <stdint.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/**
 * 音频采样率定义.
 */
typedef enum {
    AUDIO_SAMPLE_RATE_8000 = 8000, /**< 8KHz采样率 */
    AUDIO_SAMPLE_RATE_16000 = 16000, /**< 16KHz采样率 */
    AUDIO_SAMPLE_RATE_24000 = 24000, /**< 24KHz采样率 */
    AUDIO_SAMPLE_RATE_32000 = 32000, /**< 32KHz采样率 */
    AUDIO_SAMPLE_RATE_44100 = 44100, /**< 44.1KHz采样率 */
    AUDIO_SAMPLE_RATE_48000 = 48000, /**< 48KHz采样率 */
    AUDIO_SAMPLE_RATE_96000 = 96000, /**< 96KHz采样率 */
} IMPAudioSampleRate;

/**
 * 音频采样率定义.
 */
typedef enum {
    AUDIO_BIT_WIDTH_16 = 16, /**< 16bit采样精度 */
} IMPAudioBitWidth;

/**
 * 音频声道模式定义.
 */
typedef enum {
    AUDIO_SOUND_MODE_MONO = 1, /**< 单声道 */
    AUDIO_SOUND_MODE_STEREO = 2, /**< 双声道 */
} IMPAudioSoundMode;

/**
 * 定义音频净荷类型枚举
 */
typedef enum {
    PT_PCM = 0,
    PT_G711A = 1,
    PT_G711U = 2,
    PT_G726 = 3,
    PT_AEC = 4,
    PT_ADPCM = 5,
    PT_MAX = 6,
} IMPAudioPalyloadType;

/**
 * 定义解码方式
 */
typedef enum {
    ADEC_MODE_PACK = 0, /**< Pack 方式解码 */
    ADEC_MODE_STREAM = 1, /**< Stream 方式解码 */
} IMPAudioDecMode;

/**
 * 音频输入输出设备属性
 */
typedef struct {
    IMPAudioSampleRate samplerate; /**< 音频采样率 */
    IMPAudioBitWidth bitwidth; /**< 音频采样精度 */
    IMPAudioSoundMode soundmode; /**< 音频声道模式 */
    int frmNum; /**< 缓存帧的数目, 取值范围: [2, MAX_AUDIO_FRAME_NUM] */
    int numPerFrm; /**< 每帧的采样点个数 */
    int chnCnt; /**< 支持的通道数目 */
} IMPAudioIOAttr;

/**
 * 定义音频编码通道属性结构体
 */
typedef struct {
    IMPAudioPalyloadType type; /**< 音频净荷数据类型 */
    int bufSize; /**< buf 大小, 以帧为单位，[2~MAX_AUDIO_FRAME_NUM] */
    uint32_t* value; /**< 协议属性指针 */
} IMPAudioEncChnAttr;

/**
 * 定义解码通道属性结构体
 */
typedef struct {
    IMPAudioPalyloadType type; /**< 音频解码协议类型 */
    int bufSize; /**< 音频解码缓存大小 */
    IMPAudioDecMode mode; /**< 解码方式 */
    void* value; /**< 具体协议属性指针 */
} IMPAudioDecChnAttr;

/* 设置音频输入设备属性 */
int IMP_AI_SetPubAttr(int audioDevId, IMPAudioIOAttr* attr);

/* 获取音频输入设备属性 */
int IMP_AI_GetPubAttr(int audioDevId, IMPAudioIOAttr* attr);

/* 启用音频输入设备 */
int IMP_AI_Enable(int audioDevId);

/* 禁用音频输入设备 */
int IMP_AI_Disable(int audioDevId);

/* 启用音频输入通道 */
int IMP_AI_EnableChn(int audioDevId, int aiChn);

/* 禁用音频输入通道 */
int IMP_AI_DisableChn(int audioDevId, int aiChn);

/**
 * @brief 启用指定音频输入和音频输出的回声抵消功能
 *
 * @param[in] aiDevId
 * @param[in] aiChn
 * @param[in] aoDevId
 * @param[in] aoChn
 * @returns
 */
int IMP_AI_EnableAec(int aiDevId, int aiChn, int aoDevId, int aoChn);

/**
 * @brief 禁用回声抵消功能
 *
 * @param[in] aiDevId
 * @param[in] aiChn
 * @returns
 */
int IMP_AI_DisableAec(int aiDevId, int aiChn);

/**
 * @brief 启用音频输入噪声抑制功能
 *
 * @param[in] mode
 * @returns
 */
int IMP_AI_EnableNs(IMPAudioIOAttr *attr, int mode);

/**
 * @brief 禁用噪声抑制功能
 *
 * @returns
 */
int IMP_AI_DisableNs(void);

/* 创建音频编码通道 */
int IMP_AENC_CreateChn(int aeChn, IMPAudioEncChnAttr* attr);

/* 销毁音频编码通道 */
int IMP_AENC_DestroyChn(int aeChn);

/* 创建音频解码通道 */
int IMP_ADEC_CreateChn(int adChn, IMPAudioDecChnAttr* attr);

/* 销毁音频解码通道 */
int IMP_ADEC_DestroyChn(int adChn);

/* 设置音频输出设备属性 */
int IMP_AO_SetPubAttr(int audioDevId, IMPAudioIOAttr* attr);

/* 获取音频输出设备属性 */
int IMP_AO_GetPubAttr(int audioDevId, IMPAudioIOAttr *attr);

/* 启用音频输出设备 */
int IMP_AO_Enable(int audioDevId);

/* 禁用音频输出设备 */
int IMP_AO_Disable(int audioDevId);

/* 启用音频输出通道 */
int IMP_AO_EnableChn(int audioDevId, int aoChn);

/* 禁用音频输出通道 */
int IMP_AO_DisableChn(int audioDevId, int aoCh);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif // !__IMP_AUDIO_H__
