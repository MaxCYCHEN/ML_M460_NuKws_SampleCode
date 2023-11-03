//
// Includes
//
#include "NuMicro.h"
#include "fsusb_mouse.h"
#include "btn.h"

//
// Defines
//
#define TRIM_INIT       (SYS_BASE + 0x10C)

//
// Variable
//
static int8_t s_ai8MouseTable[] = { -16, -16, -16, 0, 16, 16, 16, 0};
static uint8_t s_u8MouseIdx = 0;
static uint8_t s_u8MoveLen, s_u8MouseMode = 1;

static uint8_t volatile s_u8EP2Ready = 0;
uint8_t volatile g_u8Suspend = 0;
static uint8_t s_u8Idle = 0, s_u8Protocol = 0;

//
// Prototype
//
static void fsusb_endpoint_setup(void);
static void fsusb_hid_class_request(void);
static void fsusb_ep2_handler(void);

//
// Functions
//

//
// fsusb_low_level_init
//
void fsusb_low_level_init(void)
{
    SET_USB_VBUS_PA12();
    SET_USB_D_N_PA13();
    SET_USB_D_P_PA14();
    SET_USB_OTG_ID_PA15();
    
    fsusb_endpoint_setup();
    
    USBD_Open(&gsInfo, fsusb_hid_class_request, NULL);
    USBD_Start();

    NVIC_EnableIRQ(USBD_IRQn);
}

//
// fsusb_auto_trim_execute
//
void fsusb_auto_trim_execute(void)
{
    uint32_t u32TrimInit;
    
    if((SYS->HIRCTCTL & SYS_HIRCTCTL_FREQSEL_Msk) != 1) {
        /* Start USB trim only when SOF */
        if(USBD->INTSTS & USBD_INTSTS_SOFIF_Msk) {
            /* Clear SOF */
            USBD->INTSTS = USBD_INTSTS_SOFIF_Msk;
    
            /* Re-enable crystal-less */
            SYS->HIRCTCTL = 0x01;
            SYS->HIRCTCTL |= SYS_HIRCTCTL_REFCKSEL_Msk | SYS_HIRCTCTL_BOUNDEN_Msk | (8 << SYS_HIRCTCTL_BOUNDARY_Pos);
        }
    }

    /* Disable USB Trim when error */
    if(SYS->HIRCTISTS & (SYS_HIRCTISTS_CLKERRIF_Msk | SYS_HIRCTISTS_TFAILIF_Msk)) {
        /* Init TRIM */
        M32(TRIM_INIT) = u32TrimInit;

        /* Disable crystal-less */
        SYS->HIRCTCTL = 0;

        /* Clear error flags */
        SYS->HIRCTISTS = SYS_HIRCTISTS_CLKERRIF_Msk | SYS_HIRCTISTS_TFAILIF_Msk;

        /* Clear SOF */
        USBD->INTSTS = USBD_INTSTS_SOFIF_Msk;
    }
}

//
// fsusb_hid_mouse_handler
//
void fsusb_hid_mouse_handler(void)
{
    uint8_t *pu8Buf;
    
    if (s_u8EP2Ready) {
        pu8Buf = (uint8_t *)(USBD_BUF_BASE + USBD_GET_EP_BUF_ADDR(EP2));
    
        if (btn_pressed_detect(BTN_1)) {
            pu8Buf[0] = 0x00;
            pu8Buf[1] = 16;
            pu8Buf[2] = 0x00;
            pu8Buf[3] = 0x00;
        } else {
            pu8Buf[0] = 0x00;
            pu8Buf[1] = 0x00;
            pu8Buf[2] = 0x00;
            pu8Buf[3] = 0x00;
        }
        
        s_u8EP2Ready = 0;
        /* Set transfer length and trigger IN transfer */
        USBD_SET_PAYLOAD_LEN(EP2, 4);
    }
#if 0
    if (s_u8EP2Ready) {
        pu8Buf = (uint8_t *)(USBD_BUF_BASE + USBD_GET_EP_BUF_ADDR(EP2));
        s_u8MouseMode ^= 1;

        if (s_u8MouseMode) {
            if (s_u8MoveLen > 14) {
                /* Update new report data */
                pu8Buf[0] = 0x00;
                pu8Buf[1] = (uint8_t)s_ai8MouseTable[s_u8MouseIdx & 0x07];
                pu8Buf[2] = (uint8_t)s_ai8MouseTable[(s_u8MouseIdx + 2) & 0x07];
                pu8Buf[3] = 0x00;
                s_u8MouseIdx++;
                s_u8MoveLen = 0;
            }
        } else {
            pu8Buf[0] = pu8Buf[1] = pu8Buf[2] = pu8Buf[3] = 0;
        }
        
        s_u8MoveLen++;
        s_u8EP2Ready = 0;
        /* Set transfer length and trigger IN transfer */
        USBD_SET_PAYLOAD_LEN(EP2, 4);
    }
#endif
}


