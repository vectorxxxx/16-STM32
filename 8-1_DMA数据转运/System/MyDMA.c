#include "stm32f10x.h"                  // Device header

uint16_t MyDMA_Size;

/**
  * @brief DMA初始化
  * @param  AddrA 外设起始地址
  * @param  AddrB 存储器起始地址
  * @param  Size 传输数据大小  
  * @retval 无
  */
void MyDMA_Init(uint32_t AddrA, uint32_t AddrB, uint16_t Size)
{
    MyDMA_Size = Size;
    
    // 1、开启时钟
    // AHB(Advanced High Performance Bus，高级高性能总线)
    // STM32F103C8T6 DMA资源：DMA1（7个通道）
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    
    
    // 2、初始化DMA     .\Library\stm32f10x_dac.h
    DMA_InitTypeDef DMA_InitStructure;
    // 外设：起始地址、数据宽度、地址是否自增
    DMA_InitStructure.DMA_PeripheralBaseAddr = AddrA;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;     // 字节访问
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;             // 开启自增
    // 存储器：起始地址、数据宽度、地址是否自增
    DMA_InitStructure.DMA_MemoryBaseAddr = AddrB;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                          // 外设->存储器
    DMA_InitStructure.DMA_BufferSize = Size;                                    // 传输计数器数值
    DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;                                 // 软件触发
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                               // 不开启自动重装（软件触发和自动重装是互斥的）
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                       // 这里指开启了一个通道，随便选择一个优先级即可
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    
    
    // 3、DMA使能默认关闭
    DMA_Cmd(DMA1_Channel1, DISABLE);                                            // 开关控制
}

/**
  * @brief DMA传输
  * @param  无
  * @retval 无
  */
void MyDMA_Transfer(void)
{
    // DMA失能
    DMA_Cmd(DMA1_Channel1, DISABLE);
    // 传输计数器设值
    DMA_SetCurrDataCounter(DMA1_Channel1, MyDMA_Size);
    // DMA使能
    DMA_Cmd(DMA1_Channel1, ENABLE);
    
    // 等待传输完成
    while (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);
    // 清除传输完成状态
    DMA_ClearFlag(DMA1_FLAG_TC1);
}
