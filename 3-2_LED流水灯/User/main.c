#include "stm32f10x.h"                  // Device header
#include "Delay.h"

int main(void)
{
    // 1、APB2外设时钟: stm32f10x_rcc.h
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    // 2、GPIO初始化：stm32f10x_gpio.h
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//    GPIO_InitStructure.GPIO_Pin = 
//        GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | 
//        GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    while(1)
    {
        // 3、GPIO输出函数
        GPIO_Write(GPIOA, ~0x0001);
        Delay_ms(500);
        GPIO_Write(GPIOA, ~0x0002);
        Delay_ms(500);
        GPIO_Write(GPIOA, ~0x0004);
        Delay_ms(500);
        GPIO_Write(GPIOA, ~0x0008);
        Delay_ms(500);
        GPIO_Write(GPIOA, ~0x0010);
        Delay_ms(500);
        GPIO_Write(GPIOA, ~0x0020);
        Delay_ms(500);
        GPIO_Write(GPIOA, ~0x0040);
        Delay_ms(500);
        GPIO_Write(GPIOA, ~0x0080);
        Delay_ms(500);
    }
}
