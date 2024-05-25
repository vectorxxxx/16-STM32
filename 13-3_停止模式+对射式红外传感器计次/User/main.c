#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "CountSensor.h"
#include "Delay.h"

int main(void)
{
    OLED_Init();
    CountSensor_Init();
    
    // 1.8V供电区域的所有时钟都被停止，PLL、HSI和HSE被禁止，SRAM和寄存器内容被保留下来
    // 因此开启PWR时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    
    OLED_ShowString(1,1,"Count:");
    
    while(1)
    {
        OLED_ShowNum(1,7,CountSensor_Get(),5);
        
        OLED_ShowString(2, 1, "RUNNING");
        Delay_ms(100);
        OLED_ShowString(2, 1, "       ");
        Delay_ms(100);
        
        
        // 停机模式：电压调节器开启，任一外部中断唤醒
        PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI);
        // 当一个中断或唤醒事件导致退出停止模式时，HSI被选为系统时钟
        // 因此需重新初始化系统时钟，其内部调用了SetSysClock
        SystemInit();
    }
}
