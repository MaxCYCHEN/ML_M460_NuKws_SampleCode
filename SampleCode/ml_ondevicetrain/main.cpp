/**************************************************************************//**
 * @file     main.c
 * @version  V3.00
 * @brief    This is a G-sensor real-time example with TFLITE v2.
 *           This example can change different model.
 *           Please remember that the pose should be: nu-link port is outside user's body.
 * @copyright SPDX-License-Identifier: Apache-2.0
 * @copyright Copyright (C) 2021 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#include <stdio.h>
#include <string.h>
#include "NuMicro.h"
#include "config.h"
#include <vector>
#include "svm.h"
#include <stdlib.h>
#include "svm_feature.h"
using namespace std;


#include "MPU6500.h"
#include "ili9341.h"
#include "spi_panel.h"

#define NAU8822     1
#define LOG_SVMFEATURE

static uint32_t s_au32PcmRxBuff[2][BUFF_LEN] = {{0}};
static DMA_DESC_T DMA_TXDESC[2], DMA_RXDESC[2];

static volatile uint8_t s_u8TxIdx = 0, s_u8RxIdx = 0;
static volatile uint8_t s_u8CopyData = 0;
static volatile uint8_t s_u8TimeUpFlag = 0;
static volatile uint8_t s_u8CopygsensorData = 0;
static volatile uint16_t s_u16gsensorSize = 0;
volatile uint32_t g_u32Ticks = 0;
DATABUFFER acc_x, acc_y, acc_z;
float fBuff[3];
static volatile uint8_t s_u8NetPatternFlag = 0;

#ifdef  __cplusplus
extern  "C" {
#endif
void TMR0_IRQHandler(void);
void SysTick_Handler(void);
//void PDMA0_IRQHandler(void);
void GPH_IRQHandler(void);
#ifdef  __cplusplus
}
#endif

void SYS_Init(void);
void PDMA_Init(void);
void I2C2_Init(void);

#if NAU8822
void I2C_WriteNAU8822(uint8_t u8Addr, uint16_t u16Data);
void NAU8822_Setup(void);
#else
uint8_t I2C_WriteMultiByteforNAU88L25(uint8_t u8ChipAddr, uint16_t u16SubAddr, const uint8_t *p, uint32_t u32Len);
uint8_t I2C_WriteNAU88L25(uint16_t u16Addr, uint16_t u16Dat);
void NAU88L25_Reset(void);
void NAU88L25_Setup(void);
#endif

void SysTick_Handler()
{
    g_u32Ticks++;
}


void TMR0_IRQHandler(void)
{
    s_u8TimeUpFlag = 1;
    //printf("TMR0_IRQHandler\n");

    // clear timer interrupt flag
    TIMER_ClearIntFlag(TIMER0);
}


//
// button handler
//
void GPH_IRQHandler(void)
{
    volatile uint32_t u32temp;

    /* To check if PH.0 interrupt occurred */
    if(GPIO_GET_INT_FLAG(PH, BIT0))
    {
        GPIO_CLR_INT_FLAG(PH, BIT0);
        s_u8NetPatternFlag =1;
        printf("PH.0 INT occurred.\n");
    }
    else
    {
        /* Un-expected interrupt. Just clear all PC interrupts */
        u32temp = PC->INTSRC;
        PC->INTSRC = u32temp;
        printf("Un-expected interrupts.\n");
    }
}

//
// sensor_timer run
//
void sensor_timer_run(void)
{
    // Set timer frequency to 1HZ
    TIMER_Open(TIMER0, TIMER_PERIODIC_MODE, 400);//Sets IMU default sample rate to 100Hz

    // Enable timer interrupt
    TIMER_EnableInt(TIMER0);
    NVIC_EnableIRQ(TMR0_IRQn);

    // Start Timer 0
    TIMER_Start(TIMER0);
}

