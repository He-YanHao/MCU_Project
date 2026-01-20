#ifndef __HC595_LED_H
#define __HC595_LED_H

void HC595_LED_init(void);
void LED_XIAN (uint8_t X, uint8_t Y);
void LED_Display (uint16_t Num);

#define LED_DIO_SET GPIO_SetBits(GPIOD,GPIO_Pin_0)//PD0÷√∏ﬂ
#define LED_DIO_CLR GPIO_ResetBits(GPIOD,GPIO_Pin_0)//PD0÷√µÕ
#define LED_RCLK_SET GPIO_SetBits(GPIOD,GPIO_Pin_1)//PD0÷√∏ﬂ
#define LED_RCLK_CLR GPIO_ResetBits(GPIOD,GPIO_Pin_1)//PD0÷√µÕ
#define LED_SCLK_SET GPIO_SetBits(GPIOD,GPIO_Pin_5)//PD0÷√∏ﬂ
#define LED_SCLK_CLR GPIO_ResetBits(GPIOD,GPIO_Pin_5)//PD0÷√µÕ

#endif
