#ifndef AML_IPC_DEFINE_H
#define AML_IPC_DEFINE_H

#include <assert.h>
#include <inttypes.h>
#include <limits.h>
#include <pthread.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h>

typedef int AmlStatus;

#include "aml_ipc_log.h"
#include "aml_ipc_object.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#define AML_STATUS_OK 0
#define AML_STATUS_FAIL -1
#define AML_STATUS_ALLOC -2
#define AML_STATUS_WRONG_DIR -3
#define AML_STATUS_NOT_FOUND -4
#define AML_STATUS_NOT_IMPLEMENT -5
#define AML_STATUS_NOT_OPENED -6
#define AML_STATUS_WRONG_PIXFMT -7
#define AML_STATUS_QUERYCAP_ERROR -8
#define AML_STATUS_SETFMT_ERROR -9
#define AML_STATUS_SCTRL_ERROR -10
#define AML_STATUS_WRONG_PARAM -11
#define AML_STATUS_NOT_BIND -12
#define AML_STATUS_WRONG_CHANNEL -13
#define AML_STATUS_NO_FUNCTION -14
#define AML_STATUS_TIMEOUT -15
#define AML_STATUS_ALREADY_BIND -16
#define AML_STATUS_OPEN_ERROR -17
#define AML_STATUS_ALREADY_OPEN -18
#define AML_STATUS_HOOK_CONTINUE -19
#define AML_STATUS_WRONG_ACODEC -20
#define AML_STATUS_WRONG_VCODEC -21
#define AML_STATUS_ITERATE_DONE -22
#define AML_STATUS_ITERATE_UNSYNC -23
#define AML_STATUS_FLUSHING -24
#define AML_STATUS_NOT_IN_CANVAS -25