void SYS_Init(void)
{
    /* Set PF multi-function pins for XT1_OUT(PF.2) and XT1_IN(PF.3) */
    SET_XT1_OUT_PF2();
    SET_XT1_IN_PF3();

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/

    /* Enable HIRC and HXT clock */
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk | CLK_PWRCTL_HXTEN_Msk);

    /* Wait for HIRC and HXT clock ready */
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk | CLK_STATUS_HXTSTB_Msk);

    /* Set PCLK0 and PCLK1 to HCLK/2 */
    CLK->PCLKDIV = (CLK_PCLKDIV_APB0DIV_DIV2 | CLK_PCLKDIV_APB1DIV_DIV2);

    /* Set core clock to 200MHz */
    CLK_SetCoreClock(FREQ_200MHZ);

    /* Enable all GPIO clock */
    CLK->AHBCLK0 |= CLK_AHBCLK0_GPACKEN_Msk | CLK_AHBCLK0_GPBCKEN_Msk | CLK_AHBCLK0_GPCCKEN_Msk | CLK_AHBCLK0_GPDCKEN_Msk |
                    CLK_AHBCLK0_GPECKEN_Msk | CLK_AHBCLK0_GPFCKEN_Msk | CLK_AHBCLK0_GPGCKEN_Msk | CLK_AHBCLK0_GPHCKEN_Msk;
    CLK->AHBCLK1 |= CLK_AHBCLK1_GPICKEN_Msk | CLK_AHBCLK1_GPJCKEN_Msk;

    /* Enable UART0 module clock */
    CLK_EnableModuleClock(UART0_MODULE);

    /* Select UART0 module clock source as HIRC and UART0 module clock divider as 1 */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_HIRC, CLK_CLKDIV0_UART0(1));

    /* Enable I2C2 module clock */
    CLK_EnableModuleClock(I2C2_MODULE);

    /* Enable SPI2 module clock */
    CLK_EnableModuleClock(SPI2_MODULE);

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/

    /* Set multi-function pins for UART0 RXD and TXD */
    SET_UART0_RXD_PB12();
    SET_UART0_TXD_PB13();

    /* Enable TIMER0 clock */
    CLK_EnableModuleClock(TMR0_MODULE);

    /* Set TIMER0 clock */
    CLK_SetModuleClock(TMR0_MODULE, CLK_CLKSEL1_TMR0SEL_HIRC, 0);

    /* Set SPI2 module clock */
    CLK_SetModuleClock(SPI2_MODULE, CLK_CLKSEL3_SPI2SEL_PCLK1, MODULE_NoMsk);

    /* Set I2C2 multi-function pins */
    SET_I2C2_SDA_PD0();
    SET_I2C2_SCL_PD1();

    /* Enable I2C2 clock pin (PD1) schmitt trigger */
    PD->SMTEN |= GPIO_SMTEN_SMTEN1_Msk;
}


/* Init I2C interface */
void I2C2_Init(void)
{
    /* Open I2C2 and set clock to 100k */
    I2C_Open(I2C2, 400000);

    /* Get I2C2 Bus Clock */
    printf("I2C clock %d Hz\n", I2C_GetBusClockFreq(I2C2));
}



