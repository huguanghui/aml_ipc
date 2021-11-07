#ifndef __U_AUDIO_H
#define __U_AUDIO_H

struct audio_param {
    int mic_sample_rate;
    int spk_sample_rate;
    short mic_volume;
    short spk_volume;
    int speak_en;
};

struct gaudio {
    struct usb_function func;
    struct usb_gadget *gadget;

    struct gaudio_snd_dev control;
    struct gaudio_snd_dev playback;
    struct gaudio_snd_dev capture;
};

struct f_audio {
    struct gaudio card;

    struct usb_ep* out_ep;
    struct usb_ep* in_ep;

    spinlock_t lock;
    struct f_audio_buf* copy_buf;
    struct work_struct playback_work;
    node_list_t* play_frame_list;

    wait_queue_head_t event_queue;
    struct list_head event_list;
    spinlock_t event_lock;

#ifdef CONFIG_GADGET_UAC1_PLAY
    struct list_head play_queue;
    struct list_head out_reqs_list;
#endif

    int in_online;
    int out_online;

#ifdef CONFIG_GADGET_UAC1_CAP_MIC
    struct task_struct* capture_thread;
    struct list_head cb_list_free;
    struct list_head cb_list_queued;
    spinlock_t cb_list_lock;
#endif

#ifdef CONFIG_GADGET_UAC1_CAP_USER
    spinlock_t audio_list_lock;
    struct list_head idle_reqs;
    struct list_head audio_data_list;
    wait_queue_head_t online_wq;
    wait_queue_head_t write_wq;
    atomic_t open_excl;
    atomic_t write_excl;
    atomic_t read_excl;
#endif //

    atomic_t interface_conn;
    volatile u8 mic_disconn;

    wait_queue_head_t read_wq;
    struct usb_request* cur_req;

    struct list_head cs;
    u8 set_cmd;
    struct usb_audio_control* set_con;
};

#endif // !__U_AUDIO_H
