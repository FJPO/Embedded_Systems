#include "hal.h"
#include <math.h>

int frames = 8;
int indexes_len = 3;
int pointers[] = {GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5,
                  GPIO_PIN_6, GPIO_PIN_8, GPIO_PIN_9,
                  GPIO_PIN_11, GPIO_PIN_12};
int indexes[] = {0, 1, frames-1};

int getDelay(){
    const int T = 50;
    int result = 0;
    int sw_switches[4] = {GPIO_PIN_4, GPIO_PIN_8, GPIO_PIN_10, GPIO_PIN_12};
    for(int i = 0; i<4; i++)
        if (HAL_GPIO_ReadPin(GPIOE, sw_switches[i]) == GPIO_PIN_SET) result += pow(2, i);

    return 500 + result*T;
}

void TIM6_IRQ_Handler()
{
    for(int j = 0; j<indexes_len; j++)
        HAL_GPIO_WritePin(GPIOD, pointers[indexes[j]], GPIO_PIN_SET);
    WRITE_REG(TIM6_ARR, getDelay());
    WRITE_REG(TIM7_ARR, getDelay());

}
void TIM7_IRQ_Handler()
{
    for(int j = 0; j<indexes_len; j++)
        HAL_GPIO_WritePin(GPIOD, pointers[indexes[j]], GPIO_PIN_RESET);
    indexes[0] += 1;
    indexes[1] += 1;
    indexes[2] -= 1;
    if(indexes[0] == frames) {
        indexes[0] = 0;
        indexes[1] = 1;
        indexes[2] = frames-1;
    }
}

int umain()
{
    registerTIM6_IRQHandler(TIM6_IRQ_Handler);
    registerTIM7_IRQHandler(TIM7_IRQ_Handler);
    __enable_irq();
    WRITE_REG(TIM6_ARR, getDelay());
    WRITE_REG(TIM6_DIER, TIM_DIER_UIE);
    WRITE_REG(TIM6_PSC, 0);

    WRITE_REG(TIM7_ARR, getDelay());
    WRITE_REG(TIM7_DIER, TIM_DIER_UIE);
    WRITE_REG(TIM7_PSC, 1);

    WRITE_REG(TIM6_CR1, TIM_CR1_CEN);
    WRITE_REG(TIM7_CR1, TIM_CR1_CEN);
    return 0;
}






//int frames = 8;
//int indexes_len = 3;
//int pointers[8] = {GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_11, GPIO_PIN_12};
//int indexes[3] = {0, 1, frames-1};
//
//int getDelay(){
//    const int T = 50;
//    int result = 0;
//    int sw_switches[4] = {GPIO_PIN_4, GPIO_PIN_8, GPIO_PIN_10, GPIO_PIN_12};
//    for(int i = 0; i<4; i++)
//        if (HAL_GPIO_ReadPin(GPIOE, sw_switches[i]) == GPIO_PIN_SET) result += pow(2, i);
//
//    return 500 + result*T;
//}
//
//void led_on(){
//    for(int j = 0; j<indexes_len; j++)
//        HAL_GPIO_WritePin(GPIOD, pointers[indexes[j]], GPIO_PIN_SET);
//}
//
//void led_off(){
//    for(int j = 0; j<indexes_len; j++)
//        HAL_GPIO_WritePin(GPIOD, pointers[indexes[j]], GPIO_PIN_RESET);
//    indexes[0] += 1;
//    indexes[1] += 1;
//    indexes[2] -= 1;
//    if(indexes[0] == frames) {
//        indexes[0] = 0;
//        indexes[1] = 1;
//        indexes[2] = frames-1;
//    }
//}
//
//
//int umain(){
//
//    registerTIM6_IRQHandler(led_on);
//    registerTIM6_IRQHandler(led_off);
//
//    __enable_irq();
//    WRITE_REG(TIM6_ARR, getDelay());
//    WRITE_REG(TIM6_DIER, TIM_DIER_UIE);
//    WRITE_REG(TIM6_PSC, 0);
//
//    WRITE_REG(TIM7_ARR, getDelay());
//    WRITE_REG(TIM7_DIER, TIM_DIER_UIE);
//    WRITE_REG(TIM7_PSC, 1);
//
//    WRITE_REG(TIM6_CR1, TIM_CR1_CEN);
//    WRITE_REG(TIM7_CR1, TIM_CR1_CEN);
//
//
//
//    return 0;
//}
