
#include <stdio.h>
#include "NCT7717.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Global variables                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
volatile    uint8_t     g_u8NCT7717Addr;

void NCT7717_low_levle_init(void)
{
    SET_I2C2_SCL_PD1();
    SET_I2C2_SDA_PD0();
    
    /* Open I2C2 module and set bus clock */
    I2C_Open(I2C2, 100000);
}

int8_t NCT7717_test(void)
{
    int Ther;
    /* Slave Address */
    g_u8NCT7717Addr = NCT7717_DEVICE_ID;

    printf("Who am I = %02x\n", I2C_ReadByteOneReg(I2C2, NCT7717_DEVICE_ID, 0xFD));
    if(I2C_ReadByteOneReg(I2C2, NCT7717_DEVICE_ID, 0xFD) == 0x50){
        printf("NCT7717 found.\n");
    }else{ 
        printf("NCT7717 not found!!!\n");
        return -1;
    }
    
    Ther=I2C_ReadByteOneReg(I2C2, NCT7717_DEVICE_ID, 0x00); 
    printf("Temperature is %d\n", Ther);
    
    if((Ther<0) || (Ther>50))
    {
        printf("NCT7717 Sensor Error\n");
        return -1;
    }
    
    printf("<done>\n");
    
    return 0;
}

/*** (C) COPYRIGHT 2016 Nuvoton Technology Corp. ***/
