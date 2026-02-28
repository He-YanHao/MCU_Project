#ifndef __RTC_TIME_H__
#define __RTC_TIME_H__

#include <time.h>

void BSP_RTC_Init(void);
void RTC_SetTime(int year, int mon, int day, int hour, int min, int sec);
void RTC_GetTime(struct tm *time_new);
void RTC_PrintfTime(void);

#endif
