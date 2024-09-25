#include "stm32f10x.h"                  // Device header
#include "PWM.h"

//Servo���ڷ�װ����ĽǶ��������ȡ����

void Servo_Init(){
	PWM_Init();
}

/**
  * ��    ����������ýǶ�
  * ��    ����Angle Ҫ���õĶ���Ƕȣ���Χ��0~180
  * �� �� ֵ����
  */

void Servo_SetAngle1(float Angle){
	PWM_SetCompare1(Angle / 180 * 2000 + 500);
}

void Servo_SetAngle2(float Angle){
	PWM_SetCompare2(Angle / 180 * 2000 + 500);
}

void Servo_SetAngle3(float Angle){
	PWM_SetCompare3(Angle / 180 * 2000 + 500);
}

void Servo_SetAngle4(float Angle){
	PWM_SetCompare4(Angle / 180 * 2000 + 500);
}


/**
  * ��    ���������ȡ�Ƕ�
  * ��    ������
  * �� �� ֵ������Ƕȣ���Χ��0~180
  */

uint8_t Servo_GetAngle1(){
	return (TIM_GetCapture1(TIM3)-500) * 180 / 2000;
}

uint8_t Servo_GetAngle2(){
	return (TIM_GetCapture2(TIM3)-500) * 180 / 2000;
}

uint8_t Servo_GetAngle3(){
	return (TIM_GetCapture3(TIM3)-500) * 180 / 2000;
}

uint8_t Servo_GetAngle4(){
	return (TIM_GetCapture4(TIM3)-500) * 180 / 2000;
}








