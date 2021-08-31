#include <linux/file.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>

#include "ion.h"

union ion_ioctl_arg {
    struct ion_allocation_data allocation;
    struct ion_heap_query query;
};

static int validate_ioctl_arg(unsigned int cmd, union ion_ioctl_arg* arg)
{
    switch (cmd) {
    case ION_IOC_HEAP_QUERY:
        if (arg->query.reserved0 || arg->query.reserved1 || arg->query.reserved2) {
            return -EINVAL;
        }
        break;
    default:
        break;
    }
    return 0;
}

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

    switch (cmd) {
    case ION_IOC_ALLOC: {
        int fd;
        fd = ion_alloc(data.allocation.len, data.allocation.heap_id_mask, data.allocation.flags);
        if (fd < 0) {
            return fd;
        }
        data.allocation.fd = fd;
        break;
    }
    case ION_IOC_HEAP_QUERY: {
        ret = ion_query_heaps(&data.query);
        break;
    }
    default:
        return -ENOTTY;
    }

    if (dir & _IOC_READ) {
        if (copy_to_user((void __user*)arg, &data, _IOC_SIZE(cmd))) {
            return -EFAULT;
        }
    }

    return ret;
}
