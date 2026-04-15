

/* ABOV H/W Define */
#define ADC_START(n)					ADC->CR_b.ADST = (n)
#define ADC_SET_CHANNEL(n)				ADC->CR_b.ADSEL = (n)
#define ADC_EOCF()						(ADC->CR_b.ADCIFLAG == 0)
#define ADC_GET_DATA()					ADC->DR

#define DPI_GET_BIT(pt,bit)				(((pt)->INDR >> (bit))&1)

#define DPO_SET_BIT(pt,bit)				((pt)->BSR = (1<<(bit)))
#define DPO_CLR_BIT(pt,bit)				((pt)->BCR = (1<<(bit)))

/* User Define */
#define	D_KEY_ON_LEVEL					1									// Key입력시 Port에 입력되는 레벨.
//#define	DEF_SET_DUTY(n)					TIM14->CCR1 = (n)				// Duty가변 추가.(PWM)

/* Port Input Define */
#define	DPI_KEY							DPI_GET_BIT(PE,1)
//#define	DPI_SW2							DPI_GET_BIT(PE,3)
//#define	DPI_SW3							DPI_GET_BIT(PE,5)
//#define	DPI_SW4							DPI_GET_BIT(PE,6)
//#define	DPI_232_MODE					DPI_GET_BIT(PE,7)

/* AD Input Define */
#define DAI_CH_CURRENT					7

/* Port Output Define */
#define DPO_LED_ON						DPO_SET_BIT(PE,0)
#define DPO_LED_OFF						DPO_CLR_BIT(PE,0)


#define DPO_LED_PWR_ON					DPO_CLR_BIT(PD,2)
#define DPO_LED_PWR_OFF					DPO_SET_BIT(PD,2)


extern uint8_t	Flag_1ms;


/* End of file */

