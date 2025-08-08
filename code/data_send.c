#include "zf_common_headfile.h"

static uint8 vofa_tail[4] = {0x00, 0x00, 0x80, 0x7f};

#if VOFA_SEND_MODE == 0

void vofa_send_1ch(float date)
{
    uint8 temp[4] = {0};
    memcpy(temp, &date, 4);
    wireless_uart_send_buffer(temp, sizeof(temp));
    wireless_uart_send_buffer(vofa_tail, sizeof(vofa_tail));
}

void vofa_send_2ch(float date1, float date2)
{
    float temp[2] = {date1, date2};
    uint8 temp1[8] = {0};
    memcpy(temp1, temp, 8);
    wireless_uart_send_buffer(temp1, sizeof(temp1));
    wireless_uart_send_buffer(vofa_tail, sizeof(vofa_tail));
}

void vofa_send_3ch(float date1, float date2, float date3)
{
    float temp[3] = {date1, date2, date3};
    uint8 temp1[12] = {0};
    memcpy(temp1, temp, 12);
    wireless_uart_send_buffer(temp1, sizeof(temp1));
    wireless_uart_send_buffer(vofa_tail, sizeof(vofa_tail));
}

void vofa_send_4ch(float date1, float date2, float date3, float date4)
{
    float temp[4] = {date1, date2, date3, date4};
    uint8 temp1[16] = {0};
    memcpy(temp1, temp, 16);
    wireless_uart_send_buffer(temp1, sizeof(temp1));
    wireless_uart_send_buffer(vofa_tail, sizeof(vofa_tail));
}

void vofa_send_5ch(float date1, float date2, float date3, float date4, float date5)
{
    float temp[5] = {date1, date2, date3, date4, date5};
    uint8 temp1[20] = {0};
    memcpy(temp1, temp, 20);
    wireless_uart_send_buffer(temp1, sizeof(temp1));
    wireless_uart_send_buffer(vofa_tail, sizeof(vofa_tail));
}

void vofa_send_6ch(float date1, float date2, float date3, float date4, float date5, float date6)
{
    float temp[6] = {date1, date2, date3, date4, date5, date6};
    uint8 temp1[24] = {0};
    memcpy(temp1, temp, 24);
    wireless_uart_send_buffer(temp1, sizeof(temp1));
    wireless_uart_send_buffer(vofa_tail, sizeof(vofa_tail));
}

void vofa_send_7ch(float date1, float date2, float date3, float date4, float date5, float date6, float date7)
{
    float temp[7] = {date1, date2, date3, date4, date5, date6, date7};
    uint8 temp1[28] = {0};
    memcpy(temp1, temp, 28);
    wireless_uart_send_buffer(temp1, sizeof(temp1));
    wireless_uart_send_buffer(vofa_tail, sizeof(vofa_tail));
}

void vofa_send_8ch(float date1, float date2, float date3, float date4, float date5, float date6, float date7, float date8)
{
    float temp[8] = {date1, date2, date3, date4, date5, date6, date7, date8};
    uint8 temp1[32] = {0};
    memcpy(temp1, temp, 32);
    wireless_uart_send_buffer(temp1, sizeof(temp1));
    wireless_uart_send_buffer(vofa_tail, sizeof(vofa_tail));
}

void vofa_send_9ch(float date1, float date2, float date3, float date4, float date5, float date6, float date7, float date8, float date9)
{
    float temp[9] = {date1, date2, date3, date4, date5, date6, date7, date8, date9};
    uint8 temp1[36] = {0};
    memcpy(temp1, temp, 36);
    wireless_uart_send_buffer(temp1, sizeof(temp1));
    wireless_uart_send_buffer(vofa_tail, sizeof(vofa_tail));
}

void vofa_send_10ch(float date1, float date2, float date3, float date4, float date5, float date6, float date7, float date8, float date9, float date10)
{
    float temp[10] = {date1, date2, date3, date4, date5, date6, date7, date8, date9, date10};
    uint8 temp1[40] = {0};
    memcpy(temp1, temp, 40);
    wireless_uart_send_buffer(temp1, sizeof(temp1));
    wireless_uart_send_buffer(vofa_tail, sizeof(vofa_tail));
}

void vofa_send_11ch(float date1, float date2, float date3, float date4, float date5, float date6, float date7, float date8, float date9, float date10, float date11)
{
    float temp[11] = {date1, date2, date3, date4, date5, date6, date7, date8, date9, date10, date11};
    uint8 temp1[44] = {0};
    memcpy(temp1, temp, 44);
    wireless_uart_send_buffer(temp1, sizeof(temp1));
    wireless_uart_send_buffer(vofa_tail, sizeof(vofa_tail));
}

