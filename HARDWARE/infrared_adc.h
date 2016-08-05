#ifndef __ADC_H
#define __ADC_H	
#include "sys.h" 
#define _F_O ADC_Channel_0  //前1     PA0
#define _F_I ADC_Channel_4  //前2     PA4
#define _B_O ADC_Channel_5  //后1     PA5
#define _B_I ADC_Channel_6  //后2     PA6
#define _L_O ADC_Channel_7  //左1     PA7
#define _L_I ADC_Channel_14  //左2    PC4
#define _R_O ADC_Channel_15 //右1     PC5
#define _R_I ADC_Channel_8 //右2      PB0

void Infrared_Adc_Init(void); 				
u16  Get_Adc(u8 ch); 				
float  Infrared_ScanIn(u8 ch);
float  Infrared_ScanOut(u8 ch);
int   Test_LineIn(u8 ch);
int   Test_LineOut(u8 ch);
#endif 















