/**
  ******************************************************************************
  * @file    TIM/TIM_TimeBase/stm32f0xx_it.c 
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    24-July-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F0xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup TIM_TimeBase
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern uint16_t	gu16ADC_curr_value;
extern uint16_t	gu16ADC_batt_value;
extern __IO uint16_t CCR1_Val;
extern __IO uint16_t CCR2_Val;
extern __IO uint16_t CCR3_Val;
extern __IO uint16_t CCR4_Val;
/* Private function prototypes -----------------------------------------------*/
extern uint16_t AD_CHK_2(uint8_t);
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
//  while (1)
//  {
//  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/******************************************************************************/
/**
  * @brief  This function handles TIM3 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM3_IRQHandler(void)
{
	TIM3->SR = 0;
	Flag_1ms = 1;
}

#define 	DF_LED_RED		1
#define 	DF_LED_GRN		2
#define 	DF_LED_BLU		4
void Disp_Led_Control(uint32_t u32LED, uint32_t on_time, uint32_t* Counter);
extern uint32_t gu32RedBR;
extern uint32_t gu32GrnBR;
extern uint32_t gu32BluBR;
extern uint32_t gu32DispLED_Red_BR_Cnt;
extern uint32_t gu32DispLED_Grn_BR_Cnt;
extern uint32_t gu32DispLED_Blu_BR_Cnt;


void TIM14_IRQHandler(void)		//10us
{
	TIM14->SR = 0;
	Disp_Led_Control(DF_LED_RED, gu32RedBR, &gu32DispLED_Red_BR_Cnt);
	Disp_Led_Control(DF_LED_GRN, gu32GrnBR, &gu32DispLED_Grn_BR_Cnt);
	Disp_Led_Control(DF_LED_BLU, gu32BluBR, &gu32DispLED_Blu_BR_Cnt);
}

void USART1_IRQHandler(void)
{
	// USART1 수신 인터럽트 처리
	if (USART1->ISR & USART_ISR_RXNE) {
		// 수신된 데이터에 대한 처리
		
		D_UART_STAT_CLR = USART_ICR_ORECF;			// Overrun Clear
//		if(D_UART_RX_STAT & USART_ISR_RXNE)	
		{
			uint32_t data = D_UART_RX_BUF;
			if(data == D_STX)
			{
				gu8Index = 0;
			}
			gu8ArrBuf[gu8Index] = data;
			gu8Index++;
			if(gu8Index >= DN_UART_DATA_LEN)
			{
				gu8Index = 0;
				if((gu8ArrBuf[0] == D_STX) && (gu8ArrBuf[DN_UART_DATA_LEN-1] == D_ETX))
				{
					gu8ModeNumComm = gu8ArrBuf[1] & 0x0F;
					gu8SpeedComm = gu8ArrBuf[2];
					gu8RND_NUM = gu8ArrBuf[3];
					if(gu8ArrBuf[1] & 0x80)				// 싱크 플래그 값이 있는 경우.
					{
						gu8_FMode_step1 = 0; gu8_FMode_step2 = 0; gu8BlankFlag = 0;
					}
					else if(gu8ArrBuf[1] & 0x40)		// Mode12 를 위한 Flag
					{
						gu8_FMode_step1 = 2;
					}
					
				}
			}

		}
	}
	
	// USART1 송신 인터럽트 처리
	if (USART1->ISR & USART_ISR_TXE) {
		// 송신 버퍼가 빈 경우에만 데이터 전송 가능
		// 데이터를 송신 버퍼에 쓰면 자동으로 전송됨
		
		if(gu8Index >= DN_UART_DATA_LEN)
		{
//			gu8Index = 0;
			D_UART_CTRL1 &= ~(USART_CR1_TE|USART_CR1_TXEIE);
		}
		else
		{
			D_UART_TX_BUF = gu8ArrBuf[gu8Index++];
		}
		USART1->ICR |= USART_ICR_TCCF;
	}
	USART1->ICR = 0x1FFFF;
}

/******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f0xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
