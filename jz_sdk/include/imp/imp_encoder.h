#ifndef __IMP_ENCODER_H__
#define __IMP_ENCODER_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef enum {
    IMP_H264_NAL_UNKNOWN = 0,
    IMP_H264_NAL_SLICE = 1,
    IMP_H264_NAL_SLICE_DPA = 2,
    IMP_H264_NAL_SLICE_DPB = 3,
    IMP_H264_NAL_SLICE_DPC = 4,
    IMP_H264_NAL_SLICE_IDR = 5,
    IMP_H264_NAL_SEI = 6,
    IMP_H264_NAL_SPS = 7,
    IMP_H264_NAL_PPS = 8,
    IMP_H264_NAL_AUD = 9,
    IMP_H264_NAL_FILLER = 12,
} IMPEncoderH264NaluType;

typedef enum {
    IMP_H265_NAL_SLICE_TRAIL_N = 0,
    IMP_H265_NAL_SLICE_TRAIL_R = 1,
    IMP_H265_NAL_SLICE_TSA_N = 2,
    IMP_H265_NAL_SLICE_TSA_R = 3,
    IMP_H265_NAL_SLICE_STSA_N = 4,
    IMP_H265_NAL_SLICE_STSA_R = 5,
    IMP_H265_NAL_SLICE_RADL_N = 6,
    IMP_H265_NAL_SLICE_RADL_R = 7,
    IMP_H265_NAL_SLICE_RASL_N = 8,
    IMP_H265_NAL_SLICE_RASL_R = 9,

    IMP_H265_NAL_SLICE_BLA_W_LP = 16,
    IMP_H265_NAL_SLICE_BLA_W_RADL = 17,
    IMP_H265_NAL_SLICE_BLA_N_LP = 18,
    IMP_H265_NAL_SLICE_IDR_W_RADL = 19,
    IMP_H265_NAL_SLICE_IDR_N_LP = 20,
    IMP_H265_NAL_SLICE_CRA = 21,

    IMP_H265_NAL_VPS = 32,
    IMP_H265_NAL_SPS = 33,
    IMP_H265_NAL_PPS = 34,
    IMP_H265_NAL_AUD = 35,
    IMP_H265_NAL_EOS = 36,
    IMP_H265_NAL_EOB = 37,
    IMP_H265_NAL_FILLER_DATA = 38,
    IMP_H265_NAL_PREFIX_SET = 39,
    IMP_H265_NAL_SUFFIX_SET = 40,

    IMP_H265_NAL_INVALID = 64,
} IMPEncoderH265NaluType;

typedef union {
    IMPEncoderH264NaluType h264NalType;
    IMPEncoderH265NaluType h265NalType;
} IMPEncoderNalType;

typedef enum {
    IMP_ENC_SLICE_SI = 4,
    IMP_ENC_SLICE_SP = 3,
    IMP_ENC_SLICE_GOLDEN = 3,
    IMP_ENC_SLICE_I = 2,
    IMP_ENC_SLICE_P = 1,
    IMP_ENC_SLICE_B = 0,
    IMP_ENC_SLICE_CONCEAL = 6,
    IMP_ENC_SLICE_SKIP = 7,
    IMP_ENC_SLICE_REPEAT = 8,
    IMP_ENC_SLICE_MAX_ENUM,
} IMPEncoderSliceType;

typedef struct {
    uint32_t offset;
    uint32_t length;
    int64_t timestamp;
    bool frameEnd;
    IMPEncoderNalType nalType;
    IMPEncoderSliceType sliceType;
} IMPEncoderPack;

typedef struct {
    uint32_t phyAddr;
    uint32_t virAddr;
    uint32_t streamSize;
    IMPEncoderPack* pack;
    uint32_t packCount;
    uint32_t seq;
} IMPEncoderStream;

typedef enum {
    IMP_ENC_TYPE_AVC = 0,
    IMP_ENC_TYPE_HEVC = 1,
    IMP_ENC_TYPE_JPEG = 4,
} IMPEncoderEncType;

#define IMP_ENC_AVC_PROFILE_IDC_BASELINE 66
#define IMP_ENC_AVC_PROFILE_IDC_MAIN 77
#define IMP_ENC_AVC_PROFILE_IDC_HIGH 100
#define IMP_ENC_HEVC_PROFILE_IDC_MAIN 1

typedef enum {
    IMP_ENC_PROFILE_AVC_BASELINE = ((IMP_ENC_TYPE_AVC << 24) | (IMP_ENC_AVC_PROFILE_IDC_BASELINE)),
    IMP_ENC_PROFILE_AVC_MAIN = ((IMP_ENC_TYPE_AVC << 24) | (IMP_ENC_AVC_PROFILE_IDC_MAIN)),
    IMP_ENC_PROFILE_AVC_HIGH = ((IMP_ENC_TYPE_AVC << 24) | (IMP_ENC_AVC_PROFILE_IDC_HIGH)),
    IMP_ENC_PROFILE_HEVC_MAIN = ((IMP_ENC_TYPE_HEVC << 24) | (IMP_ENC_HEVC_PROFILE_IDC_MAIN)),
    IMP_ENC_PROFILE_JPEG = (IMP_ENC_TYPE_JPEG << 24),
} IMPEncoderProfile;

