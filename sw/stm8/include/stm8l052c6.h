/* STM8L052C6.h */

/* Copyright (c) 2003-2017 STMicroelectronics */

#ifndef __STM8L052C6__
#define __STM8L052C6__

/* STM8L052C6 */

/* functions */
#define enableInterrupts()    {__asm__("rim\n");}    // enable interrupts
#define disableInterrupts()   {__asm__("sim\n");}    // disable interrupts
#define iret()                {__asm__("iret\n");}   // Interrupt routine return
#define pop_ccr()             {__asm__("pop cc\n");} // Pop CCR from the stack
#define push_ccr()            {__asm__("push cc\n");}// Push CCR on the stack
#define rim()                 {__asm__("rim\n");}    // enable interrupts
#define sim()                 {__asm__("sim\n");}    // disable interrupts
#define nop()                 {__asm__("nop\n");}    // No Operation
#define trap()                {__asm__("trap\n");}   // Trap (soft IT)
#define wfi()                 {__asm__("wfi\n");}    // Wait For Interrupt
#define halt()                {__asm__("halt\n");}   // Halt


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

/* Port A */
/*****************************************************************/

//! Port A data output latch register
#define PA_ODR (*(volatile uint8_t *)0x5000)

//! Port A input pin value register
#define PA_IDR (*(volatile uint8_t *)0x5001)

//! Port A data direction register
#define PA_DDR (*(volatile uint8_t *)0x5002)

//! Port A control register 1
#define PA_CR1 (*(volatile uint8_t *)0x5003)

//! Port A control register 2
#define PA_CR2 (*(volatile uint8_t *)0x5004)

/* Port B */
/*****************************************************************/

//! Port B data output latch register
#define PB_ODR (*(volatile uint8_t *)0x5005)

//! Port B input pin value register
#define PB_IDR (*(volatile uint8_t *)0x5006)

//! Port B data direction register
#define PB_DDR (*(volatile uint8_t *)0x5007)

//! Port B control register 1
#define PB_CR1 (*(volatile uint8_t *)0x5008)

//! Port B control register 2
#define PB_CR2 (*(volatile uint8_t *)0x5009)

/* Port C */
/*****************************************************************/

//! Port C data output latch register
#define PC_ODR (*(volatile uint8_t *)0x500a)

//! Port C input pin value register
#define PC_IDR (*(volatile uint8_t *)0x500b)

//! Port C data direction register
#define PC_DDR (*(volatile uint8_t *)0x500c)

//! Port C control register 1
#define PC_CR1 (*(volatile uint8_t *)0x500d)

//! Port C control register 2
#define PC_CR2 (*(volatile uint8_t *)0x500e)

/* Port D */
/*****************************************************************/

//! Port D data output latch register
#define PD_ODR (*(volatile uint8_t *)0x500f)

//! Port D input pin value register
#define PD_IDR (*(volatile uint8_t *)0x5010)

//! Port D data direction register
#define PD_DDR (*(volatile uint8_t *)0x5011)

//! Port D control register 1
#define PD_CR1 (*(volatile uint8_t *)0x5012)

//! Port D control register 2
#define PD_CR2 (*(volatile uint8_t *)0x5013)

/* Port E */
/*****************************************************************/

//! Port E data output latch register
#define PE_ODR (*(volatile uint8_t *)0x5014)

//! Port E input pin value register
#define PE_IDR (*(volatile uint8_t *)0x5015)

//! Port E data direction register
#define PE_DDR (*(volatile uint8_t *)0x5016)

//! Port E control register 1
#define PE_CR1 (*(volatile uint8_t *)0x5017)

//! Port E control register 2
#define PE_CR2 (*(volatile uint8_t *)0x5018)

/* Port F */
/*****************************************************************/

//! Port F data output latch register
#define PF_ODR (*(volatile uint8_t *)0x5019)

//! Port F input pin value register
#define PF_IDR (*(volatile uint8_t *)0x501a)

//! Port F data direction register
#define PF_DDR (*(volatile uint8_t *)0x501b)

//! Port F control register 1
#define PF_CR1 (*(volatile uint8_t *)0x501c)

//! Port F control register 2
#define PF_CR2 (*(volatile uint8_t *)0x501d)

#define PIN_0   ((uint8_t)(1<<0))   //!< Port pin 0
#define PIN_1   ((uint8_t)(1<<1))   //!< Port pin 1
#define PIN_2   ((uint8_t)(1<<2))   //!< Port pin 2
#define PIN_3   ((uint8_t)(1<<3))   //!< Port pin 3
#define PIN_4   ((uint8_t)(1<<4))   //!< Port pin 4
#define PIN_5   ((uint8_t)(1<<5))   //!< Port pin 5
#define PIN_6   ((uint8_t)(1<<6))   //!< Port pin 6
#define PIN_7   ((uint8_t)(1<<7))   //!< Port pin 7

/* Flash */
/*****************************************************************/

//! Flash control register 1
#define FLASH_CR1 (*(volatile uint8_t *)0x5050)

//! Flash control register 2
#define FLASH_CR2 (*(volatile uint8_t *)0x5051)

//! Flash Program memory unprotection register
#define FLASH_PUKR (*(volatile uint8_t *)0x5052)

//! Data EEPROM unprotection register
#define FLASH_DUKR (*(volatile uint8_t *)0x5053)

//! Flash in-application programming status register
#define FLASH_IAPSR (*(volatile uint8_t *)0x5054)

/* Direct memory access controller 1 (DMA1) */
/*****************************************************************/

//! DMA1 global configuration & status register
#define DMA1_GCSR (*(volatile uint8_t *)0x5070)

//! DMA1 global interrupt register 1
#define DMA1_GIR1 (*(volatile uint8_t *)0x5071)

//! DMA1 channel 0 configuration register
#define DMA1_C0CR (*(volatile uint8_t *)0x5075)

//! DMA1 channel 0 status & priority register
#define DMA1_C0SPR (*(volatile uint8_t *)0x5076)

//! DMA1 number of data to transfer register (channel 0)
#define DMA1_C0NDTR (*(volatile uint8_t *)0x5077)

//! DMA peripheral address high register (channel 0)
#define DMA1_C0PARH (*(volatile uint8_t *)0x5078)

//! DMA peripheral address low register (channel 0)
#define DMA1_C0PARL (*(volatile uint8_t *)0x5079)

//! DMA memory address high register (channel 0)
#define DMA1_C0M0ARH (*(volatile uint8_t *)0x507b)

//! DMA memory address low register (channel 0)
#define DMA1_C0M0ARL (*(volatile uint8_t *)0x507c)

//! DMA1 channel 1 configuration register
#define DMA1_C1CR (*(volatile uint8_t *)0x507f)

//! DMA1 channel 1 status & priority register
#define DMA1_C1SPR (*(volatile uint8_t *)0x5080)

//! DMA1 number of data to transfer register (channel 1)
#define DMA1_C1NDTR (*(volatile uint8_t *)0x5081)

//! DMA peripheral address high register (channel 1)
#define DMA1_C1PARH (*(volatile uint8_t *)0x5082)

//! DMA peripheral address low register (channel 1)
#define DMA1_C1PARL (*(volatile uint8_t *)0x5083)

//! DMA memory address high register (channel 1)
#define DMA1_C1M0ARH (*(volatile uint8_t *)0x5085)

//! DMA memory address low register (channel 1)
#define DMA1_C1M0ARL (*(volatile uint8_t *)0x5086)

//! DMA1 channel 2 configuration register
#define DMA1_C2CR (*(volatile uint8_t *)0x5089)

//! DMA1 channel 2 status & priority register
#define DMA1_C2SPR (*(volatile uint8_t *)0x508a)

//! DMA1 number of data to transfer register (channel 2)
#define DMA1_C2NDTR (*(volatile uint8_t *)0x508b)

//! DMA peripheral address high register (channel 2)
#define DMA1_C2PARH (*(volatile uint8_t *)0x508c)

