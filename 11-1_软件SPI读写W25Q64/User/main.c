#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "W25Q64.h"

uint8_t MID;
uint16_t DID;

uint32_t Address = 0x000000;
uint8_t Length = 4;
uint8_t ArrayWrite[] = {0x01, 0x02, 0x03, 0x04};
uint8_t ArrayRead[4];

int main(void)
{
    OLED_Init();
    W25Q64_Init();

    OLED_ShowString(1, 1, "MID:    DID:");
    OLED_ShowString(2, 1, "W:");
    OLED_ShowString(3, 1, "R:");
    
    W25Q64_GetID(&MID, &DID);
    OLED_ShowHexNum(1, 5, MID, 2);
    OLED_ShowHexNum(1, 13, DID, 4);
    
    W25Q64_SectorErase(Address);
    W25Q64_PageProgram(Address, ArrayWrite, Length);
    OLED_ShowHexNum(2, 3, ArrayWrite[0], 2);
    OLED_ShowHexNum(2, 6, ArrayWrite[1], 2);
    OLED_ShowHexNum(2, 9, ArrayWrite[2], 2);
    OLED_ShowHexNum(2, 12, ArrayWrite[3], 2);
    
    W25Q64_ReadData(Address, ArrayRead, Length);
    OLED_ShowHexNum(3, 3, ArrayRead[0], 2);
    OLED_ShowHexNum(3, 6, ArrayRead[1], 2);
    OLED_ShowHexNum(3, 9, ArrayRead[2], 2);
    OLED_ShowHexNum(3, 12, ArrayRead[3], 2);
    while(1)
    {
        
    }
}
