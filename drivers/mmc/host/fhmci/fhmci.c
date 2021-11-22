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
#include <linux/mapping.h>

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

static int sd_init(struct fhmci_host* host, unsigned int drv_degree, unsigned int sam_degree)
{
    u32 slot_id = host->id;
    struct clk* sdc_clk;
    struct clk* sdc_clk_drv;
    struct clk* sdc_clk_sample;
    char name[32];

    sprintf(name, "sdc%d_clk", slot_id);
    sdc_clk = clk_get(NULL, name);
    clk_prepare_enable(sdk_clk);

    fh_pmu_sdc_reset(slot_id);

    clk_set_rate(sdc_clk, 25000000);
    sprintf(name, "sdc%d_clk_sample", slot_id);
    sdc_clk_sample = clk_get(NULL, name);
    clk_set_phase(sdc_clk_sample, sam_degree);

    sprintf(name, "sdc%d_clk_drv", slot_id);
    sdc_clk_drv = clk_get(NULL, name);
    clk_set_phase(sdc_clk_drv, drv_degree);

    return 0;
}

static int fh_mci_probe(struct platform_device* pdev)
{
    struct mmc_host* mmc;
    struct fhmci_host* host = NULL;
    int ret = 0, irq;
    u32 id = 0xffffffff;
    u32 drv_degree = 0;
    u32 sam_degree = 0;
#ifdef CONFIG_USE_OF
    struct device_node* np = pdev->dev.of_node;
    u32 bus_width = 0;
    u32 wp_flag = 0xffffffff;
    u32 cd_flag = 0xffffffff;
    u32 scan_flag = 0xffffffff;
#else
    struct resource* regs;
#endif /* CONFIG_USE_OF */

    fhmci_trace(2, "begin");
    fhmci_assert(pdev);

    mmc = mmc_alloc_host(sizeof(struct fhmci_host), &pdev->dev);
    if (!mmc) {
        fhmci_error("no mem for hi mci host controller!\n");
        ret = -ENOMEM;
        goto out;
    }
    host = mmc_priv(mmc);
    host->card_status = 0xffffffff;
    host->bus_hz = 50000000;
    mmc->ops = &fh_mci_ops;
    mmc->rescan_disable = FALSE;
    mmc->f_min = DIV_ROUND_UP(host->bus_hz, 510);
    mmc->f_max = host->bus_hz;

    mmc->caps = MMC_CAP_SD_HIGHSPEED
        | MMC_CAP_MMC_HIGHSPEED
        | MMC_CAP_NEEDS_POLL
        | MMC_CAP_SDIO_IRQ;

    mmc->caps2 |= MMC_CAP2_NO_PRESCAN_POWERUP;
    mmc->caps2 |= MMC_CAP2_FULL_PWR_CYCLE;

#ifdef CONFIG_USE_OF
    of_property_read_u32(np, "buswidth", &bus_width);
    of_property_read_u32(np, "id", &id);
    of_property_read_u32(np, "drv-degree", &drv_degree);
    of_property_read_u32(np, "sam-degree", &sam_degree);
    of_property_read_u32(np, "cd-fixed", &cd_flag);
    of_property_read_u32(np, "wp-fixed", &wp_flag);
    of_property_read_u32(np, "scan-mux", &scan_mux)

        mmc->rescan_max_num
        = scan_mux;
    if (bus_width == 4) {
        mmc->caps |= MMC_CAP_4_BIT_DATA;
    }

    if (cd_flag == 1) {
        host->get_cd = fh_mci_sys_card_detect_fixed;
    } else if (cd_flag == 0) {
        host->get_cd = fh_mci_sys_card_detect;
    } else {
        pr_err("fh_mci_probe cd-fixed value error\n");
    }

    if (wp_flag == 1) {
        host->get_ro = fh_mci_sys_read_only_fixed;
    } else if (wp_flag == 0) {
        host->get_ro = fh_mci_ctrl_card_readonly;
    } else {
        pr_err("fh_mci_probe wp-fixed value error\n");
    }

    host->base = of_iomap(np, 0);
    if (!host->base) {
        fhmci_error("no mem for fhmci base!\n");
        ret = -ENOMEM;
        goto out;
    }

    irq = irq_of_parse_and_map(np, 0);
    if (irq < 0) {
        printk(KERN_ERR "no IRQ defined!\n");
        goto out;
    }
#else

#endif /* CONFIG_USE_OF */

    mmc->max_blk_count = 2048;
    mmc->max_segs = 64;
    mmc->max_seg_size = mmc->max_blk_size * mmc->max_blk_count;
    mmc->max_req_size = mmc->max_blk_size * mmc->max_blk_count;

    mmc->ocr_avail = MMC_VDD_32_33 | MMC_VDD_33_34;

    host->dma_vaddr = dma_alloc_coherent(&pdev->dev, PAGE_SIZE,
        &host->dma_paddr, GFP_KERNEL);
    if (!host->dma_vaddr) {
        fhmci_error("no mem for fhmci dma!\n");
        ret = -ENOMEM;
        goto out;
    }

    host->mmc = mmc;
    host->id = id;

    sd_init(host, drv_degree, sam_degree);

    host->fifo_depth = (fhmci_readl(host->base + MCI_FIFOTH) & 0xfff0000) >> 16;

    fhmci_writel(SD_POWER_ON, host->base + MCI_PWREN);

    spin_lock_init(&host->lock);
    platform_set_drvdata(pdev, mmc);

    init_waitqueue_head(&host->intr_wait);

    host->irq = irq;
    snprintf(host->isr_name, sizeof(host->isr_name), "sdio-%d", id);
    ret = request_irq(irq, hisd_irq, 0, host->isr_name, host);
    if (ret) {
        printk(KERN_ERR "request_irq error :%d!\n", ret);
        goto out;
    }

    mmc_add_host(mmc);
    return 0;
out:
    if (host) {
        if (host->base)
            iounmap(host->base);

        if (host->dma_vaddr)
            dma_free_coherent(&pdev->dev, PAGE_SIZE,
                host->dma_vaddr, host->dma_paddr);
    }
    if (mmc) {
        mmc_free_host(mmc);
    }

    return ret;
}

#ifdef CONFIG_PM
static int fh_mci_suspend(struct platform_device* dev, pm_message_t state)
{
    struct mmc_host* mmc = platform_get_drvdata(dev);
    struct fhmci_host* host;
    int ret = 0;

    fhmci_trace(2, "begin");
    fhmci_assert(dev);

    if (mmc) {
        host = mmc_priv(mmc);
    }

    fhmci_trace(2, "end");

    return ret;
}

static int fh_mci_resume(struct platform_device* dev)
{
    struct mmc_host* mmc = platform_get_drvdata(dev);
    struct fhmci_host* host;
    int ret = 0;

    fhmci_trace(2, "begin");
    fhmci_assert(dev);

    if (mmc) {
        host = mmc_priv(mmc);
        fh_mci_init_card(host);
    }

    fhmci_trace(2, "end");

    return ret;
}
#else
#define fh_mci_suspend NULL
#define fh_mci_resume NULL
#endif

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
    } else {
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
