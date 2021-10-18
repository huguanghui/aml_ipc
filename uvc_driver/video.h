#ifndef __LINUX_USB_VIDEO_H
#define __LINUX_USB_VIDEO_H

#include <linux/types.h>

// UVC constants

// A.2. Video Interface Subclass Codes
#define UVC_SC_UNDEFINED 0x00
#define UVC_SC_VIDEOCONTROL 0x01
#define UVC_SC_VIDEOSTREAMING 0x02
#define UVC_SC_VIDEO_INTERFACE_COLLECTION 0x03

// A.3. Video Interface Protocol Codes
#define UVC_PC_PROTOCOL_UNDEFINED 0x00

// A.5. Video Clas-Specific VC Interface Descriptor Subtypes
#define UVC_VC_DESCRIPTOR_UNDEFINED 0x00
#define UVC_VC_HEADER 0x01
#define UVC_VC_INPUT_TERMINAL 0x02
#define UVC_VC_OUTPUT_TERMINAL 0x03
#define UVC_VC_SELECTOR_UNIT 0x04
#define UVC_VC_PROCESSING_UNIT 0x05
#define UVC_VC_EXTENSION_UNIT 0x06

#endif // !__LINUX_USB_VIDEO_H
