#include "stm32f10x.h"                  // Device header
#include "stdlib.h"
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
#include "stdio.h"

extern uint8_t move_mode1 ;//用于接收蓝牙串口数据的变量
extern uint8_t move_mode3 ;//用于接收语音识别串口数据的变量
extern uint8_t move_mode ;//决定状态变量
extern uint8_t previous_mode ;//用于保存上一次接收串口数据的变量

extern uint16_t bz_flag;//避障标志位 ON/OFF 默认off
extern uint16_t hw_flag;//红外标志位 ON/OFF 默认off

extern int happiness; //开心指数
extern int stamina;  //体力值

char read[8]={0};  //朗读开心值或体力值的字符串变量

extern uint16_t ForwardCount; //连续前进计数
extern uint16_t BackwardCount; //连续后退计数
extern uint16_t LeftCount; //连续左转计数
extern uint16_t RightCount; //连续右转计数


void mode_Forward(void){//前进
	OLED_ShowImage(0,0,128,64,BMP2);
	move_forward();
	LED13_Turn();
	if(previous_mode == 'f'){
		ForwardCount++;
	}else{
		ForwardCount=0;
	}
	if(ForwardCount>=5){
		happiness -=3;
	}
	previous_mode = move_mode;
	stamina -= 5;
}

void mode_Behind(void){//后退
	OLED_ShowImage(0,0,128,64,BMP2);
	move_behind();
	LED13_Turn();
	if(previous_mode == 'b'){
		BackwardCount++;
	}else{
		BackwardCount=0;
	}
	if(BackwardCount>=5){
		happiness -=3;
	}
	previous_mode = move_mode;
	stamina -= 5;
}

void mode_Left(void){//左转
	OLED_ShowImage(0,0,128,64,BMP3);
	move_left();
	LED13_Turn();
	if(previous_mode == 'l'){
		LeftCount++;
	}else{
		LeftCount=0;
	}
	if(LeftCount>=5){
		happiness -=3;
	}
	previous_mode = move_mode;
	stamina -= 5;
}

void mode_Right(void){//右转
	OLED_ShowImage(0,0,128,64,BMP4);
	move_right();
	LED13_Turn();
	if(previous_mode == 'l'){
		RightCount++;
	}else{
		RightCount=0;
	}
	if(RightCount>=5){
		happiness -=3;
	}
	previous_mode = move_mode;
	stamina -= 5;
}

void mode_swing_FrontBack(void){//前后摇摆
	OLED_ShowImage(0, 0, 128, 64, BMP11); //迷糊脸
	move_shake_qianhou();
	LED13_Turn();
	previous_mode = move_mode;
	stamina -= 5;
	happiness +=5;
	
}

void mode_swing_LeftRight(void){//左右摇摆
	OLED_ShowImage(0, 0, 128, 64, BMP11); //迷糊脸
	move_shake_zuoyou();
	LED13_Turn();
	previous_mode = move_mode;
	stamina -= 5;
	happiness +=5;
	
}
void mode_swing_Dance(void){//跳舞
	OLED_ShowImage(0, 0, 128, 64, BMP5); //特殊脸
	move_dance();
	LED13_Turn();
	previous_mode = move_mode;
	stamina -= 5;
	happiness +=5;
	
}

void mode_Stand(void){//立正
	OLED_ShowImage(0, 0, 128, 64, BMP1); //立正脸
	move_dance();
	LED13_Turn();
	previous_mode = move_mode;
	stamina -= 1;
	happiness -=5;
	SYN_FrameInfo(0, (uint8_t *)"[v12][m5][t5]立正汪汪汪");
	Delay_s(1);
	
}

