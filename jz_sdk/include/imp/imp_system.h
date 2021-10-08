#ifndef __IMP_SYSTEM_H__
#define __IMP_SYSTEM_H__

#include "imp_common.h"

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

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
