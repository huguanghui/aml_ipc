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

static struct usb_string ucamera_strings[] = {
    [USB_GADGET_PRODUCT_IDX].s = camera_product_label,
    [USB_GADGET_MANUFACTURER_IDX].s = camera_vendor_label,
    [USB_GADGET_SERIAL_IDX].s = ucamera_serial_number,
    {}
};

static struct usb_gadget_strings ucamera_string_table = {
    .language = 0x0409,
    .strings = ucamera_strings,
};

static struct usb_gadget_strings *ucamera_device_strings[] = {
    &ucamera_string_table,
    NULL,
};

struct struct usb_device_descriptor ucamera_device_descriptor = {
    .bLength = USB_DT_DEVICE_SIZE,
    .bDescriptorType = USB_DT_DEVICE,
    .bcdUSB = cpu_to_le16(0x0200),
    .bDeviceClass = USB_CLASS_MISC,
    .bDeviceSubClass = 0x02,
    .bDeviceProtocol = 0x01,
    .bMaxPacketSize0 = 0,
    // .idVendor = 0x18D1,
    // .idProduct = 0xD00D,
    // .bcdDevice = 0x0100,
    .iManufacturer = 0,
    .iProduct = 0,
    .iSerialNumber = 0,
    .bNumConfigurations = 0,
};

static int ucamera_config_bind(struct usb_configuration* c)
{
    int ret;

    ret = ucamera_bind_uvc(c);
    if (ret) {
        printk("ucamera bind uvc func failed\n");
    }

    if (1 == audio_start) {
        ret = ucam_bind_uac(c);
        if (ret) {
            printk("ucamera bind uac func failed\n");
        }
    }

    if (1 == hid_start) {
        ret = ucamera_bind_hid(c);
        if (ret) {
            printk("ucamera bind hid func failed\n");
        }
        if (emergency) {
            printk("emergency start up \n");
            return 0;
        }
    }

    if (1 == adb_start) {
        ret = ucamera_bind_adb(c);
        if (ret) {
            printk("ucamera bind adb func failed\n");
        }
    }

    return ret;
}

static struct usb_configuration ucamera_config_driver = {
    .label = ucamera_config_label,
    .bConfigurationValue = 1,
    .iConfiguration = 0,
    .bmAttributes = USB_CONFIG_ATT_ONE | USB_CONFIG_ATT_SELFPOWER,
    .MaxPower = 0xC8,
};

static int ucamera_unbind(struct usb_composite_dev* cdev)
{
#ifdef INCLUDE_HID_INTERFACE
    if (hid_start != 0) {
        hid_cleanup(cdev);
    }
#endif /* INCLUDE_HID_INTERFACE */

#ifdef INCLUDE_ADB_INTERFACE
    if (1 == adb_start) {
        adb_cleanup();
    }
#endif

    return 0;
}

static int ucamera_bind(struct usb_composite_dev* cdev)
{
    int ret;

    ret = usb_string_ids_tab(cdev, ucamera_strings);
    if (ret < 0) {
        goto error;
    }
    ucamera_device_descriptor.iManufacturer = ucamera_strings[USB_GADGET_MANUFACTURER_IDX].id;
    ucamera_device_descriptor.iProduct = ucamera_strings[USB_GADGET_PRODUCT_IDX].id;
    ucamera_device_descriptor.iSerialNumber = ucamera_strings[USB_GADGET_SERIAL_IDX].id;
#ifdef INCLUDE_HID_INTERFACE
    if (hid_start != 0) {
        ret = hid_init(cdev);
        if (ret) {
            printk("%s: Failed to init hid", __func__);
        }
    }
#endif

#ifdef INCLUDE_ADB_INTERFACE
    if (1 == adb_start) {
        ret = adb_init(cdev);
        if (ret) {
            printk("%s: Failed to init adb", __func__);
        }
    }
#endif /* INCLUDE_ADB_INTERFACE */

    if ((ret = usb_add_config(cdev, &ucamera_config_driver, ucamera_config_bind)) < 0) {
        goto error;
    }

    usb_composite_overwrite_options(cdev, &overwrite);
    return 0;

error:
    ucamera_unbind(cdev);
    return ret;
}

