/*-------------------------------------------------------------------
/*      (C) Copyright 2021-2022, nbufe. All Rights Reserved
/*-------------------------------------------------------------------
/* File name   : DLED.c
/* Description :
/* Author      : nbufe
/*-------------------------------------------------------------------
/* Update note:
/* ---------   -----------   ----------------------------------------
/*   Date        Author                       Note
/* ---------   -----------   ----------------------------------------
/*
/*
**/

/* ����ͷ�ļ� -------------------------------------------------------*/
#include "DigLED.h"
#include "STC8Ax_Delay.h"

/* ˽�����Ͷ��� -----------------------------------------------------*/
/* None. */

/* ˽�к궨�� -------------------------------------------------------*/
code const uint8_t DigDisplay_table[] = // ��׼�ֿ�
{
//   0     1     2     3     4    5     6     7     8     9     o     C
    0xFC, 0x60, 0xDA, 0xF2, 0x66,0xB6, 0xBE, 0xE0, 0xFE, 0xF6, 0xC6, 0x9C
};

#define DigShow_PORT 	P1
#define DigShow_COM 	P2

/* ˽�б��� ---------------------------------------------------------*/
unsigned char DisplayBuf[4] = { 0 };
unsigned char Display_index = 0;//��̬ɨ������ܵĲ������

/* ��չ���� ---------------------------------------------------------*/
/* None. */

/* ˽�к���ԭ�� -----------------------------------------------------*/
/* None. */

/* ������ -----------------------------------------------------------*/
/*
 * ����: ��������ܵĶ�̬ɨ�����
 * ����: None
 * ����: None
 * ˵��: 
 */
void DigLED_Display()
{
    DigShow_COM = 0xFF;
    DigShow_PORT = DisplayBuf[Display_index];
	DigShow_COM = _crol_(0xFE, Display_index);
    if (++Display_index > 3)
		Display_index = 0;    //1λ����
}

/*
 * ����: ��ָ��λ��ʾ��ֵ
 * ����: index λ��
 *       number ��ʾֵ
 *       dot С���� 1����ʾ  0������ʾ
 * ����: None
 * ˵��: 
 */
void DisplayLoad(unsigned char index, unsigned char number, unsigned char dot)
{
    DisplayBuf[index] = DigDisplay_table[number] | ( dot ? 0x01 : 0 ) ;		//С����Ϊ��7λ
}

/*>>>>>>>>>> (C) COPYRIGHT NBUFE <<<<<< >>>>>> END OF FILE <<<<<<<<<<*/
