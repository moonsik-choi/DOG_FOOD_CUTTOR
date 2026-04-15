/**
 *******************************************************************************
 * @file        A31G11x_hal_crc.c
 * @author      ABOV R&D Division
 * @brief       Cyclic Redundancy Check
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

#include "A31G11x_hal_crc.h"
#include "A31G11x_hal_scu.h"

/* Public Functions --------------------------------------------------------- */
//******************************************************************************
// Function
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         Initialize CRC/Checksum peripheral
 * @param         None
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_CRC_Init( void )
{
   // enable peripheral clock
   HAL_SCU_Peripheral_EnableClock2( PPCLKEN2_CRCLKE, PPxCLKE_Enable );

   // return
   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         DeInitialize CRC peripheral
 * @param         None
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_CRC_DeInit( void )
{
   // reset peripheral and disable peripheral clock
   HAL_SCU_Peripheral_SetReset2( PPRST2_CRRST );
   HAL_SCU_Peripheral_EnableClock2( PPCLKEN2_CRCLKE, PPxCLKE_Disable );

   // return
   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set CRC/Checksum Address
 * @param[in]     u32SAdr
 *                   CRC/Checksum Start Address
 *                   -  0x10000000 ~ 0x10007FFF
 * @param[in]     u32EAdr
 *                   CRC/Checksum End Address
 *                   -  0x10000000 ~ 0x10007FFF
 * @param[in]     u32IniD
 *                   CRC/Checksum Initial Data
 * @return        @ref HAL_Status_Type
 * @details       This function sets the start/end address for range and initial data for calculation
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_CRC_SetAddress( uint32_t u32SAdr, uint32_t u32EAdr, uint32_t u32IniD )
{
   SCUCG->PPCLKEN2_b.CRCLKE = 1;    // CRC/Checksum Clock Enable

   CRC->SADR = u32SAdr;             // Set start address
   CRC->EADR = u32EAdr;             // Set end address
   CRC->INIT = u32IniD;             // Set initial data for CRC/Checksum

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         CRC/Checksum Auto Mode Start and Result
 * @param[in]     u32SEL
 *                   CRC/Checksum Selection
 *                   -  CRC_CRC
 *                   -  CRC_CHECKSUM
 * @param[in]     u32POLY
 *                   CRC Polynomial Selection
 *                   -  CRC_CCITT
 *                   -  CRC_16
 * @param[in]     u32FirstBit
 *                   CRC 1st Shifted-in bit
 *                   -  CRC_MSB
 *                   -  CRC_LSB
 * @return        CRC/Checksum Result
 * @details       This function starts CRC/Checksum calculation and gets result
 *//*-------------------------------------------------------------------------*/
uint32_t HAL_CRC_ConfigAutoMode( uint32_t u32SEL, uint32_t u32POLY, uint32_t u32FirstBit )
{
   uint32_t    imgPRIMASK;

   if( SystemCoreClock > 20000000uL )
   {
      SCUCG->SCDIVR1_b.HDIV = 3;          // HCLK should be less than or equal to 20MHz during CRC/Checksum auto mode
   }

   CRC->CR = 0
             | ( 0x1uL << CRC_CR_RLTCLR_Pos )   // CRC/Checksum Result Data Register (CRC_RLT) Initialization
             | MODS_AutoMode                    // User/Auto Mode Selection
             | u32SEL                           // CRC/Checksum Selection
             | u32POLY                          // Polynomial Selection (CRC only)
             | CRC_NOINC                        // CRC/Checksum Start Address Auto Increment Control (User mode only)
             | u32FirstBit;                     // First Shifted-in Selection (CRC only)

   imgPRIMASK = __get_PRIMASK();          // backup PRIMASK (current global interrupt configuration)
   DI();                                  // disable global interrupt
   CRCRun();
   while( ChkCRCFinish() ) {}             // Check if CRC/Checksum finishes or not
   __set_PRIMASK( imgPRIMASK );           // restore PRIMASK

   SCUCG->PPCLKEN2_b.CRCLKE = 0;          // CRC/Checksum Clock Disable

   if( SystemCoreClock > 20000000uL )
   {
      SCUCG->SCDIVR1_b.HDIV = 4;          // HCLK should be set with original frequency
   }

   return ( CRC->RLT & 0xffff );
}

/*-------------------------------------------------------------------------*//**
 * @brief         CRC/Checksum User Mode Start
 * @param[in]     u32SEL
 *                   CRC/Checksum Selection
 *                   -  CRC_CRC
 *                   -  CRC_CHECKSUM
 * @param[in]     u32POLY
 *                   CRC Polynomial Selection
 *                   -  CRC_CCITT
 *                   -  CRC_16
 * @param[in]     u32AdrInc
 *                   Auto Increment of Start Address
 *                   -  CRC_NOINC
 *                   -  CRC_AUTOINC
 * @param[in]     u32FirstBit
 *                   CRC 1st Shifted-in bit
 *                   -  CRC_MSB
 *                   -  CRC_LSB
 * @return        @ref HAL_Status_Type
 * @details       This function sets the control register for CRC/Checksum User mode
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_CRC_ConfigUserMode( uint32_t u32SEL, uint32_t u32POLY, uint32_t u32AdrInc, uint32_t u32FirstBit )
{
   CRC->CR = 0
             | ( 0x1uL << CRC_CR_RLTCLR_Pos )   // CRC/Checksum Result Data Register (CRC_RLT) Initialization
             | MODS_UserMode                    // User/Auto Mode Selection
             | u32SEL                           // CRC/Checksum Selection
             | u32POLY                          // Polynomial Selection (CRC only)
             | u32AdrInc                        // CRC/Checksum Start Address Auto Increment Control (User mode only)
             | u32FirstBit;                     // First Shifted-in Selection (CRC only)
   CRCRun();

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         CRC/Checksum Input on User Mode
 * @param[in]     u32Input
 *                   CRC/Checksum Input
 * @return        CRC/Checksum Result with finish Indicator
 *                   -  0x8a29xxxx if on run
 *                   -  0x0000xxxx if Finish
 * @details       This function calculates up to finish.
 *                The CRC_SADR register should be incremented by 4 every input on CRC_NOINC
 *//*-------------------------------------------------------------------------*/
uint32_t HAL_CRC_UserInput( uint32_t u32Input )
{
   uint32_t    u32Result = 0x8a290000uL;
   uint32_t    CRC_EADR;

   CRC_InData( u32Input );
   CRC_EADR = CRC->EADR;
   if( ( !ChkCRCFinish() ) || ( CRC->SADR > CRC_EADR ) )    // "Auto"/"User" Increment of Start Address
   {
      CRCStop();                             // Stop forcingly on User Increment of Start Address
      SCUCG->PPCLKEN2_b.CRCLKE = 0;          // CRC/Checksum Clock Disable
      u32Result = ( CRC->RLT & 0xffff );
   }

   return u32Result;
}

