/**
 * @file stm8l15x_ipt.h
 * Interrupt declarations.
 */
#ifndef _STM8L15x_IPT_H_
#define _STM8L15x_IPT_H_

// --- Include section ---------------------------------------------------------

// --- Definitions -------------------------------------------------------------

#define IPT_TRAP            0   //!< TRAP interrupt
#define IPT_FLASH           1   //!< FLASH EOP/PG_DIS interrupt
#define IPT_DMA1_CHN01      2   //!< DMA1 channel 0/1 interrupt
#define IPT_DMA1_CHN23      3   //!< DMA1 channel 2/3 interrupt
#define IPT_RTC_CSSLSE      4   //!< RTC /CSS_LSE interrupt
#define IPT_EXTI_PORT_EF    5   //!< EXTI PORTE / EXTI PORTF/PVD interrupt
#define IPT_EXTI_PORT_BG    6   //!< EXTI PORTB / EXTI PORTG interrupt
#define IPT_EXTI_PORT_DH    7   //!< EXTI PORTD / EXTI PORTH interrupt
#define IPT_EXTI_PIN0       8   //!< EXTI pin 0 interrupt
#define IPT_EXTI_PIN1       9   //!< EXTI pin 1 interrupt
#define IPT_EXTI_PIN2       10  //!< EXTI pin 2 interrupt
#define IPT_EXTI_PIN3       11  //!< EXTI pin 3 interrupt
#define IPT_EXTI_PIN4       12  //!< EXTI pin 4 interrupt
#define IPT_EXTI_PIN5       13  //!< EXTI pin 5 interrupt
#define IPT_EXTI_PIN6       14  //!< EXTI pin 6 interrupt
#define IPT_EXTI_PIN7       15  //!< EXTI pin 7 interrupt
#define IPT_LCD_AES         16  //!< LCD / AES interrupt
#define IPT_SWITCH_CSS_BREAK_DAC    17  //!< Switch CLK/CSS/TIM1 Break/DAC interrupt
#define IPT_ADC1_COMP               18  //!< ADC1 / COMP interrupt
#define IPT_TIM2_UPD_OVF_TRG_BRK_USART2_TX 19  //!< TIM2 UPD/OVF/TRG/BRK / USART2 TX interrupt
#define IPT_TIM2_CC_USART2_RX       20  //!< TIM2 CAP / USART2 RX interrupt
#define IPT_TIM3_UPD_OVF_TRG_BRK_USART3_TX 21  //!< TIM3 UPD/OVF/TRG/BRK /USART3 TX interrupt
#define IPT_TIM3_CC_USART3_RX       22  //!< TIM3 CAP/ USART3 RX interrupt
#define IPT_TIM1_UPD_OVF_TRG_COM    23  //!< TIM1 UPD/OVF/TRG/COM interrupt
#define IPT_TIM1_CC                 24  //!< TIM1 CAP interrupt
#define IPT_TIM4_UPD_OVF            25  //!< TIM4 UPD / OVF interrupt
#define IPT_SPI1                    26  //!< SPI1 interrupt
#define IPT_USART1_TX_TIM5_UPD_OVF_TRG_BRK 27  //!< USART1 TX / TIM5 UPD/OVF/TRG/BRK interrupt
#define IPT_USART1_RX_TIM5_CC       28  //!< USART1 RX / TIM5 CAP interrupt
#define IPT_I2C1_SPI2               29  //!< I2C1 / SPI2 interrupt

// --- Type definitions --------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif /* _STM8L15x_IPT_H_ */
