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
#endif /* DEBUG */

#include "ucam.h"

static struct usb_configuration ucamera_config_driver = {
    .label = ucamera_config_label,
    .bConfigurationValue = 1,
    .iConfiguration = 0,
    .bmAttributes = USB_CONFIG_ATT_ONE | USB_CONFIG_ATT_SELFPOWER,
    .MaxPower = 0xC8,
};

static int ucamera_unbind(struct usb_composite_dev *cdev)
{
#ifdef INCLUDE_HID_INTERFACE
    if (hid_start != 0) {
        hid_cleanup(cdev);
    }
#endifINCLUDE_HID_INTERFACE

#ifdef INCLUDE_ADB_INTERFACE
    if (1 == adb_start) {
        adb_cleanup();
    }
#endifINCLUDE_ADB_INTERFACE
    return 0;
}

static int ucamera_bind(struct usb_composite_dev *cdev)
{
    int ret = 0;

    ret = usb_string_ids_tab(cdev, ucamera_strings);
    if (ret < 0)
        goto error;

    // ucamera_device_descriptor

#ifdef INCLUDE_HID_INTERFACE
    if (hid_start != 0) {
        ret = hid_init(cdev);
        if (ret) {
            printk("%s: Failed to init hid", __func__);
        }
    }
#endif

#ifdef INCLUDE_ADB_INTERFACE
    if (1 == adb_setup()) {
        if(ret) {
            printk("%s: Failed to init adb", __func__);
        }
    }
#endif


    if ((ret = usb_add_config(cdev, &ucamera_config_driver, ucamera_config_bind)) < 0) {
        goto error;
    }
    usb_composite_overwrite_options(cdev, &coverwrite);

    return 0;

error:
    ucamer_unbind(cdev);

    return ret;
}

static __refdata struct usb_composite_driver ucamera_driver = {
    .name = "g_camera",
    .dev = &ucamera_device_descriptor,
    .strings = ucamer_device_strings,
    .max_speed = USB_SPEED_SUPER,
    .bind = ucamera_bind,
    .unbind = ucamera_unbind,
};

static long ucamera_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    return 0;
}

static int ucamera_open(struct inode* inode, struct file* filp)
{
    return 0;
}

static int ucamera_release(struct inode* inode, struct file* filp)
{
    return 0;
}

static ssize_t ucamera_read(struct file* filp, char __user* buf, size_t count, loff_t* f_ops)
{
    int ret = 0;
    ret = copy_to_user(buf, &ucamera_inited, sizeof(ucamera_inited));
    if (ret != 0) {
        printk("ERROR:copy to user if failed\n");
    }
    return ret;
}

static struct file_operations ucamera_fops = {
    .owner = THIS_MODULE,
    .read = ucamera_read,
    .unlocked_ioctl = ucamera_ioctl,
    .open = ucamera_open,
    .release = ucamera_release,
};

static struct miscdevice ucamera_fops = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "ucamera",
    .fops = &ucamera_fops,
};

static int __init ucamera_init(void)
{
    int ret = 0;
    ret = misc_register(&ucamera_misc);
    return ret;
}

static void __exit ucamera_exit(void)
{
    misc_deregister(&ucamera_misc);
}

module_init(ucamera_init);
module_exit(ucamera_exit);

MODULE_AUTHOR("Laurent Pinchart");
MODULE_DESCRIPTION("Ucamera video Gadget");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1.0");
