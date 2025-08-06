// /*
//  * menu.c
//  *
//  *  Created on: 2024年3月15日
//  *      Author: Hecate
//  */
// #include "menu.h"

// #include "zf_device_key.h"
// #include "zf_device_tft180.h"
// #include "zf_driver_delay.h"
// #include "zf_driver_flash.h"
// #include "zf_common_font.h"
// #include "zf_device_mt9v03x.h"  // 添加摄像头头文件

// #include "loop.h"
// #include "camera.h"
// #include "camera.h"
// #include "imu.h"

// static MENUTYPE menuType[6];  // 修改为6个菜单页
// static MENUSTATE menuState;

// void menuInit(void)
// {
//     menuState.level0 = 0;
//     menuState.level1 = 0;
//     menuState.menuState = camera;

//     menuType[0].typeName = "general";

//     menuType[0].typeOptionName0 = "imgCenX";
//     menuType[0].typeOption0 = &image_center_x;
//     menuType[0].typeOptionStep0 = 5.0f;
//     menuType[0].typeOptionStep00 = 10.0f;

//     menuType[0].typeOptionName1 = "imgGauS";
//     menuType[0].typeOption1 = &imageGaussianSigma;
//     menuType[0].typeOptionStep1 = 0.1f;
//     menuType[0].typeOptionStep11 = 1.0f;

//     menuType[0].typeOptionName2 = "imgUp";
//     menuType[0].typeOption2 = &imageDeadZone;
//     menuType[0].typeOptionStep2 = 0.01f;
//     menuType[0].typeOptionStep22 = 0.1f;

//     menuType[0].typeOptionName3 = "imgDo";
//     menuType[0].typeOption3 = &imageDeadZoneDown;
//     menuType[0].typeOptionStep3 = 0.01f;
//     menuType[0].typeOptionStep33 = 0.1f;

//     menuType[0].typeOptionName4 = "TurErr";
//     menuType[0].typeOption4 = &staticTurnErr;
//     menuType[0].typeOptionStep4 = 0.01f;
//     menuType[0].typeOptionStep44 = 0.1f;

//     menuType[0].typeOptionName5 = "MarErr";
//     menuType[0].typeOption5 = &staticMarchErr;
//     menuType[0].typeOptionStep5 = 1.0f;
//     menuType[0].typeOptionStep55 = 10.0f;

//     menuType[0].typeOptionName6 = "pitMid";
//     menuType[0].typeOption6 = &pitMid;
//     menuType[0].typeOptionStep6 = 0.001f;
//     menuType[0].typeOptionStep66 = 0.01f;

//     menuType[0].typeOptionName7 = "rolMid";
//     menuType[0].typeOption7 = &rolMid;
//     menuType[0].typeOptionStep7 = 0.001f;
//     menuType[0].typeOptionStep77 = 0.01f;

//     menuType[1].typeName = "poetry";

//     menuType[1].typeOptionName0 = "l_march";
//     menuType[1].typeOption0 = &lStAng;
//     menuType[1].typeOptionStep0 = 0.5f;
//     menuType[1].typeOptionStep00 = 1.0f;

//     menuType[1].typeOptionName1 = "r_march";
//     menuType[1].typeOption1 = &rStAng;
//     menuType[1].typeOptionStep1 = 0.5f;
//     menuType[1].typeOptionStep11 = 1.0f;

//     menuType[1].typeOptionName2 = "l_decBe";
//     menuType[1].typeOption2 = &lReAng;
//     menuType[1].typeOptionStep2 = 0.5f;
//     menuType[1].typeOptionStep22 = 1.0f;

//     menuType[1].typeOptionName3 = "r_decBe";
//     menuType[1].typeOption3 = &rReAng;
//     menuType[1].typeOptionStep3 = 0.5f;
//     menuType[1].typeOptionStep33 = 1.0f;

//     menuType[1].typeOptionName4 = "l_decTu";
//     menuType[1].typeOption4 = &lturnAng;
//     menuType[1].typeOptionStep4 = 0.5f;
//     menuType[1].typeOptionStep44 = 1.0f;

//     menuType[1].typeOptionName5 = "r_decTu";
//     menuType[1].typeOption5 = &rturnAng;
//     menuType[1].typeOptionStep5 = 0.5f;
//     menuType[1].typeOptionStep55 = 1.0f;

//     menuType[1].typeOptionName6 = "l_acc";
//     menuType[1].typeOption6 = &lOutAng;
//     menuType[1].typeOptionStep6 = 0.5f;
//     menuType[1].typeOptionStep66 = 1.0f;

//     menuType[1].typeOptionName7 = "r_acc";
//     menuType[1].typeOption7 = &rOutAng;
//     menuType[1].typeOptionStep7 = 0.5f;
//     menuType[1].typeOptionStep77 = 1.0f;

//     menuType[2].typeName = "turn";

//     menuType[2].typeOptionName0 = "lGain";
//     menuType[2].typeOption0 = &dynamicXeroPointGain1;
//     menuType[2].typeOptionStep0 = 0.0001f;
//     menuType[2].typeOptionStep00 = 0.001f;