void mode_SlowStand(void){//缓慢起身
	if(move_mode3 == 'q'){//语音交互收到起身发送q
		SYN_FrameInfo(0, (uint8_t *)"[v12][m5][t5]好累好累");
	}
	OLED_ShowImage(0, 0, 128, 64, BMP1); //立正脸
	move_slow_stand(previous_mode);
	LED13_Turn();
	previous_mode = move_mode;
	move_mode = '0';
	stamina -= 1;
}
void mode_Strech(void){//坐下擦脸
	if(move_mode3 == 's'){//语音交互收到起身发送s
		SYN_FrameInfo(0, (uint8_t *)"[v12][m5][t5]我的脸很干净");
	}
	OLED_ShowImage(0, 0, 128, 64, BMP1); //立正脸
	move_slow_stand(previous_mode);
	OLED_ShowImage(0, 0, 128, 64, BMP2);//前进脸
	move_stretch();
	OLED_ShowImage(0, 0, 128, 64, BMP12);//前进脸
	LED13_Turn();
	previous_mode = move_mode;
	move_mode = '0';
	stamina += 3;
	happiness +=5;

}
void mode_hello(void){//打招呼
	if(previous_mode != '5' && previous_mode != 'q'){
		OLED_ShowImage(0, 0, 128, 64, BMP1); //立正脸
		move_slow_stand(previous_mode);
	}
	OLED_ShowImage(0, 0, 128, 64, BMP12);//猫猫脸
	int i;
	for(i=0;i<20;i++)//
	{
		Servo_SetAngle1(90-i);
		Servo_SetAngle2(90+i);
		//Servo_SetAngle3(90+i);
		Servo_SetAngle4(90-i);
		Delay_ms(7);
	}
	for(i=0;i<40;i++)//
	{
		Servo_SetAngle2(110+i);
		Servo_SetAngle4(70-i);
		Delay_ms(7);
	}
	for(i=0;i<60;i++)//右前足缓慢抬起
	{
		Servo_SetAngle1(70+i);
		Delay_ms(4);
	}
	//下面是摇两次右前足
	SYN_FrameInfo(0, (uint8_t *)"[v12][m5][t5]你好你好");
	Servo_SetAngle1(180);
	Delay_ms(400);
	Servo_SetAngle1(130);
	Delay_ms(400);
	Servo_SetAngle1(180);
	Delay_ms(400);
	Servo_SetAngle1(130);
	Delay_ms(400);
	Servo_SetAngle1(70);
	Delay_ms(500);
	LED13_Turn();
	previous_mode = move_mode;
	move_mode = '0';
	stamina -=2;
	happiness +=2;
}
void mode_TwoHands(void)//交替抬手
{
	OLED_ShowImage(0, 0, 128, 64, BMP1); //立正脸
	move_stand();
	move_two_hands();
	LED13_Turn();
	previous_mode = move_mode;
	move_mode = '0';
	stamina -=5;
	happiness +=2;
}
void mode_lanyao(void)//懒腰
{
	OLED_ShowImage(0, 0, 128, 64, BMP1); //立正脸
	move_slow_stand(previous_mode);
	OLED_ShowImage(0, 0, 128, 64, BMP9);//开心脸
	lan_yao();
	OLED_ShowImage(0, 0, 128, 64, BMP1);//立正脸
	LED13_Turn();
	previous_mode = move_mode;
	move_mode = '0';
	stamina +=5;
	happiness +=1;
}

void mode_headup(void)//抬头
{
	OLED_ShowImage(0, 0, 128, 64, BMP1); //立正脸
	move_slow_stand(previous_mode);
	OLED_ShowImage(0, 0, 128, 64, BMP10);//调皮脸
	move_head_up();
	LED13_Turn();
	previous_mode = move_mode;
	move_mode = '0';
	stamina -=5;
	
}
void mode_sleeppa(void)//趴下睡觉
{
	if (previous_mode != '5' && previous_mode != 'q') {
		OLED_ShowImage(0, 0, 128, 64, BMP1); //立正脸
		move_slow_stand(previous_mode);
	}
	if (rand()%2) {//随机产生两种表情中的一种
		OLED_ShowImage(0, 0, 128, 64, BMP6); //普通睡觉脸
	}
	else{
		OLED_ShowImage(0, 0, 128, 64, BMP8); //酣睡脸
	}
	move_sleep_p();
	SYN_FrameInfo(2, (uint8_t *)"[v12][m5][t5]睡了睡了");Delay_s(1);
	previous_mode = move_mode;
	move_mode = '0';
	stamina =100;
	happiness +=15;
}


void mode_sleepwo(void)//卧下睡觉
{
	if (previous_mode != '5' && previous_mode != 'q') {
		OLED_ShowImage(0, 0, 128, 64, BMP1); //立正脸
		move_slow_stand(previous_mode);
		Delay_s(1);
	}
	if (rand()%2) {//随机产生两种表情中的一种
		OLED_ShowImage(0, 0, 128, 64, BMP6); //普通睡觉脸
	}
	else{
		OLED_ShowImage(0, 0, 128, 64 , BMP8); //酣睡脸
	}
	move_sleep_w();
	SYN_FrameInfo(2, (uint8_t *)"[v12][m5][t5]晚安啦");Delay_s(1);
	previous_mode = move_mode;
	move_mode = '0';
	stamina =100;
	happiness +=15;
}
void mode_nanshou(void)//难受
{
	OLED_ShowImage(0, 0, 128, 64, BMP2); //前进脸	
	move_sleep_w();
	SYN_FrameInfo(2, (uint8_t *)"[v12][m5][t5]我是不是发烧了");Delay_s(1);
	previous_mode = move_mode;
	move_mode = '0';
	stamina -=50;
	happiness -=50;
}

