#include "keyPad/keypad.h"

static keypad_config_t keypad_config;
static bool keypad_initialized = false;

void keypad_init(const keypad_config_t *config)
{
    if(config == NULL)
        return;

    // Store the configuration
    keypad_config = *config;

    // Configure Row pins as Push-Pull Outputs
    for (int i = 0; i < NUM_ROWS; i++) {
        gpio_config_t row_config = {
            .port = keypad_config.row_port[i],
            .pin = keypad_config.row_pin[i],
            .mode = GPIO_MODE_OUTPUT,
            .otype = GPIO_OTYPE_PUSHPULL,
            .ospeed = GPIO_OSPEED_LOW,
            .pupd = GPIO_PUPD_NONE
        };
        gpio_init(&row_config);
        // Initially set all rows high
        gpio_set_pin(keypad_config.row_port[i], keypad_config.row_pin[i]);
    }

    // Configure Column pins as Inputs with Pull-up resistors
    for (int i = 0; i < NUM_COLS; i++) {
        gpio_config_t col_config = {
            .port = keypad_config.col_port[i],
            .pin = keypad_config.col_pin[i],
            .mode = GPIO_MODE_INPUT,
            .pupd = GPIO_PUPD_PULLUP
        };
        gpio_init(&col_config);
    }

    // Initialize the ring buffer
    ring_buffer_init(&keypad_rb, keypad_buffer_data, KEYPAD_BUFFER_SIZE);

    keypad_config = *config;
    keypad_initialized = true;
}

static char keypad_get_key(void)
{
    // Sequentially pull each row low and check all columns
    for (int r = 0; r < NUM_ROWS; r++) {
        // Pull the current row LOW
        gpio_reset_pin(keypad_config.row_port[r], keypad_config.row_pin[r]);

        // Check each column in that row
        for (int c = 0; c < NUM_COLS; c++) {
            // If a column reads LOW, a button is pressed at (r, c)
            if (gpio_read_pin(keypad_config.col_port[c], keypad_config.col_pin[c]) == 0) {
                // Key found. Set row high again and return the key.
                gpio_set_pin(keypad_config.row_port[r], keypad_config.row_pin[r]);
                return keypad_map[r][c];
            }
        }

        // Set the current row back to HIGH before checking the next row
        gpio_set_pin(keypad_config.row_port[r], keypad_config.row_pin[r]);
    }

    return '\0'; // No key pressed
}

void keypad_scan(void)
{
    if(!keypad_initialized) return;

    char key = keypad_get_key();
    switch(keypad_state) {
        case KEY_STATE_IDLE:
            if (key != '\0') {
                // A key is pressed, move to debounce state
                keypad_state = KEY_STATE_DEBOUNCE;
                debounce_counter = 0;
            }
            break;

        case KEY_STATE_DEBOUNCE:
            if (key != '\0') {
                debounce_counter++;
                if (debounce_counter >= DEBOUNCE_TIME) {
                    // Key has been held long enough, it's a valid press
                    ring_buffer_write(&keypad_rb, (uint8_t)key);
                    keypad_state = KEY_STATE_PRESSED;
                }
            } else {
                // Key was released before debounce time, it was noise. Reset.
                keypad_state = KEY_STATE_IDLE;
            }
            break;

        case KEY_STATE_PRESSED:
            if (key == '\0') {
                // The key has been released, go back to idle.
                keypad_state = KEY_STATE_IDLE;
            }
            // Do nothing while key is held down (prevents auto-repeat)
            break;
    }
}

bool keypad_read_key(char *key)
{
    if (key == NULL) return false;
    return ring_buffer_read(&keypad_rb, (uint8_t *)key);
}