#define AML_ASSERT(c)                                                                \
    do {                                                                             \
        if (!(c)) {                                                                  \
            printf("%s %s: %d ASSERT fail: %s\n", __FILE__, __FUNC__, __LINE__, #c); \
        }                                                                            \
    } while (0)

enum AmlPixelFormat {
    AML_PIXFMT_RGB_565 = 1,
    AML_PIXFMT_RGB_888,
    AML_PIXFMT_BGR_888,
    AML_PIXFMT_RGBA_8888,
    AML_PIXFMT_BGRA_8888,
    AML_PIXFMT_RGBX_8888,
    AML_PIXFMT_YU12, // YYYY......U....V... I420
    AML_PIXFMT_YV12, // YYYY......V....U...
    AML_PIXFMT_NV12, // YYYY......UV....
    AML_PIXFMT_NV21, // YYYY......VU....
    AML_PIXFMT_NV16, // YYYY......UVUV....
    AML_PIXFMT_YUYV, // YUYV
    AML_PIXFMT_UVVY, // UYVY

    AML_PIXFMT_SDK_MAX
};

#define AML_IPC_PIXFMT_IS_VALID(pixfmt) \
    (pixfmt >= AML_PIXFMT_RGB_565 && pixfmt < AML_PIXFMT_SDK_MAX)
int aml_ipc_pixfmt_bits_per_pixel(enum AmlPixelFormat pixfmt);
int aml_ipc_pixfmt_is_yuv(enum AmlPixelFormat pixfmt);
int aml_ipc_pixfmt_nplane(enum AmlPixelFormat pixfmt);
int aml_ipc_pixfmt_bits_horizontal(enum AmlPixelFormat pixfmt, int* hbits);
int aml_ipc_pixfmt_bits_vertical(enum AmlPixelFormat pixfmt, int* vbits);
void aml_ipc_pixfmt_set_pixel(enum AmlPixelFormat pixfmt, int color, void* buf);

// SDK color format 0xaabbggrr -> char [4] {rr, gg, bb, aa}
#define AML_RGBA(r, g, b, a) \
    (((uint8_t)(r) << 0) | ((uint8_t)(g) << 8) | ((uint8_t)(b) << 16) | ((uint8_t)(a) << 24))

#define AML_MIN(x, y) ((x) < (y) ? (x) : (y))
#define AML_MAX(x, y) ((x) < (y) ? (y) : (x))
#define AML_ABS(x) ((x) < (0) ? (-x) : (x))

AML_LOG_EXTERN(comm);
AML_LOG_EXTERN(frame);
AML_LOG_EXTERN(frameflow);
AML_LOG_EXTERN(dmabuf);

// calculate packed image size for different format
// not like aml_ipc_ge2d_get_image_size, which will align the buffer for GE2D HW
int aml_ipc_get_image_size(enum AmlPixelFormat pixfmt, int width, int height, int* pitch,
    int line_align);

struct AmlRect {
    int left;
    int top;
    int width;
    int height;
};

static inline int aml_rect_intersect(struct AmlRect* src1, struct AmlRect* src2, struct AmlRect* dst)
{
    struct AmlRect tmp;
    if (dst == NULL)
        dst = &tmp;
    dst->left = AML_MAX(src1->left, src2->left);
    dst->top = AML_MAX(src1->top, src2->top);
    dst->width = AML_MIN(src1->left + src1->width, src1->left + src2->width) - dst->left;
    dst->height = AML_MIN(src1->top + src1->height, src2->top + src2->height) - dst->top;
    return (dst->width > 0) && (dst->height > 0);
}

#define AML_DMABUF_IS_ION (1 << 0)
#define AML_DMABUF_ALLOCATE (1 << 1)
#define AML_DMABUF_LOCKED (1 << 2)

struct AmlIPCVideoBuffer {
    AML_OBJ_EXTENDS(AmlIPCVideoBuffer, AmlObject, AmlObjectClass);
    int dma_fd;
    void* data;
    int size;
    int flags;
};
AML_OBJ_DECLARE_TYPEID(AmlIPCVideoBuffer, AmlObject, AmlObjectClass);
#define AML_IPC_VIDEO_BUFFER(obj) ((struct AmlIPCVideoBuffer*)(obj))

// map flag for aml_ipc_dma_buffer_map
#define AML_MAP_READ (1 << 0)
#define AML_MAP_WRITE (1 << 1)

AmlStatus aml_ipc_dma_buffer_alloc(struct AmlIPCVideoBUffer* dmabuf, int size);

AmlStatus aml_ipc_dma_buffer_dealloc(struct AmlIPCVideoBuffer* dmabuf);

AmlStatus aml_ipc_dma_buffer_init(struct AmlIPCVideoBuffer* dmabuf, int fd, int size);

void* aml_ipc_dma_buffer_map(struct AmlIPCVideoBuffer* dmabuf, int mapflag);

AmlStatus aml_ipc_dma_buffer_unmap(struct AmlIPCVideoBuffer* dmabuf);

struct AmlIPCFrame {
    AML_OBJ_EXTENDS(AmlIPCFrame, AmlObject, AmlObjectClass);
    int64_t pts_us;
    void* data;
    int size;
};

AML_OBJ_DECLARE_TYPEID(AmlIPCFrame, AmlObject, AmlObjectClss);
#define AML_IPC_FRAME(obj) ((struct AmlIPCFrame*)(obj))

static inline AmlStatus aml_ipc_frame_init(struct AmlIPCFrame* frame, void* data, size_t size,
    int64_t pts_us)
{
    frame->data = data;
    frame->size = size;
    frame->pts_us = pts_us;
    return AML_STATUS_OK;
}

static inline AmlStatus aml_ipc_frame_set_data(struct AmlIPCFrame* frame, void* data, size_t size)
{
    frame->data = data;
    frame->size = size;
    return AML_STATUS_OK;
}

static inline AmlStatus aml_ipc_frame_set_pts(struct AmlIPCFrame* frame, int64_t pts_us)
{
    frame->pts_us = pts_us;
    return AML_STATUS_OK;
}

struct AmlIPCVideoFormat {
    enum AmlPixelFormat pixfmt;
    int width;
    int height;
    int fps;
};

struct AmlIPCVideoFrame {
    AML_OBJ_EXTENDS(AmlIPCVideoFrame, AmlIPCFrame, AmlObjectClass);
    struct AmlIPCVideoFormat format;
    struct AmlIPCVideoBuffer* dmabuf;
    int pitch;
};
AML_OBJ_DECLARE_TYPEID(AmlIPCVideoFrame, AmlIPCFrame, AmlObjectClass);
#define AML_IPC_VIDEO_FRAME(obj) ((strcut AmlIPCVideoFrame*)(obj))

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !AML_IPC_DEFINE_H
