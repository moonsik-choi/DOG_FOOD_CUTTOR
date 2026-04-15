/**
 *******************************************************************************
 * @file        main.c
 * @author      ABOV R&D Division
 * @brief       Main Example Code
 *
 * Copyright 2020 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/

//******************************************************************************
// Include
//******************************************************************************

#include "main.h"
#include "main_conf.h"
#include "ks_BITBAND.h"

/* Private typedef ---------------------------------------------------------- */
/* Private define ----------------------------------------------------------- */
/* Private macro ------------------------------------------------------------ */
/* Private variables -------------------------------------------------------- */
/* Private define ----------------------------------------------------------- */
/* Private function prototypes ---------------------------------------------- */
void Process(void);

void GPIO_Configure( void );
void TIMER10_Configure( void );
void TIMER11_Configure( void );
void TIMER30_Configure( void );
void ADC_Configure( void );
void WDT_Configure( void );
void mainloop( void );
int main( void );
void Error_Handler( void );

/* Private variables -------------------------------------------------------- */
//******************************************************************************
// Variable
//******************************************************************************
TIMER1n_PERIODICCFG_Type      TIMER1n_Config;
TIMER1n_PERIODICCFG_Type      TIMER11_Config;
volatile uint32_t             count;
volatile uint32_t             flag;

//******************************************************************************
// Function
//******************************************************************************
void TIMER30_IRQHandler_IT( void )
{
   if( ( HAL_TIMER3n_GetStatus_IT( ( TIMER3n_Type* )TIMER30 ) & TIMER3n_INT_PMATEN ) == TIMER3n_INT_PMATEN )
   {
      if( flag == 0 )
      {
         flag = 1;
         HAL_GPIO_ClearPin( ( Pn_Type* )PC, _BIT( 0 ) );
      }
      else
      {
         flag = 0;
         HAL_GPIO_SetPin( ( Pn_Type* )PC, _BIT( 0 ) );
      }
   }

   HAL_TIMER3n_ClearStatus_IT( ( TIMER3n_Type* )TIMER30, 0x7F );
}

/*-------------------------------------------------------------------------*//**
 * @brief         Main loop
 * @param         None
 * @return        None
 *//*-------------------------------------------------------------------------*/
void mainloop( void )
{
	TIMER10_Configure();
#ifdef RGB_LED_GPIO_MODE
	TIMER11_Configure();
#else
	GPIO_Configure();
	TIMER30_Configure();
#endif
	ADC_Configure();
	__enable_irq();
	
	WDT_Configure();
	DPO_LED_PWR_ON;
	while (1)
	{
		Process();
	}
}

/*-------------------------------------------------------------------------*//**
 * @brief         Main program
 * @param         None
 * @return        None
 *//*-------------------------------------------------------------------------*/
