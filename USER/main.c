#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "beep.h"
#include "Ultrasonic_Distance.h"
#include "DC_Drive_Wheels.h"
#include "iwdg.h"
#include "infrared_adc.h"
#include "init.h"
#include "move.h"

int main(void)
{ 
	//float temp;
	System_Init();
	//Ultrasonic_Work();
	move();
/*	while(1){
	 
		Turn_Left();
	    delay_ms(10000);
		Turn_Right();
		delay_ms(10000);
		ALL_Stop();
	    break;
	
	}*/
	  
	while(1){
		
    GPIO_ResetBits(GPIOF,GPIO_Pin_9); 
	  delay_ms(100);
	GPIO_SetBits(GPIOF,GPIO_Pin_9);
	  delay_ms(100);
	 /* TIM_SetCompare1(TIM4,1999);
	  TIM_SetCompare2(TIM4,0);
	  TIM_SetCompare3(TIM4,1000);
	  TIM_SetCompare4(TIM4,1000);*/
		//temp=Infrared_Scan(_F_O);
		//printf("%f\n",temp);
		/*if(Test_Line(_F_O)){
			GPIO_ResetBits(GPIOF,GPIO_Pin_9);
		  GPIO_SetBits(GPIOF,GPIO_Pin_10 );
	  }*/
		/*else if(temp >= 1 && temp <= 2){
		  GPIO_ResetBits(GPIOF,GPIO_Pin_10);
      GPIO_SetBits(GPIOF,GPIO_Pin_9);			
		}*/
	/*	else{
		  GPIO_ResetBits(GPIOF,GPIO_Pin_9 | GPIO_Pin_10);
		}*/
		
	}
}


