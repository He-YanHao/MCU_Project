#include "bsp_w25q64.h"

void w25q64_init_config(void)
{	
	//SPI参数定义结构体
	spi_parameter_struct spi_init_struct;
	
	rcu_periph_clock_enable(RCU_GPIOF);  // 使用F端口
    rcu_periph_clock_enable(RCU_SPI4);     // 使能SPI4
    //引脚复用
    gpio_af_set(GPIOF, GPIO_AF_5, GPIO_PIN_7);
    gpio_af_set(GPIOF, GPIO_AF_5, GPIO_PIN_8);
    gpio_af_set(GPIOF, GPIO_AF_5, GPIO_PIN_9);
    //引脚模式
    gpio_mode_set(GPIOF, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_7);
    gpio_mode_set(GPIOF, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_8);
    gpio_mode_set(GPIOF, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_9);
    //输出模式
    gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7);
    gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_8);
    gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
    
	//开启CS引脚时钟
	rcu_periph_clock_enable(RCU_GPIOF);
	//配置CS引脚模式
	gpio_mode_set(GPIOF, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_6);
	//配置CS输出模式
	gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
	//W25Q64不选中
	gpio_bit_write(GPIOF, GPIO_PIN_6, SET);

	spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;  	// 传输模式全双工
	spi_init_struct.device_mode          = SPI_MASTER;                	// 配置为主机
	spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;        	// 8位数据
	spi_init_struct.clock_polarity_phase = SPI_CK_PL_HIGH_PH_2EDGE;		//极性相位  
	spi_init_struct.nss                  = SPI_NSS_SOFT;              	//软件cs
	spi_init_struct.prescale             = SPI_PSC_2;                 	//SPI时钟预分频为2
	spi_init_struct.endian               = SPI_ENDIAN_MSB;            	//高位在前
	//将参数填入SPI4
	spi_init(SPI4, &spi_init_struct);
	//使能SPI
	spi_enable(SPI4);

}
/******************************************************************
 * 函 数 名 称：spi_read_write_byte
 * 函 数 说 明：硬件SPI的读写
 * 函 数 形 参：dat=发送的数据
 * 函 数 返 回：读取到的数据
 * 作       者：LC
 * 备       注：无
******************************************************************/
uint8_t spi_read_write_byte(uint8_t dat)
{
	//等待发送缓冲区为空
	while(RESET == spi_i2s_flag_get(SPI4,  SPI_FLAG_TBE) );
         spi_i2s_data_transmit(SPI4, dat);
	//等待接收缓冲区为空
	while(RESET == spi_i2s_flag_get(SPI4,  SPI_FLAG_RBNE) );
         return spi_i2s_data_receive(SPI4);
}

/******************************************************************
 * 函 数 名 称：W25Q64_readID
 * 函 数 说 明：读取W25Q64的厂商ID和设备ID
 * 函 数 形 参：无
 * 函 数 返 回：设备正常返回EF16
 * 作       者：LC
 * 备       注：无
******************************************************************/
uint16_t W25Q64_readID(void)
{
	uint16_t  temp = 0;	  	
	gpio_bit_write(GPIOF, GPIO_PIN_6, RESET);
		
	spi_read_write_byte(0x90);//发送读取ID命令	    
	spi_read_write_byte(0x00); 	    
	spi_read_write_byte(0x00); 	    
	spi_read_write_byte(0x00); 		
	//接收数据
	temp |= spi_read_write_byte(0xFF)<<8;  
	temp |= spi_read_write_byte(0xFF);	
		 
	gpio_bit_write(GPIOF, GPIO_PIN_6, SET);	
	return temp;
}



/**********************************************************
 * 函 数 名 称：W25Q64_wait_busy
 * 函 数 功 能：判断W25Q64是否忙
 * 传 入 参 数：无
 * 函 数 返 回：无
 * 作       者：LC
 * 备       注：无
**********************************************************/
void W25Q64_wait_busy(void)   
{   
    unsigned char byte = 0;
    do
     { 
        gpio_bit_write(GPIOF, GPIO_PIN_6, RESET);                              
        spi_read_write_byte(0x05);                 
        byte = spi_read_write_byte(0Xff);       
        gpio_bit_write(GPIOF, GPIO_PIN_6, SET);         
     }while( ( byte & 0x01 ) == 1 );  
}  

