#ifndef __RTC_H__
#define __RTC_H__

#include "gd32f4xx.h"

void rtc_time_init(void);
void rtc_set_time(uint8_t year, uint8_t month, uint8_t date, uint8_t week, uint8_t hour, uint8_t minute, uint8_t second);
void RtcShowTime(void);

#endif
