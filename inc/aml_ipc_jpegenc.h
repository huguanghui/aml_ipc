#include "aml_ipc_venc.h"

#ifndef AML_IPC_JPEGENC_H

#define AML_IPC_JPEGENC_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

enum AmlJpegEncChannel {
    AML_JPEGENC_INPUT,
    AML_JPEGENC_OUTPUT
};
struct AmlIPCJpegEncPriv;
struct AmlIPCJpegEnc {
    AML_OBJ_EXTENDS(AmlIPCJpegEnc, AmlIPCComponent, AmlIPCComponentClass);
    struct AmlIPCJpegEncPriv* priv;
    int quality;
    int qtbl_id;
    int pixfmt;
};
AML_OBJ_DECLARE_TYPEID(AmlIPCJpegEnc, AmlIPCComponent, AmlIPCComponentClass);

struct AmlIPCJpegEnc* aml_ipc_jpegenc_new();

AmlStatus aml_ipc_jpegenc_set_quality(struct AmlIPCJpegEnc* enc, int quality);

AmlStatus aml_ipc_jpegenc_encode_frame(struct AmlIPCJpegEnc* enc, struct AmlIPCVideoFrame* frame, struct AmlIPCFrame** out);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !AML_IPC_JPEGENC_H
