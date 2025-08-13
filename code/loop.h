/*
 * loop.h
 *
 *  Created on: 2025年3月6日
 *      Author: Hecate
 */

#ifndef CODE_LOOP_H_
#define CODE_LOOP_H_

#include <stdint.h>

#include "pid.h"
#include "ladrc.h"
#include "lowPassFilter.h"
#include "schmitt.h"

extern uint8_t state;        // 直立使能标识位
extern uint8_t visionState;  // 视觉处理使能标识位
extern uint8_t testState;    // 测试状态标识位

extern float dynamicXeroPointGain0;  // 右转压弯增益
extern float dynamicXeroPointGain1;  // 左转压弯增益

extern float pitMid;
extern float rolMid;

extern float imageAreaMin;
extern float imageAreaMax;

extern float upDeadZone;
extern float downDeadZone;
extern float leftDeadZone;
extern float rightDeadZone;

extern float originVisionState;

extern float lStAng;
extern float rStAng;
extern float lReAng;
extern float rReAng;
extern float lturnAng;
extern float rturnAng;
extern float lOutAng;
extern float rOutAng;

extern float yawPwmMax;

extern float pitOmeP;
extern float pitOmeI;
extern float pitAngP;
extern float pitVelP;
extern float pitVelI;

extern float rolOmeP;
extern float rolOmeI;
extern float rolAngP;
extern float rolVelP;
extern float rolVelI;

extern float schRealRolHigh;
extern float schRealRolLow;
extern float schWantRolHigh;
extern float schWantRolLow;
extern float schYawErrHigh;
extern float schYawErrLow;
extern float schVidioCenterHigh;
extern float schVidioCenterLow;

extern PID pitOme;  // 俯仰角速度环
extern PID pitAng;  // 俯仰角度环
extern PID pitVel;  // 俯仰速度环

extern PID rolOme;  // 横滚角速度环
extern PID rolAng;  // 横滚角度环
extern PID rolVel;  // 横滚速度环

extern PID yawOme;  // 偏航角速度环

extern LowPassFilterData rolRpm;  // 飞轮转速
extern LowPassFilterData pitRpm;  // 行进轮转速

extern SchmittTrigger rYawAng;
extern SchmittTrigger eYawOme;
extern SchmittTrigger sYawErr;
extern SchmittTrigger vCenter;

extern float sPwm;     // 行进轮pwm输出占空比
extern float rRolPwm;  // 右飞轮pwm输出占空比
extern float lRolPwm;  // 左飞轮pwm输出占空比

#define buzzerPin P33_10  // 蜂鸣器引脚

extern float pitVelTar;  // 控制期望行进速度接口
extern float yawOmeTar;  // 控制期望角速度接口

extern uint8_t buzzerState;  // 蜂鸣器开关接口（0为关，1为开）

#define tRad(x) (x) * 0.01745329252f
#define tDeg(x) (x) * 57.29577951f

void controllerInit(void);  // 控制器初始化函数

void pit0(void);  // 定时器中断函数0
void pit1(void);  // 定时器中断函数1
void pit2(void);  // 定时器中断函数2
void pit3(void);  // 定时器中断函数3

#endif /* CODE_LOOP_H_ */
