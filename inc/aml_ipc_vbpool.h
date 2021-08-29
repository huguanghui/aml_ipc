#ifndef AML_IPC_VBPOOL_H
#define AML_IPC_VBPOOL_H

#include "aml_ipc_component.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

enum AmlIPCVBPoolChannel {
    AML_VBPOOL_OUTPUT
};

struct AmlIPCVBPoolBuffer;
struct AmlIPCVBPoolPriv;
struct AmlIPCVBPool {
    AML_OBJ_EXTENDS(AmlIPCVBPool, AmlIPCComponent, AmlIPCComponentClass);
    struct AmlIPCVBPoolPriv* priv;
    struct AmlIPCVideoFormat format;
    int num_buffer;
    int active;
    int live;
    int line_alignment;
    char* fill_color;
};
AML_OBJ_DECLARE_TYPEID(AmlIPCVBPool, AmlIPCComponent, AmlIPCComponentClass);

struct AmlIPCVBPool* aml_ipc_vbpool_new(int nbuf, struct AmlIPCVideoFormat* format);

AmlStatus aml_ipc_vbpool_set_alignment(struct AmlIPCVBPool* vbp, int alignment);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !AML_IPC_VBPOOL_H
