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

float thresholdLow = 30;    // 二值化低阈值
float thresholdHigh = 255;  // 二值化高阈值

float visionCenter = 0.0f;
float visionCenterLeft = 60.0f;
float visionCenterRight = 107.0f;
float visionPolarity = 1.0f;

float visionErrRange = 1.89f;
float visionPitErr = 53.0f;

float pMin = 0.6f;
float pMax = 2.7f;

float areaSelected = 0.0f;
float BlobCount = 0.0f;

static inline float dynamicErrGain(BEAINF* obj)
{
    return (pMax - (pMax - pMin) * powf((float) obj->sbea[obj->selectedIndex].beaArea / 600.0f, 0.3f));
}
static inline float errGenerate(BEAINF* obj)
{
    if (obj->beaCount) return fRang(((float) obj->sbea[obj->selectedIndex].beaX - visionCenter) * dynamicErrGain(obj) * visionErrRange * 0.0106383f, visionErrRange);
    else return (obj->lastFrameSelectedBeaXY[0] > 80) ? (visionPolarity ? visionErrRange : -visionErrRange) : (visionPolarity ? -visionErrRange : visionErrRange);
    return 0.0f;
}

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

        twoPassEightConnectedAreaProcess(&beaInf);  // 连通区域处理

        areaSelected = (float) beaInf.sbea[beaInf.selectedIndex].beaArea;  // 计算选中区域的面积
        BlobCount = (float) beaInf.beaCount;

        if (beaInf.beaCount) schmittProcess(&vCenter, (float) beaInf.sbea[beaInf.selectedIndex].beaX);
        visionCenter = vCenter.outputState ? visionCenterRight : visionCenterLeft;

        visionErr = errGenerate(&beaInf);

        schmittProcess(&sYawErr, fabs(visionErr));

        if (!startState) yawOmeTar = visionState ? (sYawErr.outputState ? visionErr : 0) : 0;
        else
        {
            yawOmeTar = 0;
            if (beaInf.beaCount) startState = 0;
        }
        pitVelTar = visionState ? (visionPitErr) : 0;

        mt9v03x_finish_flag = 0;
    }
}
