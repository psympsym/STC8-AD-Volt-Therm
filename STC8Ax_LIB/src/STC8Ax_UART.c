/*-------------------------------------------------------------------
/*      (C) Copyright 2021-2022, nbufe. All Rights Reserved 
/*-------------------------------------------------------------------
/* File name   : STC8Ax_UART.c
/* Description : STC8Ax ���ں�����
/* Author      : nbufe
/* Update date : 2021.12.15
/*-------------------------------------------------------------------
/* Update note:
/* ---------   -----------   ----------------------------------------
/*   Date        Author                       Note
/* ---------   -----------   ----------------------------------------
/*  
/*
**/

/* ����ͷ�ļ� -------------------------------------------------------*/
#include "STC8Ax_UART.h"

/* ˽�����Ͷ��� -----------------------------------------------------*/
/* None. */

/* ˽�к궨�� -------------------------------------------------------*/
/* None. */

/* ˽�б��� ---------------------------------------------------------*/
/* None. */

/* ��չ���� ---------------------------------------------------------*/
/* ����æ��־λ���Զ��壬��ֹ�മ�ڳ�ͻ�ı��� */
uint8_t UartBusyFlg = 0; //Busy flag of receive

/* ˽�к���ԭ�� -----------------------------------------------------*/
/* None. */

/* ������ -----------------------------------------------------------*/
/**
 * ���ܣ�����1��ʼ������
 * ������uartx  ���ڳ�ʼ���ṹ��������ʼ��ʱ�붨��þ�����������ַ���Ρ�
 * ���أ�SUCCESS ���سɹ���
 *       ERROR    ����ʧ�ܡ�
 * ˵����
 */
ErrorStatus UART1_Init(const UART_InitTypeDef* uartx)
{
    SM0 = uartx -> Mode >> 1, SM1 = uartx -> Mode;
    switch(uartx -> BRTGen)
    {
        case UART_BRT_TIM1:
        {
            TR1 = 0;        //Turn off timer0
            ET1 = 0;        //Ban timer0 interrupt
            AUXR &= 0xFE;   //Select timer 1 as baud rate generator
            TMOD &= 0x0F; 	//Timer 1 is fixed to 16 bit auto reload mode
            AUXR = (AUXR & 0xBF) | ( !uartx -> BRTMode << 6 );
            T1L = (65536UL - CONF_FOSC / (uartx -> BaudRate * 4 * (11 * (!(AUXR & 0x40)) + 1 )));
            T1H = (65536UL - CONF_FOSC / (uartx -> BaudRate * 4 * (11 * (!(AUXR & 0x40)) + 1 ))) >> 8;
            TR1 = 1;        //Turn on timer0
            break;
        }
        case UART_BRT_TIM2:
        {
            AUXR &= 0xEF;  //Turn off timer2
            IE2 &= 0xFB;   //Ban timer2 interrupt
            AUXR |= 0x01;  //Select timer2 as baud rate generator
            AUXR = (AUXR & 0xFB) | ( !uartx -> BRTMode << 2 );
            T2L = (65536UL - CONF_FOSC / (uartx -> BaudRate * 4 * (11 * (!(AUXR & 0x04)) + 1 )));
            T2H = (65536UL - CONF_FOSC / (uartx -> BaudRate * 4 * (11 * (!(AUXR & 0x04)) + 1 ))) >> 8;
            AUXR |= 0X10;  //Turn on timer2
            break;
        }
        default:
            return ERROR;
    }
    AUXR = (AUXR & 0xDF) | (uartx -> BRTDouble << 5);
    SCON = (SCON & 0xDF) | (uartx -> MulitComm << 5);
    SCON = (SCON & 0xEF) | (uartx -> RxEnable << 4);
    AUXR2 = (AUXR2 & 0xEF) | (uartx -> Relay << 4);
    UART1_CLEAR_BUSY_FLAG();
    return SUCCESS;
}

/**
 * ���ܣ������жϳ�ʼ��������
 * ������pri �ж����ȼ���
 *       run ʹ�ܿ���λ��
 * ���أ�SUCCESS ���سɹ���
 *       ERROR    ����ʧ�ܡ�
 * ˵����
 */
ErrorStatus NVIC_UART1_Init(eNVIC_Pri pri, BOOL run)
{
    ES = run;
    NVIC_UART1_PRI(pri);
    return SUCCESS;
}

/**
 * ���ܣ�����2��ʼ������
 * ������uartx  ���ڳ�ʼ���ṹ��������ʼ��ʱ�붨��þ�����������ַ���Ρ�
 * ���أ�SUCCESS ���سɹ���
 *       ERROR    ����ʧ�ܡ�
 * ˵����
 */
