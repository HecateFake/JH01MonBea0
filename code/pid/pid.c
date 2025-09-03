/*
 * pid.c
 *
 *  Created on: 2024年12月9日
 *      Author: Hecate
 *      Update: 2025年9月2日
 *
 * @file pid.c
 * @brief PID控制器算法实现
 *
 * 本文件实现了完整的PID控制算法，包括位置式PID和增量式PID两种模式。
 * 支持自动模式选择、积分限幅、参数调节等功能，适用于各种实时控制系统。
 *
 * @version 1.1
 * @date 2025-09-02
 *
 * @details 算法特点：
 *          - 位置式PID：直接输出控制量，适合大多数控制系统
 *          - 增量式PID：输出控制增量，适合累积输出系统
 *          - 积分限幅：防止积分饱和问题
 *          - 除零保护：确保Ki=0时的安全性
 *          - 函数指针：高效的算法选择机制
 */
#include "pid.h"

#include <math.h>

/**
 * @brief 增量式PID控制算法处理函数
 *
 * 此函数实现增量式PID控制算法。增量式PID输出控制增量而非绝对控制值，
 * 适用于需要累积控制输出的系统，如步进电机控制等。
 *
 * @param obj 指向PID控制器对象的指针，包含所有PID参数和状态变量
 * @param currentValue 当前系统输出值/反馈值
 *
 * @details 算法实现：
 *          - 误差计算：error = expectedValue - currentValue
 *          - 增量计算：delta = kp*(e[k]-e[k-1]) + ki*e[k] + kd*(e[k]-2*e[k-1]+e[k-2])
 *          - 控制输出累积：controlValue += delta
 *
 * @note 增量式PID的优点：
 *       - 不会产生积分饱和
 *       - 计算机故障时影响较小
 *       - 手动/自动切换时冲击较小
 */
static void incrementalPidProcessHandler(PID *obj, float currentValue)
{
    // 误差计算
    obj->error = obj->expectedValue - currentValue;

    // 定义增量并计算
    float delta =
        obj->kp * (obj->error - obj->lastError)                                // 比例
        + obj->ki * obj->error                                                 // 积分
        + obj->kd * (obj->error - 2 * obj->lastError + obj->beforeLastError);  // 微分

    // 增量累加为控制值
    obj->controlValue += delta;

    // 误差存储
    obj->beforeLastError = obj->lastError;
    obj->lastError = obj->error;
}

/**
 * @brief 位置式PID控制算法处理函数
 *
 * 此函数实现位置式PID控制算法。位置式PID直接输出控制值，
 * 适用于大多数控制系统，如温度控制、速度控制等。
 *
 * @param obj 指向PID控制器对象的指针，包含所有PID参数和状态变量
 * @param currentValue 当前系统输出值/反馈值
 *
 * @details 算法实现：
 *          - 误差计算：error = expectedValue - currentValue
 *          - 积分累积：integral += error
 *          - 积分限幅：防止积分饱和
 *          - 控制输出：controlValue = kp*e + ki*∑e + kd*(e[k]-e[k-1])
 *
 * @note 位置式PID特点：
 *       - 直接输出控制量
 *       - 需要积分限幅防止饱和
 *       - 每次输出都与整个过去的状态有关
 */
static void positionalPidHandler(PID *obj, float currentValue)
{
    // 误差计算
    obj->error = obj->expectedValue - currentValue;

    // 积分累加
    obj->integral += obj->error;

    // 积分限幅 - 添加除零保护
    if (obj->ki != 0.0f)
    {
        if (obj->integral > obj->integralMax) obj->integral = obj->integralMax;
        else if (obj->integral < -obj->integralMax) obj->integral = -obj->integralMax;
    }

    // 控制值计算
    obj->controlValue =
        obj->kp * obj->error                        // 比例
        + obj->ki * obj->integral                   // 积分
        + obj->kd * (obj->error - obj->lastError);  // 微分

    // 误差存储
    obj->lastError = obj->error;
}

