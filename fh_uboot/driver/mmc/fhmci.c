#include <mmc.h>
#include <fhmci.h>

static unsigned int fh_mci_sys_card_detect(struct fhmci_host* host)
{
    unsigned int card_status;

    card_status = readl(host->base + MCI_CDETECT);

    return card_status & FHMCI_CARD0;
}

static int fh_mci_initialize(int dev_num)
{
    struct mmc* mmc = NULL;
    static struct fhmci_host* host;

    FHMCI_DEBUG_FUN("Function Call");

    host = malloc(sizeof(struct fhmci_host));

    if (!host) {
        return -ENOMEM;
    }

    memset(host, 0, sizeof(struct fhmci_host));

    mmc = &host->mmc;
    mmc->priv = host;

    if (dev_num == 0) {
        host->base = SDC0_REG_BASE;
        host->dma_des = fh_dma_des;
    } else {
        host->base = SDC1_REG_BASE;
        host->dma_des = fh_dma_des1;
    }

    host->card_status = fh_mci_sys_card_detect(host);
    sprintf(mmc->name, DRIVER_NAME);
    mmc->send_cmd = fh_mci_request;
    mmc->set_ios = fh_mci_set_ios;
    mmc->init = fh_mci_init;

    mmc->host_caps = MMC_MODE_HS | MMC_MODE_HS_52MHz
        | MMC_MODE_4BIT | MMC_MODE_8BIT;

    mmc->voltages = MMC_VDD_32_33 | MMC_VDD_33_34;

    mmc->f_min = MMC_CCLK_MIN;
    mmc->f_max = MMC_CCLK_MAX;

    mmc_register(mmc);

    return 0;
}

int board_mmc_init(int dev_num)
{
    FHMCI_DEBUG_FUN("Function Call");
    return fh_mci_initialize(dev_num);
}

int mmc_flash_init(int dev)
{
    struct mmc* mmc;
    int err = 0;

    FHMCI_DEBUG_FUN("Function Call");

    mmc = find_mmc_device(dev);
    if (!mmc) {
        printf("No MCI%d found!!!\n", dev);
        return -1;
    }

    if (((struct fhmci_host*)(mmc->priv))->card_status) {
        printf("MMC FLASH INIT: No card on slot%d\n", dev);
        return -1;
    }

    err = mmc_init(mmc);
    if (err) {
        printf("No EMMC device found!!!\n");
        return err;
    }

    FHMCI_DEBUG_INFO("OCR register: %08x", mmc->ocr);
    FHMCI_DEBUG_INFO("CID register: %08x %08x %08x %08x",
        mmc->cid[0], mmc->cid[1], mmc->cid[2], mmc->cid[3]);
    FHMCI_DEBUG_INFO("CSD register: %08x %08x %08x %08x",
        mmc->csd[0], mmc->csd[1], mmc->csd[2], mmc->csd[3]);
    FHMCI_DEBUG_INFO("RCA register: %08x", mmc->rca);

    print_mmc_info(mmc);

    if (!IS_SD(mmc)) {
        check_ext_csd(mmc);
    }

    return 0;
}
