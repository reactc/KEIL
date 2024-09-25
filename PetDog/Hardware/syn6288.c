#include "stm32f10x.h"                  // Device header
#include "usart2.h"
#include "string.h"
#include "delay.h"
#include "syn6288.h"
//PA2---RXD  PA3---TXD


//Music:ѡ�񱳾����֡�0:�ޱ������֣�1~15��ѡ�񱳾�����
void SYN_FrameInfo(u8 Music, u8 *HZdata){
	unsigned  char  Frame_Info[50];
	unsigned  char  HZ_Length;
	unsigned  char  ecc  = 0;  			//����У���ֽ�
	unsigned  int i = 0;
	HZ_Length = strlen((char*)HZdata); 			//��Ҫ�����ı��ĳ���
	
	
	Frame_Info[0] = 0xFD ; 			//����֡ͷFD
	Frame_Info[1] = 0x00 ; 			//�������������ȵĸ��ֽ�
	Frame_Info[2] = HZ_Length + 3; 		//�������������ȵĵ��ֽ�
	Frame_Info[3] = 0x01 ; 			//���������֣��ϳɲ�������
	Frame_Info[4] = 0x01 | Music << 4 ; //����������������������趨


	for(i = 0; i < 5; i++)   				//���η��͹���õ�5��֡ͷ�ֽ�
	{
		ecc = ecc ^ (Frame_Info[i]);		//�Է��͵��ֽڽ������У��
	}

	for(i = 0; i < HZ_Length; i++)   		//���η��ʹ��ϳɵ��ı�����
	{
		ecc = ecc ^ (HZdata[i]); 				//�Է��͵��ֽڽ������У��
	}

	memcpy(&Frame_Info[5], HZdata, HZ_Length);
	Frame_Info[5 + HZ_Length] = ecc;
	USART2_SendString(Frame_Info, 5 + HZ_Length + 1);
}
//���ã�ֹͣ�ϳɡ���ͣ�ϳɵ����� ��Ĭ�ϲ�����9600bps��
void YS_SYN_Set(u8 *Info_data)//�̶�����Ϣ����
{
	u8 Com_Len;
	Com_Len = strlen((char*)Info_data);
	USART2_SendString(Info_data, Com_Len);
}