void vofa_send_12ch(float date1, float date2, float date3, float date4, float date5, float date6, float date7, float date8, float date9, float date10, float date11, float date12)
{
    float temp[12] = {date1, date2, date3, date4, date5, date6, date7, date8, date9, date10, date11, date12};
    uint8 temp1[48] = {0};
    memcpy(temp1, temp, 48);
    wireless_uart_send_buffer(temp1, sizeof(temp1));
    wireless_uart_send_buffer(vofa_tail, sizeof(vofa_tail));
}

#elif VOFA_SEND_MODE == 1

void vofa_send_1ch(float date)
{
    float temp[1] = {date};
    wifi_spi_send_buffer((uint8 *) temp, sizeof(float));
    wifi_spi_send_buffer(vofa_tail, sizeof(vofa_tail));
}

void vofa_send_2ch(float date1, float date2)
{
    float temp[2] = {date1, date2};
    wifi_spi_send_buffer((uint8 *) temp, sizeof(float) * 2);
    wifi_spi_send_buffer(vofa_tail, sizeof(vofa_tail));
}

void vofa_send_3ch(float date1, float date2, float date3)
{
    float temp[3] = {date1, date2, date3};
    wifi_spi_send_buffer((uint8 *) temp, sizeof(float) * 3);
    wifi_spi_send_buffer(vofa_tail, sizeof(vofa_tail));
}

void vofa_send_4ch(float date1, float date2, float date3, float date4)
{
    float temp[4] = {date1, date2, date3, date4};
    wifi_spi_send_buffer((uint8 *) temp, sizeof(float) * 4);
    wifi_spi_send_buffer(vofa_tail, sizeof(vofa_tail));
}

void vofa_send_5ch(float date1, float date2, float date3, float date4, float date5)
{
    float temp[5] = {date1, date2, date3, date4, date5};
    wifi_spi_send_buffer((uint8 *) temp, sizeof(float) * 5);
    wifi_spi_send_buffer(vofa_tail, sizeof(vofa_tail));
}

void vofa_send_6ch(float date1, float date2, float date3, float date4, float date5, float date6)
{
    float temp[6] = {date1, date2, date3, date4, date5, date6};
    wifi_spi_send_buffer((uint8 *) temp, sizeof(float) * 6);
    wifi_spi_send_buffer(vofa_tail, sizeof(vofa_tail));
}

void vofa_send_7ch(float date1, float date2, float date3, float date4, float date5, float date6, float date7)
{
    float temp[7] = {date1, date2, date3, date4, date5, date6, date7};
    wifi_spi_send_buffer((uint8 *) temp, sizeof(float) * 7);
    wifi_spi_send_buffer(vofa_tail, sizeof(vofa_tail));
}

void vofa_send_8ch(float date1, float date2, float date3, float date4, float date5, float date6, float date7, float date8)
{
    float temp[8] = {date1, date2, date3, date4, date5, date6, date7, date8};
    wifi_spi_send_buffer((uint8 *) temp, sizeof(float) * 8);
    wifi_spi_send_buffer(vofa_tail, sizeof(vofa_tail));
}

void vofa_send_9ch(float date1, float date2, float date3, float date4, float date5, float date6, float date7, float date8, float date9)
{
    float temp[9] = {date1, date2, date3, date4, date5, date6, date7, date8, date9};
    wifi_spi_send_buffer((uint8 *) temp, sizeof(float) * 9);
    wifi_spi_send_buffer(vofa_tail, sizeof(vofa_tail));
}

void vofa_send_10ch(float date1, float date2, float date3, float date4, float date5, float date6, float date7, float date8, float date9, float date10)
{
    float temp[10] = {date1, date2, date3, date4, date5, date6, date7, date8, date9, date10};
    wifi_spi_send_buffer((uint8 *) temp, sizeof(float) * 10);
    wifi_spi_send_buffer(vofa_tail, sizeof(vofa_tail));
}

void vofa_send_11ch(float date1, float date2, float date3, float date4, float date5, float date6, float date7, float date8, float date9, float date10, float date11)
{
    float temp[11] = {date1, date2, date3, date4, date5, date6, date7, date8, date9, date10, date11};
    wifi_spi_send_buffer((uint8 *) temp, sizeof(float) * 11);
    wifi_spi_send_buffer(vofa_tail, sizeof(vofa_tail));
}

void vofa_send_12ch(float date1, float date2, float date3, float date4, float date5, float date6, float date7, float date8, float date9, float date10, float date11, float date12)
{
    float temp[12] = {date1, date2, date3, date4, date5, date6, date7, date8, date9, date10, date11, date12};
    wifi_spi_send_buffer((uint8 *) temp, sizeof(float) * 12);
    wifi_spi_send_buffer(vofa_tail, sizeof(vofa_tail));
}

#endif
