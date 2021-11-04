#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include "video.h"

#include "f_uvc.h"

#include "uvc_queue.c"
#include "uvc_video.c"
#include "uvc_v4l2.c"
#include "f_uvc.c"

int af_en = 0;
module_param(af_en, int, S_IRUGO);
int uvc_extension_en = 0;
module_param(uvc_extension_en, int, S_IRUGO);

static struct uvc_descriptor_header** ucamera_streaming_cls = NULL;
static unsigned int stillimg_en = 0;

/* --------------------------------------------------------------------------
 * Device Descriptor
 */
DECLARE_UVC_HEADER_DESCRIPTOR(1);

static const struct UVC_HEADER_DESCRIPTOR(1) uvc_control_header = {
    .bLength = UVC_DT_HEADER_SIZE(1),
    .bDescriptorType = USB_DT_CS_INTERFACE,
    .bDescriptorSubType = UVC_VC_HEADER,
    .bcdUVC = cpu_to_le16(0x0100),
    .wTotalLength = 0,
    .dwClockFrequency = cpu_to_le32(48000000),
    .bInCollection = 0,
    .baInterfaceNr[0] = { 0 },
};

static struct uvc_descriptor_header* uvc_fs_control_cls[8] = {
    (const struct uvc_descriptor_header*)&uvc_control_header,
    (const struct uvc_descriptor_header*)&uvc_camera_terminal,
    (const struct uvc_descriptor_header*)&uvc_processing,
    (const struct uvc_descriptor_header*)&uvc_output_terminal,
    NULL,
};

static const struct uvc_descriptor_header* const uvc_ss_control_cls[] = {
    (const struct uvc_descriptor_header*)&uvc_input_header,
    (const struct uvc_descriptor_header*)&uvc_camera_terminal,
    (const struct uvc_descriptor_header*)&uvc_processing,
    (const struct uvc_descriptor_header*)&uvc_output_terminal,
    NULL,
};

static const struct uvc_descriptor_header* const uvc_fs_streaming_cls[] = {
    (cosnt struct uvc_descriptor_header*)&uvc_input_header,
};

/* --------------------------------------------------------------------------
 * USB Configuration
 */

int ucam_bind_uvc(struct usb_configuration* c)
{
    if (ucamera_streaming_cls == NULL) {
        printk("webcam bind uvc config failed\n");
        return -1;
    }
    if (af_en) {
        uvc_camera_terminal.bmControls[0] = 0x2a;
        uvc_camera_terminal.bmControls[3] = 0x02;
    }
    if (uvc_extension_en) {
        uvc_output_terminal.bSourceID = 4;
        uvc_fs_control_cls[3] = &uvc_extension_unit;
        uvc_fs_control_cls[4] = &uvc_output_terminal;
        uvc_fs_control_cls[5] = NULL;
    }
    return uvc_bind_config(c, uvc_fs_control_cls, uvc_ss_control_cls,
        uvc_fs_streaming_cls, ucamera_streaming_cls, uvc_ss_streaming_cls);
}
EXPORT_SYMBOL(ucam_bind_uvc);

/* --------------------------------------------------------------------------
 * Driver
 */

int uvc_set_descriptor(unsigned long arg)
{
    return 0;
}
EXPORT_SYMBOL(uvc_set_descriptor);

int uvc_sti_init(void)
{
    return 0;
}
EXPORT_SYMBOL(uvc_sti_init);
