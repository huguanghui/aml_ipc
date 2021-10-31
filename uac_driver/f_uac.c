#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/atomic.h>
#include <linux/kthread.h>
#include <linux/sched/rt.h>

#include "u_uac1.h"

#define IN_EP_REQ_COUNT (MIC_CH * 25)
#define IN_EP_MAX_PACKET_SIZE 384

#define AUDIO_NODE_NUM 40

static int IN_SAMPLE_RATE = 16000;
static int IN_EP_ACTUAL_SIZE = 16000 * 1 * 2 / 1000;

static int OUT_SAMPLE_RATE = 48000;

#define OUT_EP_MAX_PACKET_SIZE 0x200
static int req_buf_size = OUT_EP_MAX_PACKET_SIZE;
module_param(req_buf_size, int, S_IRUGO);
MODULE_PARAM_DESC(req_buf_size, "ISO OUT endpoint request buffer size");

static int req_count = 256;
module_param(req_count, int, S_IRUGO);
MODULE_PARAM_DESC(req_count, "ISO OUT endpoint request count");

static int audio_buf_size = 48000 * 2 * 20 / 1000;
module_param(audio_buf_size, int, S_IRUGO);
MODULE_PARAM_DESC(audio_buf_size, "Audio buffer size");

static int generic_get_cmd(struct usb_audio_control* con, u8 cmd, int value);
static int generic_set_cmd(struct usb_audio_control* con, u8 cmd);

static struct f_audio* __audio = NULL;

static struct usb_descriptor_header** f_audio_desc = NULL;
static int speak_enable = 0;
static int F_AUDIO_NUM_INTERFACES = 1;
static int UAC_DT_AC_HEADER_LENDTH = 0;
static int UAC_DT_TOTAL_LENGTH = 0;

struct usb_string audio_strings_def[3] = { 0 };

static struct usb_gadget_strings uac_string_table = {
    .language = 0x0409,
    .strings = audio_strings_def,
};

static struct usb_gadget_string* uac_strings[] = {
    &uac_string_table,
    NULL,
};
