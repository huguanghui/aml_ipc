#ifndef AML_IPC_AIN_H

#define AML_IPC_AIN_H

#include "aml_ipc_component.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

enum AmlAInChannel { AML_AIN_OUTPUT };
enum AmlAInSource {
    AML_AIN_SRC_ALSA,
    AML_AIN_SRC_FILE
};
struct AmlIPCAinPriv;
struct MyStruct {
    AML_OBJ_EXTENDS(AmlIPCAin, AmlIPCComponent, AmlIPCComponentClass);
    struct AmlIPCAinPriv* priv;
    struct AmlIPCAudioFormat format;
    unsigned int buffer_time;
    unsigned int period_time;
    int block_size;
    char* device;
    char* vol_device;
    char* vol_control;
    float volume;
    int active_mode;
    int source;
    int thread_priority;
};
AML_OBJ_DECLARE_TYPEID(AmlIPCAIn, AmlIPCComponent, AmlIPCComponentClass);

#define AML_IPC_AIN(obj) ((struct AmlIPCAIn*)(obj))

struct AmlIPCAIn* aml_ipc_ain_new();

AmlStatus aml_ipc_ain_set_volume(struct AmlIPCAIn* ain, float vol);

AmlStatus aml_ipc_ain_set_active(struct AmlIPCAIn* ain, int active);

AmlStatus aml_ipc_ain_set_device(struct AmlIPCAIn* ain, char* device);

AmlStatus aml_ipc_ain_set_buffer_time(struct AmlIPCAIn* ain, int time);

AmlStatus aml_ipc_ain_set_period_time(struct AmlIPCAIn* ain, int time);

AmlStatus aml_ipc_ain_set_blksize(struct AmlIPCAIn* ain, int size);

AmlStatus aml_ipc_ain_set_codec(struct AmlIPCAIn* ain, enum AmlIPCAudioCodec codec);

AmlStatus aml_ipc_ain_set_rate(struct AmlIPCAIn* ain, int rate);

AmlStatus aml_ipc_ain_set_channel(struct AmlIPCAIn* ain, int channel);

AmlStatus aml_ipc_ain_set_vol_device(struct AmlIPCAIn* ain, char* vol_dev);

AmlStatus aml_ipc_ain_set_vol_control(struct AmlIPCAIn* ain, char* vol_ctl);

AmlStatus aml_ipc_ain_set_source(struct AmlIPCAIn* ain, enum AmlAInSource src);

AmlStatus aml_ipc_ain_set_thread_priority(struct AmlIPCAIn* ain, int priority);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !AML_IPC_AIN_H
