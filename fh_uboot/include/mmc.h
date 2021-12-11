#ifndef _MMC_H_
#define _MMC_H_

#include <linux/list.h>

struct mmc_cid {
    unsigned long psn;
    unsigned short oid;
    unsigned char mid;
    unsigned char prv;
    unsigned char mdt;
    char pnm[7];
};

struct mmc_csd {
    u8 csd_structure:2,
       spec_vers:4;
       rsvd1:2;
    u8 taac;
    u8 nsac;
    u8 tran_speed;
    u16 ccc:12,
        read_bl_len:4;
    u64 read_bl_partial:1,
        write_blk_misalign:1,
        read_blk_misalign:1,
        dsr_imp:1,
        rsvd2:2,
        c_size:12,
        vdd_r_curr_min:3,
        vdd_r_curr_max:3,
        vdd_w_curr_min:3,
        vdd_w_curr_max:3,
        c_size_mult:3,
        sector_size:5,
        erase_grp_size:5,
        wp_grp_size:5,
        wp_grp_enable:1,
        default_ecc:2,
        r2w_factor:3,
        write_bl_len:4,
        write_bl_partial:1,
        rsvd3:5;
    u8 file_format_grp:1,
        copy:1,
        perm_write_protect:1,
        tmp_write_protect:1,
        file_format:2,
        ecc:2;
    u8 crc:7;
    u8 one:1;
};

struct mmc_cmd {
    ushort cmdidx;
    uint resp_type;
    uint cmdarg;
    uint response[4];
    uint flags;
};

struct mmc_data {
    union {
        char *dest;
        const char *src;
    };
    uint flags;
    uint blocks;
    uint blocksize;
};

struct mmc_ext_csd {
    u8 rev;
    u8 erase_group_def;
    u8 sec_feature_support;
    u8 rel_sectors;
    u8 rel_param;
    u8 part_config;
    unsigned int part_time;
    unsigned int sa_timeout;
    unsigned int hs_max_dtr;
    unsigned int sectors;
    unsigned int card_type;
    unsigned int hc_erase_size;
    unsigned int hc_erase_timeout;
    unsigned int sec_trim_mult;
    unsigned int sec_erase_mult;
    unsigned int trim_timeout;
    unsigned int enhanced_area_en;
    unsigned long long enhanced_area_offset;
    unsigned int enhanced_area_size;
    unsigned int boot_size;
    u8 raw_partition_support;
    u8 raw_erased_mem_count;
    u8 raw_ext_csd_structure;
    u8 raw_card_type;
    u8 raw_s_a_timeout;
    u8 raw_hc_erase_gap_size;
    u8 raw_erase_timeout_mult;
    u8 raw_hc_erase_grp_size;
    u8 raw_sec_trim_mult;
    u8 raw_sec_erase_mult;
    u8 raw_sec_feature_support;
    u8 raw_trim_mult;
    u8 raw_sectors[4];
};

struct mmc {
    struct list_head link;
    char name[32];
    void *priv;
    uint voltages;
    uint version;
    uint f_min;
    uint f_max;
    int high_capacity;
    uint bus_width;
    uint clock;
    uint card_caps;
    uint host_caps;
    uint ocr;
    uint scr[2];
    uint csd[4];
    uint cid[4];
    ushort rca;
    struct mmc_ext_csd ext_csd;
    uint tran_speed;
    uint read_bl_len;
    uint write_bl_len;
    u64 capacity;
    block_dev_desc_t block_dev;
    int (*send_cmd)(struct mmc *mmc, struct mmc_cmd *cmd, struct mmc_data *data);
    void *set_ios)(struct mmc *mmc);
    int (*init)(struct mmc *mmc);
};

int mmc_send_cmd(struct mmc *mmc, struct mmc_cmd *cmd, struct mmc_data *data);
int mmc_register(struct mmc *mmc);
int mmc_initialize(bd_t *bis);
int mmc_init(struct mmc *mmc);
int mmc_read(struct mmc *mmc, u64 src, uchar *dst, int size);
ulong mmc_bwrite(int dev_num, ulong start, lbaint_t blkcnt, const void*src);

struct mmc *find_mmc_device(int dev_num);
void print_mmc_devices(char separator);

#endif // !_MMC_H_
