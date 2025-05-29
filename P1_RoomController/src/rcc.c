#include "rcc.h"

void activate_syscfg()
{
	RCC->APB2ENR |= (0x1 << 0);
}

void gpio_activate(uint8_t gpio)
{
	if(gpio == 0xFF)
		return;
	RCC->AHB2ENR |= (0x1 << gpio);
}

void usart_activate(uint8_t usart)
{
	switch(usart) {
		case 1:
			RCC->APB2ENR |= (0x1 << 14);
		case 2:
			RCC->APB1ENR1 |= (0x1 << 17);
		case 3:
			RCC->APB1ENR1 |= (0x1 << 18);
		case 4:
			RCC->APB1ENR1 |= (0x1 << 19);
		case 5:
			RCC->APB1ENR1 |= (0x1 << 20);
		default:
			return;
	}
}

void tim_activate(uint8_t timer)
{
	switch(timer) {
		case 1:
			RCC->APB2ENR |= (0x1 << 11);
		case 2:
			RCC->APB1ENR1 |= (0x1 << 0);
		case 3:
			RCC->APB1ENR1 |= (0x1 << 1);
		case 4:
			RCC->APB1ENR1 |= (0x1 << 2);
		case 5:
			RCC->APB1ENR1 |= (0x1 << 3);
		case 6:
			RCC->APB1ENR1 |= (0x1 << 4);
		case 7:
			RCC->APB1ENR1 |= (0x1 << 5);
		case 8:
			RCC->APB2ENR |= (0x1 << 13);
		case 15:
			RCC->APB2ENR |= (0x1 << 16);
		case 16:
			RCC->APB2ENR |= (0x1 << 17);
		case 17:
			RCC->APB2ENR |= (0x1 << 18);
		default:
			return;
	}
}
