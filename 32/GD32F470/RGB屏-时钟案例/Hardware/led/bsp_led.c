#include "bsp_led.h"

/************************************************
函数名称 ： led_gpio_config
功    能 ： led灯gpio引脚配置
参    数 ： 无
返 回 值 ： 无
作    者 ： LC
*************************************************/
void led_gpio_config(void)
{
	/* 使能时钟 */
	rcu_periph_clock_enable(RCU_LED1);
	/* 配置为输出模式 浮空模式 */
	gpio_mode_set(PORT_LED1,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,PIN_LED1);
	/* 配置为推挽输出 50MHZ */
	gpio_output_options_set(PORT_LED1,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,PIN_LED1);

	/* 使能时钟 */
	rcu_periph_clock_enable(RCU_LED2);
	/* 配置为输出模式 浮空模式 */
	gpio_mode_set(PORT_LED2,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,PIN_LED2);
	/* 配置为推挽输出 50MHZ */
	gpio_output_options_set(PORT_LED2,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,PIN_LED2);

	/* 使能时钟 */
	rcu_periph_clock_enable(RCU_LED3);
	/* 配置为输出模式 浮空模式 */
	gpio_mode_set(PORT_LED3,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,PIN_LED3);
	/* 配置为推挽输出 50MHZ */
	gpio_output_options_set(PORT_LED3,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,PIN_LED3);

	/* 使能时钟 */
	rcu_periph_clock_enable(RCU_LED4);
	/* 配置为输出模式 浮空模式 */
	gpio_mode_set(PORT_LED4,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,PIN_LED4);
	/* 配置为推挽输出 50MHZ */
	gpio_output_options_set(PORT_LED4,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,PIN_LED4);
}


uint32_t LED_PORT[4] = {PORT_LED1, PORT_LED2, PORT_LED3, PORT_LED4};
uint32_t LED_PIN[4] = {PIN_LED1, PIN_LED2, PIN_LED3, PIN_LED4};
/**********************************************************
 * 函 数 名 称：led_waterfall_light
 * 函 数 功 能：LED流水灯
 * 传 入 参 数：无
 * 函 数 返 回：无
 * 作       者：LCKFB
 * 备       注：led_speed为流水速度，越大越慢
**********************************************************/
void led_waterfall_light(void)
{
	static int led_count = 0;
	static int i = 0;
	int led_speed = 2000;
	
	led_count++;
	
	if( led_count >= led_speed )
	{
		led_count = 0;
		gpio_bit_toggle(LED_PORT[i],LED_PIN[i]);
		i = (i+1)%4;
	}
}
