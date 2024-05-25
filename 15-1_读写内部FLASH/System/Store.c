#include "stm32f10x.h"                  // Device header
#include "MyFLASH.h"

#define STORE_DATA_SIZE           512
#define STORE_START_ADDRESS       0x0800FC00
#define STORE_INIT_FLAG           0xA5A5

uint16_t Store_Data[STORE_DATA_SIZE];

/**
  * @brief 初始化
  * @param  无
  * @retval 无
  */
void Store_Init(void)
{
    if(MyFLASH_ReadHalfWord(STORE_START_ADDRESS) != STORE_INIT_FLAG)
    {
        MyFLASH_ErasePage(STORE_START_ADDRESS);                           // 要先擦除
        MyFLASH_ProgramHalfWord(STORE_START_ADDRESS, STORE_INIT_FLAG);
        for (uint16_t i = 1; i < STORE_DATA_SIZE; i++)
        {
            MyFLASH_ProgramHalfWord(STORE_START_ADDRESS + i * 2, 0x0000);
        }
    }
    
    // 从FLASH闪存中加载数据到SRAM数组中
    for (uint16_t i = 0; i < STORE_DATA_SIZE; i++)
    {
        Store_Data[i] = MyFLASH_ReadHalfWord(STORE_START_ADDRESS + i * 2);
    }
}

/**
  * @brief 保存
  * @param  无
  * @retval 无
  */
void Store_Save(void)
{
    MyFLASH_ErasePage(STORE_START_ADDRESS);   
    for (uint16_t i = 0; i < STORE_DATA_SIZE; i++)
    {
        MyFLASH_ProgramHalfWord(STORE_START_ADDRESS + i * 2, Store_Data[i]);
    }
}

/**
  * @brief 清除
  * @param  无
  * @retval 无
  */
void Store_Clear(void)
{
    for (uint16_t i = 1; i < STORE_DATA_SIZE; i++)
    {
        Store_Data[i] = 0x0000;
    }
    Store_Save();
}
