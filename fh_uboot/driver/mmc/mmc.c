#include <mmc.h>

static struct list_head mmc_devices;
static int cur_dev_num = -1;

int mmc_initialize(bd_t* bis)
{
    INIT_LIST_HEAD(&mmc_devices);

#ifdef CONFIG_MMC_SDC0
    cur_dev_num = 0;
    if (board_mmc_init(0) < 0)
        cpu_mmc_init(bis);
#endif
    print_mmc_devices(',');

    return 0;
}

int mmc_register(struct mmc* mmc)
{
    mmc->block_dev.if_type = IF_TYPE_MMC;
    mmc->block_dev.dev = cur_dev_num++;
    mmc->block_dev.removable = 1;
    mmc->block_dev.block_read = mmc_bread;
    mmc->block_dev.block_write = mmc_bwrite;

    INIT_LIST_HEAD(&mmc->link);
    list_add_tail(&mmc->link, &mmc_devices);

    return 0;
}

int mmc_startup(struct mmc* mmc)
{
    int err;
    uint mult, freq;
    u64 cmult, csize;
    struct mmc_cmd cmd;

    cmd.cmdidx = MMC_CMD_ALL_SEND_CID;
    cmd.resp_type = MMC_RSP_R2;
    cmd.cmdarg = 0;
    cmd.flags = 0;

    err = mmc_send_cmd(mmc, &cmd, NULL);

    if (err) {
        return err;
    }

    memcpy(mmc->cid, cmd.response, 16);

    cmd.cmdidx = SD_CMD_SNED_RELATIVE_ADDR;
    cmd.cmdarg = mmc->rca << 16;
    cmd.resp_type = MMC_RSP_R6;
    cmd.flags = 0;

    err = mmc_send_cmd(mmc, &cmd, NULL);

    if (err) {
        return err;
    }

    if (IS_SD(mmc)) {
        mmc->rca = (cmd.response[0] >> 16) & 0xffff;
    }

    cmd.cmdidx = MMC_CMD_SEND_CSD;
    cmd.resp_type = MMC_RSP_R2;
    cmd.cmdarg = mmc->rca << 16;
    cmd.flags = 0;

    err = mmc_send_cmd(mmc, &cmd, NULL);

    if (err) {
        return err;
    }

    mmc->csd[0] = cmd.response[0];
    mmc->csd[1] = cmd.response[1];
    mmc->csd[2] = cmd.response[2];
    mmc->csd[3] = cmd.response[3];

    if (mmc->version == MMC_VERSION_UNKNOWN) {
        int version = (cmd.response[0] >> 26) & 0xf;

        switch (version) {
        case 0:
            mmc->version = MMC_VERSION_1_2;
            break;
        case 1:
            mmc->version = MMC_VERSION_1_4;
            break;
        case 2:
            mmc->version = MMC_VERSION_2_2;
            break;
        case 3:
            mmc->version = MMC_VERSION_3;
            break;
        case 4:
            mmc->version = MMC_VERSION_4;
            break;
        default:
            mmc->version = MMC_VERSION_1_2;
            break
        }
    }

    freq = fbase[(cmd.response[0] & 0x7)];
    mult = multipliers[((cmd.response[0] >> 3) & 0xf)];

    mmc->tran_speed = freq * mult;

    mmc->read_bl_len = 1 << ((cmd.response[1] >> 16) & 0xf);

    if (IS_SD(mmc)) {
        mmc->write_bl_len = mmc->read_bl_len;
    } else {
        mmc->write_bl_len = 1 << ((cmd.response[3] >> 22) & 0xf);
    }

    if (mmc->high_capacity) {
        csize = (mmc->csd[1] & 0x3f) << 16
            | (mmc->csd[2] & 0xffff0000) >> 16;
        cmult = 8;
    } else {
        csize = (mmc->csd[1] & 0x3ff) << 2
            | (mmc->csd[2] & 0xc0000000) >> 30;
        cmult = (mmc->csd[2] & 0x00038000) >> 15;
    }

    mmc->capacity = (csize + 1) << (cmult + 2);
    mmc->capacity *= mmc->read_bl_len;

    if (mmc->read_bl_len > 512)
        mmc->read_bl_len = 512;

    if (mmc->write_bl_len > 512)
        mmc->write_bl_len = 512;

    cmd.cmdidx = MMC_CMD_SELECT_CARD;
    cmd.resp_type = MMC_RSP_R1b;
    cmd.cmdarg = mmc->rca << 16;
    cmd.flags = 0;
    err = mmc_send_cmd(mmc, &cmd, NULL);

    if (err) {
        return err;
    }

    if (IS_SD(mmc)) {
        err = sd_change_freq(mmc);
    } else {
        err = mmc_change_freq(mmc);
    }

    if (err) {
        return err;
    }

    mmc->card_caps &= mmc->host_caps;

    if (IS_SD(mmc)) {
        if (mmc->card_caps & MMC_MODE_4BIT) {
            cmd.cmdidx = MMC_CMD_APP_CMD;
            cmd.resp_type = MMC_RSP_R1;
            cmd.cmdarg = mmc->rca << 16;
            cmd.flags = 0;

            err = mmc_send_cmd(mmc, &cmd, NULL);
            if (err) {
                return err;
            }

            cmd.cmdidx = SD_CMD_APP_SET_BUS_WIDTH;
            cmd.resp_type = MMC_RSP_R1;
            cmd.cmdarg = 2;
            cmd.flags = 0;
            err = mmc_send_cmd(mmc, &cmd, NULL);
            if (err) {
                return err;
            }

            mmc_set_bus_width(mmc, 4);
        }

        if (mmc->card_caps & MMC_MODE_HS) {
            mmc_set_clock(mmc, 50000000);
        } else {
            mmc_set_clock(mmc, 25000000);
        }
    } else {
        if (mmc->card_caps & MMC_MODE_4BIT) {
            err = mmc_switch(mmc, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_BUS_WIDTH,
                EXT_CSD_BUS_WIDTH_4);

            if (err) {
                return err;
            }
            mmc_set_bus_width(mmc, 4);
        } else if (mmc->card_caps & MMC_MODE_8BIT) {
            err = mmc_switch(mmc, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_BUS_WIDTH,
                EXT_CSD_BUS_WIDTH_8);

            if (err) {
                return err;
            }

            mmc_set_bus_width(mmc, 8);
        }

        if (mmc->card_caps & MMC_MODE_HS) {
            if (mmc->card_caps & MMC_MODE_HS_52MHz)
                mmc_set_clock(mmc, 52000000);
            else
                mmc_set_clock(mmc, 26000000);
        } else {
            mmc_set_clock(mmc, 20000000);
        }
    }

    if (!IS_SD(mmc)) {
        char ext_csd[512];
        err = mmc_send_ext_csd(mmc, ext_csd);
        if (err) {
            printf("read est_csd error!");
            return 0;
        }
        err = mmc_read_ext_csd(mmc, (u8*)ext_csd);
        if (err) {
            printf("read ext_csd error!");
            return 0;
        }
    }

    mmc->block_dev.lun = 0;
    mmc->block_dev.type = 0;
    mmc->block_dev.blksz = mmc->read_bl_len;
    mmc->block_dev.lba = lldiv(mmc->capacity, mmc->read_bl_len);
    sprintf(mmc->block_dev.vendor, "Man %06x Snr %08x", mmc->cid[0] >> 8,
        (mmc->cid[2] << 8) | (mmc->cid[3] >> 24));
    sprintf(mmc->block_dev.product, "%c%c%c%c%c", mmc->cid[0] & 0xff,
        (mmc->cid[1] >> 24), (mmc->cid[1] >> 16) & 0xff);
    sprintf(mmc->block_dev.revision, "%d.%d", mmc->cid[2] >> 28,
        (mmc->cid[2] >> 24) & 0xf);
    init_part(&mmc->block_dev);

    return 0;
}

int mmc_init(struct mmc* mmc)
{
    int err;

    err = mmc->init(mmc);

    if (err)
        return err;

    mmc_set_bus_width(mmc, 1);
    mmc_set_clock(mmc, 1);

    err = mmc_go_idle(mmc);

    if (err)
        return err;

    err = mmc_send_if_cond(mmc);

    err = sd_send_op_cond(mmc);

    if (err == TIMEOUT) {
        err = mmc_send_op_cond(mmc);

        if (err) {
            printf("Card did not respond to voltage select!\n");
            return UNUSABLE_ERR;
        }
    }

    return mmc_startup(mmc);
}
