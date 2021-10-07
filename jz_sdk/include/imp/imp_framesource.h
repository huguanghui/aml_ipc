#ifndef __IMP_FRAMESOURCE_H__
#define __IMP_FRAMESOURCE_H__

#include "imp_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef struct {
    int enable;
    int left;
    int top;
    int width;
    int height;
} IMPFSChnCrop;

typedef struct {
    int enable;
    int outwidth;
    int outheight;
} IMPFSChnScaler;

typedef enum {
    FS_PHY_CHANNEL,
    FS_EXT_CHANNEL,
} IMPFSChnType;

typedef enum {
    FIFO_CACHE_PRIORITY = 0,
    FIFO_DATA_PRIORITY,
} IMPFSChnFifoType;

typedef struct {
    int maxdepth;
    IMPFSChnFifoType type;
} IMPFSChnFifoAttr;

typedef struct {
    int picWidth;
    int picHeight;
    IMPPixelFormat pixFmt;
    IMPFSChnCrop crop;
    IMPFSChnScaler scaler;
    int outFrmRateNum;
    int outFrmRateDen;
    int nrVBs;
    IMPFSChnType type;
    IMPFSChnCrop fcrop;
} IMPFSChnAttr;

int IMP_FrameSource_CreateChn(int chnNum, IMPFSChnAttr* chn_attr);

int IMP_FrameSource_DestroyChn(int chnNum);

int IMP_FrameSource_EnableChn(int chnNum);

int IMP_FrameSource_DisableChn(int chnNum);

int IMP_FrameSource_SetSource(int extchnNum, int sourcechnNum);

int IMP_FrameSource_GetChnAttr(int chnNum, IMPFSChnAttr* chnAttr);

int IMP_FrameSource_SetChnAttr(int chnNum, IMPFSChnAttr* chnAttr);

int IMP_FrameSource_SetFrameDepth(int chnNum, int depth);

int IMP_FrameSource_GetFrameDepth(int chnNum, int* depth);

int IMP_FrameSource_GetFrame(int chnNum, IMPFrameInfo** frame);

int IMP_FrameSource_GetTimedFrame(int chnNum, IMPFrameTimestamp* framets, int block, void* framedata, IMPFrameInfo* frame);

int IMP_FrameSource_ReleaseFrame(int chnNum, IMPFrameInfo* frame);

int IMP_FrameSource_SnapFrame(int chnNum, IMPPixelFormat fmt, int width, int height, void *framedata, IMPFrameInfo *frame);

int IMP_FrameSource_SetMaxDelay(int chnNum, int maxcnt);

int IMP_FrameSource_GetMaxDelay(int chnNum, int *maxcnt);

int IMP_FrameSource_GetDelay(int chnNum, int *cnt);

int IMP_FrameSource_SetChnFifoAttr(int chnNum, IMPFSChnFifoAttr *attr);

int IMP_FrameSource_GetChnFifoAttr(int chnNum, IMPFSChnFifoAttr *attr);

int IMP_FrameSource_SetPool(int chnNum, int poolID);

int IMP_FrameSource_GetPool(int chnNum);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif // !__IMP_FRAMESOURCE_H__
