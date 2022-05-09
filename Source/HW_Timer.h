#ifndef _HW_TIMER_H_
#define _HW_TIMER_H_
//*
#include "mbed.h"
#define HW_TIMER_MODULE TIM7
#define HW_TIMER_IRQn TIM7_IRQn
#define HW_TIMER_APB_EN RCC_APB1ENR_TIM7EN
#define HW_TIMER_APB APB1ENR
//*/
/*
#define HW_TIMER_MODULE TIM17
#define HW_TIMER_IRQn TIM17_IRQn
#define HW_TIMER_APB_EN RCC_APB2ENR_TIM17EN
#define HW_TIMER_APB APB2ENR
//*/
void TIM_IRQHandler();
void init_HWTimer_ISR(int frq);


#endif

