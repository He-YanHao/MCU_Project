#include "board_Key.h"
//开发板上一共有三个按键，一个复位、一个 BOOT0 和一个 KEY
void Key_Init(void)
{
    /* 开启时钟 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

KEY_NUM Key_scan(void)
{
    /* 获取PA0引脚的高低电平状态 */
    if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
    {
        while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0));
        return KEY1;
    } else
    {
        return KEY0;
    }
}
