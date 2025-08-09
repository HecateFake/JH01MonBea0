/*
 * imu.c
 *
 *  Created on: 2025年3月6日
 *      Author: ROG
 */
#include "imu.h"

#include "zf_driver_gpio.h"
#include "zf_driver_delay.h"
#include "zf_device_imu660rx.h"

#include "loop.h"
#include "lowPassFilter.h"
#include "mahony.h"

postureData imuData = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};                 // 定义姿态结构体
static float quaternion[4] = {1, 0, 0, 0};  // 定义姿态四元数

// 零漂计算函数
void getOffset(postureData *obj)
{
    // 计数开始蜂鸣
    gpio_set_level(buzzerPin, GPIO_HIGH);
    system_delay_ms(20);
    gpio_set_level(buzzerPin, GPIO_LOW);

    system_delay_ms(10);

    for (int i = 0; i < offsetCount; i++)
    {
        imu660rx_get_gyro();
        obj->xOffset += imu660rx_gyro_x;
        obj->yOffset += imu660rx_gyro_y;
        obj->zOffset += imu660rx_gyro_z;
        system_delay_ms(offsetT);
    }
    obj->xOffset /= offsetCount;
    obj->yOffset /= offsetCount;
    obj->zOffset /= offsetCount;
    system_delay_ms(10);

    // 计数结束蜂鸣
    gpio_set_level(buzzerPin, GPIO_HIGH);
    system_delay_ms(20);
    gpio_set_level(buzzerPin, GPIO_LOW);
}

// 四元数转换函数
static void quat2euler(float quat[4], float *phi, float *theta, float *psi)
{
    float R[3][3];
    R[0][0] = 1 - 2 * (quat[2] * quat[2] + quat[3] * quat[3]);
    R[0][1] = 2 * (quat[1] * quat[2] - quat[0] * quat[3]);
    R[0][2] = 2 * (quat[0] * quat[2] + quat[1] * quat[3]);
    R[1][0] = 2 * (quat[1] * quat[2] + quat[0] * quat[3]);
    R[1][1] = 1 - 2 * (quat[1] * quat[1] + quat[3] * quat[3]);
    R[1][2] = 2 * (quat[2] * quat[3] - quat[0] * quat[1]);
    R[2][0] = 2 * (quat[1] * quat[3] - quat[0] * quat[2]);
    R[2][1] = 2 * (quat[0] * quat[1] + quat[2] * quat[3]);
    R[2][2] = 1 - 2 * (quat[1] * quat[1] + quat[2] * quat[2]);

    *phi = atan2(R[2][1], R[2][2]);
    *theta = asin(-R[2][0]);
    *psi = atan2(R[1][0], R[0][0]);
}

// 姿态解算函数
void postureProcess(postureData *obj)
{
    imu660rx_get_acc();
    imu660rx_get_gyro();

    obj->ax = (float) imu660rx_acc_x * 4.880429477794045876e-4f;
    obj->ay = (float) imu660rx_acc_y * 4.880429477794045876e-4f;
    obj->az = (float) imu660rx_acc_z * 4.880429477794045876e-4f;

    obj->gx = (imu660rx_gyro_x - obj->xOffset) * 1.220509966e-3f;
    obj->gy = (imu660rx_gyro_y - obj->yOffset) * 1.220509966e-3f;
    obj->gz = (imu660rx_gyro_z - obj->zOffset) * 1.220509966e-3f;

    MahonyAHRSupdateIMU(quaternion, obj->gx, obj->gy, obj->gz, obj->ax, obj->ay, obj->az);  // 马哈尼滤波
    quat2euler(quaternion, &obj->pitch, &obj->roll, &obj->yaw);                             // 四元数转换欧拉角
}