/*---------------------------------------------------------------------------------------------------------*/
/*  Main Function                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
int32_t main(void)
{
    struct svm_node one_imu_svmnode[7] = {{.index =1, .value=0}, {.index =2, .value=0}, {.index =3, .value=0},
        {.index =4, .value=0}, {.index =5, .value=0}, {.index =6, .value=0},
        {.index =-1, .value=0}
    };
    float svm_scale_max[6] = {0,0,0,0,0,0};
    float svm_scale_min[6] = {65535, 65535, 65535, 65535, 65535, 65535};
    uint8_t x_ready, y_ready, z_ready;
    float label;
    static uint8_t u8Model_Ready = 0;

    float svm_problem[2];

    int32_t xin_len;
    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Init System, peripheral clock and multi-function I/O */
    SYS_Init();

    /* Init UART to 115200-8n1 for print message */
    UART_Open(UART0, 115200);

    printf("+-----------------------------------------------------------------------+\n");
    printf("|                Magic Wand real time inference with TFLM         |\n");
    printf("+-----------------------------------------------------------------------+\n");
    printf("  NOTE: \n");

    /* Use systick to measure inference time */
    SysTick_Config(SystemCoreClock / 1000);

    /* Init I2C2 to access codec */
    I2C2_Init();

    /*IMU gsensor init*/
    MPU6500_low_levle_init();

    /*gsensor 100Hz timer init*/
    sensor_timer_run();


    /*-----------------------------------------------------------------------------------------------------*/
    /* GPIO Interrupt Function Test                                                                        */
    /*-----------------------------------------------------------------------------------------------------*/
    printf("PH.0 and PH.1 are used to test interrupt ......\n");

    /* Configure PH.0 as Input mode and enable interrupt by rising edge trigger */
    GPIO_SetMode(PH, BIT0, GPIO_MODE_INPUT);
    GPIO_EnableInt(PH, 0, GPIO_INT_BOTH_EDGE);
    NVIC_EnableIRQ(GPH_IRQn);

    /* Enable interrupt de-bounce function and select de-bounce sampling cycle time is 1024 clocks of LIRC clock */
    GPIO_SET_DEBOUNCE_TIME(PH, GPIO_DBCTL_DBCLKSRC_LIRC, GPIO_DBCTL_DBCLKSEL_1024);
    GPIO_ENABLE_DEBOUNCE(PH, BIT0);

    const char outputClass[4][8] = {
        "O",
        "L",
        "W",
        "NULL"
    };
    float threshold = 0.5;
    const uint32_t gsensorBufferElements = 128*3;//sizeof(audioBuffer) / sizeof(int16_t);

    spi_panel_init();
    // Check fonts
    ILI9341_FillScreen(ILI9341_BLACK);
    ILI9341_WriteString(0, 0,        "     M467 Demo     ", Font_16x26, ILI9341_WHITE,  ILI9341_RED);
    ILI9341_WriteString(0, 2*16,   " On device Train   ", Font_16x26, ILI9341_WHITE,  ILI9341_RED);

    printf("Classifying..\r\n");

    g_u32Ticks = 0;
    xin_len = 0;

    svm_train_init(NULL, NULL);

    SVM_DataBufferReset(&acc_x);
    SVM_DataBufferSetSensorGain(&acc_x, 1000);
    SVM_DataBufferSetAxisIndex(&acc_x, 0);
    SVM_DataBufferReset(&acc_y);
    SVM_DataBufferSetSensorGain(&acc_y, 1000);
    SVM_DataBufferSetAxisIndex(&acc_y, 1);
    SVM_DataBufferReset(&acc_z);
    SVM_DataBufferSetSensorGain(&acc_z, 1000);
    SVM_DataBufferSetAxisIndex(&acc_z, 2);

    label = 0;
    xin_len = -1;

    printf("Classifying..\r\n");


    while(1)
    {
        /*
          Every 10ms, a timer triggers to fill the IMU data into data buffer.
        */
        if(s_u8TimeUpFlag)
        {
            //s_u8CopygsensorData = mainclassify.FillSensorData();


            MPU6500_readXYZ_mg(fBuff);
            //printf("%d,%d,%d\r\n", (int)fBuff[0], (int)fBuff[1], (int)fBuff[2]);
            x_ready = (SVM_DataBufferUpdateFeature(&acc_x, (fBuff[0])));
            y_ready = (SVM_DataBufferUpdateFeature(&acc_y, (fBuff[1])));
            z_ready = (SVM_DataBufferUpdateFeature(&acc_z, (fBuff[2])));


            if(x_ready&&y_ready&&z_ready)
            {
                xin_len++;
                one_imu_svmnode[0].value = acc_x.stat.output_amplitude/100;
                one_imu_svmnode[1].value = acc_x.stat.output_variance*10000;
                one_imu_svmnode[2].value = acc_y.stat.output_amplitude/100;
                one_imu_svmnode[3].value = acc_y.stat.output_variance*10000;
                one_imu_svmnode[4].value = acc_z.stat.output_amplitude/100;
                one_imu_svmnode[5].value = acc_z.stat.output_variance*10000;

#ifdef LOG_SVMFEATURE                
                printf("svm_feature:");
                for(int i=0; i<6; i++)
                {
                    printf("%f,", one_imu_svmnode[i].value);
                }
                printf("\n");
#endif                
                /*
                   find max ,min of 6 features for svm scaling
                */
                for(int i=0; i<6; i++)
                {
                    if(u8Model_Ready==0)
                    {
                        //printf("%f, ",one_imu_svmnode[i].value);
                        if(one_imu_svmnode[i].value >= svm_scale_max[i])  svm_scale_max[i] = one_imu_svmnode[i].value ;
                        if(one_imu_svmnode[i].value <= svm_scale_min[i])  svm_scale_min[i] = one_imu_svmnode[i].value ;
                    }
                }
                //printf("find min, max of 6 features simple b-sort\n");

                //printf("xin_len:%d  ", xin_len);
                //printf("Start Pattern Log\r\n");
                if(xin_len<=(PROB_X_DATAIN_NUMS/2))
                {
                    ILI9341_WriteString(0, 3*26, "  Get class 1 Data  ", Font_16x26, ILI9341_WHITE, ILI9341_BLACK);
                    train_feed_prob(&one_imu_svmnode[0], xin_len, 0);
                }
                while(!s_u8NetPatternFlag && !(xin_len<=(PROB_X_DATAIN_NUMS/2)))
                {
                    //printf("Press Button for Next Pattern\r\n");
                    ILI9341_WriteString(0, 3*26+3*16, "Press Button for Next Pattern", Font_16x26, ILI9341_GREEN, ILI9341_BLACK);
                }

                if(xin_len>(PROB_X_DATAIN_NUMS/2) &&  xin_len<(PROB_X_DATAIN_NUMS))
                {

                    ILI9341_WriteString(0, 3*26, " Get class  2 Data ", Font_16x26, ILI9341_WHITE, ILI9341_BLUE);
                    ILI9341_WriteString(0, 3*26+3*16, "                                        ", Font_16x26, ILI9341_GREEN, ILI9341_BLACK);
                    train_feed_prob(&one_imu_svmnode[0], xin_len, 1);
                }
                //else
                if(xin_len>=(PROB_X_DATAIN_NUMS))
                {
                    //Do scaling
                    //output(index, value);
                    if(u8Model_Ready==0)
                    {
                        svm_scale_prob((xin_len-1), svm_scale_max, svm_scale_min);
                        svm_train_start(NULL, 0);
                        u8Model_Ready=1;

                        ILI9341_WriteString(0, 3*26+3*16, "Training...                             ", Font_16x26, ILI9341_RED, ILI9341_BLACK);
                    }
                    else//Model Train done
                    {
                        train_feed_prob(&one_imu_svmnode[0], 0, 1);
                        svm_scale_prob( 1, svm_scale_max, svm_scale_min);
                        svm_predict_single(0, svm_problem);

                        ILI9341_WriteString(0, 3*26, "   Training Done    ", Font_16x26, ILI9341_WHITE, ILI9341_BLUE);


                        if(svm_problem[0]>0.8)
                        {
                            ILI9341_WriteString(0, 3*26+3*16, "Predicting... C1                      ", Font_16x26, ILI9341_GREEN, ILI9341_BLACK);
                        }
                        else if(svm_problem[1]>0.8)
                        {
                            ILI9341_WriteString(0, 3*26+3*16, "Predicting... C2                      ", Font_16x26, ILI9341_GREEN, ILI9341_BLACK);
                        }
                        else
                        {
                            ILI9341_WriteString(0, 3*26+3*16, "Predicting...                         ", Font_16x26, ILI9341_GREEN, ILI9341_BLACK);
                        }
                    }

                    //while(1);
                }
            }  //if(x_ready&&y_ready&&z_ready)
            s_u8TimeUpFlag = 0;
        }

    }//while(1)
}
