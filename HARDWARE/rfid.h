#ifndef   _RFID_H
#define   _RFID_H
#include  "sys.h"
#define UART4_LEN  			200  	//�����������ֽ��� 200
extern u8  UART4_RX_BUF[UART4_LEN]; //���ջ���,���UART4_LEN���ֽ�
extern u8  BOOK[5][13];                    //��Ƶ����־�룬���������Ϣ��5���飬9Bʶ���룬3B��Ч��Ϣ
//extern u8  UART4_TX_BUF[UART4_LEN]; //���ջ���,���UART4_LEN���ֽ�
extern u16 UART4_RX_STA;         		//����״̬���	
extern int UART4_RX_SUCCESS;            //���ճɹ�Ϊ1������Ϊ0
void UART4_TX(u8 command1,u8 command2,u8 *UID);
void UART4_IRQHandler(void);
void read_books(void);
void rfid_init(void);
#endif