//! DMA peripheral address low register (channel 2)
#define DMA1_C2PARL (*(volatile uint8_t *)0x508d)

//! DMA memory address high register (channel 2)
#define DMA1_C2M0ARH (*(volatile uint8_t *)0x508f)

//! DMA memory address low register (channel 2)
#define DMA1_C2M0ARL (*(volatile uint8_t *)0x5090)

//! DMA1 channel 3 configuration register
#define DMA1_C3CR (*(volatile uint8_t *)0x5093)

//! DMA1 channel 3 status & priority register
#define DMA1_C3SPR (*(volatile uint8_t *)0x5094)

//! DMA1 number of data to transfer register (channel 3)
#define DMA1_C3NDTR (*(volatile uint8_t *)0x5095)

//! DMA1 peripheral address high register (channel 3)
#define DMA1_C3PARH_C3M1ARH (*(volatile uint8_t *)0x5096)

//! DMA1 peripheral address low register (channel 3)
#define DMA1_C3PARL_C3M1ARL (*(volatile uint8_t *)0x5097)

//! DMA memory address high register (channel 3)
#define DMA1_C3M0ARH (*(volatile uint8_t *)0x5099)

//! DMA memory address low register (channel 3)
#define DMA1_C3M0ARL (*(volatile uint8_t *)0x509a)

/* System configuration (SYSCFG) */
/*****************************************************************/

//! Remapping register 1
#define SYSCFG_RMPCR1 (*(volatile uint8_t *)0x509e)

#define SYSCFG_RMPCR1_USART1TXRXPORTA   ((uint8_t)0x10)    //!< USART1 Tx- Rx (PC3- PC2) remapping to PA2- PA3
#define SYSCFG_RMPCR1_USART1TXRXPORTC   ((uint8_t)0x20)    //!< USART1 Tx- Rx (PC3- PC2) remapping to PC5- PC6
#define SYSCFG_RMPCR1_USART1TXRXMASK    ((uint8_t)0x30)
#define SYSCFG_RMPCR1_USART1CLK         ((uint16_t)0x40)    //!< USART1 CK (PC4) remapping to PA0
#define SYSCFG_RMPCR1_SPI1FULL          ((uint16_t)0x80)    //!< SPI1 MISO- MOSI- SCK- NSS(PB7- PB6- PB5- PB4)
                                                            //!< remapping to PA2- PA3- PC6- PC5

//! Remapping register 2
#define SYSCFG_RMPCR2 (*(volatile uint8_t *)0x509f)

#define SYSCFG_RMPCR2_ADC1ExtTRIG1      ((uint16_t)0x0201)  //!< ADC1 External Trigger 1 (PA6) remapping to PD0
#define SYSCFG_RMPCR2_TIM2TRIGPortA     ((uint16_t)0x0202)  //!< TIM2 Trigger (PB3) remapping to PA4
#define SYSCFG_RMPCR2_TIM3TRIGPortA     ((uint16_t)0x0204)  //!< TIM3 Trigger (PD1) remapping to PA5
#define SYSCFG_RMPCR2_TIM2TRIGLSE       ((uint16_t)0x0208)  //!< TIM2 Trigger remapping to LSE
#define SYSCFG_RMPCR2_TIM3TRIGLSE       ((uint16_t)0x0210)  //!< TIM3 Trigger remapping to LSE
#define SYSCFG_RMPCR2_SPI2Full          ((uint16_t)0x0220)  //!< SPI2 MISO- MOSI- SCK- NSS(PG7- PG6- PG5- PG4)
                                                            //!< remapping to PI3- PI2- PI1- PI0
#define SYSCFG_RMPCR2_TIM3TRIGPortG     ((uint16_t)0x0240)  //!< TIM3 Trigger (PD1) remapping to PG3
#define SYSCFG_RMPCR2_TIM23BKIN         ((uint16_t)0x0280)  //!< TIM2 Break Input (PA4) remapping to PG0
                                                            //!< and TIM3 Break Input (PA5) remapping to PG1


/* External Interrupt Control Register (ITC) */
/*****************************************************************/

//! External interrupt control register 1
#define EXTI_CR1 (*(volatile uint8_t *)0x50a0)

//! External interrupt control register 2
#define EXTI_CR2 (*(volatile uint8_t *)0x50a1)

//! External interrupt control register 3
#define EXTI_CR3 (*(volatile uint8_t *)0x50a2)

//! External interrupt status register 1
#define EXTI_SR1 (*(volatile uint8_t *)0x50a3)

//! External interrupt status register 2
#define EXTI_SR2 (*(volatile uint8_t *)0x50a4)

//! External interrupt port select register
#define EXTI_CONF (*(volatile uint8_t *)0x50a5)

/* Wait For Event (WFE) */
/*****************************************************************/

//! WFE control register 1
#define WFE_CR1 (*(volatile uint8_t *)0x50a6)

//! WFE control register 2
#define WFE_CR2 (*(volatile uint8_t *)0x50a7)

//! WFE control register 3
#define WFE_CR3 (*(volatile uint8_t *)0x50a8)

/* Reset (RST) */
/*****************************************************************/

//! Reset control register
#define RST_CR (*(volatile uint8_t *)0x50b0)

//! Reset status register
#define RST_SR (*(volatile uint8_t *)0x50b1)

/* Power control (PWR) */
/*****************************************************************/

//! Power control and status register 1
#define PWR_CSR1 (*(volatile uint8_t *)0x50b2)

//! Power control and status register 2
#define PWR_CSR2 (*(volatile uint8_t *)0x50b3)

/* Clock Control (CLK) */
/*****************************************************************/

//! System clock divider register
#define CLK_CKDIVR (*(volatile uint8_t *)0x50c0)

//! Clock RTC register
#define CLK_CRTCR (*(volatile uint8_t *)0x50c1)

//! Internal clock control register
#define CLK_ICKCR (*(volatile uint8_t *)0x50c2)

//! Peripheral clock gating register 1
#define CLK_PCKENR1 (*(volatile uint8_t *)0x50c3)

//! Peripheral clock gating register 2
#define CLK_PCKENR2 (*(volatile uint8_t *)0x50c4)

//! Configurable clock control register
#define CLK_CCOR (*(volatile uint8_t *)0x50c5)

//! External clock control register
#define CLK_ECKCR (*(volatile uint8_t *)0x50c6)

//! System clock status register
#define CLK_SCSR (*(volatile uint8_t *)0x50c7)

//! System clock switch register
#define CLK_SWR (*(volatile uint8_t *)0x50c8)

//! Clock switch control register
#define CLK_SWCR (*(volatile uint8_t *)0x50c9)

//! Clock security system register
#define CLK_CSSR (*(volatile uint8_t *)0x50ca)

//! Clock BEEP register
#define CLK_CBEEPR (*(volatile uint8_t *)0x50cb)

//! HSI calibration register
#define CLK_HSICALR (*(volatile uint8_t *)0x50cc)

//! HSI clock calibration trimming register
#define CLK_HSITRIMR (*(volatile uint8_t *)0x50cd)

//! HSI unlock register
#define CLK_HSIUNLCKR (*(volatile uint8_t *)0x50ce)

//! Main regulator control status register
#define CLK_REGCSR (*(volatile uint8_t *)0x50cf)

#define CLK_CKDIV_128   	(uint8_t)0x07   //!< System clock divider: 128
#define CLK_CKDIV_64    	(uint8_t)0x06   //!< System clock divider: 64
#define CLK_CKDIV_32    	(uint8_t)0x05   //!< System clock divider: 32
#define CLK_CKDIV_16    	(uint8_t)0x04   //!< System clock divider: 16
#define CLK_CKDIV_8     	(uint8_t)0x03   //!< System clock divider: 8
#define CLK_CKDIV_4     	(uint8_t)0x02   //!< System clock divider: 4
#define CLK_CKDIV_2     	(uint8_t)0x01   //!< System clock divider: 2
#define CLK_CKDIV_1     	(uint8_t)0x00   //!< System clock divider: 1

