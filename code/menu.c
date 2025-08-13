/*
 * menu.c
 *
 *  Created on: 2024年3月15日
 *      Author: Hecate
 */
#include "menu.h"

#include "zf_device_key.h"
#include "zf_device_ips200.h"
#include "zf_driver_delay.h"
#include "zf_driver_flash.h"
#include "zf_common_font.h"
#include "zf_device_mt9v03x.h"
#include "zf_driver_gpio.h"
#include "zf_driver_pwm.h"

#include "loop.h"
#include "camera.h"
#include "imu.h"
#include "motor.h"
#include "small_driver_uart_control.h"

static MENUTYPE menuType[7];

static inline void saveSettingsToFlash(void)
{
    flash_buffer_clear();

    flash_union_buffer[0].float_type = visionCenterLeft;        // menuType[2][0]
    flash_union_buffer[1].float_type = visionCenterRight;       // menuType[2][1]
    flash_union_buffer[2].float_type = originVisionState;       // menuType[2][2]
    flash_union_buffer[3].float_type = visionPolarity;          // menuType[2][3]
    flash_union_buffer[4].float_type = thresholdLow;            // menuType[1][0]
    flash_union_buffer[5].float_type = thresholdHigh;           // menuType[1][1]
    flash_union_buffer[6].float_type = upDeadZone;              // menuType[2][4]
    flash_union_buffer[7].float_type = downDeadZone;            // menuType[2][5]
    flash_union_buffer[8].float_type = leftDeadZone;            // menuType[2][6]
    flash_union_buffer[9].float_type = rightDeadZone;           // menuType[2][7]
    flash_union_buffer[10].float_type = imageAreaMinLow;        // menuType[2][8]
    flash_union_buffer[11].float_type = imageAreaMaxLow;        // menuType[2][9]
    flash_union_buffer[12].float_type = imageAreaMinHigh;       // menuType[2][10]
    flash_union_buffer[13].float_type = imageAreaMaxHigh;       // menuType[2][11]
    flash_union_buffer[14].float_type = visionErrRange;         // menuType[3][0]
    flash_union_buffer[15].float_type = visionPitErr;           // menuType[3][1]
    flash_union_buffer[16].float_type = pMin;                   // menuType[3][2]
    flash_union_buffer[17].float_type = pMax;                   // menuType[3][3]
    flash_union_buffer[18].float_type = pitMid;                 // menuType[3][4]
    flash_union_buffer[19].float_type = rolMid;                 // menuType[3][5]
    flash_union_buffer[20].float_type = dynamicXeroPointGain1;  // menuType[4][0]
    flash_union_buffer[21].float_type = dynamicXeroPointGain0;  // menuType[4][1]
    flash_union_buffer[22].float_type = lStAng;                 // menuType[4][2]
    flash_union_buffer[23].float_type = rStAng;                 // menuType[4][3]
    flash_union_buffer[24].float_type = lReAng;                 // menuType[4][4]
    flash_union_buffer[25].float_type = rReAng;                 // menuType[4][5]
    flash_union_buffer[26].float_type = lturnAng;               // menuType[4][6]
    flash_union_buffer[27].float_type = rturnAng;               // menuType[4][7]
    flash_union_buffer[28].float_type = lOutAng;                // menuType[4][8]
    flash_union_buffer[29].float_type = rOutAng;                // menuType[4][9]
    flash_union_buffer[30].float_type = pitOmeP;                // menuType[5][0]
    flash_union_buffer[31].float_type = pitOmeI;                // menuType[5][1]
    flash_union_buffer[32].float_type = pitAngP;                // menuType[5][2]
    flash_union_buffer[33].float_type = pitVelP;                // menuType[5][3]
    flash_union_buffer[34].float_type = pitVelI;                // menuType[5][4]
    flash_union_buffer[35].float_type = rolOmeP;                // menuType[5][5]
    flash_union_buffer[36].float_type = rolOmeI;                // menuType[5][6]
    flash_union_buffer[37].float_type = rolAngP;                // menuType[5][7]
    flash_union_buffer[38].float_type = rolVelP;                // menuType[5][8]
    flash_union_buffer[39].float_type = rolVelI;                // menuType[5][9]
    flash_union_buffer[40].float_type = yawPwmMax;              // menuType[5][10]
    flash_union_buffer[41].float_type = schRealRolHigh;         // menuType[6][0]
    flash_union_buffer[42].float_type = schRealRolLow;          // menuType[6][1]
    flash_union_buffer[43].float_type = schWantRolHigh;         // menuType[6][2]
    flash_union_buffer[44].float_type = schWantRolLow;          // menuType[6][3]
    flash_union_buffer[45].float_type = schYawErrHigh;          // menuType[6][4]
    flash_union_buffer[46].float_type = schYawErrLow;           // menuType[6][5]
    flash_union_buffer[47].float_type = schVidioCenterHigh;     // menuType[6][6]
    flash_union_buffer[48].float_type = schVidioCenterLow;      // menuType[6][7]

    if (flash_check(0, 0)) flash_erase_page(0, 0);
    flash_write_page_from_buffer(0, 0);
}

