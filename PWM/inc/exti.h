#ifndef EXTI_H
#define EXTI_H

#include <stdint.h>
#include "syscfg.h"
#include "gpio.h"
#include "nvic.h"

#define EXTI ((ExtendedInterrupts_t *)0x40010400UL)

typedef struct {
    volatile uint32_t IMR1;
    volatile uint32_t EMR1;
    volatile uint32_t RTSR1;
    volatile uint32_t FTSR1;
    volatile uint32_t SWIER1;
    volatile uint32_t PR1;
    volatile uint32_t RESERVED[3];
    volatile uint32_t IMR2;
    volatile uint32_t EMR2;
    volatile uint32_t RTSR2;
    volatile uint32_t FTSR2;
    volatile uint32_t SWIER2;
    volatile uint32_t PR2;
}ExtendedInterrupts_t;

typedef enum {
    RISING_EDGE,
    FALLING_EDGE,
    BOTH_EDGES
}Trigger_t;

/**
 * @brief Activa el reloj del GPIO, y configura el pin para recibir interrupciones.
 *
 * La función es igual a configure_gpio_input() pero además se encarga
 * de activar las interrupciones correspondientes del pin que se recibe
 * como argumento.
 *
 * @param *GPIOx Dirección del puerto GPIO.
 * @param pin Enumeración del pin a configurar
 * @param trigger Enumeración que indica el tipo de disparo para la
 * interrupción:
 *                  - RISING_EDGE
 *                  - FALLING_EDGE
 *                  - BOTH_EDGES
 *
 * @note Si solo se desea establecer el pin como salida y no activar
 * su interrupción, vease configure_gpio_input().
 */
void gpio_interrupt_enable(gpio_t *GPIOx, PINx pin, Trigger_t trigger);

/**
 * @brief Activa las interrupciones de UART.
 *
 * La función configura los registros para activar
 * la interrupción del NVIC (vector global de
 * interrupciones) correspondiente al puerto de
 * comunicación serial UART o USART que se esté
 * utilizando.
 *
 * @param *USARTx Dirección del dispositivo de
 * comunicación a configurar.
 */
void usart_interrupt_enable(uint8_t USARTx);

#endif
