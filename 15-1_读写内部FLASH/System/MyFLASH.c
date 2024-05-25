#include "stm32f10x.h"                  // Device header

/**
  * @brief 读字
  * @param  Address 地址
  * @retval 字
  */
uint32_t MyFLASH_ReadWord(uint32_t Address)
{
    return *((__IO uint32_t *)(Address));
}

/**
  * @brief 读半字
  * @param  Address 地址
  * @retval 半字
  */
uint16_t MyFLASH_ReadHalfWord(uint32_t Address)
{
    return *((__IO uint16_t *)(Address));
}

/**
  * @brief 读一个字节
  * @param  Address 地址
  * @retval 一个字节
  */
uint8_t MyFLASH_ReadByte(uint32_t Address)
{
    return *((__IO uint8_t *)(Address));
}

/**
  * @brief 全擦除
  * @param  无
  * @retval 无
  */
void MyFLASH_EraseAllPages(void)
{
    FLASH_Unlock();
    FLASH_EraseAllPages();
    FLASH_Lock();
}
 
/**
  * @brief 页擦除
  * @param  无
  * @retval 无
  */  
void MyFLASH_ErasePage(uint32_t Page_Address)
{
    FLASH_Unlock();
    FLASH_ErasePage(Page_Address);
    FLASH_Lock();
}
  
/**
  * @brief 字编程
  * @param  Address 地址
  * @param  Data 数据
  * @retval 无
  */  
void MyFLASH_ProgramWord(uint32_t Address, uint32_t Data)
{
    FLASH_Unlock();
    FLASH_ProgramWord(Address, Data);
    FLASH_Lock();
}  

/**
  * @brief 半字编程
  * @param  Address 地址
  * @param  Data 数据
  * @retval 无
  */  
void MyFLASH_ProgramHalfWord(uint32_t Address, uint16_t Data)
{
    FLASH_Unlock();
    FLASH_ProgramHalfWord(Address, Data);
    FLASH_Lock();
}
