/******************************************************************************
 * @file     main.c
 * @version  V3.00
 * @brief    Configure USCI_SPI0 as Master mode and demonstrate how to communicate with an off-chip SPI Slave device.
 *           This sample code needs to work with USCI_SPI_SlaveMode sample code.
 *
 * @copyright SPDX-License-Identifier: Apache-2.0
 * @copyright Copyright (C) 2021 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/
#include <stdio.h>
#include "NuMicro.h"

#define TEST_COUNT  16

static uint32_t s_au32SourceData[TEST_COUNT];
static uint32_t s_au32DestinationData[TEST_COUNT];
static volatile uint32_t s_u32TxDataCount;
static volatile uint32_t s_u32RxDataCount;

/* Function prototype declaration */
void SYS_Init(void);
void USCI_SPI_Init(void);
void USCI0_IRQHandler(void);

/* ------------- */
/* Main function */
/* ------------- */
int main()
{
    uint32_t u32DataCount;

    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Init System, IP clock and multi-function I/O. */
    SYS_Init();

    /* Lock protected registers */
    SYS_LockReg();

    /* Configure UART0: 115200, 8-bit word, no parity bit, 1 stop bit. */
    UART_Open(UART0, 115200);

    /* Init USCI_SPI0 */
    USCI_SPI_Init();

    printf("\n\n");
    printf("+--------------------------------------------------------+\n");
    printf("|            USCI_SPI Master Mode Sample Code            |\n");
    printf("+--------------------------------------------------------+\n");
    printf("\n");
    printf("Configure USCI_SPI0 as a master.\n");
    printf("Bit length of a transaction: 16\n");
    printf("The I/O connection for USCI_SPI0:\n");
    printf("    USCI_SPI0_SS (PD.4)\n    USCI_SPI0_CLK (PD.0)\n");
    printf("    USCI_SPI0_MISO (PD.2)\n    USCI_SPI0_MOSI (PD.1)\n\n");
    printf("USCI_SPI controller will transfer %d data to a off-chip slave device.\n", TEST_COUNT);
    printf("In the meanwhile the USCI_SPI controller will receive %d data from the off-chip slave device.\n", TEST_COUNT);
    printf("After the transfer is done, the %d received data will be printed out.\n", TEST_COUNT);
    printf("The USCI_SPI master configuration is ready.\n");

    for(u32DataCount = 0; u32DataCount < TEST_COUNT; u32DataCount++)
    {
        /* Write the initial value to source buffer */
        s_au32SourceData[u32DataCount] = 0x5500 + u32DataCount;
        /* Clear destination buffer */
        s_au32DestinationData[u32DataCount] = 0;
    }

    printf("Before starting the data transfer, make sure the slave device is ready. Press any key to start the transfer.");
    getchar();
    printf("\n");

    /* Enable TX end interrupt */
    USPI_EnableInt(USPI0, USPI_TXEND_INT_MASK);
    s_u32TxDataCount = 0;
    s_u32RxDataCount = 0;
    NVIC_EnableIRQ(USCI0_IRQn);

    /* Write to TX Buffer */
    USPI_WRITE_TX(USPI0, s_au32SourceData[s_u32TxDataCount++]);

    /* Wait for transfer done */
    while(s_u32RxDataCount < TEST_COUNT);

    /* Print the received data */
    printf("Received data:\n");
    for(u32DataCount = 0; u32DataCount < TEST_COUNT; u32DataCount++)
    {
        printf("%d:\t0x%X\n", u32DataCount, s_au32DestinationData[u32DataCount]);
    }
    /* Disable TX end interrupt */
    USPI_DisableInt(USPI0, USPI_TXEND_INT_MASK);
    NVIC_DisableIRQ(USCI0_IRQn);
    printf("The data transfer was done.\n");

    printf("\n\nExit USCI_SPI driver sample code.\n");

    /* Close USCI_SPI0 */
    USPI_Close(USPI0);
    while(1);
}

void SYS_Init(void)
{
    /* Set PF multi-function pins for XT1_OUT(PF.2) and XT1_IN(PF.3) */
    SET_XT1_OUT_PF2();
    SET_XT1_IN_PF3();

    /* Set PF multi-function pins for X32_OUT(PF.4) and X32_IN(PF.5) */
    SET_X32_OUT_PF4();
    SET_X32_IN_PF5();

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/

    /* Enable HIRC, HXT and LXT clock */
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk | CLK_PWRCTL_HXTEN_Msk | CLK_PWRCTL_LXTEN_Msk);

    /* Wait for HIRC, HXT and LXT clock ready */
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk | CLK_STATUS_HXTSTB_Msk | CLK_STATUS_LXTSTB_Msk);

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

    /* Enable USCI0 module clock */
    CLK_EnableModuleClock(USCI0_MODULE);

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/

    /* Set multi-function pins for UART0 RXD and TXD */
    SET_UART0_RXD_PB12();
    SET_UART0_TXD_PB13();

    /* Set USCI0_SPI multi-function pins */
    SET_USCI0_CLK_PD0();
    SET_USCI0_DAT0_PD1();
    SET_USCI0_DAT1_PD2();
    SET_USCI0_CTL0_PD4();

    /* USCI_SPI clock pin enable schmitt trigger */
    PD->SMTEN |= GPIO_SMTEN_SMTEN0_Msk;
}

void USCI_SPI_Init(void)
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init USCI_SPI0                                                                                          */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Configure USCI_SPI0 as a master, USCI_SPI0 clock rate 2 MHz,
       clock idle low, 16-bit transaction, drive output on falling clock edge and latch input on rising edge. */
    USPI_Open(USPI0, USPI_MASTER, USPI_MODE_0, 16, 2000000);
    /* Enable the automatic hardware slave selection function and configure USCI_SPI_SS pin as low-active. */
    USPI_EnableAutoSS(USPI0, 0, USPI_SS_ACTIVE_LOW);
}

void USCI0_IRQHandler(void)
{
    uint32_t u32RxData;

    /* Clear TX end interrupt flag */
    USPI_CLR_PROT_INT_FLAG(USPI0, USPI_PROTSTS_TXENDIF_Msk);

    /* Waiting for RX is not empty */
    while(USPI_GET_RX_EMPTY_FLAG(USPI0) == 1);

    /* Check RX EMPTY flag */
    while(USPI_GET_RX_EMPTY_FLAG(USPI0) == 0)
    {
        /* Read RX Buffer */
        u32RxData = USPI_READ_RX(USPI0);
        s_au32DestinationData[s_u32RxDataCount++] = u32RxData;
    }
    /* Check TX data count */
    if(s_u32TxDataCount < TEST_COUNT)
    {
        /* Write to TX Buffer */
        USPI_WRITE_TX(USPI0, s_au32SourceData[s_u32TxDataCount++]);
    }
}
