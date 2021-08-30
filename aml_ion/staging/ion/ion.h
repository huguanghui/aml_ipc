#ifndef _ION_H
#define _ION_H

#include <linux/device.h>
#include <linux/dma-direction.h>
#include <linux/kref.h>
#include <linux/miscdevice.h>
#include <linux/mm_types.h>
#include <linux/mutex.h>
#include <linux/rbtree.h>
#include <linux/sched.h>
#include <linux/shrinker.h>
#include <linux/types.h>

#include "../uapi/ion.h"

struct ion_platform_heap {
    enum ion_heap_type type;
    unsigned int id;
    const char* name;
    phys_addr_t base;
    size_t size;
    phys_addr_t align;
    void* priv;
};

struct ion_buffer {
    union {
        struct rb_node node;
        struct list_head list;
    };
    struct ion_device* dev;
    struct ion_heap* heap;
    unsigned long flags;
    unsigned long private_flags;
    size_t size;
    void* priv_virt;
    struct mutex lock;
    int kmap_cnt;
    void* vaddr;
    struct sg_table* sg_table;
    struct list_head attachments;
};

void ion_buffer_destroy(struct ion_buffer* buffer);

struct ion_device {
    struct miscdevice dev;
    struct rb_root buffers;
    struct mutex buffer_lock;
    struct rw_semaphore lock;
    struct plist_head heaps;
    struct dentry* debug_root;
    int heap_cnt;
};

struct ion_heap_ops {
    int (*allocate)(struct ion_heap* heap, struct ion_buffer* buffer,
        unsigned long len, unsigned long flags);
    void (*free)(struct ion_buffer* buffer);
    void* (*map_kernel)(struct ion_heap* heap, struct ion_buffer* buffer);
    void (*unmap_kernel)(struct ion_heap* mapper, struct ion_buffer* buffer);
    int (*map_user)(struct ion_heap* mapper, struct ion_buffer* buffer, struct vm_area_struct* vma);
    int (*shrink)(struct ion_heap* heap, gfp_t gfp_mask, int nr_to_scan);
};

#define ION_HEAP_FLAG_DEFER_FREE BIT(0)
#define ION_PRIV_FLAG_SHRINKER_FREE BIT(0)

struct ion_heap {
    struct plist_node node;
    struct ion_device* dev;
    enum ion_heap_type type;
    struct ion_heap_ops* ops;
    unsigned long flags;
    unsigned int id;
    const char* name;
    struct shrinker shrinker;
    struct list_head free_list;
    size_t free_list_size;
    spinlock_t free_lock;
    wait_queue_head_t waitqueue;
    struct task_struct* task;

    int (*debug_show)(struct ion_heap* heap, struct seq_file* s, void* unused);
};

void ion_device_add_heap(struct ion_heap* heap);

void* ion_heap_map_kernel(struct ion_heap* heap, struct ion_buffer* buffer);
void ion_heap_unmap_kernel(struct ion_heap* heap, struct ion_buffer* buffer);
int ion_heap_map_user(struct ion_heap* heap, struct ion_buffer* buffer, struct vm_area_struct* vma);
int ion_heap_buffer_zero(struct ion_buffer* buffer);
int ion_heap_pages_zero(struct pageg* page, size_t size, pgprot_t pgprot);

int ion_alloc(size_t len, unsigned int heap_id_mask, unsigned int flags);

int ion_heap_init_shrinker(struct ion_heap* heap);

int ion_heap_init_deferred_free(struct ion_heap* heap);

void ion_heap_freelist_add(struct ion_heap* heap, struct ion_buffer* buffer);

size_t ion_heap_freelist_drain(struct ion_heap* heap, size_t size);

size_t ion_heap_freelist_shrink(struct ion_heap* heap, size_t size);

size_t ion_heap_freelist_size(struct ion_heap* heap);

struct ion_page_pool {
    int high_count;
    int low_count;
    struct list_head high_items;
    struct list_head low_items;
    struct mutex mutex;
    gfp_t gfp_mask;
    unsigned int order;
    struct plist_node list;
};

struct ion_page_pool* ion_page_pool_create(gfp_t gfp_mask, unsigned int order);
void ion_page_pool_destroy(struct ion_page_pool* pool);
struct page* ion_page_pool_alloc(struct ion_page_pool* pool);
void ion_page_pool_free(struct ion_page_pool* pool, struct page* page);

int ion_page_pool_shrink(struct ion_page_pool* pool, gfp_t gfp_mask, int nr_to_scan);

long ion_ioctl(struct file* filp, unsigned int cmd, unsigned long arg);

#ifdef CONFIG_AMLOGIC_MODIFY
int meson_ion_cma_heap_match(const char* name);
void meson_ion_cma_heap_id_set(unsigned int id);
struct device* meson_ion_get_dev(void);
#endif

#endif // !_ION_H
