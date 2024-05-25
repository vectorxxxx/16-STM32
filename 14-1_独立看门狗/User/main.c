#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Delay.h"
#include "Key.h"

uint8_t KeyNum;

int main(void)
{
    OLED_Init();
    Key_Init();
    
    OLED_ShowString(1, 1, "IWDG TEST");
    
    if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) == SET)
    {
        OLED_ShowString(2, 1, "IWDGRST");
        Delay_ms(200);  
        OLED_ShowString(2, 1, "       ");
        Delay_ms(100);
        
        // 必须要手动清除标志位
        RCC_ClearFlag();
    }
    else 
    {
        OLED_ShowString(3, 1, "RST");
        Delay_ms(200);
        OLED_ShowString(3, 1, "   ");
        Delay_ms(100);
    }

    // 解除IWDG_PR和IWDG_RLR的写保护(0x5555)
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    // 超时时间：TIWDG = TLSI × PR预分频系数 × (RL + 1)
    // 其中：TLSI = 1 / FLSI = 1 / 40kHz = 0.025ms
    // RL = 1000ms / 0.025ms / 16 - 1 = 2500 - 1
    IWDG_SetPrescaler(IWDG_Prescaler_16);           // 预分频
    IWDG_SetReload(2500 - 1);                       // 重装载
    IWDG_ReloadCounter();                           // 喂狗
    IWDG_Enable();                                  // 启用独立看门狗(0xCCCC)
    
    while(1)
    {
        KeyNum = Key_GetNum();
        
        IWDG_ReloadCounter();
        
        OLED_ShowString(4, 1, "FEED");
        Delay_ms(200);  
        OLED_ShowString(4, 1, "    ");
        Delay_ms(600);
    }
}
