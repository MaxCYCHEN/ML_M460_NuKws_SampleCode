//
// Includes
//
#include "NuMicro.h"

//
// Variables
//
static CANFD_FD_MSG_T sRxMsgFrame;
static CANFD_FD_MSG_T sTxMsgFrame;

//
// Functions
//

//
// canfd_low_levle_init
//
void canfd_low_levle_init(void)
{
    CANFD_FD_T sCANFD_Config;
    
    SET_CAN0_TXD_PJ10();
    SET_CAN0_RXD_PJ11();
    
    CANFD_GetDefaultConfig(&sCANFD_Config, CANFD_OP_CAN_FD_MODE);
    sCANFD_Config.sBtConfig.sNormBitRate.u32BitRate = 1000000;
    sCANFD_Config.sBtConfig.sDataBitRate.u32BitRate = 4000000;
    
    CANFD_Open(CANFD0, &sCANFD_Config);
    
    CANFD_SetSIDFltr(CANFD0, 0, CANFD_RX_BUFFER_STD(0x222, 1));

    CANFD_RunToNormal(CANFD0, TRUE);
}

//
// canfd_txrx_test
//
int8_t canfd_txrx_test(void)
{
    uint8_t i;
    uint8_t cnt = 32;
    uint8_t delay;
    uint8_t retry;
    
    while(1) {
        if (retry > 10) {
            printf("FAIL...\r\n");
            return -1;
        }
        
        sTxMsgFrame.eIdType = eCANFD_SID;
        sTxMsgFrame.eFrmType = eCANFD_DATA_FRM;
        sTxMsgFrame.bFDFormat = 1;
        sTxMsgFrame.bBitRateSwitch = 1;
        sTxMsgFrame.u32Id = 0x111;
        sTxMsgFrame.u32DLC = 64;
        
        for (i = 0; i < sTxMsgFrame.u32DLC; i++) {
            sTxMsgFrame.au8Data[i] = 0xA5;
        }
        
        printf("Transmit[%d]\r\n", cnt);
        if (CANFD_TransmitTxMsg(CANFD0, 0, &sTxMsgFrame) != 1) {
            printf("Failed to transmit message\n");
            return -1;
        }

        delay = 0;
        
        while(1) {
            if (CANFD_ReadRxBufMsg(CANFD0, 1, &sRxMsgFrame) == 1) {
                printf("Receiving[%d]\r\n", cnt);
                for (i = 0; i < 64; i++) {
                    if (sRxMsgFrame.au8Data[i] != 0xA5) {
                        printf("Comapre fail...\r\n");
                        return -1;
                    }
                }
                cnt--;
                break;
            } else {
                CLK_SysTickDelay(100000);
                if (delay++ > 10) {
                    printf("time-out, retry[%d]\r\n", retry++);
                    break;
                }
            }
        }
        
        if (cnt == 0) {
            printf("PASS...\r\n");
            return 0;
        }
    }
}
