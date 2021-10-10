#ifndef __IMP_SYSTEM_H__
#define __IMP_SYSTEM_H__

#include "imp_common.h"
#include <stdint.h>

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif

typedef struct {
    char aVersion[64];
} IMPVersion;

int IMP_System_Init(void);

int IMP_System_Exit();

int64_t IMP_System_GetTimeStamp();

int IMP_System_RebaseTimeStamp(int64_t basets);

uint32_t IMP_System_ReadReg32(uint32_t regAddr);

void IMP_System_WriteReg32(uint32_t regAddr, uint32_t value);

int IMP_System_GetVersion(IMPVersion *pstVersion);

const char* IMP_System_GetCPUInfo(void);

int IMP_System_Bind(IMPCell *srcCell, IMPCell *dstCell);

int IMP_System_Unbind(IMPCell *srcCell, IMPCell *dstCell);

int IMP_System_GetBindbyDest(IMPCell *dstCell, IMPCell *srcCell);

int IMP_System_MemPoolRequest(int poolId, size_t size, const char *name);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
