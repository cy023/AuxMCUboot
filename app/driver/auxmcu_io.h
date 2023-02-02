/**
 * 
 * ========== Basic Operation ==========
 * 
 * Prog/Run switch              PF0 (I)
 * Prog/Run Mode to Main MCU    PD7 (O) / PG1 (?)
 * 
 * Reset_Main MCU               PD0 (O)
 * 
 * UART1 TX <-> Main MCU RX     PD3 (O)
 * UART1 RX <-> Main MCU TX     PD2 (I)
 * 
 * TX LED                       PC7 (O)
 * RX LED                       PC6 (O)
 * 
 * ========== ASA BUS Extended ==========
 * 
 * M/S switch                   PF1 (I)
 * M/S mode to Main MCU         PD6 (O)
 * 
 * ASAID knob - b0              PF4 (I)
 * ASAID knob - b1              PF5 (I)
 * ASAID knob - b2              PF6 (I)
 * ASAID knob - b3              PF7 (I)
 * 
 * ASAID bus - Addr0            PB4 (I)
 * ASAID bus - Addr1            PB5 (I)
 * ASAID bus - Addr2            PB6 (I)
 * 
 * ASA bus CS                   PD1 (I)
 * CS pin to Main MCU           PD5 (O)
 *
 */

#ifndef AUXMCU_IO_H
#define AUXMCU_IO_H

#include <inttypes.h>

#define MS_MODE_MASTER  1
#define MS_MODE_SLAVE   0

#define PR_MODE_PROG    1
#define PR_MODE_RUN     0

/**
 * @brief GPIO initialization.
 */
void gpio_init(void);

/**
 * @brief Compare the ASAID on ASA BUS and on the knob.
 * 
 * @return uint8_t 
 *      - 1 : matched.
 *      - 0 : no matched.
 */
uint8_t asa_id_compared(void);

/**
 * @brief Dectect the M/S mode for ASA BUS.
 *
 * @return uint8_t
 *      - MS_MODE_MASTER (0) : Master mode.
 *      - MS_MODE_SLAVE  (1) :  Slave mode.
 */
uint8_t m_s_detect(void);

/**
 * @brief Send the M/S mode signal from Aux MCU to Main MCU.
 * 
 * @param mode Master/Slave mode.
 *      - MS_MODE_MASTER (0) : Master mode.
 *      - MS_MODE_SLAVE  (1) :  Slave mode.
 */
void m_s_update(uint8_t mode);

/**
 * @brief Aux MCU dectect the Prog/Run switch status.
 *
 * @return uint8_t
 *      - PR_MODE_PROG (1) : Prog mode.
 *      - PR_MODE_RUN  (0) :  Run mode.
 */
uint8_t p_r_detect(void);

/**
 * @brief Send the Prog/Run mode signal from Aux MCU to Main MCU.
 * 
 * @param mode Prog/Run mode signal.
 *      - PR_MODE_PROG (1) : Prog mode.
 *      - PR_MODE_RUN  (0) :  Run mode.
 */
void p_r_update(uint8_t mode);

/**
 * @brief Turn on the TX LED.
 */
void tx_led_on(void);

/**
 * @brief Turn on the TX LED.
 */
void tx_led_off(void);

/**
 * @brief Turn on the RX LED.
 */
void rx_led_on(void);

/**
 * @brief Turn off the RX LED.
 */
void rx_led_off(void);

/**
 * @brief Send the reset signal from Aux MCU to Main MCU.
 */
void reset_main_mcu(void);

/**
 * @brief Set Main MCU Prog/Run mode pin to "Prog" (high).
 */
void prog_main_mcu(void);

/**
 * @brief Set Main MCU Prog/Run mode pin to "Run" (low).
 */
void run_main_mcu(void);

#endif  /* AUXMCU_IO_H */
