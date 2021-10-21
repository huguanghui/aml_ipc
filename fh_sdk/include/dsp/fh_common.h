#ifndef __FH_COMMON_H__
#define __FH_COMMON_H__

#include "types/type_def.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
#pragma pack(4)

#ifdef API_PRT
#define API_DEBUG(...)                            \
    do {                                          \
        fh_printf("%s %d: ", __func__, __LINE__); \
        fh_printf(__VA_ARGS__);                   \
    } while (0)
#define API_NOTICE(...)                           \
    do {                                          \
        fh_printf("%s %d: ", __func__, __LINE__); \
        fh_printf(__VA_ARGS__);                   \
    } while (0)
#else
#define API_DEBUG(...)
#define API_NOTICE(...)                           \
    do {                                          \
        fh_printf("%s %d: ", __func__, __LINE__); \
        fh_printf(__VA_ARGS__);                   \
    } while (0)
#endif

#define RETURN_NULL (0)
#define RETURN_OK (0)
#define NO_DEVICE (-1001)
#define OPEN_DEVICE_ERR (-1001)
#define CLOSE_DEVICE_ERR (-1002)
#define FIND_DEVICE_ERR (-1003)
#define PARAM_NULL (-1004)
#define PARAM_ERR (-1005)
#define ALIGN_ERR (-1006)
#define MODULE_ENABLE_ERR (-1007)
#define CHAN_ERR (-1008)
#define MEM_NULL (-1009)
#define GET_STREAM_EMPTY (-1010)
#define HAS_CREATED (-1011)
#define SDK_ERR (-1012)
#define NOT_SUPPORT (-1013)
#define NOT_READY (-1014)
#define NOT_INIT (-1015)

#define MAX_OSD_LINE (8)
#define MAX_INVERT_CNT (8)
#define MAX_MASK_AREA (8)
#define MAX_GBOX_AREA (16)
#define DEFAULT_ALIGN (8)
#define MAX_VPU_CHANNO (4)
#define MAX_ENC_CHANNO (8)

#define MALLOCED_MEM_BASE_DSP (1)
#define MALLOCED_MEM_VPUPKG (0)

#define FH8632 (0x863200A1)
#define FH8632v2 (0x863200A2)
#define FH8620H (0x863200B1)
#define FH8856 (0x885600A1)
#define FH8852 (0x885600B1)
#define FH8626V100 (0x8626A100)
#define FH8852V200 (0x8852A200)
#define FH8856V200 (0x8856A200)
#define FH8858V200 (0x8858A200)
#define FH8652 (0x8652A100)
#define FH8656 (0x8656A100)
#define FH8658 (0x8658A100)

typedef enum {
    FH_RO_OPS_0 = 0,
    FH_RO_OPS_90 = 1,
    FH_RO_OPS_180 = 2,
    FH_RO_OPS_270 = 3,
    FH_ROTATE_OPS_DUMMY = 0xffffffff,
} FH_ROTATE_OPS;

typedef enum {
    VPU_VOMODE_BLK = 0,
    VPU_VOMODE_SCAN = 1,
    VPU_VOMODE_TILE192 = 2,
    VPU_VOMODE_TILE224 = 3,
    VPU_VOMODE_TILE256 = 4,
    VPU_VOMODE_RGB888 = 5,
    VPU_VOMODE_YUYV = 6,
    VPU_VOMODE_BUTT,
    FH_VPU_VO_MODE_DUMMY = 0xffffffff,
} FH_VPU_VO_MODE;

typedef struct fhVERSION_INFO_ {
    FH_UINT32 build_data;
    FH_UINT32 sw_version;
    FH_UINT32 hw_version;
} FH_VERSION_INFO;

typedef struct fhPOINT_ {
    FH_UINT32 u32X;
    FH_UINT32 u32Y;
} FH_POINT;

typedef struct fhSIZE_S {
    FH_UINT32 u32Width;
    FH_UINT32 u32Height;
} FH_SIZE;

typedef struct fhRECT_S {
    FH_UINT32 u32X;
    FH_UINT32 u32Y;
    FH_UINT32 u32Width;
    FH_UINT32 u32Height;
} FH_AREA;

typedef struct {
    FH_ADDR addr;
    FH_UINT32 size;
} FH_ADDR_INFO;

typedef struct {
    FH_UINT32 base;
    FH_VOID * vbase;
    FH_UINT32 size;
} FH_MEM_INFO;

typedef struct {
    FH_UINT16 frame_count;
    FH_UINT16 frame_time;
} FH_FRAMERATE;

typedef struct {
    FH_UINT32 y;
    FH_UINT32 cb;
    FH_UINT32 cr;
    FH_UINT32 alpha;
} FH_YC_COLOR;

typedef struct {
    FH_UINT32 avgTime;
    FH_UINT32 maxTime;
    FH_UINT32 avgCycle;
    FH_UINT32 maxCycle;
    FH_UINT32 win_size;
    FH_UINT64 time_stamp;
} FH_HW_AVG_TIME;

#pragma pack()
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif // !__FH_COMMON_H__
