#ifndef	__SPI_PANEL_H__
#define __SPI_PANEL_H__

#include <stdio.h>
#include "NuMicro.h"

#define SWAPAXIS ENABLE

#define IMAGE_BACKGROUND_INDEX		0x00000000
#define IMAGE_BACKGROUND_SIZE		153600

#define IMAGE_DIGITAL_ONE_INDEX		0x00032000
#define IMAGE_DIGITAL_ONE_SIZE		20480

#define IMAGE_DIGITAL_TWO_INDEX		0x00042000
#define IMAGE_DIGITAL_TWO_SIZE		20480

#define IMAGE_DIGITAL_THREE_INDEX	0x00052000
#define IMAGE_DIGITAL_THREE_SIZE	20480

#define IMAGE_DIGITAL_FOUR_INDEX	0x00062000
#define IMAGE_DIGITAL_FOUR_SIZE		20480

#define IMAGE_DIGITAL_FIVE_INDEX	0x00072000
#define IMAGE_DIGITAL_FIVE_SIZE		20480

#define IMAGE_DIGITAL_SIX_INDEX		0x00082000
#define IMAGE_DIGITAL_SIX_SIZE		20480

#define IMAGE_DIGITAL_SEVEN_INDEX	0x00092000
#define IMAGE_DIGITAL_SEVEN_SIZE	20480

#define IMAGE_DIGITAL_EIGHT_INDEX	0x000A2000
#define IMAGE_DIGITAL_EIGHT_SIZE	20480

#define IMAGE_DIGITAL_NINE_INDEX	0x000B2000
#define IMAGE_DIGITAL_NINE_SIZE		20480

#define IMAGE_DIGITAL_ZERO_INDEX	0x000C2000
#define IMAGE_DIGITAL_ZERO_SIZE		20480

typedef enum {
	PATTERN_ONE,
	PATTERN_TWO,
	PATTERN_THREE,
	PATTERN_FOUR,
	PATTERN_FIVE,
	PATTERN_SIX,
	PATTERN_SEVEN,
	PATTERN_EIGHT,
	PATTERN_NINE,
	PATTERN_ZERO,
	PATTERN_NONE
} PATTERN_DIGI_NUMBER;

#define LCD_RES_X 240
#define LCD_RES_Y 320

#define SPI_PANEL_PORT    SPI2
#define SPI_PANEL_FREQ    40000000

#define GPIO_SPI_SS     PA11                   // UNO SS
#define GPIO_LCM_DC     PB2                    // UNO D0
#define GPIO_LCM_RST    PB3                    // UNO D1
#define ILI9341_LED     PB5//PE5               // UNO D7

//static void _write_command(unsigned char temp);
//static void _write_data(unsigned char temp);
#ifdef  __cplusplus
extern  "C" {
#endif 
void spi_panel_init(void);

void LCD_Set_Pos(void);
void LCD_Set_Color(uint16_t color);
void spi_panel_show_to_LCD(uint8_t* pattern);   //testMing  uint16_t -> uint8_t
void _write_command(uint8_t temp);
void _write_data(uint8_t temp);
void _poll_spi(uint8_t temp);

#ifdef  __cplusplus
}
#endif 

#endif	/* __SPI_PANEL_H__ */
