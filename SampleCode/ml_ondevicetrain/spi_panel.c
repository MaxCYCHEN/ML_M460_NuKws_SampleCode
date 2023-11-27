#include "spi_panel.h"





//extern MainClassify mainclassify;
void _write_command(uint8_t temp)
{
    GPIO_LCM_DC = 0;
    GPIO_SPI_SS = 0;

    SPI_WRITE_TX(SPI_PANEL_PORT, temp);
    while(SPI_IS_BUSY(SPI_PANEL_PORT));

    GPIO_SPI_SS = 1;
}

void _write_data(uint8_t temp)
{
    GPIO_LCM_DC = 1;
    GPIO_SPI_SS = 0;

    SPI_WRITE_TX(SPI_PANEL_PORT, temp);

    while(SPI_IS_BUSY(SPI_PANEL_PORT));
    GPIO_SPI_SS = 1;
}


void _poll_spi(uint8_t temp)
{
    SPI_WRITE_TX(SPI_PANEL_PORT, temp);

    while(SPI_IS_BUSY(SPI_PANEL_PORT));
    
}


void ili9341_init(void)
{
    GPIO_LCM_DC = 0;
    GPIO_LCM_RST = 0;
    ILI9341_LED = 0;

    GPIO_LCM_DC = 1;
    GPIO_LCM_RST = 0;
    CLK_SysTickDelay(20000);
    GPIO_LCM_RST = 1;
    CLK_SysTickDelay(40000);


    _write_command(0xCF);
    _write_data(0x00);
    _write_data(0xD9);
    _write_data(0X30);

    _write_command(0xED);
    _write_data(0x64);
    _write_data(0x03);
    _write_data(0X12);
    _write_data(0X81);

    _write_command(0xE8);
    _write_data(0x85);
    _write_data(0x10);
    _write_data(0x78);

    _write_command(0xCB);
    _write_data(0x39);
    _write_data(0x2C);
    _write_data(0x00);
    _write_data(0x34);
    _write_data(0x02);

    _write_command(0xF7);
    _write_data(0x20);

    _write_command(0xEA);
    _write_data(0x00);
    _write_data(0x00);

    _write_command(0xC0);    //Power control
    _write_data(0x21);   //VRH[5:0]

    _write_command(0xC1);    //Power control
    _write_data(0x12);   //SAP[2:0];BT[3:0]

    _write_command(0xC5);    //VCM control
    _write_data(0x32);
    _write_data(0x3C);

    _write_command(0xC7);    //VCM control2
    _write_data(0XC1);

    _write_command(0x36);    // Memory Access Control
    _write_data(0xa8);
    //_write_data(0x68); //左右
    //_write_data(0x28);


    _write_command(0x3A);
    _write_data(0x55);

    _write_command(0xB1);
    _write_data(0x00);
    _write_data(0x18);

    _write_command(0xB6);    // Display Function Control
    _write_data(0x0A);
    _write_data(0xA2);



    _write_command(0xF2);    // 3Gamma Function Disable
    _write_data(0x00);

    _write_command(0x26);    //Gamma curve selected
    _write_data(0x01);

    _write_command(0xE0);    //Set Gamma
    _write_data(0x0F);
    _write_data(0x20);
    _write_data(0x1E);
    _write_data(0x09);
    _write_data(0x12);
    _write_data(0x0B);
    _write_data(0x50);
    _write_data(0XBA);
    _write_data(0x44);
    _write_data(0x09);
    _write_data(0x14);
    _write_data(0x05);
    _write_data(0x23);
    _write_data(0x21);
    _write_data(0x00);

    _write_command(0XE1);    //Set Gamma
    _write_data(0x00);
    _write_data(0x19);
    _write_data(0x19);
    _write_data(0x00);
    _write_data(0x12);
    _write_data(0x07);
    _write_data(0x2D);
    _write_data(0x28);
    _write_data(0x3F);
    _write_data(0x02);
    _write_data(0x0A);
    _write_data(0x08);
    _write_data(0x25);
    _write_data(0x2D);
    _write_data(0x0F);

    _write_command(0x11);    //Exit Sleep
    CLK_SysTickDelay(120000);
    _write_command(0x29);    //Display on

    ILI9341_LED = 1;
}



