#ifndef INC_DISPLAY_H_
#define INC_DISPLAY_H_

#include "main.h"

void display7SEG(int num);
void update7SEG (int index);
void setTrafficDisplay(uint8_t road1_time, uint8_t road2_time);

const int MAX_LED;
int index_led;
int led_buffer [4];

#endif /* INC_DISPLAY_H_ */
