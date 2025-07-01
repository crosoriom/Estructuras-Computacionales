#include "exti.h"

/**
 * @brief Helper function to get the correct NVIC IRQn for a given EXTI line.
 * @param[in] pin The pin number (0-15).
 * @return IRQn_Type The corresponding IRQ number for the pin.
 */
static IRQn_t get_irqn_for_exti_line(uint8_t pin)
{
    if (pin == 0) return EXTI0_IRQn;
    if (pin == 1) return EXTI1_IRQn;
    if (pin == 2) return EXTI2_IRQn;
    if (pin == 3) return EXTI3_IRQn;
    if (pin == 4) return EXTI4_IRQn;
    if (pin >= 5 && pin <= 9) return EXTI9_5_IRQn;
    
    // Default case for pins 10-15
    return EXTI15_10_IRQn;
}

void exti_gpio_init(gpio_t *GPIOx, PINx pin, Trigger_t trigger)
{
    // 1. Configure the GPIO pin as an input.
    configure_gpio_input(GPIOx, pin);

    // 2. Map the GPIO port to the EXTI line using SYSCFG.
    exti_map(GPIOx, pin);

    // 3. Unmask the interrupt request from the EXTI line.
    EXTI->IMR1 |= (1 << pin);

    // 4. Configure the trigger edge selection.
    switch (trigger) {
    case RISING_EDGE:
        EXTI->RTSR1 |= (0x1 << pin);
        EXTI->FTSR1 &= ~(0x1 << pin);
        break;
    case FALLING_EDGE:
        EXTI->RTSR1 &= ~(0x1 << pin);
        EXTI->FTSR1 |= (0x1 << pin);
        break;
    case BOTH_EDGES:
        EXTI->RTSR1 |= (0x1 << pin);
        EXTI->FTSR1 |= (0x1 << pin);
        break;
    }

    // 4. Configure the trigger edge selection.
    nvic_irq_enable(get_irqn_for_exti_line(pin));
}

void usart_interrupt_enable(uint8_t USART)
{
    switch(USART) {                          //enable nvic irq
        case 1:
            nvic_irq_enable(USART1_IRQn);
            break;
        case 2:
            nvic_irq_enable(USART2_IRQn);
            break;
        case 3:
            nvic_irq_enable(USART3_IRQn);
            break;
    }
}

// This default handler clears the interrupt flag to prevent a crash,
// then enters an infinite loop. This helps a developer immediately see
// that an unhandled interrupt has occurred during debugging.

#define DEFAULT_IRQ_HANDLER(line_bit) \
    do { \
        if (EXTI->PR1 & (line_bit)) { \
            EXTI->PR1 = (line_bit); \
        } \
        while(1); \
    } while(0)

__attribute__((weak)) void EXTI0_IRQHandler(void)   { DEFAULT_IRQ_HANDLER(1U << 0); }
__attribute__((weak)) void EXTI1_IRQHandler(void)   { DEFAULT_IRQ_HANDLER(1U << 1); }
__attribute__((weak)) void EXTI2_IRQHandler(void)   { DEFAULT_IRQ_HANDLER(1U << 2); }
__attribute__((weak)) void EXTI3_IRQHandler(void)   { DEFAULT_IRQ_HANDLER(1U << 3); }
__attribute__((weak)) void EXTI4_IRQHandler(void)   { DEFAULT_IRQ_HANDLER(1U << 4); }

__attribute__((weak)) void EXTI9_5_IRQHandler(void) {
    // For shared handlers, we must check which line caused it.
    // This default implementation just clears all possible flags and spins.
    uint32_t pending_flags = EXTI->PR1 & (0x1F << 5); // Check lines 5 through 9
    EXTI->PR1 = pending_flags; // Clear all pending flags found
    while(1);
}

__attribute__((weak)) void EXTI15_10_IRQHandler(void) {
    uint32_t pending_flags = EXTI->PR1 & (0x3F << 10); // Check lines 10 through 15
    EXTI->PR1 = pending_flags; // Clear all pending flags found
    while(1);
}
