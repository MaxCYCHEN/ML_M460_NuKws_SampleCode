//
// Includes
//
#include "spi_flash.h"
#include <string.h>

//
// Defines
//
//#define FLASH_BLOCK_SIZE        0x10000     // 64KB
//#define FLASH_TOTAL_SIZE        0x00400000  // 4MB = 32Mb
//#define BUFFER_SIZE             2048
//#define TEST_PATTERN            0x5A5A5A5A

#define TEST_NUMBER 1   /* page numbers */
#define TEST_LENGTH 256 /* length */
#define SPI_FLASH_PORT QSPI0

//
// Variable
//
static uint8_t s_au8SrcArray[TEST_LENGTH];
static uint8_t s_au8DestArray[TEST_LENGTH];

//
// Functions
//

//
// spi_flash_low_level_init
//
void spi_flash_low_level_init(void)
{
    printf("Initialize SPI flash...\r\n");
    
    SET_QSPI0_MOSI0_PJ1();
    SET_QSPI0_MISO0_PJ0();
    SET_QSPI0_CLK_PI15();
    SET_QSPI0_SS_PI14();
    
    /* Enable QSPI0 clock pin (PI15) schmitt trigger */
    PI->SMTEN |= GPIO_SMTEN_SMTEN15_Msk;
    /* Enable QSPI0 I/O high slew rate */
    GPIO_SetSlewCtl(PI, BIT12|BIT13|BIT14|BIT15, GPIO_SLEWCTL_HIGH);
    GPIO_SetSlewCtl(PJ, BIT0|BIT1, GPIO_SLEWCTL_HIGH);
    
}
void D2D3_SwitchToNormalMode(void)
{
    SYS->GPI_MFP3 =  SYS->GPI_MFP3 & ~(SYS_GPI_MFP3_PI13MFP_Msk | SYS_GPI_MFP3_PI12MFP_Msk);
    GPIO_SetMode(PI, BIT13, GPIO_MODE_OUTPUT);
    GPIO_SetMode(PI, BIT12, GPIO_MODE_OUTPUT);
    PI13 = 1;
    PI12 = 1;
}

void D2D3_SwitchToQuadMode(void)
{
    SET_QSPI0_MOSI1_PI13();
    SET_QSPI0_MISO1_PI12();
}

__STATIC_INLINE void wait_QSPI_IS_BUSY(QSPI_T *qspi)
{
    uint32_t u32TimeOutCnt = SystemCoreClock; /* 1 second time-out */

    while(QSPI_IS_BUSY(qspi))
    {
        if(--u32TimeOutCnt == 0)
        {
            printf("Wait for QSPI time-out!\n");
            break;
        }
    }
}

uint16_t SpiFlash_ReadMidDid(void)
{
    uint8_t u8RxData[6], u8IDCnt = 0;

    // /CS: active
    QSPI_SET_SS_LOW(SPI_FLASH_PORT);

    // send Command: 0x90, Read Manufacturer/Device ID
    QSPI_WRITE_TX(SPI_FLASH_PORT, 0x90);

    // send 24-bit '0', dummy
    QSPI_WRITE_TX(SPI_FLASH_PORT, 0x00);
    QSPI_WRITE_TX(SPI_FLASH_PORT, 0x00);
    QSPI_WRITE_TX(SPI_FLASH_PORT, 0x00);

    // receive 16-bit
    QSPI_WRITE_TX(SPI_FLASH_PORT, 0x00);
    QSPI_WRITE_TX(SPI_FLASH_PORT, 0x00);

    // wait tx finish
    wait_QSPI_IS_BUSY(SPI_FLASH_PORT);

    // /CS: de-active
    QSPI_SET_SS_HIGH(SPI_FLASH_PORT);

    while(!QSPI_GET_RX_FIFO_EMPTY_FLAG(SPI_FLASH_PORT))
        u8RxData[u8IDCnt ++] = (uint8_t)QSPI_READ_RX(SPI_FLASH_PORT);

    return (uint16_t)((u8RxData[4] << 8) | u8RxData[5]);
}

