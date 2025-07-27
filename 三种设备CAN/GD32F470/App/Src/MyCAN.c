#include "MyCAN.h"

void MyCAN_Init(void)
{
    can_deinit(CAN1);
    can_parameter_struct can_parameter;
    can_parameter.working_mode = 0;//工作模式 
    can_parameter.resync_jump_width = 0;//再同步补偿宽度 
    can_parameter.time_segment_1 = 0;//位段1 
    can_parameter.time_segment_2 = 0;//位段2 
    can_parameter.time_triggered = 0;//时间触发通信模式 
    can_parameter.auto_bus_off_recovery = 0;//自动离线恢复
    can_parameter.auto_wake_up = 0;//自动唤醒
    can_parameter.auto_retrans = 0;//自动重传 
    can_parameter.rec_fifo_overwrite = 0;//接收FIFO满时覆盖 
    can_parameter.trans_fifo_order = 0;//发送FIFO顺序 
    can_parameter.prescaler = 0;//波特率分频系数
    
    can_init(CAN1, &can_parameter);
    
    //can_struct_para_init(CAN_INIT_STRUCT, );
    
}
