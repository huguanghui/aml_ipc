#ifndef AML_IPC_FILEOUT_H

#define AML_IPC_FILEOUT_H

#include "aml_ipc_component.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

enum AmlFileOutChannel {
    AML_FILEOUT_INPUT
};

enum AmlFileOutFormat {
    AML_FILEOUT_FMT_RAW,
    AML_FILEOUT_FMT_AML
};

struct AmlIPCFileOutPriv;
struct AmlIPCFileOut {
    AML_OBJ_EXTENDS(AmlIPCFileOut, AmlIPCComponent, AmlIPCComponentClass);
    struct AmlIPCFileOutPriv* priv;
    char* file_path;
    int frames_to_skip;
    int frames_to_dump;
    enum AmlFileOutFormat format;
};
AML_OBJ_DECLARE_TYPEID(AmlIPCFileOut, AmlIPCComponent, AmlIPCComponentClass);

AmlStatus aml_ipc_fileout_set_path(struct AmlIPCFileOut* fo, char* path);

AmlStatus aml_ipc_fileout_set_skip(struct AmlIPCFileOut* fo, int num);

AmlStatus aml_ipc_fileout_set_dump(struct AmlIPCFileOut* fo, int num);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !AML_IPC_FILEOUT_H
