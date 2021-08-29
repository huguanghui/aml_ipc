#ifndef AML_IPC_QUEUE_H
#define AML_IPC_QUEUE_H

#include "aml_ipc_component.h"
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

enum AmlIPCQueueChannel {
    AML_QUEUE_INPUT,
    AML_QUEUE_OUTPUT
};

struct AmlIPCQueue {
    AML_OBJ_EXTENDS(AmlIPCQueue, AmlIPCComponent, AmlIPCComponentClass);
    struct AmlIPCFrame** buffer;
    pthread_t tid;
    pthread_mutex_t lock;
    pthread_cond_t cond;
    volatile int started;
    volatile int flush;
    volatile int lock_count;

    int buffer_rp;
    int buffer_wp;
    int buffer_num;
    int buffer_num_alloc;
    int active;
};
AML_OBJ_DECLARE_TYPEID(AmlIPCQueue, AmlIPCComponent, AmlIPCComponentClass);

struct AmlIPCQueue* aml_ipc_queue_new(int numbuffer);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !AML_IPC_QUEUE_H
