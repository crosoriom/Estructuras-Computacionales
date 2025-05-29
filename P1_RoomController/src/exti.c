#include "exti.h"

void gpio_interrupt_enable(gpio_t *GPIOx, PINx pin, Trigger_t trigger)
{
    configure_gpio_input(GPIOx, pin);
    exti_map(GPIOx, pin);

    EXTI->IMR1 |= (1 << pin);
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
    if(pin >= 10)
        nvic_irq_enable(EXTI15_10_IRQn);
    else if (pin >= 5)
        nvic_irq_enable(EXTI9_5_IRQn);
    else if (pin == 4)
        nvic_irq_enable(EXTI4_IRQn);
    else if (pin == 3)
        nvic_irq_enable(EXTI3_IRQn);
    else if (pin == 2)
        nvic_irq_enable(EXTI2_IRQn);
    else if (pin == 1)
        nvic_irq_enable(EXTI1_IRQn);
    else
        nvic_irq_enable(EXTI0_IRQn);
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

void EXTI4_IRQHandler()
{
    /*Insertar código*/;
}

void EXTI3_IRQHandler()
{
    EXTI->PR1 = (1 << 3);
    /*Insertar código*/;
}

void EXTI2_IRQHandler()
{
    EXTI->PR1 = (1 << 2);
    /*Insertar código*/;
}

void EXTI1_IRQHandler()
{
    /*Insertar código*/;
}

void EXTI0_IRQHandler()
{
    /*Insertar código*/;
}
