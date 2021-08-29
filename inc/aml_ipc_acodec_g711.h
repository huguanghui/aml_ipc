#ifndef AML_IPC_ACODEC_G711_H

#define AML_IPC_ACODEC_G711_H

#include "aml_ipc_component.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct AmlIPCACodecG711 {
    AML_OBJ_EXTENDS(AmlIPCACodecG711, AmlIPCComponent, AmlIPCComponentClass);
    struct AmlIPCAudioFormat format;
};
AML_OBJ_DECLARE_TYPEID(AmlIPCACodecG711, AmlIPCComponent, AmlIPCComponentClass);

struct AmlIPCACodecG711* aml_ipc_g711_new(enum AmlIPCAudioCodec codec);

AmlStatus aml_ipc_g711_set_codec(struct AmlIPCACodecG711* g711, enum AmlIPCAudioCodec codec);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !AML_IPC_ACODEC_G711_H
