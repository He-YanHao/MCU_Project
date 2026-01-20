#ifndef __AT24C256_H
#define __AT24C256_H

void MyI2C_Init(void);
void AT24C256_WriteByte(uint16_t WordAddress,uint8_t Data);
uint8_t AT24C256_ReadByte(uint16_t WordAddress);

#endif
