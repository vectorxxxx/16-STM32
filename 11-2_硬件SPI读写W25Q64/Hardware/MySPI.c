#include "stm32f10x.h"                  // Device header

/**
  * @brief 写入SPI片选CS(SS)
  * @param  BitValue 电位，0或1
  * @retval 无
  */
void MySPI_W_SS(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)BitValue);      // 需要强转为 BitAction 类型
}
    
/**
  * @brief SPI初始化
  * @param  无
  * @retval 无
  */
void MySPI_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                        // 通用推挽输出（软件控制）
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
        
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                         // 复用推挽输出（硬件控制）
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                           // 上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// SPI初始化
    SPI_InitTypeDef SPI_InitStructure;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                           // 主机模式
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;      // 双线全双工
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                       // 8位数据
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                      // 高位先行
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;    // 128分频，72M/128=576k bit/s
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;                              // 极性
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;                            // 第一个边沿
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                               // 软件控制NSS
    SPI_InitStructure.SPI_CRCPolynomial = 7;                                // CRC多项式，暂时用不到，给默认值7
    SPI_Init(SPI1, &SPI_InitStructure);
    
    // SPI使能
    SPI_Cmd(SPI1, ENABLE);
    
    // 设置默认电平
    MySPI_W_SS(1);                                                          // SPI片选
}

/**
  * @brief SPI时序基本单元——起始条件
  * @param  无
  * @retval 无
  */
void MySPI_Start(void)
{
    MySPI_W_SS(0);
}

/**
  * @brief SPI时序基本单元——终止条件
  * @param  无
  * @retval 无
  */
void MySPI_Stop(void)
{
    MySPI_W_SS(1);
}

/**
  * @brief SPI时序基本单元——交换一个字节（模式0）
  * @param  Byte 交换的一个字节
  * @retval 无
  */
uint8_t MySPI_SwapByte(uint8_t SendByte)
{
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) != SET);
    SPI_I2S_SendData(SPI1, SendByte);
    
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) != SET);
    return SPI_I2S_ReceiveData(SPI1);
}
