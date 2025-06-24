#ifndef TIM_H
#define TIM_H

#include <stdint.h>
#include "gpio.h"
#include "rcc.h"

//--- Timer Peripheral Base Addresses ---//
// Base address for advanced control TIMs
#define TIM1 ((Advanced_Timer_t *)0x40012C00UL)
#define TIM8 ((Advanced_Timer_t *)0x40013400UL)

// Base address for general purpose TIMs
#define TIM2 ((GeneralPurpose_Timer_t *)0x40000000UL)
#define TIM3 ((GeneralPurpose_Timer_t *)0x40000400UL)
#define TIM4 ((GeneralPurpose_Timer_t *)0x40000800UL)
#define TIM5 ((GeneralPurpose_Timer_t *)0x40000C00UL)

// Base address for basic TIMs
#define TIM6 ((Basic_Timer_t *)0x40001000UL)
#define TIM7 ((Basic_Timer_t *)0x40001400UL)

// Base address for general purpose TIMs 15, 16 and 17
#define TIM15 ((GeneralPurpose_Timer_15_t *)0x40014000UL)
#define TIM16 ((GeneralPurpose_Timer_16_17_t *)0x40014400UL)
#define TIM17 ((GeneralPurpose_Timer_16_17_t *)0x40014800UL)

//--- Timer Register Structures ---//

/**
 * @brief Structure for Advanced-Control Timers (TIM1/TIM8) register map.
 */
typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SMCR;
    volatile uint32_t DIER;
    volatile uint32_t SR;
    volatile uint32_t EGR;
    volatile uint32_t CCMR1;
    volatile uint32_t CCMR2;
    volatile uint32_t CCER;
    volatile uint32_t CNT;
    volatile uint32_t PSC;
    volatile uint32_t ARR;
    volatile uint32_t RCR;
    volatile uint32_t CCR1;
    volatile uint32_t CCR2;
    volatile uint32_t CCR3;
    volatile uint32_t CCR4;
    volatile uint32_t BDTR;
    volatile uint32_t DCR;
    volatile uint32_t DMAR;
    volatile uint32_t OR1;
    volatile uint32_t CCMR3;
    volatile uint32_t CCR5;
    volatile uint32_t CCR6;
    volatile uint32_t OR2;
    volatile uint32_t OR3;
}Advanced_Timer_t;

/**
 * @brief Structure for General Purpose Timers (TIM2/TIM3/TIM4/TIM5) register map.
 */
typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SMCR;
    volatile uint32_t DIER;
    volatile uint32_t SR;
    volatile uint32_t EGR;
    volatile uint32_t CCMR1;
    volatile uint32_t CCMR2;
    volatile uint32_t CCER;
    volatile uint32_t CNT;
    volatile uint32_t PSC;
    volatile uint32_t ARR;
    volatile uint32_t RESERVED0;
    volatile uint32_t CCR1;
    volatile uint32_t CCR2;
    volatile uint32_t CCR3;
    volatile uint32_t CCR4;
    volatile uint32_t RESERVED1;
    volatile uint32_t DCR;
    volatile uint32_t DMAR;
    volatile uint32_t OR1[4];
    volatile uint32_t OR2;
} GeneralPurpose_Timer_t;

// Structure for General Purpose Timer (TIM15)
typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SMCR;
    volatile uint32_t DIER;
    volatile uint32_t SR;
    volatile uint32_t EGR;
    volatile uint32_t CCMR1;
    volatile uint32_t RESERVED0;
    volatile uint32_t CCER;
    volatile uint32_t CNT;
    volatile uint32_t PSC;
    volatile uint32_t ARR;
    volatile uint32_t RCR;
    volatile uint32_t CCR1;
    volatile uint32_t CCR2;
    volatile uint32_t RESERVED1[3];
    volatile uint32_t BDTR;
    volatile uint32_t DCR;
    volatile uint32_t DMAR;
    volatile uint32_t OR1[4];
    volatile uint32_t OR2;
} GeneralPurpose_Timer_15_t;

// Structure for General Purpose Timers (TIM16/TIM17)
typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t RESERVED0;
    volatile uint32_t DIER;
    volatile uint32_t SR;
    volatile uint32_t EGR;
    volatile uint32_t CCMR1;
    volatile uint32_t RESERVED1;
    volatile uint32_t CCER;
    volatile uint32_t CNT;
    volatile uint32_t PSC;
    volatile uint32_t ARR;
    volatile uint32_t RCR;
    volatile uint32_t CCR1;
    volatile uint32_t RESERVED2[4];
    volatile uint32_t BDTR;
    volatile uint32_t DCR;
    volatile uint32_t DMAR;
    volatile uint32_t OR1[4];
    volatile uint32_t OR2;
} GeneralPurpose_Timer_16_17_t;

typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t RESERVED0;
    volatile uint32_t DIER;
    volatile uint32_t SR;
    volatile uint32_t EGR;
    volatile uint32_t RESERVED1[3];
    volatile uint32_t CNT;
    volatile uint32_t PSC;
    volatile uint32_t ARR;
} Basic_Timer_t;

/**
 * @brief Enumeration for the four main timer channels.
 */
typedef enum {
    TIM_CHANNEL1 = 0,
    TIM_CHANNEL2,
    TIM_CHANNEL3,
    TIM_CHANNEL4,
} timer_channel_t;

/**
 * @brief Configuration structure for PWM setup.
 */
typedef struct {
    GeneralPurpose_Timer_t *pwmTimer;
    timer_channel_t pwmChannel;
    int prescaler;
    int period;
} pwm_config_t;

/**
 * @brief Maps a timer and channel to a specific GPIO pin and alternate function.
 *
 * @param Timer Pointer to the timer peripheral.
 * @param channel The timer channel.
 *
 * @note This implementation provides one possible default pin for each timer channel.
 *       The STM32L476RG often has multiple pin options for each.
 * @return gpio_af_map_t A structure with GPIO port, pin, and AF number.
 *                       Returns a struct with GPIOx=0 if no mapping is found.
 */
gpio_af_map_t gpio_timmer_map(void *Timer, timer_channel_t channel);

/**
 * @brief Enables the clock for a specific timer peripheral.
 * @param Timer Pointer to the timer peripheral.
 */
void timer_clocl_enable(void *Timer);

/**
 * @brief Initializes a timer channel for PWM output.
 * @param config Pointer to a pwm_config_t structure containing the configuration.
 */
void pwm_init(pwm_config_t *configStructure);

/**
 * @brief Sets the duty cycle for a PWM channel.
 * @param TIMx Pointer to the timer peripheral (e.g., TIM2).
 * @param channel The timer channel to modify.
 * @param dutyCycle The desired duty cycle in percent (0 to 100).
 */
void pwm_set_dutyCycle(GeneralPurpose_Timer_t *TIMx, timer_channel_t channel, int dutyCycle);

#endif
