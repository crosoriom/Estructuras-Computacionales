#ifndef KEYPAD_H
#define KEYPAD_H

#include "ringBuffer/inc/ringBuffer.h"
#include "gpio.h"
#include "exti.h"
#include "systick.h"

#define NUM_ROWS 4
#define NUM_COLS 4
#define KEYPAD_BUFFER_SIZE (NUM_ROWS * NUM_COLS)

static const char keypad_map[NUM_ROWS][NUM_COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

static ring_buffer_t keypad_rb;
static uint8_t keypad_buffer_data[KEYPAD_BUFFER_SIZE];

typedef struct {
    gpio_t *row_port[NUM_ROWS];
    uint8_t row_pin[NUM_ROWS];
    gpio_t *col_port[NUM_COLS];
    uint8_t col_pin[NUM_COLS];
} keypad_config_t;

static keypad_config_t keypad_config;

/**
  * @brief  Initializes the GPIO pins and EXTI for the 4x4 keypad.
  *
  * @param  config: Pointer to the keypad_config_t structure containing GPIO details.
  *
  * @note   This function enables necessary peripheral clocks, configures GPIOs,
  * and sets up EXTI interrupts for column pins.
  */
void keypad_init(keypad_config_t *keypad_config);

/**
  * @brief  Scans the keypad to detect which button was pressed and stores it in the ring buffer.
  *
  * @param  triggered_exti_line: The EXTI line number that triggered the interrupt.
  *
  * @note   This function should be called from your GPIO EXTI interrupt handler.
  * It implements basic debouncing.
  */
void keypad_scan_and_store(PINx triggered_column);

#endif
