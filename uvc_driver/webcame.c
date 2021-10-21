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