static inline void loadSettingsFromFlash(void)
{
    if (flash_check(0, 0))
    {
        flash_buffer_clear();
        flash_read_page_to_buffer(0, 0);

        visionCenterLeft = flash_union_buffer[0].float_type;
        visionCenterRight = flash_union_buffer[1].float_type;
        originVisionState = flash_union_buffer[2].float_type;
        visionPolarity = flash_union_buffer[3].float_type;
        thresholdLow = flash_union_buffer[4].float_type;
        thresholdHigh = flash_union_buffer[5].float_type;
        upDeadZone = flash_union_buffer[6].float_type;
        downDeadZone = flash_union_buffer[7].float_type;
        leftDeadZone = flash_union_buffer[8].float_type;
        rightDeadZone = flash_union_buffer[9].float_type;
        imageAreaMinLow = flash_union_buffer[10].float_type;        // menuType[2][8]
        imageAreaMaxLow = flash_union_buffer[11].float_type;        // menuType[2][9]
        imageAreaMinHigh = flash_union_buffer[12].float_type;       // menuType[2][10]
        imageAreaMaxHigh = flash_union_buffer[13].float_type;       // menuType[2][11]
        visionErrRange = flash_union_buffer[14].float_type;         // menuType[3][0]
        visionPitErr = flash_union_buffer[15].float_type;           // menuType[3][1]
        pMin = flash_union_buffer[16].float_type;                   // menuType[3][2]
        pMax = flash_union_buffer[17].float_type;                   // menuType[3][3]
        pitMid = flash_union_buffer[18].float_type;                 // menuType[3][4]
        rolMid = flash_union_buffer[19].float_type;                 // menuType[3][5]
        dynamicXeroPointGain1 = flash_union_buffer[20].float_type;  // menuType[4][0]
        dynamicXeroPointGain0 = flash_union_buffer[21].float_type;  // menuType[4][1]
        lStAng = flash_union_buffer[22].float_type;                 // menuType[4][2]
        rStAng = flash_union_buffer[23].float_type;                 // menuType[4][3]
        lReAng = flash_union_buffer[24].float_type;                 // menuType[4][4]
        rReAng = flash_union_buffer[25].float_type;                 // menuType[4][5]
        lturnAng = flash_union_buffer[26].float_type;               // menuType[4][6]
        rturnAng = flash_union_buffer[27].float_type;               // menuType[4][7]
        lOutAng = flash_union_buffer[28].float_type;                // menuType[4][8]
        rOutAng = flash_union_buffer[29].float_type;                // menuType[4][9]
        pitOmeP = flash_union_buffer[30].float_type;                // menuType[5][0]
        pitOmeI = flash_union_buffer[31].float_type;                // menuType[5][1]
        pitAngP = flash_union_buffer[32].float_type;                // menuType[5][2]
        pitVelP = flash_union_buffer[33].float_type;                // menuType[5][3]
        pitVelI = flash_union_buffer[34].float_type;                // menuType[5][4]
        rolOmeP = flash_union_buffer[35].float_type;                // menuType[5][5]
        rolOmeI = flash_union_buffer[36].float_type;                // menuType[5][6]
        rolAngP = flash_union_buffer[37].float_type;                // menuType[5][7]
        rolVelP = flash_union_buffer[38].float_type;                // menuType[5][8]
        rolVelI = flash_union_buffer[39].float_type;                // menuType[5][9]
        yawPwmMax = flash_union_buffer[40].float_type;              // menuType[5][10]
        schRealRolHigh = flash_union_buffer[41].float_type;         // menuType[6][0]
        schRealRolLow = flash_union_buffer[42].float_type;          // menuType[6][1]
        schWantRolHigh = flash_union_buffer[43].float_type;         // menuType[6][2]
        schWantRolLow = flash_union_buffer[44].float_type;          // menuType[6][3]
        schYawErrHigh = flash_union_buffer[45].float_type;          // menuType[6][4]
        schYawErrLow = flash_union_buffer[46].float_type;           // menuType[6][5]
        schVidioCenterHigh = flash_union_buffer[47].float_type;     // menuType[6][6]
        schVidioCenterLow = flash_union_buffer[48].float_type;      // menuType[6][7]
    }
}

static int8_t level0 = 2;
static int8_t level1 = 0;
static MENUSTATEENUM menuState = visionShow;

static int8_t showType = 0;
static uint8_t brushedMotorTest = 0;
static uint8_t brushlessMotorTest = 0;

