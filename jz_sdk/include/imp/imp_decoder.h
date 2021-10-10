#ifndef __IMP_DECODER_H__
#define __IMP_DECODER_H__

#include "jz_sdk/include/imp/imp_common.h"
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef struct {
    IMPPayloadType decType;
    uint32_t maxWidth;
    uint32_t maxHeight;
    IMPPixelFormat pixelFormat;
    uint32_t nrKeepStream;
    uint32_t frmRateNum;
    uint32_t frmRateDen;
} IMPDecoderAttr;

typedef struct {
    IMPDecoderAttr decAttr;
} IMPDecoderCHNAttr;

typedef struct {
    int i_payload;
    uint8_t* p_payload;
    int64_t timeStamp;
} IMPDecoderNal;

typedef struct {
    IMPDecoderNal decoderNal;
} IMPDecoderStream;

int IMP_Decoder_CreateChn(int decChn, const IMPDecoderCHNAttr* attr);

int IMP_Decoder_DestroyChn(int decChn);

int IMP_Decoder_StartRecvPic(int decChn);

int IMP_Decoder_StopRecvPic(int decChn);

int IMP_Decoder_SendStreamTimeout(int decChn, IMPDecoderStream *stream, uint32_t timeoutMsec);

int IMP_Decoder_PollingFrame(int decChn, uint32_t timeoutMsec);

int IMP_Decoder_GetFrame(int decChn, IMPFrameInfo **frame);

int IMP_Decoder_ReleaseFrame(int devChn, IMPFrameInfo *frame);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
