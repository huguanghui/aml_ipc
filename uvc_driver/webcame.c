#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include "video.h"

#include "f_uvc.h"

#include "uvc_queue.c"
#include "uvc_video.c"
#include "uvc_v4l2.c"
#include "f_uvc.c"

int af_en = 0;
module_param(af_en, int, S_IRUGO);
int uvc_extension_en = 0;
module_param(uvc_extension_en, int, S_IRUGO);

static struct uvc_descriptor_header** ucamera_streaming_cls = NULL;
static unsigned int stillimg_en = 0;

/* --------------------------------------------------------------------------
 * Device Descriptor
 */
DECLARE_UVC_HEADER_DESCRIPTOR(1);

static const struct UVC_HEADER_DESCRIPTOR(1) uvc_control_header = {
    .bLength = UVC_DT_HEADER_SIZE(1),
    .bDescriptorType = USB_DT_CS_INTERFACE,
    .bDescriptorSubType = UVC_VC_HEADER,
    .bcdUVC = cpu_to_le16(0x0100),
    .wTotalLength = 0,
    .dwClockFrequency = cpu_to_le32(48000000),
    .bInCollection = 0,
    .baInterfaceNr[0] = { 0 },
};

static struct uvc_format_uncompressed uvc_format_nv12 = {
    .bLength = UVC_DT_FORMAT_UNCOMPRESSED_SIZE,
    .bDescriptorType = USB_DT_CS_INTERFACE,
    .bDescriptorSubType = UVC_VS_FORMAT_UNCOMPRESSED,
    .bFormatIndex = 1,
    .bNumFrameDescriptors = 2,
    .guidFormat = { 'N',  'V',  '1',  '2', 0x00, 0x00, 0x10, 0x00,
                    0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 },
    .bBitsPerPixel = 12,
    .bDefaultFrameIndex = 1,
    .bAspectRatioX = 0,
    .bAspectRatioY = 0,
    .bmInterfaceFlags = 0,
    .bCopyProtect = 0,
};

typedef struct uvc_image {
    __u16 wWidth;
    __u16 wHeight;
} uvc_image_t;

struct uvc_still_image_frame {
    __u8 bLength;
    __u8 bDescriptorType;
    __u8 bDescriptorSubType;
    __u8 bEndpointAddress;
    __u8 bNumImageSizePatterns;
    uvc_image_t img[1];
    __u8 bNumCompressionPattern;
} __attribute__((__packed__));

#define UVC_DT_STILL_IMAGE_FRAME_SIZE(n) (10+(4*n)-4)

static const struct uvc_still_image_frame uvc_still_image_frame_base = {
    .bLength = UVC_DT_STILL_IMAGE_FRAME_SIZE(1),
    .bDescriptorType = USB_DT_CS_INTERFACE,
    .bDescriptorSubType = UVC_VS_STILL_IMAGE_FRAME,
    .bEndpointAddress = 0x00,
    .bNumImageSizePatterns = 1,
    .img[0].wWidth = cpu_to_le16(1920),
    .img[0].wHeight = cpu_to_le16(1080),
    .bNumCompressionPattern = 0,
};

static struct uvc_format_mjpeg uvc_format_mjpeg = {
    .bLength = UVC_DT_FORMAT_MJPEG_SIZE,
    .bDescriptorType = USB_DT_CS_INTERFACE,
    .bDescriptorSubType = UVC_VS_FORMAT_MJPEG,
    .bFormatIndex = 2,
    .bNumFrameDescriptors = 4,
    .bmFlags = 0,
    .bDefaultFrameIndex = 1,
    .bAspectRatioX = 0,
    .bAspectRatioY = 0,
    .bmInterfaceFlags = 0,
    .bCopyProtect = 0,
}

static struct uvc_format_framebased uvc_format_h264 = {
    .bLength = UVC_DT_FORMAT_FRAMEBASED_SIZE,
    .bDescriptorType = USB_DT_CS_INTERFACE,
    .bDescriptorSubType = UVC_VS_FORMAT_FRAME_BASED,
    .bFormatIndex = 3,
    .bNumFrameDescriptors = 4,
    .guidFormat = { 'H', '2', '6', '4', 0x00, 0x00, 0x10, 0x00,
        0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 },
    .bBitsPerPixel = 16,
    .bDefaultFrameIndex = 1,
    .bAspectRatioX = 0,
    .bAspectRatioY = 0,
    .bmInterfaceFlags = 0,
    .bCopyProtect = 0,
    .bVariableSize = 1,
};

DECLARE_UVC_FRAME_FRAMEBASED(1);

static const struct UVC_FRAME_FRAMEBASED(1) uvc_frame_h264_360p = {
    .bLength = UVC_DT_FRAME_FRAMEBASED_SIZE(1),
    .bDescriptorType = USB_DT_CS_INTERFACE,
    .bDescriptorSubType = UVC_VS_FRAME_FRAMEBASED,
    .bFrameIndex = 3,
    .bmCapabilities = 0,
    .wWidth = cpu_to_le16(640),
    .wHeight = cpu_to_le16(360),
    .dwMinBitRate = cpu_to_le32(7372800),
    .dwMaxBitRate = cpu_to_le32(7372800),
    .dwDefaultFrameInterval = cpu_to_le32(333333),
    .bFrameIntervalType = 1,
    .dwFrameInterval[0] = cpu_to_le32(333333),
};