/**
 * @brief PID算法处理函数指针数组
 *
 * 根据PidMode枚举值作为索引，选择对应的PID算法处理函数。
 * 这种设计避免了条件分支，提高了执行效率。
 *
 * @note 数组索引对应关系：
 *       - [0] positionMode  -> positionalPidHandler
 *       - [1] incrementMode -> incrementalPidProcessHandler
 */
static const void (*pidHandlers[])(PID*, float) = {
    positionalPidHandler,        // 位置式PID处理函数
    incrementalPidProcessHandler // 增量式PID处理函数
};

/**
 * @brief PID控制器主处理函数
 *
 * 此函数根据PID控制器的模式选择相应的算法进行处理。
 * 支持位置式和增量式两种PID算法。
 *
 * @param obj 指向PID控制器对象的指针
 * @param currentValue 当前系统输出值/反馈值
 *
 * @details 根据obj->pidMode的值选择处理方式：
 *          - positionMode: 使用位置式PID算法
 *          - incrementMode: 使用增量式PID算法
 */
void pidProcess(PID *obj, float currentValue)
{
    pidHandlers[obj->pidMode](obj, currentValue);
}

/**
 * @brief 初始化PID控制器
 *
 * 此函数初始化PID控制器的所有参数和状态变量。
 * 根据积分限幅参数自动选择PID模式。
 *
 * @param integralMax 积分限幅值：
 *                    - 当integralMax = 0时，选择增量式PID（无积分限幅需求）
 *                    - 当integralMax > 0时，选择位置式PID（需要积分限幅）
 * @param obj 指向要初始化的PID控制器对象的指针
 * @param target 控制目标值/设定值
 * @param kp 比例增益系数
 *           - 决定系统的响应速度
 *           - 过大会导致超调和振荡
 *           - 过小会导致响应缓慢
 * @param ki 积分增益系数
 *           - 消除稳态误差
 *           - 过大会导致积分饱和和振荡
 *           - 过小会导致稳态误差消除缓慢
 * @param kd 微分增益系数
 *           - 预测误差变化趋势，提前纠正
 *           - 过大会放大噪声
 *           - 合适的值可以减少超调，提高稳定性
 *
 * @note 参数调节建议：
 *       1. 先调kp，使系统有合适的响应速度
 *       2. 再调ki，消除稳态误差
 *       3. 最后调kd，改善动态性能
 *
 * @implementation 实现细节：
 *                 - integralMax=0时自动选择增量式PID
 *                 - integralMax>0时自动选择位置式PID
 *                 - 所有状态变量都会被重置为0
 *                 - 确保初始状态的一致性
 */
void pidInit(float integralMax, PID *obj, float target, float kp, float ki, float kd)
{
    // 根据积分限幅参数自动选择PID模式
    if (integralMax == 0)
        obj->pidMode = incrementMode;  // 增量式PID（无积分限幅）
    else
        obj->pidMode = positionMode;   // 位置式PID（需要积分限幅）

    // 设置控制参数
    obj->expectedValue = target;
    obj->kp = kp;
    obj->ki = ki;
    obj->kd = kd;
    if (ki) obj->integralMax = fabs(integralMax/ki);
    else obj->integralMax = 0.0f; // 避免除零错误

    // 初始化所有状态变量为0，确保干净的起始状态
    obj->lastError = 0;
    obj->beforeLastError = 0;
    obj->integral = 0;
    obj->controlValue = 0;
    obj->error = 0;
}

/**
 * @brief 设置PID控制器的目标值
 *
 * 此函数用于在运行时更新PID控制器的目标设定值，
 * 常用于目标值动态变化的控制场景。
 *
 * @param obj 指向PID控制器对象的指针
 * @param targetValue 新的目标设定值
 *
 * @note 目标值变化时，系统会产生新的误差，
 *       PID控制器将自动调整输出以跟踪新目标
 */
void pidSetTarget(PID *obj, float targetValue)
{
    obj->expectedValue = targetValue;
}
