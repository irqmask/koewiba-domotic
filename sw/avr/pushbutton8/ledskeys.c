/**
 * @addtogroup LEDS_KEYS
 *
 * @{
 * @file    ledskeys.c
 * @brief   Control LEDs and keys.
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include "prjconf.h"

#ifdef PRJCONF_UC_AVR
#include <avr/io.h>
#include <avr/interrupt.h>
#endif

#include "appconfig.h"
#include "pcbconfig.h"

#include "clock.h"
#include "spi.h"
#include "ledskeys.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

static const uint8_t    g_led_flash_signal[] = { 0x00, 0xFF, 0x00, 0xFF };
static uint8_t          g_led_data = 0;
static uint8_t          g_led_flash_mask = 0x00;
static uint8_t          g_led_fast_flash_mask = 0x00;
static uint8_t          g_led_flash_index = 0;
static clock_timer_t    g_timer;

static uint8_t key_pressed;
static uint8_t key_repeated;
static uint8_t key_state;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

static void sn74595_latch(void)
{
    LED_STB_PORT |= (1<<LED_STB_PIN);
    LED_STB_PORT &= ~(1<<LED_STB_PIN);
}

static void sn74595_OE_on(void)
{
    LED_OE_PORT &= ~(1<<LED_OE_PIN);
}

static void sn74595_OE_off(void)
{
    LED_OE_PORT |= (1<<LED_OE_PIN);
}

static void sn74595_send(uint8_t data)
{
    spi_transmit_blk(data);
    sn74595_latch();
}

static uint8_t ledindex_to_dataindex(uint8_t led_index)
{
    switch (led_index) {
    case 0: return LED0;
    case 1: return LED1;
    case 2: return LED2;
    case 3: return LED3;
    case 4: return LED4;
    case 5: return LED5;
    case 6: return LED6;
    case 7: return LED7;
    default: return 0;
    }
}

static void led_on(uint8_t led_index)
{
    uint8_t data_index;

    data_index = ledindex_to_dataindex(led_index);

    g_led_data |= (1<<data_index);
}

static void led_off(uint8_t led_index)
{
    uint8_t data_index;

    data_index = ledindex_to_dataindex(led_index);

    g_led_data &= ~(1<<data_index);
}

static uint8_t keys_read(void)
{
    uint8_t keystate = 0xFF;

    if (KEY0_IPORT & (1<<KEY0_PIN)) keystate &= ~(1<<0);
    if (KEY1_IPORT & (1<<KEY1_PIN)) keystate &= ~(1<<1);
    if (KEY2_IPORT & (1<<KEY2_PIN)) keystate &= ~(1<<2);
    if (KEY3_IPORT & (1<<KEY3_PIN)) keystate &= ~(1<<3);
    if (KEY4_IPORT & (1<<KEY4_PIN)) keystate &= ~(1<<4);
    if (KEY5_IPORT & (1<<KEY5_PIN)) keystate &= ~(1<<5);
    if (KEY6_IPORT & (1<<KEY6_PIN)) keystate &= ~(1<<6);
    if (KEY7_IPORT & (1<<KEY7_PIN)) keystate &= ~(1<<7);
    return keystate;
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Initialized the LED Display and key driver.
 * The timer irq 0 is started to generate an irq every 1,41 milliseconds.
 * Key and display data registers are cleared to zero.
 */
