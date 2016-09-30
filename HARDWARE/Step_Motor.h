#ifndef _SM_H
#define _SM_H
 
#include "sys.h"
void step_motor_Init(void);
void step_motor_move1(u32,u32,u32,u8);
void step_motor_move2(u32,u32,u32,u8);
void TIM1_UP_TIM10_IRQHandler(void);
#endif
