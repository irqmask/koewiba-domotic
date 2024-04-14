#include <stdbool.h>
#include <stdint.h>

// include
#include "stm8l052c6.h"

#include "adc.h"
#include "cmd.h"
#include "debug.h"
#include "encoder.h"
#include "lcd.h"
#include "motor.h"
#include "timer.h"
#include "uart.h"


static void initClock()
{
    // enable system clock
    CLK_SWCR |= CLK_SWCR_SWEN;
    CLK_SWR = CLK_SWR_SRC_HSI;
    CLK_CKDIVR = CLK_CKDIV_8;
    while (CLK_SCSR != CLK_SWR_SRC_HSI);

    // Enable external low speed crystal for RTC
    CLK_PCKENR2 |= CLK_PCKENR2_RTC;
    CLK_CRTCR = CLK_RTCCLK_SRC_LSE | CLK_RTCCLK_DIV_1;
}

void app_irq_every_millisec(void)
{
    enc_on_irq();
}

void main(void)
{
    char c;
    //uint16_t adc, oldadc;
    //int16_t diff = 0;
    //char buf[16];

    char ticks_written = 0;
    char lcd_test_value = 0;
    initClock();
    uart_initialize();

    LOG_DEBUG("# heatervalve\n");
    lcd_initialize();

    adc_initialize();
    enc_initialize();

    timer_initialize();
    enableInterrupts();

    motor_initialize();
    motor_start_homing();
    cmd_initialize();

    LOG_DEBUG("# init complete\n");

    while (1) {
        if (uart_rx_pending()) {
            c = uart_rx_data();
            cmd_process_char(c);
        }

        motor_background();
        /*
        adc = adc_read();
        diff = adc - oldadc;
        if (adc < 2037) {
            dec2bcd(adc, buf);
        }
        oldadc = adc;
        */
        if ((timer_get_millis() % 500) == 0) {
            if (enc_val_changed()) {
                printf("enc %02x\n", enc_read());
            }
        } else {
            ticks_written = 0;
        }
    }
}

// un-comment interrupt declaration if needed

//void TRAP_IRQHandler(void) __interrupt(IPT_TRAP);
//void FLASH_IRQHandler(void) __interrupt(IPT_FLASH);
//void DMA1_CHANNEL0_1_IRQHandler(void) __interrupt(IPT_DMA1_CHN01);
//void DMA1_CHANNEL2_3_IRQHandler(void) __interrupt(IPT_DMA1_CHN23);
//void RTC_CSSLSE_IRQHandler(void) __interrupt(IPT_RTC_CSSLSE);
//void EXTIE_F_PVD_IRQHandler(void) __interrupt(IPT_EXTI_PORT_EF);
//void EXTIB_G_IRQHandler(void) __interrupt(IPT_EXTI_PORT_BG);
//void EXTID_H_IRQHandler(void) __interrupt(IPT_EXTI_PORT_DH);
//void EXTI0_IRQHandler(void) __interrupt(IPT_EXTI_PIN0);
//void EXTI1_IRQHandler(void) __interrupt(IPT_EXTI_PIN1);
//void EXTI2_IRQHandler(void) __interrupt(IPT_EXTI_PIN2);
//void EXTI3_IRQHandler(void) __interrupt(IPT_EXTI_PIN3);
//void EXTI4_IRQHandler(void) __interrupt(IPT_EXTI_PIN4);
//void EXTI5_IRQHandler(void) __interrupt(IPT_EXTI_PIN5);
//void EXTI6_IRQHandler(void) __interrupt(IPT_EXTI_PIN6);
//void EXTI7_IRQHandler(void) __interrupt(IPT_EXTI_PIN7);
//void LCD_AES_IRQHandler(void) __interrupt(IPT_LCD_AES);
//void SWITCH_CSS_BREAK_DAC_IRQHandler(void) __interrupt(IPT_SWITCH_CSS_BREAK_DAC);
//void ADC1_COMP_IRQHandler(void) __interrupt(IPT_ADC1_COMP);
//void TIM2_UPD_OVF_TRG_BRK_USART2_TX_IRQHandler(void) __interrupt(IPT_TIM2_UPD_OVF_TRG_BRK_USART2_TX);
//void TIM2_CC_USART2_RX_IRQHandler(void) __interrupt(IPT_TIM2_CC_USART2_RX);
//void TIM3_UPD_OVF_TRG_BRK_USART3_TX_IRQHandler(void) __interrupt(IPT_TIM3_UPD_OVF_TRG_BRK_USART3_TX);
//void TIM3_CC_USART3_RX_IRQHandler(void) __interrupt(IPT_TIM3_CC_USART3_RX);
void TIM1_UPD_OVF_TRG_COM_IRQHandler(void) __interrupt(IPT_TIM1_UPD_OVF_TRG_COM);
//void TIM1_CC_IRQHandler(void) __interrupt(IPT_TIM1_CC);
//void TIM4_UPD_OVF_TRG_IRQHandler(void) __interrupt(IPT_TIM4_UPD_OVF);
//void SPI1_IRQHandler(void) __interrupt(IPT_SPI1);
//void USART1_TX_TIM5_UPD_OVF_TRG_BRK_IRQHandler(void) __interrupt(IPT_USART1_TX_TIM5_UPD_OVF_TRG_BRK);
void USART1_RX_TIM5_CC_IRQHandler(void) __interrupt(IPT_USART1_RX_TIM5_CC);
//void I2C1_SPI2_IRQHandler(void) __interrupt(IPT_I2C1_SPI2);

