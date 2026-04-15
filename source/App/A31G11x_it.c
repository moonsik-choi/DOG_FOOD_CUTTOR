/**
 *******************************************************************************
 * @file        A31G11x_it.c
 * @author      ABOV R&D Division
 * @brief       Interrupt Source File
 *
 * Copyright 2020 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/

#include "main.h"

/* Private typedef ---------------------------------------------------------- */
/* Private define ----------------------------------------------------------- */
/* Private macro ------------------------------------------------------------ */
/* Private define ----------------------------------------------------------- */
/* Private function prototypes ---------------------------------------------- */
/* Private variables -------------------------------------------------------- */


/******************************************************************************/
/*            Cortex M0+ Processor Exceptions Handlers                        */
/******************************************************************************/

/*-------------------------------------------------------------------------*//**
 * @brief         This function handles NMI exception.
 * @param         None
 * @return        None
 *//*-------------------------------------------------------------------------*/
void NMI_Handler( void )
{
}

/*-------------------------------------------------------------------------*//**
 * @brief         This function handles Hard Fault exception.
 * @param         None
 * @return        None
 *//*-------------------------------------------------------------------------*/
void HardFault_Handler( void )
{
   /* Go to infinite loop when Hard Fault exception occurs */
   while( 1 )
   {
   }
}

/*-------------------------------------------------------------------------*//**
 * @brief         This function handles SVCall exception
 * @param         None
 * @return        None
 *//*-------------------------------------------------------------------------*/
void SVC_Handler( void )
{
}

/*-------------------------------------------------------------------------*//**
 * @brief         This function handles PendSVC exception
 * @param         None
 * @return        None
 *//*-------------------------------------------------------------------------*/
void PendSV_Handler( void )
{
}

/*-------------------------------------------------------------------------*//**
 * @brief         This function handles SysTick Handler.
 * @param         None
 * @return        None
 *//*-------------------------------------------------------------------------*/
void SysTick_Handler( void )
{
}

/******************************************************************************/
/*                 A31G11x Peripherals Interrupt Handlers                     */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_A31G11x.s).                                                 */
/******************************************************************************/

extern uint32_t gu32LED_BR;

void LVI_Handler(void){}
void WUT_Handler(void){}
void WDT_Handler(void){}
void EINT0_Handler(void){}
void EINT1_Handler(void){}
void EINT2_Handler(void){}
void EINT3_Handler(void){}
void TIMER10_Handler(void)
{
	T1nMaInt_ClrFg(TIMER10);
	Flag_1ms = 1;
}
void TIMER11_Handler(void)
{
	T1nMaInt_ClrFg(TIMER11);
}
void TIMER12_Handler(void){}
void I2C0_Handler(void){}
void USART10_Handler(void){}
void WT_Handler(void){}
void TIMER30_Handler(void)
{
    if( ( HAL_TIMER3n_GetStatus_IT( ( TIMER3n_Type* )TIMER30 ) & TIMER3n_INT_PMATEN ) == TIMER3n_INT_PMATEN )
    {
        // gu32LED_BR : 0~100 (%) 사용
        uint32_t pdr  = TIMER30->PDR;               // 타이머 주기 카운트
        uint32_t duty = (gu32LED_BR * pdr) / 100U;  // % → 실제 카운트로 변환

        if (duty > pdr)
        {
            duty = pdr;                             // 안전하게 클리핑
        }

        TIMER30->ADR = duty;
        TIMER30->BDR = 0;
        TIMER30->CDR = 0;
    }
    
    HAL_TIMER3n_ClearStatus_IT( ( TIMER3n_Type* )TIMER30, 0x7F );
}void I2C1_Handler(void){}
void TIMER20_Handler(void){}
void TIMER21_Handler(void){}
void USART11_Handler(void){}
void ADC_Handler(void){}
void UART0_Handler(void){}
void UART1_Handler(void){}