//     menuType[2].typeOptionName1 = "rGain";
//     menuType[2].typeOption1 = &dynamicXeroPointGain0;
//     menuType[2].typeOptionStep1 = 0.0001f;
//     menuType[2].typeOptionStep11 = 0.001f;

//     menuType[3].typeName = "pitPID";

//     menuType[3].typeOptionName0 = "pitOmeP";
//     menuType[3].typeOption0 = &pitOmeP;
//     menuType[3].typeOptionStep0 = 10.0f;
//     menuType[3].typeOptionStep00 = 100.0f;

//     menuType[3].typeOptionName1 = "pitOmeI";
//     menuType[3].typeOption1 = &pitOmeI;
//     menuType[3].typeOptionStep1 = 1.0f;
//     menuType[3].typeOptionStep11 = 10.0f;

//     menuType[3].typeOptionName2 = "pitAngP";
//     menuType[3].typeOption2 = &pitAngP;
//     menuType[3].typeOptionStep2 = 0.1f;
//     menuType[3].typeOptionStep22 = 1.0f;

//     menuType[3].typeOptionName3 = "pitVelP";
//     menuType[3].typeOption3 = &pitVelP;
//     menuType[3].typeOptionStep3 = 0.0001f;
//     menuType[3].typeOptionStep33 = 0.001f;

//     menuType[3].typeOptionName4 = "pitVelI";
//     menuType[3].typeOption4 = &pitVelI;
//     menuType[3].typeOptionStep4 = 0.00001f;
//     menuType[3].typeOptionStep44 = 0.0001f;

//     menuType[4].typeName = "rolPID";

//     menuType[4].typeOptionName0 = "rolOmeP";
//     menuType[4].typeOption0 = &rolOmeP;
//     menuType[4].typeOptionStep0 = 10.0f;
//     menuType[4].typeOptionStep00 = 100.0f;

//     menuType[4].typeOptionName1 = "rolOmeI";
//     menuType[4].typeOption1 = &rolOmeI;
//     menuType[4].typeOptionStep1 = 1.0f;
//     menuType[4].typeOptionStep11 = 10.0f;

//     menuType[4].typeOptionName2 = "rolAngP";
//     menuType[4].typeOption2 = &rolAngP;
//     menuType[4].typeOptionStep2 = 0.1f;
//     menuType[4].typeOptionStep22 = 1.0f;

//     menuType[4].typeOptionName3 = "rolVelP";
//     menuType[4].typeOption3 = &rolVelP;
//     menuType[4].typeOptionStep3 = 0.01f;
//     menuType[4].typeOptionStep33 = 0.1f;

//     menuType[4].typeOptionName4 = "rolVelI";
//     menuType[4].typeOption4 = &rolVelI;
//     menuType[4].typeOptionStep4 = 0.00001f;
//     menuType[4].typeOptionStep44 = 0.0001f;

//     menuType[5].typeName = "camera";

//     menuType[5].typeOptionName0 = "gain";
//     menuType[5].typeOption0 = &a;  // 增益
//     menuType[5].typeOptionStep0 = 1.0f;
//     menuType[5].typeOptionStep00 = 10.0f;

//     menuType[5].typeOptionName1 = "exp";
//     menuType[5].typeOption1 = &b;  // 曝光
//     menuType[5].typeOptionStep1 = 10.0f;
//     menuType[5].typeOptionStep11 = 100.0f;

//     menuType[5].typeOptionName2 = "aexp";
//     menuType[5].typeOption2 = &c;  // 自动曝光
//     menuType[5].typeOptionStep2 = 1.0f;
//     menuType[5].typeOptionStep22 = 10.0f;

//     menuType[5].typeOptionName3 = "otsu";
//     menuType[5].typeOption3 = &otsu;  // 大津法阈值
//     menuType[5].typeOptionStep3 = 1.0f;
//     menuType[5].typeOptionStep33 = 10.0f;
// }

// static inline void menuSelectLevel0(void)
// {
//     tft180_show_string(10, 15 * 0, menuType[0].typeName);
//     tft180_show_string(10, 15 * 1, menuType[1].typeName);
//     tft180_show_string(10, 15 * 2, menuType[2].typeName);
//     tft180_show_string(10, 15 * 3, menuType[3].typeName);
//     tft180_show_string(10, 15 * 4, menuType[4].typeName);
//     tft180_show_string(10, 15 * 5, menuType[5].typeName);  // 添加第6个菜单
//     for (int i = 0; i <= 5; i++)                           // 修改为5
//     {
//         tft180_show_string(0, 15 * i, (i == menuState.level0) ? ">" : " ");
//     }
// }

