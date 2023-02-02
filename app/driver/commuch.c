#include <avr/io.h>
#include "commuch.h"

#define BAUDRATE    38400UL
#define UBRR_VAL    (F_CPU / 16 / BAUDRATE - 1)

void com_channel_init(void)
{
    UBRR1H = (uint8_t)(UBRR_VAL >> 8);
    UBRR1L = (uint8_t)UBRR_VAL;
    UCSR1B = (1 << RXEN1) | (1 << TXEN1);
    UCSR1C = (3 << UCSZ10);
}

void com_channel_deinit(void)
{
    UBRR1 = 0;
    UCSR1B = 0;
    UCSR1C = 0;
}

void com_channel_putc(uint8_t c)
{
    while (!(UCSR1A & (1 << UDRE1)))
        ;
    UDR1 = c;
}
