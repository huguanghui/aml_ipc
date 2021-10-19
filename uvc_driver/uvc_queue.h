#ifndef _UVC_QUEUEH_H_
#define _UVC_QUEUEH_H_

#ifdef __KERNEL__

#include <linux/kernel.h>
#include <linux/poll.h>
#include <linux/videodev2.h>
#include <media/videobuf2-core.h>

#define UVC_MAX_FRAME_SIZE (16 * 1024 * 1024)
#define UVC_MAX_VIDEO_BUFFERS 32

enum uv_buffer_state {
    UVC_BUF_STATE_IDLE = 0,
    UVC_BUF_STATE_QUEUED = 1,
    UVC_BUF_STATE_ACTIVE = 2,
    UVC_BUF_STATE_DONE = 3,
    UVC_BUF_STATE_ERROR = 4,
};

struct uvc_buffer {
    struct vb2_buffer buf;
    struct list_head queue;

    enum uv_buffer_state state;
    void* mem;
    unsigned int length;
    unsigned int bytesused;
};

#define UVC_QUEUE_DISCONNECTED (1 << 0)
#define UVC_QUEUE_DROP_INCOMPLETE (1 << 1)
#define UVC_QUEUE_PAUSED (1 << 2)

struct uvc_video_queue {
    struct vb2_queue queue;
    struct mutex mutex;

    unsigned int flags;
    __u32 sequence;

    unsigned int buf_used;
    spinlock_t irqlock;
    struct list_head irqqueue;
};

static inline int uvc_queue_streaming(struct uvc_video_queue* queue)
{
    return vb2_is_streaming(&queue->queue);
}

#endif //

#endif // !_UVC_QUEUEH_H_