void menuInit(void)
{
    menuState = visionShow;
    showType = 0;
    brushedMotorTest = 0;
    brushlessMotorTest = 0;

    menuType[0].typeName = "Vision Show";
    menuType[0].typeOptionCount = 7;

    menuType[0].typeOptionName[0] = "Vision Error";
    menuType[0].typeOption[0] = &visionErr;
    menuType[0].typeOptionStepS[0] = 0.0f;
    menuType[0].typeOptionStepL[0] = 0.0f;

    menuType[0].typeOptionName[1] = "Blob Count";
    menuType[0].typeOption[1] = &BlobCount;
    menuType[0].typeOptionStepS[1] = 0.0f;
    menuType[0].typeOptionStepL[1] = 0.0f;

    menuType[0].typeOptionName[2] = "RPM-Brushless";
    menuType[0].typeOption[2] = &rolRpm.filteredValue;
    menuType[0].typeOptionStepS[2] = 0.0f;
    menuType[0].typeOptionStepL[2] = 0.0f;

    menuType[0].typeOptionName[3] = "RPM-Brushed";
    menuType[0].typeOption[3] = &pitRpm.filteredValue;
    menuType[0].typeOptionStepS[3] = 0.0f;
    menuType[0].typeOptionStepL[3] = 0.0f;

    menuType[0].typeOptionName[4] = "Pitch Angle";
    menuType[0].typeOption[4] = &imuData.roll;
    menuType[0].typeOptionStepS[4] = 0.0f;
    menuType[0].typeOptionStepL[4] = 0.0f;

    menuType[0].typeOptionName[5] = "Roll Angle";
    menuType[0].typeOption[5] = &imuData.pitch;
    menuType[0].typeOptionStepS[5] = 0.0f;
    menuType[0].typeOptionStepL[5] = 0.0f;

    menuType[0].typeOptionName[6] = "Yaw Angle";
    menuType[0].typeOption[6] = &imuData.yaw;
    menuType[0].typeOptionStepS[6] = 0.0f;
    menuType[0].typeOptionStepL[6] = 0.0f;

    menuType[1].typeName = "Threshold Edit";
    menuType[1].typeOptionCount = 2;

    menuType[1].typeOptionName[0] = "Low Threshold";
    menuType[1].typeOption[0] = &thresholdLow;
    menuType[1].typeOptionStepS[0] = 1.0f;
    menuType[1].typeOptionStepL[0] = 10.0f;

    menuType[1].typeOptionName[1] = "High Threshold";
    menuType[1].typeOption[1] = &thresholdHigh;
    menuType[1].typeOptionStepS[1] = 1.0f;
    menuType[1].typeOptionStepL[1] = 10.0f;

    menuType[2].typeName = "Vision Settings";
    menuType[2].typeOptionCount = 12;

    menuType[2].typeOptionName[0] = "Left Image Center";
    menuType[2].typeOption[0] = &visionCenterLeft;
    menuType[2].typeOptionStepS[0] = 1.0f;
    menuType[2].typeOptionStepL[0] = 10.0f;

    menuType[2].typeOptionName[1] = "Right Image Center";
    menuType[2].typeOption[1] = &visionCenterRight;
    menuType[2].typeOptionStepS[1] = 1.0f;
    menuType[2].typeOptionStepL[1] = 10.0f;

    menuType[2].typeOptionName[2] = "Origin Image Center";
    menuType[2].typeOption[2] = &originVisionState;
    menuType[2].typeOptionStepS[2] = 1.0f;
    menuType[2].typeOptionStepL[2] = 1.0f;

    menuType[2].typeOptionName[3] = "Image Polarity";
    menuType[2].typeOption[3] = &visionPolarity;
    menuType[2].typeOptionStepS[3] = 1.0f;
    menuType[2].typeOptionStepL[3] = 1.0f;

    menuType[2].typeOptionName[4] = "Up Dead Zone";
    menuType[2].typeOption[4] = &upDeadZone;
    menuType[2].typeOptionStepS[4] = 0.01f;
    menuType[2].typeOptionStepL[4] = 0.1f;

    menuType[2].typeOptionName[5] = "Down Dead Zone";
    menuType[2].typeOption[5] = &downDeadZone;
    menuType[2].typeOptionStepS[5] = 0.01f;
    menuType[2].typeOptionStepL[5] = 0.1f;

    menuType[2].typeOptionName[6] = "Left Dead Zone";
    menuType[2].typeOption[6] = &leftDeadZone;
    menuType[2].typeOptionStepS[6] = 0.01f;
    menuType[2].typeOptionStepL[6] = 0.1f;

    menuType[2].typeOptionName[7] = "Right Dead Zone";
    menuType[2].typeOption[7] = &rightDeadZone;
    menuType[2].typeOptionStepS[7] = 0.01f;
    menuType[2].typeOptionStepL[7] = 0.1f;

    menuType[2].typeOptionName[8] = "Area Min Low";
    menuType[2].typeOption[8] = &imageAreaMinLow;
    menuType[2].typeOptionStepS[8] = 1.0f;
    menuType[2].typeOptionStepL[8] = 10.0f;

    menuType[2].typeOptionName[9] = "Area Max Low";
    menuType[2].typeOption[9] = &imageAreaMaxLow;
    menuType[2].typeOptionStepS[9] = 10.0f;
    menuType[2].typeOptionStepL[9] = 100.0f;

    menuType[2].typeOptionName[10] = "Area Min High";
    menuType[2].typeOption[10] = &imageAreaMinHigh;
    menuType[2].typeOptionStepS[10] = 1.0f;
    menuType[2].typeOptionStepL[10] = 10.0f;

    menuType[2].typeOptionName[11] = "Area Max High";
    menuType[2].typeOption[11] = &imageAreaMaxHigh;
    menuType[2].typeOptionStepS[11] = 1.0f;
    menuType[2].typeOptionStepL[11] = 10.0f;

    menuType[3].typeName = "Action Settings";
    menuType[3].typeOptionCount = 6;

    menuType[3].typeOptionName[0] = "Error Max";
    menuType[3].typeOption[0] = &visionErrRange;
    menuType[3].typeOptionStepS[0] = 0.01f;
    menuType[3].typeOptionStepL[0] = 0.1f;

    menuType[3].typeOptionName[1] = "March Velocity";
    menuType[3].typeOption[1] = &visionPitErr;
    menuType[3].typeOptionStepS[1] = 1.0f;
    menuType[3].typeOptionStepL[1] = 10.0f;

    menuType[3].typeOptionName[2] = "Error Gain Min";
    menuType[3].typeOption[2] = &pMin;
    menuType[3].typeOptionStepS[2] = 0.1f;
    menuType[3].typeOptionStepL[2] = 1.0f;

    menuType[3].typeOptionName[3] = "Error Gain Max";
    menuType[3].typeOption[3] = &pMax;
    menuType[3].typeOptionStepS[3] = 0.1f;
    menuType[3].typeOptionStepL[3] = 1.0f;

    menuType[3].typeOptionName[4] = "Pitch Mid";
    menuType[3].typeOption[4] = &pitMid;
    menuType[3].typeOptionStepS[4] = 0.001f;
    menuType[3].typeOptionStepL[4] = 0.01f;

    menuType[3].typeOptionName[5] = "Roll Mid";
    menuType[3].typeOption[5] = &rolMid;
    menuType[3].typeOptionStepS[5] = 0.001f;
    menuType[3].typeOptionStepL[5] = 0.01f;

    menuType[4].typeName = "Dynamic Settings";
    menuType[4].typeOptionCount = 10;

    menuType[4].typeOptionName[0] = "Left Turn Gain";
    menuType[4].typeOption[0] = &dynamicXeroPointGain1;
    menuType[4].typeOptionStepS[0] = 0.0001f;
    menuType[4].typeOptionStepL[0] = 0.001f;

    menuType[4].typeOptionName[1] = "Right Turn Gain";
    menuType[4].typeOption[1] = &dynamicXeroPointGain0;
    menuType[4].typeOptionStepS[1] = 0.0001f;
    menuType[4].typeOptionStepL[1] = 0.001f;

    menuType[4].typeOptionName[2] = "Left Straight Angle";
    menuType[4].typeOption[2] = &lStAng;
    menuType[4].typeOptionStepS[2] = 0.5f;
    menuType[4].typeOptionStepL[2] = 1.0f;

    menuType[4].typeOptionName[3] = "Right Straight Angle";
    menuType[4].typeOption[3] = &rStAng;
    menuType[4].typeOptionStepS[3] = 0.5f;
    menuType[4].typeOptionStepL[3] = 1.0f;

    menuType[4].typeOptionName[4] = "Left Slow Angle";
    menuType[4].typeOption[4] = &lReAng;
    menuType[4].typeOptionStepS[4] = 0.5f;
    menuType[4].typeOptionStepL[4] = 1.0f;

    menuType[4].typeOptionName[5] = "Right Slow Angle";
    menuType[4].typeOption[5] = &rReAng;
    menuType[4].typeOptionStepS[5] = 0.5f;
    menuType[4].typeOptionStepL[5] = 1.0f;

    menuType[4].typeOptionName[6] = "Left Turn Angle";
    menuType[4].typeOption[6] = &lturnAng;
    menuType[4].typeOptionStepS[6] = 0.5f;
    menuType[4].typeOptionStepL[6] = 1.0f;

    menuType[4].typeOptionName[7] = "Right Turn Angle";
    menuType[4].typeOption[7] = &rturnAng;
    menuType[4].typeOptionStepS[7] = 0.5f;
    menuType[4].typeOptionStepL[7] = 1.0f;

    menuType[4].typeOptionName[8] = "Left Acceleration Angle";
    menuType[4].typeOption[8] = &lOutAng;
    menuType[4].typeOptionStepS[8] = 0.5f;
    menuType[4].typeOptionStepL[8] = 1.0f;

    menuType[4].typeOptionName[9] = "Right Acceleration Angle";
    menuType[4].typeOption[9] = &rOutAng;
    menuType[4].typeOptionStepS[9] = 0.5f;
    menuType[4].typeOptionStepL[9] = 1.0f;

    menuType[5].typeName = "PID Settings";
    menuType[5].typeOptionCount = 11;

    menuType[5].typeOptionName[0] = "Pitch Omega P";
    menuType[5].typeOption[0] = &pitOmeP;
    menuType[5].typeOptionStepS[0] = 10.0f;
    menuType[5].typeOptionStepL[0] = 100.0f;

    menuType[5].typeOptionName[1] = "Pitch Omega I";
    menuType[5].typeOption[1] = &pitOmeI;
    menuType[5].typeOptionStepS[1] = 1.0f;
    menuType[5].typeOptionStepL[1] = 10.0f;

    menuType[5].typeOptionName[2] = "Pitch Angle P";
    menuType[5].typeOption[2] = &pitAngP;
    menuType[5].typeOptionStepS[2] = 0.1f;
    menuType[5].typeOptionStepL[2] = 1.0f;

    menuType[5].typeOptionName[3] = "Pitch Velocity P";
    menuType[5].typeOption[3] = &pitVelP;
    menuType[5].typeOptionStepS[3] = 0.0001f;
    menuType[5].typeOptionStepL[3] = 0.001f;

    menuType[5].typeOptionName[4] = "Pitch Velocity I";
    menuType[5].typeOption[4] = &pitVelI;
    menuType[5].typeOptionStepS[4] = 0.00001f;
    menuType[5].typeOptionStepL[4] = 0.0001f;

    menuType[5].typeOptionName[5] = "Roll Omega P";
    menuType[5].typeOption[5] = &rolOmeP;
    menuType[5].typeOptionStepS[5] = 10.0f;
    menuType[5].typeOptionStepL[5] = 100.0f;

    menuType[5].typeOptionName[6] = "Roll Omega I";
    menuType[5].typeOption[6] = &rolOmeI;
    menuType[5].typeOptionStepS[6] = 1.0f;
    menuType[5].typeOptionStepL[6] = 10.0f;

    menuType[5].typeOptionName[7] = "Roll Angle P";
    menuType[5].typeOption[7] = &rolAngP;
    menuType[5].typeOptionStepS[7] = 0.1f;
    menuType[5].typeOptionStepL[7] = 1.0f;

    menuType[5].typeOptionName[8] = "Roll Velocity P";
    menuType[5].typeOption[8] = &rolVelP;
    menuType[5].typeOptionStepS[8] = 0.01f;
    menuType[5].typeOptionStepL[8] = 0.1f;

    menuType[5].typeOptionName[9] = "Roll Velocity I";
    menuType[5].typeOption[9] = &rolVelI;
    menuType[5].typeOptionStepS[9] = 0.00001f;
    menuType[5].typeOptionStepL[9] = 0.0001f;

    menuType[5].typeOptionName[10] = "Yaw Pwm Max";
    menuType[5].typeOption[10] = &yawPwmMax;
    menuType[5].typeOptionStepS[10] = 100.0f;
    menuType[5].typeOptionStepL[10] = 1000.0f;

    menuType[6].typeName = "Schmitt Settings";
    menuType[6].typeOptionCount = 8;

    menuType[6].typeOptionName[0] = "Real Roll High";
    menuType[6].typeOption[0] = &schRealRolHigh;
    menuType[6].typeOptionStepS[0] = 0.1f;
    menuType[6].typeOptionStepL[0] = 1.0f;

    menuType[6].typeOptionName[1] = "Real Roll Low";
    menuType[6].typeOption[1] = &schRealRolLow;
    menuType[6].typeOptionStepS[1] = 0.1f;
    menuType[6].typeOptionStepL[1] = 1.0f;

    menuType[6].typeOptionName[2] = "Wanted Roll High";
    menuType[6].typeOption[2] = &schWantRolHigh;
    menuType[6].typeOptionStepS[2] = 0.1f;
    menuType[6].typeOptionStepL[2] = 1.0f;

    menuType[6].typeOptionName[3] = "Wanted Roll Low";
    menuType[6].typeOption[3] = &schWantRolLow;
    menuType[6].typeOptionStepS[3] = 0.1f;
    menuType[6].typeOptionStepL[3] = 1.0f;

    menuType[6].typeOptionName[4] = "Vision Err High";
    menuType[6].typeOption[4] = &schYawErrHigh;
    menuType[6].typeOptionStepS[4] = 0.01f;
    menuType[6].typeOptionStepL[4] = 0.1f;

    menuType[6].typeOptionName[5] = "Vision Err Low";
    menuType[6].typeOption[5] = &schYawErrLow;
    menuType[6].typeOptionStepS[5] = 0.01f;
    menuType[6].typeOptionStepL[5] = 0.1f;

    menuType[6].typeOptionName[6] = "Vision Center High";
    menuType[6].typeOption[6] = &schVidioCenterHigh;
    menuType[6].typeOptionStepS[6] = 1.0f;
    menuType[6].typeOptionStepL[6] = 10.0f;

    menuType[6].typeOptionName[7] = "Vision Center Low";
    menuType[6].typeOption[7] = &schVidioCenterLow;
    menuType[6].typeOptionStepS[7] = 1.0f;
    menuType[6].typeOptionStepL[7] = 10.0f;

    // 加载flash中保存的设置
    loadSettingsFromFlash();
}

