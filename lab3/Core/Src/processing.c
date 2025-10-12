#include "display.h"
#include "reading.h"
#include "processing.h"
#include "timer.h"

int mode = 0;                 // bắt đầu ở mode 1

// ----- Normal mode timing -----
int green_time = 3;
int yellow_time = 2;
int red_time = 5;

// traffic state
int traffic_state = 0; // 0=G1_R2, 1=Y1_R2, 2=R1_G2, 3=R1_Y2
int counter1 = 0;      // countdown
int button_increase_flag = 0; // Flag to prevent multiple increments

enum ButtonState { BUTTON_RELEASED, BUTTON_PRESSED, BUTTON_PRESSED_MORE_THAN_1_SECOND };
enum ButtonState buttonState = BUTTON_RELEASED;

void fsm_for_processing(void)
{
    // --- Xử lý nút nhấn for mode change (button 0) ---
    switch (buttonState)
    {
        case BUTTON_RELEASED:
            if (is_button_pressed(0)) {
                buttonState = BUTTON_PRESSED;
                mode++;
                if (mode > 4) mode = 1; // quay vòng 1-4
                // Reset timer for new mode
                if (mode == 1) setTimer0(1000);
                else setTimer0(250); // For blink in modes 2-4
            }
            break;

        case BUTTON_PRESSED:
            if (!is_button_pressed(0)) {
                buttonState = BUTTON_RELEASED;
            } else if (is_button_pressed_1s(0)) {
                buttonState = BUTTON_PRESSED_MORE_THAN_1_SECOND;
            }
            break;

        case BUTTON_PRESSED_MORE_THAN_1_SECOND:
            if (!is_button_pressed(0)) {
                buttonState = BUTTON_RELEASED;
            }
            break;
    }

    // --- Mode logic ---
    switch (mode)
    {
    case 1:
        // Set LEDs and display based on state (every 10ms ok)
        switch (traffic_state) {
            case 0: // G1_R2
                HAL_GPIO_WritePin(GPIOA, L1_Pin | L2_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(GPIOA, L3_Pin, GPIO_PIN_SET);

                HAL_GPIO_WritePin(GPIOA, L4_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOA, L5_Pin | L6_Pin, GPIO_PIN_RESET);

                setTrafficDisplay(counter1, counter1 + yellow_time);
                break;

            case 1: // Y1_R2
                HAL_GPIO_WritePin(GPIOA, L1_Pin | L3_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(GPIOA, L2_Pin, GPIO_PIN_SET);

                HAL_GPIO_WritePin(GPIOA, L4_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOA, L5_Pin | L6_Pin, GPIO_PIN_RESET);

                setTrafficDisplay(counter1, counter1);
                break;

            case 2: // R1_G2
                HAL_GPIO_WritePin(GPIOA, L1_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOA, L2_Pin | L3_Pin, GPIO_PIN_RESET);

                HAL_GPIO_WritePin(GPIOA, L4_Pin | L5_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(GPIOA, L6_Pin, GPIO_PIN_SET);

                setTrafficDisplay(counter1 + yellow_time, counter1);
                break;

            case 3: // R1_Y2
                HAL_GPIO_WritePin(GPIOA, L1_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOA, L2_Pin | L3_Pin, GPIO_PIN_RESET);

                HAL_GPIO_WritePin(GPIOA, L4_Pin | L6_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(GPIOA, L5_Pin, GPIO_PIN_SET);

                setTrafficDisplay(counter1, counter1);
                break;
        }

        // Decrement and transition every 1s
        if (timer0_flag == 1) {
            counter1--;
            if (counter1 <= 0) {
                traffic_state = (traffic_state + 1) % 4;
                if (traffic_state == 0 || traffic_state == 2) counter1 = green_time;
                else counter1 = yellow_time;
            }
            timer0_flag = 0;
            setTimer0(1000);
        }
        break;

        case 2: // Modify Red Duration
                HAL_GPIO_WritePin(GPIOA, L2_Pin | L3_Pin | L5_Pin | L6_Pin, GPIO_PIN_RESET);

                if (timer0_flag == 1) {
                    HAL_GPIO_TogglePin(GPIOA, L1_Pin | L4_Pin); // Blink both red LEDs
                    setTimer0(250);
                }

                // Increase red_time with button 1 (Increase)
                if (is_button_pressed(1) && !button_increase_flag) {
                    button_increase_flag = 1;
                    red_time++;
                    // Constraint: Red time must be > Green time.
                    if (red_time <= green_time) red_time = green_time + 1;
                    if (red_time > 99) red_time = green_time + 1; // Wrap around if max is exceeded

                    // **LOGIC: Adjust yellow_time based on the new red_time**
                    yellow_time = red_time - green_time;
                }
                if (!is_button_pressed(1)) {
                    button_increase_flag = 0;
                }

                // Set value with button 2 (Set)
                if (is_button_pressed(2)) {
                    mode = 1; // Return to normal mode
                    traffic_state = 0;
                    counter1 = green_time;
                    setTimer0(1000);
                }

                setTrafficDisplay(red_time, 2); // Show current red_time and mode number
                break;

            case 3: // Modify Yellow Duration
                HAL_GPIO_WritePin(GPIOA, L1_Pin | L3_Pin | L4_Pin | L6_Pin, GPIO_PIN_RESET);

                if (timer0_flag == 1) {
                    HAL_GPIO_TogglePin(GPIOA, L2_Pin | L5_Pin); // Blink both yellow LEDs
                    setTimer0(250);
                }

                // Increase yellow_time with button 1
                if (is_button_pressed(1) && !button_increase_flag) {
                    button_increase_flag = 1;
                    yellow_time++;
                    if (yellow_time > 99) yellow_time = 1;

                    // **LOGIC: Adjust red_time based on the new yellow_time**
                    red_time = green_time + yellow_time;

                    // Constraint: If red_time exceeds max, revert the change
                    if (red_time > 99) {
                        yellow_time--;
                        red_time = green_time + yellow_time;
                    }
                }
                if (!is_button_pressed(1)) {
                    button_increase_flag = 0;
                }

                // Set value with button 2
                if (is_button_pressed(2)) {
                    mode = 1;
                    traffic_state = 0;
                    counter1 = green_time;
                    setTimer0(1000);
                }

                setTrafficDisplay(yellow_time, 3);
                break;

            case 4: // Modify Green Duration
                HAL_GPIO_WritePin(GPIOA, L1_Pin | L2_Pin | L4_Pin | L5_Pin, GPIO_PIN_RESET);

                if (timer0_flag == 1) {
                    HAL_GPIO_TogglePin(GPIOA, L3_Pin | L6_Pin); // Blink both green LEDs
                    setTimer0(250);
                }

                // Increase green_time with button 1
                if (is_button_pressed(1) && !button_increase_flag) {
                    button_increase_flag = 1;
                    green_time++;
                    if (green_time > 99) green_time = 1;

                    // **LOGIC: Adjust red_time based on the new green_time**
                    red_time = green_time + yellow_time;

                    // Constraint: If red_time exceeds max, revert the change
                    if (red_time > 99) {
                        green_time--;
                        red_time = green_time + yellow_time;
                    }
                }
                if (!is_button_pressed(1)) {
                    button_increase_flag = 0;
                }

                // Set value with button 2
                if (is_button_pressed(2)) {
                    mode = 1;
                    traffic_state = 0;
                    counter1 = green_time;
                    setTimer0(1000);
                }

                setTrafficDisplay(green_time, 4);
                break;
            }
        }
