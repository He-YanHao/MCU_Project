#include "board.h"

void board_Init(void)
{
    Delay_Init();
    BSP_LED_Init();
    Key_Init();
    board_BSP_USART_Init();
    //KeyEXTI_Init();
}
