#include "gpio.h"

uint8_t get_gpio_number(gpio_t *GPIOx)
{
	if(GPIOx == GPIOA) return 0;
	else if(GPIOx == GPIOB) return 1;
	else if(GPIOx == GPIOC) return 2;
	else if(GPIOx == GPIOD) return 3;
	else if(GPIOx == GPIOE) return 4;
	else if(GPIOx == GPIOF) return 5;
	else if(GPIOx == GPIOG) return 6;
	else if(GPIOx == GPIOH) return 7;
	else return 0xFF;
}

void reset_gpio_mode(gpio_t *GPIOx, PINx pin)
{
	GPIOx->MODER &= ~(0x3U << (2 * pin));
}

void configure_gpio_input(gpio_t *GPIOx, PINx pin)
{
	gpio_activate(get_gpio_number(GPIOx));
	reset_gpio_mode(GPIOx, pin);
}

void configure_gpio_output(gpio_t *GPIOx, PINx pin)
{
	gpio_activate(get_gpio_number(GPIOx));
	reset_gpio_mode(GPIOx, pin);
	GPIOx->MODER |= (0x1U << (2 * pin));
	GPIOx->OTYPER &= ~(0x1U << pin);
}

void configure_gpio_ODmode(gpio_t *GPIOx, PINx pin)
{
	gpio_activate(get_gpio_number(GPIOx));
	reset_gpio_mode(GPIOx, pin);
	GPIOx->MODER |= (0x1U << (2 * pin));
	GPIOx->OTYPER |= (0x1U << pin);
}

void configure_gpio_alternateFunctionMode(gpio_t *GPIOx, PINx pin)
{
	gpio_activate(get_gpio_number(GPIOx));
	reset_gpio_mode(GPIOx, pin);
	GPIOx->MODER |= (0x2U << (2 * pin));
}

void configure_gpio_usart(gpio_t *GPIOx, PINx pin)
{
    configure_gpio_alternateFunctionMode(GPIOx, pin);
    if(pin <= PIN_7) {                                  //select register for mapping de AF7(UART) to pin
        GPIOx->AFRL &= ~(0xFU << (4 * pin));            //resets the register
        GPIOx->AFRL |= (0x7U << (4 * pin));             //writes 0111 in the register
    } else {
        GPIOx->AFRH &= ~(0xFU << (4 * pin));
        GPIOx->AFRH |= (0x7U << (4 * pin));
    }
    GPIOx->OSPEEDR |= (0x3U <<(2 * pin));               //gpio CLK to very high speed for uart com
    GPIOx->PUPDR &= ~(0x3U <<(2 * pin));                //no pull-up pull-down mode
}

void configure_gpio_pwm(gpio_t *GPIOx, PINx pin, uint8_t alternateFunction)
{
	configure_gpio_alternateFunctionMode(GPIOx, pin);
	if(alternateFunction == 0xFFU)
		return;
    if(pin <= PIN_7) {                                  //select register for mapping de AF7(UART) to pin
        GPIOx->AFRL &= ~(0xFU << (4 * pin));            //resets the register
        GPIOx->AFRL |= (alternateFunction << (4 * pin));             //writes the corresponding AF in the register
    } else {
        GPIOx->AFRH &= ~(0xFU << (4 * pin));
        GPIOx->AFRH |= (alternateFunction << (4 * pin));
    }
}

void gpio_set_highLevel(gpio_t *GPIOx, PINx pin)
{
	GPIOx->ODR |= (0x1U << pin);
}

void gpio_set_lowLevel(gpio_t *GPIOx, PINx pin)
{
	GPIOx->ODR &= ~(0x1U << pin);
}

int gpio_getLevel(gpio_t *GPIOx, PINx pin, InputMode_t mode)
{
	switch(mode) {
	case PULLUP:
		if((GPIOx->IDR & (1 << pin)) == 0)
			return 1;
		else
			return 0;
		break;
	case PULLDOWN:
		if((GPIOx->IDR & (1 << pin)) == 1)
			return 1;
		else
			return 0;
		break;
	}
}

void gpio_toggle_level(gpio_t *GPIOx, PINx pin)
{
	GPIOx->ODR ^= (0x1 << pin);
}