void mode_wuhan(void)//武汉
{
	OLED_ShowImage(0, 0, 128, 64, BMP7); //love花痴脸	
	Servo_SetAngle1(135);//抬起右前脚
	SYN_FrameInfo(2, (uint8_t *)"[v12][m5][t5]武汉确实都是甜妹，猫系女孩");Delay_s(6);
	OLED_ShowImage(0, 0, 128, 64, BMP11); //开心迷糊脸	
	Servo_SetAngle3(45);//抬起左前脚
	SYN_FrameInfo(2, (uint8_t *)"[v12][m5][t5]拉天载借头忍不住多看了两眼");Delay_s(5);
	Servo_SetAngle1(90);//
	Servo_SetAngle3(90);//收回两只前脚
	OLED_ShowImage(0, 0, 128, 64, BMP9); //开心脸	
	SYN_FrameInfo(2, (uint8_t *)"[v12][m5][t5]她跟我说喵喵喵");Delay_s(3);
	OLED_ShowImage(0, 0, 128, 64, BMP7); //love花痴脸
	SYN_FrameInfo(2, (uint8_t *)"[v12][m5][t5]喵莫比喵");
	move_shake_qianhou();
	move_shake_qianhou();
	Delay_s(5);
	previous_mode = move_mode;
	move_mode = '0';
	stamina -=2;
	happiness +=5;
}

void mode_mendoumeiyou(void)//门都没有
{
	OLED_ShowImage(0, 0, 128, 64, BMP5); //嘿嘿脸	
	Servo_SetAngle1(135);//抬起右前脚
	SYN_FrameInfo(2, (uint8_t *)"[v12][m5][t5]大家都散了吧");Delay_s(2);Delay_ms(500);
	OLED_ShowImage(0, 0, 128, 64, BMP10); //调皮脸	
	Servo_SetAngle3(45);//抬起左前脚
	SYN_FrameInfo(2, (uint8_t *)"[v12][m5][t5]她家老穷了");Delay_s(2);Delay_ms(500);
	Servo_SetAngle2(45);//右后脚向后抬
	Servo_SetAngle4(135);//左后脚向后抬
	OLED_ShowImage(0, 0, 128, 64, BMP12); //猫猫脸	
	SYN_FrameInfo(2, (uint8_t *)"[v12][m5][t5]我刚刚私信她表白");Delay_s(2);Delay_ms(700);
	OLED_ShowImage(0, 0, 128, 64, BMP11); //开心迷糊脸
	SYN_FrameInfo(2, (uint8_t *)"[v12][m5][t5] 她说门都没有");
	move_shake_qianhou();
	move_shake_qianhou();
	move_shake_qianhou();
	Delay_s(2);
	previous_mode = move_mode;
	move_mode = '0';
	stamina -=2;
	happiness +=2;
}

