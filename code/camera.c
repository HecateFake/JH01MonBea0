/*
 * camera.c
 *
 *  Created on: 2025年4月30日
 *      Author: Hecate
 */
#include "camera.h"

#include "loop.h"
#include <string.h>
#include "openMorph.h"

uint8_t thresholdLow = 10;   // 二值化低阈值
uint8_t thresholdHigh = 40;  // 二值化高阈值

uint16_t visionCenter = 60;

float visionErrRange = 1.83f;
float vidionPitErr = 55.0f;

float pMin = 1.0f;
float pMax = 2.0f;

static inline float errGenerate(BEAINF* obj)
{
    if (obj->beaCount) return fRang((obj->sbea[obj->selectedIndex].beaX - visionCenter) * 0.01063829787234042553191489361702f * visionErrRange * (pMin + (pMax - pMin) * (obj->sbea[obj->selectedIndex].beaArea / obj->areaRange[1])), visionErrRange);
    else return (obj->lastFrameSelectedBeaXY[0] > 80) ? visionErrRange : -visionErrRange;
    return 0.0f;
}

uint8_t kernel[3 * 3] =
    {
        WHITE, WHITE, WHITE,
        WHITE, WHITE, WHITE,
        WHITE, WHITE, WHITE};                 // 定义3x3的结构元素
CroppedImage8 kernelStruct = {kernel, 3, 3};  // 定义结构元素的结构体

uint8_t binRel[MT9V03X_W * MT9V03X_H] = {0};
uint8_t binBuf[MT9V03X_W * MT9V03X_H] = {0};
uint8_t binSho[MT9V03X_W * MT9V03X_H] = {0};

CroppedImage8 binaryReal = {binRel, MT9V03X_W, MT9V03X_H};
CroppedImage8 binaryBuffer = {binBuf, MT9V03X_W, MT9V03X_H};
CroppedImage8 binaryShow = {binSho, MT9V03X_W, MT9V03X_H};

BEAINF beaInf;
void visionProcessMT9V034(void)
{
    if (mt9v03x_finish_flag)
    {
        memcpy(binBuf, mt9v03x_image[0], MT9V03X_W * MT9V03X_H * sizeof(uint8_t));
        toBinaryDoubleThreshold(&binaryBuffer, &binaryReal, thresholdHigh, thresholdLow);  // 二值化处理

        morphErode(&binaryReal, &kernelStruct, &binaryBuffer, BLACK);  // 腐蚀运算

        twoPassEightConnectedAreaProcess(&beaInf);  // 连通区域处理

        float err = errGenerate(&beaInf);

        schmittProcess(&sYawErr, err);

        yawOmeTar = visionState ? (sYawErr.outputState ? err : 0) : 0;
        pitVelTar = visionState ? (vidionPitErr) : 0;

        mt9v03x_finish_flag = 0;
    }
}