ErrorStatus UART2_Init(const UART_InitTypeDef* uartx)
{
    S2CON = (S2CON & 0x3F) | ((uartx -> Mode & 0x02) << 6);
    if(uartx -> BRTGen == UART_BRT_TIM2)
    {
        AUXR &= 0xEF;  //Turn off timer2
        IE2 &= 0xFB;   //Ban timer2 interrupt
        AUXR = (AUXR & 0xFB) | ( !uartx -> BRTMode << 2 );
        T2L = (65536UL - CONF_FOSC / (uartx -> BaudRate * 4 * (11 * (!(AUXR & 0x04)) + 1 )));
        T2H = (65536UL - CONF_FOSC / (uartx -> BaudRate * 4 * (11 * (!(AUXR & 0x04)) + 1 ))) >> 8;
        AUXR |= 0X10;  //Turn on timer2
    }
    else return ERROR;
    S2CON = (S2CON & 0xDF) | (uartx -> MulitComm << 5);
    S2CON = (S2CON & 0xEF) | (uartx -> RxEnable << 4);
    UART2_CLEAR_BUSY_FLAG();
    return SUCCESS;
}

/**
 * ���ܣ������жϳ�ʼ��������
 * ������pri �ж����ȼ���
 *       run ʹ�ܿ���λ��
 * ���أ�SUCCESS ���سɹ���
 *       ERROR    ����ʧ�ܡ�
 * ˵����
 */
ErrorStatus NVIC_UART2_Init(eNVIC_Pri pri, BOOL run)
{
    IE2 = (IE2 & 0xFE) | (run);
    NVIC_UART2_PRI(pri);
    return SUCCESS;
}

/**
 * ���ܣ�����3��ʼ������
 * ������uartx  ���ڳ�ʼ���ṹ��������ʼ��ʱ�붨��þ�����������ַ���Ρ�
 * ���أ�SUCCESS ���سɹ���
 *       ERROR    ����ʧ�ܡ�
 * ˵����
 */
ErrorStatus UART3_Init(const UART_InitTypeDef* uartx)
{
    S3CON = (S3CON & 0x3F) | ((uartx -> Mode & 0x02) << 6);
    switch(uartx -> BRTGen)
    {
        case UART_BRT_TIM2:
        {
            AUXR &= 0xEF;  //Turn off timer2
            IE2 &= 0xFB;   //Ban timer2 interrupt
            S3CON &= 0xBF;  //Select timer2 as baud rate generator
            AUXR = (AUXR & 0xFB) | ( !uartx -> BRTMode << 2 );
            T2L = (65536UL - CONF_FOSC / (uartx -> BaudRate * 4 * (11 * (!(AUXR & 0x04)) + 1 )));
            T2H = (65536UL - CONF_FOSC / (uartx -> BaudRate * 4 * (11 * (!(AUXR & 0x04)) + 1 ))) >> 8;
            AUXR |= 0X10;  //Turn on timer2
            break;
        }
        case UART_BRT_TIM3:
        {
            T4T3M &= 0xF7;  //Turn off timer3
            IE2 &= 0xDF;  //Ban timer3 interrupt
            S3CON |= 0x40;    //Select timer3 as baud rate generator
            T4T3M = (T4T3M & 0xFD) | ( !uartx -> BRTMode << 1 );
            T3L = (65536UL - CONF_FOSC / (uartx -> BaudRate * 4 * (11 * (!(T4T3M & 0x02)) + 1 )));
            T3H = (65536UL - CONF_FOSC / (uartx -> BaudRate * 4 * (11 * (!(T4T3M & 0x02)) + 1 ))) >> 8;
            T4T3M |= 0x08; //Turn on timer3
            break;
        }
        default:
            return ERROR;
    }
    S3CON = (S3CON & 0xDF) | (uartx -> MulitComm << 5);
    S3CON = (S3CON & 0xEF) | (uartx -> RxEnable << 4);
    UART3_CLEAR_BUSY_FLAG();
    return SUCCESS;
}

/**
 * ���ܣ������жϳ�ʼ��������
 * ������pri �ж����ȼ���
 *       run ʹ�ܿ���λ��
 * ���أ�SUCCESS ���سɹ���
 *       ERROR    ����ʧ�ܡ�
 * ˵����
 */
ErrorStatus NVIC_UART3_Init(BOOL run)
{
    IE2 = (IE2 & 0xF7) | (run << 3);
    return SUCCESS;
}

