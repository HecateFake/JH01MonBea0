/*
 * menu.c
 *
 *  Created on: 2024年3月15日
 *      Author: Hecate
 */

#ifndef CODE_MENU_H_
#define CODE_MENU_H_

#include <stdint.h>

typedef struct
{
    char *typeName;

    uint8_t typeOptionCount;

    char *typeOptionName[14];
    float *typeOption[14];
    float typeOptionStepS[14];
    float typeOptionStepL[14];
} MENUTYPE;

typedef enum
{
    visionShow,
    thresholdEditor,
    select0,
    select1,
    edit,
    menuStateCount,
} MENUSTATEENUM;

typedef struct
{
    MENUSTATEENUM menuState;
    uint8_t level0;
    uint8_t level1;
} MENUSTATE;

void menuInit(void);
void menuScanner(void);

#endif /* CODE_MENU_H_ */