// static inline void menuSelectLevel1(void)
// {
//     switch (menuState.level0)
//     {
//         case 0:
//             tft180_show_string(10, 15 * 0, menuType[0].typeOptionName0);
//             tft180_show_float(80, 15 * 0, *menuType[0].typeOption0, 3, 1);  // imgCenX: 百位数，1位小数
//             tft180_show_string(10, 15 * 1, menuType[0].typeOptionName1);
//             tft180_show_float(80, 15 * 1, *menuType[0].typeOption1, 1, 1);  // imgGauS: 个位数，1位小数
//             tft180_show_string(10, 15 * 2, menuType[0].typeOptionName2);
//             tft180_show_float(80, 15 * 2, *menuType[0].typeOption2, 1, 2);  // imgUp: 0.xx格式
//             tft180_show_string(10, 15 * 3, menuType[0].typeOptionName3);
//             tft180_show_float(80, 15 * 3, *menuType[0].typeOption3, 1, 2);  // imgDo: 0.xx格式
//             tft180_show_string(10, 15 * 4, menuType[0].typeOptionName4);
//             tft180_show_float(80, 15 * 4, *menuType[0].typeOption4, 1, 2);  // TurErr: x.xx格式
//             tft180_show_string(10, 15 * 5, menuType[0].typeOptionName5);
//             tft180_show_float(80, 15 * 5, *menuType[0].typeOption5, 2, 1);  // MarErr: 十位数，1位小数
//             tft180_show_string(10, 15 * 6, menuType[0].typeOptionName6);
//             tft180_show_float(80, 15 * 6, *menuType[0].typeOption6, 1, 3);  // pitMid: 0.xxx格式
//             tft180_show_string(10, 15 * 7, menuType[0].typeOptionName7);
//             tft180_show_float(80, 15 * 7, *menuType[0].typeOption7, 2, 3);  // rolMid: -x.xxx格式
//             for (int i = 0; i <= 7; i++)
//             {
//                 tft180_show_string(0, 15 * i, (i == menuState.level1) ? ">" : " ");
//             }
//             break;
//         case 1:
//             tft180_show_string(10, 15 * 0, menuType[1].typeOptionName0);
//             tft180_show_float(80, 15 * 0, *menuType[1].typeOption0, 2, 1);  // 角度值：十位数，1位小数
//             tft180_show_string(10, 15 * 1, menuType[1].typeOptionName1);
//             tft180_show_float(80, 15 * 1, *menuType[1].typeOption1, 2, 1);
//             tft180_show_string(10, 15 * 2, menuType[1].typeOptionName2);
//             tft180_show_float(80, 15 * 2, *menuType[1].typeOption2, 2, 1);
//             tft180_show_string(10, 15 * 3, menuType[1].typeOptionName3);
//             tft180_show_float(80, 15 * 3, *menuType[1].typeOption3, 2, 1);
//             tft180_show_string(10, 15 * 4, menuType[1].typeOptionName4);
//             tft180_show_float(80, 15 * 4, *menuType[1].typeOption4, 2, 1);
//             tft180_show_string(10, 15 * 5, menuType[1].typeOptionName5);
//             tft180_show_float(80, 15 * 5, *menuType[1].typeOption5, 2, 1);
//             tft180_show_string(10, 15 * 6, menuType[1].typeOptionName6);
//             tft180_show_float(80, 15 * 6, *menuType[1].typeOption6, 2, 1);
//             tft180_show_string(10, 15 * 7, menuType[1].typeOptionName7);
//             tft180_show_float(80, 15 * 7, *menuType[1].typeOption7, 2, 1);
//             for (int i = 0; i <= 7; i++)
//             {
//                 tft180_show_string(0, 15 * i, (i == menuState.level1) ? ">" : " ");
//             }
//             break;
//         case 2:
//             if (menuState.level1 > 1)
//             {
//                 menuState.level1 = 0;
//             }
//             tft180_show_string(10, 15 * 0, menuType[2].typeOptionName0);
//             tft180_show_float(80, 15 * 0, *menuType[2].typeOption0, 1, 4);  // 增益值：0.xxxx格式
//             tft180_show_string(10, 15 * 1, menuType[2].typeOptionName1);
//             tft180_show_float(80, 15 * 1, *menuType[2].typeOption1, 1, 4);
//             for (int i = 0; i <= 1; i++)
//             {
//                 tft180_show_string(0, 15 * i, (i == menuState.level1) ? ">" : " ");
//             }
//             break;
//         case 3:
//             if (menuState.level1 > 4)
//             {
//                 menuState.level1 = 0;
//             }
//             tft180_show_string(10, 15 * 0, menuType[3].typeOptionName0);
//             tft180_show_float(80, 15 * 0, *menuType[3].typeOption0, 4, 1);  // pitOmeP: 千位数，1位小数
//             tft180_show_string(10, 15 * 1, menuType[3].typeOptionName1);
//             tft180_show_float(80, 15 * 1, *menuType[3].typeOption1, 3, 1);  // pitOmeI: 百位数，1位小数
//             tft180_show_string(10, 15 * 2, menuType[3].typeOptionName2);
//             tft180_show_float(80, 15 * 2, *menuType[3].typeOption2, 1, 1);  // pitAngP: x.x格式
//             tft180_show_string(10, 15 * 3, menuType[3].typeOptionName3);
//             tft180_show_float(80, 15 * 3, *menuType[3].typeOption3, 1, 4);  // pitVelP: 0.xxxx格式
//             tft180_show_string(10, 15 * 4, menuType[3].typeOptionName4);
//             tft180_show_float(80, 15 * 4, *menuType[3].typeOption4, 1, 5);  // pitVelI: 0.xxxxx格式
//             for (int i = 0; i <= 4; i++)
//             {
//                 tft180_show_string(0, 15 * i, (i == menuState.level1) ? ">" : " ");
//             }
//             break;
//         case 4:
//             if (menuState.level1 > 4)
//             {
//                 menuState.level1 = 0;
//             }
//             tft180_show_string(10, 15 * 0, menuType[4].typeOptionName0);
//             tft180_show_float(80, 15 * 0, *menuType[4].typeOption0, 4, 1);  // rolOmeP: 千位数，1位小数
//             tft180_show_string(10, 15 * 1, menuType[4].typeOptionName1);
//             tft180_show_float(80, 15 * 1, *menuType[4].typeOption1, 3, 1);  // rolOmeI: 百位数，1位小数
//             tft180_show_string(10, 15 * 2, menuType[4].typeOptionName2);
//             tft180_show_float(80, 15 * 2, *menuType[4].typeOption2, 1, 1);  // rolAngP: x.x格式
//             tft180_show_string(10, 15 * 3, menuType[4].typeOptionName3);
//             tft180_show_float(80, 15 * 3, *menuType[4].typeOption3, 1, 2);  // rolVelP: 0.xx格式
//             tft180_show_string(10, 15 * 4, menuType[4].typeOptionName4);
//             tft180_show_float(80, 15 * 4, *menuType[4].typeOption4, 1, 5);  // rolVelI: 0.xxxxx格式
//             for (int i = 0; i <= 4; i++)
//             {
//                 tft180_show_string(0, 15 * i, (i == menuState.level1) ? ">" : " ");
//             }
//             break;
//         case 5:  // 添加camera菜单的选项显示
//             if (menuState.level1 > 3)
//             {
//                 menuState.level1 = 0;
//             }
//             tft180_show_string(10, 15 * 0, menuType[5].typeOptionName0);
//             tft180_show_float(80, 15 * 0, *menuType[5].typeOption0, 3, 1);  // gain: 百位数，无小数
//             tft180_show_string(10, 15 * 1, menuType[5].typeOptionName1);
//             tft180_show_float(80, 15 * 1, *menuType[5].typeOption1, 4, 1);  // exposure: 千位数，无小数
//             tft180_show_string(10, 15 * 2, menuType[5].typeOptionName2);
//             tft180_show_float(80, 15 * 2, *menuType[5].typeOption2, 2, 1);  // auto_exp: 个位数，无小数
//             tft180_show_string(10, 15 * 3, menuType[5].typeOptionName3);
//             tft180_show_float(80, 15 * 3, *menuType[5].typeOption3, 4, 1);  // otsu: 百位数，无小数
//             for (int i = 0; i <= 3; i++)
//             {
//                 tft180_show_string(0, 15 * i, (i == menuState.level1) ? ">" : " ");
//             }
//             break;
//     }
// }