static inline void cameraShow(int8_t* showType)
{
    if (*showType > 1) *showType = 0;
    else if (*showType < 0) *showType = 1;
    ips200_show_gray_image(1, 1, binSho, 188, 120, 188, 118, 0);
    ips200_show_uint((uint16_t) beaInf.sbea[beaInf.selectedIndex].beaX + 1, (uint16_t) beaInf.sbea[beaInf.selectedIndex].beaY + 1, (uint16_t) (beaInf.areaTopMax + beaInf.kMax * (float) (beaInf.sbea[beaInf.selectedIndex].beaY - beaInf.yRange[0])), 3);
    ips200_show_uint((uint16_t) beaInf.sbea[beaInf.selectedIndex].beaX + 1, (uint16_t) beaInf.sbea[beaInf.selectedIndex].beaY + 17, beaInf.sbea[beaInf.selectedIndex].beaArea, 3);
    ips200_show_uint((uint16_t) beaInf.sbea[beaInf.selectedIndex].beaX + 1, (uint16_t) beaInf.sbea[beaInf.selectedIndex].beaY + 33, (uint16_t) (beaInf.areaTopMin + beaInf.kMin * (float) (beaInf.sbea[beaInf.selectedIndex].beaY - beaInf.yRange[0])), 3);
    ips200_show_gray_image(1, 120, *showType ? binRel : binBuf, 188, 120, 188, 119, 0);
    ips200_draw_line(0, 0, 0, 239, RGB565_GREEN);
    ips200_draw_line(0, 0, 319, 0, RGB565_GREEN);
    ips200_draw_line(319, 0, 319, 239, RGB565_GREEN);
    ips200_draw_line(0, 239, 319, 239, RGB565_GREEN);
    ips200_draw_line(0, 119, 189, 119, RGB565_GREEN);
    ips200_draw_line(189, 0, 189, 239, RGB565_GREEN);
    ips200_draw_line(visionCenter, beaInf.yRange[0] + 1, visionCenter, beaInf.yRange[1] - 1, visionPolarity ? RGB565_PINK : RGB565_BLUE);
    ips200_draw_line(beaInf.xRange[0] + 1, beaInf.yRange[0], beaInf.xRange[0] + 1, beaInf.yRange[1], RGB565_CYAN);
    ips200_draw_line(beaInf.xRange[1], beaInf.yRange[0], beaInf.xRange[1], beaInf.yRange[1], RGB565_CYAN);
    ips200_draw_line(beaInf.xRange[0], beaInf.yRange[0], beaInf.xRange[1], beaInf.yRange[0], RGB565_CYAN);
    ips200_draw_line(beaInf.xRange[0], beaInf.yRange[1], beaInf.xRange[1], beaInf.yRange[1], RGB565_CYAN);
    for (uint8_t i = 0; i < menuType[0].typeOptionCount; i++)
    {
        ips200_show_string(194, 34 * i + 1, menuType[0].typeOptionName[i]);
        if ((i == 4 || i == 5 || i == 6) && !testState) ips200_show_float(194, 17 * (2 * i + 1) + 1, tDeg(*menuType[0].typeOption[i]), 4, 5);
        else ips200_show_float(194, 17 * (2 * i + 1) + 1, *menuType[0].typeOption[i], 4, 5);
        ips200_draw_line(190, 17 * (2 * i + 1) + 1 - 1, 318, 17 * (2 * i + 1) + 1 - 1, RGB565_BLUE);
        if (i < menuType[0].typeOptionCount - 1) ips200_draw_line(190, 17 * (2 * i + 2) + 1 - 1, 318, 17 * (2 * i + 2) + 1 - 1, RGB565_GREEN);
    }
}

