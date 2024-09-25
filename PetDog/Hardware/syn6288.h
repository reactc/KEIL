#ifndef __SYN6288_H_
#define __SYN6288_H_
#include "stm32f10x.h"                  // Device header
//Music:选择背景音乐。0:无背景音乐，1~15：选择背景音乐
void SYN_FrameInfo(u8 Music, u8 *HZdata);
//配置，停止合成、暂停合成等设置 ，默认波特率9600bps。
void YS_SYN_Set(u8 *Info_data);//固定的信息变量
#endif