//
// fsusb_endpoint_setup
//
static void fsusb_endpoint_setup(void)
{
    USBD->STBUFSEG = SETUP_BUF_BASE;

    //
    // EP0
    //  - CTRL_IN
    //
    USBD_CONFIG_EP(EP0, USBD_CFG_CSTALL | USBD_CFG_EPMODE_IN | 0);
    USBD_SET_EP_BUF_ADDR(EP0, EP0_BUF_BASE);
    
    //
    // EP1
    //  - CTRL_OUT
    //
    USBD_CONFIG_EP(EP1, USBD_CFG_CSTALL | USBD_CFG_EPMODE_OUT | 0);
    USBD_SET_EP_BUF_ADDR(EP1, EP1_BUF_BASE);

    //
    // EP2
    //  - INT_IN
    //
    USBD_CONFIG_EP(EP2, USBD_CFG_EPMODE_IN | FS_INT_IN_EP_NUM);
    USBD_SET_EP_BUF_ADDR(EP2, EP2_BUF_BASE);

    s_u8EP2Ready = 1;
}

//
// fsusb_hid_class_request
//
static void fsusb_hid_class_request(void)
{
    uint8_t au8Buf[8];

    USBD_GetSetupPacket(au8Buf);

    /* request data transfer direction */
    if (au8Buf[0] & 0x80) {
        // Device to host
        switch(au8Buf[1]) {
            case GET_REPORT:
            case GET_IDLE: {
                USBD_SET_PAYLOAD_LEN(EP1, au8Buf[6]);
                /* Data stage */
                USBD_PrepareCtrlIn(&s_u8Idle, au8Buf[6]);
                /* Status stage */
                USBD_PrepareCtrlOut(0, 0);
                break;
            }
            case GET_PROTOCOL: {
                USBD_SET_PAYLOAD_LEN(EP1, au8Buf[6]);
                /* Data stage */
                USBD_PrepareCtrlIn(&s_u8Protocol, au8Buf[6]);
                /* Status stage */
                USBD_PrepareCtrlOut(0, 0);
                break;
            }
            default: {
                /* Setup error, stall the device */
                USBD_SetStall(EP0);
                USBD_SetStall(EP1);
                break;
            }
        }
    } else {
        // Host to device
        switch(au8Buf[1]) {
            case SET_REPORT: {
                if(au8Buf[3] == 3) {
                    /* Request Type = Feature */
                    USBD_SET_DATA1(EP1);
                    USBD_SET_PAYLOAD_LEN(EP1, 0);
                }
                break;
            }
            case SET_IDLE: {
                s_u8Idle = au8Buf[3];
                /* Status stage */
                USBD_SET_DATA1(EP0);
                USBD_SET_PAYLOAD_LEN(EP0, 0);
                break;
            }
            case SET_PROTOCOL: {
                s_u8Protocol = au8Buf[2];
                /* Status stage */
                USBD_SET_DATA1(EP0);
                USBD_SET_PAYLOAD_LEN(EP0, 0);
                break;
            }
            default: {
                // Stall
                /* Setup error, stall the device */
                USBD_SetStall(EP0);
                USBD_SetStall(EP1);
                break;
            }
        }
    }
}

//
// fsusb_ep2_handler
//
static void fsusb_ep2_handler(void)
{
    s_u8EP2Ready = 1;
}

