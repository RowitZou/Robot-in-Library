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
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10,ENABLE);  ///ʹ��TIM10ʱ��
	
    TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM10,&TIM_TimeBaseInitStructure);//��ʼ��TIM10
	
	TIM_ITConfig(TIM10,TIM_IT_Update,ENABLE); //����ʱ��10�����ж�
	TIM_Cmd(TIM10,ENABLE); //ʹ�ܶ�ʱ��10
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM1_UP_TIM10_IRQn; //��ʱ��10�ж�
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


void TIM1_UP_TIM10_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM10,TIM_IT_Update)==SET) //����ж�
	{
		IWDG_ReloadCounter();//reload
	}
	TIM_ClearITPendingBit(TIM10,TIM_IT_Update);  //����жϱ�־λ
}
