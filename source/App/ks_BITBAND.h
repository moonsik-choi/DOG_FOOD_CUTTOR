
#define PERI_BASE	PERIPH_BASE

///* Bit-Band for Device Specific Peripheral Registers */
//#define BITBAND_OFFSET (0x02000000UL)
//#define BITBAND_PERI_BASE (PERI_BASE + BITBAND_OFFSET)
//#define BITBAND_PERI(addr, bitnum) (BITBAND_PERI_BASE + (((uint32_t)(addr) - PERI_BASE) << 5) + ((uint32_t)(bitnum) << 2))
//
///* General Purpose Input/Output Port (GPIOA) */
//#define PA_ODR0                           (*((__IO uint32_t *)BITBAND_PERI(&GPIOA->ODR,0)))
//#define PA_ODR1                           (*((__IO uint32_t *)BITBAND_PERI(&GPIOA->ODR,1)))
//#define PA_ODR2                           (*((__IO uint32_t *)BITBAND_PERI(&GPIOA->ODR,2)))
//#define PA_ODR3                           (*((__IO uint32_t *)BITBAND_PERI(&GPIOA->ODR,3)))
//#define PA_ODR4                           (*((__IO uint32_t *)BITBAND_PERI(&GPIOA->ODR,4)))
//#define PA_ODR5                           (*((__IO uint32_t *)BITBAND_PERI(&GPIOA->ODR,5)))
//#define PA_ODR6                           (*((__IO uint32_t *)BITBAND_PERI(&GPIOA->ODR,6)))
//#define PA_ODR7                           (*((__IO uint32_t *)BITBAND_PERI(&GPIOA->ODR,7)))
//#define PA_ODR8                           (*((__IO uint32_t *)BITBAND_PERI(&GPIOA->ODR,8)))
//#define PA_ODR9                           (*((__IO uint32_t *)BITBAND_PERI(&GPIOA->ODR,9)))
//#define PA_ODR10                          (*((__IO uint32_t *)BITBAND_PERI(&GPIOA->ODR,10)))
//#define PA_ODR11                          (*((__IO uint32_t *)BITBAND_PERI(&GPIOA->ODR,11)))
//#define PA_ODR12                          (*((__IO uint32_t *)BITBAND_PERI(&GPIOA->ODR,12)))
//#define PA_ODR13                          (*((__IO uint32_t *)BITBAND_PERI(&GPIOA->ODR,13)))
//#define PA_ODR14                          (*((__IO uint32_t *)BITBAND_PERI(&GPIOA->ODR,14)))
//#define PA_ODR15                          (*((__IO uint32_t *)BITBAND_PERI(&GPIOA->ODR,15)))
//
//#define PA_IDR0                           (*((__I uint32_t *)BITBAND_PERI(&GPIOA->IDR,0)))
//#define PA_IDR1                           (*((__I uint32_t *)BITBAND_PERI(&GPIOA->IDR,1)))
//#define PA_IDR2                           (*((__I uint32_t *)BITBAND_PERI(&GPIOA->IDR,2)))
//#define PA_IDR3                           (*((__I uint32_t *)BITBAND_PERI(&GPIOA->IDR,3)))
//#define PA_IDR4                           (*((__I uint32_t *)BITBAND_PERI(&GPIOA->IDR,4)))
//#define PA_IDR5                           (*((__I uint32_t *)BITBAND_PERI(&GPIOA->IDR,5)))
//#define PA_IDR6                           (*((__I uint32_t *)BITBAND_PERI(&GPIOA->IDR,6)))
//#define PA_IDR7                           (*((__I uint32_t *)BITBAND_PERI(&GPIOA->IDR,7)))
//#define PA_IDR8                           (*((__I uint32_t *)BITBAND_PERI(&GPIOA->IDR,8)))
//#define PA_IDR9                           (*((__I uint32_t *)BITBAND_PERI(&GPIOA->IDR,9)))
//#define PA_IDR10                          (*((__I uint32_t *)BITBAND_PERI(&GPIOA->IDR,10)))
//#define PA_IDR11                          (*((__I uint32_t *)BITBAND_PERI(&GPIOA->IDR,11)))
//#define PA_IDR12                          (*((__I uint32_t *)BITBAND_PERI(&GPIOA->IDR,12)))
//#define PA_IDR13                          (*((__I uint32_t *)BITBAND_PERI(&GPIOA->IDR,13)))
//#define PA_IDR14                          (*((__I uint32_t *)BITBAND_PERI(&GPIOA->IDR,14)))
//#define PA_IDR15                          (*((__I uint32_t *)BITBAND_PERI(&GPIOA->IDR,15)))

