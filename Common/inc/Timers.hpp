#ifndef __TIMERS_H_
#define __TIMERS_H_

#include "stm32h7xx_ll_tim.h"
#include "main.h"

void Init_Timer1(unsigned int period_us);

void Init_Timer2(unsigned int PSC_val, unsigned int ARR_val, bool ISR_Enable);

void Init_Timer3(unsigned int PSC_val, unsigned int ARR_val, bool ISR_Enable);




//polling delays
unsigned int TIM2_Elapsed_ms(unsigned int startTime);
void Wait3_us(int delay_us);
void Wait3_ms(int delay_ms);
void Wait3_s(int delay_s);

#endif
