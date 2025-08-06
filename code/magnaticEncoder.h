/*
 * magnaticEncoder.h
 *
 *  Created on: 2025年7月7日
 *      Author: Hecate
 */

#ifndef CODE_MAGNATICENCODER_H_
#define CODE_MAGNATICENCODER_H_

#define magEnc0SpiEnum  (SPI_4)
#define magEnc0SpiSpeed (20*1000*1000)
#define magEnc0SpiClk   (SPI4_SCLK_P22_3)
#define magEnc0SpiMosi  (SPI4_MOSI_P22_0)
#define magEnc0SpiMiso  (SPI4_MISO_P22_1)
#define magEnc0SpiCs    (P22_2)

#define magEnc1SpiEnum  (SPI_4)
#define magEnc1SpiSpeed (20*1000*1000)
#define magEnc1SpiClk   (SPI4_SCLK_P22_3)
#define magEnc1SpiMosi  (SPI4_MOSI_P22_0)
#define magEnc1SpiMiso  (SPI4_MISO_P22_1)
#define magEnc1SpiCs    (P33_9)

void magnaticEncoderInit(void);

extern float estPos0;
extern float estVel0;
extern float estPos1;
extern float estVel1;

#define estMagnaticVel (estVel0-estVel1)

void magnaticEncoderPhaseLockedLoopInit(float w0, float j, float thr);
void magnaticEncoderGetData(void);

#endif /* CODE_MAGNATICENCODER_H_ */