// static inline void menuEdit(void)
// {
//     switch (menuState.level0)
//     {
//         case 0:
//             tft180_show_string(10, 15 * 0, menuType[0].typeOptionName0);
//             tft180_show_float(80, 15 * 0, *menuType[0].typeOption0, 3, 1);
//             tft180_show_string(10, 15 * 1, menuType[0].typeOptionName1);
//             tft180_show_float(80, 15 * 1, *menuType[0].typeOption1, 1, 1);
//             tft180_show_string(10, 15 * 2, menuType[0].typeOptionName2);
//             tft180_show_float(80, 15 * 2, *menuType[0].typeOption2, 1, 2);
//             tft180_show_string(10, 15 * 3, menuType[0].typeOptionName3);
//             tft180_show_float(80, 15 * 3, *menuType[0].typeOption3, 1, 2);
//             tft180_show_string(10, 15 * 4, menuType[0].typeOptionName4);
//             tft180_show_float(80, 15 * 4, *menuType[0].typeOption4, 1, 2);
//             tft180_show_string(10, 15 * 5, menuType[0].typeOptionName5);
//             tft180_show_float(80, 15 * 5, *menuType[0].typeOption5, 2, 1);
//             tft180_show_string(10, 15 * 6, menuType[0].typeOptionName6);
//             tft180_show_float(80, 15 * 6, *menuType[0].typeOption6, 1, 3);
//             tft180_show_string(10, 15 * 7, menuType[0].typeOptionName7);
//             tft180_show_float(80, 15 * 7, *menuType[0].typeOption7, 2, 3);
//             for (int i = 0; i <= 7; i++)
//             {
//                 tft180_show_string(0, 15 * i, (i == menuState.level1) ? "$" : " ");  // 修改为 "$"
//             }
//             break;
//         case 1:
//             tft180_show_string(10, 15 * 0, menuType[1].typeOptionName0);
//             tft180_show_float(80, 15 * 0, *menuType[1].typeOption0, 2, 1);
//             tft180_show_string(10, 15 * 1, menuType[1].typeOptionName1);
//             tft180_show_float(80, 15 * 1, *menuType[1].typeOption1, 2, 1);
//             tft180_show_string(10, 15 * 2, menuType[1].typeOptionName2);
//             tft180_show_float(80, 15 * 2, *menuType[1].typeOption2, 2, 1);
//             tft180_show_string(10, 15 * 3, menuType[1].typeOptionName3);
//             tft180_show_float(80, 15 * 3, *menuType[1].typeOption3, 2, 1);
//             tft180_show_string(10, 15 * 4, menuType[1].typeOptionName4);
//             tft180_show_float(80, 15 * 4, *menuType[1].typeOption4, 2, 1);
//             tft180_show_string(10, 15 * 5, menuType[1].typeOptionName5);
//             tft180_show_float(80, 15 * 5, *menuType[1].typeOption5, 2, 1);
//             tft180_show_string(10, 15 * 6, menuType[1].typeOptionName6);
//             tft180_show_float(80, 15 * 6, *menuType[1].typeOption6, 2, 1);
//             tft180_show_string(10, 15 * 7, menuType[1].typeOptionName7);
//             tft180_show_float(80, 15 * 7, *menuType[1].typeOption7, 2, 1);
//             for (int i = 0; i <= 7; i++)
//             {
//                 tft180_show_string(0, 15 * i, (i == menuState.level1) ? "$" : " ");
//             }
//             break;
//         case 2:
//             tft180_show_string(10, 15 * 0, menuType[2].typeOptionName0);
//             tft180_show_float(80, 15 * 0, *menuType[2].typeOption0, 1, 4);
//             tft180_show_string(10, 15 * 1, menuType[2].typeOptionName1);
//             tft180_show_float(80, 15 * 1, *menuType[2].typeOption1, 1, 4);
//             for (int i = 0; i <= 1; i++)
//             {
//                 tft180_show_string(0, 15 * i, (i == menuState.level1) ? "$" : " ");
//             }
//             break;
//         case 3:
//             tft180_show_string(10, 15 * 0, menuType[3].typeOptionName0);
//             tft180_show_float(80, 15 * 0, *menuType[3].typeOption0, 4, 1);
//             tft180_show_string(10, 15 * 1, menuType[3].typeOptionName1);
//             tft180_show_float(80, 15 * 1, *menuType[3].typeOption1, 3, 1);
//             tft180_show_string(10, 15 * 2, menuType[3].typeOptionName2);
//             tft180_show_float(80, 15 * 2, *menuType[3].typeOption2, 1, 1);
//             tft180_show_string(10, 15 * 3, menuType[3].typeOptionName3);
//             tft180_show_float(80, 15 * 3, *menuType[3].typeOption3, 1, 4);
//             tft180_show_string(10, 15 * 4, menuType[3].typeOptionName4);
//             tft180_show_float(80, 15 * 4, *menuType[3].typeOption4, 1, 5);
//             for (int i = 0; i <= 4; i++)
//             {
//                 tft180_show_string(0, 15 * i, (i == menuState.level1) ? "$" : " ");
//             }
//             break;
//         case 4:
//             tft180_show_string(10, 15 * 0, menuType[4].typeOptionName0);
//             tft180_show_float(80, 15 * 0, *menuType[4].typeOption0, 4, 1);
//             tft180_show_string(10, 15 * 1, menuType[4].typeOptionName1);
//             tft180_show_float(80, 15 * 1, *menuType[4].typeOption1, 3, 1);
//             tft180_show_string(10, 15 * 2, menuType[4].typeOptionName2);
//             tft180_show_float(80, 15 * 2, *menuType[4].typeOption2, 1, 1);
//             tft180_show_string(10, 15 * 3, menuType[4].typeOptionName3);
//             tft180_show_float(80, 15 * 3, *menuType[4].typeOption3, 1, 2);
//             tft180_show_string(10, 15 * 4, menuType[4].typeOptionName4);
//             tft180_show_float(80, 15 * 4, *menuType[4].typeOption4, 1, 5);
//             for (int i = 0; i <= 4; i++)
//             {
//                 tft180_show_string(0, 15 * i, (i == menuState.level1) ? "$" : " ");
//             }
//             break;
//         case 5:  // 添加camera菜单的编辑显示
//             tft180_show_string(10, 15 * 0, menuType[5].typeOptionName0);
//             tft180_show_float(80, 15 * 0, *menuType[5].typeOption0, 3, 1);
//             tft180_show_string(10, 15 * 1, menuType[5].typeOptionName1);
//             tft180_show_float(80, 15 * 1, *menuType[5].typeOption1, 4, 1);
//             tft180_show_string(10, 15 * 2, menuType[5].typeOptionName2);
//             tft180_show_float(80, 15 * 2, *menuType[5].typeOption2, 2, 1);
//             tft180_show_string(10, 15 * 3, menuType[5].typeOptionName3);
//             tft180_show_float(80, 15 * 3, *menuType[5].typeOption3, 4, 1);
//             for (int i = 0; i <= 3; i++)
//             {
//                 tft180_show_string(0, 15 * i, (i == menuState.level1) ? "$" : " ");
//             }
//             break;
//     }
// }

