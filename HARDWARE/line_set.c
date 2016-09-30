#include "line_set.h"
#include "rfid.h"
 
PATTEN line_set(void){
   int i;
   PATTEN patten=0;
	
	//确定路线的模式，共3个模式
	for (i=0;i<5;i++){
	   if(BOOK[i][9]==0x01){
	      if(patten==PATTEN2||patten==PATTEN3)  patten=PATTEN3;
          else                                  patten=PATTEN1;		   
	   }
	   else if(BOOK[i][9]==0x02){
	      if(patten==PATTEN1||patten==PATTEN3) patten= PATTEN3;     
	      else                                  patten= PATTEN2;
	   }
	}
	
	//确定在哪个书架停下，标签号存入BOOK[i][12]
    for(i=0;i<5;i++){
	   if(BOOK[i][9]==0x01){
	       switch(BOOK[i][11]){
			   case 0x01: {BOOK[i][12]=0x05;break;}
		       case 0x02: {BOOK[i][12]=0x06;break;}
			   case 0x03: {BOOK[i][12]=0x07;break;}
			   case 0x04: {BOOK[i][12]=0x08;break;}
		   }
	   }   
	  else if(BOOK[i][9]==0x02){
	       switch (BOOK[i][11]){
		       case 0x01: {
			      if(patten==PATTEN2) {BOOK[i][12]=0x00;break;}
				  else                {BOOK[i][12]=0x01;break;}
			   }
		       case 0x02: {
			      if(patten==PATTEN2) {BOOK[i][12]=0x01;break;}
				  else                {BOOK[i][12]=0x02;break;}
			   }
			   case 0x03: {
			      if(patten==PATTEN2) {BOOK[i][12]=0x02;break;}
				  else                {BOOK[i][12]=0x03;break;}
			   }
		       case 0x04: {
			      if(patten==PATTEN2) {BOOK[i][12]=0x03;break;}
				  else                {BOOK[i][12]=0x04;break;}
			   }
		   }
	  }
	
	}

  return patten;
}
