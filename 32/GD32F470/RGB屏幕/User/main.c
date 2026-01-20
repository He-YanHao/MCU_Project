#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "bsp_led.h"

int main(void)
{

    systick_config();   // 滴答定时器初始化
	  led_gpio_config();  // led初始化

    gpio_bit_set(GPIOD,GPIO_PIN_7);             // PD7输出高电平
	  //gpio_bit_reset(GPIOD,GPIO_PIN_7);						// PD7输出低电平
    gpio_bit_write(PORT_LED2,PIN_LED2,RESET);		// LED2输出低电平
		//gpio_bit_write(PORT_LED2,PIN_LED2,SET);  			// LED2输出高电平
    while(1) 
		{
			
    }
}
