#ifndef _ULTRASONICWAVE_H_
#define _ULTRASONICWAVE_H_
#include "stm32f10x.h"      


void UltrasonicWave_Init(void);
void UltrasonicWave_Start(void);
uint16_t UltrasonicWave_Getvalue(void);
void avoidBarrier(void);


#endif
