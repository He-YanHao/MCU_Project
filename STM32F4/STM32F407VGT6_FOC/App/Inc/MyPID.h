#ifndef __MYPID_H
#define __MYPID_H

#include "stm32f4xx.h"
#include "test.h"

typedef struct{         //定义PID所需数据的结构体
    double Kp;          //比例系数
    double Ki;          //积分系数
    double Kd;          //微分系数
    uint32_t T;         //采样周期
    double Target;      //目标值
    double add_MAX;     //增益输出限幅MAX
    double add_MIN;     //增益输出限幅MIN
    double integral;    //积分累积值
    double prev_error;  //上一次的误差 用于计算微分
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
