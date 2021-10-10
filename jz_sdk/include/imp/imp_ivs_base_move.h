#ifndef __IMP_IVS_BASE_MOVE_H__
#define __IMP_IVS_BASE_MOVE_H__

#include <stdint.h>
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define IMP_IVS_MOVE_MAX_ROI_CNT 32

#include <imp/imp_ivs.h>

typedef struct {
    int skipFrameCnt;
    int referenceCnt;
    int sadMode;
    int sense;
    IMPFrameInfo frameInfo;
} IMP_IVS_BaseMoveParam;

typedef struct {
    int ret;
    uint8_t* data;
    int datalen;
    int64_t timeStamp;
} IMP_IVS_BaseMoveOutput;

IMPIVSInterface *IMP_IVS_CreateBaseMoveInterface(IMP_IVS_BaseMoveParam *param);

void IMP_IVS_DestroyBaseMoveInterface(IMPIVSInterface *moveInterface);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif // !__IMP_IVS_BASE_MOVE_H__