#define CLK_PCKENR1_TIM2    (uint8_t)0x01   //!< Peripheral Clock Enable 1, TIM2
#define CLK_PCKENR1_TIM3    (uint8_t)0x02   //!< Peripheral Clock Enable 1, TIM3
#define CLK_PCKENR1_TIM4    (uint8_t)0x04   //!< Peripheral Clock Enable 1, TIM4
#define CLK_PCKENR1_I2C1    (uint8_t)0x08   //!< Peripheral Clock Enable 1, I2C1
#define CLK_PCKENR1_SPI1    (uint8_t)0x10   //!< Peripheral Clock Enable 1, SPI1
#define CLK_PCKENR1_USART1  (uint8_t)0x20   //!< Peripheral Clock Enable 1, USART1
#define CLK_PCKENR1_BEEP    (uint8_t)0x40   //!< Peripheral Clock Enable 1, BEEP
#define CLK_PCKENR1_DAC     (uint8_t)0x80   //!< Peripheral Clock Enable 1, DAC
#define CLK_PCKENR2_ADC1    (uint8_t)0x01   //!< Peripheral Clock Enable 2, ADC1
#define CLK_PCKENR2_TIM1    (uint8_t)0x02   //!< Peripheral Clock Enable 2, TIM1
#define CLK_PCKENR2_RTC     (uint8_t)0x04   //!< Peripheral Clock Enable 2, RTC
#define CLK_PCKENR2_LCD     (uint8_t)0x08   //!< Peripheral Clock Enable 2, LCD
#define CLK_PCKENR2_DMA1    (uint8_t)0x10   //!< Peripheral Clock Enable 2, DMA1
#define CLK_PCKENR2_COMP    (uint8_t)0x20   //!< Peripheral Clock Enable 2, COMP1 and COMP2
#define CLK_PCKENR2_BOOTROM (uint8_t)0x80   //!< Peripheral Clock Enable 2, Boot ROM

#define CLK_SWR_SRC_LSE 	(1 << 3)    	//!< System clock source LSE
#define CLK_SWR_SRC_HSE 	(1 << 2)        //!< System clock source HSE
#define CLK_SWR_SRC_LSI 	(1 << 1)        //!< System clock source LSI
#define CLK_SWR_SRC_HSI 	(1 << 0)        //!< System clock source HSI

#define CLK_SWCR_SWIF   	(1 << 3)        //!< Clock switch interrupt flag
#define CLK_SWCR_SWIEN  	(1 << 2)        //!< Clock switch interrupt enable
#define CLK_SWCR_SWEN   	(1 << 1)        //!< Switch start/stop
#define CLK_SWCR_SWBSY  	(1 << 0)        //!< Switch busy

#define CLK_RTCCLK_SRC_Off  ((uint8_t)0x00) //!< Clock RTC Off
#define CLK_RTCCLK_SRC_HSI  ((uint8_t)0x02) //!< Clock RTC : HSI
#define CLK_RTCCLK_SRC_LSI  ((uint8_t)0x04) //!< Clock RTC : LSI
#define CLK_RTCCLK_SRC_HSE  ((uint8_t)0x08) //!< Clock RTC : HSE
#define CLK_RTCCLK_SRC_LSE  ((uint8_t)0x10) //!< Clock RTC : LSE

#define CLK_RTCCLK_DIV_1    ((uint8_t)0x00) //!< Clock RTC Div 1
#define CLK_RTCCLK_DIV_2    ((uint8_t)0x20) //!< Clock RTC Div 2
#define CLK_RTCCLK_DIV_4    ((uint8_t)0x40) //!< Clock RTC Div 4
#define CLK_RTCCLK_DIV_8    ((uint8_t)0x60) //!< Clock RTC Div 8
#define CLK_RTCCLK_DIV_16   ((uint8_t)0x80) //!< Clock RTC Div 16
#define CLK_RTCCLK_DIV_32   ((uint8_t)0xA0) //!< Clock RTC  Div 32
#define CLK_RTCCLK_DIV_64   ((uint8_t)0xC0) //!< Clock RTC  Div 64

// Window Watchdog (WWDG)
// *****************************************************************

//! WWDG Control Register
#define WWDG_CR     (*(volatile uint8_t *)0x50d3)
//! WWDR Window Register
#define WWDG_WR     (*(volatile uint8_t *)0x50d4)

/* Independent Watchdog (IWDG) */
/*****************************************************************/

//! IWDG Key Register
#define IWDG_KR     (*(volatile uint8_t *)0x50e0)

//! IWDG Prescaler Register
#define IWDG_PR     (*(volatile uint8_t *)0x50e1)

//! IWDG Reload Register
#define IWDG_RLR    (*(volatile uint8_t *)0x50e2)

/* Beeper (BEEP) */
/*****************************************************************/

//! BEEP Control/Status Register 1
#define BEEP_CSR1   (*(volatile uint8_t *)0x50f0)

//! BEEP Control/Status Register 2
#define BEEP_CSR2   (*(volatile uint8_t *)0x50f3)

/* Real-time clock (RTC) */
/*****************************************************************/

//! Time Register 1
#define RTC_TR1     (*(volatile uint8_t *)0x5140)

//! Time Register 2
#define RTC_TR2     (*(volatile uint8_t *)0x5141)

//! Time Register 3
#define RTC_TR3     (*(volatile uint8_t *)0x5142)

//! Date Register 1
#define RTC_DR1     (*(volatile uint8_t *)0x5144)

//! Date Register 2
#define RTC_DR2     (*(volatile uint8_t *)0x5145)

//! Date Register 3
#define RTC_DR3     (*(volatile uint8_t *)0x5146)

//! Control Register 1
#define RTC_CR1     (*(volatile uint8_t *)0x5148)

//! Control Register 2
#define RTC_CR2     (*(volatile uint8_t *)0x5149)

//! Control Register 3
#define RTC_CR3     (*(volatile uint8_t *)0x514a)

//! Initialization and Status Register 1
#define RTC_ISR1    (*(volatile uint8_t *)0x514c)

//! Initialization and Status Register 2
#define RTC_ISR2    (*(volatile uint8_t *)0x514d)

//! Synchronous Prescaler Register High
#define RTC_SPRERH  (*(volatile uint8_t *)0x5150)

//! Synchronous Prescaler Register Low
#define RTC_SPRERL  (*(volatile uint8_t *)0x5151)

//! Asynchronous Prescaler Register
#define RTC_APRER   (*(volatile uint8_t *)0x5152)

//! Wakeup Timer Register High
#define RTC_WUTRH   (*(volatile uint8_t *)0x5154)

//! Wakeup Timer Register Low
#define RTC_WUTRL   (*(volatile uint8_t *)0x5155)

//! Write Protection Register
#define RTC_WPR     (*(volatile uint8_t *)0x5159)

//! Alarm A Register 1
#define RTC_ALRMAR1 (*(volatile uint8_t *)0x515c)

//! Alarm A Register 2
#define RTC_ALRMAR2 (*(volatile uint8_t *)0x515d)

//! Alarm A Register 3
#define RTC_ALRMAR3 (*(volatile uint8_t *)0x515e)

//! Alarm A Register 4
#define RTC_ALRMAR4 (*(volatile uint8_t *)0x515f)

/* Serial Peripheral Interface 1 (SPI1) */
/*****************************************************************/

//! SPI1 Control Register 1
#define SPI1_CR1    (*(volatile uint8_t *)0x5200)

//! SPI1 Control Register 2
#define SPI1_CR2    (*(volatile uint8_t *)0x5201)

//! SPI1 Interrupt Control Register
#define SPI1_ICR    (*(volatile uint8_t *)0x5202)

//! SPI1 Status Register
#define SPI1_SR     (*(volatile uint8_t *)0x5203)

