#ifndef __SYN6288_H_
#define __SYN6288_H_
#include "stm32f10x.h"                  // Device header
//Music:ѡ�񱳾����֡�0:�ޱ������֣�1~15��ѡ�񱳾�����
void SYN_FrameInfo(u8 Music, u8 *HZdata);
//���ã�ֹͣ�ϳɡ���ͣ�ϳɵ����� ��Ĭ�ϲ�����9600bps��
void YS_SYN_Set(u8 *Info_data);//�̶�����Ϣ����
#endif