void spi_panel_init(void)
{
//    SYS->GPA_MFPH &= ~(SYS_GPA_MFPH_PA8MFP_Msk | SYS_GPA_MFPH_PA9MFP_Msk |     \
//            SYS_GPA_MFPH_PA10MFP_Msk | SYS_GPA_MFPH_PA11MFP_Msk);
//    SYS->GPA_MFPH |= (SYS_GPA_MFPH_PA8MFP_SPI2_MOSI | SYS_GPA_MFPH_PA9MFP_SPI2_MISO |
//                     SYS_GPA_MFPH_PA10MFP_SPI2_CLK | SYS_GPA_MFPH_PA11MFP_GPIO);

    SET_SPI2_MOSI_PA8();
    SET_SPI2_MISO_PA9();
    SET_SPI2_CLK_PA10();
    //SET_SPI2_SS_PA11();

    /* Enable SPI0 clock pin (PA2) schmitt trigger */
    PA->SMTEN |= GPIO_SMTEN_SMTEN2_Msk;

    /* Enable SPI0 I/O high slew rate */
    GPIO_SetSlewCtl(PA, 0xF, GPIO_SLEWCTL_HIGH);

    // PA11、PB2、PB2、PB5(backlight) : drive NuTFT
    GPIO_SetMode(PA, BIT11, GPIO_MODE_OUTPUT);
    GPIO_SetMode(PB, (BIT2 | BIT3), GPIO_MODE_OUTPUT);
    //GPIO_SetMode(PE, BIT5, GPIO_MODE_OUTPUT);
    GPIO_SetMode(PB, BIT5, GPIO_MODE_OUTPUT);

    SPI_Open(SPI_PANEL_PORT, SPI_MASTER, SPI_MODE_0, 8, SPI_PANEL_FREQ);
    SPI_DisableAutoSS(SPI_PANEL_PORT);
    SPI_ENABLE(SPI_PANEL_PORT);

    ili9341_init();


}


void LCD_Set_Color(uint16_t color)
{
    _write_data(color >> 8);
    _write_data(color);
}

void _set_address(unsigned int x1, unsigned int x2, unsigned int y1, unsigned int y2)
{
    _write_command(0x2a);
    _write_data(x1 >> 8);
    _write_data(x1);
    _write_data(x2 >> 8);
    _write_data(x2);
    _write_command(0x2b);
    _write_data(y1 >> 8);
    _write_data(y1);
    _write_data(y2 >> 8);
    _write_data(y2);
    _write_command(0x2c);
}

void LCD_Set_Pos(void)
{
#if SWAPAXIS == ENABLE
    _set_address(0, (LCD_RES_Y - 1), 0, (LCD_RES_X - 1));
#else
    _set_address(0, (LCD_RES_X - 1), 0, (LCD_RES_Y - 1));
#endif
}

void spi_panel_show_to_LCD(uint8_t* pattern)   //testMing
{
    uint32_t i, j;
    uint32_t color, gcolor;
    volatile  uint16_t r,g,b,color_grayu16,rr,gg,bb;
    uint8_t  color_grays8 = 0x00;
    //volatile uint8_t *pic_addr = (uint8_t *)(SPIM_DMM_MAP_ADDR + IMAGE_BACKGROUND_INDEX);
    volatile uint8_t *pic_addr = (uint8_t *)(color_grays8);
    _set_address(0, (LCD_RES_Y - 1), 0, (LCD_RES_X - 1));
    for(j=0;j<320*240;j++)
    {
        color = *pic_addr;
        color |= *pic_addr<< 8;
            
        LCD_Set_Color(color);
    }             
    
    
}


//void spi_panel_show_to_LCD(uint8_t* pattern)   //testMing
//{
//    uint32_t i, j;
//    uint32_t color, gcolor;
//    volatile  uint16_t r,g,b,color_grayu16,rr,gg,bb;
//    volatile int8_t  color_grays8;
//    //volatile uint8_t *pic_addr = (uint8_t *)(SPIM_DMM_MAP_ADDR + IMAGE_BACKGROUND_INDEX);
//    volatile uint8_t *pic_addr = (uint8_t *)(pattern);
//    _set_address(0, (LCD_RES_Y - 1), 0, (LCD_RES_X - 1));
//    for(j=0;j<320*240;j++)
//    {
//        color = *pic_addr++;
//        color |= *pic_addr++ << 8;
//            
//        LCD_Set_Color(color);
//    }             
//    
//    
//}