//! SPI1 Data Register
#define SPI1_DR     (*(volatile uint8_t *)0x5204)

//! SPI1 CRC Polynomial Register
#define SPI1_CRCPR  (*(volatile uint8_t *)0x5205)

//! SPI1 Rx CRC Register
#define SPI1_RXCRCR (*(volatile uint8_t *)0x5206)

//! SPI1 Tx CRC Register
#define SPI1_TXCRCR (*(volatile uint8_t *)0x5207)

/**
 * @}
 */

/** 
 * @addtogroup STM8_I2C
 * I2C Bus Interface 1 (I2C1)
 * @{
 */

//! I2C1 control register 1
#define I2C1_CR1    (*(volatile uint8_t *)0x5210)

//! I2C1 control register 2
#define I2C1_CR2    (*(volatile uint8_t *)0x5211)

//! I2C1 frequency register
#define I2C1_FREQR  (*(volatile uint8_t *)0x5212)

//! I2C1 Own address register low
#define I2C1_OARL   (*(volatile uint8_t *)0x5213)

//! I2C1 Own address register high
#define I2C1_OARH   (*(volatile uint8_t *)0x5214)

//! I2C1 data register
#define I2C1_DR     (*(volatile uint8_t *)0x5216)

//! I2C1 status register 1
#define I2C1_SR1    (*(volatile uint8_t *)0x5217)

//! I2C1 status register 2
#define I2C1_SR2    (*(volatile uint8_t *)0x5218)

//! I2C1 status register 3
#define I2C1_SR3    (*(volatile uint8_t *)0x5219)

//! I2C1 interrupt control register
#define I2C1_ITR    (*(volatile uint8_t *)0x521a)

//! I2C1 Clock control register low
#define I2C1_CCRL   (*(volatile uint8_t *)0x521b)

//! I2C1 Clock control register high
#define I2C1_CCRH   (*(volatile uint8_t *)0x521c)

//! I2C1 TRISE register
#define I2C1_TRISER (*(volatile uint8_t *)0x521d)

//! I2C1 packet error checking register
#define I2C1_PECR   (*(volatile uint8_t *)0x521e)

/**
 * @}
 */

/** 
 * @addtogroup STM8_USART1
 * Universal synch/asynch receiver transmitter 1 (USART1)
 * @{
 */

//! USART1 Status Register
#define USART1_SR   (*(volatile uint8_t *)0x5230)

//! USART1 Data Register
#define USART1_DR   (*(volatile uint8_t *)0x5231)

//! USART1 Baud Rate Register 1
#define USART1_BRR1 (*(volatile uint8_t *)0x5232)

//! USART1 Baud Rate Register 2
#define USART1_BRR2 (*(volatile uint8_t *)0x5233)

//! USART1 Control Register 1
#define USART1_CR1  (*(volatile uint8_t *)0x5234)

//! USART1 Control Register 2
#define USART1_CR2  (*(volatile uint8_t *)0x5235)

//! USART1 Control Register 3
#define USART1_CR3  (*(volatile uint8_t *)0x5236)

//! USART1 Control Register 4
#define USART1_CR4  (*(volatile uint8_t *)0x5237)

//! USART1 Control Register 5
#define USART1_CR5  (*(volatile uint8_t *)0x5238)

//! USART1 Guard time Register
#define USART1_GTR  (*(volatile uint8_t *)0x5239)

//! USART1 Prescaler Register
#define USART1_PSCR (*(volatile uint8_t *)0x523a)

#define USART_CR1_R8    ((uint8_t)1 << 7)
#define USART_CR1_T8    ((uint8_t)1 << 6)
#define USART_CR1_UARTD ((uint8_t)1 << 5)
#define USART_CR1_M     ((uint8_t)1 << 4)
#define USART_CR1_WAKE  ((uint8_t)1 << 3)
#define USART_CR1_PCEN  ((uint8_t)1 << 2)
#define USART_CR1_PS    ((uint8_t)1 << 1)
#define USART_CR1_PIEN  ((uint8_t)1 << 0)

#define USART_CR2_TIEN  ((uint8_t)1 << 7)
#define USART_CR2_TCIEN ((uint8_t)1 << 6)
#define USART_CR2_RIEN  ((uint8_t)1 << 5)
#define USART_CR2_ILIEN ((uint8_t)1 << 4)
#define USART_CR2_TEN   ((uint8_t)1 << 3)
#define USART_CR2_REN   ((uint8_t)1 << 2)
#define USART_CR2_RWU   ((uint8_t)1 << 1)
#define USART_CR2_SBK   ((uint8_t)1 << 0)

#define USART_CR3_LINEN ((uint8_t)1 << 6)
#define USART_CR3_STOP2 ((uint8_t)1 << 5)
#define USART_CR3_STOP1 ((uint8_t)1 << 4)
#define USART_CR3_CLKEN ((uint8_t)1 << 3)
#define USART_CR3_CPOL  ((uint8_t)1 << 2)
#define USART_CR3_CPHA  ((uint8_t)1 << 1)
#define USART_CR3_LBCL  ((uint8_t)1 << 0)

#define USART_SR_TXE    ((uint8_t)1 << 7)
#define USART_SR_TC     ((uint8_t)1 << 6)
#define USART_SR_RXNE   ((uint8_t)1 << 5)
#define USART_SR_IDLE   ((uint8_t)1 << 4)
#define USART_SR_OR     ((uint8_t)1 << 3)
#define USART_SR_NF     ((uint8_t)1 << 2)
#define USART_SR_FE     ((uint8_t)1 << 1)
#define USART_SR_PE     ((uint8_t)1 << 0)

/**
 * @}
 */

/**
 * @addtogroup STM8_TIM2
 * 16-Bit Timer 2 (TIM2) 
 * @{
 */

//! TIM2 Control register 1
#define TIM2_CR1    (*(volatile uint8_t *)0x5250)

//! TIM2 Control register 2
#define TIM2_CR2    (*(volatile uint8_t *)0x5251)

//! TIM2 Slave Mode Control register
#define TIM2_SMCR   (*(volatile uint8_t *)0x5252)

//! TIM2 External trigger register
#define TIM2_ETR    (*(volatile uint8_t *)0x5253)

//! TIM2 DMA request enable register
#define TIM2_DER    (*(volatile uint8_t *)0x5254)

//! TIM2 Interrupt enable register
#define TIM2_IER    (*(volatile uint8_t *)0x5255)

//! TIM2 Status register 1
#define TIM2_SR1    (*(volatile uint8_t *)0x5256)

//! TIM2 Status register 2
#define TIM2_SR2    (*(volatile uint8_t *)0x5257)

//! TIM2 Event Generation register
#define TIM2_EGR    (*(volatile uint8_t *)0x5258)

//! TIM2 Capture/Compare mode register 1
#define TIM2_CCMR1  (*(volatile uint8_t *)0x5259)

//! TIM2 Capture/Compare mode register 2
#define TIM2_CCMR2  (*(volatile uint8_t *)0x525a)

//! TIM2 Capture/Compare enable register 1
#define TIM2_CCER1  (*(volatile uint8_t *)0x525b)

//! TIM2 Counter High
#define TIM2_CNTRH  (*(volatile uint8_t *)0x525c)

//! TIM2 Counter Low
#define TIM2_CNTRL  (*(volatile uint8_t *)0x525d)

//! TIM2 Prescaler register
#define TIM2_PSCR   (*(volatile uint8_t *)0x525e)

//! TIM2 Auto-Reload Register High
#define TIM2_ARRH   (*(volatile uint8_t *)0x525f)
//! TIM2 Auto-Reload Register Low
#define TIM2_ARRL   (*(volatile uint8_t *)0x5260)

//! TIM2 Capture/Compare Register 1 High
#define TIM2_CCR1H  (*(volatile uint8_t *)0x5261)
//! TIM2 Capture/Compare Register 1 Low
#define TIM2_CCR1L  (*(volatile uint8_t *)0x5262)

