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

/* 包含头文件 -------------------------------------------------------*/
#include "DigLED.h"
#include "STC8Ax_Delay.h"

/* 私有类型定义 -----------------------------------------------------*/
/* None. */

/* 私有宏定义 -------------------------------------------------------*/
code const uint8_t DigDisplay_table[] = // 标准字库
{
//   0     1     2     3     4    5     6     7     8     9     o     C
    0xFC, 0x60, 0xDA, 0xF2, 0x66,0xB6, 0xBE, 0xE0, 0xFE, 0xF6, 0xC6, 0x9C
};

#define DigShow_PORT 	P1
#define DigShow_COM 	P2

/* 私有变量 ---------------------------------------------------------*/
unsigned char DisplayBuf[4] = { 0 };
unsigned char Display_index = 0;//动态扫描数码管的步骤变量

/* 扩展变量 ---------------------------------------------------------*/
/* None. */

/* 私有函数原形 -----------------------------------------------------*/
/* None. */

/* 函数体 -----------------------------------------------------------*/
/*
 * 功能: 驱动数码管的动态扫描程序
 * 参数: None
 * 返回: None
 * 说明: 
 */
void DigLED_Display()
{
    DigShow_COM = 0xFF;
    DigShow_PORT = DisplayBuf[Display_index];
	DigShow_COM = _crol_(0xFE, Display_index);
    if (++Display_index > 3)
		Display_index = 0;    //1位结束
}

/*
 * 功能: 在指定位显示数值
 * 参数: index 位序
 *       number 显示值
 *       dot 小数点 1：显示  0：不显示
 * 返回: None
 * 说明: 
 */
void DisplayLoad(unsigned char index, unsigned char number, unsigned char dot)
{
    DisplayBuf[index] = DigDisplay_table[number] | ( dot ? 0x01 : 0 ) ;		//小数点为第7位
}

/*>>>>>>>>>> (C) COPYRIGHT NBUFE <<<<<< >>>>>> END OF FILE <<<<<<<<<<*/
