/*
 * mahony.h
 *
 *  Created on: 2025年1月16日
 *      Author: ROG
 */

#ifndef CODE_MAHONY_H_
#define CODE_MAHONY_H_

void MahonyAHRSupdate(float q[4], float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
void MahonyAHRSupdateIMU(float q[4], float gx, float gy, float gz, float ax, float ay, float az);

#endif /* CODE_MAHONY_H_ */
