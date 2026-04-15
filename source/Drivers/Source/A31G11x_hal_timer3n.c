/**
 *******************************************************************************
 * @file        A31G11x_hal_timer3n.c
 * @author      ABOV R&D Division
 * @brief       3N typed Timer
 *
 * Copyright 2020 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/

/* Includes ----------------------------------------------------------------- */
//******************************************************************************
// Include
//******************************************************************************

#include "A31G11x_hal_timer3n.h"

/* Public Functions --------------------------------------------------------- */
//******************************************************************************
// Function
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         Initialize the TIMER3n peripheral with the specified parameters.
 * @param[in]     TIMER3x
 *                   Pointer to the target TIMER3
 *                   -  TIMER30
 * @param[in]     TIMER3n_Config
 *                   Pointer to the configuration information for the specified peripheral.
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_TIMER3n_Init( TIMER3n_Type* TIMER3x, TIMER3n_CFG_Type* TIMER3n_Config )
{
   /* Check TIMER3 handle */
   if( TIMER3x == NULL )
   {
      return HAL_ERROR;
   }

#if 1    // supported
   if( TIMER3x == ( TIMER3n_Type* )TIMER30 )
   {
      // enable peripheral clock
      SCUCG->PPCLKEN1_b.T30CLKE = 1;
   }
#endif

#if 0    // not supported
   if( TIMER3x == ( TIMER3n_Type* )TIMER31 )
   {
      // enable peripheral clock
      SCUCG->PPCLKEN1_b.T31CLKE = 1;
   }
#endif

   // CR
   TIMER3x->CR =  0
                  | ( TIMER3n_Config->T3nMS  << TIMER3n_CR_T3nMS_Pos )
                  | ( TIMER3n_Config->T3nCLK << TIMER3n_CR_T3nCLK_Pos )
                  | ( TIMER3n_Config->T3nECE << TIMER3n_CR_T3nECE_Pos )
                  | ( TIMER3n_Config->T3nCPOL << TIMER3n_CR_T3nCPOL_Pos )
                  ;

   // PDR
   TIMER3x->PDR = TIMER3n_Config->PDR;

   // ADR & BDR & CDR
   TIMER3x->ADR = TIMER3n_Config->ADR;
   TIMER3x->BDR = TIMER3n_Config->BDR;
   TIMER3x->CDR = TIMER3n_Config->CDR;

   // PREDR
   TIMER3x->PREDR = TIMER3n_Config->Prescaler;

   // clear counter and prescaler
   TIMER3n_ClearCounter( TIMER3x );

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Close Timer/Counter device
 * @param[in]     TIMER3x
 *                   Pointer to the target TIMER3
 *                   -  TIMER30
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_TIMER3n_DeInit( TIMER3n_Type* TIMER3x )
{
   /* Check TIMER2 handle */
   if( TIMER3x == NULL )
   {
      return HAL_ERROR;
   }

   // Disable timer/counter
   TIMER3x->CR_b.T3nEN = 0;

#if 1    // supported
   if( TIMER3x == ( TIMER3n_Type* )TIMER30 )
   {
      // disable peripheral clock
      SCUCG->PPCLKEN1_b.T30CLKE = 0;
   }
#endif

#if 0    // not supported
   if( TIMER3x == ( TIMER3n_Type* )TIMER31 )
   {
      // disable peripheral clock
      SCUCG->PPCLKEN1_b.T31CLKE = 0;
   }
#endif

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Interrupt Control Register
 * @param[in]     TIMER3x
 *                   Pointer to the target TIMER3
 *                   -  TIMER30
 * @param[in]     NewState
 *                   Next State of Interrupt Operation
 *                   -  ENABLE, DISABLE
 * @param[in]     TIMER3n_IntCfg
 *                   Interrupt Sourtce Setting
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_TIMER3n_ConfigInterrupt( TIMER3n_Type* TIMER3x, uint32_t NewState, uint32_t TIMER3n_IntCfg )
{
   /* Check TIMER3 handle */
   if( TIMER3x == NULL )
   {
      return HAL_ERROR;
   }

   if( NewState == ENABLE )
   {
      TIMER3x->INTCR |= TIMER3n_IntCfg;
   }
   else
   {
      TIMER3x->INTCR &= ( ~TIMER3n_IntCfg ) & 0x7f;
   }

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         TIMER3n PWM Mode Setting (Initial : Back to Back Mode, Internal Clock, 6channel Mode)
 * @param[in]     TIMER3x
 *                   Pointer to the target TIMER3
 *                   -  TIMER30
 * @param[in]     updatedata
 *                   PWM Duty Update Timing
 * @param[in]     intcount
 *                   Period Match Interrupt Occurence Seletion
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_TIMER3n_MPWMCmd( TIMER3n_Type* TIMER3x, uint32_t updatedata, uint32_t intcount )
{
   /* Check TIMER3 handle */
   if( TIMER3x == NULL )
   {
      return HAL_ERROR;
   }

   TIMER3x->CR = 0
                 | TIMER3n_CLKINT
                 | TIMER3n_INVM
                 | TIMER3n_6CHMOD
                 | updatedata
                 | intcount
                 | 1             // Clear TIMER3n & Precaler
                 ;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Enable or Disable PWM start
 * @param[in]     TIMER3x
 *                   Pointer to the target TIMER3
 *                   -  TIMER30
 * @param[in]     NewState
 *                   Next State of Functional Operation
 *                   -  ENABLE, DISABLE
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_TIMER3n_Start( TIMER3n_Type* TIMER3x, uint32_t NewState )
{
   /* Check TIMER3 handle */
   if( TIMER3x == NULL )
   {
      return HAL_ERROR;
   }

   if( NewState == TIMER3n_ENABLE )
   {
      TIMER3x->CR |= TIMER3n_ENABLE;
   }
   else
   {
      TIMER3x->CR &= ~TIMER3n_ENABLE;
   }

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         PWM Output Port Control Register (Initial : 6channel enable, output low)
 * @param[in]     TIMER3x
 *                   Pointer to the target TIMER3
 *                   -  TIMER30
 * @param[in]     NewState
 *                   Next State of Functional Operation
 *                   -  ENABLE, DISABLE
 * @param[in]     pwmApol
 *                   Timer 30xA Output Polarity Selection
 * @param[in]     pwmBpol
 *                   Timer 30xB Output Polarity Selection
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_TIMER3n_OutputCtrl( TIMER3n_Type* TIMER3x,  uint32_t NewState, uint32_t pwmApol, uint32_t pwmBpol )
{
   uint32_t    temp;

   /* Check TIMER3 handle */
   if( TIMER3x == NULL )
   {
      return HAL_ERROR;
   }

   temp = 0
          | ( ( uint32_t )TIMER30_T30_OUTCR_WTIDKY_Value << TIMER30_OUTCR_WTIDKY_Pos )
          | ( pwmApol | pwmBpol )
          ;
   if( NewState == ENABLE )
   {
      temp |= ( 0x3f << 8 );
   }

   TIMER3x->OUTCR = temp;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set Prescaler data.
 * @param[in]     TIMER3x
 *                   Pointer to the target TIMER3
 *                   -  TIMER30
 * @param[in]     prescale
 *                   Timer30 Prescaler Value
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_TIMER3n_ClockPrescaler( TIMER3n_Type* TIMER3x, uint32_t prescale )
{
   /* Check TIMER3 handle */
   if( TIMER3x == NULL )
   {
      return HAL_ERROR;
   }

   TIMER3x->PREDR = ( prescale & 0x0fff ); // period. it sould be larger than 0x10

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set period data.
 * @param[in]     TIMER3x
 *                   Pointer to the target TIMER3
 *                   -  TIMER30
 * @param[in]     period
 *                   MPWM period data
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_TIMER3n_SetPeriod( TIMER3n_Type* TIMER3x, uint32_t period )
{
   /* Check TIMER3 handle */
   if( TIMER3x == NULL )
   {
      return HAL_ERROR;
   }

   TIMER3x->PDR = ( period & 0xffff ); // period. it sould be larger than 0x10

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set duty A data.
 * @param[in]     TIMER3x
 *                   Pointer to the target TIMER3
 *                   -  TIMER30
 * @param[in]     aduty
 *                   Timer30 Aduty data
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_TIMER3n_SetADuty( TIMER3n_Type* TIMER3x, uint32_t aduty )
{
   /* Check TIMER3 handle */
   if( TIMER3x == NULL )
   {
      return HAL_ERROR;
   }

   TIMER3x->ADR = ( ( aduty ) & 0xffff ); // if using I/O control function, set period data

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set duty B data.
 * @param[in]     TIMER3x
 *                   Pointer to the target TIMER3
 *                   -  TIMER30
 * @param[in]     bduty
 *                   Timer30 Bduty data
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_TIMER3n_SetBDuty( TIMER3n_Type* TIMER3x, uint32_t bduty )
{
   /* Check TIMER3 handle */
   if( TIMER3x == NULL )
   {
      return HAL_ERROR;
   }

   TIMER3x->BDR = ( ( bduty ) & 0xffff ); // if using I/O control function, set period data

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set duty C data.
 * @param[in]     TIMER3x
 *                   Pointer to the target TIMER3
 *                   -  TIMER30
 * @param[in]     cduty
 *                   Timer30 Cduty data
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_TIMER3n_SetCDuty( TIMER3n_Type* TIMER3x, uint32_t cduty )
{
   /* Check TIMER3 handle */
   if( TIMER3x == NULL )
   {
      return HAL_ERROR;
   }

   TIMER3x->CDR = ( ( cduty ) & 0xffff ); // if using I/O control function, set period data

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set dead time (delay time)
 * @param[in]     TIMER3x
 *                   Pointer to the target TIMER3
 *                   -  TIMER30
 * @param[in]     dten
 *                   dead time enable
 * @param[in]     dtpos
 *                   dead timer position (front or back)
 * @param[in]     clkdata
 *                   dead time
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_TIMER3n_SetDelayTime( TIMER3n_Type* TIMER3x, uint32_t dten, uint32_t dtpos, uint32_t clkdata )
{
   uint32_t    temp;

   /* Check TIMER3 handle */
   if( TIMER3x == NULL )
   {
      return HAL_ERROR;
   }

   temp = TIMER3x->CR;
   temp &= ~( 0x03 << 8 );
   temp |= dten | dtpos;
   TIMER3x->CR = temp;

   TIMER3x->DLY = clkdata & 0x03ff;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set HIZCR Register
 * @param[in]     TIMER3x
 *                   Pointer to the target TIMER3
 *                   -  TIMER30
 * @param[in]     u32T30HizSet
 *                   Timer 30 Output High-Impedance Setting Data
 * @return        @ref HAL_Status_Type
 * @details       This function sets the Timer 30 HIZ/ADT Control Register
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_TIMER3n_SetHizReg( TIMER3n_Type* TIMER3x, uint32_t u32T30HizSet )
{
   /* Check TIMER3 handle */
   if( TIMER3x == NULL )
   {
      return HAL_ERROR;
   }

   TIMER3x->HIZCR = u32T30HizSet;  //Setting Timer 30 High-Impedance Control Register

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set ADC Tirgger Source & Timing
 * @param[in]     TIMER3x
 *                   Pointer to the target TIMER3
 *                   -  TIMER30
 * @param[in]     u32triggerpoint
 *                   Timer 30 Output High-Impedance Setting Data
 * @param[in]     u32triggertime
 *                   Timer 30 A/DC Trigger Setting Data
 * @return        @ref HAL_Status_Type
 * @details       This function sets the Timer 30 HIZ/ADT Control Register
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_TIMER3n_SetADCTrigger( TIMER3n_Type* TIMER3x, uint32_t u32triggerpoint, uint32_t u32triggertime )
{
   /* Check TIMER3 handle */
   if( TIMER3x == NULL )
   {
      return HAL_ERROR;
   }

   TIMER3x->ADTCR = u32triggerpoint;  //Setting Timer 30 A/DC Trigger Control Register
   TIMER3x->ADTDR = u32triggertime;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Interrupt Flag Clear
 * @param[in]     TIMER3x
 *                   Pointer to the target TIMER3
 *                   -  TIMER30
 * @param[in]     TIMER3n_IntCfg
 *                   Select clear interrupt
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_TIMER3n_ClearStatus_IT( TIMER3n_Type* TIMER3x, uint32_t TIMER3n_IntCfg )
{
   /* Check TIMER3 handle */
   if( TIMER3x == NULL )
   {
      return HAL_ERROR;
   }

   TIMER3x->INTFLAG = TIMER3n_IntCfg;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Get Interrupt Flag
 * @param[in]     TIMER3x
 *                   Pointer to the target TIMER3
 *                   -  TIMER30
 * @return        Interrupt Flag
 *//*-------------------------------------------------------------------------*/
uint32_t HAL_TIMER3n_GetStatus_IT( TIMER3n_Type* TIMER3x )
{
   return TIMER3x->INTFLAG;
}

