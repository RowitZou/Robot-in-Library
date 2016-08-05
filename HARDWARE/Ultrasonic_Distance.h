#ifndef _ULTRASONIC_DISTANCE_H
#define _ULTRASONIC_DISTANCE_H
#include "sys.h"

void Ultrasonic_Init(void);
void Ultrasonic_Work(void);
void  TIM1_BRK_TIM9_IRQHandler(void);
void  TIM3_IRQHandler  (void);

#endif