typedef enum {
    IMP_ENC_PIC_FORMAT_400_8BITS = 0x0088,
    IMP_ENC_PIC_FORMAT_420_8BITS = 0x0188,
    IMP_ENC_PIC_FORMAT_422_8BITS = 0x0288,
} IMPEncoderPicFormat;

typedef enum {
    IMP_ENC_OPT_QP_TAB_RELATIVE = 0x00000001,
    IMP_ENC_OPT_FIX_PREDICTOR = 0x00000002,
    IMP_ENC_OPT_CUSTOM_LDA = 0x00000004,
    IMP_ENC_OPT_ENABLE_AUTO_QP = 0x00000008,
    IMP_ENC_OPT_ADAPT_AUTO_QP = 0x00000010,
    IMP_ENC_OPT_COMPRESS = 0x00000020,
    IMP_ENC_OPT_FORCE_REC = 0x00000040,
    IMP_ENC_OPT_FORCE_MV_OUT = 0x00000080,
    IMP_ENC_OPT_HIGH_FREQ = 0x00002000,
    IMP_ENC_OPT_SRD = 0x00008000,
    IMP_ENC_OPT_FORCE_MV_CLIP = 0x00020000,
    IMP_ENC_OPT_RDO_COST_MODE = 0x0004000,
} IMPEncoderEncOptions;

typedef enum {
    IMP_ENC_TOOL_WPP = 0x00000001,
    IMP_ENC_TOOL_TILE = 0x00000002,
    IMP_ENC_TOOL_LF = 0x00000004,
    IMP_ENC_TOOL_LF_X_SLICE = 0x00000008,
    IMP_ENC_TOOL_LF_X_TILE = 0x00000010,
    IMP_ENC_TOOL_SCL_LST = 0x00000020,
    IMP_ENC_TOOL_CONST_INTRA_PRED = 0x00000040,
    IMP_ENC_TOOL_TRANSFO_SKIP = 0x00000080,
    IMP_ENC_TOOL_PCM = 0x00000800,
} IMPEncoderEncTools;

typedef struct {
    bool enable;
    uint32_t x;
    uint32_t y;
    uint32_t w;
    uint32_t h;
} IMPEncoderCropCfg;

typedef struct {
    IMPEncoderProfile eProfile;
    uint8_t uLevel;
    uint8_t uTier;
    uint16_t uWidth;
    uint16_t uHeight;
    IMPEncoderPicFormat ePicFormat;
    uint32_t eEncOptions;
    uint32_t eEncTools;
    IMPEncoderCropCfg crop;
} IMPEncoderEncAttr;

typedef enum {
    IMP_ENC_GOP_CTRL_MODE_DEFAUL = 0x02,
    IMP_ENC_GOP_CTRL_MODE_PYRAMIDAL = 0x04,
    IMP_ENC_GOP_CTRL_MAX_ENUM = 0xff,
} IMPEncoderGopCtrlMode;

typedef struct {
    IMPEncoderGopCtrlMode uGopCtrlMode;
    uint16_t uGopLength;
    uint8_t uNumB;
    uint32_t uMaxSameSenceCnt;
    bool bEnableLT;
    uint32_t uFreqLT;
    bool bLTRC;
} IMPEncoderGopAttr;

typedef enum {
    IMP_ENC_RC_MODE_FIXQP = 0x0,
    IMP_ENC_RC_MODE_CBR = 0x1,
    IMP_ENC_RC_MODE_VBR = 0x2,
    IMP_ENC_RC_MODE_CAPPED_VBR = 0x4,
    IMP_ENC_RC_MODE_CAPPED_QUALITY = 0x8,
    IMP_ENC_RC_MODE_INVALID = 0xff,
} IMPEncoderRcMode;

typedef enum IMPEncoderRcOptions {
    IMP_ENC_RC_OPT_NONE = 0x00000000,
    IMP_ENC_RC_SCN_CHG_RES = 0x00000001,
    IMP_ENC_RC_DELAYED = 0x00000002,
    IMP_ENC_RC_STATIC_SCENE = 0x00000004,
    IMP_ENC_RC_ENABLE_SKIP = 0x00000008,
    IMP_ENC_RC_OPT_SC_PREVENTION = 0x00000010,
    IMP_ENC_RC_MAX_ENUM,
} IMPEncoderRcOptions;

typedef struct {
    int16_t iInitialQP;
} IMPEncoderAttrFixQP;

typedef struct {
    uint32_t uTargetBitRate;
    int16_t iInitialQP;
    int16_t iMinQP;
    int16_t iMaxQP;
    int16_t iIPDelta;
    int16_t iPBDelta;
    uint32_t eRcOptions;
    uint32_t uMaxPictureSize;
} IMPEncoderAttrCbr;

typedef struct {
    uint32_t uTargetBitRate;
    uint32_t uMaxBitRate;
    int16_t iInitialQP;
    int16_t iMinQP;
    int16_t iMaxQP;
    int16_t iIPDelta;
    int16_t iPBDelta;
    uint32_t eRcOptions;
    uint32_t uMaxPictureSize;
} IMPEncoderAttrVbr;

