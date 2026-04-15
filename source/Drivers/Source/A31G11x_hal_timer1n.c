/**
 *******************************************************************************
 * @file        A31G11x_hal_timer1n.c
 * @author      ABOV R&D Division
 * @brief       1N typed Timer
 *
 * Copyright 2022 0BOV Semiconductor Co.,Ltd. All rights reserved.
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

#include "A31G11x_hal_scu.h"
#include "A31G11x_hal_timer1n.h"

/* Public Functions --------------------------------------------------------- */
//******************************************************************************
// Function
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         Initialize the TIMER1n peripheral with the specified parameters.
 *                   -  Set Clock frequency for Timer
 *                   -  Set initial configuration for Timer
 * @param[in]     TIMER1x
 *                   Pointer to the target TIMER1
 *                   -  TIMER10 ~ TIMER12
 * @param[in]     TimerCounterMode
 *                   Timer Counter Mode
 *                   -  TIMER1n_PERIODIC_MODE:  Timer mode
 *                   -  TIMER1n_PWM_MODE:       Counter rising mode
 *                   -  TIMER1n_ONESHOT_MODE:   Counter falling mode
 *                   -  TIMER1n_CAPTURE_MODE:   Counter on both edges
 * @param[in]     TIMER1n_Config
 *                   Pointer to the configuration information for the specified peripheral.
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_TIMER1n_Init( TIMER1n_Type* TIMER1x, TIMER1n_MODE_OPT TimerCounterMode, void* TIMER1n_Config )
{
   TIMER1n_PERIODICCFG_Type*     pTimeCfg;
   TIMER1n_PWMCFG_Type*          pPwmOneshotCfg;
   TIMER1n_CAPTURECFG_Type*      pCaptureCfg;
   uint16_t                      reg_val16;

   /* Check TIMER1 handle */
   if( TIMER1x == NULL )
   {
      return HAL_ERROR;
   }

#if 1    // supported
   if( TIMER1x == ( TIMER1n_Type* )TIMER10 )
   {
      HAL_SCU_Peripheral_EnableClock1( PPCLKEN1_T10CLKE, PPxCLKE_Enable );
   }
#endif

#if 1    // supported
   if( TIMER1x == ( TIMER1n_Type* )TIMER11 )
   {
      HAL_SCU_Peripheral_EnableClock1( PPCLKEN1_T11CLKE, PPxCLKE_Enable );
   }
#endif

#if 1    // supported
   if( TIMER1x == ( TIMER1n_Type* )TIMER12 )
   {
      HAL_SCU_Peripheral_EnableClock1( PPCLKEN1_T12CLKE, PPxCLKE_Enable );
   }
#endif

#if 0    // not supported
   if( TIMER1x == ( TIMER1n_Type* )TIMER13 )
   {
      HAL_SCU_Peripheral_EnableClock1( PPCLKEN1_T13CLKE, PPxCLKE_Enable );
   }
#endif

#if 0    // not supported
   if( TIMER1x == ( TIMER1n_Type* )TIMER14 )
   {
      HAL_SCU_Peripheral_EnableClock1( PPCLKEN1_T14CLKE, PPxCLKE_Enable );
   }
#endif

#if 0    // not supported
   if( TIMER1x == ( TIMER1n_Type* )TIMER15 )
   {
      HAL_SCU_Peripheral_EnableClock1( PPCLKEN1_T15CLKE, PPxCLKE_Enable );
   }
#endif

#if 0    // not supported
   if( TIMER1x == ( TIMER1n_Type* )TIMER16 )
   {
      HAL_SCU_Peripheral_EnableClock1( PPCLKEN1_T16CLKE, PPxCLKE_Enable );
   }
#endif

   if( TimerCounterMode == TIMER1n_PERIODIC_MODE )
   {
      pTimeCfg = ( TIMER1n_PERIODICCFG_Type* )TIMER1n_Config;

      reg_val16 = 0
                  | TIMER1n_CR_CKSEL_SET( pTimeCfg->CkSel )
                  | TIMER1n_CR_MODE_SET( TimerCounterMode )
                  | TIMER1n_CR_STARTLVL_SET( pTimeCfg->StartLevel )
                  ;
      if( pTimeCfg->CkSel == 1 )
      {
         reg_val16 = reg_val16 | TIMER1n_CR_ECE_SET( pTimeCfg->ECE );
      }
      TIMER1x->CR = reg_val16;

      TIMER1x->PREDR = ( ( pTimeCfg->Prescaler - 1 ) & TIMER1n_PRS_MASK );
      TIMER1x->ADR = pTimeCfg->ADR;
   }
   else if( ( TimerCounterMode == TIMER1n_PWM_MODE ) || ( TimerCounterMode == TIMER1n_ONESHOT_MODE ) )
   {
      pPwmOneshotCfg = ( TIMER1n_PWMCFG_Type* )TIMER1n_Config;

      reg_val16 = 0
                  | TIMER1n_CR_CKSEL_SET( pPwmOneshotCfg->CkSel )
                  | TIMER1n_CR_MODE_SET( TimerCounterMode )
                  | TIMER1n_CR_STARTLVL_SET( pPwmOneshotCfg->StartLevel )
                  ;
      if( pPwmOneshotCfg->CkSel == 1 )
      {
         reg_val16 = reg_val16 | TIMER1n_CR_ECE_SET( pPwmOneshotCfg->ECE );
      }
      TIMER1x->CR = reg_val16;

      TIMER1x->PREDR = ( ( pPwmOneshotCfg->Prescaler - 1 ) & TIMER1n_PRS_MASK );
      TIMER1x->ADR = pPwmOneshotCfg->ADR;
      TIMER1x->BDR = pPwmOneshotCfg->BDR;
   }
   else if( TimerCounterMode == TIMER1n_CAPTURE_MODE )
   {
      pCaptureCfg = ( TIMER1n_CAPTURECFG_Type* )TIMER1n_Config;

      reg_val16 = 0
                  | TIMER1n_CR_CKSEL_SET( pCaptureCfg->CkSel )
                  | TIMER1n_CR_MODE_SET( TimerCounterMode )
                  | TIMER1n_CR_CPOL_SET( pCaptureCfg->ClrMode )
                  ;
      if( pCaptureCfg->CkSel == 1 )
      {
         reg_val16 = reg_val16 | TIMER1n_CR_ECE_SET( pCaptureCfg->ECE );
      }
      TIMER1x->CR = reg_val16;

      TIMER1x->PREDR = ( ( pCaptureCfg->Prescaler - 1 ) & TIMER1n_PRS_MASK );
      TIMER1x->ADR = pCaptureCfg->ADR;
   }
   TIMER1x->CR |= 0x1; //  timer counter clear

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Close Timer/Counter device
 * @param[in]     TIMER1x
 *                   Pointer to the target TIMER1
 *                   -  TIMER10 ~ TIMER12
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_TIMER1n_DeInit( TIMER1n_Type* TIMER1x )
{
   /* Check TIMER1 handle */
   if( TIMER1x == NULL )
   {
      return HAL_ERROR;
   }

   // Disable timer/counter
   TIMER1x->CR = 0x00;

#if 1    // supported
   if( TIMER1x == ( TIMER1n_Type* )TIMER10 )
   {
      HAL_SCU_Peripheral_EnableClock1( PPCLKEN1_T10CLKE, PPxCLKE_Disable );
   }
#endif

#if 1    // supported
   if( TIMER1x == ( TIMER1n_Type* )TIMER11 )
   {
      HAL_SCU_Peripheral_EnableClock1( PPCLKEN1_T11CLKE, PPxCLKE_Disable );
   }
#endif

#if 1    // supported
   if( TIMER1x == ( TIMER1n_Type* )TIMER12 )
   {
      HAL_SCU_Peripheral_EnableClock1( PPCLKEN1_T12CLKE, PPxCLKE_Disable );
   }
#endif

#if 0    // not supported
   if( TIMER1x == ( TIMER1n_Type* )TIMER13 )
   {
      HAL_SCU_Peripheral_EnableClock1( PPCLKEN1_T13CLKE, PPxCLKE_Disable );
   }
#endif

#if 0    // not supported
   if( TIMER1x == ( TIMER1n_Type* )TIMER14 )
   {
      HAL_SCU_Peripheral_EnableClock1( PPCLKEN1_T14CLKE, PPxCLKE_Disable );
   }
#endif

#if 0    // not supported
   if( TIMER1x == ( TIMER1n_Type* )TIMER15 )
   {
      HAL_SCU_Peripheral_EnableClock1( PPCLKEN1_T15CLKE, PPxCLKE_Disable );
   }
#endif

#if 0    // not supported
   if( TIMER1x == ( TIMER1n_Type* )TIMER16 )
   {
      HAL_SCU_Peripheral_EnableClock1( PPCLKEN1_T16CLKE, PPxCLKE_Disable );
   }
#endif

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Configure the peripheral interrupt.
 * @param[in]     TIMER1x
 *                   Pointer to the target TIMER1
 *                   -  TIMER10 ~ TIMER12
 * @param[in]     TIMER1n_IntCfg
 *                   Specifies the interrupt flag
 *                   -  TIMER1n_INTCFG_MIE:  OVIE Interrupt enable
 *                   -  TIMER1n_INTCFG_CIE:  MBIE Interrupt enable
 * @param[in]     NewState
 *                   Next State of Interrupt Operation
 *                   -  ENABLE, DISABLE
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_TIMER1n_ConfigInterrupt( TIMER1n_Type* TIMER1x, TIMER1n_INT_Type TIMER1n_IntCfg, FunctionalState NewState )
{
   uint8_t     tmp;

   /* Check TIMER1 handle */
   if( TIMER1x == NULL )
   {
      return HAL_ERROR;
   }

   switch( TIMER1n_IntCfg )
   {
      case TIMER1n_INTCFG_MIE:
         tmp = TIMER1n_MATINTEN;
         break;
      case TIMER1n_INTCFG_CIE:
         tmp = TIMER1n_CAPINTEN;
         break;
   }

   if( NewState == ENABLE )
   {
      TIMER1x->CR |= tmp;
   }
   else
   {
      TIMER1x->CR &= ( ~tmp );
   }

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Start/Stop Timer/Counter device
 * @param[in]     TIMER1x
 *                   Pointer to the target TIMER1
 *                   -  TIMER10 ~ TIMER12
 * @param[in]     NewState
 *                   Next State of Functional Operation
 *                   -  ENABLE, DISABLE
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_TIMER1n_Cmd( TIMER1n_Type* TIMER1x, FunctionalState NewState )
{
   /* Check TIMER1 handle */
   if( TIMER1x == NULL )
   {
      return HAL_ERROR;
   }

   if( NewState == ENABLE )
   {
      TIMER1x->CR |=  TIMER1n_ENABLE;
   }
   else
   {
      TIMER1x->CR &= ~TIMER1n_ENABLE;
   }

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set TIMER1n CR/PREDR Registers
 * @param[in]     TIMER1x
 *                   Pointer to the target TIMER1
 *                   -  TIMER10 ~ TIMER12
 * @param[in]     u32T1nSet
 *                   TIMER1n Control Register Setting Data
 * @param[in]     u32T1nClk
 *                   TIMER1n Prescaler Data
 * @return        @ref HAL_Status_Type
 * @details       This function sets the mode, external clock edge, Timer out polarity,
 *                Capture Polarity and Timer match/capture interrupt.
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_TIMER1n_SetRegister( TIMER1n_Type* TIMER1x, uint32_t u32T1nSet, uint32_t u32T1nClk )
{
   /* Check TIMER1 handle */
   if( TIMER1x == NULL )
   {
      return HAL_ERROR;
   }

   TIMER1x->CR = u32T1nSet;      // Setting TIMER1n Control Register

   TIMER1x->PREDR = u32T1nClk;   // Setting TIMER1n Prescaler data

   return HAL_OK;
}

