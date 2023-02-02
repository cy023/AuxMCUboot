#include <avr/io.h>
#include "auxmcu_io.h"

void gpio_init(void)
{
    /**
     * PORTF
     *   - PF7 (I): ASAID knob bit 3
     *   - PF6 (I): ASAID knob bit 2
     *   - PF5 (I): ASAID knob bit 1
     *   - PF4 (I): ASAID knob bit 0
     *   - PF3 (X)
     *   - PF2 (X)
     *   - PF1 (I): M/S switch      - detect M/S      mode.
     *   - PF0 (I): Prog/Run switch - detect Prog/Run mode.
     */
    DDRF = 0b00000000;

    /**
     * PORTD
     *   - PD7 (O): Prog/Run Mode to Main MCU. (Prog (1) / Run(0))
     *   - PD6 (O): M/S mode to Main MCU. (M (1) / S(0))
     *   - PD5 (O): CS pin to Main MCU. (If matched, CS pin pull low.)
     *   - PD4 (X)
     *   - PD3 (O): UART1 TX <-> Main MCU RX.
     *   - PD2 (I): UART1 RX <-> Main MCU TX.
     *   - PD1 (I): (EXTINT1: rising/falling edge-triggered) Use in Slave mode, detect CS pin from ASA BUS.
     *   - PD0 (I/O):
     *       - (I): (EXTINT0: rising edge-triggered) Detect the reset button signal.
     *       - (O): Reset Main MCU. Normal high.
     */
    DDRD = 0b11101000;

    /**
     * PORTC
     *   - PC7 (O): TX LED.
     *   - PC6 (O): RX LED.
     *   - PC5 (X)
     *   - PC4 (X)
     *   - PC3 (X)
     *   - PC2 (X)
     *   - PC1 (X)
     *   - PC0 (X)
     */
    DDRC = 0b11000000;

    /**
     * PORTB
     *   - PB7 (X)
     *   - PB6 (I): ASAID bus bit 2.
     *   - PB5 (I): ASAID bus bit 1.
     *   - PB4 (I): ASAID bus bit 0.
     *   - PB3 (X)
     *   - PB2 (X)
     *   - PB1 (X)
     *   - PB0 (X)
     */
    DDRB = 0b00000000;
}

/**
 * @brief Dectect the ASAID on ASA BUS.
 * 
 * @return uint8_t ASAID (0 ~ 7)
 */
static inline uint8_t asa_id_bus(void)
{
    return ((PINB & 0b01110000) >> 4);
}

/**
 * @brief Dectect the ASAID on Knob.
 * 
 * @return uint8_t ASAID (0 ~ 9)
 */
static inline uint8_t asa_id_knob(void)
{
    return ((PINF & 0b11110000) >> 4);
}

uint8_t asa_id_compared(void)
{
    return asa_id_bus() == asa_id_knob();
}

uint8_t m_s_detect(void)
{
    if (PINF & (1 << PF1))
        return MS_MODE_MASTER;
    else
        return MS_MODE_SLAVE;
}

void m_s_update(uint8_t mode)
{
    switch (mode) {
        case MS_MODE_SLAVE: {
            PORTD &= ~(1 << PD6);
            if (asa_id_compared())
                PORTD &= ~(1 << PD5);
            else
                PORTD |=  (1 << PD5);
            break;
        }
        case MS_MODE_MASTER: {
            PORTD |= (1 << PD6);
            break;
        }
    }
}

uint8_t p_r_detect(void)
{
    if (PINF & (1 << PF0))
        return PR_MODE_PROG;
    else
        return PR_MODE_RUN;
}

void p_r_update(uint8_t mode)
{
    switch (mode) {
        case PR_MODE_PROG: {
            PORTD |= (1 << PD7);
            break;
        }
        case PR_MODE_RUN: {
            PORTD &= ~(1 << PD7);
            break;
        }
    }
}

void tx_led_on(void)
{
    PORTC &= ~(1 << PC7);
}

void tx_led_off(void)
{
    PORTC |= (1 << PC7);
}

void rx_led_on(void)
{
    PORTC &= ~(1 << PC6);
}

void rx_led_off(void)
{
    PORTC |= (1 << PC6);
}

/**
 * pull down time = 1 / f_cpu  * N
 *                = 1 / 16 MHz * 16
 *                = 1e-6 s
 *                = 1 us
 */
void reset_main_mcu(void)
{
    // Set the Main MCU reset pin (PD0) as output, and pull it down.
    DDRD |= (1 << PD0);
    PORTD &= ~(1 << PD0);

    // Consume 16 clock cycle to pull down reset pin.
    // This delay time depends on the minimum pull low duaration of Main MCU reset pin.
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");

    // Pull high PD0, and reset it to input.
    PORTD |= (1 << PD0);
    DDRD &= ~(1 << PD0);
}

// FIXME: pull high when Prog mode.
void prog_main_mcu(void)
{
    // Set Main MCU Prog/Run mode pin (PD7) as output, and pull it high.
    DDRD |= (1 << PD7);
    PORTD &= ~(1 << PD7);
}

// FIXME: pull down when Prog mode.
void run_main_mcu(void)
{
    // Set Main MCU Prog/Run mode pin (PD7) as output, and pull it down.
    DDRD |= (1 << PD7);
    PORTD |= (1 << PD7);
}
