#include "stm32f10x.h"                  // Device header

uint16_t CountSensor_Counter;

/**
  * @brief 对射式红外传感器计次初始化
  * @param  无
  * @retval 无
  */
void CountSensor_Init(void)
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
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    
    // 3、AFIO 终端引脚选择  stm32f10x_gpio.h
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);
    
    
    // 4、EXTI 边沿检测及控制  stm32f10x_exti.h
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = EXTI_Line14;
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
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief 获取计次
  * @param  无
  * @retval CountSensor_Counter 计次
  */ 
uint16_t CountSensor_Get(void)
{
    return CountSensor_Counter;
}

/**
  * @brief 外部中断函数  startup_stm32f10x_md.s
  * @param  无
  * @retval 无
  */
void EXTI15_10_IRQHandler(void)
{
    // 获取外部中断状态
    if(EXTI_GetITStatus(EXTI_Line14) == SET)
    {
        // 计次加一
        CountSensor_Counter++;
        
        // 清除外部中断挂起位
        EXTI_ClearITPendingBit(EXTI_Line14);
    }
}
