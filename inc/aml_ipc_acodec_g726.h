#ifndef AML_IPC_ACODEC_G726_H

#define AML_IPC_ACODEC_G726_H

#include "aml_ipc_component.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

enum AmlG726BitRate {
    AML_G726_16K = 16000,
    AML_G726_24K = 24000,
    AML_G726_32K = 32000,
    AML_G726_40K = 40000
};
enum AmlG726Endian {
    AML_G726_BE,
    AML_G726_LE
};

struct AmlIPCACodecG726Priv;
struct AmlIPCACodecG726 {
    AML_OBJ_EXTENDS(AmlIPCACodecG726, AmlIPCComponent, AmlIPCComponentClass);
    struct AmlIPCAudioFormat format;
    struct AmlIPCACodecG726Priv* priv;
    int bit_rate;
    int endian;
};
AML_OBJ_DECLARE_TYPEID(AmlIPCACodecG726, AmlIPCComponent, AmlIPCComponentClass);

struct AmlIPCACodecG726* aml_ipc_g726_new(enum AmlIPCAudioCodec codec, enum AmlG726BitRate b, enum AmlG726Endian e);

AmlStatus aml_ipc_g726_set_bitrate(struct AmlIPCACodecG726* g726, enum AmlG726BitRate bps);

AmlStatus aml_ipc_g726_set_codec(struct AmlIPCACodecG726* g726, enum AmlIPCAudioCodec codec);

AmlStatus aml_ipc_g726_set_endian(struct AmlIPCACodecG726* g726, enum AmlG726Endian endian);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !AML_IPC_ACODEC_G726_H
