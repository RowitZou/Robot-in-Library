#include "iwdg.h"

//TIM13
void IWDG_Init(void)
{   
	u16 arr = 5000-1;
	u16 psc = 16800-1;
	u8 prer = 4;
	u16 rlr = 40000;
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13,ENABLE);  ///ʹ��TIM13ʱ��
	
    TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM13,&TIM_TimeBaseInitStructure);//��ʼ��TIM13
	
	TIM_ITConfig(TIM13,TIM_IT_Update,ENABLE); //����ʱ��13�����ж�
	TIM_Cmd(TIM13,ENABLE); //ʹ�ܶ�ʱ��13
	
	NVIC_InitStructure.NVIC_IRQChannel= TIM8_UP_TIM13_IRQn; //��ʱ��13�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; //��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00; //�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //ʹ�ܶ�IWDG->PR IWDG->RLR��д
	
	IWDG_SetPrescaler(prer); //����IWDG��Ƶϵ��

	IWDG_SetReload(rlr);   //����IWDGװ��ֵ

	IWDG_ReloadCounter(); //reload
	
	IWDG_Enable();       //ʹ�ܿ��Ź�
}


void  TIM8_UP_TIM13_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM13,TIM_IT_Update)==SET) //����ж�
	{
		IWDG_ReloadCounter();//reload
	}
	TIM_ClearITPendingBit(TIM13,TIM_IT_Update);  //����жϱ�־λ
}
