//
// Includes
//
#include "audio.h"

//
// Define
//
#define AUDIO_MUTE()        PD3 = 1
#define AUDIO_UNMUTE()      PD3 = 0

//
// Variable
//
extern const uint16_t sound[];
uint32_t sound_size = 20 * 1024;
uint32_t sound_idx = 0;

//
// Prototype
//
static void nau8822_write_reg(uint8_t u8Addr, uint16_t u16Data);
static void nau8822_setup(void);
static void nau8822_configSampleRate(uint32_t u32SampleRate);

//
// Functions
//

//
// audio_low_level_init
//
void audio_low_level_init(void)
{
    //
    // I2S
    //
    SET_I2S0_BCLK_PI6();
    SET_I2S0_MCLK_PI7();
    SET_I2S0_DI_PI8();
    SET_I2S0_DO_PI9();
    SET_I2S0_LRCK_PI10();
    
	I2S_Open(I2S0, I2S_MODE_SLAVE, 16000, I2S_DATABIT_16, I2S_STEREO, I2S_FORMAT_I2S);
    
     //
    // JKEN#
    //
    SYS->GPD_MFP0 &= ~(SYS_GPD_MFP0_PD3MFP_Msk);
    GPIO_SetMode(PD, BIT3, GPIO_MODE_OUTPUT);
    PD3 = 0; 

    //
    // I2C
    //
    SET_I2C2_SDA_PD0();
    SET_I2C2_SCL_PD1();
    
    I2C_Open(I2C2, 100000);
}   

//
// audio_statup_sequence
//
void audio_statup_sequence(void)
{
	nau8822_setup();
    nau8822_configSampleRate(44100);
}

//
// audio_start_play
//
void audio_start_play(void)
{
    printf("Start to play...\r\n");
    
    I2S_EnableMCLK(I2S0, 12000000);
    I2S0->CTL0 |= I2S_CTL0_ORDER_Msk;
    
    PDMA_Open(PDMA0, 1 << 2);
    PDMA_SetTransferCnt(PDMA0, 2, PDMA_WIDTH_32, sound_size);
    PDMA_SetTransferAddr(PDMA0, 2, (uint32_t)&sound, PDMA_SAR_INC, (uint32_t)&I2S0->TXFIFO, PDMA_DAR_FIX);
    PDMA_SetTransferMode(PDMA0, 2, PDMA_I2S0_TX, FALSE, 0);
    PDMA_SetBurstType(PDMA0, 2, PDMA_REQ_SINGLE, PDMA_BURST_1);
    PDMA_EnableInt(PDMA0, 2, PDMA_INT_TRANS_DONE);    
    NVIC_EnableIRQ(PDMA0_IRQn);
    
    I2S_ENABLE_TXDMA(I2S0);
    I2S_ENABLE_TX(I2S0);
}

//
// audio_setup_sample_rate
//
void audio_setup_sample_rate(uint32_t rate)
{
	nau8822_configSampleRate(rate);
}

//
// nau8822_configSampleRate
//
static void nau8822_configSampleRate(uint32_t u32SampleRate)
{
    printf("Configure Sampling Rate to %d\n", u32SampleRate);

    if((u32SampleRate % 8) == 0)
    {
        nau8822_write_reg(36, 0x008);    //12.288Mhz
        nau8822_write_reg(37, 0x00C);
        nau8822_write_reg(38, 0x093);
        nau8822_write_reg(39, 0x0E9);
    }
    else
    {
        nau8822_write_reg(36, 0x007);    //11.2896Mhz
        nau8822_write_reg(37, 0x021);
        nau8822_write_reg(38, 0x161);
        nau8822_write_reg(39, 0x026);
    }

    switch(u32SampleRate)
    {
        case 16000:
            nau8822_write_reg(6, 0x1AD);    /* Divide by 6, 16K */
            nau8822_write_reg(7, 0x006);    /* 16K for internal filter coefficients */
            break;

        case 44100:
            nau8822_write_reg(6, 0x14D);    /* Divide by 2, 48K */
            nau8822_write_reg(7, 0x000);    /* 48K for internal filter coefficients */
            break;

        case 48000:
            nau8822_write_reg(6, 0x14D);    /* Divide by 2, 48K */
            nau8822_write_reg(7, 0x000);    /* 48K for internal filter coefficients */
            break;

        case 96000:
            nau8822_write_reg(6, 0x109);    /* Divide by 1, 96K */
            nau8822_write_reg(72, 0x013);
            break;
    }
}

