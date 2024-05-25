#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>

uint16_t Serial_RxData;
uint8_t Serial_RxFlag;

/**
  * @brief 串口初始化
  * @param  无
  * @retval 无
  */
void Serial_Init(void)
{
    // 1、开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    // 2、GPIO
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                                     // 复用推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;               
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                                       // 上拉输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;               
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // 3、USART
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 9600;                                          // 9600波特率
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;     // 硬件数据流控
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;                     // 发送控制器 + 接收控制器
    USART_InitStructure.USART_Parity = USART_Parity_No;                                 // 无奇偶校验
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                              // 1个停止位
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                         // 8位字长
    USART_Init(USART1, &USART_InitStructure);
    
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);                                      // 开启中断
    
    // 4、NVIC
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);
    
    // 5、使能
    USART_Cmd(USART1, ENABLE);
}

/**
  * @brief 发送字节
  * @param  Byte 字节
  * @retval 无
  */
void Serial_SendByte(uint8_t Byte)
{
    USART_SendData(USART1, Byte);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

/**
  * @brief 发送数组
  * @param  Array   数组
  * @param  Length  数组长度
  * @retval 无
  */
void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
    uint16_t i;
    for(i = 0; i < Length; i++)
    {
        Serial_SendByte(Array[i]);
    }
}

/**
  * @brief 发送字符串
  * @param  String 字符串
  * @retval 无
  */
void Serial_SendString(char *String)
{
    uint8_t i;
    for(i = 0; String[i] != '\0'; i++)  // 不等于ASCII码表中的空字符，空字符是字符串的结束标志位
    {
        Serial_SendByte(String[i]);
    }
}

/**
  * @brief 幂运算
  * @param  X 底数
  * @param  Y 指数
  * @retval 幂运算结果   
  */
uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
    uint32_t Result = 1;
    
    while(Y--)
    {
        Result *= X;
    }
    
    return Result;
}
    
/**
  * @brief 发送数字
  * @param  Number 数字
  * @retval 无
  */
void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
    uint8_t i;
    for(i = 0; i < Length; i++)
    {
        Serial_SendByte(Number / Serial_Pow(10, Length - 1 - i) % 10 + '0');
    }
}

/**
  * @brief 使用printf需要重定向的底层函数
  * @param 保持原始格式即可，无需变动
  * @retval 保持原始格式即可，无需变动
  */
int fputc(int ch, FILE *f)
{
    Serial_SendByte(ch);
    return ch;
}

/**
  * @brief 打印format格式字符串到串口
  * @param  无
  * @retval 无
  */
void Serial_Printf(char *format, ...)
{
    char String[100];
    va_list arg;
    va_start(arg, format);
    vsprintf(String, format, arg);
    va_end(arg);
    Serial_SendString(String);
}

/**
  * @brief 获取RxFlag
  * @param  无
  * @retval RxFlag
  */
uint8_t Serial_GetRxFlag(void)
{
    uint8_t Flag = Serial_RxFlag;
    Serial_RxFlag = 0;
    return Flag;
}

/**
  * @brief 获取RxData
  * @param  无
  * @retval RxData
  */
uint16_t Serial_GetRxData(void)
{
    return Serial_RxData;
}
    
/**
  * @brief USART1中断函数
  * @param  无
  * @retval 无
  */
void USART1_IRQHandler(void)
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET) 
    {
        Serial_RxData = USART_ReceiveData(USART1);
        Serial_RxFlag = 1;
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}
