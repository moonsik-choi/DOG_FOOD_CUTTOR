/**
 *******************************************************************************
 * @file        A31G11x_hal_sculv.c
 * @author      ABOV R&D Division
 * @brief       LV sub system in System Control Unit
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

#include "A31G11x_hal_sculv.h"
#include "A31G11x_hal_scu.h"

/* Public Functions --------------------------------------------------------- */
//******************************************************************************
// Function
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         Set LVI Block Enable/Disable and Voltage Level
 * @param[in]     u32LviEnBit
 *                   LVI Enable/Disable Control
 *                   -  LVIEN_Disable, LVIEN_Enable
 * @param[in]     u32LviIntEnBit
 *                   LVI Interrupt Enable/Disable Control
 *                   -  LVINTEN_Disable, LVINTEN_Enable
 * @param[in]     u32LviLevel
 *                   LVI Voltage Level
 *                   -  LVIVS_2p00V    // 2.00V
 *                   -  LVIVS_2p13V    // 2.13V
 *                   -  LVIVS_2p28V    // 2.28V
 *                   -  LVIVS_2p46V    // 2.46V
 *                   -  LVIVS_2p67V    // 2.67V
 *                   -  LVIVS_3p04V    // 3.04V
 *                   -  LVIVS_3p20V    // 3.20V
 *                   -  LVIVS_3p55V    // 3.55V
 *                   -  LVIVS_3p75V    // 3.75V
 *                   -  LVIVS_3p99V    // 3.99V
 *                   -  LVIVS_4p25V    // 4.25V
 *                   -  LVIVS_4p55V    // 4.55V
 * @return        None
 * @details       This function sets LVI voltage level.
 *//*-------------------------------------------------------------------------*/
void HAL_LVI_Init( uint32_t u32LviEnBit, uint32_t u32LviIntEnBit, uint32_t u32LviLevel )
{
   // enable peripheral clock
   HAL_SCU_Peripheral_EnableClock2( PPCLKEN2_LVICLKE, PPxCLKE_Enable );

   SCULV->LVICR = ( u32LviEnBit | u32LviIntEnBit | u32LviLevel );
}

