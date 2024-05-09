#include "stm32f10x.h"                  // Device header

/**
  * @brief LED初始化
  * @param  无
  * @retval 无
  */
void LED_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    // LED接在A1、A2两个端口上
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
    // 这里的速度是输出模式下的频率，输入模式下可以不用管，加上也无所谓
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // 默认熄灭
    GPIO_SetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_2);
}

/**
  * @brief 点亮LED1
  * @param  无
  * @retval 无
  */
void LED1_ON(void)
{
    GPIO_ResetBits(GPIOA, GPIO_Pin_1);
}

/**
  * @brief 熄灭LED1
  * @param  无
  * @retval 无
  */
void LED1_OFF(void)
{
    GPIO_SetBits(GPIOA, GPIO_Pin_1);
}

/**
  * @brief 亮灭切换LED1
  * @param  无
  * @retval 无
  */
void LED1_Turn(void)
{
    if(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_1) == 0)
    {
        LED1_OFF();
    }
    else 
    {
        LED1_ON();
    }
}

/**
  * @brief 点亮LED2
  * @param  无
  * @retval 无
  */
void LED2_ON(void)
{
    GPIO_ResetBits(GPIOA, GPIO_Pin_2);
}

/**
  * @brief 熄灭LED2
  * @param  无
  * @retval 无
  */
void LED2_OFF(void)
{
    GPIO_SetBits(GPIOA, GPIO_Pin_2);
}

/**
  * @brief 亮灭切换LED2
  * @param  无
  * @retval 无
  */
void LED2_Turn(void)
{
    if(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_2) == 0)
    {
        LED2_OFF();
    }
    else 
    {
        LED2_ON();
    }
}
