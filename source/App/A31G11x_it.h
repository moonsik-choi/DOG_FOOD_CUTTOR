/**
 *******************************************************************************
 * @file        A31G11x_it.h
 * @author      ABOV R&D Division
 * @brief       Interrupt Header File
 *
 * Copyright 2020 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/

/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef __A31G11x_IT_H
#define __A31G11x_IT_H

/* Includes ----------------------------------------------------------------- */

#ifdef __cplusplus
extern "C" {
#endif

/* Exported types ----------------------------------------------------------- */
/* Exported constants ------------------------------------------------------- */
/* Exported macro ----------------------------------------------------------- */


/* Exported functions ------------------------------------------------------- */

void NMI_Handler( void );
void HardFault_Handler( void );
void MemManage_Handler( void );
void BusFault_Handler( void );
void UsageFault_Handler( void );
void SVC_Handler( void );
void DebugMon_Handler( void );
void PendSV_Handler( void );
void SysTick_Handler( void );

void LVI_Handler(void);
void WUT_Handler(void);
void WDT_Handler(void);
void EINT0_Handler(void);
void EINT1_Handler(void);
void EINT2_Handler(void);
void EINT3_Handler(void);
void TIMER10_Handler(void);
void TIMER11_Handler(void);
void TIMER12_Handler(void);
void I2C0_Handler(void);
void USART10_Handler(void);
void WT_Handler(void);
void TIMER30_Handler(void);
void I2C1_Handler(void);
void TIMER20_Handler(void);
void TIMER21_Handler(void);
void USART11_Handler(void);
void ADC_Handler(void);
void UART0_Handler(void);
void UART1_Handler(void);

#ifdef __cplusplus
}
#endif

#endif   /* __A31G11x_IT_H */

