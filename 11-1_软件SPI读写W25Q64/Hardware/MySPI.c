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
  * @brief 写入SPI时钟CLK(SCK)
  * @param  BitValue 电位，0或1
  * @retval 无
  */
void MySPI_W_SCK(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOA, GPIO_Pin_5, (BitAction)BitValue);
}
  
/**
  * @brief 写入SPI主机输出从机输入
  * @param  BitValue 电位，0或1
  * @retval 无
  */  
void MySPI_W_MOSI(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOA, GPIO_Pin_7, (BitAction)BitValue);
}
  
/**
  * @brief 读取SPI主机输入从机输出
  * @param  无
  * @retval BitValue 电位，0或1
  */  
uint8_t MySPI_R_MISO(void)
{
    return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6);
}
    
/**
  * @brief SPI初始化
  * @param  无
  * @retval 无
  */
void MySPI_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
    // 初始化电位
    MySPI_W_SS(1);                      // SPI片选
    MySPI_W_SCK(0);                     // SPI时钟
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
    uint8_t i, ReceiveByte = 0x00;  // 不要忘记进行初始化赋值                    
    
    for(i = 0; i < 8; i++)
    {
        MySPI_W_MOSI(SendByte & (0x80 >> i));
        MySPI_W_SCK(1);
        if(MySPI_R_MISO() == 1) ReceiveByte |= (0x80 >> i);
        MySPI_W_SCK(0);
    }
    
    return ReceiveByte;
}
