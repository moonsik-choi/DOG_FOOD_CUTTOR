/**
******************************************************************************
* @file    DAC/TwoChannels_TriangleWave/main.c 
* @author  MCD Application Team
* @version V3.5.0
* @date    08-April-2011
* @brief   Main program body.
******************************************************************************
* @attention
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
******************************************************************************
*/ 

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "main_conf.h"
#include "ks_BITBAND.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Init Structure definition */
//TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern uint8_t	u8Flag_FMode;	
/* Private function prototypes -----------------------------------------------*/
void Process(void);
void Initial(void);
void RCC_Configuration(void);
void GPIO_Configuration(void);
void Delay(__IO uint32_t nCount);

/* Private functions ---------------------------------------------------------*/

/**
* @brief   Main program.
* @param  None
* @retval None
*/

void main(void)
{
	
	__disable_irq();
	
	/* System Clocks Configuration */
	RCC_Configuration();   
	Initial();
	
	__enable_irq();
	while (1)
	{
		
//		PA_ODR0 = 1;
		Process();
	}
}

/**
* @brief  Configures the different system clocks.
* @param  None
* @retval None
*/
void RCC_Configuration(void)
{   
	/* Enable peripheral clocks ------------------------------------------------*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
}

/**
* @brief  Inserts a delay time.
* @param  nCount: specifies the delay time length.
* @retval None
*/
void Delay(__IO uint32_t nCount)
{
	for(; nCount != 0; nCount--);
}

#ifdef  USE_FULL_ASSERT

/**
* @brief  Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param  file: pointer to the source file name
* @param  line: assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t* file, uint32_t line)
{ 
	/* User can add his own implementation to report the file name and line number,
	ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	
	/* Infinite loop */
	while (1)
	{
	}
}

#endif

/**
* @}
*/ 

uint16_t	ADC_Conversion(uint32_t	ch)
{
	ADC1->CHSELR = (1<<ch);
	ADC1_START(1);
	while(!ADC1_EOCF);
	return (ADC1->DR >> 0);
}

void Initial(void)
{
//ADC
	ADC1->CR = ADC_CR_ADEN;
	ADC1->CR = /*ADC_CR_ADCAL |*/ ADC_CR_ADEN;					// SWStart, ADC Enable
//	while(ADC1->CR & ADC_CR_ADCAL);			// Wait
	ADC1->CFGR1 = ((0 << 26) | (0<3));		// 12bit
	ADC1->CFGR2 = 0x00000000;
	ADC1->SMPR = 0x01;
	
//Timer		1ms
	TIM3->CR1 = 0x0000;
	TIM3->CR2 = 0x0000;
	TIM3->SMCR = 0x0000;
	TIM3->DIER = 0x0001;
	
	TIM3->ARR = 1000;
	TIM3->PSC = 48-1;				// 1us
	
	TIM3->CR1 |= 0x01;							// Timer Start
	NVIC_SetPriority((IRQn_Type)TIM3_IRQn, 2);
	NVIC_EnableIRQ((IRQn_Type)TIM3_IRQn);

//Timer 30us
	TIM14->CR1 = 0x0000;
	TIM14->CR2 = 0x0000;
	TIM14->SMCR = 0x0000;
	TIM14->DIER = 0x0001;
	
	TIM14->ARR = 10*5;
	TIM14->PSC = 48-1;				// 1us
	
	TIM14->CR1 |= 0x01;							// Timer Start
	NVIC_SetPriority((IRQn_Type)TIM14_IRQn, 0);
	NVIC_EnableIRQ((IRQn_Type)TIM14_IRQn);

////Timer		PWM 10KHz
//	TIM14->CR1 = 0x0000;
//	TIM14->CR2 = 0x0000;
//	TIM14->SMCR = 0x0000;
//	TIM14->DIER = 0x0001;
//	
//	TIM14->ARR = 100;				// 1us * 100 = 0.1ms(10KHz)
//	TIM14->PSC = 8-1;				// 1us
//	TIM14->CCMR1 = 0x68;
//	TIM14->CCER = 0x09;
////	TIM14->BDTR = 0x8000;		// Set MOE
//	
//	TIM14->CR1 |= 0x01;							// Timer Start
//	
//	NVIC_SetPriority((IRQn_Type)TIM14_IRQn, 3);
//	NVIC_EnableIRQ((IRQn_Type)TIM14_IRQn);
////	NVIC_SetPendingIRQ((IRQn_Type)TIM14_IRQn);		// main roof 시작 전 TIM14 Interrupt를 실행시켜서 AD값을 읽어오도록 함.

//UART
	// Configure the USART1 peripheral
	USART1->CR1 &= ~USART_CR1_UE;  						// Disable USART1
	
	// Set the baud rate to 9600 bps
	USART1->BRR = 48000000 / 19200;  					// Assuming UART clock is 48MHz
	
//	USART1->CR1 |= USART_CR1_TE | USART_CR1_RE;  		// Enable transmitter and receiver
	USART1->CR1 |= USART_CR1_UE;  						// Enable USART1
//	NVIC_SetPriority((IRQn_Type)USART1_IRQn, 3);
//	NVIC_EnableIRQ((IRQn_Type)USART1_IRQn);	
//PORT
	
	
	/*
	GPIO port mode register MODER (MODER[1:0]=00):
	00: Input mode (reset state)
	01: General purpose output mode
	10: Alternate function mode
	11: Analog mode
	GPIO port output type register OTYPER
	0: Output push-pull (reset state)
	1: Output open-drain
	GPIO port output speed register OSPEEDR
	x0: Low speed
	01: Medium speed
	11: High speed
	GPIO port pull-up/pull-down register PUPDR
	00: No pull-up, pull-down
	01: Pull-up
	10: Pull-down
	11: Reserved
	*/
	
	/* Once the DAC channel is enabled, the corresponding GPIO pin is automatically 
	connected to the DAC converter. In order to avoid parasitic consumption, 
	the GPIO pin should be configured in analog */

//	GPIOA->MODER 	= 0x28003A57;			// -FF- -II- IAFF OOOA	//USART6
	GPIOA->MODER 	= 0x28283057;			// -FF- -FF- IAII OOOA  //USART1
	GPIOA->OTYPER 	= 0x0000;				// All Push-pull
	GPIOA->OSPEEDR	= 0x28555555;
	GPIOA->PUPDR 	= 0x24008A00;			// PA7, PA5, PA4:PDN
	GPIOA->ODR 		= 0x0000;				// LED All OFF
//	GPIOA->AFR[0]	= 0 | 5<<20 | 5<<16;	// PA5:AF5 UART6_RX, PA4:AF5:UART6_TX	//USART6
	GPIOA->AFR[1]	= 0 | 1<<8 | 1<<4;		// PA10:AF5 UART1_RX, PA9:AF5:UART1_TX	//USART1
	
	GPIOB->MODER 	= 0<<2;					// ---- ---- ---- --I- 
	GPIOB->PUPDR 	= 2<<2;					// PB1:PDN

	GPIOB->MODER 	= 1<<2 | 0<<0;			// ---- ---- ---- --OI 
	GPIOF->PUPDR 	= 0<<2 | 2<<0;			// PF0:PDN


}



void TIMER10_Configure( void )
{
   count = 0;
   flag = 0;

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