// static inline void saveSettingsToFlash(void)
// {
//     flash_buffer_clear();
//     flash_union_buffer[0].float_type = image_center_x;
//     flash_union_buffer[1].float_type = imageGaussianSigma;
//     flash_union_buffer[2].float_type = imageDeadZone;
//     flash_union_buffer[3].float_type = imageDeadZoneDown;  // 添加 imageDeadZoneDown
//     flash_union_buffer[4].float_type = staticTurnErr;
//     flash_union_buffer[5].float_type = staticMarchErr;
//     flash_union_buffer[6].float_type = pitMid;
//     flash_union_buffer[7].float_type = rolMid;
//     flash_union_buffer[8].float_type = lStAng;
//     flash_union_buffer[9].float_type = rStAng;
//     flash_union_buffer[10].float_type = lReAng;
//     flash_union_buffer[11].float_type = rReAng;
//     flash_union_buffer[12].float_type = lturnAng;
//     flash_union_buffer[13].float_type = rturnAng;
//     flash_union_buffer[14].float_type = lOutAng;
//     flash_union_buffer[15].float_type = rOutAng;
//     flash_union_buffer[16].float_type = dynamicXeroPointGain0;
//     flash_union_buffer[17].float_type = dynamicXeroPointGain1;
//     flash_union_buffer[18].float_type = pitOmeP;
//     flash_union_buffer[19].float_type = pitOmeI;
//     flash_union_buffer[20].float_type = pitAngP;
//     flash_union_buffer[21].float_type = pitVelP;
//     flash_union_buffer[22].float_type = pitVelI;
//     flash_union_buffer[23].float_type = rolOmeP;
//     flash_union_buffer[24].float_type = rolOmeI;
//     flash_union_buffer[25].float_type = rolAngP;
//     flash_union_buffer[26].float_type = rolVelP;
//     flash_union_buffer[27].float_type = rolVelI;
//     flash_union_buffer[28].float_type = a;     // 保存摄像头增益
//     flash_union_buffer[29].float_type = b;     // 保存摄像头曝光
//     flash_union_buffer[30].float_type = c;     // 保存自动曝光
//     flash_union_buffer[31].float_type = otsu;  // 保存大津法阈值
//     if (flash_check(0, 0))
//     {
//         flash_erase_page(0, 0);
//     }
//     flash_write_page_from_buffer(0, 0);
//     flash_buffer_clear();
// }

