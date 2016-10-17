 /*
     This function is measuring the distance.
	 
	 Use PE1,PE3,PE5,PC13,PB3,PB5,PB7,PB9,TIM9,TIM3 
	 TIM9 LEFT ; TIM3 RIGHT
 */
 #include "sys.h"
 #include "delay.h"
 #include "usart.h"
 #include "Ultrasonic_Distance.h"
 #include "DC_Drive_Wheels.h"
 #define  DISTANCE1 710
 #define  DISTANCE2 740
 extern int left_state;
 
 TIM_ICInitTypeDef  TIM9_ICInitStructure;
 TIM_ICInitTypeDef  TIM3_ICInitStructure;
 
 void Ultrasonic_Init(void){
    
  u16 arr=0XFFFF;  
  u16 psc=84-1;   
  u16 psc1=168-1; 
  GPIO_InitTypeDef  GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE);  	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	

  //GPIOE1,3,GPIOC13 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_3 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 ;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  GPIO_ResetBits(GPIOE,GPIO_Pin_3);
  GPIO_SetBits(GPIOE,GPIO_Pin_1);
  GPIO_ResetBits(GPIOC,GPIO_Pin_13);
  
  //GPIOB9,7,3
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9| GPIO_Pin_7|GPIO_Pin_3;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_ResetBits(GPIOB,GPIO_Pin_3);
  GPIO_SetBits(GPIOB,GPIO_Pin_9);
  GPIO_ResetBits(GPIOB,GPIO_Pin_7);

  //GPIOE5
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOE,&GPIO_InitStructure); 
  
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource5,GPIO_AF_TIM9);
  
  TIM_TimeBaseStructure.TIM_Prescaler=psc1;
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
  TIM_TimeBaseStructure.TIM_Period=arr;  
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
  
  TIM_TimeBaseInit(TIM9,&TIM_TimeBaseStructure);
  
  TIM9_ICInitStructure.TIM_Channel = TIM_Channel_1; 
  TIM9_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM9_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 
  TIM9_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	
  TIM9_ICInitStructure.TIM_ICFilter = 0x00;
  TIM_ICInit(TIM9, &TIM9_ICInitStructure);
  
  TIM_ITConfig(TIM9,TIM_IT_Update|TIM_IT_CC1,ENABLE);
  
  TIM_Cmd(TIM9,ENABLE ); 	
  
  NVIC_InitStructure.NVIC_IRQChannel =  TIM1_BRK_TIM9_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);	
	
  //GPIOB5
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; 
  GPIO_Init(GPIOB,&GPIO_InitStructure); 
  
  GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_TIM3); 
  
  TIM_TimeBaseStructure.TIM_Prescaler=psc;  
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period=arr;  
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
  
  TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
  
  TIM3_ICInitStructure.TIM_Channel = TIM_Channel_2; 
  TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	
  TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 
  TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	
  TIM3_ICInitStructure.TIM_ICFilter = 0x00;
  TIM_ICInit(TIM3, &TIM3_ICInitStructure);
  
  TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_CC2,ENABLE);
  
  TIM_Cmd(TIM3,ENABLE ); 	
  
  NVIC_InitStructure.NVIC_IRQChannel =  TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);	
	
 }
 
u8  TIM9CH1_CAPTURE_STA=0; 			
u16	TIM9CH1_CAPTURE_VAL;    
 
