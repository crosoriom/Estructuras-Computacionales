#include "tim.h"

int timer_selection(int GPIOx, int pin)
{
    if(GPIOx == 0) {
        if (pin == 0) return 0x1U; // TIM2_CH1, TIM2_ETR (AF1), TIM5_CH1 (AF2), TIM8_ETR (AF1) - Prioritizing TIM2/TIM5/TIM8 AF1/AF2
        if (pin == 1) return 0x1U; // TIM2_CH2 (AF1), TIM5_CH2 (AF2)
        if (pin == 2) return 0x1U; // TIM2_CH3 (AF1), TIM5_CH3 (AF2), TIM15_CH1 (AF14)
        if (pin == 3) return 0x1U; // TIM2_CH4 (AF1), TIM5_CH4 (AF2), TIM15_CH2 (AF14)
        if (pin == 5) return 0x1U; // TIM2_CH1, TIM2_ETR (AF1)
        if (pin == 6) return 0x2U; // TIM3_CH1 (AF2), TIM1_BKIN (AF1), TIM8_BKIN (AF2), TIM16_CH1 (AF14)
        if (pin == 7) return 0x2U; // TIM3_CH2 (AF2), TIM1_CH1N (AF1), TIM8_CH1N (AF2), TIM17_CH1 (AF14)
        if (pin == 8) return 0x1U; // TIM1_CH1 (AF1)
        if (pin == 9) return 0x1U; // TIM1_CH2 (AF1), TIM15_BKIN (AF14)
        if (pin == 10) return 0x1U; // TIM1_CH3 (AF1), TIM17_BKIN (AF14)
        if (pin == 11) return 0x1U; // TIM1_CH4 (AF1), TIM1_BKIN2 (AF1)
        if (pin == 15) return 0x1U; // TIM2_CH1, TIM2_ETR (AF1)
    } else if(GPIOx == 1) {
        if (pin == 0) return 0x2U; // TIM3_CH3 (AF2), TIM1_CH2N (AF1), TIM8_CH2N (AF2)
        if (pin == 1) return 0x2U; // TIM3_CH4 (AF2), TIM1_CH3N (AF1), TIM8_CH3N (AF2)
        if (pin == 3) return 0x1U; // TIM2_CH2 (AF1), TIM3_ETR (AF2)
        if (pin == 4) return 0x2U; // TIM3_CH1 (AF2), TIM17_BKIN (AF14)
        if (pin == 5) return 0x2U; // TIM3_CH2 (AF2), LPTIM1_IN1 (AF1), TIM16_BKIN (AF14)
        if (pin == 6) return 0x2U; // TIM4_CH1 (AF2), LPTIM1_ETR (AF1), TIM8_BKIN2 (AF2), TIM16_CH1N (AF14)
        if (pin == 7) return 0x2U; // TIM4_CH2 (AF2), LPTIM1_IN2 (AF1), TIM8_BKIN (AF2), TIM17_CH1N (AF14)
        if (pin == 8) return 0x2U; // TIM4_CH3 (AF2), TIM16_CH1 (AF14)
        if (pin == 9) return 0x2U; // TIM4_CH4 (AF2), TIM17_CH1 (AF14)
        if (pin == 10) return 0x1U; // TIM2_CH3 (AF1)
        if (pin == 11) return 0x1U; // TIM2_CH4 (AF1)
        if (pin == 14) return 0xEU; // TIM1_CH2N (AF1), TIM8_CH2N (AF3), TIM15_CH1 (AF14)
        if (pin == 15) return 0xEU; // TIM1_CH3N (AF1), TIM8_CH3N (AF3), TIM15_CH2 (AF14)
    } else if(GPIOx == 2) {
        if (pin == 6) return 0x2U; // TIM3_CH1 (AF2), TIM8_CH1 (AF2)
        if (pin == 7) return 0x2U; // TIM3_CH2 (AF2), TIM8_CH2 (AF2)
        if (pin == 8) return 0x2U; // TIM3_CH3 (AF2), TIM8_CH3 (AF2)
        if (pin == 9) return 0x2U; // TIM3_CH4 (AF2), TIM8_CH4 (AF2), TIM8_BKIN2 (AF2), TIM8_BKIN2_COMP1 (AF9)
    } else if(GPIOx == 3) {
        if (pin == 12) return 0x2U; // TIM4_CH1 (AF2)
        if (pin == 13) return 0x2U; // TIM4_CH2 (AF2)
        if (pin == 14) return 0x2U; // TIM4_CH3 (AF2)
        if (pin == 15) return 0x2U; // TIM4_CH4 (AF2)
    } else if(GPIOx == 4) {
        if (pin == 0) return 0x2U; // TIM4_ETR (AF2), TIM16_CH1 (AF14)
        if (pin == 1) return 0xEU; // TIM17_CH1 (AF14)
        if (pin == 3) return 0x2U; // TIM3_CH1 (AF2)
        if (pin == 4) return 0x2U; // TIM3_CH2 (AF2)
        if (pin == 5) return 0x2U; // TIM3_CH3 (AF2)
        if (pin == 6) return 0x2U; // TIM3_CH4 (AF2)
        if (pin == 9) return 0x1U; // TIM1_CH1 (AF1)
        if (pin == 11) return 0x1U; // TIM1_CH2 (AF1)
        if (pin == 13) return 0x1U; // TIM1_CH3 (AF1)
        if (pin == 14) return 0x1U; // TIM1_CH4 (AF1), TIM1_BKIN2 (AF2), TIM1_BKIN2_COMP2 (AF3)
    } else if(GPIOx == 5) {
        if (pin == 6) return 0x2U; // TIM5_ETR (AF1), TIM5_CH1 (AF2) - Prioritizing AF1/AF2
        if (pin == 7) return 0x2U; // TIM5_CH2 (AF2)
        if (pin == 8) return 0x2U; // TIM5_CH3 (AF2)
        if (pin == 9) return 0x2U; // TIM5_CH4 (AF2)
    } else if(GPIOx == 6) {
        if (pin == 10) return 0xEU; // LPTIM1_IN1 (AF1), TIM15_CH1 (AF14)
        if (pin == 11) return 0xEU; // LPTIM1_IN2 (AF1), TIM15_CH2 (AF14)
    }
    return 0xFFU;
}

