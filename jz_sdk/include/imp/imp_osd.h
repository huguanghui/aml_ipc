#ifndef __IMP_OSD_H__
#define __IMP_OSD_H__

#include "imp_common.h"
#include <stdint.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define INVHANDLE (-1)

typedef int IMPRgnHandle;

typedef enum {
    OSD_BLACK = 0xff000000,
    OSD_WHITE = 0xffffffff,
    OSD_RED = 0xffff0000,
    OSD_GREEN = 0xff00ff00,
    OSD_BLUE = 0xff0000ff,
} IMPOsdColour;

typedef enum {
    OSD_REG_INV = 0,
    OSD_REG_LINE = 1,
    OSD_REG_RECT = 2,
    OSD_REG_BITMAP = 3,
    OSD_REG_COVER = 4,
    OSD_REG_PIC = 5,
    OSD_REG_PIC_RMEM = 6,
} IMPOsdRgnType;

typedef struct {
    uint32_t color;
    uint32_t linewidth;
} lineRectData;

typedef struct {
    uint32_t color;
} coverData;

typedef struct {
    void* pData;
} picData;

typedef union {
    void* bitmapData;
    lineRectData lineRectData;
    coverData coverData;
    picData picData;
} IMPOSDRgnAttrData;

typedef struct {
    IMPOsdRgnType type;
    IMPRect rect;
    IMPPixelFormat fmt;
    IMPOSDRgnAttrData data;
} IMPOSDRgnAttr;

typedef struct {
    uint64_t ts;
    uint64_t minus;
    uint64_t plus;
} IMPOSDRgnTimestamp;

typedef struct {
    int show;
    IMPPoint offPos;
    float scalex;
    float scaley;
    int gAlphaEn;
    int fgAlpha;
    int bgAlpha;
    int layer;
} IMPOSDGrpRgnAttr;

int IMP_OSD_CreateGroup(int grpNum);

int IMP_OSD_DestroyGroup(int grpNum);

int IMP_OSD_AttachToGroup(IMPCell* from, IMPCell* to);

IMPRgnHandle IMP_OSD_CreateRgn(IMPOSDRgnAttr* prAttr);

void IMP_OSD_DestroyRgn(IMPRgnHandle handle);

int IMP_OSD_RegisterRgn(IMPRgnHandle handle, int grpNum, IMPOSDGrpRgnAttr* pgrAttr);

int IMP_OSD_UnRegisterRgn(IMPRgnHandle handle, int grpNum);

int IMP_OSD_SetRgnAttr(IMPRgnHandle handle, IMPOSDRgnAttr* prAttr);

int IMP_OSD_SetRgnAttrWithTimestamp(IMPRgnHandle handle, IMPOSDRgnAttr* prAttr, IMPOSDRgnTimestamp* prTs);

int IMP_OSD_GetRgnAttr(IMPRgnHandle handle, IMPOSDRgnAttr* prAttr);

int IMP_OSD_UpdateRgnAttrData(IMPRgnHandle handle, IMPOSDRgnAttrData* prAttrData);

int IMP_OSD_SetGrpRgnAttr(IMPRgnHandle handle, int grpNum, IMPOSDGrpRgnAttr* pgrAttr);

int IMP_OSD_GetGrpRgnAttr(IMPRgnHandle handle, int grpNum, IMPOSDGrpRgnAttr* pgrAttr);

int IMP_OSD_ShowRgn(IMPRgnHandle handle, int grpNum, int showFlag);

int IMP_OSD_Start(int grpNum);

int IMP_OSD_Stop(int grpNum);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif // !__IMP_OSD_H__
