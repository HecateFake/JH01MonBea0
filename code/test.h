/*
 * test.h
 *
 *  Created on: 2025年9月1日
 *      Author: Hecate
 */
#ifndef CODE_TEST_H_
#define CODE_TEST_H_

#include <stdint.h>

extern uint8_t neoTestState;

typedef enum
{
    AUTO,
    REMOTE,
    TEST_MODE_COUNT,
} TEST_MODE;
#define TEST_MODE (AUTO)  // Define your test mode here(AUTO, REMOTE)

void testInit(void);
void testProcess(void);

#endif /* CODE_TEST_H_ */
