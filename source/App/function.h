

#define D_UART_CH				USART10
//#define D_UART_TX_BUF			D_UART_CH->DR
//#define D_UART_RX_BUF			D_UART_CH->DR
//#define M_UART_TBEMP()			((D_UART_CH->ST & USART1n_ST_DREn_Msk) == 0x80)		// 0 : Not empty, 1: Empty
//#define M_UART_RBFLL()			((D_UART_CH->ST & USART1n_ST_RXCn_Msk) == 0x20)		// 0 : Not full, 1: Buffer full
//#define M_UART_SET_TXE()		(D_UART_CH->CR1 |= USART1n_CR1_TXEn_Msk)
//#define M_UART_SET_RXE()		(D_UART_CH->CR1 |= USART1n_CR1_RXEn_Msk)
//#define M_UART_CLR_TXE()		(D_UART_CH->CR1 &= (~USART1n_CR1_TXEn_Msk))
//#define M_UART_CLR_RXE()		(D_UART_CH->CR1 &= (~USART1n_CR1_RXEn_Msk))
//#define D_UART_STATUS			D_UART_CH->ST
//	#define D_UART_CTRL1			D_UART_CH->CR1
//	#define DN_UART_DATA_LEN		5
//#define D_STX					0xAA
//#define D_ETX					0x55


extern uint8_t gu8_ADC_Flag;
extern uint8_t gu8Speed;

