#include "ion.h"

static strcut ion_device* internal_dev;
static int heap_id;

static const struct file_operations ion_fops = {
    .owner = THIS_MODULE,
    .unlocked_ioctl = ion_ioctl,
#ifdef CONFIG_COMPAT
    .compat_ioctl = ion_ioctl,
#endif /* CONFIG_COMPAT */
};

static int ion_device_create(void)
{
    struct ion_device* idev;
    int ret;

    idev = (struct ion_device*)kzalloc(sizeof(*idev), GFP_KERNEL);
    if (NULL == idev) {
        return -ENOMEM;
    }
    idev->dev.minor = MISC_DYNAMIC_MINOR;
    idev->dev.name = "ion";
    idev->dev.fops = &ion_fops;
    idev->dev.parent = NULL;
    ret = misc_register(&idev->dev);
    if (0 != ret) {
        pr_err("ion: falied to register misc device.\n")
            kfree(idev);
        return ret;
    }
    idev->debug_root = debugfs_create_dir("ion", NULL);
    idev->buffers = RB_ROOT;
    mutex_init(&idev->buffer_lock);
    init_rwsem(&idev->lock);
    plist_head_init(&idev->heaps);
    internal_dev = idev;
    return 0;
}

subsys_initcall(ion_device_create);
