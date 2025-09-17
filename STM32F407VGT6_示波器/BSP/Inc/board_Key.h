#ifndef __BOARD_KEY_H
#define __BOARD_KEY_H

#include "stm32f4xx.h"
#include "Delay.h"

typedef enum{
    KEY0,//����1
    KEY1 //����2
}KEY_NUM;

typedef enum {
    BUTTON_EVENT_NONE,      // ���¼�
    BUTTON_EVENT_PRESS,     // ����
    BUTTON_EVENT_RELEASE,   // �ͷ�
    BUTTON_EVENT_CLICK,     // ����
    BUTTON_EVENT_DOUBLE_CLICK, // ˫��
    BUTTON_EVENT_LONG_PRESS,   // ����
    BUTTON_EVENT_HOLD       // ���ְ���
} ButtonEvent;

#define  KEY_TIM    RCU_TIMER5  // ��ʱ��ʱ��

void Key_Init(void);
void KeyEXTI_Init(void);
KEY_NUM Key_scan(void);

//extern uint16_t Delay_Num[];
//extern uint8_t Delay_Leve;
//extern uint8_t KeyEXTI;

#endif
