#include "hal.h"

int umain() {
    int delay = 1000;

    for (int i = 0; i < 8; i++) {
        HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_3);
        HAL_Delay(delay);
    }

    return 0;
}