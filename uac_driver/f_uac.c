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

int audio_bind_config(struct usb_configuration* c)
{
    struct f_audio* audio;
    int status;
    struct usb_ep* ep;

    audio = kzalloc(sizeof(*audio), GFP_KERNEL);
    if (!audio)
        return -ENOMEM;

    audio->card.func.name = "audio";
    audio->card.gadget = c->cdev->gadget;

#ifdef CONFIG_GADGET_UAC1_PLAY
    if (speak_enable) {
        INIT_LIST_HEAD(&audio->play_queue);
        audio->play_frame_list = init_node_list(AUDIO_NODE_NUM, audio_buf_size, NL_NO_BLOCK, NL_NO_BLOCK, NL_KEEPUPDATE);
        if (audo->play_frame_list == NULL) {
            printk("UAC:Init node list Failed\n");
            return -ENOMEM;
        }
    }
#endif
    spin_lock_init(&audio->lock);
    spin_lock_init(&audio->event_lock);

#ifdef CONFIG_GADGET_UAC1_CAP_MIC
    status = gaudio_setup(audio);
    if (status < 0) {
        return status;
    }
#endif /* CONFIG_GADGET_UAC1_CAP_MIC */
    if (audio_strings_def[0].id == 0) {
        status = usb_string_ids_tab(c->cdev, audio_strings_def);
    }
    if (status) {
        printk("UAC:Allocating String ID Failed\n");
        return status;
    }
    uac_iad.iFunction = audio_strings_def[0].id;
    ac_interface_desc.iInterface = audio_strings_def[0].id;

    audio->card.func.strings = uac_strings;
    audio->card.func.bind = f_audio_bind;
    audio->card.func.unbind = f_audio_unbind;
    audio->card.func.set_alt = f_audio_set_alt;
    audio->card.func.setup = f_audio_setup;
    audio->card.func.disable = f_audio_disable;

    control_selector_init(audio);

#ifdef CONFIG_GADGET_UAC1_PLAY
    if (speak_enable) {
        INIT_WORK(&audio->playback_work, f_audio_playback_work);
    }
#endif /* CONFIG_GADGET_UAC1_PLAY */

    status = usb_add_function(c, &audio->card.func);
    if (status) {
        return status;
    }

    return 0;
}

int ucam_bind_uac(struct usb_configuration* c)
{
    return audio_bind_config(c);
}
EXPORT_SYMBOL(ucam_bind_uac);
