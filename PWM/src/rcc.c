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

void rcc_clock_selector(rcc_clock_t clock)
{
	switch (clock) {
	case MSI_CLOCK:
		return; // Reloj usado por defecto en el hardware

	case HSI_CLOCK:
		RCC->CR |= HSI_CLOCK_ENABLE;
		while(!(RCC->CR & HSI_CLOCK_READY));

		RCC->CFGR &= ~(0x3 << 0);
		RCC->CFGR |= (0x1 << 0);
		while(!(RCC->CFGR & HSI_SWS_READY));
		return;

	case PLL_CLOCK:
        // --- PASO 1: CONFIGURAR LA LATENCIA DE LA FLASH (¡CRÍTICO!) ---
        // Se llama a la función del driver de FLASH ANTES de cambiar el reloj.
        flash_configure_for_high_speed();

        // --- PASO 2: CONFIGURAR Y HABILITAR EL PLL ---
        // Fuente: MSI (4 MHz), PLLN = *40, PLLM = /1, PLLR = /2 => 80 MHz
		while(!(RCC->CR & MSI_READY));

        // Seleccionar MSI como fuente del PLL
		RCC->PLLCFGR &= ~(0x3 << 0);
		RCC->PLLCFGR |= (0x1 << 0); // Fuente MSI

        // Configurar multiplicador PLLN = 40
		RCC->PLLCFGR &= ~(0x7F << 8);
		RCC->PLLCFGR |= (40 << 8);

        // Configurar divisores PLLM y PLLR a /1 y /2 respectivamente (valores 0b000 y 0b00)
		RCC->PLLCFGR &= ~(0x7 << 4);  // Limpiar PLLM
        RCC->PLLCFGR &= ~(0x3 << 25); // Limpiar PLLR

        // Habilitar la salida principal del PLL (PLLREN)
        RCC->PLLCFGR |= (0x1 << 24);

        // Habilitar el PLL
		RCC->CR |= (0x1 << 24);
		while(!(RCC->CR & (0x1 << 25))); // Esperar a que el PLL esté listo (PLLRDY)

        // --- PASO 3: CONFIGURAR PRESCALERS DE BUS (si es necesario) ---
        // Por ahora, los dejamos sin división para máxima velocidad en todos los buses
        RCC->CFGR &= ~(0xF << 4);  // AHB HPRE = No división
        RCC->CFGR &= ~(0x7 << 8);  // APB1 PPRE1 = No división
        RCC->CFGR &= ~(0x7 << 11); // APB2 PPRE2 = No división

        // --- PASO 4: SELECCIONAR EL PLL COMO RELOJ DEL SISTEMA ---
		RCC->CFGR &= ~(0x3 << 0); // Limpiar bits SW
		RCC->CFGR |= (0x3 << 0);  // Seleccionar PLL

		while(!((RCC->CFGR & (0x3 << 2)) == (0x3 << 2))); // Esperar a que SWS indique PLL

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