static const struct UVC_FRAME_FRAMEBASED(1) uvc_frame_h264_720p = {
    .bLength = UVC_DT_FRAME_FRAMEBASED_SIZE(1),
    .bDescriptorType = USB_DT_CS_INTERFACE,
    .bDescriptorSubType = UVC_VS_FRAME_FRAMEBASED,
    .bFrameIndex = 4,
    .bmCapabilities = 0,
    .wWidth = cpu_to_le16(1280),
    .wHeight = cpu_to_le16(720),
    .dwMinBitRate = cpu_to_le32(221184000),
    .dwMaxBitRate = cpu_to_le32(368640000),
    .dwDefaultFrameInterval = cpu_to_le32(333333),
    .bFrameIntervalType = 1,
    .dwFrameInterval[0] = cpu_to_le32(333333),
};

static const struct UVC_FRAME_FRAMEBASED(1) uvc_frame_h264_960p = {
    .bLength = UVC_DT_FRAME_FRAMEBASED_SIZE(1),
    .bDescriptorType = USB_DT_CS_INTERFACE,
    .bDescriptorSubType = UVC_VS_FRAME_FRAMEBASED,
    .bFrameIndex = 2,
    .bmCapabilities = 0,
    .wWidth = cpu_to_le16(1280),
    .wHeight = cpu_to_le16(960),
    .dwMinBitRate = cpu_to_le32(196608000),
    .dwMaxBitRate = cpu_to_le32(589824000),
    .dwDefaultFrameInterval = cpu_to_le32(333333),
    .bFrameIntervalType = 1,
    .dwFrameInterval[0] = cpu_to_le32(333333),
};

static const struct UVC_FRAME_FRAMEBASED(1) uvc_frame_h264_1080p = {
    .bLength = UVC_DT_FRAME_FRAMEBASED_SIZE(1),
    .bDescriptorType = USB_DT_CS_INTERFACE,
    .bDescriptorSubType = UVC_VS_FRAME_FRAMEBASED,
    .bFrameIndex = 1,
    .bmCapabilities = 0,
    .wWidth = cpu_to_le16(1920),
    .wHeight = cpu_to_le16(1080),
    .dwMinBitRate = cpu_to_le32(497664000),
    .dwMaxBitRate = cpu_to_le32(829440000),
    .dwDefaultFrameInterval = cpu_to_le32(333333),
    .bFrameIntervalType = 1,
    .dwFrameInterval[0] = cpu_to_le32(333333),
};

static const struct UVC_FRAME_FRAMEBASED(1) uvc_frame_h264_4mp = {
    .bLength = UVC_DT_FRAME_FRAMEBASED_SIZE(1),
    .bDescriptorType = USB_DT_CS_INTERFACE,
    .bDescriptorSubType = UVC_VS_FRAME_FRAMEBASED,
    .bFrameIndex = 5,
    .bmCapabilities = 0,
    .wWidth = cpu_to_le16(2560),
    .wHeight = cpu_to_le16(1440),
    .dwMinBitRate = cpu_to_le32(884736000),
    .dwMaxBitRate = cpu_to_le32(1769462000),
    .dwDefaultFrameInterval = cpu_to_le32(333333),
    .bFrameIntervalType = 1,
    .dwFrameInterval[0] = cpu_to_le32(333333),
};

static const struct UVC_FRAME_FRAMEBASED(1) uvc_frame_h264_5mp = {
    .bLength = UVC_DT_FRAME_FRAMEBASED_SIZE(1),
    .bDescriptorType = USB_DT_CS_INTERFACE,
    .bDescriptorSubType = UVC_VS_FRAME_FRAMEBASED,
    .bFrameIndex = 6,
    .bmCapabilities = 0,
    .wWidth = cpu_to_le16(2592),
    .wHeight = cpu_to_le16(1944),
    .dwMinBitRate = cpu_to_le32(806215680),
    .dwMaxBitRate = cpu_to_le32(1209323520),
    .dwDefaultFrameInterval = cpu_to_le32(666666),
    .bFrameIntervalType = 1,
    .dwFrameInterval[0] = cpu_to_le32(666666),
};

static struct uvc_color_matching_descriptor uvc_color_matching = {
    .bLength = UVC_DT_COLOR_MATCHING_SIZE,
    .bDescriptorType = USB_DT_CS_INTERFACE,
    .bDescriptorSubType = UVC_VS_COLORFORMAT,
    .bColorPrimaries = 1,
    .bTransferCharacteristics = 1,
    .bMatrixCoefficients = 4,
};

