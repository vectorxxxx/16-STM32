#include "stm32f10x.h"                  // Device header
#include "MySPI.h"
#include "W25Q64_Ins.h"

/**
  * @brief W25Q64初始化
  * @param  无
  * @retval 无
  */
void W25Q64_Init(void)
{
    MySPI_Init();
}

/**
  * @brief W25Q64获取厂商ID、设备ID
  * @param  MID 厂商ID
  * @param  DID 设备ID
  * @retval 无
  */
void W25Q64_GetID(uint8_t *MID, uint16_t *DID)
{
    MySPI_Start();
    MySPI_SwapByte(W25Q64_JEDEC_ID);
    *MID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);
    *DID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);
    *DID <<= 8;
    *DID |= MySPI_SwapByte(W25Q64_DUMMY_BYTE);
    MySPI_Stop();
}

/**
  * @brief W25Q64写使能
  * @param  无
  * @retval 无
  */
void W25Q64_WriteEnable(void)
{
    MySPI_Start();
    MySPI_SwapByte(W25Q64_WRITE_ENABLE);
    MySPI_Stop();
}

/**
  * @brief W25Q64读取状态寄存器1
  * @param  无
  * @retval 无
  */
void W25Q64_WaitBusy(void)
{
    uint32_t Timeout;
    MySPI_Start();
    MySPI_SwapByte(W25Q64_READ_STATUS_REGISTER_1);
    while((MySPI_SwapByte(W25Q64_WRITE_ENABLE) & 0x01) == 0x01)
    {
        if(--Timeout == 0) break;
    }
    MySPI_Stop();
}

/**
  * @brief 页编程
  * @param  Address 地址
  * @param  Array 数组
  * @param  Length 数组长度
  * @retval 无
  */
void W25Q64_PageProgram(uint32_t Address, uint8_t *DataArray, uint16_t Length)
{
    uint16_t i;
    W25Q64_WriteEnable();
    
    MySPI_Start();
    MySPI_SwapByte(W25Q64_PAGE_PROGRAM);
    MySPI_SwapByte(Address >> 16);    // Address时24位，函数参数为8为，利用自动舍弃高位的特点
    MySPI_SwapByte(Address >> 8);
    MySPI_SwapByte(Address);
    for(i = 0; i < Length; i++)
    {
        MySPI_SwapByte(DataArray[i]);
    }
    MySPI_Stop();
    
    W25Q64_WaitBusy();
}

/**
  * @brief 扇区擦除
  * @param  Address 地址
  * @retval 无
  */
void W25Q64_SectorErase(uint32_t Address)
{
    W25Q64_WriteEnable();
    
    MySPI_Start();
    MySPI_SwapByte(W25Q64_SECTOR_ERASE_4KB);
    MySPI_SwapByte(Address >> 16);    // Address时24位，函数参数为8为，利用自动舍弃高位的特点
    MySPI_SwapByte(Address >> 8);
    MySPI_SwapByte(Address);
    MySPI_Stop();
    
    W25Q64_WaitBusy();
}

/**
  * @brief 读取数据
  * @param  Address 地址
  * @param  Array 数组
  * @param  Length 数组长度
  * @retval 无
  */
void W25Q64_ReadData(uint32_t Address, uint8_t *DataArray, uint32_t Length)
{
    uint32_t i;
    
    MySPI_Start();
    MySPI_SwapByte(W25Q64_READ_DATA);
    MySPI_SwapByte(Address >> 16);    // Address时24位，函数参数为8为，利用自动舍弃高位的特点
    MySPI_SwapByte(Address >> 8);
    MySPI_SwapByte(Address);
    for(i = 0; i < Length; i++)
    {
        DataArray[i] = MySPI_SwapByte(W25Q64_DUMMY_BYTE);
    }
    MySPI_Stop();
}