void mode_zahuishi(void)//咋回事
{
	OLED_ShowImage(0, 0, 128, 64, BMP5); //嘿嘿脸	
	Servo_SetAngle1(135);//抬起右前脚
	SYN_FrameInfo(2, (uint8_t *)"[v12][m5][t5]你这倒显得是我的不是了");Delay_s(5);//两个字刚好1s
	OLED_ShowImage(0, 0, 128, 64, BMP10); //调皮脸	
	Servo_SetAngle3(45);//抬起左前脚
	SYN_FrameInfo(2, (uint8_t *)"[v12][m5][t5]可不要放在心上");Delay_s(3);
	Servo_SetAngle2(45);//右后脚向后抬
	Servo_SetAngle4(135);//左后脚向后抬
	OLED_ShowImage(0, 0, 128, 64, BMP12); //猫猫脸	
	SYN_FrameInfo(2, (uint8_t *)"[v12][m5][t5]给我画上一笔");Delay_s(2);
	OLED_ShowImage(0, 0, 128, 64, BMP11); //开心迷糊脸
	SYN_FrameInfo(2, (uint8_t *)"[v12][m5][t5]不然我这心窝子可要难受了");
	move_shake_qianhou();
	move_shake_qianhou();
	Delay_s(2);
	previous_mode = move_mode;
	move_mode = '0';
	stamina -=2;
}
void mode_world(void)//世界之光
{
	OLED_ShowImage(0, 0, 128, 64, BMP5); //嘿嘿脸	
	SYN_FrameInfo(2, (uint8_t *)"[v12][m5][t5]我感觉很一般");
	Servo_SetAngle2(135);//右后脚向前抬
	Servo_SetAngle4(45);//左后脚向前抬
	Delay_s(2);
	previous_mode = move_mode;
	move_mode = '0';
}
void mode_xiaodai(void)//语音唤醒小呆
{
	OLED_ShowImage(0, 0, 128, 64, BMP1); //立正脸
	SYN_FrameInfo(2, (uint8_t *)"[v12][m5][t5]我在");
	Delay_s(1);
	previous_mode = move_mode;
	move_mode = '0';
}
void OLED_happiness(void)//OLED展示开心值
{
	OLED_ShowChinese(0,0,"开心值，");
	if(happiness==100)
	{OLED_ShowNum(64,0,100,3,OLED_8X16);}
	else {OLED_ShowNum(64,0,happiness,2,OLED_8X16);}
	OLED_UpdateArea(0,0,128,16);
}
void mode_happiness(void)//展示开心值
{
	OLED_ShowImage(0, 8, 128, 48, BMP1); //立正脸
	OLED_happiness();
	sprintf(read,"[v12][m5][t5]当前的开心值为%d",happiness);
	SYN_FrameInfo(2,(u8*)read);Delay_s(4);
	if(happiness>=60){
		OLED_ShowImage(0, 8, 128, 48, BMP9); //开心脸
		OLED_happiness();
		SYN_FrameInfo(2, (uint8_t *)"[v12][m5][t5]今天的心情还可以");
		move_shake_qianhou();
		move_shake_qianhou();Delay_s(4);
	}
	else if(happiness<=10){
		OLED_ShowImage(0, 8, 128, 48, BMP2);
		OLED_happiness();
		SYN_FrameInfo(2, (uint8_t *)"[v12][m5][t5]想出去玩");
		if (rand()%2) //随机产生两种动作中的一种
			{move_sleep_p();}
		else{move_sleep_w();}
	}Delay_s(3);
	previous_mode = move_mode;
	move_mode = '0';
}

void OLED_stamina(void)//OLED展示体力值
{
	OLED_ShowChinese(0,0,"体力值，");
	if(stamina==100)
	{OLED_ShowNum(64,0,100,3,OLED_8X16);}
	else {OLED_ShowNum(64,0,stamina,2,OLED_8X16);}
	OLED_UpdateArea(0,0,128,16);
}
void mode_stamina(void)//展示体力值
{
	OLED_ShowImage(0, 8, 128, 48, BMP1); //立正脸
	OLED_stamina();
	sprintf(read,"[v12][m5][t5]当前的体力值为%d",stamina);
	SYN_FrameInfo(2,(u8*)read);Delay_s(4);
	if(stamina>=60){
		OLED_ShowImage(0, 8, 128, 48, BMP9); //开心脸
		OLED_stamina();
		SYN_FrameInfo(2, (uint8_t *)"[v12][m5][t5]小呆精力满满");
		move_shake_qianhou();
		move_shake_qianhou();Delay_s(6);
	}
	else if(stamina<=10){
		OLED_ShowImage(0, 8, 128, 48, BMP2);
		OLED_stamina();
		SYN_FrameInfo(2, (uint8_t *)"[v12][m5][t5]好累，好累");
		if (rand()%2) //随机产生两种动作中的一种
			{move_sleep_p();}
		else{move_sleep_w();}
	}Delay_s(4);
	previous_mode = move_mode;
	move_mode = '0';
}

void mode_index(void)//展示开心值和体力值
{
	OLED_Clear();
	OLED_ShowChinese(0,0,"开心值，");
	if(happiness==100)
	{OLED_ShowNum(64,0,100,3,OLED_8X16);}
	else {OLED_ShowNum(64,0,happiness,2,OLED_8X16);}
	OLED_ShowChinese(0,16,"体力值，");
	if(stamina==100)
	{OLED_ShowNum(64,16,100,3,OLED_8X16);}
	else {OLED_ShowNum(64,16,stamina,2,OLED_8X16);}
	OLED_UpdateArea(0,0,128,32);
	sprintf(read,"[v12][m5][t5]当前的开心值为%d，体力值为%d",happiness,stamina);
	SYN_FrameInfo(2,(u8*)read);Delay_s(10);
	previous_mode = move_mode;
	move_mode = '0';
}



