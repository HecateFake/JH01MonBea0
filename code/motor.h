/*
 * motor.h
 *
 *  Created on: 2025年3月4日
 *      Author: Hecate
 */

#ifndef CODE_MOTOR_H_
#define CODE_MOTOR_H_

#include "zf_driver_pwm.h"
#include "zf_driver_gpio.h"

#define singleMotorDir P21_5            // 行进轮方向引脚
#define singleMotorPwm ATOM0_CH2_P21_4  // 行进轮pwm引脚

#define singleEncoderTimer TIM6_ENCODER          // 行进轮编码器计时器
#define singleEncoderCh1 TIM6_ENCODER_CH1_P20_3  // 行进轮编码器引脚1
#define singleEncoderCh2 TIM6_ENCODER_CH2_P20_0  // 行进轮编码器引脚2

void motorInit(void);                                                                       // 有刷电机初始化函数
void motorSetting(float target, float max, gpio_pin_enum dirPin, pwm_channel_enum pwmPin);  // 有刷电机设置函数

#define sMotorSetting(x, y) (motorSetting(x, y, singleMotorDir, singleMotorPwm))

#define sEncGetCon() (float) encoder_get_count(singleEncoderTimer)
#define sEncCleCon() encoder_clear_count(singleEncoderTimer)

#endif /* CODE_MOTOR_H_ */
