#ifndef __MYPID_H
#define __MYPID_H

#include "stm32f4xx.h"
#include "test.h"

typedef struct{         //����PID�������ݵĽṹ��
    double Kp;          //����ϵ��
    double Ki;          //����ϵ��
    double Kd;          //΢��ϵ��
    uint32_t T;         //��������
    double Target;      //Ŀ��ֵ
    double add_MAX;     //��������޷�MAX
    double add_MIN;     //��������޷�MIN
    double integral;    //�����ۻ�ֵ
    double prev_error;  //��һ�ε���� ���ڼ���΢��
}PID_NUM;

void Kp_Change(PID_NUM* PID, double Kp);
void Ki_Change(PID_NUM* PID, double Ki);
void Kd_Change(PID_NUM* PID, double Kd);
void T_Change(PID_NUM* PID, uint32_t T);
void Target_Change(PID_NUM* PID, double Target);
void add_MAX_Change(PID_NUM* PID, double add_MAX);
void add_MIN_Change(PID_NUM* PID, double add_MIN);
void integral_Change(PID_NUM* PID, double integral);
void prev_error_Change(PID_NUM* PID, double prev_error);
void PID_Init(PID_NUM* PID);
double Speed(PID_NUM* PID);
double PID_OUT(PID_NUM* PID);



#endif
