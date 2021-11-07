#include <asm-generic/errno-base.h>
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

static unsigned int streaming_interval = 1;
module_param(streaming_interval, uint, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(streaming_interval, "1 - 16");

static unsigned int streaming_maxpacket = 3072;
module_param(streaming_maxpacket, uint, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(streaming_maxpacket, "1 - 1023(FS), 1 - 3072 (hs/ss)");

static unsigned int streaming_maxburst;
module_param(streaming_maxburst, uint, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(streaming_maxburst, "0 - 15 (ss only)");

static void uvc_function_ep0_complete(struct usb_ep* ep, struct usb_request* req)
{
    return;
}

static int uvc_function_setup(struct usb_function* f, const struct usb_ctrlrequest* ctrl)
{
    struct uvc_device* uvc = to_uvc(f);
    struct v4l2_event v4l2_event;
    struct uvc_event* uvc_event = (void*)&v4l2_event.u.data;

    if ((ctrl->bRequestType & USB_TYPE_MASK) != USB_TYPE_CLASS) {
        return -EINVAL;
    }

    if (le16_to_cpu(ctrl->wLength) > UVC_MAX_REQUEST_SIZE) {
        return -EINVAL;
    }

    memset(&v4l2_event, 0, sizeof(v4l2_event));
    v4l2_event.type = UVC_EVENT_SETUP;
    memcpy(&uvc_event->req, ctrl, sizeof(uvc_event->req));
    v4l2_event_queue(uvc->vdev, &v4l2_event);

    return 0;
}

void uvc_function_setup_continue(struct uvc_device* uvc)
{
    return;
}

static int uvc_function_get_alt(struct usb_function* f, unsigned interface)
{
    return 0;
}

static int uvc_function_set_alt(struct usb_function* f, unsigned interface, unsigned alt)
{
    return 0;
}

static void uvc_function_disable(struct usb_function* f)
{
    return;
}

void uvc_function_connect(struct uvc_device* uvc)
{
    return;
}

void uvc_function_disconnect(struct uvc_device* uvc)
{
    return;
}

static int uvc_register_video(struct uvc_device* uvc)
{
    struct usb_composite_dev *cdev = uvc->func.config->cdev;
    struct video_device *video;

    video = video_device_alloc();
    if (video == NULL) {
        return -ENOMEM;
    }

    video->parent = &cdev->gadget->dev;
    video->fops = &uvc_v4l2_fops;
    video->release = video_device_release;
    strlcpy(video->name, cdev->gadget->name, sizeof(video->name));

    uvc->vdev = video;
    video_set_drvdata(video, uvc);

    return video_register_device(video, VFL_TYPE_GRABBER, -1);
}

static struct usb_descriptor_header** uvc_copy_descriptors(struct uvc_device* uvc, enum usb_device_speed speed)
{
    return NULL;
}

static void uvc_function_unbind(struct usb_configuration* c, struct usb_function* f)
{
    return;
}

static int uvc_function_bind(struct usb_configuration* c, struct usb_function* f)
{
    struct usb_composite_dev* cdev = c->cdev;
    struct uvc_device* uvc = to_uvc(f);
    unsigned int max_packet_mult;
    unsigned int max_packet_size;
    struct usb_ep* ep;
    int ret = -EINVAL;

    streaming_interval = clamp(streaming_interval, 1U, 16U);
    streaming_maxpacket = clamp(streaming_maxpacket, 1U, 3072U);
    streaming_maxburst = min(streaming_maxburst, 15U);

    if (streaming_maxpacket <= 1024) {
        max_packet_mult = 1;
        max_packet_size = streaming_maxpacket;
    } else if (streaming_maxpacket <= 2048) {
        max_packet_mult = 2;
        max_packet_size = streaming_maxpacket / 2;
    } else {
        max_packet_mult = 3;
        max_packet_size = streaming_maxpacket / 3;
    }

    uvc_fs_streaming_ep.wMaxPacketSize = min(streaming_maxpacket, 1023U);
    uvc_fs_streaming_ep.bInterval = streaming_interval;

    uvc_hs_streaming_ep.wMaxPacketSize = max_packet_size;
    uvc_hs_streaming_ep.wMaxPacketSize |= ((streaming_maxburst - 1) << 11);
    uvc_hs_streaming_ep.bInterval = streaming_interval;

    uvc_ss_streaming_ep.wMaxPacketSize = max_packet_size;
    uvc_ss_streaming_ep.bInterval = streaming_interval;
    uvc_ss_streaming_comp.bmAttributes = max_packet_mult - 1;
    uvc_ss_streaming_comp.bMaxBurst = streaming_maxburst;
    uvc_ss_streaming_comp.wBytesPerInterval = max_packet_size * max_packet_mult * streaming_maxburst;

    ep = usb_ep_autoconfig(cdev->gadget, &uvc_control_ep);
    if (!ep) {
        goto error;
    }
    uvc->control_ep = ep;
    ep->driver_data = uvc;

    if (gadget_is_superspeed(c->cdev->gadget)) {
        ep = usb_ep_autoconfig_ss(cdev->gadget, &uvc_ss_streaming_ep, &uvc_ss_streaming_comp);
    } else if (gadget_is_dualspeed(cdev->gadget)) {
        ep = usb_ep_autoconfig(cdev->gadget, &uvc_hs_streaming_ep);
    } else {
        ep = usb_ep_autoconfig(cdev->gadget, &uvc_fs_streaming_ep);
    }

    if (!ep) {
        goto error;
    }
    uvc->video.ep = ep;
    ep->driver_data = uvc;

    uvc_fs_streaming_ep.bEndpointAddress = uvc->video.ep->address;
    uvc_hs_streaming_ep.bEndpointAddress = uvc->video.ep->address;
    uvc_ss_streaming_ep.bEndpointAddress = uvc->video.ep->address;

    if ((ret = usb_interface_id(c, f)) < 0) {
        goto error;
    }
    uvc_iad.bFirstInterface = ret;
    uvc_control_intf.bInterfaceNumber = ret;
    uvc->control_intf = ret;

    if ((ret = usb_interface_id(c, f)) < 0) {
        goto error;
    }
    uvc_streaming_intf_alt0.bInterfaceNumber = ret;
    uvc_streaming_intf_alt1.bInterfaceNumber = ret;
    uvc->streaming_intf = ret;

    f->fs_descriptors = uvc_copy_descriptors(uvc, USB_SPEED_FULL);
    if (gadget_is_dualspeed()) {
        f->hs_descriptors = uvc_copy_descriptors(uvc, USB_SPEED_HIGH);
    }
    if (gadget_is_superspeed()) {
        f->ss_descriptors = uvc_copy_descriptors(uvc, USB_SPEED_SUPER);
    }

    uvc->control_req = usb_ep_alloc_request(cdev->gadget->ep0, GFP_KERNEL);
    uvc->control_buf = kmalloc(UVC_MAX_REQUEST_SIZE, GFP_KERNEL);
    if (uvc->control_req == NULL || uvc->control_buf == NULL) {
        ret = -ENOMEM;
        goto error;
    }

    uvc->control_req->buf = uvc->control_buf;
    uvc->control_req->complete = uvc_function_ep0_complete;
    uvc->control_req->context = uvc;

    if ((ret = usb_function_deactivate(f)) < 0) {
        goto error;
    }

    ret = uvc_video_init(&uvc->video);
    if (ret < 0) {
        goto error;
    }

    ret = uvc_register_video(uvc);
    if (ret < 0) {
        printk(KERN_INFO "Unable to register video device\n");
        goto error;
    }

    return 0;

error:
    if (uvc->vdev) {
        video_device_release(uvc->vdev);
    }

    if (uvc->control_ep) {
        uvc->control_ep->driver_data = NULL;
    }
    if (uvc->video.ep) {
        uvc->video.ep->driver_data = NULL;
    }

    if (uvc->control_req) {
        usb_ep_free_request(cdev->gadget->ep0, uvc->control_ep);
        kfree(uvc->control_buf);
    }

    usb_free_all_descriptors(f);
    return 0;
}

int uvc_bind_config(struct usb_configuration* c,
    const struct uvc_descriptor_header* const* fs_control,
    const struct uvc_descriptor_header* const* ss_control,
    const struct uvc_descriptor_header* const* fs_streaming,
    const struct uvc_descriptor_header* const* hs_streaming,
    const struct uvc_descriptor_header* const* ss_streaming)
{
    struct uvc_device* uvc;
    int ret = 0;

    if (!gadget_is_dualspeed(c->cdev->gadget)) {
        return -EINVAL;
    }

    uvc = kzalloc(sizeof(*uvc), GFP_KERNEL);
    if (uvc == NULL) {
        return -ENOMEM;
    }
    uvc->state = UVC_STATE_DISCONNECTED;

    if (fs_control == NULL || fs_control[0] == NULL || fs_control[0]->bDescriptorSubType != UVC_VC_HEADER) {
        goto error;
    }

    if (ss_control == NULL || ss_control[0] == NULL || ss_control[0]->bDescriptorSubType != UVC_VC_HEADER) {
        goto error;
    }

    if (fs_streaming == NULL || fs_streaming[0] == NULL || fs_streaming[0]->bDescriptorSubType != UVC_VS_INPUT_HEADER) {
        goto error;
    }

    if (hs_streaming == NULL || hs_streaming[0] == NULL || hs_streaming[0]->bDescriptorSubType != UVC_VS_INPUT_HEADER) {
        goto error;
    }

    if (ss_streaming == NULL || ss_streaming[0] == NULL || ss_streaming[0]->bDescriptorSubType != UVC_VS_INPUT_HEADER) {
        goto error;
    }

    uvc->desc.fs_control = fs_control;
    uvc->desc.ss_control = ss_control;
    uvc->desc.fs_streaming = fs_streaming;
    uvc->desc.hs_streaming = hs_streaming;
    uvc->desc.ss_streaming = ss_streaming;

    if (uvc_en_us_strings[UVC_STRING_CONTROL_IDX].id == 0) {
        ret = usb_string_ids_tab(c->cdev, uvc_en_us_strings);
        if (ret) {
            goto error;
        }
        uvc_iad.iFunction = uvc_en_us_strings[UVC_STRING_CONTROL_IDX].id;
        uvc_control_intf.iInterface = uvc_en_us_strings[UVC_STRING_CONTROL_IDX].id;
    }

    /* Register the function. */
    uvc->func.name = "uvc";
    uvc->func.strings = uvc_function_strings;
    uvc->func.bind = uvc_function_bind;
    uvc->func.unbind = uvc_function_unbind;
    uvc->func.get_alt = uvc_function_get_alt;
    uvc->func.set_alt = uvc_function_set_alt;
    uvc->func.disable = uvc_function_disable;
    uvc->func.setup = uvc_function_setup;
    ret = usb_add_function(c, &uvc->func);
    if (ret) {
        kfree(uvc);
    }
    return ret;

error:
    kfree(uvc);
    return 0;
}

int uvc_name_set(char* name)
{
    uvc_en_us_strings[UVC_STRING_CONTROL_IDX].s = (const char*)name;
    return 0;
}
EXPORT_SYMBOL(uvc_name_set);

module_param_named(trace, uvc_gadget_trace_param, uint, S_IRUGO | S_IWUSR);
MODULE_PARAM_DESC(trace, "Trace level bitmask");