int main( void )
{
   /* Initialize all port */
   Port_Init();

   /* Configure the system clock to HSI 40MHz */
   SystemClock_Config();

   /* Infinite loop */
   mainloop();

   return 0;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Driver error handler
 * @param         None
 * @return        None
 *//*-------------------------------------------------------------------------*/
void Error_Handler( void )
{
   while( 1 )
   {
   }
}


/*-------------------------------------------------------------------------*//**
 * @brief         GPIO_Configure
 * @param         None
 * @return        None
 *//*-------------------------------------------------------------------------*/
void GPIO_Configure( void )
{
   // Test Pin setting PE0
//   HAL_GPIO_ConfigOutput( ( Pn_Type* )PE, 0, PUSH_PULL_OUTPUT );
//   HAL_GPIO_ConfigPullup( ( Pn_Type* )PE, 0, PUPDx_Disable );
//   HAL_GPIO_ClearPin( ( Pn_Type* )PE, _BIT( 0 ) );

	HAL_GPIO_ConfigOutput( ( Pn_Type* )PE, 0, ALTERN_FUNC );	// PWM30AA - Motor PWM
//	HAL_GPIO_ConfigOutput( ( Pn_Type* )PE, 2, ALTERN_FUNC );	// PWM30BA - not used
//	HAL_GPIO_ConfigOutput( ( Pn_Type* )PE, 4, ALTERN_FUNC );	// PWM30CA - not used
}

void ADC_Configure( void )
{
   ADC_CFG_Type      ADC_Config;

   // ADC configuration
   {
      // adc input pin configuration
      {
         // configure PA7 as a AN7
         HAL_GPIO_ConfigOutput( ( Pn_Type* )PA, 7, ALTERN_FUNC );
         HAL_GPIO_ConfigFunction( ( Pn_Type* )PA, 7, AFSRx_AF3 );

         // configure PB2 as a AN10
         HAL_GPIO_ConfigOutput( ( Pn_Type* )PB, 2, ALTERN_FUNC );
         HAL_GPIO_ConfigFunction( ( Pn_Type* )PB, 2, AFSRx_AF3 );
      }

      // adc configuration
      {
         ADC_Config.RefSel = ADC_REF_VDD;    // 0:ADC_REF_VDD ,1:ADC_REF_AVREF
         ADC_Config.TrgSel = ADC_TRIGGER_DISABLE;
         ADC_Config.InClkDiv = 14;           // 40M/14 = 2.8M (less than 3M)
         if( HAL_ADC_Init( ADC, &ADC_Config ) != HAL_OK )
         {
            /* Initialization Error */
            Error_Handler();
         }
      }
   }
}


/*-------------------------------------------------------------------------*//**
 * @brief         TIMER10_Configure
 * @param         None
 * @return        None
 *//*-------------------------------------------------------------------------*/
void TIMER10_Configure( void )
{
   count = 0;
   flag = 0;

   // TIMER10
   TIMER1n_Config.CkSel = TIMER1n_PCLK;               // PCLK = 40MHz
   TIMER1n_Config.Prescaler = 40;                     // fTIMER = PCLK / 40 = 1MHz
   TIMER1n_Config.ADR = ( 1000 );                     // Period  = ADR / fTIMER = 1000us
   TIMER1n_Config.StartLevel = TIMER1n_START_LOW;
   TIMER1n_Config.BDR = 0;                            // Duty(B) = BDR / fTIMER =    0us  // dummy parameter
   TIMER1n_Config.ECE = TIMER1n_FALLING_EGDE;         // dummy parameter

   HAL_TIMER1n_Init( ( TIMER1n_Type* )TIMER10, TIMER1n_PERIODIC_MODE, &TIMER1n_Config );
   HAL_TIMER1n_ConfigInterrupt( ( TIMER1n_Type* )TIMER10, TIMER1n_INTCFG_MIE, ENABLE );

   NVIC_SetPriority( TIMER10_IRQn, 3 );
   NVIC_EnableIRQ( TIMER10_IRQn );
   HAL_INT_EInt_MaskDisable( MSK_TIMER10 );

   HAL_TIMER1n_Cmd( ( TIMER1n_Type* )TIMER10, ENABLE );   // timer start & clear
}

void TIMER11_Configure( void )
{
   // TIMER11
   TIMER1n_Config.CkSel = TIMER1n_PCLK;               // PCLK = 40MHz
   TIMER1n_Config.Prescaler = 40;                     // fTIMER = PCLK / 40 = 1MHz
   TIMER1n_Config.ADR = ( 21 );                     // Period  = ADR / fTIMER = 50us
   TIMER1n_Config.StartLevel = TIMER1n_START_LOW;
   TIMER1n_Config.BDR = 0;                            // Duty(B) = BDR / fTIMER =    0us  // dummy parameter
   TIMER1n_Config.ECE = TIMER1n_FALLING_EGDE;         // dummy parameter

   HAL_TIMER1n_Init( ( TIMER1n_Type* )TIMER11, TIMER1n_PERIODIC_MODE, &TIMER1n_Config );
   HAL_TIMER1n_ConfigInterrupt( ( TIMER1n_Type* )TIMER11, TIMER1n_INTCFG_MIE, ENABLE );

   NVIC_SetPriority( TIMER11_IRQn, 3 );
   NVIC_EnableIRQ( TIMER11_IRQn );
   HAL_INT_EInt_MaskDisable( MSK_TIMER11 );

   HAL_TIMER1n_Cmd( ( TIMER1n_Type* )TIMER11, ENABLE );   // timer start & clear
}

/*-------------------------------------------------------------------------*//**
 * @brief         TIMER30_Configure
 * @param         None
 * @return        None
 *//*-------------------------------------------------------------------------*/
#define PWM_HZ	20000			// 수정 가능(PWM 주파수 설정), Period값 PDR레지스터가 100이 되도록 해야함.
#define CLK_DIV	10				// 경우에 따라 수정 가능,  Period값 PDR레지스터가 100이 되도록 해야함.
#define HCLK	40000000UL		// 수정 금지.
void TIMER30_Configure( void )
{
   // select peripheral clock: MCLK
   HAL_SCU_Peripheral_ClockSelection( PPCLKSR_T30CLK, T30CLK_MCLK );

   HAL_SCU_Peripheral_EnableClock1( PPCLKEN1_T30CLKE, PPxCLKE_Enable );

   HAL_TIMER3n_ClockPrescaler( ( TIMER3n_Type* )TIMER30, (CLK_DIV-1) );      // 40MHz/(0+1) = 40MHz

   HAL_TIMER3n_SetPeriod( ( TIMER3n_Type* )TIMER30, ( uint16_t )( ( ( uint32_t )(HCLK/CLK_DIV) / ( PWM_HZ ) ) / 1 )+1 );
//   HAL_TIMER3n_SetPeriod( ( TIMER3n_Type* )TIMER30, 100 );
   
   HAL_TIMER3n_OutputCtrl( ( TIMER3n_Type* )TIMER30, ENABLE, 1<<14, 0 );

   HAL_TIMER3n_ConfigInterrupt( ( TIMER3n_Type* )TIMER30, ENABLE, TIMER3n_INT_PMATEN );
   HAL_TIMER3n_ClearStatus_IT( ( TIMER3n_Type* )TIMER30, 0x7F );

//   HAL_TIMER3n_SetADCTrigger( ( TIMER3n_Type* )TIMER30, TIMER3n_ADT_BTTGEN, 0 );

   HAL_TIMER3n_MPWMCmd( ( TIMER3n_Type* )TIMER30, TIMER3n_UPMATCH, TIMER3n_E1PERIOD );

//   HAL_TIMER3n_SetDelayTime( ( TIMER3n_Type* )TIMER30, TIMER3n_DLYINSEN, TIMER3n_INSFRONT, 79 );    // (79 + 1) / 40MHz = 2us

   HAL_TIMER3n_Start( ( TIMER3n_Type* )TIMER30, TIMER3n_ENABLE );

   NVIC_EnableIRQ( TIMER30_IRQn );
   HAL_INT_EInt_MaskDisable( MSK_TIMER30 );
}

/*-------------------------------------------------------------------------*//**
 * @brief         WDT_Configure - Watchdog Timer (3sec timeout)
 * @param         None
 * @return        None
 *//*-------------------------------------------------------------------------*/
void WDT_Configure( void )
{
   WDT_CFG_Type WDT_Config;

   WDT_Config.wdtResetEn   = ENABLE;        // WDT underflow -> System Reset
   WDT_Config.wdtClkDiv    = WDT_DIV_256;   // WDTRC(40kHz) / 256 = 156Hz
   WDT_Config.wdtTmrConst  = 468;           // 468 / 156Hz = 3sec timeout
   WDT_Config.wdtWTmrConst = 0xFFFFFF;      // Window disabled (max value)

   HAL_WDT_Init( &WDT_Config );
}

