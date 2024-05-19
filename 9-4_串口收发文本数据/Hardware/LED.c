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
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // 默认熄灭
    GPIO_SetBits(GPIOA, GPIO_Pin_1);
}

/**
  * @brief 点亮LED
  * @param  无
  * @retval 无
  */
void LED_ON(void)
{
    GPIO_ResetBits(GPIOA, GPIO_Pin_1);
}

/**
  * @brief 熄灭LED
  * @param  无
  * @retval 无
  */
void LED_OFF(void)
{
    GPIO_SetBits(GPIOA, GPIO_Pin_1);
}

/**
  * @brief 亮灭切换LED
  * @param  无
  * @retval 无
  */
void LED_TURN(void)
{
    if(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_1) == 0)
    {
        LED_OFF();
    }
    else 
    {
        LED_ON();
    }
}
