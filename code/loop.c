/*
 * loop.c
 *
 *  Created on: 2025年3月6日
 *      Author: Hecate
 */
#include "loop.h"

#include "magnaticEncoder.h"
#include "imu.h"
#include "camera.h"

#include "zf_device_uart_receiver.h"
#include "zf_driver_gpio.h"
#include "small_driver_uart_control.h"

// 定义控制器--------------------------------------------------------------------------------------------------------
PID pitOme;  // 俯仰角速度环
PID pitAng;  // 俯仰角度环
PID pitVel;  // 俯仰速度环

PID rolOme;  // 横滚角速度环
PID rolAng;  // 横滚角度环
PID rolVel;  // 横滚速度环

PID yawOme;  // 偏航角速度环

LowPassFilterData rolRpm;  // 飞轮转速
LowPassFilterData pitRpm;  // 行进轮转速

SchmittTrigger rYawAng;
SchmittTrigger eYawOme;
SchmittTrigger sYawErr;
SchmittTrigger vCenter;
//--------------------------------------------------------------------------------------------------------

// 控制器初始化函数--------------------------------------------------------------------------------------------------------
static float rolSafeRange[2] = {-2.0f, -1.0f};  // 定义横滚角安全范围
static float pitSafeRange[2] = {-0.5f, 0.5f};   // 定义俯仰角安全范围

#define angPeriodMultiple 3  // 角度环周期倍率
#define velPeriodMultiple 3  // 角速度环周期倍率

#define deadZone 800.0f  // 行进轮死区

float dynamicXeroPointGain1 = 0.0025f;  // 左转压弯增益
float dynamicXeroPointGain0 = 0.0025f;  // 右转压弯增益

float pitMid = 0.047f;
float rolMid = -1.535f;

float imageAreaMinLow = 3.0f;
float imageAreaMaxLow = 600.0f;
float imageAreaMinHigh = 0.0f;
float imageAreaMaxHigh = 0.0f;

float upDeadZone = 0.08f;     // 图像处理死区
float downDeadZone = 0.29f;   // 图像处理死区
float leftDeadZone = 0.08f;   // 图像处理死区
float rightDeadZone = 0.00f;  // 图像处理死区

float lStAng = 11.0f;
float rStAng = 11.0f;
float lReAng = 10.5f;
float rReAng = 10.5f;
float lturnAng = 10.5f;
float rturnAng = 10.0f;
float lOutAng = 17.5f;
float rOutAng = 17.5f;

float yawPwmMax = 3000.0f;

float pitOmeP = -800.0f;
float pitOmeI = -57.0f;
float pitAngP = 2.5f;
float pitVelP = -0.0026f;
float pitVelI = -0.00000f;

float rolOmeP = 1100.0f;
float rolOmeI = 120.0f;
float rolAngP = 3.0f;
float rolVelP = -0.060f;
float rolVelI = -0.00000f;

float schRealRolHigh = 8.0f;
float schRealRolLow = 6.0f;
float schWantRolHigh = 1.0f;
float schWantRolLow = 0.8f;
float schYawErrHigh = 0.56f;
float schYawErrLow = 0.36f;
float schVidioCenterHigh = 95.0f;
float schVidioCenterLow = 65.0f;

float originVisionState = 1.0f;

uint8_t testState = 0;
uint8_t startState = 1;

float sPwm = 0;     // 行进轮pwm输出占空比
float rRolPwm = 0;  // 右飞轮pwm输出占空比
float lRolPwm = 0;  // 左飞轮pwm输出占空比
void controllerInit(void)
{
    startState = 1;
    state = 0;
    visionState = 0;   // 视觉状态初始化为0
    buzzerState = 0;   // 蜂鸣器关闭
    pitVelTar = 0.0f;  // 行进速度传递变量
    yawOmeTar = 0.0f;  // 偏航速度传递变量

    sEncCleCon();  // 清除行进轮编码器计数

    mt9v03x_init();
    twoPassConnectedAreaInit(&beaInf, BLACK, &binaryBuffer, &binaryShow, imageAreaMinLow, imageAreaMaxLow, imageAreaMinHigh, imageAreaMaxHigh, leftDeadZone, rightDeadZone, upDeadZone, downDeadZone);  // 初始化连通区域检测

    magnaticEncoderPhaseLockedLoopInit(150.0f, 0.25f, 0.008f);  // 磁编码器锁相环初始化

    lowPassFilterInit(&rolRpm, 0.1f);  // 飞轮转速低通滤波器初始化
    lowPassFilterInit(&pitRpm, 0.1f);  // 行进轮转速低通滤波器初始化

    pidInit(0.0f, &pitOme, 0.0f, pitOmeP, pitOmeI, -0.0f);  // 俯仰角速度环初始化
    pidInit(1.0f, &pitAng, pitMid, pitAngP, 0.0f, 0.0f);    // 俯仰角度环初始化
    pidInit(0.1f, &pitVel, 0.0f, pitVelP, pitVelI, -0.0f);  // 俯仰速度环初始化

    pidInit(0.0f, &rolOme, 0.0f, rolOmeP, rolOmeI, 0.0f);      // 横滚角速度环初始化
    pidInit(1.0f, &rolAng, rolMid, rolAngP, 0.0f, 0.0f);       // 横滚角度环初始化
    pidInit(1000.0f, &rolVel, 0.0f, rolVelP, rolVelI, -0.0f);  // 横滚速度环初始化

    schmittInit(&rYawAng, schRealRolHigh, schRealRolLow, 0);
    schmittInit(&eYawOme, schWantRolHigh, schWantRolLow, 0);
    schmittInit(&sYawErr, schYawErrHigh, schYawErrLow, 1);
    schmittInit(&vCenter, schVidioCenterHigh, schVidioCenterLow, (uint8_t) originVisionState);

    pidInit(1, &yawOme, 0.0f, -4000.0f, -0.0f, -0.0f);  // 偏航角速度环初始化

    sPwm = 0;
    lRolPwm = 0;
    rRolPwm = 0;
}
//--------------------------------------------------------------------------------------------------------

