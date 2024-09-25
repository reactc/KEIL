#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
#include "Delay.h"
#include <stdlib.h>
//***�����ϳ�***//
//PA2---RXD  PA3---TXD

uint8_t USART2_RxData;		//���崮�ڽ��յ����ݱ���
uint8_t USART2_RxFlag;		//���崮�ڽ��յı�־λ����	

/**
  * ��    �������ڳ�ʼ��
  * ��    ������
  * �� �� ֵ����
  */
void USART2_Init(void)
{
	/*����ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//����GPIOA��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//����USART2��ʱ�ӣ������ϳɣ�
	/*GPIO��ʼ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//��PA2���ų�ʼ��Ϊ�����������,PA2��TX
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//��PA3���ų�ʼ��Ϊ����,PA3��RX
	
	/*USART��ʼ��*/
	USART_InitTypeDef USART_InitStructure;					//����ṹ�����
	USART_InitStructure.USART_BaudRate = 9600;				//������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ�������ƣ�����Ҫ
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	//ģʽ������ģʽ�ͽ���ģʽ��ѡ��
	USART_InitStructure.USART_Parity = USART_Parity_No;		//��żУ�飬����Ҫ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	//ֹͣλ��ѡ��1λ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//�ֳ���ѡ��8λ
	USART_Init(USART2, &USART_InitStructure);				//���ṹ���������USART_Init������USART2
	
	/*�ж��������*/
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);			//�������ڽ������ݵ��ж�
	/*NVIC�жϷ���*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			//����NVICΪ����2
	/*NVIC����*/
	NVIC_InitTypeDef NVIC_InitStructure;					//����ṹ�����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;		//ѡ������NVIC��USART1��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//ָ��NVIC��·ʹ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;		//ָ��NVIC��·����ռ���ȼ�Ϊ
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//ָ��NVIC��·����Ӧ���ȼ�Ϊ
	NVIC_Init(&NVIC_InitStructure);							//���ṹ���������NVIC_Init������NVIC����
	
	/*USARTʹ��*/
	USART_Cmd(USART2, ENABLE);								//ʹ��USART2�����ڿ�ʼ����
}


/**
  * ��    �������ڷ���һ���ַ���
  * ��    ����String Ҫ�����ַ������׵�ַ,����
  * �� �� ֵ����
  */
void USART2_SendString(u8 *DAT, u8 len)
{
	u8 i;
	for(i = 0; i < len; i++)
	{
		while((USART2->SR & 0X40) == 0);
		USART2->DR = (*DAT++);
	}
}

/**
  * ��    ������ȡ���ڽ��ձ�־λ
  * ��    ������
  * �� �� ֵ�����ڽ��ձ�־λ����Χ��0~1�����յ����ݺ󣬱�־λ��1����ȡ���־λ�Զ�����
  */
uint8_t USART2_GetRxFlag(void)
{
	if (USART2_RxFlag == 1)			//�����־λΪ1
	{
		USART2_RxFlag = 0;
		return 1;					//�򷵻�1�����Զ������־λ
	}
	return 0;						//�����־λΪ0���򷵻�0
}

/**
  * ��    ������ȡ���ڽ��յ�����
  * ��    ������
  * �� �� ֵ�����յ����ݣ���Χ��0~255
  */
uint8_t USART2_GetRxData(void)
{
	return USART2_RxData;			//���ؽ��յ����ݱ���
}

/**
  * ��    ����USART2�жϺ���
  * ��    ������
  * �� �� ֵ����
  * ע������˺���Ϊ�жϺ�����������ã��жϴ������Զ�ִ��
  *           ������ΪԤ����ָ�����ƣ����Դ������ļ�����
  *           ��ȷ����������ȷ���������κβ��죬�����жϺ��������ܽ���
  */

void USART2_IRQHandler(void)
{
	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)		//�ж��Ƿ���USART1�Ľ����¼��������ж�
	{
		USART2_RxData = USART_ReceiveData(USART2);				//��ȡ���ݼĴ���������ڽ��յ����ݱ���
		USART2_RxFlag = 1;										//�ý��ձ�־λ����Ϊ1
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);			//���USART1��RXNE��־λ
																//��ȡ���ݼĴ������Զ�����˱�־λ
																//����Ѿ���ȡ�����ݼĴ�����Ҳ���Բ�ִ�д˴���
	}
}


