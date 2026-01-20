#ifndef _BSP_LED_H
#define _BSP_LED_H

#include "gd32f4xx.h"
#include "systick.h"


#define RCU_LED1  		RCU_GPIOE   		// GPIOD的时钟
#define PORT_LED1 		GPIOE				// GPIOD的端口
#define PIN_LED1 		GPIO_PIN_3  		// GPIOD的引脚


#define RCU_LED2  		RCU_GPIOD   		// GPIOD的时钟
#define PORT_LED2 		GPIOD				// GPIOD的端口
#define PIN_LED2 		GPIO_PIN_7  		// GPIOD的引脚


#define RCU_LED3  		RCU_GPIOG   		// GPIOD的时钟
#define PORT_LED3 		GPIOG				// GPIOD的端口
#define PIN_LED3 		GPIO_PIN_3  		// GPIOD的引脚


#define RCU_LED4  		RCU_GPIOA   		// GPIOD的时钟
#define PORT_LED4 		GPIOA				// GPIOD的端口
#define PIN_LED4 		GPIO_PIN_5  		// GPIOD的引脚

void led_gpio_config(void); 	  // led gpio引脚配置
void led_waterfall_light(void);

#endif /* BSP_LED_H */
