#ifndef __IMP_IVS_MOVE_H__
#define __IMP_IVS_MOVE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define IMP_IVS_MOVE_MAX_ROI_CNT 52

#include <imp/imp_ivs.h>

typedef struct {
    int sense[IMP_IVS_MOVE_MAX_ROI_CNT];
    int skipFrameCnt;
    IMPFrameInfo frameInfo;
    IMPRect roiRect[IMP_IVS_MOVE_MAX_ROI_CNT];
    int roiRectCnt;
} IMP_IVS_MoveParam;

typedef struct {
    int retRoi[IMP_IVS_MOVE_MAX_ROI_CNT];
} IMP_IVS_MoveOutput;

IMPIVSInterface *IMP_IVS_CreateMoveInterface(IMP_IVS_MoveParam *param);

void IMP_IVS_DestroyMoveInterface(IMPIVSInterface *moveInterface);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif // !__IMP_IVS_MOVE_H__
