#include "keyPad/inc/keypad.h"

void keypad_init(keypad_config_t *config)
{
    if(config == NULL)
        return;

    // Initialize the GPIO pins for each row as output
    for(int i = 0; i < NUM_ROWS; i++) {
        gpio_t *GPIOx = config->row_port[i];
        PINx pin = config->row_pin[i];

        configure_gpio_output(GPIOx, pin);
    }

    // Initialize the GPIOs pins and enable the interruptions for column pins
    for(int i = 0; i < NUM_COLS; i++) {
        gpio_t *GPIOx = config->col_port[i];
        PINx pin = config->col_pin[i];

        gpio_interrupt_enable(GPIOx, pin, FALLING_EDGE);
    }

    // Initialize the ring buffer
    ring_buffer_init(&keypad_rb, keypad_buffer_data, KEYPAD_BUFFER_SIZE);

    keypad_config = *config;
}

void keypad_scan_and_store(PINx triggered_pin)
{
    gpio_t *triggered_column_port = NULL;
    uint8_t triggered_column = 0xFF;

    // Press debounce
    delay(1);
    if(gpio_getLevel(triggered_column_port, triggered_pin, PULLUP) != 0) {
        // Pin is high again. False trigger
        exti_clear(triggered_pin);
        return;
    }

    // Match a column with the triggered_pin and disable EXTIs for each column for prevent re-triggering
    for(int column = 0; column < NUM_COLS; column++) {
        PINx pin = keypad_config.col_pin[column];
        if(keypad_config.col_pin[column] == triggered_pin) {
            triggered_column_port = keypad_config.col_port[column];
            triggered_column = column;
        }
        exti_disable(pin);
    }

    // invalid triggering. The pin is not in keypad config structure
    if(triggered_column == 0xFF) {
        return;
    }

    char pressed_key = 0;
    for(int row = 0; row < NUM_ROWS; row++) {
        gpio_t *GPIOrow = keypad_config.row_port[row];
        PINx row_pin = keypad_config.row_pin[row];

        gpio_set_lowLevel(GPIOrow, row_pin);
        delay(50);          //50 ticks delay for set time
        if(gpio_getLevel(triggered_column_port, triggered_pin, PULLUP) == 0) {
            pressed_key = keypad_map[row][triggered_column];
            break;          //key found
        }
    }

    // Set high again all the rows
    for(int row = 0; row < NUM_ROWS; row++) {
        gpio_t *GPIOrow = keypad_config.row_port[row];
        PINx row_pin = keypad_config.row_pin[row];
        gpio_set_highLevel(GPIOrow, row_pin);
    }

    if(pressed_key != 0)
        ring_buffer_write(&keypad_rb, pressed_key);

    // Unpress Debounce. Waits for button release for re-enabling the interruptions
    while(gpio_getLevel(triggered_column_port, triggered_pin, PULLUP))
        delay(1);
    delay(50);

    // Re-enable the interruptions
    for(int column; column < NUM_COLS; column ++) {
        PINx pin = keypad_config.col_pin[column];
        exti_enable(pin);
    }
    return;
}
