#include "hal.h"

int n_frames = 14;
int n_led = 8;
int n_sw = 4;

int i = 0;

GPIO_PinState frames[14][8] = {
    {GPIO_PIN_SET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, 
        GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET},
    {GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_RESET, GPIO_PIN_RESET, 
        GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET},
    {GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_RESET, 
        GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET},
    {GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET, 
        GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET},
    {GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET, 
        GPIO_PIN_SET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET},
    {GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET, 
        GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_RESET, GPIO_PIN_RESET},
    {GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET, 
        GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_RESET},
    {GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET, 
        GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET},
    {GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET, 
        GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_RESET},
    {GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET, 
        GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_RESET, GPIO_PIN_RESET},
    {GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET, 
        GPIO_PIN_SET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET},
    {GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET, 
        GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET},
    {GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_RESET, 
        GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET},
    {GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_RESET, GPIO_PIN_RESET, 
        GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET}
};

int led_num[] = {GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, 
                GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_11, GPIO_PIN_12};

int sw_num[] = {GPIO_PIN_4, GPIO_PIN_8, GPIO_PIN_10, GPIO_PIN_12};

void set_led_by_frame_num(int num) {
    for (int j = 0; j < n_led; j++) {
        HAL_GPIO_WritePin(GPIOD, led_num[j], frames[i][j]);
    }
}

int pow2(int a) {
    int res = 1;
    for (int j = 0; j < a; j++){
        res *= 2;
    }
    return res;
}

int get_num_from_sw() {
    int num = 0;
    for (int j = 0; j < n_sw; j++) {
        if (HAL_GPIO_ReadPin(GPIOE, sw_num[j]) == GPIO_PIN_SET) {
            num = num | pow2(n_sw - j - 1);
        }
    }
    return num;
}

void TIM6_IRQ_Handler() {
    set_led_by_frame_num(i);
    i++;
    if (i > 13) {
        i = 0;
    }
    WRITE_REG(TIM6_ARR, 500 + 250 * get_num_from_sw());
}

int umain() {
    i = 0;

    registerTIM6_IRQHandler(TIM6_IRQ_Handler);

    WRITE_REG(TIM6_ARR, 500 + 250 * get_num_from_sw());
    WRITE_REG(TIM6_DIER, TIM_DIER_UIE);
    WRITE_REG(TIM6_PSC, 0);

    __enable_irq();

    WRITE_REG(TIM6_CR1, TIM_CR1_CEN);

    return 0;
}