typedef struct {
    uint32_t uTargetBitRate;
    uint32_t uMaxBitRate;
    int16_t iInitialQP;
    int16_t iMinQP;
    int16_t iMaxQP;
    int16_t iIPDelta;
    int16_t iPBDelta;
    uint32_t eRcOptions;
    uint32_t uMaxPictureSize;
    uint16_t uMaxPSNR;
} IMPEncoderAttrCappedVbr;

typedef IMPEncoderAttrCappedVbr IMPEncoderAttrCappedQuality;

typedef struct {
    IMPEncoderRcMode rcMode;
    union {
        IMPEncoderAttrFixQP attrFixQp;
        IMPEncoderAttrCbr attrCbr;
        IMPEncoderAttrVbr attrVbr;
        IMPEncoderAttrCappedVbr attrCappedVbr;
        IMPEncoderAttrCappedQuality attrCappedQuality;
    };
} IMPEncoderAttrRcMode;

typedef struct {
    uint32_t frmRateNum;
    uint32_t frmRateDen;
} IMPEncoderFrmRate;

typedef struct {
    IMPEncoderAttrRcMode attrRcMode;
    IMPEncoderFrmRate outFrmRate;
} IMPEncoderRcAttr;

typedef struct {
    IMPEncoderEncAttr encAttr;
    IMPEncoderRcAttr rcAttr;
    IMPEncoderGopAttr gopAttr;
} IMPEncoderChnAttr;

typedef struct {
    bool registered;
    uint32_t leftPics;
    uint32_t leftStreamBytes;
    uint32_t leftStreamFrames;
    uint32_t curPacks;
    uint32_t work_done;
} IMPEncoderChnStat;

int IMP_Encoder_CreateGroup(int encGroup);

int IMP_Encoder_DestroyGroup(int encGroup);

int IMP_Encoder_SetDefaultParam(IMPEncoderChnAttr* chnAttr, IMPEncoderProfile profile, IMPEncoderRcMode rcMode, uint16_t uWidth, uint16_t uHeight, uint32_t frmRateNum, uint32_t frmRateDen, uint32_t uGopLength, int uMaxSameSenceCnt, int iInitialQP, uint32_t uTargetBitRate);

int IMP_Encoder_CreateChn(int encChn, const IMPEncoderChnAttr* attr);

int IMP_Encoder_DestroyChn(int encChn);

int IMP_Encoder_GetChnAttr(int encChn, IMPEncoderChnAttr* const attr);

int IMP_Encoder_RegisterChn(int encGroup, int encChn);

int IMP_Encoder_UnRegisterChn(int encChn);

int IMP_Encoder_StartRecvPic(int encChn);

int IMP_Encoder_StopRecvPic(int encChn);

int IMP_Encoder_Query(int encChn, IMPEncoderChnStat* stat);

int IMP_Encoder_GetStream(int encChn, IMPEncoderStream* stream, bool blockFlag);

int IMP_Encoder_ReleaseStream(int encChn, IMPEncoderStream* stream);

int IMP_Encoder_PollingModulesStream(uint32_t* encChnBitmap, uint32_t timeoutMsec);

int IMP_Encoder_GetFd(int encChn);

int IMP_Encoder_SetbufshareChn(int encChn, int shareChn);

int IMP_Encoder_SetChnResizeMode(int encChn, int en);

int IMP_Encoder_SetMaxStreamCnt(int encChn, int nrMaxStream);

int IMP_Encoder_GetMaxStreamCnt(int encChn, int* nrMaxStream);

int IMP_Encoder_RequestIDR(int encChn);

int IMP_Encoder_FlushStream(int encChn);

int IMP_Encoder_GetChnFrmRate(int encChn, IMPEncoderFrmRate* pstFps);

int IMP_Encoder_SetChnFrmRate(int encChn, const IMPEncoderFrmRate* pstFps);

int IMP_Encoder_SetChnBitRate(int encChn, int iTargetBitRate, int iMaxBitRate);

int IMP_Encoder_SetChnGopLength(int encChn, int iGopLength);

int IMP_Encoder_GetChnAttrRcMode(int encChn, IMPEncoderAttrRcMode* pstRcModeCfg);

int IMP_Encoder_SetChnAttrRcMode(int encChn, const IMPEncoderAttrRcMode* pstRcModeCfg);

int IMP_Encoder_GetChnGopAttr(int encChn, IMPEncoderGopAttr* pGopAttr);

int IMP_Encoder_SetChnGopAttr(int encChn, const IMPEncoderGopAttr* pGopAttr);

int IMP_Encoder_SetFisheyeEnableStatus(int encChn, int enable);

int IMP_Encoder_GetFisheyeEnableStatus(int encChn, int* enable);

int IMP_Encoder_GetChnEncType(int encChn, IMPEncoderEncType* encType);

int IMP_Encoder_SetPool(int chnNum, int poolID);

int IMP_Encoder_GetPool(int chnNum);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif // !__IMP_ENCODER_H__
