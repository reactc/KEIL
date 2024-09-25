#include "stm32f10x.h"                  // Device header
#include "Movement.h"
#include "Delay.h"

//HW1--PA11前  HW2--PA4后
//HW3--PA12左  HW4--PA5右

extern uint8_t t1 ; //存储红外的信号 前
extern uint8_t t2 ; //存储红外的信号 后
extern uint8_t t3 ; //存储红外的信号 左
extern uint8_t t4 ; //存储红外的信号 右
void redOutside_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5| GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);
	GPIO_ResetBits(GPIOA,GPIO_Pin_11);
	GPIO_ResetBits(GPIOA,GPIO_Pin_12);
}

uint8_t Get_redOutside1(void) //前
{
	return GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11);
}
uint8_t Get_redOutside2(void) //前
{
	return GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4);
}
uint8_t Get_redOutside3(void) //前
{
	return GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12);
}
uint8_t Get_redOutside4(void) //前
{
	return GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5);
}

void Edge_detect(void){
	t1 = Get_redOutside1();
	t2 = Get_redOutside2();
	t3 = Get_redOutside3();
	t4 = Get_redOutside4();
	
	if(t1==1){
		move_behind();
		t1=0;
	}	
	if(t2==2){
		move_forward();
		t2=0;
	}	
	if(t3==1){
		move_right_hw();
		move_forward();
		move_right_hw();
		move_forward();
		t3=0;
	}	
	if(t4==1){
		move_left_hw();
		move_forward();
		move_left_hw();
		move_forward();
		t4=0;
	}
}

