#ifndef AML_IPC_VENC_H

#define AML_IPC_VENC_H

#include "aml_ipc_component.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct AmlRectFloat {
    float left;
    float top;
    float width;
    float height;
};
enum AmlIPCVencChannel {
    AML_VENC_INPUT,
    AML_VENC_OUTPUT,
};

#define AML_VENC_MAX_QUALITY_LEVEL 6

enum AmlIPCVencGopOpt {
    AML_VENC_GOP_OPT_NONE,
    AML_VENC_GOP_OPT_ALL_I,
    AML_VENC_GOP_OPT_IP,
    AML_VENC_GOP_OPT_IBBBP,
    AML_VENC_GOP_OPT_IP_SVC1,
    AML_VENC_GOP_OPT_IP_SVC2,
    AML_VENC_GOP_OPT_IP_SVC3,
    AML_VENC_GOP_OPT_IP_SVC4
};

enum AmlIPCVencIntraRefreshMode {
    AML_VENC_INTRA_REFRESH_DISABLE,
    AML_VENC_INTRA_REFRESH_ROW,
    AML_VENC_INTRA_REFRESH_COLUMN,
    AML_VENC_INTRA_REFRESH_STEP,
    AML_VENC_INTRA_REFRESH_ADAPTIVE,
};

enum AmlIPCVencProfile {
    AML_VENC_PROFILE_AUTO,
    AML_VENC_PROFILE_H264_BASELINE,
    AML_VENC_PROFILE_H264_MAIN,
    AML_VENC_PROFILE_H264_HGIH,
};

struct AmlIPCVencPriv;
struct AmlIPCVenc {
    AML_OBJ_EXTENDS(AmlIPCVenc, AmlIPCComponent, AmlIPCComponentClass);
    struct AmlIPCVencPriv* priv;
    enum AmlIPCVideoCodec codec;
    int width;
    int height;
    enum AmlPixelFormat pixfmt;
    int gop;
    int framerate;
    int bitrate;
    int min_buffers;
    int max_buffers;
    int encoder_bufsize;
    int quality_level;
    int gop_opt;
    enum AmlIPCVencIntraRefreshMode intra_refresh_mode;
    int intra_refresh_num;
    int ltr_idr_pq_delta;
    int ltr_goodp_pq_delta;
    int ltr_goodp_interval;
    enum AmlIPCVencProfile profile;
    int p_qp_delta;
    int i_qp_min;
    int i_qp_max;
    int p_qp_min;
    int p_qp_max;
    int b_qp_min;
    int b_qp_max;
    int qp_mode;
    int max_delta_qp;
    int multi_slice_nCTU;
    int multi_slice_size;
    int rotation;
    int mirroring;
    int roi_enabled;
};
AML_OBJ_DECLARE_TYPEID(AmlIPCVenc, AmlIPCComponent, AmlIPCComponentClass);

struct AmlIPCVenc* aml_ipc_venc_new(enum AmlIPCVideoCodec codec);

AmlStatus aml_ipc_venc_set_bitrate(struct AmlIPCVenc* enc, int bitrate);
AmlStatus aml_ipc_venc_set_gop_size(struct AmlIPCVenc* enc, int gopsize);
AmlStatus aml_ipc_venc_force_insert_idr(struct AmlIPCVenc* enc);
AmlStatus aml_ipc_venc_set_roi(struct AmlIPCVenc* enc, int quality, struct AmlRectFloat* rect,
    int index);
AmlStatus aml_ipc_venc_clear_roi(struct AmlIPCVenc* enc, int index);
AmlStatus aml_ipc_venc_set_quality_level(struct AmlIPCVenc* enc, int quality_level);

AmlStatus aml_ipc_venc_set_codec(struct AmlIPCVenc* enc, enum AmlIPCVideoCodec codec);

AmlStatus aml_ipc_venc_set_gop(struct AmlIPCVenc* enc, int gop);

AmlStatus aml_ipc_venc_set_bufsize(struct AmlIPCVenc* enc, int size);

AmlStatus aml_ipc_venc_set_gop_option(struct AmlIPCVenc* enc, enum AmlIPCVencGopOpt opt);

AmlStatus aml_ipc_venc_set_intra_refresh(struct AmlIPCVenc* enc, enum AmlIPCVencIntraRefreshMode mode, int nCTU);

AmlStatus aml_ipc_venc_set_longterm_reference(struct AmlIPCVenc* enc, int ltr_goodp_interval,
    int ltr_goodp_qp_delta, int ltr_idr_qp_delta);

AmlStatus aml_ipc_venc_set_profile(struct AmlIPCVenc* enc, enum AmlIPCVencProfile profile);

AmlStatus aml_ipc_venc_set_idr_qp_delta(struct AmlIPCVenc* enc, int delta_min, int delta_max);

AmlStatus aml_ipc_venc_set_qp_range(struct AmlIPCVenc* enc, int minqpI, int maxqpI, int minqpP,
    int maxqpP, int minqpB, int maxqpB);

AmlStatus aml_ipc_venc_set_slice_split(struct AmlIPCVenc* enc, int nCTU, int size);

AmlStatus aml_ipc_venc_set_rotation(struct AmlIPCVenc* enc, int angle);

AmlStatus aml_ipc_venc_set_mirroring(struct AmlIPCVenc* enc, int horizontal, int vertical);

AmlStatus aml_ipc_venc_enable_roi_feature(struct AmlIPCVenc* enc, int enable);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !AML_IPC_VENC_H
