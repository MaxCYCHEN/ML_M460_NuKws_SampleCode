#include <stdio.h>
#include <string.h>
#include "NuMicro.h"

#include "btn.h"
#include "spi_flash.h"
#include "hyperram.h"
#include "fsusb_mouse.h"
#include "hsusb_mouse.h"
#include "audio.h"
#include "diskio.h"
#include "ff.h"
#include "synopGMAC_network_interface.h"
#include "canfd_test.h"
#include "MPU6500.h"
#include "NCT7717.h"
#include "WiFi.h"

//
// Define
//

#define IMU_SAMPLERATE_100HZ    100
#define IMU_SAMPLERATE_50HZ       50
#define IMU_SAMPLERATE_10HZ       10



//
// Variable
//
static TCHAR sd_path[] = { '0', ':', 0 };
static volatile uint32_t gu32SysTickCnts = 0;

extern synopGMACdevice GMACdev[GMAC_CNT];
extern struct sk_buff txbuf[GMAC_CNT];
extern struct sk_buff rxbuf[GMAC_CNT];
extern void synopGMAC0_intr_handler(void);

//
// Prototype
//
static void led_low_level_init(void);
static void tmr_tick_low_level_init(void);

static void sdhc_low_level_init(void);
static int8_t sdhc_rw_test(void);

static void emac_low_level_init(void);
static int8_t emac_loopback_test(void);

void TMR0_IRQHandler(void);


//
// Functions
//
void TMR0_IRQHandler(void)
{
    // clear timer interrupt flag
    TIMER_ClearIntFlag(TIMER0);
    MPU6500_readXYZ_mg();

}
//
// sys_init
//
static void sys_init(void)
{
    /* Unlock protected registers */
    SYS_UnlockReg();
	
    /* Set XT1_OUT(PF.2) and XT1_IN(PF.3) to input mode */
    PF->MODE &= ~(GPIO_MODE_MODE2_Msk | GPIO_MODE_MODE3_Msk);

    /* Enable HXT clock */
    CLK_EnableXtalRC(CLK_PWRCTL_HXTEN_Msk);

    /* Wait for HXT clock ready */
    CLK_WaitClockReady(CLK_STATUS_HXTSTB_Msk);
    
    /* Enable LIRC clock */
    CLK_EnableXtalRC(CLK_PWRCTL_LIRCEN_Msk);

    /* Wait for LIRC clock ready */
    CLK_WaitClockReady(CLK_STATUS_LIRCSTB_Msk);
	
    /* Enable HIRC48M clock */
    CLK_EnableXtalRC(CLK_PWRCTL_HIRC48MEN_Msk);

    /* Waiting for HIRC48M clock ready */
    CLK_WaitClockReady(CLK_STATUS_HIRC48MSTB_Msk);
    
    /* Set core clock as PLL_CLOCK from PLL */
    CLK_SetCoreClock(192000000);

    /* Enable all GPIO clock */
    CLK->AHBCLK0 |= CLK_AHBCLK0_GPACKEN_Msk | CLK_AHBCLK0_GPBCKEN_Msk | CLK_AHBCLK0_GPCCKEN_Msk | CLK_AHBCLK0_GPDCKEN_Msk |
                    CLK_AHBCLK0_GPECKEN_Msk | CLK_AHBCLK0_GPFCKEN_Msk | CLK_AHBCLK0_GPGCKEN_Msk | CLK_AHBCLK0_GPHCKEN_Msk;
    CLK->AHBCLK1 |= CLK_AHBCLK1_GPICKEN_Msk | CLK_AHBCLK1_GPJCKEN_Msk;

    /* Set PCLK0/PCLK1 to HCLK/2 */
    CLK->PCLKDIV = (CLK_PCLKDIV_PCLK0DIV2 | CLK_PCLKDIV_PCLK1DIV2);
	
    /* Enable module clock */
    CLK_EnableModuleClock(TMR0_MODULE);
    CLK_EnableModuleClock(UART0_MODULE);
    CLK_EnableModuleClock(UART2_MODULE);
    CLK_EnableModuleClock(QSPI0_MODULE);
    CLK_EnableModuleClock(USBD_MODULE);
    CLK_EnableModuleClock(HSUSBD_MODULE);
    CLK_EnableModuleClock(SDH0_MODULE);
    CLK_EnableModuleClock(EMAC0_MODULE);
    CLK_EnableModuleClock(CANFD0_MODULE);
    CLK_EnableModuleClock(I2C2_MODULE);
    
    /* Set module clock */
    CLK_SetModuleClock(TMR0_MODULE, CLK_CLKSEL1_TMR0SEL_HXT, NULL);
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_HXT, CLK_CLKDIV0_UART0(1));
    CLK_SetModuleClock(QSPI0_MODULE, CLK_CLKSEL2_QSPI0SEL_PCLK0, MODULE_NoMsk);
    //CLK_SetModuleClock(USBD_MODULE, CLK_CLKSEL0_USBSEL_HIRC48M, CLK_CLKDIV0_USB(1));
    CLK_SetModuleClock(USBD_MODULE, CLK_CLKSEL0_USBSEL_PLL_DIV2, CLK_CLKDIV0_USB(2));
    CLK_SetModuleClock(SDH0_MODULE, CLK_CLKSEL0_SDH0SEL_PLL_DIV2, CLK_CLKDIV0_SDH0(5));
    CLK_SetModuleClock(CANFD0_MODULE, CLK_CLKSEL0_CANFD0SEL_HCLK, CLK_CLKDIV5_CANFD0(1));
    CLK_SetModuleClock(UART2_MODULE, CLK_CLKSEL3_UART2SEL_HIRC, CLK_CLKDIV4_UART2(1));

    /* Enable FSUSB/HSUSB PHY */
    SYS->USBPHY = ((SYS->USBPHY & ~(SYS_USBPHY_USBROLE_Msk | SYS_USBPHY_HSUSBROLE_Msk | SYS_USBPHY_HSUSBACT_Msk)) 
        | SYS_USBPHY_USBEN_Msk | SYS_USBPHY_SBO_Msk | SYS_USBPHY_HSUSBEN_Msk);
    CLK_SysTickDelay(25);
    SYS->USBPHY |= SYS_USBPHY_HSUSBACT_Msk;
    
    /* Lock protected registers */
