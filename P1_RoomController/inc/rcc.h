#ifndef RCC_H
#define RCC_H

#include <stdint.h>

#define RCC ((ResetClockControl_t *)0x40021000UL)

typedef struct {
	volatile uint32_t CR;
	volatile uint32_t ICSCR;
	volatile uint32_t CFGR;
	volatile uint32_t PLLCFGR;
	volatile uint32_t PLLSAI1CFGR;
	volatile uint32_t PLLSAI2CFGR;
	volatile uint32_t CIER;
	volatile uint32_t CIFR;
	volatile uint32_t CICR;
	volatile uint32_t RESERVED0;
	volatile uint32_t AHB1RSTR;
	volatile uint32_t AHB2RSTR;
	volatile uint32_t AHB3RSTR;
	volatile uint32_t RESERVED1;
	volatile uint32_t APB1RSTR1;
	volatile uint32_t APB1RSTR2;
	volatile uint32_t APB2RSTR;
	volatile uint32_t RESERVED2;
	volatile uint32_t AHB1ENR;
	volatile uint32_t AHB2ENR;
	volatile uint32_t AHB3ENR;
	volatile uint32_t RESERVED3;
	volatile uint32_t APB1ENR1;
	volatile uint32_t APB1ENR2;
	volatile uint32_t APB2ENR;
	volatile uint32_t RESERVED4;
	volatile uint32_t AHB1SMENR;
	volatile uint32_t AHB2SMENR;
	volatile uint32_t AHB3SMENR;
	volatile uint32_t RESERVED5;
	volatile uint32_t APB1SMENR1;
	volatile uint32_t APB1SMENR2;
	volatile uint32_t APB2SMENR;
	volatile uint32_t RESERVED6;
	volatile uint32_t CCIPR;
	volatile uint32_t BDCR;
	volatile uint32_t CSR;
}ResetClockControl_t;

/**
 * @brief Activa el reloj de SysCfg
 *
 * La función activa el reloj que permite las
 * configuraciones del sistema, tales como
 * buses, interrupciones, etc.
 */
void activate_syscfg(void);

/**
 * @brief Activa el reloj del GPIO
 *
 * La función se encarga de activar el
 * reloj de los GPIOs. Recibe como argumento
 * un número del 0 al 7 para cada puerto.
 *
 * @param gpio número correspondiente al
 * puerto a activar.
 *
 * @warning Si recibe un número que no esté entre
 * el rango especificado o 0xFF puede ocurrir un
 * error.
 */
void gpio_activate(uint8_t gpio);

/**
 * @brief Activa el reloj para UART
 *
 * La función activa el reloj que permite usar
 * los periféricos de comunicación serial UART o
 * USART. Debe recibir como argumento un número
 * entre 1 y 5.
 *
 * @param usart Número correspondiente para
 * el periferico a activar.
 *
 * @note Si el número que se pasa como argumento
 * no está dentro del rango indicado, no ocurrirá
 * nada.
 */
void usart_activate(uint8_t usart);

/**
 * @brief	Activa el reloj para los timers
 *
 * La función activa el reloj que permite usar los contadores de
 * pulsos para manejo de tiempo.
 *
 * @param	timmer Número correspondiente al periferico a ser
 * 			activado.
 *
 * @note	Si el número que se pasa como argumento no
 * 			corresponde a un timer existente, no ocurrirá nada.
 */
void tim_activate(uint8_t timer);

#endif
