#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
#include "Delay.h"
#include <stdlib.h>
//***����ʶ��***//

uint8_t USART3_RxData;		//���崮�ڽ��յ����ݱ���
uint8_t USART3_RxFlag;		//���崮�ڽ��յı�־λ����	

/**
  * ��    �������ڳ�ʼ��
  * ��    ������
  * �� �� ֵ����
  */
  
  //PB10--LD_RXD  PB11--LD_TXD
void USART3_Init(void)
{
	/*����ʱ��*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//����USART3��ʱ�ӣ�����ʶ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//����GPIOB��ʱ��
	/*GPIO��ʼ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//��PB10���ų�ʼ��Ϊ�����������,PB10��TX
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//��PB11���ų�ʼ��Ϊ����,PB11��RX
	
	/*USART��ʼ��*/
	USART_InitTypeDef USART_InitStructure;					//����ṹ�����
	USART_InitStructure.USART_BaudRate = 9600;				//������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ�������ƣ�����Ҫ
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	//ģʽ������ģʽ�ͽ���ģʽ��ѡ��
	USART_InitStructure.USART_Parity = USART_Parity_No;		//��żУ�飬����Ҫ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	//ֹͣλ��ѡ��1λ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//�ֳ���ѡ��8λ
	USART_Init(USART3, &USART_InitStructure);				//���ṹ���������USART_Init������USART3
	
	/*�ж��������*/
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);			//�������ڽ������ݵ��ж�
	/*NVIC�жϷ���*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			//����NVICΪ����2
	/*NVIC����*/
	NVIC_InitTypeDef NVIC_InitStructure;					//����ṹ�����
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;		//ѡ������NVIC��USART1��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//ָ��NVIC��·ʹ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;		//ָ��NVIC��·����ռ���ȼ�Ϊ
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//ָ��NVIC��·����Ӧ���ȼ�Ϊ
	NVIC_Init(&NVIC_InitStructure);							//���ṹ���������NVIC_Init������NVIC����
	
	/*USARTʹ��*/
	USART_Cmd(USART3, ENABLE);								//ʹ��USART3�����ڿ�ʼ����
}


/**
  * ��    ������ȡ���ڽ��ձ�־λ
  * ��    ������
  * �� �� ֵ�����ڽ��ձ�־λ����Χ��0~1�����յ����ݺ󣬱�־λ��1����ȡ���־λ�Զ�����
  */
uint8_t USART3_GetRxFlag(void)
{
	if (USART3_RxFlag == 1)			//�����־λΪ1
	{
		USART3_RxFlag = 0;
		return 1;					//�򷵻�1�����Զ������־λ
	}
	return 0;						//�����־λΪ0���򷵻�0
}

/**
  * ��    ������ȡ���ڽ��յ�����
  * ��    ������
  * �� �� ֵ�����յ����ݣ���Χ��0~255
  */
uint8_t USART3_GetRxData(void)
{
	return USART3_RxData;			//���ؽ��յ����ݱ���
}

/**
  * ��    ����USART2�жϺ���
  * ��    ������
  * �� �� ֵ����
  * ע������˺���Ϊ�жϺ�����������ã��жϴ������Զ�ִ��
  *           ������ΪԤ����ָ�����ƣ����Դ������ļ�����
  *           ��ȷ����������ȷ���������κβ��죬�����жϺ��������ܽ���
  */

void USART3_IRQHandler(void)
{
	if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)		//�ж��Ƿ���USART1�Ľ����¼��������ж�
	{
		USART3_RxData = USART_ReceiveData(USART3);				//��ȡ���ݼĴ���������ڽ��յ����ݱ���
		USART3_RxFlag = 1;										//�ý��ձ�־λ����Ϊ1
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);			//���USART1��RXNE��־λ
																//��ȡ���ݼĴ������Զ�����˱�־λ
																//����Ѿ���ȡ�����ݼĴ�����Ҳ���Բ�ִ�д˴���
	}
}


