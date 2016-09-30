#ifndef   _RFID_H
#define   _RFID_H
#include  "sys.h"
#define UART4_LEN  			200  	//�����������ֽ��� 200
#define UART5_LEN  			200  	//�����������ֽ��� 200
extern u8  UART4_RX_BUF[UART4_LEN]; //���ջ���,���UART4_LEN���ֽ�
extern u8  UART5_RX_BUF[UART5_LEN];
extern u8  BOOK[5][13];                    //��Ƶ����־�룬���������Ϣ��5���飬9Bʶ���룬3B��Ч��Ϣ
extern u8  SHELF[10];                     //Ѳ���ϵ���Ƶ����9Bʶ���룬3B��Ч��Ϣ
//extern u8  UART4_TX_BUF[UART4_LEN]; //���ջ���,���UART4_LEN���ֽ�
extern u16 UART4_RX_STA;         		//����״̬���	UART4
extern u16 UART5_RX_STA;                //����״̬��� UART5
extern int UART4_RX_SUCCESS;            //���ճɹ�Ϊ1������Ϊ0
extern int UART5_RX_SUCCESS;            //���ճɹ�Ϊ1������Ϊ0
extern u8 SHELF_CHECK;
extern u8 SHELF_CHECK_1;
extern u8 SHELF_CHECK_2;
void UART4_TX(u8 command1,u8 command2,u8 *UID);
void UART5_TX(u8 command1,u8 command2,u8 *UID);
void UART5_IRQHandler(void);
void UART4_IRQHandler(void);
void read_shelves(void);
void read_books(void);
void rfid_init(void);
#endif
