#include "main.h"

usart_config_t uartConfig = {
    .USART_PORT = USART2,
    .GPIO_PORT = GPIOA,
    .TX_PIN = PIN_2,
    .RX_PIN = PIN_3,
    .BAUDRATE = 115200,
    .STOP_BITS = ONE_STOP_BIT,
    .WORD_LENGHT = EIGHT_BITS_LENGHT
};

pwm_config_t pwmConfig = {
    .pwmTimer = TIM3,
    .pwmChannel = TIM_CHANNEL1,
    .period = PWM_PERIOD,
    .prescaler = PWM_PRESCALER
};

int main()
{
    systick_init(4000);
    gpio_interrupt_enable(GPIOC, PIN_13, FALLING_EDGE);
    configure_gpio_output(GPIOA, PIN_5);
    usart_init(&uartConfig);
    usart_interrupt_enable(2);
    pwm_init(&pwmConfig);
    pwm_set_dutyCycle(TIM3,     TIM_CHANNEL1, 70);
    usart_send_string(USART2, "\n\rSistema Inicializado. Esperando eventos.\n\r");
    for(;;) {
        control_external_led();
        heartbeat();
    }
    return 0;
}

void EXTI15_10_IRQHandler(void)
{
    if(EXTI->PR1 & (1 << 13)) {
        EXTI->PR1 = (1 << 13);
        button_pressed();
    }
}

void USART2_IRQHandler(void)
{
    if(USART2->ISR & (1 << 5)) {
        char command = (char)usart_receive_string(USART2);
        uart_character_received(command);
    }
}
