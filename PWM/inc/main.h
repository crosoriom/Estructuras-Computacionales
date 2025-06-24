#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>
#include "systick.h"
#include "gpio.h"
#include "exti.h"
#include "uart.h"
#include "tim.h"

uint32_t actualTick = 0;
uint8_t button_state = 0;
uint32_t buttonTick = 0;
uint8_t door_state = 0;
uint32_t doorTick = 0;

// Parameters for PWM control
#define TIM_CLK_FREQ    4000000
#define PWM_PRESCALER   100
#define PWM_FREQ        1000
#define PWM_PERIOD      (TIM_CLK_FREQ / PWM_FREQ)
#define DUTY_CYCLE      50
#define PWM_PIN         5

void heartbeat(void)
{
    if (systick_getTick() - actualTick >= 500) {
        gpio_toggle_level(GPIOA, PIN_5);
        actualTick = systick_getTick();
    }
    return;
}

void control_external_led(void)
{
    if (button_state == 1) {
        if (systick_getTick() - buttonTick >= 3000) {
            usart_send_string(USART2, "Puerta cerrada de forma automática.\n\r");
            gpio_set_lowLevel(GPIOA, PIN_4);
            button_state = 0;
        }
    }
    return;
}

void button_pressed(void)
{
    usart_send_string(USART2, "Botón presionado. Puerta abierta.\n\r");
    button_state = 1;
    buttonTick = systick_getTick();
    return;
}

void uart_character_received(char cmd)
{
    switch (cmd) {
        case '1':
            pwm_set_dutyCycle(TIM3, TIM_CHANNEL1, 100);
            usart_send_string(USART2, "Lámpara: brillo al 100%.\r\n");
            break;

        case '2':
            pwm_set_dutyCycle(TIM3, TIM_CHANNEL1, 70);
            usart_send_string(USART2, "Lámpara: brillo al 70%.\r\n");
            break;

        case '3':
            pwm_set_dutyCycle(TIM3, TIM_CHANNEL1, 50);
            usart_send_string(USART2, "Lámpara: brillo al 50%.\r\n");
            break;

        case '4':
            pwm_set_dutyCycle(TIM3, TIM_CHANNEL1, 20);
            usart_send_string(USART2, "Lámpara: brillo al 20%.\r\n");
            break;

        case '0':
            pwm_set_dutyCycle(TIM3, TIM_CHANNEL1, 0);
            usart_send_string(USART2, "Lámpara apagada.\r\n");
            break;

        case 'o':
        case 'O':
            gpio_set_highLevel(GPIOA, PIN_4);
            door_state = 1;
            doorTick = systick_getTick();
            usart_send_string(USART2, "Puerta abierta de forma remota.\r\n");
            break;

        case 'c':
        case 'C':
            gpio_set_lowLevel(GPIOA, PIN_4);
            door_state = 0;
            usart_send_string(USART2, "Puerta cerrada de forma remota.\r\n");
            break;

        default:
            usart_send_string(USART2, "Comando desconocido.\r\n");
            break;
    }
    return;
}

#endif
