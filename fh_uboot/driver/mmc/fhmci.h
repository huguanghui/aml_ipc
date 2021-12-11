
#ifndef _FHMCI_H_
#define _FHMCI_H_

#include <mmc.h>

struct fhmci_dma_des {
    unsigned long idmac_des_ctrl;
    unsigned long idmac_des_buf_size;
    unsigned long idmac_des_buf_addr;
    unsigned long idmac_des_next_addr;
};

struct fhmci_host {
    struct mmc mmc;
    unsigned long base;
    unsigned int card_status;
    int cmd_id;
    struct mmc_cmd *cmd;
    struct fhmci_dma_des *dma_des;
};

void fh_mci_sys_init(void);

#endif // !_FHMCI_H_
