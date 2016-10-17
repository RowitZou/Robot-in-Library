#include "DC_Drive_Wheels.h"
#include "infrared_adc.h"
#include "delay.h"
#include "move.h"
#include "rfid.h"
#include "up_shelf.h"
#include "Ultrasonic_Distance.h"

int left_state = 0, back_state = 0;

void move_1(void){  //第一书架路线
   
	int fo=0,fi=0,bo=0,bi=0,lo=0,li=0,ro=0,ri=0;
	int state = FRONT;
	int front_Turn_right_enable=0;
	int back_Turn_left_enable=0;
	ALL_Stop();
	
	while(1){
	
		switch (state){
		  case	FRONT: {
		 // lo= Test_LineOut(_L_O);
		  ro= Test_LineOut(_R_O);
			  
		//转向控制，待优化
		  //if(lo)                     {/*state=LEFT; B_Straight(); delay_ms(300);L_Straight(); delay_ms(550);break;*/}
		  if(ro){
	        if(!front_Turn_right_enable){
			   F_Straight();
			   delay_ms(1000);
			   front_Turn_right_enable=1;
			}	
            else			
		    {state=RIGHT;B_Straight(); delay_ms(150);R_Straight();  delay_ms(800);break;}
		  }
		  TIM_SetCompare1(TIM4,900);         //PWM脉冲占空比调节
	      TIM_SetCompare2(TIM4,900);
		  TIM_SetCompare3(TIM4,900);
	      TIM_SetCompare4(TIM4,900);
		  fi= Test_LineIn(_F_I);
		  bi= Test_LineIn(_B_I);
		  li= Test_LineIn(_L_I);
		  ri= Test_LineIn(_R_I);
		  if((!fi)&&(!bi)&&(!li)&&(!ri))        {state = WRONG; break;} 
		  else if((ri&&(!fi)&&(!bi)&&(!li))||(ri&&bi&&(!fi)&&(!li))||(li&&fi&&(!ri)&&(!bi)))       {F_Turn_Right();}
		  else if((li&&(!fi)&&(!bi)&&(!ri))||(li&&bi&&(!fi)&&(!ri))||(fi&&ri&&(!li)&&(!bi)))       {F_Turn_Left();}
		  else                       {F_Straight();}
		  break;
		 }
		 
		 case BACK: {
		  lo= Test_LineOut(_L_O);
		 // ro= Test_LineOut(_R_O);
		  //if(ro)                     {/*state=RIGHT;F_Straight(); delay_ms(250); R_Straight(); delay_ms(550);ALL_Stop();return;*/}
		  if(lo){               
             if(!back_Turn_left_enable){
			   B_Straight();
			   delay_ms(1000);
			   back_Turn_left_enable=1;
			  }	
			 else
              {state=LEFT; left_state = 1; F_Straight(); delay_ms(150);L_Straight(); delay_ms(800);break;}
		  }
		  
		  TIM_SetCompare1(TIM4,900);
	      TIM_SetCompare2(TIM4,900);
		  TIM_SetCompare3(TIM4,900);
	      TIM_SetCompare4(TIM4,900);
		  fi= Test_LineIn(_F_I);
		  bi= Test_LineIn(_B_I);
		  li= Test_LineIn(_L_I);
		  ri= Test_LineIn(_R_I);
		  if((!fi)&&(!bi)&&(!li)&&(!ri))        {state = WRONG; break;} 
		  else if((li&&(!fi)&&(!bi)&&(!ri))||(li&&fi&&(!bi)&&(!ri))||(bi&&ri&&(!li)&&(!fi)))       {B_Turn_Right();}
		  else if((ri&&(!fi)&&(!bi)&&(!li))||(ri&&fi&&(!bi)&&(!li))||(bi&&li&&(!ri)&&(!fi)))       {B_Turn_Left();}
		  else                       {B_Straight();}
		  break;
		 }
		  
		 case LEFT:{
		  
		  fo= Test_LineOut(_F_O);
		  //bo= Test_LineOut(_B_O);
		 // if(bo)                     {/*state=BACK; R_Straight(); delay_ms(350);B_Straight(); delay_ms(550);break;*/}
		 if(fo)                {ALL_Stop();return;}
		  TIM_SetCompare1(TIM4,900);
	      TIM_SetCompare2(TIM4,900);
		  TIM_SetCompare3(TIM4,900);
	      TIM_SetCompare4(TIM4,900);
		  fi= Test_LineIn(_F_I);
		  bi= Test_LineIn(_B_I);
		  li= Test_LineIn(_L_I);
		  ri= Test_LineIn(_R_I);
		  if((!fi)&&(!bi)&&(!li)&&(!ri))        {state = WRONG; break;} 
		  else if((fi&&(!ri)&&(!bi)&&(!li))||(fi&&ri&&(!bi)&&(!li))||(li&&bi&&(!ri)&&(!fi)))       {L_Turn_Right();}
		  else if((bi&&(!fi)&&(!li)&&(!ri))||(ri&&bi&&(!fi)&&(!li))||(fi&&li&&(!ri)&&(!bi)))       {L_Turn_Left();}
		  else                       {L_Straight();}
		  break;
		 }
		 
		 case RIGHT: {
		   
		  read_shelves();
		  if(SHELF_CHECK){
		    R_Straight();
			
			  if(SHELF_CHECK==0xff){
				delay_ms(1200);
				SHELF_CHECK = 0;
			    SHELF_CHECK_1 = 0;
			    SHELF_CHECK_2 = 0;
			} 
			else{
		   delay_ms(2000);
			ALL_Stop();	
			Ultrasonic_Work();
			up_shelf(SHELF_CHECK);
            if(SHELF_CHECK_1)
               up_shelf(SHELF_CHECK_1);
            if(SHELF_CHECK_2)
               up_shelf(SHELF_CHECK_2);				
			B_Straight();
	        while(!Test_LineIn(_B_I));
	        ALL_Stop();
			//delay_ms(2000);
			SHELF_CHECK = 0;
			SHELF_CHECK_1 = 0;
			SHELF_CHECK_2 = 0;
		  }
	  }
		 // fo= Test_LineOut(_F_O);
		  bo= Test_LineOut(_B_O);
		  //if(fo)                     {/*state=FRONT; L_Straight(); delay_ms(350);F_Straight(); delay_ms(550);break;*/}
		 if(bo)                {state=BACK; back_state = 1; L_Straight(); delay_ms(150);B_Straight(); delay_ms(800);break;}
		  TIM_SetCompare1(TIM4,900);
	      TIM_SetCompare2(TIM4,900);
		  TIM_SetCompare3(TIM4,900);
	      TIM_SetCompare4(TIM4,900);
		  fi= Test_LineIn(_F_I);
		  bi= Test_LineIn(_B_I);
		  li= Test_LineIn(_L_I);
		  ri= Test_LineIn(_R_I);
		  if((!fi)&&(!bi)&&(!li)&&(!ri))        {state = WRONG; break;} 
		  else if((bi&&(!fi)&&(!ri)&&(!li))||(li&&bi&&(!fi)&&(!ri))||(ri&&fi&&(!li)&&(!bi)))       {R_Turn_Right();}
		  else if((fi&&(!bi)&&(!li)&&(!ri))||(li&&fi&&(!bi)&&(!ri))||(bi&&ri&&(!li)&&(!fi)))       {R_Turn_Left();}
		  else                       {R_Straight();}
		  break;
		 }
		 case WRONG:{
		   
			 ALL_Stop();
			 fo = Test_LineOut(_F_O);
			 bo = Test_LineOut(_B_O);
			 lo = Test_LineOut(_L_O);
			 ro = Test_LineOut(_R_O);
		 if((fo||bo)&&(!lo)&&(!ro)){
		     if(back_state)  state = BACK;
			 else state = FRONT;
		 }
		 
		 else if((!fo)&&(!bo)&&(lo&&ro)){
		     if(left_state)  state = LEFT;
		     else    state = RIGHT;
		 }
		 break;
		 }
	 }
	}

}

