/**
 *******************************************************************************
 * @file        A31G11x_hal_adc.c
 * @author      ABOV R&D Division
 * @brief       Analog-to-Digital Converter
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

#include "A31G11x_hal_adc.h"
#include "A31G11x_hal_scu.h"

/* Public Functions --------------------------------------------------------- */
//******************************************************************************
// Function
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         Initialize the ADC peripheral with the specified parameters.
 * @param[in]     ADCx
 *                   Pointer to the target ADC
 *                   -  ADC
 * @param[in]     ADC_Config
 *                   Pointer to a ADC_CFG_Type structure
 *                   that contains the configuration information for the specified peripheral.
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_ADC_Init( ADC_Type* ADCx, ADC_CFG_Type* ADC_Config )
{
   uint32_t    tempreg;

   /* Check ADC handle */
   if( ADCx == NULL )
   {
      return HAL_ERROR;
   }

   HAL_SCU_Peripheral_EnableClock2( PPCLKEN2_ADCLKE, PPxCLKE_Enable );

   ADCx->CR = ( 1 << ADC_CR_ADCEN_Pos ); // ADCEN;

   tempreg = 0
             | ( 1 << ADC_CR_ADCEN_Pos ) // ADCEN
             | ( ( ( ADC_Config->TrgSel ) & 7 ) << ADC_CR_TRIG_Pos ) // TRGSRC
             | ( ( ( ADC_Config->RefSel ) & 1 ) << ADC_CR_REFSEL_Pos )
             | ( 1 << ADC_CR_ADCIFLAG_Pos ) // clear flag
             ;
   ADCx->CR = tempreg;

   ADCx->PREDR = ( ADC_Config->InClkDiv & ADC_PREDR_PRED_Msk );

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Close ADC
 * @param[in]     ADCx
 *                   Pointer to the target ADC
 *                   -  ADC
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_ADC_DeInit( ADC_Type* ADCx )
{
   /* Check ADC handle */
   if( ADCx == NULL )
   {
      return HAL_ERROR;
   }

   ADCx->CR = 0;
   HAL_SCU_Peripheral_EnableClock2( PPCLKEN2_ADCLKE, PPxCLKE_Disable );

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         ADC interrupt configuration
 * @param[in]     ADCx
 *                   Pointer to the target ADC
 *                   -  ADC
 * @param[in]     NewState:
 *                   Next State of Interrupt Operation
 *                   -  ENABLE, DISABLE
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_ADC_ConfigInterrupt( ADC_Type* ADCx, FunctionalState NewState )
{
   uint32_t    tempreg;

   /* Check ADC handle */
   if( ADCx == NULL )
   {
      return HAL_ERROR;
   }

   tempreg = ADCx->CR;
   tempreg &= ~( 1 << ADC_CR_ADCIEN_Pos );

   if( NewState )
   {
      tempreg |= ( 1 << ADC_CR_ADCIEN_Pos );
   }
   ADCx->CR = tempreg;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Select ADC Channel Number
 * @param[in]     ADCx
 *                   Pointer to the target ADC
 *                   -  ADC
 * @param[in]     Channel
 *                   Channel Number
 *                   -  ADC_CR_ADSEL_AN0 ~ ADC_CR_ADSEL_AN10
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_ADC_ChannelSel( ADC_Type* ADCx, uint32_t Channel )
{
   uint32_t    temp_reg;

   /* Check ADC handle */
   if( ADCx == NULL )
   {
      return HAL_ERROR;
   }

   temp_reg = ADCx->CR & ( uint32_t )( ~ADC_CR_ADSEL_Msk );
   temp_reg |= ( uint32_t )( Channel & ADC_CR_ADSEL_Msk );
   ADCx->CR = temp_reg;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Start A/D conversion
 * @param[in]     ADCx
 *                   Pointer to the target ADC
 *                   -  ADC
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_ADC_Start( ADC_Type* ADCx )
{
   /* Check ADC handle */
   if( ADCx == NULL )
   {
      return HAL_ERROR;
   }

   ADCx->CR |= ( 1 << ADC_CR_ADST_Pos );

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Stop A/D conversion
 *                If this function called after a conversion cycle starts,
 *                the current conversion is completed
 * @param[in]     ADCx
 *                   Pointer to the target ADC
 *                   -  ADC
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_ADC_Stop( ADC_Type* ADCx )
{
   /* Check ADC handle */
   if( ADCx == NULL )
   {
      return HAL_ERROR;
   }

   ADCx->CR &= ~( 1 << ADC_CR_ADST_Pos );

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Clear ADC channel status
 * @param[in]     ADCx
 *                   Pointer to the target ADC
 *                   -  ADC
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_ADC_ClearStatus( ADC_Type* ADCx )
{
   uint32_t    tempreg;

   /* Check ADC handle */
   if( ADCx == NULL )
   {
      return HAL_ERROR;
   }

   tempreg = ADCx->CR;
   tempreg |= ( 1 << ADC_CR_ADCIFLAG_Pos );

   ADCx->CR = tempreg;  // clear flag;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Get ADC channel status
 * @param[in]     ADCx
 *                   Pointer to the target ADC
 *                   -  ADC
 * @return        ADC status register
 *//*-------------------------------------------------------------------------*/
uint32_t HAL_ADC_GetStatus( ADC_Type* ADCx )
{
   uint32_t    tempreg;

   tempreg = ADCx->CR;
   tempreg &= ( 1 << ADC_CR_ADCIFLAG_Pos );

   return tempreg;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Get Result conversion from A/D data register
 * @param[in]     ADCx
 *                   Pointer to the target ADC
 *                   -  ADC
 * @return        Result of conversion
 *//*-------------------------------------------------------------------------*/
uint16_t HAL_ADC_GetData( ADC_Type* ADCx )
{
   uint16_t    adc_value;

   adc_value = ADCx->DR;

   return ADC_DR_RESULT( adc_value );
}

