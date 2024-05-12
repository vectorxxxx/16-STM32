#include "stm32f10x.h"                  // Device header
#include "PWM.h"

/**
* @brief 直流电机初始化
  * @param  无
  * @retval 无
  */
void Motor_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    PWM_Init();
}

/**
  * @brief 设置直流电机转速
  * @param  Speed 角度，范围-100~100
  * @retval 无
  */
void Motor_SetSpeed(int8_t Speed)
{
    if(Speed>=0)
    {
        GPIO_SetBits(GPIOA, GPIO_Pin_4);
        GPIO_ResetBits(GPIOA, GPIO_Pin_5);
        PWM_SetCompare(Speed);
    }
    else 
    {
        GPIO_ResetBits(GPIOA, GPIO_Pin_4);
        GPIO_SetBits(GPIOA, GPIO_Pin_5);
        PWM_SetCompare(-Speed);
    }
}
