/**
 *******************************************************************************
 * @file        A31G11x_hal_crc.h
 * @author      ABOV R&D Division
 * @brief       CRC Header File
 *
 * Copyright 2020 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/

#ifndef _CRC_H_
#define _CRC_H_

#include "A31G11x.h"
#include "A31G11x_hal_aa_types.h"

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Constant
//******************************************************************************

// User/Auto Mode Selection Control
/*
#define CRC_USER_M               (0x0uL << 7)
#define CRC_AUTO_M               (0x1uL << 7)
*/
#define MODS_UserMode            (CRC_CR_MODS_UserMode << CRC_CR_MODS_Pos)
#define MODS_AutoMode            (CRC_CR_MODS_AutoMode << CRC_CR_MODS_Pos)

//----------   CRC/Checksum RLT Clear Constant Definition   ----------
#define CRC_RLTCLR               (0x1uL << 6)

//----------   CRC/Checksum Selection Constant Definition   ----------
#define CRC_CRC                  (0x0uL << 5)
#define CRC_CHECKSUM             (0x1uL << 5)

//----------   CRC Polynominal Constant Definition   ----------
#define CRC_CCITT                (0x0uL << 4)
#define CRC_16                   (0x1uL << 4)

//----------   CRC/Checksum Start Address Auto Increment Constant Definition   ----------
#define CRC_NOINC                (0x0uL << 3)
#define CRC_AUTOINC              (0x1uL << 3)

//----------   CRC/Checksum 1st Shifted-in Bit Constant Definition   ----------
#define CRC_MSB                  (0x0uL << 1)
#define CRC_LSB                  (0x1uL << 1)

//******************************************************************************
// Macro
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         CRC In Data
 * @param[in]     u32InData
 *                CRC/Checksum Input Data
 * @details       This macro puts input data for calculation
 *//*-------------------------------------------------------------------------*/
#define CRC_InData( u32InData )  (CRC->IN = u32InData)

/*-------------------------------------------------------------------------*//**
 * @brief         CRC/Checksum Run
 * @details       This macro starts CRC/Checksum calculation
 *//*-------------------------------------------------------------------------*/
#define CRCRun()                 { CRC->CR_b.CRCRUN = 1; NOP(); NOP(); NOP(); NOP(); }

/*-------------------------------------------------------------------------*//**
 * @brief         CRC/Checksum Stop
 * @details       This macro stops forcingly CRC/Checksum calculation
 *//*-------------------------------------------------------------------------*/
#define CRCStop()                (CRC->CR_b.CRCRUN = 0)

/*-------------------------------------------------------------------------*//**
 * @brief         CRC/Checksum Finish Check
 * @details       This macro checks CRC/Checksum finish
 *//*-------------------------------------------------------------------------*/
#define ChkCRCFinish()           (CRC->CR_b.CRCRUN)

//******************************************************************************
// Function
//******************************************************************************

HAL_Status_Type HAL_CRC_Init( void );
HAL_Status_Type HAL_CRC_DeInit( void );

HAL_Status_Type HAL_CRC_SetAddress( uint32_t u32SAdr, uint32_t u32EAdr, uint32_t u32IniD );

uint32_t HAL_CRC_ConfigAutoMode( uint32_t u32SEL, uint32_t u32POLY, uint32_t u32FirstBit );
HAL_Status_Type HAL_CRC_ConfigUserMode( uint32_t u32SEL, uint32_t u32POLY, uint32_t u32AdrInc, uint32_t u32FirstBit );
uint32_t HAL_CRC_UserInput( uint32_t u32Input );

#ifdef __cplusplus
}
#endif

#endif   /* _CRC_H_ */

