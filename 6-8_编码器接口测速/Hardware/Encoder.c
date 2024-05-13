#include "stm32f10x.h"                  // Device header

/**
  * @brief 编码器初始化
  * @param  无
  * @retval 无
  */
void Encoder_Init(void)
{
    // 0、开启时钟   .\Library\stm32f10x_rcc.h
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    
    // 1、GPIO       .\Library\stm32f10x_gpio.h
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                          // 上拉输入
    // 默认复用功能：PA6 —— TIM3_CH1，PA7 —— TIM3_CH2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    
    // 2、时基单元   .\Library\stm32f10x_tim.h
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;            // 不分频
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;        // 向上计数
    TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;                      // ARR自动重装器
    TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;                       // PSC预分频器：不分频
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;                   // 重复计数器，高级定时器才会用到
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
    
    
    // 3、输入捕获   .\Library\stm32f10x_tim.h
    TIM_ICInitTypeDef TIM_ICInitStructure;
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;                       // 通道一     
    TIM_ICInitStructure.TIM_ICFilter = 0xF;                                // 滤波器，过滤信号抖动
    TIM_ICInit(TIM3, &TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;                       // 通道二     
    TIM_ICInitStructure.TIM_ICFilter = 0xF;                                // 滤波器，过滤信号抖动
    TIM_ICInit(TIM3, &TIM_ICInitStructure);

    // 4、编码器接口
    // TIM_EncoderMode_TI12: 在TI1和TI2上计数
    // TIM_IC1Polarity 和 TIM_IC2Polarity 相同，表示均不反向；不相同表示反向
    TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
    
    // 5、TIM使能
    TIM_Cmd(TIM3, ENABLE);
}

/**
  * @brief 获取位置
  * @param  无
  * @retval 编码器CNT值，-65535~65535
  */
int16_t Encoder_Get(void)
{
    return TIM_GetCounter(TIM3);
}

