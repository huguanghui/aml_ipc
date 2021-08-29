#ifndef AML_IPC_RAWVIDEOPARSE_H

#define AML_IPC_RAWVIDEOPARSE_H

#include "aml_ipc_component.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

enum AmlRawVideoParseChannel {
    AML_RAWVIDEOPARSE_INPUT,
    AML_RAWVIDEOPARSE_OUTPUT,
    AML_RAWVIDEOPARSE_MAX
};

struct AmlIPCRawVideoParsePriv;
struct AmlIPCRawVideoParse {
    AML_OBJ_EXTENDS(AmlIPCRawVideParse, AmlIPCComponent, AmlIPCComponentClass);
    struct AmlIPCRawVideoParsePriv* priv;
    struct AmlIPCVideoFormat format;
    int pitch;
    int is_live;
};
AML_OBJ_DECLARE_TYPEID(AmlIPCRawVideoParse, AmlIPCComponent, AmlIPCComponentClass);

struct AmlIPCRawVideoParse* aml_ipc_rawvideoparse_new(struct AmlIPCVideoFormat* format);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !AML_IPC_RAWVIDEOPARSE_H