//! TIM2 Capture/Compare Register 2 High
#define TIM2_CCR2H  (*(volatile uint8_t *)0x5263)
//! TIM2 Capture/Compare Register 2 Low
#define TIM2_CCR2L  (*(volatile uint8_t *)0x5264)

//! TIM2 Break register
#define TIM2_BKR    (*(volatile uint8_t *)0x5265)

//! TIM2 Output idle state register
#define TIM2_OISR   (*(volatile uint8_t *)0x5266)

/**
 * @}
 */

/**
 * @addtogroup STM8_TIM3
 * 16-Bit Timer 3 (TIM3) 
 * @{
 */

//! TIM3 Control register 1
#define TIM3_CR1    (*(volatile uint8_t *)0x5280)

//! TIM3 Control register 2
#define TIM3_CR2    (*(volatile uint8_t *)0x5281)

//! TIM3 Slave Mode Control register
#define TIM3_SMCR   (*(volatile uint8_t *)0x5282)

//! TIM3 External trigger register
#define TIM3_ETR    (*(volatile uint8_t *)0x5283)

//! TIM3 DMA request enable register
#define TIM3_DER    (*(volatile uint8_t *)0x5284)

//! TIM3 Interrupt enable register
#define TIM3_IER    (*(volatile uint8_t *)0x5285)

//! TIM3 Status register 1
#define TIM3_SR1    (*(volatile uint8_t *)0x5286)

//! TIM3 Status register 2
#define TIM3_SR2    (*(volatile uint8_t *)0x5287)

//! TIM3 Event Generation register
#define TIM3_EGR    (*(volatile uint8_t *)0x5288)

//! TIM3 Capture/Compare mode register 1
#define TIM3_CCMR1  (*(volatile uint8_t *)0x5289)

//! TIM3 Capture/Compare mode register 2
#define TIM3_CCMR2  (*(volatile uint8_t *)0x528a)

//! TIM3 Capture/Compare enable register 1
#define TIM3_CCER1  (*(volatile uint8_t *)0x528b)

//! TIM3 Counter High
#define TIM3_CNTRH  (*(volatile uint8_t *)0x528c)

//! TIM3 Counter Low
#define TIM3_CNTRL  (*(volatile uint8_t *)0x528d)

//! TIM3 Prescaler register
#define TIM3_PSCR   (*(volatile uint8_t *)0x528e)

//! TIM3 Auto-Reload Register High
#define TIM3_ARRH   (*(volatile uint8_t *)0x528f)

//! TIM3 Auto-Reload Register Low
#define TIM3_ARRL   (*(volatile uint8_t *)0x5290)

//! TIM3 Capture/Compare Register 1 High
#define TIM3_CCR1H  (*(volatile uint8_t *)0x5291)

//! TIM3 Capture/Compare Register 1 Low
#define TIM3_CCR1L  (*(volatile uint8_t *)0x5292)

//! TIM3 Capture/Compare Register 2 High
#define TIM3_CCR2H  (*(volatile uint8_t *)0x5293)

//! TIM3 Capture/Compare Register 2 Low
#define TIM3_CCR2L  (*(volatile uint8_t *)0x5294)

//! TIM3 Break register
#define TIM3_BKR    (*(volatile uint8_t *)0x5295)

//! TIM3 Output idle state register
#define TIM3_OISR   (*(volatile uint8_t *)0x5296)

/**
 * @}
 */

/**
 * @addtogroup STM8_TIM1
 * 16-Bit Timer 1 (TIM1) 
 * @{
 */

//! TIM1 Control register 1
#define TIM1_CR1    (*(volatile uint8_t *)0x52b0)

//! TIM1 Control register 2
#define TIM1_CR2    (*(volatile uint8_t *)0x52b1)

//! TIM1 Slave Mode Control register
#define TIM1_SMCR   (*(volatile uint8_t *)0x52b2)

//! TIM1 external trigger register
#define TIM1_ETR    (*(volatile uint8_t *)0x52b3)

//! TIM1 DMA request enable register
#define TIM1_DER    (*(volatile uint8_t *)0x52b4)

//! TIM1 Interrupt enable register
#define TIM1_IER    (*(volatile uint8_t *)0x52b5)

//! TIM1 Status register 1
#define TIM1_SR1    (*(volatile uint8_t *)0x52b6)

//! TIM1 Status register 2
#define TIM1_SR2    (*(volatile uint8_t *)0x52b7)

//! TIM1 Event Generation register
#define TIM1_EGR    (*(volatile uint8_t *)0x52b8)

//! TIM1 Capture/Compare mode register 1
#define TIM1_CCMR1  (*(volatile uint8_t *)0x52b9)

//! TIM1 Capture/Compare mode register 2
#define TIM1_CCMR2  (*(volatile uint8_t *)0x52ba)

//! TIM1 Capture/Compare mode register 3
#define TIM1_CCMR3  (*(volatile uint8_t *)0x52bb)

//! TIM1 Capture/Compare mode register 4
#define TIM1_CCMR4  (*(volatile uint8_t *)0x52bc)

//! TIM1 Capture/Compare enable register 1
#define TIM1_CCER1  (*(volatile uint8_t *)0x52bd)

//! TIM1 Capture/Compare enable register 2
#define TIM1_CCER2  (*(volatile uint8_t *)0x52be)

//! TIM1 Counter High
#define TIM1_CNTRH  (*(volatile uint8_t *)0x52bf)
//! TIM1 Counter Low
#define TIM1_CNTRL  (*(volatile uint8_t *)0x52c0)

//! TIM1 Prescaler Register High
#define TIM1_PSCRH  (*(volatile uint8_t *)0x52c1)
//! TIM1 Prescaler Register Low
#define TIM1_PSCRL  (*(volatile uint8_t *)0x52c2)

//! TIM1 Auto-Reload Register High
#define TIM1_ARRH   (*(volatile uint8_t *)0x52c3)
//! TIM1 Auto-Reload Register Low
#define TIM1_ARRL   (*(volatile uint8_t *)0x52c4)

//! TIM1 Repetition counter register
#define TIM1_RCR    (*(volatile uint8_t *)0x52c5)

//! TIM1 Capture/Compare Register 1 High
#define TIM1_CCR1H  (*(volatile uint8_t *)0x52c6)
//! TIM1 Capture/Compare Register 1 Low
#define TIM1_CCR1L  (*(volatile uint8_t *)0x52c7)

//! TIM1 Capture/Compare Register 2 High
#define TIM1_CCR2H  (*(volatile uint8_t *)0x52c8)
//! TIM1 Capture/Compare Register 2 Low
#define TIM1_CCR2L  (*(volatile uint8_t *)0x52c9)

//! TIM1 Capture/Compare Register 3 High
#define TIM1_CCR3H  (*(volatile uint8_t *)0x52ca)
//! TIM1 Capture/Compare Register 3 Low
#define TIM1_CCR3L  (*(volatile uint8_t *)0x52cb)

//! TIM1 Capture/Compare Register 4 High
#define TIM1_CCR4H  (*(volatile uint8_t *)0x52cc)
//! TIM1 Capture/Compare Register 4 Low
#define TIM1_CCR4L  (*(volatile uint8_t *)0x52cd)

//! TIM1 Break register
#define TIM1_BKR    (*(volatile uint8_t *)0x52ce)

//! TIM1 Dead-time register
#define TIM1_DTR    (*(volatile uint8_t *)0x52cf)

//! TIM1 Output idle state register
#define TIM1_OISR   (*(volatile uint8_t *)0x52d0)

//! TIM1 DMA control register 1
#define TIM1_DCR1   (*(volatile uint8_t *)0x52d1)

//! TIM1 DMA control register 2
#define TIM1_DCR2   (*(volatile uint8_t *)0x52d2)

//! TIM1 DMA address for burst mode
#define TIM1_DMAR   (*(volatile uint8_t *)0x52d3)

