#include "stm32f10x.h"                  // Device header

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
    
    // ADC的输入时钟不得超过14MHz，它是由PCLK2经分频产生
    // 所以可选的分频为：RCC_PCLK2_Div6、RCC_PCLK2_Div8
    // 分频后频率分别为：72MHz/6=12MHz、 72MHz/8=9MHz
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);                                       // 6分频
    
    
    // 2、GPIO
    GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;                           // 模拟输入模式，ADC专属配置
    //    PA0	ADC12_IN0
    //    PA1 	ADC12_IN1
    //    PA2 	ADC12_IN2
    //    PA3 	ADC12_IN3
    //    PA4 	ADC12_IN4
    //    PA5 	ADC12_IN5
    //    PA6 	ADC12_IN6
    //    PA7 	ADC12_IN7
    //    PB0 	ADC12_IN8
    //    PB1 	ADC12_IN9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;                               // PA0
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    
    // 3、ADC     .\Library\stm32f10x_adc.h
    // 11.12.2 ADC控制寄存器 1(ADC_CR1)
    // 11.12.3 ADC控制寄存器 2(ADC_CR2)
    // 11.12.9 ADC规则序列寄存器 1(ADC_SQR1)
    ADC_InitTypeDef ADC_InitStructure;
    // CONT：连续转换 (Continuous conversion)
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                     // 单次转换
    // SCAN：扫描模式 (Scan mode)
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;                           // 非扫描模式
    // L[3:0]：规则通道序列长度 (Regular channel sequence length)
    ADC_InitStructure.ADC_NbrOfChannel = 1;                                 // 通道数目
    // EXTTRIG：规则通道的外部触发转换模式 (External trigger conversion mode for regular channels)
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;     // 不用外部事件启动转换
    // ALIGN：数据对齐 (Data alignment)
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                  // 数据右对齐
    // DUALMOD[3:0]：双模式选择 (Dual mode selection)
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                      // 独立模式（此模式里，双ADC同步不工作，每个ADC接口独立工作）
    ADC_Init(ADC1, &ADC_InitStructure);

    
    // 4、开关控制
    ADC_Cmd(ADC1, ENABLE);
    
    
    // 5、校准
    // 11.12.3 ADC控制寄存器 2(ADC_CR2)
    //  RSTCAL：复位校准 (Reset calibration)
    //  该位由软件设置并由硬件清除。在校准寄存器被初始化后该位将被清除。
    //  0：校准寄存器已初始化；
    //  1：初始化校准寄存器。
    ADC_ResetCalibration(ADC1);                                             // 复位校准
    while (ADC_GetResetCalibrationStatus(ADC1) == SET);
    //  CAL： A/D校准 (A/D Calibration)
    //  该位由软件设置以开始校准，并在校准结束时由硬件清除。
    //  0：校准完成；
    //  1：开始校准。
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1) == SET);
}

/**
  * @brief 获取AD值
  * @param  ADC_Channel AD通道
  * @retval AD值，0~4095
  */
uint16_t AD_GetValue(uint8_t ADC_Channel)
{
    // 11.12.11 ADC规则序列寄存器 3(ADC_SQR3)
    ADC_RegularChannelConfig(ADC1, ADC_Channel, 1, ADC_SampleTime_55Cycles5);  // 序列1，采样时间
    
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);                                 // 软件触发控制
    // 11.12.1 ADC状态寄存器(ADC_SR)
    //  EOC：转换结束位 (End of conversion)
    //  该位由硬件在(规则或注入)通道组转换结束时设置，由软件清除或由读取ADC_DR时清除
    //  0：转换未完成；
    //  1：转换完成。
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
    return ADC_GetConversionValue(ADC1);
}