void move_2(void){  //第二书架路线
   
	int fo=0,fi=0,bo=0,bi=0,lo=0,li=0,ro=0,ri=0;
	int state = FRONT;
	ALL_Stop();
	
	while(1){
	
		switch (state){
		  case	FRONT: {
		  lo= Test_LineOut(_L_O);
		  ro= Test_LineOut(_R_O);
			  
		//转向控制，待优化
		  if(lo)                     {/*state=LEFT; B_Straight(); delay_ms(300);L_Straight(); delay_ms(550);break;*/}
		  else if(ro)                {state=RIGHT;B_Straight(); delay_ms(150);R_Straight();  delay_ms(800);break;}
		  
		  TIM_SetCompare1(TIM4,900);         //PWM脉冲占空比调节
	      TIM_SetCompare2(TIM4,900);
		  TIM_SetCompare3(TIM4,900);
	      TIM_SetCompare4(TIM4,900);
		  fi= Test_LineIn(_F_I);
		  bi= Test_LineIn(_B_I);
		  li= Test_LineIn(_L_I);
		  ri= Test_LineIn(_R_I);
		  if((!fi)&&(!bi)&&(!li)&&(!ri))        {state = WRONG; break;} 
		  else if((ri&&(!fi)&&(!bi)&&(!li))||(ri&&bi&&(!fi)&&(!li))||(li&&fi&&(!ri)&&(!bi)))       {F_Turn_Right();}
		  else if((li&&(!fi)&&(!bi)&&(!ri))||(li&&bi&&(!fi)&&(!ri))||(fi&&ri&&(!li)&&(!bi)))       {F_Turn_Left();}
		  else                       {F_Straight();}
		  break;
		 }
		 
		 case BACK: {
		  lo= Test_LineOut(_L_O);
		  ro= Test_LineOut(_R_O);
		  if(ro)                     {/*state=RIGHT;F_Straight(); delay_ms(250); R_Straight(); delay_ms(550);ALL_Stop();return;*/}
		  else if(lo)                {state=LEFT; left_state = 1;F_Straight(); delay_ms(150);L_Straight(); delay_ms(800);break;}
		  
		  TIM_SetCompare1(TIM4,900);
	      TIM_SetCompare2(TIM4,900);
		  TIM_SetCompare3(TIM4,900);
	      TIM_SetCompare4(TIM4,900);
		  fi= Test_LineIn(_F_I);
		  bi= Test_LineIn(_B_I);
		  li= Test_LineIn(_L_I);
		  ri= Test_LineIn(_R_I);
		  if((!fi)&&(!bi)&&(!li)&&(!ri))        {state = WRONG;break;} 
		  else if((li&&(!fi)&&(!bi)&&(!ri))||(li&&fi&&(!bi)&&(!ri))||(bi&&ri&&(!li)&&(!fi)))       {B_Turn_Right();}
		  else if((ri&&(!fi)&&(!bi)&&(!li))||(ri&&fi&&(!bi)&&(!li))||(bi&&li&&(!ri)&&(!fi)))       {B_Turn_Left();}
		  else                       {B_Straight();}
		  break;
		 }
		  
		 case LEFT:{
		  
		  fo= Test_LineOut(_F_O);
		  bo= Test_LineOut(_B_O);
		  if(bo)                     {/*state=BACK; R_Straight(); delay_ms(350);B_Straight(); delay_ms(550);break;*/}
		  else if(fo)                {ALL_Stop();return;}
		  TIM_SetCompare1(TIM4,900);
	      TIM_SetCompare2(TIM4,900);
		  TIM_SetCompare3(TIM4,900);
	      TIM_SetCompare4(TIM4,900);
		  fi= Test_LineIn(_F_I);
		  bi= Test_LineIn(_B_I);
		  li= Test_LineIn(_L_I);
		  ri= Test_LineIn(_R_I);
		  if((!fi)&&(!bi)&&(!li)&&(!ri))        {state = WRONG; break;} 
		  else if((fi&&(!ri)&&(!bi)&&(!li))||(fi&&ri&&(!bi)&&(!li))||(li&&bi&&(!ri)&&(!fi)))       {L_Turn_Right();}
		  else if((bi&&(!fi)&&(!li)&&(!ri))||(ri&&bi&&(!fi)&&(!li))||(fi&&li&&(!ri)&&(!bi)))       {L_Turn_Left();}
		  else                       {L_Straight();}
		  break;
		 }
		 
		 case RIGHT: {
		   
		  read_shelves();
		  if(SHELF_CHECK){
		    R_Straight();
			
			if(SHELF_CHECK==0xff){
				delay_ms(1000);
				SHELF_CHECK = 0;
			    SHELF_CHECK_1 = 0;
			    SHELF_CHECK_2 = 0;
			} 
			else{
		    delay_ms(2000);
			ALL_Stop();	
			Ultrasonic_Work();
			up_shelf(SHELF_CHECK);
            if(SHELF_CHECK_1)
               up_shelf(SHELF_CHECK_1);
            if(SHELF_CHECK_2)
               up_shelf(SHELF_CHECK_2);				
			B_Straight();
	        while(!Test_LineIn(_B_I));
	        ALL_Stop();
			//delay_ms(2000);
			SHELF_CHECK = 0;
			SHELF_CHECK_1 = 0;
			SHELF_CHECK_2 = 0;
		  }
	  }
		  fo= Test_LineOut(_F_O);
		  bo= Test_LineOut(_B_O);
		  if(bo)                     {/*state=FRONT; L_Straight(); delay_ms(350);F_Straight(); delay_ms(550);break;*/}
		  else if(fo)                {state=BACK; back_state = 1; L_Straight(); delay_ms(150);B_Straight(); delay_ms(800);break;}
		  TIM_SetCompare1(TIM4,900);
	      TIM_SetCompare2(TIM4,900);
		  TIM_SetCompare3(TIM4,900);
	      TIM_SetCompare4(TIM4,900);
		  fi= Test_LineIn(_F_I);
		  bi= Test_LineIn(_B_I);
		  li= Test_LineIn(_L_I);
		  ri= Test_LineIn(_R_I);
		  if((!fi)&&(!bi)&&(!li)&&(!ri))        {state = WRONG; break;} 
		  else if((bi&&(!fi)&&(!ri)&&(!li))||(li&&bi&&(!fi)&&(!ri))||(ri&&fi&&(!li)&&(!bi)))       {R_Turn_Right();}
		  else if((fi&&(!bi)&&(!li)&&(!ri))||(li&&fi&&(!bi)&&(!ri))||(bi&&ri&&(!li)&&(!fi)))       {R_Turn_Left();}
		  else                       {R_Straight();}
		  break;
		 }
		 case WRONG:{
		   
			 ALL_Stop();
			 fo = Test_LineOut(_F_O);
			 bo = Test_LineOut(_B_O);
			 lo = Test_LineOut(_L_O);
			 ro = Test_LineOut(_R_O);
		 if((fo||bo)&&(!lo)&&(!ro)){
		     if(back_state)  state = BACK;
			 else state = FRONT;
		 }
		 
		 else if((!fo)&&(!bo)&&(lo||ro)){
		     if(left_state)  state = LEFT;
		     else    state = RIGHT;
		 }
		 break;
		 }
	 }
	}

}

