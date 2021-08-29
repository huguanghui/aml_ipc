#include "aml_ipc_component.h"
#include "aml_ipc_ge2d.h"

#ifndef AML_IPC_OSD_H

#define AML_IPC_OSD_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct AmlIPCOSDContexPriv;
struct AmlIPCOSDContext {
    AML_OBJ_EXTENDS(AmlIPCOSDContext, AmlObject, AmlObjectClass);
    struct AmlIPCOSDContexPriv* priv;
    char* font_file;
    int font_size;
    int color;
    int bgcolor;
    int line_width;
    int outline_width;
    int outline_color;
    int auto_reverse_color;
};
AML_OBJ_DECLARE_TYPEID(AmlIPCOSDContext, AmlObject, AmlObjectClass);

AmlStatus aml_ipc_osd_load_font(struct AmlIPCOSDContext* ctx, const char* fontfile, int size);
AmlStatus aml_ipc_osd_set_font_size(struct AmlIPCOSDContext* ctx, int size);
AmlStatus aml_ipc_osd_get_font_bbox(struct AmlIPCOSDContext* ctx, int* maxw, int* maxh);

AmlStatus aml_ipc_osd_preload_text(struct AmlIPCOSDContext* ctx, const char* text, int* w, int* h);
AmlStatus aml_ipc_osd_preload_png(struct AmlIPCOSDContext* ctx, const char* file, int* w, int* h);
AmlStatus aml_ipc_osd_preload_jpeg(struct AmlIPCOSDContext* ctx, const char* file, int* w, int* h);

AmlStatus aml_ipc_osd_start_paint(struct AmlIPCOSDContext* ctx, struct AmlIPCVideoFrame* frame);
AmlStatus aml_ipc_osd_load_png(struct AmlIPCOSDContext* ctx, const char* file, int x, int y);
AmlStatus aml_ipc_osd_load_jpeg(struct AmlIPCOSDContext* ctx, const char* file, int x, int y);
AmlStatus aml_ipc_osd_draw_text(struct AmlIPCOSDContext* ctx, const char* text, int* x, int* y);
AmlStatus aml_ipc_osd_draw_text_len(struct AmlIPCOSDContext* ctx, const char* text, int len, int* x, int* y);
AmlStatus aml_ipc_osd_draw_line(struct AmlIPCOSDContext* ctx, int x1, int y1, int x2, int y2);
AmlStatus aml_ipc_osd_draw_rect(struct AmlIPCOSDContext* ctx, struct AmlRect* rc);
AmlStatus aml_ipc_osd_fill_rect(struct AmlIPCOSDContext* ctx, struct AmlRect* rc, int color);
AmlStatus aml_ipc_osd_alpha_blend(struct AmlIPCOSDContext* ctx, struct AmlIPCVideoFrame* src,
    struct AmlRect* src_rc, int x, int y);
AmlStatus aml_ipc_osd_done_paint(struct AmlIPCOSDContext* ctx);

struct AmlIPCOSDContext* aml_ipc_osd_context_new();

void aml_ipc_osd_set_pixel(struct AmlIPCOSDContext* ctx, uint8_t* dst, int color);

AmlStatus aml_ipc_osd_set_color(struct AmlIPCOSDContext* ctx, int color);
AmlStatus aml_ipc_osd_set_bgcolor(struct AmlIPCOSDContext* ctx, int color);
AmlStatus aml_ipc_osd_set_width(struct AmlIPCOSDContext* ctx, int width);
AmlStatus aml_ipc_osd_clear_rect(struct AmlIPCOSDContext* ctx, struct AmlRect* rc);
AmlStatus aml_ipc_osd_set_text_outline(struct AmlIPCOSDContext* ctx, int width, int color);

AmlStatus aml_ipc_osd_set_text_auto_reverse_color(struct AmlIPCOSDContext* ctx, int mode);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !AML_IPC_OSD_H
