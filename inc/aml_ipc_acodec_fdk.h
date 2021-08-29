#ifndef AML_IPC_ACODEC_FDK_H

#define AML_IPC_ACODEC_FDK_H

#include "aml_ipc_component.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

enum AmlAACAOT {
    AML_AAC_LC = 2,
    AML_AAC_SBR = 5,
    AML_AAC_PS = 29,
    AML_AAC_LD = 23,
    AML_AAC_ELD = 39
};

enum AmlAACBitrateMode {
    AML_AAC_BITRATE_CBR = 0,
    AML_AAC_BITRATE_VERY_LOW,
    AML_AAC_BITRATE_LOW,
    AML_AAC_BITRATE_MEDIUM,
    AML_AAC_BITRATE_HIGH,
    AML_AAC_BITRATE_VERY_HIGH
};

struct AmlIPCAudioAACFrame {
    AML_OBJ_EXTENDS(AmlIPCAudioAACFrame, AmlIPCAudioFrame, AmlObjectClass);
    int aac_aot;
};
AML_OBJ_DECLARE_TYPEID(AmlIPCAudioAACFrame, AmlIPCAudioFrame, AmlObjectClass);

struct AmlIPCACodecFDKPriv;
struct AmlIPCACodecFDK {
    AML_OBJ_EXTENDS(AmlIPCACodecFDK, AmlIPCComponent, AmlIPCComponentClass);
    struct AmlIPCACodecFDKPriv* priv;
    struct AmlIPCAudioFormat format;
    int aac_aot;
    int aac_bitrate;
    int aac_bitrate_mode;
    int adts_header;
};
AML_OBJ_DECLARE_TYPEID(AmlIPCACodecFDK, AmlIPCComponent, AmlIPCComponentClass);

struct AmlIPCACodecFDK* aml_ipc_fdkaac_new(enum AmlIPCAudioCodec codec, enum AmlAACAOT aot);

AmlStatus aml_ipc_fdkaac_set_bitrate(struct AmlIPCACodecFDK* fdk, int bitrate);

AmlStatus aml_ipc_fdkaac_set_adts_header(struct AmlIPCACodecFDK* fdk, int present);

AmlStatus aml_ipc_fdkaac_set_codec(struct AmlIPCACodecFDK* fdk, enum AmlIPCAudioCodec codec);

AmlStatus aml_ipc_fdkaac_set_bitrate_mode(struct AmlIPCACodecFDK* fdk, int aac_bitrate_mode);

AmlStatus aml_ipc_fdkaac_set_object_type(struct AmlIPCACodecFDK* fdk, enum AmlAACAOT aot);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !AML_IPC_ACODEC_FDK_H
