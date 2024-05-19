#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "MPU6050.h"
#include "Delay.h"

int16_t AX, AY, AZ;
int16_t GX, GY, GZ;

int main(void)
{
    OLED_Init();
    MPU6050_Init();
    
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
