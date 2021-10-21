#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/list.h>
#include <linux/mutex.h>
#include <linux/string.h>
#include <linux/usb/ch9.h>
#include <linux/usb/gadget.h>
#include <linux/usb/video.h>
#include <linux/vmalloc.h>
#include <linux/wait.h>

#include <media/v4l2-dev.h>
#include <media/v4l2-event.h>

#include "uvc.h"

unsigned int uvc_gadget_trace_param;


static void uvc_function_ep0_complete(struct usb_ep *ep, struct usb_request *req)
{
    return;
}

static int uvc_function_setup(struct usb_function *f, const struct usb_ctrlrequest *ctrl)
{
    return 0;
}

void uvc_function_setup_continue(struct uvc_device *uvc)
{
    return;
}

static int uvc_function_get_alt(struct usb_function *f, unsigned interface)
{
    return 0;
}

static int uvc_function_set_alt(struct usb_function *f, unsigned interface, unsigned alt)
{
    return 0;
}

static void uvc_function_disable(struct usb_function *f)
{
    return;
}

void uvc_function_connect(struct uvc_device *uvc)
{
    return;
}

void uvc_function_disconnect(struct uvc_device *uvc)
{
    return;
}

static int uvc_register_video(struct uvc_device *uvc)
{
    return 0;
}

static struct usb_descriptor_header **uvc_copy_descriptors(struct uvc_device *uvc, enum usb_device_speed speed)
{
    return NULL;
}

static void uvc_function_unbind(struct usb_configuration *c, struct usb_function *f)
{
    return;
}

static int uvc_function_bind(struct usb_configuration *c, struct usb_function *f)
{
    return 0;
}

int uvc_bind_config(struct usb_configuration *c,
    const struct uvc_descriptor_header * const *fs_control,
    const struct uvc_descriptor_header * const *ss_control,
    const struct uvc_descriptor_header * const *fs_streaming,
    const struct uvc_descriptor_header * const *hs_streaming,
    const struct uvc_descriptor_header * const *ss_streaming)
{
    return 0;
}

int uvc_name_set(char *name)
{
    uvc_en_us_strings[UVC_STRING_CONTROL_IDX].s = (const char*)name;
    return 0;
}
EXPORT_SYMBOL(uvc_name_set);

module_param_named(trace, uvc_gadget_trace_param, uint, S_IRUGO|S_IWUSR);
MODULE_PARAM_DESC(trace, "Trace level bitmask");