//    SYS_LockReg();
}

//
// debug_port_init
//
static void debug_port_init(void)
{
    SET_UART0_RXD_PB12();
    SET_UART0_TXD_PB13();
    
    UART_Open(UART0, 115200);
}


//
// imu timer run
//
static void imu_timer_run(void)
{
     // Set timer frequency to 1HZ
    TIMER_Open(TIMER0, TIMER_PERIODIC_MODE, IMU_SAMPLERATE_100HZ);//Sets IMU default sample rate to 100Hz

    // Enable timer interrupt
    TIMER_EnableInt(TIMER0);
    NVIC_EnableIRQ(TMR0_IRQn);

    // Start Timer 0
    TIMER_Start(TIMER0);
}

//
// main
//
int main(void)
{
    int8_t result = 0;
    
    //
    // Initialize system clock
    //
    sys_init();
    
    //
    // Initialize debug port 
    //
    debug_port_init();
	
	
	
	
	 
		
	printf("\r\n");
	printf(" ==================================\r\n");
	printf("|        < NuMaker-M467HJ >        |\r\n");
	printf("|     Mass Production Test Code    |\r\n");
	printf(" ==================================\r\n");
	printf("\r\n");

//    printf("\r\n ============== TICK INIT ============== \r\n\r\n");
//    tmr_tick_low_level_init();
//    printf("\r\n ================= Fin ================== \r\n");
//    
//    
//    printf("\r\n =============== LED INIT ============== \r\n\r\n");
//    led_low_level_init();
//    printf("\r\n ================= Fin ================== \r\n");
//    
//    
//    printf("\r\n =============== BTN INIT ============== \r\n\r\n");
//    btn_low_level_init();
//    printf("\r\n ================= Fin ================== \r\n");
//    
//    
//    printf("\r\n ============ SPI FLASH TEST ============ \r\n\r\n");
//    spi_flash_low_level_init();
//    result += spi_flash_rw_test();
//    printf("\r\n ================= Fin ================== \r\n");


//    printf("\r\n ============== SDHC INIT ============== \r\n\r\n");
//    sdhc_low_level_init();
//    SDH_Open_Disk(SDH0, CardDetect_From_GPIO);
//    CLK_SysTickDelay(100);
//    
//    if (SD0.IsCardInsert) {
//        f_chdrive(sd_path);
//        result += sdhc_rw_test();
//    } else {
//        result--;
//    }
//    printf("\r\n ================= Fin ================== \r\n");


//    printf("\r\n ============= CANFD INIT ============== \r\n\r\n");
//    canfd_low_levle_init();
//    result += canfd_txrx_test();
//    printf("\r\n ================= Fin ================= \r\n");
    
    printf("\r\n ============= GYRO INIT ============== \r\n\r\n");
    MPU6500_low_levle_init();
    result += MPU6500_test();
    printf("\r\n ================= Fin ================= \r\n");
    
//    printf("\r\n ============= Thermal INIT ============== \r\n\r\n");
//    NCT7717_low_levle_init();
//    result += NCT7717_test();
//    printf("\r\n ================= Fin ================= \r\n");
//    
//    printf("\r\n ============= WiFi INIT ============== \r\n\r\n");
//    esp_low_levle_init();
//    result += esp_test();
//    printf("\r\n ================= Fin ================= \r\n");
//    
//    printf("\r\n ============== EAMC INIT ============== \r\n\r\n");
//    emac_low_level_init();
//    result += emac_loopback_test();
//    printf("\r\n ================= Fin ================= \r\n");
//    
//    printf("\r\n ============== FSUSB INIT ============== \r\n\r\n");
//    fsusb_low_level_init();
//    printf("\r\n ================= Fin ================== \r\n");
//    

//    printf("\r\n ============== HSUSB INIT ============== \r\n\r\n");
//    hsusb_low_level_init();
//    printf("\r\n ================= Fin ================== \r\n");
    
    //
	  // Initialize and start timer INT  
	  //
    imu_timer_run();
		
		while(1) {
			    
			
//			 printf("\r\n ======= GYRO LOOPING======== \r\n\r\n");
//			 result = MPU6500_test();
//        //
//        // FS-USBD
//        //
//        //fsusb_auto_trim_execute();
//        fsusb_hid_mouse_handler();
//        
//        //
//        // HS-USBD
//        //
//        hsusb_hid_mouse_handler();
//        
//        if (result >= 0) {
//            PH4 = PH5 = PH6 = 0;
//        } else {
//            if (TIMER_GetIntFlag(TIMER0) == 1) {
//                PH4 ^= 1;
//                PH5 ^= 1;
//                PH6 ^= 1;
//                TIMER_ClearIntFlag(TIMER0);
//            }
//        }
    }
}

