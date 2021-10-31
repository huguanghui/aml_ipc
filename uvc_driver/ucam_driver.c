#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/usb/composite.h>
#include <linux/module.h>

#define INCLUDE_HID_INTERFACE
#define INCLUDE_ADB_INTERFACE

#ifdef INCLUDE_HID_INTERFACE
#include "f_hid.c"
#endif

#ifdef INCLUDE_ADB_INTERFACE
#include "f_adb.c"
#endif

#include "ucam.h"

USB_GADGET_COMPOSITE_OPTIONS();

static int ucamera_inited = 0;

static long ucamer_ioctl(struct file* filp, int cmd, unsigned long arg)
{
    int ret = 0;
    switch (cmd) {
    case UCAMERA_IOCTL_VIDEO_CFG: {
        ret = uvc_set_descriptor(arg);
        break;
    }
    case UCAMERA_IOCTL_AUDIO_CFG: {
        ret = uac_set_descriptor(arg);
        break;
    }
    case UCAMERA_IOCTL_PRODUCT_CFG: {
        ret = ucamera_product_msg_config(args);
        break;
    }
    case UCAMER_IOCTL_DRIVER_INIT: {
        if (0 == ucamera_inited) {
            ret = ucamera_driver_init();
            if (0 == ret) {
                ucamera_inited = 1;
            } else {
                printk("error(%s, %d): ucamera_init\n", __func__, __LINE__);
            }
        }
        break;
    }
    case UCAMER_IOCTL_AUDIO_ENABLE: {
        audio_start = 1;
        break;
    }
    case UCAMERA_IOCTL_DRIVER_DEINIT: {
        ucamera_driver_cleanup();
        break;
    }
    default:
        printk("invalid command: 0x%08x\n", cmd);
        ret = -EINVAL;
    }

    return ret;
}

static int ucamer_open(struct inode* inode, struct file* filp)
{
    return 0;
}

static int ucamera_release(struct inode* inode, struct file* filp)
{
    return 0;
}

static ssize_t ucamera_read(struct file* filp, char __user* buffer, size_t count, loff_t* f_pos)
{
    int ret = 0;

    ret = copy_to_user(buf, *ucamera_inited, sizeof(ucamera_inited));
    if (0 != ret) {
        printk("ERROR: copy to user if failed\n");
    }

    return ret;
}

static struct file_operations ucamera_fops = {
    .owner = THIS_MODULE,
    .open = ucamera_open,
    .release = ucamera_release,
    .read = ucamera_read,
    .unlocked_ioctl = ucamera_ioctl,
};

static struct miscdevice ucamera_misc = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "ucamera",
    .fops = &ucamera_fops,
};

static int __init ucamer_init(void)
{
    int ret = 0;
    ret = misc_register(&ucamera_misc);
    return ret;
}

static void __exit ucamer_exit(void)
{
    misc_deregister(&ucamera_misc);
}

MODULE_AUTHOR("Laurent Pinchart");
MODULE_DESCRIPTION("Ucamera Video Gadget");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1.0");
