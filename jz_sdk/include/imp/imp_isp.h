#ifndef __IMP_IVS_H__
#define __IMP_IVS_H__

#include <stdbool.h>
#include <stdint.h>

#include "imp_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef enum {
    TX_SENSOR_CONTROL_INTERFACE_I2C = 1,
    TX_SENSOR_CONTROL_INTERFACE_SPI,
} IMPSensorControlBusType;

typedef struct {
    char type[20];
    int addr;
    int i2c_adapter_id;
} IMPI2CInfo;

typedef struct {
    char modalias[32];
    int bus_num;
} IMPSPIInfo;

typedef struct {
    char name[32];
    IMPSensorControlBusType cbus_type;
    union {
        IMPI2CInfo i2c;
        IMPSPIInfo spi;
    };
    unsigned short rst_gpio;
    unsigned short pwdn_gpio;
    unsigned short power_gpio;
} IMPSensorInfo;

int IMP_ISP_Open(void);

int IMP_ISP_Close(void);

int IMP_ISP_AddSensor(IMPSensorInfo* pinfo);

int IMP_ISP_DelSensor(IMPSensorInfo* pinfo);

int IMP_ISP_EnableSensor(void);

int IMP_ISP_DisableSensor(void);

int IMP_ISP_SetSensorRegister(uint32_t reg, uint32_t value);

int IMP_ISP_GetSensorRegister(uint32_t reg, uint32_t* value);

typedef enum {
    IMPISP_TUNING_OPS_MODE_DISABLE,
    IMPISP_TUNING_OPS_MODE_ENABLE,
    IMPISP_TUNING_OPS_MODE_BUTT,
} IMPISPTuningOpsMode;

typedef enum {
    IMPISP_TUNING_OPS_TYPE_AUTO,
    IMPISP_TUNING_OPS_TYPE_MANUAL,
    IMPISP_TUNING_OPS_TYPE_BUTT,
} IMPISPTuningOpsType;

typedef struct {
    unsigned int zone[15][15];
} IMPISPZone;

int IMP_ISP_EnableTuning(void);

int IMP_ISP_DisableTuning(void);

int IMP_ISP_Tuning_SetSensorFPS(uint32_t fps_num, uint32_t fps_den);

int IMP_ISP_Tuning_GetSensorFPS(uint32_t *fps_num, uint32_t *fps_den);

typedef enum {
    IMPISP_ANTIFLICKER_DISABLE,
    IMPISP_ANTIFLICKER_50HZ,
    IMPISP_ANTIFLICKER_60HZ,
    IMPISP_ANTIFLICKER_BUTT,
} IMPISPAntiflickerAttr;

int IMP_ISP_Tuning_SetAntiFlickerAttr(IMPISPAntiflickerAttr attr);

int IMP_ISP_Tuning_GetAntiFlickerAttr(IMPISPAntiflickerAttr *pattr);

int IMP_ISP_Tuning_SetBrightness(unsigned char bright);

int IMP_ISP_Tuning_GetBrightness(unsigned char *pbright);

int IMP_ISP_Tuning_SetContrast(unsigned char contrast);

int IMP_ISP_Tuning_GetContrast(unsigned char *pcontrast);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif // !__IMP_IVS_H__
