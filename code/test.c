/*
 * test.c
 *
 *  Created on: 2025年9月1日
 *      Author: Hecate
 */
#include "test.h"

#include "zf_device_uart_receiver.h"
#include "camera.h"
#include "loop.h"

uint8_t neoTestState = 0;
static uint32_t testTimer = 0;
static uint8_t remoteFirstFlag = 1;

void testInit(void)
{
    neoTestState = 0;
    testTimer = 0;
    remoteFirstFlag = 1;
}

static inline void testTimerUpdate(void)
{
    if (neoTestState)
    {
        testTimer++;
        if (testTimer >= 12000) testTimer = 0;
    }
    else testTimer = 0;
}
static void AUTOtestHandler(void)
{
    // Handle AUTO test mode
    testTimerUpdate();
    if (testTimer <= 3000)
    {
        pitVelTar = visionPitErr;
        yawOmeTar = 0.0f;
    }
    else if (testTimer <= 6000)
    {
        pitVelTar = visionPitErr;
        yawOmeTar = visionErrRange;
    }
    else if (testTimer <= 9000)
    {
        pitVelTar = visionPitErr;
        yawOmeTar = 0.0f;
    }
    else
    {
        pitVelTar = visionPitErr;
        yawOmeTar = -visionErrRange;
    }
}
#define telMax 800
#define telMid 992
static uint16_t channel5Last = 0;
static void REMOTEtestHandler(void)
{
    // Handle REMOTE test mode
    if (uart_receiver.state)
    {
        if (uart_receiver.finsh_flag)
        {
            pitVelTar = -(((float) uart_receiver.channel[1] - telMid) / telMax) * visionPitErr;
            yawOmeTar = (((float) uart_receiver.channel[0] - telMid) / telMax) * visionErrRange;

            if (remoteFirstFlag)
            {
                remoteFirstFlag = 0;
                channel5Last = uart_receiver.channel[5];
                controllerInit();
            }
            if (uart_receiver.channel[5] != channel5Last) controllerInit();
            channel5Last = uart_receiver.channel[5];
            uart_receiver.finsh_flag = 0;
        }
    }
    else controllerInit();
}
static const void (*testHandlers[TEST_MODE_COUNT])(void) = {
    AUTOtestHandler,
    REMOTEtestHandler,
};

void testProcess(void)
{
    if (neoTestState) testHandlers[TEST_MODE]();
    else return;
}
