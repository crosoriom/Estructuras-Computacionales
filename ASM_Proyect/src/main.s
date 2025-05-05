// Memory section
    .section .bss
    .global timer_ms
timer_ms:
    .word 0
button_press_time:
    .word 0
led_timer_active:
    .byte 0

// Program section

    .section .text
    .syntax unified
    .thumb
    .global main
    .global init_led
    .global init_button
    .global init_systick
    .global read_gpio
    .global Systick_Handler

// Directions and constants for GPIO Registers

.equ RCC,           0x40021000
.equ RCC_AHB2ENR,   RCC + 0x4C
.equ GPIOA,         0x48000000
.equ GPIOA_MODER,  GPIOA + 0x00
.equ GPIOA_ODR,     GPIOA + 0x14
.equ GPIOC,         0x48000800
.equ GPIOC_MODER,  GPIOC + 0x00
.equ GPIOC_IDR,     GPIOC + 0x10
.equ LD2_PIN,       5
.equ LD2_TIME,      3000
.equ BTN_PIN,       13

// Directions and constants for SysTick Registers

.equ STK_CTRL,      0xE000E010
.equ STK_LOAD,      0xE000E014
.equ STK_VAL,       0xE000E018
.equ STK_CALIB,     0xE000E01C
.equ HSI_FREC,      4000

// Main program
main:
    bl init_led
    bl init_button
    bl init_systick

    movw r4, #:lower16:timer_ms
    movt r4, #:upper16:timer_ms

    movw r5, #:lower16:button_press_time
    movt r5, #:upper16:button_press_time

    movw r6, #:lower16:led_timer_active
    movt r6, #:upper16:led_timer_active
loop:
    bl read_gpio
    cmp r0, #1
    bne check_timer                 // If button is not pressed, check the timer status

    movw r0, #:lower16:GPIOA_ODR
    movt r0, #:upper16:GPIOA_ODR
    ldr r1, [r0]
    orr r1, r1, #(1 << LD2_PIN)     // Turn on the LED
    str r1, [r0]

    ldr r2, [r4]
    str r2, [r5]                    // Load the actual time in the button time variable
    mov r1, #1
    strb r1, [r6]                   // Turn on the led timer

    b wait_interrupt
check_timer:
    ldrb r1, [r6]
    cmp r1, #1
    bne wait_interrupt              // If led timer is not active, skip the checking

    ldr r2, [r4]
    ldr r3, [r5]

    movw r7, #:lower16:LD2_TIME
    movt r7, #:upper16:LD2_TIME
    adds r3, r3, r7

    cmp r2, r3
    blt wait_interrupt              // If actual time is less than the time for LD2 turn off, branch to wait interruption

    movw r0, #:lower16:GPIOA_ODR
    movt r0, #:upper16:GPIOA_ODR
    ldr r1, [r0]
    bic r1, r1, #(1 << LD2_PIN)     // Turn off the LED
    str r1, [r0]

    mov r1, #0
    strb r1, [r6]                   // Turn off the led timer
wait_interrupt:
    b loop

// LD2 configuration
init_led:
    movw r0, #:lower16:RCC_AHB2ENR
    movt r0, #:upper16:RCC_AHB2ENR
    ldr r1, [r0]
    orr r1, r1, #(1 << 0)   // Set the bit for the GPIOA clock
    str r1, [r0]
    
    movw r0, #:lower16:GPIOA_MODER
    movt r0, #:upper16:GPIOA_MODER
    ldr r1, [r0]
    bic r1, r1, #(0b11 << (LD2_PIN * 2))
    orr r1, r1, #(0b01 << (LD2_PIN * 2))    // Led pin configured as output (01 value)
    str r1, [r0]
    
    bx lr

// BUTTON configuration
init_button:
    movw r0, #:lower16:RCC_AHB2ENR
    movt r0, #:upper16:RCC_AHB2ENR
    ldr r1, [r0]
    orr r1, r1, #(1 << 2)   // Set the bit for the GPIOC clock
    str r1, [r0]

    movw r0, #:lower16:GPIOC_MODER
    movt r0, #:upper16:GPIOC_MODER
    ldr r1, [r0]
    bic r1, r1, #(0b11 << (BTN_PIN * 2))    // Button pin configured as input (00 value)
    str r1, [r0]
    
    bx lr

// SysTick configuration
init_systick:
    movw r0, #:lower16:STK_LOAD
    movt r0, #:upper16:STK_LOAD
    movw r1, #:lower16:HSI_FREC
    movt r1, #:upper16:HSI_FREC
    subs r1, r1, #1         // Load the system timer a value of HSI_FREC - 1. This is because starts count in 0.
    str r1, [r0]
    
    movw r0, #:lower16:STK_CTRL
    movt r0, #:upper16:STK_CTRL
    movs r1, #(1 << 0)|(1 << 1)|(1 << 2)    // Enable the SysTick counter, the counter exception and configure the clock fot the counter
    str r1, [r0]
    
    bx lr

// Button checker
read_gpio:
    movw r0, #:lower16:GPIOC_IDR
    movt r0, #:upper16:GPIOC_IDR
    ldr r1, [r0]
    tst r1, #(1 << BTN_PIN)
    ite eq
    moveq r0, #1
    movne r0, #0

    bx lr

// SysTick interruption handler
    .thumb_func
Systick_Handler:
    movw r0, #:lower16:timer_ms
    movt r0, #:upper16:timer_ms
    ldr r1, [r0]
    adds r1, r1, #1         // Increment the time counter 1ms
    str r1, [r0]
    
    bx lr
