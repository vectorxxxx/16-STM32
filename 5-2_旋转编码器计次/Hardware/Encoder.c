#include "stm32f10x.h"                  // Device header

// 旋转有正负，这里使用有符号类型
int16_t Encoder_Count;

/**
  * @brief 旋转编码器计次初始化
  * @param  无
  * @retval 无
  */
void Encoder_Init(void)
{
    // 1、RCC时钟外设
    // EXTI(外部中断无需时钟)、NVIC(内核无需时钟)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  // GPIOB
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);   // AFIO
    
    
    // 2、GPIOB
    GPIO_InitTypeDef GPIO_InitStructure;
    // 引脚：EXTI输入线 
    // 复用功能：外部中断输入 
    // GPIO配置：浮空输入、上拉输入、下拉输入
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  // 上拉输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    
    // 3、AFIO 终端引脚选择  stm32f10x_gpio.h
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);
    
    
    // 4、EXTI 边沿检测及控制  stm32f10x_exti.h
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = EXTI_Line0 | EXTI_Line1;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    // 触发响应方式：中断响应、事件响应
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;  // 中断响应
    // 触发方式：上升沿、下降沿、双边沿
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  // 下降沿
    EXTI_Init(&EXTI_InitStructure);
    
    
    // 5、NVIC  misc.h
    // ============================================================================================================================
    //   NVIC_PriorityGroup   | NVIC_IRQChannelPreemptionPriority | NVIC_IRQChannelSubPriority  | Description
    // ============================================================================================================================
    //  NVIC_PriorityGroup_2  |                0-3                |            0-3              |   2 bits for pre-emption priority
    //                        |                                   |                             |   2 bits for subpriority
    // ---------------------------------------------------------------------------------------------------------------------------- 
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief 获取旋转编码器计次
  * @param  无
  * @retval Tmp
  */
int16_t Encoder_Get(void)
{
    // 获取一次旋转增量后就对Encoder_Count进行清零，由主函数进行增减运算
    int16_t Tmp = Encoder_Count;
    Encoder_Count = 0;
    return Tmp;
}

/**
  * @brief 反向旋转外部中断函数
  * @param  无
  * @retval 无
  */
void EXTI0_IRQHandler(void)
{
    // 反向旋转时，A相下降沿，B相低电平
    if(EXTI_GetITStatus(EXTI_Line0) == SET)
    {
        if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)
        {
            Encoder_Count--;
        }
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

/**
  * @brief 正向旋转外部中断函数
  * @param  无
  * @retval 无
  */
void EXTI1_IRQHandler(void)
{
    // 正向旋转时，B相下降沿，A相低电平
    if(EXTI_GetITStatus(EXTI_Line1) == SET)
    {
        if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0)
        {
            Encoder_Count++;
        }
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
}
