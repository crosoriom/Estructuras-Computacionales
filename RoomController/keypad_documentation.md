# 4x4 Keypad Driver Documentation

This document provides detailed information about the 4x4 keypad driver for embedded systems. The driver is interrupt-driven, uses a ring buffer to store key presses, and includes software debouncing.

## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Dependencies](#dependencies)
- [File Structure](#file-structure)
- [API Reference](#api-reference)
  - [Constants](#constants)
  - [Data Structures](#data-structures)
  - [`keypad_init()`](#keypad_init)
  - [`keypad_scan_and_store()`](#keypad_scan_and_store)
- [Flow of Operation](#flow-of-operation)
  - [Initialization Flow](#initialization-flow)
  - [Key Press & Scan Flowchart](#key-press--scan-flowchart)
- [How to Use](#how-to-use)
  - [1. Configuration](#1-configuration)
  - [2. Initialization](#2-initialization)
  - [3. Interrupt Handler Setup](#3-interrupt-handler-setup)
  - [4. Reading Key Presses](#4-reading-key-presses)
  - [Complete Example](#complete-example)

## Overview

This driver provides a simple interface to handle a standard 4x4 matrix keypad. It is designed to be efficient by using external interrupts (EXTI) on the column pins. When a key is pressed, it pulls a column line low, triggering an interrupt. The interrupt service routine then calls the `keypad_scan_and_store()` function to determine which key was pressed and stores its character representation in a ring buffer.

## Features

-   **Interrupt-Driven:** Low CPU usage as it only runs when a key is pressed.
-   **Ring Buffer:** Key presses are queued in a ring buffer, ensuring no presses are lost if the main loop is busy.
-   **Software Debounce:** Includes simple delay-based debouncing for both key press and release.
-   **Configurable:** GPIO pins for rows and columns can be easily configured using the `keypad_config_t` struct.

## Dependencies

This driver relies on other low-level drivers for its operation. Ensure these are included and configured in your project.
-   `ringBuffer.h`: For the ring buffer implementation.
-   `gpio.h`: For General Purpose I/O pin control.
-   `exti.h`: For External Interrupt handling.
-   `systick.h`: For the `delay()` function used in debouncing.

## File Structure

-   `keypad.h`: Header file containing public function prototypes, data structures, and constants.
-   `keypad.c`: Implementation file containing the driver logic.

## API Reference

### Constants

-   `NUM_ROWS`: The number of rows on the keypad (defined as 4).
-   `NUM_COLS`: The number of columns on the keypad (defined as 4).
-   `KEYPAD_BUFFER_SIZE`: The size of the internal ring buffer for storing key presses.

### Data Structures

#### `keypad_config_t`
This structure holds the GPIO configuration for the keypad. An instance of this struct must be populated by the user and passed to the `keypad_init()` function.

```c
typedef struct {
    gpio_t *row_port[NUM_ROWS];
    uint8_t row_pin[NUM_ROWS];
    gpio_t *col_port[NUM_COLS];
    uint8_t col_pin[NUM_COLS];
} keypad_config_t;
```
-   `row_port`, `row_pin`: Arrays defining the GPIO port and pin for each row.
-   `col_port`, `col_pin`: Arrays defining the GPIO port and pin for each column.

---

### `keypad_init()`

Initializes the GPIO pins and EXTI for the 4x4 keypad.

**Signature:**
```c
void keypad_init(keypad_config_t *keypad_config);
```

**Parameters:**
-   `keypad_config`: A pointer to the `keypad_config_t` structure containing the GPIO pin details for the keypad.

**Description:**
This function performs the following actions:
1.  Configures the row pins as push-pull outputs.
2.  Configures the column pins as inputs with pull-up resistors and enables a falling-edge external interrupt on each one.
3.  Initializes the internal ring buffer to store key presses.
4.  Saves the user-provided configuration for use by the scan function.

---

### `keypad_scan_and_store()`

Scans the keypad to detect which button was pressed and stores it in the ring buffer.

**Signature:**
```c
void keypad_scan_and_store(PINx triggered_pin);
```

**Parameters:**
-   `triggered_pin`: The pin number that triggered the interrupt. This should be one of the configured column pins.

**Description:**
This function is designed to be called from within the GPIO External Interrupt (EXTI) handler. It should **not** be called from the main application loop. It performs the following steps:
1.  Implements a short delay for debouncing the initial key press.
2.  Disables all column interrupts to prevent re-triggering during the scan.
3.  Scans the rows by setting each one low sequentially.
4.  Checks the state of the triggered column pin to identify the pressed key's row.
5.  If a key is found, its character (from `keypad_map`) is pushed into the ring buffer.
6.  Sets all row pins high again.
7.  Waits for the key to be released (un-press debounce).
8.  Re-enables the column interrupts.

## Flow of Operation

### Initialization Flow
1.  User populates a `keypad_config_t` struct with the correct GPIO ports and pins.
2.  User calls `keypad_init()` with a pointer to the configuration struct.
3.  The driver configures row pins as outputs and column pins as inputs with falling-edge interrupts.
4.  The driver initializes the ring buffer.
5.  The system is now ready and waits for a key press.

### Key Press & Scan Flowchart

This flowchart illustrates the logic inside the `keypad_scan_and_store()` function, which is triggered by an external interrupt.

```mermaid
graph TD
    A[Key Press pulls a Column Pin LOW] --> B{EXTI Interrupt Triggered};
    B --> C[ISR calls keypad_scan_and_store(pin)];
    C --> D[Press Debounce: delay(1ms)];
    D --> E{Is Triggered Pin still LOW?};
    E -- No --> F[False Trigger: Clear EXTI & Return];
    E -- Yes --> G[Identify Triggered Column & Disable ALL Column EXTIs];
    
    subgraph Scan Routine
        G --> H[Start Row Scan Loop: row = 0];
        H --> I[Set current Row Pin LOW];
        I --> J[Settle Delay: delay(50µs)];
        J --> K{Is Triggered Column Pin now LOW?};
        K -- Yes --> L[Key Found! Get Char from keypad_map];
        L --> M[Write Character to Ring Buffer];
        M --> N[Break Scan Loop];
        K -- No --> O[Set current Row Pin HIGH];
        O --> P{More rows to scan?};
        P -- Yes --> H;
        P -- No --> Q[End Scan Loop];
    end
    
    N --> Q;
    Q --> R[Set All Row Pins HIGH];
    R --> S[Un-press Debounce: Wait for Pin to go HIGH];
    S --> T[Settle Delay: delay(50ms)];
    T --> U[Re-enable ALL Column EXTIs];
    U --> V[Return from function];
```

## How to Use

### 1. Configuration

Define a `keypad_config_t` variable and populate it with the GPIOs connected to your keypad.

```c
// Assuming keypad is connected to GPIOA and GPIOB
#include "keypad.h"
#include "gpio.h" // For GPIOA, GPIOB, etc.

keypad_config_t my_keypad = {
    .row_port = {GPIOA, GPIOA, GPIOA, GPIOA},
    .row_pin  = {PIN0, PIN1, PIN2, PIN3},
    .col_port = {GPIOB, GPIOB, GPIOB, GPIOB},
    .col_pin  = {PIN4, PIN5, PIN6, PIN7}
};
```

### 2. Initialization

In your `main()` function, call `keypad_init()` once to set up the hardware.

```c
int main(void) {
    // ... other system initializations
    
    keypad_init(&my_keypad);
    
    // ... main loop
}
```

### 3. Interrupt Handler Setup

You must provide an EXTI interrupt handler. This handler will be responsible for clearing the interrupt flag and calling `keypad_scan_and_store()`. The example below is for STM32, where multiple pins can share an interrupt line.

```c
// This is a hypothetical EXTI handler for pins 4-7
void EXTI4_15_IRQHandler(void) {
    if (exti_get_triggered(PIN4)) {
        exti_clear(PIN4);
        keypad_scan_and_store(PIN4);
    }
    if (exti_get_triggered(PIN5)) {
        exti_clear(PIN5);
        keypad_scan_and_store(PIN5);
    }
    if (exti_get_triggered(PIN6)) {
        exti_clear(PIN6);
        keypad_scan_and_store(PIN6);
    }
    if (exti_get_triggered(PIN7)) {
        exti_clear(PIN7);
        keypad_scan_and_store(PIN7);
    }
}
```

### 4. Reading Key Presses

In your main application loop, check if there is data in the ring buffer and read it. You need a way to access the keypad's ring buffer, for example, by making `keypad_rb` non-static or by adding a getter function like `keypad_get_char()`.

Assuming a getter function `keypad_get_char()` exists that wraps `ring_buffer_read()`:

```c
// In keypad.h
char keypad_get_char(void);

// In keypad.c
char keypad_get_char(void) {
    char key = 0;
    ring_buffer_read(&keypad_rb, &key);
    return key;
}

// In main.c
int main(void) {
    // ... initialization ...
    keypad_init(&my_keypad);

    while(1) {
        if (keypad_has_data()) { // Assuming a function to check buffer status
            char pressed_key = keypad_get_char();
            // Do something with the key, e.g., print to UART
            printf("Key pressed: %c\n", pressed_key);
        }
    }
}
```

### Complete Example

```c
#include "keypad.h"
#include "gpio.h"
#include "exti.h"
#include "ringBuffer.h" // For ring buffer access

// Expose the ring buffer for the main application to use
extern ring_buffer_t keypad_rb;

// 1. Define keypad GPIO configuration
keypad_config_t my_keypad = {
    .row_port = {GPIOA, GPIOA, GPIOA, GPIOA},
    .row_pin  = {PIN0, PIN1, PIN2, PIN3},
    .col_port = {GPIOB, GPIOB, GPIOB, GPIOB},
    .col_pin  = {PIN4, PIN5, PIN6, PIN7}
};

// 3. Define the interrupt handler
void EXTI4_15_IRQHandler(void) {
    // A more generic handler:
    for (int i = 0; i < NUM_COLS; i++) {
        PINx current_pin = my_keypad.col_pin[i];
        if (exti_get_triggered(current_pin)) {
            exti_clear(current_pin);
            keypad_scan_and_store(current_pin);
            break; // Assume only one key press at a time
        }
    }
}

int main(void) {
    // System initializations (Clocks, SysTick, etc.)
    // ...

    // 2. Initialize the keypad driver
    keypad_init(&my_keypad);
    
    // Enable the corresponding EXTI IRQ in the NVIC
    // NVIC_EnableIRQ(EXTI4_15_IRQn);

    uint8_t key_pressed;

    while (1) {
        // 4. Read from the ring buffer in the main loop
        if (ring_buffer_read(&keypad_rb, &key_pressed) == 0) { // 0 means success
            // A key was read from the buffer
            // Process the key_pressed variable
        }
    }
}
```
