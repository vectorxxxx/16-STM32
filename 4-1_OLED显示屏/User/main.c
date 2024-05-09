#include "stm32f10x.h"                  // Device header
#include "OLED.h"

int main(void)
{
    OLED_Init();
    OLED_ShowChar(1,1,'A');
    OLED_ShowString(1,3,"Hello World!");
    OLED_ShowNum(2,1,342345,6);
    OLED_ShowSignedNum(2,8,-644,4);
    OLED_ShowHexNum(3,1,0xBF18,4);
    OLED_ShowBinNum(4,1,0xBF18,16);
    while(1)
    {

    }
}
