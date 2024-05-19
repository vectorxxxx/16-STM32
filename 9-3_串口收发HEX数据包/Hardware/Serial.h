#ifndef __USART_H
#define __USART_H

#include <stdio.h>

extern uint8_t Serial_TxPacket[];   // 声明数组可以不写长度
extern uint8_t Serial_RxPacket[];

void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendString(char *String);
void Serial_SendNumber(uint32_t Number, uint16_t Length);
void Serial_Printf(char *format, ...);
uint8_t Serial_GetRxFlag(void);
void Serial_SendPacket(void);

#endif
