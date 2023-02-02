#include <avr/io.h>
#include "interrupt.h"

void uart_int_init(void)
{
    UCSR1B |= (1 << RXCIE1) | (1 << TXCIE1);
}

void uart_int_deinit(void)
{
    UCSR1B &= ~(1 << RXCIE1);
    UCSR1B &= ~(1 << TXCIE1);
}

void tim_init(void)
{
    TCCR1B = (1 << WGM12) | (1 << CS11);
    TIMSK1 = (1 << OCIE1A);
    OCR1A = 9999;
}

void ex_int_init(void)
{
    EICRA = (1 << ISC01) | (1 << ISC00);  // PD0 (EXT0) rising edge-triggered
    EICRA |= (1 << ISC10);                // PD1 (EXT1) rising/falling edge-triggered
    EIMSK = (1 << INT0) | (1 << INT1);
}
