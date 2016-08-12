#ifndef   _RFID_H
#define   _RFID_H
#include  "sys.h"
#define UART4_LEN  			200  	//定义最大接收字节数 200
extern u8  UART4_RX_BUF[UART4_LEN]; //接收缓冲,最大UART4_LEN个字节
extern u8  BOOK[5][13];                    //射频卡标志码，加上书的信息，5本书，9B识别码，3B有效信息
//extern u8  UART4_TX_BUF[UART4_LEN]; //接收缓冲,最大UART4_LEN个字节
extern u16 UART4_RX_STA;         		//接收状态标记	
extern int UART4_RX_SUCCESS;            //接收成功为1，否则为0
void UART4_TX(u8 command1,u8 command2,u8 *UID);
void UART4_IRQHandler(void);
void read_books(void);
void rfid_init(void);
#endif