// 速度控制期望--------------------------------------------------------------------------------------------------------
float pitVelTar = 0;  // 行进速度传递变量
float yawOmeTar = 0;  // 偏航速度传递变量
//--------------------------------------------------------------------------------------------------------

// 失控判断函数(返回布尔值)--------------------------------------------------------------------------------------------------------
static inline int outOfControlSafetyDetection(postureData *obj)
{
    if (obj->pitch < rolSafeRange[0] || obj->pitch > rolSafeRange[1]) return TRUE;
    else if (obj->roll < pitSafeRange[0] || obj->roll > pitSafeRange[1]) return TRUE;
    else return FALSE;
}
//--------------------------------------------------------------------------------------------------------

// 动态零点计算函数--------------------------------------------------------------------------------------------------------
static inline float Dynamic_control(float Speed, float Target_TurnAngleSpeed)
{
    if (Target_TurnAngleSpeed > 0) return Speed * Target_TurnAngleSpeed * -dynamicXeroPointGain0;
    else return Speed * Target_TurnAngleSpeed * -dynamicXeroPointGain1;
}
//--------------------------------------------------------------------------------------------------------

// 控制闭环内部调用函数--------------------------------------------------------------------------------------------------------
static inline void getVel(void)
{
    // 获取行进轮转速
    lowPassFilter(&pitRpm, (float) sEncGetCon());
    sEncCleCon();
}
// pid速度环 9毫秒一次调用
static inline void velLoop(void)
{
    pidSetTarget(&pitVel, pitVelTar);  // 传递期望速度

    // 计算俯仰速度环
    pidProcess(&pitVel, pitRpm.filteredValue);

    // 计算横滚速度环
    pidProcess(&rolVel, rolRpm.filteredValue * 45.0f);
}
//--------------------------------------------------------------------------------------------------------
// pid角度环 3毫秒一次调用
static inline void angLoop(void)
{
    // 传递外环控制值
    float tPitAng = pitMid + pitVel.controlValue;

    schmittProcess(&rYawAng, fabs(tDeg(imuData.pitch - rolMid)));
    schmittProcess(&eYawOme, fabs(yawOmeTar));

    if (!testState)
    {
        if (yawOmeTar <= 0)
        {
            buzzerState = 0;
            if (rYawAng.outputState == 0 && eYawOme.outputState == 1)
                pidSetTarget(&pitAng, tPitAng - tRad(lReAng));
            else if (rYawAng.outputState == 0 && eYawOme.outputState == 0)
                pidSetTarget(&pitAng, tPitAng - tRad(lStAng));
            else if (rYawAng.outputState == 1 && eYawOme.outputState == 0)
                pidSetTarget(&pitAng, tPitAng - tRad(lOutAng));
            else if (rYawAng.outputState == 1 && eYawOme.outputState == 1)
            {
                pidSetTarget(&pitAng, tPitAng - tRad(lturnAng));
                buzzerState = 1;
            }
        }
        else if (yawOmeTar > 0)
        {
            buzzerState = 0;
            if (rYawAng.outputState == 0 && eYawOme.outputState == 1)
                pidSetTarget(&pitAng, tPitAng - tRad(rReAng));
            else if (rYawAng.outputState == 0 && eYawOme.outputState == 0)
                pidSetTarget(&pitAng, tPitAng - tRad(rStAng));
            else if (rYawAng.outputState == 1 && eYawOme.outputState == 0)
                pidSetTarget(&pitAng, tPitAng - tRad(rOutAng));
            else if (rYawAng.outputState == 1 && eYawOme.outputState == 1)
            {
                pidSetTarget(&pitAng, tPitAng - tRad(rturnAng));
                buzzerState = 1;
            }
        }
    }
    else pidSetTarget(&pitAng, tPitAng);

    pidSetTarget(&rolAng, rolMid + 0.0001f * rolVel.controlValue + Dynamic_control(70.0f, yawOmeTar));

    // 计算角度环pid
    pidProcess(&pitAng, imuData.roll);
    pidProcess(&rolAng, imuData.pitch);
}
//--------------------------------------------------------------------------------------------------------
// pid角速度环 1毫秒一次调用
static inline void omeLoop(void)
{
    // 传递外环控制值
    pidSetTarget(&pitOme, pitAng.controlValue);
    pidSetTarget(&rolOme, rolAng.controlValue);
    pidSetTarget(&yawOme, yawOmeTar);

    // 计算角速度环pid
    pidProcess(&pitOme, imuData.gz);
    pidProcess(&rolOme, imuData.gx);
    pidProcess(&yawOme, imuData.gy);
}
// 电机控制函数--------------------------------------------------------------------------------------------------------
#define rolPwmMax 10000.0f
static inline void ultimateMotorControl(void)
{
    // 行进轮控制值叠加死区
    if (fabs(pitOme.controlValue) < deadZone)
    {
        if (pitOme.controlValue >= 0) sPwm = deadZone;
        else sPwm = -deadZone;
    }
    else sPwm = pitOme.controlValue;

    // 偏航角差速控制
    if (fabs(yawOme.controlValue) > yawPwmMax)
    {
        if (yawOme.controlValue > 0)
        {
            rRolPwm = rolOme.controlValue + yawPwmMax;
            lRolPwm = rolOme.controlValue - yawPwmMax;
        }
        else
        {
            rRolPwm = rolOme.controlValue - yawPwmMax;
            lRolPwm = rolOme.controlValue + yawPwmMax;
        }
    }
    else  // 差速限幅
    {
        rRolPwm = rolOme.controlValue + yawOme.controlValue;
        lRolPwm = rolOme.controlValue - yawOme.controlValue;
    }

    // 飞轮转速限幅
    if (rRolPwm > rolPwmMax) rRolPwm = rolPwmMax;
    else if (rRolPwm < -rolPwmMax) rRolPwm = -rolPwmMax;
    if (lRolPwm > rolPwmMax) lRolPwm = rolPwmMax;
    else if (lRolPwm < -rolPwmMax) lRolPwm = -rolPwmMax;
}
//--------------------------------------------------------------------------------------------------------

