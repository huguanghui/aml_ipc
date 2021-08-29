#ifndef AML_IPC_ISP_CTRL_H

#define AML_IPC_ISP_CTRL_H

#include <stdint.h>
#include <sys/types.h>

#include "aml_ipc_component.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

enum AmlISPAEState {
    AE_STATE_NONE,
    AE_STATE_SEARCHING,
    AE_STATE_CONVERGING
};

struct AmlISPExposureParam {
    int32_t enable_auto_exposure;
    int32_t manual_exposure_value;
    int32_t current_exposure_value;
    enum AmlISPAEState current_ae_state_id;
    int32_t enable_auto_gain;
    int32_t manual_gain_value;
    int32_t current_gain_value;
    int32_t auto_gain_compensation;
    int32_t enable_system_manual_exposure;
    int32_t manual_system_exposure_value;
};

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !AML_IPC_ISP_CTRL_H
