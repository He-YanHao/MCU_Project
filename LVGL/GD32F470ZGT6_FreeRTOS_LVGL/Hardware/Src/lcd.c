#include "lcd.h"
#include "lcd_tli.h"
#include "lcd_spi.h"
#include "gd32f4xx.h"

__attribute__((section(".sdram.lcd_fb")))
volatile uint16_t lcd_fb[LCD_WIDTH * LCD_HEIGHT];

// 初始化LCD
void lcd_init(void)
{
#ifdef LCD_SPI
    lcd_spi_init();
#else
    lcd_tli_init();
#endif
}

// 打点函数 (x,y)：起点坐标 color：点的颜色
void lcd_draw_point(uint16_t x,uint16_t y,uint16_t color)
{
#ifdef LCD_SPI
    lcd_spi_draw_point(x, y, color);
#else
    lcd_tli_draw_point(x, y, color);
#endif
}

/*!
    \brief      configure DISP ON/OFF GPIO
    \param[in]  none
    \param[out] none
    \retval     none
*/
void lcd_disp_en_config(void)
{
    /* enable the periphral clock */
    rcu_periph_clock_enable(RCU_GPIOD);
    gpio_mode_set(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_13);
    gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13);
}

/*!
    \brief      DISP GPIO OFF
    \param[in]  none
    \param[out] none
    \retval     none
*/
void lcd_disp_off(void)
{
    gpio_bit_reset(GPIOD, GPIO_PIN_13);
}

/*!
    \brief      DISP GPIO ON
    \param[in]  none
    \param[out] none
    \retval     none
*/
void lcd_disp_on(void)
{
    gpio_bit_set(GPIOD, GPIO_PIN_13);
}
