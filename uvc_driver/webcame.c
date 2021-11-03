#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include "video.h"

#include "f_uvc.h"

#include "uvc_queue.c"
#include "uvc_video.c"
#include "uvc_v4l2.c"
#include "f_uvc.c"

/* --------------------------------------------------------------------------
 * Device Descriptor
 */

int af_en = 0;
module_param(af_en, int, S_IRUGO);
int uvc_extension_en = 0;
module_param(uvc_extension_en, int, S_IRUGO);

static struct uvc_descriptor_header** ucamera_streaming_cls = NULL;
static unsigned int stillimg_en = 0;

/* --------------------------------------------------------------------------
 * USB Configuration
 */

int uvc_bind_uvc(struct usb_configuration* c)
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
