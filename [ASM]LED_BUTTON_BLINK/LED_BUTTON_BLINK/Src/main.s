/*
 * main.s
 *
 *  Created on: Nov 16, 2024
 *      Authors: Cristian Camilo Osorio Mesa, Maicol Stiven Mora Caicedo
 * 
 */

.syntax unified
.global main

.section .bss
ms_counter:  .space 4      @ Reservar una palabra para el contador de milisegundos en RAM

.section .text
.equ STK_BASE, 0xE000E010      @ Dirección base del bloque de sistema (PM0214, Pag 246)
.equ STK_CTRL, STK_BASE + 0x00 @ Dirección del Registro de Control de SysTick (PM0214, Pag 246)
.equ STK_LOAD, STK_BASE + 0x04 @ Dirección del Registro de Valor de Recarga de SysTick

.equ RCC_BASE, 0x40021000       @ Dirección base del RCC (RM0351, pag 79)
.equ RCC_AHB2ENR, RCC_BASE + 0x4C @ AHB2ENR para habilitar el GPIOA y GPIOC (RM0351, pag 251)

.equ GPIOA_BASE, 0x48000000         @ Dirección base de GPIOA (RM0351, pag 78)
.equ GPIOA_MODER, GPIOA_BASE + 0x00 @ Modo del puerto GPIOA (RM0351, pag 304)
.equ GPIOA_ODR, GPIOA_BASE + 0x14   @ Registro de salida del puerto GPIOA
.equ LED_PIN, 5                     @ Pin 5 del puerto GPIOA para el LED

.equ GPIOC_BASE, 0x48000800         @ Dirección base de GPIOC (RM0351, pag 78)
.equ GPIOC_MODER, GPIOC_BASE + 0x00 @ Modo del puerto GPIOC (RM0351, pag 304)
.equ GPIOC_IDR, GPIOC_BASE + 0x10   @ Registro de entrada del puerto GPIOC
.equ BUTTON_PIN, 13                 @ Pin 13 del puerto GPIOC para el botón azul

main:
    bl init_ms_counter              @ Inicializar el contador de milisegundos
    bl configure_systick_and_start  @ Configurar y arrancar SysTick
    bl configure_gpio               @ Habilitar clock y configurar el GPIO para el LED
    bl configure_button             @ Configurar el GPIO para el botón azul

loop:
    ldr r0, [r6]            @ Leer el Registro de Control de SysTick (r6 contiene la dirección de STK_CTRL)
    tst r0, #0x00010000     @ Probar el bit COUNTFLAG
    beq loop                @ Si COUNTFLAG no está establecido, seguir esperando

    ldr r0, =GPIOC_IDR      @ Leer el estado del botón azul
    ldr r1, [r0]            @ r1 contiene el valor del registro de entrada
    tst r1, #(1 << BUTTON_PIN) @ Probar el bit del botón (1 si no está presionado, 0 si está presionado)
    beq loop                @ Si el botón está presionado, omitir el parpadeo del LED

    bl increase_ms_counter_and_blink  @ Incrementar el contador de milisegundos y parpadear
    b loop                  @ Permanecer en este bucle indefinidamente

@ Configurar GPIO para el botón azul
configure_button:
    ldr r0, =RCC_AHB2ENR    @ Habilitar el reloj para GPIOC
    ldr r1, [r0]
    orr r1, r1, #(1 << 2)   @ Bit 2 habilita el clock para GPIOC
    str r1, [r0]

    ldr r0, =GPIOC_MODER    @ Configurar el pin 13 como entrada
    ldr r1, [r0]
    bic r1, r1, #(0x3 << (BUTTON_PIN * 2)) @ Limpiar los bits de configuración para el pin 13
    str r1, [r0]
    bx lr                   @ Retornar de la función

@ Inicializar el contador de milisegundos
init_ms_counter:
    ldr r5, =ms_counter
    mov r0, #0
    str r0, [r5]
    bx lr

@ Configurar SysTick
configure_systick_and_start:
    ldr r6, =STK_CTRL
    ldr r1, =STK_LOAD

    mov r0, #0x4
    str r0, [r6]

    ldr r0, =#3999999
    str r0, [r1]

    mov r0, #0x5
    str r0, [r6]
    bx lr

@ Configurar GPIO para el LED
configure_gpio:
    ldr r7, =GPIOA_ODR
    ldr r0, =RCC_AHB2ENR
    ldr r1, [r0]
    orr r1, r1, #(1 << 0)
    str r1, [r0]

    ldr r0, =GPIOA_MODER
    ldr r1, [r0]
    bic r1, r1, #(0x3 << (LED_PIN * 2))
    orr r1, r1, #(0x1 << (LED_PIN * 2))
    str r1, [r0]
    bx lr

@ Incrementar contador y parpadear LED
increase_ms_counter_and_blink:
    ldr r0, [r5]
    add r0, r0, #1
    str r0, [r5]

    cmp r0, #1
    blt end_blink

    ldr r1, [r7]
    eor r1, r1, #(1 << LED_PIN)
    str r1, [r7]

    mov r0, #0
    str r0, [r5]

end_blink:
    bx lr

.end