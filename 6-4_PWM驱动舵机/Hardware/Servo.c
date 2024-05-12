#include "stm32f10x.h"                  // Device header
#include "PWM.h"

/**
* @brief 舵机初始化
  * @param  无
  * @retval 无
  */
void Servo_Init(void)
{
    PWM_Init();
}

/**
  * @brief 设置舵机输出轴转角
  * @param  Angle 角度，范围0~180
  * @retval 无
  */
void Servo_SetAngle(float Angle)
{
    // 0°       0.5ms   500
    // 45°      1ms     1000
    // 90°      1.5ms   1500
    // 135°     2ms     2000
    // 180°     2.5ms   2500
    PWM_SetCompare2(500 + Angle / 180 * (2500 - 500));
}
