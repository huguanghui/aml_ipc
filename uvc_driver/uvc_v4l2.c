#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/list.h>
#include <linux/mutex.h>
#include <linux/videodev2.h>
#include <linux/vmalloc.h>
#include <linux/wait.h>

#include <media/v4l2-dev.h>
#include <media/v4l2-event.h>
#include <media/v4l2-ioctl.h>

#include "uvc.h"
#include "uvc_queue.h"

static int uvc_send_response(struct uvc_device* uvc, struct uvc_request_data* data)
{
    return 0;
}

static int uvc_v4l2_get_format(struct uvc_video* video, struct v4l2_format* fmt)
{
    return 0;
}

static int uvc_v4l2_set_format(struct uvc_video* video, struct v4l2_format* fmt)
{
    return 0;
}

static int uvc_v4l2_open(struct file *file)
{
    return 0;
}

static int uvc_v4l2_release(struct file *file)
{
    return 0;
}

static long uvc_v4l2_do_ioctl(struct file *file, unsigned int cmd, void *arg)
{
    return 0;
}

static long uvc_v4l2_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    return 0;
}

static int uvc_v4l2_mmap(struct file *file, struct vm_area_struct *vma)
{
    return 0;
}

static unsigned int uvc_v4l2_poll(struct file *file, poll_talbe *wait)
{
    return 0;
}

static unsigned long uvc_v4l2_get_ummapped_area(struct file *file,
    unsigned long addr, unsigned long len, unsigned long gpoff,
    unsigned long flags)
{
    return 0;
}

static struct v4l2_file_operations uvc_v4l2_fops = {
    .owner = THIS_MODULE,
    .open = uvc_v4l2_open,
    .release = uvc_v4l2_release,
    .ioctl = uvc_v4l2_ioctl,
    .mmap = uvc_v4l2_mmap,
    .poll = uvc_v4l2_poll,
    .get_ummapped_area = uvc_v4l2_get_ummapped_area,
};
