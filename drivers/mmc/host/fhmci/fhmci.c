#include <linux/moduleparam.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/mm.h>
#include <linux/interrupt.h>
#include <linux/dma-mapping.h>
#include <linux/scatterlist.h>
#include <linux/regulator/consumer.h>
#include <linux/mmc/host.h>
#include <linux/mmc/mmc.h>
#include <linux/mmc/sd.h>
#include <linux/mmc/card.h>
#include <linux/slab.h>

#include <linux/ioport.h>
#include <linux/device.h>
#include <linux/gpio.h>

#include <linux/delay.h>
#include <linux/dma-mapping.h>

extern int trace_level;
#define FHMCI_TRACE_LEVEL 5

#define FHMCI_TRACE_FMT KERN_INFO

#define fhmci_trace(level, msg...)                                 \
    do {                                                           \
        if ((level) >= trace_level) {                              \
            printk(FHMCI_TRACE_FMT "%s:%d: ", __func__, __LINE__); \
            printk(msg);                                           \
            printk("\n");                                          \
        }                                                          \
    } while (0)

#define fhmci_assert(cond)                          \
    do {                                            \
        if (!(cond)) {                              \
            printk(KERN_ERR "Assert:fhmci:%s:%d\n", \
                __func__,                           \
                __LINE__);                          \
            BUG();                                  \
        }                                           \
    } while (0)

#define fhmci_error(s...)                                     \
    do {                                                      \
        printk(KERN_ERR "fhmci:%s:%d: ", __func__, __LINE__); \
        printk(s);                                            \
        printk("\n");                                         \
    } while (0)

static const struct of_device_id fh_sdio_of_match[] = {
    {
        .compatible = "fh,fh-sdc",
    },
    {},
}

MODULE_DEVICE_TABLE(of, fh_sdio_of_match);

static struct platform_driver fh_sdio_driver = {
    .driver = {
        .name = "fh_mci",
        .of_match_table = fh_sdio_of_match,
    },
    .probe = fh_mci_probe,
    .remove = fh_mci_remove,
    .suspend = fh_mci_suspend,
    .resume = fh_mci_resume,
};

static ssize_t fh_mci_rescan_control(struct class* cls, struct class_attribute* attr, const char* _buf, size_t _count)
{
    int cmd = 0;
    int err = 0;
    char name[32];
    struct device_node* np;
    struct platform_device* pdev = NULL;
    struct mmc_host* mmc;
    struct device* dev;

    err = kstrtoint(_buf, 10, &cmd);
    if (err) {
        return _count;
    }
    sprintf(name, "sdc%d", cmd);
    np = of_kind_node_by_name(NULL, name);
    if (np && !IS_ERR(np))
        pdev = of_find_device_by_node(np);
    else {
        sprintf(name, "fh_mci.%d", cmd);
        dev = bus_find_device_by_name(&platform_bus_type, NULL, name);
        if (dev && !IS_ERR(dev))
            pdev = to_platform_device(dev);
        else
            pr_err("%s:%d, dev not found\n",
            __func__, __LINE__);
    }
    if (!pdev) {
        pr_err("%s:%d,device %s not found\n",
        __func__, __LINE__, np->name);
        return -ENODEV;
    }
    mmc = platform_get_drvdata(pdev);
    if (mmc) {
        mmc->rescan_count = 0;
    }
    else {
        pr_err("%s:%d,mmc not found\n",
        __func__, __LINE__);
    }

    return _count;
}

static struct class* fhmci_rescan_class;

static CLASS_ATTR(mmc_rescan, 0644, NULL, fh_mci_rescan_contorl);

static void fh_mci_rescan_init(void)
{
    int err = 0;

    fhmci_rescan_class = class_create(THIS_MODULE, "fhmci");
    err = class_create_file(fhmci_rescan_class, &class_attr_mmc_rescan);
    if (err) {
        fhmci_error("fhmci_rescan_class: create class file failed!");
    }
}

static void fh_mci_rescan_exit(void)
{
    class_remove_file(fhmci_rescan_class, &class_attr_mmc_rescan);
    class_destroy(fhmci_rescan_class);
}

static int __init fh_mci_init(void)
{
    int ret;

    fhmci_trace(2, "mci init begin");
    fh_mci_rescan_init();

    ret = platform_driver_register(&fh_sdio_driver);
    if (ret) {
        fhmci_error("Platform friver register is failed!");
    }

    return ret;
}

static void __exit fh_mci_exit(void)
{
    fh_mci_rescan_exit();
    platform_driver_unregister(&fh_sdio_driver);
}

module_init(fh_mci_init);
module_exit(fh_mci_exit);

MODULE_LICENSE("GPL")
MODULE_AUTHOR("HGH")
MODULE_DESCRIPTION("Fullhan SDIO device driver")
