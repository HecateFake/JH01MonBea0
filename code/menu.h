// /*
//  * menu.c
//  *
//  *  Created on: 2024年3月15日
//  *      Author: Hecate
//  */

// #ifndef CODE_MENU_H_
// #define CODE_MENU_H_

// #include <stdint.h>

// typedef struct
// {
//     char *typeName;

//     char *typeOptionName0;
//     float *typeOption0;
//     float typeOptionStep0;
//     float typeOptionStep00;

//     char *typeOptionName1;
//     float *typeOption1;
//     float typeOptionStep1;
//     float typeOptionStep11;

//     char *typeOptionName2;
//     float *typeOption2;
//     float typeOptionStep2;
//     float typeOptionStep22;

//     char *typeOptionName3;
//     float *typeOption3;
//     float typeOptionStep3;
//     float typeOptionStep33;

//     char *typeOptionName4;
//     float *typeOption4;
//     float typeOptionStep4;
//     float typeOptionStep44;

//     char *typeOptionName5;
//     float *typeOption5;
//     float typeOptionStep5;
//     float typeOptionStep55;

//     char *typeOptionName6;
//     float *typeOption6;
//     float typeOptionStep6;
//     float typeOptionStep66;

//     char *typeOptionName7;
//     float *typeOption7;
//     float typeOptionStep7;
//     float typeOptionStep77;
// } MENUTYPE;

// typedef enum
// {
//     select0,
//     select1,
//     edit,
//     camera,
// } MENUSTATEENUM;

// typedef struct
// {
//     MENUSTATEENUM menuState;
//     uint8_t level0;
//     uint8_t level1;
// } MENUSTATE;

// void menuInit(void);
// void menuScanner(void);

// #endif /* CODE_MENU_H_ */