// static uint8 flashFlag = 1;
// void menuScanner(void)
// {
//     if (flashFlag)
//     {
//         if (flash_check(0, 0))
//         {
//             flash_buffer_clear();
//             flash_read_page_to_buffer(0, 0);
//             image_center_x = flash_union_buffer[0].float_type;
//             imageGaussianSigma = flash_union_buffer[1].float_type;
//             imageDeadZone = flash_union_buffer[2].float_type;
//             imageDeadZoneDown = flash_union_buffer[3].float_type;  // 添加 imageDeadZoneDown
//             staticTurnErr = flash_union_buffer[4].float_type;
//             staticMarchErr = flash_union_buffer[5].float_type;
//             pitMid = flash_union_buffer[6].float_type;
//             rolMid = flash_union_buffer[7].float_type;
//             lStAng = flash_union_buffer[8].float_type;
//             rStAng = flash_union_buffer[9].float_type;
//             lReAng = flash_union_buffer[10].float_type;
//             rReAng = flash_union_buffer[11].float_type;
//             lturnAng = flash_union_buffer[12].float_type;
//             rturnAng = flash_union_buffer[13].float_type;
//             lOutAng = flash_union_buffer[14].float_type;
//             rOutAng = flash_union_buffer[15].float_type;
//             dynamicXeroPointGain0 = flash_union_buffer[16].float_type;
//             dynamicXeroPointGain1 = flash_union_buffer[17].float_type;
//             pitOmeP = flash_union_buffer[18].float_type;
//             pitOmeI = flash_union_buffer[19].float_type;
//             pitAngP = flash_union_buffer[20].float_type;
//             pitVelP = flash_union_buffer[21].float_type;
//             pitVelI = flash_union_buffer[22].float_type;
//             rolOmeP = flash_union_buffer[23].float_type;
//             rolOmeI = flash_union_buffer[24].float_type;
//             rolAngP = flash_union_buffer[25].float_type;
//             rolVelP = flash_union_buffer[26].float_type;
//             rolVelI = flash_union_buffer[27].float_type;
//             a = flash_union_buffer[28].float_type;     // 读取摄像头增益
//             b = flash_union_buffer[29].float_type;     // 读取摄像头曝光
//             c = flash_union_buffer[30].float_type;     // 读取自动曝光
//             otsu = flash_union_buffer[31].float_type;  // 读取大津法阈值
//             flashFlag = 0;
//         }
//         flash_buffer_clear();
//         flashFlag = 0;
//     }

//     switch (menuState.menuState)
//     {
//         case select0:
//             menuSelectLevel0();
//             if (key_get_state(KEY_1) == KEY_SHORT_PRESS)
//             {
//                 menuState.level0++;
//                 if (menuState.level0 >= 6)  // 修改为6个菜单
//                 {
//                     menuState.level0 = 0;
//                 }
//             }
//             else if (key_get_state(KEY_2) == KEY_SHORT_PRESS)
//             {
//                 tft180_clear();
//                 menuState.menuState = select1;
//             }
//             else if (key_get_state(KEY_3) == KEY_SHORT_PRESS)
//             {
//                 controllerInit();
//                 saveSettingsToFlash();
//                 tft180_full(RGB565_GREEN);
//                 tft180_full(RGB565_WHITE);
//             }
//             else if (key_get_state(KEY_3) == KEY_LONG_PRESS)
//             {
//                 flash_erase_page(0, 0);
//                 tft180_full(RGB565_RED);
//                 while (key_get_state(KEY_3) == KEY_LONG_PRESS)
//                     ;
//                 tft180_full(RGB565_WHITE);
//             }
//             else if (key_get_state(KEY_4) == KEY_SHORT_PRESS)
//             {
//                 tft180_clear();
//                 menuState.menuState = camera;
//             }

