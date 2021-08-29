#include "aml_ipc_define.h"
#include <pthread.h>

#ifndef AML_IPC_COMPONENT_H

#define AML_IPC_COMPONENT_H

#ifdef __cplusplus
extern "C" {
#endif

// SDK reserve channel ID: 0xFFFF0000 ~ 0xFFFFFFFF
// AML_DEFAULT_CHANNEL can be used to specify the default input/output channel, it is usually
// the first input/output channel
#define AML_DEFAULT_CHANNEL 0xFFFF0000

struct AmlIPCComponentPriv;
struct AmlIPCComponent {
    AML_OBJ_EXTENDS(AmlIPCComponet, AmlObject, AmlIPCComponentClass);
    struct AmlIPCComponentPriv* priv;
    pthread_mutex_t lock;
    pthread_cond_t cond;
    TAILQ_ENTRY(AmlIPCComponent)
    next; // link to next component in the same pipeline
    uint32_t flags;
};

struct AmlIPCComponentClass {
    struct AmlObjectClass parent_class;
    AmlStatus (*get_frame)(struct AmlIPCComponent* self, int ch, int timeout_us, struct AmlIPCFrame** out);
    AmlStatus (*handle_frame)(struct AmlIPCComponent* self, int ch, struct AmlIPCFrame* frame);
    AmlStatus (*start)(struct AmlIPCComponent* self);
    AmlStatus (*stop)(struct AmlIPCComponent* self);
};
AML_OBJ_DECLARE_TYPEID(AmlIPCComponent, AmlObject, AmlIPCComponentClass);
#define AML_IPC_COMPONENT(obj) ((struct AmlIPCComponent*)(obj))

/**
* @brief send frame to this component (intput channel) or downstream components (output channel)
*
* @Param comp
* @Param ch, channel ID
*   for input channel, the frame will send to the component itself (comp)
*   for output channel, the frame will be sent to downstream components bound to the channel
*   AML_DEFAULT_CHANNEL can be used to specify the default input channel
* @Param frame, frame data to send, callee will take owner ship, caller shall not touch the frame
*
* @Returns, return the result
*/
AmlStatus aml_ipc_send_frame(struct AmlIPCComponent* comp, int ch, struct AmlIPCFrame* frame);

/**
* @brief get frame from a component or from upstream component, check aml_ipc_get_next_frame for
*    the difference
*
* @Param comp
* @Param ch, channel ID
*   for input channel, it will get frame from upstream component bound to the channel
*   for output channel, it will get frame from this componet (comp)
*    AML_DEFAULT_CHANNEL can be used to specify the default output channel
* @Param timeout_us, timeout for the frame
*    0 : do not wait for the frame
*    -1: wait the frame for ever (will not return until frame is ready, or component stop)
*    >0: timeout in microseconds (1/1000000 sec)
* @Param out
*
* @Returns, return the result
*/
AmlStatus aml_ipc_get_frame(struct AmlIPCComponent* comp, int ch, int timeout_us, struct AmlIPCFrame** out);

/**
* @brief hook the frames on a channel, this will callback for every frame flow on the channel
*
* @Param comp
* @Param ch, channel ID
*    AML_DEFAULT_CHANNEL can be used to specify the default output channel
* @Param param, user provided pointer which will pas to the callback
* @Param callback, user provided callback, it will be called when the frame flows on the channel,
*     the callback takes two parameter: frame pointer and user provided pointer, the frame
*     pointer is only valid in the callback, if you need to store the pointer and use it otherwise,
*     use aml_obj_addref, and don't forget to call aml_obj_release when done.
*     callback is called in the streaming thread, it shall return as soon as possible, else it may
*     block the streaming thread, which may cause frame dropping.
* if the callback return AML_STATUS_HOOK_CONTINUE, the frame will continue to handle by other hooks
* and the bound components, returning other values will stop further processing on the frame.
*
* @Returns
*/
AmlStatus aml_ipc_add_frame_hook(struct AmlIPCComponent* comp, int ch, void* param, AmlStatus (*callback)(struct AmlIPCFrame*, void*));
AmlStatus aml_ipc_remove_frame_hook(struct AmlIPCComponent* comp, int ch, void* param, AmlStatus (*callback)(struct AmlIPCFrame*, void*));

/**
* @brief watch the frame on a channel, this is similar to frame hook, except that watch callback is
* called earlier than hook, and watch does not return any value, watch callback shall do as few
* things as possible
*
* @Param comp
* @Param ch
* @Param param
* @Param watch
*
* @Returns
*/
AmlStatus aml_ipc_add_frame_watch(struct AmlIPCComponent* comp, int ch, void* param, void (*watch)(struct AmlIPCFrame*, void*));
AmlStatus aml_ipc_remove_frame_watch(struct AmlIPCComponent* comp, int ch, void* param, void (*watch)(struct AmlIPCFrame*, void*));

/**
* @brief bind two channel and let data flow between the components (via the bind channels)
*  An output channel can bind to more than one input channels, in such cause all input channels
*  will receive the data
*
* @Param provider, data provider component
* @Param output, output channel ID of the provider
*   AML_DEFAULT_CHANNEL can be used to specify the default output channel
* @Param receiver, data receiver component
* @Param input, input channel ID of the receiver
*   AML_DEFAULT_CHANNEL can be used to specify the default input channel
*
* @Returns
*/
AmlStatus aml_ipc_bind(struct AmlIPCComponent* provider, int output, struct AmlIPCComponent* receiver, int input);

/**
* @brief unbind the receiver channel from it's provider
*
* @Param receiver, data reveive component
* @Param input, input channel ID of the receiver
*   AML_DEFAULT_CHANNEL can be used to specify the default input channel
*
* @Returns
*/
AmlStatus aml_ipc_unbind(struct AmlIPCComponent* receiver, int input);

/**
* @brief start producing data
*
* @Param comp, usually an output component (isp,ain,queue)
*
* @Returns
*/
AmlStatus aml_ipc_start(struct AmlIPCComponent* comp);
AmlStatus aml_ipc_stop(struct AmlIPCComponent* comp);

/**
* @brief get next frame from a component's output channel
*  This API will wait for the data from the component's output channel, unlike aml_ipc_get_frame,
*  which can only get frame data from inactive components such as ISP,Ain,queue,vbpool...etc, this
*  API, can get frame data from any components, once there's data flow from their output channel
*
* @Param comp
* @Param ch, channel ID, must be output channel
*   AML_DEFAULT_CHANNEL can be used to specify the default output channel
* @Param timeout_us, timeout for the frame
*   0 : do not wait for the frame, DO NOT use this value
*   -1: wait the frame for ever (will not return until frame is ready, or component stop)
*   >0: timeout in microseconds (1/1000000 sec)
* @Param out, output the frame address, caller shall free the frame when done with it
*
* @Returns, return the result
*/
AmlStatus aml_ipc_get_next_frame(struct AmlIPCComponent* comp, int ch, int timeout_us, struct AmlIPCFrame** out);

#ifdef __cplusplus
}
#endif

#endif // !AML_IPC_COMPONENT_
