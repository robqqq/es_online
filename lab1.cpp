#include "hal.h"

void LED_set_by_num(int num, uint* LED_pins) {
    for (int i = 0; i < 8; i++) {
        if ((num < 6 && i >= num && i <= num + 2) || (num == 6 && (i == 6 || i == 7 || i == 0)) || (num == 7 && (i == 7 || i == 0 || i == 1))) {
            HAL_GPIO_WritePin(GPIOD, LED_pins[i], GPIO_PIN_SET);
        } else {
            HAL_GPIO_WritePin(GPIOD, LED_pins[i], GPIO_PIN_RESET);
        }
    }
}

void LED_set_by_switches(uint* LED_pins, uint* switch_pins) {
    for (int i = 0; i < 4; i++) {
        HAL_GPIO_WritePin(GPIOD, LED_pins[i], HAL_GPIO_ReadPin(GPIOE, switch_pins[i]));
    }
}

enum Color {
    Green = 0, Yellow, Red
};

void CPU_LED_set_by_color(Color color) {
    switch (color)
    {
    case Green:
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
        break;
    
    case Yellow:
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
        break;

    case Red:
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
        break;

    default:
        break;
    }
}

void pause_animate() {
    CPU_LED_set_by_color(Red);
    HAL_Delay(500);
    while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15) == GPIO_PIN_SET){
        
    }
    CPU_LED_set_by_color(Green);
    HAL_Delay(500);
}

int umain() {
    int delay = 500;
    uint LED_pins[] = {GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_11, GPIO_PIN_12};
    uint switch_pins[] = {GPIO_PIN_4, GPIO_PIN_8, GPIO_PIN_10, GPIO_PIN_12};

    CPU_LED_set_by_color(Yellow);

    while (!(HAL_GPIO_ReadPin(GPIOE, switch_pins[0]) == GPIO_PIN_SET &&
            HAL_GPIO_ReadPin(GPIOE, switch_pins[1]) == GPIO_PIN_RESET &&
            HAL_GPIO_ReadPin(GPIOE, switch_pins[2]) == GPIO_PIN_SET &&
            HAL_GPIO_ReadPin(GPIOE, switch_pins[3]) == GPIO_PIN_RESET)) {
        LED_set_by_switches(LED_pins, switch_pins);
    }

    CPU_LED_set_by_color(Green);
    int animation_frame = 0;

    while (true){
        if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15) == GPIO_PIN_RESET) {
            pause_animate();
        }
        LED_set_by_num(animation_frame, LED_pins);
        animation_frame = (animation_frame + 1) % 8;
        HAL_Delay(delay);
    }

    return 0;
}