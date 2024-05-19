#include "stm32f10x.h"                  // Device header
#include "OLED.h"
// #include "MyI2C.h"
// #include "MPU6050_Reg.h"
#include "MPU6050.h"
#include "Delay.h"

int16_t AX, AY, AZ;
int16_t GX, GY, GZ;

int main(void)
{
    OLED_Init();
    MPU6050_Init();
    
//    MyI2C_Init();
//    MyI2C_Start();
//    MyI2C_SendByte(0xD0);   // 1101 000 0, 前7为指定设备地址，最后1位为读写位
//    uint8_t Ack = MyI2C_ReceiveAck();
//    MyI2C_Stop();
//    
//    OLED_ShowNum(1, 1, Ack, 2);

    
//    MPU6050_Init();
//    MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x66);
//    uint8_t Data = MPU6050_ReadReg(MPU6050_PWR_MGMT_1);
//    OLED_ShowHexNum(1, 1, Data, 2);
    
    OLED_ShowString(1, 1, "ID:");
    OLED_ShowHexNum(1, 4, MPU6050_GetID(), 2);
    
    
    while(1)
    {
        MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);
        
        OLED_ShowSignedNum(2, 1, AX, 5);
        OLED_ShowSignedNum(3, 1, AY, 5);
        OLED_ShowSignedNum(4, 1, AZ, 5);
                
        OLED_ShowSignedNum(2, 8, GX, 5);
        OLED_ShowSignedNum(3, 8, GY, 5);
        OLED_ShowSignedNum(4, 8, GZ, 5);
        
        Delay_ms(500);
    }
}
