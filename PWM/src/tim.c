#include "tim.h"

int get_gptim_number(GeneralPurpose_Timer_t *TIMx)
{
    if(TIMx == TIM2) return 2;
    else if(TIMx == TIM3) return 3;
    else if(TIMx == TIM4) return 4;
    else if(TIMx == TIM5) return 5;
    else return 0;
}

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

void pwm_init(pwm_config_t *config)
{
    GeneralPurpose_Timer_t *TIMx = config->pwmTimer;
    timer_channel_t channel = config->pwmChannel;

    tim_activate(get_gptim_number(TIMx));
    return;
}
