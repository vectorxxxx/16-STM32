#include "stm32f10x.h"                  // Device header
#include "Delay.h"

int main(void)
{
    // 1、APB2外设时钟: stm32f10x_rcc.h
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    // 2、GPIO初始化：stm32f10x_gpio.h
    GPIO_InitTypeDef GPIO_InitStructure;
    // GPIO_Mode_AIN: AIN(Analog IN)，模拟输入
    // GPIO_Mode_IN_FLOATING: 浮空输入
    // GPIO_Mode_IPD: IPD(In Pull Down)，下拉输入
    // GPIO_Mode_IPU: IPU(In Pull Up)，上拉输入
    // GPIO_Mode_Out_OD: OD(Open Drain)，开漏输出
    // GPIO_Mode_Out_PP: PP(Push Pull)，推挽输出
    // GPIO_Mode_AF_OD: AF(Alternate Function)，复用开漏
    // GPIO_Mode_AF_PP: 复用推挽
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    while(1)
    {
        // 3、GPIO输出函数
        GPIO_ResetBits(GPIOA, GPIO_Pin_0);
        Delay_ms(500);
        GPIO_SetBits(GPIOA, GPIO_Pin_0);
        Delay_ms(500);
        
        GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET);
        Delay_ms(500);
        GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);
        Delay_ms(500);
        
        GPIO_WriteBit(GPIOA, GPIO_Pin_0, (BitAction)0);
        Delay_ms(500);
        GPIO_WriteBit(GPIOA, GPIO_Pin_0, (BitAction)1);
        Delay_ms(500);
    }
}
