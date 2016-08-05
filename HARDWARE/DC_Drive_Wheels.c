#include "sys.h"
#include "delay.h"
#include "DC_Drive_Wheels.h"

//PF0 1 Front Wheel
//PF2 3 Rear  Wheel
//PF4 5 Left  Wheel
//PF6 7 Right Wheel

void DC_Drive_Wheels_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 |  GPIO_Pin_4 |  GPIO_Pin_5 |  GPIO_Pin_6 |  GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOF, &GPIO_InitStructure);
	
  GPIO_SetBits(GPIOF,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 |  GPIO_Pin_5 |  GPIO_Pin_6 |  GPIO_Pin_7);

}

void F_Straight(void){
   GPIO_SetBits(GPIOF,GPIO_Pin_4|GPIO_Pin_6);
   GPIO_ResetBits(GPIOF,GPIO_Pin_5|GPIO_Pin_7);
   GPIO_SetBits(GPIOF,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3);
}

void B_Straight(void){
   GPIO_SetBits(GPIOF,GPIO_Pin_5|GPIO_Pin_7);
   GPIO_ResetBits(GPIOF,GPIO_Pin_4|GPIO_Pin_6);
   GPIO_SetBits(GPIOF,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3);
}

void L_Straight(void){
   GPIO_SetBits(GPIOF,GPIO_Pin_0|GPIO_Pin_2);
   GPIO_ResetBits(GPIOF,GPIO_Pin_1|GPIO_Pin_3);
   GPIO_SetBits(GPIOF,GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
}

void R_Straight(void){
   GPIO_SetBits(GPIOF,GPIO_Pin_1|GPIO_Pin_3);
   GPIO_ResetBits(GPIOF,GPIO_Pin_0|GPIO_Pin_2);
   GPIO_SetBits(GPIOF,GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
}
void F_Turn_Left(void){
   GPIO_SetBits(GPIOF,GPIO_Pin_0| GPIO_Pin_4|GPIO_Pin_6);
   GPIO_ResetBits(GPIOF,GPIO_Pin_1| GPIO_Pin_5|GPIO_Pin_7);
   GPIO_SetBits(GPIOF,GPIO_Pin_2 | GPIO_Pin_3);
}

void F_Turn_Right(void){
   GPIO_SetBits(GPIOF,GPIO_Pin_1| GPIO_Pin_4|GPIO_Pin_6);
   GPIO_ResetBits(GPIOF,GPIO_Pin_0| GPIO_Pin_5|GPIO_Pin_7);
   GPIO_SetBits(GPIOF,GPIO_Pin_2 | GPIO_Pin_3);
}

void B_Turn_Left(void){
   GPIO_SetBits(GPIOF,GPIO_Pin_3| GPIO_Pin_5|GPIO_Pin_7);
   GPIO_ResetBits(GPIOF,GPIO_Pin_2| GPIO_Pin_4|GPIO_Pin_6);
   GPIO_SetBits(GPIOF,GPIO_Pin_0 | GPIO_Pin_1);
}

void B_Turn_Right(void){
   GPIO_SetBits(GPIOF,GPIO_Pin_2| GPIO_Pin_5|GPIO_Pin_7);
   GPIO_ResetBits(GPIOF,GPIO_Pin_3| GPIO_Pin_4|GPIO_Pin_6);
   GPIO_SetBits(GPIOF,GPIO_Pin_0 | GPIO_Pin_1);
}

void L_Turn_Left(void){
   GPIO_SetBits(GPIOF,GPIO_Pin_5| GPIO_Pin_0|GPIO_Pin_2);
   GPIO_ResetBits(GPIOF,GPIO_Pin_4| GPIO_Pin_1|GPIO_Pin_3);
   GPIO_SetBits(GPIOF,GPIO_Pin_6 | GPIO_Pin_7);
}

void L_Turn_Right(void){
   GPIO_SetBits(GPIOF,GPIO_Pin_4| GPIO_Pin_0|GPIO_Pin_2);
   GPIO_ResetBits(GPIOF,GPIO_Pin_5| GPIO_Pin_1|GPIO_Pin_3);
   GPIO_SetBits(GPIOF,GPIO_Pin_6 | GPIO_Pin_7);
}

void R_Turn_Left(void){
   GPIO_SetBits(GPIOF,GPIO_Pin_6| GPIO_Pin_1|GPIO_Pin_3);
   GPIO_ResetBits(GPIOF,GPIO_Pin_7| GPIO_Pin_0|GPIO_Pin_2);
   GPIO_SetBits(GPIOF,GPIO_Pin_4 | GPIO_Pin_5);
}

void R_Turn_Right(void){
   GPIO_SetBits(GPIOF,GPIO_Pin_7| GPIO_Pin_1|GPIO_Pin_3);
   GPIO_ResetBits(GPIOF,GPIO_Pin_6| GPIO_Pin_0|GPIO_Pin_2);
   GPIO_SetBits(GPIOF,GPIO_Pin_4 | GPIO_Pin_5);
}

void ALL_Stop(void){

 GPIO_SetBits(GPIOF,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);

}
