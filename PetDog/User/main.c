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
uint8_t RxData;			//用于接收串口数据的变量
uint8_t move_mode1 = '0';//用于接收蓝牙串口数据的变量
uint8_t move_mode3 = '0';//用于接收语音识别串口数据的变量
uint8_t move_mode = '0';//决定状态变量
uint8_t previous_mode = '0';//用于保存上一次接收串口数据的变量
uint8_t t1=0 ; //存储红外的信号 前
uint8_t t2=0 ; //存储红外的信号 后
uint8_t t3=0 ; //存储红外的信号 左
uint8_t t4=0 ; //存储红外的信号 右

uint16_t ForwardCount=0; //连续前进计数
uint16_t BackwardCount=0; //连续后退计数
uint16_t LeftCount=0; //连续左转计数
uint16_t RightCount=0; //连续右转计数

uint16_t bz_flag;//避障标志位 ON/OFF 默认off
uint16_t hw_flag;//红外标志位 ON/OFF 默认off

int happiness; //开心指数
int stamina;  //体力值

uint16_t Distance=100; // 记录距离 给初值大于10cm(不触发避障) 
int main(void)
{	
	/*模块初始化*/
	LED_Init();			//LED初始化
	Servo_Init();		//舵机初始化
	OLED_Init();		//OLED初始化
	USART1_Init();		//蓝牙串口初始化
	USART2_Init();
	USART3_Init();		//语音识别串口初始化
	redOutside_Init();
	UltrasonicWave_Init();
	happiness=50;//初始化开心指数
	stamina=100;//初始化体力值
	bz_flag=0;//避障标志位 ON/OFF 默认off
	hw_flag=0;//红外标志位 ON/OFF 默认off
	OLED_Clear();
	OLED_ShowImage(0, 0, 128, 64, BMP1); //立正
	
	/**************语音合成芯片设置命令*********************/
	//选择背景音乐2。(0：无背景音乐  1-15：背景音乐可选)
	//m[0~16]:0背景音乐为静音，16背景音乐音量最大
	//v[0~16]:0朗读音量为静音，16朗读音量最大
	//t[0~5]:0朗读语速最慢，5朗读语速最快
	
	SYN_FrameInfo(2, (uint8_t *)"[v12][m5][t5]旺旺");Delay_s(1);//唤醒后狗叫一声,音量12适合演示，10适合调试
	
	while (1)
	{
		//***体力值与开心值反应设置***//
		if(stamina>0)//体力值大于零才会听令
	{
		move_mode1 = USART1_GetRxData();		//获取蓝牙串口接收的数据
		move_mode3 = USART3_GetRxData();		//获取语音识别串口接收的数据
		if (USART1_GetRxFlag()) {//设置蓝牙接收优先级大于语音识别
			move_mode = move_mode1;
		}
		else if (USART3_GetRxFlag()){
			move_mode = move_mode3;
		}
	}
		if(stamina<0)//体力值小于0，执行“难受”状态
	{
		mode_nanshou();
		stamina=0;
	}
		if(stamina==0)//体力值为0后，只接受睡觉指令
	{
		if(USART1_GetRxData()=='2'|USART3_GetRxData()=='2')
		{mode_sleepwo();}
		else if(USART1_GetRxData()=='p'|USART3_GetRxData()=='p')
		{mode_sleeppa();}
	}
		if(stamina>100)//体力值不大于100
		{stamina=100;}
		
		if(happiness<0){//开心值小于0后，执行“难受”状态；给出增加开心值的指令后摆脱难受状态
		happiness=0;
		mode_nanshou();
		}
		if(happiness>100)//开心值不大于100
		{happiness=100;}
		
		//***红外悬崖检测模块设置***//
		if(move_mode == 'H') //红外开
		{hw_flag=1;}
		if(move_mode == 'h') //红外关
		{hw_flag=0;
			//以下给四个传感器输出清零
			t1=0;t2=0;t3=0;t4=0;
		}
		if(hw_flag){
			Edge_detect();//打开红外悬崖检测模式	
		}
		
	   //***超声避障模块设置***//
		if(move_mode == 'x') //超声开
		{
			bz_flag=1;
		}
		if(move_mode == 'c') //超声关
		{
			bz_flag=0;
			Distance=100;
		}
		if(bz_flag){
			avoidBarrier();//打开超声避障模式
		}
		
		
		//***互动设置***//
		if (move_mode == 'f') { //前进
			mode_Forward();
		} else if (move_mode == 'b') { //后退
			mode_Behind();
		} else if (move_mode == 'l') { //左转
			mode_Left();
		} else if (move_mode == 'r') { //右转
			mode_Right();
		} else if (move_mode == 'w') { //前后摇摆
			mode_swing_FrontBack();
		} else if (move_mode == 'z') { //左右摇摆
			mode_swing_LeftRight();
		} else if (move_mode == 'd') { //跳舞
			mode_swing_Dance();
		} else if (move_mode == '5') { //立正
			mode_Stand();
		} else if (move_mode == 'q' && previous_mode != '0') { //起身
			mode_SlowStand();
		} else if (move_mode == 's' && previous_mode != 's') { //坐下
			mode_Strech();
		} else if (move_mode == 'j') { //交替抬手
			mode_TwoHands();
		} else if (move_mode == 'y') { //伸懒腰
			mode_lanyao();
		} else if (move_mode == '1') { //抬头
			mode_headup();
		} else if (move_mode == 'p' && previous_mode != 'p') { //趴下睡觉
			mode_sleeppa();
		} else if (move_mode == '2' && previous_mode != '2') { //卧下睡觉
			mode_sleepwo();
		} else if (move_mode == 'n') { //难受
			mode_nanshou();
		} else if (move_mode == 'B') { //武汉
			mode_wuhan();
		} else if (move_mode == 'Y') { //门都没有
			mode_mendoumeiyou();
		} else if (move_mode == 'X') { //咋回事
			mode_zahuishi();
		} else if (move_mode == 'W') { //为什么
			mode_world();
		} else if (move_mode == 'o') { //打招呼
			mode_hello();
		} else if (move_mode == 'U') { //被叫“小呆”
			mode_xiaodai();
		} else if (move_mode == 'K') { //展示开心值
			mode_happiness();
		} else if (move_mode == 'T') { //展示体力值
			mode_stamina();
		} else if (move_mode == 'Z') { //展示开心值和体力值
			mode_index();
		} 
	}
	
}