void leds_keys_init (void)
{
    // clear key data registers
    key_repeated    = 0;
    key_state       = 0;
    key_pressed     = 0;

    // clear display data registers
    g_led_data = 0;
    g_led_flash_mask = 0x00;
    g_led_fast_flash_mask = 0x00;
    g_led_flash_index = 0;

    // set in/out port data direction and initial values.

    // initialize LEDs and switch all off
    LED_OE_DDR |= (1<<LED_OE_PIN);
    LED_STB_DDR |= (1<<LED_STB_PIN);
    sn74595_OE_off();
    LED_STB_PORT &= ~(1<<LED_STB_PIN);
    spi_transmit_blk(0);
    sn74595_latch();
    sn74595_OE_on();

    // set inputs for keys and enable pull-ups
    KEY0_DDR &= ~(1<<KEY0_PIN);
    KEY0_PORT |= (1<<KEY0_PIN);
    KEY1_DDR &= ~(1<<KEY1_PIN);
    KEY1_PORT |= (1<<KEY1_PIN);
    KEY2_DDR &= ~(1<<KEY2_PIN);
    KEY2_PORT |= (1<<KEY2_PIN);
    KEY3_DDR &= ~(1<<KEY3_PIN);
    KEY3_PORT |= (1<<KEY3_PIN);
    KEY4_DDR &= ~(1<<KEY4_PIN);
    KEY4_PORT |= (1<<KEY4_PIN);
    KEY5_DDR &= ~(1<<KEY5_PIN);
    KEY5_PORT |= (1<<KEY5_PIN);
    KEY6_DDR &= ~(1<<KEY6_PIN);
    KEY6_PORT |= (1<<KEY6_PIN);
    KEY7_DDR &= ~(1<<KEY7_PIN);
    KEY7_PORT |= (1<<KEY7_PIN);

    KEY0_WAKEUP_REG |= (1<<KEY0_WAKEUP);
    KEY1_WAKEUP_REG |= (1<<KEY1_WAKEUP);
    KEY2_WAKEUP_REG |= (1<<KEY2_WAKEUP);
    KEY3_WAKEUP_REG |= (1<<KEY3_WAKEUP);
    KEY4_WAKEUP_REG |= (1<<KEY4_WAKEUP);
    KEY5_WAKEUP_REG |= (1<<KEY5_WAKEUP);
    KEY6_WAKEUP_REG |= (1<<KEY6_WAKEUP);
    KEY7_WAKEUP_REG |= (1<<KEY7_WAKEUP);

    // initialize and start timer
    clk_timer_start(&g_timer, CLOCK_MS_2_TICKS(150));
}

void leds_keys_background (void)
{
    uint8_t flashed_leds;
    if (clk_timer_is_elapsed(&g_timer)) {
        key_pressed = keys_read();

        flashed_leds = 0;
        flashed_leds |= (g_led_flash_signal[g_led_flash_index >> 2] & g_led_flash_mask);
        flashed_leds |= (g_led_flash_signal[g_led_flash_index >> 1] & g_led_fast_flash_mask);
        g_led_flash_index++;
        g_led_flash_index &= 0x07;

        sn74595_send(g_led_data | flashed_leds);
        clk_timer_start(&g_timer, CLOCK_MS_2_TICKS(150));
    }
}

void led_switch (uint8_t led_index, eLEDMode_t mode)
{
    uint8_t data_idx;
    if (led_index >= 8) return;
    data_idx = ledindex_to_dataindex(led_index);

    switch (mode) {
    case eLED_on:
        g_led_flash_mask &= ~(1<<data_idx);
        g_led_fast_flash_mask &= ~(1<<data_idx);
        led_on(led_index);
        break;

    case eLED_blink_slow:
        g_led_flash_mask |= (1<<data_idx);
        g_led_fast_flash_mask &= ~(1<<data_idx);
        led_off(led_index);
        break;

    case eLED_blink_fast:
        g_led_flash_mask &= ~(1<<data_idx);
        g_led_fast_flash_mask |= (1<<data_idx);
        led_off(led_index);
        break;

    //case eLED_off:
    default:
        g_led_flash_mask &= ~(1<<data_idx);
        g_led_fast_flash_mask &= ~(1<<data_idx);
        led_off(led_index);
        break;
    }
}

/**
 * Check if a key has been pressed. Each pressed key is reported only once.
 *
 * @param[in] uKeyMask
 * Mask of keys that should be checked.
 *
 * @returns
 * Mask of keys which are pressed.
 */
uint8_t key_get_pressed (uint8_t mask)
{
    // read and clear atomic !
    mask &= key_pressed; // read key(s)
    key_pressed ^= mask; // clear key(s)
    return mask;
}

/**
 * Check if a key has been pressed long enough such that the key repeat
 * functionality kicks in. After a small setup delay the key is reported beeing
 * pressed in subsequent calls to this function. This simulates the user
 * repeatedly pressing and releasing the key.
 *
 * @param[in] uKeyMask
 * Mask of keys that should be checked.
 *
 * @returns
 * Mask of keys which are pressed repetitive.
 */
uint8_t key_get_pressed_repeat (uint8_t mask)
{
    // read and clear atomic !
    mask &= key_repeated; // read key(s)
    key_repeated ^= mask; // clear key(s)
    return mask;
}

uint8_t key_get_pressed_short (uint8_t mask)
{
    // read key state and key press atomic !
    return key_get_pressed(~key_state & mask);
}

uint8_t key_get_pressed_long (uint8_t mask)
{
    return key_get_pressed(key_get_pressed_repeat(mask));
}

/** @} */
