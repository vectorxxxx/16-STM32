#include "stm32f10x.h"                  // Device header

/**
  * @brief 光敏传感器初始化
  * @param  无
  * @retval 无
  */
void LightSensor_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
  * @brief 读取光敏传感器高低电平
  * @param  无
  * @retval 高低电平：1/0，有光输出0，无光输出1
  */
uint8_t LightSensor_Get(void)
{
    return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13);
}

