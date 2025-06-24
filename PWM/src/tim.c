#include "tim.h"

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
            break;
        case TIM_CHANNEL2:
            result.GPIOx = GPIOB;
            result.pin = 3;
            result.afNumber = 1;
            break;
        case TIM_CHANNEL3:
            result.GPIOx = GPIOB;
            result.pin = 10;
            result.afNumber = 1;
            break;
        case TIM_CHANNEL4:
            result.GPIOx = GPIOB;
            result.pin = 11;
            result.afNumber = 1;
            break;
        }
    if(Timer == TIM3)
        switch (channel) {
        case TIM_CHANNEL1:
            result.GPIOx = GPIOA;
            result.pin = 6;
            result.afNumber = 2;
            break;
        case TIM_CHANNEL2:
            result.GPIOx = GPIOA;
            result.pin = 7;
            result.afNumber = 2;
            break;
        case TIM_CHANNEL3:
            result.GPIOx = GPIOB;
            result.pin = 0;
            result.afNumber = 2;
            break;
        case TIM_CHANNEL4:
            result.GPIOx = GPIOB;
            result.pin = 1;
            result.afNumber = 2;
            break;
        }
    if(Timer == TIM4)
        switch (channel) {
        case TIM_CHANNEL1:
            result.GPIOx = GPIOB;
            result.pin = 6;
            result.afNumber = 2;
            break;
        case TIM_CHANNEL2:
            result.GPIOx = GPIOB;
            result.pin = 7;
            result.afNumber = 2;
            break;
        case TIM_CHANNEL3:
            result.GPIOx = GPIOB;
            result.pin = 8;
            result.afNumber = 2;
            break;
        case TIM_CHANNEL4:
            result.GPIOx = GPIOB;
            result.pin = 9;
            result.afNumber = 2;
            break;
        }
    if(Timer == TIM5)
        switch (channel) {
        case TIM_CHANNEL1:
            result.GPIOx = GPIOA;
            result.pin = 0;
            result.afNumber = 2;
            break;
        case TIM_CHANNEL2:
            result.GPIOx = GPIOA;
            result.pin = 1;
            result.afNumber = 2;
            break;
        case TIM_CHANNEL3:
            result.GPIOx = GPIOA;
            result.pin = 2;
            result.afNumber = 2;
            break;
        case TIM_CHANNEL4:
            result.GPIOx = GPIOA;
            result.pin = 3;
            result.afNumber = 2;
            break;
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

    // 1. Enable Timer Clock
    timer_clock_enable(TIMx);

    // 2. Configure GPIO pin for Alternate Function
    gpio_af_map_t gpio = gpio_timmer_map(TIMx, config->pwmChannel);
    if(gpio.GPIOx == 0)
        return; // Exit if no valid pin mapping found
    configure_gpio_pwm(gpio.GPIOx, gpio.pin, gpio.afNumber);

    // 3. Configure Timer Base: Prescaler and Period
    TIMx->PSC = config->prescaler - 1;
    TIMx->ARR = config->period - 1;

    // 4. Configure PWM Channel
    volatile uint32_t *ccmr_reg = (config->pwmChannel < TIM_CHANNEL3) ? &TIMx->CCMR1 : &TIMx->CCMR2;
    uint8_t shift = (config->pwmChannel % 2) * 8;   // 0 for ch1/3, 8 for ch2/4

    // Set PWM Mode 1 (OCxM bits = 110) and enable Preload (OCxPE bit = 1)
    // Preload enable is crucial for glitch-free duty cycle updates.
    *ccmr_reg &= ~((0x7U << (4 + shift)) | (0x1U << (3 + shift))); // Clear previous settings
    *ccmr_reg |= (0x6U << (4 + shift));   // OCxM = 110 for PWM Mode 1
    *ccmr_reg |= (0x1U << (3 + shift));   // OCxPE = 1 (Output Compare Preload Enable)

    // 5. Enable the specific PWM Channel Output
    TIMx->CCER |= (0x1U << (4 * config->pwmChannel));

    // 6. Main Output Enable and Counter Start
    TIMx->CR1 |= (0x1U << 7);   // ARPE: Auto-reload preload enable
    TIMx->EGR |= (0x1U << 0);   // UG: Generate and update event to load PSC and ARR
    TIMx->CR1 |= (0x1U << 0);   // CEN: Counter enable. Starts the timer.
}

void pwm_set_dutyCycle(GeneralPurpose_Timer_t *TIMx, timer_channel_t channel, int dutyCycle)
{
    if(dutyCycle < 0 || dutyCycle > 100)
        return;

    // The period is the value in ARR + 1
    uint32_t period = TIMx->ARR;
    uint32_t crrValue = ((period + 1) * dutyCycle) / 100;

    switch (channel) {
        case TIM_CHANNEL1: TIMx->CCR1 = crrValue; break;
        case TIM_CHANNEL2: TIMx->CCR2 = crrValue; break;
        case TIM_CHANNEL3: TIMx->CCR3 = crrValue; break;
        case TIM_CHANNEL4: TIMx->CCR4 = crrValue; break;
        default: return;
    }
}