static struct uvc_descriptor_header* uvc_fs_control_cls[8] = {
    (const struct uvc_descriptor_header*)&uvc_control_header,
    (const struct uvc_descriptor_header*)&uvc_camera_terminal,
    (const struct uvc_descriptor_header*)&uvc_processing,
    (const struct uvc_descriptor_header*)&uvc_output_terminal,
    NULL,
};

static const struct uvc_descriptor_header* const uvc_ss_control_cls[] = {
    (const struct uvc_descriptor_header*)&uvc_input_header,
    (const struct uvc_descriptor_header*)&uvc_camera_terminal,
    (const struct uvc_descriptor_header*)&uvc_processing,
    (const struct uvc_descriptor_header*)&uvc_output_terminal,
    NULL,
};

static const struct uvc_descriptor_header* const uvc_fs_streaming_cls[] = {
    (const struct uvc_descriptor_header*)&uvc_input_header,
    (const struct uvc_descriptor_header*)&uvc_format_yuv,
    (const struct uvc_descriptor_header*)&uvc_format_yuv_360p,
    (const struct uvc_descriptor_header*)&uvc_format_yuv_720p,
    (const struct uvc_descriptor_header*)&uvc_format_mjpg,
    (const struct uvc_descriptor_header*)&uvc_format_mjpg_720p,
    (const struct uvc_descriptor_header*)&uvc_format_mjpg_960p,
    (const struct uvc_descriptor_header*)&uvc_format_mjpg_1080p,
    (const struct uvc_descriptor_header*)&uvc_color_matching,
    NULL,
};

static const struct uvc_descriptor_header* const uvc_hs_streaming_cls[] = {
    (const struct uvc_descriptor_header*)&uvc_input_header,
    (const struct uvc_descriptor_header*)&uvc_format_yuv,
    (const struct uvc_descriptor_header*)&uvc_format_yuv_360p,
    (const struct uvc_descriptor_header*)&uvc_format_yuv_720p,
    (const struct uvc_descriptor_header*)&uvc_still_image_frame_base,
    (const struct uvc_descriptor_header*)&uvc_format_mjpg,
    (const struct uvc_descriptor_header*)&uvc_format_mjpg_1080p,
    (const struct uvc_descriptor_header*)&uvc_format_mjpg_960p,
    (const struct uvc_descriptor_header*)&uvc_format_mjpg_360p,
    (const struct uvc_descriptor_header*)&uvc_format_mjpg_720p,
    (const struct uvc_descriptor_header*)&uvc_still_image_frame_base,
    (const struct uvc_descriptor_header*)&uvc_frame_h264,
    (const struct uvc_descriptor_header*)&uvc_frame_h264_1080p,
    (const struct uvc_descriptor_header*)&uvc_frame_h264_960p,
    (const struct uvc_descriptor_header*)&uvc_frame_h264_360p,
    (const struct uvc_descriptor_header*)&uvc_frame_h264_720p,
    (const struct uvc_descriptor_header*)&uvc_color_matching,
    NULL,
};

static const struct uvc_descriptor_header* const* const uvc_ss_streaming_cls[] = {
    (const struct uvc_descriptor_header*)&uvc_input_header,
    (const struct uvc_descriptor_header*)&uvc_format_yuv,
    (const struct uvc_descriptor_header*)&uvc_format_yuv_360p,
    (const struct uvc_descriptor_header*)&uvc_format_yuv_720p,
    (const struct uvc_descriptor_header*)&uvc_format_mjpg,
    (const struct uvc_descriptor_header*)&uvc_format_mjpg_720p,
    (const struct uvc_descriptor_header*)&uvc_format_mjpg_960p,
    (const struct uvc_descriptor_header*)&uvc_format_mjpg_1080p,
    (const struct uvc_descriptor_header*)&uvc_color_matching,
    NULL,
};

/* --------------------------------------------------------------------------
 * USB Configuration
 */

int ucam_bind_uvc(struct usb_configuration* c)
{
    if (ucamera_streaming_cls == NULL) {
        printk("webcam bind uvc config failed\n");
        return -1;
    }
    if (af_en) {
        uvc_camera_terminal.bmControls[0] = 0x2a;
        uvc_camera_terminal.bmControls[3] = 0x02;
    }
    if (uvc_extension_en) {
        uvc_output_terminal.bSourceID = 4;
        uvc_fs_control_cls[3] = &uvc_extension_unit;
        uvc_fs_control_cls[4] = &uvc_output_terminal;
        uvc_fs_control_cls[5] = NULL;
    }
    return uvc_bind_config(c, uvc_fs_control_cls, uvc_ss_control_cls,
        uvc_fs_streaming_cls, ucamera_streaming_cls, uvc_ss_streaming_cls);
}
EXPORT_SYMBOL(ucam_bind_uvc);

/* --------------------------------------------------------------------------
 * Driver
 */

int uvc_set_descriptor(unsigned long arg)
{
    return 0;
}
EXPORT_SYMBOL(uvc_set_descriptor);

int uvc_sti_init(void)
{
    return 0;
}
EXPORT_SYMBOL(uvc_sti_init);