static inline void thresholdEdit(int8_t* showType, int8_t* editEnum)
{
    if (*showType > 1) *showType = 0;
    else if (*showType < 0) *showType = 1;
    if (*editEnum >= menuType[1].typeOptionCount) *editEnum = 0;
    else if (*editEnum < 0) *editEnum = menuType[1].typeOptionCount - 1;
    ips200_show_gray_image(1, 1, binSho, 188, 120, 188, 118, 0);
    ips200_show_uint((uint16_t) beaInf.sbea[beaInf.selectedIndex].beaX + 1, (uint16_t) beaInf.sbea[beaInf.selectedIndex].beaY + 1, (uint32_t) beaInf.beaCount, 2);
    ips200_show_gray_image(1, 120, *showType ? binRel : binBuf, 188, 120, 188, 119, 0);
    ips200_show_string(194, 1, menuType[1].typeName);
    ips200_show_string(194, 54, menuType[1].typeOptionName[0]);
    ips200_show_float(194, 91, *menuType[1].typeOption[0], 4, 5);
    ips200_show_string(194, 165, menuType[1].typeOptionName[1]);
    ips200_show_float(194, 202, *menuType[1].typeOption[1], 4, 5);
    ips200_show_string(190, 18, !*editEnum ? "##" : "  ");
    ips200_show_string(190, 129, *editEnum ? "##" : "  ");
    ips200_draw_line(0, 0, 0, 239, RGB565_GREEN);
    ips200_draw_line(0, 0, 319, 0, RGB565_GREEN);
    ips200_draw_line(319, 0, 319, 239, RGB565_GREEN);
    ips200_draw_line(0, 239, 319, 239, RGB565_GREEN);
    ips200_draw_line(0, 119, 189, 119, RGB565_GREEN);
    ips200_draw_line(189, 0, 189, 239, RGB565_GREEN);
    ips200_draw_line(190, 17, 318, 17, RGB565_GREEN);
    ips200_draw_line(190, 128, 318, 128, RGB565_GREEN);
    ips200_draw_line(190, 34, 206, 34, RGB565_GREEN);
    ips200_draw_line(206, 18, 206, 34, RGB565_GREEN);
    ips200_draw_line(190, 145, 206, 145, RGB565_GREEN);
    ips200_draw_line(206, 129, 206, 145, RGB565_GREEN);
}

