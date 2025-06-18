#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include "rcc.h"

/*Base address for all GPIOs*/
#define GPIOA ((gpio_t *)0x48000000UL)
#define GPIOB ((gpio_t *)0x48000400UL)
#define GPIOC ((gpio_t *)0x48000800UL)
#define GPIOD ((gpio_t *)0x48000C00UL)
#define GPIOE ((gpio_t *)0x48001000UL)
#define GPIOF ((gpio_t *)0x48001400UL)
#define GPIOG ((gpio_t *)0x48001800UL)
#define GPIOH ((gpio_t *)0x48001C00UL)

typedef enum {
	PIN_0,
	PIN_1,
	PIN_2,
	PIN_3,
	PIN_4,
	PIN_5,
	PIN_6,
	PIN_7,
	PIN_8,
	PIN_9,
	PIN_10,
	PIN_11,
	PIN_12,
	PIN_13,
	PIN_14,
	PIN_15
}PINx;

typedef enum {
	PULLUP,
	PULLDOWN
}InputMode_t;

typedef struct {
	volatile uint32_t MODER;
	volatile uint32_t OTYPER;
	volatile uint32_t OSPEEDR;
	volatile uint32_t PUPDR;
	volatile uint32_t IDR;
	volatile uint32_t ODR;
	volatile uint32_t BSRR;
	volatile uint32_t LCKR;
	volatile uint32_t AFRL;
	volatile uint32_t AFRH;
	volatile uint32_t BRR;
	volatile uint32_t ASCR;
}gpio_t;

typedef struct {
	gpio_t *GPIOx;
	PINx pin;
	uint8_t afNumber;
}gpio_af_map_t;

/**
 * @brief Tranforma la dirección de un GPIO en un entero
 *
 * La función convierte la dirección de uno de los puertos
 * GPIO en un número entero de 8 bits.
 *
 * @param[in] *GPIOx Dirección al puerto GPIO
 * @return Numero entero equivalente al puerto pasado como
 * argumento:
 * 				GPIOA -> 0
 * 				...
 * 				GPIOH -> 7
 *
 * @warning La función retorna un valor de 0xFF si se
 * suministra un argumento inválido.
 */
uint8_t get_gpio_number(gpio_t *GPIOx);

/**
 * @brief Establece el pin del GPIO como entrada
 *
 * La función se encarga de activar el reloj del GPIO
 * pasado como argumento y escribe el registro al pin
 * correspondiente para configurar este como entrada.
 *
 * @param[in] *GPIOx Dirección al puerto GPIO
 * @param[in] pin Enumeración correspondiente del pin
 * deseado
 *
 * @note Si se desea activar las interrupciones
 * externas para los pines GPIO, vease la función
 * gpio_interrupt_enable().
 */
void configure_gpio_input(gpio_t *GPIOx, PINx pin);

/**
 * @brief Establece el pin del GPIO como salida
 *
 * La función se encarga de activar el reloj del GPIO
 * pasado como argumento y escribe el registro del pin
 * correspondiente para configurarlo como salida.
 *
 * @param[in] *GPIOx Dirección al puerto GPIO
 * @param[in] pin Enumeración correspondiente del pin
 * deseado
 *
 * @note Esta función configura el pin como usa salida
 * simple, para modo Open-Drain use configure_gpio_ODmode.
 */
void configure_gpio_output(gpio_t *GPIOx, PINx pin);

/**
 * @brief Establece el pin del GPIO en salida OD
 *
 * La función se encarga de activar el reloj del GPIO
 * pasado como argumento y escribe los registros del
 * pin correspondiente para configurarlo como salida
 * Open-Drain.
 *
 * @param[in] *GPIOx Dirección al puerto GPIO
 * @param[in] pin Enumeración correspondiente del pin
 * deseado
 */
void configure_gpio_ODmode(gpio_t *GPIOx, PINx pin);

/**
 * @brief Configura el pin del GPIO en modo alterno.
 *
 * La función se encarga de activar el reloj del GPIO
 * pasado como argumento y escribe el registro del pin
 * correspondiente para que este funcione como salida
 * o entrada de alguno de los periféricos del MC.
 *
 * @param[in] *GPIOx Dirección al puerto GPIO
 * @param[in] pin Enumeración correspondiente del pin
 * deseado
 */
void configure_gpio_alternateFunctionMode(gpio_t *GPIOx, PINx pin);

/**
 * @brief Configura el pin del GPIO para UART/USART.
 *
 * La función se encarga de configurar el pin del
 * GPIO para funcionar como tx/rx en comunicación
 * serial UART/USART.
 *
 * @param[in] *GPIOx Dirección al puerto GPIO
 * @param[in] pin Enumeración correspondiente del pin
 * a configurar
 */
void configure_gpio_usart(gpio_t *GPIOx, PINx pin);

/**
 * @brief Configura el pin del GPIO para control PWM.
 *
 * La función se encarga de configurar el pin del
 * GPIO para que por medio del los timers fe propósito
 * general se genere un tren de pulsos cuadrados.
 *
 * @param[in] *GPIOx Direcciión al puerto GPIO
 * @param[in] pin Enumeración correspondiente del pines
 * a configurar
 * @param[in] AF número en formato hezadecimal correspondiente
 * a la función a configurar
 */
void configure_gpio_pwm(gpio_t *GPIOx, PINx pin, uint8_t AF);

/**
 * @brief Establece el nivel de salida del pin en bajo.
 *
 * La función se encarga de establecer en el registro
 * ODR al pin del GPIO correspondiente los bits
 * apropiados para establecerlo en nivel bajo.
 *
 * @param[in] *GPIOx Dirección al puerto GPIO
 * @param[in] pin Enumeración correspondiente del pin
 * deseado
 *
 * @warning El pin debe estar configurado como salida.
 */
void gpio_set_lowLevel(gpio_t *GPIOx, PINx pin);

/**
 * @brief Establece el nivel de salida del pin en alto.
 *
 * La función se encarga de establecer en el registro
 * ODR al pin del GPIO correspondiente los bits
 * apropiados para establecerlo en nivel alto.
 *
 * @param[in] *GPIOx Dirección al puerto GPIO
 * @param[in] pin Enumeración correspondiente del pin
 * deseado
 *
 * @warning El pin debe estar configurado como salida.
 */
void gpio_set_highLevel(gpio_t *GPIOx, PINx pin);

int gpio_getLevel(gpio_t *GPIOx, PINx pin, InputMode_t mode);

/**
 * @brief Intercambia el nivel de salida del pin.
 *
 * La función aplica una operación XOR al estado
 * altual del pin GPIO. Si este último se encuentra
 * en estado alto lo pasa a estado bajo y visceversa.
 *
 * @param[in] *GPIOx Dirección al puerto GPIO
 * @param[in] pin Enumeración correspondiente del pin
 * deseado
 *
 * @warning El pin debe estar configurado como salida.
 */
void gpio_toggle_level(gpio_t *GPIOx, PINx pin);

#endif
