#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "MyRTC.h"
#include "Delay.h"


int main(void)
{
    OLED_Init();
    MyRTC_Init();
    
    OLED_ShowString(1, 1, "CNT :");
    OLED_ShowString(2, 1, "ALR :");
    OLED_ShowString(3, 1, "ALRF:");

    
    // 整个1.8V供电区域被断电，PLL、HSI和HSE也被断电，SRAM和寄存器内容丢失，只有备份的寄存器和待机电路维持供电
    // 尽管 MyRTC_Init(); 会开启一次 PWR 时钟，但是不建议省略。因为依赖性变高，一旦不再使用 RTC 了，就会出现隐形 Bug
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    // 在待机模式下，所有的I/O引脚变为高阻态（浮空输入）
    // 4.4.2 电源控制/状态寄存器(PWR_CSR): WKUP引脚用于将CPU从待机模式唤醒， WKUP引脚被强置为输入下拉的配置(WKUP引脚上的上升沿将系统从待机模式唤醒)
    PWR_WakeUpPinCmd(ENABLE);
    
    
    // 设置RTC闹钟
    uint32_t Alarm = RTC_GetCounter() + 5;
    RTC_SetAlarm(Alarm);
    OLED_ShowNum(2, 6, Alarm, 10);
    
    
    while(1)
    {
        OLED_ShowNum(1, 6, RTC_GetCounter(), 10);
        OLED_ShowNum(3, 6, RTC_GetFlagStatus(RTC_FLAG_ALR), 1);
        
        OLED_ShowString(4, 1, "RUNNING");
        Delay_ms(100);
        OLED_ShowString(4, 1, "       ");
        Delay_ms(100);
        
        
        // 模拟关闭外挂设备
        OLED_ShowString(4, 9, "STANDBY");
        Delay_ms(1000);
        OLED_ShowString(4, 9, "       ");
        Delay_ms(100);
        OLED_Clear();
        
        
        // 待机模式
        // 执行完WFI/WFE指令后，STM32进入待机模式，唤醒后程序从头开始运行
        // WKUP引脚的上升沿、RTC闹钟事件的上升沿、NRST引脚上外部复位、IWDG复位退出待机模式
        PWR_EnterSTANDBYMode();
    }
}
