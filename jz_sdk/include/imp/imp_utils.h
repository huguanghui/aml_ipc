#ifndef __IMP_UTILS_H__
#define __IMP_UTILS_H__

#include "imp_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#undef offsetof
#ifdef __compiler_offsetof
#define offsetof(TYPE, MEMBER) __compiler_offsetof(TYPE, MEMBER)
#else
#define offsetof(TYPE, MEMBER) ((size_t) & ((TYPE*)0)->MEMBER)
#endif //

#define container_of(ptr, type, member) ({ \
    const typeof(((type*)0)->member) *__mptr = (ptr); \
    (type*)((char *)__mptr - offsetof(type, member)); })

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

char* IMPPixfmtToString(IMPPixelFormat pixfmt);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif // !__IMP_UTILS_H__