static inline void menuShow0(int8_t* selectEnum)
{
    if (*selectEnum > 6) *selectEnum = 2;
    else if (*selectEnum < 2) *selectEnum = 6;
    ips200_draw_line(0, 85 - 1, 319, 85 - 1, RGB565_GREEN);
    ips200_draw_line(0, 85, 0, 17 * 5 + 85 - 2, RGB565_GREEN);
    ips200_draw_line(17, 85, 17, 17 * 5 + 85 - 2, RGB565_GREEN);
    ips200_draw_line(319, 85, 319, 17 * 5 + 85 - 2, RGB565_GREEN);
    for (uint8_t i = 0; i < 5; i++)
    {
        ips200_show_string(1, 17 * i + 85, (i + 2 == *selectEnum) ? ">>" : "  ");
        ips200_show_string(22, 17 * i + 85, menuType[i + 2].typeName);
        ips200_draw_line(0, 17 * (i + 1) + 85 - 1, 319, 17 * (i + 1) + 85 - 1, RGB565_GREEN);
    }
}

static inline void menuShow(int8_t* page, int8_t* selectEnum)
{
    if (*page > 6) *page = 2;
    else if (*page < 2) *page = 6;
    if (*selectEnum >= menuType[*page].typeOptionCount) *selectEnum = 0;
    else if (*selectEnum < 0) *selectEnum = menuType[*page].typeOptionCount - 1;
    uint16_t emptyY = 240 - menuType[*page].typeOptionCount * 17 - 1;
    emptyY = emptyY >> 1;
    if (emptyY) ips200_draw_line(0, emptyY - 1, 319, emptyY - 1, RGB565_GREEN);
    ips200_draw_line(0, emptyY, 0, 17 * menuType[*page].typeOptionCount + emptyY - 2, RGB565_GREEN);
    ips200_draw_line(17, emptyY, 17, 17 * menuType[*page].typeOptionCount + emptyY - 2, RGB565_GREEN);
    ips200_draw_line(225, emptyY, 225, 17 * menuType[*page].typeOptionCount + emptyY - 2, RGB565_GREEN);
    ips200_draw_line(319, emptyY, 319, 17 * menuType[*page].typeOptionCount + emptyY - 2, RGB565_GREEN);
    for (uint8_t i = 0; i < menuType[*page].typeOptionCount; i++)
    {
        ips200_show_string(1, 17 * i + emptyY, (i == *selectEnum) ? ">>" : "  ");
        ips200_show_string(22, 17 * i + emptyY, menuType[*page].typeOptionName[i]);
        ips200_show_float(230, 17 * i + emptyY, *menuType[*page].typeOption[i], 4, 5);
        ips200_draw_line(0, 17 * (i + 1) + emptyY - 1, 319, 17 * (i + 1) + emptyY - 1, RGB565_GREEN);
    }
}