void SpiFlash_ChipErase(void)
{
    // /CS: active
    QSPI_SET_SS_LOW(SPI_FLASH_PORT);

    // send Command: 0x06, Write enable
    QSPI_WRITE_TX(SPI_FLASH_PORT, 0x06);

    // wait tx finish
    wait_QSPI_IS_BUSY(SPI_FLASH_PORT);

    // /CS: de-active
    QSPI_SET_SS_HIGH(SPI_FLASH_PORT);

    //////////////////////////////////////////

    // /CS: active
    QSPI_SET_SS_LOW(SPI_FLASH_PORT);

    // send Command: 0xC7, Chip Erase
    QSPI_WRITE_TX(SPI_FLASH_PORT, 0xC7);

    // wait tx finish
    wait_QSPI_IS_BUSY(SPI_FLASH_PORT);

    // /CS: de-active
    QSPI_SET_SS_HIGH(SPI_FLASH_PORT);

    QSPI_ClearRxFIFO(SPI_FLASH_PORT);
}

uint8_t SpiFlash_ReadStatusReg(void)
{
    uint8_t u8Val;

    QSPI_ClearRxFIFO(SPI_FLASH_PORT);

    // /CS: active
    QSPI_SET_SS_LOW(SPI_FLASH_PORT);

    // send Command: 0x05, Read status register
    QSPI_WRITE_TX(SPI_FLASH_PORT, 0x05);

    // read status
    QSPI_WRITE_TX(SPI_FLASH_PORT, 0x00);

    // wait tx finish
    wait_QSPI_IS_BUSY(SPI_FLASH_PORT);

    // /CS: de-active
    QSPI_SET_SS_HIGH(SPI_FLASH_PORT);

    // skip first rx data
    u8Val = (uint8_t)QSPI_READ_RX(SPI_FLASH_PORT);
    u8Val = (uint8_t)QSPI_READ_RX(SPI_FLASH_PORT);

    return u8Val;
}

uint8_t SpiFlash_ReadStatusReg2(void)
{
    uint8_t u8Val;

    QSPI_ClearRxFIFO(SPI_FLASH_PORT);

    // /CS: active
    QSPI_SET_SS_LOW(SPI_FLASH_PORT);

    // send Command: 0x35, Read status register
    QSPI_WRITE_TX(SPI_FLASH_PORT, 0x35);

    // read status
    QSPI_WRITE_TX(SPI_FLASH_PORT, 0x00);

    // wait tx finish
    wait_QSPI_IS_BUSY(SPI_FLASH_PORT);

    // /CS: de-active
    QSPI_SET_SS_HIGH(SPI_FLASH_PORT);

    // skip first rx data
    u8Val = (uint8_t)QSPI_READ_RX(SPI_FLASH_PORT);
    u8Val = (uint8_t)QSPI_READ_RX(SPI_FLASH_PORT);

    return u8Val;
}

void SpiFlash_WriteStatusReg(uint8_t u8Value1, uint8_t u8Value2)
{
    // /CS: active
    QSPI_SET_SS_LOW(SPI_FLASH_PORT);

    // send Command: 0x06, Write enable
    QSPI_WRITE_TX(SPI_FLASH_PORT, 0x06);

    // wait tx finish
    wait_QSPI_IS_BUSY(SPI_FLASH_PORT);

    // /CS: de-active
    QSPI_SET_SS_HIGH(SPI_FLASH_PORT);

    ///////////////////////////////////////

    // /CS: active
    QSPI_SET_SS_LOW(SPI_FLASH_PORT);

    // send Command: 0x01, Write status register
    QSPI_WRITE_TX(SPI_FLASH_PORT, 0x01);

    // write status
    QSPI_WRITE_TX(SPI_FLASH_PORT, u8Value1);
    QSPI_WRITE_TX(SPI_FLASH_PORT, u8Value2);

    // wait tx finish
    wait_QSPI_IS_BUSY(SPI_FLASH_PORT);

    // /CS: de-active
    QSPI_SET_SS_HIGH(SPI_FLASH_PORT);
}

