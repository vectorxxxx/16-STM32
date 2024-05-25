#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Delay.h"
#include "Key.h"

uint8_t KeyNum;

int main(void)
{
    OLED_Init();
    Key_Init();
    
    OLED_ShowString(1, 1, "WWDG TEST");
    
    if (RCC_GetFlagStatus(RCC_FLAG_WWDGRST) == SET)
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

    // 开启PCLK1时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
    WWDG_SetPrescaler(WWDG_Prescaler_8);
    // 18.3 WWDG功能描述
    // 在系统复位后，看门狗总是处于关闭状态，设置WWDG_CR寄存器的WDGA位能够开启看门狗，随后它不能再被关闭，除非发生复位。
    // 递减计数器处于自由运行状态，即使看门狗被禁止，递减计数器仍继续递减计数。当看门狗被启用时， T6位必须被设置，以防止立即产生一个复位。
    // 其中：TPCLK1 = 1 / FPCLK1 = 1 / 36MHz
    // 窗口时间：TWIN = TPCLK1 × 4096 × WDGTB预分频系数 × (T[5:0] - W[5:0]) = 30ms
    // W[5:0] = 54 - 30 / (1 / 36MHz) / 4096 / 8 = 54 - 33 = 21;
    WWDG_SetWindowValue(0x40 | 21);
    // 超时时间：TWWDG = TPCLK1 × 4096 × WDGTB预分频系数 × (T[5:0] + 1) = 50ms
    // T[5:0] = 50ms / (1 / 36MHz) / 4096 / 8 - 1 ≈ 55 - 1 = 54
    WWDG_Enable(0x40 | 54);                              // T[6:0] = T[6] | T[5:0]
    
    while(1)
    {
        KeyNum = Key_GetNum();
        
        OLED_ShowString(4, 1, "FEED");
        Delay_ms(20);  
        OLED_ShowString(4, 1, "    ");
        Delay_ms(20);
        
        //Delay_ms(29);
        //Delay_ms(30);
        //Delay_ms(31);
        //Delay_ms(49);
        //Delay_ms(50);
        
        WWDG_SetCounter(0x40 | 54);
    }
}
