#include "stm32f10x.h"                  // Device header

const uint8_t AD_Size = 4;

uint16_t AD_Value[4];

/**
  * @brief ADC初始化
  * @param  无
  * @retval 无
  */
void AD_Init(void)
{
    // 1、开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);                    // 开启ADC时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);                   // 开启GPIO时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);                                       // 6分频
    
    
    // 2、GPIO
    GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;                           // 模拟输入模式，ADC专属配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;                               // PA0
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    
    // 3、ADC     .\Library\stm32f10x_adc.h
    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                     // 单次转换
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;                            // 扫描模式
    ADC_InitStructure.ADC_NbrOfChannel = AD_Size;                           // 通道数目
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;     // 不用外部事件启动转换
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                  // 数据右对齐
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                      // 独立模式（此模式里，双ADC同步不工作，每个ADC接口独立工作）
    ADC_Init(ADC1, &ADC_InitStructure);

    // 多通道
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5); 
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_55Cycles5);


    // 4、DMA     .\Library\stm32f10x_dma.h
    DMA_InitTypeDef DMA_InitStructure;
    
    // 外设：起始地址、数据宽度、地址是否自增
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;             // ADC数据寄存器地址
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // 半字，取低16位
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;            // 不开启自增
    
    // 存储器：起始地址、数据宽度、地址是否自增
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)AD_Value;                  // SRAM存储器地址
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;         // 半字，取低16位
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                     // 开启自增
    
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                          // 外设->存储器
    DMA_InitStructure.DMA_BufferSize = (uint32_t)AD_Size;                       // 传输计数器数值
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                // 硬件触发
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                               // 不开启自动重装
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                       // 优先级
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    
    
    // 5、使能
    // DMA使能
    DMA_Cmd(DMA1_Channel1, ENABLE);
    // ADC_DMA请求使能
    ADC_DMACmd(ADC1, ENABLE);
    // ADC使能
    ADC_Cmd(ADC1, ENABLE);
    
    
    // 6、校准
    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1) == SET);
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1) == SET);
}

/**
  * @brief 获取AD值
  * @param  无
  * @retval 无
  */
void AD_GetValue(void)
{
    // DMA失能
    DMA_Cmd(DMA1_Channel1, DISABLE);
    // 传输计数器设值
    DMA_SetCurrDataCounter(DMA1_Channel1, AD_Size);
    // DMA使能
    DMA_Cmd(DMA1_Channel1, ENABLE);
    
    // 软件触发转换
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    
    // 等待转运完成
    while (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);
    // 清除转运完成状态
    DMA_ClearFlag(DMA1_FLAG_TC1);
}