/**
 * @}
 */

/**
 * @addtogroup STM8_TIM4
 * 8-Bit  Timer 4 (TIM4) 
 * @{
 */
/*****************************************************************/

//! TIM4 Control Register 1
#define TIM4_CR1    (*(volatile uint8_t *)0x52e0)

//! TIM4 Control Register 2
#define TIM4_CR2    (*(volatile uint8_t *)0x52e1)

//! TIM4 Slave Mode Control Register
#define TIM4_SMCR   (*(volatile uint8_t *)0x52e2)

//! TIM4 DMA request Enable Register
#define TIM4_DER    (*(volatile uint8_t *)0x52e3)

//! TIM4 Interrupt Enable Register
#define TIM4_IER    (*(volatile uint8_t *)0x52e4)

//! TIM4 Status Register 1
#define TIM4_SR1    (*(volatile uint8_t *)0x52e5)

//! TIM4 Event Generation Register
#define TIM4_EGR    (*(volatile uint8_t *)0x52e6)

//! TIM4 Counter
#define TIM4_CNTR   (*(volatile uint8_t *)0x52e7)

//! TIM4 Prescaler Register
#define TIM4_PSCR   (*(volatile uint8_t *)0x52e8)

//! TIM4 Auto-Reload Register
#define TIM4_ARR    (*(volatile uint8_t *)0x52e9)

/**
 * @}
 */

/**
 * @addtogroup STM8_IR
 * Infra Red Interface (IR) 
 * @{
 */

//! Infra-red control register
#define IR_CR       (*(volatile uint8_t *)0x52ff)

/**
 * @}
 */

/**
 * @addtogroup STM8_ADC1
 * Analog to digital converter 1 (ADC1)
 * @}
 */

//! ADC1 Configuration register 1
#define ADC1_CR1    (*(volatile uint8_t *)0x5340)

//! ADC1 Configuration register 2
#define ADC1_CR2    (*(volatile uint8_t *)0x5341)

//! ADC1 Configuration register 3
#define ADC1_CR3    (*(volatile uint8_t *)0x5342)

//! ADC1 status register
#define ADC1_SR     (*(volatile uint8_t *)0x5343)

//! ADC Data Register High
#define ADC1_DRH    (*(volatile uint8_t *)0x5344)
//! ADC Data Register Low
#define ADC1_DRL    (*(volatile uint8_t *)0x5345)

//! ADC High Threshold Register High
#define ADC1_HTRH   (*(volatile uint8_t *)0x5346)
//! ADC High Threshold Register Low
#define ADC1_HTRL   (*(volatile uint8_t *)0x5347)

//! ADC Low Threshold Register High
#define ADC1_LTRH   (*(volatile uint8_t *)0x5348)
//! ADC Low Threshold Register Low
#define ADC1_LTRL   (*(volatile uint8_t *)0x5349)

//! ADC1 channel sequence 1 register
#define ADC1_SQR1   (*(volatile uint8_t *)0x534a)

//! ADC1 channel sequence 2 register
#define ADC1_SQR2   (*(volatile uint8_t *)0x534b)

//! ADC1 channel sequence 3 register
#define ADC1_SQR3   (*(volatile uint8_t *)0x534c)

//! ADC1 channel sequence 4 register
#define ADC1_SQR4   (*(volatile uint8_t *)0x534d)

//! ADC1 Trigger disable 1
#define ADC1_TRIGR1 (*(volatile uint8_t *)0x534e)

//! ADC1 Trigger disable 2
#define ADC1_TRIGR2 (*(volatile uint8_t *)0x534f)

//! ADC1 Trigger disable 3
#define ADC1_TRIGR3 (*(volatile uint8_t *)0x5350)

//! ADC1 Trigger disable 4
#define ADC1_TRIGR4 (*(volatile uint8_t *)0x5351)

#define ADC1_CR1_OVERIE (1 << 7)
#define ADC1_CR1_RES1 (1 << 6)
#define ADC1_CR1_RES0 (1 << 5)
#define ADC1_CR1_AWDIE (1 << 4)
#define ADC1_CR1_EOCIE (1 << 3)
#define ADC1_CR1_CONT (1 << 2)
#define ADC1_CR1_START (1 << 1)
#define ADC1_CR1_ADON (1 << 0)

#define ADC1_SR_OVER (1 << 2)
#define ADC1_SR_AWD (1 << 1)
#define ADC1_SR_EOC (1 << 0)

#define ADC1_SQR1_DMAOFF (1 << 7)

/**
 * @}
 */

/**
 * @addtogroup STM8_LCD
 * LCD controller (LCD) 
 * @{
 */

//! LCD control register 1
#define LCD_CR1     (*(volatile uint8_t *)0x5400)

//! LCD control register 2
#define LCD_CR2     (*(volatile uint8_t *)0x5401)

//! LCD control register 3
#define LCD_CR3     (*(volatile uint8_t *)0x5402)

//! LCD frequency selection register
#define LCD_FRQ     (*(volatile uint8_t *)0x5403)

//! LCD Port mask register 0
#define LCD_PM0     (*(volatile uint8_t *)0x5404)

//! LCD Port mask register 1
#define LCD_PM1     (*(volatile uint8_t *)0x5405)

//! LCD Port mask register 2
#define LCD_PM2     (*(volatile uint8_t *)0x5406)

//! LCD Port mask register 3
#define LCD_PM3     (*(volatile uint8_t *)0x5407)

//! LCD memory array pointer
#define LCD_RAM     ((volatile uint8_t *)0x540c)

//! LCD display memory 0
#define LCD_RAM0    (*(volatile uint8_t *)0x540c)

//! LCD display memory 1
#define LCD_RAM1    (*(volatile uint8_t *)0x540d)

//! LCD display memory 2
#define LCD_RAM2    (*(volatile uint8_t *)0x540e)

//! LCD display memory 3
#define LCD_RAM3    (*(volatile uint8_t *)0x540f)

//! LCD display memory 4
#define LCD_RAM4    (*(volatile uint8_t *)0x5410)

//! LCD display memory 5
#define LCD_RAM5    (*(volatile uint8_t *)0x5411)

//! LCD display memory 6
#define LCD_RAM6    (*(volatile uint8_t *)0x5412)

//! LCD display memory 7
#define LCD_RAM7    (*(volatile uint8_t *)0x5413)

//! LCD display memory 8
#define LCD_RAM8    (*(volatile uint8_t *)0x5414)

//! LCD display memory 9
#define LCD_RAM9    (*(volatile uint8_t *)0x5415)

//! LCD display memory 10
#define LCD_RAM10   (*(volatile uint8_t *)0x5416)

//! LCD display memory 11
#define LCD_RAM11   (*(volatile uint8_t *)0x5417)

//! LCD display memory 12
#define LCD_RAM12   (*(volatile uint8_t *)0x5418)

//! LCD display memory 13
#define LCD_RAM13   (*(volatile uint8_t *)0x5419)

//! LCD control register 4
#define LCD_CR4     (*(volatile uint8_t *)0x542F)

#define LCD_CR1_DUTY_STATIC ((uint8_t)0x00) //!< Static duty cylce
#define LCD_CR1_DUTY_1_2    ((uint8_t)0x02) //!< 1/2 duty cycle
#define LCD_CR1_DUTY_1_3    ((uint8_t)0x04) //!< 1/3 duty cycle
#define LCD_CR1_DUTY_1_4    ((uint8_t)0x06) //!< 1/4 duty cycle

#define LCD_CR1_BIAS_1_3    ((uint8_t)0x00) //!< 1/3 bias
#define LCD_CR1_BIAS_1_2    ((uint8_t)0x01) //!< 1/2 bias

