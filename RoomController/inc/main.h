#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>
#include "roomcontrol.h"
#include "systick.h"
#include "gpio.h"
#include "exti.h"
#include "keyPad/inc/keypad.h"

uint32_t actualTick = 0;

#define KEYPAD_ROW_PORTS GPIOA, GPIOB, GPIOB, GPIOB
#define KEYPAD_ROW_PINS PIN_10, PIN_3, PIN_5, PIN_4
#define KEYPAD_COL_PORTS GPIOB, GPIOA, GPIOA, GPIOC
#define KEYPAD_COL_PINS PIN_10, PIN_8, PIN_9, PIN_7

#endif
