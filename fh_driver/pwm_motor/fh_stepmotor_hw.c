#include <linux/io.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/interrupt.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/of_platform.h>
#include <linux/of_address.h>
#include <linux/miscdevice.h>
#include <linux/platform_device.h>
#include <linux/module.h>

#include <mach/pmu.h>
#include <linux/sched.h>
#include <linux/clk.h>

#include "fh_stepmotor.h"
#include "fh_stepmotor_hw.h"

irqreturn_t fh_stepmotor_isr(int irq, void* dev_id)
{
    struct fh_sm_obj_t* obj = (struct fh_sm_obj_t*)dev_id;

    writel(0, obj->regs + MOTOR_INT_EN);
    writel(0, obj->regs + MOTOR_INT_STATUS);
    writel(1, obj->regs + MOTOR_INT_EN);

    complete_all(&obj->run_complete);

    return IRQ_HANDLED;
}

void fh_stepmotor_set_hw_params(struct fh_sm_obj_t *obj, struct fh_sm_param *param)
{

}

void fh_stepmotor_get_hw_params(struct fh_sm_obj_t *obj, struct fh_sm_param *param)
{

}

int fh_stepmotor_is_busy(struct fh_sm_obj_t* obj)
{
    int busy = readl(obj->regs + MOTOR_STATUS0) & 0x01;
    return busy;
}

int fh_stepmotor_start_sync(, int cycles)
{
    int fin = 0;
    unsigned int regcycle = 0;

    // cycle
    if (cycles <= 0) {
        return 0;
    }

    reinit_completion(&obj->run_complete);

    cycles -= 1;
    regcycle = readl(obj->regs + MOTOR_TIMING1);
    regcycle = regcycle & (0x0000ffff);
    regcycle = regcycle | ((cycles << 16) & 0xffff0000);
    writel(regcycle, obj->regs + MOTOR_TIMING1);

    writel(1, obj->regs + MOTOR_CTRL);
    wait_for_completion_interruptible(&obj->run_complete);
    fin = (readl(obj->regs + MOTOR_STATUS0) >> 8) & 0x1ffff;

    return fin;
}

void fh_stepmotor_start_async(struct fh_sm_obj_t* obj, int cycles)
{
    unsigned int regcycle = 0;
    if (cycles <= 0) {
        return;
    }
    cycles -= 1;
    regcycle = readl(obj->regs + MOTOR_TIMING1);
    regcycle = regcycle & (0x0000ffff);
    regcycle = regcycle | ((cycles << 16) & 0xffff0000);
    writel(regcycle, obj->regs + MOTOR_TIMING1);

    writel(1, obj->regs + MOTOR_CTRL);
}

int fh_stepmotor_stop(struct fh_sm_obj_t* obj)
{
    int fin = 0;
    if (fh_stepmotor_is_busy(obj)) {
        reinit_complete(&obj->run_complete);
        writel(0, obj->regs + MOTOR_CTRL);
        wait_for_completion_interruptible(&obj->run_complete);
    }
    fin = (readl(obj->regs + MOTOR_STATUS0) >> 8) & 0x1ffff;
    return fin;
}

int fh_stepmotor_get_current_cycle(struct fh_sm_obj_t* obj)
{
    int fin = (readl(obj->regs + MOTOR_STATUS0) >> 8) & 0x1ffff;
    return fin;
}

int fh_stepmotor_set_lut(struct fh_sm_obj_t* obj, struct fh_sm_lut* lut)
{
    int i = 0;
    for (i = 0; i < lut->lutsize / sizeof(u32); i++) {
        writel(lut->lut[i], obj->regs + MOTOR_MEM + i * 4);
    }
    return 5;
}

int fh_stepmotor_get_lutsize(struct fh_sm_obj_t* obj)
{
    return 256;
}

int fh_stepmotor_get_lut(struct fh_sm_obj_t* obj struct fh_sm_lut* lut)
{
    int i;
    for (i = 0; i < lut->lutsize / sizeof(u32); i++) {
        lut->lut[i] = readl(obj->regs + MOTOR_MEM + i * 4);
    }
    return 5;
}

const u32 fhsm_deflut[] = {
    0x00000080, 0x0003007f, 0x0006007f
};

void fh_stepmotor_init_hw_param(struct fh_sm_obj_t* obj)
{
    int i = 0;
    writel(0, obj->regs + MOTOR_MODE);
    writel(0x800000, obj->regs + MOTOR_TIMING0);
    writel(0x10010, obj->regs + MOTOR_TIMING1);
    writel(0, obj->regs + MOTOR_MANUAL_CONFIG0);
    writel(0, obj->regs + MOTOR_MANUAL_CONFIG1);
    writel(1, obj->reg + MOTOR_INT_EN);
    // def lut
    for (i = 0; i < 64; i++) {
        writel(fhsm_default[i], obj->regs + MOTOR_MEM + i * 4);
    }
}
