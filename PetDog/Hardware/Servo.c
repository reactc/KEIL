#include "stm32f10x.h"                  // Device header
#include "PWM.h"

//Servo用于封装舵机的角度设置与读取函数

void Servo_Init(){
	PWM_Init();
}

/**
  * 函    数：舵机设置角度
  * 参    数：Angle 要设置的舵机角度，范围：0~180
  * 返 回 值：无
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
  * 函    数：舵机读取角度
  * 参    数：无
  * 返 回 值：舵机角度，范围：0~180
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








