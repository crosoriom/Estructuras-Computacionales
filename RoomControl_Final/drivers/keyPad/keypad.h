#ifndef KEYPAD_H
#define KEYPAD_H

#include "ringBuffer/ringBuffer.h"
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

// --- Debouncing State Machine ---
typedef enum {
    KEY_STATE_IDLE,      // Waiting for a key press
    KEY_STATE_DEBOUNCE,  // A key has been pressed, waiting for it to stabilize
    KEY_STATE_PRESSED    // Key is confirmed as pressed, waiting for release
} keypad_state_t;

static keypad_state_t keypad_state = KEY_STATE_IDLE;
static uint8_t debounce_counter = 0;
#define DEBOUNCE_TIME 20 // Debounce for 20 scan calls (e.g., 20ms if scanned every 1ms)

/**
 * @brief Initializes the keypad GPIOs.
 *
 * Configures the row pins as outputs (initially high) and the column pins
 * as inputs with pull-up resistors.
 *
 * @param[in] config Pointer to the keypad_config_t struct with pin definitions.
 */
void keypad_init(const keypad_config_t *config);

/**
 * @brief Periodically scans the keypad for key presses.
 *
 * This function should be called at a regular interval (e.g., every 5-10ms)
 * from a timer interrupt (like SysTick) or the main loop. It handles the
 * entire scanning and debouncing logic internally.
 */
void keypad_scan(void);

/**
 * @brief Reads a single character from the keypad buffer.
 *
 * @param[out] key Pointer to a variable to store the read key.
 * @return true if a key was read, false if the buffer is empty.
 */
bool keypad_read_key(char *key);

#endif
