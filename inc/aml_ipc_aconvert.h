#include "aml_ipc_component.h"
#include <string.h>

#ifndef AML_IPC_ACONVERT_H

#define AML_IPC_ACONVERT_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

enum AmlAConvertChannel {
    AML_ACONVERT_INPUT,
    AML_ACONVERT_OUTPUT
};

struct AmlIPCAConvertPriv;
struct AmlIPCAConvert {
    AML_OBJ_EXTENDS(AmlIPCAConvert, AmlIPCComponent, AmlIPCComponentClass);
    struct AmlIPCAudioFormat format;
    struct AmlIPCAConvertPriv* priv;
    char* channel_mapping;
};
AML_OBJ_DECLARE_TYPEID(AmlIPCAConvert, AmlIPCComponent, AmlIPCComponentClass);

struct AmlIPCAConvert* aml_ipc_aconv_new(struct AmlIPCAudioFormat* fmt);

AmlStatus aml_ipc_aconv_set_channel_mapping(struct AmlIPCAConvert* aconv, const char* mapping);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !AML_IPC_ACONVERT_H
