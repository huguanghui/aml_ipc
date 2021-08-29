#ifndef AML_IPC_OVERLAY_H

#define AML_IPC_OVERLAY_H

#include "aml_ipc_component.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

enum AmlIPCOverlayChannel {
    AML_OVERLAY_INPUT,
    AML_OVERLAY_OUTPUT
};
struct AmlIPCOverlayPriv;
struct AmlIPCOverlay {
    AML_OBJ_EXTENDS(AmlIPCOverlay, AmlIPCComponent, AmlIPCComponentClass);
    struct AmlIPCOverlayPriv* priv;
    char* font_file;
    char* draw;
    int font_size;
    int font_color;
    int bg_color;
    int pos_x;
    int pos_y;
};
AML_OBJ_DECLARE_TYPEID(AmlIPCOverlay, AmlIPCComponent, AmlIPCComponentClass);

struct AmlIPCOverlay* aml_ipc_overlay_new(void);
typedef void (*AmlIPCScriptFunc)(void* sce, void* param);
AmlStatus aml_ipc_script_bind_str(struct AmlIPCOverlay* ov, const char* name, char* val);
AmlStatus aml_ipc_script_bind_int(struct AmlIPCOverlay* ov, const char* name, int val);
AmlStatus aml_ipc_script_bind_func(struct AmlIPCOverlay* ov, const char* name, AmlIPCScriptFunc* func, void* param);

int aml_ipc_script_arg_int(void* sce, int idx);
char* aml_ipc_script_arg_str(void* sce, int idx);
void aml_ipc_script_abort(void* sce, int retval, const char* fmt, ...);

AmlStatus aml_ipc_script_compile(struct AmlIPCOverlay* ov, const char* script, int len);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !AML_IPC_OVERLAY_H
