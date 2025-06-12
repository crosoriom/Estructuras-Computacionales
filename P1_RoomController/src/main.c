#include "main.h"

usart_config_t usart_config = {
    .USART_PORT = USART2,
    .GPIO_PORT = GPIOA,
    .TX_PIN = PIN_2,
    .RX_PIN = PIN_3,
    .BAUDRATE = 115200,
    .STOP_BITS = ONE_STOP_BIT,
    .WORD_LENGHT = NINE_BITS_LENGHT,
    .PARITY = ODD_PARITY
};

pwm_config_t pwm_config = {
    .pwmTimer = TIM3,
    .pwmChannel = TIM_CHANNEL1,
    .period = PWM_PERIOD,
    .prescaler = PWM_PRESCALER
};

int main()
{
    systick_init(4000);
    gpio_interrupt_enable(BUTTON, FALLING_EDGE);
    configure_gpio_output(HEARTBEAT_LED);
    configure_gpio_output(EXTERNAL_LED);
    usart_init(&usart_config);
    usart_interrupt_enable(2);
    //pwm_init(&pwm_config);
    room_app_init();
    for(;;) {
        control_door();
        if(systick_getTick() - actualTick >= 500) {
            gpio_toggle_level(HEARTBEAT_LED);
            actualTick = systick_getTick();
        }
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
        uart_received(command);
    }
}
