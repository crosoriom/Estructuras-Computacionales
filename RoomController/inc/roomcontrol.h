#ifndef ROOMCONTROL_H
#define ROOMCONTROL_H

#include <stdint.h>
#include "systick.h"
#include "uart.h"
#include "tim.h"

// Parameters for PWM control
#define TIM_CLK_FREQ    4000000
#define PWM_PRESCALER   100
#define PWM_FREQ        1000
#define PWM_PERIOD      (TIM_CLK_FREQ / PWM_FREQ)
#define DUTY_CYCLE      50
#define PWM_PIN         5

// Hardware PINs
#define HEARTBEAT_LED GPIOA, PIN_5
#define BUTTON GPIOC, PIN_13
#define EXTERNAL_LED GPIOA, PIN_7

#define TIME_OPEN 10000

void button_pressed(void);
void uart_received(char command);
void room_app_init(void);
void control_door(void);

#endif
