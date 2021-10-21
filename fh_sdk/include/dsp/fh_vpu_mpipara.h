#ifndef __FH_VPU_MPIPARA_H__
#define __FH_VPU_MPIPARA_H__

#include "types/type_def.h"
#include "fh_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
#pragma pack(4)

typedef enum {
    VPU_MODE_ISP = 0,
    VPU_MODE_MEM = 1,
    FH_VPU_VI_MODE_DUMMY = 0xffffffff,
} FH_VPU_VI_MODE;

typedef enum {
    PIXEL_FORMAT_NOCHROME = 0,
    PIXEL_FORMAT_420 = 1,
    PIXEL_FORMAT_422 = 2,
    PIXEL_FORMAT_444 = 3,
    PIXEL_FORMAT_RGB888 = 4,
    FH_VPU_PIXELFORMAT_DUMMY = 0xffffffff,
} FH_VPU_PIXELFORMAT;

typedef struct {
    FH_SIZE vi_size;
} FH_VPU_SIZE;

#pragma pack()
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif // !__FH_VPU_MPIPARA_H__
