/**
 *******************************************************************************
 * @file        A31G11x_hal_adc.h
 * @author      ABOV R&D Division
 * @brief       ADC Header File
 *
 * Copyright 2020 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/

#ifndef _ADC_H_
#define _ADC_H_

#include "A31G11x.h"
#include "A31G11x_hal_aa_types.h"

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Constant
//******************************************************************************

//----------   ADC Trigger Signal Definition   ----------
#define ADC_ADST                    (0x0uL << 11)
#define ADC_T10M                    (0x1uL << 11)
#define ADC_T11M                    (0x2uL << 11)
#define ADC_T12M                    (0x3uL << 11)
#define ADC_T30                     (0x4uL << 11)

//----------   ADC Reference Selection Definition   ----------
#define ADC_VDD                     (0x0uL << 10)
#define ADC_AVREF                   (0x1uL << 10)

//----------   ADC Convesion Start Definition   ----------
//#define ADC_ADST                  (0x1uL << 8)

//----------   ADC Interrupt Enable/Disable Definition   ----------
#define ADC_INTDIS                  (0x0uL << 5)
#define ADC_INTEN                   (0x1uL << 5)

//----------   ADC Channel Selection Definition   ----------
#define ADC_AN0                     (0x00uL << 0)
#define ADC_AN1                     (0x01uL << 0)
#define ADC_AN2                     (0x02uL << 0)
#define ADC_AN3                     (0x03uL << 0)
#define ADC_AN4                     (0x04uL << 0)
#define ADC_AN5                     (0x05uL << 0)
#define ADC_AN6                     (0x06uL << 0)
#define ADC_AN7                     (0x07uL << 0)
#define ADC_AN8                     (0x08uL << 0)
#define ADC_AN9                     (0x09uL << 0)
#define ADC_AN10                    (0x0auL << 0)

/*-------------------------------------------------------------------------*//**
 * Macro defines for ADC Status register
 *//*-------------------------------------------------------------------------*/
#define ADC_STAT_END                ((1uL << 4))

#define ADC_REF_VDD                 0  /**<  ADC ref source VDD */
#define ADC_REF_AVREF               1  /**<  ADC ref source AVREF */

#define ADC_TRIGGER_DISABLE         0  /**<  Event Trigger Disabled/Soft-Trigger Only */
#define ADC_TRIGGER_TIMER10         1  /**<  Timer10 Event Trigger */
#define ADC_TRIGGER_TIMER11         2  /**<  Timer11 Event Trigger */
#define ADC_TRIGGER_TIMER12         3  /**<  Timer12 Event Trigger */
#define ADC_TRIGGER_TIMER30         4  /**<  Timer30 Event Trigger */

//******************************************************************************
// Type
//******************************************************************************

//==============================================================================
// Structure
//==============================================================================

typedef struct
{
   /* ADC_CR */
   uint32_t    RefSel;           /**<  RestartEn = ENABLE or DISABLE */
   uint32_t    TrgSel;

   /* ADC_PREDR */
   uint32_t    InClkDiv;
} ADC_CFG_Type;

//******************************************************************************
// Macro
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         ADC Enable
 * @details       This macro Enable ADC Block
 *//*-------------------------------------------------------------------------*/
#define ADCEN()                     (ADC->CR_b.ADCEN = 1)

/*-------------------------------------------------------------------------*//**
 * @brief         ADC Disable
 * @details       This macro Disable ADC Block
 *//*-------------------------------------------------------------------------*/
#define ADCDIS()                    (ADC->CR_b.ADCEN = 0)

/*-------------------------------------------------------------------------*//**
 * @brief         ADC Start bit set
 * @details       This macro starts ADC conversion
 *//*-------------------------------------------------------------------------*/
#define ADCADST_Set()               (ADC->CR_b.ADST = 1)

/*-------------------------------------------------------------------------*//**
 * @brief         ADC Start bit get
 * @details       This macro gets ADST bit
 *//*-------------------------------------------------------------------------*/
#define ADCADST_Get()               (ADC->CR_b.ADST)

/*-------------------------------------------------------------------------*//**
 * @brief         Get flags of ADC Interrupt
 * @details       This macro gets interrupt flag of ADC
 *//*-------------------------------------------------------------------------*/
#define ADCInt_GetFg()              (ADC->CR_b.ADCIFLAG)

/*-------------------------------------------------------------------------*//**
 * @brief         Clear flags of ADC Interrupt
 * @details       This macro gets interrupt flag of ADC
 *//*-------------------------------------------------------------------------*/
#define ADCInt_ClrFg()              (ADC->CR_b.ADCIFLAG = 1)

/*-------------------------------------------------------------------------*//**
 * @brief         Get data of ADC Conversion
 * @details       This macro gets data of ADC Conversion
 *//*-------------------------------------------------------------------------*/
#define ADCData_Get()               (ADC->DR)

/*-------------------------------------------------------------------------*//**
 * Macro defines for ADC Data register
 *//*-------------------------------------------------------------------------*/
/**   When DONE is 1, this field contains result value of ADC conversion */
#define ADC_DR_RESULT( n )          ((n) & ADC_DR_ADDATA_Msk)

//******************************************************************************
// Function
//******************************************************************************

HAL_Status_Type HAL_ADC_Init( ADC_Type* ADCx, ADC_CFG_Type* ADC_Config );
HAL_Status_Type HAL_ADC_DeInit( ADC_Type* ADCx );

HAL_Status_Type HAL_ADC_ConfigInterrupt( ADC_Type* ADCx, FunctionalState NewState );
HAL_Status_Type HAL_ADC_ChannelSel( ADC_Type* ADCx, uint32_t Channel );
HAL_Status_Type HAL_ADC_Start( ADC_Type* ADCx );
HAL_Status_Type HAL_ADC_Stop( ADC_Type* ADCx );
HAL_Status_Type HAL_ADC_ClearStatus( ADC_Type* ADCx );
uint32_t HAL_ADC_GetStatus( ADC_Type* ADCx );

uint16_t HAL_ADC_GetData( ADC_Type* ADCx );

#ifdef __cplusplus
}
#endif

#endif   /* _ADC_H_ */

