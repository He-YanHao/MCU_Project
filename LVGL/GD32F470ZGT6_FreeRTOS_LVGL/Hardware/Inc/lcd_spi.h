#ifndef __LCD_SPI_H__
#define __LCD_SPI_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void lcd_spi_init(void);
void lcd_spi_draw_point(uint16_t x,uint16_t y,uint16_t color);

#if defined(__cplusplus)
}
#endif

#endif /* GD32F450Z_LCD_H */
