#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "MyRTC.h"


int main(void)
{
    OLED_Init();
    MyRTC_Init();
    
    OLED_ShowString(1, 1, "Date: yyyy-mm-dd");
    OLED_ShowString(2, 1, "Time: HH:MM:SS");
    OLED_ShowString(3, 1, "CNT :");
    OLED_ShowString(4, 1, "DIV :");
    
    while(1)
    {
        MyRTC_GetDateTime();
        
        
        OLED_ShowNum(1, 7, MyRTC_DateTime[0], 4);
        OLED_ShowNum(1, 12, MyRTC_DateTime[1], 2);
        OLED_ShowNum(1, 15, MyRTC_DateTime[2], 2);
        OLED_ShowNum(2, 7, MyRTC_DateTime[3], 2);
        OLED_ShowNum(2, 10, MyRTC_DateTime[4], 2);
        OLED_ShowNum(2, 13, MyRTC_DateTime[5], 2);
        
        OLED_ShowNum(3, 7, RTC_GetCounter(), 10);
        
        //OLED_ShowNum(4, 7, (32767 - RTC_GetDivider()) / 32767.0 * 999, 10);
        OLED_ShowNum(4, 7, RTC_GetDivider(), 10);
        
    }
}
