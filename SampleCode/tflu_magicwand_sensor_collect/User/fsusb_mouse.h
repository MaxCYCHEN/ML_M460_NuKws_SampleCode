#ifndef __FSUSB_MOUSE_H__
#define __FSUSB_MOUSE_H__

//
// Includes
//
#include "NuMicro.h"

//
// VID & PID
//
#define FSUSBD_VID        0x0416
#define FSUSBD_PID        0xB001

//
// HID Class Request
//
#define GET_REPORT          0x01
#define GET_IDLE            0x02
#define GET_PROTOCOL        0x03
#define SET_REPORT          0x09
#define SET_IDLE            0x0A
#define SET_PROTOCOL        0x0B

//
// USB HID Class protocol
//
#define HID_NONE            0x00
#define HID_KEYBOARD        0x01
#define HID_MOUSE           0x02

//
// USB HID Class Report Type
//
#define HID_RPT_TYPE_INPUT      0x01
#define HID_RPT_TYPE_OUTPUT     0x02
#define HID_RPT_TYPE_FEATURE    0x03

//
// Endpoint Maximum Packet Size
//
#define EP0_MAX_PKT_SIZE    8
#define EP1_MAX_PKT_SIZE    EP0_MAX_PKT_SIZE
#define EP2_MAX_PKT_SIZE    8

//
// Endpoint Memory Assigned
//
#define SETUP_BUF_BASE      0
#define SETUP_BUF_LEN       8

#define EP0_BUF_BASE        (SETUP_BUF_BASE + SETUP_BUF_LEN)
#define EP0_BUF_LEN         EP0_MAX_PKT_SIZE

#define EP1_BUF_BASE        (SETUP_BUF_BASE + SETUP_BUF_LEN)
#define EP1_BUF_LEN         EP1_MAX_PKT_SIZE

#define EP2_BUF_BASE        (EP1_BUF_BASE + EP1_BUF_LEN)
#define EP2_BUF_LEN         EP2_MAX_PKT_SIZE

//
// Endpoint Number Assigned
//
#define FS_INT_IN_EP_NUM           0x01
#define FS_INT_IN_EP_INTERVAL      10

//
// Feature Supported
//
#define USBD_SELF_POWERED       0
#define USBD_REMOTE_WAKEUP      0
#define USBD_MAX_POWER          50  /* The unit is in 2mA. ex: 50 * 2mA = 100mA */

//
// Descriptor Length
//
#define FS_LEN_CONFIG_AND_SUBORDINATE      (LEN_CONFIG + LEN_INTERFACE + LEN_HID + LEN_ENDPOINT)


//
// Global Variables
//
extern uint8_t volatile g_u8Suspend;

//
// Functions
//
void fsusb_low_level_init(void);
void fsusb_auto_trim_execute(void);
void fsusb_hid_mouse_handler(void);

#endif  /* __FSUSB_MOUSE_H__ */
