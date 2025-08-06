/*
 * motor.c
 *
 *  Created on: 2025年3月4日
 *      Author: Hecate
 */
#include "zf_common_headfile.h"

// 有刷电机设置函数
void motorInit(void)
{
    gpio_init(singleMotorDir, GPO, GPIO_LOW, GPO_PUSH_PULL); // 有刷电机方向引脚初始化
    pwm_init(singleMotorPwm, 17000, 000);                    // 有刷电机pwm引脚初始化
}

void motorSetting(float target, float max, gpio_pin_enum dirPin, pwm_channel_enum pwmPin)
{
    // 正转
    if (target >= 0)
    {
        gpio_set_level(dirPin, GPIO_HIGH); // 方向设置
        if (target > max)
            target = max;                     // 限制占空比
        pwm_set_duty(pwmPin, (uint32)target); // 设置占空比
    }

    // 反转
    else if (target < 0)
    {
        gpio_set_level(dirPin, GPIO_LOW); // 方向设置
        if ((-target) > max)
            target = (-max);                     // 限制占空比
        pwm_set_duty(pwmPin, (uint32)(-target)); // 设置占空比
    }
}
