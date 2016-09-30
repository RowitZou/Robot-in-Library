#include "delay.h"
#include "step_motor.h"
#include <math.h>
u8 STEP_MOTOR_IT = 0 ;

//��������
void step_motor_Init(void)
{    	 //���޸�����
	
  u32 arr = 4000-1; //1MHz/4000=250Hz
  u32 psc = 168-1;
  GPIO_InitTypeDef  GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);//Enable the GPIOF clock
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//Enable the GPIOF clock

  //GPIOF9,F10 configuration
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOG, &GPIO_InitStructure);
	
  GPIO_ResetBits(GPIOG,GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 
	
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_TIM1); 
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_TIM1);
	      
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;       
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_13;
    GPIO_Init(GPIOE,&GPIO_InitStructure);   
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
	TIM_TimeBaseStructure.TIM_Period=arr;   
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 9;  //25���������һ���ж�
	
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
	
	 TIM_ClearFlag(TIM1,TIM_FLAG_Update);          //�жϱ�־λ����
     TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);       //������/�Ƚ�1�ж�
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;	
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 
	TIM_OCInitStructure.TIM_Pulse = TIM1->CCR2; 
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);  
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);  
	
	TIM_OCInitStructure.TIM_Pulse = TIM1->CCR3; 
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);  
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable); 
 
    TIM_ARRPreloadConfig(TIM1,ENABLE);
		

    NVIC_InitStructure.NVIC_IRQChannel          = TIM1_UP_TIM10_IRQn ;  //ѡ��ʱ��TIM1
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;          //ѡ������ʽ���ȼ������ж�Ƕ�׼����йأ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 2;       //ѡ�������ȼ���ͬ����ʽ���ȼ�����Ӧ˳��
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;    //ѡ��ʹ���ж�Դ
    NVIC_Init(&NVIC_InitStructure);
	
}

void step_motor_move1(u32 HZ_Min, u32 HZ_Max, u32 time, u8 dir){
   
	u32 psc_max = 1000000/HZ_Min;  //���Ƶ��
	u32 psc_min = 1000000/HZ_Max;  //���Ƶ��
	u32 psc = psc_max;
	u32 interval = 10; //�Զ���װ��ֵdelta

	if(dir)  GPIO_SetBits(GPIOG,GPIO_Pin_2);        //��ת
	else     GPIO_ResetBits(GPIOG,GPIO_Pin_2);      //��ת
	TIM_SetAutoreload(TIM1,psc);
	TIM_SetCompare3(TIM1,psc/2);
	TIM_SetCompare2(TIM1,0);
	TIM_Cmd(TIM1, ENABLE); 
 	TIM_CtrlPWMOutputs(TIM1,ENABLE);	
    
	while(1){
	   if(STEP_MOTOR_IT){
	      STEP_MOTOR_IT = 0;
	      if(psc<=psc_min&&time>0) time--;  //����
		  else if(psc>psc_min&&time>0){     //����
		     //psc = a[i++];
			  psc-=interval;
			 TIM_SetAutoreload(TIM1,psc);
	         TIM_SetCompare3(TIM1,psc/2);
		  }
		  else if(time<=0&&psc<=psc_max){  //����
		    // psc = a[--i];
			  psc+=interval;
			 TIM_SetAutoreload(TIM1,psc);
	         TIM_SetCompare3(TIM1,psc/2);
		  }
		  else {
		     TIM_SetCompare3(TIM1,0);
			 TIM_Cmd(TIM1,DISABLE);
			 TIM_CtrlPWMOutputs(TIM1,DISABLE);
			 STEP_MOTOR_IT = 0;
			 return;
		  }
	   }
	}
}

void step_motor_move2(u32 HZ_Min, u32 HZ_Max, u32 time, u8 dir){
   
	u32 psc_max = 1000000/HZ_Min;  //���Ƶ��
	u32 psc_min = 1000000/HZ_Max;  //���Ƶ��
	u32 psc = psc_max;
	u32 interval = 10; //�Զ���װ��ֵdelta

	if(dir)  GPIO_SetBits(GPIOG,GPIO_Pin_4);        //��ת
	else     GPIO_ResetBits(GPIOG,GPIO_Pin_4);      //��ת
	TIM_SetAutoreload(TIM1,psc);
	TIM_SetCompare2(TIM1,psc/2);
	TIM_SetCompare3(TIM1,0);
	TIM_Cmd(TIM1, ENABLE); 
 	TIM_CtrlPWMOutputs(TIM1,ENABLE);	
    
	while(1){
	   if(STEP_MOTOR_IT){
	      STEP_MOTOR_IT = 0;
	      if(psc<=psc_min&&time>0) time--;  //����
		  else if(psc>psc_min&&time>0){     //����
		     //psc = a[i++];
			  psc-=interval;
			 TIM_SetAutoreload(TIM1,psc);
	         TIM_SetCompare2(TIM1,psc/2);
		  }
		  else if(time<=0&&psc<=psc_max){  //����
		    // psc = a[--i];
			  psc+=interval;
			 TIM_SetAutoreload(TIM1,psc);
	         TIM_SetCompare2(TIM1,psc/2);
		  }
		  else {
		     TIM_SetCompare2(TIM1,0);
			 TIM_Cmd(TIM1,DISABLE);
			 TIM_CtrlPWMOutputs(TIM1,DISABLE);
			 STEP_MOTOR_IT = 0;
			 return;
		  }
	   }
	}
}

void TIM1_UP_TIM10_IRQHandler(void){
  
  if(TIM_GetITStatus(TIM1,TIM_IT_Update)==SET) //����ж�
	{
		STEP_MOTOR_IT = 1;
	}
  TIM_ClearITPendingBit(TIM1,TIM_IT_Update);  //����жϱ�־λ
}