//
// led_low_level_init
//
static void led_low_level_init(void)
{
    GPIO_SetMode(PH, (BIT4 | BIT5 | BIT6), GPIO_MODE_OUTPUT);
    PH4 = PH5 = PH6 = 1;
}

//
// tmr_tick_low_level_init
//
static void tmr_tick_low_level_init(void)
{
    TIMER_Open(TIMER0, TIMER_PERIODIC_MODE, 4);
    TIMER_EnableInt(TIMER0);
    TIMER_Start(TIMER0);
}

//
// plat_delay
//
void plat_delay(uint32_t ticks)
{
	uint32_t tgtTicks = gu32SysTickCnts + ticks;    // target tick count to delay execution to
	while (gu32SysTickCnts == tgtTicks) {}
}

//
// get_fattime
//
unsigned long get_fattime(void)
{
    return 0;
}

//
// sdhc_low_level_init
//
static void sdhc_low_level_init(void)
{
    SET_SD0_DAT0_PE2();
    SET_SD0_DAT1_PE3();
    SET_SD0_DAT2_PE4();
    SET_SD0_DAT3_PE5();
    SET_SD0_CLK_PE6();
    SET_SD0_CMD_PE7();
    SET_SD0_nCD_PD13();
}

//
// sdhc_rw_test
//
static int8_t sdhc_rw_test(void)
{
	FIL file;
	FRESULT res;
	UINT bw, br;
	int cmp;
    
	TCHAR pattern[32] = "1234567890";
	TCHAR readback[32];
	
    printf("Open file...");
	res = f_open(&file, "test.txt", (FA_READ | FA_WRITE | FA_CREATE_ALWAYS));
	if (res != FR_OK) {
        printf("<fail>\n");
        return -1;
    }
	printf("<done>\n");
    
    printf("Write file...");
	res = f_write(&file, pattern, strlen(pattern), &bw);
	if (res != FR_OK) {
        printf("<fail>\n");
        return -1;
    }
	printf("<done>\n");
    
    printf("Seek to top...");
    f_lseek(&file, 0);
    printf("<done>\n");
    
    printf("Read file...");
	res = f_read(&file, readback, strlen(pattern), &br);
	if (res != FR_OK) {
        printf("<fail>\n");
        return -1;
    }
	printf("<done>\n");
    
    printf("Compare...");
    if (strcmp(pattern, readback) != 0) {
        f_close(&file);
        printf("<fail>\n");
        return -1;
    }
    printf("<done>\n");
    
    printf("Close file...");
	f_close(&file);
    printf("<done>\n");
    
	return 0;
}

//
// emac_low_level_init
//
static void emac_low_level_init(void)
{
    synopGMACdevice *gmacdev = &GMACdev[0];
    
    SET_EMAC0_RMII_MDC_PE8();
    SET_EMAC0_RMII_MDIO_PE9();
    SET_EMAC0_RMII_TXD0_PE10();
    SET_EMAC0_RMII_TXD1_PE11();
    SET_EMAC0_RMII_TXEN_PE12();    
    SET_EMAC0_RMII_REFCLK_PC8();
    SET_EMAC0_RMII_RXD0_PC7();
    SET_EMAC0_RMII_RXD1_PC6();
    SET_EMAC0_RMII_CRSDV_PA7();
    SET_EMAC0_RMII_RXERR_PA6();
    SET_EMAC0_PPS_PB6();

    synopGMAC_open(0);
    synopGMAC_promisc_enable(gmacdev);
    synopGMAC_set_mode(0, 1);

    NVIC_EnableIRQ(EMAC0_TXRX_IRQn);    
}


