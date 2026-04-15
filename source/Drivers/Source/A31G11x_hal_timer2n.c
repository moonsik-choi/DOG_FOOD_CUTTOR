/**
 *******************************************************************************
 * @file        A31G11x_hal_timer2n.c
 * @author      ABOV R&D Division
 * @brief       2N typed Timer
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

#include "A31G11x_hal_timer2n.h"

/* Public Functions --------------------------------------------------------- */
//******************************************************************************
// Function
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         Initialize the TIMER2n peripheral with the specified parameters.
 *                   -  Set Clock frequency for Timer
 *                   -  Set initial configuration for Timer
 * @param[in]     TIMER2x
 *                   Pointer to the target TIMER2
 *                   -  TIMER20 ~ TIMER21
 * @param[in]     TimerCounterMode
 *                   Timer Counter Mode
 *                   -  TIMER2n_PERIODIC_MODE:  Timer mode
 *                   -  TIMER2n_PWM_MODE:       Counter rising mode
 *                   -  TIMER2n_ONESHOT_MODE:   Counter falling mode
 *                   -  TIMER2n_CAPTURE_MODE:   Counter on both edges
 * @param[in]     TIMER2n_Config
 *                   Pointer to the configuration information for the specified peripheral.
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_TIMER2n_Init( TIMER2n_Type* TIMER2x, TIMER2n_MODE_OPT TimerCounterMode, void* TIMER2n_Config )
{
   TIMER2n_PERIODICCFG_Type*     pTimeCfg;
   TIMER2n_PWMCFG_Type*          pPwmOneshotCfg;
   TIMER2n_CAPTURECFG_Type*      pCaptureCfg;
   uint16_t                      reg_val16;

   /* Check TIMER2 handle */
   if( TIMER2x == NULL )
   {
      return HAL_ERROR;
   }

#if 1    // supported
   if( TIMER2x == ( TIMER2n_Type* )TIMER20 )
   {
      SCUCG->PPCLKEN1_b.T20CLKE = 1;
   }
#endif

#if 1    // supported
   if( TIMER2x == ( TIMER2n_Type* )TIMER21 )
   {
      SCUCG->PPCLKEN1_b.T21CLKE = 1;
   }
#endif

   if( TimerCounterMode == TIMER2n_PERIODIC_MODE )
   {
      pTimeCfg = ( TIMER2n_PERIODICCFG_Type* )TIMER2n_Config;

      SCUCG->PPCLKSR_b.T20CLK = pTimeCfg->TCLK_SEL;
      reg_val16 = 0
                  | ( TimerCounterMode << 12 )
                  | TIMER2n_CR_CLK_SET( pTimeCfg->CkSel )
                  | TIMER2n_CR_ECE_SET( pTimeCfg->ExtCkEdge )
                  | TIMER2n_CR_OPOL_SET( pTimeCfg->StartLevel )
                  | TIMER2n_CR_CPOL_SET( pTimeCfg->CapEdge )
                  ;
      TIMER2x->CR = reg_val16;
      TIMER2x->PREDR = ( ( pTimeCfg->Prescaler - 1 ) & TIMER2n_PREDR_MASK );
      TIMER2x->ADR = pTimeCfg->ADR;
      TIMER2x->BDR = pTimeCfg->BDR;
   }
   else if( ( TimerCounterMode == TIMER2n_PWM_MODE ) || ( TimerCounterMode == TIMER2n_ONESHOT_MODE ) )
   {
      pPwmOneshotCfg = ( TIMER2n_PWMCFG_Type* )TIMER2n_Config;

      SCUCG->PPCLKSR_b.T20CLK = pPwmOneshotCfg->TCLK_SEL;
      reg_val16 = 0
                  | ( TimerCounterMode << 12 )
                  | TIMER2n_CR_CLK_SET( pPwmOneshotCfg->CkSel )
                  | TIMER2n_CR_ECE_SET( pPwmOneshotCfg->ExtCkEdge )
                  | TIMER2n_CR_OPOL_SET( pPwmOneshotCfg->StartLevel )
                  | TIMER2n_CR_CPOL_SET( pPwmOneshotCfg->CapEdge )
                  ;
      TIMER2x->CR = reg_val16;
      TIMER2x->PREDR = ( ( pPwmOneshotCfg->Prescaler - 1 ) & TIMER2n_PREDR_MASK );
      TIMER2x->ADR = pPwmOneshotCfg->ADR;
      TIMER2x->BDR = pPwmOneshotCfg->BDR;
   }
   else if( TimerCounterMode == TIMER2n_CAPTURE_MODE )
   {
      pCaptureCfg = ( TIMER2n_CAPTURECFG_Type* )TIMER2n_Config;

      SCUCG->PPCLKSR_b.T20CLK = pCaptureCfg->TCLK_SEL;
      reg_val16 = 0
                  | ( TimerCounterMode << 12 )
                  | TIMER2n_CR_CLK_SET( pCaptureCfg->CkSel )
                  | TIMER2n_CR_CPOL_SET( pCaptureCfg->ClrMode )
                  | TIMER2n_CR_CAPSEL_SET( pCaptureCfg->CAPCkSel )
                  ;
      TIMER2x->CR = reg_val16;
      TIMER2x->PREDR = ( ( pCaptureCfg->Prescaler - 1 ) & TIMER2n_PREDR_MASK );
   }

   TIMER2x->CR_b.T2nCLR = TIMER2n_CLEAR;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Close Timer/Counter device
 * @param[in]     TIMER2x
 *                   Pointer to the target TIMER2
 *                   -  TIMER20 ~ TIMER21
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_TIMER2n_DeInit( TIMER2n_Type* TIMER2x )
{
   /* Check TIMER2 handle */
   if( TIMER2x == NULL )
   {
      return HAL_ERROR;
   }

   // Disable timer/counter
   TIMER2x->CR_b.T2nEN = 0;

#if 1    // supported
   if( TIMER2x == ( TIMER2n_Type* )TIMER20 )
   {
      SCUCG->PPCLKEN1_b.T20CLKE = 0;
   }
#endif

#if 1    // supported
   if( TIMER2x == ( TIMER2n_Type* )TIMER21 )
   {
      SCUCG->PPCLKEN1_b.T21CLKE = 0;
   }
#endif

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Configure the peripheral interrupt.
 * @param[in]     TIMER2x
 *                   Pointer to the target TIMER2
 *                   -  TIMER20 ~ TIMER21
 * @param[in]     TIMER2n_IntCfg
 *                   Specifies the interrupt flag
 *                   -  TIMER2n_CR_MATCH_INTR:     Match interrupt enable
 *                   -  TIMER2n_CR_CAPTURE_INTR:   Capture interrupt enable
 * @param[in]     NewState
 *                   Next State of Interrupt Operation
 *                   -  ENABLE, DISABLE
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_TIMER2n_ConfigInterrupt( TIMER2n_Type* TIMER2x, TIMER2n_INT_Type TIMER2n_IntCfg, FunctionalState NewState )
{
   /* Check TIMER2 handle */
   if( TIMER2x == NULL )
   {
      return HAL_ERROR;
   }

   switch( TIMER2n_IntCfg )
   {
      case TIMER2n_CR_MATCH_INTR:
         if( NewState == ENABLE )
         {
            TIMER2x->CR_b.T2nMIEN = 1;
         }
         else if( NewState == DISABLE )
         {
            TIMER2x->CR_b.T2nMIEN = 0;
         }
         break;
      case TIMER2n_CR_CAPTURE_INTR:
         if( NewState == ENABLE )
         {
            TIMER2x->CR_b.T2nCIEN = 1;
         }
         else if( NewState == DISABLE )
         {
            TIMER2x->CR_b.T2nCIEN = 0;
         }
         break;
   }

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Start/Stop Timer/Counter device
 * @param[in]     TIMER2x
 *                   Pointer to the target TIMER2
 *                   -  TIMER20 ~ TIMER21
 * @param[in]     NewState
 *                   Next State of Functional Operation
 *                   -  ENABLE, DISABLE
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_TIMER2n_Cmd( TIMER2n_Type* TIMER2x, FunctionalState NewState )
{
   /* Check TIMER2 handle */
   if( TIMER2x == NULL )
   {
      return HAL_ERROR;
   }

   if( NewState == ENABLE )
   {
      TIMER2x->CR_b.T2nEN = TIMER2n_ENABLE;
   }
   else
   {
      TIMER2x->CR_b.T2nEN = TIMER2n_DISABLE;
   }

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Clear Timer/Counter device,
 * @param[in]     TIMER2x
 *                   Pointer to the target TIMER2
 *                   -  TIMER20 ~ TIMER21
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_TIMER2n_ClearCounter( TIMER2n_Type* TIMER2x )
{
   /* Check TIMER2 handle */
   if( TIMER2x == NULL )
   {
      return HAL_ERROR;
   }

   TIMER2x->CR_b.T2nCLR |= TIMER2n_CLEAR;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Update value
 * @param[in]     TIMER2x
 *                   Pointer to the target TIMER2
 *                   -  TIMER20 ~ TIMER21
 * @param[in]     CountCh
 *                   -  0 = GRA
 *                   -  1 = GRB
 * @param[in]     Value
 *                   updated match value
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_TIMER2n_UpdateCountValue( TIMER2n_Type* TIMER2x, uint8_t CountCh, uint16_t Value )
{
   /* Check TIMER2 handle */
   if( TIMER2x == NULL )
   {
      return HAL_ERROR;
   }

   switch( CountCh )
   {
      case 0:
         TIMER2x->ADR = Value;
         break;
      case 1:
         TIMER2x->BDR = Value;
         break;
   }

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Clear Timer Status
 * @param[in]     TIMER2x
 *                   Pointer to the target TIMER2
 *                   -  TIMER20 ~ TIMER21
 * @param[in]     value
 *                   clear value
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_TIMER2n_ClearStatus( TIMER2n_Type* TIMER2x, uint8_t value )
{
   /* Check TIMER2 handle */
   if( TIMER2x == NULL )
   {
      return HAL_ERROR;
   }

   TIMER2x->CR |= ( 0xD & value );

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Get Timer Status
 * @param[in]     TIMER2x
 *                   Pointer to the target TIMER2
 *                   -  TIMER20 ~ TIMER21
 * @return        Value of status register
 *//*-------------------------------------------------------------------------*/
uint8_t HAL_TIMER2n_GetStatus( TIMER2n_Type* TIMER2x )
{
   return TIMER2x->CR;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Read value of capture register in timer/counter device
 * @param[in]     TIMER2x
 *                   Pointer to the target TIMER2
 *                   -  TIMER20 ~ TIMER21
 * @return        Value of count register
 *//*-------------------------------------------------------------------------*/
uint32_t HAL_TIMER2n_GetCaptureData( TIMER2n_Type* TIMER2x )
{
   return TIMER2x->CAPDR;
}