gpio_af_map_t gpio_timmer_map(void *Timer, timer_channel_t channel)
{
    gpio_af_map_t result = {
        .GPIOx = 0
    };
    if(Timer == TIM2)
        switch (channel) {
        case TIM_CHANNEL1:
            result.GPIOx = GPIOA;
            result.pin = 5;
            result.afNumber = 1;
        case TIM_CHANNEL2:
            result.GPIOx = GPIOB;
            result.pin = 3;
            result.afNumber = 1;
        case TIM_CHANNEL3:
            result.GPIOx = GPIOB;
            result.pin = 10;
            result.afNumber = 1;
        case TIM_CHANNEL4:
            result.GPIOx = GPIOB;
            result.pin = 11;
            result.afNumber = 1;
        }
    if(Timer == TIM3)
        switch (channel) {
        case TIM_CHANNEL1:
            result.GPIOx = GPIOB;
            result.pin = 4;
            result.afNumber = 2;
        case TIM_CHANNEL2:
            result.GPIOx = GPIOA;
            result.pin = 7;
            result.afNumber = 2;
        case TIM_CHANNEL3:
            result.GPIOx = GPIOB;
            result.pin = 0;
            result.afNumber = 2;
        case TIM_CHANNEL4:
            result.GPIOx = GPIOB;
            result.pin = 1;
            result.afNumber = 2;
        }
    if(Timer == TIM4)
        switch (channel) {
        case TIM_CHANNEL1:
            result.GPIOx = GPIOB;
            result.pin = 6;
            result.afNumber = 2;
        case TIM_CHANNEL2:
            result.GPIOx = GPIOB;
            result.pin = 7;
            result.afNumber = 2;
        case TIM_CHANNEL3:
            result.GPIOx = GPIOB;
            result.pin = 8;
            result.afNumber = 2;
        case TIM_CHANNEL4:
            result.GPIOx = GPIOB;
            result.pin = 9;
            result.afNumber = 2;
        }
    if(Timer == TIM5)
        switch (channel) {
        case TIM_CHANNEL1:
            result.GPIOx = GPIOA;
            result.pin = 0;
            result.afNumber = 2;
        case TIM_CHANNEL2:
            result.GPIOx = GPIOA;
            result.pin = 1;
            result.afNumber = 2;
        case TIM_CHANNEL3:
            result.GPIOx = GPIOA;
            result.pin = 2;
            result.afNumber = 2;
        case TIM_CHANNEL4:
            result.GPIOx = GPIOA;
            result.pin = 3;
            result.afNumber = 2;
        }
    return result;
}

void timer_clock_enable(void *Timer)
{
    if(Timer == TIM1) tim_activate(1);
    else if(Timer == TIM2) tim_activate(2);
    else if(Timer == TIM3) tim_activate(3);
    else if(Timer == TIM4) tim_activate(4);
    else if(Timer == TIM5) tim_activate(5);
    else if(Timer == TIM6) tim_activate(6);
    else if(Timer == TIM7) tim_activate(7);
    else if(Timer == TIM8) tim_activate(8);
    else if(Timer == TIM15) tim_activate(15);
    else if(Timer == TIM16) tim_activate(16);
    else if(Timer == TIM17) tim_activate(17);
    else return;
}

void pwm_init(pwm_config_t *config)
{
    GeneralPurpose_Timer_t *TIMx = config->pwmTimer;
    timer_clock_enable(TIMx);
    gpio_af_map_t gpio = gpio_timmer_map(TIMx, config->pwmChannel);
    if(gpio.GPIOx == 0)
        return;
    configure_gpio_pwm(gpio.GPIOx, gpio.pin, gpio.afNumber);

    TIMx->PSC = config->prescaler - 1;
    TIMx->ARR = config->period - 1;

    volatile uint32_t *ccmr_reg = (config->pwmChannel < TIM_CHANNEL3) ? &TIMx->CCMR1 : &TIMx->CCMR2;
    uint8_t shift = (config->pwmChannel % 2) * 8;

    *ccmr_reg |= (0x6U << (4 + shift));
    TIMx->CCER |= (0x1U << (4 * config->pwmChannel));

    TIMx->CR1 |= (0x1U << 0);
}

void pwm_set_dutyCycle(GeneralPurpose_Timer_t *TIMx, timer_channel_t channel, int dutyCycle)
{
    if(dutyCycle < 0 || dutyCycle > 100)
        return;

    uint32_t period = TIMx->ARR;
    uint32_t crrValue = ((period + 1) * dutyCycle) / 100;
    if(channel == TIM_CHANNEL1) TIMx->CCR1 |= crrValue;
    else if(channel == TIM_CHANNEL2) TIMx->CCR2 |= crrValue;
    else if(channel == TIM_CHANNEL3) TIMx->CCR3 |= crrValue;
    else if(channel == TIM_CHANNEL4) TIMx->CCR4 |= crrValue;
    else return;
}
