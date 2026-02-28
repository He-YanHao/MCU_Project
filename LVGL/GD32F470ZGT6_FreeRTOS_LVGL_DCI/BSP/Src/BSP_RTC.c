#include "BSP_RTC.h"
#include <stdio.h>

void rtc_time_init(void)
{
	//使能电源管理时钟
	rcu_periph_clock_enable(RCU_PMU);
	//启用对备份域中寄存器的写访问
	pmu_backup_write_enable();
	//打开振荡器 32.768 KHz
	rcu_osci_on(RCU_LXTAL);
	//等待振荡器32.768 KHz稳定的标志设置或振荡器启动超时
	rcu_osci_stab_wait(RCU_LXTAL);
	//配置RTC时钟源选择LXTAL=32.768 KHz晶振
	rcu_rtc_clock_config(RCU_RTCSRC_LXTAL);
	//开启RTC时钟
	rcu_periph_clock_enable(RCU_RTC);
	//等到时钟同步后，再进行寄存器更新
	rtc_register_sync_wait();

	//获取RTC时钟项选择
	//获取备份域控制寄存器（RCU_BDCTL）的第8第9位状态
	RTC_BKP0 = GET_BITS(RCU_BDCTL, 8, 9);

	if(RTC_BKP0 == 0x02)
	{
        
	} else
	{
		RTC_BKP0 = 0x02;
		rtc_set_time(0x26,0x01,0x13,0x02,0x02,0x13,0x40);
	}
}

void rtc_set_time(uint8_t year, uint8_t month, uint8_t date, uint8_t week, uint8_t hour, uint8_t minute, uint8_t second)
{
    rtc_parameter_struct   rtc_initpara;
    rtc_initpara.factor_asyn = 0x7F;        //RTC异步预分频值:0x0 ~ 0x7F
    rtc_initpara.factor_syn  = 0xFF;        //RTC同步预分频值:0x0 - 0x7FFF
    rtc_initpara.year        = year;        //设置年份
    rtc_initpara.month       = month;       //设置月份  
    rtc_initpara.date        = date;        //设置日期
    rtc_initpara.day_of_week = week;        //设置星期
    rtc_initpara.hour        = hour;        //设置时
    rtc_initpara.minute      = minute;      //设置分钟  
    rtc_initpara.second      = second;      //设置秒
    rtc_initpara.display_format = RTC_24HOUR;//24小时制    
    rtc_init(&rtc_initpara);
}


//BCD转10进制
int BcdToDecimal(int bcd) 
{
	int decimal = 0;
	int temp = 1;
	int number = 0;
	while(bcd > 0) 
	{
		number = bcd % 16;
		decimal += number * temp;
		temp *= 10;
		bcd /= 16;
	}
    return decimal;
}

void RtcShowTime(void)
{
	rtc_parameter_struct   rtc_initpara_time;
	rtc_current_time_get(&rtc_initpara_time);  
		
	printf("Current time: %d:%d:%d\n\r",
    BcdToDecimal(rtc_initpara_time.hour),
    BcdToDecimal(rtc_initpara_time.minute), 
	BcdToDecimal(rtc_initpara_time.second));
		
	printf("%d-%d-%d\n\r", \
	BcdToDecimal(rtc_initpara_time.year) + 2000, 
	BcdToDecimal(rtc_initpara_time.month), 
	BcdToDecimal(rtc_initpara_time.date));
}