//             break;

//         case select1:
//             menuSelectLevel1();
//             if (key_get_state(KEY_1) == KEY_SHORT_PRESS)
//             {
//                 menuState.level1++;
//                 if (menuState.level0 == 0 && menuState.level1 > 7)
//                 {
//                     menuState.level1 = 0;
//                 }
//                 else if (menuState.level0 == 1 && menuState.level1 > 7)
//                 {
//                     menuState.level1 = 0;
//                 }
//                 else if (menuState.level0 == 2 && menuState.level1 > 1)
//                 {
//                     menuState.level1 = 0;
//                 }
//                 else if (menuState.level0 == 3 && menuState.level1 > 4)
//                 {
//                     menuState.level1 = 0;
//                 }
//                 else if (menuState.level0 == 4 && menuState.level1 > 4)
//                 {
//                     menuState.level1 = 0;
//                 }
//                 else if (menuState.level0 == 5 && menuState.level1 > 3)  // camera菜单有4个选项(0-3)
//                 {
//                     menuState.level1 = 0;
//                 }
//             }
//             else if (key_get_state(KEY_2) == KEY_SHORT_PRESS)
//             {
//                 tft180_clear();
//                 menuState.menuState = edit;
//             }
//             else if (key_get_state(KEY_3) == KEY_SHORT_PRESS)
//             {
//                 tft180_clear();
//                 menuState.menuState = select0;
//             }
//             else if (key_get_state(KEY_4) == KEY_SHORT_PRESS)
//             {
//                 tft180_clear();
//                 menuState.menuState = camera;
//             }
//             break;

//         case edit:
//             menuEdit();
//             if (key_get_state(KEY_4) == KEY_LONG_PRESS)
//             {
//                 tft180_clear();
//                 controllerInit();
//                 saveSettingsToFlash();

//                 menuState.menuState = select1;
//                 while (key_get_state(KEY_4) == KEY_LONG_PRESS)
//                     ;
//             }
//             // Short press: edit current option by step
//             else if (key_get_state(KEY_2) == KEY_SHORT_PRESS)
//             {
//                 switch (menuState.level1)
//                 {
//                     case 0:
//                         *menuType[menuState.level0].typeOption0 -= menuType[menuState.level0].typeOptionStep0;
//                         break;
//                     case 1:
//                         *menuType[menuState.level0].typeOption1 -= menuType[menuState.level0].typeOptionStep1;
//                         break;
//                     case 2:
//                         *menuType[menuState.level0].typeOption2 -= menuType[menuState.level0].typeOptionStep2;
//                         break;
//                     case 3:
//                         *menuType[menuState.level0].typeOption3 -= menuType[menuState.level0].typeOptionStep3;
//                         break;
//                     case 4:
//                         *menuType[menuState.level0].typeOption4 -= menuType[menuState.level0].typeOptionStep4;
//                         break;
//                     case 5:
//                         *menuType[menuState.level0].typeOption5 -= menuType[menuState.level0].typeOptionStep5;
//                         break;
//                     case 6:
//                         *menuType[menuState.level0].typeOption6 -= menuType[menuState.level0].typeOptionStep6;
//                         break;
//                     case 7:
//                         *menuType[menuState.level0].typeOption7 -= menuType[menuState.level0].typeOptionStep7;
//                         break;
//                 }
//             }
//             else if (key_get_state(KEY_3) == KEY_SHORT_PRESS)
//             {
//                 switch (menuState.level1)
//                 {
//                     case 0:
//                         *menuType[menuState.level0].typeOption0 += menuType[menuState.level0].typeOptionStep0;
//                         break;
//                     case 1:
//                         *menuType[menuState.level0].typeOption1 += menuType[menuState.level0].typeOptionStep1;
//                         break;
//                     case 2:
//                         *menuType[menuState.level0].typeOption2 += menuType[menuState.level0].typeOptionStep2;
//                         break;
//                     case 3:
//                         *menuType[menuState.level0].typeOption3 += menuType[menuState.level0].typeOptionStep3;
//                         break;
//                     case 4:
//                         *menuType[menuState.level0].typeOption4 += menuType[menuState.level0].typeOptionStep4;
//                         break;
//                     case 5:
//                         *menuType[menuState.level0].typeOption5 += menuType[menuState.level0].typeOptionStep5;
//                         break;
//                     case 6:
//                         *menuType[menuState.level0].typeOption6 += menuType[menuState.level0].typeOptionStep6;
//                         break;
//                     case 7:
//                         *menuType[menuState.level0].typeOption7 += menuType[menuState.level0].typeOptionStep7;
//                         break;
//                 }
//             }
//             // Long press: edit current option by stepXX
//             else if (key_get_state(KEY_1) == KEY_SHORT_PRESS)
//             {
//                 tft180_clear();
//                 switch (menuState.level1)
//                 {
//                     case 0:
//                         *menuType[menuState.level0].typeOption0 -= menuType[menuState.level0].typeOptionStep00;
//                         break;
//                     case 1:
//                         *menuType[menuState.level0].typeOption1 -= menuType[menuState.level0].typeOptionStep11;
//                         break;
//                     case 2:
//                         *menuType[menuState.level0].typeOption2 -= menuType[menuState.level0].typeOptionStep22;
//                         break;
//                     case 3:
//                         *menuType[menuState.level0].typeOption3 -= menuType[menuState.level0].typeOptionStep33;
//                         break;
//                     case 4:
//                         *menuType[menuState.level0].typeOption4 -= menuType[menuState.level0].typeOptionStep44;
//                         break;
//                     case 5:
//                         *menuType[menuState.level0].typeOption5 -= menuType[menuState.level0].typeOptionStep55;
//                         break;
//                     case 6:
//                         *menuType[menuState.level0].typeOption6 -= menuType[menuState.level0].typeOptionStep66;
//                         break;
//                     case 7:
//                         *menuType[menuState.level0].typeOption7 -= menuType[menuState.level0].typeOptionStep77;
//                         break;
//                 }
//             }
//             else if (key_get_state(KEY_4) == KEY_SHORT_PRESS)
//             {
//                 tft180_clear();
//                 switch (menuState.level1)
//                 {
//                     case 0:
//                         *menuType[menuState.level0].typeOption0 += menuType[menuState.level0].typeOptionStep00;
//                         break;
//                     case 1:
//                         *menuType[menuState.level0].typeOption1 += menuType[menuState.level0].typeOptionStep11;
//                         break;
//                     case 2:
//                         *menuType[menuState.level0].typeOption2 += menuType[menuState.level0].typeOptionStep22;
//                         break;
//                     case 3:
//                         *menuType[menuState.level0].typeOption3 += menuType[menuState.level0].typeOptionStep33;
//                         break;
//                     case 4:
//                         *menuType[menuState.level0].typeOption4 += menuType[menuState.level0].typeOptionStep44;
//                         break;
//                     case 5:
//                         *menuType[menuState.level0].typeOption5 += menuType[menuState.level0].typeOptionStep55;
//                         break;
//                     case 6:
//                         *menuType[menuState.level0].typeOption6 += menuType[menuState.level0].typeOptionStep66;
//                         break;
//                     case 7:
//                         *menuType[menuState.level0].typeOption7 += menuType[menuState.level0].typeOptionStep77;
//                         break;
//                 }
//             }
//             break;