static inline void menuEdit(int8_t* page, int8_t* selectEnum)
{
    if (*page > 6) *page = 2;
    else if (*page < 2) *page = 6;
    if (*selectEnum >= menuType[*page].typeOptionCount) *selectEnum = 0;
    else if (*selectEnum < 0) *selectEnum = menuType[*page].typeOptionCount - 1;
    uint16_t emptyY = 240 - menuType[*page].typeOptionCount * 17 - 1;
    emptyY = emptyY >> 1;
    if (emptyY) ips200_draw_line(0, emptyY - 1, 319, emptyY - 1, RGB565_GREEN);
    ips200_draw_line(0, emptyY, 0, 17 * menuType[*page].typeOptionCount + emptyY - 2, RGB565_GREEN);
    ips200_draw_line(17, emptyY, 17, 17 * menuType[*page].typeOptionCount + emptyY - 2, RGB565_GREEN);
    ips200_draw_line(225, emptyY, 225, 17 * menuType[*page].typeOptionCount + emptyY - 2, RGB565_GREEN);
    ips200_draw_line(319, emptyY, 319, 17 * menuType[*page].typeOptionCount + emptyY - 2, RGB565_GREEN);
    for (uint8_t i = 0; i < menuType[*page].typeOptionCount; i++)
    {
        ips200_show_string(1, 17 * i + emptyY, (i == *selectEnum) ? "=>" : "  ");
        ips200_show_string(22, 17 * i + emptyY, menuType[*page].typeOptionName[i]);
        ips200_show_float(230, 17 * i + emptyY, *menuType[*page].typeOption[i], 4, 5);
        ips200_draw_line(0, 17 * (i + 1) + emptyY - 1, 319, 17 * (i + 1) + emptyY - 1, RGB565_GREEN);
    }
}

