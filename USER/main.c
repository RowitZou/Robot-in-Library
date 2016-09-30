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
#include "rfid.h" 
#include "Ste_Engine.h"
#include "line_set.h"
#include "Step_Motor.h"
#include "crawler_belt.h"
#include "up_shelf.h"

int main(void)
{ 
	PATTEN mode;
	
	System_Init();  
	read_books();
	delay_ms(5000);   
    mode = line_set();	//放书时间
	TIM_SetCompare1(TIM4,900);         //PWM脉冲占空比调节
    TIM_SetCompare2(TIM4,900);
	F_Straight();
	delay_ms(1500);
	
	if(mode==PATTEN1) move_1();
	else if(mode == PATTEN2) move_2();
	else              move_3(); 
	//delay_ms(5000);
	//up_shelf(0x43);
	//Ultrasonic_Work();
	/*while(1){
	
		read_shelves();
		if(SHELF_CHECK){
		 	GPIO_SetBits(GPIOF,GPIO_Pin_8);
	delay_ms(30);
	GPIO_ResetBits(GPIOF,GPIO_Pin_8);
			break;
		
		}
	
	}*/
	//move_2();
	//ste_engine();
	//step_motor_move1(600,2000,50,0);
	/*TIM_SetCompare1(TIM4,800);
	TIM_SetCompare2(TIM4,800);
	F_Straight();
	delay_ms(3000);
	ALL_Stop();*/
	/*delay_ms(1000);
	L_Straight();
	delay_ms(5000);
	ALL_Stop();*/
	//step_motor_move2(2500,9000,14600,1);
	/*crawler_belt_move();
	delay_ms(4000);
	crawler_belt_stop();*/
	while(1){
	   GPIO_ResetBits(GPIOF,GPIO_Pin_9 | GPIO_Pin_10);
       delay_ms(500);
       GPIO_SetBits(GPIOF,GPIO_Pin_9 | GPIO_Pin_10);
	   delay_ms(500);
	}
}


