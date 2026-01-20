#include "board.h"
#include "RC522.h"
#include <stdio.h>
#include "string.h"

/* 卡的ID存储，32位,4字节 */
u8 ucArray_ID [ 4 ];        
uint8_t ucStatusReturn;    //返回状态 

int main(void)
{
    int i = 0;
    
    uint8_t read_write_data[16]={0};//读写数据缓存
    uint8_t card_KEY[6] ={0xff,0xff,0xff,0xff,0xff,0xff};//默认密码

    board_init();

    RC522_Init( );//IC卡IO口初始化 
    RC522_Rese( );//复位RC522    
    
    while(1) 
    {
        /* 寻卡（方式：范围内全部），第一次寻卡失败后再进行一次，寻卡成功时卡片序列传入数组ucArray_ID中 */
        if ( ( ucStatusReturn = PcdRequest ( PICC_REQALL, ucArray_ID ) ) != MI_OK )
        {  
                        ucStatusReturn = PcdRequest ( PICC_REQALL, ucArray_ID );
        }

        if ( ucStatusReturn == MI_OK  )
        {
                /* 防冲突操作，被选中的卡片序列传入数组ucArray_ID中 */
                if ( PcdAnticoll ( ucArray_ID ) == MI_OK )
                {
                        //输出卡ID                
                        printf("ID: %X %X %X %X\r\n", ucArray_ID [ 0 ], ucArray_ID [ 1 ], ucArray_ID [ 2 ], ucArray_ID [ 3 ]);

                         //选卡
                        if( PcdSelect(ucArray_ID) != MI_OK )
                        {
                            printf("PcdSelect failure\r\n");
                        }
                        //校验卡片密码
                        //数据块6的密码A进行校验（所有密码默认为16个字节的0xff）
                        if( PcdAuthState(PICC_AUTHENT1B, 6, card_KEY,  ucArray_ID) != MI_OK )
                        {
                            printf("PcdAuthState failure\r\n");
                        }
                        //往数据块4写入数据read_write_data
                        read_write_data[0] = 0xaa;//将read_write_data的第一位数据改为0xaa
                        if( PcdWrite(4,read_write_data) != MI_OK )
                        {
                            printf("PcdWrite failure\r\n");
                        }
                        //将read_write_data的16位数据，填充为0（清除数据的意思）
                        memset(read_write_data,0,16);
                        delay_us(8);
                        //读取数据块4的数据
                        if( PcdRead(4,read_write_data) != MI_OK )
                        {
                            printf("PcdRead failure\r\n");
                        }
                        //输出读出的数据
                        for( i = 0; i < 16; i++ )
                        {
                              Gui_DrawFont_GBK16(0,0,BLACK,WHITE,"read_write_data[i]");
                        }
                        printf("\r\n");
                }
        }        
    }
}