int32_t SpiFlash_WaitReady(void)
{
    volatile uint8_t u8ReturnValue;
    uint32_t u32TimeOutCnt = SystemCoreClock; /* 1 second time-out */

    do
    {
        if(--u32TimeOutCnt == 0)
        {
            printf("Wait for QSPI time-out!\n");
            return -1;
        }

        u8ReturnValue = SpiFlash_ReadStatusReg();
        u8ReturnValue = u8ReturnValue & 1;
    }
    while(u8ReturnValue != 0); // check the BUSY bit

    return 0;
}

void SpiFlash_NormalPageProgram(uint32_t u32StartAddress, uint8_t *u8DataBuffer)
{
    uint32_t u32Cnt = 0;

    // /CS: active
    QSPI_SET_SS_LOW(SPI_FLASH_PORT);

    // send Command: 0x06, Write enable
    QSPI_WRITE_TX(SPI_FLASH_PORT, 0x06);

    // wait tx finish
    wait_QSPI_IS_BUSY(SPI_FLASH_PORT);

    // /CS: de-active
    QSPI_SET_SS_HIGH(SPI_FLASH_PORT);


    // /CS: active
    QSPI_SET_SS_LOW(SPI_FLASH_PORT);

    // send Command: 0x02, Page program
    QSPI_WRITE_TX(SPI_FLASH_PORT, 0x02);

    // send 24-bit start address
    QSPI_WRITE_TX(SPI_FLASH_PORT, (u32StartAddress >> 16) & 0xFF);
    QSPI_WRITE_TX(SPI_FLASH_PORT, (u32StartAddress >> 8)  & 0xFF);
    QSPI_WRITE_TX(SPI_FLASH_PORT, u32StartAddress       & 0xFF);

    // write data
    while(1)
    {
        if(!QSPI_GET_TX_FIFO_FULL_FLAG(SPI_FLASH_PORT))
        {
            QSPI_WRITE_TX(SPI_FLASH_PORT, u8DataBuffer[u32Cnt++]);
            if(u32Cnt > 255) break;
        }
    }

    // wait tx finish
    wait_QSPI_IS_BUSY(SPI_FLASH_PORT);

    // /CS: de-active
    QSPI_SET_SS_HIGH(SPI_FLASH_PORT);

    QSPI_ClearRxFIFO(SPI_FLASH_PORT);
}

void spiFlash_EnableQEBit(void)
{
    uint8_t u8Status1 = SpiFlash_ReadStatusReg();
    uint8_t u8Status2 = SpiFlash_ReadStatusReg2();

    u8Status2 |= 0x2;
    SpiFlash_WriteStatusReg(u8Status1, u8Status2);
    SpiFlash_WaitReady();
}

void spiFlash_DisableQEBit(void)
{
    uint8_t u8Status1 = SpiFlash_ReadStatusReg();
    uint8_t u8Status2 = SpiFlash_ReadStatusReg2();

    u8Status2 &= ~0x2;
    SpiFlash_WriteStatusReg(u8Status1, u8Status2);
    SpiFlash_WaitReady();
}

