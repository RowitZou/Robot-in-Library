#include "iwdg.h"

//TIM10
void IWDG_Init(void)
{   
	u16 arr = 5000-1;
	u16 psc = 16800-1;
	u8 prer = 4;
	u16 rlr = 40000;
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10,ENABLE);  ///使能TIM10时钟
	
    TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM10,&TIM_TimeBaseInitStructure);//初始化TIM10
	
	TIM_ITConfig(TIM10,TIM_IT_Update,ENABLE); //允许定时器10更新中断
	TIM_Cmd(TIM10,ENABLE); //使能定时器10
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM1_UP_TIM10_IRQn; //定时器10中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; //抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00; //子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //使能对IWDG->PR IWDG->RLR的写
	
	IWDG_SetPrescaler(prer); //设置IWDG分频系数

	IWDG_SetReload(rlr);   //设置IWDG装载值

	IWDG_ReloadCounter(); //reload
	
	IWDG_Enable();       //使能看门狗
}


void TIM1_UP_TIM10_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM10,TIM_IT_Update)==SET) //溢出中断
	{
		IWDG_ReloadCounter();//reload
	}
	TIM_ClearITPendingBit(TIM10,TIM_IT_Update);  //清除中断标志位
}
