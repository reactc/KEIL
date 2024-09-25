#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Timer.h"
#include "Movement.h"


//PA1---ECHO   PA0---TRIG
uint16_t Time; //记录时间

extern uint16_t Distance;

int repeatCounter = 0;//重复次数,解决bug用

void UltrasonicWave_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);
}

//计算TIME的次数，一次100us
void UltrasonicWave_Start(void){
	GPIO_SetBits(GPIOA,GPIO_Pin_0);
	Delay_us(50);
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);
	
	Timer_Init();
}

uint16_t UltrasonicWave_Getvalue(void){
	UltrasonicWave_Start();
	Delay_ms(100);
	return ((Time*0.0001)*34000)/2;
}

void avoidBarrier(){
	Distance = UltrasonicWave_Getvalue();
	Delay_ms(10);
	if(Distance>15){
		move_forward();
		repeatCounter++;
	}else {
		Distance = UltrasonicWave_Getvalue();
		while(Distance<15){
			repeatCounter = 0;
			move_right();
			move_right();
			Delay_ms(10);
			Distance = UltrasonicWave_Getvalue();
			if(Distance>15){
				break;
			}
		}
	}
}


