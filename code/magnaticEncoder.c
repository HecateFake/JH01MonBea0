/*
 * magnaticEncoder.c
 *
 *  Created on: 2025年7月7日
 *      Author: Hecate
 */

#include "magnaticEncoder.h"

#include <stdint.h>
#include <math.h>

#include "zf_driver_spi.h"
#include "zf_driver_gpio.h"

void magnaticEncoderInit(void)
{
    spi_init(magEnc0SpiEnum, SPI_MODE0, magEnc0SpiSpeed, magEnc0SpiClk, magEnc0SpiMosi, magEnc0SpiMiso, SPI_CS_NULL);
    spi_init(magEnc1SpiEnum, SPI_MODE0, magEnc1SpiSpeed, magEnc1SpiClk, magEnc1SpiMosi, magEnc1SpiMiso, SPI_CS_NULL);

    gpio_init(magEnc0SpiCs, GPO, GPIO_HIGH, GPO_PUSH_PULL);
    gpio_init(magEnc1SpiCs, GPO, GPIO_HIGH, GPO_PUSH_PULL);
}

#define magEnc0 0
#define magEnc1 1

static uint16_t magEncAng[2] = {0};
static uint16_t magEncAngLast[2] = {0};
static uint8_t magFirstFlag = 1;
#define magEnc0SpiSetCs(x) ((x) ? (gpio_high(magEnc0SpiCs)) : (gpio_low(magEnc0SpiCs)))
#define magEnc1SpiSetCs(x) ((x) ? (gpio_high(magEnc1SpiCs)) : (gpio_low(magEnc1SpiCs)))
static void magnaticEncoderGetAng(void)
{
    magEncAngLast[magEnc0] = magEncAng[magEnc0];
    magEncAngLast[magEnc1] = magEncAng[magEnc1];

    magEnc0SpiSetCs(0);
    magEncAng[magEnc0] = spi_read_16bit_register(magEnc0SpiEnum, 0x8021) - 16384;
    magEnc0SpiSetCs(1);

    magEnc1SpiSetCs(0);
    magEncAng[magEnc1] = spi_read_16bit_register(magEnc1SpiEnum, 0x8021) - 16384;
    magEnc1SpiSetCs(1);

    if (magFirstFlag)
    {
        magEncAngLast[magEnc0] = magEncAng[magEnc0];
        magEncAngLast[magEnc1] = magEncAng[magEnc1];

        magFirstFlag = 0;
    }
}

#define PI (3.1415926535898f)

#define speedDt 0.0002f
// static uint8_t magSpeFirstFlag = 1;
float estPos0 = 0;
float estVel0 = 0;
float estPos1 = 0;
float estVel1 = 0;
static float pllKp = 0;
static float pllKi = 0;
static float velThr = 0;
static int32_t loopCount0 = 0;
static int32_t loopCount1 = 0;
static int32_t loopCountLast0 = 0;
static int32_t loopCountLast1 = 0;

void magnaticEncoderPhaseLockedLoopInit(float w0, float j, float thr)
{
    pllKp = w0 * j;
    pllKi = w0 * w0;
    velThr = thr;
}

void magnaticEncoderGetData(void)
{
    magnaticEncoderGetAng();

    int16_t delta0 = (int16_t) magEncAng[magEnc0] - (int16_t) magEncAngLast[magEnc0];
    int16_t delta1 = (int16_t) magEncAng[magEnc1] - (int16_t) magEncAngLast[magEnc1];

    if (delta0 > 8192)
        loopCount0--;
    else if (delta0 < -8192)
        loopCount0++;
    if (delta1 > 8192)
        loopCount1--;
    else if (delta1 < -8192)
        loopCount1++;

    estVel0 = (loopCount0 - loopCountLast0) * (16384) + (float) delta0;
    estVel1 = (loopCount1 - loopCountLast1) * (16384) + (float) delta1;

    loopCountLast0 = loopCount0;
    loopCountLast1 = loopCount1;

    //    float ang0 = (float)loopCount0 * (2.0f * PI) + (float)magEncAng[magEnc0] * (2.0f * PI / 16384.0f);
    //    float ang1 = (float)loopCount1 * (2.0f * PI) + (float)magEncAng[magEnc1] * (2.0f * PI / 16384.0f);
    //
    //    if (magSpeFirstFlag)
    //    {
    //        estPos0 = ang0;
    //        estPos1 = ang1;
    //        magSpeFirstFlag = 0;
    //    }
    //
    //    estPos0 += estVel0 * speedDt;
    //    estPos1 += estVel1 * speedDt;
    //
    //    float err0 = ang0 - estPos0;
    //    float err1 = ang1 - estPos1;
    //
    //    err0 = fmodf(err0, 2.0f * PI);
    //    err1 = fmodf(err1, 2.0f * PI);
    //    if (err0 > PI)
    //        err0 -= 2.0f * PI;
    //    else if (err0 < -PI)
    //        err0 += 2.0f * PI;
    //    if (err1 > PI)
    //        err1 -= 2.0f * PI;
    //    else if (err1 < -PI)
    //        err1 += 2.0f * PI;
    //
    //    estPos0 += pllKp * err0 * speedDt;
    //    estVel0 += pllKi * err0 * speedDt;
    //    estPos1 += pllKp * err1 * speedDt;
    //    estVel1 += pllKi * err1 * speedDt;
    //
    //    if (fabs(estVel0) < velThr)
    //        estVel0 = 0.0f;
    //    if (fabs(estVel1) < velThr)
    //        estVel1 = 0.0f;
}
