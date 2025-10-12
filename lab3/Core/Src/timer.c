#include "main.h"
#include "reading.h"
#include "processing.h"
#include "display.h"

int timer0_counter = 0;
int timer0_flag = 0;
int TIMER_CYCLE = 10;
int counter = 25;

void setTimer0(int duration){
  timer0_counter = duration / TIMER_CYCLE ;
  timer0_flag = 0;
}

void timer_run(){
  if(timer0_counter > 0){
    timer0_counter--;
    if(timer0_counter == 0) timer0_flag = 1;
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM2)
  {
    button_reading();
    timer_run();
	counter--;
    if(counter <= 0){
        counter = 25;
        update7SEG(index_led);
        index_led++;
        if(index_led>= MAX_LED) index_led = 0;
    }
    fsm_for_processing();
  }
}
