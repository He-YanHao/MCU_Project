#ifndef __OLED_H
#define __OLED_H 

#include "stm32f4xx.h"
#include "Delay.h"

//����LED�˿ڶ���
#define LED_ON   GPIO_ResetBits(GPIOC,GPIO_Pin_12)
#define LED_OFF  GPIO_SetBits(GPIOC,GPIO_Pin_12)

//OLED�˿ڶ���
// SCK = PA5
// SDA = PA7
// RES = PA3
// DC  = PA2
// CS  = PA4

#define OLED_SCL_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_5)//SCL
#define OLED_SCL_Set() GPIO_SetBits(GPIOA,GPIO_Pin_5)

#define OLED_SDA_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_7)//SDA
#define OLED_SDA_Set() GPIO_SetBits(GPIOA,GPIO_Pin_7)

#define OLED_RES_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_3)//RES
#define OLED_RES_Set() GPIO_SetBits(GPIOA,GPIO_Pin_3)

#define OLED_DC_Clr()  GPIO_ResetBits(GPIOA,GPIO_Pin_2)//DC
#define OLED_DC_Set()  GPIO_SetBits(GPIOA,GPIO_Pin_2)
 		     
#define OLED_CS_Clr()  GPIO_ResetBits(GPIOA,GPIO_Pin_4)//CS
#define OLED_CS_Set()  GPIO_SetBits(GPIOA,GPIO_Pin_4)


#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����

/*����ָ��*/
void OLED_Init(void);//��ʼ��OLED
void OLED_Clear(void);//��������
void OLED_ClearFresh(void);//���Դ溯��
void OLED_Refresh(void);//�����Դ浽OLED	
void OLED_ColorTurn(uint8_t i);//���Ժ���
void OLED_DisplayTurn(uint8_t i);//��Ļ��ת180��
void OLED_DisPlay_On(void);//����OLED��ʾ 
void OLED_DisPlay_Off(void);//�ر�OLED��ʾ
/*��ͼָ��*/
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);//���� x:0~127 y:0~63 t:1 ��� 0,���
void OLED_DrawLine(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t mode);////���� x1,y1:������� x2,y2:��������
void OLED_DrawCircle(uint8_t x,uint8_t y,uint8_t r);//x,y:Բ������ r:Բ�İ뾶
/*����ָ��*/
//�����Сsize���� 0806���� 1206���� 1608���� 2412���� ͨ��8 12 16 24����
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size1,uint8_t mode);
//��ָ��λ����ʾһ���ַ�,���������ַ� 
//x:0~127 y:0~63 size1:ѡ������ 6x8/6x12/8x16/12x24 mode:0,��ɫ��ʾ;1,������ʾ
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t size1,uint8_t mode);
//��ʾ�ַ��� x,y:������� size1:�����С  *chr:�ַ�����ʼ��ַ mode:0,��ɫ��ʾ;1,������ʾ
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size1,uint8_t mode);
//��ʾ���� x,y:������� num:Ҫ��ʾ������ len:���ֵ�λ�� size:�����С mode:0,��ɫ��ʾ;1,������ʾ
void OLED_ShowChinese(uint8_t x,uint8_t y,uint8_t num,uint8_t size1,uint8_t mode);
//��ʾ���� x,y:������� num:���ֶ�Ӧ����� mode:0,��ɫ��ʾ;1,������ʾ
void OLED_ScrollDisplay(uint8_t num,uint8_t space,uint8_t mode);
//num ��ʾ���ֵĸ��� space ÿһ����ʾ�ļ�� mode:0,��ɫ��ʾ;1,������ʾ
void OLED_ShowPicture(uint8_t x,uint8_t y,uint8_t sizex,uint8_t sizey,uint8_t BMP[],uint8_t mode);
//x,y��������� sizex,sizey,ͼƬ���� BMP[]��Ҫд���ͼƬ���� mode:0,��ɫ��ʾ;1,������ʾ
/*����ָ��*/
void OLED_ClearPoint(uint8_t x,uint8_t y);
void OLED_ShowChar6x8(uint8_t x,uint8_t y,uint8_t chr,uint8_t mode);
void OLED_WR_Byte(uint8_t dat,uint8_t mode);//SPI�������ݻ�ָ��

#endif
