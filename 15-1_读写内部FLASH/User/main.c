#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Key.h"
//#include "MyFLASH.h"
#include "Store.h"

uint8_t KeyNum;

int main(void)
{
    OLED_Init();
    Key_Init();
    Store_Init();
    
    OLED_ShowString(1, 1, "FLAG:");
    OLED_ShowString(2, 1, "DATA:");
    
    while(1)
    {
        KeyNum = Key_GetNum();
        
        if(KeyNum == 1)
        {
            Store_Data[1]++;
            Store_Data[2] += 2;
            Store_Data[3] += 4;
            Store_Data[4] += 8;
            Store_Save();
        }
        else if(KeyNum == 2)
        {
            Store_Clear();
        }
        
        OLED_ShowHexNum(1, 7, Store_Data[0], 4);
        OLED_ShowHexNum(3, 1, Store_Data[1], 4);
        OLED_ShowHexNum(3, 7, Store_Data[2], 4);
        OLED_ShowHexNum(4, 1, Store_Data[3], 4);
        OLED_ShowHexNum(4, 7, Store_Data[4], 4);
    }
}


//int main(void)
//{
//    OLED_Init();
//    Key_Init();
//    
//    OLED_ShowHexNum(1, 1, MyFLASH_ReadWord(0x08000000), 8);
//    OLED_ShowHexNum(2, 1, MyFLASH_ReadWord(0x08000000), 4);
//    OLED_ShowHexNum(3, 1, MyFLASH_ReadWord(0x08000000), 2);
//    
//    MyFLASH_ProgramWord(0x0800FC00, 0x12345678);
//    MyFLASH_ProgramHalfWord(0x0800FC10, 0xABCD);
//    
//    while(1)
//    {
//        KeyNum = Key_GetNum();
//        
//        if(KeyNum == 1)
//        {
//            MyFLASH_EraseAllPages();
//        }
//        else if(KeyNum == 2)
//        {
//            MyFLASH_ErasePage(0x08000000);
//        }
//    }
//}
