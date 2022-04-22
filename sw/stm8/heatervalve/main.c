#include <stdbool.h>
#include <stdint.h>

#include "STM8L052C6.h"

#include "adc.h"
#include "cmd.h"
#include "control_temp.h"
#include "motor.h"
#include "remote_tempsense.h"
#include "timer.h"
#include "uart.h"


static void initClock()
{
    // set the frequency to 16 MHz
    CLK_CKDIVR = 0;
    // enable peripherals
    CLK_PCKENR1 = 0xFF;
    CLK_PCKENR2 = 0xFF;
}


void main(void)
{
    char c;
    uint16_t adc, oldadc;
    int16_t diff = 0;
    char buf[16];

    initClock();
    uart_initialize();
    uart_write("heatervalve\n");
    enableInterrupts();

    adc_initialize();

    timer_initialize();
    motor_initialize();
    motor_start_homing();
    cmd_initialize();
    remts_initialize();
    ctrl_temp_initialize();
    uart_write("init complete\n");
    while (1) {
        if (USART1_SR & USART_SR_RXNE) {
            c = USART1_DR;
            cmd_process_char(c);
        }
        ctrl_temp_background();
        motor_background();
        adc = adc_read();
        diff = adc - oldadc;
        if (adc < 2037) {
            dec2bcd(adc, buf);
        }
        oldadc = adc;
    }
}


//#define UART_RECV_ISR 28
#define TIM2_OVF_ISR 19


extern volatile uint32_t g_tim2_millis;


void tim2_isr()
__interrupt(TIM2_OVF_ISR) {
    TIM2_SR1 = 0;
    g_tim2_millis++;
}
