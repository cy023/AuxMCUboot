#ifndef COMCHANNEL_H
#define COMCHANNEL_H

#include <inttypes.h>

void com_channel_init(void);

void com_channel_deinit(void);

void com_channel_putc(uint8_t c);

#endif /* COMCHANNEL_H */
