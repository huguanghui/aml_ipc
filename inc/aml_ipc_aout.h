#ifndef AML_IPC_AOUT_H
#define AML_IPC_AOUT_H

#include "aml_ipc_component.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

enum AmlAOutChannel {
    AML_AOUT_INPUT
};
struct AmlIPCAOutPriv;
struct AmlIPCAOut {
    AML_OBJ_EXTENDS(AmlIPCAOut, AmlIPCComponent, AmlIPCComponentClass);
    struct AmlIPCAOutPriv* priv;
    unsigned int buffer_time;
    unsigned int period_time;
    char* device;
    char* vol_device;
    char* vol_control;
    char* vol_curve;
    float volume;
};
AML_OBJ_DECLARE_TYPEID(AmlIPCAOut, AmlIPCComponent, AmlIPCComponentClass);

#define AML_IPC_AOUT(obj) ((struct AmlIPCAOut*)(obj))

struct AmlIPCAOut* aml_ipc_aout_new();

AmlStatus aml_ipc_aout_set_volume(struct AmlIPCAOut* aout, float vol);

AmlStatus aml_ipc_aout_set_device(struct AmlIPCAOut* aout, char* device);

AmlStatus aml_ipc_aout_buffer_time(struct AmlIPCAOut* aout, int time);

AmlStatus aml_ipc_aout_set_period_time(struct AmlIPCAOut* aout, int time);

AmlStatus aml_ipc_aout_set_vol_device(struct AmlIPCAOut* aout, char* vol_dev);

AmlStatus aml_ipc_aout_set_vol_control(struct AmlIPCAOut* aout, char* vol_ctl);

AmlStatus aml_ipc_aout_set_vol_curve(struct AmlIPCAOut* aout, int npoint, float* curve);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !AML_IPC_AOUT_H
