#ifndef INTERRUPT_H
#define INTERRUPT_H

/**
 * @brief uart1 interrupt initialization.
 * 
 *  TX/RX interrupt enable.
 * 
 */
void uart_int_init(void);

/**
 * @brief uart1 interrupt deinitialization.
 * 
 *  TX/RX interrupt disable.
 * 
 */
void uart_int_deinit(void);

/**
 * @brief timer1 interrupt initialization.
 * 
 * Mode: CTC
 * Prescaler: 8
 * OCR1 = 9999
 * 
 * F_tim = 16000000 / (2 * prescaler * (OCRn + 1))
 *       = 100 hz
 * 
 * F_int = F_tim * 2
 *       = 200 hz
 * 
 */
void tim_init(void);

/**
 * @brief external interrupt initialization.
 *
 * Reset signal:
 *      PD0 (EXT0), rising edge-triggered.
 * 
 * ASA BUS CS signal:
 *      PD1 (EXT1), rising and falling edge-triggered.
 * 
 */
void ex_int_init(void);

#endif /* INTERRUPT_H */
