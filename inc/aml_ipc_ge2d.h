#ifndef AML_IPC_GE2D_H

#define AML_IPC_GE2D_H

#include "aml_ipc_component.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

enum AmlIPCGE2DChannel {
    AML_GE2D_CH0,
    AML_GE2D_CH1,
    AML_GE2D_OUTPUT,
    AML_GE2D_CH_SDK_MAX
};
enum AmlIPCGE2DOP {
    AML_GE2D_OP_SDK_MIN,
    AML_GE2D_OP_FILLRECT,
    AML_GE2D_OP_BITBLT_TO,
    AML_GE2D_OP_BITBLT_FROM,
    AML_GE2D_OP_STRETCHBLT_TO,
    AML_GE2D_OP_STRETCHBLT_FROM,
    AML_GE2D_OP_ALPHABLEND_TO,
    AML_GE2D_OP_ALPHABLEND_FROM,
    AML_GE2D_OP_SDK_MAX
};

struct AmlIPCGE2DPriv;
struct MyStruct {
    AML_OBJ_EXTENDS(AmlIPCGE2D, AmlIPCComponent, AmlIPCComponentClass);
    pthread_mutex_t buf_lock;
    struct AmlIPCGE2DPriv* priv;
    struct AmlIPCFrame* buffer;
    enum AmlIPCGE2DOP op;
    int pass_through;
    int rotation; // degree, 0~360
    int is_bt709tobt601j;
    int sync_margin;
};
AML_OBJ_DECLARE_TYPEID(AmlIPCGE2D, AmlIPCComponent, AmlIPCComponentClass);

struct AmlIPCOSDFrame {
    AML_OBJ_EXTENDS(AmlIPCOSDFrame, AmlIPCVideoFrame, AmlObjectClass);
    TAILQ_ENTRY(AmlIPCOSDFrame)
    node;
    struct AmlRect src_rect;
    struct AmlRect dst_rect;
    uint32_t color;
};
AML_OBJ_DECLARE_TYPEID(AmlIPCOSDFrame, AmlIPCVideoFrame, AmlObjectClass);
#define AML_IPC_OSD_FRAME(obj) ((struct AmlIPCOSDFrame*)(obj))

struct AmlIPCOSDFrameList {
    AML_OBJ_EXTENDS(AmlIPCOSDFrameList, AmlIPCFrame, AmlObjectClass);
    TAILQ_HEAD(AmlIPCOSDFrameListHead, AmlIPCOSDFrame)
    head;
};
AML_OBJ_DECLARE_TYPEID(AmlIPCOSDFrameList, AmlIPCFrame, AmlObjectClass);
#define AML_IPC_OSD_FRAME_LIST(obj) ((struct AmlIPCOSDFrameList*)(obj))

struct AmlIPCGE2D* aml_ipc_ge2d_new(enum AmlIPCGE2DOP op);
AmlStatus aml_ipc_ge2d_set_op(struct AmlIPCGE2D* ge2d, enum AmlIPCGE2DOP op);
AmlStatus aml_ipc_ge2d_set_passthrough(struct AmlIPCGE2D* ge2d, int passthrough);
AmlStatus aml_ipc_ge2d_set_bt709to601j(struct AmlIPCGE2D* ge2d, int bt709to601j);

AmlStatus aml_ipc_ge2d_set_sync_margin(struct AmlIPCGE2D* ge2d, int margin_us);

AmlStatus aml_ipc_ge2d_set_rotation(struct AmlIPCGE2D* ge2d, int angle);

AmlStatus aml_ipc_ge2d_lock_static_buffer(struct AmlIPCGE2D* ge2d);
AmlStatus aml_ipc_ge2d_unlock_static_buffer(struct AmlIPCGE2D* ge2d);
AmlStatus aml_ipc_ge2d_set_static_buffer(struct AmlIPCGE2D* ge2d, struct AmlIPCFrame* buf);

AmlStatus aml_ipc_ge2d_static_buffer_list_replace(struct AmlIPCGE2D* ge2d, struct AmlIPCOSDFrame* oldOsd, struct AmlIPCOSDFrame* newOsd);
AmlStatus aml_ipc_ge2d_static_buffer_list_append(struct AmlIPCGE2D* ge2d, struct AmlIPCOSDFrame* osd);
AmlStatus aml_ipc_ge2d_static_buffer_list_remove(struct AmlIPCGE2D* ge2d, struct AmlIPCOSDFrame* osd);

int aml_ipc_ge2d_get_image_size(enum AmlPixelFormat pixfmt, int width, int height, int* pitch);

AmlStatus aml_ipc_ge2d_process(struct AmlIPCGE2D* ge2d, struct AmlIPCVideoFrame* dst, struct AmlRect* rc, int color);

AmlStatus aml_ipc_ge2d_bitblt(struct AmlIPCGE2D* ge2d, struct AmlIPCVideoFrame* src,
    struct AmlIPCVideoFrame* dst, struct AmlRect* srcrc, int dx, int dy);

AmlStatus aml_ipc_ge2d_stretchblt(struct AmlIPCGE2D* ge2d, struct AmlIPCVideoFrame* src,
    struct AmlIPCVideoFrame* dst, struct AmlRect* srcrc, struct AmlRect* dstrc);

AmlStatus aml_ipc_ge2d_blend(struct AmlIPCGE2D* ge2d, struct AmlIPCVideoFrame* src,
    struct AmlIPCVideoFrame* dst, struct AmlIPCVideoFrame* out,
    struct AmlRect* srcrc, int dx, int dy, int ox, int oy);

struct AmlIPCOSDFrameList* aml_ipc_osd_frame_list_new();

AmlStatus aml_ipc_osd_frame_list_replace(struct AmlIPCOSDFrameList* list,
    struct AmlIPCOSDFrame* oldOsd,
    struct AmlIPCOSDFrame* newOsd);

AmlStatus aml_ipc_osd_frame_list_append(struct AmlIPCOSDFrameList* list,
    struct AmlIPCOSDFrame* osd);

AmlStatus aml_ipc_osd_frame_list_remove(struct AmlIPCOSDFrameList* list,
    struct AmlIPCOSDFrame* osd);

AmlStatus aml_ipc_osd_frame_set_src_rect(struct AmlIPCOSDFrame* osd, int x, int y, int w, int h);

AmlStatus aml_ipc_osd_frame_set_dst_rect(struct AmlIPCOSDFrame* osd, int x, int y, int w, int h);

AmlStatus aml_ipc_osd_frame_set_rect(struct AmlIPCOSDFrame* osd, struct AmlRect* src, struct AmlRect* dst);

AmlStatus aml_ipc_osd_frame_set_color(struct AmlIPCOSDFrame* osd, uint32_t gbra);

struct AmlIPCOSDFrame* aml_ipc_osd_frame_new_alloc(struct AmlIPCVideoFormat* fmt);

struct AmlIPCOSDFrame* aml_ipc_osd_frame_new_fill_rect(struct AmlRect* rc, uint32_t gbra);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !AML_IPC_GE2D_H