// 定时器中断函数--------------------------------------------------------------------------------------------------------
// 定时器中断函数0
uint8_t state = 0;        // 直立使能标识位
uint8_t visionState = 0;  // 视觉处理使能标识位
static short timer = 0;   // 定时器中断时间标识位
uint8_t buzzerState = 0;
static uint8_t buzzerCount = 0;
void pit0(void)
{
    timer++;  // 计时标识自增

    postureProcess(&imuData);  // 姿态解算
    if (timer == angPeriodMultiple * velPeriodMultiple) getVel();

    if (state)
    {
        if (timer == angPeriodMultiple * velPeriodMultiple) velLoop();

        if (timer % angPeriodMultiple == 0) angLoop();

        omeLoop();

        ultimateMotorControl();

        if (outOfControlSafetyDetection(&imuData)) controllerInit();
    }

    // 设置电机转速
    sMotorSetting(sPwm, 10000);
    small_driver_set_duty(rRolPwm, -lRolPwm);

    if (timer == angPeriodMultiple * velPeriodMultiple) timer = 0;  // 时间标识位清零

    key_scanner();  // 按键扫描函数

    if (buzzerState == 0)
    {
        gpio_low(buzzerPin);
        buzzerCount = 0;
    }
    else
    {
        buzzerCount++;
        if (buzzerCount == 50 || buzzerCount == 60)
        {
            if (gpio_get_level(buzzerPin) == GPIO_LOW) gpio_high(buzzerPin);
            else
            {
                gpio_low(buzzerPin);
                buzzerCount = 0;
            }
        }
    }
}
//--------------------------------------------------------------------------------------------------------
// 定时器中断函数1
void pit1(void)
{
    magnaticEncoderGetData();
    lowPassFilter(&rolRpm, estMagnaticVel);
}
//--------------------------------------------------------------------------------------------------------
// 定时器中断函数2
void pit2(void)
{
}
//--------------------------------------------------------------------------------------------------------
// 定时器中断函数3
void pit3(void)
{
}
//--------------------------------------------------------------------------------------------------------
