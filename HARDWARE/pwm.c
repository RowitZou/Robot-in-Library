#include "pwm.h"
#include "led.h"
#include "usart.h"
 
void TIM4_PWM_Init(u32 arr,u32 psc)
{		 					 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  	
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 
	
	//GPIO_PinAFConfig(GPIOA,GPIO_PinSource2|GPIO_PinSource3,GPIO_AF_TIM2); 
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4); 
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource14,GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource15,GPIO_AF_TIM4);
	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;           
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;       
//	GPIO_Init(GPIOA,&GPIO_InitStructure);     

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
    GPIO_Init(GPIOD,&GPIO_InitStructure);   
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
	TIM_TimeBaseStructure.TIM_Period=arr;   
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;	
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 
	TIM_OCInitStructure.TIM_Pulse = TIM4->CCR1; 
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  
	
	TIM_OCInitStructure.TIM_Pulse = TIM4->CCR2; 
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable); 

    TIM_OCInitStructure.TIM_Pulse = TIM4->CCR3; 
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  
	
	TIM_OCInitStructure.TIM_Pulse = TIM4->CCR4; 
	TIM_OC4Init(TIM4, &TIM_OCInitStructure); 
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  
 
    TIM_ARRPreloadConfig(TIM4,ENABLE);
	
	TIM_Cmd(TIM4, ENABLE); 
 	TIM_CtrlPWMOutputs(TIM4,ENABLE);		  
}  