/* General Purpose Input/Output Port (GPIOB) */
//#define PB_ODR0                           (*((__IO uint32_t *)BITBAND_PERI(&GPIOB->ODR,0)))
//#define PB_ODR1                           (*((__IO uint32_t *)BITBAND_PERI(&GPIOB->ODR,1)))
//#define PB_ODR2                           (*((__IO uint32_t *)BITBAND_PERI(&GPIOB->ODR,2)))
//#define PB_ODR3                           (*((__IO uint32_t *)BITBAND_PERI(&GPIOB->ODR,3)))
//#define PB_ODR4                           (*((__IO uint32_t *)BITBAND_PERI(&GPIOB->ODR,4)))
//#define PB_ODR5                           (*((__IO uint32_t *)BITBAND_PERI(&GPIOB->ODR,5)))
//#define PB_ODR6                           (*((__IO uint32_t *)BITBAND_PERI(&GPIOB->ODR,6)))
//#define PB_ODR7                           (*((__IO uint32_t *)BITBAND_PERI(&GPIOB->ODR,7)))
//#define PB_ODR8                           (*((__IO uint32_t *)BITBAND_PERI(&GPIOB->ODR,8)))
//#define PB_ODR9                           (*((__IO uint32_t *)BITBAND_PERI(&GPIOB->ODR,9)))
//#define PB_ODR10                          (*((__IO uint32_t *)BITBAND_PERI(&GPIOB->ODR,10)))
//#define PB_ODR11                          (*((__IO uint32_t *)BITBAND_PERI(&GPIOB->ODR,11)))
//#define PB_ODR12                          (*((__IO uint32_t *)BITBAND_PERI(&GPIOB->ODR,12)))
//#define PB_ODR13                          (*((__IO uint32_t *)BITBAND_PERI(&GPIOB->ODR,13)))
//#define PB_ODR14                          (*((__IO uint32_t *)BITBAND_PERI(&GPIOB->ODR,14)))
//#define PB_ODR15                          (*((__IO uint32_t *)BITBAND_PERI(&GPIOB->ODR,15)))
//
//#define PB_IDR0                           (*((__I uint32_t *)BITBAND_PERI(&GPIOB->IDR,0)))
//#define PB_IDR1                           (*((__I uint32_t *)BITBAND_PERI(&GPIOB->IDR,1)))
//#define PB_IDR2                           (*((__I uint32_t *)BITBAND_PERI(&GPIOB->IDR,2)))
//#define PB_IDR3                           (*((__I uint32_t *)BITBAND_PERI(&GPIOB->IDR,3)))
//#define PB_IDR4                           (*((__I uint32_t *)BITBAND_PERI(&GPIOB->IDR,4)))
//#define PB_IDR5                           (*((__I uint32_t *)BITBAND_PERI(&GPIOB->IDR,5)))
//#define PB_IDR6                           (*((__I uint32_t *)BITBAND_PERI(&GPIOB->IDR,6)))
//#define PB_IDR7                           (*((__I uint32_t *)BITBAND_PERI(&GPIOB->IDR,7)))
//#define PB_IDR8                           (*((__I uint32_t *)BITBAND_PERI(&GPIOB->IDR,8)))
//#define PB_IDR9                           (*((__I uint32_t *)BITBAND_PERI(&GPIOB->IDR,9)))
//#define PB_IDR10                          (*((__I uint32_t *)BITBAND_PERI(&GPIOB->IDR,10)))
//#define PB_IDR11                          (*((__I uint32_t *)BITBAND_PERI(&GPIOB->IDR,11)))
//#define PB_IDR12                          (*((__I uint32_t *)BITBAND_PERI(&GPIOB->IDR,12)))
//#define PB_IDR13                          (*((__I uint32_t *)BITBAND_PERI(&GPIOB->IDR,13)))
//#define PB_IDR14                          (*((__I uint32_t *)BITBAND_PERI(&GPIOB->IDR,14)))
//#define PB_IDR15                          (*((__I uint32_t *)BITBAND_PERI(&GPIOB->IDR,15)))
//
//#define PC_ODR9                           (*((__IO uint32_t *)BITBAND_PERI(&GPIOC->ODR,9)))
//#define PC_IDR9                           (*((__I uint32_t *)BITBAND_PERI(&GPIOC->IDR,9)))


#ifdef	STM32F030
#define ADC1_EOCF          ((ADC1->ISR >> 2)&1)//                (*((__I uint32_t *)BITBAND_PERI(&ADC1->ISR,2)))
#define ADC1_START(x)         ADC1->CR |= (x<<2)//                (*((__IO uint32_t *)BITBAND_PERI(&ADC1->CR,2)))


#define SET_BIT(REG, BIT)     ((REG) |= (BIT))
#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))

#define READ_BIT(REG, BIT)    ((REG) & (BIT))
#define CLEAR_REG(REG)        ((REG) = (0x0))
#define WRITE_REG(REG, VAL)   ((REG) = (VAL))
#define READ_REG(REG)         ((REG))
#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))
#endif





/* End of file */

