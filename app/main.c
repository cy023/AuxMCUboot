#include <avr/io.h>
#include <stdio.h>

#include "driver/auxmcu_io.h"
#include "driver/commuch.h"
#include "driver/interrupt.h"
// #include "boot/bootprotocol.h"

#include "Descriptors.h"
#include "DualVirtualSerial.h"

void comu_handler(void);
void ctrl_handler(void);

ISR (USART1_RX_vect)
{
    volatile uint8_t data = UDR1;
    if (CDC_comu.State.ControlLineStates.HostToDevice == 3) {
        CDC_Device_SendByte(&CDC_comu, data);
        rx_led_on();
    }
}

ISR (USART1_TX_vect)
{
    tx_led_on();
}

ISR (INT0_vect)
{
    uint8_t state;
    state = p_r_detect();
    p_r_update(state);

    if (state == PR_MODE_PROG) {
        state = m_s_detect();
        m_s_update(state);
    }
}

ISR (INT1_vect)
{
    uint8_t state;
    state = m_s_detect();
    m_s_update(state);
}

ISR (TIMER1_COMPA_vect)
{
    tx_led_off();
    rx_led_off();
}

/** Configures the board hardware and chip peripherals for the demo's functionality. */
void SetupHardware(void)
{
    /* Disable watchdog if enabled by bootloader/fuses */
    MCUSR &= ~(1 << WDRF);
    wdt_disable();

    /* Disable clock division */
    clock_prescale_set(clock_div_1);

    /* Hardware Initialization */
    ex_int_init();
    tim_init();
    gpio_init();
    USB_Init();
}

int main(void)
{
    SetupHardware();
    // Set PD2 and PD3 Hi-Z to prevent UART competition
    // When USB plugs in, the "EVENT_USB_Device_Connect" will trigger uart initialization.
    DDRD &= ~(3 << 2);
    PORTD |= (3 << 2);
    MCUCR |= (1 << 4);

    GlobalInterruptEnable();

    for (;;) {
        comu_handler();
        ctrl_handler();
    }
}

void comu_handler(void)
{
    int16_t ReceivedByte;

    ReceivedByte = CDC_Device_ReceiveByte(&CDC_comu);
    if (!(ReceivedByte < 0)) {
        com_channel_putc((uint8_t)ReceivedByte);
    }

    CDC_Device_USBTask(&CDC_comu);
    USB_USBTask();
}

void ctrl_handler(void)
{
    // TODO:
    CDC_Device_USBTask(&CDC_ctrl);
    USB_USBTask();
}
