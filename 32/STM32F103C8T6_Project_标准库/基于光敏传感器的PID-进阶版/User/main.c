#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "AD.h"

#define ON GPIO_WriteBit(GPIOC, GPIO_Pin_13, (BitAction)(0))
#define OFF GPIO_WriteBit(GPIOC, GPIO_Pin_13, (BitAction)(1))



#define TARGET 3000 //目标值
#define T 10

void PID(void)
{
    double Error0 = 0, Error1 = 0,ErrorLeiJia = 0;
    double Actual = 0, PIDout = 0;//定义实际值
    double Kp = 0.5;//定义比例系数
    double Ki = 0.5;//定义积分系数
    double Kd = 0.5;//定义微分系数

    //目标值获取
    for(uint8_t i = 0; i < 100; i++)
    {
        Actual = Actual + (double)AD_GetValue();//获取实际值
    }
    Actual = Actual/100;
    
    //PID计算
    Error1 = 3000 - Actual;//获取本次误差
    Error0 = Error1;//本次误差赋值与上次误差
    ErrorLeiJia += Error1;
    PIDout = Actual + Kp * Error0 + Ki * ErrorLeiJia + Kd * (Error1 - Error0);//获得PID修正
    
    //输出数值
    OLED_ShowSignedNum(1, 1, (int32_t)(Kp * Error1), 4);//
    OLED_ShowSignedNum(1, 6, (int32_t)(Ki * ErrorLeiJia), 4);//
    OLED_ShowSignedNum(1, 11, (int32_t)(Kd * (Error1 - Error0)), 4);//
    OLED_ShowNum(2, 8, Actual, 4);//实际值
    OLED_ShowNum(3, 8, PIDout, 4);//修正过的值
    OLED_ShowSignedNum(4, 8, Error1, 4);//误差
    
    Delay_ms(500);
}

int main(void)
{
	/*模块初始化*/
	OLED_Init();			//OLED初始化
	AD_Init();				//AD初始化
    CS_Init();

	/*显示静态字符串*/
//	OLED_ShowString(1, 1, "Target:");//目标值
//  OLED_ShowNum(1, 8, TARGET, 4);
	OLED_ShowString(2, 1, "Actual:");//实际值
	OLED_ShowString(3, 1, "PIDout:");//修正过的值
	OLED_ShowString(4, 1, " Error:");//误差
	while (1)
	{
        PID();
        //Delay_ms(500);
	}
}
