#include "stm32f10x.h"
#include "stdlib.h"
#include <stdio.h>
#include "LED.h"
#include "Delay.h"
#include "OLED.h"
#include "Servo.h"
#include "PWM.h"
#include "Movement.h"
#include "usart1.h"
#include "usart2.h"
#include "usart3.h"
#include "syn6288.h"
#include "key.h"
#include "hongwai.h"
#include "UltrasonicWave.h"
#include "Mode.h"
uint8_t Key_Num;
uint8_t RxData;			//���ڽ��մ������ݵı���
uint8_t move_mode1 = '0';//���ڽ��������������ݵı���
uint8_t move_mode3 = '0';//���ڽ�������ʶ�𴮿����ݵı���
uint8_t move_mode = '0';//����״̬����
uint8_t previous_mode = '0';//���ڱ�����һ�ν��մ������ݵı���
uint8_t t1=0 ; //�洢������ź� ǰ
uint8_t t2=0 ; //�洢������ź� ��
uint8_t t3=0 ; //�洢������ź� ��
uint8_t t4=0 ; //�洢������ź� ��

uint16_t ForwardCount=0; //����ǰ������
uint16_t BackwardCount=0; //�������˼���
uint16_t LeftCount=0; //������ת����
uint16_t RightCount=0; //������ת����

uint16_t bz_flag;//���ϱ�־λ ON/OFF Ĭ��off
uint16_t hw_flag;//�����־λ ON/OFF Ĭ��off

int happiness; //����ָ��
int stamina;  //����ֵ

uint16_t Distance=100; // ��¼���� ����ֵ����10cm(����������) 
int main(void)
{	
	/*ģ���ʼ��*/
	LED_Init();			//LED��ʼ��
	Servo_Init();		//�����ʼ��
	OLED_Init();		//OLED��ʼ��
	USART1_Init();		//�������ڳ�ʼ��
	USART2_Init();
	USART3_Init();		//����ʶ�𴮿ڳ�ʼ��
	redOutside_Init();
	UltrasonicWave_Init();
	happiness=50;//��ʼ������ָ��
	stamina=100;//��ʼ������ֵ
	bz_flag=0;//���ϱ�־λ ON/OFF Ĭ��off
	hw_flag=0;//�����־λ ON/OFF Ĭ��off
	OLED_Clear();
	OLED_ShowImage(0, 0, 128, 64, BMP1); //����
	
	/**************�����ϳ�оƬ��������*********************/
	//ѡ�񱳾�����2��(0���ޱ�������  1-15���������ֿ�ѡ)
	//m[0~16]:0��������Ϊ������16���������������
	//v[0~16]:0�ʶ�����Ϊ������16�ʶ��������
	//t[0~5]:0�ʶ�����������5�ʶ��������
	
	SYN_FrameInfo(2, (uint8_t *)"[v12][m5][t5]����");Delay_s(1);//���Ѻ󹷽�һ��,����12�ʺ���ʾ��10�ʺϵ���
	
	while (1)
	{
		//***����ֵ�뿪��ֵ��Ӧ����***//
		if(stamina>0)//����ֵ������Ż�����
	{
		move_mode1 = USART1_GetRxData();		//��ȡ�������ڽ��յ�����
		move_mode3 = USART3_GetRxData();		//��ȡ����ʶ�𴮿ڽ��յ�����
		if (USART1_GetRxFlag()) {//���������������ȼ���������ʶ��
			move_mode = move_mode1;
		}
		else if (USART3_GetRxFlag()){
			move_mode = move_mode3;
		}
	}
		if(stamina<0)//����ֵС��0��ִ�С����ܡ�״̬
	{
		mode_nanshou();
		stamina=0;
	}
		if(stamina==0)//����ֵΪ0��ֻ����˯��ָ��
	{
		if(USART1_GetRxData()=='2'|USART3_GetRxData()=='2')
		{mode_sleepwo();}
		else if(USART1_GetRxData()=='p'|USART3_GetRxData()=='p')
		{mode_sleeppa();}
	}
		if(stamina>100)//����ֵ������100
		{stamina=100;}
		
		if(happiness<0){//����ֵС��0��ִ�С����ܡ�״̬���������ӿ���ֵ��ָ����������״̬
		happiness=0;
		mode_nanshou();
		}
		if(happiness>100)//����ֵ������100
		{happiness=100;}
		
		//***�������¼��ģ������***//
		if(move_mode == 'H') //���⿪
		{hw_flag=1;}
		if(move_mode == 'h') //�����
		{hw_flag=0;
			//���¸��ĸ��������������
			t1=0;t2=0;t3=0;t4=0;
		}
		if(hw_flag){
			Edge_detect();//�򿪺������¼��ģʽ	
		}
		
	   //***��������ģ������***//
		if(move_mode == 'x') //������
		{
			bz_flag=1;
		}
		if(move_mode == 'c') //������
		{
			bz_flag=0;
			Distance=100;
		}
		if(bz_flag){
			avoidBarrier();//�򿪳�������ģʽ
		}
		
		
		//***��������***//
		if (move_mode == 'f') { //ǰ��
			mode_Forward();
		} else if (move_mode == 'b') { //����
			mode_Behind();
		} else if (move_mode == 'l') { //��ת
			mode_Left();
		} else if (move_mode == 'r') { //��ת
			mode_Right();
		} else if (move_mode == 'w') { //ǰ��ҡ��
			mode_swing_FrontBack();
		} else if (move_mode == 'z') { //����ҡ��
			mode_swing_LeftRight();
		} else if (move_mode == 'd') { //����
			mode_swing_Dance();
		} else if (move_mode == '5') { //����
			mode_Stand();
		} else if (move_mode == 'q' && previous_mode != '0') { //����
			mode_SlowStand();
		} else if (move_mode == 's' && previous_mode != 's') { //����
			mode_Strech();
		} else if (move_mode == 'j') { //����̧��
			mode_TwoHands();
		} else if (move_mode == 'y') { //������
			mode_lanyao();
		} else if (move_mode == '1') { //̧ͷ
			mode_headup();
		} else if (move_mode == 'p' && previous_mode != 'p') { //ſ��˯��
			mode_sleeppa();
		} else if (move_mode == '2' && previous_mode != '2') { //����˯��
			mode_sleepwo();
		} else if (move_mode == 'n') { //����
			mode_nanshou();
		} else if (move_mode == 'B') { //�人
			mode_wuhan();
		} else if (move_mode == 'Y') { //�Ŷ�û��
			mode_mendoumeiyou();
		} else if (move_mode == 'X') { //զ����
			mode_zahuishi();
		} else if (move_mode == 'W') { //Ϊʲô
			mode_world();
		} else if (move_mode == 'o') { //���к�
			mode_hello();
		} else if (move_mode == 'U') { //���С�С����
			mode_xiaodai();
		} else if (move_mode == 'K') { //չʾ����ֵ
			mode_happiness();
		} else if (move_mode == 'T') { //չʾ����ֵ
			mode_stamina();
		} else if (move_mode == 'Z') { //չʾ����ֵ������ֵ
			mode_index();
		} 
	}
	
}

