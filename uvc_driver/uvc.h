#ifndef _UVC_GADGET_H_
#define _UVC_GADGET_H_

#include <linux/ioctl.h>
#include <linux/types.h>
#include <linux/usb/ch9.h>

#define UVC_EVENT_FIRST (UVC_EVENT_PRIVATE_START + 0)
#define UVC_EVENT_CONNECT (UVC_EVENT_PRIVATE_START + 0)
#define UVC_EVENT_DISCONNECT (UVC_EVENT_PRIVATE_START + 1)
#define UVC_EVENT_STREAMON (UVC_EVENT_PRIVATE_START + 2)
#define UVC_EVENT_STREAMOFF (UVC_EVENT_PRIVATE_START + 3)
#define UVC_EVENT_SETUP (UVC_EVENT_PRIVATE_START + 4)
#define UVC_EVENT_DATA (UVC_EVENT_PRIVATE_START + 5)
#define UVC_EVENT_LAST (UVC_EVENT_PRIVATE_START + 5)

#ifdef __KERNEL__

struct uvc_device {
    struct video_device* vdev;
    enum uvc_state state;
    struct usb_function func;
    struct uvc_video video;

    struct {
        const struct uvc_descriptor_header* const* fs_control;
        const struct uvc_descriptor_header* const* ss_control;
        const struct uvc_descriptor_header* const* fs_streaming;
        const struct uvc_descriptor_header* const* hs_streaming;
        const struct uvc_descriptor_header* const* ss_streaming;
    } desc;

    unsigned int control_intf;
    struct usb_ep* control_ep;
    struct usb_request* control_req;
    void* control_buf;

    unsigned int streaming_intf;

    /* Event */
    unsigned int event_length;
    unsigned int event_setup_out : 1;
};

#endif

#endif // !_UVC_GADGET_H_
