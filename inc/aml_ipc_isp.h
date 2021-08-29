#ifndef AML_IPC_ISP_H

#define AML_IPC_ISP_H

#include "aml_ipc_component.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

enum AmlISPChannel {
    AML_ISP_FR,
    AML_ISP_META,
    AML_ISP_DS1,
    AML_ISP_DS2,
    AML_ISP_SDK_MAX
};

enum AmlISPPatternTypes {
    AML_ISP_PATTERN_FLAT_FIELD,
    AML_ISP_PATTERN_HORIZONTAL_GRADIENT,
    AML_ISP_PATTERN_VERTICAL_GRADIENT,
    AML_ISP_PATTERN_VERTICAL_BARS,
    AML_ISP_PATTERN_DISABLE
};

struct AmlIPCISPPriv;
struct AmlIPCISP {
    AML_OBJ_EXTENDS(AmlIPCISP, AmlIPCComponent, AmlIPCComponentClass);
    struct AmlIPCISPPriv* priv;
    char* device;
    int active;
    int per_chan_thread;
    enum AmlISPPatternTypes testpattern;
    int sensor_fps;
};
AML_OBJ_DECLARE_TYPEID(AmlIPCISP, AmlIPCComponent, AmlIPCComponentClass);

#define AML_ISP_WIDTH_MIN (128)
#define AML_ISP_WIDTH_MAX (32768)
#define AML_ISP_HEIGHT_MIN (128)
#define AML_ISP_HEIGHT_MAX (32768)
#define AML_ISP_FPS_MIN (1)
#define AML_ISP_FPS_MAX (240)

#define AML_WB_GAIN_AUTO (-1)
#define AML_WB_GAIN_MIN (0)
#define AML_WB_GAIN_MAX (65535)
#define AML_EXPOSURE_AUTO (-1)
#define AML_EXPOSURE_MIN (1)
#define AML_EXPOSURE_MAX (1000)

#define AML_SHARPNESS_MIN (0)
#define AML_SHARPNESS_MAX (255)
#define AML_BRIGHTNESS_MIN (0)
#define AML_BRIGHTNESS_MAX (255)
#define AML_CONTRAST_MIN (0)
#define AML_CONTRAST_MAC (255)
#define AML_HUE_MIN (0)
#define AML_HUE_MAX (360)
#define AML_SATURATION_MIN (0)
#define AML_SATURATION_MAX (255)
#define AML_SENSOR_FPS_MIN (0)
#define AML_SENSOR_FPS_MAX (120)
#define AML_FPS_MIN (0)
#define AML_FPS_MAX (120)
#define AML_COMPENSATION_MIN (0)
#define AML_COMPENSATION_MAX (255)
#define AML_NR_NONE (-1)
#define AML_NR_MIN (0)
#define AML_NR_MAX (255)

#define AML_PIXFMT_ISP_RAW (AML_PIXFMT_SDK_MAX + 0)

struct AmlIPCISP* aml_ipc_isp_new(const char* dev, int nbuf_fr, int nbuf_ds1, int nbuf_ds2);
AmlStatus aml_ipc_isp_set_capture_format(struct AmlIPCISP* isp, enum AmlISPChannel chan,
    struct AmlIPCVideoFormat* fmt);
AmlStatus aml_ipc_isp_get_capture_format(struct AmlIPCISP* isp, enum AmlISPChannel chan,
    struct AmlIPCVideoFormat* fmt);
AmlStatus aml_ipc_isp_set_picture_params(struct AmlIPCISP* isp, enum AmlISPChannel chan,
    int brightness, int contrast, int saturation, int hue, int sharpness);

AmlStatus aml_ipc_isp_set_wdr(struct AmlIPCISP* isp, bool enabled);
AmlStatus aml_ipc_isp_get_wdr(struct AmlIPCISP* isp, bool* enabled);

