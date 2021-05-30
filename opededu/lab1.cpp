#include "hal.h"



void check_button(){
    if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15) == GPIO_PIN_RESET){
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
        while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15) == GPIO_PIN_RESET) HAL_Delay(100);
        while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15) != GPIO_PIN_RESET) HAL_Delay(100);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);

    }
}

bool sw_code_correct(GPIO_PinState* states){
    uint sw_switches[4] = {GPIO_PIN_4, GPIO_PIN_8, GPIO_PIN_10, GPIO_PIN_12};
    for(int i = 0; i<4; i++)
        states[i] = HAL_GPIO_ReadPin(GPIOE, sw_switches[i]);
    return states[0] == GPIO_PIN_RESET && states[1] == GPIO_PIN_RESET &&
           states[2] == GPIO_PIN_SET && states[3] == GPIO_PIN_RESET;
}

void animation_mode(){
    int delay = 500;
    int size = 6;
    uint left[6] = {GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_5, GPIO_PIN_4};
    uint right[6] = {GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_11, GPIO_PIN_12, GPIO_PIN_11, GPIO_PIN_9};

    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
    for(;;) {
        for(int i = 0; i < size; i++) {
            HAL_GPIO_WritePin(GPIOD, left[i], GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD, right[i], GPIO_PIN_SET);
            HAL_Delay(delay);
            check_button();
            HAL_GPIO_WritePin(GPIOD, left[i], GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, right[i], GPIO_PIN_RESET);
            HAL_Delay(delay);
            if(!sw_code_correct(new GPIO_PinState[0])) return;
        }

    }
}

void switch_mode(){
    uint leds[4] = {GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6};
    uint sw_switches[4] = {GPIO_PIN_4, GPIO_PIN_8, GPIO_PIN_10, GPIO_PIN_12};
    GPIO_PinState states[4];
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
    for(;;){
        for(int i = 0; i<4; i++){
            states[i] = HAL_GPIO_ReadPin(GPIOE, sw_switches[i]);
            HAL_GPIO_WritePin(GPIOD, leds[i], states[i]);
        }
        if(sw_code_correct(states)) {
            for(int i = 0; i<4; i++){
                HAL_GPIO_WritePin(GPIOD, leds[i], GPIO_PIN_RESET);
            }
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
            HAL_Delay(100);
            return;
        }
    }
}

int umain() {
    for(;;) {
        switch_mode();
        animation_mode();
    }
    return 0;
}

