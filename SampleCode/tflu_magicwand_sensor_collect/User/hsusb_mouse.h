#ifndef __HSUSB_MOUSE_H__
#define __HSUSB_MOUSE_H__

//
// VID & PID
//
#define HSUSBD_VID              0x0416
#define HSUSBD_PID              0x8249

//
// HID Class Request
//
#define GET_REPORT              0x01
#define GET_IDLE                0x02
#define GET_PROTOCOL            0x03
#define SET_REPORT              0x09
#define SET_IDLE                0x0A
#define SET_PROTOCOL            0x0B

//
// USB HID Class protocol
//
#define HID_NONE                0x00
#define HID_KEYBOARD            0x01
#define HID_MOUSE               0x02

//
// USB HID Class Report Type
//
#define HID_RPT_TYPE_INPUT      0x01
#define HID_RPT_TYPE_OUTPUT     0x02
#define HID_RPT_TYPE_FEATURE    0x03

//
// USB HID Protocol Type
//
#define HID_BOOT_PROTOCOL       0x00
#define HID_REPORT_PROTOCOL     0x01

//
// Endpoint Maximum Packet Size
//
#define CEP_MAX_PKT_SIZE        64
#define CEP_OTHER_MAX_PKT_SIZE  64
#define EPA_MAX_PKT_SIZE        64
#define EPA_OTHER_MAX_PKT_SIZE  64

//
// Endpoint Memory Assigned
//
#define CEP_BUF_BASE    0
#define CEP_BUF_LEN     CEP_MAX_PKT_SIZE
#define EPA_BUF_BASE    (CEP_BUF_BASE + CEP_BUF_LEN)
#define EPA_BUF_LEN     EPA_MAX_PKT_SIZE

//
// Endpoint Number Assigned
//
#define HS_INT_IN_EP_NUM           0x01
#define HS_INT_IN_EP_INTERVAL      1

//
// Feature Supported
//
#define USBD_SELF_POWERED               0
#define USBD_REMOTE_WAKEUP              0
#define USBD_MAX_POWER                  50  /* The unit is in 2mA. ex: 50 * 2mA = 100mA */

//
// Descriptor Length
//
#define HS_LEN_CONFIG_AND_SUBORDINATE      (LEN_CONFIG+LEN_INTERFACE+LEN_HID+LEN_ENDPOINT)

//
// Functions
//
void hsusb_low_level_init(void);
void hsusb_hid_mouse_handler(void);

#endif  /* __HSUSB_MOUSE_H__ */