#define LCD_CR1_BLINKMODE_OFF           ((uint8_t)0x00) //!< Blink inactive
#define LCD_CR1_BLINKMODE_SEG0_COM0     ((uint8_t)0x40) //!< SEG0 on, COM0 blink
#define LCD_CR1_BLINKMODE_SEG0_ALLCOM   ((uint8_t)0x80) //!< SEG0 on, all COM blink
#define LCD_CR1_BLINKMODE_ALLSEG_ALLCOM ((uint8_t)0xC0) //!< All SEG on, all COm Blink

#define LCD_CR1_BLINKFREQ_DIV_8         ((uint8_t)0x00) //!< The Blink frequency = fLcd/8
#define LCD_CR1_BLINKFREQ_DIV_16        ((uint8_t)0x08) //!< The Blink frequency = fLcd/16
#define LCD_CR1_BLINKFREQ_DIV_32        ((uint8_t)0x10) //!< The Blink frequency = fLcd/32
#define LCD_CR1_BLINKFREQ_DIV_64        ((uint8_t)0x18) //!< The Blink frequency = fLcd/64
#define LCD_CR1_BLINKFREQ_DIV_128       ((uint8_t)0x20) //!< The Blink frequency = fLcd/128
#define LCD_CR1_BLINKFREQ_DIV_256       ((uint8_t)0x28) //!< The Blink frequency = fLcd/256
#define LCD_CR1_BLINKFREQ_DIV_512       ((uint8_t)0x30) //!< The Blink frequency = fLcd/512
#define LCD_CR1_BLINKFREQ_DIV_1024      ((uint8_t)0x38) //!< The Blink frequency = fLcd/1024

#define LCD_CR1_BLINK_MASK  ((uint8_t)0xC0) //!< Blink bits mask
#define LCD_CR1_BLINKF_MASK ((uint8_t)0x38) //!< Blink frequency bits mask
#define LCD_CR1_DUTY_MASK   ((uint8_t)0x06) //!< Duty bits mask
#define LCD_CR1_B2_MASK     ((uint8_t)0x01) //!< Bias selector bit mask

#define LCD_CR2_CTRST_LVL_0 ((uint8_t)0x00) //!< Medium density / High density maximum voltage = 2.60V / 2.60V
#define LCD_CR2_CTRST_LVL_1 ((uint8_t)0x02) //!< Medium density / High density maximum voltage = 2.70V / 2.73V
#define LCD_CR2_CTRST_LVL_2 ((uint8_t)0x04) //!< Medium density / High density maximum voltage = 2.80V / 2.86V
#define LCD_CR2_CTRST_LVL_3 ((uint8_t)0x06) //!< Medium density / High density maximum voltage = 2.90V / 2.99V
#define LCD_CR2_CTRST_LVL_4 ((uint8_t)0x08) //!< Medium density / High density maximum voltage = 3.00V / 3.12V
#define LCD_CR2_CTRST_LVL_5 ((uint8_t)0x0A) //!< Medium density / High density maximum voltage = 3.10V / 3.25V
#define LCD_CR2_CTRST_LVL_6 ((uint8_t)0x0C) //!< Medium density / High density maximum voltage = 3.20V / 3.38V
#define LCD_CR2_CTRST_LVL_7 ((uint8_t)0x0E) //!< Medium density / High density maximum voltage = 3.30V / 3.51V
#define LCD_CR2_CTRST_MASK  ((uint8_t)0x0E) //!< Contrast control bits mask

#define LCD_CR2_PON_DUR_0   ((uint8_t)0x00) //!< Pulse on duration = 0/CLKprescaler
#define LCD_CR2_PON_DUR_1   ((uint8_t)0x20) //!< Pulse on duration = 1/CLKprescaler
#define LCD_CR2_PON_DUR_2   ((uint8_t)0x40) //!< Pulse on duration = 2/CLKprescaler
#define LCD_CR2_PON_DUR_3   ((uint8_t)0x60) //!< Pulse on duration = 3/CLKprescaler
#define LCD_CR2_PON_DUR_4   ((uint8_t)0x80) //!< Pulse on duration = 4/CLKprescaler
#define LCD_CR2_PON_DUR_5   ((uint8_t)0xA0) //!< Pulse on duration = 5/CLKprescaler
#define LCD_CR2_PON_DUR_6   ((uint8_t)0xC0) //!< Pulse on duration = 6/CLKprescaler
#define LCD_CR2_PON_DUR_7   ((uint8_t)0xE0) //!< Pulse on duration = 7/CLKprescaler
#define LCD_CR2_PON_MASK    ((uint8_t)0xE0) //!< Pulse on duration bits mask

#define LCD_CR2_HD_MASK     ((uint8_t)0x10) //!< High drive enable bit mask

#define LCD_CR2_VSEL_INT    ((uint8_t)0x00) //!< Internal voltage source for the LCD
#define LCD_CR2_VSEL_EXT    ((uint8_t)0x01) //!< External voltage source for the LCD
#define LCD_CR2_VSEL_MASK   ((uint8_t)0x01) //!< Voltage source bit mask

#define LCD_CR3_LCDEN       ((uint8_t)0x40) //!< Enable bit
#define LCD_CR3_SOFIE       ((uint8_t)0x20) //!< Start of frame interrupt enable bit
#define LCD_CR3_SOF         ((uint8_t)0x10) //!< Start of frame bit
#define LCD_CR3_SOFC        ((uint8_t)0x08) //!< Clear start of frame bit
#define LCD_CR3_DEAD_0      ((uint8_t)0x00) //!< No dead Time
#define LCD_CR3_DEAD_1      ((uint8_t)0x01) //!< One Phase between different couple of frame
#define LCD_CR3_DEAD_2      ((uint8_t)0x02) //!< Two Phase between different couple of frame
#define LCD_CR3_DEAD_3      ((uint8_t)0x03) //!< Tree Phase between different couple of frame
#define LCD_CR3_DEAD_4      ((uint8_t)0x04) //!< Four Phase between different couple of frame
#define LCD_CR3_DEAD_5      ((uint8_t)0x05) //!< Five Phase between different couple of frame
#define LCD_CR3_DEAD_6      ((uint8_t)0x06) //!< Six Phase between different couple of frame
#define LCD_CR3_DEAD_7      ((uint8_t)0x07) //!< Seven Phase between different couple of frame
#define LCD_CR3_DEAD_MASK   ((uint8_t)0x07) //!< DEAD time bits mask

#define LCD_FRQ_PS_1        ((uint8_t)0x00) //!< CLKprescaler = ClKinput
#define LCD_FRQ_PS_2        ((uint8_t)0x10) //!< CLKprescaler = ClKinput/2
#define LCD_FRQ_PS_4        ((uint8_t)0x20) //!< CLKprescaler = ClKinput/4
#define LCD_FRQ_PS_8        ((uint8_t)0x30) //!< CLKprescaler = ClKinput/8
#define LCD_FRQ_PS_16       ((uint8_t)0x40) //!< CLKprescaler = ClKinput/16
#define LCD_FRQ_PS_32       ((uint8_t)0x50) //!< CLKprescaler = ClKinput/32
#define LCD_FRQ_PS_64       ((uint8_t)0x60) //!< CLKprescaler = ClKinput/64
#define LCD_FRQ_PS_128      ((uint8_t)0x70) //!< CLKprescaler = ClKinput/128
#define LCD_FRQ_PS_256      ((uint8_t)0x80) //!< CLKprescaler = ClKinput/256
#define LCD_FRQ_PS_512      ((uint8_t)0x90) //!< CLKprescaler = ClKinput/512
#define LCD_FRQ_PS_1024     ((uint8_t)0xA0) //!< CLKprescaler = ClKinput/1024
#define LCD_FRQ_PS_2048     ((uint8_t)0xB0) //!< CLKprescaler = ClKinput/2048
#define LCD_FRQ_PS_4096     ((uint8_t)0xC0) //!< CLKprescaler = ClKinput/4096
#define LCD_FRQ_PS_8192     ((uint8_t)0xD0) //!< CLKprescaler = ClKinput/8192
#define LCD_FRQ_PS_16384    ((uint8_t)0xE0) //!< CLKprescaler = ClKinput/16384
#define LCD_FRQ_PS_32768    ((uint8_t)0xF0) //!< CLKprescaler = ClKinput/32768
#define LCD_FRQ_PS_MASK     ((uint8_t)0xF0) //!< Mask of prescaler bits

