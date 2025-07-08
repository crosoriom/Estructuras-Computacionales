#include "main.h"

int actualTick = 0;

const keypad_config_t extern_keypad_conf = {
    .row_port = {GPIOA, GPIOB, GPIOB, GPIOB},
    .row_pin = {10, 3, 5, 4},
    .col_port = {GPIOB, GPIOA, GPIOA, GPIOC},
    .col_pin = {10, 8, 9, 7}
};

usart_config_t usart2_conf = {
    .USART_PORT = USART2,
    .BAUDRATE = 115200,
    .WORD_LENGHT = NINE_BITS_LENGHT,
    .STOP_BITS = ONE_STOP_BIT,
    .PARITY = ODD_PARITY
};

gpio_config_t heartbeat_conf = {
    .port = GPIOA,
    .pin = 5,
    .mode = GPIO_MODE_OUTPUT
};

int main(void) {
    // 1. Initialize system clock, systick, etc.
    rcc_set_system_clock(SYSCLK_SRC_MSI);
    systick_init(4000); // 1ms tick for an 80MHz clock

    gpio_init(&heartbeat_conf);
    exti_gpio_init(GPIOC, 13, FALLING_EDGE);

    //keypad_init(&extern_keypad_conf);
    usart_init(&usart2_conf, 4000000);
    
    usart_send_string(USART2, "MC Started!\r\n");
    while(1) {
        //char pressed_key;

        if(systick_getTick() - actualTick >= 500) {
            actualTick = systick_getTick();
            gpio_toggle_pin(GPIOA, 5);
            //usart_send_string(USART2, "Blink\n\r");
        }
        /*
        //keypad_scan();
        // Check the buffer for a new key
        if (keypad_read_key(&pressed_key)) {
            // A key was read, print it to the serial port
            usart_send_string(USART2, "Key pressed: ");
            usart_send_char(USART2, pressed_key);
            usart_send_string(USART2, "\r\n");
        }
        // The display is static now. You could add animations or
        // update the text based on sensor readings here.
        */
    }
    return 0;
}

void EXTI15_10_IRQHandler(void)
{
    if(EXTI->PR1 & (1 << 13)) {
        EXTI->PR1 = (1 << 13);
        usart_send_string(USART2, "Button pressed.\r\n");
    }
}
