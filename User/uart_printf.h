/*--------------------------------------------------------------------
/* File name   : xxx.h
/* Author      : nbufe
/* Copyright(C): 2021-2022 nbufe. All rights reserved.
/*--------------------------------------------------------------------
 */
#ifndef __MCU_ID_H_
#define __MCU_ID_H_

/* 包含头文件 -------------------------------------------------------*/
#include "STC8Ax_REG.h"
#include "STC8Ax_CONF.h"
#include "stdio.h"

/* 类型定义 ---------------------------------------------------------*/
/* None. */

/* 宏定义 -----------------------------------------------------------*/
/* None. */

/* 扩展变量 ---------------------------------------------------------*/
/* None. */

/* 函数声明 ---------------------------------------------------------*/
void Printf_Msg(void);
void UART1_Init(void);   //9600bps@24.000MHz
void UART1_SendByte(uint8_t byte);
void UART1_SendString(const uint8_t *str);

#endif /* __XXX_H_ */

/*>>>>>>>>>> (C) COPYRIGHT NBUFE <<<<<< >>>>>> END OF FILE <<<<<<<<<<*/

