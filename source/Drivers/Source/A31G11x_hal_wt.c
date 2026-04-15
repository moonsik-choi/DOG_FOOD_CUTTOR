/**
 *******************************************************************************
 * @file        A31G11x_hal_wt.c
 * @author      ABOV R&D Division
 * @brief       Watch Timer
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

#include "A31G11x_hal_wt.h"
#include "A31G11x_hal_scu.h"

/* Public Functions --------------------------------------------------------- */
//******************************************************************************
// Function
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         Initialize the WT peripheral with the specified parameters.
 * @param[in]     WT_Config
 *                   Pointer to a WT_CFG_Type structure
 *                   that contains the configuration information for the specified peripheral.
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_WT_Init( WT_CFG_Type* WT_Config )
{
   /* Check WT_Config */
   if( WT_Config == NULL )
   {
      return HAL_ERROR;
   }

   // enable peripheral clock
   HAL_SCU_Peripheral_EnableClock2( PPCLKEN2_WTCLKE, PPxCLKE_Enable );

   WT->CR = 0
            | ( ( WT_Config->wtClkDiv & 0x03 ) << 4 )   // WTINTV[1:0]
            | WT_CR_WTCLR
            ;
   WT->DR = ( WT_Config->wtTmrConst & 0xFFF );

   // return
   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         deinit for Watch Timer function
 * @param         None
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_WT_DeInit( void )
{
   WT->CR = 0;

   // return
   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Configure the peripheral interrupt.
 * @param[in]     NewState
 *                   Next State of Interrupt Operation
 *                   -  ENABLE, DISABLE
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_WT_ConfigInterrupt( FunctionalState NewState )
{
   uint32_t    reg_val = 0;

   reg_val = ( WT->CR & 0xFF );

   if( NewState == ENABLE )
   {
      reg_val |= WT_CR_WTIEN;          // WTIEN bit
   }
   else
   {
      reg_val &= ~WT_CR_WTIEN;
   }

   WT->CR = reg_val | WT_CR_WTIFLAG;   // (1<<1):  clear for WTIFLAG

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Enable WT activity
 * @param[in]     ctrl
 *                   Next State of Functional Operation
 *                   -  ENABLE, DISABLE
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_WT_Start( FunctionalState ctrl )
{
   uint32_t    tmp_reg;

   tmp_reg = WT->CR & 0xFF;
   tmp_reg &= ~( 0x1 << 7 );  // Disable watch-dog timer counter

   if( ctrl == ENABLE )
   {
      tmp_reg |= ( 0x1 << 7 );   // Enable watch timer counter
   }

   tmp_reg |= WT_CR_WTCLR;    // clear the counter and divider

   WT->CR = tmp_reg;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set WT CR Register
 * @param[in]     u32WTSet
 *                      Watch Timer Control Register Setting Data
 * @return        @ref HAL_Status_Type
 * @details       This function sets the mode, external clock edge, Timer out polarity,
 *                Capture Polarity and Timer match/capture interrupt.
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_WT_SetRegister( uint32_t u32WTSet )
{
   WT->CR = u32WTSet;      // Setting Timer 1n Control Register

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Clear the timer status register of WT
 * @param         None
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_WT_ClearStatus( void )
{
   WT->CR |= WT_CR_WTIFLAG;   // (1<<1):  clear for WTIFLAG

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Get the timer status register of WT
 * @param         None
 * @return        the status register of WT
 *//*-------------------------------------------------------------------------*/
uint32_t HAL_WT_GetStatus( void )
{
   return ( WT->CR & WT_STATUS_BITMASK );
}

/*-------------------------------------------------------------------------*//**
 * @brief         Get the current value of WT
 * @param         None
 * @return        current value of WT
 *//*-------------------------------------------------------------------------*/
uint32_t HAL_WT_GetCurrentCount( void )
{
   return ( WT->CNT & 0xFFF );
}

