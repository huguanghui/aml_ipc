#ifndef __IMP_COMMON_H__
#define __IMP_COMMON_H__

#include <stdint.h>
#include <pthread.h>
#include <linux/videodev2.h>
#include <stdio.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif // __cplusplus
#endif //

typedef enum {
    DEV_ID_FS, /**< 视频源 */
    DEV_ID_ENC, /**< 编码器 */
    DEV_ID_DEC, /**< 解码器 */
    DEV_ID_IVS, /**< 算法 */
    DEV_ID_OSD, /**< 图像叠加 */
    DEV_ID_FG1DIRECT, /**< FB FG1Direct */
    DEV_ID_RESERVED_START,
    DEV_ID_RESERVED_END = 23,
    NR_MAX_DEVICES,
} IMPDeviceID;

typedef struct {
    IMPDeviceID deviceID;
    int groupID;
    int outputID;
} IMPCell;

typedef struct {
    int index;
    int pool_idx;

    uint32_t width;
    uint32_t height;
    uint32_t pixfmt;
    uint32_t size;

    uint32_t phyAddr;
    uint32_t virAddr;

    int64_t timeStamp;
    uint32_t priv[0];
} IMPFrameInfo;

typedef struct {
    uint64_t ts;
    uint64_t minus;
    uint64_t plus;
} IMPFrameTimestamp;

typedef enum {
    PT_JPEG,
    PT_H264,
    PT_H265,
} IMPPayloadType;

typedef enum {
    PIX_FMT_YUV420P,
    PIX_FMT_YUYV422,
    PIX_FMT_UYVY422,
    PIX_FMT_YUV422P,
    PIX_FMT_YUV444P,
    PIX_FMT_YUV410P,
    PIX_FMT_YUV411P,
    PIX_FMT_GRAY8,
    PIX_FMT_MONOWHITE,
    PIX_FMT_MONOBLACK,

    PIX_FMT_NV12,
    PIX_FMT_NV21,

    PIX_FMT_RGB24,
    PIX_FMT_BGR24,

    PIX_FMT_ARGB,
    PIX_FMT_RGBA,
    PIX_FMT_ABGR,
    PIX_FMT_BGRA,

    PIX_FMT_NB,
} IMPPixelFormat;

typedef struct {
    int x;
    int y;
} IMPPoint;

typedef struct {
    IMPPoint p0;
    IMPPoint p1;
} IMPRect;

static inline int calc_pic_size(int width, int height, IMPPixelFormat imp_pixfmt)
{
    int bpp1 = 0, bpp2 = 1, size;

#define BPP(FMT, A, B) \
    case FMT:          \
        bpp1 = A;      \
        bpp2 = B;      \
        break
    switch (imp_pixfmt) {
        BPP(PIX_FMT_NV12, 3, 2);
        BPP(PIX_FMT_YUYV422, 2, 1);
        BPP(PIX_FMT_UYVY422, 2, 1);
    default:
        break;
    }
#undef BPP
    size = width * height * bpp1 / bpp2;

    return size;
}

static inline const char* fmt_to_string(IMPPixelFormat imp_pixfmt)
{
    static const char* nv12 = "nv12";
    static const char* yuyv422 = "yuyv422";

    switch (imp_pixfmt) {
    case PIX_FMT_NV12:
        return nv12;
    case PIX_FMT_YUYV422:
        return yuyv422;
    default:
        break;
    }
    return NULL;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif // __cplusplus
#endif //

#endif
