#ifndef _F_UVC_H_
#define _F_UVC_H_

#include <linux/usb/composite.h>
#include <linux/usb/video.h>

int uvc_bind_config(struct usb_configuration* c,
    const struct uvc_descriptor_header* const* fs_control,
    const struct uvc_descriptor_header* const* hs_control,
    const struct uvc_descriptor_header* const* fs_streaming,
    const struct uvc_descriptor_header* const* hs_streaming,
    const struct uvc_descriptor_header* const* ss_streaming);

#endif // !_F_UVC_H_
