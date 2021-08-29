#include "aml_ipc_component.h"

#ifndef AML_IPC_PIPELINE_H

#define AML_IPC_PIPELINE_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct AmlIPCPipelinePriv;
struct AmlIPCPipeline {
    AML_OBJ_EXTENDS(AmlIPCPipeline, AmlIPCComponent, AmlIPCComponentClass);
    TAILQ_HEAD(AmlIPCComonentListHead, AmlIPCComponent)
    children;
    struct AmlIPCPipelinePriv* priv;
    int sync_seq;
};
AML_OBJ_DECLARE_TYPEID(AmlIPCPipeline, AmlIPCComponent, AmlIPCComponentClass);

struct AmlIPCPipelineIterator {
    struct AmlIPCComponent* last;
    int sync_seq;
};

struct AmlIPCPipeline* aml_ipc_pipeline_new();

AmlStatus aml_ipc_pipeline_add(struct AmlIPCPipeline* pipeline, struct AmlIPCComponent* comp);

AmlStatus aml_ipc_pipeline_add_many(struct AmlIPCPipeline* pipeline, struct AmlIPCComponent* comp, ...);

AmlStatus aml_ipc_pipeline_remove(struct AmlIPCPipeline* pipeline, struct AmlIPCComponent* comp);

AmlStatus aml_ipc_pipeline_iterate(struct AmlIPCPipeline* pipeline, struct AmlIPCPipelineIterator* iter, struct AmlIPCComponent** comp);

AmlStatus aml_ipc_pipeline_dump_to_markdown(struct AmlIPCPipeline* pipeline, FILE* fp);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !AML_IPC_PIPELINE_H