void menuScanner(void)
{
    switch (menuState)
    {
        case visionShow:
            cameraShow(&showType);
            if (key_get_state(KEY_1) == KEY_SHORT_PRESS)
            {
                menuState = select0;
                ips200_clear();
            }
            else if (key_get_state(KEY_1) == KEY_LONG_PRESS)
            {
                switch (brushedMotorTest)
                {
                    case 0:
                        sPwm = 3000;
                        brushedMotorTest++;
                        break;
                    case 1:
                        sPwm = -3000;
                        brushedMotorTest++;
                        break;
                    case 2:
                        sPwm = 0;
                        brushedMotorTest = 0;
                        break;
                }
                ips200_clear();
                while (key_get_state(KEY_1) == KEY_LONG_PRESS);
            }
            else if (key_get_state(KEY_2) == KEY_SHORT_PRESS)
            {
                menuState = thresholdEditor;
                ips200_clear();
            }
            else if (key_get_state(KEY_2) == KEY_LONG_PRESS)
            {
                switch (brushlessMotorTest)
                {
                    case 0:
                        lRolPwm = 2000;
                        rRolPwm = 2000;
                        brushlessMotorTest++;
                        break;
                    case 1:
                        lRolPwm = -2000;
                        rRolPwm = -2000;
                        brushlessMotorTest++;
                        break;
                    case 2:
                        lRolPwm = 0;
                        rRolPwm = 0;
                        brushlessMotorTest = 0;
                        break;
                }
                ips200_clear();
                while (key_get_state(KEY_2) == KEY_LONG_PRESS);
            }
            else if (key_get_state(KEY_3) == KEY_SHORT_PRESS) showType++;
            else if (key_get_state(KEY_3) == KEY_LONG_PRESS)
            {
                if (testState) testState = 0;
                else testState = 1;
                ips200_clear();
                while (key_get_state(KEY_3) == KEY_LONG_PRESS);
            }
            else if (key_get_state(KEY_4) == KEY_SHORT_PRESS)
            {
                saveSettingsToFlash();
                menuInit();
                controllerInit();
                ips200_full(RGB565_GREEN);
                ips200_clear();
            }
            else if (key_get_state(KEY_4) == KEY_LONG_PRESS)
            {
                flash_erase_page(0, 0);
                flash_buffer_clear();
                menuInit();
                controllerInit();
                ips200_full(RGB565_RED);
                while (key_get_state(KEY_4) == KEY_LONG_PRESS);
                ips200_clear();
            }
            if (key_get_state(KEY_5) == KEY_SHORT_PRESS)
            {
                if (testState)
                {
                    state = 1;
                    visionState = 0;
                }
                else
                {
                    state = 1;
                    visionState = 1;
                }
            }
            break;
        case thresholdEditor:
            thresholdEdit(&showType, &level1);
            if (key_get_state(KEY_1) == KEY_SHORT_PRESS)
            {
                *menuType[1].typeOption[level1] -= menuType[1].typeOptionStepL[level1];
                saveSettingsToFlash();
            }
            else if (key_get_state(KEY_2) == KEY_SHORT_PRESS)
            {
                *menuType[1].typeOption[level1] -= menuType[1].typeOptionStepS[level1];
                saveSettingsToFlash();
            }
            else if (key_get_state(KEY_3) == KEY_SHORT_PRESS)
            {
                *menuType[1].typeOption[level1] += menuType[1].typeOptionStepS[level1];
                saveSettingsToFlash();
            }
            else if (key_get_state(KEY_4) == KEY_SHORT_PRESS)
            {
                *menuType[1].typeOption[level1] += menuType[1].typeOptionStepL[level1];
                saveSettingsToFlash();
            }
            else if (key_get_state(KEY_5) == KEY_SHORT_PRESS) level1++;
            else if (key_get_state(KEY_3) == KEY_LONG_PRESS)
            {
                showType++;
                ips200_clear();
                while (key_get_state(KEY_3) == KEY_LONG_PRESS);
            }
            else if (key_get_state(KEY_1) == KEY_LONG_PRESS)
            {
                menuState = select0;
                ips200_clear();
                while (key_get_state(KEY_1) == KEY_LONG_PRESS);
            }
            else if (key_get_state(KEY_5) == KEY_LONG_PRESS)
            {
                saveSettingsToFlash();
                menuInit();
                controllerInit();
                menuState = visionShow;
                ips200_clear();
                while (key_get_state(KEY_5) == KEY_LONG_PRESS);
            }
            break;
        case select0:
            menuShow0(&level0);
            if (key_get_state(KEY_1) == KEY_SHORT_PRESS)
            {
                saveSettingsToFlash();
                menuInit();
                controllerInit();
                menuState = visionShow;
                ips200_clear();
            }
            else if (key_get_state(KEY_2) == KEY_SHORT_PRESS) level0--;
            else if (key_get_state(KEY_3) == KEY_SHORT_PRESS) level0++;
            else if (key_get_state(KEY_4) == KEY_SHORT_PRESS)
            {
                menuState = select1;
                ips200_clear();
            }
            else if (key_get_state(KEY_5) == KEY_SHORT_PRESS)
            {
                saveSettingsToFlash();
                menuInit();
                controllerInit();
                menuState = visionShow;
                ips200_clear();
            }
            break;
        case select1:
            menuShow(&level0, &level1);
            if (key_get_state(KEY_1) == KEY_SHORT_PRESS)
            {
                menuState = select0;
                ips200_clear();
            }
            else if (key_get_state(KEY_2) == KEY_SHORT_PRESS) level1--;
            else if (key_get_state(KEY_3) == KEY_SHORT_PRESS) level1++;
            else if (key_get_state(KEY_4) == KEY_SHORT_PRESS)
            {
                menuState = edit;
                ips200_clear();
            }
            else if (key_get_state(KEY_5) == KEY_SHORT_PRESS)
            {
                saveSettingsToFlash();
                menuInit();
                controllerInit();
                menuState = visionShow;
                ips200_clear();
            }
            break;
        case edit:
            menuEdit(&level0, &level1);
            if (key_get_state(KEY_1) == KEY_SHORT_PRESS)
            {
                *menuType[level0].typeOption[level1] -= menuType[level0].typeOptionStepL[level1];
                saveSettingsToFlash();
            }
            else if (key_get_state(KEY_2) == KEY_SHORT_PRESS)
            {
                *menuType[level0].typeOption[level1] -= menuType[level0].typeOptionStepS[level1];
                saveSettingsToFlash();
            }
            else if (key_get_state(KEY_3) == KEY_SHORT_PRESS)
            {
                *menuType[level0].typeOption[level1] += menuType[level0].typeOptionStepS[level1];
                saveSettingsToFlash();
            }
            else if (key_get_state(KEY_4) == KEY_SHORT_PRESS)
            {
                *menuType[level0].typeOption[level1] += menuType[level0].typeOptionStepL[level1];
                saveSettingsToFlash();
            }
            else if (key_get_state(KEY_5) == KEY_SHORT_PRESS)
            {
                saveSettingsToFlash();
                menuInit();
                controllerInit();
                menuState = select1;
                ips200_clear();
            }
            break;
    }
}
