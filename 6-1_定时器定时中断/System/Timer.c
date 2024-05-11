#include "stm32f10x.h"                  // Device header

/**
  * @brief 定时器初始化
  * @param  无
  * @retval 无
  */
void Timer_Init(void)
{
    // 1、RCC内部时钟        stm32f10x_rcc.h
    // STM32F103C8T6定时器资源：TIM1(高级定时器，APB2总线)、TIM2/TIM3/TIM4(通用定时器，APB1总线)
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);                // 选择通用定时器TIM2
    
    
    // 2、内部时钟模式       stm32f10x_tim.h
    TIM_InternalClockConfig(TIM2);
    
    
    // 3、时基单元           stm32f10x_tim.h
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;         // 内部时钟分频因子设置为1，不分频，用于配置滤波器时钟，不影响时基单元功能
    // 计数器模式：向上计数、向下计数、中央对齐模式
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;     // 向上计数
    // 计数器计数频率：CK_CNT    = CK_PSC / (PSC + 1)
    // 计数器溢出频率：CK_CNT_OV = CK_CNT / (ARR + 1)
	//				             = CK_PSC / (PSC + 1) / (ARR + 1)
    // 其中，
    //      PSC(Prescaler, 预分频器)
    //      ARR(Auto-Reload Register, 自动重装器)
    //      CNT(Counter, 计数器)
    // 72MHz下，预分频器为7200-1，自动重装器为10000-1，计数器计数完毕为1s:
    //      带入“计数器计数频率”公式：                计数器时钟频率 = 主时钟频率 / (预分频器 + 1) = 72MHz / (7200 - 1 + 1) = 10KHz
    //      当计数器从0计数到9999时（即10000次）：    计数时间 = 自动重装器的值 / 计数器的时钟频率 = 10000 / 10kHz = 1s
    TIM_TimeBaseInitStructure.TIM_Period = 10000 - 1;                   // 自动重装器
    TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;                 // 预分频器
    // 重复次数计数器，取值范围：0x00 ~ 0xFF
    // 高级计数器才有，当前使用的是通用计数器，给定 0x00 即可
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0x00;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
    
    
    // 4、中断输出控制       stm32f10x_tim.h
    // 清除定时器更新标志位：TIM_TimeBaseInit函数末尾，手动产生了更新事件。若不清除此标志位，则开启中断后，会立刻进入一次中断
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    // 中断源：更新中断、捕获比较中断、触发器中断...
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);                          // UIE(Update Interrupt Enable, 允许更新中断)
    
    // 5、NVIC(嵌套向量中断控制器)    misc.h
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;           // 抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;                  // 响应优先级
    NVIC_Init(&NVIC_InitStructure);
    
    // 6、运行控制           stm32f10x_tim.h
    TIM_Cmd(TIM2, ENABLE);
}

/**
  * @brief TIM2定时中断函数   startup_stm32f10x_md.s
  * @param  无
  * @retval 无
  */
/*
void TIM2_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
    {
        
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}
*/
