#include "stm32f10x.h"                  // Device header
#include "Delay.h"


#define MyI2C_SCL   GPIO_Pin_10
#define MyI2C_SDA   GPIO_Pin_11

/**
  * @brief 写入SCL电平
  * @param  SCL 高低电平
  * @retval 无
  */
void MyI2C_W_SCL(uint8_t SCL)
{
    GPIO_WriteBit(GPIOB, MyI2C_SCL, (BitAction) SCL);
    Delay_us(10);												//延时10us，防止时序频率超过要求
}

/**
  * @brief 写入SDA电平
  * @param  SDA 高低电平
  * @retval 无
  */
void MyI2C_W_SDA(uint8_t SDA)
{
    GPIO_WriteBit(GPIOB, MyI2C_SDA, (BitAction) SDA);
    Delay_us(10);												//延时10us，防止时序频率超过要求    
}

/**
  * @brief 读取SDA电平
  * @param  无
  * @retval SDA 高低电平
  */
uint8_t MyI2C_R_SDA(void)
{
    uint8_t BitValue = GPIO_ReadInputDataBit(GPIOB, MyI2C_SDA);
    Delay_us(10);												//延时10us，防止时序频率超过要求    
    return BitValue;
}
    
/**
  * @brief I2C初始化
  * @param  无
  * @retval 无
  */
void MyI2C_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;            // 开漏输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = MyI2C_SCL | MyI2C_SDA;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
    GPIO_SetBits(GPIOB, MyI2C_SCL);
    GPIO_SetBits(GPIOB, MyI2C_SDA);
}

/**
  * @brief 起始条件：SCL高电平期间，SDA从高电平切换到低电平
  * @param  无
  * @retval 无
  */
void MyI2C_Start(void)
{
    MyI2C_W_SDA(1);
    MyI2C_W_SCL(1);
    MyI2C_W_SDA(0);
    MyI2C_W_SCL(0);
}

/**
  * @brief 终止条件：SCL高电平期间，SDA从低电平切换到高电平
  * @param  无
  * @retval 无
  */
void MyI2C_Stop(void)
{
    MyI2C_W_SDA(0);
    MyI2C_W_SCL(1);    
    MyI2C_W_SDA(1);
}

/**
  * @brief 发送一个字节：SCL低电平期间，主机将数据位依次放到SDA线上（高位先行），然后释放SCL，从机将在SCL高电平期间读取数据位
                            所以SCL高电平期间SDA不允许有数据变化，依次循环上述过程8次，即可发送一个字节
  * @param  Byte 发送的一个字节
  * @retval 无
  */
void MyI2C_SendByte(uint8_t Byte)
{
    uint8_t i;
    MyI2C_W_SCL(0);
    
    for (i = 0; i < 8; i++)
    {
        MyI2C_W_SDA(Byte & (0x80 >> i));
        MyI2C_W_SCL(1);        
        MyI2C_W_SCL(0);
    }
}

/**
  * @brief 接收一个字节：SCL低电平期间，从机将数据位依次放到SDA线上（高位先行），然后释放SCL，主机将在SCL高电平期间读取数据位
                            所以SCL高电平期间SDA不允许有数据变化，依次循环上述过程8次，即可接收一个字节（主机在接收之前，需要释放SDA）
  * @param  无
  * @retval Byte 接收的一个字节
  */
uint8_t MyI2C_ReceiveByte(void)
{
    uint8_t i, Byte=0;
    MyI2C_W_SDA(1);
    
    for (i = 0; i < 8; i++)
    {
        MyI2C_W_SCL(1);
        if(MyI2C_R_SDA()) Byte |= (0x80 >> i);
        MyI2C_W_SCL(0);
    }
    
    return Byte;
}

/**
  * @brief 发送应答：主机在接收完一个字节之后，在下一个时钟发送一位数据
  * @param  AckBit 发送的应答，数据0表示应答，数据1表示非应答
  * @retval 无
  */
void MyI2C_SendAck(uint8_t AckBit)
{
    MyI2C_W_SDA(AckBit);
    MyI2C_W_SCL(1);
    MyI2C_W_SCL(0);
}

/**
  * @brief 接收应答：主机在发送完一个字节之后，在下一个时钟接收一位数据，判断从机是否应答（主机在接收之前，需要释放SDA）
  * @param  无
  * @retval AckBit 接收的应答，数据0表示应答，数据1表示非应答
  */
uint8_t MyI2C_ReceiveAck(void)
{
    uint8_t AckBit=0;
    MyI2C_W_SDA(1);
    
    MyI2C_W_SCL(1);
    AckBit = MyI2C_R_SDA();
    MyI2C_W_SCL(0);
    
    return AckBit;
}
