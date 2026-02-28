#include "lcd_spi.h"
#include "lcd.h"
#include "gd32f4xx.h"

static void lcd_spi_gpio_config(void);
static void lcd_spi_config(void);
static void lcd_disp_en_config(void);
static void lcd_disp_off(void);
static void lcd_disp_on(void);

void lcd_spi_init(void)
{
    lcd_spi_gpio_config();
    lcd_spi_config();
    lcd_disp_en_config();
    lcd_disp_on();
}

void lcd_spi_draw_point(uint16_t x,uint16_t y,uint16_t color)
{

}

static void lcd_spi_gpio_config(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_af_set(GPIOA, GPIO_AF_14, GPIO_PIN_3);
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_8 | GPIO_PIN_11 | GPIO_PIN_12);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_8 | GPIO_PIN_11 | GPIO_PIN_12);
}

static void lcd_spi_config(void)
{
    rcu_periph_clock_enable(RCU_SPI3);

}
