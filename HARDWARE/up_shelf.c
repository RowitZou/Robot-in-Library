#include "up_shelf.h"
#include "Ste_Engine.h"
#include "Step_Motor.h"
#include "Ultrasonic_Distance.h"
#include "rfid.h"
#include "infrared_adc.h"
#include "sys.h"
#include "delay.h"
#include "DC_Drive_Wheels.h"
#include "crawler_belt.h"

void up_shelf(u8 info){
   
    switch((info>>4)&0x0f){
	 
		case 0x01:  
			{step_motor_move1(2000,4000,880,1);
             ste_engine();
				crawler_belt_move();
			 delay_ms(1500);
			 crawler_belt_stop();
			//	delay_ms(2000);
             step_motor_move1(2000,4000,880,0);
             break;
			}				//   
			
		case 0x02:  
			{step_motor_move1(2000,4000,430,1);
             ste_engine();
				crawler_belt_move();
			 delay_ms(1500);
			 crawler_belt_stop();
             step_motor_move1(2000,4000,430,0);
             break;
			}		
			
		case 0x03:                              //第三格
			{//step_motor_move1(600,1000,100,0);
             ste_engine();
				crawler_belt_move();
			 delay_ms(1500);
			 crawler_belt_stop();
             //step_motor_move1(600,1000,100,1);
             break;
			}		
			
		case 0x04:
			{step_motor_move1(2000,4000,410,0);
             ste_engine();
			 crawler_belt_move();
			 delay_ms(1500);
			 crawler_belt_stop();
				//delay_ms(3000);
             step_motor_move1(2000,4000,410,1);
             break;
			}		
			
		case 0x05:
			{step_motor_move1(2000,4000,870,0);
             ste_engine();
			 crawler_belt_move();
			 delay_ms(1500);
			 crawler_belt_stop();
			 //delay_ms(3000);
             step_motor_move1(2000,4000,870,1);
             break;
			}		
	
		default: break;
	}
	/*delay_ms(1000);
	ste_engine();
	crawler_belt_move();
	delay_ms(1500);
	crawler_belt_stop();*/
	switch(info&0x0f){
	
		case 0x02:{
		  
			step_motor_move2(2500,9000,1000,1);   //800HZ到4500HZ, 持续时间1000，正转
			delay_ms(500);
			 crawler_belt_move();
			 delay_ms(5500);
			 crawler_belt_stop();
			step_motor_move2(2500,9000,1000,0);
		    break;
		
		}
		
		case 0x03:{
		  
			step_motor_move2(2500,9000,6800,0);
			delay_ms(500);
			 crawler_belt_move();
			 delay_ms(5500);
			 crawler_belt_stop();
			step_motor_move2(2500,9000,6800,1);
		    break;
		
		}
		
		case 0x04:{
		  
			step_motor_move2(2500,9000,14800,0);
			delay_ms(500);
			 crawler_belt_move();
			 delay_ms(5500);
			 crawler_belt_stop();
			step_motor_move2(2500,9000,14800,1);
		    break;
		
		}
	
	}
	/*crawler_belt_move();
    delay_ms(6000);
	crawler_belt_stop();*/
	return ; 
}
