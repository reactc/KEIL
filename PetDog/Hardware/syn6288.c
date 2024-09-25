#include "stm32f10x.h"                  // Device header
#include "usart2.h"
#include "string.h"
#include "delay.h"
#include "syn6288.h"
//PA2---RXD  PA3---TXD


//Music:选择背景音乐。0:无背景音乐，1~15：选择背景音乐
void SYN_FrameInfo(u8 Music, u8 *HZdata){
	unsigned  char  Frame_Info[50];
	unsigned  char  HZ_Length;
	unsigned  char  ecc  = 0;  			//定义校验字节
	unsigned  int i = 0;
	HZ_Length = strlen((char*)HZdata); 			//需要发送文本的长度
	
	
	Frame_Info[0] = 0xFD ; 			//构造帧头FD
	Frame_Info[1] = 0x00 ; 			//构造数据区长度的高字节
	Frame_Info[2] = HZ_Length + 3; 		//构造数据区长度的低字节
	Frame_Info[3] = 0x01 ; 			//构造命令字：合成播放命令
	Frame_Info[4] = 0x01 | Music << 4 ; //构造命令参数：背景音乐设定


	for(i = 0; i < 5; i++)   				//依次发送构造好的5个帧头字节
	{
		ecc = ecc ^ (Frame_Info[i]);		//对发送的字节进行异或校验
	}

	for(i = 0; i < HZ_Length; i++)   		//依次发送待合成的文本数据
	{
		ecc = ecc ^ (HZdata[i]); 				//对发送的字节进行异或校验
	}

	memcpy(&Frame_Info[5], HZdata, HZ_Length);
	Frame_Info[5 + HZ_Length] = ecc;
	USART2_SendString(Frame_Info, 5 + HZ_Length + 1);
}
//配置，停止合成、暂停合成等设置 ，默认波特率9600bps。
void YS_SYN_Set(u8 *Info_data)//固定的信息变量
{
	u8 Com_Len;
	Com_Len = strlen((char*)Info_data);
	USART2_SendString(Info_data, Com_Len);
}









