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

static struct uvc_descriptor_header ** ucamera_streaming_cls = NULL;
static unsigned int stillimg_en = 0;

int uvc_bind_uvc(struct usb_configuration *c)
{
    return 0;
}
EXPORT_SYMBOL(ucam_bind_uvc);

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
