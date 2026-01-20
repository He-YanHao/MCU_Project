#ifndef _BSP_RTC_H__
#define _BSP_RTC_H__

#include "gd32f4xx.h"

void rtc_config(void);
void RtcShowTime(void);
void RtcTimeConfig(uint8_t year, uint8_t month, uint8_t date, uint8_t week, uint8_t hour, uint8_t minute, uint8_t second);
#endif
