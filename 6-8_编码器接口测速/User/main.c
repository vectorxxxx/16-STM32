#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Timer.h"
#include "Encoder.h"

int16_t Location;
int16_t LastLocation;
int16_t Speed;

int main(void)
{
    OLED_Init();
    Timer_Init();
    Encoder_Init();
    
    OLED_ShowString(1, 1, "Location:");
    OLED_ShowString(2, 1, "Speed:");
    
    while(1)
    {
        Location = Encoder_Get();
        Speed = Location - LastLocation;
        
        OLED_ShowSignedNum(1, 10, Location, 5);
        OLED_ShowSignedNum(2, 7, Speed, 5);
    }
}

void TIM2_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
    {
        LastLocation = Encoder_Get();
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}