/**
 * ���ܣ�����4��ʼ������
 * ������uartx  ���ڳ�ʼ���ṹ��������ʼ��ʱ�붨��þ�����������ַ���Ρ�
 * ���أ�SUCCESS ���سɹ���
 *       ERROR    ����ʧ�ܡ�
 * ˵����
 */
ErrorStatus UART4_Init(const UART_InitTypeDef* uartx)
{
    S4CON = (S4CON & 0x3F) | ((uartx -> Mode & 0x02) << 6);
    switch(uartx -> BRTGen)
    {
        case UART_BRT_TIM2:
        {
            AUXR &= 0xEF;  //Turn off timer2
            IE2 &= 0xFB;   //Ban timer2 interrupt
            S4CON &= 0xBF;  //Select timer2 as baud rate generator
            AUXR = (AUXR & 0xFB) | ( !uartx -> BRTMode << 2 );
            T2L = (65536UL - CONF_FOSC / (uartx -> BaudRate * 4 * (11 * (!(AUXR & 0x04)) + 1 )));
            T2H = (65536UL - CONF_FOSC / (uartx -> BaudRate * 4 * (11 * (!(AUXR & 0x04)) + 1 ))) >> 8;
            AUXR |= 0X10;  //Turn on timer2
            break;
        }
        case UART_BRT_TIM4:
        {
            T4T3M &= 0x7F;  //Turn off timer3
            IE2 &= 0xBF;  //Ban timer4 interrupt
            S4CON |= 0x40;   //Select timer3 as baud rate generator
            T4T3M = (T4T3M & 0xDF) | ( !uartx -> BRTMode << 5 );
            T4L = (65536UL - CONF_FOSC / (uartx -> BaudRate * 4 * (11 * (!(T4T3M & 0x20)) + 1 )));
            T4H = (65536UL - CONF_FOSC / (uartx -> BaudRate * 4 * (11 * (!(T4T3M & 0x20)) + 1 ))) >> 8;
            T4T3M |= 0x80; //Turn on timer4
            break;
        }
        default:
            return ERROR;
    }
    S4CON = (S4CON & 0xDF) | (uartx -> MulitComm << 5);
    S4CON = (S4CON & 0xEF) | (uartx -> RxEnable << 4);
    UART4_CLEAR_BUSY_FLAG();
    return SUCCESS;
}

/**
 * ���ܣ������жϳ�ʼ��������
 * ������pri �ж����ȼ���
 *       run ʹ�ܿ���λ��
 * ���أ�SUCCESS ���سɹ���
 *       ERROR    ����ʧ�ܡ�
 * ˵����
 */
ErrorStatus NVIC_UART4_Init(BOOL run)
{
    IE2 = (IE2 & 0xEF) | (run << 4);
    return SUCCESS;
}


/**
 * ���ܣ����ڷ���һ���ֽں�����
 * ������dat Ҫ���͵��ֽ����ݡ�
 * ���أ�None.
 * ˵������Ҫ�����жϣ������жϷ�������������ͱ�־λ��
 */
void UARTx_SendByte(eUART_CH uartx , uint8_t dat)
{
		/* ���ڵȴ��Զ���æ��־λ */
		while(UartBusyFlg & 0x01<<uartx);
	
	  /* ���������Զ���æ��־λ */
		UartBusyFlg |= 0x01<<uartx;

		if(UART1 == uartx)	SBUF = dat;
		if(UART2 == uartx)	S2BUF = dat;
		if(UART3 == uartx)	S3BUF = dat;
		if(UART4 == uartx)	S4BUF = dat;	
}

/**
 * ���ܣ����ڷ����ַ�������
 * ������ str Ҫ���͵��ַ�����ַ��
 * ���أ�None.
 * ˵������Ҫ�����жϣ������жϷ�������������ͱ�־λ��
 */
void UARTx_SendString(eUART_CH uartx , const uint8_t *str)
{
    while(*str != '\0')
    {
        UARTx_SendByte(uartx , *(str++));
    }
}

/**
 * ���ܣ����ڷ������麯��
 * ������arry Ҫ���͵������ַ��
 *       len  Ҫ���͵����鳤�ȡ�
 * ���أ�None.
 * ˵������Ҫ�����жϣ������жϷ�������������ͱ�־λ��
 */
void UARTx_SendArray(eUART_CH uartx , const uint8_t *arry, uint16_t len)
{
    while(len--)
    {
        UARTx_SendByte(uartx , *(arry++));
    }
}

/*>>>>>>>>>> (C) COPYRIGHT NBUFE <<<<<< >>>>>> END OF FILE <<<<<<<<<<*/
