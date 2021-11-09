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

static int gaudio_lock(atomic_t* excl)
{
    if (atomic_inc_return(excl) == 1) {
        return 0;
    } else {
        atomic_dec(excl);
        return -1;
    }
}

static void gaudio_unlock(atomic_t* excl)
{
    atomic_dec(excl);
}

static int gaudio_open(struct inode* ip, struct file* fp)
{
    if (!__audio) {
        return -ENODEV;
    }
    if (gaudio_lock(&__audio->open_excl))
        return -EBUSY;
    fp->private_data = __audio;
    return 0;
}

static int gaudio_release(struct inode* ip, struct file* fp)
{
    gaudio_unlock(&((struct f_audio*)fp->private_data)->open_excl);
    return 0;
}

static struct file_operations f_audio_fops = {
    .owner = THIS_MODULE,
    .write = gaudio_write,
    .read = gaudio_read,
    .open = gaudio_open,
    .release = gaudio_release,
    .poll = gaudio_poll,
    .unlocked_ioctl = gaudio_ioctl,
    .mmap = gaudio_mmap,
};

static struct miscdevice gaudio_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "gaudio",
    .fops = &gaudio_fops,
};

static int f_audio_bind(struct usb_configuration* c, struct usb_function* f)
{
    struct usb_composite_dev* cdev = c->cdev;
    struct f_audio* audio = func_to_audio(f);
    int status, i;
    struct usb_ep* ep = NULL;

    usb_function_deactivate(f);
    f_audio_build_desc(audio);

    status = usb_interface_id(c, f);
    if (status < 0)
        goto fail;
    ac_interface_desc.bInterfaceNumber = status;
    uac_iad.bFirstInterface = status;
    uac_iad.bInterfaceCount = 1;

#if defined(CONFIG_GADGET_UAC1_PLAY) && defined(CONFIG_GADGET_UAC1_CAP)
    if (speak_enable) {
        status = usb_interface_id(c, f);
        if (status < 0)
            goto fail;
        spk_as_interface_alt_0_desc.bInterfaceNumber = status;
        spk_as_interface_alt_1_desc.bInterfaceNumber = status;
        ac_header_desc.baInterfaceNr[0] = status;
        play_intf = status;
        uac_iad.bInterfaceCount += 1;

        status = usb_interface_id(c, f);
        if (status < 0)
            goto fail;
        mic_as_interface_alt_0_desc.bInterfaceNumber = status;
        mic_as_interface_alt_1_desc.bInterfaceNumber = status;
        ac_header_desc.baInterfaceNr[1] = status;
        cap_intf = status;
        uac_iad.bInterfaceCount += 1;
    } else {
        status = usb_interface_id(c, f);
        if (status < 0)
            goto fail;
        mic_as_interface_alt_0_desc.bInterfaceNumber = status;
        mic_as_interface_alt_1_desc.bInterfaceNumber = status;
        ac_header_desc.baInterfaceNr[0] = status;
        cap_intf = status;
        uac_iad.bInterfaceCount += 1;
    }
#elif defined(CONFIG_GADGET_UAC1_PLAY)
    status = usb_interface_id(c, f);
    if (status < 0)
        goto fail;
    spk_as_interface_alt_0_desc.bInterfaceNumber = status;
    spk_as_interface_alt_1_desc.bInterfaceNumber = status;
    ac_header_desc.baInterfaceNr[0] = status;
    play_intf = status;
    uac_iad.bInterfaceCount += 1;
#elif defined(CONFIG_GADGET_UAC1_CAP)
    status = usb_interface_id(c, f);
    if (status < 0)
        goto fail;
    mic_as_interface_alt_0_desc.bInterfaceNumber = status;
    mic_as_interface_alt_1_desc.bInterfaceNumber = status;
    ac_header_desc.baInterfaceNr[0] = status;
    cap_intf = status;
    uac_iad.bInterfaceCount += 1;
#else
#error cap or play should be defined
#endif
    status = -ENODEV;

#ifdef CONFIG_GADGET_UAC1_PLAY
    if (speak_enable) {
        ep = usb_ep_autoconfig(cdev->gadget, &as_out_ep_desc);
        if (!ep)
            goto fail;
        audio->out_ep = ep;
        audio->out_ep->desc = &as_out_ep_desc;
        ep->driver_data = cdev;
    }
#endif /* CONFIG_GADGET_UAC1_PLAY */

#if CONFIG_GADGET_UAC1_CAP
    ep = usb_ep_autoconfig(cdev->gadget, &as_in_ep_desc);
    if (!ep)
        goto fail;
    audio->in_ep = ep;
    audio->in_ep->desc = &as_in_ep_desc;
    ep->driver_data = cdev;
#endif /* CONFIG_GADGET_UAC1_CAP */

    status = -ENOMEM;

    status = usb_assign_descriptors(f, f_audio_desc, f_audio_desc, NULL);
    if (status)
        goto fail;
    usb_function_activate(f);

    atomic_set(&audio->interface_conn, 1);

    init_waitqueue_head(&audio->event_queue);
    INIT_LIST_HEAD(&audio->event_list);

    __audio = audio;
#ifdef CONFIG_GADGET_UAC1_CAP_USER
    init_waitqueue_head(&audio->read_wq);
    init_waitqueue_head(&audio->write_wq);
    init_waitqueue_head(&audio->online_wq);
    atomic_set(&audio->open_excl, 0);
    atomic_set(&audio->read_excl, 0);
    atomic_set(&audio->write_excl, 0);
    INIT_LIST_HEAD(&audio->idle_reqs);
    INIT_LIST_HEAD(&audio->audio_data_list);
    spin_lock_init(&audio->audio_list_lock);

    status = misc_register(&audio_device);
    if (status)
        goto fail;

    for (i = 0, status = 0; i < IN_EP_REQ_COUNT && status == 0; i++) {
        struct usb_request* req = audio_request_new(audio->in_ep, IN_EP_MAX_PACKET_SIZE);
        if (req) {
            req->context = audio;
            req->complete = audio_data_complete;
            audio_req_put(audio, &audio->idle_reqs, req);
        } else
            status = -ENOMEM;
    }
    audio->cur_seq = NULL;
    if (status == -ENOMEM) {
        goto fail;
    }
#endif /* CONFIG_GADGET_UAC1_CAP_USER */

#ifdef CONFIG_GADGET_UAC1_CAP_MIC
    spin_lock_init(&audio->cb_list_lock);
    audio->capture__thread = kthread_create(audio_capture_thread, audio, "usb-audio-capture");
    if (IS_ERR(audio->capture_thread))
        return PTR_ERR(audio->capture_thread);

    INIT_LIST_HEAD(&audio->cb_list_free);
    INIT_LIST_HEAD(&audio->cb_list_queued);
#endif /* CONFIG_GADGET_UAC1_CAP_MIC */

    return 0;

fail:
    if (ep)
        ep->driver_data = NULL;

    return status;
}

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

int uac_set_descriptor(unsigned long arg)
{
    int ret = 0;
    struct audio_param* a_param = NULL;

    a_param = kmalloc(sizeof(struct audio_param), GFP_KERNEL);
    ret = copy_from_user(a_param, (void __user*)arg, sizeof(struct audio_param));
    if (ret) {
        printk("error(%s %d):failed to copy audio param!\n", __func__, __LINE__);
        return -EIO;
    }
    uac_set_audio_param(a_param);
    return 0;
}
EXPORT_SYMBOL(uac_set_descriptor);