//
// emac_loopback_test
//
int8_t emac_loopback_test(void)
{
    struct sk_buff *rskb = &rxbuf[0];
    struct sk_buff *tskb = &txbuf[0];
    
    uint32_t i;
    uint8_t pattern[128];
    
    for (i = 0; i < 128; i++) {
        pattern[i] = 0xA5;
    }
    
    rskb->rdy = 0;
    
    printf("Wait for linking...\r\n");
    for (i = 0; i < 100; i++)
        CLK_SysTickDelay(250000);
    
    //
    // Tx
    //
    printf("Send packet...\r\n");
    tskb->len = 128;
    memcpy((uint8_t *)((u64)(tskb->data)), pattern, 128);
    synopGMAC_xmit_frames(tskb, 0, 0, 0);    

    //
    // Wait
    //
    i = 0;
    while (!rskb->rdy) {
        CLK_SysTickDelay(100000);
        if (i++ > 50) {
            printf("time-out...\r\n");
            return -1;
        }
    }

    printf("Revice packet...\r\n");
    for (i = 0; i < 128; i++) {
        if (*(uint8_t *)(((u64)(rskb->data)) + i) != pattern[i]) {
            printf("Comapre fail...\r\n");
            return -1;
        }
    }
    printf("Comapre done...\r\n");
    
    return 0;
}

//
// SysTick_Handler
//
void SysTick_Handler(void)
{
	gu32SysTickCnts++;
}

//
// EMAC0_IRQHandler
//
void EMAC0_IRQHandler(void)
{
    struct sk_buff *rskb = &rxbuf[0];

    rskb->rdy = 0;
    
    synopGMAC0_intr_handler();
}

//
// SDH0_IRQHandler
//
void SDH0_IRQHandler(void)
{
    unsigned int volatile isr;

    // FMI data abort interrupt
    if(SDH0->GINTSTS & SDH_GINTSTS_DTAIF_Msk) {
        /* ResetAllEngine() */
        SDH0->GCTL |= SDH_GCTL_GCTLRST_Msk;
    }

    //----- SD interrupt status
    isr = SDH0->INTSTS;
    if(isr & SDH_INTSTS_BLKDIF_Msk) {
        // block down
        SD0.DataReadyFlag = TRUE;
        SDH0->INTSTS = SDH_INTSTS_BLKDIF_Msk;
    }

    if(isr & SDH_INTSTS_CDIF_Msk)    // port 0 card detect
    {
        //----- SD interrupt status
        // it is work to delay 50 times for SD_CLK = 200KHz
        {
            int volatile i;         // delay 30 fail, 50 OK
            for(i = 0; i < 0x500; i++); // delay to make sure got updated value from REG_SDISR.
            isr = SDH0->INTSTS;
        }

        if(isr & SDH_INTSTS_CDSTS_Msk)
        {
            printf("\n***** card remove !\n");
            SD0.IsCardInsert = FALSE;   // SDISR_CD_Card = 1 means card remove for GPIO mode
            memset(&SD0, 0, sizeof(SDH_INFO_T));
        }
        else
        {
            printf("***** card insert !\n");
            SDH_Open(SDH0, CardDetect_From_GPIO);
            SDH_Probe(SDH0);
        }

        SDH0->INTSTS = SDH_INTSTS_CDIF_Msk;
    }

    // CRC error interrupt
    if(isr & SDH_INTSTS_CRCIF_Msk)
    {
        if(!(isr & SDH_INTSTS_CRC16_Msk))
        {
            //printf("***** ISR sdioIntHandler(): CRC_16 error !\n");
            // handle CRC error
        }
        else if(!(isr & SDH_INTSTS_CRC7_Msk))
        {
            if(!SD0.R3Flag)
            {
                //printf("***** ISR sdioIntHandler(): CRC_7 error !\n");
                // handle CRC error
            }
        }
        SDH0->INTSTS = SDH_INTSTS_CRCIF_Msk;      // clear interrupt flag
    }

    if(isr & SDH_INTSTS_DITOIF_Msk)
    {
        printf("***** ISR: data in timeout !\n");
        SDH0->INTSTS |= SDH_INTSTS_DITOIF_Msk;
    }

    // Response in timeout interrupt
    if(isr & SDH_INTSTS_RTOIF_Msk)
    {
        printf("***** ISR: response in timeout !\n");
        SDH0->INTSTS |= SDH_INTSTS_RTOIF_Msk;
    }
}
