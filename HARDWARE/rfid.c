#include "rfid.h"
#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "beep.h"

//UART4 PC10 TX , PC11 RX, PC12 TX, PD2 RX
//PA13 14 15 UART4; PD3 4 5 UART5
u8  UART4_RX_BUF[UART4_LEN];
u8  BOOK[5][13]={0};
u16 UART4_RX_STA=0;
int UART4_RX_SUCCESS = 0;

void rfid_init(void){

	u32 bound = 19200;      //设置波特率
	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; 
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOD,GPIO_Pin_4|GPIO_Pin_3);
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_UART4); 
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_UART4); 
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource12,GPIO_AF_UART5);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource2,GPIO_AF_UART5);
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 |GPIO_Pin_12; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIOC,&GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOD,&GPIO_InitStructure); 

	USART_InitStructure.USART_BaudRate = bound;     //波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;  //停止位1
	USART_InitStructure.USART_Parity = USART_Parity_No;   //奇偶校验位0
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;  //收发模式
    USART_Init(UART4, &USART_InitStructure); 
	USART_Init(UART5, &USART_InitStructure); 
	
    USART_Cmd(UART4, ENABLE);  
	USART_Cmd(UART5, ENABLE); 
	
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);  //串口4接收中断

    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
	NVIC_Init(&NVIC_InitStructure);
	
}

void UART4_TX(u8 command1,u8 command2,u8 *UID){
	int i;
    if(command1==0x08&&command2==0x01){
		u8 UART4_TX_BUF[UART4_LEN]={0xaa,0xbb,6,0,0,0,0x08,0x01,49}; //ISO15693模式
		UART4_TX_BUF[9]=UART4_TX_BUF[4];
		for(i=5;i<=8;i++)  UART4_TX_BUF[9]^=UART4_TX_BUF[i];
		for(i=0;i<=9;i++){
		  USART_SendData(UART4,UART4_TX_BUF[i]);
	     while((UART4->SR&0X40)==0);
		}
	}
	
	if(command1==0x07&&command2==0x01){
		u8 UART4_TX_BUF[UART4_LEN]={0xaa,0xbb,6,0,0,0,0x07,0x01,0x03}; //两灯模式
		UART4_TX_BUF[9]=UART4_TX_BUF[4];
		for(i=5;i<=8;i++)  UART4_TX_BUF[9]^=UART4_TX_BUF[i];
		for(i=0;i<=9;i++){
		  USART_SendData(UART4,UART4_TX_BUF[i]);
	     while((UART4->SR&0X40)==0);
		}
	}
	if(command1==0x01&&command2==0x10){
		u8 UART4_TX_BUF[UART4_LEN]={0xaa,0xbb,5,0,0,0,0x01,0x10}; //单卡模式
		UART4_TX_BUF[8]=UART4_TX_BUF[4];
		for(i=5;i<=7;i++)  UART4_TX_BUF[8]^=UART4_TX_BUF[i];
		for(i=0;i<=8;i++){
		  USART_SendData(UART4,UART4_TX_BUF[i]);
	     while((UART4->SR&0X40)==0);
		}
	}
	if(command1==0x05&&command2==0x10){
		u8 UART4_TX_BUF[UART4_LEN]={0xaa,0xbb,16,0,0,0,0x05,0x10,0,0,0,0,0,0,0,0,0,0,3}; //读卡的前三个字节数据
		for(i=8;i<=16;i++){
			UART4_TX_BUF[i]=*UID;
			UID++;
		}
		UART4_TX_BUF[19]=UART4_TX_BUF[4];
		for(i=5;i<=18;i++)  {
		    if(UART4_TX_BUF[i]==0xaa)
				UART4_TX_BUF[19]^=0x00;
			UART4_TX_BUF[19]^=UART4_TX_BUF[i];
		}
		for(i=0;i<=19;i++){
		//  printf("%x ",UART4_TX_BUF[i]);
		  USART_SendData(UART4,UART4_TX_BUF[i]);
		  while((UART4->SR&0X40)==0);
		  if(i>=2&&UART4_TX_BUF[i]==0xaa){
			   USART_SendData(UART4,0x00);
			   while((UART4->SR&0X40)==0);
		  }
		}
		//printf("\r\n");
	}
}

void read_books(void){

	int num=0,i,j;
  GPIO_SetBits(GPIOD,GPIO_Pin_3);
  delay_ms(100);
  while(!UART4_RX_SUCCESS){                    //启动读卡器的ISO15693模式
	  UART4_TX(0x08,0x01,NULL);
		delay_ms(50);
	}
  UART4_RX_SUCCESS = 0;
	
  while(num<3){              //一共5本书
	  
	while(!UART4_RX_SUCCESS){
	  UART4_TX(0x01,0x10,NULL);
	  delay_ms(50);
	}
	 UART4_RX_SUCCESS = 0;
	
	//确保同一本书不会多次扫描
	for(i=0;i<num;i++){
	   for(j=0;j<9;j++){
	     if(BOOK[i][j]!=UART4_RX_BUF[j+7])
			 break;
	   }
	   if(j>=9) break;
	}
	
   if(i>=num){
	for(i=0;i<9;i++){
	   BOOK[num][i]=UART4_RX_BUF[i+7];
	}  
	
	while(!UART4_RX_SUCCESS){
	  UART4_TX(0x05,0x10,BOOK[num]);
	  delay_ms(50);
	}
	 UART4_RX_SUCCESS = 0;
	
	GPIO_SetBits(GPIOF,GPIO_Pin_8);
	delay_ms(30);
	GPIO_ResetBits(GPIOF,GPIO_Pin_8);
	num++;
   /* for(i=0;i<(UART4_RX_STA&0x3fff);i++)
    	 printf("%x ",UART4_RX_BUF[i]);
	  printf("\r\n");*/
   }
  }
  
  GPIO_ResetBits(GPIOD,GPIO_Pin_3);
}

void UART4_IRQHandler(void){

	u8 Res;
    int i;
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)  
	{
		Res =USART_ReceiveData(UART4);
		
		if(Res == 0xaa) {
			UART4_RX_STA|=0x4000;                //标志接收到0xaa
			if(UART4_RX_STA&0x8000) {            //如果已经处于命令接收状态，记录0xaa
				UART4_RX_BUF[UART4_RX_STA&0X3FFF]=Res ;
                UART4_RX_STA++;		
			}                    				
		}
		else if(UART4_RX_STA&0x4000){            //上一个字节是0xaa
			if(Res == 0xbb){
		       UART4_RX_STA=0x8000;
			}
			else {
			   UART4_RX_STA&=0xbfff;
			}
		}
		
		else if(UART4_RX_STA&0x8000){
			UART4_RX_BUF[UART4_RX_STA&0X3FFF]=Res ;
            UART4_RX_STA++;	
			
			//接收完成处理数据
		    if((UART4_RX_BUF[0]+0x0002)<=(UART4_RX_STA&0x3fff)){
			    if((UART4_RX_STA&0x3fff)>=8&&UART4_RX_BUF[6]==0x00){            //状态字为成功
					UART4_RX_BUF[1]=UART4_RX_BUF[2];
				    for(i=3;i<=UART4_RX_BUF[0];i++){
						UART4_RX_BUF[1]^=UART4_RX_BUF[i];
				    }
					if(UART4_RX_BUF[1]==UART4_RX_BUF[(UART4_RX_STA&0x3fff)-1])  //校验字检验
						UART4_RX_SUCCESS = 1;
				}   
			}		
		}
  } 
}
