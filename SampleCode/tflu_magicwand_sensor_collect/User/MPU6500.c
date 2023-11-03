/**************************************************************************//**
 * @file     MPU6500.c
 * @version  V3.00
 * @brief
 *           Show how to use I2C Signle byte API Read and Write data to Slave
 *           Needs to work with I2C_Slave sample code.
 * @copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/
#include <stdio.h>
#include "MPU6500.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Global variables                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
volatile    uint8_t     g_u8DeviceAddr;
unsigned    char        BUF[10];        //接收資料緩存區
short T_X,T_Y,T_Z,T_T;                  //X,Y,Z軸，溫度

/*-----------------------------------------------*/
// Init MPU6500
//
/*-----------------------------------------------*/
void Init_MPU6500(void)
{

    I2C_WriteByteOneReg(I2C2, MPU6500_DEVICE_ID, PWR_MGMT_1,   0x00);//IMU use internal oscillator
    I2C_WriteByteOneReg(I2C2, MPU6500_DEVICE_ID, SMPLRT_DIV,   0x07);//FIFO Sample Rate 125Hz
    I2C_WriteByteOneReg(I2C2, MPU6500_DEVICE_ID, CONFIG,       0x06);//Enabale DLPF
    I2C_WriteByteOneReg(I2C2, MPU6500_DEVICE_ID, GYRO_CONFIG,  0x18); //Gyro FS +-2000dps
    I2C_WriteByteOneReg(I2C2, MPU6500_DEVICE_ID, ACCEL_CONFIG, 0x01);//Accel FS +-2g
  //----------------
//    Single_Write(GYRO_ADDRESS,0x6A,0x00);//close Master Mode

}

void MPU6500_low_levle_init(void)
{
    SET_I2C2_SCL_PD1();
    SET_I2C2_SDA_PD0();
    
    /* Open I2C2 module and set bus clock */
    I2C_Open(I2C2, 100000);
}

int8_t MPU6500_test(void)
{
    /* Slave Address */
    g_u8DeviceAddr = MPU6500_DEVICE_ID;

    printf("Who am I = %02x\n", I2C_ReadByteOneReg(I2C2, MPU6500_DEVICE_ID, WHO_AM_I));
    if(I2C_ReadByteOneReg(I2C2, MPU6500_DEVICE_ID, WHO_AM_I) == 0x70){
        printf("MPU6500 found.\n");
        /* Init MPU6500 sensor */
        Init_MPU6500();
    }else{ 
        printf("MPU6500 not found!!!\n");
        return -1;
    }
    
    /* X-axis */
    BUF[0]=I2C_ReadByteOneReg(I2C2, MPU6500_DEVICE_ID, ACCEL_XOUT_L); 
    BUF[1]=I2C_ReadByteOneReg(I2C2, MPU6500_DEVICE_ID, ACCEL_XOUT_H);
    T_X=    (BUF[1]<<8)|BUF[0];
//   T_X/=164;
    
    /* Y-axis */
    BUF[2]=I2C_ReadByteOneReg(I2C2, MPU6500_DEVICE_ID, ACCEL_YOUT_L);
    BUF[3]=I2C_ReadByteOneReg(I2C2, MPU6500_DEVICE_ID, ACCEL_YOUT_H);
    T_Y=    (BUF[3]<<8)|BUF[2];
//   T_Y/=164;
    
    /* Z-axis */
    BUF[4]=I2C_ReadByteOneReg(I2C2, MPU6500_DEVICE_ID, ACCEL_ZOUT_L);
    BUF[5]=I2C_ReadByteOneReg(I2C2, MPU6500_DEVICE_ID, ACCEL_ZOUT_H);
    T_Z=    (BUF[5]<<8)|BUF[4];
//   T_Z/=164;
    
    if(((T_Z+T_Y+T_X)<0) || ((T_Z+T_Y+T_X)>40000))
    {
        printf("GYRO Sensor Error\n");
        return -1;
    }
    
    printf("<done>\n");
    
    return 0;
}


int8_t MPU6500_readXYZ_mg(void)
{
	    float acc_sensitivity = 16384/1000;//16384/1000;
//    /* Slave Address */
//    g_u8DeviceAddr = MPU6500_DEVICE_ID;

//    printf("Who am I = %02x\n", I2C_ReadByteOneReg(I2C2, MPU6500_DEVICE_ID, WHO_AM_I));
//    if(I2C_ReadByteOneReg(I2C2, MPU6500_DEVICE_ID, WHO_AM_I) == 0x70){
//        printf("MPU6500 found.\n");
//        /* Init MPU6500 sensor */
//        Init_MPU6500();
//    }else{ 
//        printf("MPU6500 not found!!!\n");
//        return -1;
//    }
    
    /* X-axis */
    BUF[0]=I2C_ReadByteOneReg(I2C2, MPU6500_DEVICE_ID, ACCEL_XOUT_L); 
    BUF[1]=I2C_ReadByteOneReg(I2C2, MPU6500_DEVICE_ID, ACCEL_XOUT_H);
    T_X=    (BUF[1]<<8)|BUF[0];
//   T_X/=164;
    
    /* Y-axis */
    BUF[2]=I2C_ReadByteOneReg(I2C2, MPU6500_DEVICE_ID, ACCEL_YOUT_L);
    BUF[3]=I2C_ReadByteOneReg(I2C2, MPU6500_DEVICE_ID, ACCEL_YOUT_H);
    T_Y=    (BUF[3]<<8)|BUF[2];
//   T_Y/=164;
    
    /* Z-axis */
    BUF[4]=I2C_ReadByteOneReg(I2C2, MPU6500_DEVICE_ID, ACCEL_ZOUT_L);
    BUF[5]=I2C_ReadByteOneReg(I2C2, MPU6500_DEVICE_ID, ACCEL_ZOUT_H);
    T_Z=    (BUF[5]<<8)|BUF[4];
//   T_Z/=164;
    
    //if(((T_Z+T_Y+T_X)<0) || ((T_Z+T_Y+T_X)>40000))
    //{
    //    printf("GYRO Sensor Error\n");
    //    return -1;
    //}
    
		printf("%d,%d,%d\n", (int)((float)(T_X)/acc_sensitivity),(int)((float)(T_Y)/acc_sensitivity),(int)((float)(T_Z)/acc_sensitivity));
    //printf("<done>\n");
    
    return 0;
}
/*** (C) COPYRIGHT 2016 Nuvoton Technology Corp. ***/