void move_3(void){  //P字形路线
   
	int fo=0,fi=0,bo=0,bi=0,lo=0,li=0,ro=0,ri=0;
	int state = FRONT;
	int front_Turn_right_enable=0;
	ALL_Stop();
    TIM_SetCompare1(TIM4,900);         //PWM脉冲占空比调节
	TIM_SetCompare2(TIM4,900);
    TIM_SetCompare3(TIM4,900);
	TIM_SetCompare4(TIM4,900);
	
	while(1){
	
		switch (state){
		  case	FRONT: {
		 // lo= Test_LineOut(_L_O);
		  ro= Test_LineOut(_R_O);
			  
		//转向控制，待优化
		 // if(lo)                     {}
		  if(ro){
	        if(!front_Turn_right_enable){
			   F_Straight();
			   delay_ms(1000);
			   front_Turn_right_enable=1;
			}	
            else			
		    {state=RIGHT;B_Straight(); delay_ms(150);R_Straight();  delay_ms(800);break;}
		  }
		  fi= Test_LineIn(_F_I);
		  bi= Test_LineIn(_B_I);
		  li= Test_LineIn(_L_I);
		  ri= Test_LineIn(_R_I);
		  if((!fi)&&(!bi)&&(!li)&&(!ri))        {state = WRONG;break;} 
		  else if((ri&&(!fi)&&(!bi)&&(!li))||(ri&&bi&&(!fi)&&(!li))||(li&&fi&&(!ri)&&(!bi)))       {F_Turn_Right();}
		  else if((li&&(!fi)&&(!bi)&&(!ri))||(li&&bi&&(!fi)&&(!ri))||(fi&&ri&&(!li)&&(!bi)))       {F_Turn_Left();}
		  else                       {F_Straight();}
		  break;
		 }
		 
		 case BACK: {
		  lo= Test_LineOut(_L_O);
		  ro= Test_LineOut(_R_O);
		  if(ro){
              if(left_state){
		        ALL_Stop();
		        return;
			  }
		  }
		  else if(lo){
			 if(!left_state){
		      state=LEFT; 
			  left_state = 1;
		      F_Straight(); 
		      delay_ms(150);
		      L_Straight(); 
		      delay_ms(800);
		      break;
			 }
		  }
		  fi= Test_LineIn(_F_I);
		  bi= Test_LineIn(_B_I);
		  li= Test_LineIn(_L_I);
		  ri= Test_LineIn(_R_I);
		  if((!fi)&&(!bi)&&(!li)&&(!ri)){
		     state = WRONG;
			  break;
		  } 
		  else if((li&&(!fi)&&(!bi)&&(!ri))||(li&&fi&&(!bi)&&(!ri))||(bi&&ri&&(!li)&&(!fi)))       {B_Turn_Right();}
		  else if((ri&&(!fi)&&(!bi)&&(!li))||(ri&&fi&&(!bi)&&(!li))||(bi&&li&&(!ri)&&(!fi)))       {B_Turn_Left();}
		  else                       {B_Straight();}
		  break;
		 }
		  
		 case LEFT:{
		  
		  read_shelves();
          if(SHELF_CHECK){
		    L_Straight();

			if(SHELF_CHECK==0xff){
			    delay_ms(1000);
				SHELF_CHECK = 0;
			    SHELF_CHECK_1 = 0;
			    SHELF_CHECK_2 = 0;
			} 
			else{
			delay_ms(2000);
			ALL_Stop();	
			Ultrasonic_Work();
			up_shelf(SHELF_CHECK);
            if(SHELF_CHECK_1)
               up_shelf(SHELF_CHECK_1);
            if(SHELF_CHECK_2)
               up_shelf(SHELF_CHECK_2);				
			B_Straight();
	        while(!Test_LineIn(_B_I));
	        ALL_Stop();
			//delay_ms(2000);
			SHELF_CHECK = 0;
			SHELF_CHECK_1 = 0;
			SHELF_CHECK_2 = 0;
		   }
		  }
		  //fo= Test_LineOut(_F_O);
		  bo= Test_LineOut(_B_O);
		  if(bo){ 
		    state=BACK; 
		    R_Straight(); 
		    delay_ms(150);
		    B_Straight();
   		    delay_ms(800);
		    break;
		  }
		 // else if(fo)                {}
		  fi= Test_LineIn(_F_I);
		  bi= Test_LineIn(_B_I);
		  li= Test_LineIn(_L_I);
		  ri= Test_LineIn(_R_I);
		  if((!fi)&&(!bi)&&(!li)&&(!ri)){
		     state = WRONG;
			  break;
		  } 
		  else if((fi&&(!ri)&&(!bi)&&(!li))||(fi&&ri&&(!bi)&&(!li))||(li&&bi&&(!ri)&&(!fi)))       {L_Turn_Right();}
		  else if((bi&&(!fi)&&(!li)&&(!ri))||(ri&&bi&&(!fi)&&(!li))||(fi&&li&&(!ri)&&(!bi)))       {L_Turn_Left();}
		  else                       {L_Straight();}
		  break;
		 }
		 
		 case RIGHT: {
			 
		  read_shelves();
		  if(SHELF_CHECK){
		    R_Straight();
			
			if(SHELF_CHECK==0xff){
				delay_ms(1000);
				SHELF_CHECK = 0;
			    SHELF_CHECK_1 = 0;
			    SHELF_CHECK_2 = 0;
			} 
			else{
			delay_ms(2000);
			ALL_Stop();	
			Ultrasonic_Work();
			up_shelf(SHELF_CHECK);
            if(SHELF_CHECK_1)
               up_shelf(SHELF_CHECK_1);
            if(SHELF_CHECK_2)
               up_shelf(SHELF_CHECK_2);				
			B_Straight();
	        while(!Test_LineIn(_B_I));
	        ALL_Stop();
			//delay_ms(2000);
			SHELF_CHECK = 0;
			SHELF_CHECK_1 = 0;
			SHELF_CHECK_2 = 0;
		    }
		  }
		  
		  //fo= Test_LineOut(_F_O);
		  bo= Test_LineOut(_B_O);
		  //if(fo)                     {/*state=FRONT; L_Straight(); delay_ms(350);F_Straight(); delay_ms(550);break;*/}
		  if(bo){
			 state=BACK;  
			 back_state = 1;
		     L_Straight(); 
		     delay_ms(150);
		     B_Straight(); 
		     delay_ms(800);
		     break;
		  }
		  fi= Test_LineIn(_F_I);
		  bi= Test_LineIn(_B_I);
		  li= Test_LineIn(_L_I);
		  ri= Test_LineIn(_R_I);
		  if((!fi)&&(!bi)&&(!li)&&(!ri)){
		     state = WRONG;
			  break;
		  } 
		  else if((bi&&(!fi)&&(!ri)&&(!li))||(li&&bi&&(!fi)&&(!ri))||(ri&&fi&&(!li)&&(!bi)))       {R_Turn_Right();}
		  else if((fi&&(!bi)&&(!li)&&(!ri))||(li&&fi&&(!bi)&&(!ri))||(bi&&ri&&(!li)&&(!fi)))       {R_Turn_Left();}
		  else                       {R_Straight();}
		  break;
		 }
		 case WRONG:{
		   
			 ALL_Stop();
			 fo = Test_LineOut(_F_O);
			 bo = Test_LineOut(_B_O);
			 lo = Test_LineOut(_L_O);
			 ro = Test_LineOut(_R_O);
		 if((fo&&bo)&&(!lo)&&(!ro)){
		     if(back_state)  state = BACK;
			 else state = FRONT;
		 }
		 
		 else if((!fo)&&(!bo)&&(lo||ro)){
		     if(left_state)  state = LEFT;
		     else    state = RIGHT;
		 }
		 break;
		 }
	 }
	}

}