//
// nau8822_setup
//
static void nau8822_setup(void)
{
    nau8822_write_reg(0,  0x000);   /* Reset all registers */
    CLK_SysTickDelay(10000);

    nau8822_write_reg(1,  0x02F);
    nau8822_write_reg(2,  0x1B3);   /* Enable L/R Headphone, ADC Mix/Boost, ADC */
    nau8822_write_reg(3,  0x07F);   /* Enable L/R main mixer, DAC */
    nau8822_write_reg(4,  0x010);   /* 16-bit word length, I2S format, Stereo */
    nau8822_write_reg(5,  0x000);   /* Companding control and loop back mode (all disable) */
    nau8822_write_reg(6,  0x14D);   /* Divide by 2, 48K */
    nau8822_write_reg(7,  0x000);   /* 48K for internal filter coefficients */
    nau8822_write_reg(10, 0x008);   /* DAC soft mute is disabled, DAC oversampling rate is 128x */
    nau8822_write_reg(14, 0x108);   /* ADC HP filter is disabled, ADC oversampling rate is 128x */
    nau8822_write_reg(15, 0x1EF);   /* ADC left digital volume control */
    nau8822_write_reg(16, 0x1EF);   /* ADC right digital volume control */

    nau8822_write_reg(44, 0x000);   /* LLIN/RLIN is not connected to PGA */
    nau8822_write_reg(47, 0x050);   /* LLIN connected, and its Gain value */
    nau8822_write_reg(48, 0x050);   /* RLIN connected, and its Gain value */
    nau8822_write_reg(50, 0x001);   /* Left DAC connected to LMIX */
    nau8822_write_reg(51, 0x001);   /* Right DAC connected to RMIX */
}

//
// nau8822_write_reg
//
static void nau8822_write_reg(uint8_t u8Addr, uint16_t u16Data)
{
    I2C_START(I2C2);
    I2C_WAIT_READY(I2C2);

    I2C_SET_DATA(I2C2, 0x1A << 1);
    I2C_SET_CONTROL_REG(I2C2, I2C_CTL_SI);
    I2C_WAIT_READY(I2C2);

    I2C_SET_DATA(I2C2, (uint8_t)((u8Addr << 1) | (u16Data >> 8)));
    I2C_SET_CONTROL_REG(I2C2, I2C_CTL_SI);
    I2C_WAIT_READY(I2C2);

    I2C_SET_DATA(I2C2, (uint8_t)(u16Data & 0x00FF));
    I2C_SET_CONTROL_REG(I2C2, I2C_CTL_SI);
    I2C_WAIT_READY(I2C2);

    I2C_STOP(I2C2);
}

//
// PDMA0_IRQHandler
//
void PDMA0_IRQHandler(void)
{
    uint32_t u32Status = PDMA_GET_INT_STATUS(PDMA0);

    if(u32Status & 0x2)    /* done */
    {
        if(PDMA_GET_TD_STS(PDMA0) & 0x4) {
            I2S_DISABLE_TX(I2S0);
            I2S_DISABLE_TXDMA(I2S0);
            
            PDMA_Open(PDMA0, 1 << 2);
            PDMA_SetTransferCnt(PDMA0, 2, PDMA_WIDTH_32, sound_size);
            PDMA_SetTransferAddr(PDMA0, 2, (uint32_t)&sound, PDMA_SAR_INC, (uint32_t)&I2S0->TXFIFO, PDMA_DAR_FIX);
            PDMA_SetTransferMode(PDMA0, 2, PDMA_I2S0_TX, FALSE, 0);
            PDMA_SetBurstType(PDMA0, 2, PDMA_REQ_SINGLE, PDMA_BURST_1);
            PDMA_EnableInt(PDMA0, 2, PDMA_INT_TRANS_DONE);   
            
            I2S_ENABLE_TXDMA(I2S0);
            I2S_ENABLE_TX(I2S0);
        }
        
        PDMA_CLR_TD_FLAG(PDMA0, PDMA_TDSTS_TDIF2_Msk);
    }
    else if(u32Status & 0x400)   /* Timeout */
    {
        PDMA_CLR_TMOUT_FLAG(PDMA0, PDMA_TDSTS_TDIF2_Msk);
    }
}
