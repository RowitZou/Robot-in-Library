#include "DC_Drive_Wheels.h"
#include "infrared_adc.h"
#include "delay.h"
#include "move.h"
#define FRONT 0
#define BACK  1
#define LEFT  2
#define RIGHT 3

void move(void){
   
	int fo=0,fi=0,bo=0,bi=0,lo=0,li=0,ro=0,ri=0;
	int state = FRONT;
	ALL_Stop();
	
	while(1){
	   
		/* fo= Test_Line(_F_O);
	     fi= Test_Line(_F_I);
		 bo= Test_Line(_B_O);
		 bi= Test_Line(_B_I);
	     lo= Test_Line(_L_O);
		 li= Test_Line(_L_I);
	     ro= Test_Line(_R_O);
	     ri= Test_Line(_R_I);*/
	
		switch (state){
		  case	FRONT: {
		  lo= Test_LineOut(_L_O);
		  ro= Test_LineOut(_R_O);
			  
		//转向控制，待优化
		  if(lo)                     {state=LEFT; B_Straight(); delay_ms(350);L_Straight(); delay_ms(500);break;}
		  else if(ro)                {state=RIGHT;B_Straight(); delay_ms(350);R_Straight();  delay_ms(500);break;}
		  TIM_SetCompare1(TIM4,1000);         //PWM脉冲占空比调节
	      TIM_SetCompare2(TIM4,1000);
		  TIM_SetCompare3(TIM4,800);
	      TIM_SetCompare4(TIM4,800);
		  fi= Test_LineIn(_F_I);
		  bi= Test_LineIn(_B_I);
		  li= Test_LineIn(_L_I);
		  ri= Test_LineIn(_R_I);
		  if(((!fi)&&(!bi)&&(!li)&&(!ri))||((!fi)&&(!bi)&&li&&ri))        {ALL_Stop();} 
		  else if((ri&&(!fi)&&(!bi)&&(!li))||(ri&&bi&&(!fi)&&(!li))||(li&&fi&&(!ri)&&(!bi)))       {F_Turn_Right();}
		  else if((li&&(!fi)&&(!bi)&&(!ri))||(li&&bi&&(!fi)&&(!ri))||(fi&&ri&&(!li)&&(!bi)))       {F_Turn_Left();}
		  else                       {F_Straight();}
		  break;
		 }
		 
		 case BACK: {
		  lo= Test_LineOut(_L_O);
		  ro= Test_LineOut(_R_O);
		  if(ro)                     {state=RIGHT;F_Straight(); delay_ms(350); R_Straight(); delay_ms(500);break;}
		  else if(lo)                {state=LEFT; F_Straight(); delay_ms(350);L_Straight(); delay_ms(500);break;}
		  TIM_SetCompare1(TIM4,1000);
	      TIM_SetCompare2(TIM4,1000);
		  TIM_SetCompare3(TIM4,800);
	      TIM_SetCompare4(TIM4,800);
		  fi= Test_LineIn(_F_I);
		  bi= Test_LineIn(_B_I);
		  li= Test_LineIn(_L_I);
		  ri= Test_LineIn(_R_I);
		  if(((!fi)&&(!bi)&&(!li)&&(!ri))||((!fi)&&(!bi)&&li&&ri))        {ALL_Stop();} 
		  else if((li&&(!fi)&&(!bi)&&(!ri))||(li&&fi&&(!bi)&&(!ri))||(bi&&ri&&(!li)&&(!fi)))       {B_Turn_Right();}
		  else if((ri&&(!fi)&&(!bi)&&(!li))||(ri&&fi&&(!bi)&&(!li))||(bi&&li&&(!ri)&&(!fi)))       {B_Turn_Left();}
		  else                       {B_Straight();}
		  break;
		 }
		  
		 case LEFT:{
		  fo= Test_LineOut(_F_O);
		  bo= Test_LineOut(_B_O);
		  if(bo)                     {state=BACK; R_Straight(); delay_ms(350);B_Straight(); delay_ms(500);break;}
		  else if(fo)                {state=FRONT;R_Straight(); delay_ms(350);F_Straight(); delay_ms(500);ALL_Stop();return;}
		  TIM_SetCompare1(TIM4,800);
	      TIM_SetCompare2(TIM4,800);
		  TIM_SetCompare3(TIM4,1000);
	      TIM_SetCompare4(TIM4,1000);
		  fi= Test_LineIn(_F_I);
		  bi= Test_LineIn(_B_I);
		  li= Test_LineIn(_L_I);
		  ri= Test_LineIn(_R_I);
		  if(((!fi)&&(!bi)&&(!li)&&(!ri))||((!li)&&(!ri)&&fi&&bi))        {ALL_Stop();} 
		  else if((fi&&(!ri)&&(!bi)&&(!li))||(fi&&ri&&(!bi)&&(!li))||(li&&bi&&(!ri)&&(!fi)))       {L_Turn_Right();}
		  else if((bi&&(!fi)&&(!li)&&(!ri))||(ri&&bi&&(!fi)&&(!li))||(fi&&li&&(!ri)&&(!bi)))       {L_Turn_Left();}
		  else                       {L_Straight();}
		  break;
		 }
		 
		 case RIGHT: {
		  fo= Test_LineOut(_F_O);
		  bo= Test_LineOut(_B_O);
		  if(fo)                     {state=FRONT; L_Straight(); delay_ms(350);F_Straight(); delay_ms(500);break;}
		  else if(bo)                {state=BACK;  L_Straight(); delay_ms(350);B_Straight(); delay_ms(500);break;}
		  TIM_SetCompare1(TIM4,800);
	      TIM_SetCompare2(TIM4,800);
		  TIM_SetCompare3(TIM4,1000);
	      TIM_SetCompare4(TIM4,1000);
		  fi= Test_LineIn(_F_I);
		  bi= Test_LineIn(_B_I);
		  li= Test_LineIn(_L_I);
		  ri= Test_LineIn(_R_I);
		  if(((!fi)&&(!bi)&&(!li)&&(!ri))||((!li)&&(!ri)&&fi&&bi))        {ALL_Stop();} 
		  else if((bi&&(!fi)&&(!ri)&&(!li))||(li&&bi&&(!fi)&&(!ri))||(ri&&fi&&(!li)&&(!bi)))       {R_Turn_Right();}
		  else if((fi&&(!bi)&&(!li)&&(!ri))||(li&&fi&&(!bi)&&(!ri))||(bi&&ri&&(!li)&&(!fi)))       {R_Turn_Left();}
		  else                       {R_Straight();}
		  break;
		 }
	 }
	}

}
