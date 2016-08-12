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
#include "pwm.h"
#include "rfid.h"

void System_Init(void){

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(168);
	IWDG_Init();
	LED_Init();
	BEEP_Init();
	//uart_init(115200);
	DC_Drive_Wheels_Init();
	//Ultrasonic_Init();
	Infrared_Adc_Init();
    TIM4_PWM_Init(1000-1,84-1);
	rfid_init();
}
