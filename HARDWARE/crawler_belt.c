#include "crawler_belt.h"

void crawler_belt_init(void){
   
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);//Enable the GPIOF clock

  //GPIOF9,F10 configuration
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOG, &GPIO_InitStructure);

  GPIO_SetBits(GPIOG,GPIO_Pin_6 | GPIO_Pin_7);

}

void crawler_belt_move(void){
  
	GPIO_ResetBits(GPIOG,GPIO_Pin_6);

}

void crawler_belt_stop(){

    GPIO_SetBits(GPIOG,GPIO_Pin_6);
}
