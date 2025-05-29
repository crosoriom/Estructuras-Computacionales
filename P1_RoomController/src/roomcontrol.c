#include "roomcontrol.h"

int state_door = 0;
int open_tick = 0;
int lamp_state = 20;

void room_app_init()
{
    gpio_set_lowLevel(EXTERNAL_LED);
    usart_send_string(USART2, "Controlador de Sala v1.0\r\n");
    usart_send_string(USART2, "Desarrollador: Cristian Camilo Osorio Mesa\r\n");
    usart_send_string(USART2, "Estado inicial:\r\n");
    usart_send_string(USART2, "\t- Lámpara: 20%\r\n");
    usart_send_string(USART2, "\t- Puerta: cerrada\r\n");
    pwm_set_dutyCycle(TIM3, TIM_CHANNEL1, lamp_state);
}

void uart_received(char command)
{
    switch (command) {
        case '1':
            lamp_state = 100;
            pwm_set_dutyCycle(TIM3, TIM_CHANNEL1, lamp_state);
            usart_send_string(USART2, "Lámpara: brillo al 100%.\r\n");
            break;
        case '2':
            lamp_state = 70;
            pwm_set_dutyCycle(TIM3, TIM_CHANNEL1, lamp_state);
            usart_send_string(USART2, "Lámpara: brillo al 70%.\r\n");
            break;
        case '3':
            lamp_state = 50;
            pwm_set_dutyCycle(TIM3, TIM_CHANNEL1, lamp_state);
            usart_send_string(USART2, "Lámpara: brillo al 50%.\r\n");
            break;
        case '4':
            lamp_state = 20;
            pwm_set_dutyCycle(TIM3, TIM_CHANNEL1, lamp_state);
            usart_send_string(USART2, "Lámpara: brillo al 20%.\r\n");
            break;
        case '0':
            lamp_state = 0;
            pwm_set_dutyCycle(TIM3, TIM_CHANNEL1, lamp_state);
            usart_send_string(USART2, "Lámpara: brillo al 0%.\r\n");
            break;
        case 'O':
            gpio_set_highLevel(EXTERNAL_LED);
            open_tick = systick_getTick();
            state_door = 1;
            usart_send_string(USART2, "Puerta abierta remotamente.\r\n");
            break;
        case 'C':
            gpio_set_lowLevel(EXTERNAL_LED);
            state_door = 0;
            usart_send_string(USART2, "Puerta cerrada remotamente.\r\n");
            break;
        case 's':
            usart_send_string(USART2, "\t- Lámpara: {lamp_state}%\r\n");
            if(state_door == 0) usart_send_string(USART2, "\t- Puerta: cerrada\r\n");
            else usart_send_string(USART2, "\t- Puerta: abierta\r\n");
        default:
            usart_send_string(USART2, "Comando desconocido.\r\n");
            break;
    }
}


void button_pressed()
{
    usart_send_string(USART2, "Evento: Botón presionado - Abriendo puerta.\r\n");
    gpio_set_highLevel(EXTERNAL_LED);
    state_door = 1;
    open_tick = systick_getTick();
}

void control_door()
{
    if(state_door == 1 && (systick_getTick() - open_tick <= TIME_OPEN)) {
        state_door = 0;
        gpio_set_lowLevel(EXTERNAL_LED);
        usart_send_string(USART2, "Puerta cerrada de forma automática.");
    }
}
