#ifndef __USART3_H
#define __USART3_H
#include "stdio.h"	
#include "stm32f10x.h"   

#define USART3_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART3_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
void USART3_Init(void);

uint8_t USART3_GetRxFlag(void);
uint8_t USART3_GetRxData(void);

#endif


