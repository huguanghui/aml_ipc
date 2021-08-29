#ifndef AML_IPC_FILEIN_H

#define AML_IPC_FILEIN_H

#include "aml_ipc_component.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

enum AmlFileInChannel {
    AML_FILEIN_OUTPUT
};

enum AmlFileInFormat {
    AML_FILEIN_FMT_RAW,
    AML_FILEIN_FMT_AML
};
struct AmlIPCFileInPriv;
struct AmlIPCFileIn {
    AML_OBJ_EXTENDS(AmlIPCFileIn, AmlIPCComponent, AmlIPCComponentClass);
    struct AmlIPCFileInPriv* priv;
    struct AmlIPCFrame* frame_template;
    char* file_path;
    int active_mode;
    int block_size;
    enum AmlFileInFormat format;
    int is_repeat;
};
AML_OBJ_DECLARE_TYPEID(AmlIPCFileIn, AmlIPCComponent, AmlIPCComponentClass);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !AML_IPC_FILEIN_H
