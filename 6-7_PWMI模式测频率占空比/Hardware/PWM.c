#include "stm32f10x.h"                  // Device header

/**
  * @brief PWM初始化
  * @param  无
  * @retval 无
  */
void PWM_Init(void)
{
    // 0、开启时钟   .\Library\stm32f10x_rcc.h
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    
    // 1、GPIO       .\Library\stm32f10x_gpio.h
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                       // 复用推挽输出模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;                           // PA0 默认复用功能 TIM2_CH1_ETR
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    
    // 2、时钟源     .\Library\stm32f10x_tim.h
    TIM_InternalClockConfig(TIM2);
    
    
    // 3、时基单元   .\Library\stm32f10x_tim.h
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;            // 不分频
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;        // 向上计数
    TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;                        // ARR自动重装器
    TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;                      // PSC预分频器
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0x00;                //重复计数器，高级定时器才会用到
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
    
    
    // 4、输出比较   .\Library\stm32f10x_tim.h
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);
    // 向上计数：CNT<CCR时，REF置有效电平，CNT≥CCR时，REF置无效电平
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                      // PWM模式1
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;              // 输出比较极性
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;          // 输出使能
    TIM_OCInitStructure.TIM_Pulse = 0;                                     // CCR捕获比较器
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    
    
    // 5、TIM使能    .\Library\stm32f10x_tim.h
    TIM_Cmd(TIM2, ENABLE);
}

/**
  * @brief 动态设置捕获比较寄存器CCR的数值
  * @param  Compare1 CCR数值
  * @retval 无
  */
void PWM_SetCompare1(uint16_t Compare1)
{
    TIM_SetCompare1(TIM2, Compare1);
}

/**
  * @brief 设置预分频器的分频值
  * @param  Prescaler 分频值
  * @retval 无
  */
void PWM_SetPrescaler(uint16_t Prescaler)
{
    TIM_PrescalerConfig(TIM2, Prescaler, TIM_PSCReloadMode_Immediate);     // 不设置影子寄存器，立即加载
}
