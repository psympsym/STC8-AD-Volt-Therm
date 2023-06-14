/*-------------------------------------------------------------------
/*      (C) Copyright 2021-2022, nbufe. All Rights Reserved 
/*-------------------------------------------------------------------
/* File name   : mcu_id.c
/* Description : 
/* Author      : nbufe
/*-------------------------------------------------------------------
/* 芯片型号:STC8A8K64D4
/* 钟说明:单片机片内高速24MHz时钟 波特率说明:9600b/s
/* Update note:
/* ---------   -----------   ----------------------------------------
/*   Date        Author                       Note
/* ---------   -----------   ----------------------------------------
/*  
/*
**/

/* 包含头文件 -------------------------------------------------------*/
#include "uart_printf.h"   

/* 私有类型定义 -----------------------------------------------------*/
/* None. */

/* 私有宏定义 -------------------------------------------------------*/
#define  SYSCLK    (24000000UL)    						//系统时钟频率值
#define  BAUDSET   (65536 - SYSCLK/9600/4)    //波特率设定与计算

/* 私有变量 ---------------------------------------------------------*/
/* None. */

/* 扩展变量 ---------------------------------------------------------*/
/* None. */

/* 私有函数原形 -----------------------------------------------------*/
void UART1_Init(void);                       //串口1初始化函数
void UART1_SendByte(uint8_t byte);           //串口1发送单字符数据函数
void UART1_SendString(const uint8_t *str); 	 //串口1发送字符串数据函数
char putchar(char ch);                		 //发送字符重定向函数

/* 函数体 -----------------------------------------------------------*/

/*
在使用单片机开发过程中，很多情况下都希望能够通过串口输出一些调试信息。
二种串口输出各种格式数据的方法：
一.自己完全编程串口函数，进行格式化各种可能要使用的数据；
二.比较简单的方法是使用库中的printf函数，这种方法虽然比较占用资源，但在很多情况下比较简单实用。
在使用printf函数之前需要注意亮点，一是调用头文件stdio.h，二是重定义putchar发送单个字符函数。
*/
/*
 * 功能：printf函数在格式化输出
 * 参数：str 要发送的字符串地址。
 * 返回：None.
*/
char putchar(char ch)	//发送字符重定向函数
{
    UART1_SendByte(ch);	//通过串口1发送数据
    return ch;
}

/*
 * 功能: 读取单片机ID号
 * 参数: None
 * 返回: None
 * 说明: None
 */
void Printf_Msg(void)
{
	;
}

/*
 * 功能: 串口1初始化函数
 * 参数: None
*/
void UART1_Init(void)   	//9600bps@24.000MHz
{
    SCON = 0x50;        	//8位数据,可变波特率
    AUXR |= 0x04;       	//定时器时钟1T模式
    AUXR |= 0x01;       	//串口1选择定时器2为波特率发生器
    T2L = BAUDSET;      	//设置定时初始值
    T2H = BAUDSET >> 8; 	//设置定时初始值
    AUXR |= 0x10;        	//定时器1开始计时
    RI = 0;
    TI = 0;      			//清除接收数据标志位和发送数据标志位
}

/**
 * 功能：串口发送一个字节函数，
 * 参数：byte 要发送的字节数据。
*/
void UART1_SendByte(uint8_t byte)
{
    SBUF = byte;
    while(!TI);
    TI = 0;
}

/**
 * 功能：串口发送字符串函数
 * 参数：str 要发送的字符串地址
*/
void UART1_SendString(const uint8_t *str)
{
    while(*str != '\0')
    {
        UART1_SendByte(*(str++));
    }
}

/*>>>>>>>>>> (C) COPYRIGHT NBUFE <<<<<< >>>>>> END OF FILE <<<<<<<<<<*/