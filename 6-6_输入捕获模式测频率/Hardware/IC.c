#include "stm32f10x.h"                  // Device header

void IC_Init(void)
{
     // 0、开启时钟   .\Library\stm32f10x_rcc.h
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    
    // 1、GPIO       .\Library\stm32f10x_gpio.h
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;                  // 输入捕获通道x 浮空输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    
    // 2、时钟源     .\Library\stm32f10x_tim.h
    TIM_InternalClockConfig(TIM3);
    
    
    // 3、时基单元   .\Library\stm32f10x_tim.h
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;            // 不分频
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;        // 向上计数
    TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;                      // ARR自动重装器
    TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;                      // PSC预分频器
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;                   // 重复计数器，高级定时器才会用到
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
    
    
    // 4、输入捕获   .\Library\stm32f10x_tim.h
    TIM_ICInitTypeDef TIM_ICInitStructure;
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;                       // 通道一     
    TIM_ICInitStructure.TIM_ICFilter = 0xF;                                // 滤波器，过滤信号抖动
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;            // 边沿检测极性选择：上升沿触发
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;                  // 无预分频器，捕获输入口上检测到的每一个边沿都触发一次捕获
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;        // 直通，不交叉
    TIM_ICInit(TIM3, &TIM_ICInitStructure);
    
    
    // 5、触发源选择、从模式   .\Library\stm32f10x_tim.h
    TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);                            // 触发源选择 TI1FP1(Timer Input 1 Filter Polarity 1）
    TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);                         // 从模式选择复位
    
    // 6、TIM使能
    TIM_Cmd(TIM3, ENABLE);
}

/**
  * @brief 获取频率
  * @param  无
  * @retval 频率值
  */
uint16_t IC_GetFreq(void)
{
    // 测周法得到频率fx = fc / N，这里不执行+1的操作也可
    return 1000000 / (TIM_GetCapture1(TIM3) + 1);
}
