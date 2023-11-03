#ifndef __SPI_FLASH_H__
#define __SPI_FLASH_H__

//
// Includes
//
#include "NuMicro.h"

//
// Prototype
//
void spi_flash_low_level_init(void);
int8_t spi_flash_rw_test(void);

#endif  /* __SPI_FLASH_H__ */
