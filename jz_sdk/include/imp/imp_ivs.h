#ifndef __IMP_IVS_H__
#define __IMP_IVS_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include <imp/imp_common.h>

#define IMP_IVS_DEFAULT_TIMEOUTS (-1)

typedef struct IMPIVSInterface IMPIVSInterface;

struct IMPIVSInterface {
    void* param;
    int paramSize;
    IMPPixelFormat pixfmt;
    int (*init)(IMPIVSInterface* inf);
    int (*exit)(IMPIVSInterface* inf);
    int (*preProcessSync)(IMPIVSInterface* inf, IMPFrameInfo* frame);
    int (*processAsync)(IMPIVSInterface* inf, IMPFrameInfo* frame);
    int (*getResult)(IMPIVSInterface* inf, void** result);
    int (*releaseResult)(IMPIVSInterface* inf, void* result);
    int (*getParam)(IMPIVSInterface* inf, void* param);
    int (*setParam)(IMPIVSInterface* inf, void* param);
    int (*flushFrame)(IMPIVSInterface* inf);
    void* priv;
};

int IMP_IVS_CreateGroup(int GrpNum);

int IMP_IVS_DestroyGroup(int GrpNum);

int IMP_IVS_CreateChn(int ChnNum, IMPIVSInterface* handler);

int IMP_IVS_DestroyChn(int ChnNum);

int IMP_IVS_RegisterChn(int GrpNum, int ChnNum);

int IMP_IVS_UnRegisterChn(int ChnNum);

int IMP_IVS_StartRecvPic(int ChnNum);

int IMP_IVS_StopRecvPic(int ChnNum);

int IMP_IVS_PollingResult(int ChnNum, int timeoutMs);

int IMP_IVS_GetResult(int ChnNum, void **result);

int IMP_IVS_ReleaseResult(int ChnNum, void *result);

int IMP_IVS_ReleaseData(void *vaddr);

int IMP_IVS_GetParam(int chnNum, void *param);

int IMP_IVS_SetParam(int chnNum, void *param);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif // !__IMP_IVS_H__
