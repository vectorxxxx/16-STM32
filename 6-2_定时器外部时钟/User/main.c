#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Timer.h"

uint16_t Num;

int main(void)
{
    OLED_Init();
    Timer_Init();
    
    OLED_ShowString(1, 1, "Num:");
    OLED_ShowString(2, 1, "Count:");
    
    while(1)
    {
        OLED_ShowNum(1, 5, Num, 5);
        OLED_ShowNum(2, 7, Timer_GetCounter(), 5);
    }
}

/**
  * @brief TIM2定时中断函数   startup_stm32f10x_md.s
  * @param  无
  * @retval 无
  */
void TIM2_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
    {
        Num++;
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}
