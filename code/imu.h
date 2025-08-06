/*
 * imu.h
 *
 *  Created on: 2025年3月6日
 *      Author: ROG
 */

#ifndef CODE_IMU_H_
#define CODE_IMU_H_

#define offsetT 5       // 零漂采样间隔(ms)
#define offsetCount 200 // 零漂采样次数
#define IMUsamplingT 1  // 采样间隔(ms)

// 定义姿态结构体类型
typedef struct
{
    float roll;
    float pitch;
    float yaw;

    float ax;
    float ay;
    float az;

    float gx;
    float gy;
    float gz;

    int xOffset;
    int yOffset;
    int zOffset;
} postureData;

extern postureData imuData; // 声明姿态结构体

void getOffset(postureData *obj);      // 零漂计算函数
void postureProcess(postureData *obj); // 姿态解算函数

#endif /* CODE_IMU_H_ */