u8  TIM3CH2_CAPTURE_STA=0; 			
u16	TIM3CH2_CAPTURE_VAL;    
 
 void Ultrasonic_Work(void){
	 
	long long temp1 = 0;
	long long temp2 = 0;
	int parallel_state = 10;
	TIM_SetCompare1(TIM4,800);
	TIM_SetCompare2(TIM4,800);
    TIM_SetCompare3(TIM4,800);
	TIM_SetCompare4(TIM4,800);
     while(1){
      GPIO_SetBits(GPIOE,GPIO_Pin_3); 
	  GPIO_SetBits(GPIOB,GPIO_Pin_7);
	   delay_us(10);
	  GPIO_ResetBits(GPIOE,GPIO_Pin_3); 
	  GPIO_ResetBits(GPIOB,GPIO_Pin_7); 
 	   if(TIM9CH1_CAPTURE_STA&0X80)        //成功捕获到了一次高电平
		{
			temp1=TIM9CH1_CAPTURE_STA&0X3F; 
			temp1*=0XFFFF;		 		         //溢出时间总和
			temp1+=TIM9CH1_CAPTURE_VAL;		
           // temp1=temp1/2;			//得到总的高电平时间
			/*if(parallel_state){
			  if(temp1<=DISTANCE||temp2<=DISTANCE){
			     ALL_Stop();
				 return ;
			  }
			  else  F_Straight();
			}
			else if(temp1-temp2<=6&&temp2-temp1<=6) {
			  parallel_state = 1;
			  ALL_Stop();
			  delay_ms(100);
			}
			else if(temp1<temp2){
	          Shelf_Turn_Left();		
			}
			else  {
			  Shelf_Turn_Right();
			}*/
			//printf("ADISTANCE:%.3lf m\r\n",(temp1*0.00034)/2.0); //打印总的高点平时间
			TIM9CH1_CAPTURE_STA=0;			    //开启下一次捕获
		}
		if(TIM3CH2_CAPTURE_STA&0X80)        //成功捕获到了一次高电平
		{
			temp2=TIM3CH2_CAPTURE_STA&0X3F; 
			temp2*=0XFFFF;		 		         //溢出时间总和
			temp2+=TIM3CH2_CAPTURE_VAL;		   //得到总的高电平时间
		    if(!parallel_state){
			  if(!left_state){
			     if(temp1<=DISTANCE1||temp2<=DISTANCE1){
			          ALL_Stop();
				      return ;
			     }
			     else  F_Straight();
		      }
			  else{
			     if(temp1<=DISTANCE2||temp2<=DISTANCE2){
			          ALL_Stop();
				      return ;
			     }
			     else  F_Straight();
		      }
			}
			else if(parallel_state>1){
			  parallel_state--;
			}
			else if(temp1-temp2<=6&&temp2-temp1<=6) {
			  parallel_state = 0;
			  ALL_Stop();
			  delay_ms(10);
			}
			else if(temp1<temp2){
	          Shelf_Turn_Left();		
			}
			else  {
			  Shelf_Turn_Right();
			}
			//printf("                          BDISTANCE:%.3lf m\r\n",(temp2*0.00034)/2.0); //打印总的高点平时间
			TIM3CH2_CAPTURE_STA=0;			     //开启下一次捕获
		}
		   delay_ms(20); 
	}
 }
 
 /*
 
   TIM1_BRK_TIM9 和 TIM3中断输入捕获
 
 */
void  TIM1_BRK_TIM9_IRQHandler (void)
{ 		    

 	if((TIM9CH1_CAPTURE_STA&0X80)==0)
	{
		if(TIM_GetITStatus(TIM9, TIM_IT_Update) != RESET)
		{	     
			if(TIM9CH1_CAPTURE_STA&0X40)
			{
				if((TIM9CH1_CAPTURE_STA&0X3F)==0X3F)
				{
					TIM9CH1_CAPTURE_STA|=0X80;		
					TIM9CH1_CAPTURE_VAL=0XFFFF;
				}else TIM9CH1_CAPTURE_STA++;
			}	 
		}
		if(TIM_GetITStatus(TIM9, TIM_IT_CC1) != RESET)
		{	
			if(TIM9CH1_CAPTURE_STA&0X40)		
			{	  			
				TIM9CH1_CAPTURE_STA|=0X80;		
			  TIM9CH1_CAPTURE_VAL=TIM_GetCapture1(TIM9);
	 			TIM_OC1PolarityConfig(TIM9,TIM_ICPolarity_Rising); 
			}else  								
			{
				TIM9CH1_CAPTURE_STA=0;			
				TIM9CH1_CAPTURE_VAL=0;
				TIM9CH1_CAPTURE_STA|=0X40;		
				TIM_Cmd(TIM9,DISABLE ); 	
	 			TIM_SetCounter(TIM9,0);
	 			TIM_OC1PolarityConfig(TIM9,TIM_ICPolarity_Falling);		
				TIM_Cmd(TIM9,ENABLE ); 	
			}		    
		}			     	    					   
 	}
	TIM_ClearITPendingBit(TIM9, TIM_IT_CC1|TIM_IT_Update); 
}

void  TIM3_IRQHandler  (void)
{ 		    

 	if((TIM3CH2_CAPTURE_STA&0X80)==0)
	{
		if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)//overflow
		{	     
			if(TIM3CH2_CAPTURE_STA&0X40)//already capture the high level
			{
				if((TIM3CH2_CAPTURE_STA&0X3F)==0X3F)
				{
					TIM3CH2_CAPTURE_STA|=0X80;		
					TIM3CH2_CAPTURE_VAL=0XFFFF;
				}else TIM3CH2_CAPTURE_STA++;
			}	 
		}
		if(TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET)
		{	
			if(TIM3CH2_CAPTURE_STA&0X40)		
			{	  			
				TIM3CH2_CAPTURE_STA|=0X80;		
			  TIM3CH2_CAPTURE_VAL=TIM_GetCapture2(TIM3);
	 			TIM_OC2PolarityConfig(TIM3,TIM_ICPolarity_Rising); 
			}else  								
			{
				TIM3CH2_CAPTURE_STA=0;			
				TIM3CH2_CAPTURE_VAL=0;
				TIM3CH2_CAPTURE_STA|=0X40;		
				TIM_Cmd(TIM3,DISABLE ); 	
	 			TIM_SetCounter(TIM3,0);
	 			TIM_OC2PolarityConfig(TIM3,TIM_ICPolarity_Falling);		
				TIM_Cmd(TIM3,ENABLE ); 
			}		    
		}			     	    					   
 	}
	TIM_ClearITPendingBit(TIM3, TIM_IT_CC2|TIM_IT_Update); //clear the interruption flag bit
}
