/*
 * display.c
 *
 *  Created on: Oct 10, 2025
 *      Author: chip
 */
#include "display.h"
#include "main.h"
#include "reading.h"
#include "processing.h"

const int MAX_LED = 4;
int index_led = 0;
int led_buffer [4];

void TurnOff(){
	 // Turn OFF LEDs first
	 HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, GPIO_PIN_SET);
	 HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, GPIO_PIN_SET);
	 HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, GPIO_PIN_SET);
	 HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, GPIO_PIN_SET);

}

void display7SEG(int num){
	  uint8_t segCode[10] = {
	      0b0000001, // 0
	      0b1001111, // 1
	      0b0010010, // 2
	      0b0000110, // 3
	      0b1001100, // 4
	      0b0100100, // 5
	      0b0100000, // 6
	      0b0001111, // 7
	      0b0000000, // 8
	      0b0000100  // 9
	  };

	  uint8_t code = segCode[num];

	  HAL_GPIO_WritePin(GPIOB, A_Pin, (code >> 6) & 0x01); //A
	  HAL_GPIO_WritePin(GPIOB, B_Pin, (code >> 5) & 0x01); //B
	  HAL_GPIO_WritePin(GPIOB, C_Pin, (code >> 4) & 0x01); //C
	  HAL_GPIO_WritePin(GPIOB, D_Pin, (code >> 3) & 0x01); //D
	  HAL_GPIO_WritePin(GPIOB, E_Pin, (code >> 2) & 0x01); //E
	  HAL_GPIO_WritePin(GPIOB, F_Pin, (code >> 1) & 0x01); //F
	  HAL_GPIO_WritePin(GPIOB, G_Pin, (code >> 0) & 0x01); //G
  }

void update7SEG (int index){
TurnOff();
switch (index){
	case 0:
		display7SEG(led_buffer[0]);
		HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, GPIO_PIN_RESET);
		break;
	case 1:
		display7SEG(led_buffer[1]);
		HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, GPIO_PIN_RESET);
		break;
	case 2:
		display7SEG(led_buffer[2]);
		HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, GPIO_PIN_RESET);
		break;
	case 3:
		display7SEG(led_buffer[3]);
		HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, GPIO_PIN_RESET);
		break ;
	default:
		break;
	 }
}

void setTrafficDisplay(uint8_t road1_time, uint8_t road2_time) {
    // Split numbers into tens and ones
    led_buffer[0] = road1_time / 10;
    led_buffer[1] = road1_time % 10;
    led_buffer[2] = road2_time / 10;
    led_buffer[3] = road2_time % 10;
}