void SpiFlash_QuadFastRead(uint32_t u32StartAddress, uint8_t *u8DataBuffer)
{
    uint32_t u32Cnt;

    // enable quad mode
    spiFlash_EnableQEBit();

    // /CS: active
    QSPI_SET_SS_LOW(SPI_FLASH_PORT);

    // Command: 0xEB, Fast Read quad data
    QSPI_WRITE_TX(SPI_FLASH_PORT, 0xEB);
    wait_QSPI_IS_BUSY(SPI_FLASH_PORT);

    // enable SPI quad IO mode and set direction to input
    D2D3_SwitchToQuadMode();
    QSPI_ENABLE_QUAD_OUTPUT_MODE(SPI_FLASH_PORT);

    // send 24-bit start address
    QSPI_WRITE_TX(SPI_FLASH_PORT, (u32StartAddress >> 16) & 0xFF);
    QSPI_WRITE_TX(SPI_FLASH_PORT, (u32StartAddress >> 8)  & 0xFF);
    QSPI_WRITE_TX(SPI_FLASH_PORT, u32StartAddress       & 0xFF);

    // dummy byte
    QSPI_WRITE_TX(SPI_FLASH_PORT, 0x00);
    QSPI_WRITE_TX(SPI_FLASH_PORT, 0x00);
    QSPI_WRITE_TX(SPI_FLASH_PORT, 0x00);

    wait_QSPI_IS_BUSY(SPI_FLASH_PORT);
    QSPI_ENABLE_QUAD_INPUT_MODE(SPI_FLASH_PORT);

    // clear RX buffer
    QSPI_ClearRxFIFO(SPI_FLASH_PORT);

    // read data
    for(u32Cnt = 0; u32Cnt < 256; u32Cnt++)
    {
        QSPI_WRITE_TX(SPI_FLASH_PORT, 0x00);
        wait_QSPI_IS_BUSY(SPI_FLASH_PORT);
        u8DataBuffer[u32Cnt] = (uint8_t)QSPI_READ_RX(SPI_FLASH_PORT);
    }

    // wait tx finish
    wait_QSPI_IS_BUSY(SPI_FLASH_PORT);

    // /CS: de-active
    QSPI_SET_SS_HIGH(SPI_FLASH_PORT);

    QSPI_DISABLE_QUAD_MODE(SPI_FLASH_PORT);
    D2D3_SwitchToNormalMode();

    // disable quad mode
    spiFlash_DisableQEBit();
}

//
// spi_flash_rw_test
//
int8_t spi_flash_rw_test(void)
{
    uint32_t u32ByteCount, u32FlashAddress, u32PageNumber;
    uint16_t u16ID;
    
    D2D3_SwitchToNormalMode();

    /* Configure SPI_FLASH_PORT as a master, MSB first, 8-bit transaction, QSPI Mode-0 timing, clock is 2MHz */
    QSPI_Open(SPI_FLASH_PORT, QSPI_MASTER, QSPI_MODE_0, 8, 2000000);

    /* Disable auto SS function, control SS signal manually. */
    QSPI_DisableAutoSS(SPI_FLASH_PORT);

    if((u16ID = SpiFlash_ReadMidDid()) != 0xEF15)
    {
        printf("Wrong ID, 0x%x\n", u16ID);
        return -1;
    }

//    printf("Erase chip ...");

//    /* Erase SPI flash */
    SpiFlash_ChipErase();

    /* Wait ready */
    if( SpiFlash_WaitReady() < 0 ) return -1;

    printf("[OK]\n");

    /* init source data buffer */
    for(u32ByteCount = 0; u32ByteCount < TEST_LENGTH; u32ByteCount++)
    {
        s_au8SrcArray[u32ByteCount] = (uint8_t)u32ByteCount;
    }

    printf("Start to write data to Flash ...");
    /* Program SPI flash */
    u32FlashAddress = 0;
    for(u32PageNumber = 0; u32PageNumber < TEST_NUMBER; u32PageNumber++)
    {
        /* page program */
        SpiFlash_NormalPageProgram(u32FlashAddress, s_au8SrcArray);
        if( SpiFlash_WaitReady() < 0 ) return -1;
        u32FlashAddress += 0x100;
    }

    printf("[OK]\n");

    /* clear destination data buffer */
    for(u32ByteCount = 0; u32ByteCount < TEST_LENGTH; u32ByteCount++)
    {
        s_au8DestArray[u32ByteCount] = 0;
    }

    printf("Read & Compare ...");

    /* Read SPI flash */
    u32FlashAddress = 0;
    for(u32PageNumber = 0; u32PageNumber < TEST_NUMBER; u32PageNumber++)
    {
        /* page read */
        SpiFlash_QuadFastRead(u32FlashAddress, s_au8DestArray);
        u32FlashAddress += 0x100;

        for(u32ByteCount = 0; u32ByteCount < TEST_LENGTH; u32ByteCount++)
        {
            if(s_au8DestArray[u32ByteCount] != s_au8SrcArray[u32ByteCount])
                return -1;
        }
    }
    printf("<done>\n");
    
    return 0;
}
