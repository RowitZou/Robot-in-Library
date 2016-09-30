#include "led.h"
#include "usart.h"
#include "delay.h"
#include "ste_engine.h"
 
 //PB15 TIM12
void TIM12_PWM_Init(u32 arr,u32 psc)
{		 					 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12,ENABLE);  		
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_TIM12); 
	    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
   // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;       
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_Init(GPIOB,&GPIO_InitStructure);   
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
	TIM_TimeBaseStructure.TIM_Period=arr;   
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM12,&TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;	
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 
	
	TIM_OC2Init(TIM12, &TIM_OCInitStructure);  
	TIM_OC2PreloadConfig(TIM12, TIM_OCPreload_Enable);  
 
    TIM_ARRPreloadConfig(TIM12,ENABLE);	
	
	TIM_Cmd(TIM12,ENABLE); 
 	//TIM_CtrlPWMOutputs(TIM12,ENABLE);		  
}  

void ste_engine(void)
{ 
	 u32 push = 690;
	 u32 pull = 1520;
	 u32 middle = 1120;
	 u32 i;
	
	for(i=pull;i>=push;i-=10){
	   TIM_SetCompare2(TIM12,i);
	 if(i>=middle)
	   delay_ms(30);
	 else
	   delay_ms(25);
	}
	 
	delay_ms(1000);
	for(i=push;i<=pull;i+=10){
	   TIM_SetCompare2(TIM12,i);
	   delay_ms(40);
	}
	 
	 TIM_SetCompare2(TIM12,0);
	
}