static __refdata struct usb_composite_driver ucamera_driver = {
    .name = "g_camera",
    .dev = &ucamera_device_descriptor,
    .strings = ucamera_device_strings,
    .max_speed = USB_SPEED_SUPER,
    .bind = ucamera_bind,
    .unbind = ucamera_unbind,
};

static int ucamera_driver_init(void)
{
    int status;

#ifdef INCLUDE_HID_INTERFACE
    if (hid_start != 0) {
        status = platform_device_register(&my_hid);
        if (status < 0) {
            printk("%s hid register failed\n", __func__);
            platform_device_unregister(&my_hid);
            return status;
        }

        status = platform_driver_probe(&hidg_plat_driver, hidg_plat_driver_probe);
        if (status < 0) {
            return status;
        }
    }
#endif /* INCLUDE_HID_INTERFACE */

    status = usb_composite_probe(&ucamera_driver);
    if (status < 0) {
        return status;
    }
    return status;
}

static void ucamera_driver_cleanup(void)
{
#ifdef INCLUDE_HID_INTERFACE
    if (hid_start != 0) {
        platform_driver_unregister(&hidg_plat_driver);
        platform_device_unregister(&my_hid);
    }
#endif
    usb_composite_unregister(&ucamera_driver);
}

int ucamera_product_msg_config(unsigned long arg)
{
    int ret = 0;
    struct Ucamera_Product_Cfg* pcfg;

    pcfg = kmalloc(sizeof(struct Ucamera_Product_Cfg), GFP_KERNEL);
    ret = copy_from_user(pcfg, (void __user*)arg, sizeof(struct Ucamera_Product_Cfg));
    if (ret) {
        printk("error(%s, %d): failed to copy_from user!\n", __func__, __LINE__);
        ret = -EIO;
        return ret;
    }
    if (pcfg->Pid) {
        UCAMERA_PRODUCT_ID = pcfg->Pid;
    }
    if (pcfg->Vid) {
        UCAMERA_VENDOR_ID = pcfg->Vid;
    }
    if (pcfg->version) {
        UCAMERA_DEVICE_BCD = pcfg->version;
    }

    memcpy(ucamera_vendor_label, pcfg->manufacturer, 64);
    memcpy(ucamera_product_label, pcfg->product, 64);
    memcpy(ucamera_serial_label, pcfg->serial, 64);

    memcpy(video_device_name, pcfg->video_name, 64);
    memcpy(audio_device_name, pcfg->audio_name, 64);

    ucamera_device_descriptor.idVendor = cpu_to_le16(UCAMERA_VENDOR_ID);
    ucamera_device_descriptor.idProduct = cpu_to_le16(UCAMERA_PRODUCT_ID);
    ucamera_device_descriptor.bcdDevice = cpu_to_le16(UCAMERA_DEVICE_BCD);
    uvc_name_set(video_device_name);
    uac_name_set(audio_device_name);

    return ret;
}

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
    case UCAMERA_IOCTL_ADB_ENABLE: {
        adb_start = 1;
        break;
    }
    case UCAMERA_IOCTL_HID_EMER_ENABLE: {
        if (0 == ucamera_inited && 0 == emergency) {
            struct Ucamera_Product_Cfg* pcfg;
            pcfg = kmalloc(sizeof(struct Ucamera_Product_Cfg), GFP_KERNEL);
            ucamera_device_descriptor.idVendor = cpu_to_le16(UCAMERA_VENDOR_ID + 1);
            ucamera_device_descriptor.idProduct = cpu_to_le16(UCAMERA_PRODUCT_ID + 1);
            ucamera_device_descriptor.bcdDevice = cpu_to_le16(UCAMERA_DEVICE_BCD);
            ret = ucamera_driver_init();
            if (0 == ret) {
                ucamera_inited = 1;
                emergency = 1;
            } else {
                printk("error(%s, %d): ucamera_init\n", __func__, __LINE__);
            }
        }
        break;
    }
    case UCAMERA_IOCTL_STILL_IMG_CAP: {
        uvc_sti_init();
        break;
    }
    default:
        printk("invalid command: 0x%08x\n", cmd);
        ret = -EINVAL;
    }

    return ret;
}

static int ucamera_open(struct inode* inode, struct file* filp)
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
