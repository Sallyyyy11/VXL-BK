#ifndef INC_READING_H_
#define INC_READING_H_
#include "main.h"

#define N0_OF_BUTTONS 3
#define DURATION_FOR_AUTO_INCREASING 100
#define BUTTON_IS_PRESSED GPIO_PIN_RESET
#define BUTTON_IS_RELEASED GPIO_PIN_SET


void button_reading(void);

unsigned char is_button_pressed(uint8_t index);
unsigned char is_button_pressed_1s(uint8_t index);
#endif /* INC_READING_H_ */