AmlStatus aml_ipc_isp_set_WB_gain(struct AmlIPCISP* isp, int crgain, int cbgain);
AmlStatus aml_ipc_isp_get_WB_gain(struct AmlIPCISP* isp, int* crgain, int* cbgain);
AmlStatus aml_ipc_isp_set_bw_mode(struct AmlIPCISP* isp, bool bw);
AmlStatus aml_ipc_isp_get_bw_mode(struct AmlIPCISP* isp, int* bw);
AmlStatus aml_ipc_isp_set_exposure(struct AmlIPCISP* isp, int exposure);
AmlStatus aml_ipc_isp_get_exposure(struct AmlIPCISP* isp, int* exposure);
AmlStatus aml_ipc_isp_set_sharpness(struct AmlIPCISP* isp, int sharpness);
AmlStatus aml_ipc_isp_get_sharpness(struct AmlIPCISP* isp, int* sharpness);
AmlStatus aml_ipc_isp_set_brightness(struct AmlIPCISP* isp, int brightness);
AmlStatus aml_ipc_isp_get_brightness(struct AmlIPCISP* isp, int* brightness);
AmlStatus aml_ipc_isp_set_contrast(struct AmlIPCISP* isp, int contrast);
AmlStatus aml_ipc_isp_get_contrast(struct AmlIPCISP* isp, int* contrast);
AmlStatus aml_ipc_isp_set_hue(struct AmlIPCISP* isp, int hue);
AmlStatus aml_ipc_isp_get_hue(struct AmlIPCISP* isp, int* hue);
AmlStatus aml_ipc_isp_set_saturation(struct AmlIPCISP* isp, int saturation);
AmlStatus aml_ipc_isp_get_saturation(struct AmlIPCISP* isp, int* sjaturation);
AmlStatus aml_ipc_isp_set_hflip(struct AmlIPCISP* isp, bool hflip);
AmlStatus aml_ipc_isp_get_hflip(struct AmlIPCISP* isp, bool* hflip);
AmlStatus aml_ipc_isp_set_vflip(struct AmlIPCISP* isp, bool vflip);
AmlStatus aml_ipc_isp_get_vflip(struct AmlIPCISP* isp, bool* vflip);
AmlStatus aml_ipc_set_sensor_fps(struct AmlIPCISP* isp, int fps);
AmlStatus aml_ipc_get_sensor_fps(struct AmlIPCISP* isp, int* fps);
AmlStatus aml_ipc_isp_set_fps(struct AmlIPCISP* isp, enum AmlISPChannel chan, int fps);
AmlStatus aml_ipc_isp_get_fps(struct AmlIPCISP* isp, enum AmlISPChannel chan, int* fps);
AmlStatus aml_ipc_isp_set_compensation(struct AmlIPCISP* isp, int compensation);
AmlStatus aml_ipc_isp_get_compensation(struct AmlIPCISP* isp, int* compensation);
AmlStatus aml_ipc_isp_set_nr(struct AmlIPCISP* isp, int space_zone, int time_zone);
AmlStatus aml_ipc_isp_get_nr(struct AmlIPCISP* isp, int* space_zone, int* time_zone);
AmlStatus aml_ipc_isp_set_defog(struct AmlIPCISP* isp, int mode, int value);
AmlStatus aml_ipc_isp_get_defog(struct AmlIPCISP* isp, int* mode, int* value);
AmlStatus aml_ipc_isp_set_gain(struct AmlIPCISP* isp, bool bauto, int value);
AmlStatus aml_ipc_isp_get_gain(struct AmlIPCISP* isp, bool* bauto, int* value);
AmlStatus aml_ipc_isp_set_custom_calibration(struct AmlIPCISP* isp, int value);
AmlStatus aml_ipc_isp_get_custom_calibration(struct AmlIPCISP* isp, int* value);
AmlStatus aml_ipc_isp_set_test_pattern(struct AmlIPCISP* isp, enum AmlISPPatternTypes type);
AmlStatus aml_ipc_isp_get_test_pattern(struct AmlIPCISP* isp, enum AmlISPPatternTypes* type);

AmlStatus aml_ipc_isp_enumerate_capture_formats(struct AmlIPCISP* isp, enum AmlISPChannel chan,
    enum AmlPixelFormat pixfmt, int* numres, struct AmlIPCVideoFormat* res);

AmlStatus aml_ipc_isp_set_device(struct AmlIPCISP* isp, char* dev);

AmlStatus aml_ipc_isp_start_channel(struct AmlIPCISP* isp, enum AmlISPChannel chan, int nbuf);
AmlStatus aml_ipc_isp_stop_channel(struct AmlIPCISP* isp, enum AmlISPChannel chan);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !AML_IPC_ISP_H
