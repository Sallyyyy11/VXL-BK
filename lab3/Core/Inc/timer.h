/*
 * timer.h
 *
 *  Created on: Oct 10, 2025
 *      Author: chip
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

int timer0_counter;
int timer0_flag;
int TIMER_CYCLE;

void setTimer0(int duration);
void timer_run();

#endif /* INC_TIMER_H_ */
