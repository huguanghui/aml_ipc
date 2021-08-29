#ifndef AML_IPC_APROCESS_H

#define AML_IPC_APROCESS_H

#include "aml_ipc_component.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

enum AmlAProcessChannel {
    AML_APROCESS_INPUT,
    AML_APROCESS_OUTPUT
};

struct AmlIPCAProcessPriv;
struct AmlIPCAProcess {
    AML_OBJ_EXTENDS(AmlIPCAProcess, AmlIPCComponent, AmlIPCComponentClass);
    struct AmlIPCAProcessPriv* priv;
    int en_denoise;
    int en_AGC;
    int level_AGC;
    int en_VAD;
    int process_buffer_ms;
};
AML_OBJ_DECLARE_TYPEID(AmlIPCAProcess, AmlIPCComponent, AmlIPCComponentClass);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !AML_IPC_APROCESS_H
