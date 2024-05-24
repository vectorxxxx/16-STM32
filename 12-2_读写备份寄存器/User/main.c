#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Key.h"
#include "Delay.h"

uint8_t KeyNum;
uint16_t ArrayWrite[] = {0x1234, 0x5678};
uint16_t ArrayRead[4];

int main(void)
{
    OLED_Init();
    Key_Init();

    // 设置RCC_APB1ENR的PWREN和BKPEN，使能PWR和BKP时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);
    
	// 设置PWR_CR的DBP，使能对BKP和RTC的访问
    PWR_BackupAccessCmd(ENABLE);
    
    OLED_ShowString(1, 1, "W:");   
    OLED_ShowString(2, 1, "R:");
    
    while(1)
    {
        KeyNum = Key_GetNum();
        if (KeyNum == 1)
        {
            BKP_WriteBackupRegister(BKP_DR1, ArrayWrite[0]);
            BKP_WriteBackupRegister(BKP_DR2, ArrayWrite[1]);
            OLED_ShowHexNum(1, 3, ArrayWrite[0], 4);
            OLED_ShowHexNum(1, 8, ArrayWrite[1], 4);
            
            ArrayWrite[0]++;
            ArrayWrite[1]++;
        }
        
        Delay_ms(500);
        
        ArrayRead[0] = BKP_ReadBackupRegister(BKP_DR1);
        ArrayRead[1] = BKP_ReadBackupRegister(BKP_DR2);
        OLED_ShowHexNum(2, 3, ArrayRead[0], 4);
        OLED_ShowHexNum(2, 8, ArrayRead[1], 4);
        
    }
}
