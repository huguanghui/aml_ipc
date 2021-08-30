#include <linux/file.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>

#include "ion.h"

union ion_ioctl_arg {
    struct ion_allocation_data allocation;
    struct ion_heap_query query;
};

static unsigned int ion_ioctl_dir(unsigned int cmd)
{
    switch (cmd) {
    default:
        return _IOC_DIR(cmd);
    }
}

long ion_ioctl(struct file* filp, unsigned int cmd, unsigned long arg)
{
    int ret = 0;
    unsigned int dir;
    union ion_ioctl_arg data;

    dir = ion_ioctl_dir(cmd);
    if (_IOC_SIZE(cmd) > sizeof(data)) {
        return -EINVAL;
    }

    if (copy_from_user(&data, (void __user*)arg, _IOC_SIZE(cmd))) {
        return -EFAULT;
    }

    ret = validate_ioctl_arg(cmd, &data);
    if (0 != ret) {
        pr_warn_once("%s: ioctl validate failed\n", __func__);
        return ret;
    }
    if (0 == (dir & _IOC_WRITE)) {
        memset(&data, 0, sizeof(data));
    }

    return ret;
}
