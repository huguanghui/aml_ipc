#ifndef __FH_SYSTEM_MPI_H__
#define __FH_SYSTEM_MPI_H__

#include "types/type_def.h"
#include "fh_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

FH_SINT32 FH_SYS_Init(FH_VOID);

FH_SINT32 FH_SYS_Init_Pre(FH_VOID);

FH_SINT32 FH_SYS_Init_Post(FH_VOID);

FH_SINT32 FH_SYS_Exit(FH_VOID);

FH_SINT32 FH_SYS_BindVpu2Enc(FH_UINT32 srcchn, FH_UINT32 dstschn);

FH_SINT32 FH_SYS_BindVpu2Bgm(FH_VOID);

FH_SINT32 FH_SYS_BindVpu2Nn(FH_UINT32 dstchn);

FH_SINT32 FH_SYS_GetBindbyDest(FH_UINT32* srcchn, FH_UINT32 dstschn);

FH_SINT32 FH_SYS_UnBindbySrc(FH_UINT32 srcchn);

FH_SINT32 FH_SYS_UnBindbyDst(FH_UINT32 dstchn);

FH_SINT32 FH_SYS_GetVersion(FH_VERSION_INFO* pstSystemversion);

FH_CHAR* FH_DSP_Version(FH_UINT32 print_enable);

FH_UINT32 FH_SYS_GetChipID(FH_VOID);

FH_SINT32 FH_SYS_SetReg(FH_UINT32 addr, FH_UINT32 value);

FH_SINT32 FH_SYS_GetReg(FH_UINT32 u32Addr, FH_UINT32* pu32Value);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif // !__FH_SYSTEM_MPI_H__
