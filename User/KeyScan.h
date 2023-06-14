#ifndef _KeyScan_H_
#define _KeyScan_H_

/* ---------------------------------- 包含头文件 --------------------------------- */
#include "STC8Ax_REG.h"

/* 私有类型定义 */
sbit key_sr1 = P5 ^ 1; 		/*!< K1键 引脚定义 */
sbit key_sr2 = P5 ^ 0; 		/*!< K2键 引脚定义 */
sbit key_sr3 = P5 ^ 2; 		/*!< K3键 引脚定义 */
sbit key_sr4 = P5 ^ 3; 		/*!< K4键 引脚定义 */

/* ---------------------------------- 扩展变量 ---------------------------------- */
extern uint8 flag;

void KeyScan();

#endif