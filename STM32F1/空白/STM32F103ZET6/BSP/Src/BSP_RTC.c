#include "BSP_RTC.h"
#include "stm32f10x.h"
#include <stdio.h>

#define BKP_DR1_RTC_TIME 0X0002

/**
  * 函    数：RTC初始化
  * 参    数：无
  * 返 回 值：无
  */
void BSP_RTC_Init(void)
{
    // 开启PWR和BKP的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);			
	// 备份寄存器访问使能 开启对备份寄存器的访问
	PWR_BackupAccessCmd(ENABLE);
	// 查看备份寄存器的标志位 判断RTC是否配置过了
	if (BKP_ReadBackupRegister(BKP_DR1) != BKP_DR1_RTC_TIME)			
	{
        // 开启LSE时钟
		RCC_LSEConfig(RCC_LSE_ON);
        // 等待LSE准备就绪
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) != SET);	
		// 选择RTCCLK来源为LSE
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
        // RTC CLK使能
		RCC_RTCCLKCmd(ENABLE);
		// 等待同步
		RTC_WaitForSynchro();
        // 等待上一次操作完成
		RTC_WaitForLastTask();
		// 设置RTC预分频器 预分频后的计数频率为 1Hz
		RTC_SetPrescaler(32768 - 1);
        // 等待上一次操作完成
		RTC_WaitForLastTask();
		// 设置默认时间
		RTC_SetTime(2026,1,18,7,9,0);
        // 写入代表RTC已设入时间的标志位
		BKP_WriteBackupRegister(BKP_DR1, BKP_DR1_RTC_TIME);
	} else {
        // RTC不是第一次配置
        // 等待同步
		RTC_WaitForSynchro();
        // 等待上一次操作完成
		RTC_WaitForLastTask();
	}
}

// 设置RTC时间
void RTC_SetTime(int year, int mon, int day, int hour, int min, int sec)
{
	struct tm time_date = {.tm_year = year - 1900,
                           .tm_mon  = mon - 1,
                           .tm_mday = day,
                           .tm_hour = hour,
                           .tm_min  = min,
                           .tm_sec  = sec
                          };
    // 获取秒 并转化为东八区
	time_t time_s = mktime(&time_date) - 8 * 60 * 60;
    // 将秒计数器写入到RTC的CNT中
	RTC_SetCounter(time_s);
    // 等待上一次操作完成
    RTC_WaitForLastTask();
}

// 获取RTC时间
void RTC_GetTime(struct tm *time_new)
{
    // 读取RTC的CNT 获取当前的秒计数器 并进行东八区修正
	time_t time_s = RTC_GetCounter() + 8 * 60 * 60;		
    // 将秒计数器转换为日期时间格式
    *time_new = *localtime(&time_s);
}

void RTC_PrintfTime(void)
{
    struct tm time_new = {0};
    RTC_GetTime(&time_new);
    printf("new time is:\r\n");
    printf("%d year %d mon %d day %d week\r\n", time_new.tm_year + 1900, time_new.tm_mon + 1, time_new.tm_mday, time_new.tm_wday);
    printf("%d hour %d min %d sec\r\n", time_new.tm_hour, time_new.tm_min, time_new.tm_sec);
}
