#ifndef AML_IPC_GDC_H

#define AML_IPC_GDC_H

#include "aml_ipc_component.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

enum AmlGDCChannel {
    AML_GDC_CH0,
    AML_GDC_CH1,
    AML_GDC_OUTPUT
};
struct AmlIPCGDCPriv;
struct AmlIPCGDC {
    AML_OBJ_EXTENDS(AmlIPCGDC, AmlIPCComponent, AmlIPCComponentClass);
    struct AmlIPCGDCPriv* priv;
    char* config_file;
    int pass_through;
};
AML_OBJ_DECLARE_TYPEID(AmlIPCGDC, AmlIPCComponent, AmlIPCComponentClass);

struct AmlIPCGDC* aml_ipc_gdc_new();

AmlStatus aml_ipc_gdc_set_passthrough(struct AmlIPCGDC* gdc, bool passthrough);
AmlStatus aml_ipc_gdc_set_configfile(struct AmlIPCGDC* gdc, const char* config);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !AML_IPC_GDC_H
