#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/usb/ch9.h>
#include <linux/time.h>
#include <linux/usb/gadget.h>

#include <media/v4l2-dev.h>

#include "uvc.h"
#include "uvc_queue.h"

static int uvc_video_encode_header(struct uvc_video* video, struct uvc_buffer* buf, u8* data, int len)
{
    return 12;
}

static int uvc_video_encode_data(struct uvc_video* video, struct uvc_buffer* buf, u8* data, int len)
{
    return 0;
}

static void uvc_video_encode_bulk(struct usb_request* req, struct uvc_video* video, struct uvc_buffer* buf)
{
    return;
}

static void uvc_video_encode_isoc(struct usb_request* req, struct uvc_video* video, struct uvc_buffer* bur)
{
    return;
}

static void uvc_video_complete(struct usb_ep* ep, struct usb_request* req)
{
    return;
}

static int uvc_video_free_requests(struct uvc_video* video)
{
    return 0;
}

static int uvc_video_alloc_requests(struct uvc_video* video)
{
    return 0;
}

static int uvc_video_pump(struct uvc_video* video)
{
    return 0;
}

static int uvc_video_enable(struct uvc_video *video, int enable)
{
    return 0;
}

static int uvc_video_init(struct uvc_video *video)
{
    return 0;
}
