#ifndef __MYSPI_H
#define __MYSPI_H
#include "stm32f10x.h"                  // Device header

/******************************************************************************************/
void SPI1_Init(void);			 //��ʼ��SPI��
void SPI1_SetSpeed(u8 SpeedSet); //����SPI�ٶ�   
u8 SPI1_ReadWriteByte(u8 TxData);//SPI���߶�дһ���ֽ�
		 
#endif
