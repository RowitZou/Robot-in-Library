#include "delay.h"
#include "step_motor.h"
#include <math.h>
u8 STEP_MOTOR_IT = 0 ;

//脉冲设置
void step_motor_Init(void)
{    	 //待修改完善
	
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
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 9;  //25次溢出产生一次中断
	
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
	
	 TIM_ClearFlag(TIM1,TIM_FLAG_Update);          //中断标志位清零
     TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);       //允许捕获/比较1中断
	
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
		

    NVIC_InitStructure.NVIC_IRQChannel          = TIM1_UP_TIM10_IRQn ;  //选择定时器TIM1
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;          //选择抢先式优先级（与中断嵌套级别有关）
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 2;       //选择子优先级（同抢先式优先级的响应顺序）
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;    //选择使能中断源
    NVIC_Init(&NVIC_InitStructure);
	
}

void step_motor_move1(u32 HZ_Min, u32 HZ_Max, u32 time, u8 dir){
   
	u32 psc_max = 1000000/HZ_Min;  //最高频率
	u32 psc_min = 1000000/HZ_Max;  //最低频率
	u32 psc = psc_max;
	u32 interval = 10; //自动重装载值delta

	if(dir)  GPIO_SetBits(GPIOG,GPIO_Pin_2);        //正转
	else     GPIO_ResetBits(GPIOG,GPIO_Pin_2);      //反转
	TIM_SetAutoreload(TIM1,psc);
	TIM_SetCompare3(TIM1,psc/2);
	TIM_SetCompare2(TIM1,0);
	TIM_Cmd(TIM1, ENABLE); 
 	TIM_CtrlPWMOutputs(TIM1,ENABLE);	
    
	while(1){
	   if(STEP_MOTOR_IT){
	      STEP_MOTOR_IT = 0;
	      if(psc<=psc_min&&time>0) time--;  //匀速
		  else if(psc>psc_min&&time>0){     //加速
		     //psc = a[i++];
			  psc-=interval;
			 TIM_SetAutoreload(TIM1,psc);
	         TIM_SetCompare3(TIM1,psc/2);
		  }
		  else if(time<=0&&psc<=psc_max){  //减速
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
   
	u32 psc_max = 1000000/HZ_Min;  //最高频率
	u32 psc_min = 1000000/HZ_Max;  //最低频率
	u32 psc = psc_max;
	u32 interval = 10; //自动重装载值delta

	if(dir)  GPIO_SetBits(GPIOG,GPIO_Pin_4);        //正转
	else     GPIO_ResetBits(GPIOG,GPIO_Pin_4);      //反转
	TIM_SetAutoreload(TIM1,psc);
	TIM_SetCompare2(TIM1,psc/2);
	TIM_SetCompare3(TIM1,0);
	TIM_Cmd(TIM1, ENABLE); 
 	TIM_CtrlPWMOutputs(TIM1,ENABLE);	
    
	while(1){
	   if(STEP_MOTOR_IT){
	      STEP_MOTOR_IT = 0;
	      if(psc<=psc_min&&time>0) time--;  //匀速
		  else if(psc>psc_min&&time>0){     //加速
		     //psc = a[i++];
			  psc-=interval;
			 TIM_SetAutoreload(TIM1,psc);
	         TIM_SetCompare2(TIM1,psc/2);
		  }
		  else if(time<=0&&psc<=psc_max){  //减速
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
  
  if(TIM_GetITStatus(TIM1,TIM_IT_Update)==SET) //溢出中断
	{
		STEP_MOTOR_IT = 1;
	}
  TIM_ClearITPendingBit(TIM1,TIM_IT_Update);  //清除中断标志位
}
