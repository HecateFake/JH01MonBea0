/*
 * camera.h
 *
 *  Created on: 2025年4月30日
 *      Author: Hecate
 */

#ifndef CODE_CAMERA_H_
#define CODE_CAMERA_H_

#include <stdint.h>

#include "zf_device_mt9v03x.h"
#include "openMorph.h"
#include "schmitt.h"

extern float visionErr;  // 视觉误差

extern float thresholdLow;   // 二值化低阈值
extern float thresholdHigh;  // 二值化高阈值

extern float visionCenter;
extern float visionPolarity;

extern float visionErrRange;
extern float visionPitErr;

extern float pMin;
extern float pMax;

extern float areaSelected;

extern uint8_t binRel[MT9V03X_W * MT9V03X_H];
extern uint8_t binBuf[MT9V03X_W * MT9V03X_H];
extern uint8_t binSho[MT9V03X_W * MT9V03X_H];

extern CroppedImage8 binaryReal;
extern CroppedImage8 binaryBuffer;
extern CroppedImage8 binaryShow;

extern BEAINF beaInf;

#define fRang(x, y) fabs(x) < (y) ? (x) : ((x) < 0 ? -fabs(y) : fabs(y))

void visionProcessMT9V034(void);

#endif /* CODE_CAMERA_H_ */
