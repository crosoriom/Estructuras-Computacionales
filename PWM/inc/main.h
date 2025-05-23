#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>

// Parameters for PWM control
#define TIM2_CLK_FREQ   4000000
#define PWM_FREQ        1000
#define PWM_PERIOD      (TIM2_CLK_FREQ / PWM_FREQ)
#define DUTY_CYCLE      50
#define PWM_PIN         5

#endif
