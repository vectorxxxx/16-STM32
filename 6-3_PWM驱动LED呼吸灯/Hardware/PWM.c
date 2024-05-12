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
    
//    // 重定义功能
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
//    // 《STM32F10xxx参考手册（中文）》 > 8.3.7 定时器复用功能重映射 > 表43 TIM2复用功能重映像
//    //              TIM2_REMAP[1:0]==00     TIM2_REMAP[1:0]==01     TIM2_REMAP[1:0]==10     TIM2_REMAP[1:0]==11
//    // 复用功能     (没有重映像)            (部分重映像)            (部分重映像)            (完全重映像)
//    // TIM2_CH1_ETR PA0                     PA15                    PA0                     PA15
//    // TIM2_CH2     PA1                     PB3                     PA1                     PB3
//    // TIM2_CH3                 PA2                                             PB10
//    // TIM2_CH4                 PA3                                             PB11
//    // 《STM32F10xxx参考手册（中文）》 > 8.4.2 复用重映射和调试I/O配置寄存器(AFIO_MAPR) > 小、中和大容量产品的寄存器映像和位定义
//    // 00：没有重映像(CH1/ETR/PA0， CH2/PA1， CH3/PA2， CH4/PA3)；
//    // 01：部分映像(CH1/ETR/PA15， CH2/PB3， CH3/PA2， CH4/PA3)；
//    // 10：部分映像(CH1/ETR/PA0， CH2/PA1， CH3/PB10， CH4/PB11)；
//    // 11：完全映像(CH1/ETR/PA15， CH2/PB3， CH3/PB10， CH4/PB11)。
//    GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);                 // 部分重映射TIM2的引脚，可查看
//    // GPIO_Remap_SWJ_NoJTRST      : Full SWJ Enabled (JTAG-DP + SW-DP) but without JTRST
//    // GPIO_Remap_SWJ_JTAGDisable  : JTAG-DP Disabled and SW-DP Enabled
//    // GPIO_Remap_SWJ_Disable      : Full SWJ Disabled (JTAG-DP + SW-DP)
//    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);              // 将JTAG引脚失能，作为普通GPIO引脚使用
    
    // 1、GPIO       .\Library\stm32f10x_gpio.h
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                       // 复用推挽输出模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;                           // PA0 默认复用功能 TIM2_CH1_ETR
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;                            // PA15 重定义功能 TIM2_CH1_ETR
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    
    // 2、时钟源     .\Library\stm32f10x_tim.h
    TIM_InternalClockConfig(TIM2);
    
    
    // 3、时基单元   .\Library\stm32f10x_tim.h
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;            // 不分频
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;        // 向上计数
    // PWM频率：	Freq = CK_PSC / (PSC + 1) / (ARR + 1) = 1KHZ
    // PWM占空比：	Duty = CCR / (ARR + 1)                = 50%
    // PWM分辨率：	Reso = 1 / (ARR + 1)                  = 1ms
    // 代入公式，可得：ARR = 100 - 1, CCR = 50, PSC = 720 - 1;
    TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;                        // ARR自动重装器
    TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1;                     // PSC预分频器
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0x00;
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