//
// USBD_IRQHandler
//
void USBD_IRQHandler(void)
{
    uint32_t volatile u32IntSts = USBD_GET_INT_FLAG();
    uint32_t volatile u32State = USBD_GET_BUS_STATE();

    if(u32IntSts & USBD_INTSTS_FLDET) {
        // Floating detect
        USBD_CLR_INT_FLAG(USBD_INTSTS_FLDET);

        if(USBD_IS_ATTACHED()) {
            /* USB Plug In */
            USBD_ENABLE_USB();
        } else {
            /* USB Un-plug */
            USBD_DISABLE_USB();
        }
    }

    if(u32IntSts & USBD_INTSTS_WAKEUP) {
        /* Clear event flag */
        USBD_CLR_INT_FLAG(USBD_INTSTS_WAKEUP);
    }

    if(u32IntSts & USBD_INTSTS_BUS) {
        /* Clear event flag */
        USBD_CLR_INT_FLAG(USBD_INTSTS_BUS);

        if(u32State & USBD_STATE_USBRST) {
            /* Bus reset */
            USBD_ENABLE_USB();
            USBD_SwReset();
            g_u8Suspend = 0;
        }
        
        if(u32State & USBD_STATE_SUSPEND) {
            /* Enter power down to wait USB attached */
            g_u8Suspend = 1;

            /* Enable USB but disable PHY */
            USBD_DISABLE_PHY();
        }
        
        if(u32State & USBD_STATE_RESUME) {
            /* Enable USB and enable PHY */
            USBD_ENABLE_USB();
            g_u8Suspend = 0;
        }
    }

    if(u32IntSts & USBD_INTSTS_USB) {
        // USB event
        if(u32IntSts & USBD_INTSTS_SETUP) {
            // Setup packet
            /* Clear event flag */
            USBD_CLR_INT_FLAG(USBD_INTSTS_SETUP);

            /* Clear the data IN/OUT ready flag of control end-points */
            USBD_STOP_TRANSACTION(EP0);
            USBD_STOP_TRANSACTION(EP1);

            USBD_ProcessSetupPacket();
        }

        // EP events
        if(u32IntSts & USBD_INTSTS_EP0) {
            /* Clear event flag */
            USBD_CLR_INT_FLAG(USBD_INTSTS_EP0);
            // control IN
            USBD_CtrlIn();
        }

        if(u32IntSts & USBD_INTSTS_EP1) {
            /* Clear event flag */
            USBD_CLR_INT_FLAG(USBD_INTSTS_EP1);
            // control OUT
            USBD_CtrlOut();
        }

        if(u32IntSts & USBD_INTSTS_EP2) {
            /* Clear event flag */
            USBD_CLR_INT_FLAG(USBD_INTSTS_EP2);
            // Interrupt IN
            fsusb_ep2_handler();
        }

        if(u32IntSts & USBD_INTSTS_EP3) {
            /* Clear event flag */
            USBD_CLR_INT_FLAG(USBD_INTSTS_EP3);
        }

        if(u32IntSts & USBD_INTSTS_EP4) {
            /* Clear event flag */
            USBD_CLR_INT_FLAG(USBD_INTSTS_EP4);
        }

        if(u32IntSts & USBD_INTSTS_EP5) {
            /* Clear event flag */
            USBD_CLR_INT_FLAG(USBD_INTSTS_EP5);
        }

        if(u32IntSts & USBD_INTSTS_EP6) {
            /* Clear event flag */
            USBD_CLR_INT_FLAG(USBD_INTSTS_EP6);
        }

        if(u32IntSts & USBD_INTSTS_EP7) {
            /* Clear event flag */
            USBD_CLR_INT_FLAG(USBD_INTSTS_EP7);
        }

        if(u32IntSts & USBD_INTSTS_EP8) {
            /* Clear event flag */
            USBD_CLR_INT_FLAG(USBD_INTSTS_EP8);
        }

        if(u32IntSts & USBD_INTSTS_EP9) {
            /* Clear event flag */
            USBD_CLR_INT_FLAG(USBD_INTSTS_EP9);
        }

        if(u32IntSts & USBD_INTSTS_EP10) {
            /* Clear event flag */
            USBD_CLR_INT_FLAG(USBD_INTSTS_EP10);
        }

        if(u32IntSts & USBD_INTSTS_EP11) {
            /* Clear event flag */
            USBD_CLR_INT_FLAG(USBD_INTSTS_EP11);
        }
    }
}

