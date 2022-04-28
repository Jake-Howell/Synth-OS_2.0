#ifndef _HW_TIMER_H_
#define _HW_TIMER_H_

#include "mbed.h"


void TIM_IRQHandler();
void init_HWTimer_ISR(int psc, int arr);


#endif

