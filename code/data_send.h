#ifndef _data_send_h
#define _data_send_h

#define VOFA_SEND_MODE 0

extern float icp_read_buffer_float[128];

void vofa_send_1ch(float date);
void vofa_send_2ch(float date1, float date2);
void vofa_send_3ch(float date1, float date2, float date3);
void vofa_send_4ch(float date1, float date2, float date3, float date4);
void vofa_send_5ch(float date1, float date2, float date3, float date4, float date5);
void vofa_send_6ch(float date1, float date2, float date3, float date4, float date5, float date6);
void vofa_send_7ch(float date1, float date2, float date3, float date4, float date5, float date6, float date7);
void vofa_send_8ch(float date1, float date2, float date3, float date4, float date5, float date6, float date7, float date8);
void vofa_send_9ch(float date1, float date2, float date3, float date4, float date5, float date6, float date7, float date8, float date9);
void vofa_send_10ch(float date1, float date2, float date3, float date4, float date5, float date6, float date7, float date8, float date9, float date10);
void vofa_send_11ch(float date1, float date2, float date3, float date4, float date5, float date6, float date7, float date8, float date9, float date10, float date11);
void vofa_send_12ch(float date1, float date2, float date3, float date4, float date5, float date6, float date7, float date8, float date9, float date10, float date11, float date12);

#endif
