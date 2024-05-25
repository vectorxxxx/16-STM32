#include "stm32f10x.h"                  // Device header
#include <time.h>

uint16_t MyRTC_DateTime[] = {2024, 5, 24, 22, 23, 0};

void MyRTC_SetDateTime(void);   // 需先声明再调用

/**
  * @brief RTC初始化
  * @param  无
  * @retval 无
  */
void MyRTC_Init(void)
{
    // 设置RCC_APB1ENR的PWREN和BKPEN，使能PWR和BKP时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);
    
	// 设置PWR_CR的DBP，使能对BKP和RTC的访问
    PWR_BackupAccessCmd(ENABLE);

    if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5) 
    {
        // 开启LSE外部低速时钟
        RCC_LSEConfig(RCC_LSE_ON);
        while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) != SET);
        
        // 选择LSE外部低速时钟作为RTCCLK
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
        // 使能RTCCLK
        RCC_RTCCLKCmd(ENABLE);

        // 若在读取RTC寄存器时，RTC的APB1接口曾经处于禁止状态，则软件首先必须等待RTC_CRL寄存器中的RSF位（寄存器同步标志）被硬件置1
        RTC_WaitForSynchro();
        // 对RTC任何寄存器的写操作，都必须在前一次写操作结束后进行。可以通过查询RTC_CR寄存器中的RTOFF状态位，判断RTC寄存器是否处于更新中。仅当RTOFF状态位是1时，才可以写入RTC寄存器
        RTC_WaitForLastTask();
            
        RTC_SetPrescaler(32768 - 1);
        RTC_WaitForLastTask();
        
        //RTC_SetCounter(1716554358);
        MyRTC_SetDateTime();
        
        BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
    }
    else 
    {
        // 若在读取RTC寄存器时，RTC的APB1接口曾经处于禁止状态，则软件首先必须等待RTC_CRL寄存器中的RSF位（寄存器同步标志）被硬件置1
        RTC_WaitForSynchro();
        // 对RTC任何寄存器的写操作，都必须在前一次写操作结束后进行。可以通过查询RTC_CR寄存器中的RTOFF状态位，判断RTC寄存器是否处于更新中。仅当RTOFF状态位是1时，才可以写入RTC寄存器
        RTC_WaitForLastTask();
    }
}

/**
  * @brief RTC获取时间
  * @param  无
  * @retval 无
  */
void MyRTC_GetDateTime(void)
{
    time_t time_cnt;
    struct tm time_date;
    
    time_cnt = RTC_GetCounter() + 8 * 60 * 60;
    time_date = *localtime(&time_cnt);
    
    MyRTC_DateTime[0] = time_date.tm_year + 1900;
    MyRTC_DateTime[1] = time_date.tm_mon + 1;
    MyRTC_DateTime[2] = time_date.tm_mday;
    MyRTC_DateTime[3] = time_date.tm_hour;
    MyRTC_DateTime[4] = time_date.tm_min;
    MyRTC_DateTime[5] = time_date.tm_sec;
}

/**
  * @brief RTC设置时间
  * @param  无
  * @retval 无
  */
void MyRTC_SetDateTime(void)
{
    time_t time_cnt;
    struct tm time_date;

    time_date.tm_year = MyRTC_DateTime[0] - 1900;
    time_date.tm_mon = MyRTC_DateTime[1] - 1;
    time_date.tm_mday = MyRTC_DateTime[2];
    time_date.tm_hour = MyRTC_DateTime[3];
    time_date.tm_min = MyRTC_DateTime[4];
    time_date.tm_sec = MyRTC_DateTime[5];
    
    time_cnt = mktime(&time_date);
    RTC_SetCounter(time_cnt - 8 * 60 * 60);
    
    RTC_WaitForLastTask();
}
