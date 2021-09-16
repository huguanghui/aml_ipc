#ifndef _BUFCTRL_H_
#define _BUFCTRL_H_

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef struct mem_desc {
    unsigned int base;
    void* vbase;
    unsigned int size;
    unsigned int align;
} MEM_DESC;

int buffer_malloc(MEM_DESC *mem, int size, int align);
int buffer_malloc_withname_cached(MEM_DESC *mem, int size, int align, const char *name);
int buffer_malloc_withname(MEM_DESC *mem, int size, int align, const char *name);
int buffer_malloc_withname_sram(MEM_DESC *mem, int size, int align, const char *name);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif // !_BUFCTRL_H_
