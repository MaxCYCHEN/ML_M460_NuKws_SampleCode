
#include <stdio.h>
#include <string.h>
#include "WiFi.h"
#include "esp8266.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Global variables                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/

uint8_t esp07_version_check(void)
{
	char res[256];

	esp8266_getVersion(res);

	if (strstr((const char*)res, "3.0.0") != NULL)
		return 1;
	
	return 0;
}

void esp_low_levle_init(void)
{
    /* Configure UART0 and set UART0 baud rate */
    UART_Open(UART2, 115200);

    /* Set multi-function pins for RXD and TXD */
    SET_UART2_RXD_PC0();
    SET_UART2_TXD_PC1();
}

int8_t esp_test(void)
{

    if (!esp07_version_check()) {
		printf("WiFi test Fail \n");
        return -1;
	}
    printf("<done>\n");
    
    return 0;
}

/*** (C) COPYRIGHT 2016 Nuvoton Technology Corp. ***/
