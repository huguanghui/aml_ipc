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

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !AML_IPC_DEFINE_H