/**********************************************************
 * 函 数 名 称：W25Q64_write_enable
 * 函 数 功 能：发送写使能
 * 传 入 参 数：无
 * 函 数 返 回：无
 * 作       者：LC
 * 备       注：无
**********************************************************/
void W25Q64_write_enable(void)   
{
    gpio_bit_write(GPIOF, GPIO_PIN_6, RESET);                           
    spi_read_write_byte(0x06);                  
    gpio_bit_write(GPIOF, GPIO_PIN_6, SET); 
}                            	      

/**********************************************************
 * 函 数 名 称：W25Q64_erase_sector
 * 函 数 功 能：擦除一个扇区
 * 传 入 参 数：addr=擦除的扇区号
 * 函 数 返 回：无
 * 作       者：LC
 * 备       注：addr=擦除的扇区号，范围=0~15
**********************************************************/
void W25Q64_erase_sector(uint32_t addr)   
{
	addr *= 4096;
	W25Q64_write_enable();  //写使能   
	W25Q64_wait_busy();     //判断忙 
	gpio_bit_write(GPIOF, GPIO_PIN_6, RESET);                                        
	spi_read_write_byte(0x20);        	
	spi_read_write_byte((uint8_t)((addr)>>16));      
	spi_read_write_byte((uint8_t)((addr)>>8));   
	spi_read_write_byte((uint8_t)addr);  
	gpio_bit_write(GPIOF, GPIO_PIN_6, SET);                  
	//等待擦除完成                           	      	 
	W25Q64_wait_busy();   
}         				                      	      


/**********************************************************
 * 函 数 名 称：W25Q64_write
 * 函 数 功 能：写数据到W25Q64进行保存
 * 传 入 参 数：buffer=写入的数据内容	addr=写入地址	numbyte=写入数据的长度
 * 函 数 返 回：无
 * 作       者：LC
 * 备       注：无
**********************************************************/
void W25Q64_write(uint8_t* buffer, uint32_t addr, uint16_t numbyte)
{    //0x02e21
    unsigned int i = 0;
    W25Q64_erase_sector(addr/4096);//擦除扇区数据
    W25Q64_write_enable();//写使能    
    W25Q64_wait_busy(); //忙检测    
    //写入数据
    gpio_bit_write(GPIOF, GPIO_PIN_6, RESET);                                        
    spi_read_write_byte(0x02);                    
    spi_read_write_byte((uint8_t)((addr)>>16));     
    spi_read_write_byte((uint8_t)((addr)>>8));   
    spi_read_write_byte((uint8_t)addr);   
    for(i=0;i<numbyte;i++)
    {
        spi_read_write_byte(buffer[i]);  
    }
    gpio_bit_write(GPIOF, GPIO_PIN_6, SET);
    W25Q64_wait_busy(); //忙检测      
}

/**********************************************************
 * 函 数 名 称：W25Q64_read
 * 函 数 功 能：读取W25Q64的数据
 * 传 入 参 数：buffer=读出数据的保存地址  read_addr=读取地址		read_length=读去长度
 * 函 数 返 回：无
 * 作       者：LC
 * 备       注：无
**********************************************************/
void W25Q64_read(uint8_t* buffer,uint32_t read_addr,uint16_t read_length)   
{ 
	uint16_t i;   		
	gpio_bit_write(GPIOF, GPIO_PIN_6, RESET);            
	spi_read_write_byte(0x03);                           
	spi_read_write_byte((uint8_t)((read_addr)>>16));           
	spi_read_write_byte((uint8_t)((read_addr)>>8));   
	spi_read_write_byte((uint8_t)read_addr);   
	for(i=0;i<read_length;i++)
	{ 
		buffer[i]= spi_read_write_byte(0XFF);  
	}
	gpio_bit_write(GPIOF, GPIO_PIN_6, SET);  		    	      
} 