//         case camera:
//             tft180_show_gray_image(0, 0, image0_show[0], MT9V03X_W, MT9V03X_H, 94, 60, 0);
//             tft180_show_gray_image(0, 61, mt9v03x_image[0], MT9V03X_W, MT9V03X_H, 94, 60, 0);
//             uint8 X = 0.5f * nerest_X;
//             uint8 Y = 0.5f * nerest_Y;
//             tft180_show_uint(X, Y, blob0_counts, 1);
//             tft180_draw_line(0.5f * image_center_x, 0, 0.5f * image_center_x, 60, RGB565_GREEN);

//             tft180_show_string(95, 15 * 0, "vYawErr");
//             tft180_show_float(95, 15 * 1, vYawErr, 1, 3);  // 偏航误差：xx.xx格式
//             // tft180_show_string(95, 15 * 2, "rYawErr");
//             // tft180_show_float(95, 15 * 3, yawOmeTar, 2, 3);

//             // tft180_show_string(0, 61 + 15 * 0, "area");
//             // tft180_show_uint(40, 61 + 15 * 0, blobArea, 3);
//             // tft180_show_string(0, 61 + 15 * 1, "pitch");
//             // tft180_show_float(40, 61 + 15 * 1, imuData.roll, 1, 3);   // 姿态角：x.xxx格式
//             // tft180_show_string(0, 61 + 15 * 2, "roll");
//             // tft180_show_float(40, 61 + 15 * 2, imuData.pitch, 1, 3);  // 姿态角：-x.xxx格式
//             // tft180_show_string(0, 61 + 15 * 3, "yaw");
//             // tft180_show_float(40, 61 + 15 * 3, imuData.yaw, 1, 3);    // 偏航角：xxx.xx格式

//             if (key_get_state(KEY_4) == KEY_SHORT_PRESS)
//             {
//                 state = 1;
//                 state1 = 1;
//             }
//             if (key_get_state(KEY_2) == KEY_SHORT_PRESS)
//             {
//                 tft180_clear();
//                 menuState.menuState = select0;
//             }
//             if (key_get_state(KEY_3) == KEY_SHORT_PRESS)
//             {
//                 controllerInit();
//                 saveSettingsToFlash();
//                 state1 = 0;
//                 tft180_full(RGB565_GREEN);
//                 tft180_full(RGB565_WHITE);
//             }
//             break;
//     }
//     key_clear_all_state();
// }
