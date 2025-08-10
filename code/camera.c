/*
 * camera.c
 *
 *  Created on: 2025年4月30日
 *      Author: Hecate
 */
#include "camera.h"

#include <string.h>
#include <math.h>

#include "loop.h"
#include "openMorph.h"

float thresholdLow = 20;    // 二值化低阈值
float thresholdHigh = 255;  // 二值化高阈值

float visionCenter = 60;
float visionPolarity = 1.0f;

float visionErrRange = 1.83f;
float visionPitErr = 55.0f;

float pMin = 1.0f;
float pMax = 2.0f;

float areaSelected = 0.0f;

static inline float dynamicErrGain(BEAINF* obj)
{
    return (pMax - (pMax - pMin) * sqrtf((float) obj->sbea[obj->selectedIndex].beaArea / (float) obj->areaRange[1]));
}
static inline float errGenerate(BEAINF* obj)
{
    if (obj->beaCount) return fRang(((float)obj->sbea[obj->selectedIndex].beaX - visionCenter) * dynamicErrGain(obj) * visionErrRange * 0.0106383f, visionErrRange);
    else return (obj->lastFrameSelectedBeaXY[0] > 80) ? (visionPolarity ? visionErrRange : -visionErrRange) : (visionPolarity ? -visionErrRange : visionErrRange);
    return 0.0f;
}

uint8_t kernel[3 * 3] =
    {
        BLACK, WHITE, BLACK,
        WHITE, WHITE, WHITE,
        BLACK, WHITE, BLACK};                 // 定义3x3的结构元素
CroppedImage8 kernelStruct = {kernel, 3, 3};  // 定义结构元素的结构体

uint8_t binRel[MT9V03X_W * MT9V03X_H] = {0};
uint8_t binBuf[MT9V03X_W * MT9V03X_H] = {0};
uint8_t binSho[MT9V03X_W * MT9V03X_H] = {0};

CroppedImage8 binaryReal = {binRel, MT9V03X_W, MT9V03X_H};
CroppedImage8 binaryBuffer = {binBuf, MT9V03X_W, MT9V03X_H};
CroppedImage8 binaryShow = {binSho, MT9V03X_W, MT9V03X_H};

BEAINF beaInf;
float visionErr = 0.0f;
void visionProcessMT9V034(void)
{
    if (mt9v03x_finish_flag)
    {
        memcpy(binRel, mt9v03x_image[0], MT9V03X_W * MT9V03X_H * sizeof(uint8_t));
        toBinaryDoubleThreshold(&binaryReal, &binaryBuffer, (uint8_t) thresholdHigh, (uint8_t) thresholdLow);  // 二值化处理

        // morphErode(&binaryReal, &kernelStruct, &binaryBuffer, BLACK);  // 腐蚀运算

        twoPassEightConnectedAreaProcess(&beaInf);  // 连通区域处理

        areaSelected = (float) beaInf.sbea[beaInf.selectedIndex].beaArea;  // 计算选中区域的面积

        visionErr = errGenerate(&beaInf);

        schmittProcess(&sYawErr, visionErr);

        yawOmeTar = visionState ? (sYawErr.outputState ? visionErr : 0) : 0;
        pitVelTar = visionState ? (visionPitErr) : 0;

        mt9v03x_finish_flag = 0;
    }
}
