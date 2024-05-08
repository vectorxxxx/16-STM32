#include "stm32f10x.h"                  // Device header

int main(void)
{
    // 【一】寄存器模式
    // 疑点：为什么是GPIOC？因为是13号端口，16进制C代表13
    // 1）7.3.7 APB2 外设时钟使能寄存器(RCC_APB2ENR)
    // IOPCEN=1, IO端口C时钟开启
    //RCC->APB2ENR = 0x00000010;
    
    // 2）8.2.2 端口配置高寄存器(GPIOx_CRH) (x=A..E)
    // CNF13=00, 通用推挽输出模式
    //      00：通用推挽输出模式
    //      01：通用开漏输出模式
    //      10：复用功能推挽输出模式
    //      11：复用功能开漏输出模式
    // MODE13=11, 输出模式，最大速度50MHz
    //      00：输入模式(复位后的状态)
    //      01：输出模式，最大速度10MHz
    //      10：输出模式，最大速度2MHz
    //      11：输出模式，最大速度50MHz
    //GPIOC->CRH   = 0x00300000;
    
    // 3）8.2.4 端口输出数据寄存器(GPIOx_ODR) (x=A..E)
    // ODR13=0/1, 0为亮，1为灭
    //GPIOC->ODR   = 0x00000000;
    //GPIOC->ODR   = 0x00002000;
    
    
    
    // 【二】库函数模式
    // 1）stm32f10x_rcc.c
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    // 2）stm32f10x_gpio.c
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    // 指向结构体的地址：&GPIO_InitStructure
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    // 3）stm32f10x_gpio.c
    // 指定端口设置为高电平 ==> 灭
    //GPIO_SetBits(GPIOC, GPIO_Pin_13);
    // 指定端口设置为低电平 ==> 亮
    GPIO_ResetBits(GPIOC, GPIO_Pin_13);
    while(1)
    {
        
    }
  }