#define LCD_FRQ_DIV_16      ((uint8_t)0x00) //!< LCD frequency = CLKprescaler/16
#define LCD_FRQ_DIV_17      ((uint8_t)0x01) //!< LCD frequency = CLKprescaler/17
#define LCD_FRQ_DIV_18      ((uint8_t)0x02) //!< LCD frequency = CLKprescaler/18
#define LCD_FRQ_DIV_19      ((uint8_t)0x03) //!< LCD frequency = CLKprescaler/19
#define LCD_FRQ_DIV_20      ((uint8_t)0x04) //!< LCD frequency = CLKprescaler/20
#define LCD_FRQ_DIV_21      ((uint8_t)0x05) //!< LCD frequency = CLKprescaler/21
#define LCD_FRQ_DIV_22      ((uint8_t)0x06) //!< LCD frequency = CLKprescaler/22
#define LCD_FRQ_DIV_23      ((uint8_t)0x07) //!< LCD frequency = CLKprescaler/23
#define LCD_FRQ_DIV_24      ((uint8_t)0x08) //!< LCD frequency = CLKprescaler/24
#define LCD_FRQ_DIV_25      ((uint8_t)0x09) //!< LCD frequency = CLKprescaler/25
#define LCD_FRQ_DIV_26      ((uint8_t)0x0A) //!< LCD frequency = CLKprescaler/26
#define LCD_FRQ_DIV_27      ((uint8_t)0x0B) //!< LCD frequency = CLKprescaler/27
#define LCD_FRQ_DIV_28      ((uint8_t)0x0C) //!< LCD frequency = CLKprescaler/28
#define LCD_FRQ_DIV_29      ((uint8_t)0x0D) //!< LCD frequency = CLKprescaler/29
#define LCD_FRQ_DIV_30      ((uint8_t)0x0E) //!< LCD frequency = CLKprescaler/30
#define LCD_FRQ_DIV_31      ((uint8_t)0x0F) //!< LCD frequency = CLKprescaler/31
#define LCD_FRQ_DIV_MASK    ((uint8_t)0x0F) //!< Mask of divider bits

#define LCD_RAM_IDX00       ((uint8_t)0x00)
#define LCD_RAM_IDX01       ((uint8_t)0x01)
#define LCD_RAM_IDX02       ((uint8_t)0x02)
#define LCD_RAM_IDX03       ((uint8_t)0x03)
#define LCD_RAM_IDX04       ((uint8_t)0x04)
#define LCD_RAM_IDX05       ((uint8_t)0x05)
#define LCD_RAM_IDX06       ((uint8_t)0x06)
#define LCD_RAM_IDX07       ((uint8_t)0x07)
#define LCD_RAM_IDX08       ((uint8_t)0x08)
#define LCD_RAM_IDX09       ((uint8_t)0x09)
#define LCD_RAM_IDX0A       ((uint8_t)0x0A)
#define LCD_RAM_IDX0B       ((uint8_t)0x0B)
#define LCD_RAM_IDX0C       ((uint8_t)0x0C)
#define LCD_RAM_IDX0D       ((uint8_t)0x0D)
#define LCD_RAM_IDX0E       ((uint8_t)0x0E)
#define LCD_RAM_IDX0F       ((uint8_t)0x0F)
#define LCD_RAM_IDX10       ((uint8_t)0x10)
#define LCD_RAM_IDX11       ((uint8_t)0x11)
#define LCD_RAM_IDX12       ((uint8_t)0x12)
#define LCD_RAM_IDX13       ((uint8_t)0x13)
#define LCD_RAM_IDX14       ((uint8_t)0x14)
#define LCD_RAM_IDX15       ((uint8_t)0x15)

#define LCD_CR4_MAPCOM      ((uint8_t)0x08) //!< Select the mapping scheme for the COM[7:4]
#define LCD_CR4_PAGECOM     ((uint8_t)0x04) //!< Select the LCD RAM page sector
#define LCD_CR4_DUTY8       ((uint8_t)0x02) //!< Enable the 1/8 duty
#define LCD_CR4_BIAS_1_4    ((uint8_t)0x01) //!< 1/4 bias

/**
 * @}
 */

/**
 * @addtogroup STM8_RI
 * Routing interface (RI) 
 * @{
 */

//! Timer input capture routing register 1
#define RI_ICR1     (*(volatile uint8_t *)0x5431)

//! Timer input capture routing register 2
#define RI_ICR2     (*(volatile uint8_t *)0x5432)

//! I/O input register 1
#define RI_IOIR1    (*(volatile uint8_t *)0x5433)

//! I/O input register 2
#define RI_IOIR2    (*(volatile uint8_t *)0x5434)

//! I/O input register 3
#define RI_IOIR3    (*(volatile uint8_t *)0x5435)

//! I/O control mode register 1
#define RI_IOCMR1   (*(volatile uint8_t *)0x5436)

//! I/O control mode register 2
#define RI_IOCMR2   (*(volatile uint8_t *)0x5437)

//! I/O control mode register 3
#define RI_IOCMR3   (*(volatile uint8_t *)0x5438)

//! I/O switch register 1
#define RI_IOSR1    (*(volatile uint8_t *)0x5439)

//! I/O switch register 2
#define RI_IOSR2    (*(volatile uint8_t *)0x543a)

//! I/O switch register 3
#define RI_IOSR3    (*(volatile uint8_t *)0x543b)

//! I/O group control register
#define RI_IOGCR    (*(volatile uint8_t *)0x543c)

//! Analog switch register 1
#define RI_ASCR1    (*(volatile uint8_t *)0x543d)

//! Analog switch register 2
#define RI_ASCR2    (*(volatile uint8_t *)0x543e)

//! Resistor control register
#define RI_RCR      (*(volatile uint8_t *)0x543f)

// Custom bits
#define TIM_IER_BIE     ((uint8_t)1 << 7)
#define TIM_IER_TIE     ((uint8_t)1 << 6)
#define TIM_IER_COMIE   ((uint8_t)1 << 5)
#define TIM_IER_CC4IE   ((uint8_t)1 << 4)
#define TIM_IER_CC3IE   ((uint8_t)1 << 3)
#define TIM_IER_CC2IE   ((uint8_t)1 << 2)
#define TIM_IER_CC1IE   ((uint8_t)1 << 1)
#define TIM_IER_UIE     ((uint8_t)1 << 0)

#define TIM_CR1_APRE    ((uint8_t)1 << 7)
#define TIM_CR1_CMSH    ((uint8_t)1 << 6)
#define TIM_CR1_CMSL    ((uint8_t)1 << 5)
#define TIM_CR1_DIR     ((uint8_t)1 << 4)
#define TIM_CR1_OPM     ((uint8_t)1 << 3)
#define TIM_CR1_URS     ((uint8_t)1 << 2)
#define TIM_CR1_UDIS    ((uint8_t)1 << 1)
#define TIM_CR1_CEN     ((uint8_t)1 << 0)

#define TIM_SR1_BIF     ((uint8_t)1 << 7)
#define TIM_SR1_TIF     ((uint8_t)1 << 6)
#define TIM_SR1_COMIF   ((uint8_t)1 << 5)
#define TIM_SR1_CC4IF   ((uint8_t)1 << 4)
#define TIM_SR1_CC3IF   ((uint8_t)1 << 3)
#define TIM_SR1_CC2IF   ((uint8_t)1 << 2)
#define TIM_SR1_CC1IF   ((uint8_t)1 << 1)
#define TIM_SR1_UIF     ((uint8_t)1 << 0)



#endif // __STM8L052C6__
